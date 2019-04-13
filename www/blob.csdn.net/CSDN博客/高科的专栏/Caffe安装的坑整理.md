
# Caffe安装的坑整理 - 高科的专栏 - CSDN博客

2018年09月28日 15:33:03[高科](https://me.csdn.net/pbymw8iwm)阅读数：743


怎么说了，入了深度学习的坑，就要踩一踩才算你入门，这里我整理了我在安装学习caffe自己遇到的坑：
# 1.Caffe-GPU编译问题：nvcc fatal : Unsupported gpu architecture 'compute_20'
仔细查看了一下 Makefile.config 中 CUDA_ARCH 设置未按规定设置：
`# CUDA architecture setting: going with all of them.
# For CUDA < 6.0, comment the *_50 through *_61 lines for compatibility.
# For CUDA < 8.0, comment the *_60 and *_61 lines for compatibility.
# For CUDA >= 9.0, comment the *_20 and *_21 lines for compatibility.
CUDA_ARCH := -gencode arch=compute_20,code=sm_20 \
-gencode arch=compute_20,code=sm_21 \
-gencode arch=compute_30,code=sm_30 \
-gencode arch=compute_35,code=sm_35 \
-gencode arch=compute_50,code=sm_50 \
-gencode arch=compute_52,code=sm_52 \
-gencode arch=compute_60,code=sm_60 \
-gencode arch=compute_61,code=sm_61 \
-gencode arch=compute_61,code=compute_61`
```python
因为我装的是CUDA9.0所以把下面这两行删除就可以了
```
`-gencode arch=compute_20,code=sm_20 \
-gencode arch=compute_20,code=sm_21 \`
# 2. ./include/caffe/util/hdf5.hpp:7:18: fatal error: hdf5.h: No such file or directory
`In file included from src/caffe/solvers/sgd_solver.cpp:5:0:
./include/caffe/util/hdf5.hpp:7:18: fatal error: hdf5.h: No such file or directory
compilation terminated.
Makefile:591: recipe for target '.build_release/src/caffe/solvers/sgd_solver.o' failed
make: *** [.build_release/src/caffe/solvers/sgd_solver.o] Error`发现是依赖出问题了，我已经安装了hdf5了，
root@lxsj-ThinkStation:~/caffe\# whereis hdf5
hdf5: /usr/include/hdf5
所以修改Makefile.config
`# Whatever else you find you need goes here.
INCLUDE_DIRS := $(PYTHON_INCLUDE) /usr/include/hdf5/serial /usr/local/include
LIBRARY_DIRS := $(PYTHON_LIB) /usr/lib/x86_64-linux-gnu/hdf5/serial /usr/local/lib /usr/lib`重新编译ok了

