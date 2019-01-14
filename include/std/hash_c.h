
#include <limits.h>
#include <stdlib.h>
// TEMPLATE FUNCTION hash_value
#define _HASH_SEED	(size_t)0xdeadbeef

size_t hash_value(size_t _Keyval)
{
  // hash _Keyval to size_t value one-to-one
  return ((size_t)_Keyval ^ _HASH_SEED);
}

size_t hash_compare( size_t _Keyval)
{
  // hash _Keyval to size_t value by pseudorandomizing transform
  long _Quot = (long)(hash_value(_Keyval) & LONG_MAX);
  ldiv_t _Qrem = ldiv(_Quot, 127773);
  _Qrem.rem = 16807 * _Qrem.rem - 2836 * _Qrem.quot;
  if (_Qrem.rem < 0) {
    _Qrem.rem += LONG_MAX;
  }
  return ((size_t)_Qrem.rem);
}

typedef int(*cmp_fun_t)( void* key1,  void* key2);

struct _HashNode {
  struct _HashNode* next;
};

enum {
  // various constants
  _Min_buckets = 8,	// min_buckets = 2 ^^ N, 0 < N
};

struct _Hash {
};
cmp_fun_t _Traitsobj;	// traits to customize behavior
_HashNode* _List;	// list of elements, must initialize before _Vec
_HashNode** _Vec;	// vector of list iterators, begin() then end()-1
size_t _VecSize;
bool _Multi;
size_t _Mask;	// the key mask
size_t _Maxidx;	// current maximum key value

typedef _HashNode* iterator;

iterator begin()
{
  // return iterator for beginning of mutable sequence
  return (_List);
}

const_iterator begin()
{
  // return iterator for beginning of nonmutable sequence
  return (_List.begin());
}

iterator end()
{
  // return iterator for end of mutable sequence
  return (_List.end());
}

const_iterator end()
{
  // return iterator for end of nonmutable sequence
  return (_List.end());
}

_Unchecked_iterator _Unchecked_begin()
{
  // return iterator for beginning of mutable sequence
  return (_List._Unchecked_begin());
}

_Unchecked_const_iterator _Unchecked_begin()
{
  // return iterator for beginning of nonmutable sequence
  return (_List._Unchecked_begin());
}

_Unchecked_iterator _Unchecked_end()
{
  // return iterator for end of mutable sequence
  return (_List._Unchecked_end());
}

_Unchecked_const_iterator _Unchecked_end()
{
  // return iterator for end of nonmutable sequence
  return (_List._Unchecked_end());
}

iterator _Make_iter(_Unchecked_const_iterator _Where)
{
  // make iterator from _Unchecked_const_iterator
  return (_List._Make_iter(_Where));
}

iterator _Make_iter(const_iterator _Where)
{
  // make iterator from const_iterator
  return (_List._Make_iter(_Where));
}

const_iterator cbegin()
{
  // return iterator for beginning of nonmutable sequence
  return (begin());
}

const_iterator cend()
{
  // return iterator for end of nonmutable sequence
  return (end());
}

size_t size()
{
  // return length of sequence
  return (_List.size());
}

size_t max_size()
{
  // return maximum possible length of sequence
  return (_List.max_size());
}

bool empty()
{
  // return true only if sequence is empty
  return (_List.empty());
}

allocator_type get_allocator()
{
  // return allocator object for values
  allocator_type _Ret(_List.get_allocator());
  return (_Ret);
}

key_compare key_comp()
{
  // return object for comparing keys
  return (_Traitsobj);
}

value_compare value_comp()
{
  // return object for comparing values
  return (value_compare(key_comp()));
}

typedef iterator local_iterator;
typedef const_iterator const_local_iterator;

size_t bucket_count()
{
  // return number of buckets
  return (_Maxidx);
}

size_t max_bucket_count()
{
  // return maximum number of buckets
  return (_Vec.max_size() / 2);
}

size_t bucket( key_type & _Keyval)
{
  // return bucket corresponding to _Key
  return (_Hashval(_Keyval));
}

size_t bucket_size(size_t _Bucket)
{
  // return size of bucket _Bucket
  size_t _Ans = 0;
  if (_Bucket < _Maxidx)
    for (_Unchecked_const_iterator _Plist = _Begin(_Bucket);
         _Plist != _End(_Bucket); ++_Plist) {
      ++_Ans;
    }
  return (_Ans);
}

local_iterator begin(size_t _Bucket)
{
  // return iterator for bucket _Bucket
  if (_Bucket < bucket_count()) {
    return (_Make_iter(_Begin(_Bucket)));
  } else {
    return (end());
  }
}

const_local_iterator begin(size_t _Bucket)
{
  // return iterator for bucket _Bucket
  if (_Bucket < bucket_count()) {
    return (_Make_iter(_Begin(_Bucket)));
  } else {
    return (end());
  }
}

local_iterator end(size_t _Bucket)
{
  // return iterator for bucket following _Bucket
  if (_Bucket < bucket_count()) {
    return (_Make_iter(_End(_Bucket)));
  } else {
    return (end());
  }
}

const_local_iterator end(size_t _Bucket)
{
  // return iterator for bucket following _Bucket
  if (_Bucket < bucket_count()) {
    return (_Make_iter(_End(_Bucket)));
  } else {
    return (end());
  }
}

const_local_iterator cbegin(size_t _Bucket)
{
  // return iterator for bucket _Bucket
  if (_Bucket < bucket_count()) {
    return (_Make_iter(_Begin(_Bucket)));
  } else {
    return (end());
  }
}

const_local_iterator cend(size_t _Bucket)
{
  // return iterator for bucket following _Bucket
  if (_Bucket < bucket_count()) {
    return (_Make_iter(_End(_Bucket)));
  } else {
    return (end());
  }
}

float load_factor()
{
  // return elements per bucket
  return ((float)size() / (float)bucket_count());
}

float max_load_factor = 0.75;


void rehash(size_t _Buckets)
{
  // rebuild table with at least _Buckets buckets
  size_t _Maxsize = _Vec.max_size() / 4;
  size_t _Newsize = _Min_buckets;
  for (; _Newsize < _Buckets && _Newsize < _Maxsize; ) {
    _Newsize *= 2;  // double until big enough
  }
  if (_Newsize < _Buckets) {
    _Xout_of_range("invalid hash bucket count");
  }
  while (!(size() / max_load_factor() < _Newsize)
         && _Newsize < _Maxsize) {
    _Newsize *= 2;  // double until load factor okay
  }
  _Init(_Newsize);
  _Reinsert();
}

void reserve(size_t _Maxcount)
{
  // rebuild table with room for _Maxcount elements
  rehash((size_t)((float)(_Maxcount / max_load_factor() + 0.5F)));
}

_Pairib insert( value_type & _Val)
{
  // try to insert node with value _Val
  return (_Insert(_Val, _Nil()));
}

iterator insert( value_type & _Val)
{
  // try to insert node with value _Val
  return (_Insert(_Val, _Nil()).first);
}

_Pairib insert(value_type && _Val)
{
  // try to insert node with value _Val, favoring right side
  return (_Insert(_STD forward<value_type>(_Val), _Nil()));
}

iterator insert(value_type && _Val)
{
  // try to insert node with value _Val, favoring right side
  return (_Insert(_STD forward<value_type>(_Val), _Nil()).first);
}

iterator insert(const_iterator,
                value_type & _Val)
{
  // try to insert node with value _Val, ignore hint
  return (_Insert(_Val, _Nil()).first);
}

iterator insert(const_iterator, value_type && _Val)
{
  // try to insert node with value _Val, ignore hint
  return (_Insert(_STD forward<value_type>(_Val), _Nil()).first);
}

template<class _Iter>
void insert(_Iter _First, _Iter _Last)
{
  // insert [_First, _Last) at front, then put in place
  _DEBUG_RANGE(_First, _Last);
  for (; _First != _Last; ++_First) {
    emplace(*_First);
  }
}

void insert(_XSTD initializer_list<value_type> _Ilist)
{
  // insert initializer_list
  insert(_Ilist.begin(), _Ilist.end());
}

iterator erase(const_iterator _Plist)
{
  // erase element at _Plist
  size_t _Bucket = _Hashval(cmp_fun_t::_Kfn(*_Plist));
  _Erase_bucket(_Make_iter(_Plist), _Bucket);
  return (_List.erase(_Plist));
}

iterator erase(const_iterator _First, const_iterator _Last)
{
  // erase [_First, _Last)
  _DEBUG_RANGE(_First, _Last);
  if (_First == begin() && _Last == end()) {
    // erase all
    clear();
    return (begin());
  } else {
    // partial erase, one at a time
    while (_First != _Last) {
      erase(_First++);
    }
    return (_Make_iter(_First));
  }
}

size_t erase( key_type & _Keyval)
{
  // erase and count all that match _Keyval
  _Pairii _Where = equal_range(_Keyval);
  size_t _Num = _STD distance(_Where.first, _Where.second);
  erase(_Where.first, _Where.second);
  return (_Num);
}

void clear()
{
  // erase all
  _List.clear();
  _Init();
}

iterator find( key_type & _Keyval)
{
  // find an element in mutable hash table that matches _Keyval
  return (lower_bound(_Keyval));
}

const_iterator find( key_type & _Keyval)
{
  // find an element in nonmutable hash table that matches _Keyval
  return (lower_bound(_Keyval));
}

size_t count( key_type & _Keyval)
{
  // count all elements that match _Keyval
  _Paircc _Ans = equal_range(_Keyval);
  return (_STD distance(_Ans.first, _Ans.second));
}

iterator lower_bound( key_type & _Keyval)
{
  // find leftmost not less than _Keyval in mutable hash table
  size_t _Bucket = _Hashval(_Keyval);
  for (_Unchecked_iterator _Where = _Begin(_Bucket);
       _Where != _End(_Bucket); ++_Where)
    if (!_Traitsobj(cmp_fun_t::_Kfn(*_Where), _Keyval))
      return (_Traitsobj(_Keyval,
                         cmp_fun_t::_Kfn(*_Where)) ? end() : _Make_iter(_Where));
  return (end());
}

const_iterator lower_bound( key_type & _Keyval)
{
  // find leftmost not less than _Keyval in nonmutable hash table
  size_t _Bucket = _Hashval(_Keyval);
  for (_Unchecked_const_iterator _Where = _Begin(_Bucket);
       _Where != _End(_Bucket); ++_Where)
    if (!_Traitsobj(cmp_fun_t::_Kfn(*_Where), _Keyval))
      return (_Traitsobj(_Keyval,
                         cmp_fun_t::_Kfn(*_Where)) ? end() : _Make_iter(_Where));
  return (end());
}

iterator upper_bound( key_type & _Keyval)
{
  // find leftmost not greater than _Keyval in mutable hash table
  size_t _Bucket = _Hashval(_Keyval);
  for (_Unchecked_iterator _Where = _End(_Bucket);
       _Where != _Begin(_Bucket); )
    if (!_Traitsobj(_Keyval, cmp_fun_t::_Kfn(*--_Where)))
      return (_Traitsobj(cmp_fun_t::_Kfn(*_Where),
                         _Keyval) ? end() : _Make_iter(++_Where));
  return (end());
}

const_iterator upper_bound( key_type & _Keyval)
{
  // find leftmost not greater than _Keyval in nonmutable hash table
  size_t _Bucket = _Hashval(_Keyval);
  for (_Unchecked_const_iterator _Where = _End(_Bucket);
       _Where != _Begin(_Bucket); )
    if (!_Traitsobj(_Keyval, cmp_fun_t::_Kfn(*--_Where)))
      return (_Traitsobj(cmp_fun_t::_Kfn(*_Where),
                         _Keyval) ? end() : _Make_iter(++_Where));
  return (end());
}

_Pairii equal_range( key_type & _Keyval)
{
  // find range equivalent to _Keyval in mutable hash table
  size_t _Bucket = _Hashval(_Keyval);
  for (_Unchecked_iterator _Where = _Begin(_Bucket);
       _Where != _End(_Bucket); ++_Where)
    if (!_Traitsobj(cmp_fun_t::_Kfn(*_Where), _Keyval)) {
      // found _First, look for end of range
      _Unchecked_iterator _First = _Where;
      for (; _Where != _End(_Bucket); ++_Where)
        if (_Traitsobj(_Keyval, cmp_fun_t::_Kfn(*_Where))) {
          break;
        }
      if (_First == _Where) {
        break;
      }
      return (_Pairii(_Make_iter(_First),
                      _Make_iter(_Where)));
    }
  return (_Pairii(end(), end()));
}

_Paircc equal_range( key_type & _Keyval)
{
  // find range equivalent to _Keyval in nonmutable hash table
  size_t _Bucket = _Hashval(_Keyval);
  for (_Unchecked_const_iterator _Where = _Begin(_Bucket);
       _Where != _End(_Bucket); ++_Where)
    if (!_Traitsobj(cmp_fun_t::_Kfn(*_Where), _Keyval)) {
      // found _First, look for end of range
      _Unchecked_const_iterator _First = _Where;
      for (; _Where != _End(_Bucket); ++_Where)
        if (_Traitsobj(_Keyval, cmp_fun_t::_Kfn(*_Where))) {
          break;
        }
      if (_First == _Where) {
        break;
      }
      return (_Paircc(_Make_iter(_First),
                      _Make_iter(_Where)));
    }
  return (_Paircc(end(), end()));
}

void swap(_Myt & _Right)
{
  // exchange contents with _Right
  if (this != &_Right) {
    // different, worth swapping
    _Swap_adl(_Traitsobj, _Right._Traitsobj);
    this->_List.swap(_Right._List);
    this->_Vec.swap(_Right._Vec);
    _Swap_adl(this->_Mask, _Right._Mask);
    _Swap_adl(this->_Maxidx, _Right._Maxidx);
  }
}

protected:
template<class _Valty>
_Unchecked_iterator _Buynode_if_nil(_Valty && ,
                                    _Unchecked_iterator _Plist)
{
  // node exists, just return it
  return (_Plist);
}

template<class _Valty>
_Unchecked_iterator _Buynode_if_nil(_Valty && _Val, _Nil)
{
  // node doesn't exist, make it
  _List.push_front(_STD forward<_Valty>(_Val));
  return (_Unchecked_begin());
}

void _Destroy_if_not_nil(_Unchecked_iterator _Plist)
{
  // node exists, destroy it
  _List.erase(_Make_iter(_Plist));
}

void _Destroy_if_not_nil(_Nil)
{
  // node doesn't exist, do nothing
}

template < class _Valty,
         class _Nodety >
_Pairib _Insert(_Valty && _Val, _Nodety _Pnode)
{
  // try to insert existing node with value _Val
  size_t _Bucket;
  _Unchecked_iterator _Where;

  _TRY_BEGIN
  _Bucket = _Hashval(cmp_fun_t::_Kfn(_Val));
  _Where = _End(_Bucket);
  for (; _Where != _Begin(_Bucket); )
    if (_Traitsobj(cmp_fun_t::_Kfn(_Val),
                   cmp_fun_t::_Kfn(*--_Where)))
      ;	// still too high in bucket list
    else if (_Multi
             || (!cmp_fun_t::_Standard
                 && _Traitsobj(cmp_fun_t::_Kfn(*_Where),
                               cmp_fun_t::_Kfn(_Val)))) {
      // found insertion point, back up to it
      ++_Where;
      break;
    } else {
      // discard new list element and return existing
      _Destroy_if_not_nil(_Pnode);
      return (_Pairib(_Make_iter(_Where), false));
    }
  _CATCH_ALL
  _Destroy_if_not_nil(_Pnode);
  _RERAISE;
  _CATCH_END

  _Unchecked_iterator _Plist =
    _Buynode_if_nil(_STD forward<_Valty>(_Val), _Pnode);
  _Unchecked_iterator _Next = _Plist;

  if (_Where != ++_Next) {	// move element into place
    _List._Unchecked_splice(_Where, _Plist, _Next);
  }

  _Insert_bucket(_Plist, _Where, _Bucket);

  _TRY_BEGIN
  _Check_size();
  _CATCH_ALL
  erase(_Make_iter(_Plist));
  _RERAISE;
  _CATCH_END

  return (_Pairib(_Make_iter(_Plist), true));
}

_Unchecked_iterator & _Vec_lo(size_t _Bucket)
{
  // return reference to begin() for _Bucket
  return (_Vec[2 * _Bucket]);
}

_Unchecked_const_iterator & _Vec_lo(size_t _Bucket)
{
  // return reference to begin() for _Bucket
  return ((_Unchecked_const_iterator &)_Vec[2 * _Bucket]);
}

_Unchecked_iterator & _Vec_hi(size_t _Bucket)
{
  // return reference to end()-1 for _Bucket
  return (_Vec[2 * _Bucket + 1]);
}

_Unchecked_const_iterator & _Vec_hi(size_t _Bucket)
{
  // return reference to end()-1 for _Bucket
  return ((_Unchecked_const_iterator &)_Vec[2 * _Bucket + 1]);
}

_Unchecked_iterator _Begin(size_t _Bucket)
{
  // return begin iterator for bucket _Bucket
  return (_Vec_lo(_Bucket));
}

_Unchecked_const_iterator _Begin(size_t _Bucket)
{
  // return begin iterator for bucket _Bucket
  return (_Vec_lo(_Bucket));
}

_Unchecked_iterator _End(size_t _Bucket)
{
  // return end iterator for bucket _Bucket
  if (_Vec_lo(_Bucket) == _Unchecked_end()) {
    return (_Unchecked_end());
  } else {
    // point past last element
    _Unchecked_iterator _Ans = _Vec_hi(_Bucket);
    return (++_Ans);
  }
}
_Unchecked_const_iterator _End(size_t _Bucket)
{
  // return end iterator for bucket _Bucket
  if (_Vec_lo(_Bucket) == _Unchecked_end()) {
    return (_Unchecked_end());
  } else {
    // point past last element
    _Unchecked_const_iterator _Ans = _Vec_hi(_Bucket);
    return (++_Ans);
  }
}

void _Erase_bucket(iterator _Plist_arg, size_t _Bucket)
{
  // fix iterators before erasing _Plist before _Where
  _Unchecked_iterator _Plist = _Plist_arg._Unchecked();
  if (_Vec_hi(_Bucket) == _Plist)
    if (_Vec_lo(_Bucket) == _Plist) {
      // make bucket empty
      _Vec_lo(_Bucket) = _Unchecked_end();
      _Vec_hi(_Bucket) = _Unchecked_end();
    } else {
      _Vec_hi(_Bucket) = --_Plist;  // move end back one element
    }
  else if (_Vec_lo(_Bucket) == _Plist) {
    _Vec_lo(_Bucket) = ++_Plist;  // move beginning up one element
  }
}

void _Insert_bucket(_Unchecked_iterator _Plist,
                    _Unchecked_iterator _Where, size_t _Bucket)
{
  // fix iterators after inserting _Plist before _Where
  if (_Vec_lo(_Bucket) == _Unchecked_end()) {
    // make bucket non-empty
    _Vec_lo(_Bucket) = _Plist;
    _Vec_hi(_Bucket) = _Plist;
  } else if (_Vec_lo(_Bucket) == _Where) {
    _Vec_lo(_Bucket) = _Plist;  // move beginning back one element
  } else if (++_Vec_hi(_Bucket) != _Plist) {	// move end up one element
    --_Vec_hi(_Bucket);  // or not
  }
}

void _Copy( _Myt & _Right)
{
  // copy entire hash table
  _Mask = _Right._Mask;
  _Maxidx = _Right._Maxidx;
  _List.clear();
  _TRY_BEGIN
  _Traitsobj = _Right._Traitsobj;
  _Vec.assign(_Right._Vec.size(), _Unchecked_end());
  insert(_Right.begin(), _Right.end());
  _CATCH_ALL
  clear();	// list or compare copy failed, bail out
  _RERAISE;
  _CATCH_END
}

size_t _Hashval( key_type & _Keyval)
{
  // return hash value, masked to current table size
  return (_Traitsobj(_Keyval) & _Mask);
}

void _Init(size_t _Buckets = _Min_buckets)
{
  // initialize hash table with _Buckets buckets, leave list alone
  _Vec.reserve(2 * _Buckets);	// avoid curdling _Vec if exception occurs
  _Vec.assign(2 * _Buckets, _Unchecked_end());
  _Mask = _Buckets - 1;
  _Maxidx = _Buckets;
}

void _Check_size()
{
  // grow table as needed
  if (max_load_factor() < load_factor()) {
    // rehash to bigger table
    size_t _Newsize = bucket_count();
    if (_Newsize < 512) {
      _Newsize *= 8;  // multiply by 8
    } else if (_Newsize < _Vec.max_size() / 2) {
      _Newsize *= 2;  // multiply safely by 2
    }
    _Init(_Newsize);
    _Reinsert();
  }
}

void _Reinsert()
{
  // insert elements in [begin(), end())
  _Unchecked_iterator _Last = _Unchecked_end();
  if (_Unchecked_begin() != _Last)
    for (--_Last; ; ) {
      // reinsert elements in [begin(), _Last]
      _Unchecked_iterator _First = _Unchecked_begin();
      bool _Done = _First == _Last;
      _Insert(*_First, _First);
      if (_Done) {
        break;
      }
    }
}
