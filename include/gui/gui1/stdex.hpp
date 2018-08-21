

namespace std
{


  // FUNCTION TEMPLATE clamp
  template < class _Ty,
           class _Pr >
  constexpr const _Ty & clamp(const _Ty & _Val, const _Ty & _Min_val,
                              const _Ty & _Max_val, _Pr _Pred)
  {
    // returns _Val constrained to [_Min_val, _Max_val] ordered by _Pred
#if _ITERATOR_DEBUG_LEVEL == 2
    return (_DEBUG_LT_PRED(_Pred, _Max_val, _Min_val)
            ? (_DEBUG_ERROR("invalid bounds arguments passed to std::clamp"), _Val)
            : _DEBUG_LT_PRED(_Pred, _Max_val, _Val)
            ? _Max_val
            : _DEBUG_LT_PRED(_Pred, _Val, _Min_val)
            ? _Min_val
            : _Val);
#else /* ^^^ _ITERATOR_DEBUG_LEVEL == 2 ^^^ // vvv _ITERATOR_DEBUG_LEVEL != 2 vvv */
    return (_Pred(_Max_val, _Val)
            ? _Max_val
            : _Pred(_Val, _Min_val)
            ? _Min_val
            : _Val);
#endif /* _ITERATOR_DEBUG_LEVEL == 2 */
  }

  template<class _Ty>
  constexpr const _Ty & clamp(const _Ty & _Val, const _Ty & _Min_val,
                              const _Ty & _Max_val)
  {
    // returns _Val constrained to [_Min_val, _Max_val]
    return (_STD clamp(_Val, _Min_val, _Max_val, less<>()));
  }

}