#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef struct node node;
struct node {
  node * left;
  node * right;
  int value;
  char * text;
  int height;
  int l;
  node * parent;
};

void free_nodes(node* root);
void printall(node* root, char *str);
int max(int a, int b){
  if (a > b)
    return a;
  else
    return b;
}
void string_to_tree(node *root, char * string, int t, int l){
  if (t > l){
    char * left_string;
    char * right_string;
    int a, b;
    if ( (int) (t / 2) < t / 2.0){
       a = t / 2;
       b = a + 1;
    } else {
      a = t / 2;
      b = t / 2;
    }
    left_string = malloc((a + 1) * sizeof(char));
    right_string = malloc((b + 1) * sizeof(char));
    // PODE ESTAR FALTANDO \0

    left_string[a] = '\0';
    right_string[b] = '\0';
    for(int i = 0; i < a; i++){
      left_string[i] = string[i];
    }
    for(int i = 0; i < b; i++){
      right_string[i] = string[i + a];
    }
    node* node_left = malloc(sizeof(node));
    node* node_right = malloc(sizeof(node));
    node_left->parent = root;
    node_right->parent = root;
    node_left->l = l;
    node_right->l = l;
    root->left = node_left;
    root->right = node_right;
    free(string);
    string_to_tree(root->left, left_string, a, l);
    string_to_tree(root->right, right_string, b, l);
  } else {
    root->text = string;
    root->value = t;
    root->height = 1;
  }
}
int summ(node* root){
  if(root->left == NULL && root->right == NULL)
  {
    return root->value;
  }
  int value = summ(root->left) + summ(root->right);
  root->value = value;
  return value;
}
int get_height(node * root){
  if (root == NULL){
    return 0;
  }
  int height = 1 + max(get_height(root->left), get_height(root->right));
  root->height = height;
  return height;
}
void loop_sum(node *root){
  if (root->left == NULL && root->right == NULL){
      return;
  } else{
    root->value = summ(root->left);
    loop_sum(root->left);
    loop_sum(root->right);
  }
}
void insert(node *root, int index, char letter){
  char *new_text = malloc( (root->value + 2) * sizeof(char) );
  int i;
  for(i = 0; i < index; i++){
    new_text[i] = root->text[i];
  }
  new_text[i] = letter;
  i++;
  for(; i < root->value + 1; i++){
    new_text[i] = root->text[i - 1];
  }
  free(root->text);
  root->text = new_text;
  root->value += 1;
  if (root->value > root->l){
    int val = root->value % 2;
    int a = root->value / 2;
    int b = a + val;
    char *left_string, *right_string;
    left_string = malloc((a + 1) * sizeof(char));
    right_string = malloc((b + 1) * sizeof(char));
    // PODE ESTAR FALTANDO \0
    for(int z = 0; z < a; z++){
      left_string[z] = root->text[z];
    }
    for(int z = 0; z < b; z++){
      right_string[z] = root->text[z + a];
    }

    left_string[a] = '\0';
    right_string[b] = '\0';
  //  free(root->text); 
    node *left_node, *right_node;
    left_node = malloc(sizeof(node));
    right_node = malloc(sizeof(node));
    left_node->parent = root;
    right_node->parent = root;
    left_node->l = root->l;
    right_node->l = root->l;
    left_node->value = a;
    left_node->text = left_string;
    right_node->value = b;
    right_node->text = right_string;
    root->right = right_node;
    root->left = left_node;
  }
}
void removing(node *root, int index, int t){
  int i = 0;
  char * new_text = malloc((root->value) * sizeof(char));
  new_text[root->value - 1] = '\0';
  for(; i < root->value - 1; i ++){
    if (i >= index){
      new_text[i] = root->text[i + 1];
    } else {
      new_text[i] = root->text[i];
    }
  }
  root->value -= 1;   
  new_text[root->value] = '\0'; 
  if(root->value == 0){
    node *parent = root->parent;
    if (parent->right == root){
      if (parent->left->text == NULL){
        char *str = malloc((t+1) * sizeof(char));
        str[t] = '\0';
        printall(parent->left,str);
        free_nodes(parent->left->left);
        free_nodes(parent->left->right);
        parent->left->text = strdup(str);
        parent->left->value = strlen(str);
        free(str);
      }
      parent->text = strdup(parent->left->text);
      parent->value = parent->left->value;
      free(root->text);
      free(root);
      free(parent->left->text);
      free(parent->left);
     
    } else {
      if (parent->right->text == NULL){
        char *str = malloc((t+1) * sizeof(char));
        str[t] = '\0';
        printall(parent->right,str);
        free_nodes(parent->right->left);
        free_nodes(parent->right->right);
        parent->right->text = strdup(str);
        parent->right->value = strlen(str);
        free(str);
      }
      parent->text = strdup(parent->right->text);
      parent->value = parent->right->value;

     free(root->text);
     free(root);
     free(parent->right->text);
     free(parent->right);
      
    }
    parent->left = NULL;
    parent->right = NULL;

    if (parent->value > parent->l){
      char *str_save = parent->text;
      parent->text = NULL;
      string_to_tree(parent, str_save, t, parent->l);

    }
    
    free(new_text);
  } else {
    free(root->text);
    root->text = strdup(new_text);
    free(new_text);
  }
}

char search(node *root, int index, int x, int t){

  if(root->right == NULL && root->left == NULL){
    if (x == - 1){
      char letter;
      scanf(" %c", &letter);
      insert(root, index, letter);
    } else if (x == -2){
      removing(root, index, t);
      return ' ';
    }
  
    return root->text[index];

  } else{
    if (index >= root->value){
      index -= root->value;
      root = root->right;
    } else{
      root = root->left;
    }

    return search(root, index, x, t);
  }
}

void printall(node *root, char * full){

  if (root == NULL) return;
  if (root->left == NULL || root->right == NULL){
    if(root->text != NULL && full != NULL){
      strcat(full, root->text);
    }
  } else{ 
    printall(root->left, full);
    printall(root->right, full);
  }
}

void printing(node *root, int i, int j){
  for(int x = i; x < j; x++){
    printf("%c", search(root, x, 0, 0));
  }
  printf("\n");

}

void free_nodes(node *root){
  if (root->left == NULL && root->right == NULL){
    free(root->text);
    free(root);
    return;
} else{
  node *left = root->left;
  node *right = root->right;
  free(root->text);
  free(root);
  free_nodes(left);
  free_nodes(right);
}

}

node *tree_to_string(node * root, int t){
  char* str = malloc((t + 1) * sizeof(char));
  str[t] = '\0';
  printall(root, str);
  free_nodes(root->left);
  free_nodes(root->right);
  int tt = strlen(str);
  str[tt] = '\0';
  string_to_tree(root, str, tt, root->l);
  return root;
}

void checkNodes(node *root, int t){
  if (root->left == NULL && root->right == NULL){
    return;
} else{
  int deltaHeight = root->right->height - root->left->height;
  if(deltaHeight < -4 || deltaHeight > 4){
    tree_to_string(root, t);
  } else {
    checkNodes(root->left, t);
    checkNodes(root->right, t);
  }
}
}

int main(){

  int p;
  char * sequence;
  int l, t;

  scanf(" %d %d %d", &p, &t, &l);
  
  sequence = malloc((t + 1) * sizeof(char));
  scanf(" %s", sequence);

  node *root = malloc(sizeof(node));
  root->parent = NULL;
  root->l = l;

  string_to_tree(root, sequence, t, l);
  loop_sum(root);



  int i = 0;
  for(; i < p; i++){
    char text[50];
    scanf(" %s", text);
    if(strcmp(text, "INSERT") == 0){
      int index;
      scanf(" %d", &index);
      search(root, index, -1, 0);
      loop_sum(root);
      get_height(root);
      t += 1;
      checkNodes(root, t);
    } else if(strcmp(text, "REMOVE") == 0){
      int index;
      scanf(" %d", &index);
      t -= 1;
      search(root, index, -2, t);
      loop_sum(root);
      get_height(root);
      checkNodes(root, t);
    } else if(strcmp(text, "PRINTALL") == 0){
      char *full_text = malloc((t + 1) * sizeof(char));
      full_text[t] = '\0';
      printall(root, full_text);
      printf("DNA inteiro: %s\n", full_text);
      free(full_text);
    } else if(strcmp(text, "PRINT") == 0){
      int i, j;
      scanf(" %d", &i);
      scanf(" %d", &j);
      printf("Trecho %d-%d: ", i, j);
      printing(root, i, j);
    }
    if(strcmp(text, "sair") == 0)
    break;
  }

  char *str = malloc((t + 1) * sizeof(char));
  printall(root, str);

  int a, tt, c , g; 
  a = 0, tt= 0, c = 0, g = 0;
  for (int i = 0; i < t; i++){
    if (str[i] == 'A')
      a += 1;
    else if (str[i] == 'T')
      tt += 1;
    else if (str[i] == 'C')
      c += 1;
    else if (str[i] == 'G')
      g += 1;
  }

  printf("Contagem final: A:%d T:%d C:%d G:%d", a, tt, c, g);

  free(str);

  return 0;
}
