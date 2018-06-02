
int Slice::forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs, const int* slices_ptr) 
{
  const Blob* prev = bottom_blobs[0];
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;

  int q = 0;
  const int* slices_ptr = (const int*)slices.data;

  for (size_t i = 0; i < top_blobs.size(); i++) {
    int slice = slices_ptr[i];

    if (slice == -233) {
      slice = (channels - q) / (top_blobs.size() - i);
    }

    Blob* next = top_blobs[i];
    next->create(w, h, slice);

    if (next->empty()) {
      return -100;
    }

    int size = prev->cstep * slice;

    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData;

    for (j = 0; j < size; j++) {
      outptr[j] = ptr[j];
    }

    q += slice;
  }

  return 0;
}


int Slice_forward(const std::vector<Blob>& bottom_blobs, std::vector<Blob>& top_blobs) const
{
  const Blob* prev = bottom_blobs[0];
  int w = prevSize.w;
  int h = prevSize.h;
  int channels = prevSize.c;

  int q = 0;
  const int* slices_ptr = (const int*)slices.data;

  for (size_t i = 0; i < top_blobs.size(); i++) {
    int slice = slices_ptr[i];

    if (slice == -233) {
      slice = (channels - q) / (top_blobs.size() - i);
    }

    Blob* next = top_blobs[i];
    next->create(w, h, slice);

    if (next->empty()) {
      return -100;
    }

    int size = prev->cstep * slice;

    const float* ptr = prevData + q * Data_2DSize(prevSize);
    float* outptr = nextData;

#if __ARM_NEON
    int nn = size >> 3;
    int remain = size - (nn << 3);
#else
    int remain = size;
#endif // __ARM_NEON

#if __ARM_NEON
#if __aarch64__

    for (; nn > 0; nn--) {
      float32x4_t _p = vld1q_f32(ptr);
      float32x4_t _p2 = vld1q_f32(ptr + 4);
      vst1q_f32(outptr, _p);
      vst1q_f32(outptr + 4, _p2);

      ptr += 8;
      outptr += 8;
    }

#else

    if (nn > 0) {
      asm volatile(
          "0:                             \n"
          "pld        [%1, #256]          \n"
          "vld1.f32   {d0-d3}, [%1 :128]! \n"
          "subs       %0, #1              \n"
          "vst1.f32   {d0-d3}, [%2 :128]! \n"
          "bne        0b                  \n"
          : "=r"(nn),     // %0
          "=r"(ptr),    // %1
          "=r"(outptr)  // %2
          : "0"(nn),
          "1"(ptr),
          "2"(outptr)
          : "cc", "memory", "q0"
      );
    }

#endif // __aarch64__
#endif // __ARM_NEON

    for (; remain > 0; remain--) {
      *outptr++ = *ptr++;
    }

    q += slice;
  }

  return 0;
}



static void SliceLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int i, count = Data_count(prev->size);
  int count_;
  int num_slices_;
  int slice_size_;
  int slice_axis_;
  int* slice_point_ = NULL;

#if 0
  

  const SliceParameter& slice_param = this->layer_param_.slice_param();
  CHECK(!(slice_param.has_axis() && slice_param.has_slice_dim()))
      << "Either axis or slice_dim should be specified; not both.";
  slice_point_.clear();
  std::copy(slice_param.slice_point().begin(),
      slice_param.slice_point().end(),
      std::back_inserter(slice_point_));



  const int num_axes = bottom[0]->num_axes();
  const SliceParameter& slice_param = this->layer_param_.slice_param();
  if (slice_param.has_slice_dim()) {
    slice_axis_ = (int)(slice_param.slice_dim());
    // Don't allow negative indexing for slice_dim, a uint32 -- almost
    // certainly unintended.
    CHECK_GE(slice_axis_, 0) << "casting slice_dim from uint32 to int32 "
        << "produced negative result; slice_dim must satisfy "
        << "0 <= slice_dim < " << kMaxBlobAxes;
    CHECK_LT(slice_axis_, num_axes) << "slice_dim out of range.";
  } else {
    slice_axis_ = bottom[0]->CanonicalAxisIndex(slice_param.axis());
  }
  vector<int> top_shape = bottom[0]->shape();
  const int bottom_slice_axis = bottom[0]->shape(slice_axis_);
  num_slices_ = bottom[0]->count(0, slice_axis_);
  slice_size_ = bottom[0]->count(slice_axis_ + 1);
  int count = 0;
  if (slice_point_.size() != 0) {
    CHECK_EQ(slice_point_.size(), layer->o_size - 1);
    CHECK_LE(layer->o_size, bottom_slice_axis);
    int prev = 0;
    vector<int> slices;
    for (i = 0; i < slice_point_.size(); ++i) {
      CHECK_GT(slice_point_[i], prev);
      slices.push_back(slice_point_[i] - prev);
      prev = slice_point_[i];
    }
    slices.push_back(bottom_slice_axis - prev);
    for (i = 0; i < layer->o_size; ++i) {
      top_shape[slice_axis_] = slices[i];
      top[i]->Reshape(top_shape);
      count += top[i]->count();
    }
  } else {
    CHECK_EQ(bottom_slice_axis % layer->o_size, 0)
        << "Number of top blobs (" << layer->o_size << ") should evenly "
        << "divide input slice axis (" << bottom_slice_axis << ")";
    top_shape[slice_axis_] = bottom_slice_axis / layer->o_size;
    for (i = 0; i < layer->o_size; ++i) {
      top[i]->Reshape(top_shape);
      count += top[i]->count();
    }
  }
  CHECK_EQ(count, Data_count(prev->size));
  if (layer->o_size == 1) {
    top[0]->ShareData(*bottom[0]);
    top[0]->ShareDiff(*bottom[0]);
  }
  
#endif
  
  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      if (layer->o_size == 1) { return; }
      int offset_slice_axis = 0;
      const Dtype* prevData = prevData;
      const int bottom_slice_axis = bottom[0]->shape(slice_axis_);
      for (i = 0; i < layer->o_size; ++i) {
        Dtype* nextData = top[i]->mutable_data();
        const int top_slice_axis = top[i]->shape(slice_axis_);
        for (n = 0; n < num_slices_; ++n) {
          const int top_offset = n * top_slice_axis * slice_size_;
          const int bottom_offset =
            (n * bottom_slice_axis + offset_slice_axis) * slice_size_;
          blas_scopy(top_slice_axis * slice_size_,
            prevData + bottom_offset, nextData + top_offset);
        }
        offset_slice_axis += top_slice_axis;
      }
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      
      if (!propagate_down[0] || layer->o_size == 1) { return; }
      int offset_slice_axis = 0;
      Dtype* prevDiff = bottom[0]->mutable_diff();
      const int bottom_slice_axis = bottom[0]->shape(slice_axis_);
      for (i = 0; i < layer->o_size; ++i) {
        const Dtype* nextDiff = top[i]->cpu_diff();
        const int top_slice_axis = top[i]->shape(slice_axis_);
        for (n = 0; n < num_slices_; ++n) {
          const int top_offset = n * top_slice_axis * slice_size_;
          const int bottom_offset =
            (n * bottom_slice_axis + offset_slice_axis) * slice_size_;
          blas_scopy(top_slice_axis * slice_size_,
            nextDiff + top_offset, prevDiff + bottom_offset);
        }
        offset_slice_axis += top_slice_axis;
      }
    }
    break;
  default:
    break;
  }
}

