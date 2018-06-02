


  /**
  * @brief Produce point heat maps by a series of the input 2d coordinates.
  *
  * TODO(dox): thorough documentation for Forward, Backward, and proto params.
  */
  
  class HotspotLayer : public Layer {
  public:
    explicit HotspotLayer(const LayerParameter& param)
      : Layer(param) {}

    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);

    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Hotspot"; }
    virtual inline int MinBottomBlobs() const { return 1; }
    virtual inline int ExactTopBlobs() const { return 1; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    int height_;
    int width_;
    Dtype gaussian_std_;
    int data_height_;
    int data_width_;
    bool mean_removed_;
  };



#include "hotspot_layer.hpp"
#define CV_PI 3.1415926535897932384626433832795
#define GAUSSIAN(x0,y0,x,y) 0.5 / gaussian_std_ / gaussian_std_ / CV_PI * exp(-0.5 * (((x0)-(x)) * ((x0)-(x)) + ((y0)-(y)) * ((y0)-(y))) / gaussian_std_ / gaussian_std_)



  const float kEps = 1e-4;


void HotspotLayer::LayerSetUp(const vector<Blob*>& bottom,
                                  const vector<Blob*>& top) {
  HotspotParameter hotspot_param = this->layer_param_.hotspot_param();
  height_ = hotspot_param.output_height();
  width_ = hotspot_param.output_width();
  gaussian_std_ = hotspot_param.gaussian_std();
  data_height_ = hotspot_param.data_height();
  data_width_ = hotspot_param.data_width();
  mean_removed_ = hotspot_param.mean_removed();
}


void HotspotLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(prev->size.c % 2, 0) << "The coordinate blob's size must be able to divided by 2!";
  CHECK(data_width_ * data_height_ > 0) << "The data layers' size must be specified.";
  int num_point = prev->size.c / 2;
  if (height_ * width_ == 0) {
    CHECK(layer->i_size == 2) << "When the ooutput_height and output_width is not mannually set, "
      << "one more bottom blob is needed to inference the size of the output.";
    height_ = prev1->size.h;
    width_ = prev1->size.w;
  }
  top[0]->Reshape({ prev->size.n, num_point, height_, width_ });
}


void HotspotLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const Dtype* point_data = prevData;
  Dtype* nextData = nextData;
  
  const int num_point = prevSize.c / 2;
  const int num = prev->size.n;
  Dtype temp;
  for (n = 0; n < num; n++) {
    for (i = 0; i < num_point; i++) {
      float p1 = (point_data[n * num_point * 2 + 2 * i] / (Dtype)data_width_ + (mean_removed_ ? 0.5 : 0)) * (Dtype)width_;
      float p2 = (point_data[n * num_point * 2 + 2 * i + 1] / (Dtype)data_height_ + (mean_removed_ ? 0.5 : 0)) * (Dtype)height_;
      LOG(INFO)<< "n:"<<n<<" p:"<<i<< " p1:" << p1 << " p2:" << p2;
      for (h = 0; h < height_; h++) {
        for (w = 0; w < width_; w++) {
          temp = GAUSSIAN(p1, p2, (Dtype)w, (Dtype)h);
          if (temp > kEps) {
            nextData[top[0]->offset(n, i, h, w)] = temp;
          }
          else {
            nextData[top[0]->offset(n, i, h, w)] = 0;
          }
        }
      }
    }
  }
}


void HotspotLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
  // Acatually, we can get gradient from the nextDiff for the coordinate values.
  // However, I can't imagine in which scene this could be useful.
  // I will implement it one day I come up with some ideas to utilize the gradients.
  NOT_IMPLEMENTED;
}


#ifdef CPU_ONLY
STUB_GPU(HotspotLayer);
#endif

INSTANTIATE_CLASS(HotspotLayer);
REGISTER_LAYER_CLASS(Hotspot);


