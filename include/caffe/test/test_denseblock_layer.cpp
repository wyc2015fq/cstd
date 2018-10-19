#include <vector>

#include "gtest/gtest.h"

#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/filler.hpp"
#include "caffe/layers/denseblock_layer.hpp"

#ifdef USE_CUDNN
#include "caffe/layers/cudnn_conv_layer.hpp"
#endif

#include "caffe/test/test_caffe_main.hpp"
#include "caffe/test/test_gradient_check_util.hpp"


namespace caffe
{

template <typename TypeParam>
class DenseBlockLayerTest : public MultiDeviceTest<TypeParam>
{
  typedef typename TypeParam::Dtype Dtype;

protected:
  DenseBlockLayerTest()
    : blob_bottom_(new Blob<Dtype>(2, 3, 6, 4)),
    blob_bottom_2_(new Blob<Dtype>(2, 3, 6, 4)),
    blob_top_(new Blob<Dtype>()),
    blob_top_2_(new Blob<Dtype>()) {}
  virtual void SetUp() {
    // fill the values
    FillerParameter filler_param;
    filler_param.set_value(1.);
    GaussianFiller<Dtype> filler(filler_param);
    filler.Fill(this->blob_bottom_);
    filler.Fill(this->blob_bottom_2_);
    blob_bottom_vec_.push_back(blob_bottom_);
    blob_top_vec_.push_back(blob_top_);
    blob_top_vec_2_.push_back(blob_top_2_);
  }

  virtual ~DenseBlockLayerTest() {
    delete blob_bottom_;
    delete blob_bottom_2_;
    delete blob_top_;
    delete blob_top_2_;
  }

  virtual Blob<Dtype>* MakeReferenceTop(Blob<Dtype>* top) {
    this->ref_blob_top_.reset(new Blob<Dtype>());
    this->ref_blob_top_->ReshapeLike(*top);
    return this->ref_blob_top_.get();
  }

  Blob<Dtype>* const blob_bottom_;
  Blob<Dtype>* const blob_bottom_2_;
  Blob<Dtype>* const blob_top_;
  Blob<Dtype>* const blob_top_2_;
  SHARED_PTR<Blob<Dtype> > ref_blob_top_;
  vector<Blob<Dtype>*> blob_bottom_vec_;
  vector<Blob<Dtype>*> blob_top_vec_;
  vector<Blob<Dtype>*> blob_top_vec_2_;
};

TYPED_TEST_CASE(DenseBlockLayerTest, TestDtypesAndDevices);

template <typename Dtype>
void convolution_Fwd_fast(Blob<Dtype>* input, Blob<Dtype>* output, Blob<Dtype>* filter, int N, int c_output, int c_input, int h_img, int w_img, int h_filter, int w_filter)
{
  //std::vector<Dtype> data_col;
  //Dtype* data_col = data_col.data();
  void im2col_cpu(const Dtype* data_im, const int channels,
    const int height, const int width, const int kernel_h, const int kernel_w,
    const int pad_h, const int pad_w,
    const int stride_h, const int stride_w,
    const int dilation_h, const int dilation_w,
    Dtype* data_col);

  int outputShape[] = { N, c_output, h_img, w_img };
  vector<int> outputShapeVec(outputShape, outputShape + 4);
  output->Reshape(outputShapeVec);
  Dtype* outputPtr = output->mutable_cpu_data();
  for (int n = 0; n < N; ++n) {
    for (int c_outIdx = 0; c_outIdx < c_output; ++c_outIdx) {
      for (int hIdx = 0; hIdx < h_img; ++hIdx) {
        for (int wIdx = 0; wIdx < w_img; ++wIdx) {
          outputPtr[output->offset(n, c_outIdx, hIdx, wIdx)] = 0;
          for (int c_inIdx = 0; c_inIdx < c_input; ++c_inIdx) {
            for (int filter_x = 0; filter_x < h_filter; ++filter_x) {
              for (int filter_y = 0; filter_y < w_filter; ++filter_y) {
                int localX = hIdx + (h_filter / 2) - filter_x;
                int localY = wIdx + (w_filter / 2) - filter_y;
                outputPtr[output->offset(n, c_outIdx, hIdx, wIdx)] += (filter->data_at(c_outIdx, c_inIdx, filter_x, filter_y) * getZeroPaddedValue(false, input, n, c_inIdx, localX, localY));
              }
            }
          }
        }
      }
    }
  }
}
#if 0
TYPED_TEST(DenseBlockLayerTest, TestSimpleConvolutionFwd)
{
  typedef typename TypeParam::Dtype Dtype;
  int N = this->blob_bottom_->shape(0);
  int c_input = this->blob_bottom_->shape(1);
  int h_img = this->blob_bottom_->shape(2);
  int w_img = this->blob_bottom_->shape(3);
  int c_output = this->blob_bottom_2_->shape(0);
  int h_filter = this->blob_bottom_2_->shape(2);
  int w_filter = this->blob_bottom_2_->shape(3);
  convolution_Fwd(this->blob_bottom_, this->blob_top_, this->blob_bottom_2_,
    N, c_output, c_input, h_img, w_img, h_filter, w_filter);
  // Check against reference convolution.
  convolution_Fwd_fast(this->blob_bottom_, this->MakeReferenceTop(this->blob_top_), this->blob_bottom_2_,
    N, c_output, c_input, h_img, w_img, h_filter, w_filter);
  const Dtype* top_data;
  const Dtype* ref_top_data;
  top_data = this->blob_top_->cpu_data();
  ref_top_data = this->ref_blob_top_->cpu_data();
  for (int i = 0; i < this->blob_top_->count(); ++i) {
    EXPECT_NEAR(top_data[i], ref_top_data[i], 1e-4);
  }
}
#endif

TYPED_TEST(DenseBlockLayerTest, TestCPUvsGPU)
{
  typedef typename TypeParam::Dtype Dtype;
  LayerParameter layer_param;
  DenseBlockParameter* convolution_param = layer_param.mutable_denseblock_param();
  convolution_param->set_numtransition(10);
  Caffe::set_mode(Caffe::GPU);
  SHARED_PTR<DenseBlockLayer<Dtype> > layer(new DenseBlockLayer<Dtype>(layer_param));
  layer->SetUp(this->blob_bottom_vec_, this->blob_top_vec_);
  layer->Forward(this->blob_bottom_vec_, this->blob_top_vec_);

  Caffe::set_mode(Caffe::CPU);
  SHARED_PTR<DenseBlockLayer<Dtype> > layer_2(new DenseBlockLayer<Dtype>(layer_param));
  layer_2->SetUp(this->blob_bottom_vec_, this->blob_top_vec_2_);
  layer_2->Forward(this->blob_bottom_vec_, this->blob_top_vec_2_);
  
  // Check against reference convolution.
  const Dtype* top_data = this->blob_top_->cpu_data();
  const Dtype* top_data_2 = this->blob_top_2_->cpu_data();
  for (int i = 0; i < this->blob_top_->count(); ++i) {
    //printf("%6.3lf %6.3lf\n", (double)top_data[i], (double)top_data_2[i]);
    EXPECT_NEAR(top_data[i], top_data_2[i], 1e-4);
  }
#if 0
#endif
  return;
}


}
