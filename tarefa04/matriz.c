#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {

  int **m;
  int n_dim, m_dim; // memória alocada
  int n_size, m_size; // Ocupação da matriz
} matrix;


matrix *init_matrix(int n_dim, int m_dim); // Aloca a Matriz
void *add_line(matrix *m, int *line); // Adiciona uma nova linha
void free_matrix(matrix* m); // Libera espaço da Matriz
void* realloc_n(matrix *m, double factor); // Realloc da primeira dimensao da matriz
void* realloc_m(matrix *m, double factor); // Realloc da Segunda dimensao da matriz
void print_matrix(matrix * m);
void* remove_line(matrix * m, int index); // REmove uma linha
void* add_column(matrix* m, int* column); // ADiciona uma coluna
void* remove_column(matrix *m, int index); // remove uma coluna
void free_matri(int **matri, int n_dim);



int main(){

  int n_dim, m_dim;
  scanf(" %d %d", &n_dim, &m_dim);

  matrix* m = init_matrix(n_dim, m_dim);
  
  m->m_size = m_dim;
  for(int i = 0; i < n_dim; i++){
    int * vec = malloc(m_dim * sizeof(int));
    for (int j = 0; j < m_dim; j++){
      scanf(" %d", &vec[j]);
    }
    add_line(m, vec);
    free(vec);
    }


  char* text = malloc(3 * sizeof(char));

  scanf(" %s", text);

  while (strcmp(text, "S")){
    if (!strcmp(text, "AP")){
      int* vect = malloc(m->m_size * sizeof(int));
      for (int i =0; i < m->m_size; i++){
        scanf("%d", &vect[i]);
      }
      add_line(m, vect);
      free(vect);
    } else if (!strcmp(text, "V")){
      print_matrix(m);
    } else if(!strcmp(text, "AH")){
      int* vec = malloc(m->n_size * sizeof(int));
      for (int i =0; i < m->n_size; i++){
        scanf(" %d", &vec[i]);
      }

      add_column(m, vec);

      free(vec);
    } else if(!strcmp(text, "RP")){
        int line_index;
        scanf(" %d", &line_index);

        remove_line(m, line_index);
    } else if(!strcmp(text, "RH")){
        int column_index;
        scanf(" %d", &column_index);

        remove_column(m, column_index);
    }

    scanf(" %s", text);
  }

  free_matrix(m);


  return 0;
}

void* add_column(matrix* m, int* column){
  
  if (m->m_size == m->m_dim){
    printf("Tamanho da matriz: %d x %d\n",(int)m->n_size, (int)(m->m_size + 1));
    m= realloc_m(m, 2);
  }

  for(int i = 0; i < m->n_size; i++){
    int value = column[i];
    int j;
    for (j = m->m_size - 1; j >= 0 && value < m->m[i][j]; j--){
      m->m[i][j + 1] = m->m[i][j];
    }

    m->m[i][j + 1] = value;
  }

  m->m_size += 1;

  return m;

}

void* remove_column(matrix* m, int index){

  for (int i = 0; i < m->n_size; i++){
    for (int j = index; j < m->m_size - 1; j++)
    { 
      m->m[i][j] = m->m[i][j + 1];
    }
  }
  m->m_size -= 1;

  if (m->m_size <= m->m_dim / 4){
    printf("Tamanho da matriz: %d x %d\n", (int)m->n_size, (int)(m->m_size));
    if (m->m_dim > 1)
      realloc_m(m, 0.5);
  }

  return m;
}

void* realloc_m(matrix *m, double factor){
  int **matri = m->m;
  m->m = malloc(m->n_dim * sizeof(int*));
  int old_m_dim = m->m_dim;
  m->m_dim *= factor;

  for(int i = 0; i < m->n_dim; i++){
    m->m[i] = malloc(m->m_dim * sizeof(int));
  }
  for (int i = 0; i < m->n_size; i++){
    for (int j = 0; j < m->m_size; j++){
      m->m[i][j] = matri[i][j];
    }
  }

  printf("Memória realocada: %d x %d -> %d x %d\n",(int) (m->n_dim), (int)(old_m_dim), (int)m->n_dim, (int)(m->m_dim));

  //  liberar memória aqui
      free_matri(matri, m->n_dim);


  return m;
}

void* remove_line(matrix *m, int index){
  // TODO dar free no cara a ser removido (index)
  for (int i = index; i < m->n_size -1; i++){
    for (int j = 0; j < m->m_size; j++){
    m->m[i][j] = m->m[i + 1][j];
    }
  }

  m->n_size -= 1;

  if (m->n_size <= m->n_dim/4){
    printf("Tamanho da matriz: %d x %d\n",(int) m->n_size, (int)(m->m_size));
    realloc_n(m, 0.5);
  } 

  return m;
}

matrix *init_matrix(int n_dim, int m_dim){

  matrix *m = malloc(sizeof(matrix));

  m->m = malloc(n_dim * sizeof(int*));

  m->m_size = 0;
  m->n_size = 0;

  for (int i = 0; i < n_dim; i++){
      m->m[i] = malloc(m_dim * sizeof(int));
    
  }

  m->n_dim = n_dim;
  m->m_dim = m_dim;

  return m;

}

void* add_line(matrix *m, int *line){
    
    if (m->n_size == m->n_dim){
      printf("Tamanho da matriz: %d x %d\n", (int)(m->n_size + 1),(int) (m->m_size));
      realloc_n(m, 2);
    }

    for (int i = 0; i < m->m_size; i++){
      m->m[m->n_size][i] = line[i];
    }
    
    m->n_size += 1;

  
    return m;
}

void free_matri(int** matri, int n_dim){
  for(int i = 0; i < n_dim; i++){
    free(matri[i]);
 }
 free(matri);
}

void free_matrix(matrix *m){
  free_matri(m->m, m->n_dim);
  free(m);
}

void* realloc_n(matrix *m, double factor){
  int **matri = m->m;
  int old_n_dim = m->n_dim;
  m->n_dim *= factor;
  m->m = malloc(m->n_dim * sizeof(int*));
 

  for(int i = 0; i < m->n_dim; i++){
    m->m[i] = malloc(m->m_dim * sizeof(int));
  }
  
  for (int i = 0; i < m->n_size; i++){
    for (int j = 0; j < m->m_size; j++){
      m->m[i][j] = matri[i][j];
    }
  }

  printf("Memória realocada: %d x %d -> %d x %d\n", (int)(old_n_dim),(int) (m->m_dim), (int)m->n_dim, (int)(m->m_dim));

  //  liberar memória aqui
  free_matri(matri, (int)(old_n_dim));


  return m;
}

void print_matrix(matrix * m){
printf("MATRIZ ATUAL:\n");
for (int i = 0; i < m->n_size; i ++){
    for (int j = 0; j < m->m_size; j ++){
      if  (j != m->m_size - 1)
        printf("%d ", m->m[i][j]);
      else 
        printf("%d", m->m[i][j]);

    }
    printf("\n");
  }
}

