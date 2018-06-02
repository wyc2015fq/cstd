

struct PriorBox : public Layer
{
public:
  PriorBox();

  virtual int load_param(const ParamDict& pd);

  virtual int forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const;

public:
  Blob min_sizes;
  Blob max_sizes;
  Blob aspect_ratios;
  float variances[4];
  int flip;
  int clip;
  int image_width;
  int image_height;
  float step_width;
  float step_height;
  float offset;
};

PriorBox::PriorBox()
{
  one_blob_only = false;
  support_inplace = false;
}

int PriorBox::load_param(const ParamDict& pd)
{
  min_sizes = pd.get(0, Blob());
  max_sizes = pd.get(1, Blob());
  aspect_ratios = pd.get(2, Blob());
  variances[0] = pd.get(3, 0.f);
  variances[1] = pd.get(4, 0.f);
  variances[2] = pd.get(5, 0.f);
  variances[3] = pd.get(6, 0.f);
  flip = pd.get(7, 1);
  clip = pd.get(8, 0);
  image_width = pd.get(9, 0);
  image_height = pd.get(10, 0);
  step_width = pd.get(11, -233.f);
  step_height = pd.get(12, -233.f);
  offset = pd.get(13, 0.f);

  return 0;
}

int PriorBox::forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const
{
  int w = bottom_blobs[0].w;
  int h = bottom_blobs[0].h;

  int image_w = image_width;
  int image_h = image_height;

  if (image_w == -233) {
    image_w = bottom_blobs[1].w;
  }

  if (image_h == -233) {
    image_h = bottom_blobs[1].h;
  }

  float step_w = step_width;
  float step_h = step_height;

  if (step_w == -233) {
    step_w = (float)image_w / w;
  }

  if (step_h == -233) {
    step_h = (float)image_h / h;
  }

  int num_min_size = min_sizes.w;
  int num_max_size = max_sizes.w;
  int num_aspect_ratio = aspect_ratios.w;

  int num_prior = num_min_size * num_aspect_ratio + num_min_size + num_max_size;

  if (flip) {
    num_prior += num_min_size * num_aspect_ratio;
  }

  Blob* next = top_blobs[0];
  next->create(4 * w * h * num_prior, 2);

  #pragma omp parallel for

  for (i = 0; i < h; i++) {
    float* box = nextData + i * w * num_prior * 4;

    float center_x = offset * step_w;
    float center_y = offset * step_h + i * step_h;

    for (j = 0; j < w; j++) {
      float box_w;
      float box_h;

      for (k = 0; k < num_min_size; k++) {
        float min_size = min_sizes.data[k];

        // min size box
        box_w = box_h = min_size;

        box[0] = (center_x - box_w * 0.5f) / image_w;
        box[1] = (center_y - box_h * 0.5f) / image_h;
        box[2] = (center_x + box_w * 0.5f) / image_w;
        box[3] = (center_y + box_h * 0.5f) / image_h;

        box += 4;

        if (num_max_size > 0) {
          float max_size = max_sizes.data[k];

          // max size box
          box_w = box_h = sqrt(min_size * max_size);

          box[0] = (center_x - box_w * 0.5f) / image_w;
          box[1] = (center_y - box_h * 0.5f) / image_h;
          box[2] = (center_x + box_w * 0.5f) / image_w;
          box[3] = (center_y + box_h * 0.5f) / image_h;

          box += 4;
        }

        // all aspect_ratios
        for (p = 0; p < num_aspect_ratio; p++) {
          float ar = aspect_ratios[p];

          box_w = min_size * sqrt(ar);
          box_h = min_size / sqrt(ar);

          box[0] = (center_x - box_w * 0.5f) / image_w;
          box[1] = (center_y - box_h * 0.5f) / image_h;
          box[2] = (center_x + box_w * 0.5f) / image_w;
          box[3] = (center_y + box_h * 0.5f) / image_h;

          box += 4;

          if (flip) {
            box[0] = (center_x - box_h * 0.5f) / image_h;
            box[1] = (center_y - box_w * 0.5f) / image_w;
            box[2] = (center_x + box_h * 0.5f) / image_h;
            box[3] = (center_y + box_w * 0.5f) / image_w;

            box += 4;
          }
        }
      }

      center_x += step_w;
    }

    center_y += step_h;
  }

  if (clip) {
    float* box = next;

    for (i = 0; i < next->w; i++) {
      box[i] = MIN(MAX(box[i], 0.f), 1.f);
    }
  }

  // set variance
  float* var = next->row(1);

  for (i = 0; i < next->w / 4; i++) {
    var[0] = variances[0];
    var[1] = variances[1];
    var[2] = variances[2];
    var[3] = variances[3];

    var += 4;
  }

  return 0;
}
