

 void pow_zi(dcomplex*, dcomplex*, integer*);
static void pow_ci(scomplex *p, scomplex *a, integer *b)         /* p = a**b  */
{
dcomplex p1, a1;

a1.r = a->r;
a1.i = a->i;

pow_zi(&p1, &a1, b);

p->r = p1.r;
p->i = p1.i;
}
