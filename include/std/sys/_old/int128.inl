// File:Int128.h

struct _int128 {
  unsigned char lo;
  unsigned char hi;
};

template <typename T>
void kara_mul(T* c1, T* c0, const T* a, const T* b) {
  enum {half_shift = sizeof(T)*4, half_mask = (1<<half_shift) - 1};
  T a0 = half_mask&a, a1 = a>>half_shift;
  T b0 = half_mask&b, b1 = b>>half_shift;
  T t0 = a0*b0, t1 = a1*b1, t2;
  t2 = (t1 + t0 + (a1-a0)(b0-b1))*B^n;
  *c0 = t0 + (t2&half_mask);
  *c1 = t1 + (t2>>half_shift);
}
int int128sum( _int128* dst, const _int128 *x, const _int128 *y, int c) {
  dst->lo = x->lo + y->lo + c;
  dst->hi = x->hi + y->hi + (dst->lo < x->lo);
  return (dst->hi < x->hi);
}
int int128dif( _int128* dst, const _int128 *x, const _int128 *y, int c) {
  dst->lo = x->lo - y->lo - c;
  dst->hi = x->hi - y->hi - (dst->lo > x->lo);
  return (dst->hi > x->hi);
}
int int128mul( _int128* dst, const _int128 *x, const _int128 *y, uint64 c) {
  return 0;
}
inline _int128 operator & (const _int128 &lft, const _int128 &rhs) {
  _int128 dst = {lft.lo&rhs.lo, lft.hi&rhs.hi};
  return dst;
}
inline _int128 operator | (const _int128 &lft, const _int128 &rhs) {
  _int128 dst = {lft.lo|rhs.lo, lft.hi|rhs.hi};
  return dst;
}
inline _int128 operator ^ (const _int128 &lft, const _int128 &rhs) {
  _int128 dst = {lft.lo^rhs.lo, lft.hi^rhs.hi};
  return dst;
}
inline _int128 operator ~ (const _int128 &rhs) {
  _int128 dst = {~rhs.lo, ~rhs.hi};
  return dst;
}
inline _int128 operator + (const _int128 &lft, const _int128 &rhs) {
  _int128 dst;
  int128sum(&dst, &lft, &rhs, 0);
  return dst;
}

inline _int128 operator - (const _int128 &lft, const _int128 &rhs) {
  _int128 dst;
#if 0
  _int128 rhs2 = {~rhs.lo, ~rhs.hi};
  int128sum(&dst, &lft, &rhs2, 1);
#else
  int128dif(&dst, &lft, &rhs, 0);
#endif
  return dst;
}


inline bool operator==(const _int128 &lft, const _int128 &rhs) {
  return (lft.lo == rhs.lo) && (lft.hi == rhs.hi);
}


#if 0
struct _int128;
struct _uint128;
extern "C" {    
  void int128sum( void *dst, const void *x, const void *y);
  void int128dif( void *dst, const void *x, const void *y);
  void int128mul( void *dst, const void *x, const void *y);
  void int128div( void *dst, const void *x, const void *y);
  void int128rem( void *dst, const void *x, const void *y);
  void int128neg( void *dst, const void *x);
  int  int128cmp(const void *n1, const void *n2);
  void uint128div( void *dst, const void *x, const void *y);
  void uint128rem( void *dst, const void *x, const void *y);
  int  uint128cmp(const void *n1, const void *n2);
};

struct _int128 {
private:
  _int128(unsigned __int64 _lo, const unsigned __int64 _hi) : lo(_lo), hi(_hi) {
  }
public:
  unsigned __int64 lo;
  unsigned __int64 hi;

  inline _int128() {
  }
  inline _int128(unsigned __int64 n) : lo(n), hi(0) {
  }
  inline _int128(__int64 n) : lo(n), hi((n>=0)?0:-1) { // remember signextend hi if n < 0 (2-complement)
  }
  inline _int128(unsigned int n) : lo(n), hi(0) {
  }
  inline _int128(int n) : lo(n), hi((n>=0)?0:-1) {
  }
  inline _int128(unsigned short n) : lo(n), hi(0) {
  }
  inline _int128(short n) : lo(n), hi((n>=0)?0:-1) {
  }
  explicit _int128(const char *str);

  operator unsigned __int64() const {
    return lo;
  }
  operator __int64() const {
    return lo;
  }
  operator unsigned int() const {
    return (unsigned int)lo;
  }
  operator int() const {
    return (int)lo;
  }
  inline _int128 operator+(const _int128 &rhs) const {
    _int128 result;
    int128sum(&result, this, &rhs);
    return result;
  }

  inline _int128 operator-(const _int128 &rhs) const {
    _int128 result;
    int128dif(&result, this, &rhs);
    return result;
  }

  inline _int128 operator-() const {
    _int128 result;
    int128neg(&result, this);
    return result;
  }
  inline _int128 operator*(const _int128 &rhs) const {
    _int128 result;
    int128mul(&result, this, &rhs);
    return result;
  }

  inline _int128 operator/(const _int128 &rhs) const {
    _int128 result, copy(*this);
    int128div(&result, &copy, &rhs);
    return result;
  }
  inline _int128 operator%(const _int128 &rhs) const {
    _int128 result, copy(*this);
    int128rem(&result, &copy, &rhs);
    return result;
  };

  inline _int128 &operator+=(const _int128 &rhs) {
    const _int128 copy(*this);
    int128sum(this, &copy, &rhs);
    return *this;
  }
  inline _int128 &operator-=(const _int128 &rhs) {
    const _int128 copy(*this);
    int128dif(this, &copy, &rhs);
    return *this;
  }
  inline _int128 &operator*=(const _int128 &rhs) {
    const _int128 copy(*this);
    int128mul(this, &copy, &rhs);
    return *this;
  }
  inline _int128 &operator/=(const _int128 &rhs) {
    const _int128 copy(*this);
    int128div(this, &copy, &rhs);
    return *this;
  }
  inline _int128 &operator%=(const _int128 &rhs) {
    const _int128 copy(*this);
    int128rem(this, &copy, &rhs);
    return *this;
  }

  inline _int128 operator&(const _int128 &rhs) const {
    return _int128(lo&rhs.lo, hi&rhs.hi);
  }
  inline _int128 operator|(const _int128 &rhs) const {
    return _int128(lo|rhs.lo, hi|rhs.hi);
  }
  inline _int128 operator^(const _int128 &rhs) const {
    return _int128(lo^rhs.lo, hi^rhs.hi);
  }

  const char *parseDec(const char *str); // return pointer to char following the number
  const char *parseHex(const char *str); // do
  const char *parseOct(const char *str); // do
};

struct _uint128 {
public:
  unsigned __int64 lo;
  unsigned __int64 hi;

  inline _uint128() {
  }
  inline _uint128(const _int128 &n) : lo(n.lo), hi(n.hi) {
  }
  inline _uint128(unsigned __int64 n) : lo(n), hi(0) {
  }
  inline _uint128(__int64 n) : lo(n), hi((n>=0)?0:-1) {
  }
  inline _uint128(unsigned int n) : lo(n), hi(0) {
  }
  inline _uint128(int n) : lo(n), hi((n>=0)?0:-1) {
  }
  inline _uint128(unsigned short n) : lo(n), hi(0) {
  }
  inline _uint128(short n) : lo(n), hi((n>=0)?0:-1) {
  }
  explicit _uint128(const char *str);

  inline operator _int128() const {
    return *(_int128*)(void*)this;
  }
  inline operator unsigned __int64() const {
    return lo;
  }
  inline operator __int64() const {
    return lo;
  }
  inline operator unsigned int() const {
    return (unsigned int)lo;
  }
  inline operator int() const {
    return (int)lo;
  }

  inline _uint128 operator+(const _uint128 &rhs) const {
    _uint128 result;
    int128sum(&result, this, &rhs);
    return result;
  }

  inline _uint128 operator-(const _uint128 &rhs) const {
    _uint128 result;
    int128dif(&result, this, &rhs);
    return result;
  }

  inline _uint128 operator*(const _uint128 &rhs) const {
    _uint128 result;
    int128mul(&result, this, &rhs);
    return result;
  }

  inline _uint128 operator/(const _uint128 &rhs) const {
    _uint128 result, copy(*this);
    uint128div(&result, &copy, &rhs);
    return result;
  }

  inline _uint128 operator%(const _uint128 &rhs) const {
    _uint128 result, copy(*this);
    uint128rem(&result, &copy, &rhs);
    return result;
  };

  inline _uint128 &operator+=(const _uint128 &rhs) {
    const _uint128 copy(*this);
    int128sum(this, &copy, &rhs);
    return *this;
  }
  inline _uint128 &operator-=(const _uint128 &rhs) {
    const _uint128 copy(*this);
    int128dif(this, &copy, &rhs);
    return *this;
  }
  inline _uint128 &operator*=(const _uint128 &rhs) {
    const _uint128 copy(*this);
    int128mul(this, &copy, &rhs);
    return *this;
  }
  inline _uint128 &operator/=(const _uint128 &rhs) {
    const _uint128 copy(*this);
    uint128div(this, &copy, &rhs);
    return *this;
  }
  inline _uint128 &operator%=(const _uint128 &rhs) {
    const _uint128 copy(*this);
    uint128rem(this, &copy, &rhs);
    return *this;
  }
  const char *parseDec(const char *str); // return pointer to char following the number
  const char *parseHex(const char *str); // do
  const char *parseOct(const char *str); // do

};

inline bool operator==(const _int128 &lft, const _int128 &rhs) {
  return (lft.lo == rhs.lo) && (lft.hi == rhs.hi);
}
inline bool operator==(const _int128 &lft, const _uint128 &rhs) {
  return (lft.lo == rhs.lo) && (lft.hi == rhs.hi);
}
inline bool operator==(const _uint128 &lft, const _int128 &rhs) {
  return (lft.lo == rhs.lo) && (lft.hi == rhs.hi);
}
inline bool operator==(const _uint128 &lft, const _uint128 &rhs) {
  return (lft.lo == rhs.lo) && (lft.hi == rhs.hi);
}
inline bool operator!=(const _int128 &lft, const _int128 &rhs) {
  return (lft.lo != rhs.lo) || (lft.hi != rhs.hi);
}
inline bool operator!=(const _int128 &lft, const _uint128 &rhs) {
  return (lft.lo != rhs.lo) || (lft.hi != rhs.hi);
}
inline bool operator!=(const _uint128 &lft, const _int128 &rhs) {
  return (lft.lo != rhs.lo) || (lft.hi != rhs.hi);
}
inline bool operator!=(const _uint128 &lft, const _uint128 &rhs) {
  return (lft.lo != rhs.lo) || (lft.hi != rhs.hi);
}

inline bool operator>(const _int128 &lft, const _int128 &rhs) {
  return int128cmp(&lft, &rhs) > 0;
}
inline bool operator>(const _int128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) > 0;
}
inline bool operator>(const _uint128 &lft, const _int128 &rhs) {
  return uint128cmp(&lft, &rhs) > 0;
}
inline bool operator>(const _uint128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) > 0;
}

inline bool operator>=(const _int128 &lft, const _int128 &rhs) {
  return int128cmp(&lft, &rhs) >= 0;
}
inline bool operator>=(const _int128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) >= 0;
}
inline bool operator>=(const _uint128 &lft, const _int128 &rhs) {
  return uint128cmp(&lft, &rhs) >= 0;
}
inline bool operator>=(const _uint128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) >= 0;
}

inline bool operator<(const _int128 &lft, const _int128 &rhs) {
  return int128cmp(&lft, &rhs) < 0;
}
inline bool operator<(const _int128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) < 0;
}
inline bool operator<(const _uint128 &lft, const _int128 &rhs) {
  return uint128cmp(&lft, &rhs) < 0;
}
inline bool operator<(const _uint128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) < 0;
}

inline bool operator<=(const _int128 &lft, const _int128 &rhs) {
  return int128cmp(&lft, &rhs) <= 0;
}
inline bool operator<=(const _int128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) <= 0;
}
inline bool operator<=(const _uint128 &lft, const _int128 &rhs) {
  return uint128cmp(&lft, &rhs) <= 0;
}
inline bool operator<=(const _uint128 &lft, const _uint128 &rhs) {
  return uint128cmp(&lft, &rhs) <= 0;
}

char    * _i128toa(_int128 value, char *str, int radix);
char    * _ui128toa(_uint128 value, char *str, int radix);
wchar_t * _i128tow(_int128 value, wchar_t *str, int radix);
wchar_t * _ui128tow(_uint128 value, wchar_t *str, int radix);

inline char radixLetter(unsigned int c) {
  return (c < 10) ? ('0' + c) : ('a' + (c-10));
}

inline wchar_t wradixLetter(unsigned int c) {
  return (c < 10) ? ('0' + c) : ('a' + (c-10));
}

inline bool isodigit(unsigned char ch) {
  return ('0' <= ch) && (ch < '8');
}

unsigned int convertNumberChar(char digit);

#if 0
; File: Int128x64.asm
; build obj-file with
; ml64 /nologo /c /Zf /Fo$(IntDir)Int128x64.obj Int128x64.asm
.CODE

;void int128sum(_int128 &dst, cnost _int128 &x, const _int128 &y);
int128sum PROC
    push    rbx
    mov     rax, qword ptr[rdx]
    add     rax, qword ptr[r8]
    mov     rbx, qword ptr[rdx+8]
    adc     rbx, qword ptr[r8+8]
    mov     qword ptr[rcx], rax
    mov     qword ptr[rcx+8], rbx
    pop     rbx
    ret
int128sum ENDP

;void int128dif( _int128 &dst, const _int128 &x, const _int128 &y);
int128dif PROC
    push    rbx
    mov     rax, qword ptr[rdx]
    sub     rax, qword ptr[r8]
    mov     rbx, qword ptr[rdx+8]
    sbb     rbx, qword ptr[r8+8]
    mov     qword ptr[rcx]  , rax
    mov     qword ptr[rcx+8], rbx
    pop     rbx
    ret
int128dif ENDP

;void int128mul(_int128 &dst, const _int128 &x, const _int128 &y);
int128mul PROC
    push    rbx
    mov     rax, qword ptr[rdx+8]           ; rax = x.hi
    mov     rbx, qword ptr[r8+8]            ; rbx = y.hi
    or      rbx, rax                        ; rbx = x.hi | y.hi
    mov     rbx, qword ptr[r8]              ; rbx = y.lo
    jne     Hard                            ; if(x.hi|y.hi) goto Hard
                                            ; simple int64 multiplication
    mov     rax, qword ptr[rdx]             ; rax = x.lo
    mul     rbx                             ; rdx:rax = rax * rbx
    mov     qword ptr[rcx]  , rax           ; dst.lo = rax
    mov     qword ptr[rcx+8], rdx           ; dst.hi = rdx
    pop     rbx
    ret
Hard:                                       ; assume rax = x.hi, rbx = y.lo
    push    rsi
    mov     rsi, rdx                        ; need rdx for highend of mul, so rsi=&x
    mul     rbx                             ; rdx:rax = x.hi * y.lo
    mov     r9 , rax                        ; 
    mov     rax, qword ptr[rsi]             ; rax     = x.lo
    mul     qword ptr[r8+8]                 ; rdx:rax = x.lo * y.hi
    add     r9, rax                         ; r9      = lo(x.hi*y.lo+x.lo*y.hi); 
    mov     rax, qword ptr[rsi]             ; rax     = x.lo
    mul     rbx                             ; rdx:rax = x.lo * y.lo
    add     rdx, r9
    mov     qword ptr[rcx]  , rax
    mov     qword ptr[rcx+8], rdx
    pop     rsi
    pop     rbx
    ret
int128mul ENDP


;void int128div(_int128 &dst, const _int128 &x, const _int128 &y);
int128div PROC
    push        rdi
    push        rsi
    push        rbx
    push        rcx
    mov         r9,  rdx
    xor         rdi, rdi
    mov         rax, qword ptr[r9+8]
    or          rax, rax
    jge         L1
    inc         rdi
    mov         rdx, qword ptr[r9]
    neg         rax
    neg         rdx
    sbb         rax, 0
    mov         qword ptr[r9+8], rax
    mov         qword ptr[r9], rdx
L1:
    mov         rax, qword ptr[r8+8]
    or          rax, rax
    jge         L2
    inc         rdi
    mov         rdx, qword ptr[r8]
    neg         rax
    neg         rdx
    sbb         rax,0
    mov         qword ptr[r8+8], rax
    mov         qword ptr[r8], rdx
L2:
    or          rax, rax
    jne         L3
    mov         rcx, qword ptr[r8]
    mov         rax, qword ptr[r9+8]
    xor         rdx, rdx
    div         rcx
    mov         rbx, rax
    mov         rax, qword ptr[r9]
    div         rcx
    mov         rdx, rbx
    jmp         L4
L3:
    mov         rbx,rax
    mov         rcx,qword ptr[r8]
    mov         rdx,qword ptr[r9+8]
    mov         rax,qword ptr[r9]
L5:
    shr         rbx, 1
    rcr         rcx, 1
    shr         rdx, 1
    rcr         rax, 1
    or          rbx, rbx
    jne         L5
    div         rcx
    mov         rsi, rax
    mul         qword ptr[r8+8]
    mov         rcx, rax
    mov         rax, qword ptr[r8]
    mul         rsi
    add         rdx, rcx
    jb          L6
    cmp         rdx, qword ptr[r9+8]
    ja          L6
    jb          L7
    cmp         rax, qword ptr[rdx]
    jbe         L7
L6:
    dec         rsi
L7:
    xor         rdx, rdx
    mov         rax, rsi
L4:
    dec         rdi
    jne         L8
    neg         rdx
    neg         rax
    sbb         rdx, 0
L8:
    pop         rcx
    pop         rbx
    pop         rsi
    pop         rdi
    mov         qword ptr[rcx], rax
    mov         qword ptr[rcx+8], rdx
    ret
int128div ENDP

;void int128rem( _int128 &dst, const _int128 &x, const _int128 &y);
int128rem PROC
    push        rbx
    push        rdi
    push        rcx
    mov         r9,  rdx
    xor         rdi, rdi
    mov         rax, qword ptr[r9+8]
    or          rax, rax
    jge         L1
    inc         rdi
    mov         rdx, qword ptr[r9]
    neg         rax
    neg         rdx
    sbb         rax, 0
    mov         qword ptr[r9+8], rax
    mov         qword ptr[r9], rdx
L1:
    mov         rax, qword ptr[r8+8]
    or          rax, rax
    jge         L2
    mov         rdx, qword ptr[r8]
    neg         rax
    neg         rdx
    sbb         rax, 0
    mov         qword ptr[r8+8], rax
    mov         qword ptr[r8], rdx
L2:
    or          rax, rax
    jne         L3
    mov         rcx, qword ptr[r8]
    mov         rax, qword ptr[r9+8]
    xor         rdx, rdx
    div         rcx
    mov         rax, qword ptr[r9]
    div         rcx
    mov         rax, rdx
    xor         rdx, rdx
    dec         rdi
    jns         L4
    jmp         L8
L3:
    mov         rbx, rax
    mov         rcx, qword ptr[r8]
    mov         rdx, qword ptr[r9+8]
    mov         rax, qword ptr[r9]
L5:
    shr         rbx, 1
    rcr         rcx, 1
    shr         rdx, 1
    rcr         rax, 1
    or          rbx, rbx
    jne         L5
    div         rcx
    mov         rcx, rax
    mul         qword ptr[r8+8]
    xchg        rax, rcx
    mul         qword ptr[r8]
    add         rdx, rcx
    jb          L6
    cmp         rdx, qword ptr[r9+8]
    ja          L6
    jb          L7
    cmp         rax, qword ptr[r9]
    jbe         L7
L6:
    sub         rax, qword ptr[r8]
    sbb         rdx, qword ptr[r8+8]
L7:
    sub         rax, qword ptr[r9]
    sbb         rdx, qword ptr[r9+8]
    dec         rdi
    jns         L8
L4:
    neg         rdx
    neg         rax
    sbb         rdx, 0
L8:
    pop         rcx
    pop         rdi
    pop         rbx
    mov         qword ptr[rcx], rax
    mov         qword ptr[rcx+8], rdx
    ret
int128rem ENDP

;void int128neg( _int128 &dst, const _int128 &x);
int128neg PROC
    mov         rax,qword ptr[rdx]
    neg         rax
    mov         r8, qword ptr[rdx+8]
    adc         r8, 0
    neg         r8
    mov         qword ptr[rcx], rax
    mov         qword ptr[rcx+8], r8
    ret
int128neg ENDP

;int int128cmp(const _int128 &n1, const _int128 &n2);
int128cmp PROC
    mov         rax, qword ptr[rcx+8]       ; n1.hi
    cmp         rax, qword ptr[rdx+8]       ; n2.hi
    jl          lessthan                            ; signed compare of n1.hi and n2.hi
    jg          greaterthan
    mov         rax, qword ptr[rcx]         ; n2.lo
    cmp         rax, qword ptr[rdx]         ; n2.lo
    jb          lessthan                    ; unsigned compare of n1.lo and n2.lo
    ja          greaterthan
    mov         rax, 0                      ; they are equal
    ret
greaterthan:
    mov         rax, 1
    ret
lessthan:
    mov         rax, -1
    ret
int128cmp ENDP

END

; File:UInt128x64.asm
; build obj-file with
; ml64 /nologo /c /Zf /Fo$(IntDir)UInt128x64.obj UInt128x64.asm

.CODE

;void uint128div(_uint128 &dst, const _uint128 &x, const _uint128 &y);
uint128div PROC
    push        rbx
    push        rsi
    push        rcx
    mov         r9, rdx
    mov         rax, qword ptr[r8+8]
    or          rax, rax
    jne         L1
    mov         rcx, qword ptr[r8]
    mov         rax, qword ptr[r9+8]
    xor         rdx, rdx
    div         rcx
    mov         rbx, rax
    mov         rax, qword ptr[r9]
    div         rcx
    mov         rdx, rbx
    jmp         L2
L1:
    mov         rcx, rax
    mov         rbx, qword ptr[r8]
    mov         rdx, qword ptr[r9+8]
    mov         rax, qword ptr[r9]
L3:
    shr         rcx, 1
    rcr         rbx, 1
    shr         rdx, 1
    rcr         rax, 1
    or          rcx, rcx
    jne         L3
    div         rbx
    mov         rsi, rax
    mul         qword ptr[r8+8]
    mov         rcx, rax
    mov         rax, qword ptr[r8]
    mul         rsi
    add         rdx, rcx
    jb          L4
    cmp         rdx, qword ptr[r9+8]
    ja          L4
    jb          L5
    cmp         rax, qword ptr[r9]
    jbe         L5
L4:
    dec         rsi
L5:
    xor         rdx, rdx
    mov         rax, rsi
L2:
    pop         rcx
    pop         rsi
    pop         rbx
    mov         qword ptr[rcx], rax
    mov         qword ptr[rcx+8], rdx
    ret
uint128div ENDP

;void uint128rem(_uint128 &dst, const _uint128 &x, const _uint128 &y);
uint128rem PROC
    push        rbx
    push        rcx
    mov         r9, rdx
    mov         rax, qword ptr[r8+8]
    or          rax, rax
    jne         L1
    mov         rcx, qword ptr[r8]
    mov         rax, qword ptr[r9+8]
    xor         rdx, rdx
    div         rcx
    mov         rax, qword ptr[r9]
    div         rcx
    mov         rax, rdx
    xor         rdx, rdx
    jmp         L2
L1:
    mov         rcx, rax
    mov         rbx, qword ptr[r8]
    mov         rdx, qword ptr[r9+8]
    mov         rax, qword ptr[r9]
L3:
    shr         rcx, 1
    rcr         rbx, 1
    shr         rdx, 1
    rcr         rax, 1
    or          rcx, rcx
    jne         L3
    div         rbx
    mov         rcx, rax
    mul         qword ptr[r8+8]
    xchg        rax, rcx
    mul         qword ptr[r8]
    add         rdx, rcx
    jb          L4
    cmp         rdx, qword ptr[r9+8]
    ja          L4
    jb          L5
    cmp         rax, qword ptr[r9]
    jbe         L5
L4:
    sub         rax, qword ptr[r8]
    sbb         rdx, qword ptr[r8+8]
L5:
    sub         rax, qword ptr[r9]
    sbb         rdx, qword ptr[r9+8]
    neg         rdx
    neg         rax
    sbb         rdx, 0
L2:
    pop         rcx
    pop         rbx
    mov         qword ptr[rcx], rax
    mov         qword ptr[rcx+8], rdx
    ret
uint128rem ENDP

;int uint128cmp(const _uint128 &n1, const _uint128 &n2);
uint128cmp PROC
    mov         rax, qword ptr[rcx+8]       ; n1.hi
    cmp         rax, qword ptr[rdx+8]       ; n2.hi
    jb          lessthan                    ; usigned compare of n1.hi and n2.hi
    ja          greaterthan
    mov         rax, qword ptr[rcx]         ; n2.lo
    cmp         rax, qword ptr[rdx]         ; n2.lo
    jb          lessthan                    ; unsigned compare of n1.lo and n2.lo
    ja          greaterthan
    mov         rax, 0                      ; they are equal
    ret
greaterthan:
    mov         rax, 1
    ret
lessthan:
    mov         rax, -1
    ret
uint128cmp ENDP

END
#endif


#include "pch.h"

#ifdef _M_X64

#include <Math/Int128.h>

static const _int128 _0(0);
static const _int128 _10(10);
static const _int128 _16(16);
static const _int128 _8(16);

char *_i128toa(_int128 value, char *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  char *s = str;
  const bool negative = value < _0;
  if (negative && (radix == 10)) {
    value = -value;
    while (value != _0) {
      const unsigned int c = value % _10;
      *(s++) = radixLetter(c);
      value /= _10;
    }
    *(s++) = '-';
    *s = 0;
    return _strrev(str);
  }

  _uint128 v(value);
  const _uint128 r(radix);
  while (v != _0) {
    const unsigned int c = v % r;
    *(s++) = radixLetter(c);
    v /= r;
  }
  if (s == str) {
    return strcpy(str, "0");
  }
  else {
    *s = 0;
    return _strrev(str);
  }
  return str;
}

wchar_t *_i128tow(_int128 value, wchar_t *str, int radix) {
  wchar_t *s = str;
  const bool negative = value < _0;
  if (negative && (radix == 10)) {
    value = -value;
    while (value != _0) {
      const unsigned int c = value % _10;
      *(s++) = wradixLetter(c);
      value /= _10;
    }
    *(s++) = '-';
    *s = 0;
    return _wcsrev(str);
  }

  _uint128 v(value);
  const _uint128 r(radix);
  while (v != _0) {
    const unsigned int c = v % r;
    *(s++) = radixLetter(c);
    v /= r;
  }
  if (s == str) {
    return wcscpy(str, L"0");
  }
  else {
    *s = 0;
    return _wcsrev(str);
  }
  return str;
}

const char *_int128::parseDec(const char *str) { // return pointer to char following the number
  bool negative = false;
  bool gotDigit = false;
  switch (*str) {
  case '+':
    str++;
    break;
  case '-':
    str++;
    negative = true;
  }
  *this = _0;
  while (isdigit(*str)) {
    gotDigit = true;
    const unsigned int d = *(str++) - '0';
    *this *= _10;
    *this += d;
  }
  if (!gotDigit) {
    throw "_int128:string is not a number";
  }
  if (negative) {
    *this = -*this;
  }
  return str;
}

const char *_int128::parseHex(const char *str) {
  *this = 0;
  while (isxdigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _16;
    *this += d;
  }
  return str;
}

const char *_int128::parseOct(const char *str) {
  *this = 0;
  while (isodigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _8;
    *this += d;
  }
  return str;
}

_int128::_int128(const char *str) {
  if (*str == '-') {
    parseDec(str);
  } else {
    if (!isdigit(*str)) {
      throw exception("_int128:string is not an integer");
    }
    if (*str == '0') {
      switch (str[1]) {
      case 'x':
        parseHex(str + 2);
        break;
      case 0:
        *this = 0;
        break;
      default:
        parseOct(str + 1);
      }
    }
    else {
      parseDec(str);
    }
  }
}
#endif // _M_X64


// File:UInt128IOx64.cpp
#include "pch.h"

#ifdef _M_X64

#include <Math/Int128.h>

static const _uint128 _0(0);
static const _uint128 _10(10);
static const _uint128 _16(16);
static const _uint128 _8(16);

char*_ui128toa(_uint128 value, char *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  char *s = str;
  const _uint128 r(radix);
  while (value != _0) {
    const unsigned int c = value % r;
    *(s++) = radixLetter(c);
    value /= r;
  }
  if (s == str) {
    return strcpy(str, "0");
  }
  else {
    *s = 0;
    return _strrev(str);
  }
}

wchar_t *_ui128tow(_uint128 value, wchar_t *str, int radix) {
  assert(radix >= 2 && radix <= 36);
  wchar_t *s = str;
  const _uint128 r(radix);
  while (value != _0) {
    const unsigned int c = value % r;
    *(s++) = wradixLetter(c);
    value /= r;
  }
  if (s == str) {
    return wcscpy(str, L"0");
  }
  else {
    *s = 0;
    return _wcsrev(str);
  }
}

const char *_uint128::parseDec(const char *str) {
  *this = 0;
  while (isdigit(*str)) {
    const unsigned int d = *(str++) - '0';
    *this *= _10;
    *this += d;
  }
  return str;
}

const char *_uint128::parseHex(const char *str) {
  *this = 0;
  while (isxdigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _16;
    *this += d;
  }
  return str;
}

const char *_uint128::parseOct(const char *str) {
  *this = 0;
  while (isodigit(*str)) {
    const unsigned int d = convertNumberChar(*(str++));
    *this *= _8;
    *this += d;
  }
  return str;
}

_uint128::_uint128(const char *str) {
  if (!isdigit(*str)) {
    throw exception("_uint128:string is not an integer");
  }
  if (*str == '0') {
    switch (str[1]) {
    case 'x':
      parseHex(str + 2);
      break;
    case 0:
      *this = 0;
      break;
    default:
      parseOct(str + 1);
      break;
    }
  }
  else {
    parseDec(str);
  }
}

#endif // _M_X64

// File:Int128IOCommon.cpp
#include "pch.h"

#ifdef _M_X64

#include <Math/Int128.h>

unsigned int convertNumberChar(char digit) {
  switch(digit) {
  case '0': return 0;
  case '1': return 1;
  case '2': return 2;
  case '3': return 3;
  case '4': return 4;
  case '5': return 5;
  case '6': return 6;
  case '7': return 7;
  case '8': return 8;
  case '9': return 9;
  case 'a':
  case 'A': return 10;
  case 'b':
  case 'B': return 11;
  case 'c':
  case 'C': return 12;
  case 'd':
  case 'D': return 13;
  case 'e':
  case 'E': return 14;
  case 'f':
  case 'F': return 15;
  default :
    return 0;
  }
}

#endif // _M_X64


#endif

