
#include "img/imgio.inl"
int test_img_comp() {
  const char *in_fn = "Z:/wenping/ryw_1520663161.png";
  const char *out_fn = "D:/ryw_1520663161_200k.jpg";
  img_t im[1] = {0};
  if (imread(in_fn, 3, 1, im)) {
    imwrite(out_fn, im);
  }
  return 0;
}
