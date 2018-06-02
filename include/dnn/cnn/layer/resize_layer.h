


  /**
  * @brief ResizeLayer resize blob using bilinear interpolation
  *
  * 	   In layer configuration, <code>is_pyramid_test</code> specifiy whether
  * 	   the output height and width is fixed. if <code> is_pyramid_test == false </code>,
  * 	   you should give the <code>height</code> and <code>width</code> for output.
  * 	   Otherwise, you should give both <code>out_height_scale</code> and
  * 	   <code>out_width_scale</code>
  */
  
  class ResizeLayer : public Layer {
  public:
    explicit ResizeLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top);

    virtual inline const char* type() const { return "Resize"; }

    virtual inline int ExactNumTopBlobs() const { return 1; }
    virtual inline int ExactNumBottomBlobs() const { return 1; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    vector<Blob*> locs_;
    int out_height_;
    int out_width_;
    int out_channels_;
    int out_num_;

  };


  
  void ResizeLayer::LayerSetUp(const vector<Blob*>& bottom,
                                      const vector<Blob*>& top) {
    // Configure the kernel size, padding, stride, and inputs.
    ResizeParameter resize_param = this->layer_param_.resize_param();


    bool is_pyramid_test = resize_param.is_pyramid_test();
    if (is_pyramid_test == false) {
      CHECK(resize_param.has_height()) << "output height is required ";
      CHECK(resize_param.has_width()) << "output width is required ";
      this->out_height_ = resize_param.height();
      this->out_width_ = resize_param.width();
    }
    else {
      CHECK(resize_param.has_out_height_scale()) << "output height scale is required ";
      CHECK(resize_param.has_out_width_scale()) << "output width scale is required ";
      int in_height = prev->size.h;
      int in_width = prev->size.w;
      this->out_height_ = int(resize_param.out_height_scale() * in_height);
      this->out_width_ = int(resize_param.out_width_scale() * in_width);
    }




    for (i = 0; i<4; i++) {
      this->locs_.push_back(new Blob);
    }
  }

  
  void ResizeLayer::Reshape(const vector<Blob*>& bottom,
                                   const vector<Blob*>& top) {

    ResizeParameter resize_param = this->layer_param_.resize_param();

    bool is_pyramid_test = resize_param.is_pyramid_test();
    if (is_pyramid_test == false) {

      this->out_height_ = resize_param.height();
      this->out_width_ = resize_param.width();
    }
    else {
      int in_height = prev->size.h;
      int in_width = prev->size.w;
      this->out_height_ = int(resize_param.out_height_scale() * in_height);
      this->out_width_ = int(resize_param.out_width_scale() * in_width);
    }


    this->out_num_ = prev->size.n;
    this->out_channels_ = prev->size.c;
    top[0]->Reshape(out_num_, out_channels_, out_height_, out_width_);

    for (i = 0; i<4; ++i) {
      this->locs_[i]->Reshape(1, 1, out_height_, out_width_);
    }
  }

  
  void ResizeLayer::Forward(const vector<Blob*>& bottom,
                                       const vector<Blob*>& top) {
    //	ResizeBlob(bottom[0],top[0],this->locs_[0],this->locs_[1],this->locs_[2],this->locs_[3]);
    ResizeBlob(bottom[0], top[0]);

  }

  
  void ResizeLayer::Backward(const vector<Blob*>& top,
                                        const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    Dtype* prevDiff = bottom[0]->mutable_diff();
    Dtype* nextDiff = top[0]->mutable_diff();

    const Dtype* loc1 = this->locs_[0]->cpu_data();
    const Dtype* weight1 = this->locs_[0]->cpu_diff();
    const Dtype* loc2 = this->locs_[1]->cpu_data();
    const Dtype* weight2 = this->locs_[1]->cpu_diff();
    const Dtype* loc3 = this->locs_[2]->cpu_data();
    const Dtype* weight3 = this->locs_[2]->cpu_diff();
    const Dtype* loc4 = this->locs_[3]->cpu_data();
    const Dtype* weight4 = this->locs_[3]->cpu_diff();


    caffe::blas_sset(Data_count(prev->size), (Dtype)(0), prevDiff);

    caffe::GetBiLinearResizeMatRules(prev->size.h, prev->size.w,
                                         top[0]->height(), top[0]->width(),
                                         this->locs_[0]->mutable_data(), this->locs_[0]->mutable_diff(),
                                         this->locs_[1]->mutable_data(), this->locs_[1]->mutable_diff(),
                                         this->locs_[2]->mutable_data(), this->locs_[2]->mutable_diff(),
                                         this->locs_[3]->mutable_data(), this->locs_[3]->mutable_diff());

    for (n = 0; n< this->out_num_; ++n) {
      for (c = 0; c < this->out_channels_; ++c) {
        int bottom_diff_offset = bottom[0]->offset(n, c);
        int top_diff_offset = top[0]->offset(n, c);

        for (idx = 0; idx < this->out_height_* this->out_width_; ++idx) {
          prevDiff[bottom_diff_offset + (int)(loc1[idx])] += nextDiff[top_diff_offset + idx] * weight1[idx];
          prevDiff[bottom_diff_offset + (int)(loc2[idx])] += nextDiff[top_diff_offset + idx] * weight2[idx];
          prevDiff[bottom_diff_offset + (int)(loc3[idx])] += nextDiff[top_diff_offset + idx] * weight3[idx];
          prevDiff[bottom_diff_offset + (int)(loc4[idx])] += nextDiff[top_diff_offset + idx] * weight4[idx];
        }
      }
    }
  }

#ifdef CPU_ONLY
  STUB_GPU(ResizeLayer);
#endif

  INSTANTIATE_CLASS(ResizeLayer);
  REGISTER_LAYER_CLASS(Resize);

