#include <stdio.h>
#include <string.h>

typedef float nota;
typedef struct _avaliacoes {

nota pl, p2, trab;
} avaliacoes;
typedef struct _aluno {
int ra;
avaliacoes notas;
nota media;
char nome [50];

} aluno;

typedef struct _disciplina {
int codigo, qntAlunos;
nota media;
} disciplina;

void misterio2(aluno fichas[], int qtde){
  int i, j;
  aluno troca;
  for (i = qtde; i > 0; i--){
    for (j = 0; j < i - 1; j++){
      if ((strcmp(fichas[j].nome, fichas[j+1].nome) < 0)){
        troca = fichas[j];
        fichas[j] = fichas[j + 1];
        fichas[j + 1] = troca;
      }
    }
  }
}

int main(){

  disciplina disciplina1;
  aluno aluno1, aluno2, aluno3, aluno4, aluno5;

  

  return 0;
}
