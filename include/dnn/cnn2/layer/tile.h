

#ifndef LAYER_TILE_H
#define LAYER_TILE_H

#include "layer.h"

namespace ncnn
{

struct Tile : public Layer
{
public:
  Tile();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* bottom_blob, Blob* top_blob) const;

public:
  int dim;
  int tiles;
};

} // namespace ncnn

#endif // LAYER_TILE_H
