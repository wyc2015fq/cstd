

#ifndef LAYER_EMBED_H
#define LAYER_EMBED_H

#include "layer.h"

namespace ncnn
{

struct Embed : public Layer
{
public:
  Embed();
  virtual ~Embed();

  virtual int load_param(const ParamDict& pd);

#if NCNN_STDIO
  virtual int load_model(FILE* binfp);
#endif // NCNN_STDIO
  virtual int load_model(const unsigned char*& mem);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

public:
  // param
  int num_output;
  int input_dim;
  int bias_term;

  int weight_data_size;

  // model
  Blob weight_data;
  Blob bias_data;
};

} // namespace ncnn

#endif // LAYER_EMBED_H
