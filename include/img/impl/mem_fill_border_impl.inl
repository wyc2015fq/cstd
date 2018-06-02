{
  int y, x;
  const arrtype* a0 = (const arrtype*)A;
  arrtype* dst0 = (arrtype*)dst;
  ASSERT(ah <= bh && aw <= bw);
  al /= sizeof(arrtype);
  bl /= sizeof(arrtype);
  
  switch (type) {
  case BD_CONSTANT:
    {
      const arrtype* a1 = a0;
      arrtype c0;
      int law = MIN(l + aw, bw);
      if (c) {
        c0 = *(arrtype*)c;
      } else {
        memset(&c0, 0, sizeof(arrtype));
      }
      
      for (y = 0; y < bh; ++y, dst0 += bl) {
        if (y >= t && y < (t + ah)) {
          a0 = a1 + (y - t) * al;
          for (x = MAX(0, l); x < law; ++x) {
            dst0[x] = a0[x - l];
          }
          
          for (x = 0; x < MIN(l, bw); ++x) {
            dst0[x] = c0;
          }
          
          for (x = MAX(0, l + aw); x < bw; ++x) {
            dst0[x] = c0;
          }
        }
        else {
          for (x = 0; x < bw; ++x) {
            dst0[x] = c0;
          }
        }
      }
    }
    break;
    
  default:
    {
      int* xoff=0;
      xoff = MALLOC(int, bw);
      for (x = 0; x < bw; ++x) {
        xoff[x] = border_interpolate(x-l, aw, type);
      }
      for (y = 0; y < bh; ++y, dst0+=bl) {
        int ay = border_interpolate(y-t, ah, type);
        const arrtype* a1 = a0 + ay*al;
        for (x = 0; x < bw-3; x+=4) {
          dst0[x+0] = a1[xoff[x+0]];
          dst0[x+1] = a1[xoff[x+1]];
          dst0[x+2] = a1[xoff[x+2]];
          dst0[x+3] = a1[xoff[x+3]];
        }
        for (; x < bw; ++x) {
          dst0[x] = a1[xoff[x]];
        }
      }
      FREE(xoff);
    }
    break;
  }
}
