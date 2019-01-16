

#include "mempool_c.h"

#define _REFERENCE_X(a, b)  a&
#define _POINTER_X(a, b)    a*
struct DList;

typedef struct DList* _Nodeptr;

struct DList {
  _Nodeptr _Next, _Prev;
  int ttt;
};

typedef void (*set_node_fun_t)(_Nodeptr* _S, const DList* _P);
typedef int (*cmp_node_fun_t)(const DList* _S1, const DList* _S2);

#define _Freenode(_S)  if (Freenode) {Freenode(&_S, NULL);}
#define _Copynode(_S, _From)  if (Freenode) {Freenode(&_S, _From);}
//void _Freenode(_Nodeptr _S){  }

typedef _REFERENCE_X(_Nodeptr, node_alloc_fun_t) _Nodepref;
#define _Acc_Next(_P) ((*_P)._Next)
#define _Acc_Prev(_P) ((*_P)._Prev)
#define _Acc_Value(_P) (_P)

_Nodeptr& begin(_Nodeptr list_Head) {return (_Nodeptr(_Acc_Next(list_Head))); }
_Nodeptr& end(_Nodeptr list_Head) {return (_Nodeptr((list_Head))); }
_Nodeptr _Inc(_Nodeptr _P) {
  return _P->_Next;
}
_Nodepref Inc(_Nodepref _P) {
  return _P = _P->_Next;
}
_Nodeptr _Dec(_Nodeptr _P) {
  return _P->_Prev;
}
_Nodepref Dec(_Nodepref _P) {
  return _P = _P->_Prev;
}
size_t size(_Nodeptr list_Head) {
  size_t _Size = 0;
  _Nodeptr b = begin(list_Head);
  _Nodeptr e = end(list_Head);
  for (; b != e; b = _Inc(b)) {
    ++_Size;
  }
  return (_Size);
}
void init(_Nodeptr _P) {
  if (_P->_Next == _P->_Prev && _P->_Next == NULL) {
    _P->_Next = _P->_Prev = _P;
    _P->ttt = 456;
  }
}
bool empty(_Nodeptr list_Head) {
  return (begin(list_Head) == end(list_Head));
}

_Nodeptr _Buynode(_Nodeptr _Narg, _Nodeptr _Parg, _Nodeptr x)
{
  _Nodeptr _S = x;
  _Acc_Next(_S) = _Narg;
  _Acc_Prev(_S) = _Parg;
  return (_S);
}
_Nodeptr insert(_Nodeptr _P, _Nodeptr x)
{
  _Nodeptr _S = _P;
  _S->_Prev = _Buynode(_S, _Acc_Prev(_S), x);
  _S = _Acc_Prev(_S);
  _Acc_Next(_Acc_Prev(_S)) = _S;
  return (_Nodeptr(_S));
}
void insert(_Nodeptr _P, size_t _M,  _Nodeptr _X)
{
  for (; 0 < _M; --_M) {
    insert(_P, _X);
  }
}

void insert(_Nodeptr _P, _Nodeptr _F, _Nodeptr _L)
{
  for (; _F != _L; Inc(_F))
    insert(_P, _Acc_Value(_F));
}

_Nodeptr erase(_Nodeptr _P, set_node_fun_t Freenode)
{
  _Nodeptr _S = (_P);
  _P = _P->_Next;
  _Acc_Next(_Acc_Prev(_S)) = _Acc_Next(_S);
  _Acc_Prev(_Acc_Next(_S)) = _Acc_Prev(_S);
  _Freenode(_S, NULL);
  //--_Size;
  return (_P);
}
_Nodeptr erase(_Nodeptr _F, _Nodeptr _L, set_node_fun_t Freenode)
{
  for (; _F != _L; ) {
    _F = erase(_F, Freenode);
  }
  return (_F);
}

_Nodeptr push_front(_Nodeptr list_Head, _Nodeptr _X) {return insert(begin(list_Head), _X); }
_Nodeptr pop_front(_Nodeptr list_Head, set_node_fun_t Freenode) { return erase(begin(list_Head), Freenode); }
void push_back(_Nodeptr list_Head, _Nodeptr _X) {insert(end(list_Head), _X); }
void pop_back(_Nodeptr list_Head, set_node_fun_t Freenode) {erase(_Dec(end(list_Head)), Freenode); }
void assign(_Nodeptr list_Head, _Nodeptr _F, _Nodeptr _L, set_node_fun_t Freenode)
{
  erase(begin(list_Head), end(list_Head), Freenode);
  insert(begin(list_Head), _F, _L);
}
void assign(_Nodeptr list_Head, size_t _N,  _Nodeptr _X, set_node_fun_t Freenode)
{
  erase(begin(list_Head), end(list_Head), Freenode);
  insert(begin(list_Head), _N, _X);
}
void resize(_Nodeptr list_Head, size_t _N, _Nodeptr _X, set_node_fun_t Freenode)
{
  int n = size(list_Head);
  if (n < _N) {
    insert(end(list_Head), _N - n, _X);
  }
  else {
    for (; _N < n; --n) {
      pop_back(list_Head, Freenode);
    }
  }
}
void list_Head_assign(_Nodeptr list_Head, _Nodeptr _X_Head, set_node_fun_t Freenode)
{
  if (list_Head != _X_Head) {
    _Nodeptr _F1 = begin(list_Head);
    _Nodeptr _L1 = end(list_Head);
    _Nodeptr _F2 = begin(_X_Head);
    _Nodeptr _L2 = end(_X_Head);
    for (; _F1 != _L1 && _F2 != _L2; Inc(_F1), Inc(_F2)) {
      _Copynode(_F1, _F2);
    }
    erase(_F1, _L1, Freenode);
    insert(_L1, _F2, _L2);
  }
  //return (list_Head);
}
void list_Head_free(_Nodeptr list_Head, set_node_fun_t Freenode)
{
  erase(begin(list_Head), end(list_Head), Freenode);
}
void clear(_Nodeptr list_Head, set_node_fun_t Freenode) {
  erase(begin(list_Head), end(list_Head), Freenode);
}
void _Splice(_Nodeptr _P, _Nodeptr _X_Head, _Nodeptr _F, _Nodeptr _L)
{
  _Acc_Next(_Acc_Prev(_L)) = _P;
  _Acc_Next(_Acc_Prev(_F)) = _L;
  _Acc_Next(_Acc_Prev(_P)) = _F;
  _Nodeptr _S = _Acc_Prev(_P);
  _Acc_Prev(_P) = _Acc_Prev(_L);
  _Acc_Prev(_L) = _Acc_Prev(_F);
  _Acc_Prev(_F) = _S;
}
void _Xran() 
{}

void splice(_Nodeptr _P, _Nodeptr _X_Head)
{
  if (!empty(_X_Head)) {
    _Splice(_P, _X_Head, begin(_X_Head), end(_X_Head));
    //_Size += _X->_Size;
    //_X->_Size = 0;
  }
}
void splice(_Nodeptr _P, _Nodeptr _X_Head, _Nodeptr _F)
{
  _Nodeptr _L = _F;
  if (_P != _F && _P != Inc(_L)) {
    _Splice(_P, _X_Head, _F, _L);
    //++_Size;
    //--_X->_Size;
  }
}
void splice(_Nodeptr _P, _Nodeptr _X_Head, _Nodeptr _F, _Nodeptr _L)
{
  if (_F != _L) {
    _Splice(_P, _X_Head, _F, _L);
  }
}

void remove_if(_Nodeptr list_Head, _Nodeptr _V, cmp_node_fun_t cmp, set_node_fun_t Freenode)
{
  _Nodeptr _L = end(list_Head);
  for (_Nodeptr _F = begin(list_Head); _F != _L; Inc(_F)) {
    if (0==cmp(_Acc_Value(_F), _V)) {
      erase(_F, Freenode);
    }
  }
}
void unique(_Nodeptr list_Head, cmp_node_fun_t cmp, set_node_fun_t Freenode) {
  _Nodeptr _F = begin(list_Head), _L = end(list_Head);
  for (_Nodeptr _M = _F; _M != _L; _M = _F) {
    _M = _Acc_Next(_M);
    if (0==cmp(_Acc_Value(_F), _Acc_Value(_M))) {
      erase(_M, Freenode);
    } else {
      _F = _M;
    }
  }
}

void merge(_Nodeptr list_Head, _Nodeptr _X_Head, cmp_node_fun_t cmp)
{
  if (_X_Head != list_Head) {
    _Nodeptr _F1 = begin(list_Head), _L1 = end(list_Head);
    _Nodeptr _F2 = begin(_X_Head), _L2 = end(_X_Head);
    while (_F1 != _L1 && _F2 != _L2) {
      if (cmp(_Acc_Value(_F2), _Acc_Value(_F1)) < 0) {
        _Nodeptr _Mid2 = _F2;
        _Splice(_F1, _X_Head, _F2, Inc(_Mid2));
        _F2 = _Mid2;
      }
      else {
        Inc(_F1);
      }
    }
    if (_F2 != _L2) {
      _Splice(_L1, _X_Head, _F2, _L2);
    }
    //list_Head->_Size += _X->_Size;
    //_X->_Size = 0;
  }
}
void swap(_Nodeptr list_Head, _Nodeptr _X)
{
  T_SWAP(DList, *list_Head, *_X);
}
void sort(_Nodeptr list_Head, cmp_node_fun_t _Pr)
{
  if (2 <= size(list_Head)) {
    const size_t _MAXN = 15;
    DList _X[1] = { 0 }, node_alloc_fun_t[_MAXN + 1] = { 0 };
    size_t _N = 0;
    init(_X);
    for (size_t i = 0; i < _MAXN+1; ++i) {
      init(node_alloc_fun_t + i);
    }
    while (!empty(list_Head)) {
      splice(begin(_X), list_Head, begin(list_Head));
      size_t _I;
      for (_I = 0; _I < _N && !empty(&node_alloc_fun_t[_I]); ++_I) {
        merge(&node_alloc_fun_t[_I], _X, _Pr);
        swap(&node_alloc_fun_t[_I], _X);
      }
      if (_I == _MAXN) {
        merge(&node_alloc_fun_t[_I], _X, _Pr);
      }
      else {
        swap(&node_alloc_fun_t[_I], _X);
        if (_I == _N) {
          ++_N;
        }
      }
    }
    while (0 < _N) {
      merge(list_Head, &node_alloc_fun_t[--_N], _Pr);
    }
  }
}
void reverse(_Nodeptr list_Head)
{
  if (2 <= size(list_Head)) {
    _Nodeptr _L = end(list_Head);
    _Nodeptr _F = begin(list_Head);
    for (Inc(_F); _F != _L; ) {
      _Nodeptr _M = _F;
      _Splice(begin(list_Head), list_Head, _M, Inc(_F));
    }
  }
}

#define offsetof(TYPE, MEMBER) ((unsigned long)(&(((TYPE *)0)->MEMBER)))
#define type_ptr(TYPE, MEMBER, mbptr)  (TYPE*)((char*)(mbptr) - offsetof(TYPE, MEMBER))

struct test_node {
  char str[10];
  DList list;
  int i;
};

DList* new_test_node(int i, const char* str) {
  test_node* ptr = (test_node*)malloc(sizeof(test_node));
  memset(ptr, 0, sizeof(test_node));
  ptr->i = i;
  ptr->list.ttt = 123;
  strcpy(ptr->str, str);
  return &ptr->list;
}

void set_test_node(DList** a1, const DList* b) {
  if (b) {
    test_node* pb = type_ptr(test_node, list, b);
    test_node* pa;
    if (*a1 == NULL) {
      pa = (test_node*)malloc(sizeof(test_node));
      memset(pa, 0, sizeof(test_node));
      *a1 = &pa->list;
    }
    else {
      pa = type_ptr(test_node, list, *a1);
    }
    pa->i = pb->i;
    strcpy(pa->str, pb->str);
  }
  else {
    test_node* pa = type_ptr(test_node, list, *a1);
    free(pa);
    *a1 = NULL;
  }
  return ;
}
int cmp_test_node(const DList* a, const DList* b) {
  test_node* pa = type_ptr(test_node, list, a);
  test_node* pb = type_ptr(test_node, list, b);
  return CMP(pa->i, pa->i);
}
#include <string>
using namespace std;
int aaa(int a) throw() {
  throw new int(a);
  return 0;
}
union ttt{
  double d;
  __int64 i64;
};
int test_nan() {
  double aa = sqrt(-1);
  double bb = sqrt(-1);
  bool cc = *(__int64*)&aa == *(__int64*)&bb;
  bool dd = sqrt(-1) == sqrt(-1);
  ttt t;
  t.i64 = 0xfff8000000000000U;
  bool ee = t.d == t.d;
  bool ff = t.i64 == t.i64;
  return 0;
}
int test_list() {
  for (int i = 0; i < 1000; ++i) {
    try {
      aaa(i);
      break;
    }
    catch (int* a) {
      printf("adf %d\n", *a);
      delete a;
    }
  }
  DList dlist[1] = { dlist, dlist };
  int i, n=2;
  init(dlist);
  for (i = 0; i < n; ++i) {
    push_back(dlist, new_test_node(i, "asdf"));
  }
  int n1 = size(dlist);
  assert(n1 == n);
  if (1) {
    DList* p = begin(dlist),* e = end(dlist);
    for (i = 0; p != e; Inc(p), ++i) {
      test_node* t = type_ptr(test_node, list, p);
      assert(t->i == i);
    }
  }
  if (0) {
    reverse(dlist);
    DList* p = begin(dlist), *e = end(dlist);
    for (i = n-1; p != e; Inc(p), --i) {
      test_node* t = type_ptr(test_node, list, p);
      printf("%d\n", t->i);
      assert(t->i == i);
    }
  }
  if (1) {
    sort(dlist, cmp_test_node);
    DList* p = begin(dlist), *e = end(dlist);
    for (i = 0; p != e; Inc(p), ++i) {
      test_node* t = type_ptr(test_node, list, p);
      printf("%d\n", t->i);
      assert(t->i == i);
    }
  }
  clear(dlist, set_test_node);
  return 0;
}



#undef _Acc_Next
#undef _Acc_Prev
#undef _Acc_Value
