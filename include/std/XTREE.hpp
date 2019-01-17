
#define _POINTER_X(a, b)  a*
#define _REFERENCE_X(a, b)  a&

#include "types_c.h"
#include <limits.h>

enum _Redbl { _Red, _Black };
struct RBTreeNode;
typedef int _Ty;
typedef _POINTER_X(RBTreeNode, _A) RBTreeNodeptr;
struct RBTreeNode {
  _Redbl _Color;
  RBTreeNodeptr _Left, _Parent, _Right;
  _Ty _Value;
};

typedef _REFERENCE_X(RBTreeNodeptr, _A) RBTreeNodepref;
typedef _REFERENCE_X(_Redbl, _A) _Rbref;


//template <typename T>void swap(T& a, T& b) { T t = a; a = b, b = t; }

static _Rbref _Color(RBTreeNodeptr _P)
{
  return ((_Rbref)(*_P)._Color);
}
static RBTreeNodeptr _Key(RBTreeNodeptr _P)
{
  return _P;
}
static RBTreeNodepref _Left(RBTreeNodeptr _P)
{
  return ((RBTreeNodepref)(*_P)._Left);
}
static RBTreeNodepref _Parent(RBTreeNodeptr _P)
{
  return ((RBTreeNodepref)(*_P)._Parent);
}
static RBTreeNodepref _Right(RBTreeNodeptr _P)
{
  return ((RBTreeNodepref)(*_P)._Right);
}
static RBTreeNodepref _Value(RBTreeNodeptr _P)
{
  return _P;
}

static RBTreeNode _Nil[1] = { _Black , 0};

typedef size_t size_type;
typedef int difference_type;
typedef RBTreeNodeptr iterator;

static RBTreeNodeptr _Max(RBTreeNodeptr _P) {
  while (_Right(_P) != _Nil) {
    _P = _Right(_P);
  }
  return (_P);
}
static RBTreeNodeptr _Min(RBTreeNodeptr _P) {
  while (_Left(_P) != _Nil) {
    _P = _Left(_P);
  }
  return (_P);
}
RBTreeNodepref _Inc(RBTreeNodepref _Ptr) {
  if (_Right(_Ptr) != _Nil) {
    _Ptr = _Min(_Right(_Ptr));
  }
  else {
    RBTreeNodeptr _P;
    while (_Ptr == _Right(_P = _Parent(_Ptr))) {
      _Ptr = _P;
    }
    if (_Right(_Ptr) != _P) {
      _Ptr = _P;
    }
  }
  return _Ptr;
}
RBTreeNodepref _Dec(RBTreeNodepref _Ptr) {
  if (_Color(_Ptr) == _Red
    && _Parent(_Parent(_Ptr)) == _Ptr) {
    _Ptr = _Right(_Ptr);
  }
  else if (_Left(_Ptr) != _Nil) {
    _Ptr = _Max(_Left(_Ptr));
  }
  else {
    RBTreeNodeptr _P;
    while (_Ptr == _Left(_P = _Parent(_Ptr))) {
      _Ptr = _P;
    }
    _Ptr = _P;
  }
  return _Ptr;
}
RBTreeNodeptr Inc_(RBTreeNodepref _Ptr) {
  iterator _Tmp = _Ptr;
  _Inc(_Ptr);
  return (_Tmp);
}
RBTreeNodeptr Dec_(RBTreeNodepref _Ptr) {
  iterator _Tmp = _Ptr;
  _Dec(_Ptr);
  return (_Tmp);
}

template <typename T, typename Y>
struct pair {
  T first;
  Y second;
  pair(const T& f, const Y& s) {
    first = f, second = s;
  }
};

struct Allocator {
  virtual size_t max_size() const { return UINT_MAX; }
  virtual void construct(RBTreeNodepref _P, const RBTreeNodeptr _X) = 0;
  virtual void destroy(RBTreeNodepref _P) = 0;
  virtual int key_compare(const RBTreeNodeptr _P, const RBTreeNodeptr _X) = 0;
};

struct _Lockit {};

struct _Tree
{
  typedef Allocator _A;
  _A* allocator;
  RBTreeNodeptr _Head;
  bool _Multi;
  size_type _Size;
  typedef _Tree _Myt;
  
  typedef pair<iterator, bool> _Pairib;
  typedef pair<iterator, iterator> _Pairii;
  typedef pair<iterator, iterator> _Paircc;
  explicit _Tree(bool _Marg = true,
                 _A* _Al = NULL)
    : allocator(_Al), _Multi(_Marg)
  {_Init(); }
  _Tree(const _Myt & _X)
    : allocator(_X.allocator), _Multi(_X._Multi) {
    _Init();
    _Copy(_X);
  }
  ~_Tree() {
    erase(begin(), end());
    _Freenode(_Head);
    _Head = 0, _Size = 0;
  }
  _Myt & operator=(const _Myt & _X) {
    if (this != &_X) {
      erase(begin(), end());
      allocator = _X.allocator;
      _Copy(_X);
    }
    return (*this);
  }
  RBTreeNodeptr & _Rmost()
  {
    return (_Right(_Head));
  }
  RBTreeNodeptr & _Rmost() const
  {
    return (_Right(_Head));
  }
  RBTreeNodeptr & _Root()
  {
    return (_Parent(_Head));
  }
  RBTreeNodeptr & _Root() const
  {
    return (_Parent(_Head));
  }
  void _Rrotate(RBTreeNodeptr _X) {
    RBTreeNodeptr _Y = _Left(_X);
    _Left(_X) = _Right(_Y);
    if (_Right(_Y) != _Nil) {
      _Parent(_Right(_Y)) = _X;
    }
    _Parent(_Y) = _Parent(_X);
    if (_X == _Root()) {
      _Root() = _Y;
    }
    else if (_X == _Right(_Parent(_X))) {
      _Right(_Parent(_X)) = _Y;
    }
    else {
      _Left(_Parent(_X)) = _Y;
    }
    _Right(_Y) = _X;
    _Parent(_X) = _Y;
  }
  RBTreeNodeptr _Ubound(const RBTreeNodeptr _Kv) const {
    RBTreeNodeptr _X = _Root();
    RBTreeNodeptr _Y = _Head;
    while (_X != _Nil)
      if (allocator->key_compare(_Kv, _Key(_X))) {
        _Y = _X, _X = _Left(_X);
      }
      else {
        _X = _Right(_X);
      }
      return (_Y);
  }
  RBTreeNode head[1];
  RBTreeNodeptr _Buynode2(RBTreeNodeptr _Parg, _Redbl _Carg) {
    RBTreeNodeptr _S = head;
    _Parent(_S) = _Parg;
    _Color(_S) = _Carg;
    return (_S);
  }
  RBTreeNodeptr _Buynode(RBTreeNodeptr _Parg, _Redbl _Carg, const RBTreeNodeptr _X) {
    RBTreeNodeptr _S = NULL;
    allocator->construct(_S, _X);
    _Parent(_S) = _Parg;
    _Color(_S) = _Carg;
    return (_S);
  }
  void _Consval(RBTreeNodepref _P, const RBTreeNodeptr _V)
  {
    allocator->construct(_P, _V);
  }
  void _Freenode(RBTreeNodeptr _P)
  {
    allocator->destroy(_P);
    _P = NULL;
  }
  iterator begin()
  {return (iterator(_Lmost())); }
  iterator begin() const
  {return (iterator(_Lmost())); }
  iterator end()
  {return (iterator(_Head)); }
  iterator end() const
  {return (iterator(_Head)); }
  iterator rbegin()
  {return (iterator(end())); }
  iterator rbegin() const
  {return (iterator(end())); }
  iterator rend()
  {return (iterator(begin())); }
  iterator rend() const
  {return (iterator(begin())); }
  size_type size() const
  {return (_Size); }
  size_type max_size() const
  {return (allocator->max_size()); }
  bool empty() const
  {return (size() == 0); }
  _Pairib insert(const RBTreeNodeptr _V) {
    RBTreeNodeptr _X = _Root();
    RBTreeNodeptr _Y = _Head;
    bool _Ans = true;
    while (_X != _Nil) {
      _Y = _X;
      _Ans = allocator->key_compare(_Key(_V), _Key(_X));
      _X = _Ans ? _Left(_X) : _Right(_X);
    }
    if (_Multi) {
      return (_Pairib(_Insert(_X, _Y, _V), true));
    }
    iterator _P = iterator(_Y);
    if (!_Ans)
      ;
    else if (_P == begin()) {
      return (_Pairib(_Insert(_X, _Y, _V), true));
    } else {
      --_P;
    }
    if (allocator->key_compare(_Key(_P), _Key(_V))) {
      return (_Pairib(_Insert(_X, _Y, _V), true));
    }
    return (_Pairib(_P, false));
  }
  iterator insert(iterator _P, const RBTreeNodeptr _V) {
    if (size() == 0)
      ;
    else if (_P == begin()) {
      if (allocator->key_compare(_Key(_V), _Key(_P))) {
        return (_Insert(_Head, _P, _V));
      }
    } else if (_P == end()) {
      if (allocator->key_compare(_Key(_Rmost()), _Key(_V))) {
        return (_Insert(_Nil, _Rmost(), _V));
      }
    } else {
      iterator _Pb = _P;
      if (allocator->key_compare(_Key((--_Pb)), _Key(_V))
          && allocator->key_compare(_Key(_V), _Key(_P))) {
        if (_Right(_Pb) == _Nil) {
          return (_Insert(_Nil, _Pb, _V));
        } else {
          return (_Insert(_Head, _P, _V));
        }
      }
    }
    return (insert(_V).first);
  }
  void insert_range(iterator _F, iterator _L) {
    for (; _F != _L; _Inc(_F)) {
      insert(_F);
    }
  }
  iterator erase(iterator _P) {
    RBTreeNodeptr _X;
    RBTreeNodeptr _Y = (_P++);
    RBTreeNodeptr _Z = _Y;
    if (_Left(_Y) == _Nil) {
      _X = _Right(_Y);
    } else if (_Right(_Y) == _Nil) {
      _X = _Left(_Y);
    } else {
      _Y = _Min(_Right(_Y)), _X = _Right(_Y);
    }
    {
      _Lockit _Lk;
      if (_Y != _Z) {
        _Parent(_Left(_Z)) = _Y;
        _Left(_Y) = _Left(_Z);
        if (_Y == _Right(_Z)) {
          _Parent(_X) = _Y;
        } else {
          _Parent(_X) = _Parent(_Y);
          _Left(_Parent(_Y)) = _X;
          _Right(_Y) = _Right(_Z);
          _Parent(_Right(_Z)) = _Y;
        }
        if (_Root() == _Z) {
          _Root() = _Y;
        } else if (_Left(_Parent(_Z)) == _Z) {
          _Left(_Parent(_Z)) = _Y;
        } else {
          _Right(_Parent(_Z)) = _Y;
        }
        _Parent(_Y) = _Parent(_Z);
        T_SWAP(_Redbl, _Color(_Y), _Color(_Z));
        _Y = _Z;
      } else {
        _Parent(_X) = _Parent(_Y);
        if (_Root() == _Z) {
          _Root() = _X;
        } else if (_Left(_Parent(_Z)) == _Z) {
          _Left(_Parent(_Z)) = _X;
        } else {
          _Right(_Parent(_Z)) = _X;
        }
        if (_Lmost() != _Z)
          ;
        else if (_Right(_Z) == _Nil) {
          _Lmost() = _Parent(_Z);
        } else {
          _Lmost() = _Min(_X);
        }
        if (_Rmost() != _Z)
          ;
        else if (_Left(_Z) == _Nil) {
          _Rmost() = _Parent(_Z);
        } else {
          _Rmost() = _Max(_X);
        }
      }
      if (_Color(_Y) == _Black) {
        while (_X != _Root() && _Color(_X) == _Black)
          if (_X == _Left(_Parent(_X))) {
            RBTreeNodeptr _W = _Right(_Parent(_X));
            if (_Color(_W) == _Red) {
              _Color(_W) = _Black;
              _Color(_Parent(_X)) = _Red;
              _Lrotate(_Parent(_X));
              _W = _Right(_Parent(_X));
            }
            if (_Color(_Left(_W)) == _Black
                && _Color(_Right(_W)) == _Black) {
              _Color(_W) = _Red;
              _X = _Parent(_X);
            } else {
              if (_Color(_Right(_W)) == _Black) {
                _Color(_Left(_W)) = _Black;
                _Color(_W) = _Red;
                _Rrotate(_W);
                _W = _Right(_Parent(_X));
              }
              _Color(_W) = _Color(_Parent(_X));
              _Color(_Parent(_X)) = _Black;
              _Color(_Right(_W)) = _Black;
              _Lrotate(_Parent(_X));
              break;
            }
          } else {
            RBTreeNodeptr _W = _Left(_Parent(_X));
            if (_Color(_W) == _Red) {
              _Color(_W) = _Black;
              _Color(_Parent(_X)) = _Red;
              _Rrotate(_Parent(_X));
              _W = _Left(_Parent(_X));
            }
            if (_Color(_Right(_W)) == _Black
                && _Color(_Left(_W)) == _Black) {
              _Color(_W) = _Red;
              _X = _Parent(_X);
            } else {
              if (_Color(_Left(_W)) == _Black) {
                _Color(_Right(_W)) = _Black;
                _Color(_W) = _Red;
                _Lrotate(_W);
                _W = _Left(_Parent(_X));
              }
              _Color(_W) = _Color(_Parent(_X));
              _Color(_Parent(_X)) = _Black;
              _Color(_Left(_W)) = _Black;
              _Rrotate(_Parent(_X));
              break;
            }
          }
        _Color(_X) = _Black;
      }
    }
    _Freenode(_Y);
    --_Size;
    return (_P);
  }
  iterator erase(iterator _F, iterator _L) {
    if (size() == 0 || _F != begin() || _L != end()) {
      while (_F != _L) {
        erase(_F++);
      }
      return (_F);
    } else {
      _Erase(_Root());
      _Root() = _Nil, _Size = 0;
      _Lmost() = _Head, _Rmost() = _Head;
      return (begin());
    }
  }
  static void _Distance(RBTreeNodeptr _F, RBTreeNodeptr _L, size_t & _N)
  {
    for (; _F != _L; _Inc(_F)) { ++_N; }
  }
  size_type erase_all(const RBTreeNodeptr _X) {
    _Pairii _P = equal_range(_X);
    size_type _N = 0;
    _Distance(_P.first, _P.second, _N);
    erase(_P.first, _P.second);
    return (_N);
  }
  void erase_range(RBTreeNodeptr _F, RBTreeNodeptr _L) {
    for (; _F != _L; _Inc(_F)) {
      erase(_F);
    }
  }
  void clear()
  {erase(begin(), end()); }
  iterator find(const RBTreeNodeptr _Kv) {
    iterator _P = lower_bound(_Kv);
    return (_P == end()
            || allocator->key_compare(_Kv, _P)
            ? end() : _P);
  }
  iterator find(const RBTreeNodeptr _Kv) const {
    iterator _P = lower_bound(_Kv);
    return (_P == end()
            || allocator->key_compare(_Kv, _Key(_P))
            ? end() : _P);
  }
  size_type count(const RBTreeNodeptr _Kv) const {
    _Paircc _Ans = equal_range(_Kv);
    size_type _N = 0;
    _Distance(_Ans.first, _Ans.second, _N);
    return (_N);
  }
  iterator lower_bound(const RBTreeNodeptr _Kv)
  {return (iterator(_Lbound(_Kv))); }
  iterator lower_bound(const RBTreeNodeptr _Kv) const
  {return (iterator(_Lbound(_Kv))); }
  iterator upper_bound(const RBTreeNodeptr _Kv)
  {return (iterator(_Ubound(_Kv))); }
  iterator upper_bound(const RBTreeNodeptr _Kv) const
  {return (iterator(_Ubound(_Kv))); }
  _Pairii equal_range(const RBTreeNodeptr _Kv)
  {return (_Pairii(lower_bound(_Kv), upper_bound(_Kv))); }
  _Paircc equal_range(const RBTreeNodeptr _Kv) const
  {return (_Paircc(lower_bound(_Kv), upper_bound(_Kv))); }
  void swap(_Myt & _X) {
    T_SWAP(_A*, allocator, _X.allocator);
    if (allocator == _X.allocator) {
      T_SWAP(RBTreeNodeptr, _Head, _X._Head);
      T_SWAP(bool, _Multi, _X._Multi);
      T_SWAP(size_t, _Size, _X._Size);
    } else
    {_Myt _Ts = *this; *this = _X, _X = _Ts; }
  }
  friend void swap(_Myt & _X, _Myt & _Y)
  {_X.swap(_Y); }
protected:
  void _Copy(const _Myt & _X) {
    _Root() = _Copy(_X._Root(), _Head);
    _Size = _X.size();
    if (_Root() != _Nil) {
      _Lmost() = _Min(_Root());
      _Rmost() = _Max(_Root());
    } else {
      _Lmost() = _Head, _Rmost() = _Head;
    }
  }
  RBTreeNodeptr _Copy(RBTreeNodeptr _X, RBTreeNodeptr _P) {
    RBTreeNodeptr _R = _X;
    for (; _X != _Nil; _X = _Left(_X)) {
      RBTreeNodeptr _Y = _Buynode(_P, _Color(_X), _X);
      if (_R == _X) {
        _R = _Y;
      }
      _Right(_Y) = _Copy(_Right(_X), _Y);
      _Left(_P) = _Y;
      _P = _Y;
    }
    _Left(_P) = _Nil;
    return (_R);
  }
  void _Erase(RBTreeNodeptr _X) {
    for (RBTreeNodeptr _Y = _X; _Y != _Nil; _X = _Y) {
      _Erase(_Right(_Y));
      _Y = _Left(_Y);
      _Freenode(_X);
    }
  }
  void _Init() {
    _Head = _Buynode2(_Nil, _Red);
    _Size = 0;
    _Lmost() = _Head, _Rmost() = _Head;
  }
  iterator _Insert(RBTreeNodeptr _X, RBTreeNodeptr _Y, const RBTreeNodeptr _V) {
    RBTreeNodeptr _Z = _Buynode(_Y, _Red, _V);
    _Left(_Z) = _Nil, _Right(_Z) = _Nil;
    ++_Size;
    if (_Y == _Head || _X != _Nil
        || allocator->key_compare(_Key(_V), _Key(_Y))) {
      _Left(_Y) = _Z;
      if (_Y == _Head) {
        _Root() = _Z;
        _Rmost() = _Z;
      } else if (_Y == _Lmost()) {
        _Lmost() = _Z;
      }
    } else {
      _Right(_Y) = _Z;
      if (_Y == _Rmost()) {
        _Rmost() = _Z;
      }
    }
    for (_X = _Z; _X != _Root()
         && _Color(_Parent(_X)) == _Red; )
      if (_Parent(_X) == _Left(_Parent(_Parent(_X)))) {
        _Y = _Right(_Parent(_Parent(_X)));
        if (_Color(_Y) == _Red) {
          _Color(_Parent(_X)) = _Black;
          _Color(_Y) = _Black;
          _Color(_Parent(_Parent(_X))) = _Red;
          _X = _Parent(_Parent(_X));
        } else {
          if (_X == _Right(_Parent(_X))) {
            _X = _Parent(_X);
            _Lrotate(_X);
          }
          _Color(_Parent(_X)) = _Black;
          _Color(_Parent(_Parent(_X))) = _Red;
          _Rrotate(_Parent(_Parent(_X)));
        }
      } else {
        _Y = _Left(_Parent(_Parent(_X)));
        if (_Color(_Y) == _Red) {
          _Color(_Parent(_X)) = _Black;
          _Color(_Y) = _Black;
          _Color(_Parent(_Parent(_X))) = _Red;
          _X = _Parent(_Parent(_X));
        } else {
          if (_X == _Left(_Parent(_X))) {
            _X = _Parent(_X);
            _Rrotate(_X);
          }
          _Color(_Parent(_X)) = _Black;
          _Color(_Parent(_Parent(_X))) = _Red;
          _Lrotate(_Parent(_Parent(_X)));
        }
      }
    _Color(_Root()) = _Black;
    return (iterator(_Z));
  }
  RBTreeNodeptr _Lbound(const RBTreeNodeptr _Kv) const {
    RBTreeNodeptr _X = _Root();
    RBTreeNodeptr _Y = _Head;
    while (_X != _Nil)
      if (allocator->key_compare(_Key(_X), _Kv)) {
        _X = _Right(_X);
      } else {
        _Y = _X, _X = _Left(_X);
      }
    return (_Y);
  }
  RBTreeNodeptr & _Lmost()
  {return (_Left(_Head)); }
  RBTreeNodeptr & _Lmost() const
  {return (_Left(_Head)); }
  void _Lrotate(RBTreeNodeptr _X) {
    RBTreeNodeptr _Y = _Right(_X);
    _Right(_X) = _Left(_Y);
    if (_Left(_Y) != _Nil) {
      _Parent(_Left(_Y)) = _X;
    }
    _Parent(_Y) = _Parent(_X);
    if (_X == _Root()) {
      _Root() = _Y;
    } else if (_X == _Left(_Parent(_X))) {
      _Left(_Parent(_X)) = _Y;
    } else {
      _Right(_Parent(_X)) = _Y;
    }
    _Left(_Y) = _X;
    _Parent(_X) = _Y;
  }
};

// tree TEMPLATE OPERATORS
template < class _K, class _Ty, class _Kfn,
         class _Pr, class _A > inline
bool operator==(const _Tree & _X,
                const _Tree & _Y)
{
  return (_X.size() == _Y.size()
          && equal(_X.begin(), _X.end(), _Y.begin()));
}
template < class _K, class _Ty, class _Kfn,
         class _Pr, class _A > inline
bool operator!=(const _Tree & _X,
                const _Tree & _Y)
{return (!(_X == _Y)); }
template < class _K, class _Ty, class _Kfn,
         class _Pr, class _A > inline
bool operator<(const _Tree & _X,
               const _Tree & _Y)
{
  return (lexicographical_compare(_X.begin(), _X.end(),
                                  _Y.begin(), _Y.end()));
}
template < class _K, class _Ty, class _Kfn,
         class _Pr, class _A > inline
bool operator>(const _Tree & _X,
               const _Tree & _Y)
{return (_Y < _X); }
template < class _K, class _Ty, class _Kfn,
         class _Pr, class _A > inline
bool operator<=(const _Tree & _X,
                const _Tree & _Y)
{return (!(_Y < _X)); }
template < class _K, class _Ty, class _Kfn,
         class _Pr, class _A > inline
bool operator>=(const _Tree & _X,
                const _Tree & _Y)
{return (!(_X < _Y)); }


int test_xtree() {
  return 0;
}
