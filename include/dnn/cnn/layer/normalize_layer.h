
#define NOT_IMPLEMENTED ASSERT(0 && "NOT_IMPLEMENTED")

static int normalize_forward() {
  return 0;
}

static Dtype NormalizeLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  enum Method normalize_type_ = M_L2;
  bool fix_gradient_ = false;
  bool bp_norm_ = false;
  Dtype loss = 0;
  int n, s, c;

#define sign(x) (Dtype(0) < (x)) - ((x) < (Dtype)(0))

  normalize_type_ = MARG(distance_type);
  //fix_gradient_ = this->layer_param_.normalize_param().fix_gradient();
  //bp_norm_ = this->layer_param_.normalize_param().bp_norm() && layer->o_size == 2;

#if 0
  top[0]->Reshape(prev->size.n, prev->size.c, prev->size.h, prev->size.w);
  squared_.Reshape(prev->size.n, prev->size.c, prev->size.h, prev->size.w);
  if (layer->o_size == 2) {
    top[1]->Reshape(prev->size.n, 1, prev->size.h, prev->size.w);
  }
  norm_.Reshape(prev->size.n, 1, prev->size.h, prev->size.w);
#endif

  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const Dtype* prevData = getData(prev);
      Dtype* nextData = setDataZero(next);
      Dtype* squareData = setBuff(next, 0);
      Dtype* normData = (layer->o_size == 2) ? setData(OBLOB(1)) : setBuff(next, 1);
      int num = prev->size.n;
      int channels = prev->size.c;
      int spatial_dim = prev->size.h * prev->size.w;
      if (normalize_type_ == M_L2) {
        blas_ssqr(num*channels*spatial_dim, prevData, squareData);
        for (n = 0; n < num; n++) {
          for (s = 0; s < spatial_dim; s++) {
            normData[n*spatial_dim + s] = (Dtype)(0);
            for (c = 0; c < channels; c++) {
              normData[n*spatial_dim + s] += squareData[(n * channels + c) * spatial_dim + s];
            }
            normData[n*spatial_dim + s] += 1e-6;
            normData[n*spatial_dim + s] = sqrt(normData[n*spatial_dim + s]);
            for (c = 0; c < channels; c++) {
              nextData[(n * channels + c) * spatial_dim + s] = prevData[(n * channels + c) * spatial_dim + s] / normData[n*spatial_dim + s];
            }
          }
        }
      }
      else if (normalize_type_ == M_L1) {
        blas_sabs(num*channels*spatial_dim, prevData, squareData);
        for (n = 0; n < num; n++) {
          for (s = 0; s < spatial_dim; s++) {
            normData[n*spatial_dim +s] = (Dtype)(0);
            for (c = 0; c < channels; c++) {
              normData[n*spatial_dim + s] += squareData[(n * channels + c) * spatial_dim + s];
            }
            normData[n*spatial_dim + s] += 1e-6;
            normData[n*spatial_dim + s] = normData[n*spatial_dim + s];
            for (c = 0; c < channels; c++) {
              nextData[(n * channels + c) * spatial_dim + s] = prevData[(n * channels + c) * spatial_dim + s] / normData[n*spatial_dim + s];
            }
          }
        }
      }
      else {
        NOT_IMPLEMENTED;
      }
    }
    
    break;
  case Op_Backward:
    {
      const Dtype* nextDiff = getDiff(next);
      const Dtype* nextData = getData(next);
      const Dtype* prevData = getData(prev);
      const Dtype* squareData = setBuff(next, 0);
      const Dtype* normData = (layer->o_size == 2) ? getData(OBLOB(1)) : getBuff(next, 1);
      Dtype* prevDiff = setDiffZero(prev);
      
      int num = prev->size.n;
      int channels = prev->size.c;
      int spatial_dim = prev->size.h * prev->size.w;
      if (1) {
        if (normalize_type_ == M_L2) {
          for (n = 0; n < num; ++n) {
            for (s = 0; s < spatial_dim; s++) {
              Dtype a = blas_sstrided_dot(channels, nextData + n*channels*spatial_dim + s, spatial_dim, nextDiff + n*channels*spatial_dim + s, spatial_dim);
              for (c = 0; c < channels; c++) {
                prevDiff[(n * channels + c) * spatial_dim + s] =
                  (nextDiff[(n * channels + c) * spatial_dim + s] - nextData[(n * channels + c) * spatial_dim + s] * a) / normData[n*spatial_dim + s];
              }
            }
          }
        }
        else if (normalize_type_ == M_L1) {
          for (n = 0; n < num; ++n) {
            for (s = 0; s < spatial_dim; s++) {
              Dtype a = blas_sstrided_dot(channels, nextData + n*channels*spatial_dim + s, spatial_dim, nextDiff + n*channels*spatial_dim + s, spatial_dim);
              for (c = 0; c < channels; c++) {
                prevDiff[(n * channels + c) * spatial_dim + s] =
                  (nextDiff[(n * channels + c) * spatial_dim + s] - sign(prevData[(n * channels + c) * spatial_dim + s]) * a) / normData[n*spatial_dim + s];
              }
            }
          }
        }
        else {
          NOT_IMPLEMENTED;
        }
      }
      
      if (bp_norm_) {
        const Dtype* normDiff = getDiff(next);
        if (normalize_type_ == M_L2) {
          for (n = 0; n < num; ++n) {
            for (s = 0; s < spatial_dim; s++) {
              for (c = 0; c < channels; c++) {
                prevDiff[(n * channels + c) * spatial_dim + s] += normDiff[n*spatial_dim + s] * nextData[(n * channels + c) * spatial_dim + s];
              }
            }
          }
        }
        else if (normalize_type_ == M_L1) {
          for (n = 0; n < num; ++n) {
            for (s = 0; s < spatial_dim; s++) {
              for (c = 0; c < channels; c++) {
                prevDiff[(n * channels + c) * spatial_dim + s] += normDiff[n*spatial_dim + s] * sign(prevData[(n * channels + c) * spatial_dim + s]);
              }
            }
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


