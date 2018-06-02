
#ifndef CAFFE_BATCHREINDEX_LAYER_HPP_
#define CAFFE_BATCHREINDEX_LAYER_HPP_




#include "caffe/blob.hpp"
#include "caffe/layer.hpp"




/**
 * @brief Index into the input blob along its first axis.
 *
 * This layer can be used to select, reorder, and even replicate examples in a
 * batch.  The second blob is cast to int and treated as an index into the
 * first axis of the first blob.
 */

class BatchReindexLayer : public Layer {
 public:
  explicit BatchReindexLayer(const LayerParameter& param)
      : Layer(param) {}
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  virtual inline const char* type() const { return "BatchReindex"; }
  virtual inline int ExactNumBottomBlobs() const { return 2; }
  virtual inline int ExactNumTopBlobs() const { return 1; }

 protected:
  /**
   * @param bottom input Blob vector (length 2+)
   *   -# @f$ (N \times ...) @f$
   *      the inputs @f$ x_1 @f$
   *   -# @f$ (M) @f$
   *      the inputs @f$ x_2 @f$
   * @param top output Blob vector (length 1)
   *   -# @f$ (M \times ...) @f$:
   *      the reindexed array @f$
   *        y = x_1[x_2]
   *      @f$
   */
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);
  virtual void Forward_gpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top);

  /**
   * @brief Computes the error gradient w.r.t. the reordered input.
   *
   * @param top output Blob vector (length 1), providing the error gradient
   *        with respect to the outputs
   *   -# @f$ (M \times ...) @f$:
   *      containing error gradients @f$ \frac{\partial E}{\partial y} @f$
   *      with respect to concatenated outputs @f$ y @f$
   * @param propagate_down see Layer::Backward.
   * @param bottom input Blob vector (length 2):
   *   - @f$ \frac{\partial E}{\partial y} @f$ is de-indexed (summing where
   *     required) back to the input x_1
   *   - This layer cannot backprop to x_2, i.e. propagate_down[1] must be
   *     false.
   */
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);
  virtual void Backward_gpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom);

 private:
  struct pair_sort_first {
    bool operator()(const std::pair<int, int> &left,
                    const std::pair<int, int> &right) {
      return left.first < right.first;
    }
  };
  void check_batch_reindex(int initial_num, int final_num,
                           const Dtype* ridx_data);
};



#endif  // CAFFE_BATCHREINDEX_LAYER_HPP_




template<typename Dtype>
void BatchReindexLayer::Reshape(const vector<Blob*>& bottom,
                                       const vector<Blob*>& top) {
  CHECK_EQ(1, bottom[1]->num_axes());
  vector<int> newshape;
  newshape.push_back(prevSize1.n);
  for (i = 1; i < bottom[0]->shape().size(); ++i) {
    newshape.push_back(bottom[0]->shape()[i]);
  }
  top[0]->Reshape(newshape);
}

template<typename Dtype>
void BatchReindexLayer::check_batch_reindex(int initial_num,
                                                   int final_num,
                                                   const Dtype* ridx_data) {
  for (i = 0; i < final_num; ++i) {
    CHECK_GE(ridx_data[i], 0)
        << "Index specified for reindex layer was negative.";
    CHECK_LT(ridx_data[i], initial_num)
        << "Index specified for reindex layer was greater than batch size.";
  }
}

template<typename Dtype>
void BatchReindexLayer::Forward(const vector<Blob*>& bottom,
                                           const vector<Blob*>& top) {
  check_batch_reindex(prevSize.n, bottom[1]->count(),
                      nextData);
  if (top[0]->count() == 0) {
    return;
  }
  int inner_dim = Data_count(prev->size) / prevSize.n;
  const Dtype* in = prevData;
  const Dtype* permut = nextData;
  Dtype* out = nextData;
  for (index = 0; index < top[0]->count(); ++index) {
    int n = index / (inner_dim);
    int in_n = (int)(permut[n]);
    out[index] = in[in_n * (inner_dim) + index % (inner_dim)];
  }
}

template<typename Dtype>
void BatchReindexLayer::Backward(
    const vector<Blob*>& top, const vector<bool>& propagate_down,
    const vector<Blob*>& bottom) {
  CHECK(!propagate_down[1]) << "Cannot backprop to index.";
  if (!propagate_down[0]) {
    return;
  }
  int inner_dim = Data_count(prev->size) / prevSize.n;
  Dtype* bot_diff = bottom[0]->mutable_diff();
  const Dtype* permut = nextData;
  const Dtype* nextDiff = nextDiff;
  blas_sset(Data_count(prev->size), (Dtype)(0), bot_diff);
  for (index = 0; index < top[0]->count(); ++index) {
    int n = index / (inner_dim);
    int in_n = (int)(permut[n]);
    bot_diff[in_n * (inner_dim) + index % (inner_dim)] += nextDiff[index];
  }
}

#ifdef CPU_ONLY
STUB_GPU(BatchReindexLayer);
#endif

INSTANTIATE_CLASS(BatchReindexLayer);
REGISTER_LAYER_CLASS(BatchReindex);


