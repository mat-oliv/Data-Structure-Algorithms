#include <stdio.h>
#include <stdlib.h>
#include "pgm.h"

void print_matrix(int **matrix, int l, int c);
int **init_matrix(int l, int m);
int main(){

  char *path;
  path = malloc(100 * sizeof(char));
  int l, q;
  int l2, c2;

  scanf(" %s", path);
  scanf(" %d %d", &l, &q);

  image *new_image = read_image(path);

  free(path);

  l2 = new_image->n - l + 1;
  c2 = new_image->m - l + 1;

  int **kern_matrix;
  kern_matrix = init_matrix(l, l);

  int **final_matrix = init_matrix(l2+1, c2+1);

  int delta_x = (l - 1) / 2;
  int delta_y = (l - 1) / 2;


  for (int i = 0; i < l; i++ ){
    for (int j = 0; j < l; j++){
      scanf("%d", &kern_matrix[i][j]);
    }
  } 

  
  int sum_value;
  for( int i = delta_x; i <= l2; i ++){
    for (int j = delta_y; j <= c2; j ++){

      sum_value = 0;
      for(int h = i - delta_x; h < i + delta_x + 1; h++){
        for (int k = j - delta_y; k < j + delta_y + 1; k++){
        
          sum_value += kern_matrix[h - (i - delta_x)][k - (j - delta_y)] * new_image->matrix[h][k];

        }
      }

      final_matrix[i - delta_x][j - delta_y] = (sum_value) / q;
      
      

    }
  }

  image_p final_image;
  final_image = create_image(l2, c2);

  final_image->matrix = final_matrix;


  for (int i =0; i < l; i++)
    free(kern_matrix[i]);
  free(kern_matrix);

  print_image(final_image);
  for(int i = 0; i < l; i++)
    free(final_matrix[i]);
  free(final_matrix);
  return 0;
}

int **init_matrix(int l, int m){
  int **ken_matrix = malloc(l * sizeof(int*));
  for (int i = 0; i < l; i++)
    ken_matrix[i] = malloc(m * sizeof(int));

  return ken_matrix;
}

void print_matrix(int **matrix, int l, int c){
  for (int i = 0; i < l; i ++){
    for (int j = 0; j < c; j++){
      if (j == c - 1)
        printf("%d", matrix[i][j]);
      else
        printf("%d ", matrix[i][j]);
    }
    printf("\n");
  }
}