// list standard header

#define _POINTER_X(a, b)    a*
#define _REFERENCE_X(a, b)  a&

#include "types_c.h"
#include <limits.h>

typedef int _Ty;
struct ListNode;
typedef _POINTER_X(ListNode, _A) ListNodeptr;
struct ListNode {
  ListNodeptr _Next, _Prev;
  _Ty _Value;
};

typedef _REFERENCE_X(ListNodeptr, _A) ListNodepref;
typedef _REFERENCE_X(ListNode, _A) _Vref;

typedef void(*ListNodeConstruct)(ListNodepref _P, const ListNodeptr _X);
typedef void(*ListNodeDestroy)(ListNodepref _P);
typedef int (ListNodePr1)(const ListNodeptr _F);
typedef int (ListNodeCmp)(const ListNodeptr _F, const ListNodeptr _M);

#define _Acc_Next(_P)   (_P)->_Next
#define _Acc_Prev(_P)   (_P)->_Prev

ListNodeptr & _Inc(ListNodeptr & _Ptr)
{
  _Ptr = _Acc_Next(_Ptr);
  return (_Ptr);
}
ListNodeptr Inc_(ListNodeptr & _Ptr)
{
  ListNodeptr _Tmp = _Ptr;
  _Ptr = _Acc_Next(_Ptr);
  return (_Tmp);
}
ListNodeptr & _Dec(ListNodeptr & _Ptr)
{
  _Ptr = _Acc_Prev(_Ptr);
  return (_Ptr);
}
ListNodeptr Dec_(ListNodeptr & _Ptr)
{
  ListNodeptr _Tmp = _Ptr;
  _Ptr = _Acc_Prev(_Ptr);
  return (_Tmp);
}

typedef size_t size_type;
typedef int difference_type;

struct list {
  ListNodeptr _Head;
  size_type _Size;
};
ListNodeptr _BuyHeadnode() {
  ListNodeptr _S = (ListNodeptr)malloc(sizeof(ListNode));
  _Acc_Next(_S) = _S;
  _Acc_Prev(_S) = _S;
  return (_S);
}
ListNodeptr _Buynode(ListNodeptr _Narg, ListNodeptr _Parg, const ListNodeptr _X, ListNodeConstruct construct) {
  ListNodeptr _S = NULL;
  construct(_S, _X);
  //assert(_Narg);
  //assert(_Parg);
  //_Acc_Prev(_S) = _Parg != 0 ? _Parg : _S;
  _Acc_Next(_S) = _Narg;
  _Acc_Prev(_S) = _Parg;
  return (_S);
}
void _Copy(ListNodepref _F1, const ListNodeptr _F2, ListNodeConstruct construct) {
  construct(_F1, _F2);
}
void init(list* li) {
  li->_Head = (_BuyHeadnode());
  li->_Size = (0);
}
ListNodeptr begin(list* li) { return (_Acc_Next(li->_Head)); }
ListNodeptr begin(const list* li) { return (_Acc_Next(li->_Head)); }
ListNodeptr end(list* li) { return (li->_Head); }
ListNodeptr end(const list* li) { return (li->_Head); }

size_type size(const list* li)
{
  return (li->_Size);
}
bool empty(const list* li)
{
  return (size(li) == 0);
}
ListNodeptr insert(list* li, ListNodeptr _P, const ListNodeptr _X, ListNodeConstruct construct) {
  ListNodeptr _S = _P;
  _Acc_Prev(_S) = _Buynode(_S, _Acc_Prev(_S), _X, construct);
  _S = _Acc_Prev(_S);
  _Acc_Next(_Acc_Prev(_S)) = _S;
  //construct(&_Acc_Value(_S), _X);
  ++li->_Size;
  return (ListNodeptr(_S));
}
void insert(list* li, ListNodeptr _P, size_type _M, ListNodeptr _X, ListNodeConstruct construct) {
  for (; 0 < _M; --_M) {
    insert(li, _P, _X, construct);
  }
}
void insert(list* li, ListNodeptr _P, ListNodeptr _F, ListNodeptr _L, ListNodeConstruct construct) {
  for (; _F != _L; _Inc(_F)) {
    insert(li, _P, _F, construct);
  }
}
ListNodeptr erase(list* li, ListNodeptr _P, ListNodeDestroy destroy) {
  ListNodeptr _S = Inc_(_P);
  _Acc_Next(_Acc_Prev(_S)) = _Acc_Next(_S);
  _Acc_Prev(_Acc_Next(_S)) = _Acc_Prev(_S);
  destroy(_S);
  --li->_Size;
  return (_P);
}
ListNodeptr erase(list* li, ListNodeptr _F, ListNodeptr _L, ListNodeDestroy destroy) {
  while (_F != _L) {
    erase(li, Inc_(_F), destroy);
  }
  return (_F);
}

void assign(list* li, ListNodeptr _F, ListNodeptr _L, ListNodeConstruct construct, ListNodeDestroy destroy) {
  erase(li, begin(li), end(li), destroy);
  insert(li, begin(li), _F, _L, construct);
}
void assign(list* li, size_type _N, ListNodeptr _X, ListNodeConstruct construct, ListNodeDestroy destroy) {
  erase(li, begin(li), end(li), destroy);
  insert(li, begin(li), _N, _X, construct);
}
void push_front(list* li, ListNodeptr _X, ListNodeConstruct construct)
{
  insert(li, begin(li), _X, construct);
}
void pop_front(list* li, ListNodeDestroy destroy)
{
  erase(li, begin(li), destroy);
}
void push_back(list* li, const ListNodeptr _X, ListNodeConstruct construct)
{
  insert(li, end(li), _X, construct);
}
void pop_back(list* li, ListNodeDestroy destroy) {
  ListNodeptr p = end(li);
  erase(li, _Dec(p), destroy);
}
void copy(list* li, const list* _X, ListNodeConstruct construct, ListNodeDestroy destroy) {
  if (li != _X) {
    ListNodeptr _F1 = begin(li);
    ListNodeptr _L1 = end(li);
    ListNodeptr _F2 = begin(_X);
    ListNodeptr _L2 = end(_X);
    for (; _F1 != _L1 && _F2 != _L2; _Inc(_F1), _Inc(_F2)) {
      _Copy(_F1, _F2, construct);
    }
    erase(li, _F1, _L1, destroy);
    insert(li, _L1, _F2, _L2, construct);
  }
  return;
}
void clear(list* li, ListNodeDestroy destroy)
{
  erase(li, begin(li), end(li), destroy);
  li->_Head = 0, li->_Size = 0;
}
void resize(list* li, size_type _N, const ListNodeptr _X, ListNodeConstruct construct, ListNodeDestroy destroy) {
  if (size(li) < _N) {
    insert(li, end(li), _N - size(li), _X, construct);
  }
  else {
    while (_N < size(li)) {
      pop_back(li, destroy);
    }
  }
}
void swap(list* li, list* _X) {
  //T_SWAP(ListNode, *head, *_X->head);
  T_SWAP(ListNodeptr, li->_Head, _X->_Head);
  T_SWAP(size_t, li->_Size, _X->_Size);
}
void _Splice(ListNodeptr _P, list* _X, ListNodeptr _F, ListNodeptr _L) {
  _Acc_Next(_Acc_Prev(_L)) = _P;
  _Acc_Next(_Acc_Prev(_F)) = _L;
  _Acc_Next(_Acc_Prev(_P)) = _F;
  ListNodeptr _S = _Acc_Prev(_P);
  _Acc_Prev(_P) = _Acc_Prev(_L);
  _Acc_Prev(_L) = _Acc_Prev(_F);
  _Acc_Prev(_F) = _S;
}
void splice(list* li, ListNodeptr _P, list* _X, ListNodeptr _F) {
  ListNodeptr _L = _F;
  if (_P != _F && _P != _Inc(_L)) {
    _Splice(_P, _X, _F, _L);
    ++li->_Size;
    --_X->_Size;
  }
}
void splice(list* li, ListNodeptr _P, list* _X) {
  if (!empty(_X)) {
    _Splice(_P, _X, begin(_X), end(_X));
    li->_Size += _X->_Size;
    _X->_Size = 0;
  }
}
void _Distance(ListNodeptr _F, ListNodeptr _L, size_t & _N)
{
  for (; _F != _L; _Inc(_F)) { ++_N; }
}
void splice(list* li, ListNodeptr _P, list* _X, ListNodeptr _F, ListNodeptr _L) {
  if (_F != _L) {
    if (_X != li) {
      size_t _N = 0;
      _Distance(_F, _L, _N);
      li->_Size += _N;
      _X->_Size -= _N;
    }
    _Splice(_P, _X, _F, _L);
  }
}
void swap(list* li, list* _X, ListNodeConstruct construct) {
  ListNodeptr _P = begin(li);
  splice(li, _P, _X);
  splice(_X, begin(_X), li, _P, end(li));
}
void remove_if(list* li, ListNodePr1 _Pr, ListNodeDestroy destroy) {
  ListNodeptr _L = end(li);
  for (ListNodeptr _F = begin(li); _F != _L; )
    if (_Pr(_F)) {
      erase(li, Inc_(_F), destroy);
    }
    else {
      _Inc(_F);
    }
}
void unique(list* li, ListNodeDestroy destroy) {
  ListNodeptr _F = begin(li), _L = end(li);
  if (_F != _L)
    for (ListNodeptr _M = _F; _Inc(_M) != _L; _M = _F)
      if (_F->_Value == _M->_Value) {
        erase(li, _M, destroy);
      }
      else {
        _F = _M;
      }
}

void unique(list* li, ListNodeCmp _Pr, ListNodeDestroy destroy) {
  ListNodeptr _F = begin(li), _L = end(li);
  if (_F != _L)
    for (ListNodeptr _M = _F; _Inc(_M) != _L; _M = _F)
      if (0 == _Pr(_F, _M)) {
        erase(li, _M, destroy);
      }
      else {
        _F = _M;
      }
}
void merge(list* li, list* _X) {
  if (_X != li) {
    ListNodeptr _F1 = begin(li), _L1 = end(li);
    ListNodeptr _F2 = begin(_X), _L2 = end(_X);
    while (_F1 != _L1 && _F2 != _L2)
      if (_F2->_Value < _F1->_Value) {
        ListNodeptr _Mid2 = _F2;
        _Splice(_F1, _X, _F2, _Inc(_Mid2));
        _F2 = _Mid2;
      }
      else {
        _Inc(_F1);
      }
      if (_F2 != _L2) {
        _Splice(_L1, _X, _F2, _L2);
      }
      li->_Size += _X->_Size;
      _X->_Size = 0;
  }
}
void merge(list* li, list* _X, ListNodeCmp _Pr) {
  if (_X != li) {
    ListNodeptr _F1 = begin(li), _L1 = end(li);
    ListNodeptr _F2 = begin(_X), _L2 = end(_X);
    while (_F1 != _L1 && _F2 != _L2)
      if (_Pr(_F2, _F1)) {
        ListNodeptr _Mid2 = _F2;
        _Splice(_F1, _X, _F2, _Inc(_Mid2));
        _F2 = _Mid2;
      }
      else {
        _Inc(_F1);
      }
      if (_F2 != _L2) {
        _Splice(_L1, _X, _F2, _L2);
      }
      li->_Size += _X->_Size;
      _X->_Size = 0;
  }
}
void sort(list* li) {
  if (2 <= size(li)) {
    const size_t _MAXN = 15;
    list _X[1] = { 0 }, _A[_MAXN + 1] = { 0 };
    size_t _N = 0;
    init(_X);
    for (size_t _I = 0; _I <= (_MAXN); ++_I) {
      init(_A + _I);
    }
    while (!empty(li)) {
      splice(_X, begin(_X), li, begin(li));
      size_t _I;
      for (_I = 0; _I < _N && !empty(_A + _I); ++_I) {
        merge(_A + _I, _X);
        swap(_A + _I, _X);
      }
      if (_I == _MAXN) {
        merge(_A + _I, _X);
      }
      else {
        swap(_A + _I, _X);
        if (_I == _N) {
          ++_N;
        }
      }
    }
    while (0 < _N) {
      merge(li, &_A[--_N]);
    }
    clear(_X, NULL);
    for (size_t _I = 0; _I <= (_MAXN); ++_I) {
      clear(_A + _I, NULL);
    }
  }
}
void sort(list* li, ListNodeCmp _Pr) {
  if (2 <= size(li)) {
    const size_t _MAXN = 15;
    list _X[1] = { 0 }, _A[_MAXN + 1] = { 0 };
    size_t _N = 0;
    init(_X);
    for (size_t _I = 0; _I <= (_MAXN); ++_I) {
      init(_A + _I);
    }
    while (!empty(li)) {
      splice(_X, begin(_X), li, begin(li));
      size_t _I;
      for (_I = 0; _I < _N && !empty(_A + _I); ++_I) {
        merge(_A + _I, _X, _Pr);
        swap(_A + _I, _X);
      }
      if (_I == _MAXN) {
        merge(_A + _I, _X, _Pr);
      }
      else {
        swap(_A + _I, _X);
        if (_I == _N) {
          ++_N;
        }
      }
    }
    while (0 < _N) {
      merge(li, &_A[--_N], _Pr);
    }
    clear(_X, NULL);
    for (size_t _I = 0; _I <= (_MAXN); ++_I) {
      clear(_A + _I, NULL);
    }
  }
}
void reverse(list* li) {
  if (2 <= size(li)) {
    ListNodeptr _L = end(li);
    ListNodeptr _F = begin(li);
    for (_Inc(_F); _F != _L; ) {
      ListNodeptr _M = _F;
      _Splice(begin(li), li, _M, _Inc(_F));
    }
  }
}
void _Splice(list* li, ListNodeptr _P, list* _X, ListNodeptr _F, ListNodeptr _L, ListNodeConstruct construct, ListNodeDestroy destroy) {
  insert(li, _P, _F, _L, construct);
  erase(_X, _F, _L, destroy);
}

bool equal(ListNodeptr _F, ListNodeptr _L, ListNodeptr _P, ListNodeCmp cmp) {
  for (; _F != _L; _Inc(_F), _Inc(_P)) {
    if (cmp(_F, _P)) return false;
  }
  return true;
}
// list TEMPLATE OPERATORS

bool equal(const list* _X, const list* _Y, ListNodeCmp cmp)
{
  return (size(_X) == size(_Y) && equal(begin(_X), end(_X), begin(_Y), cmp));
}
// (lexicographical_compare(begin(_X), end(_X), _Y.begin(li), _Y.end(li)));

struct test_list_t {
  int i;
  ListNode list[1];
  char str[10];
};
void test_list_construct(ListNodepref _P, const ListNodeptr _X) {
  test_list_t* a = _P ? container_of(_P, test_list_t, list) : NULL;
  a = (test_list_t*)realloc(a, sizeof(test_list_t));
  _P = a->list;
  test_list_t* b = container_of(_X, test_list_t, list);
  a->i = b->i;
  _P->_Value = _X->_Value;
  strcpy(a->str, b->str);
}
void test_list_destroy(ListNodepref _P) {
  if (_P) {
    test_list_t* a = container_of(_P, test_list_t, list);
    free(a);
    _P = NULL;
  }
}
int test_list_cmp(const ListNodeptr a, const ListNodeptr b) {
  test_list_t* pa = container_of(a, test_list_t, list);
  test_list_t* pb = container_of(b, test_list_t, list);
  return pa->i < pb->i;
}

int test_list()
{
  if (1) {
    list li[1] = { 0 };
    int i, n = 100;
    test_list_t t[1] = { 0 };
    init(li);
    for (i = 0; i < n; ++i) {
      t->i = i;
      t->list->_Value = i;
      _snprintf(t->str, 10, "%d", i);
      push_back(li, t->list, test_list_construct);
    }
    if (1) {
      ListNodeptr b = begin(li);
      ListNodeptr e = end(li);
      for (i = 0; b != e; _Inc(b), ++i) {
        ASSERT(b->_Value == i);
      }
    }
    reverse(li);
    if (1) {
      ListNodeptr b = begin(li);
      ListNodeptr e = end(li);
      for (i = n - 1; b != e; _Inc(b), --i) {
        ASSERT(b->_Value == i);
      }
    }
    sort(li, test_list_cmp);
    if (1) {
      ListNodeptr b = begin(li);
      ListNodeptr e = end(li);
      for (i = 0; b != e; _Inc(b), ++i) {
        ASSERT(b->_Value == i);
      }
    }
    clear(li, test_list_destroy);
  }
  return 0;
}

