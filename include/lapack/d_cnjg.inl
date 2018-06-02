


static  VOID d_cnjg(dcomplex *r, dcomplex *z)
{
        doublereal zi = z->i;
        r->r = z->r;
        r->i = -zi;
        }

