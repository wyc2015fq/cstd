#include <algorithm>


#include "caffe/layers/hinge_loss_layer.hpp"


namespace
{

  template <typename Dtype>
  void HingeLossLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
                                          const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    Dtype* bottom_diff = bottom[0]->mdiff();
    const Dtype* label = bottom[1]->data();
    int num = bottom[0]->num();
    int count = bottom[0]->count();
    int dim = count / num;
    caffe_copy(count, bottom_data, bottom_diff);
    for (int i = 0; i < num; ++i) {
      bottom_diff[i * dim + static_cast<int>(label[i])] *= -1;
    }
    for (int i = 0; i < num; ++i) {
      for (int j = 0; j < dim; ++j) {
        bottom_diff[i * dim + j] = std::max(
                                     Dtype(0), 1 + bottom_diff[i * dim + j]);
      }
    }
    Dtype* loss = top[0]->mdata();
    switch (this->param_->hinge_loss_param().norm()) {
    case HingeLossParameter_Norm_L1:
      loss[0] = caffe_asum(count, bottom_diff) / num;
      break;
    case HingeLossParameter_Norm_L2:
      loss[0] = caffe_dot(count, bottom_diff, bottom_diff) / num;
      break;
    default:
      LOG(FATAL) << "Unknown Norm";
    }
  }

  template <typename Dtype>
  void HingeLossLayer::Backward(CPUContext* context, const vector<Blob*> & top,
      const vector<Blob*> & bottom)
  {
    if (bottom[1]->propagate_down_) {
      LOG(FATAL) << this->type()
                 << " Layer cannot backpropagate to label inputs.";
    }
    if (bottom[0]->propagate_down_) {
      Dtype* bottom_diff = bottom[0]->mdiff();
      const Dtype* label = bottom[1]->data();
      int num = bottom[0]->num();
      int count = bottom[0]->count();
      int dim = count / num;
      for (int i = 0; i < num; ++i) {
        bottom_diff[i * dim + static_cast<int>(label[i])] *= -1;
      }
      const Dtype loss_weight = top[0]->diff()[0];
      switch (this->param_->hinge_loss_param().norm()) {
      case HingeLossParameter_Norm_L1:
        caffe_sign(count, bottom_diff, bottom_diff);
        caffe_scal(count, loss_weight / num, bottom_diff);
        break;
      case HingeLossParameter_Norm_L2:
        caffe_scal(count, loss_weight * 2 / num, bottom_diff);
        break;
      default:
        LOG(FATAL) << "Unknown Norm";
      }
    }
  }

  INSTANTIATE_CLASS(HingeLossLayer);
  REGISTER_LAYER_CLASS(HingeLoss);

}  // namespace
