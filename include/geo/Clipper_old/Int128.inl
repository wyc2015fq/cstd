
// Int128 class (enables safe math on signed 64bit integers)
// eg Int128 val1((cInt)9223372036854775807); //ie 2^63 -1
//    Int128 val2((cInt)9223372036854775807);
//    Int128 val3 = val1 * val2;
//    val3.AsString => "85070591730234615847396907784232501249" (8.5e+37)


class Int128
{
public:
  cUInt lo;
  cInt hi;

  Int128(cInt _lo = 0) {
    lo = (cUInt)_lo;

    if (_lo < 0) {
      hi = -1;
    }
    else {
      hi = 0;
    }
  }


  Int128(const Int128& val): lo(val.lo), hi(val.hi) {}

  Int128(const cInt& _hi, const uint64& _lo): lo(_lo), hi(_hi) {}

  Int128& operator = (const cInt& val) {
    lo = (uint64)val;

    if (val < 0) {
      hi = -1;
    }
    else {
      hi = 0;
    }

    return *this;
  }

  bool operator == (const Int128& val) const {
    return (hi == val.hi && lo == val.lo);
  }

  bool operator != (const Int128& val) const {
    return !(*this == val);
  }

  bool operator > (const Int128& val) const {
    if (hi != val.hi) {
      return hi > val.hi;
    }
    else {
      return lo > val.lo;
    }
  }

  bool operator < (const Int128& val) const {
    if (hi != val.hi) {
      return hi < val.hi;
    }
    else {
      return lo < val.lo;
    }
  }

  bool operator >= (const Int128& val) const {
    return !(*this < val);
  }

  bool operator <= (const Int128& val) const {
    return !(*this > val);
  }

  Int128& operator += (const Int128& rhs) {
    hi += rhs.hi;
    lo += rhs.lo;

    if (lo < rhs.lo) {
      hi++;
    }

    return *this;
  }

  Int128 operator + (const Int128& rhs) const {
    Int128 result(*this);
    result += rhs;
    return result;
  }

  Int128& operator -= (const Int128& rhs) {
    *this += -rhs;
    return *this;
  }

  Int128 operator - (const Int128& rhs) const {
    Int128 result(*this);
    result -= rhs;
    return result;
  }

  Int128 operator-() const { //unary negation
    if (lo == 0) {
      return Int128(-hi, 0);
    }
    else {
      return Int128(~hi, ~lo + 1);
    }
  }

  Int128 operator/ (const Int128& rhs) const {
    if (rhs.lo == 0 && rhs.hi == 0) {
      throw "Int128 operator/: divide by zero";
    }

    bool negate = (rhs.hi < 0) != (hi < 0);
    Int128 dividend = *this;
    Int128 divisor = rhs;

    if (dividend.hi < 0) {
      dividend = -dividend;
    }

    if (divisor.hi < 0) {
      divisor = -divisor;
    }

    if (divisor < dividend) {
      Int128 result = Int128(0);
      Int128 cntr = Int128(1);

      while (divisor.hi >= 0 && !(divisor > dividend)) {
        divisor.hi <<= 1;

        if ((cInt)divisor.lo < 0) {
          divisor.hi++;
        }

        divisor.lo <<= 1;

        cntr.hi <<= 1;

        if ((cInt)cntr.lo < 0) {
          cntr.hi++;
        }

        cntr.lo <<= 1;
      }

      divisor.lo >>= 1;

      if ((divisor.hi & 1) == 1) {
        divisor.lo |= 0x8000000000000000L;
      }

      divisor.hi = (uint64)divisor.hi >> 1;

      cntr.lo >>= 1;

      if ((cntr.hi & 1) == 1) {
        cntr.lo |= 0x8000000000000000L;
      }

      cntr.hi >>= 1;

      while (cntr.hi != 0 || cntr.lo != 0) {
        if (!(dividend < divisor)) {
          dividend -= divisor;
          result.hi |= cntr.hi;
          result.lo |= cntr.lo;
        }

        divisor.lo >>= 1;

        if ((divisor.hi & 1) == 1) {
          divisor.lo |= 0x8000000000000000L;
        }

        divisor.hi >>= 1;

        cntr.lo >>= 1;

        if ((cntr.hi & 1) == 1) {
          cntr.lo |= 0x8000000000000000L;
        }

        cntr.hi >>= 1;
      }

      if (negate) {
        result = -result;
      }

      return result;
    }
    else if (rhs.hi == this->hi && rhs.lo == this->lo) {
      return Int128(negate ? -1 : 1);
    }
    else {
      return Int128(0);
    }
  }

  double AsDouble() const {
    const double shift64 = 18446744073709551616.0; //2^64

    if (hi < 0) {
      cUInt lo_ = ~lo + 1;

      if (lo_ == 0) {
        return (double)hi * shift64;
      }
      else {
        return -(double)(lo_ + ~hi * shift64);
      }
    }
    else {
      return (double)(lo + hi * shift64);
    }
  }

};


Int128 Int128Mul(cInt lhs, cInt rhs)
{
  bool negate = (lhs < 0) != (rhs < 0);

  if (lhs < 0) {
    lhs = -lhs;
  }

  uint64 int1Hi = uint64(lhs) >> 32;
  uint64 int1Lo = uint64(lhs & 0xFFFFFFFF);

  if (rhs < 0) {
    rhs = -rhs;
  }

  uint64 int2Hi = uint64(rhs) >> 32;
  uint64 int2Lo = uint64(rhs & 0xFFFFFFFF);

  //nb: see comments in clipper.pas
  uint64 a = int1Hi * int2Hi;
  uint64 b = int1Lo * int2Lo;
  uint64 c = int1Hi * int2Lo + int1Lo * int2Hi;

  Int128 tmp;
  tmp.hi = cInt(a + (c >> 32));
  tmp.lo = cInt(c << 32);
  tmp.lo += cInt(b);

  if (tmp.lo < b) {
    tmp.hi++;
  }

  if (negate) {
    tmp = -tmp;
  }

  return tmp;
};