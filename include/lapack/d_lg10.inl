

#define log10e 0.43429448190325182765

static double d_lg10(doublereal *x)

{
return( log10e * log(*x) );
}

