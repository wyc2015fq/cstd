#ifndef CAFFE_PYTHON_LAYER_HPP_
#define CAFFE_PYTHON_LAYER_HPP_

#include <boost/python.hpp>




namespace bp = boost::python;

namespace
{

  template <typename Dtype>
  class PythonLayer : public Layer
  {
  public:
    PythonLayer(PyObject* self, )
      : Layer(param), self_(bp::handle<>(bp::borrowed(self))) { }

    virtual void LayerSetUp(const vector<Blob*> & bottom,
                            const vector<Blob*> & top) {
      // Disallow PythonLayer in MultiGPU training stage, due to GIL issues
      // Details: https://github.com/BVLC/caffe/issues/2936
      if (this->phase_ == TRAIN && Caffe::solver_count() > 1
          && !ShareInParallel()) {
        LOG(FATAL) << "PythonLayer is not implemented in Multi-GPU training";
      }
      self_.attr("param_str") = bp::str(
                                  this->param_->python_param().param_str());
      self_.attr("phase") = static_cast<int>(this->phase_);
      self_.attr("setup")(bottom, top);
    }
    virtual void Reshape(const vector<Blob*> & bottom,
                         const vector<Blob*> & top) {
      self_.attr("reshape")(bottom, top);
    }

    virtual inline bool ShareInParallel() const {
      return this->param_->python_param().share_in_parallel();
    }

    virtual inline const char* type() const { return "Python"; }

  public:
    virtual void Forward(CPUContext* context, const vector<Blob*> & bottom,
                             const vector<Blob*> & top) {
      self_.attr("forward")(bottom, top);
    }
    virtual void Backward(CPUContext* context, const vector<Blob*> & top,
                              const vector<Blob*> & bottom) {
      self_.attr("backward")(top, propagate_down, bottom);
    }

  private:
    bp::object self_;
  };

}  // namespace

#endif
