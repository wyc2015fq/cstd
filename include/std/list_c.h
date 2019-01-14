

#include "mempool_c.h"

#define _REFERENCE_X(a, b)  a&
#define _POINTER_X(a, b)    a*
struct _Node;

typedef struct _Node* _Nodeptr;

struct _Node {
  _Nodeptr _Next, _Prev;
};


typedef void (*node_alloc_fun_t)(_Node** pp, void* x);
typedef void (*node_free_fun_t)(_Node** pp);

struct list_t {
  node_alloc_fun_t node_alloc;
  node_free_fun_t node_free;
  _Nodeptr _Head;
  size_t _Size;
};

node_alloc_fun_t node_alloc;
node_free_fun_t node_free;
_Nodeptr _Head;
size_t _Size;
int _OffSet;

typedef _REFERENCE_X(_Nodeptr, node_alloc_fun_t) _Nodepref;
struct _Acc {
  //typedef node_alloc_fun_t::reference _Vref;
  static _Nodepref _Next(_Nodeptr _P)
  {return ((_Nodepref)(*_P)._Next); }
  static _Nodepref _Prev(_Nodeptr _P)
  {return ((_Nodepref)(*_P)._Prev); }
  static void* _Value(_Nodeptr _P)
  {return ((char*)_P + _OffSet); }
};

typedef _Nodeptr iterator;

iterator& begin(list_t* list) {return (iterator(_Acc::_Next(list->_Head))); }
iterator& end(list_t* list) {return (iterator(list->_Head)); }
size_t size(list_t* list) {return (list->_Size); }
bool empty(list_t* list) {return (size(list) == 0); }

_Nodeptr _Buynode(_Nodeptr _Narg = 0, _Nodeptr _Parg = 0, void* x = 0)
{
  _Nodeptr _S = NULL;
  node_alloc(&_S, x);
  _Acc::_Next(_S) = _Narg != 0 ? _Narg : _S;
  _Acc::_Prev(_S) = _Parg != 0 ? _Parg : _S;
  return (_S);
}
void _Freenode(_Nodeptr _S)
{node_free(&_S); }


iterator insert(iterator _P, void* x)
{
  _Nodeptr _S = _P;
  _S->_Prev = _Buynode(_S, _Acc::_Prev(_S), x);
  _S = _Acc::_Prev(_S);
  _Acc::_Next(_Acc::_Prev(_S)) = _S;
  ++_Size;
  return (iterator(_S));
}
void insert(iterator _P, size_t _M,  void* _X)
{
  for (; 0 < _M; --_M) {
    insert(_P, _X);
  }
}

void insert(iterator _P, iterator _F, iterator _L)
{
  for (; _F != _L; ++_F)
    insert(_P, _Acc::_Value(_F));
}

iterator erase(iterator _P)
{
  _Nodeptr _S = (_P);
  _P = _P->_Next;
  _Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
  _Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
  _Freenode(_S);
  --_Size;
  return (_P);
}
iterator erase(iterator _F, iterator _L)
{
  for (; _F != _L; _F = _F->_Next) {
    erase(_F);
  }
  return (_F);
}

iterator push_front(list_t* list, void* _X) {return insert(begin(list), _X); }
iterator pop_front(list_t* list) { return erase(begin(list)); }
void push_back(list_t* list, void* _X) {insert(end(list), _X); }
void pop_back(list_t* list) {erase(--end(list)); }
void assign(list_t* list, iterator _F, iterator _L)
{
  erase(begin(list), end(list));
  insert(begin(list), _F, _L);
}
void assign(list_t* list, size_t _N,  void* _X )
{
  erase(begin(list), end(list));
  insert(begin(list), _N, _X);
}
void resize(list_t* list, size_t _N, void* _X)
{
  if (size(list) < _N) {
    insert(end(list), _N - size(list), _X);
  } else
    while (_N < size(list)) {
      pop_back(list);
    }
}
void list_assign(list_t* list, list_t* _X)
{
  if (list != _X) {
    iterator _F1 = begin(list);
    iterator _L1 = end(list);
    iterator _F2 = begin(_X);
    iterator _L2 = end(_X);
    for (; _F1 != _L1 && _F2 != _L2; ++_F1, ++_F2) {
      *_F1 = *_F2;
    }
    erase(_F1, _L1);
    insert(_L1, _F2, _L2);
  }
  //return (*list);
}
void list_free(list_t* list)
{
  erase(begin(list), end(list));
  _Freenode(_Head);
  _Head = 0, _Size = 0;
}
void clear(list_t* list) {
  erase(begin(list), end(list));
}
void _Splice(iterator _P, iterator _F, iterator _L)
{
  _Acc::_Next(_Acc::_Prev(_L)) = _P;
  _Acc::_Next(_Acc::_Prev(_F)) = _L;
  _Acc::_Next(_Acc::_Prev(_P)) = _F;
  _Nodeptr _S = _Acc::_Prev(_P);
  _Acc::_Prev(_P) = _Acc::_Prev(_L);
  _Acc::_Prev(_L) = _Acc::_Prev(_F);
  _Acc::_Prev(_F) = _S;
  
}
void _Xran() 
{}

void splice(iterator _P, list_t* _X)
{
  if (!empty(_X)) {
    _Splice(_P, begin(_X), end(_X));
    _Size += _X->_Size;
    _X->_Size = 0;
  }
}
void splice(iterator _P, list_t* _X, iterator _F)
{
  iterator _L = _F;
  if (_P != _F && _P != ++_L) {
    _Splice(_P, _F, _L);
    ++_Size;
    --_X->_Size;
  }
}

typedef int(*cmp_fun_t)(const void* key1, const void* key2);

void remove_if(list_t* list, void* _V, cmp_fun_t cmp)
{
  iterator _L = end(list);
  for (iterator _F = begin(list); _F != _L; ) {
    if (0==cmp(_Acc::_Value(_F), _V)) {
      erase(_F++);
    } else {
      ++_F;
    }
  }
}
void unique(list_t* list, cmp_fun_t cmp) {
  iterator _F = begin(list), _L = end(list);
  for (iterator _M = _F; _M != _L; _M = _F) {
    _M = _Acc::_Next(_M);
    if (0==cmp(_Acc::_Value(_F), _Acc::_Value(_M))) {
      erase(_M);
    } else {
      _F = _M;
    }
  }
  
}

void merge(list_t* list, list_t* _X, cmp_fun_t cmp)
{
  if (_X != list) {
    iterator _F1 = begin(list), _L1 = end(list);
    iterator _F2 = begin(_X), _L2 = end(_X);
    while (_F1 != _L1 && _F2 != _L2)
      if (0 < cmp(_Acc::_Value(_F2), _Acc::_Value(_F1))) {
        iterator _Mid2 = _F2;
        _Splice(_F1, _F2, ++_Mid2);
        _F2 = _Mid2;
      } else {
        ++_F1;
      }
    if (_F2 != _L2) {
      _Splice(_L1, _F2, _L2);
    }
    _Size += _X->_Size;
    _X->_Size = 0;
  }
}


#if 0
void sort(list_t* list)
{
  if (2 <= size(list)) {
     size_t _MAXN = 15;
    list_t _X(allocator), _A[_MAXN + 1];
    size_t _N = 0;
    while (!empty()) {
      splice(begin(_X), list, begin(list));
      size_t _I;
      for (_I = 0; _I < _N && !node_alloc_fun_t[_I].empty(); ++_I) {
        node_alloc_fun_t[_I].merge(_X);
        node_alloc_fun_t[_I].swap(_X);
      }
      if (_I == _MAXN) {
        node_alloc_fun_t[_I].merge(_X);
      } else {
        node_alloc_fun_t[_I].swap(_X);
        if (_I == _N) {
          ++_N;
        }
      }
    }
    while (0 < _N) {
      merge(node_alloc_fun_t[--_N]);
    }
  }
}
void sort(_Pr3 _Pr)
{
  if (2 <= size(list)) {
     size_t _MAXN = 15;
    list_t _X(allocator), node_alloc_fun_t[_MAXN + 1];
    size_t _N = 0;
    while (!empty()) {
      _X->splice(begin(_X), *list, begin(list));
      size_t _I;
      for (_I = 0; _I < _N && !node_alloc_fun_t[_I].empty(); ++_I) {
        node_alloc_fun_t[_I].merge(_X, _Pr);
        node_alloc_fun_t[_I].swap(_X);
      }
      if (_I == _MAXN) {
        node_alloc_fun_t[_I].merge(_X, _Pr);
      } else {
        node_alloc_fun_t[_I].swap(_X);
        if (_I == _N) {
          ++_N;
        }
      }
    }
    while (0 < _N) {
      merge(node_alloc_fun_t[--_N], _Pr);
    }
  }
}
void reverse()
{
  if (2 <= size(list)) {
    iterator _L = end(list);
    for (iterator _F = ++begin(list); _F != _L; ) {
      iterator _M = _F;
      _Splice(begin(list), *list, _M, ++_F);
    }
  }
}

#endif
