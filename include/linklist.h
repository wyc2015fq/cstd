#ifndef _LIST_H_
#define _LIST_H_
#include <stdlib.h>
#include <stdio.h>
#define EQUL(_A, _B)  ((_A) == (_B))
#define LESS(_A, _B)  ((_A) <  (_B))
#ifndef _FARQ
#define _FARQ
#endif
#ifndef _POINTER_X
#define _POINTER_X(T, A) T _FARQ *
#define _REFERENCE_X(T, A) T _FARQ &
#endif
#define _SIZT size_t
#ifdef __cplusplus
template <typename T>
inline T* new_malloc(T* p)
{
  return new T;
}
#define _Charalloc(_P) _P = new_malloc(_P)
#define _Freenode(_P) delete (_P)
#else
#define _Charalloc(_P) (_P = malloc(sizeof(*(_P))))
#define _Freenode(_P) free(_P)
#endif
#define construct(_P, _X)  (*(_P) = (_X))
#define destroy(_P)        {}
#define _NODE(_Ty, NAME)                                                      \
  struct NAME##_Node {                                                          \
    struct NAME##_Node* _Next;                                                  \
    struct NAME##_Node* _Prev;                                                  \
    _Ty _Value;                                                                 \
  }                                                                             \
#define _LIST(_Ty, NAME)                                                      \
  struct {                                                                      \
    _NODE(_Ty, NAME) * _Head;                                                   \
    size_t _Size;                                                               \
  }
#define _Acc_Next(_P)  ((_P)->_Next)
#define _Acc_Prev(_P)  ((_P)->_Prev)
#define _Acc_Value(_P) ((_P)->_Value)
#define list_inc(_P)  ((_P) = _Acc_Next(_P))
#define list_begin(list) (_Acc_Next((list)._Head))
#define list_end(list)   ((list)._Head)
#define list_head(list)  ((list)._Head)
#define list_size(list)  ((list)._Size)
#define list_push_back(list, _X)    list_insert(list, list_end(list), _X)
#define list_push_front(list, _X)   list_insert(list, list_begin(list), _X)
#define list_pop_back(list, _X)     list_erase(list, _Acc_Prev(list_end(list)))
#define list_pop_front(list)        list_erase(list, list_begin(list))
#define list_pop_front_to(list, _X)                                           \
  do {                                                                          \
    _X = _Acc_Value(_Acc_Next(list_begin(list)));                               \
    list_pop_front(list);                                                       \
  } while(0);
#define list_front(list)  (_Acc_Value(list_begin(list)))
#define list_back(list)   (_Acc_Value(_Acc_Prev(list_end(list))))
#define list_foreach(list, _Pr)                                               \
  do {                                                                          \
    typedef _NODE(int, tmp)* _Nodeptr;                                          \
    _Nodeptr _F = (_Nodeptr)list_begin(list);                                   \
    _Nodeptr _L = (_Nodeptr)list_end(list);                                     \
    for (; _F != _L; list_inc(_F)) {                                            \
      _Pr(_Acc_Value(_F));                                                      \
    }                                                                           \
  } while(0)
#define SIZEOF_NODE(TYPE) (sizeof(int*)*2+sizeof(TYPE))
#define list_Buynode(Narg, Parg, _P)                                          \
  do {                                                                          \
    typedef _NODE(int, tmp)* _Nodeptr;                                          \
    _Nodeptr _S = (_Nodeptr)NULL;                                               \
    _Nodeptr _Narg = (_Nodeptr)(Narg);                                          \
    _Nodeptr _Parg = (_Nodeptr)(Parg);                                          \
    _Charalloc(_P);                                                             \
    _S = (_Nodeptr)_P;                                                          \
    _Acc_Next(_S) = (_Narg) != 0 ? (_Narg) : (_S);                              \
    _Acc_Prev(_S) = (_Parg) != 0 ? (_Parg) : (_S);                              \
  } while(0)
#define list_new(list)                                                        \
  do {                                                                          \
    list_Buynode(0, 0, (list)._Head);                                           \
    (list)._Size = (0);                                                         \
  } while(0)
#define list_insert(list, _S, _X)                                             \
  do {                                                                          \
    list_Buynode(_S, _Acc_Prev(_S), _Acc_Prev(_S));                             \
    _Acc_Next(_Acc_Prev(_Acc_Prev(_S))) = _Acc_Prev(_S);                        \
    construct(&_Acc_Value(_Acc_Prev(_S)), _X);                                  \
    ++list_size(list);                                                          \
  } while(0)
#define list_insert2(list, _P, _F0, _L0)                                      \
  do {                                                                          \
    typedef _NODE(int, tmp)* _Nodeptr;                                          \
    _Nodeptr _F=(_Nodeptr)_F0, _L=(_Nodeptr)_L0;                                \
    for (; _F != _L; list_inc(_F))                                              \
      list_insert(list, _P, _Acc_Value(_F));                                    \
  } while(0)
    // list_erase(aa, list_begin(aa))
#define list_erase(list, _P)                                                  \
  do {                                                                          \
    typedef _NODE(int, tmp)* _Nodeptr;                                          \
    _Nodeptr _S = (_Nodeptr)(_P);                                               \
    _Acc_Next(_Acc_Prev(_S)) = _Acc_Next(_S);                                   \
    _Acc_Prev(_Acc_Next(_S)) = _Acc_Prev(_S);                                   \
    destroy(&_Acc_Value(_S));                                                   \
    _Freenode(_S);                                                              \
    --list_size(aa);                                                            \
  } while(0)
#define list_erase2(list, _F0, _L0)                                           \
  do {                                                                          \
    typedef _NODE(int, tmp)* _Nodeptr;                                          \
    _Nodeptr _F=(_Nodeptr)_F0, _L=(_Nodeptr)_L0, _FF;                           \
    for (; _F != _L; _F = _FF) {                                                \
      _FF = _Acc_Next(_F);                                                      \
      list_erase(list, _F);                                                     \
    }                                                                           \
  } while(0)
#define list_clear(list)                                                      \
  do {                                                                          \
    for (; list_begin(list) != list_end(list); )                                \
      list_erase(list, list_begin(list));                                       \
  } while(0)
#define list_free(list)                                                       \
  do {                                                                          \
    list_clear(list);                                                           \
    _Freenode(list_head(list));                                                 \
  } while(0)
#define list_remove(_Ty, list, _V)                                            \
  do {                                                                          \
    typedef _NODE(_Ty, tmp)* _Nodeptr;                                          \
    _Nodeptr _FF, _F = (_Nodeptr)list_begin(aa);                                \
    _Nodeptr _L = (_Nodeptr)list_end(aa);                                       \
    for (; _F != _L; _F=_FF)  {                                                 \
      _FF = _Acc_Next(_F);                                                      \
      if ((_V) == _Acc_Value(_F))                                               \
        list_erase(aa, _F);                                                     \
    }                                                                           \
  } while(0)
#define list_remove_if(_Ty, list, _Pr)                                        \
  do {                                                                          \
    typedef _NODE(_Ty, tmp)* _Nodeptr;                                          \
    _Nodeptr _M, _F = (_Nodeptr)list_begin(aa);                                 \
    _Nodeptr _L = (_Nodeptr)list_end(aa);                                       \
    for (; _F != _L; _F=_M)  {                                                  \
      _M = _Acc_Next(_F);                                                       \
      if (_Pr(_Acc_Value(_F)))                                                  \
        list_erase(aa, _F);                                                     \
    }                                                                           \
  } while(0)
#define list_unique_if(_Ty, list, _Pr)                                        \
  do {                                                                          \
    typedef _NODE(_Ty, tmp)* _Nodeptr;                                          \
    _Nodeptr _M, _F = (_Nodeptr)list_begin(aa);                                 \
    _Nodeptr _L = (_Nodeptr)list_end(aa);                                       \
    if (_F != _L)                                                               \
      for (_M = _F; list_inc(_M) != _L; _M = _F)                                \
        if (_Pr(_Acc_Value(_F), _Acc_Value(_M)))                                \
          list_erase(list, _M);                                                 \
        else                                                                    \
          _F = _M;                                                              \
  } while(0)
#define list_unique(_Ty, list) list_unique_if(_Ty, list, EQUL)
#define list_Splice(_Nodeptr, list, _P0, _X, _F0, _L0)                        \
  do {                                                                          \
    if (1) {                                                                    \
      _Nodeptr _S,_P1=(_Nodeptr)_P0, _F1=(_Nodeptr)_F0, _L1=(_Nodeptr)_L0;      \
      _Acc_Next(_Acc_Prev(_L1)) = _P1;                                          \
      _Acc_Next(_Acc_Prev(_F1)) = _L1;                                          \
      _Acc_Next(_Acc_Prev(_P1)) = _F1;                                          \
      _S = _Acc_Prev(_P1);                                                      \
      _Acc_Prev(_P1) = _Acc_Prev(_L1);                                          \
      _Acc_Prev(_L1) = _Acc_Prev(_F1);                                          \
      _Acc_Prev(_F1) = _S;                                                      \
    }                                                                           \
  } while(0)
#define list_merge_if(_Ty, list, _X, _Pr)                                     \
  do {                                                                          \
    typedef _NODE(_Ty, tmp)* _Nodeptr;                                          \
    if ((void*)&(_X) != (void*)&(list)) {                                       \
      _Nodeptr _F1 = (_Nodeptr)list_begin(list), _L1 = (_Nodeptr)list_end(list);\
      _Nodeptr _F2 = (_Nodeptr)list_begin(_X), _L2 = (_Nodeptr)list_end(_X);    \
      while (_F1 != _L1 && _F2 != _L2)                                          \
        if (_Pr(_Acc_Value(_F2), _Acc_Value(_F1))) {                            \
          _Nodeptr _Mid2 = _Acc_Next(_F2);                                      \
          list_Splice(_Nodeptr, list, _F1, _X, _F2, _Mid2);                     \
          _F2 = _Mid2;                                                          \
        } else {                                                                \
          list_inc(_F1);                                                        \
        }                                                                       \
      if (_F2 != _L2)                                                           \
        list_Splice(_Nodeptr, list, _L1, _X, _F2, _L2);                         \
      list._Size += _X._Size;                                                   \
      _X._Size = 0;                                                             \
    }                                                                           \
  } while(0)
#define list_merge(_Ty, list, _X) list_merge_if(_Ty, list, _X, LESS)
#define _SWAP(a, b, T) { T t=(a); a=(b); b=t; }
#define BITSWAP(a, b)                                                         \
  do {                                                                          \
    typedef char* charptr;                                                      \
    charptr _F = (charptr)&(a), _Q = (charptr)&(b);                             \
    charptr _L = _F + sizeof(a);                                                \
    for (; _F!=_L; ++_F, ++_Q)                                                  \
      _SWAP(*_F, *_Q, char);                                                    \
  } while(0)
#define list_swap(list, _X)  BITSWAP((list), (_X))
#define list_empty(list) (0 == list_size(list))
#define list_splice1(_Nodeptr, list, _P0, _X, _F0)                            \
  do {                                                                          \
    _Nodeptr _P = (_Nodeptr)(_P0), _F = (_Nodeptr)(_F0);                        \
    _Nodeptr _L = (_Nodeptr)(_F0);                                              \
    if (_P != _F && _P != list_inc(_L)) {                                       \
      list_Splice(_Nodeptr, list, _P, _X, _F, _L);                              \
      ++list._Size;                                                             \
      --_X._Size;                                                               \
    }                                                                           \
  } while(0)
#define list_sort_if(_Ty, list, _Pr)                                          \
  do {                                                                          \
    typedef _NODE(_Ty, tmp)* _Nodeptr;                                          \
    if (2 <= list_size(list)) {                                                 \
      typedef _LIST(_Ty, tmp) _Myt;                                             \
      enum { _MAXN = 15, };                                                     \
      _Myt _X, _A[_MAXN + 1];                                                   \
      size_t _N = 0, _I;                                                        \
      list_new(_X);                                                             \
      for (_I=0; _I < (_MAXN + 1); ++_I)                                        \
        list_new(_A[_I]);                                                       \
      while (!list_empty(list)) {                                               \
        list_splice1(_Nodeptr, _X, list_begin(_X), list, list_begin(list));     \
        for (_I = 0; _I < _N && !list_empty(_A[_I]); ++_I) {                    \
          list_merge_if(_Ty, _A[_I], _X, _Pr);                                  \
          list_swap(_A[_I], _X);                                                \
        }                                                                       \
        if (_I == _MAXN) {                                                      \
          list_merge_if(_Ty, _A[_I], _X, _Pr);                                  \
        } else {                                                                \
          list_swap(_A[_I], _X);                                                \
          if (_I == _N)                                                         \
            ++_N;                                                               \
        }                                                                       \
      }                                                                         \
      while (0 < _N) { --_N;                                                    \
        list_merge_if(_Ty, list, _A[_N], _Pr);                                  \
      }                                                                         \
    }                                                                           \
  } while(0)
#define list_sort(_Ty, list)  list_sort_if(_Ty, list, LESS)
#define list_reverse(_Ty, list)                                               \
  do {                                                                          \
    typedef _NODE(int, tmp)* _Nodeptr;                                          \
    if (2 <= list_size(list)) {                                                 \
      _Nodeptr _F = (_Nodeptr)list_begin(list), _L = (_Nodeptr)list_end(list);  \
      for (list_inc(_F); _F != _L; ) {                                          \
        _Nodeptr _M = _F, _Y=(_Nodeptr)list_begin(list);                        \
        list_inc(_F);                                                           \
        list_Splice(_Nodeptr, list, _Y, list, _M, _F);                          \
      }                                                                         \
    }                                                                           \
  } while(0)
    #endif // _LIST_H_

