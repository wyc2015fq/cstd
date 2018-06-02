
#ifndef _MEMMAP_INL_
#define _MEMMAP_INL_

#include <stdlib.h>
#include <string.h>

int mem_clear(void* pp, int* n) {
  void** p = (void**)pp;
  if (*p) {
    free(*p);
    *p = NULL;
  }
  *n = 0;
  return 0;
}

int mem_delete(void* p, int* n, int pos, int m, int elemsize) {
  char* pc = (char*)p;
  *n -= m;
  memmove(pc+pos*elemsize, pc+(pos+m)*elemsize, (*n-pos)*elemsize);
  return 0;
}
int mem_pop_front(void* p, int* n, int m, int elemsize) {
  return mem_delete(p, n, 0, 1, elemsize);
}

int imap_get_copy(const void* p, int i, int elemsize, void* q) {
  char* pc = (char*)p;
  memcpy(q, pc + i*(elemsize + sizeof(int)) + sizeof(int), elemsize);
  return 0;
}
int imap_find(const void* p, int n, int key, int elemsize, void* q) {
  const char* pc = (const char*)p;
  int i;
  int ee = elemsize+sizeof(int);
  for (i=0; i<n; ++i) {
    int* pk = (int*)(pc+i*ee);
    if (*pk == key) {
      if (q) {
        imap_get_copy(p, i, elemsize, q);
      }
      return i;
    }
  }
  if (q) {
    memset(q, 0, elemsize);
  }
  return -1;
}
int imap_insert(void* pp, int* n, int key, const void* value, int elemsize) {
  void** p = (void**)pp;
  int ee = elemsize+sizeof(int);
  int n1 = imap_find(*p, *n, key, elemsize, 0);
  if (n1<0) {
    n1 = *n;
    mem_push_back(pp, n, 0, 1, ee);
  }
  if (value) {
    void** p = (void**)pp;
    char* pc = (char*)(*p);
    int* pk = (int*)(pc + n1*ee);
    *pk = key;
    memcpy(pk+1, value, elemsize);
  }
  return n1;
}

int imap_delete_byid(void* p, int* n, int k, int elemsize) {
  char* pc = (char*)p;
  int ee = elemsize+sizeof(int);
  if (k < *n) {
    --(*n);
    memcpy(pc + k*ee, pc + (*n)*ee, ee);
  }
  return 0;
}
int imap_clear(void* pp, int* n) {
  return mem_clear(pp, n);
}

typedef struct imap_t {
  int n;
  char* p;
} imap_t;

#endif // _MEMMAP_INL_
