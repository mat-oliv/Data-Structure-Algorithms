#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {

  char name[31];
  char description[101];
  char ** tags;
  int numTags;

} file;

typedef struct {
  char  v[2001][31];
  int n;
} vect;


int hash(char *str, int MAX);
void addFile(file* hashi, vect * hashTags, file * fil);
int checkTag(char *tag, vect *hashTags);
int checkHash(char *name, file *hashi);
void removeFile(char *name, file* hashi, vect *hashTags);
void changeFile(char * curName, char *newName, char* newDesc, file *hashi, vect *hashTags);
void addTag(char *tag, char *name, file *hashi, vect *hashTags);
void removeTag(char *tag, char *name, file *hashi, vect *hashTags);
void searchTag(char *tag, vect* hashTags);
void searchFile(char * name, file* hashi);

int main(){

  vect * hashTags = malloc(30 * sizeof(vect));
  for (int i = 0; i < 30; i ++){
    strcpy(hashTags[i].v[0], " ");
    hashTags[i].n += 1;
  }
  file * hashi = malloc(2000 * sizeof(file));
  for (int i = 0; i < 2000; i ++){
    strcpy(hashi[i].name, " ");
  }



  char * text = malloc(50 * sizeof(char));

  while (1){

    scanf(" %s", text);

    if( strcmp(text, "ENCERRAR") == 0){
      break;
    } else if( strcmp(text, "INSERIR") == 0) {
      scanf(" %s", text);
      if(strcmp(text, "ARQUIVO") == 0){
        file * fil = malloc(sizeof(file));
        scanf(" %s", fil->name);
        scanf(" %[^\n]", fil->description);
        scanf(" %d", &fil->numTags);

        fil->tags = malloc(30 * sizeof(char*));
        for(int i = 0; i < fil->numTags; i++){
          fil->tags[i] = malloc(31 * sizeof(char));
          scanf(" %s", fil->tags[i]);
        }

        addFile(hashi, hashTags, fil);
        free(fil);
      } else if( strcmp(text, "TAG") == 0){
        char * name = malloc(31 * sizeof(char));
        char * tag = malloc(31 * sizeof(char));

        addTag(tag, name, hashi, hashTags);

        free(name);
        free(tag);
      }

      
    } else if(strcmp(text, "REMOVER") == 0){
      scanf(" %s", text);
      if(strcmp(text, "ARQUIVO") == 0){
        char * name = malloc(31 * sizeof(char));
        scanf(" %s", name);
        removeFile(name, hashi, hashTags);
        free(name);
      } else if( strcmp(text, " TAG")){
        char * name = malloc(31 * sizeof(char));
        char * tag = malloc(31 * sizeof(char));

        scanf(" %s", name);
        scanf(" %s", tag);
        removeTag(tag, name, hashi, hashTags);

        free(name);
        free(tag);
      }
    } else if(strcmp(text, "ALTERAR") == 0){
        scanf(" %s", text);
        if(strcmp(text, "ARQUIVO") == 0){
          char * curName = malloc(31 * sizeof(char));
          char * newName = malloc(31 * sizeof(char));
          char * newDesc = malloc(101 * sizeof(char));

          scanf(" %s", curName);
          scanf(" %s", newName);
          scanf(" %[^\n]", newDesc);
          changeFile(curName, newName, newDesc, hashi, hashTags);
        
          free(newName);
          free(curName);
          free(newDesc);
        }
    } else if(strcmp(text, "BUSCAR") == 0){
      scanf(" %s", text);
      if(strcmp(text, "ARQUIVO") == 0){
        char * name = malloc(31 * sizeof(char));
        scanf(" %s", name);
        searchFile(name, hashi);
        free(name);
      } else {
        char * tag = malloc(31 * sizeof(char));
        scanf(" %s", tag);
        searchTag(tag, hashTags);
        free(tag);
      }
    }

    

  }


  return 0;
}

int hash(char *key, int MAX) {
   int n = 0;
   for (int i = 0; i < strlen(key); i++)
   n = (256 * n + key[i]) % MAX;
   return n;
}

int checkTag(char *tag, vect *hashTags){
  int first_pos = hash(tag, 30);
  char * text; 
  while (1){
  text = hashTags[first_pos].v[0];
  if (strcmp(text, " ") == 0){
    // Espaço Vazio: Tag nova
    strcpy(hashTags[first_pos].v[0], tag);
    return first_pos;
  } else if (strcmp(text, tag) == 0){
    return first_pos;
  }
  first_pos += 1;
  if (first_pos >= 30){
    first_pos = 0;
  } 
  }
}

int checkHash(char *name, file *hashi){
  int first_pos = hash(name, 2000);
  while (1){
  if (strcmp(hashi[first_pos].name, " ") == 0){
    // Espaço Vazio: Arquivo novo
    return first_pos;
  } else if (strcmp(hashi[first_pos].name, name) == 0){
    return first_pos;
  }
  first_pos += 1;
  if (first_pos >= 2000){
    first_pos = 0;
  } 
  }
}



void addFile(file *hashi, vect *hashTags, file *fil){
  for (int i = 0; i < fil->numTags; i ++){
    int pos = checkTag(fil->tags[i], hashTags);
    strcpy(hashTags[pos].v[hashTags[pos].n], fil->name);
    hashTags[pos].n += 1;
  }

  int pos2 = checkHash(fil->name, hashi);

  hashi[pos2] = *fil;  
}

void removeFile(char *name, file* hashi, vect *hashTags){
  int pos = checkHash(name, hashi);
  strcpy(hashi[pos].name, " ");
  strcpy(hashi[pos].description, " ");
  for(int i = 0; i < hashi[pos].numTags; i++){
    int pos2 = checkTag(hashi[pos].tags[i], hashTags);
    int j;
    for (j = 0; j <  hashTags[pos2].n; j++){
      if (strcmp(name, hashTags[pos2].v[j]) == 0)
      break;
    }

    for (; j < hashTags[pos2].n - 1; j++){
      strcpy(hashTags[pos2].v[j], hashTags[pos2].v[j + 1]);
    }
    hashTags[pos2].n -= 1;
  }
}

void changeFile(char * curName, char *newName, char* newDesc, file *hashi, vect *hashTags){
  int pos = checkHash(curName, hashi);
  int newPos = checkHash(newName, hashi);
  hashi[newPos] = hashi[pos];
  strcpy(hashi[newPos].name, newName);
  strcpy(hashi[newPos].description, newDesc);

  strcpy(hashi[pos].name, " ");
  strcpy(hashi[pos].description, " ");
  for(int i = 0; i < hashi[newPos].numTags; i++){
    int pos2 = checkTag(hashi[newPos].tags[i], hashTags);
    int j;
    for (j = 0; j <  hashTags[pos2].n; j++){
      if (strcmp(curName, hashTags[pos2].v[j]) == 0){
        strcpy(hashTags[pos2].v[j], newName);
        break;
      }
      }
    }
  }

void addTag(char *tag, char *name, file *hashi, vect *hashTags){
  int pos = checkHash(name, hashi);
  strcpy(hashi[pos].tags[hashi[pos].numTags], tag);
  hashi[pos].numTags += 1;

  int pos2 = checkTag(tag, hashTags);
  strcpy(hashTags[pos2].v[hashTags[pos2].n], name);
  hashTags[pos2].n += 1;
}

void removeTag(char *tag, char *name, file *hashi, vect *hashTags){
  int pos = checkHash(name, hashi);
  int pos2;
  int i;
  for (i = 0; i < hashi[pos].numTags; i++){
    if (strcmp(hashi[pos].tags[i], tag) == 0){
      break;
    }
    pos2 = checkTag(hashi[pos].tags[i], hashTags);
    int j;
    for (j = 0; j <  hashTags[pos2].n; j++){
      if (strcmp(name, hashTags[pos2].v[j]) == 0){
        break;
      }
      }
    for (; j <  hashTags[pos2].n - 1; j++){
      strcpy(hashTags[pos2].v[j], hashTags[pos2].v[j + 1]);
      }

      hashTags[pos2].n -= 1;
    }
    
  
  for(; i < hashi[pos].numTags; i++){

    pos2 = checkTag(hashi[pos].tags[i], hashTags);
    int j;
    for (j = 0; j <  hashTags[pos2].n; j++){
      if (strcmp(name, hashTags[pos2].v[j]) == 0){
        break;
      }
      }
    for (; j <  hashTags[pos2].n - 1; j++){
      strcpy(hashTags[pos2].v[j], hashTags[pos2].v[j + 1]);
      }

    hashTags[pos2].n -= 1;

    strcpy(hashi[pos].tags[i], hashi[pos].tags[i + 1]);
    
  }

  hashi[pos].numTags -= 1;


}

void searchTag(char *tag, vect* hashTags){
printf("Busca por tag: %s\n", tag);
int pos = checkTag(tag, hashTags);

if(hashTags[pos].n == 1){
  printf("0 resultados encontrados.\n");

} else {
  for(int i = 1; i < hashTags[pos].n; i++){
    printf("%s\n", hashTags[pos].v[i]);
  }
}

printf("----------\n");

}
void searchFile(char * name, file* hashi){
  printf("Acessando arquivo: %s\n", name);
  int pos = checkHash(name, hashi);
  if(strcmp(hashi[pos].name, " ") == 0){
    printf("Arquivo %s não existe.\n", name);
  } else {
  printf("Descrição: %s\n", hashi[pos].description);
  printf("Tags:");
  for(int i = 0; i < hashi[pos].numTags; i++){
    printf(" %s", hashi[pos].tags[i]);
  }
  printf("\n");
}

  printf("----------\n");
}

// Possíveis erros:
// Ter que remover tags se a quantidade de item com aquela tag for 0;
  


