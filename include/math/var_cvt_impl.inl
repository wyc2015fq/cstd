if (0 == va->nn)
{
  switch (tttt) {
#define TYPECVTDEF(a, b, c, d, e, f, g, h) case (a*100+e): VAR_CVT(va->f, (h)(vb->b)); break;
#include "typecvt.def"
#undef TYPECVTDEF
  default:
    break;
  }
}
else if (2 == va->nn)
{
  const arr2_t* a2 = vb->a2;
  int i, h1 = a2->h, w1 = a2->w, n = h1 * w1;
  switch (tttt) {
#define TYPECVTDEF(a, b, c, d, e, f, g, h) case (a*100+e): for (i=0; i<n; ++i) VAR_CVT(va->a2->f[i], (h)(vb->a2->b[i])); break;
#include "typecvt.def"
#undef TYPECVTDEF
  default:
    break;
  }
}

