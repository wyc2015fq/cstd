

#include "prelu.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(PReLU)

PReLU::PReLU()
{
  one_blob_only = true;
  support_inplace = true;
}

int PReLU::load_param(const ParamDict& pd)
{
  num_slope = pd.get(0, 0);

  return 0;
}

#if NCNN_STDIO
int PReLU::load_model(FILE* binfp)
{
  int nread;

  slope_data.create(num_slope);

  if (slope_data.empty()) {
    return -100;
  }

  nread = fread(slope_data, num_slope * sizeof(float), 1, binfp);

  if (nread != 1) {
    fprintf(stderr, "PReLU read slope_data failed %d\n", nread);
    return -1;
  }

  return 0;
}
#endif // NCNN_STDIO

int PReLU::load_model(const unsigned char*& mem)
{
  slope_data = img_t(num_slope, (float*)mem);
  mem += num_slope * sizeof(float);

  return 0;
}

int PReLU::forward(const img_t* bottom_blob, img_t* top_blob) const
{
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);
  int size = w * h;

  top_blob->create(w, h, channels);

  if (top_blob->empty()) {
    return -100;
  }

  const float* slope_data_ptr = slope_data;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);
    float slope = num_slope > 1 ? slope_data_ptr[q] : slope_data_ptr[0];

    for (int i = 0; i < size; i++) {
      if (ptr[i] < 0) {
        outptr[i] = ptr[i] * slope;
      }
      else {
        outptr[i] = ptr[i];
      }
    }
  }

  return 0;
}

int PReLU::forward_inplace(img_t* bottom_top_blob) const
{
  int w = bottom_top_blob->w;
  int h = bottom_top_blob->h;
  int channels = pBox_channels(bottom_top_blob);
  int size = w * h;

  const float* slope_data_ptr = slope_data;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    float* ptr = pBox_channel_p(bottom_top_blob, q);
    float slope = num_slope > 1 ? slope_data_ptr[q] : slope_data_ptr[0];

    for (int i = 0; i < size; i++) {
      if (ptr[i] < 0) {
        ptr[i] *= slope;
      }
    }
  }

  return 0;
}

} // namespace ncnn
