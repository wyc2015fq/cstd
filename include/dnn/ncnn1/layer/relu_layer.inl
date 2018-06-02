
static int ReLU(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* bottom_data = LBLOB(0)->data;
  img_t* top_blob = net->blobs[layer->blobs[1]].data;
  int w = bottom_data->w;
  int h = bottom_data->h;
  int q, i, channels = pBox_channels(bottom_data);
  int size = w * h;
  float slope = FARG(slope);
  
  if (top_blob==bottom_data) {
    if (slope == 0.f) {
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        float* ptr = pBox_channel_p(top_blob, q);
        
        for (i = 0; i < size; i++) {
          if (ptr[i] < 0) {
            ptr[i] = 0;
          }
        }
      }
    }
    else {
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        float* ptr = pBox_channel_p(top_blob, q);
        
        for (i = 0; i < size; i++) {
          if (ptr[i] < 0) {
            ptr[i] *= slope;
          }
        }
      }
    }
  } else {
    pBox_create(top_blob, w, h, channels, 1);
    
    if (pBox_empty(top_blob)) {
      return -100;
    }
    
    if (slope == 0.f) {
      int cnt=0;
#pragma omp parallel for
      for (q = 0; q < channels; q++) {
        const float* ptr = pBox_channel_p(bottom_data, q);
        float* outptr = pBox_channel_p(top_blob, q);
        
        for (i = 0; i < size; i++) {
          if (ptr[i] < 0) {
            outptr[i] = 0;
          }
          else {
            ++cnt;
            outptr[i] = ptr[i];
          }
        }
      }
      printf("%d %d\n", channels*size, cnt);
    }
    else {
#pragma omp parallel for
      
      for (q = 0; q < channels; q++) {
        const float* ptr = pBox_channel_p(bottom_data, q);
        float* outptr = pBox_channel_p(top_blob, q);
        
        for (i = 0; i < size; i++) {
          if (ptr[i] < 0) {
            outptr[i] = ptr[i] * slope;
          }
          else {
            outptr[i] = ptr[i];
          }
        }
      }
    }
  }
  return 0;
}


static int ReLULayer(enum LayerOp op, const struct Layer* layer, struct Net* net) {
  Blob* bottom_blob = LBLOB(0);
  Blob* top_blob = LBLOB(1);
  Dtype negative_slope = FARG(slope);
  int i;
  switch (op) {
  case LO_Init:
    break;
  case LO_Forward:
    {
      pBox_ReshapeLike(top_blob->data, bottom_blob->data);
      const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      Dtype* top_data = pBox_datap(top_blob->data);
      const int count = pBox_count(bottom_blob->data);
      for (i = 0; i < count; ++i) {
        top_data[i] = MAX(bottom_data[i], (0)) + negative_slope * MIN(bottom_data[i], (0));
      }
    }
    break;
  case LO_Backward:
    {
      pBox_ReshapeLike(bottom_blob->diff, bottom_blob->data);
      const Dtype* bottom_data = pBox_datap(bottom_blob->data);
      const Dtype* top_diff = pBox_datap(top_blob->diff);
      Dtype* bottom_diff = pBox_datap(bottom_blob->diff);
      const int count = pBox_count(bottom_blob->data);
      for (i = 0; i < count; ++i) {
        bottom_diff[i] = top_diff[i] * ((bottom_data[i] > 0) + negative_slope * (bottom_data[i] <= 0));
      }
    }
    break;
  }
  return 0;
}
