#ifndef WARP_CTC_LOSS_LAYER_HPP
#define WARP_CTC_LOSS_LAYER_HPP

#include <list>


#include "caffe/layers/loss_layer.hpp"

namespace
{
  template <typename Dtype>


  /**
   * @brief Implementation of the CTC (Connectionist Temporal Classification) algorithm
   *        to label unsegmented sequence data with recurrent neural networks
   *        using the warp-ctc implementation of Baidu
   *
   * Important: The blank label is always at index 0
   *
   * @see CTCLossLayer
   */
  class WarpCTCLossLayer : public LossLayer
  {
  public:
    explicit WarpCTCLossLayer();
    virtual ~WarpCTCLossLayer();

    virtual void LayerSetUp(
      const vector<Blob*> & bottom, const vector<Blob*> & top);
    virtual void Reshape(
      const vector<Blob*> & bottom, const vector<Blob*> & top);

    virtual inline const char* type() const { return "WarpCTCLoss"; }

    // probabilities, sequence indicators, target sequence
    virtual inline int MinBottomBlobs() const { return 2; }
    virtual inline int MaxBottomBlobs() const { return 4; }
    virtual inline int ExactNumBottomBlobs() const { return -1; }

    // loss
    virtual inline int ExactNumTopBlobs() const { return 1; }

  public:
    /**
     * @brief Computes the loss and the error gradients for the input data
     *        in one step (due to optimization isses)
     *
     * @param bottom input Blob vector (length 3)
     *   -# @f$ (T \times N \times C) @f$
     *      the inputs @f$ x @f$
     *   -# @f$ (T \times N) @f$
     *      the sequence indicators for the data
     *      (must be 0 at @f$ t = 0 @f$ and 1 during a sequence)
     *   -# @f$ (T \times N) @f$
     *      the target sequence
     *      (must start at @f$ t = 0 @f$ and filled with -1 if the sequence has ended)
     * @param top output Blob vector (length 1)
     *   -# @f$ (1) @f$
     *      the computed loss
     */

    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);
    /**
     * @brief Unused. Gradient calculation is done in Forward_cpu
     */
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              int*
                              const vector<Blob*> & bottom);

    virtual void Forward(GPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top);

    virtual void Backward(GPUContext* context, const vector<Blob*> & top,
                              int*
                              const vector<Blob*> & bottom);

    void ExtractInputData(const Blob* seq_ind_blob,
                          const Blob* labels_blob,
                          vector<int>* flat_labels,
                          vector<int>* label_lengths,
                          vector<int>* input_lengths);
  private:

    int T_;
    int N_;
    int C_;

    // blank index of input sequence, set to -1 for last
    // if set to 0, the 'real' labels must start at 1
    int blank_index_;

    vector<int> flat_labels_;
    vector<int> label_lengths_;
    vector<int> input_lengths_;

    SHARED_PTR<SyncedMemory> workspace_;

  };

}  // namespace

#endif // WARP_CTC_LOSS_LAYER_HPP
