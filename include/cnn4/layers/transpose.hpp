#ifndef CAFFE_TRANSPOSE_LAYER_HPP_
#define CAFFE_TRANSPOSE_LAYER_HPP_

class TransposeLayer : public Layer
{
public:
  vector<int> permute(const vector<int> & vec);
  DataShape bottom_counts_;
  DataShape top_counts_;
  DataShape forward_map_;
  DataShape backward_map_;

  vector<int> dim_;
  TransposeLayer() {
  }
  void init(CJSON* param) {
    cJSON_GetObjectNumberArray(param, "dim", dim_);
  }

  virtual inline const char* type() const { return "Transpose"; }
  virtual inline int ExactNumBottomBlobs() const { return 1; }
  virtual inline int ExactNumTopBlobs() const { return 1; }


  void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    CHECK_NE(bottom[0], top[0]) << this->type() << " Layer does not support "
      "in-place computation.";
  }

  void permute(int n, const int* vec, const int* dim, int* new_vec)
  {
    for (int i = 0; i < n; i++) {
      new_vec[i] = vec[dim[i]];
    }
    return ;
  }

  void Reshape(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    DataShape shape = bottom[0]->shape_;
    int num_axes = shape.num_axes();
    CHECK_GT(num_axes, 0) << "the dimension of the transposed blob should "
      "be greater than 0.";
    CHECK_LE(num_axes, kMaxBlobAxes) << "the dimension of the transposed blob should "
      "be less than kMaxBlobAxes (" << kMaxBlobAxes << ").";
    CHECK_EQ(num_axes, dim_.size()) << "the dimensions of "
      "the top blob and bottom blob must be equal.";
    DataShape top_shape = shape;
    permute(num_axes, shape.dim, &dim_[0], top_shape.dim);
    top[0]->Reshape(top_shape);
    shape = dataShape(num_axes);
    //bottom_counts_.Reshape(shape);
    //top_counts_.Reshape(shape);
    int* bottom_counts_data = (int*)bottom_counts_.dim;
    int* top_counts_data = (int*)top_counts_.dim;
    for (int i = 1; i < num_axes; i++) {
      *bottom_counts_data = bottom[0]->count(i);
      *top_counts_data = top[0]->count(i);
      bottom_counts_data++;
      top_counts_data++;
    }
    *bottom_counts_data = 1;
    *top_counts_data = 1;
    //forward_map_.Reshape(shape);
    //backward_map_.Reshape(shape);
    int* forward_map_data = (int*)forward_map_.dim;
    int* backward_map_data = (int*)backward_map_.dim;
    for (int i = 0; i < num_axes; i++) {
      *forward_map_data = dim_[i];
      backward_map_data[dim_[i]] = i;
      forward_map_data++;
    }
    //shape = dataShape(bottom[0]->count() * num_axes);
    //buf_.Reshape(shape);
  }


  void Forward_(const vector<Blob*>& bottom, const vector<Blob*>& top) {
    const int nthreads = bottom[0]->count();

    transpose(nthreads, bottom[0]->data(), top[0]->mdata(),
        bottom_counts_, top_counts_, forward_map_, (int)bottom[0]->shape_.num_axes());
  }

  void Backward_(const vector<Blob*>& top,const vector<Blob*>& bottom) {
    if (!bottom[0]->propagate_down_) {
      return;
    }
    const int nthreads = bottom[0]->count();

    transpose(nthreads, top[0]->diff(), bottom[0]->mdiff(), top_counts_, bottom_counts_, backward_map_, (int)bottom[0]->shape().num_axes() );
  }


};

INSTANTIATE_CLASS(Transpose, TransposeLayer);

#endif  // CAFFE_TRANSPOSE_LAYER_HPP_
