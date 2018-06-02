#ifndef CAFFE_PYTHON_LAYER_HPP_
#define CAFFE_PYTHON_LAYER_HPP_




#include "caffe/layer.hpp"

namespace bp = boost::python;




class PythonLayer : public Layer {
 public:
  PythonLayer(PyObject* self, const LayerParameter& param)
      : Layer(param), self_(bp::handle<>(bp::borrowed(self))) { }

  virtual void LayerSetUp(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
    // Disallow PythonLayer in MultiGPU training stage, due to GIL issues
    // Details: https://github.com/BVLC/caffe/issues/2936
    if (this->phase_ == TRAIN && Caffe::solver_count() > 1
        && !Caffe::multiprocess()) {
      LOG(FATAL) << "PythonLayer does not support CLI Multi-GPU, use train.py";
    }
    self_.attr("param_str") = bp::str(
        this->layer_param_.python_param().param_str());
    self_.attr("phase") = static_cast<int>(this->phase_);
    self_.attr("setup")(bottom, top);
  }
  virtual void Reshape(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
    self_.attr("reshape")(bottom, top);
  }

  virtual inline const char* type() const { return "Python"; }

 protected:
  virtual void Forward_cpu(const vector<Blob*>& bottom,
      const vector<Blob*>& top) {
    self_.attr("forward")(bottom, top);
  }
  virtual void Backward_cpu(const vector<Blob*>& top,
      const vector<bool>& propagate_down, const vector<Blob*>& bottom) {
    self_.attr("backward")(top, propagate_down, bottom);
  }

 private:
  bp::object self_;
};



#endif
