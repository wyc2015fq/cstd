
typedef unsigned int size_t;

#ifdef __cplusplus
#define INLINE inline
#else
#define INLINE
#endif

#define _USE_ASM_

//内联汇编编写的memcpy函数，相当于：
//#define MemCpy(dst, src, n)  memcpy( (dst), (src), (n) )
INLINE void MemCpy( void* dst, const void* src, size_t n ) {
#ifdef _USE_ASM_
  __asm {
    mov ecx, n;
    jecxz to_end;
    
    //保存方向标志
    pushf;
    
    mov edi, dst;
    mov esi, src;
    cmp edi, esi;
    jbe to_up; //dst <= src;
    
    mov eax, ecx;
    shl eax, 2;
    add
      eax, esi;
      cmp edi, eax;
      jae to_up; //dst >= src+4n
      
      mov eax, ecx;
      dec eax;
      shl eax, 2;
      add
        edi, eax;
        add
          esi, eax;
          std;
          jmp to_mov;
          
  to_up:
          cld;
          
  to_mov:
          rep movs;
          popf;
          
  to_end:
        }
#else
  if ( dst != src ) {
    if ( dst < src ) {
      const char * s = ( char* ) src;
      const char *e = ( char* ) src + n;
      char *d = ( char* ) dst;
      for ( ; s != e; ) {
        *d++ = *s++;
      }
    } else {
      const char *e = ( char* ) src;
      const char *s = ( char* ) src + n;
      char *d = ( char* ) dst + n;
      for ( ; s != e; ) {
        *--d = *--s;
      }
    }
  }
#endif
        
      }
      
      
// 内存交换
// 用汇编比用C快3倍
INLINE void MemSwap( void* dst, const void* src, size_t n ) {
#ifdef _USE_ASM_
  __asm {
    //保存方向标志
    pushf;
    mov edi, dst;
    mov esi, src;
    mov ecx, n;
    //add ecx, 3;
    
  do_swap:
    cmp ecx, 4;
    jb do_swap2;
    mov eax, [ esi ];
    mov ebx, [ edi ];
    mov [ esi ], ebx;
    mov [ edi ], eax;
    add
      esi, 4;
      add
        edi, 4;
        sub ecx, 4;
        jmp do_swap;
        
  do_swap2:
        jecxz to_end;
        mov al, [ esi ];
        mov bl, [ edi ];
        mov [ esi ], bl;
        mov [ edi ], al;
        inc esi;
        inc edi;
        dec ecx;
        jmp do_swap2;
        
  to_end:
        popf;
      }
#else
  int* d, *s, *e, tt;
  char* cd, *cs, ctt;
      
  d = ( int* ) dst;
  s = ( int* ) src;
  e = s + ( n >> 2 );
  for ( ; s != e; ++s, ++d ) {
    tt = *s;
    *s = *d;
    *d = tt;
  }
  n &= 3;
      
  cs = ( char* ) src;
  cd = ( char* ) dst;
  for ( ; n--; ++cs, ++cd ) {
    ctt = *cs;
    *cs = *cd;
    *cd = ctt;
  }
#endif
      
    }
    
// 翻转
INLINE void MemRev( void* f, void* l, int n ) {
#ifdef _USE_ASM_
  __asm {
    //保存方向标志
    pushf;
    mov esi, f;
    mov edi, l;
    mov ecx, n;
    jmp loop1;
    
  loop0:
    mov ecx, n;
    sub edi, ecx;
    
  loop1:
    cmp esi, edi;
    je to_end;
    sub edi, ecx;
    cmp esi, edi;
    je to_end;
    
  do_swap:
    cmp ecx, 4;
    jb do_swap2;
    mov eax, [ esi ];
    mov ebx, [ edi ];
    mov [ esi ], ebx;
    mov [ edi ], eax;
    add
      esi, 4;
      add
        edi, 4;
        sub ecx, 4;
        jmp do_swap;
        
  do_swap2:
        jecxz loop0;
        mov al, [ esi ];
        mov bl, [ edi ];
        mov [ esi ], bl;
        mov [ edi ], al;
        inc esi;
        inc edi;
        dec ecx;
        jmp do_swap2;
        
  to_end:
        popf;
      }
#else
  char* s, *d;
  s = ( char* ) f;
  d = ( char* ) l;
  if ( d != s ) {
    for ( d -= n; s != d; s += n ) {
      MemCpy( d, s, n );
    }
  }
#endif
      
    }
    
// 相当于 reverse_copy
//#define MemCpy(dst, src, n)  memcpy( (dst), (src), (n) )
INLINE void MemRevCpy( void* dst, const void* src, size_t n, size_t m ) {
  __asm {
    //保存方向标志
    pushf;
    mov ecx, n;
    mov edx, ecx;
    jecxz to_end;
    mov ecx, m;
    jecxz to_end;
    mov edi, dst;
    mov esi, src;
    
  loop1:
    mov ecx, m;
    sub esi, ecx;
    cmp edi, esi;
    jbe to_up; //dst <= src;
    
    mov eax, ecx;
    shl eax, 2;
    add
      eax, esi;
      cmp edi, eax;
      jae to_up; //dst >= src+4n
      
      mov eax, ecx;
      dec eax;
      shl eax, 2;
      add
        edi, eax;
        add
          esi, eax;
          std;
          jmp to_mov;
          
  to_up:
          cld;
          
  to_mov:
          rep movs;
          dec edx;
          mov ecx, edx;
          jecxz to_end;
          mov ecx, m;
          sub esi, ecx;
          jmp loop1;
          
  to_end:
          popf;
        }
      }
      
#if 0
      
#define ASSSS1(a) assert( 4==a[0] && 3==a[1] && 2==a[2] && 1==a[3] )
#define ASSSS2(a, B) assert( 4==a[0]B && 3==a[1]B && 2==a[2]B && 1==a[3]B )
template <typename LOG>
inline
int test_Memopt( LOG& log ) {
  double a[] = {1, 2, 3, 4};
  double b[ 4 ];
  MemRevCpy( b, a + 4, 4, sizeof( double ) );
  ASSSS1( b );
  MemRev( a, a + 4, sizeof( double ) );
  ASSSS1( a );
  char s[ 4 ][ 5 ] = {
                       {1, 1, 1, 1, 1},
                       {2, 2, 3, 4, 5},
                       {3, 2, 3, 4, 5},
                       {4, 2, 3, 4, 5}
                     };
  MemRev( s, s + 4, sizeof( s[ 0 ] ) );
  ASSSS2( s, [ 0 ] );
  return 0;
}
#undef ASSSS2
#undef ASSSS1


#endif
