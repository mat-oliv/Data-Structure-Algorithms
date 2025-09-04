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

  int **final_matrix = init_matrix(l2, c2);

  int delta_x = (l - 1) / 2;
  int delta_y = (l - 1) / 2;


  for (int i = 0; i < l; i++ ){
    for (int j = 0; j < l; j++){
      scanf("%d", &kern_matrix[i][j]);
    }
  } 
  
  for (int i = 0; i < l2; i++) {
    for (int j = 0; j < c2; j++){
      final_matrix[i][j] = 1;
    }
  }


  long int sum_value;
  for( int i = delta_x; i < new_image->n - delta_x; i ++){
    for (int j = delta_y; j < new_image->m - delta_y; j ++){

      sum_value = 0;
      for(int h = i - delta_x; h < i + delta_x + 1; h++){
        for (int k = j - delta_y; k < j + delta_y + 1; k++){ 
          if (h < 0 || h >= new_image->n || k < 0 || k >= new_image->m) continue;   
          sum_value += kern_matrix[h - (i - delta_x)][k - (j - delta_y)] * new_image->matrix[h][k];

        }
      }

      int final_value = sum_value / q;
     
      if (final_value > 255) final_value = 255;
      else if (final_value < 0) final_value = 0;

      final_matrix[i - delta_x][j - delta_y] = final_value;

    }
  }

  image_p final_image;
  final_image = create_image(l2, c2);

  free(final_image->matrix);
  final_image->matrix = final_matrix;


  print_image(final_image);

  for (int i =0; i < l; i++)
    free(kern_matrix[i]);
  free(kern_matrix);

  free_image(new_image);

  free_image(final_image);
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