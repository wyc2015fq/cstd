
#define icv_morph_rect_row(name, flavor, arrtype, worktype, update_extr_macro)
static void icv##name##RectRow_##flavor(const arrtype* src, arrtype* dst, void* params)
{
  const CvMorphology* state = (const CvMorphology*)params;
  int ksize.w = state->get_kernel_size().width;
  int width = state->get_width();
  int cn = CC_MAT_CN(state->get_src_type());
  int i, j, k;

  width *= cn;
  ksize.w *= cn;

  if (ksize.w == cn) {
    for (i = 0; i < width; i++) {
      dst[i] = src[i];
    }

    return;
  }

  for (k = 0; k < cn; k++, src++, dst++) {
    for (i = 0; i <= width - cn * 2; i += cn * 2) {
      const arrtype* s = src + i;
      worktype m = s[cn], t;

      for (j = cn * 2; j < ksize.w; j += cn) {
        t = s[j];
        update_extr_macro(m, t);
      }

      t = s[0];
      update_extr_macro(t, m);
      dst[i] = (arrtype)t;
      t = s[j];
      update_extr_macro(t, m);
      dst[i + cn] = (arrtype)t;
    }

    for (; i < width; i += cn) {
      const arrtype* s = src + i;
      worktype m = s[0], t;

      for (j = cn; j < ksize.w; j += cn) {
        t = s[j];
        update_extr_macro(m, t);
      }

      dst[i] = (arrtype)m;
    }
  }
}
