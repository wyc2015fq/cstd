# pytorch学习笔记（十五）：pytorch 源码编译碰到的坑总结 - Keith - CSDN博客





2017年11月17日 09:10:19[ke1th](https://me.csdn.net/u012436149)阅读数：5499
所属专栏：[pytorch学习笔记](https://blog.csdn.net/column/details/15023.html)









2017.11.17

最近打算学习一下 `pytorch` 源码，所以按照官网的教程从头编译了一下 `pytorch` 。在编译的过程中，碰到了两个坑，在这里记录一下。

**源码编译流程**
- 需要 anaconda
- 如果要编译cuda 版本的话 CUDA7.5 及以上。Cudnn 6 及以上
- 如果不想编译 cuda 版本的话：命令行执行 `export NO_CUDA=1`

```
1. export CMAKE_PREFIX_PATH="$(dirname $(which conda))/../"
2. conda install numpy pyyaml mkl setuptools cmake cffi
3. conda install -c soumith magma-cuda80 (这个可装可不装)
4. git clone --recursive https://github.com/pytorch/pytorch （下载源码）
5. cd pytorch
6. python setup.py install (坐等编译安装 完毕。)
```

**以下是碰到的几个错误**
- 第一个错误

```
CMakeFiles/Makefile2:264: recipe for target 'src/ATen/test/CMakeFiles/scalar_test.dir/all' failed
make[1]: *** [src/ATen/test/CMakeFiles/scalar_test.dir/all] Error 2
Makefile:127: recipe for target 'all' failed
make: *** [all] Error 2
```

```
解决方法是：（由于我是 ubuntu16.04 编译的， gcc 默认的版本是 5.4，但是用 4.9 编译就不会有问题）
sudo apt install gcc-4.9 g++-4.9
export CC="gcc-4.9"
export CXX="g++-4.9"
# change to pytorch directory
python setup.py clean
python setup.py install
```
- 第二个错误:(编译安装成功后， import torch 时报这个错)
`libstdc++.so.6: version `GLIBCXX_3.4.21' not found`
```java
解决方法：
conda install libgcc 
然后再 import 就没问题了
```
- 第三个错误（编译安装成功后，import torch 时报的错）
`ModuleNotFoundError: No module named 'torch._C'`
```
这个错误是 因为 我直接在 `pytorch/` 源码路径下打开的 `ipython`。 `cd` 出来就好了
the problem is that you have a folder called torch in the same directory which is being picked up. Do this: cd .. (to change directory), and then start python and import torch, it should work.
```



