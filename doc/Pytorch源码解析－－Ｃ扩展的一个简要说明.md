# Pytorch源码解析－－Ｃ扩展的一个简要说明
2019年01月30日 21:49:20
梦溪笔记
阅读数：246
更多
所属专栏： [机器学习杂记](https://blog.csdn.net/column/details/28442.html)
版本：Pytorh v1.0，　python3.7
要了解Pytorch的源码，最好先对CPython有个大致的了解，比如Python的多态是如何实现的。
Pytorch的底层源码，很多都用c++实现，例如，torch._C就是一个非常典型的Ｃ＋＋模块，很多对象都会继承“_C”模块里的内容，如，
```python
class Tensor(torch._C._TensorBase):
    def __deepcopy__(self, memo):
        if not self.is_leaf:
            raise RuntimeError("Only Tensors created explicitly by the user "
                               "(graph leaves) support the deepcopy protocol 
　　　　　　　　　　　　　　　　　　　at the moment")
        if id(self) in memo:
            return memo[id(self)]
        with torch.no_grad():
            if self.is_sparse:
                new_tensor = self.clone()
            else:
                new_storage = self.storage().__deepcopy__(memo)
                new_tensor = self.new()
                new_tensor.set_(new_storage, self.storage_offset(), 
　　　　　　　　　　　　　　　　　　　self.size(), self.stride())
            memo[id(self)] = new_tensor
            new_tensor.requires_grad = self.requires_grad
            return new_tensor
```
这里的_C，源码在torch/csrc文件夹中，我们从setup.py开始，一步步追踪一下这个Ｃ扩展。
首先，在setup.py里，这个模块是这样定义的，
```python
main_sources = ["torch/csrc/stub.cpp"]
...
################################################################################
# Declare extensions and package
################################################################################
extensions = []
packages = find_packages(exclude=('tools', 'tools.*'))
C = Extension("torch._C",
              libraries=main_libraries,
              sources=main_sources,
              language='c++',
              extra_compile_args=main_compile_args + extra_compile_args,
              include_dirs=[],
              library_dirs=library_dirs,
              extra_link_args=extra_link_args + main_link_args +
                   [make_relative_rpath('lib')],
              )
extensions.append(C)
if not IS_WINDOWS:
    DL = Extension("torch._dl",
                   sources=["torch/csrc/dl.c"],
                   language='c'
                   )
    extensions.append(DL)
```
好了，我们看到这个_C模块的接口是由torch/csrc/stub.cpp实现的，现在来看看这个stub到底怎么回事，
```cpp
/*****stub.cpp*****/
#include <Python.h>
#ifdef _WIN32
__declspec(dllimport)
#endif
extern PyObject* initModule();
#if PY_MAJOR_VERSION == 2
PyMODINIT_FUNC init_C()
{
  initModule();
}
#else
PyMODINIT_FUNC PyInit__C()
{
  return initModule();
}
#endif
```
这个PyMODINIT_FUNC只是一个调用约定（或者说是链接说明）：
```cpp
/*****windows*****/
＃if defined(__cplusplus)
#    define PyMODINIT_FUNC extern "C" __declspec(dllexport) PyObject*
#else /* __cplusplus */
#    define PyMODINIT_FUNC __declspec(dllexport) PyObject*
#endif /* __cplusplus */
/*****linux*****/
#ifndef PyMODINIT_FUNC
#       if defined(__cplusplus)
#               define PyMODINIT_FUNC extern "C" PyObject*
#       else /* __cplusplus */
#               define PyMODINIT_FUNC PyObject*
#       endif /* __cplusplus */
#endif
```
关键的还得看这个initModule()函数。这个函数在torch/csrc/module.cpp中，内容比较多，
```cpp
PyObject* initModule() {
  HANDLE_TH_ERRORS
  THInferNumThreads();
#define ASSERT_TRUE(cmd) if (!(cmd)) return nullptr
  THPUtils_addPyMethodDefs(methods, TorchMethods);
  THPUtils_addPyMethodDefs(methods, DataLoaderMethods);
  THPUtils_addPyMethodDefs(methods, torch::autograd::python_functions());
  THPUtils_addPyMethodDefs(methods, torch::multiprocessing::python_functions());
#ifdef USE_CUDA
  THPUtils_addPyMethodDefs(methods, THCPModule_methods());
#endif
#ifdef USE_CUDNN
  THPUtils_addPyMethodDefs(methods, THCUDNN_methods());
#endif
#ifdef USE_DISTRIBUTED
  THPUtils_addPyMethodDefs(methods, THDPModule_methods());
#ifdef USE_C10D
  THPUtils_addPyMethodDefs(methods, torch::distributed::c10d::python_functions());
#endif
#endif
#if PY_MAJOR_VERSION == 2
  ASSERT_TRUE(module = Py_InitModule("torch._C", methods.data()));
#else
  static struct PyModuleDef torchmodule = {
     PyModuleDef_HEAD_INIT,
     "torch._C",
     nullptr,
     -1,
     methods.data()
  };
  ASSERT_TRUE(module = PyModule_Create(&torchmodule));
#endif
  ASSERT_TRUE(THPWrapper_init(module));
  ASSERT_TRUE(THPGenerator_init(module));
  ASSERT_TRUE(THPException_init(module));
  THPSize_init(module);
  THPDtype_init(module);
  THPDTypeInfo_init(module);
  THPLayout_init(module);
  THPDevice_init(module);
  ASSERT_TRUE(THPVariable_initModule(module));
  ASSERT_TRUE(THPFunction_initModule(module));
  ASSERT_TRUE(THPEngine_initModule(module));
  // NOTE: We need to be able to access OperatorExportTypes from ONNX for use in
  // the export side of JIT, so this ONNX init needs to appear before the JIT
  // init.
  torch::onnx::initONNXBindings(module);
  torch::jit::initJITBindings(module);
  torch::autograd::initNNFunctions(module);
  torch::autograd::init_legacy_variable(module);
  torch::python::init_bindings(module);
#ifdef USE_CUDA
  torch::cuda::initModule(module);
#endif
  ASSERT_TRUE(THPDoubleStorage_init(module));
  ASSERT_TRUE(THPFloatStorage_init(module));
  ASSERT_TRUE(THPHalfStorage_init(module));
  ASSERT_TRUE(THPLongStorage_init(module));
  ASSERT_TRUE(THPIntStorage_init(module));
  ASSERT_TRUE(THPShortStorage_init(module));
  ASSERT_TRUE(THPCharStorage_init(module));
  ASSERT_TRUE(THPByteStorage_init(module));
#ifdef USE_CUDA
  // This will only initialise base classes and attach them to library namespace
  // They won't be ready for real usage until importing cuda module, that will
  // complete the process (but it defines Python classes before calling back into
  // C, so these lines have to execute first)..
  ASSERT_TRUE(THCPDoubleStorage_init(module));
  ASSERT_TRUE(THCPFloatStorage_init(module));
  ASSERT_TRUE(THCPHalfStorage_init(module));
  ASSERT_TRUE(THCPLongStorage_init(module));
  ASSERT_TRUE(THCPIntStorage_init(module));
  ASSERT_TRUE(THCPShortStorage_init(module));
  ASSERT_TRUE(THCPCharStorage_init(module));
  ASSERT_TRUE(THCPByteStorage_init(module));
  THCPStream_init(module);
  THCPEvent_init(module);
#endif
  auto set_module_attr = [&](const char* name, PyObject* v, bool incref = true) {
    // PyModule_AddObject steals reference
    if (incref) {
      Py_INCREF(v);
    }
    return PyModule_AddObject(module, name, v) == 0;
  };
#ifdef USE_CUDNN
  PyObject *has_cudnn = Py_True;
#else
  PyObject *has_cudnn = Py_False;
#endif
 ASSERT_TRUE(set_module_attr("has_cudnn", has_cudnn));
  // force ATen to initialize because it handles
  // setting up TH Errors so that they throw C++ exceptions
  at::init();
  py::reinterpret_borrow<py::module>(module).def("_demangle", &c10::demangle);
  // Set ATen warnings to issue Python warnings
  ::c10::Warning::set_warning_handler(&warning_handler);
  ASSERT_TRUE(set_module_attr("has_mkl", at::hasMKL() ? Py_True : Py_False));
  ASSERT_TRUE(set_module_attr("has_lapack", at::hasLAPACK() ? Py_True : Py_False));
#ifdef _GLIBCXX_USE_CXX11_ABI
  ASSERT_TRUE(set_module_attr("_GLIBCXX_USE_CXX11_ABI", _GLIBCXX_USE_CXX11_ABI ? Py_True : Py_False));
#else
  ASSERT_TRUE(set_module_attr("_GLIBCXX_USE_CXX11_ABI", Py_False));
#endif
  auto& defaultGenerator = at::globalContext().defaultGenerator(at::kCPU);
  THPDefaultGenerator = (THPGenerator*)THPGenerator_NewWithGenerator(
    defaultGenerator);
  // This reference is meant to be given away, so no need to incref here.
  ASSERT_TRUE(set_module_attr("default_generator", (PyObject*)THPDefaultGenerator, /* incref= */ false));
#ifdef USE_NUMPY
  if (_import_array() < 0) return nullptr;
#endif
  torch::nn::init__THNN(module);
#ifdef USE_CUDA
  torch::nn::init__THCUNN(module);
#endif
  return module;
  END_HANDLE_TH_ERRORS
}
```
这里可以看到很多我们常用的torch函数或对象的身影，那么这些对象是如何添加到模块中去的呢？以TorchMethods为例，它包括了一系列的应用函数，其定义为
```cpp
static PyMethodDef TorchMethods[] = {
  {"_initExtension",  (PyCFunction)THPModule_initExtension,   METH_O,       nullptr},
  {"_autograd_init",  (PyCFunction)THPAutograd_initExtension, METH_NOARGS,  nullptr},
  {"_add_docstr",     (PyCFunction)THPModule_addDocStr,       METH_VARARGS, nullptr},
  {"_init_names",     (PyCFunction)THPModule_initNames,       METH_O,       nullptr},
  {"_has_distributed",(PyCFunction)THPModule_hasDistributed,  METH_NOARGS,  nullptr},
  {"_safe_call",      (PyCFunction)THPModule_safeCall,          METH_VARARGS | METH_KEYWORDS, nullptr},
  {"_set_default_tensor_type", (PyCFunction)THPModule_setDefaultTensorType, METH_O, nullptr},
  {"_set_default_dtype", (PyCFunction)THPModule_setDefaultDtype, METH_O, nullptr},
  {"_infer_size",     (PyCFunction)THPModule_inferSize,         METH_VARARGS, nullptr},
  {"_crash_if_csrc_asan", (PyCFunction)THPModule_crashIfCsrcASAN, METH_O, nullptr},
  {"_crash_if_csrc_ubsan", (PyCFunction)THPModule_crashIfCsrcUBSAN, METH_O, nullptr},
  {"_crash_if_aten_asan", (PyCFunction)THPModule_crashIfATenASAN, METH_O, nullptr},
  {"_set_backcompat_broadcast_warn", (PyCFunction)THPModule_setBackcompatBroadcastWarn, METH_O, nullptr},
  {"_get_backcompat_broadcast_warn", (PyCFunction)THPModule_getBackcompatBroadcastWarn, METH_NOARGS, nullptr},
  {"_set_backcompat_keepdim_warn", (PyCFunction)THPModule_setBackcompatKeepdimWarn, METH_O, nullptr},
  {"_get_backcompat_keepdim_warn", (PyCFunction)THPModule_getBackcompatKeepdimWarn, METH_NOARGS, nullptr},
  {"get_num_threads", (PyCFunction)THPModule_getNumThreads,     METH_NOARGS,  nullptr},
  {"set_num_threads", (PyCFunction)THPModule_setNumThreads,     METH_O,       nullptr},
  {"_get_cudnn_enabled", (PyCFunction)THPModule_userEnabledCuDNN, METH_NOARGS,     nullptr},
  {"_set_cudnn_enabled", (PyCFunction)THPModule_setUserEnabledCuDNN, METH_O,  nullptr},
  {"_get_cudnn_benchmark", (PyCFunction)THPModule_benchmarkCuDNN, METH_NOARGS,     nullptr},
  {"_set_cudnn_benchmark", (PyCFunction)THPModule_setBenchmarkCuDNN, METH_O,  nullptr},
  {"_get_cudnn_deterministic", (PyCFunction)THPModule_deterministicCuDNN, METH_NOARGS,     nullptr},
  {"_set_cudnn_deterministic", (PyCFunction)THPModule_setDeterministicCuDNN, METH_O,  nullptr},
  {"_to_dlpack",      (PyCFunction)THPModule_toDLPack,          METH_O,       nullptr},
  {"_from_dlpack",    (PyCFunction)THPModule_fromDLPack,        METH_O,       nullptr},
  {"set_flush_denormal", (PyCFunction)THPModule_setFlushDenormal, METH_O,     nullptr},
  {"get_default_dtype", (PyCFunction)THPModule_getDefaultDtype, METH_NOARGS,  nullptr},
  {"_is_default_type_cuda", (PyCFunction)THPModule_isDefaultTypeCuda, METH_NOARGS,  nullptr},
  {nullptr, nullptr, 0, nullptr}
};
```
这些函数通过THPUtils_addPyMethodDefs（torch/csrc/utils.cpp）函数添加到methods向量（static std::vector<PyMethodDef> methods）中
```cpp
void THPUtils_addPyMethodDefs(std::vector<PyMethodDef>& vector, PyMethodDef* methods)
{
  if (!vector.empty()) {
    // remove nullptr terminator
    vector.pop_back();
  }
  while (true) {
    vector.push_back(*methods);
    if (!methods->ml_name) {
      break;
    }
    methods++;
  }
}
```
这里顺便看一下定义：PyMethodDef是python的方法定义（/usr/local/include/python3.7dm/methodobject.h）
```
struct PyMethodDef {
    const char  *ml_name;   /* The name of the built-in function/method */
    PyCFunction ml_meth;    /* The C function that implements it */
    int         ml_flags;   /* Combination of METH_xxx flags, which mostly
                               describe the args expected by the C func */
    const char  *ml_doc;    /* The __doc__ attribute, or NULL */
};
typedef struct PyMethodDef PyMethodDef;
```
另外一个重点是，那些大量的初始化函数（比如：THPDoubleStorage_init，THDPDoubleStorage_init(PyObject *module);）在哪里？文件中搜索是找不到的。
在torch/csrc/StorageDefs.h中有这样一个定义（这个定义和Python中的很多结构体如PyLong_Object是差不多的），
```cpp
/****StorageDefs.h******/
#pragma once
struct THPStorage {
  PyObject_HEAD
  THWStorage *cdata;
};
/****THP.h******/
#define THWStorage THStorage
#define THWStorage_(NAME) THStorage_(NAME)
#define THWTensor THTensor
#define THWTensor_(NAME) THTensor_(NAME)
/*****torch/lib/temp_install/include/th/THGeneral.h*****/
#define TH_CONCAT_4_EXPAND(x,y,z,w) x ## y ## z ## w
#define TH_CONCAT_4(x,y,z,w) TH_CONCAT_4_EXPAND(x,y,z,w)
/*****torch/lib/include/th/THStorageFunctions.h*****/
#define THStorage_(NAME) TH_CONCAT_4(TH,Real,Storage_,NAME)
/*****torch/lib/include/th/THTensor.h*****/
#define THTensor_(NAME)   TH_CONCAT_4(TH,Real,Tensor_,NAME)
/*****torch/lib/include/th/generic/THTensor.h*****/
#define THFloatTensor THTensor
#define THDoubleTensor THTensor
#define THHalfTensor THTensor
#define THByteTensor THTensor
#define THCharTensor THTensor
#define THShortTensor THTensor
#define THIntTensor THTensor
#define THLongTensor THTensor
/*****torch/lib/tmp_install/include/torch/csrc/Storage.h*****/
#define THPStorageStr TH_CONCAT_STRING_3(torch.,Real,Storage)
#define THPStorageClass TH_CONCAT_3(THP,Real,StorageClass)
#define THPStorage_(NAME) TH_CONCAT_4(THP,Real,Storage_,NAME)
```
看起来还是比较直观的，比如NAME = init, Real = Double，那么
**THStorage_init -------> THDoubleStorage_init同时我们会发现，THPStorage_(NAME)和THStorage_(NAME)的定义方式完全一致。**
这个Ｒeal的定义又从哪里来呢？接着找，发现在torch/lib/include/THC以及/torch/lib/tmp_install/include/THC目录下面有一系列的
THCGenerateXXXXType.h 文件，如THCGenerateDoubleType.h，　THCGenerateFloatType.h
我们打开其中一个THCGenerateDoubleType.h看一眼，
```cpp
#ifndef THC_GENERIC_FILE
#error "You must define THC_GENERIC_FILE before including THGenerateDoubleType.h"
#endif
#define scalar_t double
#define accreal double
#define Real Double
#define CReal CudaDouble
#define THC_REAL_IS_DOUBLE
#line 1 THC_GENERIC_FILE
#include THC_GENERIC_FILE
#undef scalar_t
#undef accreal
#undef Real
#undef CReal
#undef THC_REAL_IS_DOUBLE
#ifndef THCGenerateAllTypes
#ifndef THCGenerateFloatTypes
#undef THC_GENERIC_FILE
#endif
#endif
```
可以看到，这里定义了#define Real Double和THC_REAL_IS_DOUBLE。
接下来，我们看一下torch/csrc/generic/Storage.h，这里有我们辛苦要找的bool THPStorage_(init)(PyObject *module)，
```cpp
#ifndef TH_GENERIC_FILE
#define TH_GENERIC_FILE "torch/csrc/generic/Storage.h"
#else
#include <torch/csrc/StorageDefs.h>
THP_API PyObject * THPStorage_(New)(THWStorage *ptr);
extern PyObject *THPStorageClass;
#ifdef _THP_CORE
#include <torch/csrc/Types.h>
bool THPStorage_(init)(PyObject *module);
void THPStorage_(postInit)(PyObject *module);
extern PyTypeObject THPStorageType;
#endif
#endif
```
其函数就定义在Storage.cpp中，
```cpp
bool THPStorage_(init)(PyObject *module)
{
  static std::vector<PyMethodDef> methods;
  THPUtils_addPyMethodDefs(methods, THPStorage_(methods));
#ifndef THD_GENERIC_FILE
  THPUtils_addPyMethodDefs(methods, THPStorage_(sharingMethods));
#endif
  THPStorageType.tp_methods = methods.data();
  THPStorageType.tp_members = THPStorage_(members);
  if (PyType_Ready(&THPStorageType) < 0)
    return false;
  Py_INCREF(&THPStorageType);
  PyModule_AddObject(module, THPStorageBaseStr, (PyObject *)&THPStorageType);
  THPStorage_(initCopyMethods)();
  return true;
}
```
THPStorage_方法的实现在torch/csrc/generic/StorageMethods.cpp中，
```cpp
static PyMethodDef THPStorage_(methods)[] = {
  {"copy_", (PyCFunction)THPStorage_(copy_), METH_VARARGS | METH_KEYWORDS, nullptr},
  {"element_size", (PyCFunction)THPStorage_(elementSize), METH_NOARGS, nullptr},
  {"fill_", (PyCFunction)THPStorage_(fill_), METH_O, nullptr},
  {"new", (PyCFunction)THPStorage_(new), METH_NOARGS, nullptr},
  {"resize_", (PyCFunction)THPStorage_(resize_), METH_O, nullptr},
  {"size", (PyCFunction)THPStorage_(size), METH_NOARGS, nullptr},
#ifndef THD_GENERIC_FILE
  {"data_ptr", (PyCFunction)THPStorage_(dataPtr), METH_NOARGS, nullptr},
  {"is_pinned", (PyCFunction)THPStorage_(isPinned), METH_NOARGS, nullptr},
  {"_write_file", (PyCFunction)THPStorage_(writeFile), METH_VARARGS, nullptr},
  {"_new_with_file", (PyCFunction)THPStorage_(newWithFile), METH_O | METH_STATIC, nullptr},
  {"_set_from_file", (PyCFunction)THPStorage_(setFromFile), METH_VARARGS, nullptr},
#endif // !defined(THD_GENERIC_FILE)
#if !defined(THC_GENERIC_FILE) && !defined(THD_GENERIC_FILE)
  {"from_buffer", (PyCFunction)THPStorage_(fromBuffer), METH_VARARGS | METH_KEYWORDS | METH_STATIC, nullptr},
#endif
  {"from_file", (PyCFunction)THPStorage_(fromFile), METH_VARARGS | METH_KEYWORDS | METH_STATIC, nullptr},
#ifdef THC_GENERIC_FILE
  {"get_device", (PyCFunction)THPStorage_(getDevice), METH_NOARGS, nullptr},
#endif
  {"_set_cdata", (PyCFunction)THPStorage_(_setCdata), METH_O, nullptr},
#ifndef THD_GENERIC_FILE
#endif
  {nullptr}
};
```
我们任取其中一个函数，比如THPStorage_(new)来看一下，
```cpp
static PyObject * THPStorage_(new)(THPStorage *self)
{
  HANDLE_TH_ERRORS
  THWStoragePtr new_storage(THWStorage_(new)(LIBRARY_STATE_NOARGS));
  PyObject *_ret = THPStorage_(New)(new_storage);
  new_storage.release();
  return _ret;
  END_HANDLE_TH_ERRORS
}
```
THPStorage 的类型对象是 THP<Type>Storage，它的通用定义在 csrc/generic/Storage.cpp 文件中。Pytorch 里有很多类似如下的代码
THWStoragePtr new_storage(THWStorage_(new)(LIBRARY_STATE_NOARGS));
new_storage定义在torch/Storage.py中，
```python
class _StorageBase(object):
    ...    
    def __deepcopy__(self, memo):
        memo = memo.setdefault('torch', {})
        if self._cdata in memo:
            return memo[self._cdata]
        new_storage = self.clone()
        memo[self._cdata] = new_storage
        return new_storage
```
从storage.h上可以看到，
\#define THStorage at::StorageImpl
进一步追踪就不难发现，这个THStorage的实现StorageImpl定义在c10/StorageImpl.h中。
至此，全部材料都已经准备好了，现在我们更进一步，找一找到底是谁，在哪里，把这个通用的THPStorage_(init)变成了THP<type>Storage_(init)，如THPDoubleStorage_init，THPFloatStorage_init，THPIntStorage_init，THPLongStorage_init。
## 文件split_types.py
文件tools/setup_helpers/split_types.py的内容如下，
```python
import os
import sys
this_file = os.path.dirname(os.path.abspath(__file__))
generated_dir = os.path.abspath(os.path.join(this_file, '..', '..', 'torch', 'csrc', 'generated'))
line_start = '//generic_include '
types = [
    'Double',
    'Float',
    'Half',
    'Long',
    'Int',
    'Short',
    'Char',
    'Byte'
]
generic_include = '#define {lib}_GENERIC_FILE "{path}"'
generate_include = '#include "{lib}/{lib}Generate{type}Type.h"'
def get_gen_path_prefix(file_name):
    gen_name_prefix = file_name[len('torch/csrc/'):].replace('/', '_').replace('.cpp', '')
    gen_path_prefix = os.path.join(generated_dir, gen_name_prefix)
    return gen_path_prefix
def split_types_ninja(file_name, w):
    gen_path_prefix = get_gen_path_prefix(file_name)
    to_build = [gen_path_prefix + t + '.cpp' for t in types]
    myself = 'tools/setup_helpers/split_types.py'
    cmd = "{} {} '{}'".format(sys.executable, myself, file_name)
    w.writer.build(
        to_build, 'do_cmd', [file_name, myself],
        variables={
            'cmd': cmd,
        })
    return to_build
def split_types(file_name, ninja_global):
    # when ninja is enabled we just generate the build rule here
    if ninja_global is not None:
        return split_types_ninja(file_name, ninja_global)
    assert file_name.startswith('torch/csrc/')
    if not os.path.exists(generated_dir):
        os.makedirs(generated_dir)
    with open(file_name, 'r') as f:
        lines = f.read().split('\n')
    # Find //generic_include
    for i, l in enumerate(lines):
        if l.startswith(line_start):
            args = l[len(line_start):]
            lib_prefix, generic_file = filter(bool, args.split())
            break
    else:
        raise RuntimeError("generic include not found")
    gen_name_prefix = file_name[len('torch/csrc/'):].replace('/', '_').replace('.cpp', '')
    gen_path_prefix = os.path.join(generated_dir, gen_name_prefix)
    prefix = '\n'.join(lines[:i])
    suffix = '\n'.join(lines[i + 1:])
    to_build = []
    g_include = generic_include.format(lib=lib_prefix, path=generic_file)
    for t in types:
        t_include = generate_include.format(lib=lib_prefix, type=t)
        gen_path = gen_path_prefix + t + '.cpp'
        to_build.append(gen_path)
        with open(gen_path, 'w') as f:
            f.write(prefix + '\n' +
                    g_include + '\n' +
                    t_include + '\n' +
                    suffix)
    return to_build
# when called from ninja
if __name__ == '__main__':
    file_name = sys.argv[1].strip("'")
    split_types(file_name, None)
```
该文件在运行python setup.py build 或 install的时候会被运行，这里我们可以清楚地看到，它定义了常见的类型types。程序运行时它会在torch/csrc下创建一个文件夹generated，然后在其中生成一系列的和Storage.cpp对应的文件THP<type>Storage_(xxx)。也就是说，对应不同的types，setup.py会在先生成一系列的类型相关的临时文件，然后再进行编译，得到我们想要的编译结果。更详细的信息我这里不再展开，推荐大家看后面给出的参考资料，除了版本比较老旧以外，那里描述得也比我细致。
## Torch.Tensor
我们常用的torch.Tensor是在torch/tensor.py中定义的，
```python
class Tensor(torch._C._TensorBase):
    def __deepcopy__(self, memo):
        ...
    def __reduce_ex__(self, proto):
        ...
```
这个TensorBase的定义，是在torch/csrc/autograd/Python_variable.cpp中，它是一个普通的PyTypeObject，
```python
PyTypeObject THPVariableType = {
  PyVarObject_HEAD_INIT(nullptr, 0)
  "torch._C._TensorBase",                /* tp_name */
  sizeof(THPVariable),                   /* tp_basicsize */
  0,                                     /* tp_itemsize */
  (destructor)THPVariable_dealloc,       /* tp_dealloc */
  nullptr,                                     /* tp_print */
  nullptr,                                     /* tp_getattr */
  nullptr,                                     /* tp_setattr */
  nullptr,                                     /* tp_reserved */
  nullptr,                                     /* tp_repr */
  nullptr,                                     /* tp_as_number */
  nullptr,                                     /* tp_as_sequence */
  &THPVariable_as_mapping,               /* tp_as_mapping */
  nullptr,                                     /* tp_hash  */
  nullptr,                                     /* tp_call */
  nullptr,                                     /* tp_str */
  nullptr,                                     /* tp_getattro */
  nullptr,                                     /* tp_setattro */
  nullptr,                                     /* tp_as_buffer */
  Py_TPFLAGS_DEFAULT | Py_TPFLAGS_BASETYPE | Py_TPFLAGS_HAVE_GC, /* tp_flags */
  nullptr,                               /* tp_doc */
  (traverseproc)THPVariable_traverse,    /* tp_traverse */
  (inquiry)THPVariable_clear,            /* tp_clear */
  nullptr,                                     /* tp_richcompare */
  0,                                     /* tp_weaklistoffset */
  nullptr,                                     /* tp_iter */
  nullptr,                                     /* tp_iternext */
  nullptr,                                     /* tp_methods */
  nullptr,                                     /* tp_members */
  THPVariable_properties,                /* tp_getset */
  nullptr,                                     /* tp_base */
  nullptr,                                     /* tp_dict */
  nullptr,                                     /* tp_descr_get */
  nullptr,                                     /* tp_descr_set */
  0,                                     /* tp_dictoffset */
  nullptr,                                     /* tp_init */
  nullptr,                                     /* tp_alloc */
  THPVariable_pynew                      /* tp_new */
};
```
同时也定义了初始化函数THPVariable_initModule
```python
bool THPVariable_initModule(PyObject *module)
{
  static std::vector<PyMethodDef> methods;
  THPUtils_addPyMethodDefs(methods, torch::autograd::variable_methods);
  THPUtils_addPyMethodDefs(methods, extra_methods);
  THPVariableType.tp_methods = methods.data();
  if (PyType_Ready(&THPVariableType) < 0)
    return false;
  Py_INCREF(&THPVariableType);
  PyModule_AddObject(module, "_TensorBase",   (PyObject *)&THPVariableType);
  torch::autograd::initTorchFunctions(module);
  return true;
}
```
其中非常明显的一句：PyModule_AddObject(module, "_TensorBase",   (PyObject *)&THPVariableType);把_TensorBase和THPVariableType关联起来。此时，我们回过头来看最前面提到的torch/csrc/module.cpp中的initModule函数，可以发现其对应的调用，
```cpp
ASSERT_TRUE(THPVariable_initModule(module));
ASSERT_TRUE(THPFunction_initModule(module));
ASSERT_TRUE(THPEngine_initModule(module));
```
就可以进一步对应上这个模块的初始化过程，也不难明白，为什么说PyTorch中的Tensor的底层数据结构是Storage了。
关于THTensor,THStorage的关系等，请参考后面的资料《PyTorch – Internal Architecture Tour》。
参考资料：
<https://www.pytorchtutorial.com/how-to-implement-tensor-in-pytorch/>
<https://blog.csdn.net/g11d111/article/details/81231292>
<https://www.jianshu.com/p/91af2ab867d5>
PyTorch – Internal Architecture Tour
<http://blog.christianperone.com/2018/03/pytorch-internal-architecture-tour/>
A Tour of PyTorch Internals (Part I)
<https://pytorch.org/blog/a-tour-of-pytorch-internals-1/>
PyTorch Internals Part II – The Build System
<https://pytorch.org/blog/a-tour-of-pytorch-internals-2/>
