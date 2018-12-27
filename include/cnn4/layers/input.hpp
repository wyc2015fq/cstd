#ifndef CAFFE_INPUT_LAYER_HPP_
#define CAFFE_INPUT_LAYER_HPP_

class InputLayer : public Layer
{
public:
  // Data layers should be shared by multiple solvers in parallel
  virtual inline bool ShareInParallel() const { return true; }
  // Data layers have no bottoms, so reshaping is trivial.
   
  virtual inline const char* type() const { return "Input"; }
  virtual inline int ExactNumBottomBlobs() const { return 0; }
  virtual inline int MinTopBlobs() const { return 1; }

  std::vector<int> shape_;

  virtual void fromJson(cjson* param) {
    cjson_GetObjectNumberArray(param, "shape", shape_);
  }
  virtual void toJson(cjson* param) {
    cjson_AddNumberArrayToObject(param, "shape", shape_);
  }

  virtual void LayerSetUp(const vector<Blob*> & bottom, const vector<Blob*> & top)
  {
    const int num_top = top.size();
    const int num_shape = shape_.size()/4;
    CHECK(num_shape == 0 || num_shape == 1 || num_shape == num_top)
      << "Must specify 'shape' once, once per top blob, or not at all: "
      << num_top << " tops vs. " << num_shape << " shapes.";
    if (num_shape > 0) {
      for (int i = 0; i < num_top; ++i) {
        const int shape_index = (num_shape == 1) ? 0 : i;
        DataShape datashape;
        for (int j = 0; j < 4; ++j) {
          datashape.dim[j] = shape_[shape_index *4+j];
        }
        top[i]->Reshape(datashape);
      }
    }
  }
};


INSTANTIATE_CLASS(Input, InputLayer);


#endif  // CAFFE_INPUT_LAYER_HPP_
