

struct ExpandDims : public Layer
{
public:
  ExpandDims();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob* prev, Blob* next) const;

public:
  int expand_w;
  int expand_h;
  int expand_c;
};


ExpandDims::ExpandDims()
{
  one_blob_only = true;
  support_inplace = false;
}

int ExpandDims::load_param(const ParamDict& pd)
{
  expand_w = pd.get(0, 0);
  expand_h = pd.get(1, 0);
  expand_c = pd.get(2, 0);

  return 0;
}

int ExpandDims::forward(const Blob* prev, Blob* next) const
{
  int w = prevSize.w;
  int h = prevSize.h;
  int dims = prev->dims;

  next = prev;

  if (dims == 1) {
    if (expand_w) {
      if (expand_h) {
        next = prev->reshape(1, 1, w);
      }
      else if (expand_c) {
        next = prev->reshape(1, w, 1);
      }
      else {
        next = prev->reshape(1, w);
      }
    }
    else if (expand_h) {
      if (expand_c) {
        next = prev->reshape(w, 1, 1);
      }
      else {
        next = prev->reshape(w, 1);
      }
    }
  }
  else if (dims == 2) {
    if (expand_w) {
      next = prev->reshape(1, w, h);
    }
    else if (expand_h) {
      next = prev->reshape(w, 1, h);
    }
    else if (expand_c) {
      next = prev->reshape(w, h, 1);
    }
  }

  if (next->empty()) {
    return -100;
  }

  return 0;
}

