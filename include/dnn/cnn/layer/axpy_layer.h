
/**
 * @brief For reduce memory and time both on training and testing, we combine
 *        channel-wise scale operation and element-wise addition operation 
 *        into a single layer called "axpy".
 *       
 */

/**
 * @param Formulation:
 *            F = a * X + Y
 *	  Shape info:
 *            a:  N x C          --> bottom[0]      
 *            X:  N x C x H x W  --> bottom[1]       
 *            Y:  N x C x H x W  --> bottom[2]     
 *            F:  N x C x H x W  --> top[0]
 */
static Dtype AxpyLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* prev1 = IBLOB(1);
  struct Blob* prev2 = IBLOB(2);
  struct Blob* next = OBLOB(0);
  int n, c, spatial_dim, count = Data_count(prev->size);
  Dtype* spatial_sum_multiplier = NULL;
  DataSize prevSize = prev->size;
  DataSize prevSize1 = prev1->size;
  ASSERT(layer->i_size>=3);
  //ASSERT(prevSize.n == prevSize1.n);
  //CHECK_EQ(prevSize.c, prevSize1.c);
  //CHECK(bottom[1]->shape() == bottom[2]->shape());  
  spatial_dim = Data_2DSize(prevSize1);
  MYREALLOC(spatial_sum_multiplier, spatial_dim);
  blas_sset(spatial_dim, (1), spatial_sum_multiplier);
  
  switch (op) {
  case Op_Forward:
    next->size = prev1->size;
    {
      const Dtype* prevData = getData(prev);
      Dtype* nextData = setDataZero(next);
      int channel_dim = prevSize.c;
      const Dtype* scale_data = prevData;
      const Dtype* x_data = nextData;
      blas_scopy(count, prev2->data, nextData);
      for (n = 0; n < prevSize1.n; ++n) {
        for (c = 0; c < channel_dim; ++c) {
          int scale_offset = n * channel_dim + c;
          blas_saxpy(spatial_dim, scale_data[scale_offset], x_data + scale_offset * spatial_dim, 
            nextData + scale_offset * spatial_dim);  
        }
      }
    }
    break;
  case Op_Backward:
    {
      const Dtype* prevData = getData(prev);
      const Dtype* nextDiff = getDiff(next);
      const Dtype* nextData = getData(next);
      Dtype* prevDiff = setDiff(prev);
      const Dtype* x_data = nextData;
      Dtype* x_diff = setDiff(prev1); 
      Dtype* scale_diff = setDiff(prev);
      if (1) {
        blas_smul(count, nextDiff, x_data, x_diff);
        blas_sset(Data_count(prev->size), (Dtype)(0), scale_diff);
        blas_sgemv(CblasNoTrans, Data_count(prev->size), spatial_dim, (1),
          x_diff, spatial_sum_multiplier, (Dtype)(1), scale_diff); 
        //if (!propagate_down[1])
        {
          //blas_sset(Data_count(prev1->size), (Dtype)(0), x_diff);
        }
      }
      if (1) {
        int channel_dim = prev1->size.c;
        const Dtype* scale_data = prevData;
        for (n = 0; n < prevSize1.n; ++n) {
          for (c = 0; c < channel_dim; ++c) {
            int scale_offset = n * channel_dim + c;
            blas_sscale(spatial_dim, scale_data[scale_offset],  nextDiff + scale_offset * spatial_dim, x_diff + scale_offset * spatial_dim);  
          }
        }
      }
      if (1) {
        Dtype* prev2Diff = setDiffZero(prev2);
        blas_scopy(count, nextDiff, prev2Diff);
      }
    }
    break;
  default:
    break;
  }
  return 0;
}

