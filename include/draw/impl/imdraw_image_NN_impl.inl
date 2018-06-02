if (RCH(&brc)>0 && RCW(&brc)>0) {
  int h, w, x, y;
  int asx, asy, bsx, bsy;
  IRECT arc0, brc0;
  int th = (RCH(&arc)<<8)/RCH(&brc);
  int tw = (RCW(&arc)<<8)/RCW(&brc);
  
  arc0 = iRectInter(arc, iRECT(0, 0, aw, ah));
  brc0 = iRectInter(brc, iRECT(0, 0, bw, bh));
  
  h = RCH(&brc0), w = RCW(&brc0);
  bsx = brc0.l - brc.l, bsy = brc0.t - brc.t;
  asx = arc0.l - arc.l, asy = arc0.t - arc.t;
  if (RCH(&arc0)==h && RCW(&arc0)==w) {
    switch (bi) {
    case 1:
      for (y = 0; y < h; ++y) {
        const uchar* a0 = a + (y+arc0.t)*al + arc0.l*ai;
        uchar* b0 = b + (y+brc0.t)*bl + brc0.l*bi;
        for (x = 0; x < w; ++x, b0+=bi, a0+=ai) {
          PUTPIX1(b0, a0);
        }
      }
      break;
    case 3:
      for (y = 0; y < h; ++y) {
        const uchar* a0 = a + (y+arc0.t)*al + arc0.l*ai;
        uchar* b0 = b + (y+brc0.t)*bl + brc0.l*bi;
        for (x = 0; x < w; ++x, b0+=bi, a0+=ai) {
          PUTPIX3(b0, a0);
        }
      }
      break;
    case 4:
      for (y = 0; y < h; ++y) {
        const uchar* a0 = a + (y+arc0.t)*al + arc0.l*ai;
        uchar* b0 = b + (y+brc0.t)*bl + brc0.l*bi;
        for (x = 0; x < w; ++x, b0+=bi, a0+=ai) {
          PUTPIX4(b0, a0);
        }
      }
      break;
    }
  } else {
    int* ax_off=0;
    ax_off = MALLOC(int, w);
    for (x = 0; x < w; ++x) {
      ax_off[x] = ((((x+bsx) * tw)>>8) + arc.l)*ai;
    }
    switch (bi) {
    case 1:
      for (y = 0; y < h; ++y) {
        int ay = (((y+bsy) * th)>>8) + arc.t;
        int by = brc0.t + y;
        const uchar* a1 = a + ay*al;
        uchar* b1 = b + by*bl + brc0.l*bi;
        uchar* b0=b1;
        for (x = 0; x < w; ++x, b0+=bi) {
          const uchar* a0 = a1 + ax_off[x];
          PUTPIX1(b0, a0);
        }
      }
      break;
    case 3:
      for (y = 0; y < h; ++y) {
        int ay = (((y+bsy) * th)>>8) + arc.t;
        int by = brc0.t + y;
        const uchar* a1 = a + ay*al;
        uchar* b1 = b + by*bl + brc0.l*bi;
        uchar* b0=b1;
        for (x = 0; x < w; ++x, b0+=bi) {
          const uchar* a0 = a1 + ax_off[x];
          PUTPIX3(b0, a0);
        }
      }
      break;
    case 4:
      for (y = 0; y < h; ++y) {
        int ay = (((y+bsy) * th)>>8) + arc.t;
        int by = brc0.t + y;
        const uchar* a1 = a + ay*al;
        uchar* b1 = b + by*bl + brc0.l*bi;
        uchar* b0=b1;
        for (x = 0; x < w; ++x, b0+=bi) {
          const uchar* a0 = a1 + ax_off[x];
          PUTPIX4(b0, a0);
        }
      }
      break;
    }
    FREE(ax_off);
  }
}

#undef PUTPIX1
#undef PUTPIX3
#undef PUTPIX4
#undef PUTPIXX
#undef PUTPIXA
