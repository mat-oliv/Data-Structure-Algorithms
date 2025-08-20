#include <stdio.h>
#define episolon 0.00001
#define max  1000

int sqr(int n);
void clear_array(int list[max], int seq_size);
void copy_array(int list[max], int list2[max], int seq_size);

int main(){

  int num;

  scanf("%d", &num);

  int biggest_size = 0;
  int last_num = 0;
  int seq_size = 0;

  int big_seq[max];
  int cur_seq[max];

  while (num != 0) {

      int sqr_num = sqr(num);

      if (sqr_num != -1){
        printf("%d \n", sqr_num);
        } else {
        printf("Que ");
        for (int i = 0; i < num; i++ ){
          printf("x");
        }
        printf("ou da xuxa é esse?\n");
      }

      if (last_num <= num) {
        cur_seq[seq_size] = num;
        seq_size += 1;
      } else {
        clear_array(cur_seq, seq_size);
        seq_size = 1;
        cur_seq[0] = num;        
      }
  
      if (biggest_size < seq_size){
        biggest_size = seq_size;
        copy_array(big_seq, cur_seq, seq_size);
      }
  
      last_num = num;

      scanf("%d", &num);

    }

    if (biggest_size > 1){
    for (int i = 0; i < biggest_size; i ++){
      if (i < biggest_size - 1) {
        printf("%d ", big_seq[i]);
      } else{
        printf("%d\n", big_seq[i]);
      }
    }
  }
    return 0;
  }

void clear_array(int list[max], int seq_size){
  for (int i=0; i < seq_size; i++){
    list[i] = 0;
  }
}

void copy_array(int list[max], int list2[max], int seq_size){
  for (int i=0; i < seq_size; i++ ){
    list[i] = list2[i];
  }
}


int sqr(int n)
  {
  
   for (int r = 1; r * r <= n; r++) {
    if (r * r == n) {
      return r;
    }
   }

   return -1;
  }

