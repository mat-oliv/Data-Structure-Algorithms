#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node node;

struct node {

  int id;
  int w;
  node * next;
};


typedef struct {
  node ** adj;
  int n;
} graph;



typedef struct {

  int n;
  int w;
  int * path;

} vect;

vect * init_vect(int n);

graph * init_graph(int n);
void destroy_graph (graph* g);
void insert_edge(node* nodes, graph* g, int u, int v);
node * add_list( node* list, int w, int id);
void remove_edge(graph *g, int u, int v);
int has_edge(graph* g, int u, int v);


void search(node * nodes, int n, graph * g,node cur_node, vect * cur_path, vect * to_check, vect * best_path);

int in_list(vect * vec, int x);

int main(){

  int n, m;
  scanf(" %d %d", &n, &m);


  node * nodes = malloc(n * sizeof(node));
  graph * g = init_graph(n);

  vect * best_path = init_vect(n);


  int w;
  for(int i = 0; i < n; i++){
    nodes[i].id = i;
    scanf(" %d", &w);
    nodes[i].w = w;
  }

  
  int u, v;
  for(int i = 0; i < m; i++){
    scanf(" %d %d", &u, &v);
    insert_edge(nodes, g, u, v);
  }

  for (int i = 0; i < n; i++){
    search(nodes, n, g, nodes[i], NULL, NULL, best_path);
  }

  printf("%d\n", best_path->w);
  for(int i = 0; i < best_path->n; i++){
    if(i < best_path->n - 1)
      printf("%d ", best_path->path[i]);
    else
     printf("%d\n", best_path->path[i]);

  }

 // system("leaks rede");
  free(best_path->path);
  free(best_path);
    destroy_graph(g);
    free(nodes);
    
  return 0;
}

void search(node * nodes, int n, graph * g, node cur_node, vect * cur_path, vect * to_check, vect * best_path){
  vect * next_check;
  next_check = init_vect(n);
  if(to_check == NULL){
   node * next_node = g->adj[cur_node.id];
   while (next_node != NULL){
    next_check->path[next_check->n] = next_node->id;
    next_check->n += 1;
    next_check->w += next_node->w;
    next_node = next_node->next;
   }
  } else {
    for(int i = 0; i < to_check->n; i++){
      node * next_node = g->adj[cur_node.id];
      while (next_node != NULL){
      if(to_check->path[i] == next_node->id){
        next_check->path[next_check->n] = next_node->id;
        next_check->n += 1;
        next_check->w += next_node->w;
        break;
      }
      next_node = next_node->next;
      }
  }
  }


  vect * cur_pat;
  cur_pat = init_vect(n);

  if(cur_path != NULL){
   cur_pat->n = cur_path->n;
   cur_pat->w = cur_path->w;
   for(int i =0; i < cur_pat->n; i++){
    cur_pat->path[i] = cur_path->path[i];
   }
  } 

  cur_pat->path[cur_pat->n] = cur_node.id;
  cur_pat->w += cur_node.w;
  cur_pat->n += 1;

  int w_max = cur_pat->w + next_check->w;

 

  if(cur_pat->w > best_path->w){
    best_path->w = cur_pat->w;
    for(int i = 0; i < cur_pat->n; i++){
      best_path->path[i] = cur_pat->path[i];
    }
    best_path->n = cur_pat-> n;
  }
  if(w_max <= best_path->w || next_check->n == 0){
    free(next_check);
    free(cur_pat->path);
    free(cur_pat);
    return;
}

  for (int i = 0; i < next_check->n; i++){
    search(nodes, n, g, nodes[next_check->path[i]], cur_pat, next_check, best_path);
  }

  free(next_check);
  free(cur_pat->path);
  free(cur_pat);

}


node *add_list(node * list, int w, int id){
  node * new = malloc(sizeof(node));
  new->id = id;
  new->w = w;
  new->next = list;
  return new;
}

void insert_edge(node * nodes, graph* g, int u, int v){
 g->adj[u] = add_list(g->adj[u], nodes[v].w, v);
 g->adj[v] = add_list(g->adj[v], nodes[u].w, u);

}


graph * init_graph(int n){
  int i;
  graph * g = malloc(sizeof(graph));
  g->n = n;
  g->adj = malloc(n * sizeof(int*));
  for(i = 0; i < n; i ++){
    g->adj[i] = NULL;
  }

  return g;
}

void destroy_graph (graph * g) {
   int i;
   for (i = 0; i < g->n; i++)
   free(g->adj[i]);
   free(g->adj);
   free(g);
   }
  
vect * init_vect(int n){
  vect * v = malloc(sizeof(vect));
  v->n = 0;
  v-> w = 0 ;
  v->path = malloc(n * sizeof(int));
  return v;
}

int in_list(vect * v, int x){
  for(int i = 0; i < v->n; i++){
    if(v->path[i] == x) return 1;
  }

   return 0;
}