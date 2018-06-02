
static int AbsVal_load_param(Layer* layer, const char* param) {
  return 0;
}

static int AbsVal(enum LayerOp op, const struct Layer* layer, struct Net* net)
{
  const img_t* bottom_data = LBLOB(0)->data;
  img_t* top_blob = net->blobs[layer->blobs[1]].data;
  int w = bottom_data->w;
  int h = bottom_data->h;
  int q, i, channels = pBox_channels(bottom_data);
  int size = w * h;
  
  pBoxsetsize(top_blob, h, w, channels, 1);
  
  if (bottom_data==top_blob) {
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      float* ptr = pBox_channel_p(top_blob, q);
      
      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          ptr[i] = -ptr[i];
        }
      }
    }
  } else {
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = pBox_channel_p(bottom_data, q);
      float* outptr = pBox_channel_p(top_blob, q);
      
      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          outptr[i] = -ptr[i];
        }
        else {
          outptr[i] = ptr[i];
        }
      }
    }
  }
  
  return 0;
}

