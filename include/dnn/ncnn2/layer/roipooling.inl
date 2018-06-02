

#include "roipooling.h"
#include <math.h>
#include <algorithm>

namespace ncnn
{

DEFINE_LAYER_CREATOR(ROIPooling)

ROIPooling::ROIPooling()
{
}

int ROIPooling::load_param(const ParamDict& pd)
{
  pooled_width = pd.get(0, 0);
  pooled_height = pd.get(1, 0);
  spatial_scale = pd.get(2, 1.f);

  return 0;
}

int ROIPooling::forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const
{
  const img_t* bottom_blob = bottom_blobs[0];
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = pBox_channels(bottom_blob);

  const img_t* roi_blob = bottom_blobs[1];

  img_t* top_blob = top_blobs[0];
  top_blob->create(pooled_width, pooled_height, channels);

  if (top_blob->empty()) {
    return -100;
  }

  // For each ROI R = [x y w h]: max pool over R
  const float* roi_ptr = roi_blob;

  int roi_x1 = round(roi_ptr[0] * spatial_scale);
  int roi_y1 = round(roi_ptr[1] * spatial_scale);
  int roi_x2 = round(roi_ptr[2] * spatial_scale);
  int roi_y2 = round(roi_ptr[3] * spatial_scale);

  int roi_w = MAX(roi_x2 - roi_x1 + 1, 1);
  int roi_h = MAX(roi_y2 - roi_y1 + 1, 1);

  float bin_size_w = (float)roi_w / (float)pooled_width;
  float bin_size_h = (float)roi_h / (float)pooled_height;

  #pragma omp parallel for

  for (int q = 0; q < channels; q++) {
    const float* ptr = pBox_channel_p(bottom_blob, q);
    float* outptr = pBox_channel_p(top_blob, q);

    for (int ph = 0; ph < pooled_height; ph++) {
      for (int pw = 0; pw < pooled_width; pw++) {
        // Compute pooling region for this output unit:
        //  start (included) = floor(ph * roi_height / pooled_height)
        //  end (excluded) = ceil((ph + 1) * roi_height / pooled_height)
        int hstart = roi_y1 + floor((float)(ph) * bin_size_h);
        int wstart = roi_x1 + floor((float)(pw) * bin_size_w);
        int hend = roi_y1 + ceil((float)(ph + 1) * bin_size_h);
        int wend = roi_x1 + ceil((float)(pw + 1) * bin_size_w);

        hstart = MIN(MAX(hstart, 0), h);
        wstart = MIN(MAX(wstart, 0), w);
        hend = MIN(MAX(hend, 0), h);
        wend = MIN(MAX(wend, 0), w);

        bool is_empty = (hend <= hstart) || (wend <= wstart);

        float max = is_empty ? 0.f : ptr[hstart * w + wstart];

        for (int y = hstart; y < hend; y++) {
          for (int x = wstart; x < wend; x++) {
            int index = y * w + x;
            max = MAX(max, ptr[index]);
          }
        }

        outptr[pw] = max;
      }

      outptr += pooled_width;
    }
  }

  return 0;
}

} // namespace ncnn
