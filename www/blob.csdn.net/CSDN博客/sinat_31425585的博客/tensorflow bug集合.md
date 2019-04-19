# tensorflow bug集合 - sinat_31425585的博客 - CSDN博客
2019年04月17日 16:07:02[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：31
**1、TypeError: __new__() got an unexpected keyword argument 'serialized_options'**
原因：protobuf版本冲突
解决方法：
```cpp
pip uninstall protobuf
pip install -U protobuf
```
参考资料：
[https://blog.csdn.net/zxm1306192988/article/details/86545428](https://blog.csdn.net/zxm1306192988/article/details/86545428)
**2、undefined symbol: cblas_gemm_s8u8s32_pack**
解决方法：
```cpp
conda remove mkl mkl-include
conda install numpy pyyaml mkl=2019.3 mkl-include setuptools cmake cffi typing
```
参考资料：
[https://github.com/pytorch/pytorch/issues/18932](https://github.com/pytorch/pytorch/issues/18932)
~~~未完待续~~~
