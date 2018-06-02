
#ifndef _XTYPES_H_
typedef struct CvComplex64f { double re, im; }CvComplex64f;
CvComplex64f cvComplex64f( double _re, double _im ) { CvComplex64f c; c.re=(_re), c.im=(_im); return c; }

typedef struct CvComplex32f { float re, im; }CvComplex32f;
CvComplex32f cvComplex32f( float _re, float _im ) { CvComplex32f c; c.re=(_re), c.im=(_im); return c; }
#endif // _XTYPES_H_

#define CvComplex_set(c,a, b)      (c.re=a, c.im=b)
#define CvComplex_neg(c,a)         (c.re=-a.re, c.im=-a.im )
#define CvComplex_conj(c,a)        (c.re=a.re, c.im=-a.im )
#define CvComplex_add(c,a, b)      (c.re=a.re + b.re, c.im=a.im + b.im )
#define CvComplex_sub(c,a, b)      (c.re=a.re - b.re, c.im=a.im - b.im )
#define CvComplex_mul(c,a, b)      (c.re=a.re*b.re - a.im*b.im, c.im=a.re*b.im + a.im*b.re )
#define CvComplex_mula(c,a, b)     (c.re=a*b.re, c.im=a*b.im )
#define CvComplex_div(c,a, b)      (c.im = 1./(b.re*b.re + b.im*b.im),c.re= (TYPE)((a.re*b.re + a.im*b.im)*c.im),c.im=(TYPE)((-a.re*b.im + a.im*b.re)*c.im) )
#define CvComplex_abs(c,a)         (sqrt( (TYPE)a.re*a.re + (TYPE)a.im*a.im ))
