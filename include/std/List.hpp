// list standard header

#define _POINTER_X(a, b)  a*
#define _REFERENCE_X(a, b)  a&

#include "types_c.h"
#include <limits.h>

typedef int _Ty;
struct _Node;
typedef _POINTER_X(_Node, _A) _Nodeptr;
struct _Node {
  _Nodeptr _Next, _Prev;
  _Ty _Value;
};

typedef _REFERENCE_X(_Nodeptr, _A) _Nodepref;
typedef _REFERENCE_X(_Node, _A) _Vref;

struct Allocator {
  virtual size_t max_size() const {    return UINT_MAX;  }
  virtual void construct(_Nodepref _P, const _Nodeptr _X) = 0;
  virtual void destroy(_Nodepref _P) = 0;
};

struct _Acc {
  static _Nodepref _Next(_Nodeptr _P)
  {return ((_Nodepref)(*_P)._Next); }
  static _Nodepref _Prev(_Nodeptr _P)
  {return ((_Nodepref)(*_P)._Prev); }
  //static _Nodepref _Value(_Nodeptr _P)  {return _P; }
};

_Nodeptr & _Inc(_Nodeptr & _Ptr)
{
  _Ptr = _Acc::_Next(_Ptr);
  return (_Ptr);
}
_Nodeptr Inc_(_Nodeptr & _Ptr)
{
  _Nodeptr _Tmp = _Ptr;
  _Ptr = _Acc::_Next(_Ptr);
  return (_Tmp);
}
_Nodeptr & _Dec(_Nodeptr & _Ptr)
{
  _Ptr = _Acc::_Prev(_Ptr);
  return (_Ptr);
}
_Nodeptr Dec_(_Nodeptr & _Ptr)
{
  _Nodeptr _Tmp = _Ptr;
  _Ptr = _Acc::_Prev(_Ptr);
  return (_Tmp);
}

struct list {
  typedef size_t size_type;
public:
  typedef Allocator* _A;
  typedef list _Myt;
  typedef _Nodeptr _Nodeptr;
  _A allocator;
  _Nodeptr _Head;
  size_type _Size;

  _Node head[1];
  explicit list(const _A _Al = NULL)
    : allocator(_Al),
      _Head(_Buynode()), _Size(0) {}
  explicit list(size_type _N, const _Nodeptr _V,
                const _A & _Al)
    : allocator(_Al),
      _Head(_Buynode()), _Size(0)
  {insert(begin(), _N, _V); }
  list(const _Myt & _X)
    : allocator(_X.allocator),
      _Head(_Buynode()), _Size(0)
  {insert(begin(), _X.begin(), _X.end()); }
  typedef _Nodeptr _It;
  list(_It _F, _It _L, const _A & _Al)
    : allocator(_Al),
      _Head(_Buynode()), _Size(0)
  {insert(begin(), _F, _L); }
  ~list() {
    erase(begin(), end());
    _Head = 0, _Size = 0;
  }
  _Myt & operator=(const _Myt & _X) {
    if (this != &_X) {
      _Nodeptr _F1 = begin();
      _Nodeptr _L1 = end();
      _Nodeptr _F2 = _X.begin();
      _Nodeptr _L2 = _X.end();
      for (; _F1 != _L1 && _F2 != _L2; _Inc(_F1), _Inc(_F2)) {
        allocator->construct(_F1, _F2);
      }
      erase(_F1, _L1);
      insert(_L1, _F2, _L2);
    }
    return (*this);
  }
  _Nodeptr begin()
  {return (_Acc::_Next(_Head)); }
  _Nodeptr begin() const
  {return (_Acc::_Next(_Head)); }
  _Nodeptr end()
  {return (_Head); }
  _Nodeptr end() const
  {return (_Head); }
  _Nodeptr rbegin()
  {return (end()); }
  _Nodeptr rbegin() const
  {return (_Nodeptr(end())); }
  _Nodeptr rend()
  {return (_Nodeptr(begin())); }
  _Nodeptr rend() const
  {return (_Nodeptr(begin())); }
  void resize(size_type _N, const _Nodeptr _X) {
    if (size() < _N) {
      insert(end(), _N - size(), _X);
    } else
      while (_N < size()) {
        pop_back();
      }
  }
  size_type size() const
  {return (_Size); }
  size_type max_size() const
  {return (allocator->max_size()); }
  bool empty() const
  {return (size() == 0); }
  _A get_allocator() const
  {return (allocator); }
  void push_front(_Nodeptr _X)
  {insert(begin(), _X); }
  void pop_front()
  {erase(begin()); }
  void push_back(const _Nodeptr _X)
  {insert(end(), _X); }
  void pop_back() {
    _Nodeptr p = end();
    erase(_Dec(p));
  }
  void assign(_It _F, _It _L) {
    erase(begin(), end());
    insert(begin(), _F, _L);
  }
  void assign(size_type _N, _Nodeptr _X) {
    erase(begin(), end());
    insert(begin(), _N, _X);
  }
  _Nodeptr insert(_Nodeptr _P, const _Nodeptr _X) {
    _Nodeptr _S = _P;
    _Acc::_Prev(_S) = _Buynode(_S, _Acc::_Prev(_S), _X);
    _S = _Acc::_Prev(_S);
    _Acc::_Next(_Acc::_Prev(_S)) = _S;
    //allocator->construct(&_Acc::_Value(_S), _X);
    ++_Size;
    return (_Nodeptr(_S));
  }
  void insert(_Nodeptr _P, size_type _M, _Nodeptr _X) {
    for (; 0 < _M; --_M) {
      insert(_P, _X);
    }
  }
  void insert(_Nodeptr _P, _It _F, _It _L) {
    for (; _F != _L; _Inc(_F)) {
      insert(_P, _F);
    }
  }
  _Nodeptr erase(_Nodeptr _P) {
    _Nodeptr _S = Inc_(_P);
    _Acc::_Next(_Acc::_Prev(_S)) = _Acc::_Next(_S);
    _Acc::_Prev(_Acc::_Next(_S)) = _Acc::_Prev(_S);
    allocator->destroy(_S);
    --_Size;
    return (_P);
  }
  _Nodeptr erase(_Nodeptr _F, _Nodeptr _L) {
    while (_F != _L) {
      erase(Inc_(_F));
    }
    return (_F);
  }
  void clear()
  {erase(begin(), end()); }
  void swap(_Myt & _X) {
    if (allocator == _X.allocator) {
      T_SWAP(_Nodeptr, _Head, _X._Head);
      T_SWAP(size_t, _Size, _X._Size);
    } else {
      _Nodeptr _P = begin();
      splice(_P, _X);
      _X.splice(_X.begin(), *this, _P, end());
    }
  }
  void swap(_Myt & _X, _Myt & _Y)
  {_X.swap(_Y); }
  void splice(_Nodeptr _P, _Myt & _X) {
    if (!_X.empty()) {
      _Splice(_P, _X, _X.begin(), _X.end());
      _Size += _X._Size;
      _X._Size = 0;
    }
  }
  void splice(_Nodeptr _P, _Myt & _X, _Nodeptr _F) {
    _Nodeptr _L = _F;
    if (_P != _F && _P != _Inc(_L)) {
      _Splice(_P, _X, _F, _L);
      ++_Size;
      --_X._Size;
    }
  }
  typedef int difference_type;

  void _Distance(_Nodeptr _F, _Nodeptr _L, size_t & _N)
  { for (; _F != _L; _Inc(_F)) {  ++_N;  }  }
  void splice(_Nodeptr _P, _Myt & _X, _Nodeptr _F, _Nodeptr _L) {
    if (_F != _L) {
      if (&_X != this) {
        size_t _N = 0;
        _Distance(_F, _L, _N);
        _Size += _N;
        _X._Size -= _N;
      }
      _Splice(_P, _X, _F, _L);
    }
  }
  void remove(const _Ty & _V) {
    _Nodeptr _L = end();
    for (_Nodeptr _F = begin(); _F != _L; )
      if (_F->_Value == _V) {
        erase(Inc_(_F));
      } else {
        _Inc(_F);
      }
  }
  typedef int (_Pr1)(_Nodeptr _F);
  typedef int (_Pr2)(_Nodeptr _F, _Nodeptr _M);
  void remove_if(_Pr1 _Pr) {
    _Nodeptr _L = end();
    for (_Nodeptr _F = begin(); _F != _L; )
      if (_Pr(_F)) {
        erase(Inc_(_F));
      } else {
        _Inc(_F);
      }
  }
  void unique() {
    _Nodeptr _F = begin(), _L = end();
    if (_F != _L)
      for (_Nodeptr _M = _F; _Inc(_M) != _L; _M = _F)
        if (_F->_Value == _M->_Value) {
          erase(_M);
        } else {
          _F = _M;
        }
  }

  void unique(_Pr2 _Pr) {
    _Nodeptr _F = begin(), _L = end();
    if (_F != _L)
      for (_Nodeptr _M = _F; _Inc(_M) != _L; _M = _F)
        if (_Pr(_F, _M)) {
          erase(_M);
        } else {
          _F = _M;
        }
  }
  void merge(_Myt & _X) {
    if (&_X != this) {
      _Nodeptr _F1 = begin(), _L1 = end();
      _Nodeptr _F2 = _X.begin(), _L2 = _X.end();
      while (_F1 != _L1 && _F2 != _L2)
        if (_F2->_Value < _F1->_Value) {
          _Nodeptr _Mid2 = _F2;
          _Splice(_F1, _X, _F2, _Inc(_Mid2));
          _F2 = _Mid2;
        } else {
          _Inc(_F1);
        }
      if (_F2 != _L2) {
        _Splice(_L1, _X, _F2, _L2);
      }
      _Size += _X._Size;
      _X._Size = 0;
    }
  }
  void merge(_Myt & _X, _Pr2 _Pr) {
    if (&_X != this) {
      _Nodeptr _F1 = begin(), _L1 = end();
      _Nodeptr _F2 = _X.begin(), _L2 = _X.end();
      while (_F1 != _L1 && _F2 != _L2)
        if (_Pr(_F2, _F1)) {
          _Nodeptr _Mid2 = _F2;
          _Splice(_F1, _X, _F2, _Inc(_Mid2));
          _F2 = _Mid2;
        } else {
          _Inc(_F1);
        }
      if (_F2 != _L2) {
        _Splice(_L1, _X, _F2, _L2);
      }
      _Size += _X._Size;
      _X._Size = 0;
    }
  }
  void sort() {
    if (2 <= size()) {
      const size_t _MAXN = 15;
      _Myt _X(allocator), _A[_MAXN + 1];
      size_t _N = 0;
      for (size_t i=0; i<_MAXN+1; ++i) {
        _A[i].allocator = allocator;
      }
      while (!empty()) {
        _X.splice(_X.begin(), *this, begin());
        size_t _I;
        for (_I = 0; _I < _N && !_A[_I].empty(); ++_I) {
          _A[_I].merge(_X);
          _A[_I].swap(_X);
        }
        if (_I == _MAXN) {
          _A[_I].merge(_X);
        } else {
          _A[_I].swap(_X);
          if (_I == _N) {
            ++_N;
          }
        }
      }
      while (0 < _N) {
        merge(_A[--_N]);
      }
    }
  }
  void sort(_Pr2 _Pr) {
    if (2 <= size()) {
      const size_t _MAXN = 15;
      _Myt _X(allocator), _A[_MAXN + 1];
      size_t _N = 0;
      for (size_t i=0; i<_MAXN+1; ++i) {
        _A[i].allocator = allocator;
      }
      while (!empty()) {
        _X.splice(_X.begin(), *this, begin());
        size_t _I;
        for (_I = 0; _I < _N && !_A[_I].empty(); ++_I) {
          _A[_I].merge(_X, _Pr);
          _A[_I].swap(_X);
        }
        if (_I == _MAXN) {
          _A[_I].merge(_X, _Pr);
        } else {
          _A[_I].swap(_X);
          if (_I == _N) {
            ++_N;
          }
        }
      }
      while (0 < _N) {
        merge(_A[--_N], _Pr);
      }
    }
  }
  void reverse() {
    if (2 <= size()) {
      _Nodeptr _L = end();
      _Nodeptr _F = begin();
      for (_Inc(_F); _F != _L; ) {
        _Nodeptr _M = _F;
        _Splice(begin(), *this, _M, _Inc(_F));
      }
    }
  }
  _Nodeptr _Buynode() {
    //return _Buynode(0, 0, head);
    _Nodeptr _S = head;
    _Acc::_Next(_S) = _S;
    _Acc::_Prev(_S) = _S;
    return (_S);
  }
  _Nodeptr _Buynode(_Nodeptr _Narg, _Nodeptr _Parg, const _Nodeptr _X) {
    _Nodeptr _S = NULL;
    allocator->construct(_S, _X);
    _Acc::_Next(_S) = _Narg != 0 ? _Narg : _S;
    _Acc::_Prev(_S) = _Parg != 0 ? _Parg : _S;
    return (_S);
  }
  void _Splice(_Nodeptr _P, _Myt & _X, _Nodeptr _F, _Nodeptr _L) {
    if (allocator == _X.allocator) {
      _Acc::_Next(_Acc::_Prev(_L)) = _P;
      _Acc::_Next(_Acc::_Prev(_F)) = _L;
      _Acc::_Next(_Acc::_Prev(_P)) = _F;
      _Nodeptr _S = _Acc::_Prev(_P);
      _Acc::_Prev(_P) =
        _Acc::_Prev(_L);
      _Acc::_Prev(_L) =
        _Acc::_Prev(_F);
      _Acc::_Prev(_F) = _S;
    } else {
      insert(_P, _F, _L);
      _X.erase(_F, _L);
    }
  }
};
// list TEMPLATE OPERATORS
template<class _Ty, class _A> inline
bool operator==(const list & _X,
                const list & _Y)
{
  return (_X.size() == _Y.size()
          && equal(_X.begin(), _X.end(), _Y.begin()));
}
template<class _Ty, class _A> inline
bool operator!=(const list & _X,
                const list & _Y)
{return (!(_X == _Y)); }
template<class _Ty, class _A> inline
bool operator<(const list & _X,
               const list & _Y)
{
  return (lexicographical_compare(_X.begin(), _X.end(),
                                  _Y.begin(), _Y.end()));
}
template<class _Ty, class _A> inline
bool operator>(const list & _X,
               const list & _Y)
{return (_Y < _X); }
template<class _Ty, class _A> inline
bool operator<=(const list & _X,
                const list & _Y)
{return (!(_Y < _X)); }
template<class _Ty, class _A> inline
bool operator>=(const list & _X,
                const list & _Y)
{return (!(_X < _Y)); }

struct test_list_t {
  int i;
  _Node list[1];
  char str[10];
};

struct test_list_Allocator : Allocator {
  virtual void construct(_Nodepref _P, const _Nodeptr _X) {
    test_list_t* a = _P ? container_of(_P, test_list_t, list) : NULL;
    a = (test_list_t*)realloc(a, sizeof(test_list_t));
    _P = a->list;
    test_list_t* b = container_of(_X, test_list_t, list);
    a->i = b->i;
    _P->_Value = _X->_Value;
    strcpy(a->str, b->str);
  }
  virtual void destroy(_Nodepref _P) {
    if (_P) {
      test_list_t* a = container_of(_P, test_list_t, list);
      free(a);
      _P = NULL;
    }
  }
};

int test_list()
{
  if (1) {
    test_list_Allocator aa;
    list li(&aa);
    int i, n=100;
    test_list_t t[1] = {0};
    for (i=0; i<n; ++i) {
      t->i = 0;
      t->list->_Value = i;
      _snprintf(t->str, 10, "%d", i);
      li.push_back(t->list);
    }
    if (1) {
      _Nodeptr b = li.begin();
      _Nodeptr e = li.end();
      for (i=0;b!=e; _Inc(b), ++i) {
        ASSERT(b->_Value==i);
      }
    }
    li.reverse();
    if (1) {
      _Nodeptr b = li.begin();
      _Nodeptr e = li.end();
      for (i=n-1;b!=e; _Inc(b), --i) {
        ASSERT(b->_Value==i);
      }
    }
    li.sort();
    if (1) {
      _Nodeptr b = li.begin();
      _Nodeptr e = li.end();
      for (i=0;b!=e; _Inc(b), ++i) {
        ASSERT(b->_Value==i);
      }
    }
  }
  return 0;
}

