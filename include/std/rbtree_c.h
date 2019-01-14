

#include "types_c.h"
// tree internal header

#include "inttypes_c.h"
#include "algo.h"
#include "list.h"
#include "hashcode_c.h"
#include "mempool_c.h"

enum _Redbl {_Red, _Black};

struct rbnode;
typedef rbnode* rbnodeptr;

struct rbnode {
  _Redbl color;
  rbnodeptr left, parent, right;
};

typedef int(*cmp_rbnode_fun_t)(const rbnode* node1, const rbnode* node2);
//typedef void (*setkv_fun_t)(kvnode* kv, rbnodeptr key, rbnodeptr value);
//typedef void (*delkv_fun_t)(kvnode* kv);

static rbnode _Nil[1] = {_Black, 0};


#define _Color(_P)     (_P)->color
#define _Key(_P)       (_P)
//#define _Key(_P)     (_P)->value
#define _Left(_P)     (_P)->left
#define _Parent(_P)     (_P)->parent
#define _Right(_P)     (_P)->right

#define _Lmost(tree)   (_Left(tree->_Head))
#define _Rmost(tree)   (_Right(tree->_Head))
#define _Root(tree)    (_Parent(tree->_Head))

static rbnodeptr _Max(rbnodeptr _P)
{
  while (_Right(_P) != _Nil) {
    _P = _Right(_P);
  }
  return (_P);
}
static rbnodeptr _Min(rbnodeptr _P)
{
  while (_Left(_P) != _Nil) {
    _P = _Left(_P);
  }
  return (_P);
}

rbnodeptr _Dec(rbnodeptr& _Ptr)
{
  if (_Color(_Ptr) == _Red && _Parent(_Parent(_Ptr)) == _Ptr) {
    _Ptr = _Right(_Ptr);
  } else if (_Left(_Ptr) != _Nil) {
    _Ptr = _Max(_Left(_Ptr));
  } else {
    rbnodeptr _P;
    while (_Ptr == _Left(_P = _Parent(_Ptr))) {
      _Ptr = _P;
    }
    _Ptr = _P;
  }
  return _Ptr;
}

rbnodeptr _Inc(rbnodeptr& _Ptr)
{
  if (_Right(_Ptr) != _Nil) {
    _Ptr = _Min(_Right(_Ptr));
  } else {
    rbnodeptr _P;
    while (_Ptr == _Right(_P = _Parent(_Ptr))) {
      _Ptr = _P;
    }
    if (_Right(_Ptr) != _P) {
      _Ptr = _P;
    }
  }
  return _Ptr;
}
rbnodeptr Dec_(rbnodeptr& _Ptr) {
  rbnodeptr ret = _Ptr;
  _Dec(_Ptr);
  return ret;
}
rbnodeptr Inc_(rbnodeptr& _Ptr) {
  rbnodeptr ret = _Ptr;
  _Inc(_Ptr);
  return ret;
}

struct _Lockit {};

#define _Destval(_P)         if (Freenode) Freenode(*(_P), NULL)
#define _Freenode(_S)        if (Freenode) Freenode(_S, NULL)
#define _Copynode(_S, from)  if (Freenode) Freenode(_S, from)
typedef void (*set_rbtree_fun_t)(rbnodeptr& _S, rbnodeptr x);

rbnodeptr _Buynode(rbnodeptr _Parg, _Redbl _Carg, rbnodeptr x)
{
  rbnodeptr _S = x;
  _Parent(_S) = _Parg;
  _Color(_S) = _Carg;
  return (_S);
}

//typedef void (*consval_fun_t)(rbnodeptr* pkey, rbnodeptr key);
//typedef void (*destval_fun_t)(rbnodeptr* pkey);

struct rbtree_t {
  size_t _Size;
  rbnodeptr _Head;
  rbnode head[1];
};

rbnodeptr begin(rbtree_t* tree) { return (_Lmost(tree)); }
rbnodeptr end(rbtree_t* tree) {return (tree->_Head); }

void rbtree_Init(rbtree_t* tree)
{
  tree->_Head = _Buynode(_Nil, _Red, tree->head), tree->_Size = 0;
  _Lmost(tree) = tree->_Head, _Rmost(tree) = tree->_Head;
}

void _Lrotate(rbtree_t* tree, rbnodeptr _X)
{
  rbnodeptr _Y = _Right(_X);
  _Right(_X) = _Left(_Y);
  if (_Left(_Y) != _Nil) {
    _Parent(_Left(_Y)) = _X;
  }
  _Parent(_Y) = _Parent(_X);
  if (_X == _Root(tree)) {
    _Root(tree) = _Y;
  } else if (_X == _Left(_Parent(_X))) {
    _Left(_Parent(_X)) = _Y;
  } else {
    _Right(_Parent(_X)) = _Y;
  }
  _Left(_Y) = _X;
  _Parent(_X) = _Y;
}
void _Rrotate(rbtree_t* tree, rbnodeptr _X)
{
  rbnodeptr _Y = _Left(_X);
  _Left(_X) = _Right(_Y);
  if (_Right(_Y) != _Nil) {
    _Parent(_Right(_Y)) = _X;
  }
  _Parent(_Y) = _Parent(_X);
  if (_X == _Root(tree)) {
    _Root(tree) = _Y;
  } else if (_X == _Right(_Parent(_X))) {
    _Right(_Parent(_X)) = _Y;
  } else {
    _Left(_Parent(_X)) = _Y;
  }
  _Right(_Y) = _X;
  _Parent(_X) = _Y;
}

rbnodeptr _Insert(rbtree_t* tree, rbnodeptr _X, rbnodeptr _Y, rbnodeptr key, cmp_rbnode_fun_t key_compare)
{
  rbnodeptr _Z = _Buynode(_Y, _Red, key);
  _Left(_Z) = _Nil, _Right(_Z) = _Nil;
  ++tree->_Size;
  if (_Y == tree->_Head || _X != _Nil || key_compare(key, _Key(_Y))) {
    _Left(_Y) = _Z;
    if (_Y == tree->_Head) {
      _Root(tree) = _Z;
      _Rmost(tree) = _Z;
    } else if (_Y == _Lmost(tree)) {
      _Lmost(tree) = _Z;
    }
  } else {
    _Right(_Y) = _Z;
    if (_Y == _Rmost(tree)) {
      _Rmost(tree) = _Z;
    }
  }
  for (_X = _Z; _X != _Root(tree) && _Color(_Parent(_X)) == _Red; )
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
          _Lrotate(tree, _X);
        }
        _Color(_Parent(_X)) = _Black;
        _Color(_Parent(_Parent(_X))) = _Red;
        _Rrotate(tree, _Parent(_Parent(_X)));
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
          _Rrotate(tree, _X);
        }
        _Color(_Parent(_X)) = _Black;
        _Color(_Parent(_Parent(_X))) = _Red;
        _Lrotate(tree, _Parent(_Parent(_X)));
      }
    }
  _Color(_Root(tree)) = _Black;
  return (_Z);
}

struct _Pairib {
  rbnodeptr first;
  bool second;
  _Pairib(rbnodeptr f, bool s) {first = f, second = s;}
};
struct _Pairii {
  rbnodeptr first;
  rbnodeptr second;
  _Pairii(rbnodeptr f, rbnodeptr s) {first = f, second = s;}
};
_Pairib insert(rbtree_t* tree, rbnodeptr key, cmp_rbnode_fun_t key_compare, bool _Multi)
{
  rbnodeptr _X = _Root(tree);
  rbnodeptr _Y = tree->_Head;
  bool _Ans = true;
  while (_X != _Nil) {
    _Y = _X;
    _Ans = key_compare(key, _Key(_X));
    _X = _Ans ? _Left(_X) : _Right(_X);
  }
  if (_Multi) {
    return _Pairib(_Insert(tree, _X, _Y, key, key_compare), true);
  }
  rbnodeptr _P = rbnodeptr(_Y);
  if (!_Ans)
    ;
  else if (_P == begin(tree)) {
    return (_Pairib(_Insert(tree, _X, _Y, key, key_compare), true));
  } else {
    _Dec(_P);
  }
  if (key_compare(_Key(_P), key)) {
    return (_Pairib(_Insert(tree, _X, _Y, key, key_compare), true));
  }
  return (_Pairib(_P, false));
}

rbnodeptr insert(rbtree_t* tree, rbnodeptr _P, rbnodeptr key, cmp_rbnode_fun_t key_compare, bool _Multi)
{
  if (tree->_Size == 0)
    ;
  else if (_P == begin(tree)) {
    if (key_compare(key, _Key(_P))) {
      return (_Insert(tree, tree->_Head, _P, key, key_compare));
    }
  } else if (_P == end(tree)) {
    if (key_compare(_Key(_Rmost(tree)), key)) {
      return (_Insert(tree, _Nil, _Rmost(tree), key, key_compare));
    }
  } else {
    rbnodeptr _Pb = _P;
    if (key_compare(_Key((_Dec(_Pb))), key)
        && key_compare(key, _Key(_P))) {
      if (_Right(_Pb) == _Nil) {
        return (_Insert(tree, _Nil, _Pb, key, key_compare));
      } else {
        return (_Insert(tree, tree->_Head, _P, key, key_compare));
      }
    }
  }
  return (insert(tree, key, key_compare, _Multi).first);
}

void insert_range(rbtree_t* tree, rbnodeptr _F, rbnodeptr _L, cmp_rbnode_fun_t key_compare, bool _Multi)
{
  for (; _F != _L; _Inc(_F)) {
    insert(tree, _Key(_F), key_compare, _Multi);
  }
}

rbnodeptr erase(rbtree_t* tree, rbnodeptr _P, set_rbtree_fun_t Freenode)
{
  rbnodeptr _X;
  rbnodeptr _Y = Inc_(_P);
  rbnodeptr _Z = _Y;
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
      if (_Root(tree) == _Z) {
        _Root(tree) = _Y;
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
      if (_Root(tree) == _Z) {
        _Root(tree) = _X;
      } else if (_Left(_Parent(_Z)) == _Z) {
        _Left(_Parent(_Z)) = _X;
      } else {
        _Right(_Parent(_Z)) = _X;
      }
      if (_Lmost(tree) != _Z)
        ;
      else if (_Right(_Z) == _Nil) {
        _Lmost(tree) = _Parent(_Z);
      } else {
        _Lmost(tree) = _Min(_X);
      }
      if (_Rmost(tree) != _Z)
        ;
      else if (_Left(_Z) == _Nil) {
        _Rmost(tree) = _Parent(_Z);
      } else {
        _Rmost(tree) = _Max(_X);
      }
    }
    if (_Color(_Y) == _Black) {
      while (_X != _Root(tree) && _Color(_X) == _Black)
        if (_X == _Left(_Parent(_X))) {
          rbnodeptr _W = _Right(_Parent(_X));
          if (_Color(_W) == _Red) {
            _Color(_W) = _Black;
            _Color(_Parent(_X)) = _Red;
            _Lrotate(tree, _Parent(_X));
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
              _Rrotate(tree, _W);
              _W = _Right(_Parent(_X));
            }
            _Color(_W) = _Color(_Parent(_X));
            _Color(_Parent(_X)) = _Black;
            _Color(_Right(_W)) = _Black;
            _Lrotate(tree, _Parent(_X));
            break;
          }
        } else {
          rbnodeptr _W = _Left(_Parent(_X));
          if (_Color(_W) == _Red) {
            _Color(_W) = _Black;
            _Color(_Parent(_X)) = _Red;
            _Rrotate(tree, _Parent(_X));
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
              _Lrotate(tree, _W);
              _W = _Left(_Parent(_X));
            }
            _Color(_W) = _Color(_Parent(_X));
            _Color(_Parent(_X)) = _Black;
            _Color(_Left(_W)) = _Black;
            _Rrotate(tree, _Parent(_X));
            break;
          }
        }
      _Color(_X) = _Black;
    }
  }
  _Destval(&_Key(_Y));
  _Freenode(_Y);
  --tree->_Size;
  return (_P);
}
int size(const rbtree_t* tree) {return tree->_Size;}

void _Erase(rbnodeptr _X, set_rbtree_fun_t Freenode)
{
  for (rbnodeptr _Y = _X; _Y != _Nil; _X = _Y) {
    _Erase(_Right(_Y), Freenode);
    _Y = _Left(_Y);
    _Destval(&_Key(_X));
    _Freenode(_X);
  }
}
rbnodeptr erase(rbtree_t* tree, rbnodeptr _F, rbnodeptr _L, set_rbtree_fun_t Freenode)
{
  if (size(tree) == 0 || _F != begin(tree) || _L != end(tree)) {
    while (_F != _L) {
      erase(tree, Inc_(_F), Freenode);
    }
    return (_F);
  } else {
    _Erase(_Root(tree), Freenode);
    _Root(tree) = _Nil, tree->_Size = 0;
    _Lmost(tree) = tree->_Head, _Rmost(tree) = tree->_Head;
    return (begin(tree));
  }
}

void clear(rbtree_t* tree, set_rbtree_fun_t Freenode)
{
  erase(tree, begin(tree), end(tree), Freenode);
  tree->_Head = 0, tree->_Size = 0;
}

rbnodeptr _Lbound(rbtree_t* tree, rbnodeptr _Kv, cmp_rbnode_fun_t key_compare)
{
  rbnodeptr _X = _Root(tree);
  rbnodeptr _Y = tree->_Head;
  while (_X != _Nil)
    if (key_compare(_Key(_X), _Kv)) {
      _X = _Right(_X);
    } else {
      _Y = _X, _X = _Left(_X);
    }
  return (_Y);
}
rbnodeptr _Ubound(rbtree_t* tree, rbnodeptr key, cmp_rbnode_fun_t key_compare)
{
  rbnodeptr _X = _Root(tree);
  rbnodeptr _Y = tree->_Head;
  while (_X != _Nil)
    if (key_compare(key, _Key(_X))) {
      _Y = _X, _X = _Left(_X);
    } else {
      _X = _Right(_X);
    }
  return (_Y);
}
rbnodeptr lower_bound(rbtree_t* tree, rbnodeptr _Kv, cmp_rbnode_fun_t key_compare)
{
  return (rbnodeptr(_Lbound(tree, _Kv, key_compare)));
}
rbnodeptr upper_bound(rbtree_t* tree, rbnodeptr _Kv, cmp_rbnode_fun_t key_compare)
{
  return (rbnodeptr(_Ubound(tree, _Kv, key_compare)));
}
_Pairii equal_range(rbtree_t* tree, rbnodeptr _Kv, cmp_rbnode_fun_t key_compare)
{
  return (_Pairii(lower_bound(tree, _Kv, key_compare), upper_bound(tree, _Kv, key_compare)));
}

void _Distance(rbnodeptr _F, rbnodeptr _L, size_t & _N)
{
  for (; _F != _L; _Inc(_F)) {
    ++_N;
  }
}

size_t erase(rbtree_t* tree, rbnodeptr _X, cmp_rbnode_fun_t key_compare, set_rbtree_fun_t Freenode)
{
  _Pairii _P = equal_range(tree, _X, key_compare);
  size_t _N = 0;
  _Distance(_P.first, _P.second, _N);
  erase(tree, _P.first, _P.second, Freenode);
  return (_N);
}
rbnodeptr find(rbtree_t* tree, rbnodeptr _Kv, cmp_rbnode_fun_t key_compare)
{
  rbnodeptr _P = lower_bound(tree, _Kv, key_compare);
  return (_P == end(tree)
          || key_compare(_Kv, _Key(_P))
          ? end(tree) : _P);
}
size_t count(rbtree_t* tree, rbnodeptr _Kv, cmp_rbnode_fun_t key_compare)
{
  _Pairii _Ans = equal_range(tree, _Kv, key_compare);
  size_t _N = 0;
  _Distance(_Ans.first, _Ans.second, _N);
  return (_N);
}
rbnodeptr _Copy(rbnodeptr _X, rbnodeptr _P, set_rbtree_fun_t Freenode)
{
  rbnodeptr _R = _X;
  for (; _X != _Nil; _X = _Left(_X)) {
    rbnodeptr _Y = _Buynode(_P, _Color(_X), _X);
    if (_R == _X) {
      _R = _Y;
    }
    _Right(_Y) = _Copy(_Right(_X), _Y, Freenode);
    _Copynode(_Key(_Y), _Key(_X));
    _Left(_P) = _Y;
    _P = _Y;
  }
  _Left(_P) = _Nil;
  return (_R);
}
void _Copy(rbtree_t* tree, const rbtree_t* _X, set_rbtree_fun_t Freenode)
{
  _Root(tree) = _Copy(_Root(_X), tree->_Head, Freenode);
  tree->_Size = size(_X);
  if (_Root(tree) != _Nil) {
    _Lmost(tree) = _Min(_Root(tree));
    _Rmost(tree) = _Max(_Root(tree));
  } else {
    _Lmost(tree) = tree->_Head, _Rmost(tree) = tree->_Head;
  }
}

int test_rbtree()
{
  return 0;
}

