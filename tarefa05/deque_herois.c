// Escolhi a lista duplamente ligada, pois facilita a mudança de posição entre dois nós,
//Eem relação ao tempo consumido na operação, possiblitando também a inserção e remoção
// Nas extremidades em tempo O(1).
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
  char *name;
  struct node *next;
  struct node *prev;
} node;

typedef struct {
  node *start;
  node *end;
} deque;

void print_list(node *nod);

node* search(deque *d, int pos){
  node *start = d->start;
  for(int i = 0; i < pos; i++) start = start->next;

  return start;
  
}



node* init_node(char *name){
  node *new = malloc(sizeof(node));

  new->name = name;

  new->next = NULL;
  new->prev = NULL;

  return new;
}

void free_node(node *n){
  free(n->name);

  free(n);
}

void addBeginning(deque *d, char *name){
  node *new = init_node(name);

  if (d->start == NULL){
    d->start = new;
    new->next = NULL;
    new->prev = NULL;
    d->end = new;
    return;
  }

  new->next = d->start;
  new->prev = NULL;

  d->start->prev = new;

  d->start = new;
}

void  removeBeginning(deque *d){
  if (d->start->next == NULL){
    d->start = NULL;
    d->end = NULL;
    return;
  }
  node* new_begin = d->start->next;
  new_begin->prev = NULL;

  d->start = new_begin;
}

void reverse_caos(deque *d, int num){
  node* start = d->start;
  printf("caos");
  for(int i = 0; i < num - 1; i++){
    printf(" %s", start->name);
    start = start->next;
  }
  printf(" %s ->", start->name);

    node *cur = d->start;
    node *prev = start->next;
    node *ex_next;
    node *ex_start = d->start;

    d->start = start;

    for (int i = 0; i < num; i++) {
      ex_next = cur->next;
      cur->next = prev;
      cur->prev = ex_next;
      prev = cur;
      cur = ex_next;
    }

    if (cur == NULL){
      d->end = ex_start;
      d->end->next = NULL;
    } else {
      cur->prev = ex_start;
    }

    d->start->prev = NULL;

    node *print_node = d->start;
    for(int i = 0; i < num; i++){
      printf(" %s", print_node->name);
      print_node = print_node->next;
    }
    
    printf("\n");
    

  
}

void reverse_confusion(deque *d, int num){
  node* start = d->end;
  printf("confusao");
  for(int i = 0; i < num - 1; i++){
    start = start->prev;
  }

  node *start_print = start;
  for (int i = 0; i < num - 1; i ++){
    printf(" %s", start_print->name);
    start_print = start_print->next;
  }
    printf(" %s ->", start_print->name);

    node *cur = d->end;
    node *prev = start->prev;
    if (prev != NULL)
      prev->next = cur;
    node *ex_prev;

    node *ex_end = d->end;
    d->end = start;

    if(start->prev == NULL){
      d->start = ex_end;
    }


    for (int i = 0; i < num; i++) {
      ex_prev = cur->prev;
      if (prev != NULL){
      cur->prev = prev;
    } else{
      cur->prev = NULL;
    }
      if(i < num - 1)
        cur->next = ex_prev;
      else
        cur->next = NULL;
      prev = cur;
      cur = ex_prev;
    }

    node *print_node = d->end;
    for(int i = 0; i < num - 1; i++){
      print_node = print_node->prev;
    }

    for(int i =0; i < num - 1; i ++){
      printf(" %s", print_node->name);
      print_node = print_node->next;
    }

    printf(" %s", print_node->name);
    
    printf("\n");
  
}

void addEnd(node *end, char *name, deque *d){
  node *new = init_node(name);

  if (end == NULL){
    d->start = new;
    d->end = new;
    return;
  }


  new->next = NULL;
  new->prev = end;

  end->next = new;

  d->end = new;

  if (d->start == NULL){
    d->start = new;
  }
  
}

void removeEnd(node * end, deque *d){
  if (end == NULL) return;
  if(end->prev == NULL){
    d->start = NULL;
    d->end = NULL;
    return;
  }
  node* new_end = end->prev;
  
  new_end->next = NULL;

  d->end = new_end;
}

void print_list(node* nod){
 while (1) {
  if (nod == NULL){
   printf("\n");
    return;
  }
  if(nod->next == NULL){
    printf("%s\n", nod->name);
    break;
  }
  printf("%s ", nod->name);
  nod = nod->next;
 }
}

void set_position(deque *d, char* name, int pos){


   node *start = d->start;
   if (start == NULL){
    addBeginning(d, name);
    return;
   }

   for (int i = 0; i < pos; i++){
    if (start->next != NULL)
      start = start->next;
    else{
      addEnd(d->end, name, d);
      return;
    }
   }
   node *new = init_node(name);


   if (start->prev != NULL){
    start->prev->next = new;
    new->prev = start->prev;
  } else {
    d->start = new;
  } 
   new->next = start;
   start->prev = new;
}

node * remove_position(deque *d, int pos){
  node *start = d->start;

  for(int i = 0; i < pos; i ++){
    if(start->next != NULL)
      start = start->next;
  }

  if (pos == 0){
    removeBeginning(d);
  } else if (start->next == NULL){
    removeEnd(start, d);
    return start;
  }else {
  start->prev->next = start->next;
  start->next->prev = start->prev;
  }

  return start;
}

void attack(deque *d){
  node *start_copy = d->start;
  removeBeginning(d);
  addEnd(d->end, start_copy->name, d);

  free(start_copy);
}

void clear_list(deque *d){
  node *nod = d->start;
  node *next_nod;

  while (1) {
    if (nod == NULL){
      return;
    }
    if(nod->next == NULL){
      free_node(nod);
      break;
    }
    
    next_nod = nod->next;
    free_node(nod);
    nod = next_nod;
   }

}


int main(){

  deque *d = malloc(sizeof(deque));
  d->start = NULL;
  d->end = NULL;
  
  char *text = malloc(20 * sizeof(char));
  scanf(" %s", text);

  while (strcmp(text, "sair") != 0){
  if (strcmp(text, "convocar-frente") == 0){
    char *name = malloc(51 * sizeof(char));
    scanf(" %s", name);
    addBeginning(d, name);
    printf("%s entrou no inicio\n", name);
  } else if (strcmp(text, "convocar-fim") == 0){
    char *name = malloc(51 * sizeof(char));
    scanf(" %s", name);
    addEnd(d->end, name, d);
    printf("%s entrou no fim\n", name);
  } else if (strcmp("posicionar", text) == 0){
    char *name = malloc(51 * sizeof(char));
    scanf(" %s", name);
    int pos;
    scanf(" %d", &pos);
    set_position(d, name, pos);
    printf("%s inserido na posicao %d\n", name, pos);
  } else if(strcmp(text, "puxar-frente") == 0){
    node *old_start = d->start;
    removeBeginning(d);
    printf("%s removido do inicio\n", old_start->name);
    free_node(old_start);
  } else if(strcmp(text, "puxar-tras") == 0){
    node *old_end = d->end;
    removeEnd(d->end, d);
    printf("%s removido do fim\n", old_end->name);
    free_node(old_end);
  } else if(strcmp(text, "remover") == 0){
    int pos;
    scanf(" %d", &pos);
    node *n = remove_position(d, pos);
    printf("%s removido da posicao %d\n", n->name, pos);
    free_node(n);
  } else if(strcmp(text, "ataque") == 0){
    node *nod = d->start;
    printf("%s atacou e foi para o fim\n", nod->name);
    attack(d);
  } else if(strcmp(text, "caos") == 0){
    int num;
    scanf("%d", &num);
    reverse_caos(d, num);
  } else if(strcmp(text, "confusao") == 0){
    int num;
    scanf("%d", &num);
    reverse_confusion(d, num);
  }

  printf("ordem de ataque: ");
  print_list(d->start);
  scanf(" %s", text);
}

 
  clear_list(d);
  free(text);
  free(d);


  return 0;
}



