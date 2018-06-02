
#define icv_morph_rect_col(name, flavor, arrtype,
worktype, update_extr_macro, toggle_macro)
static void
icv##name##RectCol_##flavor(const arrtype** src,
    arrtype* dst, int dst_step, int count, void* params)
{
  const CvMorphology* state = (const CvMorphology*)params;
  int ksize.h = state->get_kernel_size().height;
  int width = state->get_width();
  int cn = CC_MAT_CN(state->get_src_type());
  int i, k;

  width *= cn;
  dst_step /= sizeof(dst[0]);

  for (; ksize.h > 1 && count > 1; count -= 2, dst += dst_step * 2, src += 2) {
    for (i = 0; i <= width - 4; i += 4) {
      const arrtype* sptr = src[1] + i;
      worktype s0 = sptr[0], s1 = sptr[1],
               s2 = sptr[2], s3 = sptr[3], t0, t1;

      for (k = 2; k < ksize.h; k++) {
        sptr = src[k] + i;
        t0 = sptr[0];
        t1 = sptr[1];
        update_extr_macro(s0, t0);
        update_extr_macro(s1, t1);
        t0 = sptr[2];
        t1 = sptr[3];
        update_extr_macro(s2, t0);
        update_extr_macro(s3, t1);
      }

      sptr = src[0] + i;
      t0 = sptr[0];
      t1 = sptr[1];
      update_extr_macro(t0, s0);
      update_extr_macro(t1, s1);
      dst[i] = (arrtype)toggle_macro(t0);
      dst[i + 1] = (arrtype)toggle_macro(t1);
      t0 = sptr[2];
      t1 = sptr[3];
      update_extr_macro(t0, s2);
      update_extr_macro(t1, s3);
      dst[i + 2] = (arrtype)toggle_macro(t0);
      dst[i + 3] = (arrtype)toggle_macro(t1);

      sptr = src[k] + i;
      t0 = sptr[0];
      t1 = sptr[1];
      update_extr_macro(t0, s0);
      update_extr_macro(t1, s1);
      dst[i + dst_step] = (arrtype)toggle_macro(t0);
      dst[i + dst_step + 1] = (arrtype)toggle_macro(t1);
      t0 = sptr[2];
      t1 = sptr[3];
      update_extr_macro(t0, s2);
      update_extr_macro(t1, s3);
      dst[i + dst_step + 2] = (arrtype)toggle_macro(t0);
      dst[i + dst_step + 3] = (arrtype)toggle_macro(t1);
    }

    for (; i < width; i++) {
      const arrtype* sptr = src[1] + i;
      worktype s0 = sptr[0], t0;

      for (k = 2; k < ksize.h; k++) {
        sptr = src[k] + i;
        t0 = sptr[0];
        update_extr_macro(s0, t0);
      }

      sptr = src[0] + i;
      t0 = sptr[0];
      update_extr_macro(t0, s0);
      dst[i] = (arrtype)toggle_macro(t0);

      sptr = src[k] + i;
      t0 = sptr[0];
      update_extr_macro(t0, s0);
      dst[i + dst_step] = (arrtype)toggle_macro(t0);
    }
  }

  for (; count > 0; count--, dst += dst_step, src++) {
    for (i = 0; i <= width - 4; i += 4) {
      const arrtype* sptr = src[0] + i;
      worktype s0 = sptr[0], s1 = sptr[1],
               s2 = sptr[2], s3 = sptr[3], t0, t1;

      for (k = 1; k < ksize.h; k++) {
        sptr = src[k] + i;
        t0 = sptr[0];
        t1 = sptr[1];
        update_extr_macro(s0, t0);
        update_extr_macro(s1, t1);
        t0 = sptr[2];
        t1 = sptr[3];
        update_extr_macro(s2, t0);
        update_extr_macro(s3, t1);
      }

      dst[i] = (arrtype)toggle_macro(s0);
      dst[i + 1] = (arrtype)toggle_macro(s1);
      dst[i + 2] = (arrtype)toggle_macro(s2);
      dst[i + 3] = (arrtype)toggle_macro(s3);
    }

    for (; i < width; i++) {
      const arrtype* sptr = src[0] + i;
      worktype s0 = sptr[0], t0;

      for (k = 1; k < ksize.h; k++) {
        sptr = src[k] + i;
        t0 = sptr[0];
        update_extr_macro(s0, t0);
      }

      dst[i] = (arrtype)toggle_macro(s0);
    }
  }
}
