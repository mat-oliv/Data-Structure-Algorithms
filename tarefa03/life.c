#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

char **init_matrix(int l, int c);
void print_matrix(char **matrix, int l, int c);
char **get_nextgen(char **matrix, int l, int c);

int main(){


  int l, c, n;
  scanf("%d %d %d", &l, &c, &n);

  char **matrix;
  matrix = init_matrix(l, c);


  for (int i = 0; i < l; i ++){
    char *line_text = malloc(2 * c * sizeof(char));
    scanf(" %s", line_text);

    for (int j = 0; j < c; j++){
        matrix[i][j] = line_text[j];
    }

    free(line_text);

  }


  for(int i = 0; i < n + 1; i ++){
    printf("\n");
    printf("Geração %d\n", i);
    print_matrix(matrix, l, c);
    
    matrix = get_nextgen(matrix, l, c);

  
  }

  for (int i = 0; i < l; i ++){
    free(matrix[i]);
  }

  free(matrix);

  return 0;
}

char **get_nextgen(char **matrix, int l, int c){
  char **m = init_matrix(l, c);

  int alive;
  for (int i = 0; i < l; i++ ){
    for (int j = 0; j < c; j++){
      alive = 0;
      for(int x = i - 1; x <  i + 2; x++){
        for (int h = j - 1; h < j + 2; h++){
          if (x < 0 || x >= l || h < 0 || h >= c) continue;
          if (x == i && h == j) continue;
          if (matrix[x][h] == '#')
            alive += 1;
        }
      }

      if (matrix[i][j] == '#')
      {
        if (alive == 3 || alive == 2){
          m[i][j] = '#';
        } else 
          m[i][j] = '.';
      } else {
        if (alive == 3){
          m[i][j] = '#';
        } else {
          m[i][j] = '.';
        }

      }



    }

  }

  for (int i = 0; i < l; i ++){
    free(matrix[i]);
  }

  free(matrix);

  return m;


}

char **init_matrix(int l, int c){
  char **m = malloc(l * sizeof(char*));
  for (int i = 0; i < l; i++ ) {
    m[i] = malloc(c * sizeof(char));
  }

  return m;

}

void print_matrix(char **matrix, int l, int c){
  for (int i = 0; i < l; i ++){
    for (int j = 0; j < c; j++){
      if (j == c - 1)
        printf("%c", matrix[i][j]);
      else
        printf("%c ", matrix[i][j]);
    }
    printf("\n");
  }
}