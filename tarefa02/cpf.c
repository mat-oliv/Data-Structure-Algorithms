#include "stdio.h"
#include "stdlib.h"
#define MAX 100
#define MAX2 5000

typedef struct {

  char name[MAX];
  char birthday[MAX];
  char birth_place[MAX];
  char cpf[MAX];
} person;

int checkString(char str1[], char str2[]){
  for (int i = 0; str1[i] == str2[i]; i++)
  {
    if (str1[i] == '\0')
      return 1;
  }

  return 0;
}

int find_person(char cpf[], int n, person person_list[]){
  for (int i=0; i < n; i ++) {
    if (checkString(cpf, person_list[i].cpf)) {
        return i;
   }
  }

  return -1;
}

void print_data(person person1){
 printf("Nome: %s\n", person1.name);
 printf("Nascimento: %s\n", person1.birthday); 
 printf("Local: %s\n", person1.birth_place);
 char formated_cpf[15];
 formated_cpf[3] = '.';
 formated_cpf[7] = '.';
 formated_cpf[11] = '-';
 int i = 0;
 int j = 0;
 while (1){
  if (j == 3 || j == 7 || j == 11) 
  {
    j += 1;
  } else {
  formated_cpf[j] = person1.cpf[i];
  i += 1;
  j += 1;
  }

  if (i == 11){
    break;
  }

 }

 printf("CPF: %s", formated_cpf);
}

int main(){

  int n;
  scanf("%d", &n);

  person person_list[MAX2];

  for (int i = 0; i < n; i++ ){
    scanf(" %[^\n]", person_list[i].name);
    scanf(" %[^\n]", person_list[i].birthday);
    scanf(" %[^\n]", person_list[i].birth_place);
    scanf(" %[^\n]", person_list[i].cpf);
  }

  char person_cpf[MAX];
  scanf(" %s", person_cpf);


  int index_person = find_person(person_cpf, n, person_list);
  if (index_person == -1){
    printf("CPF nao encontrado.");
  } else {
  person person_found = person_list[index_person];

  print_data(person_found);
  }
}