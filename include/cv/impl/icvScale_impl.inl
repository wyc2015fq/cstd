{
  int x, y;
  
  if (1 == a && 0 == b) {
    for (y=0; y<size.h; ++y, src+=srcstep, dst+=dststep) {
      for (x = 0; x <= size.w - 4; x += 4) {
        srctype t0 = src[x];
        srctype t1 = src[x + 1];
        
        dst[x] = (dsttype)t0;
        dst[x + 1] = (dsttype)t1;
        
        t0 = src[x + 2];
        t1 = src[x + 3];
        
        dst[x + 2] = (dsttype)t0;
        dst[x + 3] = (dsttype)t1;
      }
      
      for (; x < size.w; x++) {
        dst[x] = (dsttype)src[x];
      }
    }
  }
  else {
    for (y=0; y<size.h; ++y, src+=srcstep, dst+=dststep) {
      for (x = 0; x <= size.w - 4; x += 4) {
        worktype t0 = src[x] * a + b;
        worktype t1 = src[x + 1] * a + b;
        
        dst[x] = (dsttype)t0;
        dst[x + 1] = (dsttype)t1;
        
        t0 = src[x + 2] * a + b;
        t1 = src[x + 3] * a + b;
        
        dst[x + 2] = (dsttype)t0;
        dst[x + 3] = (dsttype)t1;
      }
      
      for (; x < size.w; x++) {
        dst[x] = (dsttype)(src[x] * a + b);
      }
    }
  }
}
#undef cast_macro1
