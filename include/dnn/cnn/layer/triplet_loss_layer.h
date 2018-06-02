#define CHECK_EQ(a, b)  ASSERT((a)==(b))
static void TripletLossLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* prev1 = IBLOB(1);
  struct Blob* prev2 = IBLOB(2);
  struct Blob* next = OBLOB(0);
  int i=0, j, count = Data_count(prev->size);
  Dtype margin = 1;//FARG(margin);
  
  /* bottom[3] is for sample's weight, decarded here */
  //CHECK_EQ(bottom[3]->size.c,1);
  //CHECK_EQ(bottom[3]->height(), 1);
  //CHECK_EQ(bottom[3]->width(), 1);
  
  float* diff_ap_ = reset(&next->bufs[i++], dataSize(prev->size.n, prev->size.c, 1, 1));
  float* diff_an_ = reset(&next->bufs[i++], dataSize(prev->size.n, prev->size.c, 1, 1));
  float* diff_pn_ = reset(&next->bufs[i++], dataSize(prev->size.n, prev->size.c, 1, 1));
  
  float* diff_sq_ap_ = reset(&next->bufs[i++], dataSize(prev->size.n, prev->size.c, 1, 1));
  float* diff_sq_an_ = reset(&next->bufs[i++], dataSize(prev->size.n, prev->size.c, 1, 1));
  float* dist_sq_ap_ = reset(&next->bufs[i++], dataSize(prev->size.n, 1, 1, 1));
  float* dist_sq_an_ = reset(&next->bufs[i++], dataSize(prev->size.n, 1, 1, 1));
  // vector of ones used to sum along channels
  float* summer_vec_ = reset(&next->bufs[i++], dataSize(prev->size.c, 1, 1, 1));
  float* dist_binary_ = reset(&next->bufs[i++], dataSize(prev->size.n, 1, 1, 1));
  int channels;
  Dtype loss;
  
  CHECK_EQ(prev->size.n, prev1->size.n);
  CHECK_EQ(prev1->size.n, prev2->size.n);
  CHECK_EQ(prev->size.c, prev1->size.c);
  CHECK_EQ(prev1->size.c, prev2->size.c);
  CHECK_EQ(prev->size.h, 1);
  CHECK_EQ(prev->size.w, 1);
  CHECK_EQ(prev1->size.h, 1);
  CHECK_EQ(prev1->size.w, 1);
  CHECK_EQ(prev2->size.h, 1);
  CHECK_EQ(prev2->size.w, 1);

  for (i = 0; i < prev->size.c; ++i) {
    summer_vec_[i] = (1);
  }
  for (i = 0; i < prev->size.n; ++i) {
    dist_binary_[i] = (1);
  }
  
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      
      int count = Data_count(prev->size);
      
      //const Dtype* sampleW = bottom[3]->cpu_data();
      const Dtype sampleW = (Dtype)(1);
      // a_i-p_i
      blas_ssub(count,prevData,nextData,diff_ap_);
      // a_i-n_i  
      blas_ssub(count,prevData,getData(prev2),diff_an_);
      // p_i-n_i
      blas_ssub(count,nextData,getData(prev2),diff_pn_);
      channels = prev->size.c;
      loss = (0.0);
      for (i = 0; i < prev->size.n; ++i) {
        Dtype mdist;
        dist_sq_ap_[i] = blas_sdot(channels, diff_ap_ + (i*channels), diff_ap_ + (i*channels));
        dist_sq_an_[i] = blas_sdot(channels, diff_an_ + (i*channels), diff_an_ + (i*channels));
        mdist = sampleW*MAX(margin + dist_sq_ap_[i] - dist_sq_an_[i], (Dtype)(0.0));
        loss += mdist;
        if (mdist < (Dtype)(1e-9)) {
          //dist_binary_[i] = (Dtype)(0);
          //prepare for backward pass
          blas_sset(channels, (Dtype)(0), diff_ap_ + (i*channels));
          blas_sset(channels, (Dtype)(0), diff_an_ + (i*channels));
          blas_sset(channels, (Dtype)(0), diff_pn_ + (i*channels));
        }
      }
      loss = loss / (float)(prev->size.n) / (Dtype)(2);
      nextData[0] = loss;
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      //Dtype margin = this->layer_param_.contrastive_loss_param().margin();
      //const Dtype* sampleW = bottom[3]->cpu_data();
      const Dtype sampleW = (1);
      
      for (i = 0; i < 3; ++i) {
        if (1) {
          const Dtype sign = (i < 2) ? -1 : 1;
          const Dtype alpha = sign * nextDiff[0] / (float)(IBLOB(i)->size.n);
          int num = IBLOB(i)->size.n;
          int channels = IBLOB(i)->size.c;
          for (j = 0; j < num; ++j) {
            Dtype* bout = setDiff(IBLOB(i));
            if (i == 0) {  // a
              //if(dist_binary_[j]>Dtype(0)){
              blas_saxpby(channels,alpha*sampleW,diff_pn_ + (j*channels),(Dtype)(0.0),bout + (j*channels));
              //}else{
              //  blas_sset(channels, (Dtype)(0), bout + (j*channels));
              //}
            }
            else if (i == 1) {  // p
              //if(dist_binary_[j]>Dtype(0)){
              blas_saxpby(channels,alpha*sampleW,diff_ap_ + (j*channels),(Dtype)(0.0),bout + (j*channels));
              //}else{
              //	  blas_sset(channels, (Dtype)(0), bout + (j*channels));
              //}
            }
            else if (i == 2) {  // n
              //if(dist_binary_[j]>Dtype(0)){
              blas_saxpby(channels,alpha*sampleW,diff_an_ + (j*channels),(Dtype)(0.0),bout + (j*channels));
              //}else{
              //   blas_sset(channels, (Dtype)(0), bout + (j*channels));
              //}
            }
          } // for num
        } //if propagate_down[i]
      } //for i
    }
    break;
  default:
    break;
  }
  return loss;
}

