# caffe编译的问题解决：“cublas_v2.h: No such file or directory” - 战斗蜗牛的专栏 - CSDN博客





2016年08月04日 16:33:52[vbskj](https://me.csdn.net/vbskj)阅读数：27527
个人分类：[Deep Learning继续学习](https://blog.csdn.net/vbskj/article/category/6337222)












主要问题还是没有把cuda的头文件、库的路径放置到caffe的Makefile.config中；




具体来说：

CUDA7.5中的include、lib路径是安装目录下/usr/local/cuda-7.5/targets/x86_64-linux/下面的include和lib




将其分别添加到caffe根目录下Makefile.config中的"INCLUDE_DIRS"、“LIBRARY_DIRS”后面就可以了。




make all

make test




==========接着在make runtest的时候出现错误 core dumped

1、随机数的那个例子出错，解决：export MKL_CBWR=AUTO

2、test_common.cpp，出现的CUDA launch之类的错误，可能是因为使用了GTX1080以及CUDA7.5的原因，更新CUDA到8.0就没有这个错误了。



