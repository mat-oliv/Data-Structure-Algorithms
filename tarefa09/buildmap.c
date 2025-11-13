#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 168001

typedef struct node node;

struct node {

  char name[16];
  node **neighbors;
  int id;
  int n;
  int depth;
  int state2;

  int state; // 0 > Não visitado, 1 > Processando, 2 > Visitado
};

typedef struct {
  node print_order[5001];
  int n;
  int g;
  int h;
} vect;

typedef struct {

  char node_depth[5000][5000][16];
  int m_n[5000];
  int n;

} vectc;

int get_id(char * name, int x, node ** nodes);
node * init_node();
void DFS(node* all, vect * order);

void sorting(node* nod);
void sorted(char names[][16], int m);
void BFS(node * all, vectc * vec);


int main(){

  char line[MAX];
  const char limits[] = " :";

  node ** nodes;
  nodes = malloc(5000 * sizeof(node*));

  int x = 0;

  while (fgets(line, sizeof(line), stdin) != NULL) {
      if (line[0] == '\n' && line[1] == '\0') 
          continue; 
      
      if(strcmp(line, "ff\n") == 0) break;
      char *tok = strtok(line, limits);
      int j = 0;
      node * dad;

      while(tok != NULL){
        if(tok[strlen(tok) - 1] == '\n')
          tok[strlen(tok) - 1] = '\0';
        int id = get_id(tok, x, nodes);

        node * nod;
        if (id == -1){
          nod = init_node();
          nod->id = x;
          strcpy(nod->name, tok);
          nodes[x] = nod;
          x += 1;
        } else {
          nod = nodes[id];
        }


        if (j == 0){
          nod->neighbors = malloc(5000 *sizeof(node*));
          dad = nod;
          j += 1;
          tok = strtok(NULL, limits);
          continue;
        }

        dad->neighbors[dad->n] = nod;
        dad->n += 1;

        tok = strtok(NULL, limits);
      }

  }
  vect *print_order = malloc(sizeof(vect));
  print_order->n = 0;
  print_order->g = 0;
  print_order->h = 0;

  DFS(nodes[0], print_order);

  char names[5001][16];
  if(print_order->g == -2){
    for(int i =1 ; i < print_order->h-1; i++){
        strcpy(names[i], print_order->print_order[i].name);
    }

    for(int i = 1; i < print_order->h-1 - 1; i++ ){
      for(int j = i; j < print_order->h- 1; j++){
        if(strcmp(names[j], names[i]) < 0){
          char name[16];
          strcpy(name, names[i]);
          strcpy(names[i], names[j]);
          strcpy(names[j], name);
        }
      }
    }    strcpy(names[0], print_order->print_order[0].name);
         strcpy(names[print_order->h - 1], print_order->print_order[print_order->h - 1].name);


    printf("Erro: dependências circulares entre os arquivos necessários para construir 'all'\n");
    for(int i =0 ; i < print_order->h; i++){
      printf("%s", names[i]);
      if(i < print_order->h - 1){
        printf(" -> ");
      }
    }

  } else {
    for(int i =0 ; i  < print_order->n; i++){
      printf("SEQ - %d: %s\n", i+1, print_order->print_order[i].name);
  }

  vectc *vec = malloc(sizeof(vectc));
  vec->n = 0;
  for (int i =0; i < 5000; i++){
    vec->m_n[i] = 0;
  }

  BFS(nodes[0], vec);

  for(int i = 0; i < vec->n; i++){
    sorted(vec->node_depth[i], vec->m_n[i]);
   for(int j = 0; j < vec->m_n[i]; j ++){
      printf("Distância até all - %d : %s \n", i, vec->node_depth[i][j]);
    }
  }

  
    
  }

  return 0;
}

void DFS(node * all, vect *order){
  
  if(all->state == 1){
    order->print_order[order->n] = *all;
    order->n += 1; 
    order->g = -1;
    order->print_order[5000] = *all;
    order->print_order[order->h] = *all;
    order->h += 1;
    return;
  } else if(all->state == 2){
    return;
  }
  
  all->state = 1;

  if(all->n == 0){
    order->print_order[order->n] = *all;
    order-> n += 1;
    all->state = 2;
    return;
  }
  sorting(all);
  for(int i = 0; i < all->n; i++){
    DFS(all->neighbors[i], order);
  }

  if(order->g == -1){
    if(strcmp(all->name, order->print_order[5000].name) == 0){
      order->g = -2;
    } 
    order->print_order[order->h] = *all;
      order->h += 1;

  }

  order->print_order[order->n] = *all;
  order->n += 1;
  all->state = 2;

  
  
}

void add_to_list(node* dad, node* son){
  dad->neighbors[dad->n] = son;
}

node * init_node(){
  node * nod = malloc(sizeof(node)); 
  nod->n = 0;
  nod->neighbors = NULL; 
  nod->state = 0;
  nod->depth = -1; 
  nod->state2 = 0;
  return nod; 
}

int get_id(char *name, int x, node ** nodes){
  for (int i = 0; i < x; i ++){
    if (strcmp(name, nodes[i]->name) == 0){
      return i;
      break;
    }
  }

  return -1;
}

void sorting(node * nod){
  for(int i = 0; i < nod->n; i++ ){
    for(int j = i; j < nod->n; j++){
      if(strcmp(nod->neighbors[j]->name, nod->neighbors[i]->name) < 0){
        node * nodi_copy = nod->neighbors[i];
        nod->neighbors[i] = nod->neighbors[j];
        nod->neighbors[j] = nodi_copy;
      }
    }
  }
}

void sorted(char names[][16], int n){
  for(int i = 0; i < n; i++ ){
    for(int j = i; j < n; j++){
      if(strcmp(names[j], names[i]) < 0){
        char * namesi = names[i];
        strcpy(names[i], names[j]);
        strcpy(names[j], namesi);
      }
    }
  }
}

void BFS(node * all, vectc* vec){

  node frontier[5001];
  int k = 0;
  int j = 1;
  frontier[0] = *all;
  all->depth = 0;
  while (1){
    if (k>= j || k >= 5000 || j >= 5000) break;

    node a = frontier[k];
    printf("%s", a.name);

    k += 1;

    for (int i = 0; i < a.n; i++){
      if(a.neighbors[i]->state2 == 0){
      a.neighbors[i]->state2 = 1;
      a.neighbors[i]->depth = a.depth + 1;
      frontier[j] = *a.neighbors[i];
      j += 1;
      }
    }

   strcpy(vec->node_depth[a.depth][vec->m_n[a.depth]], a.name);
   vec->m_n[a.depth] += 1;
   if(a.depth >= vec->n) 
      vec->n += 1;
    
  }
  
}

