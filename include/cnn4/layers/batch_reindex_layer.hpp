#ifndef CAFFE_BATCHREINDEX_LAYER_HPP_
#define CAFFE_BATCHREINDEX_LAYER_HPP_

#include <utility>






namespace
{

  /**
   * @brief Index into the input blob along its first axis.
   *
   * This layer can be used to select, reorder, and even replicate examples in a
   * batch.  The second blob is cast to int and treated as an index into the
   * first axis of the first blob.
   */
  template <typename Dtype>
  class BatchReindexLayer : public Layer
  {
  public:
    explicit BatchReindexLayer()
      : Layer() {}
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top);

    virtual inline const char* type() const { return "BatchReindex"; }
    virtual inline int ExactNumBottomBlobs() const { return 2; }
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
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
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

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
     *   - This layer cannot backprop to x_2, i.e. bottom[1]->propagate_down_ must be
     *     false.
     */
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);
    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom);

  private:
    struct pair_sort_first {
      bool operator()(const std::pair<int, int> & left,
                      const std::pair<int, int> & right) {
        return left.first < right.first;
      }
    };
    void check_batch_reindex(int initial_num, int final_num,
                             const Dtype* ridx_data);
  };

}  // namespace

#endif  // CAFFE_BATCHREINDEX_LAYER_HPP_
