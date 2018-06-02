

#include "caffe/filler.hpp"
#include "caffe/layer.hpp"
#include "caffe/util/im2col.hpp"

#include "caffe/custom_layers.hpp"




void LocalLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(layer->i_size, 1) << "Conv Layer takes a single blob as input.";
  CHECK_EQ(layer->o_size, 1) << "Conv Layer takes a single blob as output.";

  kernel_size_ = this->layer_param_.local_param().kernel_size();
  stride_ = this->layer_param_.local_param().stride();
  pad_ = this->layer_param_.local_param().pad();
  num_ = prev->size.n;
  channels_ = prev->size.c;
  height_ = prev->size.h;
  width_ = prev->size.w;
  num_output_ = this->layer_param_.local_param().num_output();

  height_out_ = (height_ + 2 * pad_ - kernel_size_) / stride_ + 1;
  width_out_ = (width_ + 2 * pad_ - kernel_size_) / stride_ + 1;

  M_ = num_output_;
  K_ = channels_ * kernel_size_ * kernel_size_;
  N_ = height_out_ * width_out_;

  CHECK_GT(kernel_size_, 0); 
  CHECK_GT(num_output_, 0); 
  CHECK_GE(height_, kernel_size_) << "height smaller than kernel size";
  CHECK_GE(width_, kernel_size_) << "width smaller than kernel size";
  // Set the parameters
  bias_term_ = this->layer_param_.local_param().bias_term();

  // Check if we need to set up the weights
  if (this->blobs_.size() > 0) {
    LOG(INFO) << "Skipping parameter initialization";
  } else {
    if (bias_term_) {
      this->blobs_.resize(2);
    } else {
      this->blobs_.resize(1);
    }
    // Intialize the weight
    this->blobs_[0].reset(new Blob(
        num_output_, 1, K_, N_));
    // fill the weights
    shared_ptr<Filler > weight_filler(GetFiller(
        this->layer_param_.local_param().weight_filler()));
    weight_filler->Fill(this->blobs_[0].get());
    // If necessary, intiialize and fill the bias term
    if (bias_term_) {
      this->blobs_[1].reset(new Blob(1, 1, M_, N_));
      shared_ptr<Filler > bias_filler(GetFiller(
          this->layer_param_.local_param().bias_filler()));
      bias_filler->Fill(this->blobs_[1].get());  
    }
  }
}


void LocalLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  CHECK_EQ(prev->size.c, channels_) << "Input size incompatible with"
    " weights.";
  // TODO: generalize to handle inputs of different shapes.
  for (bottom_id = 1; bottom_id < layer->i_size; ++bottom_id) {
    CHECK_EQ(num_, bottom[bottom_id]->size.n) << "Inputs must have same num.";
    CHECK_EQ(channels_, bottom[bottom_id]->size.c)
        << "Inputs must have same channels.";
    CHECK_EQ(height_, bottom[bottom_id]->height())
        << "Inputs must have same height.";
    CHECK_EQ(width_, bottom[bottom_id]->width())
        << "Inputs must have same width.";
  }

  // Shape the tops.
  for (top_id = 0; top_id < layer->o_size; ++top_id) {
    top[top_id]->Reshape(num_, num_output_, height_out_, width_out_);
  }

  // The im2col result buffer would only hold one image at a time to avoid
  // overly large memory usage.
  col_buffer_.Reshape(
      1, channels_ * kernel_size_ * kernel_size_, height_out_, width_out_);

  for (top_id = 0; top_id < layer->o_size; ++top_id) {
    top[top_id]->Reshape(num_, num_output_, height_out_, width_out_);
  }
}


void LocalLayer::Forward(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {

  Dtype* x_data = col_buffer_;
  const Dtype* weight = this->blobs_[0]->cpu_data();
  const Dtype* prevData = prevData;
  Dtype* nextData = nextData;

  Blob E;
  E.Reshape(1, 1, 1, K_);
  FillerParameter filler_param;
  filler_param.set_value(1);
  ConstantFiller filler(filler_param);
  filler.Fill(&E);

  Blob intermediate;
  intermediate.Reshape(1, 1, K_, N_);
  for (n=0; n<num_; n++) {
    im2col(prevData + bottom[0]->offset(n), channels_, height_,
               width_, kernel_size_, kernel_size_, pad_, pad_, stride_, stride_, 1, 1, x_data);

    for (m=0; m<num_output_; m++) { 
      blas_smul(K_*N_, x_data, weight+this->blobs_[0]->offset(m),
                intermediate);

      blas_sgemm(CblasNoTrans, CblasNoTrans, 1, N_, K_,
                            (Dtype)1., E.cpu_data(),
                            intermediate.cpu_data(),
                            (Dtype)0., nextData + top[0]->offset(n, m));
    }

    if (bias_term_) {
      blas_sadd(M_ * N_, this->blobs_[1]->cpu_data(),
                nextData + top[0]->offset(n),
                nextData + top[0]->offset(n));
    }
  }
}


void LocalLayer::Backward(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {

  const Dtype* nextDiff = nextDiff;
  const Dtype* prevData = prevData;
  Dtype* prevDiff = bottom[0]->mutable_diff();
  Dtype* x_data = col_buffer_;
  Dtype* x_diff = col_buffer_.mutable_diff();
  const Dtype* weight = this->blobs_[0]->cpu_data();
  Dtype* weight_diff = this->blobs_[0]->mutable_diff();
  Dtype* bias_diff = NULL;

  Blob intermediate;
  intermediate.Reshape(1, 1, 1, N_);

  Blob xt;
  xt.Reshape(1, 1, K_, N_);
  Dtype* xt_data = xt;

  if (bias_term_) {
    bias_diff = this->blobs_[1]->mutable_diff();
    memset(bias_diff, 0, sizeof(Dtype) * this->blobs_[1]->count());
    for (n = 0; n < num_; ++n) {
      blas_sadd(M_ * N_, bias_diff,
                nextDiff + top[0]->offset(n),
                bias_diff);
    }
  }

  memset(weight_diff, 0, sizeof(Dtype) * this->blobs_[0]->count());
  for (n=0; n<num_; n++) {
    im2col(prevData + bottom[0]->offset(n), channels_, height_,
               width_, kernel_size_, kernel_size_, pad_, pad_, stride_, stride_, 1, 1, x_data);

    // gradient wrt weight
    for (m=0; m<num_output_; m++) {
      Dtype* filter_weight_diff = weight_diff+this->blobs_[0]->offset(m);
      for (k=0; k<K_; k++) {
        blas_smul(N_, nextDiff+top[0]->offset(n, m),  
                  x_data+col_buffer_.offset(0,k), xt_data+xt.offset(0,0,k));
      }
      blas_saxpby(K_*N_, (Dtype)(1.0), xt_data, (Dtype)(1.0), filter_weight_diff);
    }
      
    // gradient wrt bottom data
    if (propagate_down[0]) {
      memset(x_diff, 0, col_buffer_.count() * sizeof(Dtype));
      for (m=0; m<num_output_; m++) {
        for (k=0; k<K_; k++) {
          blas_smul(N_, nextDiff+top[0]->offset(n, m),
                    weight+this->blobs_[0]->offset(m,0,k),
                    intermediate);

          blas_saxpby(N_, (Dtype)(1.0),
                          intermediate.cpu_data(), (Dtype)(1.0),
                          x_diff+col_buffer_.offset(0,k));
        }
      }

      // col2im back to the data
      col2im(x_diff, channels_, height_, width_, kernel_size_, kernel_size_,
                 pad_, pad_, stride_, stride_, 1, 1, prevDiff + bottom[0]->offset(n));

    }
  }

}

#ifdef CPU_ONLY
STUB_GPU(LocalLayer);
#endif

INSTANTIATE_CLASS(LocalLayer);
REGISTER_LAYER_CLASS(Local);

