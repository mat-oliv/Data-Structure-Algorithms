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
  int ** adj2;
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
void search2(node * nodes, int w, int n, graph * g,node cur_node, vect * to_check, vect * best_path);

int compare(const void *a, const void *b); 

int main(){

  int n, m;
  scanf(" %d %d", &n, &m);

  node * nodes = malloc(n * sizeof(node));
  node * node_sorted = malloc(n * sizeof(node));
  graph * g = init_graph(n);

  vect * best_path = init_vect(n);


  int w;
  for(int i = 0; i < n; i++){
    nodes[i].id = i;
    scanf(" %d", &w);
    nodes[i].w = w;
    node_sorted[i].id = i;
    node_sorted[i].w = w;

  }


  int u, v;
  for(int i = 0; i < m; i++){
    scanf(" %d %d", &u, &v);
    g->adj2[u][v] = 1;
    g->adj2[v][u] = 1;
    insert_edge(nodes, g, u, v);
  }

  qsort(node_sorted, n, sizeof(node), compare);


  for (int i = 0; i < n; i++){
    vect * cur_path = init_vect(n);
    search(nodes, n, g, node_sorted[i], cur_path, NULL, best_path);
    free(cur_path->path);
    free(cur_path);
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
  free(node_sorted);
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
      if(g->adj2[cur_node.id][to_check->path[i]] && cur_node.id != to_check->path[i]){
        next_check->path[next_check->n] = to_check->path[i];
        next_check->n += 1;
        next_check->w += nodes[to_check->path[i]].w;
      }
     
  }
  }

  cur_path->path[cur_path->n] = cur_node.id;
  cur_path->w += cur_node.w;
  cur_path->n += 1;

  int w_max = cur_path->w + next_check->w;

 

  if(cur_path->w > best_path->w){
    best_path->w = cur_path->w;
    for(int i = 0; i < cur_path->n; i++){
      best_path->path[i] = cur_path->path[i];
    }
    best_path->n = cur_path-> n;
  }
  if(w_max <= best_path->w || next_check->n == 0){
    free(next_check->path);
    free(next_check);

    return;
}


for(int i = 0; i < next_check->n; i++){
  for (int j = i; j < next_check->n; j++){
    if(nodes[next_check->path[j]].w > nodes[next_check->path[i]].w){
      int a = next_check->path[i];
      next_check->path[i] = next_check->path[j];
      next_check->path[j] = a;
    }
  }
}

  for (int i = 0; i < next_check->n; i++){
    int sum1 = 0;
   
    for(int j = i; j < next_check->n;  j++)
      sum1 += nodes[next_check->path[j]].w;

    if(sum1 + cur_path->w < best_path->w) break;
    search(nodes, n, g, nodes[next_check->path[i]], cur_path, next_check, best_path);
    cur_path->n -= 1;
    cur_path->w -= nodes[next_check->path[i]].w;
  }

  free(next_check->path);
  free(next_check);

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
  g->adj = malloc(n * sizeof(node*));
  g->adj2 = malloc(n * sizeof(int*));
  
  for(i = 0; i < n; i ++){
    g->adj[i] = NULL;
    g->adj2[i] = malloc(n *sizeof(int));
    for(int j = 0; j < n; j++){
      g->adj2[i][j] = 0;
    }
  }

  return g;
}

void destroy_graph (graph * g) {
   int i;
   for (i = 0; i < g->n; i++){
    if (g->adj[i] != NULL){
    node* a = g->adj[i]->next;
    while(a != NULL){
      node * b = a->next;
      free(a);
      a = b;      
    }

    free(g->adj[i]);
  }
  free(g->adj2[i]);
   }
   free(g->adj2);
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

int compare(const void *a, const void *b) {
  node *na = (node*)a;
  node *nb = (node*)b;
  return nb->w - na->w;  
}