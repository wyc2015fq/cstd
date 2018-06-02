#ifndef CAFFE_PREDICT_BOX_LAYER_HPP_
#define CAFFE_PREDICT_BOX_LAYER_HPP_




#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Generate the detection output based on bounding box regression and
 * confidence score.
 *
 * Intended for use with MultiBox detection method.
 *
 * NOTE: does not implement Backwards operation.
 */

class PredictBoxLayer : public Layer {
 public:
   explicit PredictBoxLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "PredictBox"; }

 protected:
  /**
   * @brief Get the bounding box output.
   *
   * @param bottom input Blob vector (exact 2)
   *   -# @f$ (1 \times 1 \times N \times 7) @f$
   *      N detection results.
   *   -# @f$ (1 \times 1 \times M \times 7) @f$
   *      M ground truth.
   * @param top Blob vector (length 1)
   *   -# @f$ (1 \times 1 \times N \times 4) @f$
   *      N is the number of detections, and each row is:
   *      [image_id, label, confidence, true_pos, false_pos]
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
                           const vector<Blob*>& top);
  /// @brief Not implemented
  virtual void Backward_cpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
                            const vector<bool>& propagate_down, const vector<Blob*>& bottom);

  int receptive_field_;
  int stride_;
  bool nms_;
  bool bounding_box_regression_;
  bool output_vector_;
  Dtype positive_thresh_;
  bool bounding_box_exp_;
  Blob counter_;
  bool use_stitch_;
};



#endif  // CAFFE_PREDICT_BOX_LAYER_HPP_
