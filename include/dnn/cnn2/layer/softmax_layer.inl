
static int SoftmaxLayer(enum LayerOp op, struct NetWork* net, struct Layer* layer) {
  Blob* bottom_blob = LBLOB(0);
  Blob* top_blob = LBLOB(1);
  int outer_num_ = bottom_blob->data->f;
  int count = Blob_count(bottom_blob->data);
  int dim = count/outer_num_;
  int channels = Blob_channels(bottom_blob->data);
  int inner_num_ = Blob_pixnum(bottom_blob->data);
  int i, j, k;
  Dtype* scale_data = NULL;
  Dtype* sum_multiplier_ = NULL;
  int axis = LARG(axis);

  if (1==channels) {
    channels *= bottom_blob->data->h;
    inner_num_ /= bottom_blob->data->h;
    if (1==channels) {
      channels *= bottom_blob->data->w;
      inner_num_ /= bottom_blob->data->w;
    }
  }
  MYREALLOC(scale_data, inner_num_);
  MYREALLOC(sum_multiplier_, channels);
  blas_set(channels, Dtype(1), sum_multiplier_);
  
  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      Blob_ReshapeLike(top_blob->data, bottom_blob->data);
      const Dtype* bottom_data = Blob_datap(bottom_blob->data);
      Dtype* top_data = Blob_datap(top_blob->data);
      blas_copy(count, bottom_data, top_data);
      // We need to subtract the max to avoid numerical issues, compute the exp,
      // and then normalize.
      for (i = 0; i < outer_num_; ++i) {
        // initialize scale_data to the first plane
        blas_copy(inner_num_, bottom_data + i * dim, scale_data);
        for (j = 1; j < channels; j++) {
          for (k = 0; k < inner_num_; k++) {
            scale_data[k] = MAX(scale_data[k], bottom_data[i * dim + j * inner_num_ + k]);
          }
        }
        // subtraction
        blas_gemm(CblasNoTrans, CblasNoTrans, channels, inner_num_,
          1, -1., sum_multiplier_, scale_data, 1., top_data);
        // exponentiation
        blas_exp(dim, top_data, top_data);
        // sum after exp
        blas_gemv(CblasTrans, channels, inner_num_, 1.,
          top_data, sum_multiplier_, 0., scale_data);
        // division
        for (j = 0; j < channels; j++) {
          blas_div(inner_num_, top_data, scale_data, top_data);
          top_data += inner_num_;
        }
      }
    }
    break;
  case LO_Backward:
    {
      Blob_ReshapeLike(bottom_blob->diff, bottom_blob->data);
      const Dtype* top_diff = Blob_datap(top_blob->diff);
      const Dtype* top_data = Blob_datap(top_blob->data);
      Dtype* bottom_diff = Blob_datap(bottom_blob->diff);
      int dim = count / outer_num_;
      blas_copy(count, top_diff, bottom_diff);
      for (i = 0; i < outer_num_; ++i) {
        // compute dot(top_diff, top_data) and subtract them from the bottom diff
        for (k = 0; k < inner_num_; ++k) {
          scale_data[k] = blas_strided_dot(channels, bottom_diff + i * dim + k, inner_num_, top_data + i * dim + k, inner_num_);
        }
        // subtraction
        blas_gemm(CblasNoTrans, CblasNoTrans, channels, inner_num_, 1,
          -1., sum_multiplier_, scale_data, 1., bottom_diff + i * dim);
      }
      // elementwise multiplication
      blas_mul(count, bottom_diff, top_data, bottom_diff);
    }
    break;
  }
  FREE(sum_multiplier_);
  FREE(scale_data);
  return 0;
}