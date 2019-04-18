# 机器视觉 OpenCV—python主要方法介绍 - wsp_1138886114的博客 - CSDN博客





2018年10月12日 16:07:03[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：286








当前主要介绍OpenCV的视频功能，关于图像的一些处理方法请查看之前的博客。OpenCV在视频和视觉方面的函数接口如下：
- video：用于视频分析的常用功能，比如光流法（Optical Flow）和目标跟踪等。
- calib3d：三维重建，立体视觉和相机标定等的相关功能。
- features2d：二维特征相关的功能，主要是一些不受专利保护的，商业友好的特征点检测和匹配等功能，比如ORB特征。
- object：目标检测模块，包含级联分类和Latent SVM
- ml：机器学习算法模块，包含一些视觉中最常用的传统机器学习算法。
- flann：最近邻算法库，Fast Library for Approximate Nearest Neighbors，用于在多维空间进行聚类和检索，经常和关键点匹配搭配使用。
- gpu：包含了一些gpu加速的接口，底层的加速是CUDA实现。
- photo：计算摄像学（Computational Photography）相关的接口，当然这只是个名字，其实只有图像修复和降噪而已。
- stitching：图像拼接模块，有了它可以自己生成全景照片。
- nonfree：受到专利保护的一些算法，其实就是SIFT和SURF。
- contrib：一些实验性质的算法，考虑在未来版本中加入的。
- legacy：字面是遗产，意思就是废弃的一些接口，保留是考虑到向下兼容。
- ocl：利用OpenCL并行加速的一些接口。
- superres：超分辨率模块，其实就是BTV-L1（Biliteral Total Variation – L1 regularization）算法
- viz：基础的3D渲染模块，其实底层就是著名的3D工具包VTK（Visualization Toolkit）。

与OpenCV2相比，OpenCV3的主要变化是更多的功能和更细化的模块划分。

未完待续

接下来主要写一些常见接口函数使用方法。



