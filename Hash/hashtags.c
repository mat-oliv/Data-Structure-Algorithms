#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {

  char name[31];
  char description[101];
  char  tags[30][31];
  int numTags;
  int j;

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
void searchTag(char *tag, vect* hashTags, file *hashi);
void searchFile(char * name, file* hashi);

int main(){

  vect * hashTags = malloc(30 * sizeof(vect));
  for (int i = 0; i < 30; i ++){
    hashTags[i].n = 1;
    strcpy(hashTags[i].v[0], " ");
  }
  file * hashi = malloc(2000 * sizeof(file));
  for (int i = 0; i < 2000; i ++){
    strcpy(hashi[i].name, " ");
  }


  int j = 0;
  char text[50];

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

        for(int i = 0; i < 30; i++){
          if( i < fil->numTags)
          scanf(" %s", fil->tags[i]);
        }

        j += 1;
        fil->j = j;

        addFile(hashi, hashTags, fil);
        free(fil);
      } else if( strcmp(text, "TAG") == 0){
        char * name = malloc(31 * sizeof(char));
        char * tag = malloc(31 * sizeof(char));

        scanf(" %s", name);
        scanf(" %s", tag);
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
        searchTag(tag, hashTags, hashi);
        free(tag);
      }
    }

    

  }

  free(hashTags);
  free(hashi);


  return 0;
}

int hash(char *key, int MAX) {
   int n = 0;
   for (int i = 0; i < strlen(key); i++)
   n = (256 * n + (unsigned char)key[i]) % MAX;
   return n;
}

int checkTag(char *tag, vect *hashTags){
  int first_pos = hash(tag, 30);
  char * text; 

  while (1){
  text = hashTags[first_pos].v[0];
  

  if (strcmp(text, " ") == 0){
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
  for (int i = 0; i < fil->numTags; i++){
    
    int pos = checkTag(fil->tags[i], hashTags);
    strcpy(hashTags[pos].v[hashTags[pos].n], fil->name);
    hashTags[pos].n += 1;
  }

  int pos2 = checkHash(fil->name, hashi);

  hashi[pos2] = *fil;  
}

void removeFile(char *name, file* hashi, vect *hashTags){
  int pos = checkHash(name, hashi);
  while (strcmp(name, hashi[pos].name) != 0){
    pos += 1;
    if (pos >= 2000) pos = 0;
  }
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
  while (strcmp(curName, hashi[pos].name) != 0){
    pos += 1;
    if (pos >= 2000) pos = 0;
  }
  int newPos = checkHash(newName, hashi);
  if(strcmp(curName, newName) != 0){
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
  } else {
    strcpy(hashi[pos].description, newDesc);
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
  while (strcmp(name, hashi[pos].name) != 0){
    pos += 1;
    if (pos >= 2000) pos = 0;
  }
  int pos2;
  int i;

  pos2 = checkTag(tag, hashTags);
  int x;
  for(x = 0; x < hashTags[pos2].n; x++){
    if (strcmp(hashTags[pos2].v[x], name) == 0){
      break;
    }
  }

  for (; x < hashTags[pos2].n - 1; x++){
    strcpy(hashTags[pos2].v[x], hashTags[pos2].v[x + 1]);
  }
  hashTags[pos2].n -= 1;

  for (i = 0; i < hashi[pos].numTags; i++){
    if (strcmp(hashi[pos].tags[i], tag) == 0){
      break;
    }
    }
    
  
  for(; i < hashi[pos].numTags - 1; i++){
    strcpy(hashi[pos].tags[i], hashi[pos].tags[i + 1]); 
  }
  hashi[pos].numTags -= 1;


}

void searchTag(char *tag, vect* hashTags, file * hashi){
printf("Busca por tag: %s\n", tag);
int pos = checkTag(tag, hashTags);
if(hashTags[pos].n == 1){
  printf("0 resultados encontrados.\n");
} else {
  for(int i = 1; i < hashTags[pos].n; i++){
    for(int j = i+1; j < hashTags[pos].n; j++){
      int pos2 = checkHash(hashTags[pos].v[i], hashi);
      while (strcmp(hashTags[pos].v[i], hashi[pos2].name) != 0){
        pos2 += 1;
        if (pos2 >= 2000) pos2 = 0;
      } 
      int pos3 = checkHash(hashTags[pos].v[j], hashi);
      while (strcmp(hashTags[pos].v[j], hashi[pos3].name) != 0){
        pos3 += 1;
        if (pos3 >= 2000) pos3 = 0;
      } 

      if(hashi[pos2].j > hashi[pos3].j){
        char name[31];
        strcpy(name, hashTags[pos].v[i]);
        strcpy(hashTags[pos].v[i], hashTags[pos].v[j]);
        strcpy(hashTags[pos].v[j], name);
      }
    }
    printf("%s\n", hashTags[pos].v[i]);
  }
}

printf("----------\n");

}
void searchFile(char * name, file* hashi){
  printf("Acessando arquivo: %s\n", name);
  int pos = checkHash(name, hashi);
  if(strcmp(hashi[pos].name, " ") == 0){
    int oldPos = pos;
    while (1){
      if (strcmp(hashi[pos].name, name) != 0){
        pos += 1;
        if (pos >= 2000) pos = 0;
      } else {
        break;
      }
      if (pos == oldPos) {
        printf("Arquivo %s não existe.\n", name);
        printf("----------\n");
        return;
      }
    }
  } 
  printf("Descrição: %s\n", hashi[pos].description);
  printf("Tags: ");
  for(int i = 0; i < hashi[pos].numTags; i++){
    printf("%s ", hashi[pos].tags[i]);
  }
  printf("\n");

  printf("----------\n");
}


  


