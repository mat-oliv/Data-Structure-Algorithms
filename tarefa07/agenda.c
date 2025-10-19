#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STR_SIZE 151
#define MAX_TIME 600

typedef struct {
  char * name;
  int dur;
  int priority;
  int id;

} task;

typedef struct {

  task *tasks;
  int n, alloc;
  int *indexes;

} heap;

heap * init_heap(int size);
task * init_task(int dur, int priority, int id);
void change(task * first, task * second);
void insert_task(heap* hp, heap *min, task t);
void up_in_heap(heap * hp, int k, int num);
task removing(heap *hp, int num);
void down_heap(heap *hp, int k, int num);
int priority(task a, task b, int num);
void changePriortiy(heap *max, int ID, int x, int num);
void agenda(heap *max, heap* min);

int main(){

  int m;
  scanf(" %d", &m);
  char * text = malloc(20 * sizeof(char));

  heap *hpm = init_heap(m);
  heap *hpmin = init_heap(m);


  while (1){

    scanf(" %s", text);

    if(strcmp(text, "encerrar") == 0) {
      break;
    } else if(strcmp(text, "atividade") == 0){
      int id, priority, time;
      scanf(" %d", &id);
      scanf(" %d", &priority);
      scanf(" %d", &time);
      task* t = init_task(time, priority, id);
      scanf(" %[^\n]", t->name);

      insert_task(hpm, hpmin, *t);
      printf("%s adicionada - Atividades futuras: %d\n", t->name, hpm->n);


      free(t);
    } else if (strcmp(text, "agenda") == 0) {
      agenda(hpm, hpmin);
      
    } else if (strcmp(text, "altera") == 0) {
      int id, x;
      scanf(" %d", &id);
      scanf(" %d", &x);

      int pos = hpm->indexes[id - 1];
      printf("Alterado %s %d -> %d\n", hpm->tasks[pos].name, (hpm->tasks[pos].priority), hpm->tasks[pos].priority + x);
      changePriortiy(hpm, id, x, 0);
      changePriortiy(hpmin, id, x, 1);
      
    }
  }

  printf("Restaram %d atividades\n", hpm->n);

  free(text);
  free(hpm->tasks);
  free(hpm->indexes);
  
  free(hpmin->tasks);
  free(hpmin->indexes);

  free(hpm);
  free(hpmin);

  return 0;
}



heap *init_heap(int size){
  heap * new_heap = malloc(sizeof(heap));
  new_heap->tasks = malloc(size * sizeof(task));
  new_heap->indexes = malloc(size * sizeof(int));
  new_heap->n = 0;
  new_heap->alloc = size;

  return new_heap;
}

task *init_task(int dur, int priority, int id){
  task * new_task = malloc(sizeof(task));
  new_task->name = malloc(STR_SIZE * sizeof(char));
  new_task->dur = dur;
  new_task->id = id;
  new_task->priority = priority;

  return new_task;
}

void change(task *first, task *second){
  task t = *first;
  *first = *second;
  *second = t;
}

void insert_task(heap *hp, heap *min, task t){
  hp->tasks[hp->n] = t;
  hp->indexes[t.id - 1] = hp->n;
  hp->n ++;
  up_in_heap(hp, hp->n - 1, 0);
  min->tasks[min->n] = t;
  min->indexes[t.id - 1] = min->n;
  min->n ++;
  up_in_heap(min, min->n - 1, 1);
}

#define PARENT(i) ((i-1) / 2)

int priority(task a, task b, int num){ 
  // se num == 0, é pro heap max, se num == 1, é pro heap min.
  if (num == 0) {
  if (a.priority < b.priority){
    return 1;
} else if(a.priority == b.priority){
    if(a.dur < b.dur){
      return 1;
    } else if (a.dur == b.dur){
      if(a.id > b.id)
        return 1;
    }
    }
  } else if (num == 1){
    if (a.dur > b.dur){
      return 1;
    } else if(a.dur == b.dur){
      if(a.priority < b.priority){
        return 1;
      } else if (a.priority == b.priority){
        if(a.id > b.id)
        {
          return 1;
        }
      }
    }
  }
    return 0;
}




void up_in_heap(heap * hp, int k, int num){
  // se num == 0, é pro heap max, se num == 1, é pro heap min.

  if (k > 0){
    if (priority(hp->tasks[PARENT(k)], hp->tasks[k], num)){
      change(&hp->tasks[k], &hp->tasks[PARENT(k)]);
      hp->indexes[hp->tasks[k].id-1] = k;
      hp->indexes[hp->tasks[PARENT(k)].id-1]  = PARENT(k);
      up_in_heap(hp, PARENT(k), num);
    }
  }
}

#define F_ESQ(i) (2*i+1)
#define F_DIR(i) (2*i+2)


void down_heap(heap *hp, int k, int num){
    // se num == 0, é pro heap max, se num == 1, é pro heap min.
  int big_son;
  if (F_ESQ(k) < hp->n){
    big_son = F_ESQ(k);
    if (F_DIR(k) < hp->n)
      if(priority(hp->tasks[F_ESQ(k)], hp->tasks[F_DIR(k)], num))
        big_son = F_DIR(k);
    
    if (priority(hp->tasks[k], hp->tasks[big_son], num)){
      change(&hp->tasks[k], &hp->tasks[big_son]);
      hp->indexes[hp->tasks[k].id - 1] = k;
      hp->indexes[hp->tasks[big_son].id - 1]  = big_son;
      down_heap(hp, big_son, num);
    }
  }
}

task removing(heap *hp, int num){
  // se num == 0, é pro heap max, se num == 1, é pro heap min.
  // remove sempre do topo
  task t = hp->tasks[0];
  int id1 = hp->tasks[0].id;
  int id2 = hp->tasks[hp->n - 1].id;
  change(&hp->tasks[0], &hp->tasks[hp->n - 1]);
  hp->indexes[id1 - 1] = hp->n - 1; 
  hp->indexes[id2 - 1] = 0;
  hp->n--;
  down_heap(hp, 0, num);
  return t;
}


void changePriortiy(heap *hp, int ID, int x, int num){
  // se num == 0, é pro heap max, se num == 1, é pro heap min.
  int pos = hp->indexes[ID - 1];
  hp->tasks[pos].priority += x;

  if (x > 0){
   up_in_heap(hp, pos, num);
  } else if (x < 0){
   down_heap(hp, pos, num);
  }
}

void changeDur(heap *min, int ID){
  int pos = min->indexes[ID - 1];
  min->tasks[pos].dur = -1;

  up_in_heap(min, pos, 1);
}

void agenda(heap* max, heap* min){
  int cur_time = MAX_TIME;
  printf("--AGENDA--\n");
  while (1){
    if(max->n <= 0) {
      printf("----------\n");
      break;
    }
    if (max->tasks[0].dur <= cur_time){
      int dur = max->tasks[0].dur;
      printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n", max->tasks[0].id, max->tasks[0].name, max->tasks[0].priority, max->tasks[0].dur);
      changeDur(min, max->tasks[0].id); 
      removing(max, 0);
      removing(min, 1);
      cur_time -= dur;
    } else if (min->tasks[0].dur <= cur_time){
      int dur = min->tasks[0].dur;
      printf("(%d) %s - Prioridade %d - Estimativa: %d minutos\n", min->tasks[0].id, min->tasks[0].name, min->tasks[0].priority, min->tasks[0].dur);
      changePriortiy(max, min->tasks[0].id, 100000, 0);
      removing(max, 0);
      removing(min, 1);
      cur_time -= dur;
  }else {
    printf("----------\n");
    break;
  }



}
}



