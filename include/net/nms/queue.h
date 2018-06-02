
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include <common.h>
#include <semaphore.h>

typedef struct _queue_t {
  void** data;
  int head;
  int tail;
  int size;
  int count;
  sem_t sem;
  pthread_mutex_t locker;
} queue_t;

queue_t* queue_new(int size);
void queue_delete(queue_t* Q);
int queue_resize(queue_t* Q, int newsize);
int queue_pop(queue_t* Q, void** item);
int queue_push(queue_t* Q, void* item);
//int queue_tail(queue_t *Q, void **item);

int queue_getcount(queue_t* Q);
int queue_getsize(queue_t* Q);

void queue_print(queue_t* Q);
void queue_printOccupy(queue_t* Q);
#include "queue.c"

#endif
