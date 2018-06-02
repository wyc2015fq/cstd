

#include "memorydata.h"

namespace ncnn
{

DEFINE_LAYER_CREATOR(MemoryData)

MemoryData::MemoryData()
{
  one_blob_only = false;
  support_inplace = false;
}

int MemoryData::load_param(const ParamDict& pd)
{
  w = pd.get(0, 0);
  h = pd.get(1, 0);
  c = pd.get(2, 0);

  return 0;
}

#if NCNN_STDIO
int MemoryData::load_model(FILE* binfp)
{
  int nread;

  if (c != 0) {
    data.create(w, h, c);
  }
  else if (h != 0) {
    data.create(w, h);
  }
  else if (w != 0) {
    data.create(w);
  }
  else { // 0 0 0
    data.create(1);
  }

  if (data.empty()) {
    return -100;
  }

  for (int p = 0; p < data.c; p++) {
    float* ptr = data.channel(p);
    nread = fread(ptr, data.w * data.h * sizeof(float), 1, binfp);

    if (nread != 1) {
      fprintf(stderr, "MemoryData read data failed %d\n", nread);
      return -1;
    }
  }

  return 0;
}
#endif // NCNN_STDIO

int MemoryData::load_model(const unsigned char*& mem)
{
  if (c != 0) {
    data.create(w, h, c);
  }
  else if (h != 0) {
    data.create(w, h);
  }
  else if (w != 0) {
    data.create(w);
  }
  else { // 0 0 0
    data.create(1);
  }

  if (data.empty()) {
    return -100;
  }

  for (int p = 0; p < data.c; p++) {
    float* ptr = data.channel(p);
    memcpy(ptr, mem, data.w * data.h * sizeof(float));
    mem += data.w * data.h * sizeof(float);
  }

  return 0;
}

int MemoryData::forward(const std::vector<img_t>& /*bottom_blobs*/, std::vector<img_t>& top_blobs) const
{
  img_t* top_blob = top_blobs[0];

  top_blob = data.clone();

  if (top_blob->empty()) {
    return -100;
  }

  return 0;
}

} // namespace ncnn
