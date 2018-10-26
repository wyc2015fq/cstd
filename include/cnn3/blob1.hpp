#ifndef CAFFE_BLOB_HPP_
#define CAFFE_BLOB_HPP_

#include <algorithm>
#include <string>
#include <vector>
#include "proto.h"
#include "common.hpp"
#include "syncedmem.hpp"

const int kMaxBlobAxes = 32;

//typedef float Dtype;

/**
 * @brief A wrapper around SyncedMemory holders serving as the basic
 *        computational unit through which Layer%s, Net%s, and Solver%s
 *        interact.
 *
 * TODO(dox): more thorough description.
 */

class Blob
{
public:
  Blob()
    : data_(), diff_(), count_(0), capacity_(0) {}

  /// @brief Deprecated; use <code>Blob(const vector<int>& shape)</code>.
  explicit Blob(const int num, const int channels, const int height,
    const int width)
    // capacity_ must be initialized before calling Reshape
    : capacity_(0)
  {
    Reshape(num, channels, height, width);
  }
  explicit Blob(const vector<int> & shape)
    : capacity_(0)
  {
    Reshape(shape);
  }

  /// @brief Deprecated; use <code>Reshape(const vector<int>& shape)</code>.
  void Reshape(const int num, const int channels, const int height,
    const int width)
  {
    vector<int> shape(4);
    shape[0] = num;
    shape[1] = channels;
    shape[2] = height;
    shape[3] = width;
    Reshape(shape);
  }
  /**
   * @brief Change the dimensions of the blob, allocating new memory if
   *        necessary.
   *
   * This function can be called both to create an initial allocation
   * of memory, and to adjust the dimensions of a top blob during Layer::Reshape
   * or Layer::Forward. When changing the size of blob, memory will only be
   * reallocated if sufficient memory does not already exist, and excess memory
   * will never be freed.
   *
   * Note that reshaping an input blob and immediately calling Net::Backward is
   * an error; either Net::Forward or Net::Reshape need to be called to
   * propagate the new input shape to higher layers.
   */
  void Reshape(const vector<int> & shape)
  {
    CHECK_LE(shape.size(), kMaxBlobAxes);
    count_ = 1;
    shape_.resize(shape.size());
    if (shape_data_.size() < shape.size() * sizeof(int)) {
      shape_data_.reset(shape.size() * sizeof(int));
    }
    int* shape_data = static_cast<int*>(shape_data_.mutable_cpu_data());
    for (int i = 0; i < shape.size(); ++i) {
      CHECK_GE(shape[i], 0);
      if (count_ != 0) {
        CHECK_LE(shape[i], INT_MAX / count_) << "blob size exceeds INT_MAX";
      }
      count_ *= shape[i];
      shape_[i] = shape[i];
      shape_data[i] = shape[i];
    }
    if (count_ > capacity_) {
      capacity_ = count_;
      data_.reset((capacity_ * sizeof(Dtype)));
      diff_.reset((capacity_ * sizeof(Dtype)));
    }
  }
#ifdef USE_JSON
  void Reshape(CJSON* shape)
  {
    int dim_size = shape->GetArraySize();
    CHECK_LE(dim_size, kMaxBlobAxes);
    vector<int> shape_vec(dim_size);
    cJSON_GetNumberArray(shape, shape_vec.data(), dim_size);
    Reshape(shape_vec);
  }
  void ToProto(cJSON* proto, bool write_diff = false)
  {
    cJSON_AddNumberArrayToObject(proto, "shape", &shape_[0], shape_.size());
    cJSON_AddBinaryDataToObject(proto, "data", cpu_data(), count_ * sizeof(Dtype));
    if (write_diff) {
      cJSON_AddBinaryDataToObject(proto, "diff", cpu_diff(), count_ * sizeof(Dtype));
    }
  }
  void FromProto(CJSON* proto, bool reshape = true) {
#if 0
    if (reshape) {
      vector<int> shape;
      if (proto.has_num() || proto.has_channels() ||
        proto.has_height() || proto.has_width()) {
        // Using deprecated 4D Blob dimensions --
        // shape is (num, channels, height, width).
        shape.resize(4);
        shape[0] = proto.num();
        shape[1] = proto.channels();
        shape[2] = proto.height();
        shape[3] = proto.width();
      }
      else {
        shape.resize(proto.shape().dim_size());
        for (int i = 0; i < proto.shape().dim_size(); ++i) {
          shape[i] = (int)proto.shape().dim(i);
        }
      }
      Reshape(shape);
    }
    else {
      CHECK(ShapeEquals(proto)) << "shape mismatch (reshape not set)";
    }
    // copy data
    Dtype* data_vec = mutable_cpu_data();
    if (proto.double_data_size() > 0) {
      CHECK_EQ(count_, proto.double_data_size());
      for (int i = 0; i < count_; ++i) {
        data_vec[i] = (Dtype)proto.double_data(i);
      }
    }
    else {
      CHECK_EQ(count_, proto.data_size());
      for (int i = 0; i < count_; ++i) {
        data_vec[i] = (Dtype)proto.data(i);
      }
    }
    if (proto.double_diff_size() > 0) {
      CHECK_EQ(count_, proto.double_diff_size());
      Dtype* diff_vec = mutable_cpu_diff();
      for (int i = 0; i < count_; ++i) {
        diff_vec[i] = (Dtype)proto.double_diff(i);
      }
    }
    else if (proto.diff_size() > 0) {
      CHECK_EQ(count_, proto.diff_size());
      Dtype* diff_vec = mutable_cpu_diff();
      for (int i = 0; i < count_; ++i) {
        diff_vec[i] = (Dtype)proto.diff(i);
      }
    }
#endif
  }
#endif

  void ReshapeLike(const Blob & other) {
    Reshape(other.shape());
  }
  inline string shape_string() const {
    ostringstream stream;
    for (int i = 0; i < shape_.size(); ++i) {
      stream << shape_[i] << " ";
    }
    stream << "(" << count_ << ")";
    return stream.str();
  }
  inline const vector<int> & shape() const { return shape_; }
  /**
   * @brief Returns the dimension of the index-th axis (or the negative index-th
   *        axis from the end, if index is negative).
   *
   * @param index the axis index, which may be negative as it will be
   *        "canonicalized" using CanonicalAxisIndex.
   *        Dies on out of range index.
   */
  inline int shape(int index) const {
    return shape_[CanonicalAxisIndex(index)];
  }
  inline int num_axes() const { return (int)shape_.size(); }
  inline int count() const { return count_; }
  inline int countL(int i) const { return count(0, i); }
  inline int countH(int i) const { return count(i, 4); }

  /**
   * @brief Compute the volume of a slice; i.e., the product of dimensions
   *        among a range of axes.
   *
   * @param start_axis The first axis to include in the slice.
   *
   * @param end_axis The first axis to exclude from the slice.
   */
  inline int count(int start_axis, int end_axis) const {
    if (end_axis > num_axes()) {
      int asdf = 0;
    }
    start_axis = min(start_axis, num_axes());
    end_axis = min(end_axis, num_axes());
    CHECK_LE(start_axis, end_axis);
    CHECK_GE(start_axis, 0);
    CHECK_GE(end_axis, 0);
    CHECK_LE(start_axis, num_axes());
    CHECK_LE(end_axis, num_axes());
    int count = 1;
    for (int i = start_axis; i < end_axis; ++i) {
      count *= shape(i);
    }
    return count;
  }
  /**
   * @brief Compute the volume of a slice spanning from a particular first
   *        axis to the final axis.
   *
   * @param start_axis The first axis to include in the slice.
   */
  inline int count(int start_axis) const {
    return count(start_axis, num_axes());
  }

  /**
   * @brief Returns the 'canonical' version of a (usually) user-specified axis,
   *        allowing for negative indexing (e.g., -1 for the last axis).
   *
   * @param axis_index the axis index.
   *        If 0 <= index < num_axes(), return index.
   *        If -num_axes <= index <= -1, return (num_axes() - (-index)),
   *        e.g., the last axis index (num_axes() - 1) if index == -1,
   *        the second to last if index == -2, etc.
   *        Dies on out of range index.
   */
  inline int CanonicalAxisIndex(int axis_index) const {
    CHECK_GE(axis_index, -num_axes())
      << "axis " << axis_index << " out of range for " << num_axes()
      << "-D Blob with shape " << shape_string();
    CHECK_LT(axis_index, num_axes())
      << "axis " << axis_index << " out of range for " << num_axes()
      << "-D Blob with shape " << shape_string();
    if (axis_index < 0) {
      return axis_index + num_axes();
    }
    return axis_index;
  }

  /// @brief Deprecated legacy shape accessor num: use shape(0) instead.
  inline int num() const { return LegacyShape(0); }
  /// @brief Deprecated legacy shape accessor channels: use shape(1) instead.
  inline int channels() const { return LegacyShape(1); }
  /// @brief Deprecated legacy shape accessor height: use shape(2) instead.
  inline int height() const { return LegacyShape(2); }
  /// @brief Deprecated legacy shape accessor width: use shape(3) instead.
  inline int width() const { return LegacyShape(3); }
  inline int LegacyShape(int index) const {
    CHECK_LE(num_axes(), 4)
      << "Cannot use legacy accessors on Blobs with > 4 axes.";
    CHECK_LT(index, 4);
    CHECK_GE(index, -4);
    if (index >= num_axes() || index < -num_axes()) {
      // Axis is out of range, but still in [0, 3] (or [-4, -1] for reverse
      // indexing) -- this special case simulates the one-padding used to fill
      // extraneous axes of legacy blobs.
      return 1;
    }
    return shape(index);
  }

  inline int offset(const int n, const int c = 0, const int h = 0,
    const int w = 0) const {
    CHECK_GE(n, 0);
    CHECK_LE(n, num());
    CHECK_GE(channels(), 0);
    CHECK_LE(c, channels());
    CHECK_GE(height(), 0);
    CHECK_LE(h, height());
    CHECK_GE(width(), 0);
    CHECK_LE(w, width());
    return ((n * channels() + c) * height() + h) * width() + w;
  }

  inline int offset(const vector<int> & indices) const {
    CHECK_LE(indices.size(), num_axes());
    int offset = 0;
    for (int i = 0; i < num_axes(); ++i) {
      offset *= shape(i);
      if (indices.size() > i) {
        CHECK_GE(indices[i], 0);
        CHECK_LT(indices[i], shape(i));
        offset += indices[i];
      }
    }
    return offset;
  }
  /**
   * @brief Copy from a source Blob.
   *
   * @param source the Blob to copy from
   * @param copy_diff if false, copy the data; if true, copy the diff
   * @param reshape if false, require this Blob to be pre-shaped to the shape
   *        of other (and die otherwise); if true, Reshape this Blob to other's
   *        shape if necessary
   */
  void CopyFrom(Blob& source, bool copy_diff = false, bool reshape = false)
  {
    if (source.count() != count_ || source.shape() != shape_) {
      if (reshape) {
        ReshapeLike(source);
      }
      else {
        LOG(FATAL) << "Trying to copy blobs of different sizes.";
      }
    }
    switch (mode()) {
    case GPU:
      if (copy_diff) {
        caffe_copy(count_, source.gpu_diff(),
          static_cast<Dtype*>(diff_.mutable_gpu_data()));
      }
      else {
        caffe_copy(count_, source.gpu_data(),
          static_cast<Dtype*>(data_.mutable_gpu_data()));
      }
      break;
    case CPU:
      if (copy_diff) {
        caffe_copy(count_, source.cpu_diff(),
          static_cast<Dtype*>(diff_.mutable_cpu_data()));
      }
      else {
        caffe_copy(count_, source.cpu_data(),
          static_cast<Dtype*>(data_.mutable_cpu_data()));
      }
      break;
    default:
      LOG(FATAL) << "Unknown caffe mode.";
    }
  }
#if 0
  inline Dtype data_at(const int n, const int c, const int h,  const int w) const {    return cpu_data()[offset(n, c, h, w)];  }

  inline Dtype diff_at(const int n, const int c, const int h, const int w) const {  return cpu_diff()[offset(n, c, h, w)];  }
  inline Dtype data_at(const vector<int> & index) const {    return cpu_data()[offset(index)];  }
  inline Dtype diff_at(const vector<int> & index) const {    return cpu_diff()[offset(index)];  }
  inline SyncedMemory& data() { return data_; }
  inline SyncedMemory& diff() { return diff_; }
  template <typename Dtype>
  const Dtype* cpu_data() { return (const Dtype*)data_.cpu_data(); }
  template <typename Dtype>
  void set_cpu_data(Dtype* data)
  {
    CHECK(data);
    // Make sure CPU and GPU sizes remain equal
    size_t size = count_ * sizeof(Dtype);
    if (data_.size() != size) {
      data_.reset((size));
      diff_.reset((size));
    }
    data_.set_cpu_data(data);
  }
  const int* gpu_shape() const
  {
    return (const int*)shape_data_.gpu_data();
  }
  const Dtype* gpu_data() const
  {
    return (const Dtype*)data_.gpu_data();
  }
  void set_gpu_data(Dtype* data)
  {
    CHECK(data);
    // Make sure CPU and GPU sizes remain equal
    size_t size = count_ * sizeof(Dtype);
    if (data_.size() != size) {
      data_.reset((size));
      diff_.reset((size));
    }
    data_.set_gpu_data(data);
  }
  const Dtype* cpu_diff()  {    return (const Dtype*)diff_.cpu_data();  }
  const Dtype* gpu_diff()  {    return (const Dtype*)diff_.gpu_data();  }
  Dtype* mutable_cpu_data()
  {
    return static_cast<Dtype*>(data_.mutable_cpu_data());
  }
  Dtype* mutable_gpu_data()
  {
    return static_cast<Dtype*>(data_.mutable_gpu_data());
  }
  Dtype* mutable_cpu_diff()
  {
    return static_cast<Dtype*>(diff_.mutable_cpu_data());
  }
  Dtype* mutable_gpu_diff()
  {
    
    return static_cast<Dtype*>(diff_.mutable_gpu_data());
  }
  void Update()
  {
    // We will perform update based on where the data is located.
    switch (data_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      // perform computation on CPU
      caffe_axpy<Dtype>(count_, Dtype(-1),
        static_cast<const Dtype*>(diff_.cpu_data()),
        static_cast<Dtype*>(data_.mutable_cpu_data()));
      break;
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
      // perform computation on GPU
      caffe_gpu_axpy<Dtype>(count_, Dtype(-1),
        static_cast<const Dtype*>(diff_.gpu_data()),
        static_cast<Dtype*>(data_.mutable_gpu_data()));
#else
      NO_GPU;
#endif
      break;
    default:
      LOG(FATAL) << "Syncedmem not initialized.";
    }
  }

  /// @brief Compute the sum of absolute values (L1 norm) of the data.
  Dtype asum_data()
  {
    switch (data_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      return caffe_cpu_asum(count_, cpu_data());
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
    {
      Dtype asum;
      caffe_gpu_asum(count_, gpu_data(), &asum);
      return asum;
    }
#else
      NO_GPU;
#endif
    case SyncedMemory::UNINITIALIZED:
      return 0;
    default:
      LOG(FATAL) << "Unknown SyncedMemory head state: " << data_.head();
    }
    return 0;
  }
  /// @brief Compute the sum of absolute values (L1 norm) of the diff.
  Dtype asum_diff()
  {
    switch (diff_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      return caffe_cpu_asum(count_, cpu_diff());
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
    {
      Dtype asum;
      caffe_gpu_asum(count_, gpu_diff(), &asum);
      return asum;
    }
#else
      NO_GPU;
#endif
    case SyncedMemory::UNINITIALIZED:
      return 0;
    default:
      LOG(FATAL) << "Unknown SyncedMemory head state: " << diff_.head();
    }
    return 0;
  }
  /// @brief Compute the sum of squares (L2 norm squared) of the data.
  Dtype sumsq_data()
  {
    Dtype sumsq;
    const Dtype* data;

    switch (data_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      data = cpu_data();
      sumsq = caffe_cpu_dot(count_, data, data);
      break;
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
      data = gpu_data();
      caffe_gpu_dot(count_, data, data, &sumsq);
#else
      NO_GPU;
#endif
      break;
    case SyncedMemory::UNINITIALIZED:
      return 0;
    default:
      LOG(FATAL) << "Unknown SyncedMemory head state: " << data_.head();
    }
    return sumsq;
  }
  /// @brief Compute the sum of squares (L2 norm squared) of the diff.
  Dtype sumsq_diff()
  {
    Dtype sumsq;
    const Dtype* diff;

    switch (diff_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      diff = cpu_diff();
      sumsq = caffe_cpu_dot(count_, diff, diff);
      break;
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
      diff = gpu_diff();
      caffe_gpu_dot(count_, diff, diff, &sumsq);
      break;
#else
      NO_GPU;
#endif
    case SyncedMemory::UNINITIALIZED:
      return 0;
    default:
      LOG(FATAL) << "Unknown SyncedMemory head state: " << data_.head();
    }
    return sumsq;
  }

  /// @brief Scale the blob data by a constant factor.
  void scale_data(Dtype scale_factor)
  {
    Dtype* data;

    switch (data_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      data = mutable_cpu_data();
      caffe_scal(count_, scale_factor, data);
      return;
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
      data = mutable_gpu_data();
      caffe_gpu_scal(count_, scale_factor, data);
      return;
#else
      NO_GPU;
#endif
    case SyncedMemory::UNINITIALIZED:
      return;
    default:
      LOG(FATAL) << "Unknown SyncedMemory head state: " << data_.head();
    }
  }
  /// @brief Scale the blob diff by a constant factor.
  void scale_diff(Dtype scale_factor)
  {
    Dtype* diff;

    switch (diff_.head()) {
    case SyncedMemory::HEAD_AT_CPU:
      diff = mutable_cpu_diff();
      caffe_scal(count_, scale_factor, diff);
      return;
    case SyncedMemory::HEAD_AT_GPU:
    case SyncedMemory::SYNCED:
#ifndef CPU_ONLY
      diff = mutable_gpu_diff();
      caffe_gpu_scal(count_, scale_factor, diff);
      return;
#else
      NO_GPU;
#endif
    case SyncedMemory::UNINITIALIZED:
      return;
    default:
      LOG(FATAL) << "Unknown SyncedMemory head state: " << diff_.head();
    }
  }

  /**
   * @brief Set the data_ SHARED_PTR to point to the SyncedMemory holding the
   *        data_ of Blob other -- useful in Layer%s which simply perform a copy
   *        in their Forward pass.
   *
   * This deallocates the SyncedMemory holding this Blob's data_, as
   * SHARED_PTR calls its destructor when reset with the "=" operator.
   */
  void ShareData(const Blob & other)
  {
    CHECK_EQ(count_, other.count());
    data_ = other.data();
  }
  /**
   * @brief Set the diff_ SHARED_PTR to point to the SyncedMemory holding the
   *        diff_ of Blob other -- useful in Layer%s which simply perform a copy
   *        in their Forward pass.
   *
   * This deallocates the SyncedMemory holding this Blob's diff_, as
   * SHARED_PTR calls its destructor when reset with the "=" operator.
   */
  void ShareDiff(const Blob & other)
  {
    CHECK_EQ(count_, other.count());
    diff_ = other.diff();
  }

  bool ShapeEquals(CJSON* other)
  {
    if (other.has_num() || other.has_channels() ||
      other.has_height() || other.has_width()) {
      // Using deprecated 4D Blob dimensions --
      // shape is (num, channels, height, width).
      // Note: we do not use the normal Blob::num(), Blob::channels(), etc.
      // methods as these index from the beginning of the blob shape, where legacy
      // parameter blobs were indexed from the end of the blob shape (e.g., bias
      // Blob shape (1 x 1 x 1 x N), IP layer weight Blob shape (1 x 1 x M x N)).
      return shape_.size() <= 4 &&
        LegacyShape(-4) == other.num() &&
        LegacyShape(-3) == other.channels() &&
        LegacyShape(-2) == other.height() &&
        LegacyShape(-1) == other.width();
    }
    vector<int> other_shape(other.shape().dim_size());
    for (int i = 0; i < other.shape().dim_size(); ++i) {
      other_shape[i] = other.shape().dim(i);
    }
    return shape_ == other_shape;
    return false;
  }
#endif

public:
  SyncedMemory data_;
  SyncedMemory diff_;
  SyncedMemory shape_data_;
  vector<int> shape_;
  int count_;
  int capacity_;

  DISABLE_COPY_AND_ASSIGN(Blob);
};  // class Blob

#endif  // CAFFE_BLOB_HPP_
