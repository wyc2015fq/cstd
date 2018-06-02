
#include <vector>
#include "common.h"

static void ResizeImage(const ImageData* src, ImageData* dest) {
  int32_t src_width = src->width;
  int32_t src_height = src->height;
  int32_t dest_width = dest->width;
  int32_t dest_height = dest->height;
  if (src_width == dest_width && src_height == dest_height) {
    memcpy(dest->data, src->data, src_width * src_height * sizeof(uint8_t));
    return;
  }

  double lf_x_scl = static_cast<double>(src_width) / dest_width;
  double lf_y_Scl = static_cast<double>(src_height) / dest_height;
  const uint8_t* src_data = src->data;
  uint8_t* dest_data = dest->data;
  int x, y;

  tic;
  {
#ifdef USE_OPENMP
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
#pragma omp for nowait
#endif
    for (y = 0; y < dest_height; y++) {
      for (x = 0; x < dest_width; x++) {
        double lf_x_s = lf_x_scl * x;
        double lf_y_s = lf_y_Scl * y;

        int32_t n_x_s = static_cast<int>(lf_x_s);
        n_x_s = (n_x_s <= (src_width - 2) ? n_x_s : (src_width - 2));
        int32_t n_y_s = static_cast<int>(lf_y_s);
        n_y_s = (n_y_s <= (src_height - 2) ? n_y_s : (src_height - 2));

        double lf_weight_x = lf_x_s - n_x_s;
        double lf_weight_y = lf_y_s - n_y_s;

        double dest_val = (1 - lf_weight_y) * ((1 - lf_weight_x) *
          src_data[n_y_s * src_width + n_x_s] +
          lf_weight_x * src_data[n_y_s * src_width + n_x_s + 1]) +
          lf_weight_y * ((1 - lf_weight_x) * src_data[(n_y_s + 1) * src_width + n_x_s] +
          lf_weight_x * src_data[(n_y_s + 1) * src_width + n_x_s + 1]);

        dest_data[y * dest_width + x] = static_cast<uint8_t>(dest_val);
      }
    }
  }
  toc;
}

class ImagePyramid {
 public:
  ImagePyramid()
      : max_scale_(1.0f), min_scale_(1.0f),
        scale_factor_(1.0f), scale_step_(0.8f),
        width1x_(0), height1x_(0),
        width_scaled_(0), height_scaled_(0),
        buf_img_width_(2), buf_img_height_(2),
        buf_scaled_width_(2), buf_scaled_height_(2) {
    buf_img_ = new uint8_t[buf_img_width_ * buf_img_height_];
    buf_img_scaled_ = new uint8_t[buf_scaled_width_ * buf_scaled_height_];
  }

  ~ImagePyramid() {
    delete[] buf_img_;
    buf_img_ = NULL;

    buf_img_width_ = 0;
    buf_img_height_ = 0;

    delete[] buf_img_scaled_;
    buf_img_scaled_ = NULL;

    buf_scaled_width_ = 0;
    buf_scaled_height_ = 0;

    img_scaled_.data = NULL;
    img_scaled_.width = 0;
    img_scaled_.height = 0;
  }

  inline void SetScaleStep(float step) {
    if (step > 0.0f && step <= 1.0f)
      scale_step_ = step;
  }

  inline void SetMinScale(float min_scale) {
    min_scale_ = min_scale;
  }

  inline void SetMaxScale(float max_scale) {
    max_scale_ = max_scale;
    scale_factor_ = max_scale;
    UpdateBufScaled();
  }

  void SetImage1x(const uint8_t* img_data, int32_t width, int32_t height);

  inline float min_scale() const { return min_scale_; }
  inline float max_scale() const { return max_scale_; }

  inline ImageData image1x() {
    return ImageData(buf_img_, width1x_, height1x_, 1);
  }

  const ImageData* GetNextScaleImage(float* scale_factor = NULL);

 public:
  void UpdateBufScaled();

  float max_scale_;
  float min_scale_;

  float scale_factor_;
  float scale_step_;

  int32_t width1x_;
  int32_t height1x_;

  int32_t width_scaled_;
  int32_t height_scaled_;

  uint8_t* buf_img_;
  int32_t buf_img_width_;
  int32_t buf_img_height_;

  uint8_t* buf_img_scaled_;
  int32_t buf_scaled_width_;
  int32_t buf_scaled_height_;

  ImageData img_scaled_;
};


const ImageData* ImagePyramid::GetNextScaleImage(float* scale_factor) {
  if (scale_factor_ >= min_scale_) {
    if (scale_factor != NULL)
      *scale_factor = scale_factor_;

    width_scaled_ = static_cast<int32_t>(width1x_ * scale_factor_);
    height_scaled_ = static_cast<int32_t>(height1x_ * scale_factor_);

    ImageData src_img(buf_img_, width1x_, height1x_);
    ImageData dest_img(buf_img_scaled_, width_scaled_, height_scaled_);
    ResizeImage(&src_img, &dest_img);
    scale_factor_ *= scale_step_;

    img_scaled_.data = buf_img_scaled_;
    img_scaled_.width = width_scaled_;
    img_scaled_.height = height_scaled_;
    return &img_scaled_;
  } else {
    return NULL;
  }
}

void ImagePyramid::SetImage1x(const uint8_t* img_data, int32_t width,
    int32_t height) {
  if (width > buf_img_width_ || height > buf_img_height_) {
    delete[] buf_img_;

    buf_img_width_ = width;
    buf_img_height_ = height;
    buf_img_ = new uint8_t[width * height];
  }

  width1x_ = width;
  height1x_ = height;
  memcpy(buf_img_, img_data, width * height * sizeof(uint8_t));
  scale_factor_ = max_scale_;
  UpdateBufScaled();
}

void ImagePyramid::UpdateBufScaled() {
  if (width1x_ == 0 || height1x_ == 0)
    return;

  int32_t max_width = static_cast<int32_t>(width1x_ * max_scale_ + 0.5);
  int32_t max_height = static_cast<int32_t>(height1x_ * max_scale_ + 0.5);

  if (max_width > buf_scaled_width_ || max_height > buf_scaled_height_) {
    delete[] buf_img_scaled_;

    buf_scaled_width_ = max_width;
    buf_scaled_height_ = max_height;
    buf_img_scaled_ = new uint8_t[max_width * max_height];

    img_scaled_.data = NULL;
    img_scaled_.width = 0;
    img_scaled_.height = 0;
  }
}

struct MLPLayer {
 public:
  void Compute(const float* input, float* output);

  inline int32_t GetInputDim() const { return input_dim_; }
  inline int32_t GetOutputDim() const { return output_dim_; }

  inline void SetSize(int32_t inputDim, int32_t outputDim) {
    if (inputDim <= 0 || outputDim <= 0) {
      return;  // @todo handle the errors!!!
    }
    input_dim_ = inputDim;
    output_dim_ = outputDim;
    MYREALLOC(weights_, inputDim * outputDim);
    MYREALLOC(bias_, outputDim);
  }

  inline void SetWeights(const float* weights, int32_t len) {
    if (weights == NULL || len != input_dim_ * output_dim_) {
      return;  // @todo handle the errors!!!
    }
    std::copy(weights, weights + input_dim_ * output_dim_, weights_);
  }

  inline void SetBias(const float* bias, int32_t len) {
    if (bias == NULL || len != output_dim_) {
      return;  // @todo handle the errors!!!
    }
    std::copy(bias, bias + output_dim_, bias_);
  }

 public:
  inline float Sigmoid(float x) {
    return 1.0f / (1.0f + exp(x));
  }

  inline float ReLU(float x) {
    return (x > 0.0f ? x : 0.0f);
  }

 public:
  int32_t act_func_type_;
  int32_t input_dim_;
  int32_t output_dim_;
  float* weights_;
  float* bias_;
};

void MLPLayer_free(MLPLayer* s) {
  FREE(s->weights_);
  FREE(s->bias_);
}

void MLPLayer_init(MLPLayer* s, int32_t act_func_type = 1) {
  s->input_dim_=(0), s->output_dim_=(0), s->act_func_type_=(act_func_type);
  s->weights_ = NULL;
  s->bias_ = NULL;
}

void MLPLayer::Compute(const float* input, float* output) {
#ifdef USE_OPENMP
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
#pragma omp for nowait
#endif
  for (int32_t i = 0; i < output_dim_; i++) {
    output[i] = VectorInnerProduct(input,
      weights_ + i * input_dim_, input_dim_) + bias_[i];
    output[i] = (act_func_type_ == 1 ? ReLU(output[i]) : Sigmoid(-output[i]));
  }
  
}


class MLP {
 public:
  MLP() {
    layers_size = 0;
    layers_ = NULL;
  }
  ~MLP() {
    int i;
    for (i=0; i<layers_size; ++i) {
      MLPLayer_free(layers_+i);
    }
    FREE(layers_);
  }

  void Compute(const float* input, float* output);

  inline int32_t GetInputDim() const {
    return layers_[0].GetInputDim();
  }

  inline int32_t GetOutputDim() const {
    ASSERT(layers_ && layers_size>1);
    return layers_[layers_size-1].GetOutputDim();
  }

  inline int32_t GetLayerNum() const {
    return static_cast<int32_t>(layers_size);
  }

  void AddLayer(int32_t inputDim, int32_t outputDim, const float* weights,
      const float* bias, bool is_output = false);

 public:
   int layers_size;
   MLPLayer* layers_;
};


void MLP::Compute(const float* input, float* output) {
  float* layer_buf_[2] = {0};
  MYREALLOC(layer_buf_[0], layers_[0].GetOutputDim());
  layers_[0].Compute(input, layer_buf_[0]);

  size_t i; /**< layer index */
  for (i = 1; i < layers_size - 1; i++) {
    MYREALLOC(layer_buf_[i % 2], layers_[i].GetOutputDim());
    layers_[i].Compute(layer_buf_[(i + 1) % 2], layer_buf_[i % 2]);
  }
  layers_[layers_size-1].Compute(layer_buf_[(i + 1) % 2], output);
  FREE(layer_buf_[0]);
  FREE(layer_buf_[1]);
}

void MLP::AddLayer(int32_t inputDim, int32_t outputDim, const float* weights,
    const float* bias, bool is_output) {
  if (layers_size > 0 && inputDim != layers_[layers_size-1].GetOutputDim())
    return;  // @todo handle the errors!!!

  layers_size++;
  MYREALLOC(layers_, layers_size);
  MLPLayer* layer = layers_ + layers_size - 1;
  MLPLayer_init(layer, is_output ? 0 : 1);
  layer->SetSize(inputDim, outputDim);
  layer->SetWeights(weights, inputDim * outputDim);
  layer->SetBias(bias, outputDim);
}

bool CompareBBox(const FaceInfo & a, const FaceInfo & b) {
  return a.score > b.score;
}
int NonMaximumSuppression(int bboxes_size, FaceInfo* bboxes, FaceInfo* bboxes_nms, float iou_thresh = 0.8f) {
  int bboxes_nms_size = 0;
  std::sort(bboxes, bboxes+bboxes_size, CompareBBox);

  int32_t select_idx = 0;
  int32_t num_bbox = static_cast<int32_t>(bboxes_size);
  char* mask_merged = NULL;
  bool all_merged = false;
  MYREALLOC(mask_merged, num_bbox);
  MEMSET(mask_merged, 0, num_bbox);

  while (!all_merged) {
    while (select_idx < num_bbox && mask_merged[select_idx] == 1)
      select_idx++;
    if (select_idx == num_bbox) {
      all_merged = true;
      continue;
    }

    bboxes_nms[bboxes_nms_size++] = ((bboxes)[select_idx]);
    mask_merged[select_idx] = 1;

    CRect select_bbox = (bboxes)[select_idx].bbox;
    float area1 = static_cast<float>(select_bbox.width * select_bbox.height);
    float x1 = static_cast<float>(select_bbox.x);
    float y1 = static_cast<float>(select_bbox.y);
    float x2 = static_cast<float>(select_bbox.x + select_bbox.width - 1);
    float y2 = static_cast<float>(select_bbox.y + select_bbox.height - 1);

    select_idx++;
    for (int32_t i = select_idx; i < num_bbox; i++) {
      if (mask_merged[i] == 1)
        continue;

      CRect & bbox_i = (bboxes)[i].bbox;
      float x = MAX(x1, static_cast<float>(bbox_i.x));
      float y = MAX(y1, static_cast<float>(bbox_i.y));
      float w = MIN(x2, static_cast<float>(bbox_i.x + bbox_i.width - 1)) - x + 1;
      float h = MIN(y2, static_cast<float>(bbox_i.y + bbox_i.height - 1)) - y + 1;
      if (w <= 0 || h <= 0)
        continue;

      float area2 = static_cast<float>(bbox_i.width * bbox_i.height);
      float area_intersect = w * h;
      float area_union = area1 + area2 - area_intersect;
      if (static_cast<float>(area_intersect) / area_union > iou_thresh) {
        mask_merged[i] = 1;
        bboxes_nms[bboxes_nms_size-1].score += (bboxes)[i].score;
      }
    }
  }
  FREE(mask_merged);
  return bboxes_nms_size;
}

void NonMaximumSuppression(std::vector<FaceInfo>* bboxes, std::vector<FaceInfo>* bboxes_nms, float iou_thresh = 0.8f) {
  bboxes_nms->clear();
  int n = NonMaximumSuppression(bboxes->size(), &(*bboxes)[0], &(*bboxes)[0], iou_thresh);
  bboxes_nms->resize(n);
  for (int i=0; i<n; ++i) {
    (*bboxes_nms)[i] = (*bboxes)[i];
  }
  return ;
  std::sort(bboxes->begin(), bboxes->end(), CompareBBox);

  int32_t select_idx = 0;
  int32_t num_bbox = static_cast<int32_t>(bboxes->size());
  std::vector<int32_t> mask_merged(num_bbox, 0);
  bool all_merged = false;

  while (!all_merged) {
    while (select_idx < num_bbox && mask_merged[select_idx] == 1)
      select_idx++;
    if (select_idx == num_bbox) {
      all_merged = true;
      continue;
    }

    bboxes_nms->push_back((*bboxes)[select_idx]);
    mask_merged[select_idx] = 1;

    CRect select_bbox = (*bboxes)[select_idx].bbox;
    float area1 = static_cast<float>(select_bbox.width * select_bbox.height);
    float x1 = static_cast<float>(select_bbox.x);
    float y1 = static_cast<float>(select_bbox.y);
    float x2 = static_cast<float>(select_bbox.x + select_bbox.width - 1);
    float y2 = static_cast<float>(select_bbox.y + select_bbox.height - 1);

    select_idx++;
    for (int32_t i = select_idx; i < num_bbox; i++) {
      if (mask_merged[i] == 1)
        continue;

      CRect & bbox_i = (*bboxes)[i].bbox;
      float x = MAX(x1, static_cast<float>(bbox_i.x));
      float y = MAX(y1, static_cast<float>(bbox_i.y));
      float w = MIN(x2, static_cast<float>(bbox_i.x + bbox_i.width - 1)) - x + 1;
      float h = MIN(y2, static_cast<float>(bbox_i.y + bbox_i.height - 1)) - y + 1;
      if (w <= 0 || h <= 0)
        continue;

      float area2 = static_cast<float>(bbox_i.width * bbox_i.height);
      float area_intersect = w * h;
      float area_union = area1 + area2 - area_intersect;
      if (static_cast<float>(area_intersect) / area_union > iou_thresh) {
        mask_merged[i] = 1;
        bboxes_nms->back().score += (*bboxes)[i].score;
      }
    }
  }
}

class FeatureMap {
 public:
  FeatureMap()
      : width_(0), height_(0) {
    roi_.x = 0;
    roi_.y = 0;
    roi_.width = 0;
    roi_.height = 0;
  }

  virtual ~FeatureMap() {}

  virtual void Compute(const uint8_t* input, int32_t width, int32_t height) = 0;

  inline virtual void SetROI(const CRect & roi) {
    roi_ = roi;
  }

 protected:
  int32_t width_;
  int32_t height_;

  CRect roi_;
};


enum ClassifierType {
    LAB_Boosted_Classifier,
    SURF_MLP,
    ClassifierCount
};

class Classifier {
 public:
  Classifier() {}
  virtual ~Classifier() {}

  virtual void SetFeatureMap(FeatureMap* feat_map) = 0;
  virtual bool Classify(float* score = NULL, float* outputs = NULL) = 0;
  
  virtual ClassifierType type() = 0;

  DISABLE_COPY_AND_ASSIGN(Classifier);
};

/** @struct LABFeature
 *  @brief Locally Assembled Binary (LAB) feature.
 *
 *  It is parameterized by the coordinates of top left corner.
 */
typedef struct LABFeature {
  int32_t x;
  int32_t y;
} LABFeature;

class LABFeatureMap : public FeatureMap {
 public:
  LABFeatureMap() : rect_width_(3), rect_height_(3), num_rect_(3) {}
  virtual ~LABFeatureMap() {}

  virtual void Compute(const uint8_t* input, int32_t width, int32_t height);

  inline uint8_t GetFeatureVal(int32_t offset_x, int32_t offset_y) const {
    return feat_map_[(roi_.y + offset_y) * width_ + roi_.x + offset_x];
  }

  float GetStdDev() const;

 public:
  void Reshape(int32_t width, int32_t height);
  void ComputeIntegralImages(const uint8_t* input);
  void ComputeRectSum();
  void ComputeFeatureMap();

  template<typename Int32Type>
  inline void Integral(Int32Type* data) {
    const Int32Type* src = data;
    Int32Type* dest = data;
    const Int32Type* dest_above = dest;

    *dest = *(src++);
    for (int32_t c = 1; c < width_; c++, src++, dest++)
      *(dest + 1) = (*dest) + (*src);
    dest++;
    for (int32_t r = 1; r < height_; r++) {
      for (int32_t c = 0, s = 0; c < width_; c++, src++, dest++, dest_above++) {
        s += (*src);
        *dest = *dest_above + s;
      }
    }
  }

  const int32_t rect_width_;
  const int32_t rect_height_;
  const int32_t num_rect_;

  std::vector<uint8_t> feat_map_;
  std::vector<int32_t> rect_sum_;
  std::vector<int32_t> int_img_;
  std::vector<uint32_t> square_int_img_;
};



/**
 * @class LABBaseClassifier
 * @brief Base classifier using LAB feature.
 */
class LABBaseClassifier {
 public:
  LABBaseClassifier()
    : num_bin_(255), thresh_(0.0f) {
    weights_.resize(num_bin_ + 1);
  }

  ~LABBaseClassifier() {}

  void SetWeights(const float* weights, int32_t num_bin);

  inline void SetThreshold(float thresh) { thresh_ = thresh; }

  inline int32_t num_bin() const { return num_bin_; }
  inline float weights(int32_t val) const { return weights_[val]; }
  inline float threshold() const { return thresh_; }

 public:
  int32_t num_bin_;

  std::vector<float> weights_;
  float thresh_;
};

/**
 * @class LABBoostedClassifier
 * @Brief A strong classifier constructed from base classifiers using LAB features.
 */
  static const int32_t kFeatGroupSize = 10;
  const float kStdDevThresh = 10.0f;

class LABBoostedClassifier : public Classifier {
 public:
  LABBoostedClassifier() : use_std_dev_(true) {}
  virtual ~LABBoostedClassifier() {}

  virtual bool Classify(float* score = NULL, float* outputs = NULL);

  inline virtual ClassifierType type() {
    return LAB_Boosted_Classifier;
  }

  void AddFeature(int32_t x, int32_t y);
  void AddBaseClassifier(const float* weights, int32_t num_bin, float thresh);

  inline virtual void SetFeatureMap(FeatureMap* featMap) {
    feat_map_ = (LABFeatureMap*)(featMap);
  }

  inline void SetUseStdDev(bool useStdDev) { use_std_dev_ = useStdDev; }

 public:

  std::vector<LABFeature> feat_;
  std::vector<LABBaseClassifier* > base_classifiers_;
  LABFeatureMap* feat_map_;
  bool use_std_dev_;
};

bool LABBoostModelReader_Read(std::istream* input,
    Classifier* model) {
  int32_t num_bin_;
  int32_t num_base_classifer_;
  bool is_read;
  LABBoostedClassifier* lab_boosted_classifier =
    (LABBoostedClassifier*)(model);

  input->read(reinterpret_cast<char*>(&num_base_classifer_), sizeof(int32_t));
  input->read(reinterpret_cast<char*>(&num_bin_), sizeof(int32_t));

  is_read = (!input->fail()) && num_base_classifer_ > 0 && num_bin_ > 0;

  if (is_read) {
    //ReadFeatureParam(input, lab_boosted_classifier, num_base_classifer_);
    
    int32_t x;
    int32_t y;
    for (int32_t i = 0; i < num_base_classifer_; i++) {
      input->read(reinterpret_cast<char*>(&x), sizeof(int32_t));
      input->read(reinterpret_cast<char*>(&y), sizeof(int32_t));
      lab_boosted_classifier->AddFeature(x, y);
    }

    is_read = !input->fail();
    if (is_read) {
      std::vector<float> thresh;
      thresh.resize(num_base_classifer_);
      input->read(reinterpret_cast<char*>(&thresh[0]),
        sizeof(float)* num_base_classifer_);
      
      int32_t weight_len = sizeof(float)* (num_bin_ + 1);
      std::vector<float> weights;
      weights.resize(num_bin_ + 1);
      for (int32_t i = 0; i < num_base_classifer_; i++) {
        input->read(reinterpret_cast<char*>(&weights[0]), weight_len);
        lab_boosted_classifier->AddBaseClassifier(&weights[0], num_bin_, thresh[i]);
      }
      
      is_read = !input->fail();
    }
  }
  return is_read;
}

void LABBaseClassifier::SetWeights(const float* weights, int32_t num_bin) {
  weights_.resize(num_bin + 1);
  num_bin_ = num_bin;
  std::copy(weights, weights + num_bin_ + 1, weights_.begin());
}

bool LABBoostedClassifier::Classify(float* score, float* outputs) {
  bool isPos = true;
  float s = 0.0f;

  for (size_t i = 0; isPos && i < base_classifiers_.size();) {
    for (int32_t j = 0; j < kFeatGroupSize; j++, i++) {
      uint8_t featVal = feat_map_->GetFeatureVal(feat_[i].x, feat_[i].y);
      s += base_classifiers_[i]->weights(featVal);
    }
    if (s < base_classifiers_[i - 1]->threshold())
      isPos = false;
  }
  isPos = isPos && ((!use_std_dev_) || feat_map_->GetStdDev() > kStdDevThresh);

  if (score != NULL)
    *score = s;
  if (outputs != NULL)
    *outputs = s;

  return isPos;
}

void LABBoostedClassifier::AddFeature(int32_t x, int32_t y) {
  LABFeature feat;
  feat.x = x;
  feat.y = y;
  feat_.push_back(feat);
}

void LABBoostedClassifier::AddBaseClassifier(const float* weights,
    int32_t num_bin, float thresh) {
  LABBaseClassifier* classifier = (new LABBaseClassifier());
  classifier->SetWeights(weights, num_bin);
  classifier->SetThreshold(thresh);
  base_classifiers_.push_back(classifier);
}



void LABFeatureMap::Compute(const uint8_t* input, int32_t width,
    int32_t height) {
  if (input == NULL || width <= 0 || height <= 0) {
    return;  // @todo handle the errors!!!
  }

  Reshape(width, height);
  ComputeIntegralImages(input);
  ComputeRectSum();
  ComputeFeatureMap();
}

float LABFeatureMap::GetStdDev() const {
  double mean;
  double m2;
  double area = roi_.width * roi_.height;

  int32_t top_left;
  int32_t top_right;
  int32_t bottom_left;
  int32_t bottom_right;

  if (roi_.x != 0) {
    if (roi_.y != 0) {
      top_left = (roi_.y - 1) * width_ + roi_.x - 1;
      top_right = top_left + roi_.width;
      bottom_left = top_left + roi_.height * width_;
      bottom_right = bottom_left + roi_.width;

      mean = (int_img_[bottom_right] - int_img_[bottom_left] +
        int_img_[top_left] - int_img_[top_right]) / area;
      m2 = (square_int_img_[bottom_right] - square_int_img_[bottom_left] +
        square_int_img_[top_left] - square_int_img_[top_right]) / area;
    } else {
      bottom_left = (roi_.height - 1) * width_ + roi_.x - 1;
      bottom_right = bottom_left + roi_.width;

      mean = (int_img_[bottom_right] - int_img_[bottom_left]) / area;
      m2 = (square_int_img_[bottom_right] - square_int_img_[bottom_left]) / area;
    }
  } else {
    if (roi_.y != 0) {
      top_right = (roi_.y - 1) * width_ + roi_.width - 1;
      bottom_right = top_right + roi_.height * width_;

      mean = (int_img_[bottom_right] - int_img_[top_right]) / area;
      m2 = (square_int_img_[bottom_right] - square_int_img_[top_right]) / area;
    } else {
      bottom_right = (roi_.height - 1) * width_ + roi_.width - 1;
      mean = int_img_[bottom_right] / area;
      m2 = square_int_img_[bottom_right] / area;
    }
  }

  return static_cast<float>(sqrt(m2 - mean * mean));
}

void LABFeatureMap::Reshape(int32_t width, int32_t height) {
  width_ = width;
  height_ = height;

  int32_t len = width_ * height_;
  feat_map_.resize(len);
  rect_sum_.resize(len);
  int_img_.resize(len);
  square_int_img_.resize(len);
}

void LABFeatureMap::ComputeIntegralImages(const uint8_t* input) {
  int32_t len = width_ * height_;

  UInt8ToInt32(input, &int_img_[0], len);
  Square(&int_img_[0], &square_int_img_[0], len);
  Integral(&int_img_[0]);
  Integral(&square_int_img_[0]);
}

void LABFeatureMap::ComputeRectSum() {
  int32_t width = width_ - rect_width_;
  int32_t height = height_ - rect_height_;
  const int32_t* int_img = &int_img_[0];
  int32_t* rect_sum = &rect_sum_[0];

  *rect_sum = *(int_img + (rect_height_ - 1) * width_ + rect_width_ - 1);
  VectorSub(int_img + (rect_height_ - 1) * width_ +
    rect_width_, int_img + (rect_height_ - 1) * width_, rect_sum + 1, width);

  {
#ifdef USE_OPENMP
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
#pragma omp for nowait
#endif
    for (int32_t i = 1; i <= height; i++) {
      const int32_t* top_left = int_img + (i - 1) * width_;
      const int32_t* top_right = top_left + rect_width_ - 1;
      const int32_t* bottom_left = top_left + rect_height_ * width_;
      const int32_t* bottom_right = bottom_left + rect_width_ - 1;
      int32_t* dest = rect_sum + i * width_;

      *(dest++) = (*bottom_right) - (*top_right);
      VectorSub(bottom_right + 1, top_right + 1, dest, width);
      VectorSub(dest, bottom_left, dest, width);
      VectorAdd(dest, top_left, dest, width);
    }
  }
}

void LABFeatureMap::ComputeFeatureMap() {
  int32_t width = width_ - rect_width_ * num_rect_;
  int32_t height = height_ - rect_height_ * num_rect_;
  int32_t offset = width_ * rect_height_;
  uint8_t* feat_map = &feat_map_[0];
  int32_t* rect_sum = &rect_sum_[0];

  {
#ifdef USE_OPENMP
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
#pragma omp for nowait
#endif
    for (int32_t r = 0; r <= height; r++) {
      for (int32_t c = 0; c <= width; c++) {
        uint8_t* dest = feat_map + r * width_ + c;
        *dest = 0;

        int32_t white_rect_sum = rect_sum[(r + rect_height_) * width_ + c + rect_width_];
        int32_t black_rect_idx = r * width_ + c;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x80 : 0x0);
        black_rect_idx += rect_width_;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x40 : 0x0);
        black_rect_idx += rect_width_;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x20 : 0x0);
        black_rect_idx += offset;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x08 : 0x0);
        black_rect_idx += offset;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x01 : 0x0);
        black_rect_idx -= rect_width_;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x02 : 0x0);
        black_rect_idx -= rect_width_;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x04 : 0x0);
        black_rect_idx -= offset;
        *dest |= (white_rect_sum >= rect_sum[black_rect_idx] ? 0x10 : 0x0);
      }
    }
  }
}

typedef struct SURFFeature {
  CRect patch;
  int32_t num_cell_per_row;
  int32_t num_cell_per_col;
} SURFFeature;

class SURFFeaturePool {
 public:
  SURFFeaturePool()
      : sample_width_(40), sample_height_(40),
        patch_move_step_x_(16), patch_move_step_y_(16), patch_size_inc_step_(1),
        patch_min_width_(16), patch_min_height_(16) {}

  ~SURFFeaturePool() {}

  void Create();
  void AddPatchFormat(int32_t width, int32_t height, int32_t num_cell_per_row,
      int32_t num_cell_per_col);

  inline bool empty() const { return pool_.empty(); }
  inline size_t size() const { return pool_.size(); }

  inline std::vector<SURFFeature>::const_iterator begin() const {
    return pool_.begin();
  }

  inline std::vector<SURFFeature>::const_iterator end() const {
    return pool_.end();
  }

  inline const SURFFeature & operator[](size_t idx) const {
    return pool_[idx];
  }

 public:
  void AddAllFeaturesToPool(int32_t width, int32_t height,
      int32_t num_cell_per_row, int32_t num_cell_per_col);

  typedef struct SURFPatchFormat {
    /**< aspect ratio, s.t. GCD(width, height) = 1 */
    int32_t width;
    int32_t height;

    /**< cell partition */
    int32_t num_cell_per_row;
    int32_t num_cell_per_col;
  } SURFPatchFormat;

  int32_t sample_width_;
  int32_t sample_height_;
  int32_t patch_move_step_x_;
  int32_t patch_move_step_y_;
  int32_t patch_size_inc_step_; /**< incremental step of patch width and */
                                /**< height when build feature pool      */
  int32_t patch_min_width_;
  int32_t patch_min_height_;

  std::vector<SURFFeature> pool_;
  std::vector<SURFPatchFormat> format_;
};

  static const int32_t kNumIntChannel = 8;

class SURFFeatureMap : public FeatureMap {
 public:
  SURFFeatureMap() : buf_valid_reset_(false) { InitFeaturePool(); }
  virtual ~SURFFeatureMap() {
    FREE(grad_x_);
    FREE(grad_y_);
    FREE(int_img_);
    FREE(img_buf_);
  }

  virtual void Compute(const uint8_t* input, int32_t width, int32_t height);

  inline virtual void SetROI(const CRect & roi) {
    roi_ = roi;
    if (buf_valid_reset_) {
      memset(&buf_valid_[0], 0, buf_valid_.size() * sizeof(int32_t));
      buf_valid_reset_ = false;
    }
  }

  inline int32_t GetFeatureVectorDim(int32_t feat_id) const {
    return (feat_pool_[feat_id].num_cell_per_col *
      feat_pool_[feat_id].num_cell_per_row * kNumIntChannel);
  }

  void GetFeatureVector(int32_t featID, float* featVec);

 public:
  void InitFeaturePool();
  void Reshape(int32_t width, int32_t height);

  void ComputeGradientImages(const uint8_t* input);
  void ComputeGradX(const int32_t* input);
  void ComputeGradY(const int32_t* input);
  void ComputeIntegralImages();
  void Integral();
  void MaskIntegralChannel();

  inline void FillIntegralChannel(const int32_t* src, int32_t ch) {
    int32_t* dest = int_img_ + ch;
    int32_t len = width_ * height_;
    for (int32_t i = 0; i < len; i++) {
      *dest = *src;
      *(dest + 2) = *src;
      dest += kNumIntChannel;
      src++;
    }
  }

  void ComputeFeatureVector(const SURFFeature & feat, int32_t* feat_vec);
  void NormalizeFeatureVectorL2(const int32_t* feat_vec, float* feat_vec_normed,
    int32_t len) const;

  /**
   * Number of channels should be divisible by 4.
   */
  void VectorCumAdd(int32_t* x, int32_t len, int32_t num_channel);

  bool buf_valid_reset_;

  int32_t* grad_x_;
  int32_t* grad_y_;
  int32_t* int_img_;
  int32_t* img_buf_;
  std::vector<std::vector<int32_t> > feat_vec_buf_;
  std::vector<std::vector<float> > feat_vec_normed_buf_;
  std::vector<int32_t> buf_valid_;

  SURFFeaturePool feat_pool_;
};

class SURFMLP : public Classifier {
 public:
  SURFMLP() : Classifier(), model_(new MLP()) {}
  virtual ~SURFMLP() {}

  virtual bool Classify(float* score = NULL, float* outputs = NULL);

  inline virtual void SetFeatureMap(FeatureMap* feat_map) {
    feat_map_ = (SURFFeatureMap*)(feat_map);
  }

  inline virtual ClassifierType type() {
    return SURF_MLP;
  }

  void AddFeatureByID(int32_t feat_id);
  void AddLayer(int32_t input_dim, int32_t output_dim, const float* weights,
    const float* bias, bool is_output = false);

  inline void SetThreshold(float thresh) { thresh_ = thresh; }

 public:
  std::vector<int32_t> feat_id_;
  std::vector<float> input_buf_;
  std::vector<float> output_buf_;

  MLP* model_;
  float thresh_;
  SURFFeatureMap* feat_map_;
};


void SURFFeaturePool::Create() {
  if (sample_height_ - patch_min_height_ <= sample_width_ - patch_min_width_) {
    for (size_t i = 0; i < format_.size(); i++) {
      const SURFPatchFormat & format = format_[i];
      for (int32_t h = patch_min_height_; h <= sample_height_;
          h += patch_size_inc_step_) {
        if (h % format.num_cell_per_col != 0 || h % format.height != 0)
          continue;
        int32_t w = h / format.height * format.width;
        if (w % format.num_cell_per_row != 0 || w < patch_min_width_ ||
            w > sample_width_)
          continue;
        AddAllFeaturesToPool(w, h, format.num_cell_per_row,
          format.num_cell_per_col);
      }
    }
  } else {
    for (size_t i = 0; i < format_.size(); i++) {
      const SURFPatchFormat & format = format_[i];
      for (int32_t w = patch_min_width_; w <= patch_min_width_;
          w += patch_size_inc_step_) {
        if (w % format.num_cell_per_row != 0 || w % format.width != 0)
          continue;
        int32_t h = w / format.width * format.height;
        if (h % format.num_cell_per_col != 0 || h < patch_min_height_ ||
            h > sample_height_)
          continue;
        AddAllFeaturesToPool(w, h, format.num_cell_per_row,
          format.num_cell_per_col);
      }
    }
  }
}

void SURFFeaturePool::AddPatchFormat(int32_t width, int32_t height,
    int32_t num_cell_per_row, int32_t num_cell_per_col) {
  for (size_t i = 0; i < format_.size(); i++) {
    const SURFPatchFormat & format = format_[i];
    if (format.height == height &&
      format.width == width &&
      format.num_cell_per_row == num_cell_per_row &&
      format.num_cell_per_col == num_cell_per_col)
      return;
  }

  SURFPatchFormat new_format;
  new_format.height = height;
  new_format.width = width;
  new_format.num_cell_per_row = num_cell_per_row;
  new_format.num_cell_per_col = num_cell_per_col;
  format_.push_back(new_format);
}

void SURFFeaturePool::AddAllFeaturesToPool(int32_t width, int32_t height,
    int32_t num_cell_per_row, int32_t num_cell_per_col) {
  SURFFeature feat;
  feat.patch.width = width;
  feat.patch.height = height;
  feat.num_cell_per_row = num_cell_per_row;
  feat.num_cell_per_col = num_cell_per_col;

  for (int32_t y = 0; y <= sample_height_ - height; y += patch_move_step_y_) {
    feat.patch.y = y;
    for (int32_t x = 0; x <= sample_width_ - width; x += patch_move_step_x_) {
      feat.patch.x = x;
      pool_.push_back(feat);
    }
  }
}

void SURFFeatureMap::Compute(const uint8_t* input, int32_t width,
    int32_t height) {
  if (input == NULL || width <= 0 || height <= 0) {
    return;  // @todo handle the error!
  }
  Reshape(width, height);
  ComputeGradientImages(input);
  tic;
  ComputeIntegralImages();
  toc;
}

void SURFFeatureMap::GetFeatureVector(int32_t feat_id, float* feat_vec) {
  if (buf_valid_[feat_id] == 0) {
    ComputeFeatureVector(feat_pool_[feat_id], &(feat_vec_buf_[feat_id][0]));
    NormalizeFeatureVectorL2(&(feat_vec_buf_[feat_id][0]),
      &(feat_vec_normed_buf_[feat_id][0]),
      static_cast<int32_t>(feat_vec_normed_buf_[feat_id].size()));
    buf_valid_[feat_id] = 1;
    buf_valid_reset_ = true;
  }

  memcpy(feat_vec, &(feat_vec_normed_buf_[feat_id][0]),
    feat_vec_normed_buf_[feat_id].size() * sizeof(float));
}

void SURFFeatureMap::InitFeaturePool() {
  grad_x_ = NULL;
  grad_y_ = NULL;
  int_img_ = NULL;
  img_buf_ = NULL;
  feat_pool_.AddPatchFormat(1, 1, 2, 2);
  feat_pool_.AddPatchFormat(1, 2, 2, 2);
  feat_pool_.AddPatchFormat(2, 1, 2, 2);
  feat_pool_.AddPatchFormat(2, 3, 2, 2);
  feat_pool_.AddPatchFormat(3, 2, 2, 2);
  feat_pool_.Create();

  int32_t feat_pool_size = static_cast<int32_t>(feat_pool_.size());
  feat_vec_buf_.resize(feat_pool_size);
  feat_vec_normed_buf_.resize(feat_pool_size);
  for (size_t i = 0; i < feat_pool_size; i++) {
    int32_t dim = GetFeatureVectorDim(static_cast<int32_t>(i));
    feat_vec_buf_[i].resize(dim);
    feat_vec_normed_buf_[i].resize(dim);
  }
  buf_valid_.resize(feat_pool_size, 0);
}

void SURFFeatureMap::Reshape(int32_t width, int32_t height) {
  width_ = width;
  height_ = height;

  int32_t len = width_ * height_;
  MYREALLOC(grad_x_, len);
  MYREALLOC(grad_y_, len);
  MYREALLOC(int_img_, len * kNumIntChannel);
  MYREALLOC(img_buf_, len);
}

void SURFFeatureMap::ComputeGradientImages(const uint8_t* input) {
  int32_t len = width_ * height_;
  UInt8ToInt32(input, img_buf_, len);
  ComputeGradX(img_buf_);
  ComputeGradY(img_buf_);
}

void SURFFeatureMap::ComputeGradX(const int32_t* input) {
  int32_t* dx = grad_x_;
  int32_t len = width_ - 2;

  {
#ifdef USE_OPENMP
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
#pragma omp for nowait
#endif
    for (int32_t r = 0; r < height_; r++) {
      const int32_t* src = input + r * width_;
      int32_t* dest = dx + r * width_;
      *dest = ((*(src + 1)) - (*src)) << 1;
      VectorSub(src + 2, src, dest + 1, len);
      dest += (width_ - 1);
      src += (width_ - 1);
      *dest = ((*src) - (*(src - 1))) << 1;
    }
  }
}

void SURFFeatureMap::ComputeGradY(const int32_t* input) {
  int32_t* dy = grad_y_;
  int32_t len = width_;
  VectorSub(input + width_, input, dy, len);
  VectorAdd(dy, dy, dy, len);

  {
#ifdef USE_OPENMP
#pragma omp parallel num_threads(SEETA_NUM_THREADS)
#pragma omp for nowait
#endif
    for (int32_t r = 1; r < height_ - 1; r++) {
      const int32_t* src = input + (r - 1) * width_;
      int32_t* dest = dy + r * width_;
      VectorSub(src + (width_ << 1), src, dest, len);
    }
  }
  int32_t offset = (height_ - 1) * width_;
  dy += offset;
  VectorSub(input + offset, input + offset - width_,
    dy, len);
  VectorAdd(dy, dy, dy, len);
}

void SURFFeatureMap::ComputeIntegralImages() {
  FillIntegralChannel(grad_x_, 0);
  FillIntegralChannel(grad_y_, 4);

  int32_t len = width_ * height_;
  VectorAbs(grad_x_, img_buf_, len);
  FillIntegralChannel(img_buf_, 1);
  VectorAbs(grad_y_, img_buf_, len);
  FillIntegralChannel(img_buf_, 5);
  MaskIntegralChannel();
  Integral();
}

void SURFFeatureMap::MaskIntegralChannel() {
  const int32_t* grad_x = grad_x_;
  const int32_t* grad_y = grad_y_;
  int32_t len = width_ * height_;
#ifdef USE_SSE
  __m128i dx;
  __m128i dy;
  __m128i dx_mask;
  __m128i dy_mask;
  __m128i zero = _mm_set1_epi32(0);
  __m128i xor_bits = _mm_set_epi32(0x0, 0x0, 0xffffffff, 0xffffffff);
  __m128i data;
  __m128i result;
  __m128i* src = reinterpret_cast<__m128i*>(int_img_.data());

  for (int32_t i = 0; i < len; i++) {
    dx = _mm_set1_epi32(*(grad_x++));
    dy = _mm_set1_epi32(*(grad_y++));
    dx_mask = _mm_xor_si128(_mm_cmplt_epi32(dx, zero), xor_bits);
    dy_mask = _mm_xor_si128(_mm_cmplt_epi32(dy, zero), xor_bits);

    data = _mm_loadu_si128(src);
    result = _mm_and_si128(data, dy_mask);
    _mm_storeu_si128(src++, result);
    data = _mm_loadu_si128(src);
    result = _mm_and_si128(data, dx_mask);
    _mm_storeu_si128(src++, result);
  }
#else
  int32_t dx, dy, dx_mask, dy_mask, cmp;
  int32_t xor_bits[] = {-1, -1, 0, 0};

  int32_t* src = int_img_;
  int i, j;
  for (i = 0; i < len; i++) {
      dy = *(grad_y++);
      dx = *(grad_x++);
      
      cmp = dy < 0 ? 0xffffffff : 0x0;
      for (j = 0; j < 4; j++) {
          // cmp xor xor_bits
          dy_mask = cmp ^ xor_bits[j];
          *(src) = (*src) & dy_mask;
          src++;
      }
      
      cmp = dx < 0 ? 0xffffffff : 0x0;
      for (j = 0; j < 4; j++) {
          // cmp xor xor_bits
          dx_mask = cmp ^ xor_bits[j];
          *(src) = (*src) & dx_mask;
          src++;
      }
  }
#endif
}

void SURFFeatureMap::Integral() {
  int32_t* data = int_img_;
  int32_t len = kNumIntChannel * width_;
  int r;
  // Cummulative sum by row
  for (r = 0; r < height_ - 1; r++) {
    int32_t* row1 = data + r * len;
    int32_t* row2 = row1 + len;
    VectorAdd(row1, row2, row2, len);
  }
  // Cummulative sum by column
  for (r = 0; r < height_; r++)
    VectorCumAdd(data + r * len, len, kNumIntChannel);
}

void SURFFeatureMap::VectorCumAdd(int32_t* x, int32_t len,
    int32_t num_channel) {
#ifdef USE_SSE
  __m128i x1;
  __m128i y1;
  __m128i z1;
  __m128i* x2 = reinterpret_cast<__m128i*>(x);
  __m128i* y2 = reinterpret_cast<__m128i*>(x + num_channel);
  __m128i* z2 = y2;

  len = len / num_channel - 1;
  for (int32_t i = 0; i < len; i++) {
    // first 4 channels
    x1 = _mm_loadu_si128(x2++);
    y1 = _mm_loadu_si128(y2++);
    z1 = _mm_add_epi32(x1, y1);
    _mm_storeu_si128(z2, z1);
    z2 = y2;

    // second 4 channels
    x1 = _mm_loadu_si128(x2++);
    y1 = _mm_loadu_si128(y2++);
    z1 = _mm_add_epi32(x1, y1);
    _mm_storeu_si128(z2, z1);
    z2 = y2;
  }
#else
  int32_t cols = len / num_channel - 1;
  for (int32_t i = 0; i < cols; i++) {
    int32_t* col1 = x + i * num_channel;
    int32_t* col2 = col1 + num_channel;
    VectorAdd(col1, col2, col2, num_channel);
  }
#endif
}

void SURFFeatureMap::ComputeFeatureVector(const SURFFeature & feat,
    int32_t* feat_vec) {
  int32_t init_cell_x = roi_.x + feat.patch.x;
  int32_t init_cell_y = roi_.y + feat.patch.y;
  int32_t cell_width = feat.patch.width / feat.num_cell_per_row * kNumIntChannel;
  int32_t cell_height = feat.patch.height / feat.num_cell_per_col;
  int32_t row_width = width_ * kNumIntChannel;
  const int32_t* cell_top_left[kNumIntChannel];
  const int32_t* cell_top_right[kNumIntChannel];
  const int32_t* cell_bottom_left[kNumIntChannel];
  const int32_t* cell_bottom_right[kNumIntChannel];
  int* feat_val = feat_vec;
  const int32_t* int_img = int_img_;
  int32_t offset = 0;
  int i, j, k;

  if (init_cell_y != 0) {
    if (init_cell_x != 0) {
      const int32_t* tmp_cell_top_right[kNumIntChannel];

      // cell #1
      offset = row_width * (init_cell_y - 1) +
        (init_cell_x - 1) * kNumIntChannel;
      for (i = 0; i < kNumIntChannel; i++) {
        cell_top_left[i] = int_img + (offset++);
        cell_top_right[i] = cell_top_left[i] + cell_width;
        cell_bottom_left[i] = cell_top_left[i] + row_width * cell_height;
        cell_bottom_right[i] = cell_bottom_left[i] + cell_width;
        *(feat_val++) = *(cell_bottom_right[i]) + *(cell_top_left[i]) -
                        *(cell_top_right[i]) - *(cell_bottom_left[i]);
        tmp_cell_top_right[i] = cell_bottom_right[i];
      }

      // cells in 1st row
      for (i = 1; i < feat.num_cell_per_row; i++) {
        for (j = 0; j < kNumIntChannel; j++) {
          cell_top_left[j] = cell_top_right[j];
          cell_top_right[j] += cell_width;
          cell_bottom_left[j] = cell_bottom_right[j];
          cell_bottom_right[j] += cell_width;
          *(feat_val++) = *(cell_bottom_right[j]) + *(cell_top_left[j]) -
                          *(cell_top_right[j]) - *(cell_bottom_left[j]);
        }
      }

      for (i = 0; i < kNumIntChannel; i++)
        cell_top_right[i] = tmp_cell_top_right[i];
    } else {
      const int32_t* tmp_cell_top_right[kNumIntChannel];

      // cell #1
      offset = row_width * (init_cell_y - 1) + cell_width - kNumIntChannel;
      for (i = 0; i < kNumIntChannel; i++) {
        cell_top_right[i] = int_img + (offset++);
        cell_bottom_right[i] = cell_top_right[i] + row_width * cell_height;
        tmp_cell_top_right[i] = cell_bottom_right[i];
        *(feat_val++) = *(cell_bottom_right[i]) - *(cell_top_right[i]);
      }

      // cells in 1st row
      for (i = 1; i < feat.num_cell_per_row; i++) {
        for (j = 0; j < kNumIntChannel; j++) {
          cell_top_left[j] = cell_top_right[j];
          cell_top_right[j] += cell_width;
          cell_bottom_left[j] = cell_bottom_right[j];
          cell_bottom_right[j] += cell_width;
          *(feat_val++) = *(cell_bottom_right[j]) + *(cell_top_left[j]) -
                          *(cell_top_right[j]) - *(cell_bottom_left[j]);
        }
      }

      for (i = 0; i < kNumIntChannel; i++)
        cell_top_right[i] = tmp_cell_top_right[i];
    }
  } else {
    if (init_cell_x != 0) {
      // cell #1
      offset = row_width * (cell_height - 1) +
        (init_cell_x - 1) * kNumIntChannel;
      for (i = 0; i < kNumIntChannel; i++) {
        cell_bottom_left[i] = int_img + (offset++);
        cell_bottom_right[i] = cell_bottom_left[i] + cell_width;
        *(feat_val++) = *(cell_bottom_right[i]) - *(cell_bottom_left[i]);
        cell_top_right[i] = cell_bottom_right[i];
      }

      // cells in 1st row
      for (i = 1; i < feat.num_cell_per_row; i++) {
        for (j = 0; j < kNumIntChannel; j++) {
          cell_bottom_left[j] = cell_bottom_right[j];
          cell_bottom_right[j] += cell_width;
          *(feat_val++) = *(cell_bottom_right[j]) - *(cell_bottom_left[j]);
        }
      }
    } else {
      // cell #1
      offset = row_width * (cell_height - 1) + cell_width - kNumIntChannel;
      for (i = 0; i < kNumIntChannel; i++) {
        cell_bottom_right[i] = int_img + (offset++);
        *(feat_val++) = *(cell_bottom_right[i]);
        cell_top_right[i] = cell_bottom_right[i];
      }

      // cells in 1st row
      for (i = 1; i < feat.num_cell_per_row; i++) {
        for (j = 0; j < kNumIntChannel; j++) {
          cell_bottom_left[j] = cell_bottom_right[j];
          cell_bottom_right[j] += cell_width;
          *(feat_val++) = *(cell_bottom_right[j]) - *(cell_bottom_left[j]);
        }
      }
    }
  }

  // from BR of last cell in current row to BR of first cell in next row
  offset = cell_height * row_width - feat.patch.width *
    kNumIntChannel + cell_width;

  // cells in following rows
  for (i = 1; i < feat.num_cell_per_row; i++) {
    // cells in 1st column
    if (init_cell_x == 0) {
      for (j = 0; j < kNumIntChannel; j++) {
        cell_bottom_right[j] += offset;
        *(feat_val++) = *(cell_bottom_right[j]) - *(cell_top_right[j]);
      }
    } else {
      for (j = 0; j < kNumIntChannel; j++) {
        cell_bottom_right[j] += offset;
        cell_top_left[j] = cell_top_right[j] - cell_width;
        cell_bottom_left[j] = cell_bottom_right[j] - cell_width;
        *(feat_val++) = *(cell_bottom_right[j]) + *(cell_top_left[j]) -
                        *(cell_top_right[j]) - *(cell_bottom_left[j]);
      }
    }

    // cells in following columns
    for (j = 1; j < feat.num_cell_per_row; j++) {
      for (k = 0; k < kNumIntChannel; k++) {
        cell_top_left[k] = cell_top_right[k];
        cell_top_right[k] += cell_width;

        cell_bottom_left[k] = cell_bottom_right[k];
        cell_bottom_right[k] += cell_width;

        *(feat_val++) = *(cell_bottom_right[k]) + *(cell_top_left[k]) -
                        *(cell_bottom_left[k]) - *(cell_top_right[k]);
      }
    }

    for (j = 0; j < kNumIntChannel; j++)
      cell_top_right[j] += offset;
  }
}

void SURFFeatureMap::NormalizeFeatureVectorL2(const int32_t* feat_vec,
    float* feat_vec_normed, int32_t len) const {
  double prod = 0.0;
  float norm_l2 = 0.0f;
  int i;

  for (i = 0; i < len; i++)
    prod += static_cast<double>(feat_vec[i] * feat_vec[i]);
  if (prod != 0) {
    norm_l2 = static_cast<float>(sqrt(prod));
    for (i = 0; i < len; i++)
      feat_vec_normed[i] = feat_vec[i] / norm_l2;
  } else {
    for (i = 0; i < len; i++)
      feat_vec_normed[i] = 0.0f;
  }
}

bool SURFMLP::Classify(float* score, float* outputs) {
  float* dest = &input_buf_[0];
  for (size_t i = 0; i < feat_id_.size(); i++) {
    feat_map_->GetFeatureVector(feat_id_[i] - 1, dest);
    dest += feat_map_->GetFeatureVectorDim(feat_id_[i]);
  }
  output_buf_.resize(model_->GetOutputDim());
  model_->Compute(&input_buf_[0], &output_buf_[0]);

  if (score != NULL)
    *score = output_buf_[0];
  if (outputs != NULL) {
    memcpy(outputs, &output_buf_[0],
      model_->GetOutputDim() * sizeof(float));
  }

  return (output_buf_[0] > thresh_);
}

void SURFMLP::AddFeatureByID(int32_t feat_id) {
  feat_id_.push_back(feat_id);
}

void SURFMLP::AddLayer(int32_t input_dim, int32_t output_dim,
    const float* weights, const float* bias, bool is_output) {
  if (model_->GetLayerNum() == 0)
    input_buf_.resize(input_dim);
  model_->AddLayer(input_dim, output_dim, weights, bias, is_output);
}

bool SURFMLPModelReader_Read(std::istream* input, Classifier* model) {
  std::vector<int32_t> feat_id_buf_;
  std::vector<float> weights_buf_;
  std::vector<float> bias_buf_;
  bool is_read = false;
  SURFMLP* surf_mlp = (SURFMLP*)(model);
  int32_t num_layer;
  int32_t num_feat;
  int32_t input_dim;
  int32_t output_dim;
  float thresh;
  int i;

  input->read(reinterpret_cast<char*>(&num_layer), sizeof(int32_t));
  if (num_layer <= 0) {
    is_read = false;  // @todo handle the errors and the following ones!!!
  }
  input->read(reinterpret_cast<char*>(&num_feat), sizeof(int32_t));
  if (num_feat <= 0) {
    is_read = false;
  }

  feat_id_buf_.resize(num_feat);
  input->read(reinterpret_cast<char*>(&feat_id_buf_[0]),
    sizeof(int32_t) * num_feat);
  for (i = 0; i < num_feat; i++)
    surf_mlp->AddFeatureByID(feat_id_buf_[i]);

  input->read(reinterpret_cast<char*>(&thresh), sizeof(float));
  surf_mlp->SetThreshold(thresh);
  input->read(reinterpret_cast<char*>(&input_dim), sizeof(int32_t));
  if (input_dim <= 0) {
    is_read = false;
  }

  for (i = 1; i < num_layer; i++) {
    input->read(reinterpret_cast<char*>(&output_dim), sizeof(int32_t));
    if (output_dim <= 0) {
      is_read = false;
    }

    int32_t len = input_dim * output_dim;
    weights_buf_.resize(len);
    input->read(reinterpret_cast<char*>(&weights_buf_[0]),
      sizeof(float) * len);

    bias_buf_.resize(output_dim);
    input->read(reinterpret_cast<char*>(&bias_buf_[0]), sizeof(float) * output_dim);

    if (i < num_layer - 1) {
      surf_mlp->AddLayer(input_dim, output_dim, &weights_buf_[0], &bias_buf_[0]);
    } else {
      surf_mlp->AddLayer(input_dim, output_dim, &weights_buf_[0], &bias_buf_[0], true);
    }
    input_dim = output_dim;
  }

  is_read = !input->fail();

  return is_read;
}

#define impl_ this


Classifier*
CreateClassifier_Read(ClassifierType type, std::istream* input) {
  Classifier* classifier = NULL;
  switch (type) {
  case LAB_Boosted_Classifier:
    classifier = (new LABBoostedClassifier());
    LABBoostModelReader_Read(input, classifier);
    break;
  case SURF_MLP:
    classifier = (new SURFMLP());
    SURFMLPModelReader_Read(input, classifier);
    break;
  default:
    break;
  }
  return classifier;
}

FeatureMap*
CreateFeatureMap(ClassifierType type) {
  FeatureMap* feat_map = NULL;
  switch (type) {
  case LAB_Boosted_Classifier:
    feat_map = (new LABFeatureMap());
    break;
  case SURF_MLP:
    feat_map = (new SURFFeatureMap());
    break;
  default:
    break;
  }
  return feat_map;
}



class FaceDetection {
 public:

  /**
   * @brief Detect faces on input image.
   *
   * (1) The input image should be gray-scale, i.e. `num_channels` set to 1.
   * (2) Currently this function does not give the Euler angles, which are
   *     left with invalid values.
   */
   std::vector<FaceInfo> Detect(const ImageData & img);

  /**
   * @brief Set the minimum size of faces to detect.
   *
   * The minimum size is constrained as no smaller than 20. Invalid values will
   * be ignored.
   */
   void SetMinFaceSize(int32_t size);

  /** 
   * @brief Set the maximum size of faces to detect.
   * 
   * The maximum face size actually used is computed as the minimum among: user
   * specified size, image width, image height.
   */
   void SetMaxFaceSize(int32_t size);

  /**
   * @brief Set the factor between adjacent scales of image pyramid.
   *
   * The value of the factor lies in (0, 1). For example, when it is set as 0.5,
   * an input image of size w x h will be resized to 0.5w x 0.5h, 0.25w x 0.25h,
   * 0.125w x 0.125h, etc. Invalid values will be ignored.
   */
   void SetImagePyramidScaleFactor(float factor);

  /**
   * @brief Set the sliding window step in horizontal and vertical directions.
   * 
   * The steps should take positive values, and invalid ones will be ignored.
   * Usually a step of 4 is a reasonable choice.
   */
   void SetWindowStep(int32_t step_x, int32_t step_y);

  /**
   * @brief Set the score thresh of detected faces.
   *
   * Detections with scores smaller than the threshold will not be returned.
   * Typical threshold values include 0.95, 2.8, 4.5. One can adjust the
   * threshold based on his or her own test set.
   */
   void SetScoreThresh(float thresh);

  DISABLE_COPY_AND_ASSIGN(FaceDetection);

 public:
   FaceDetection(const char* model_path);
   ~FaceDetection();

  void init()
  {
    wnd_size_ = (40), slide_wnd_step_x_ = (4), slide_wnd_step_y_ = (4),
      num_hierarchy_ = (0),
      slide_wnd_step_x_ = (4), slide_wnd_step_y_ = (4),
      min_face_size_ = (20), max_face_size_ = (-1),
      cls_thresh_ = (3.85f);
    hierarchy_size_ = NULL;
    wnd_data_buf_.resize(wnd_size_ * wnd_size_);
    wnd_data_.resize(wnd_size_ * wnd_size_);
    int i;
    for (i=0; i<ClassifierCount; ++i) {
      cls2feat_idx_[i] = -1;
    }
  }

  virtual bool LoadModel(const char* model_path);
  virtual std::vector<FaceInfo> Detect(ImagePyramid* img_pyramid);

  inline virtual void SetWindowSize(int32_t size) {
    if (size >= 20)
      wnd_size_ = size;
  }

  inline virtual void SetSlideWindowStep(int32_t step_x, int32_t step_y) {
    if (step_x > 0)
      slide_wnd_step_x_ = step_x;
    if (step_y > 0)
      slide_wnd_step_y_ = step_y;
  }


  inline bool IsLegalImage(const ImageData & image) {
    return (image.num_channels == 1 && image.width > 0 && image.height > 0 &&
      image.data != NULL);
  }

 public:


  void GetWindowData(const ImageData & img, const CRect & wnd);

  int32_t min_face_size_;
  int32_t max_face_size_;
  int32_t slide_wnd_step_x_;
  int32_t slide_wnd_step_y_;
  float cls_thresh_;

  std::vector<FaceInfo> pos_wnds_;
  ImagePyramid img_pyramid_;

  int32_t wnd_size_;
  //int32_t slide_wnd_step_x_;
  //int32_t slide_wnd_step_y_;

  int32_t num_hierarchy_;
  int32_t* hierarchy_size_;
  std::vector<int32_t> num_stage_;
  std::vector<std::vector<int32_t> > wnd_src_id_;

  std::vector<uint8_t> wnd_data_buf_;
  std::vector<uint8_t> wnd_data_;

  std::vector<Classifier* > model_;
  FeatureMap* feat_map_[ClassifierCount];
  //std::map<ClassifierType, int32_t>
  int32_t cls2feat_idx_[ClassifierCount];

};


bool FaceDetection::LoadModel(const char* model_path) {
  std::ifstream model_file(model_path, std::ios_base::binary);
  bool is_loaded = true;

  if (!model_file.is_open()) {
    is_loaded = false;
  } else {
    //hierarchy_size_;
    num_stage_.clear();
    wnd_src_id_.clear();

    int32_t hierarchy_size;
    int32_t num_stage;
    int32_t num_wnd_src;
    int32_t type_id;
    int32_t feat_map_index = 0;
    Classifier* classifier;
    ClassifierType classifier_type;
    int i, j, k;

    model_file.read(reinterpret_cast<char*>(&num_hierarchy_), sizeof(int32_t));
    MYREALLOC(hierarchy_size_, num_hierarchy_);
    for (i = 0; is_loaded && i < num_hierarchy_; i++) {
      model_file.read(reinterpret_cast<char*>(&hierarchy_size), sizeof(int32_t));
      hierarchy_size_[i] = (hierarchy_size);

      for (j = 0; is_loaded && j < hierarchy_size; j++) {
        model_file.read(reinterpret_cast<char*>(&num_stage), sizeof(int32_t));
        num_stage_.push_back(num_stage);

        for (k = 0; is_loaded && k < num_stage; k++) {
          model_file.read(reinterpret_cast<char*>(&type_id), sizeof(int32_t));
          classifier_type = static_cast<ClassifierType>(type_id);

          is_loaded = !model_file.fail() &&
            (classifier = CreateClassifier_Read(classifier_type, &model_file));
          if (is_loaded) {
            model_.push_back(classifier);
            if (cls2feat_idx_[(classifier_type)] == -1) {
              feat_map_[feat_map_index] = CreateFeatureMap(classifier_type);
              cls2feat_idx_[classifier_type] = feat_map_index++;
              //cls2feat_idx_.insert(std::map<ClassifierType, int32_t>::value_type(classifier_type, feat_map_index++));
            }
            FeatureMap* feat_map = feat_map_[cls2feat_idx_[classifier_type]];
            classifier->SetFeatureMap(feat_map);
          }
        }

        wnd_src_id_.push_back(std::vector<int32_t>());
        model_file.read(reinterpret_cast<char*>(&num_wnd_src), sizeof(int32_t));
        if (num_wnd_src > 0) {
          wnd_src_id_.back().resize(num_wnd_src);
          for (int32_t k = 0; k < num_wnd_src; k++) {
            model_file.read(reinterpret_cast<char*>(&(wnd_src_id_.back()[k])), sizeof(int32_t));
          }
        }
      }
    }

    model_file.close();
  }

  return is_loaded;
}

std::vector<FaceInfo> FaceDetection::Detect(ImagePyramid* img_pyramid) {
  float score;
  FaceInfo wnd_info;
  CRect wnd;
  float scale_factor = 0.0;
  const ImageData* img_scaled = img_pyramid->GetNextScaleImage(&scale_factor);

  wnd.height = wnd.width = wnd_size_;

  int i, j, k, m, x, y;
  // Sliding window

  std::vector<std::vector<FaceInfo> > proposals(hierarchy_size_[0]);
  FeatureMap* feat_map_1 = feat_map_[cls2feat_idx_[model_[0]->type()]];

  tic;
  while (img_scaled != NULL) {
    tic;
    feat_map_1->Compute(img_scaled->data, img_scaled->width, img_scaled->height);
    toc;
    wnd_info.bbox.width = static_cast<int32_t>(wnd_size_ / scale_factor + 0.5);
    wnd_info.bbox.height = wnd_info.bbox.width;

    int32_t max_x = img_scaled->width - wnd_size_;
    int32_t max_y = img_scaled->height - wnd_size_;
    for (y = 0; y <= max_y; y += slide_wnd_step_y_) {
      wnd.y = y;
      for (x = 0; x <= max_x; x += slide_wnd_step_x_) {
        wnd.x = x;
        feat_map_1->SetROI(wnd);

        wnd_info.bbox.x = static_cast<int32_t>(x / scale_factor + 0.5);
        wnd_info.bbox.y = static_cast<int32_t>(y / scale_factor + 0.5);

        for (i = 0; i < hierarchy_size_[0]; i++) {
          bool isPos;
          tic;
          isPos = model_[i]->Classify(&score);
          toc;
          if (isPos) {
            wnd_info.score = static_cast<double>(score);
            proposals[i].push_back(wnd_info);
          }
        }
      }
    }

    img_scaled = img_pyramid->GetNextScaleImage(&scale_factor);
  }
  toc;
  std::vector<std::vector<FaceInfo> > proposals_nms(hierarchy_size_[0]);
  for (i = 0; i < hierarchy_size_[0]; i++) {
    NonMaximumSuppression(&(proposals[i]), &(proposals_nms[i]), 0.8f);
    proposals[i].clear();
  }

  // Following classifiers
  ImageData img = img_pyramid->image1x();
  CRect roi;
  float mlp_predicts[4];  // @todo no hard-coded number!
  roi.x = roi.y = 0;
  roi.width = roi.height = wnd_size_;

  int32_t cls_idx = hierarchy_size_[0];
  int32_t model_idx = hierarchy_size_[0];
  std::vector<int32_t> buf_idx;

  for (i = 1; i < num_hierarchy_; i++) {
    buf_idx.resize(hierarchy_size_[i]);
    for (j = 0; j < hierarchy_size_[i]; j++) {
      int32_t num_wnd_src = static_cast<int32_t>(wnd_src_id_[cls_idx].size());
      std::vector<int32_t> & wnd_src = wnd_src_id_[cls_idx];
      buf_idx[j] = wnd_src[0];
      proposals[buf_idx[j]].clear();
      for (k = 0; k < num_wnd_src; k++) {
        proposals[buf_idx[j]].insert(proposals[buf_idx[j]].end(),
          proposals_nms[wnd_src[k]].begin(), proposals_nms[wnd_src[k]].end());
      }

      FeatureMap* feat_map =
        feat_map_[cls2feat_idx_[model_[model_idx]->type()]];
      for (k = 0; k < num_stage_[cls_idx]; k++) {
        int32_t num_wnd = static_cast<int32_t>(proposals[buf_idx[j]].size());
        std::vector<FaceInfo> & bboxes = proposals[buf_idx[j]];
        int32_t bbox_idx = 0;

        for (m = 0; m < num_wnd; m++) {
          if (bboxes[m].bbox.x + bboxes[m].bbox.width <= 0 ||
              bboxes[m].bbox.y + bboxes[m].bbox.height <= 0)
            continue;
          tic;
          GetWindowData(img, bboxes[m].bbox);
          toc;
          tic;
          feat_map->Compute(&wnd_data_[0], wnd_size_, wnd_size_);
          toc;
          feat_map->SetROI(roi);
          bool isPos;
          tic;
          isPos = model_[model_idx]->Classify(&score, mlp_predicts);
          toc;

          if (isPos) {
            tic;
            float x = static_cast<float>(bboxes[m].bbox.x);
            float y = static_cast<float>(bboxes[m].bbox.y);
            float w = static_cast<float>(bboxes[m].bbox.width);
            float h = static_cast<float>(bboxes[m].bbox.height);

            bboxes[bbox_idx].bbox.width = static_cast<int32_t>((mlp_predicts[3] * 2 - 1) * w + w + 0.5);
            bboxes[bbox_idx].bbox.height = bboxes[bbox_idx].bbox.width;
            bboxes[bbox_idx].bbox.x = static_cast<int32_t>((mlp_predicts[1] * 2 - 1) * w + x +
              (w - bboxes[bbox_idx].bbox.width) * 0.5 + 0.5);
            bboxes[bbox_idx].bbox.y = static_cast<int32_t>((mlp_predicts[2] * 2 - 1) * h + y +
              (h - bboxes[bbox_idx].bbox.height) * 0.5 + 0.5);
            bboxes[bbox_idx].score = score;
            bbox_idx++;
            toc;
          }
        }
        proposals[buf_idx[j]].resize(bbox_idx);

        if (k < num_stage_[cls_idx] - 1) {
          NonMaximumSuppression(&(proposals[buf_idx[j]]), &(proposals_nms[buf_idx[j]]), 0.8f);
          proposals[buf_idx[j]] = proposals_nms[buf_idx[j]];
        } else {
          if (i == num_hierarchy_ - 1) {
            NonMaximumSuppression(&(proposals[buf_idx[j]]), &(proposals_nms[buf_idx[j]]), 0.3f);
            proposals[buf_idx[j]] = proposals_nms[buf_idx[j]];
          }
        }
        model_idx++;
      }

      cls_idx++;
    }

    for (j = 0; j < hierarchy_size_[i]; j++)
      proposals_nms[j] = proposals[buf_idx[j]];
  }

  return proposals_nms[0];
}

void FaceDetection::GetWindowData(const ImageData & img, const CRect & wnd) {
  int32_t pad_left;
  int32_t pad_right;
  int32_t pad_top;
  int32_t pad_bottom;
  CRect roi = wnd;

  pad_left = pad_right = pad_top = pad_bottom = 0;
  if (roi.x + roi.width > img.width)
    pad_right = roi.x + roi.width - img.width;
  if (roi.x < 0) {
    pad_left = -roi.x;
    roi.x = 0;
  }
  if (roi.y + roi.height > img.height)
    pad_bottom = roi.y + roi.height - img.height;
  if (roi.y < 0) {
    pad_top = -roi.y;
    roi.y = 0;
  }

  wnd_data_buf_.resize(roi.width * roi.height);
  const uint8_t* src = img.data + roi.y * img.width + roi.x;
  uint8_t* dest = &wnd_data_buf_[0];
  int32_t len = sizeof(uint8_t) * roi.width;
  int32_t len2 = sizeof(uint8_t) * (roi.width - pad_left - pad_right);

  tic;
  if (pad_top > 0) {
    memset(dest, 0, len * pad_top);
    dest += (roi.width * pad_top);
  }
  if (pad_left == 0) {
    if (pad_right == 0) {
      for (int32_t y = pad_top; y < roi.height - pad_bottom; y++) {
        memcpy(dest, src, len);
        src += img.width;
        dest += roi.width;
      }
    } else {
      for (int32_t y = pad_top; y < roi.height - pad_bottom; y++) {
        memcpy(dest, src, len2);
        src += img.width;
        dest += roi.width;
        memset(dest - pad_right, 0, sizeof(uint8_t) * pad_right);
      }
    }
  } else {
    if (pad_right == 0) {
      for (int32_t y = pad_top; y < roi.height - pad_bottom; y++) {
        memset(dest, 0, sizeof(uint8_t)* pad_left);
        memcpy(dest + pad_left, src, len2);
        src += img.width;
        dest += roi.width;
      }
    } else {
      for (int32_t y = pad_top; y < roi.height - pad_bottom; y++) {
        memset(dest, 0, sizeof(uint8_t) * pad_left);
        memcpy(dest + pad_left, src, len2);
        src += img.width;
        dest += roi.width;
        memset(dest - pad_right, 0, sizeof(uint8_t) * pad_right);
      }
    }
  }
  if (pad_bottom > 0)
    memset(dest, 0, len * pad_bottom);

  toc;
  ImageData src_img(&wnd_data_buf_[0], roi.width, roi.height, 1);
  ImageData dest_img(&wnd_data_[0], wnd_size_, wnd_size_, 1);
  ResizeImage(&src_img, &dest_img);
}

static const int32_t kWndSize = 40;

FaceDetection::FaceDetection(const char* model_path)
{
  init();
  LoadModel(model_path);
}

FaceDetection::~FaceDetection() {
}

std::vector<FaceInfo> FaceDetection::Detect(
    const ImageData & img) {
  if (!impl_->IsLegalImage(img))
    return std::vector<FaceInfo>();

  int32_t min_img_size = img.height <= img.width ? img.height : img.width;
  min_img_size = (impl_->max_face_size_ > 0 ?
    (min_img_size >= impl_->max_face_size_ ? impl_->max_face_size_ : min_img_size) :
    min_img_size);

  impl_->img_pyramid_.SetImage1x(img.data, img.width, img.height);
  impl_->img_pyramid_.SetMinScale(static_cast<float>(kWndSize) / min_img_size);

  impl_->SetWindowSize(kWndSize);
  impl_->SetSlideWindowStep(impl_->slide_wnd_step_x_,
    impl_->slide_wnd_step_y_);

  impl_->pos_wnds_ = impl_->Detect(&(impl_->img_pyramid_));

  for (int32_t i = 0; i < impl_->pos_wnds_.size(); i++) {
    if (impl_->pos_wnds_[i].score < impl_->cls_thresh_) {
      impl_->pos_wnds_.resize(i);
      break;
    }
  }

  return impl_->pos_wnds_;
}

void FaceDetection::SetMinFaceSize(int32_t size) {
  if (size >= 20) {
    impl_->min_face_size_ = size;
    impl_->img_pyramid_.SetMaxScale(kWndSize / static_cast<float>(size));
  }
}

void FaceDetection::SetMaxFaceSize(int32_t size) {
  if (size >= 0)
    impl_->max_face_size_ = size;
}

void FaceDetection::SetImagePyramidScaleFactor(float factor) {
  if (factor >= 0.01f && factor <= 0.99f)
    impl_->img_pyramid_.SetScaleStep(static_cast<float>(factor));
}

void FaceDetection::SetWindowStep(int32_t step_x, int32_t step_y) {
  if (step_x > 0)
    impl_->slide_wnd_step_x_ = step_x;
  if (step_y > 0)
    impl_->slide_wnd_step_y_ = step_y;
}

void FaceDetection::SetScoreThresh(float thresh) {
  if (thresh >= 0)
    impl_->cls_thresh_ = thresh;
}


