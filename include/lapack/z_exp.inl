

#ifdef KR_headers
double exp(), cos(), sin();
VOID z_exp(r, z) dcomplex *r, *z;
#else


void z_exp(dcomplex *r, dcomplex *z)
#endif
{
        double expx, zi = z->i;

        expx = exp(z->r);
        r->r = expx * cos(zi);
        r->i = expx * sin(zi);
        }

