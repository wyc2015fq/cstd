{
  int x, y;
  uchar* sbuf=0;
  const uchar* src = (const uchar*)s;
  uchar* dst = (uchar*)d;
  arrtype t0, t1;

  if (src==dst) {
    sbuf = MALLOC(uchar, height*width*cn);
    cvCopy(src, srcstep, sbuf, width*cn, iSIZE(width*cn, height));
    src = sbuf, srcstep = width*cn;
  }

  for (y = 0; y <= height - 2; y += 2, src += 2 * srcstep, dst += 2*sizeof(arrtype)) {
    const arrtype* src0 = (const arrtype*)src;
    const arrtype* src1 = (const arrtype*)(src + srcstep);
    uchar* dst_ = dst;

    for (x = 0; x <= width - 2; x += 2, dst_ += 2*dststep) {
      arrtype* dst0 = (arrtype*)dst_;
      arrtype* dst1 = (arrtype*)(dst_ + dststep);
      t0 = src0[x];
      t1 = src1[x];
      dst0[0] = t0;
      dst0[1] = t1;
      
      t0 = src0[x + 1];
      t1 = src1[x + 1];
      dst1[0] = t0;
      dst1[1] = t1;
    }

    if (x < width) {
      arrtype* dst0 = (arrtype*)dst_;
      arrtype* dst1 = (arrtype*)(dst_ + dststep);
      t0 = src0[x];
      t1 = src1[x];
      dst0[0] = t0;
      dst0[1] = t1;
    }
  }

  if (y < height) {
    const arrtype* src0 = (const arrtype*)src;
    uchar* dst_ = dst;

    for (x = 0; x <= width - 2; x += 2, dst_ += 2 * dststep) {
      arrtype* dst0 = (arrtype*)dst_;
      arrtype* dst1 = (arrtype*)(dst_ + dststep);
      t0 = src0[x];
      t1 = src0[x + 1];
      dst0[0] = t0;
      dst1[0] = t1;
    }

    if (x < width) {
      arrtype* dst0 = (arrtype*)dst_;
      t0 = src0[x];
      dst0[0] = t0;
    }
  }
  if (sbuf) {
    FREE(sbuf);
  }
}