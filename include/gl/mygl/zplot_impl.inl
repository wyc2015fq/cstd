{
  if (point_size < 2) {
    unsigned short* pz = zb->zbuf + (p->y * zb->w + p->x);
    zz = p->z >> ZB_POINT_Z_FRAC_BITS;
    if (ZCMP(zz, *pz)) {
      uchar* pp = zb->pbuf + zb->step * p->y + p->x * zb->cn;
      PUT_PIX();
    }
    *pz = zz;
  }
  else {
    int y0 = p->y - point_size / 2;
    int y1 = y0 + point_size;
    int x0 = p->x - point_size / 2;
    int x1 = x0 + point_size;
    int x = 0, y = 0;
    x0 = BOUND(x0, 0, zb->w - 1);
    x1 = BOUND(x1, 0, zb->w);
    y0 = BOUND(y0, 0, zb->h - 1);
    y1 = BOUND(y1, 0, zb->h);
    for (y = y0; y < y1; ++y) {
      unsigned short* pz = zb->zbuf + (p->y * zb->w + x0);
      uchar* pp = zb->pbuf + zb->step * y + x0 * zb->cn;
      for (x = x0; x < x1; ++x, pp += zb->cn, ++pz) {
        zz = p->z >> ZB_POINT_Z_FRAC_BITS;
        if (ZCMP(zz, *pz)) {
          PUT_PIX();
        }
        *pz = zz;
      }
    }
  }
}
#undef PUT_PIX

