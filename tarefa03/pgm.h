/*
** Header para interagir com a leitura e escrita de pgm.
*/

// Define a struct Image.
typedef struct Image {
  int n, m; // matrix size, n lines, m columns
  int **matrix;
} image;

// Renomeia `image *` para `image_p`
typedef image *image_p;

// Cria uma nova imagem, nas dimensões especificadas.
image_p create_image(int n, int m);

// Lê (e cria) uma imagem a partir do arquivo.
image_p read_image(char *filepath);

// Imprime uma imagem para o stdout.
void print_image(image_p img);

// Libera a memória de uma imagem.
void free_image(image_p img);
