# Caffe (CNN, deep learning ) - 家家的专栏 - CSDN博客





2014年05月25日 14:38:15[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：24315








Caffe -----------Convolution Architecture For Feature Embedding (Extraction) 

1、Caffe 是什么东东？

     CNN (Deep Learning) 工具箱

     C++ 语言架构

     CPU 和GPU 无缝交换

      Python 和matlab的封装

      但是，Decaf只是CPU 版本。




2、为什么要用Caffe？

      运算速度快。简单 友好的架构 用到的一些库：

**Google Logging library (Glog):**一个C++语言的应用级日志记录框架，提供了C++风格的流操作和各种助手宏.

**lebeldb**(数据存储)： 是一个google实现的非常高效的kv数据库，单进程操作。

**CBLAS library**（CPU版本的矩阵操作）

** CUBLAS library**
 (GPU 版本的矩阵操作)




3、Caffe 架构

![](https://img-blog.csdn.net/20140525142626234)





4、预处理图像的leveldb构建

     输入：一批图像和label
 （2和3）

     输出：leveldb （4）

     指令里包含如下信息： 

     1）conver_imageset （构建leveldb的可运行程序）

     2）train/  （此目录放处理的jpg或者其他格式的图像)  

     3）label.txt  (图像文件名及其label信息)


     4）输出的leveldb文件夹的名字 

     5）CPU/GPU (指定是在cpu上还是在gpu上运行code)




5、CNN网络配置文件

    1）Imagenet_solver.prototxt
   （包含全局参数的配置的文件）

    2）Imagenet.prototxt （包含训练网络的配置的文件）

    3）Imagenet_val.prototxt （包含测试网络的配置文件）




下载网址：[http://caffe.berkeleyvision.org/](http://caffe.berkeleyvision.org/)



