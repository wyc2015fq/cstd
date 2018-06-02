
typedef struct {
  img_t im[1];
  IPOINT pos;
  double scale;
  FPOINT dpos;
  char name[32];
} image_view_t;

int imgshop(ctrl_t* c, image_view_t* imv, int n, int* psel) {
  static ctrl_t cc[10];
  int i = 0;
  IRECT rc = c->rc;
  ctrl_t* tabctrl = cc + i++;
  ctrl_t* imgctrl = cc + i++;
  flexboxlay(rc, 2, cc, 0, flex_column);
  imgctrl->rc.b = rc.b;
  if (n>0) {
    const char* strs[100] = {0};
    for (i=0; i<n; ++i) {
      strs[i] = imv[i].name;
    }
    tab_ctrl(tabctrl, n, strs, NULL, psel);
    *psel = BOUND(*psel, 0, n-1);
  }
  if (n>0) {
    texture_t tex[1] = {0};
    image_view_t* imv1 = imv + *psel;
    img_t* im = imv1->im;
    BMPINITIM(tex, im);
    image_ctrl(imgctrl, tex, &imv1->pos, &imv1->scale, &imv1->dpos);
  }
  return 0;
}
