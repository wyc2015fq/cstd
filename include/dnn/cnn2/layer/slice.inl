

#include "slice.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(Slice)

Slice::Slice()
{
}

int Slice::load_param(const ParamDict& pd)
{
  slices = pd.get(0, Blob());

  return 0;
}

int Slice::forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const
{
  const Blob* bottom_blob = bottom_blobs[0];
  int w = bottom_blob->w;
  int h = bottom_blob->h;
  int channels = Blob_channels(bottom_blob);

  int q = 0;
  const int* slices_ptr = (const int*)slices.data;

  for (size_t i = 0; i < top_blobs.size(); i++) {
    int slice = slices_ptr[i];

    if (slice == -233) {
      slice = (channels - q) / (top_blobs.size() - i);
    }

    Blob* top_blob = top_blobs[i];
    top_blob->create(w, h, slice);

    if (top_blob->empty()) {
      return -100;
    }

    int size = bottom_blob->cstep * slice;

    const float* ptr = Blob_channel_p(bottom_blob, q);
    float* outptr = top_blob->data;

    for (int j = 0; j < size; j++) {
      outptr[j] = ptr[j];
    }

    q += slice;
  }

  return 0;
}

} // namespace ncnn
