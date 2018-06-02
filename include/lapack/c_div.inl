
 void sig_die(char*,int);
static void c_div(scomplex *c, scomplex *a, scomplex *b)
{
        double ratio, den;
        double abr, abi, cr;

        if( (abr = b->r) < 0.)
                abr = - abr;
        if( (abi = b->i) < 0.)
                abi = - abi;
        if( abr <= abi )
                {
                if(abi == 0) {
#ifdef IEEE_COMPLEX_DIVIDE
                        float af, bf;
                        af = bf = (float)abr;
                        if (a->i != 0 || a->r != 0)
                                af = 1.;
                        c->i = c->r = af / bf;
                        return;
#else
                        sig_die("scomplex division by zero", 1);
#endif
                        }
                ratio = (double)b->r / b->i ;
                den = b->i * (1 + ratio*ratio);
                cr = (a->r*ratio + a->i) / den;
                c->i = (real)((a->i*ratio - a->r) / den);
                }

        else
                {
                ratio = (double)b->i / b->r ;
                den = b->r * (1 + ratio*ratio);
                cr = (a->r + a->i*ratio) / den;
                c->i = (real)((a->i - a->r*ratio) / den);
                }
        c->r = (real)cr;
}
