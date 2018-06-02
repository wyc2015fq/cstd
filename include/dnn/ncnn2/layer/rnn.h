

#ifndef LAYER_RNN_H
#define LAYER_RNN_H

#include "layer.h"

namespace ncnn
{

struct RNN : public Layer
{
public:
  RNN();
  virtual ~RNN();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const std::vector<img_t>& bottom_blobs, std::vector<img_t>& top_blobs) const;

public:
  // param
  int num_output;
  int weight_data_size;

  // model
  img_t weight_hh_data;
  img_t weight_xh_data;
  img_t weight_ho_data;
  img_t bias_h_data;
  img_t bias_o_data;
};

} // namespace ncnn

#endif // LAYER_RNN_H
