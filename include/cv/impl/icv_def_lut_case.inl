
{
  arrtype* dst = (arrtype*)dst0;
  const arrtype* lut = (const arrtype*)lut0;

#if 0
  arrtype* shuffled_lut = 0;

  if (src_is_signed) {
    int half_size = cn * 128;
    shuffled_lut = MALLOC(arrtype, half_size * 2);

    // shuffle lut
    memcpy(shuffled_lut, lut + half_size, half_size);
    memcpy(shuffled_lut + half_size, lut, half_size);

    lut = shuffled_lut;
  }

#endif


  dststep /= sizeof(dst[0]);

  switch (cn) {
  case 1: {
    for (; size.h--; src += srcstep, dst += dststep) {
      for (i = 0; i <= size.w - 4; i += 4) {
        arrtype t0 = lut[src[i]];
        arrtype t1 = lut[src[i + 1]];
        dst[i] = t0;
        dst[i + 1] = t1;

        t0 = lut[src[i + 2]];
        t1 = lut[src[i + 3]];
        dst[i + 2] = t0;
        dst[i + 3] = t1;
      }
    }

    for (; i < size.w; i++) {
      arrtype t0 = lut[src[i]];
      dst[i] = t0;
    }
  }
  break;

  case 2:
    for (; size.h--; src += srcstep, dst += dststep) {
      for (i = 0; i < size.w; i += 2) {
        arrtype t0 = lut[src[i] * 2];
        arrtype t1 = lut[src[i + 1] * 2 + 1];
        dst[i] = t0;
        dst[i + 1] = t1;
      }

    }

    break;

  case 3:
    for (; size.h--; src += srcstep, dst += dststep) {

      for (i = 0; i < size.w; i += 3) {
        arrtype t0 = lut[src[i] * 3];
        arrtype t1 = lut[src[i + 1] * 3 + 1];
        arrtype t2 = lut[src[i + 2] * 3 + 2];
        dst[i] = t0;
        dst[i + 1] = t1;
        dst[i + 2] = t2;
      }

    }

    break;

  case 4:
    for (; size.h--; src += srcstep, dst += dststep) {

      for (i = 0; i < size.w; i += 4) {
        arrtype t0 = lut[src[i] * 4];
        arrtype t1 = lut[src[i + 1] * 4 + 1];
        dst[i] = t0;
        dst[i + 1] = t1;
        t0 = lut[src[i + 2] * 4 + 2];
        t1 = lut[src[i + 3] * 4 + 3];
        dst[i + 2] = t0;
        dst[i + 3] = t1;
      }
    }

    break;

  default: {
    typedef arrtype dsttype;
    int max_block_size = (1 << 10) * cn;
    dsttype lutp[1024];
    int i, k;
    const dsttype* _lut = lut;

    if (size.w * size.h < 256) {
      for (; size.h--; src += srcstep, dst += dststep) {
        for (k = 0; k < cn; k++) {
          for (i = 0; i < size.w; i += cn) {
            dst[i + k] = _lut[src[i + k] * cn + k];
          }
        }
      }
    }
    else {
      /* repack the lut to planar layout */
      for (k = 0; k < cn; k++) {
        for (i = 0; i < 256; i++) {
          lutp[i + k * 256] = _lut[i * cn + k];
        }
      }

      for (; size.h--; src += srcstep, dst += dststep) {
        for (i = 0; i < size.w;) {
          int j, limit = MIN(size.w, i + max_block_size);

          for (k = 0; k < cn; k++, src++, dst++) {
            const dsttype* lut = lutp + k * 256;

            for (j = i; j <= limit - cn * 2; j += cn * 2) {
              dsttype t0 = lut[src[j]];
              dsttype t1 = lut[src[j + cn]];
              dst[j] = t0;
              dst[j + cn] = t1;
            }

            for (; j < limit; j += cn) {
              dst[j] = lut[src[j]];
            }
          }

          src -= cn;
          dst -= cn;
          i += limit;
        }
      }
    }
  }

  break;
  }

}