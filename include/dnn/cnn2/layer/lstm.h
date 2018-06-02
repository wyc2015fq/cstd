

#ifndef LAYER_LSTM_H
#define LAYER_LSTM_H

#include "layer.h"

namespace ncnn
{

struct LSTM : public Layer
{
public:
  LSTM();
  virtual ~LSTM();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  // param
  int num_output;
  int weight_data_size;

  // model
  Blob weight_hc_data;
  Blob weight_xc_data;
  Blob bias_c_data;
};

} // namespace ncnn

#endif // LAYER_LSTM_H
