
#include <math.h>
#include "big.h"
// d = a * b + c
sz_t mpmuladd( word* a, sz_t an, word b, word c, word* d ) {
    sz_t j;
    dword t = c;
    for( j = 0; j < an; ++j ) {
        t = MP_OP_LOW_HIGH( t +( dword ) a[j] * b, d[j], t );
    }
    if( t ) {
        t = MP_OP_LOW_HIGH( t, d[j], t );
        ++j;
    }
    return j;
}

// c+=a*b;
sz_t mpmuladd2( word* a, sz_t an, word* b, sz_t bn, word* c ) {
    sz_t i, j;
    for( i = 0; i < an; ++i, ++c ) {
        dword t = 0;
        word aw = a[i];
        for( j = 0; j < bn; ++j ) {
            t = MP_OP_LOW_HIGH( t + c[j] +( dword ) b[j] * aw, c[j], t );
        }
    }
    return 0;
}

sz_t mpshl2( word* a, sz_t an, word* b, sz_t n ) {
    sz_t i;
    dword t = 0;
    b += n / WORD_BIT;
    n %= WORD_BIT;
    t = a[an] >>( WORD_BIT - n );
    t = MP_OP_LOW_HIGH( t, b[an], t );
    for( i = an; i--; ) {
        t = MP_OP_LOW_HIGH( t | a[i] << n, b[i], t );
    }
    if( b[an] ) {
        an++;
    }
    return an + n / WORD_BIT;
}

sz_t mpaddu( word* pValue, sz_t last, word dwValue ) {
    word dwFull = 0;
    __asm {
        mov edi, pValue;
        mov ecx, last;
        mov eax, dwValue;
        add[edi], eax;
        jnc L3;
        mov edx, 1;
        jmp L2;
L1 :
        add dword ptr[edi][edx*4], 1;
        jnc L3;
        inc edx;
L2 :
        loop L1;
    }
    dwFull = 1;
    L3: return dwFull;
}

sz_t mpadd_s( word* pValue, sz_t last, word* xpValue, sz_t xlast ) {
    word dwFull = 0;
    __asm {
        mov edi, pValue;
        mov esi, xpValue;
        mov ecx, xlast;
        xor edx, edx;
        clc;
L1 :
        mov eax,[esi][edx*4];
        adc[edi][edx*4], eax;
        inc edx;
        loop L1;
        jnc L4;
        mov ecx, last /*     //如果this的有效位更长，则加进位 */
        sub ecx, xlast;
        jz L3;
L2 :
        add[edi][edx*4], 1;
        jnc L4;
        inc edx;
        loop L2;
    }
    L3: dwFull = 1;
    L4: return dwFull;
}

int mpadd( word* a, sz_t a_used, word* b, sz_t b_used, word* sum, sz_t* pc_used ) {
    // compute sum = a + b   (a_used >= b_used)
    int r, n, carry = 0;
    word aa, bb;
    word * s_last = sum + a_used;
    if( a_used < b_used ) {
        word * t;
        sz_t t_used;
        CV_SWAP( a, b, t );
        CV_SWAP( a_used, b_used, t_used );
    }
#define ADD_LOOP_BODY \
  { \
    aa = *a++; \
    bb = *b++; \
    aa += bb; \
    if (carry) \
    { \
      if (++aa > bb) carry = FALSE; \
    } \
    else if (aa < bb) \
      carry = TRUE; \
    *sum++ = aa; \
  }

    r = b_used % 16;
    switch( r ) {
        case 15: ADD_LOOP_BODY;
        case 14: ADD_LOOP_BODY;
        case 13: ADD_LOOP_BODY;
        case 12: ADD_LOOP_BODY;
        case 11: ADD_LOOP_BODY;
        case 10: ADD_LOOP_BODY;
        case 9: ADD_LOOP_BODY;
        case 8: ADD_LOOP_BODY;
        case 7: ADD_LOOP_BODY;
        case 6: ADD_LOOP_BODY;
        case 5: ADD_LOOP_BODY;
        case 4: ADD_LOOP_BODY;
        case 3: ADD_LOOP_BODY;
        case 2: ADD_LOOP_BODY;
        case 1: ADD_LOOP_BODY;
    }
    n = b_used / 16;
    while( n-- ) {
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
        ADD_LOOP_BODY;
    }
    if( sum != a ) {
        // copy rest of a to sum
        Copy_Vector( sum, a, a_used - b_used );
    }
    // propagate carry
    if( carry ) {
        while( ++*sum == 0 ) sum++;
        if( sum == s_last ) a_used++;
    }
#undef ADD_LOOP_BODY
    if( pc_used ) {
        *pc_used = a_used;
    }
    return a_used > 0;
}

int mpequl( word* pValue, sz_t last, word* xpValue, sz_t xlast ) {
    int dwFull = 1;
    __asm {
        //mov al, 1;  //return true;
        mov ecx, last;
        cmp ecx, xlast;
        jne L1;
        mov esi, pValue;
        mov edi, xpValue;
        cld;
        repe cmpsd;
        jz L2;
L1 :
        //mov al, 0;
        mov dwFull, 0;
L2 :
    }
    return dwFull;
}

int mpcmp( word* a, int a_used, word* b, int b_used ) {
#ifndef _USE_ASM
    word * ap, *bp;
    if( a_used > b_used ) return 1;
    if( a_used < b_used ) return -1;
    ap = a + a_used;
    bp = b + b_used;
    while( ap > a ) {
        word A = *--ap;
        word B = *--bp;
        if( A > B ) return 1;
        if( A < B ) return -1;
    }
    return 0;
#else
    int dwFull = 0;
    __asm {
        pushf;
        mov dwFull, 1 /*     //return true; */
        mov ecx, a_used;
        cmp ecx, b_used;
        ja Lgrt;
        jb Llss;
        mov esi, a;
        mov edi, b;
        mov edx, ecx;
        dec edx;
        shl edx, 2;
        add esi, edx;
        add edi, edx;
        std;
        repe cmpsd;
        ja Lgrt;
Llss :
        mov dwFull, -1;
        jmp Lend;
Lgrt :
        mov dwFull, 1;
Lend :
        popf;
    }
    return dwFull;
#endif
}

int mpsubu_s( word* pValue, sz_t last, word dwValue ) {
    __asm {
        mov edi, pValue;
        mov ecx, last;
        mov eax, dwValue;
        sub[edi], eax;
        jnc to_return;
        sub dword ptr[edi+4], 1;
        jnz to_return;
        dec last;
to_return :
    }
    return 0;
}

int mpsub_s( word* pValue, sz_t last, word* xpValue, sz_t xlast ) {
    int sign = mpcmp( pValue, last, xpValue, xlast );
    if( sign < 0 ) {
        __asm {
            mov eax, pValue;
            mov ebx, xpValue;
            mov pValue, ebx;
            mov xpValue, eax;
        }
    }
    __asm {
        mov edi, pValue;
        mov esi, xpValue;
        mov ecx, xlast;
        xor edx, edx;
        clc;
L1 :
        mov eax,[esi][edx*4];
        sbb[edi][edx*4], eax;
        inc edx;
        loop L1;
        jnc L3;
        mov ecx, last;
        sub ecx, xlast;
        xor eax, eax;
        stc;
L2 :
        sbb[edi][edx*4], eax;
        jnc L3;
        inc edx;
        loop L2;
L3 :
        //调整last
        mov edi, pValue;
        mov eax, last;
        mov ecx, eax;
        dec eax;
        shl eax, 2;
        add edi, eax;
        xor eax, eax;
        repe scasd;
        inc ecx;
        mov last, ecx;
    }
    return sign;
}

int mpsub( word* a, sz_t a_used, word* b, sz_t b_used, word* diff, sz_t* pc_used ) {
    // compute diff = a - b    (a > b)
#define SUB_LOOP_BODY \
  { \
    aa = *a++; \
    bb = *b++; \
    if (borrow && ++bb) borrow = FALSE; \
    if (aa < bb) borrow = TRUE; \
    *diff++ = aa - bb; \
  }

    int n;
    word aa, bb;
    BOOL borrow = FALSE;
    word * d_stop = diff + a_used;
    int c_sign;
    c_sign = mpcmp( a, a_used, b, b_used );
    if( NULL == diff ) {
        return c_sign;
    }
    if( c_sign < 0 ) {
        word * t;
        sz_t t_used;
        CV_SWAP( a, b, t );
        CV_SWAP( a_used, b_used, t_used );
    }
    switch( b_used % 16 ) {
        case 15: SUB_LOOP_BODY;
        case 14: SUB_LOOP_BODY;
        case 13: SUB_LOOP_BODY;
        case 12: SUB_LOOP_BODY;
        case 11: SUB_LOOP_BODY;
        case 10: SUB_LOOP_BODY;
        case 9: SUB_LOOP_BODY;
        case 8: SUB_LOOP_BODY;
        case 7: SUB_LOOP_BODY;
        case 6: SUB_LOOP_BODY;
        case 5: SUB_LOOP_BODY;
        case 4: SUB_LOOP_BODY;
        case 3: SUB_LOOP_BODY;
        case 2: SUB_LOOP_BODY;
        case 1: SUB_LOOP_BODY;
    }
    n = b_used / 16;
    while( n-- ) {
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
        SUB_LOOP_BODY;
    }
    if( diff != a ) Copy_Vector( diff, a, a_used - b_used );
    if( borrow ) while( -- *diff == MAX_WORD ) diff++;
    while( *--d_stop == 0 && a_used ) a_used--;
#undef SUB_LOOP_BODY
    if( pc_used ) {
        *pc_used = a_used;
    }
    return c_sign;
}

int mplshift( word* x, sz_t xn, long n, word* result, sz_t* r_used ) {
    int w_shift =( int )( n / WORD_LENGTH );
    int b_shift =( int )( n % WORD_LENGTH );
    int used = xn + w_shift;
    int r = WORD_LENGTH - b_shift;
    word * q, *q_stop, *p;
    if( b_shift &&( x[n-1] >> r ) ) used++;
    //sz = next_power(used);
    //mpint_new(*result, sz);
    q =( x );
    q_stop = q +( xn );
    p =( result );
    while( w_shift-- ) * p++ = 0;
    if( b_shift ) {
        word carry = 0;
        while( q < q_stop ) {
            word B = *q++;
            carry |=( B << b_shift );
            *p++ = carry;
            carry =( B >> r );
        }
        if( carry ) * p = carry;
    }
    else while( q < q_stop ) * p++ = *q++;
    *r_used = used;
    return 0;
}

int mprshift( word* x, sz_t xn, long n, word* result, sz_t* r_used ) {
    int sticks =( int )( n / WORD_LENGTH );
    int shifts =( int )( n % WORD_LENGTH );
    int used = n - sticks;
    word * q, *p;
    p =( result ) + used;
    q =( x ) + n;
    if( shifts != 0 ) {
        word carry = 0;
        int r = WORD_LENGTH - shifts;
        while( p >( result ) ) {
            word B = *--q;
            carry |=( B >> shifts );
            *--p = carry;
            carry =( B << r );
        }
    }
    else {
        while( p >( result ) ) * --p = *--q;
    }
    if(( result )[used-1] == 0 ) used--;
    *r_used = used;
    return 0;
}

// bitwise and
// and_used = min(a_used, a_used);
int mpand( word* a, sz_t a_used, word* b, sz_t b_used, word* and, sz_t* pand_used ) {
    word * and_ptr = and;
    word * ap = a;
    word * bp = b;
    word * a_stop = ap + a_used;
    word * b_stop = bp + b_used;
    int used;
    word * p;
    if( a_used <= b_used ) {
        used = a_used;
        p = and_ptr;
        while( ap < a_stop ) * p++ = *ap++ & *bp++;
    }
    else {
        used = b_used;
        p = and_ptr;
        while( bp < b_stop ) * p++ = *ap++ & *bp++;
    }
    while( *--p == 0 && used > 0 ) used--;
    *pand_used = used;
    return 0;
}

// bitwise or
int mpor( word* a, sz_t a_used, word* b, sz_t b_used, word* or, sz_t* por_used ) {
    word * or_ptr = or, *p;
    word * ap = a;
    word * bp = b;
    word * a_stop = ap + a_used;
    word * b_stop = bp + b_used;
    if( a_used >= b_used ) {
        *por_used = a_used;
        p = or_ptr;
        while( bp < b_stop ) * p++ = *ap++ | *bp++;
        while( ap < a_stop ) * p++ = *ap++;
    }
    else {
        *por_used = b_used;
        p = or_ptr;
        while( ap < a_stop ) * p++ = *ap++ | *bp++;
        while( bp < b_stop ) * p++ = *bp++;
    }
    return 0;
}

// q = [high, low]/D
// r = [high, low]%D
word DivTwoWordsByOne( word high, word low, word D, word* q ) {
    // precondition:  high < D
    // *q = [high,low] / D, returns remainder
    // the result has at most 32 bits
    // we can compute it with double precision floating point division
#if 0
    double H = high;
    double L = low;
    word Q =( word )(( H / D ) * pow32 + L / D );
#else
    dword t =(( dword ) high << WORD_BIT ) + low;
    word Q =( word )( t / D );
#endif
    D *= Q;
    *q = Q;
    return low - D;
}

// c=a/B;
word DivByOneWord( word* a, sz_t a_used, word B, word* quot, sz_t* pq_used ) {
    word r = 0;
    word * a_ptr = a + a_used;
    word * q_ptr = quot + a_used;
    while( a_ptr > a ) r = DivTwoWordsByOne( r, *--a_ptr, B, --q_ptr );
    if( quot[a_used-1] == 0 ) a_used--;
    return r;
}

// a[]/=B, return a[]%B
word mpdivu( word* a, sz_t a_used, word B ) {
    word * p;
    word r = 0;
    for( p = a + a_used - 1; p >= a; p-- ) r = DivTwoWordsByOne( r, *p, B, p );
    return r;
}

word Multiply_Words( word a, word b, word* high ) {
    word al = LOW_WORD( a );
    word ah = HIGH_WORD( a );
    word bl = LOW_WORD( b );
    word bh = HIGH_WORD( b );
    word c, L, H;
    c = bl * al;
    L = LOW_WORD( c );
    c = HIGH_WORD( c ) + bl * ah;
    H = HIGH_WORD( c );
    c = LOW_WORD( c ) + bh * al;
    L |=( LOW_WORD( c ) << HALF_LENGTH );
    *high = H + HIGH_WORD( c ) + bh * ah;
    return L;
}

// c[] += a[]*B, return carry
word mpmulu( word* a, sz_t a_used, word B, word* c ) {
    sz_t i;
    word low, high;
    word carry = 0;
    for( i = 0; i < a_used; ++i ) {
        word P = c[i];
        low = Multiply_Words( a[i], B, &high );
        P += low;
        if( P < low ) high++;
        // carry in addition
        P += carry;
        if( P < carry ) high++;
        // carry in addition
        c[i] = P;
        carry = high;
    }
    c[i] = carry;
    return carry;
}

// p[] = a[]*B+c, return carry
word mpmuluaddu( word* a, sz_t a_used, word B, word c, word* p, sz_t *pp_used ) {
    sz_t i, p_used = a_used;
    word P, low, high, carry = c;
    word * a_stop = a + a_used;
    for( i = 0; i < a_used; ++i ) {
        low = Multiply_Words( a[i], B, &high );
        P = carry + low;
        if( P < carry ) high++;
        // carry in addition
        p[i] = P;
        carry = high;
    }
    if( carry ) {
        p[i] = carry;
        ++p_used;
    }
    if( pp_used ) {
        *pp_used = p_used;
    }
    return carry;
}

// p[] = ((c<<an)+a[])/B, return carry
word DivAdd_Inner_Loop( word* p, word* a, word* a_stop, word B, word c ) {
    p += a_stop - a;
    while( a < a_stop-- ) {
        c = DivTwoWordsByOne( c, *a_stop, B, --p );
    }
    return c;
}

word Div_Inner_Loop( word* a, sz_t a_used, word B, word* p ) {
    // p[] -= a[]*B, return carry
    word * a_stop = a + a_used;
    word low, high;
    word carry = 0;
    word P;
    while( a < a_stop ) {
        P = *p;
        low = Multiply_Words( *a++, B, &high );
        if( P < low ) high++;
        // carry
        P -= low;
        if( P < carry ) high++;
        // carry
        P -= carry;
        *p++ = P;
        carry = high;
    }
    P = *p;
    *p = P - carry;
    return( P < carry );
}

sz_t School_Mult_Inner_Loop( word* a, sz_t a_used, word* b, int b_used, word* prod ) {
    sz_t p_used = a_used + b_used;
    word * b_last = b + b_used - 1;
        {
        word * prod0 = prod;
        if( a == prod || b == prod ) {
            prod = MALLOC( word, p_used );
        }
        while( b < b_last ) mpmulu( a, a_used, *b++, prod++ );
        if( mpmulu( a, a_used, *b, prod ) == 0 ) p_used--;
        // no carry
        if( prod0 != prod ) {
            MEMCPY( prod0, prod, p_used );
            SAFEFREE( prod );
        }
    }
    return p_used;
}

word Shift_Left( word* a, word* b, sz_t length, sz_t shift ) {
    // auxiliary function used in School_Div
    // a := (b << shift) and return carry   (0 < shift < 32)
    int r;
    word carry = 0;
    word * b_stop = b + length;
    if( shift == 0 ) {
        while( b < b_stop ) * a++ = *b++;
        return 0;
    }
    r = WORD_LENGTH - shift;
    while( b < b_stop ) {
        word B = *b++;
        carry |=( B << shift );
        *a++ = carry;
        carry =( B >> r );
    }
    return carry;
}

// q=a/b, a%=b
sz_t mpdivrem( word* a, sz_t an, word* b, sz_t bn, word* q, sz_t* pq_used, word* r, sz_t* pr_used ) {
    sz_t i = 0, j;
    word B, Q, w;
    dword A = 0, t = 0, t2 = 0;
    for(; bn &&( B = b[bn-1] ) == 0; --bn ) {
        if( 0 == bn ) {
            return 0;
        }
    }
    for( A = 0; an-- >= bn; ) {
        A = A << WORD_BIT | a[an];
        if( Q =( word ) t / B ) {
            word * a0 = a + an + 1 - bn;
            for( t = 0, t2 = 0, j = 0; j < bn; ++j ) {
                t2 = MP_OP_LOW_HIGH( t2 +( dword ) b[j] * Q, w, t2 );
                t =( int ) MP_OP_LOW_HIGH( t +( dword ) a0[j] - w, a0[j], t );
            }
            if( t ) {
                //a0[j] = (word)(t + a0[j]);
                t =( int ) MP_OP_LOW_HIGH( t +( dword ) a0[j], a0[j], t );
            }
        }
        if( q ) {
            q[an-bn] = Q;
        }
        A = a[an];
    }
    return 0;
}

#define SAFESET(_P, _V) (NULL != (_P) ? (*(_P) = (_V)) : 0)

int mpdiv( word* A, sz_t a_used, word* B, sz_t b_used, word* Q, sz_t* Q_used, word* R, sz_t* R_used ) {
    word * a, *b;
    word * b_stop, *a_ptr;
    int Q_idx = 0;
    word b1, b2;
    word sh;
    sz_t quot_used = a_used - b_used + 1;
    sz_t shift = 0;
    if( 1 == b_used ) {
        word rr;
        a = MALLOC( word, a_used + 1 );
        MEMCPY( a, A, a_used );
        if( NULL == Q ) {
            Q = a;
        }
        rr = DivByOneWord( a, a_used, B[0], Q, Q_used );
        SAFESET( Q_used, quot_used );
        SAFESET( R, rr );
        SAFESET( R_used,( R[0] == 0 ) ?0 :
        1 );
        SAFEFREE( a );
        return 0;
    }
    if( mpcmp( A, a_used, B, b_used ) < 0 ) {
        SAFESET( Q_used, 0 );
        SAFESET( R_used, a_used );
        if( R ) {
            MEMCPY( R, A, a_used );
        }
        return 0;
    }
    a = MALLOC( word, a_used + 1 );
    b = MALLOC( word, b_used );
    b_stop = b + b_used;
    // we first normalize A and B, i.e. ...
    sh = B[b_used-1];
    while(( sh & 0x80000000 ) == 0 ) {
        sh <<= 1;
        shift++;
    }
    a[a_used] = Shift_Left( a, A, a_used, shift );
    Shift_Left( b, B, b_used, shift );
    // two often used values
    b1 = b[b_used-1];
    b2 =( b_used > 1 ) ? b[b_used-2] : 0;
    /* division main loop */
    a_ptr = a + a_used;
    // current position in a
    Q_idx = quot_used - 1;
    // current position in Q
    while( Q_idx >= 0 ) {
        word q = MAX_WORD;
        if( *a_ptr != b1 ) {
            word r = DivTwoWordsByOne( *a_ptr, *( a_ptr - 1 ), b1, &q );
            word h11;
            word h10 = Multiply_Words( q, b2, &h11 );
            word h21 = r;
            word h20 = *( a_ptr - 2 );
            while( h11 > h21 ||( h11 == h21 && h10 > h20 ) ) {
                // [h11,h10] > [h21,h20]
                q--;
                if( h10 < b2 ) h11--;
                h10 -= b2;
                h21 += b1;
                if( h21 < b1 ) break;
            }
        }
        if( Div_Inner_Loop( b, b_used, q, a_ptr - b_used ) ) {
            // a -= b*q
            // if carry add one b back to a and decrement q  (never executed ?)
            word * ap, *bp;
            BOOL carry = FALSE;
            for( bp = b, ap = a_ptr - b_used; bp < b_stop; bp++, ap++ ) {
                word aa = *ap, bb = *bp;
                bb += aa;
                if( carry ) {
                    if( ++bb > aa ) carry = FALSE;
                }
                else if( bb < aa ) carry = TRUE;
                *ap = bb;
            }
            if( carry )( *ap ) ++;
            q--;
        }
        if( Q ) {
            Q[Q_idx] = q;
        }
        --Q_idx;
        a_ptr--;
    }
    if( R ) {
        // copy remainder to R[]
        word * p = a + b_used, *q = R + b_used;
        // copy and shift a to R[]
        if( shift ) {
            int r = WORD_LENGTH - shift;
            word carry = 0;
            while( p > a ) {
                word A = *--p;
                carry |=( A >> shift );
                *--q = carry;
                carry =( A << r );
            }
        }
        else {
            while( p > a ) * --q = *--p;
        }
        for( q = R + b_used - 1; b_used && *q == 0; q-- ) b_used--;
        if( R_used ) {
            *R_used = b_used;
        }
    }
    SAFEFREE( a );
    SAFEFREE( b );
    if( Q ) {
        if( quot_used && Q[quot_used-1] == 0 ) {
            quot_used--;
        }
        if( Q_used ) {
            *Q_used = quot_used;
        }
    }
    return 0;
}

word* mpgcd( word* a, sz_t a_used, word* b, sz_t b_used, sz_t* pc_used ) {
    while( 1 ) {
        if( b_used != 0 ) {
            mpdiv( a, a_used, b, b_used, 0, 0, a, &a_used );
            //a/=b
        }
        else {
            *pc_used = a_used;
            return a;
        }
        if( a_used != 0 ) {
            mpdiv( b, b_used, a, a_used, 0, 0, b, &b_used );
            //a/=b
        }
        else {
            *pc_used = b_used;
            return b;
        }
    }
}

int mpmulkara( word* a, sz_t a_used, word* b, sz_t b_used, word* prod, sz_t* pp_used );
int mpmul( word* a, sz_t a_used, word* b, sz_t b_used, word* prod, sz_t* pp_used ) {
    if( a_used == 1 && b_used == 1 ) {
        prod[0] = Multiply_Words( a[0], b[0], prod + 1 );
        *pp_used = prod[1] ?2 :
        1;
    }
    else {
        sz_t p_used;
        word * p_buf = NULL, *prod0 = prod;
        if( a_used < b_used ) {
            word * t;
            CV_SWAP( a, b, t );
            CV_SWAP( a_used, b_used, p_used );
        }
        if( prod == a || prod == b ) {
            p_used = a_used + b_used;
            prod = p_buf = MALLOC( word, p_used );
        }
        mpmulkara( a, a_used, b, b_used, prod, &p_used );
        if( p_buf ) {
            MEMCPY( prod0, p_buf, p_used );
            SAFEFREE( p_buf );
        }
        if( pp_used ) {
            *pp_used = p_used;
        }
    }
    return 0;
}

int mpmul2_s( word* pValue, sz_t last, word dwTimes ) {
    word dwFull = 0;
    __asm {
        mov edi, pValue;
        mov ebx, last;
        xor eax, eax;
        mov ecx, dwTimes;
L1 :
        mov edx,[edi];
        shld[edi], eax, cl;
        mov eax, edx;
        add edi, 4;
        dec ebx;
        jnz L1;
        neg cl;
        shr edx, cl;
        jz L2;
        mov dwFull, edx;
    }
    L2: return dwFull;
}

int mpmulu_s( word* pValue, sz_t last, word dwValue ) {
    word dwFull = 0;
    int i;
    word a = 1, b = 1, c = 1, d = 1;
    for( i = 0; i < 10; ++i ) {
        a *= pValue[i+0];
        b *= pValue[i+1];
        c *= pValue[i+2];
        d *= pValue[i+3];
    }
    __asm {
        mov eax, 0xffffffff;
        mov ebx, 0xffffffff;
        mul ebx;
        //push ebx;
        //mov eax, [esp-4];
    }
    if( dwValue == 0 ) {
        *pValue = 0;
        last = 0;
        return 0;
    }
        {
        word times;
        __asm {
            bsr ebx, dwValue;
            bsf ecx, dwValue;
            cmp ebx, ecx;
            jne M0 /*        //如果是2的n次方，则使用移位的乘法 */
            mov times, ebx;
        }
        return mpmul2_s( pValue, last, times );
    }
    M0: __asm {
        mov esi, pValue;
        mov ecx, last;
        mov ebx, dwValue /*    //乘数 */
        xor edi, edi /*      //作临时存贮器,保存高位 */
L1 :
        mov eax,[esi];
        mul ebx;
        add eax, edi;
        mov[esi], eax;
        adc edx, 0;
        mov edi, edx;
        add esi, 4;
        loop L1;
        cmp edi, 0;
        je L2;
        mov dwFull, edi;
    }
    L2: return dwFull;
}

word LeastOver( word n ) {
    __asm {
        bsr ebx, n;
        bsf ecx, n;
        cmp ebx, ecx;
        je to_set;
        inc ebx;
to_set :
        xor eax, eax;
        bts eax, ebx;
    }
}

// c += a*b
int mpmul_s( word* a, sz_t a_used, word* b, sz_t b_used, word* c, word* pc_used ) {
    word dwFull = 0;
    sz_t c_used = a_used + b_used;
    word * d = 0;
    if( c == a ) {
        d = MALLOC( word, a_used );
        MEMCPY( d, a, a_used );
        a = d;
    }
    else if( c == b ) {
        d = b = MALLOC( word, b_used );
        MEMCPY( d, b, b_used );
        b = d;
    }
    __asm {
        mov ecx, b_used;
        jecxz to_end;
L1 :
        mov ecx, a_used;
        jecxz to_end;
        dec ecx;
        mov a_used, ecx;
        //++a
        mov eax, a;
        mov ebx,[eax];
        add eax, 4;
        mov a, eax;
        mov ecx, b_used;
        // c+=b*a[i]
        mov esi, b;
        mov edi, c;
        push 0 /* // 高位 */
L2 :
        mov eax,[esi];
        mul ebx;
        add eax,[edi];
        adc edx, 0;
        add eax,[esp];
        mov[edi], eax;
        adc edx, 0;
        mov[esp], edx;
        add esi, 4;
        add edi, 4;
        loop L2;
        pop eax;
        mov[edi], eax;
        // ++c
        mov eax, c;
        add c, 4;
        mov c, eax;
        jmp L1;
    }
    to_end: if( 0 == *c ) {
        --c_used;
    }
    if( pc_used ) {
        *pc_used = c_used;
    }
    SAFEFREE( d );
    return dwFull;
}

int mpdiv2_s( word* pValue, sz_t last, word dwTimes ) {
    word dwStep = dwTimes / 32;
    if( dwStep >= last ) {
        *pValue = 0;
        last = 1;
        return 0;
    }
    else if( dwStep > 0 ) {
        last -= dwStep;
        MEMCPY( pValue, &pValue[dwStep], last );
    }
    if( dwTimes % 32 != 0 ) {
        __asm {
            mov edi, pValue;
            mov ebx, last;
            dec ebx;
            mov ecx, dwTimes;
            xor edx, edx;
L1 :
            cmp ebx, edx;
            je L2;
            mov eax, 4[edi][edx*4];
            shrd[edi][edx*4], eax, cl;
            inc edx;
            jmp L1;
L2 :
            xor eax, eax;
            shrd[edi][edx*4], eax, cl;
        }
        if( last > 1 && pValue[last-1] == 0 ) {
            last--;
        }
    }
    return 0;
}

int mpdivu_s( word* pValue, sz_t last, word dwValue, sz_t* plast ) {
    __asm {
        mov esi, pValue;
        mov ecx, last;
        mov edx, 0;
        mov ebx, dwValue;
        jmp L2;
L1 :
        mov eax,[esi][ecx*4];
        div ebx;
        mov[esi][ecx*4], eax;
L2 :
        loop L1;
        mov eax,[esi];
        div ebx;
        mov[esi], eax;
        mov eax, last;
        dec eax;
        jz L3;
        mov esi, pValue;
        cmp[esi][4*eax], 0;
        jne L3;
        dec last;
L3 :
        mov eax, edx;
    }
    *plast = last;
    return 0;
}

//商数
// #q>(last-xlast+1)
int mpdiv_s( word* pValue, sz_t last, word* xpValue, sz_t xlast, word* q, sz_t* pq_used, sz_t* pr_used ) {
    word bit_x;
    int i;
    int nXX;
    //减数在减数数组中的编号
    word * xx[32];
    sz_t xxlast = last + 1;
    //除数为0
    assert(( 0 != xlast ) && "除数为0!" );
    //被除数为0或除数为1
    if( last == 0 ||( 1 == xlast && 1 == xpValue[0] ) ) {
        *pq_used = 0;
        *pr_used = 0;
        return 0;
    }
    if( xlast == 1 ) {
        //除数为word时
        return mpdivu_s( pValue, last, xpValue[0], pq_used );
    }
    //减数数组，分别为x*2^i, （i从0到31），暂时为空，如需要再分配
    //xx[0] = new CUSuperInt(x);
    xx[0] = MALLOC( word, xlast );
    MEMCPY( xx[0],( xpValue ), xlast );
    for( i = 1; i < 32; i++ ) {
        xx[i] = NULL;
    }
    //除数高位第一个1的位置(二进制）
    bit_x = xpValue[xlast-1];
    __asm {
        bsr eax, bit_x;
        mov bit_x, eax }
    while( mpcmp( pValue, last, xx[0], xlast ) ) {
        __asm {
            //计算出高word中二进制位1的位置差
            mov esi, pValue;
            mov ecx, last;
            bsr eax,[esi][4*ecx-4];
            sub eax, bit_x;
            add eax, 32;
            and eax, 0x1f;
            mov nXX, eax;
            //如果减数指针为空，则新分配
            mov eax, type xx;
            mul nXX;
            cmp xx[eax], 0;
            jne to_cmp;
        }
        // xx[nXX] = new CUSuperInt(x);
        xx[nXX] = MALLOC( word, xlast );
        MEMCPY( xx[nXX], xpValue, xlast );
        //xx[nXX]->Double(nXX);
        mpmul2_s( xx[nXX], xlast, nXX );
        __asm {
            to_cmp: //比较
            mov eax, type xx;
            mul nXX;
            mov esi, xx[eax];
            mov ecx, xlast;
            mov eax, ecx;
            dec eax;
            shl eax, 2;
            add esi, eax;
            mov edi, pValue;
            mov eax, last;
            dec eax;
            shl eax, 2;
            add edi, eax;
            repe cmpsd;
            jbe to_sub;
            //减数变为前一个
            //如果减数指针为空，则新分配
            add nXX, 31;
            and nXX, 0x1f;
            mov eax, type xx;
            mul nXX;
            cmp xx[eax], 0;
            jne to_sub;
        }
        //xx[nXX] = new CUSuperInt(x);
        xx[nXX] = MALLOC( word, xlast );
        MEMCPY( xx[nXX], xpValue, xlast );
        //
        //xx[nXX]->Double(nXX);
        mpmul2_s( xx[nXX], xlast, nXX );
        __asm {
            to_sub: mov eax, type xx;
            mul nXX;
            mov ebx, xx[eax];
            mov esi, xx[eax];
            mov ecx, xlast + 1;
            mov edi, pValue;
            mov eax, last;
            sub eax, ecx;
            shl eax, 2;
            add edi, eax;
            push eax;
            /*  //保存eax */
            xor ebx, ebx;
            /*  //作为变址寄存器 */
            clc;
sub_loop :
            mov eax,[esi][ebx*4];
            sbb[edi][ebx*4], eax;
            inc ebx;
            loop sub_loop;
            //调整last
            mov edi, pValue;
            mov eax, xxlast;
            mov ecx, eax;
            dec eax;
            shl eax, 2;
            add edi, eax;
            xor eax, eax;
            repe scasd;
            inc ecx;
            mov last, ecx;
            //将商的相应位置1
            mov esi, q;
            pop eax;
            mov ecx, nXX;
            bts dword ptr[esi][eax], ecx;
        }
    }
    for( i = 0; i < 32; i++ ) {
        SAFEFREE( xx[i] );
    }
    return 0;
}

int mp2dec( word* pValue, sz_t last, char* pDec ) {
    word * apValue;
    sz_t alast = last;
    char * p = pDec;
    int n =( int )( last * 32 * log( 2 ) / log( 1000000000 ) + 1 ) * 9;
    //CUSuperInt a(*this);
    word remainder;
    if( NULL == pDec ) {
        return n;
    }
    apValue = MALLOC( word, last );
    MEMCPY( apValue, pValue, last );
    do {
        //remainder = (a/=1000000000);
        remainder = mpdivu_s( apValue, alast, 1000000000, &alast );
        __asm {
            mov eax, remainder;
            mov ebx, 10;
            mov edi, p;
            mov ecx, 9;
L1 :
            cmp eax, 0;
            je L2;
            xor edx, edx;
            div ebx;
            add dl, '0';
            mov[edi], dl;
            inc edi;
            loop L1;
            jmp L3;
L2 :
            mov[edi], '0';
            inc edi;
            loop L2;
L3 :
            mov p, edi;
        }
        if( p + 9 > pDec + n ) {
            int c = 1;
        }
    }
    while( alast > 1 || apValue[0] != 0 );
    while( p[-1] == '0' && p > pDec + 1 ) {
        p--;
    }
    strrev( pDec );
    SAFEFREE( apValue );
    return 0;
}

int mp2hex( word* pValue, sz_t last, char* pHex ) {
    int i, n =( last * 9 + 1 );
    if( NULL == pHex ) {
        return n;
    }
    for( i = last; i > 0; i-- ) {
        sprintf( pHex, "%08x ", pValue[i-1] );
        pHex += 9;
    }
    return 0;
}

// MemCpy.asm 用masm6.11编译生成了big.obj,ML /c /coff big.asm
#if 0
/*
.386P
.model FLAT
.code
_MemCpy   proc    near
        push    ebp
        mov     ebp,esp

  mov ecx, [ebp+16]   ;n
  jecxz to_end;

  ;保存方向标志
  pushf

  mov edi,[ebp+8]
  mov esi, [ebp+12]
  cmp edi, esi
  jbe to_up  ;dst <= src;

  mov eax, ecx
  shl eax, 2
  add eax, esi
  cmp edi, eax
  jae to_up  ;dst >= src+4n

  ;down
  mov eax, ecx
  dec eax
  shl eax, 2
  add edi, eax
  add esi, eax
  std
  jmp to_mov
to_up:
  cld
to_mov:
  rep movsd
  popf
to_end:
  pop ebp
        ret
_MemCpy endp
        end

*/
//#else
//内联汇编编写的memcpy函数
void MemCpy( word* dst, word* src, word n ) {
    __asm {
        mov ecx, n;
        jecxz to_end;
        //保存方向标志
        pushf;
        mov edi, dst;
        mov esi, src;
        cmp edi, esi;
        jbe to_up /*   //dst <= src; */
        mov eax, ecx;
        shl eax, 2;
        add eax, esi;
        cmp edi, eax;
        jae to_up;
        /*   //dst >= src+4n */
        mov eax, ecx;
        dec eax;
        shl eax, 2;
        add edi, eax;
        add esi, eax;
        std;
        jmp to_mov;
to_up :
        cld;
to_mov :
        rep movsd;
        popf;
to_end :
    }
}

#endif