

struct Interp : public Layer
{
public:
  Interp();
  virtual ~Interp();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const Blob& prev, Blob& next) const;

public:
  // param
  float width_scale;
  float height_scale;
  int output_width;
  int output_height;
  int resize_type;//1:near 2: bilinear
};

Interp::Interp()
{
  one_blob_only = true;
}

Interp::~Interp()
{
}

int Interp::load_param(const ParamDict& pd)
{
  resize_type = pd.get(0, 0);
  height_scale = pd.get(1, 1.f);
  width_scale = pd.get(2, 1.f);
  output_height = pd.get(3, 0);
  output_width = pd.get(4, 0);

  return 0;
}

int Interp::forward(const Blob& prev, Blob& next) const
{
  int h = prevSize.h;
  int w = prevSize.w;
  int c = prevSize.c;
  int oh = output_height;
  int ow = output_width;

  if (ow == 0 || ow == 0) {
    oh = h * height_scale;
    ow = w * width_scale;
  }

  if (oh == h && ow == w) {
    next = prev;
    return 0;
  }

  next->create(ow, oh, c);

  if (next->empty()) {
    return -100;
  }

  if (resize_type == 1) { //nearest
    #pragma omp parallel for
    for (q = 0; q < c; ++q) {
      const float* ptr = prevData + q * Data_2DSize(prevSize);
      float* output_ptr = nextData + q * Data_2DSize(nextSize);

      for (y = 0; y < oh; ++y) {
        const int in_y = MIN((int)(y / height_scale), (h - 1));

        for (x = 0; x < ow; ++x) {
          const int in_x = MIN((int)(x / width_scale), (w - 1));
          output_ptr[ow * y + x] = ptr[in_y * w + in_x];
        }
      }
    }

    return 0;

  }
  else if (resize_type == 2) { // bilinear
    resize_bilinear(prev, next, ow, oh);
    return 0;

  }
  else {
    fprintf(stderr, "unsupported resize type %d %d %d\n", resize_type, oh, ow);
    return -233;
  }
}


} // namespace ncnn
