
enum EltwiseOp {
  EltwiseOp_PROD,
    EltwiseOp_SUM,
    EltwiseOp_MAX,
    EltwiseOp_SORT,
};

static Dtype EltwiseLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int i, j, idx, blob_idx, index, count = Data_count(prev->size);
  enum EltwiseOp eop;
  float* coeffs = NULL;
  int* max_idx = NULL;
  float* sort_temp = NULL;
  bool stable_prod_grad;
  //DataSize size = prev->size;
  ASSERT(EltwiseOp_SUM!=eop);

  switch (eop) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      int* mask = NULL;
      const Dtype* bottom_data_a = NULL;
      const Dtype* bottom_data_b = NULL;
      switch (eop) {
      case EltwiseOp_PROD:
        blas_smul(count, prevData, nextData, nextData);
        for (i = 2; i < layer->i_size; ++i) {
          blas_smul(count, nextData, IBLOB(i)->data, nextData);
        }
        break;
      case EltwiseOp_SUM:
        blas_sset(count, (Dtype)(0), nextData);
        // TODO(shelhamer) does BLAS optimize to sum for coeff = 1?
        for (i = 0; i < layer->i_size; ++i) {
          blas_saxpy(count, coeffs[i], IBLOB(i)->data, nextData);
        }
        break;
      case EltwiseOp_MAX:
        // Initialize
        max_idx = (int*)setBuff(next, 0);
        mask = max_idx;
        blas_iset(count, -1, mask);
        blas_sset(count, (Dtype)(-FLT_MAX), nextData);
        // bottom 0 & 1
        bottom_data_a = prevData;
        bottom_data_b = nextData;
        for (idx = 0; idx < count; ++idx) {
          if (bottom_data_a[idx] > bottom_data_b[idx]) {
            nextData[idx] = bottom_data_a[idx];  // maxval
            mask[idx] = 0;  // maxid
          } else {
            nextData[idx] = bottom_data_b[idx];  // maxval
            mask[idx] = 1;  // maxid
          }
        }
        // bottom 2++
        for (blob_idx = 2; blob_idx < layer->i_size; ++blob_idx) {
          bottom_data_b = IBLOB(blob_idx)->data;
          for (idx = 0; idx < count; ++idx) {
            if (bottom_data_b[idx] > nextData[idx]) {
              nextData[idx] = bottom_data_b[idx];  // maxval
              mask[idx] = blob_idx;  // maxid
            }
          }
        }
        break;
      case EltwiseOp_SORT:
        resetArr(&sort_temp, count);
        blas_sset(count, (Dtype)(1), nextData);
        for (i = 0; i < layer->i_size; ++i) {
          blas_scopy(count, IBLOB(i)->data, sort_temp);
          blas_sadd_scalar(count, (Dtype)(1), sort_temp);
          blas_smul(count, nextData, sort_temp, nextData);
        }
        break;
      default:
        printf("Unknown elementwise operation.\n");
        break;
      }
      FREE(sort_temp);
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      const Dtype* nextData = getData(next);
      float* prevDiff = setDiff(prev);
      const int* mask = NULL;
      for (i = 0; i < layer->i_size; ++i) {
        if (1) {
          const Dtype* prevData = getData(IBLOB(i));
          Dtype* prevDiff = setDiff(IBLOB(i));
          switch (eop) {
          case EltwiseOp_PROD:
            if (stable_prod_grad) {
              bool initialized = false;
              for (j = 0; j < layer->i_size; ++j) {
                if (i == j) { continue; }
                if (!initialized) {
                  blas_scopy(count, IBLOB(j)->data, prevDiff);
                  initialized = true;
                }
                else {
                  blas_smul(count, IBLOB(j)->data, prevDiff,
                    prevDiff);
                }
              }
            }
            else {
              blas_sdiv(count, nextData, prevData, prevDiff);
            }
            blas_smul(count, prevDiff, nextDiff, prevDiff);
            break;
            if (stable_prod_grad) {
              bool initialized = false;
              for (j = 0; j < layer->i_size; ++j) {
                if (i == j) { continue; }
                if (!initialized) {
                  blas_scopy(count, IBLOB(j)->data, prevDiff);
                  blas_sadd_scalar(count, (Dtype)(1), prevDiff);
                  initialized = true;
                } else {
                  blas_scopy(count, IBLOB(j)->data, sort_temp);
                  blas_sadd_scalar(count, (Dtype)(1), sort_temp);
                  blas_smul(count, sort_temp, prevDiff, prevDiff);
                }
              }
            } else {
              blas_scopy(count, prevData, sort_temp);
              blas_sadd_scalar(count, (Dtype)(1), sort_temp);
              blas_sdiv(count, nextData, sort_temp, prevDiff);
            }
            blas_smul(count, prevDiff, nextDiff, prevDiff);
            break;
          case EltwiseOp_SUM:
            if (coeffs[i] == (Dtype)(1)) {
              blas_scopy(count, nextDiff, prevDiff);
            } else {
              blas_sscale(count, coeffs[i], nextDiff, prevDiff);
            }
            break;
          case EltwiseOp_MAX:
            max_idx = (int*)getBuff(next, 0);
            mask = max_idx;
            for (index = 0; index < count; ++index) {
              Dtype gradient = 0;
              if (mask[index] == i) {
                gradient += nextDiff[index];
              }
              prevDiff[index] = gradient;
            }
            break;
          case EltwiseOp_SORT:
            if (stable_prod_grad) {
              bool initialized = false;
              for (j = 0; j < layer->i_size; ++j) {
                if (i == j) { continue; }
                if (!initialized) {
                  blas_scopy(count, IBLOB(j)->data, prevDiff);
                  blas_sadd_scalar(count, (Dtype)(1), prevDiff);
                  initialized = true;
                }
                else {
                  blas_scopy(count, IBLOB(j)->data, sort_temp);
                  blas_sadd_scalar(count, (Dtype)(1), sort_temp);
                  blas_smul(count, sort_temp, prevDiff,
                    prevDiff);
                }
              }
            }
            else {
              blas_scopy(count, prevData, sort_temp);
              blas_sadd_scalar(count, (Dtype)(1), sort_temp);
              blas_sdiv(count, nextData, sort_temp, prevDiff);
            }
            blas_smul(count, prevDiff, nextDiff, prevDiff);
            break;
          default:
            printf("Unknown elementwise operation.\n");
          }
        }
      }
    }
    break;
  default:
    break;
  }
  return 0;
}
