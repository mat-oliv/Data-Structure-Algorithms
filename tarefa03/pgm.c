#include "pgm.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * @brief Cria uma nova imagem vazia com as dimensões especificadas.
 * * Aloca dinamicamente a memória para a estrutura da imagem e sua matriz de
 * pixels. Em caso de falha na alocação, a função libera qualquer memória já
 * alocada e retorna NULL.
 *
 * @param n O número de linhas da imagem.
 * @param m O número de colunas da imagem.
 * @return Um ponteiro para a nova imagem (image_p) ou NULL se a alocação
 * falhar.
 */
image_p create_image(int n, int m) {
  // Aloca memória para a estrutura da imagem
  image_p img = (image_p)malloc(sizeof(image));
  if (img == NULL) {
    fprintf(stderr,
            "Erro: Falha ao alocar memória para a estrutura da imagem.\n");
    return NULL;
  }
  img->n = n;
  img->m = m;

  // Aloca memória para as linhas da matriz (ponteiros para inteiros)
  img->matrix = (int **)malloc(n * sizeof(int *));
  if (img->matrix == NULL) {
    fprintf(stderr,
            "Erro: Falha ao alocar memória para as linhas da matriz.\n");
    free(img); // Libera a memória da estrutura
    return NULL;
  }

  // Aloca memória para as colunas de cada linha
  for (int i = 0; i < n; i++) {
    img->matrix[i] = (int *)malloc(m * sizeof(int));
    if (img->matrix[i] == NULL) {
      fprintf(stderr, "Erro: Falha ao alocar memória para a coluna %d.\n", i);
      // Em caso de falha, libera toda a memória alocada até agora
      for (int j = 0; j < i; j++) {
        free(img->matrix[j]);
      }
      free(img->matrix);
      free(img);
      return NULL;
    }
  }

  return img;
}

/**
 * @brief Consome linhas de comentário e espaços em branco de um fluxo de
 * arquivo.
 * * Comentários em PGM começam com '#' e vão até o final da linha.
 * Esta função irá pular qualquer número de linhas de comentário e linhas em
 * branco.
 *
 * @param file O ponteiro do arquivo de onde consumir os comentários.
 */
void consume_comments(FILE *file) {
  int ch;
  char buffer[1024]; // Buffer para descartar a linha do comentário

  // Loop para pular múltiplos comentários e linhas em branco
  while (1) {
    // Pula espaços em branco (incluindo newlines)
    while ((ch = fgetc(file)) != EOF && isspace(ch))
      ;

    if (ch == '#') {
      // Se for um comentário, lê e descarta o resto da linha
      fgets(buffer, sizeof(buffer), file);
    } else {
      // Se não for um comentário, devolve o caractere e sai do loop
      ungetc(ch, file);
      break;
    }
  }
}

/**
 * @brief Lê uma imagem no formato PGM (P2) a partir de um arquivo.
 * * Abre o arquivo, lê o cabeçalho PGM (lidando com comentários), cria uma
 * imagem com as dimensões lidas e preenche a matriz com os valores de pixel do
 * arquivo.
 *
 * @param filepath O caminho para o arquivo de imagem .pgm.
 * @return Um ponteiro para a imagem lida (image_p) ou NULL em caso de erro.
 */
image_p read_image(char *filepath) {
  FILE *file = fopen(filepath, "r");
  if (file == NULL) {
    fprintf(stderr, "Erro: Não foi possível abrir o arquivo '%s'.\n", filepath);
    return NULL;
  }

  char magic_number[3];
  int width, height, max_val;

  // Lê o número mágico (deve ser "P2")
  fscanf(file, "%2s", magic_number);
  if (strcmp(magic_number, "P2") != 0) {
    fprintf(stderr,
            "Erro: Formato de arquivo não suportado. Esperado PGM (P2).\n");
    fclose(file);
    return NULL;
  }

  consume_comments(file);

  // Lê as dimensões
  if (fscanf(file, "%d", &width) != 1) {
    fprintf(stderr, "Erro: Falha ao ler a largura da imagem.\n");
    fclose(file);
    return NULL;
  }
  consume_comments(file);
  if (fscanf(file, "%d", &height) != 1) {
    fprintf(stderr, "Erro: Falha ao ler a altura da imagem.\n");
    fclose(file);
    return NULL;
  }

  consume_comments(file);

  // Lê o valor máximo de cinza
  if (fscanf(file, "%d", &max_val) != 1) {
    fprintf(stderr, "Erro: Falha ao ler o valor máximo de cinza.\n");
    fclose(file);
    return NULL;
  }

  // Consome o newline final do cabeçalho antes dos dados do pixel
  fgetc(file);

  // Cria a imagem com as dimensões lidas
  image_p img = create_image(height, width);
  if (img == NULL) {
    fclose(file);
    return NULL;
  }

  // Lê os valores dos pixels e preenche a matriz
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      if (fscanf(file, "%d", &img->matrix[i][j]) != 1) {
        fprintf(stderr,
                "Erro: Dados de pixel inválidos ou arquivo corrompido.\n");
        free_image(img);
        fclose(file);
        return NULL;
      }
    }
  }

  fclose(file);
  return img;
}

/**
 * @brief Escreve a imagem na saída padrão (stdout) no formato PGM.
 *
 * @param img O ponteiro para a imagem a ser impressa.
 */
void print_image(image_p img) {
  if (img == NULL || img->matrix == NULL) {
    fprintf(stderr, "Erro: Tentativa de imprimir uma imagem nula.\n");
    return;
  }

  int max_value = 0;
  // Encontra o maior valor de cinza na imagem
  for (int i = 0; i < img->n; i++) {
    for (int j = 0; j < img->m; j++) {
      if (img->matrix[i][j] > max_value) {
        max_value = img->matrix[i][j];
      }
    }
  }

  if (max_value > 255) {
    fprintf(stderr,
            "Erro: Maior valor de cinza (%d) está fora do intervalo suportado "
            "[0, 255].\n", max_value);
    return;
  }

  // Imprime o cabeçalho PGM
  printf("P2\n");
  printf("%d %d\n", img->m, img->n);
  printf("%d\n", max_value);

  // Imprime a matriz de pixels
  for (int i = 0; i < img->n; i++) {
    for (int j = 0; j < img->m; j++) {
      printf("%d", img->matrix[i][j]);
      // Adiciona um espaço depois de cada pixel, exceto o último da linha
      if (j < img->m - 1) {
        printf(" ");
      }
    }
    printf("\n");
  }
}

/**
 * @brief Libera toda a memória alocada para uma imagem.
 *
 * @param img O ponteiro para a imagem a ser liberada.
 */
void free_image(image_p img) {
  if (img == NULL) {
    return;
  }

  if (img->matrix != NULL) {
    // Libera a memória de cada linha
    for (int i = 0; i < img->n; i++) {
      free(img->matrix[i]);
    }
    // Libera o ponteiro para as linhas
    free(img->matrix);
  }

  // Libera a estrutura da imagem
  free(img);
}
