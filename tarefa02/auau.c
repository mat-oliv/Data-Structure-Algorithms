#include <stdio.h>
#define MAX 100

typedef struct {

  char name[MAX];
  char first_name[MAX];
  char second_name[MAX];
  char initials[4];

} dog;

void find_initials(dog progen[], int index){
  progen[index].initials[0] = progen[index].name[0] + 32;
  progen[index].initials[1] = progen[index].first_name[0] + 32;
  progen[index].initials[2] = progen[index].second_name[0] + 32;
  progen[index].initials[3] = '\0';

}

int checkString(char str1[], char str2[]){
  for (int i = 0; str1[i] == str2[i]; i++)
  {
    if (str1[i] == '\0')
      return 1;
  }

  return 0;
}

int find_by_initials(char initials[4], dog progenitors[], int n){

  for (int i = 0; i < n; i++){
     if (checkString(initials, progenitors[i].initials)){
      return i;
     }
  }

  return -1;
}

void set_names(dog progen[], int index){
  int count = 0;
  
  for (int i=0, j = 0; progen[index].name[i] != '\0'; i++) {
    if(progen[index].name[i] == ' '){
      
      if (count == 1)
        progen[index].first_name[j + 1] = '\0';
      else if (count == 2)
        progen[index].second_name[j + 1] = '\0';
      count += 1;
      j = 0;
      continue;
    }

    if (count == 1){
      progen[index].first_name[j] = progen[index].name[i];
      j += 1;
    }else if (count == 2){
      progen[index].second_name[j] = progen[index].name[i];
      j += 1;
    }
  }
}

void puppy_names(char inp[], int index, dog progenitors[], dog puppies[], int n){
  int count = 0;
  char first_initials[4];
  char second_initials[4];

  for (int i = 0, j = 0; inp[i] != '\0'; i++){
    if (inp[i] == ' ') {
      count += 1;
      j = 0;
      continue;
    }

    if (count == 0){
      first_initials[j] = inp[i];
      j += 1;
    } else if(count == 1){
      second_initials[j] = inp[i];
      j += 1;
    } else if(count == 2){
      puppies[index].name[j] = inp[i];
      j += 1;
    }
  }

  first_initials[3] = '\0';
  second_initials[3] = '\0';

  int i = find_by_initials(first_initials, progenitors, n );
  int j = find_by_initials(second_initials, progenitors, n );


  for (int x = 0; progenitors[i].first_name[x] !=  '\0'; x++){
    puppies[index].first_name[x] = progenitors[i].first_name[x];
  }
  for (int x = 0; progenitors[j].second_name[x] !=  '\0'; x++){
    puppies[index].second_name[x] = progenitors[j].second_name[x];
  }

}





int main(){

  int n;
  scanf("%d", &n);

  dog progenitors[8080];
  int last_index;

  for (int i=0; i < n; i ++) {
    last_index = i;
    scanf(" %[^\n]", progenitors[i].name);
    set_names(progenitors, i);
    find_initials(progenitors, i);
    printf("%s\n", progenitors[i].name);
  }
  
  int m;
  scanf("%d", &m);

  dog puppies[8000];
  for (int i = 0; i < m; i++){
    char inp[MAX];
    scanf(" %[^\n]", inp);
    last_index += 1;
    puppy_names(inp, i, progenitors, puppies, 8080);

    progenitors[last_index] = puppies[i];
    set_names(progenitors, last_index);
    find_initials(progenitors, last_index);
    
    printf("%s", puppies[i].name);
    printf(" %s", puppies[i].first_name);
    printf(" %s\n", puppies[i].second_name);
  }

  return 0;
}