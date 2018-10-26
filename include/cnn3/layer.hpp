#ifndef CAFFE_LAYER_H_
#define CAFFE_LAYER_H_

#include <algorithm>
#include <string>
#include <vector>
#include <mutex>

#include "blob.hpp"
#include "common.hpp"
#include "proto.h"
#include "math_functions.hpp"

struct Layer
{
  bool is_shared_;
  std::mutex forward_mutex_;
  CJSON* param_;
  Phase phase_;
  vector<Blob* > top_;
  vector<Blob* > bottom_;
  vector<Blob* > blobs_;
  vector<bool> param_propagate_down_;
  vector<Dtype> loss_;
  /** Lock forward_mutex_ if this layer is shared */
  void Lock()
  {
    if (is_shared_) {
      forward_mutex_.lock();
    }
  }
  /** Unlock forward_mutex_ if this layer is shared */
  void Unlock()
  {
    if (is_shared_) {
      forward_mutex_.unlock();
    }
  }
  void init(CJSON* param) {
    param_ = (param);
    is_shared_ = (false);
    // Set phase and copy blobs (if there are any).
    phase_ = CJSON_GETOBJECTENUM(param, "phase", TEST, Phase);
    CJSON* blobs_json = param->GetObjectItem("blobs");
    int blobs_size = blobs_json->GetArraySize();
    if (blobs_size > 0) {
      blobs_.resize(blobs_size);
      for (int i = 0; i < blobs_size; ++i) {
        blobs_[i]->FromProto(blobs_json->GetArrayItem(i));
      }
    }
  }
};

#if 0


    void SetUp(const vector<Blob<Dtype>*> & bottom,
               const vector<Blob<Dtype>*> & top) {
      InitMutex();
      CheckBlobCounts(bottom, top);
      LayerSetUp(bottom, top);
      Reshape(bottom, top);
      SetLossWeights(top);
    }

    /**
     * @brief Does layer-specific setup: your layer should implement this function
     *        as well as Reshape.
     *
     * @param bottom
     *     the preshaped input blobs, whose data fields store the input data for
     *     this layer
     * @param top
     *     the allocated but unshaped output blobs
     *
     * This method should do one-time layer specific setup. This includes reading
     * and processing relevent parameters from the <code>param_</code>.
     * Setting up the shapes of top blobs and internal buffers should be done in
     * <code>Reshape</code>, which will be called before the forward pass to
     * adjust the top blob sizes.
     */
    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top) {}

    /**
     * @brief Whether a layer should be shared by multiple nets during data
     *        parallelism. By default, all layers except for data layers should
     *        not be shared. data layers should be shared to ensure each worker
     *        solver access data sequentially during data parallelism.
     */
    virtual inline bool ShareInParallel() const { return false; }

    /** @brief Return whether this layer is actually shared by other nets.
     *         If ShareInParallel() is true and using more than one GPU and the
     *         net has TRAIN phase, then this function is expected return true.
     */
    inline bool IsShared() const { return is_shared_; }

    /** @brief Set whether this layer is actually shared by other nets
     *         If ShareInParallel() is true and using more than one GPU and the
     *         net has TRAIN phase, then is_shared should be set true.
     */
    inline void SetShared(bool is_shared) {
      CHECK(ShareInParallel() || !is_shared)
          << type() << "Layer does not support sharing.";
      is_shared_ = is_shared;
    }

    /**
     * @brief Adjust the shapes of top blobs and internal buffers to accommodate
     *        the shapes of the bottom blobs.
     *
     * @param bottom the input blobs, with the requested input shapes
     * @param top the top blobs, which should be reshaped as needed
     *
     * This method should reshape top blobs as needed according to the shapes
     * of the bottom (input) blobs, as well as reshaping any internal buffers
     * and making any other necessary adjustments so that the layer can
     * accommodate the bottom blobs.
     */
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top) {}

    /**
     * @brief Given the bottom blobs, compute the top blobs and the loss.
     *
     * @param bottom
     *     the input blobs, whose data fields store the input data for this layer
     * @param top
     *     the preshaped output blobs, whose data fields will store this layers'
     *     outputs
     * \return The total loss from the layer.
     *
     * The Forward wrapper calls the relevant device wrapper function
     * (Forward_cpu or Forward_gpu) to compute the top blob values given the
     * bottom blobs.  If the layer has any non-zero loss_weights, the wrapper
     * then computes and returns the loss.
     *
     * Your layer should implement Forward_cpu and (optionally) Forward_gpu.
     */
    inline Dtype Forward(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top)
    {
      // Lock during forward to ensure sequential forward
      Lock();
      Dtype loss = 0;
      Reshape(bottom, top);
      switch (mode()) {
      case CPU: {
        Forward_cpu(bottom, top);
#if 0
        if (bottom.size() > 0 && top.size() > 0) {
          Dtype* bottom_data = bottom[0]->mutable_cpu_data();
          Dtype* top_data = top[0]->mutable_cpu_data();
          printf("############## %s ###########\n", this->layer_param().name().c_str());
          for (int i = 0; i < 10; i++) {
            printf("%f ", bottom_data[i]);
          }
          printf("\n");
          for (int i = 0; i < 10; i++) {
            printf("%f ", top_data[i]);
          }
          printf("\n");
        }
#endif
        for (int top_id = 0; top_id < top.size(); ++top_id) {
          if (!this->loss(top_id)) { continue; }
          const int count = top[top_id]->count();
          const Dtype* data = top[top_id]->cpu_data();
          const Dtype* loss_weights = top[top_id]->cpu_diff();
          loss += caffe_cpu_dot(count, data, loss_weights);
        }
      }
                       break;
      case GPU:
        Forward_gpu(bottom, top);
#ifndef CPU_ONLY
        for (int top_id = 0; top_id < top.size(); ++top_id) {
          if (!this->loss(top_id)) { continue; }
          const int count = top[top_id]->count();
          const Dtype* data = top[top_id]->gpu_data();
          const Dtype* loss_weights = top[top_id]->gpu_diff();
          Dtype blob_loss = 0;
          caffe_gpu_dot(count, data, loss_weights, &blob_loss);
          loss += blob_loss;
        }
#endif
        break;
      default:
        LOG(FATAL) << "Unknown caffe mode.";
      }
      Unlock();
      return loss;
    }

    /**
     * @brief Given the top blob error gradients, compute the bottom blob error
     *        gradients.
     *
     * @param top
     *     the output blobs, whose diff fields store the gradient of the error
     *     with respect to themselves
     * @param propagate_down
     *     a vector with equal length to bottom, with each index indicating
     *     whether to propagate the error gradients down to the bottom blob at
     *     the corresponding index
     * @param bottom
     *     the input blobs, whose diff fields will store the gradient of the error
     *     with respect to themselves after Backward is run
     *
     * The Backward wrapper calls the relevant device wrapper function
     * (Backward_cpu or Backward_gpu) to compute the bottom blob diffs given the
     * top blob diffs.
     *
     * Your layer should implement Backward_cpu and (optionally) Backward_gpu.
     */
    inline void Backward(const vector<Blob<Dtype>*> & top,
                         const vector<bool> & propagate_down,
                         const vector<Blob<Dtype>*> & bottom)
    {
      switch (mode()) {
      case CPU:
        Backward_cpu(top, propagate_down, bottom);
        break;
      case GPU:
        Backward_gpu(top, propagate_down, bottom);
        break;
      default:
        LOG(FATAL) << "Unknown caffe mode.";
      }
    }


    /**
     * @brief Returns the vector of learnable parameter blobs.
     */
    vector<Blob<Dtype>* > & blobs() {
      return blobs_;
    }

    /**
     * @brief Returns the layer parameter.
     */
    CJSON* layer_param() const { return param_; }

    /**
     * @brief Writes the layer parameter to a protocol buffer
     */
    virtual void ToProto(CJSON* param, bool write_diff = false)
    {
      param->Clear();
      param->CopyFrom(param_);
      param->clear_blobs();
      for (int i = 0; i < blobs_.size(); ++i) {
        blobs_[i]->ToProto(param->add_blobs(), write_diff);
      }
    }

    /**
     * @brief Returns the scalar loss associated with a top blob at a given index.
     */
    inline Dtype loss(const int top_index) const {
      return (loss_.size() > top_index) ? loss_[top_index] : Dtype(0);
    }

    /**
     * @brief Sets the loss associated with a top blob at a given index.
     */
    inline void set_loss(const int top_index, const Dtype value) {
      if (loss_.size() <= top_index) {
        loss_.resize(top_index + 1, Dtype(0));
      }
      loss_[top_index] = value;
    }

    /**
     * @brief Returns the layer type.
     */
    virtual inline const char* type() const { return ""; }

    /**
     * @brief Returns the exact number of bottom blobs required by the layer,
     *        or -1 if no exact number is required.
     *
     * This method should be overridden to return a non-negative value if your
     * layer expects some exact number of bottom blobs.
     */
    virtual inline int ExactNumBottomBlobs() const { return -1; }
    /**
     * @brief Returns the minimum number of bottom blobs required by the layer,
     *        or -1 if no minimum number is required.
     *
     * This method should be overridden to return a non-negative value if your
     * layer expects some minimum number of bottom blobs.
     */
    virtual inline int MinBottomBlobs() const { return -1; }
    /**
     * @brief Returns the maximum number of bottom blobs required by the layer,
     *        or -1 if no maximum number is required.
     *
     * This method should be overridden to return a non-negative value if your
     * layer expects some maximum number of bottom blobs.
     */
    virtual inline int MaxBottomBlobs() const { return -1; }
    /**
     * @brief Returns the exact number of top blobs required by the layer,
     *        or -1 if no exact number is required.
     *
     * This method should be overridden to return a non-negative value if your
     * layer expects some exact number of top blobs.
     */
    virtual inline int ExactNumTopBlobs() const { return -1; }
    /**
     * @brief Returns the minimum number of top blobs required by the layer,
     *        or -1 if no minimum number is required.
     *
     * This method should be overridden to return a non-negative value if your
     * layer expects some minimum number of top blobs.
     */
    virtual inline int MinTopBlobs() const { return -1; }
    /**
     * @brief Returns the maximum number of top blobs required by the layer,
     *        or -1 if no maximum number is required.
     *
     * This method should be overridden to return a non-negative value if your
     * layer expects some maximum number of top blobs.
     */
    virtual inline int MaxTopBlobs() const { return -1; }
    /**
     * @brief Returns true if the layer requires an equal number of bottom and
     *        top blobs.
     *
     * This method should be overridden to return true if your layer expects an
     * equal number of bottom and top blobs.
     */
    virtual inline bool EqualNumBottomTopBlobs() const { return false; }

    /**
     * @brief Return whether "anonymous" top blobs are created automatically
     *        by the layer.
     *
     * If this method returns true, Net::Init will create enough "anonymous" top
     * blobs to fulfill the requirement specified by ExactNumTopBlobs() or
     * MinTopBlobs().
     */
    virtual inline bool AutoTopBlobs() const { return false; }

    /**
     * @brief Return whether to allow force_backward for a given bottom blob
     *        index.
     *
     * If AllowForceBackward(i) == false, we will ignore the force_backward
     * setting and backpropagate to blob i only if it needs gradient information
     * (as is done when force_backward == false).
     */
    virtual inline bool AllowForceBackward(const int bottom_index) const {
      return true;
    }

    /**
     * @brief Specifies whether the layer should compute gradients w.r.t. a
     *        parameter at a particular index given by param_id.
     *
     * You can safely ignore false values and always compute gradients
     * for all parameters, but possibly with wasteful computation.
     */
    inline bool param_propagate_down(const int param_id) {
      return (param_propagate_down_.size() > param_id) ?
             param_propagate_down_[param_id] : false;
    }
    /**
     * @brief Sets whether the layer should compute gradients w.r.t. a
     *        parameter at a particular index given by param_id.
     */
    inline void set_param_propagate_down(const int param_id, const bool value) {
      if (param_propagate_down_.size() <= param_id) {
        param_propagate_down_.resize(param_id + 1, true);
      }
      param_propagate_down_[param_id] = value;
    }



    /** @brief Using the CPU device, compute the layer output. */
    virtual void Forward_cpu(const vector<Blob<Dtype>*> & bottom,
      const vector<Blob<Dtype>*> & top) = 0;
    /**
     * @brief Using the GPU device, compute the layer output.
     *        Fall back to Forward_cpu() if unavailable.
     */
    virtual void Forward_gpu(const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top) {
      // LOG(WARNING) << "Using CPU code as backup.";
      return Forward_cpu(bottom, top);
    }

    /**
     * @brief Using the CPU device, compute the gradients for any parameters and
     *        for the bottom blobs if propagate_down is true.
     */
    virtual void Backward_cpu(const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down,
                              const vector<Blob<Dtype>*> & bottom) {}
    /**
     * @brief Using the GPU device, compute the gradients for any parameters and
     *        for the bottom blobs if propagate_down is true.
     *        Fall back to Backward_cpu() if unavailable.
     */
    virtual void Backward_gpu(const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down,
                              const vector<Blob<Dtype>*> & bottom) {
      // LOG(WARNING) << "Using CPU code as backup.";
      Backward_cpu(top, propagate_down, bottom);
    }

    /**
     * Called by the parent Layer's SetUp to check that the number of bottom
     * and top Blobs provided as input match the expected numbers specified by
     * the {ExactNum,Min,Max}{Bottom,Top}Blobs() functions.
     */
    virtual void CheckBlobCounts(const vector<Blob<Dtype>*> & bottom,
                                 const vector<Blob<Dtype>*> & top) {
      if (ExactNumBottomBlobs() >= 0) {
        CHECK_EQ(ExactNumBottomBlobs(), bottom.size())
            << type() << " Layer takes " << ExactNumBottomBlobs()
            << " bottom blob(s) as input.";
      }
      if (MinBottomBlobs() >= 0) {
        CHECK_LE(MinBottomBlobs(), bottom.size())
            << type() << " Layer takes at least " << MinBottomBlobs()
            << " bottom blob(s) as input.";
      }
      if (MaxBottomBlobs() >= 0) {
        CHECK_GE(MaxBottomBlobs(), bottom.size())
            << type() << " Layer takes at most " << MaxBottomBlobs()
            << " bottom blob(s) as input.";
      }
      if (ExactNumTopBlobs() >= 0) {
        CHECK_EQ(ExactNumTopBlobs(), top.size())
            << type() << " Layer produces " << ExactNumTopBlobs()
            << " top blob(s) as output.";
      }
      if (MinTopBlobs() >= 0) {
        CHECK_LE(MinTopBlobs(), top.size())
            << type() << " Layer produces at least " << MinTopBlobs()
            << " top blob(s) as output.";
      }
      if (MaxTopBlobs() >= 0) {
        CHECK_GE(MaxTopBlobs(), top.size())
            << type() << " Layer produces at most " << MaxTopBlobs()
            << " top blob(s) as output.";
      }
      if (EqualNumBottomTopBlobs()) {
        CHECK_EQ(bottom.size(), top.size())
            << type() << " Layer produces one top blob as output for each "
            << "bottom blob input.";
      }
    }

    /**
     * Called by SetUp to initialize the weights associated with any top blobs in
     * the loss function. Store non-zero loss weights in the diff blob.
     */
    inline void SetLossWeights(const vector<Blob<Dtype>*> & top) {
      const int num_loss_weights = param_.loss_weight_size();
      if (num_loss_weights) {
        CHECK_EQ(top.size(), num_loss_weights) << "loss_weight must be "
                                               "unspecified or specified once per top blob.";
        for (int top_id = 0; top_id < top.size(); ++top_id) {
          const Dtype loss_weight = param_.loss_weight(top_id);
          if (loss_weight == Dtype(0)) { continue; }
          this->set_loss(top_id, loss_weight);
          const int count = top[top_id]->count();
          Dtype* loss_multiplier = top[top_id]->mutable_cpu_diff();
          caffe_set(count, loss_weight, loss_multiplier);
        }
      }
    }

#endif




#endif  // CAFFE_LAYER_H_
