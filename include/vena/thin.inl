
#include "applylutc.inl"


int bf_imthin(buf_t* bf, const img_t* im, img_t* im2, int iterations) {
  int i;
  img_t tmp[1] = {0};
  img_t lut[1] = {0};
  static const uchar luteven[] = {
#include "luteven.txt"
  };
  IMINIT(lut, countof(luteven), 1, luteven, 1, 1, 1);
  bf_imclone2(bf, im, im2);
  bf_imclone2(bf, im, tmp);
  for (i=0; i<iterations; ++i) {
    int t = Compute3by3LogicalResult(tmp, im2, lut);
    imclone(tmp, im2);
    if (!t) {
      break;
    }
  }
  bf_imfree(bf, tmp);
  return 0;
}