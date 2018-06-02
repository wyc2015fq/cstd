const arrtype* src = (const arrtype*)src0;
arrtype* dst = (arrtype*)dst0;
arrtype* dst1, t0, t1;
int x;

width *= 4;
srcstep /= sizeof(src[0]);
dststep /= sizeof(dst[0]);

for (; height--; src += srcstep, dst += 4)
{
  dst1 = dst;

  for (x = 0; x < width; x += 4, dst1 += dststep) {
    t0 = src[x];
    t1 = src[x + 1];

    dst1[0] = t0;
    dst1[1] = t1;

    t0 = src[x + 2];
    t1 = src[x + 3];

    dst1[2] = t0;
    dst1[3] = t1;
  }
}