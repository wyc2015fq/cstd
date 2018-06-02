
static int PReLU(enum LayerOp op, struct Net* net, struct Layer* layer)
{
  const Blob* prevData = IBLOB(0)->data;
  Blob* next = net->blobs[layer->blobs[1]].data;
  Blob* bottom_top_blob = next;
  int w = prevData->w;
  int h = prevData->h;
  int channels = Blob_channels(prevData);
  int size = w * h;
  int num_slope = LARG(data_size);
  const float* slope_data_ptr = Blob_datap(LBLOB(2)->data);
  
  if (prevData==next) {
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      float* ptr = Blob_channel_p(bottom_top_blob, q);
      float slope = num_slope > 1 ? slope_data_ptr[q] : slope_data_ptr[0];
      
      for (i = 0; i < size; i++) {
        if (ptr[i] < 0) {
          ptr[i] *= slope;
        }
      }
    }
  } else {
    Blob_create(next, w, h, channels);
    
    if (imempty(next)) {
      return -100;
    }
    
    
#pragma omp parallel for
    for (q = 0; q < channels; q++) {
      const float* ptr = Blob_channel_p(prevData, q);
      float* outptr = nextData + q * Data_2DSize(nextSize);
      float slope = num_slope > 1 ? slope_data_ptr[q] : slope_data_ptr[0];
      
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
  return 0;
}
