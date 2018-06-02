

/**
 * @brief Compute the index of the @f$ K @f$ max values for each datum across
 *        all dimensions @f$ (C \times H \times W) @f$.
 *
 * Intended for use after a classification layer to produce a prediction.
 * If parameter out_max_val is set to true, output is a vector of pairs
 * (max_ind, max_val) for each image. The axis parameter specifies an axis
 * along which to maximise.
 *
 * NOTE: does not implement Backwards operation.
 */

static Dtype ArgMaxLayer(enum LayerOp op, struct Net* net, struct Layer* layer) {
  struct Blob* prev = IBLOB(0);
  struct Blob* next = OBLOB(0);
  int count = Data_count(prev->size);
  
  //bool out_max_val;
  //size_t top_k;
  //bool has_axis;
  //int axis;

  switch (op) {
  case Op_Forward:
    next->size = prev->size;
    {
      const float* prevData = getData(prev);
      float* nextData = setDataZero(next);
      ASSERT(0);
    }
    break;
  case Op_Backward:
    {
      const float* prevData = getData(prev);
      const float* nextDiff = getDiff(next);
      float* prevDiff = setDiff(prev);
      ASSERT(0);
    }
    break;
  default:
    break;
  }
  return 0;
}

#if 0
void ArgMaxLayer::LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  const ArgMaxParameter& argmax_param = this->layer_param_.argmax_param();
  out_max_val = argmax_param.out_max_val();
  top_k = argmax_param.top_k();
  has_axis = argmax_param.has_axis();
  CHECK_GE(top_k, 1) << "top k must not be less than 1.";
  if (has_axis) {
    axis = bottom[0]->CanonicalAxisIndex(argmax_param.axis());
    CHECK_GE(axis, 0) << "axis must not be less than 0.";
    CHECK_LE(axis, bottom[0]->num_axes()) <<
      "axis must be less than or equal to the number of axis.";
    CHECK_LE(top_k, bottom[0]->shape(axis))
      << "top_k must be less than or equal to the dimension of the axis.";
  } else {
    CHECK_LE(top_k, bottom[0]->count(1))
      << "top_k must be less than or equal to"
        " the dimension of the flattened bottom blob per instance.";
  }
}


void ArgMaxLayer::Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
  int num_top_axes = bottom[0]->num_axes();
  if ( num_top_axes < 3 ) num_top_axes = 3;
  std::vector<int> shape(num_top_axes, 1);
  if (has_axis) {
    // Produces max_ind or max_val per axis
    shape = bottom[0]->shape();
    shape[axis] = top_k;
  } else {
    shape[0] = prevSize.n;
    // Produces max_ind
    shape[2] = top_k;
    if (out_max_val) {
      // Produces max_ind and max_val
      shape[1] = 2;
    }
  }
  top[0]->Reshape(shape);
}

void ArgMax(const float* prevData, Dtype* nextData, DataSize prevSize, int dim, int axis_dist) {
  int dim, axis_dist;
  int num = Data_count(prevSize) / dim;
  std::vector<std::pair<Dtype, int> > bottom_data_vector(dim);
  for (i = 0; i < num; ++i) {
    for (j = 0; j < dim; ++j) {
      bottom_data_vector[j] = make_pair(prevData[(i / axis_dist * dim + j) * axis_dist + i % axis_dist], j);
    }
    PARTIAL_SORT(
        bottom_data_vector.begin(), bottom_data_vector.begin() + top_k,
        bottom_data_vector.end(), std::greater<std::pair<Dtype, int> >());
    for (j = 0; j < top_k; ++j) {
      if (out_max_val) {
        if (has_axis) {
          // Produces max_val per axis
          nextData[(i / axis_dist * top_k + j) * axis_dist + i % axis_dist]
            = bottom_data_vector[j].first;
        } else {
          // Produces max_ind and max_val
          nextData[2 * i * top_k + j] = bottom_data_vector[j].second;
          nextData[2 * i * top_k + top_k + j] = bottom_data_vector[j].first;
        }
      } else {
        // Produces max_ind per axis
        nextData[(i / axis_dist * top_k + j) * axis_dist + i % axis_dist]
          = bottom_data_vector[j].second;
      }
    }
  }
}

#endif

