# OpenCV中GPU模块使用 - wishchinYang的专栏 - CSDN博客
2013年12月31日 16:38:20[wishchin](https://me.csdn.net/wishchin)阅读数：1009
个人分类：[BigDataMini																[CUDA](https://blog.csdn.net/wishchin/article/category/1846367)](https://blog.csdn.net/wishchin/article/category/1834057)
### CUDA IT168的文章系列：
### Cuda的初始化[：http://tech.it168.com/a2011/0715/1218/000001218458.shtml](http://tech.it168.com/a2011/0715/1218/000001218458.shtml)[](http://blog.csdn.net/augusdi/article/details/12751587)
### [OpenCV: OpenCV中GPU模块使用](http://blog.csdn.net/augusdi/article/details/12751587)
**CUDA基本使用方法**
在介绍OpenCV中GPU模块使用之前，先回顾下CUDA的一般使用方法，其基本步骤如下：
1.主机代码执行；2.传输数据到GPU；3.确定grid，block大小；
4.调用内核函数，GPU运行程序；5.传输结果到CPU；6.继续主机代码执行。
下图是两个向量相加的简单示例程序和处理流图。
![](http://images.cnitblog.com/blog/414008/201308/07211246-96c69d0825654c12a454d176e1b16ca4.png)
注意的问题：cu，cpp文件的组织
内核函数和其wrapper函数置于cu文件中。
在cpp文件声明wrapper函数，并调用wrapper函数。
wrapper函数的声明定义需加extern "C"。
OpenCV中GPU模块的使用
使用的步骤与CUDA的基本使用方法类似，只是OpenCV中GPU模块，已经封装的内核函数的调用，其使用步骤如下：
1.验证OpenCV是否已启用GPU模块。
2.上传待处理数据到GPU (Mat --> GpuMat)。
3.调用OpenCV支持的GPU的处理函数。
4.下载处理结果到CPU (GpuMat ---> Mat)。
其示例程序如下，完成颜色转换，BGR2GRAY。
![](http://images.cnitblog.com/blog/414008/201308/07212133-5512b1129b42402ca9545866ca77ff40.png)
[http://www.cnblogs.com/dwdxdy/p/3244508.html](http://www.cnblogs.com/dwdxdy/p/3244508.html)
