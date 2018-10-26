#ifndef CAFFE_PYTHON_LAYER_HPP_
#define CAFFE_PYTHON_LAYER_HPP_

#include <boost/python.hpp>
#include <vector>

#include "layer.hpp"

namespace bp = boost::python;

namespace caffe
{

  template <typename Dtype>
  class PythonLayer : public Layer<Dtype>
  {
  public:
    PythonLayer(PyObject* self, CJSON* param)
      : Layer<Dtype>(param), self_(bp::handle<>(bp::borrowed(self))) { }

    virtual void LayerSetUp(const vector<Blob<Dtype>*> & bottom,
                            const vector<Blob<Dtype>*> & top) {
      // Disallow PythonLayer in MultiGPU training stage, due to GIL issues
      // Details: https://github.com/BVLC/caffe/issues/2936
      if (this->phase_ == TRAIN && solver_count() > 1
          && !ShareInParallel()) {
        LOG(FATAL) << "PythonLayer is not implemented in Multi-GPU training";
      }
      self_.attr("param_str") = bp::str(
                                  this->layer_param_.python_param().param_str());
      self_.attr("phase") = static_cast<int>(this->phase_);
      self_.attr("setup")(bottom, top);
    }
    virtual void Reshape(const vector<Blob<Dtype>*> & bottom,
                         const vector<Blob<Dtype>*> & top) {
      self_.attr("reshape")(bottom, top);
    }

    virtual inline bool ShareInParallel() const {
      return this->layer_param_.python_param().share_in_parallel();
    }

    virtual inline const char* type() const { return "Python"; }

  protected:
    virtual void Forward_cpu(const vector<Blob<Dtype>*> & bottom,
                             const vector<Blob<Dtype>*> & top) {
      self_.attr("forward")(bottom, top);
    }
    virtual void Backward_cpu(const vector<Blob<Dtype>*> & top,
                              const vector<bool> & propagate_down, const vector<Blob<Dtype>*> & bottom) {
      self_.attr("backward")(top, propagate_down, bottom);
    }

  private:
    bp::object self_;
  };



#ifdef WITH_PYTHON_LAYER
  template <typename Dtype>
  SHARED_PTR(Layer<Dtype>) GetPythonLayer(CJSON* param)
  {
    Py_Initialize();
    try {
      bp::object module = bp::import(param.python_param().module().c_str());
      bp::object layer = module.attr(param.python_param().layer().c_str())(param);
      return bp::extract<SHARED_PTR<PythonLayer<Dtype> > >(layer)();
    }
    catch (bp::error_already_set) {
      PyErr_Print();
      throw;
    }
  }

  REGISTER_LAYER_CREATOR(Python, GetPythonLayer);
#endif



}  // namespace caffe

#endif
