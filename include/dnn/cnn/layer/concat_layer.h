
#if 0

static Dtype ConcatLayer_Forward(struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int i, j, q, b, dims = Blob_dims(prev);
  int axis = LARG(axis);
  DataSize prevSize = prev->size;

  if (dims == 1) { // axis == 0
    // concat vector
    // total length
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
      top_w += prevSize.w;
    }

    setData(next, top_w, 1);

    float* outptr = Blob_datap(next);

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);

      int w = prevSize.w;

      const float* ptr = Blob_datap(prev);

      for (i = 0; i < w; i++) {
        outptr[i] = ptr[i];
      }

      outptr += w;
    }

    return 0;
  }

  if (dims == 2 && axis == 0) {
    // concat image
    int w = prevSize.w;

    // total height
    int top_h = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
      top_h += prevSize.h;
    }

    Blob_create(next, w, top_h);

    if (Blob_empty(next)) {
      return -100;
    }

    float* outptr = Blob_datap(next);

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);

      int size = w * prevSize.h;

      const float* ptr = Blob_datap(prev);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }

      outptr += size;
    }

    return 0;
  }

  if (dims == 2 && axis == 1) {
    // interleave image row
    int h = prevSize.h;

    // total width
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
      top_w += prevSize.w;
    }

    Blob_create(next, top_w, h);

    if (Blob_empty(next)) {
      return -100;
    }

    #pragma omp parallel for

    for (i = 0; i < h; i++) {
      float* outptr = Blob_row0(next, i);

      for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
        const float* ptr = Blob_row0(prev, i);

        for (j = 0; j < prevSize.w; j++) {
          outptr[j] = ptr[j];
        }

        outptr += prevSize.w;
      }
    }

    return 0;
  }

  if (dims == 3 && axis == 0) {
    // concat dim
    int w = prevSize.w;
    int h = prevSize.h;

    // total channels
    int top_channels = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
      top_channels += prevSize.c;
    }

    Blob_create(next, w, h, top_channels);

    if (Blob_empty(next)) {
      return -100;
    }

    int q = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);

      int channels = prevSize.c;
      int size = Blob_total(prev);

      const float* ptr = Blob_datap(prev);
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < size; i++) {
        outptr[i] = ptr[i];
      }

      q += channels;
    }

    return 0;
  }

  if (dims == 3 && axis == 1) {
    // interleave dim height
    int w = prevSize.w;
    int channels = prev->c;

    // total height
    int top_h = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
      top_h += prevSize.h;
    }

    Blob_create(next, w, top_h, channels);

    if (Blob_empty(next)) {
      return -100;
    }

    #pragma omp parallel for
    for (q = 0; q < channels; q++) {
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
        int size = prevSize.w * prevSize.h;
        const float* ptr = prevData + q * Data_2DSize(prevSize);

        for (i = 0; i < size; i++) {
          outptr[i] = ptr[i];
        }
      }
    }

    return 0;
  }

  if (dims == 3 && axis == 2) {
    // interleave dim width
    int h = prevSize.h;
    int channels = prev->c;

    // total height
    int top_w = 0;

    for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
      top_w += prevSize.w;
    }

    Blob_create(next, top_w, h, channels);

    if (Blob_empty(next)) {
      return -100;
    }

    #pragma omp parallel for

    for (q = 0; q < channels; q++) {
      float* outptr = nextData + q * Data_2DSize(nextSize);

      for (i = 0; i < h; i++) {
        for (b = 0; b < layer->bottoms_size; b++) {
    struct Blob* prev = IBLOB(i);
          const float* ptr = Blob_crow(prev, q, i);

          for (j = 0; j < prevSize.w; j++) {
            outptr[j] = ptr[j];
          }

          outptr += prevSize.w;
        }
      }
    }

    return 0;
  }

  return 0;
}

#endif

DataSize concat_calc_outsize(const struct Net* net, const struct Layer* layer) {
  const struct Blob* prev = IBLOB(0);
  DataSize nextSize = prev->size;
  const int* dim = prev->dim;
  int concat_axis_ = IARG(axis);
  int num_concats_ = Blob_count_range(prev, 0, concat_axis_);
  int concat_input_size_ = Blob_count_range(prev, concat_axis_ + 1, 4);
  int i, j;
  //int prev_count_sum = Data_count(prev->size);
  for (i = 1; i < layer->i_size; ++i) {
    struct Blob* prev = IBLOB(i);
    for (j = 0; j < 4; ++j) {
      if (j == concat_axis_) { continue; }
      ASSERT((dim[j] == prev->dim[j]) && "All inputs must have the same shape, except at concat_axis.");
    }
    //prev_count_sum += prev->count();
    nextSize.dim[concat_axis_] += prev->size.dim[concat_axis_];
  }
  return nextSize;
}

static Dtype ConcatLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* next = OBLOB(0);
  const struct Blob* prev = IBLOB(0);
  int i, n;
  int count_=0;
  int concat_axis_ = IARG(axis);
  int num_concats_ = Blob_count_range(prev, 0, concat_axis_);
  int concat_input_size_ = Blob_count_range(prev, concat_axis_ + 1, 4);
  ASSERT(concat_axis_>=0 && concat_axis_<4);
  if (concat_axis_>0) {
    // Don't allow negative indexing for concat_dim, a uint32 -- almost
    // certainly unintended.
    //CHECK_GE(concat_axis_, 0) << "casting concat_dim from uint32 to int32 "
    //    << "produced negative result; concat_dim must satisfy "
    //    << "0 <= concat_dim < " << kMaxBlobAxes;
    //CHECK_LT(concat_axis_, num_axes) << "concat_dim out of range.";
  } else {
    // concat_axis_ = prev[0]->CanonicalAxisIndex(concat_param.axis());
  }
  // Initialize with the first blob.
  //CHECK_EQ(prev_count_sum, next[0]->count());

  switch (op) {
  case Op_Forward:
    next->size = concat_calc_outsize(net, layer);
    {
      Dtype* nextData = setDataZero(next);
      int offset_concat_axis = 0;
      const int next_concat_axis = next->size.dim[concat_axis_];
      for (i = 0; i < layer->i_size; ++i) {
        struct Blob* prev = IBLOB(i);
        const Dtype* prevData = getData(prev);
        const int prev_concat_axis = prev->size.dim[concat_axis_];
        for (n = 0; n < num_concats_; ++n) {
          blas_scopy(prev_concat_axis * concat_input_size_,
            prevData + n * prev_concat_axis * concat_input_size_,
            nextData + (n * next_concat_axis + offset_concat_axis)
            * concat_input_size_);
        }
        offset_concat_axis += prev_concat_axis;
      }
    }
    break;
  case Op_Backward:
    {
      const Dtype* nextDiff = getDiff(next);
      int offset_concat_axis = 0;
      const int next_concat_axis = next->size.dim[concat_axis_];
      for (i = 0; i < layer->i_size; ++i) {
        struct Blob* prev = IBLOB(i);
        const int prev_concat_axis = prev->size.dim[concat_axis_];
        Dtype* prevDiff = setDiffZero(prev);
        for (n = 0; n < num_concats_; ++n) {
          blas_scopy(prev_concat_axis * concat_input_size_, 
            nextDiff + (n * next_concat_axis + offset_concat_axis) * concat_input_size_,
            prevDiff + n * prev_concat_axis * concat_input_size_);
        }
        offset_concat_axis += prev_concat_axis;
      }
    }
  }
  return 0;
}
static int net_concat(struct Net* net, int ninput, const int* inputs, int axis) {
  const char* type = "concat";
  struct Layer* layer = net_addLayer(net, ninput, 1, 0, ConcatLayer, type);
  int i;
  axis = axis%4;
  axis = (axis+4)%4;
  IARG(axis) = axis;
  for (i=0; i<ninput; ++i) {
    LIBLOB(i) = inputs[i];
  }
  return LOBLOB(0) = net_addBlob(net, concat_calc_outsize(net, layer), type, "o");
}
