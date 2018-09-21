
#include <Python.h>

#pragma comment(lib,"python36.lib")

int test_python_class()
{
  Py_Initialize();

  PyObject * pModule = NULL;
  PyObject * pFunc = NULL;
  PyObject * pArg = NULL;
  PyObject * pClass = NULL;
  PyObject * pObject = NULL;

  pModule = PyImport_ImportModule("test2");
  pClass = PyObject_GetAttrString(pModule, "TestClass");//得到那个类
  pArg = PyTuple_New(1);
  PyTuple_SetItem(pArg, 0, Py_BuildValue("s", "Jacky"));
  pObject = PyEval_CallObject(pClass, pArg);//生成一个对象，或者叫作实例

  pFunc = PyObject_GetAttrString(pObject, "printName");//得到该实例的成员函数
  PyEval_CallObject(pFunc, NULL);//执行该实例的成员函数

  Py_Finalize();
  return 0;
}

#include <boost/python.hpp>
#include "caffe\proto\caffe.pb.cc"

namespace bp = boost::python;

int test_python_caffe() {
  Py_Initialize();
  caffe::LayerParameter param;
  try {
    bp::object module = bp::import("pythonLayer");
    bp::object layer = module.attr("Data_Layer_train")(param);
    return 0;
  }
  catch (bp::error_already_set) {
    PyErr_Print();
    throw;
  }
  return 0;
}
