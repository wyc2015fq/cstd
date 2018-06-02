{
  if (dim > 1) {
    CC_ERROR(CC_StsOutOfRange, "The reduced dimensionality index is out of range");
  }

  if (dim == 0) {
    //IPPI_CALL(rfunc(src->tt.data, src->s ? src->s : CC_STUB_STEP, dst->tt.data, size));

    //#define ICC_ACC_ROWS_FUNC(name, flavor, arrtype, acctype, __op__, load_macro)
    //static CStatus icv##name##Rows_##flavor##_C1R(const arrtype* src, int srcstep, acctype* dst, CSize size)
    int i, width = size.w * cn;

    for (i = 0; i < width; i++) {
      dst[i] = load_macro(src[i]);
    }

    for (; --size.h;) {
      src += srcstep;

      for (i = 0; i <= width - 4; i += 4) {
        acctype s0 = load_macro(src[i]);
        acctype s1 = load_macro(src[i + 1]);
        acctype a0 = dst[i], a1 = dst[i + 1];
        a0 = (acctype)__op__(a0, s0);
        a1 = (acctype)__op__(a1, s1);
        dst[i] = a0;
        dst[i + 1] = a1;

        s0 = load_macro(src[i + 2]);
        s1 = load_macro(src[i + 3]);
        a0 = dst[i + 2];
        a1 = dst[i + 3];
        a0 = (acctype)__op__(a0, s0);
        a1 = (acctype)__op__(a1, s1);
        dst[i + 2] = a0;
        dst[i + 3] = a1;
      }

      for (; i < width; i++) {
        acctype s0 = load_macro(src[i]), a0 = dst[i];
        a0 = (acctype)__op__(a0, s0);
        dst[i] = a0;
      }
    }
  }
  else
  {
    //IPPI_CALL(cfunc(src->tt.data, src->s ? src->s : CC_STUB_STEP, dst->tt.data, dst->s ? dst->s : CC_STUB_STEP, size));
    switch (cn) {
    case 1: {
      //#define ICC_ACC_COLS_FUNC_C1(name, flavor, arrtype, worktype, acctype, __op__)
      // static CStatus icv##name##Cols_##flavor##_C1R(const arrtype* src, int srcstep, acctype* dst, int dststep, CSize size)
      int i, width = size.w;

      for (; size.h--; src += srcstep, dst += dststep) {
        if (width == 1) {
          dst[0] = (acctype)src[0];
        }
        else {
          worktype a0 = src[0], a1 = src[1];

          for (i = 2; i <= width - 4; i += 4) {
            worktype s0 = src[i], s1 = src[i + 1];
            a0 = __op__(a0, s0);
            a1 = __op__(a1, s1);
            s0 = src[i + 2];
            s1 = src[i + 3];
            a0 = __op__(a0, s0);
            a1 = __op__(a1, s1);
          }

          for (; i < width; i++) {
            worktype s0 = src[i];
            a0 = __op__(a0, s0);
          }

          a0 = __op__(a0, a1);
          dst[0] = (acctype)a0;
        }
      }

    }
    break;

    case 2: {
    }
    break;

    case 3: {
      //#define ICC_ACC_COLS_FUNC_C3(name, flavor, arrtype, worktype, acctype, __op__)
      //static CStatus icv##name##Cols_##flavor##_C3R(const arrtype* src, int srcstep,acctype* dst, int dststep, CSize size)
      int i, width = size.w * 3;

      for (; size.h--; src += srcstep, dst += dststep) {
        worktype a0 = src[0], a1 = src[1], a2 = src[2];

        for (i = 3; i < width; i += 3) {
          worktype s0 = src[i], s1 = src[i + 1], s2 = src[i + 2];
          a0 = __op__(a0, s0);
          a1 = __op__(a1, s1);
          a2 = __op__(a2, s2);
        }

        dst[0] = (acctype)a0;
        dst[1] = (acctype)a1;
        dst[2] = (acctype)a2;
      }

    }
    break;

    case 4: {

      // #define ICC_ACC_COLS_FUNC_C4(name, flavor, arrtype, worktype, acctype, __op__)
      //static CStatus icv##name##Cols_##flavor##_C4R(const arrtype* src, int srcstep, acctype* dst, int dststep, CSize size)

      int i, width = size.w * 4;

      for (; size.h--; src += srcstep, dst += dststep) {
        worktype a0 = src[0], a1 = src[1], a2 = src[2], a3 = src[3];

        for (i = 4; i < width; i += 4) {
          worktype s0 = src[i], s1 = src[i + 1];
          a0 = __op__(a0, s0);
          a1 = __op__(a1, s1);
          s0 = src[i + 2];
          s1 = src[i + 3];
          a2 = __op__(a2, s0);
          a3 = __op__(a3, s1);
        }

        dst[0] = (acctype)a0;
        dst[1] = (acctype)a1;
        dst[2] = (acctype)a2;
        dst[3] = (acctype)a3;
      }

    }
    break;

    default:
      ASSERT(0);
      break;
    }
  }
}

#undef __op__
#undef load_macro
