
int test_showimage(ctrl_t* c) {
  static window_t win[10] = {0};
  static ctrl_t cc[10] = {0};
  static int inited = 0;
  static texture_t tex[1] = {0};
  static IPOINT pos = {0};
  static double scale = 1;
  static FPOINT dpos = {0};
  static img_t im[1] = {0};
  int i = 0;
  SYSIO;
  if (!inited) {
    render_drv_t* drv = io->cur_host->drv;
    const char* fn = NULL;
    inited = 1;
    fn = "E:/58.jpg";
    fn = "E:/pub/pic/bb05.png";
    win[0].rc = iRECT2(10, 10, 200, 200);
    //loadTextureFile(host->drv, "E:/58.jpg", tex);
    if (imread(fn, 4, 1, im)) {
      PixFmt fmt = cn2PixFmt(im->c);
      //imshow(im);cvWaitKey(-1);
      drv->SetTexture(drv, tex, fmt, im->h, im->w, im->tt.data, im->s);
    }
  }
  if (1) {
    COLOR clr = 0;
    int ix = (int)dpos.x;
    int iy = (int)dpos.y;
    if (ix>=0 && ix<im->w && iy>=0 && iy<im->h) {
      clr = *(COLOR*)(im->tt.data + iy*im->s + ix*im->c);
    }
    i = 0;
    c->sz = flexboxlay(c->rc, countof(cc), cc, 0, flex_column);
    fmtlabel_ctrl(cc+i++, "X=%.2f, Y=%.2f R=%d G=%d B=%d A=%d", dpos.x, dpos.y, GetRV(clr), GetGV(clr), GetBV(clr), GetAV(clr));
    cc[i].rc.b = c->rc.b;
    image_ctrl(cc+i++, tex, &pos, &scale, &dpos);
  } else {
    if (window_begin(&win[0], "", 0)) {
      image_ctrl(win[0].client, tex, &pos, &scale, &dpos);
      window_end();
    }
  }
  return 0;
}