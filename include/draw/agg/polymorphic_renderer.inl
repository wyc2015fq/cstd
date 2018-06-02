
int polymorphic_renderer(ctrl_t* c) {
  SYSIO;SOFTGC_ARG;
  IRECT rc = c->rc;
  FPOINT pt[] = {
     100, 60,
     369, 170,
     143, 310,
  };
  int len = countof(pt);
  //imclear(im, &rc, _RGB(255, 255, 255));
  scanline_pushcliprect(sg->sl, rc);
  brush_set_solid(sg->brush, _RGB(80, 30, 20));
  scanline_set_poly(sg->sl, pt, &len, 1, false);
  brush_set_solid(sg->brush, _RGB(255, 30, 20));
  scanline_set_recti(sg->sl, iRECT2(100, 300, 100, 100));
  scanline_popcliprect(sg->sl);
  return 0;
}
