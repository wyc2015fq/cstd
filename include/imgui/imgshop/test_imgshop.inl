
#include "imgshop_ctrl.inl"


int test_imgshop(ctrl_t* c) {
  static ctrl_t cc[10] = {0};
  static int inited = 0;
  static image_view_t imv[10] = {0};
  static int nim = 0;
  static int sel = 0;
  int i = 0;
  SYSIO;
  if (!inited) {
    const char* fn[] = {
      "aa_02.jpg",
      "bb05.png",
    };
    inited = 1;
    sys_chdir("E:/pub/pic/");
    nim = countof(fn);
    for (i=0; i<countof(fn); ++i) {
      imread(fn[i], 4, 1, imv[i].im);
      strncpy(imv[i].name, fn[i], 32);
      imv[i].scale = 1;
    }
    sel = 0;
  }
  if (1) {
    COLOR clr = 0;
    img_t* im = imv[sel].im;
    FPOINT dpos = imv[sel].dpos;
    int ix = (int)dpos.x;
    int iy = (int)dpos.y;
    if (ix>=0 && ix<im->w && iy>=0 && iy<im->h) {
      clr = *(COLOR*)(im->tt.data + iy*im->s + ix*im->c);
    }
    i = 0;
    color_background_ctrl(c->rc, _RGB(222, 222, 222), _RGB(122, 122, 122), true, true);
    c->sz = flexboxlay(c->rc, countof(cc), cc, 0, flex_column);
    fmtlabel_ctrl(cc+i++, "X=%.2f, Y=%.2f R=%d G=%d B=%d A=%d", dpos.x, dpos.y, GetRV(clr), GetGV(clr), GetBV(clr), GetAV(clr));
    cc[i].rc.b = c->rc.b;
    //image_ctrl(cc+i++, tex, &pos, &scale, &dpos);
    imgshop(cc+i++, imv, nim, &sel);
  }
  return 0;
}
