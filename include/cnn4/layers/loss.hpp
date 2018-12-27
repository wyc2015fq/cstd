#ifndef CAFFE_LOSS_LAYER_HPP_
#define CAFFE_LOSS_LAYER_HPP_


const double kLOG_THRESHOLD = 1e-20;

/**
 * @brief An interface for Layer%s that take two Blob%s as input -- usually
 *        (1) predictions and (2) ground-truth labels -- and output a
 *        singleton Blob representing the loss.
 *
 * LossLayers are typically only capable of backpropagating to their first input
 * -- the predictions.
 */

#define LossParameter_DEF(DEF) \
DEF##Int(ignore_label, -1, 0) \
DEF##Enum(normalization, NormalizationMode_VALID, NormalizationMode) \

class LossLayer : public Layer
{
public:
  LossParameter_DEF(Def);
  virtual inline int ExactNumBottomBlobs() const { return 2; }

  virtual void init() {
    LossParameter_DEF(Init);
  }
  virtual void fromJson(cjson* param) {
    LossParameter_DEF(Get);
  }
  virtual void toJson(cjson* param) {
    LossParameter_DEF(Set);
  }
  /**
   * @brief For convenience and backwards compatibility, instruct the Net to
   *        automatically allocate a single top Blob for LossLayers, into which
   *        they output their singleton loss, (even if the user didn't specify
   *        one in the prototxt, etc.).
   */
  virtual inline bool AutoTopBlobs() const { return true; }
  virtual inline int ExactNumTopBlobs() const { return 1; }
  /**
   * We usually cannot backpropagate to the labels; ignore force_backward for
   * these inputs.
   */
  virtual inline bool AllowForceBackward(const int bottom_index) const {
    return bottom_index != 1;
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    // LossLayers have a non-zero (1) loss by default.
    //if (this->param_->loss_weight_size() == 0) {
      //this->param_->add_loss_weight(Dtype(1));
    //}
    top[0]->loss_weight_ = 1;
  }

  virtual void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    CHECK_EQ(bottom[0]->num(), bottom[1]->num())
      << "The data and label should have the same number.";
    DataShape loss_shape = dataShape(1);  // Loss layers output a scalar; 0 axes.
    top[0]->Reshape(loss_shape);
  }


};


#endif  // CAFFE_LOSS_LAYER_HPP_
