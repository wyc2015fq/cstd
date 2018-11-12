#include <algorithm>
#include <cmath>


#include "caffe/layers/infogain_loss_layer.hpp"
#include "caffe/util/io.hpp"

namespace
{

  template <typename Dtype>
  void InfogainLossLayer::LayerSetUp(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    LossLayer::LayerSetUp(bottom, top);
    if (bottom.size() < 3) {
      CHECK(this->param_->infogain_loss_param().has_source())
          << "Infogain matrix source must be specified.";
      BlobProto blob_proto;
      ReadProtoFromBinaryFile(
        this->param_->infogain_loss_param().source(), &blob_proto);
      infogain_.FromProto(blob_proto);
    }
  }

  template <typename Dtype>
  void InfogainLossLayer::Reshape(
    const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    LossLayer::Reshape(bottom, top);
    Blob* infogain = NULL;
    if (bottom.size() < 3) {
      infogain = &infogain_;
    } else {
      infogain = bottom[2];
    }
    CHECK_EQ(bottom[1]->channels(), 1);
    CHECK_EQ(bottom[1]->height(), 1);
    CHECK_EQ(bottom[1]->width(), 1);
    const int num = bottom[0]->num();
    const int dim = bottom[0]->count() / num;
    CHECK_EQ(infogain->num(), 1);
    CHECK_EQ(infogain->channels(), 1);
    CHECK_EQ(infogain->height(), dim);
    CHECK_EQ(infogain->width(), dim);
  }


  template <typename Dtype>
  void InfogainLossLayer::Forward(CPUContext* context, const vector<Blob*> & bottom,
      const vector<Blob*> & top)
  {
    const Dtype* bottom_data = bottom[0]->data();
    const Dtype* bottom_label = bottom[1]->data();
    const Dtype* infogain_mat = NULL;
    if (bottom.size() < 3) {
      infogain_mat = infogain_.data();
    } else {
      infogain_mat = bottom[2]->data();
    }
    int num = bottom[0]->num();
    int dim = bottom[0]->count() / bottom[0]->num();
    Dtype loss = 0;
    for (int i = 0; i < num; ++i) {
      int label = static_cast<int>(bottom_label[i]);
      for (int j = 0; j < dim; ++j) {
        Dtype prob = std::max(bottom_data[i * dim + j], Dtype(kLOG_THRESHOLD));
        loss -= infogain_mat[label * dim + j] * log(prob);
      }
    }
    top[0]->mutable_data()[0] = loss / num;
  }

  template <typename Dtype>
  void InfogainLossLayer::Backward(CPUContext* context, const vector<Blob*> & top,
      int*
      const vector<Blob*> & bottom)
  {
    if (bottom[1]->propagate_down_) {
      LOG(FATAL) << this->type()
                 << " Layer cannot backpropagate to label inputs.";
    }
    if (propagate_down.size() > 2 && propagate_down[2]) {
      LOG(FATAL) << this->type()
                 << " Layer cannot backpropagate to infogain inputs.";
    }
    if (bottom[0]->propagate_down_) {
      const Dtype* bottom_data = bottom[0]->data();
      const Dtype* bottom_label = bottom[1]->data();
      const Dtype* infogain_mat = NULL;
      if (bottom.size() < 3) {
        infogain_mat = infogain_.data();
      } else {
        infogain_mat = bottom[2]->data();
      }
      Dtype* bottom_diff = bottom[0]->mutable_diff();
      int num = bottom[0]->num();
      int dim = bottom[0]->count() / bottom[0]->num();
      const Dtype scale = - top[0]->diff()[0] / num;
      for (int i = 0; i < num; ++i) {
        const int label = static_cast<int>(bottom_label[i]);
        for (int j = 0; j < dim; ++j) {
          Dtype prob = std::max(bottom_data[i * dim + j], Dtype(kLOG_THRESHOLD));
          bottom_diff[i * dim + j] = scale * infogain_mat[label * dim + j] / prob;
        }
      }
    }
  }

  INSTANTIATE_CLASS(InfogainLossLayer);
  REGISTER_LAYER_CLASS(InfogainLoss);
}  // namespace
