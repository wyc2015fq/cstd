{
  IMARGDEFTT;
  IRECT rc1 = iRectInter(rc, iRectClip(pclip, 0, 0, w, h));
  int rch=RCH(&rc), rcw=RCW(&rc);
  int rch1=RCH(&rc1), rcw1=RCW(&rc1);
  int x, y, xoff=rc1.l-rc.l, yoff=rc1.t-rc.t;
  const uchar* pclr = 0;
  
  if (rch>0 && rcw>0) {
    for (y = 0; y < rch1; ++y) {
      uchar* ptr_x=img + (rc1.t+y)*step + (rc1.l)*cn;
      int y0 = y+yoff;
      for (x = 0; x < rcw1; ++x, ptr_x+=cn) {
        int x0 = x+xoff;
        PUTPIXX(ptr_x, x0, y0);
      }
    }
  }
}
#undef PUTPIXX

