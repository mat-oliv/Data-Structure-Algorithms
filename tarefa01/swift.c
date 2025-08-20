#include <stdio.h>
#define cur_term 0.000001


double sine(double x);
double cosi(double x);
double absolute(double x);

int main(){

  double x;

  scanf("%lf", &x);


  printf("%.6lf\n", sine(x));
  printf("%.6lf\n", cosi(x));

  return 0;
}

double sine(double x){
 
  double sin_value = 0;
  double term_value;
  int i = 1;
  int parity = 0;
  double pow_num;
  double fac_num;
  while (1){
    if ( i == 1){
      pow_num = x;
      fac_num = 1;
    } else {
      pow_num *= x * x;
      fac_num *= i * (i - 1);
    }
    term_value = pow_num / fac_num;


    if (absolute(term_value) < cur_term)   {
      return sin_value;
    } else{
      if ( (parity % 2) == 0 ){
         sin_value += term_value;
      } else {
         sin_value -= term_value;
      }
    }

    i += 2;
    parity += 1;
    
  }
}

double cosi(double x){
 
  double cos_value = 0;
  int i = 0;
  int parity = 0;
  double term_value;
  double pow_num;
  double fac_num;
  while (1){
  

    if (i == 0){
      pow_num = 1;
      fac_num = 1;
    } else {
      pow_num *= x * x;
      fac_num *= (i) * (i - 1);
    }
    term_value = pow_num / fac_num;

    if (absolute(term_value) < cur_term)   {
      return cos_value;
    } else{
      if ( (parity % 2) == 0  ){
        cos_value += term_value;
      } else {
        cos_value -= term_value;
      }
    }

    i += 2;
    parity += 1;
    
  }
}

double absolute(double x){
  double num = x;
  if (num < 0){
    num *= -1;
  }

  return num;
}