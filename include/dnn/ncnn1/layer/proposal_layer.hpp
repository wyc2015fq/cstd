#ifndef CAFFE_PROPOSAL_LAYERS_HPP_
#define CAFFE_PROPOSAL_LAYERS_HPP_



#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layer.hpp"


#include "loss_layer.hpp"


  
  class ProposalLayer : public Layer {
  public:
    explicit ProposalLayer(const LayerParameter& param)
      : Layer(param) {}
    virtual void LayerSetUp(const vector<Blob*>& bottom,
                            const vector<Blob*>& top);
    virtual void Reshape(const vector<Blob*>& bottom,
                         const vector<Blob*>& top) {
      //LOG(FATAL) << "Reshaping happens during the call to forward.";
    }

    virtual inline const char* type() const { return "ProposalLayer"; }

  protected:
    virtual void Forward_cpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Forward_gpu(const vector<Blob*>& bottom,
                             const vector<Blob*>& top);
    virtual void Backward_cpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);
    virtual void Backward_gpu(const vector<Blob*>& top,
                              const vector<bool>& propagate_down, const vector<Blob*>& bottom);

    int base_size_;
    int feat_stride_;
    int pre_nms_topn_;
    int post_nms_topn_;
    Dtype nms_thresh_;
    int min_size_;
    Blob anchors_;
    Blob proposals_;
    Blob<int> roi_indices_;
    Blob<int> nms_mask_;
  };



#endif  // CAFFE_PROPOSAL_LAYERS_HPP_
