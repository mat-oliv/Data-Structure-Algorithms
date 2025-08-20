#include <stdio.h>
#define episolon 0.001

double sqr(int x);
int isPrime(int x);
int isPalindrome(int x);

int main(){

  int n;
  scanf("%d", &n);
  int num = 2;
  int index_num = 0;
  while (1) {
    if (isPrime(num) && isPalindrome(num)){
      index_num += 1;
      printf("%d\n", num);
    }


    if (index_num == n){
      break;
    }
    num += 1;
  }

  return 0;
}

int isPrime(int x){

  int sqr_num = (int) sqr(x);
  for (int i=2; i <= sqr_num; i ++){
    if ((x % i) == 0)
      return 0;
  }

  return 1;

}

int isPalindrome(int x){

  int cur_num = x;
  int reversed_number = 0;

  while (cur_num > 0) {
       reversed_number = reversed_number * 10 + (cur_num % 10);
       cur_num /= 10;
  }

  return reversed_number == x;

}

double sqr(int x)
{
  
    double guess = x / 2.0;
  
    while ((guess * guess - x) > episolon || (x - guess * guess) > episolon)
    {
      guess = (guess + x / guess) / 2.0;
    }

    return guess;
  
}