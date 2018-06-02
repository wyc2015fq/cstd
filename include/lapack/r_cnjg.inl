
static VOID r_cnjg(scomplex *r, scomplex *z)
{
        real zi = z->i;
        r->r = z->r;
        r->i = -zi;
}

