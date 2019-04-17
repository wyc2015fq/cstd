# caffe make的时候出错：src/caffe/util/math_functions.cu(140): error: calling a host function("std::signbit - 家家的专栏 - CSDN博客





2014年09月23日 19:14:18[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：5284









转自：http://blog.csdn.net/u013476464/article/details/38071075





error提示“src/caffe/util/math_functions.cu(140): error: calling a host function("std::signbit
 ") from a globalfunction("caffe::sgnbit_kernel ") is not allowed”

解决办法：


修改  ./include/caffe/util/math_functions.hpp 224行

删除(注释)：using std::signbit;

修改：DEFINE_CAFFE_CPU_UNARY_FUNC(sgnbit, y[i] = signbit(x[i]));

为：DEFINE_CAFFE_CPU_UNARY_FUNC(sgnbit, y[i] = std::signbit(x[i]));

这个方法感谢网友：嗆熱DJ$998。







这个解决办法，亲测可用。。。



