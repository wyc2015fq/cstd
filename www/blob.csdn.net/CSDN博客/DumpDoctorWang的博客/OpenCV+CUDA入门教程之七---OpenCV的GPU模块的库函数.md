# OpenCV+CUDA入门教程之七---OpenCV的GPU模块的库函数 - DumpDoctorWang的博客 - CSDN博客





2018年07月18日 21:02:01[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：662








**目录**

[一、矩阵运算](#%E4%B8%80%E3%80%81%E7%9F%A9%E9%98%B5%E8%BF%90%E7%AE%97)

[二、元素操作](#%E4%BA%8C%E3%80%81%E5%85%83%E7%B4%A0%E6%93%8D%E4%BD%9C)

[三、图像处理](#%E4%B8%89%E3%80%81%E5%9B%BE%E5%83%8F%E5%A4%84%E7%90%86)

[四、GpuMat运算](#%E5%9B%9B%E3%80%81GpuMat%E8%BF%90%E7%AE%97)

[五、目标识别](#%E4%BA%94%E3%80%81%E7%9B%AE%E6%A0%87%E8%AF%86%E5%88%AB)

[六、特征检测与描述](#%E5%85%AD%E3%80%81%E7%89%B9%E5%BE%81%E6%A3%80%E6%B5%8B%E4%B8%8E%E6%8F%8F%E8%BF%B0)

[七、图像滤波](#%E4%B8%83%E3%80%81%E5%9B%BE%E5%83%8F%E6%BB%A4%E6%B3%A2)

[八、相机校正及3D重建](#%E5%85%AB%E3%80%81%E7%9B%B8%E6%9C%BA%E6%A0%A1%E6%AD%A3%E5%8F%8A3D%E9%87%8D%E5%BB%BA)

[九、光流](#%E4%B9%9D%E3%80%81%E5%85%89%E6%B5%81)

[十、OpenCV3新增](#%E5%8D%81%E3%80%81OpenCV3%E6%96%B0%E5%A2%9E)

 OpenCV的GPU模块中实现了很多基本图像函数和算法，只不过把参数从从Mat变成了GpuMat。本篇博客旨在提供一个目录供大家参考。在2.4中，GPU模块有单独的文档，在3.4中，GPU模块与其他文档混合在了一起。
- OpenCV2.4 [GPU模块文档](https://docs.opencv.org/2.4/modules/gpu/doc/gpu.html)
- OpenCV3.4 [文档](https://docs.opencv.org/3.4.0/)

# 一、矩阵运算

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/operations_on_matrices.html](https://docs.opencv.org/2.4/modules/gpu/doc/operations_on_matrices.html)

3.4.* [https://docs.opencv.org/3.4.0/de/d09/group__cudaarithm__core.html](https://docs.opencv.org/3.4.0/de/d09/group__cudaarithm__core.html)

# 二、元素操作

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/per_element_operations.html](https://docs.opencv.org/2.4/modules/gpu/doc/per_element_operations.html)

3.4.* [https://docs.opencv.org/3.4.0/d8/d34/group__cudaarithm__elem.html](https://docs.opencv.org/3.4.0/d8/d34/group__cudaarithm__elem.html)

# 三、图像处理

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/image_processing.html](https://docs.opencv.org/2.4/modules/gpu/doc/image_processing.html)

3.4.* [https://docs.opencv.org/3.4.0/d0/d05/group__cudaimgproc.html](https://docs.opencv.org/3.4.0/d0/d05/group__cudaimgproc.html)

# 四、GpuMat运算

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/matrix_reductions.html](https://docs.opencv.org/2.4/modules/gpu/doc/matrix_reductions.html)

3.4.* [https://docs.opencv.org/3.4.0/d5/de6/group__cudaarithm__reduce.html](https://docs.opencv.org/3.4.0/d5/de6/group__cudaarithm__reduce.html)

# 五、目标识别

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/object_detection.html](https://docs.opencv.org/2.4/modules/gpu/doc/object_detection.html)

3.4.* [https://docs.opencv.org/3.4.0/d9/d3f/group__cudaobjdetect.html](https://docs.opencv.org/3.4.0/d9/d3f/group__cudaobjdetect.html)

# 六、特征检测与描述

2.4.*[https://docs.opencv.org/2.4/modules/gpu/doc/feature_detection_and_description.html](https://docs.opencv.org/2.4/modules/gpu/doc/feature_detection_and_description.html)

3.4.* [https://docs.opencv.org/3.4.0/d6/d1d/group__cudafeatures2d.html](https://docs.opencv.org/3.4.0/d6/d1d/group__cudafeatures2d.html)

# 七、图像滤波

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/image_filtering.html](https://docs.opencv.org/2.4/modules/gpu/doc/image_filtering.html)

3.4.* [https://docs.opencv.org/3.4.0/dc/d66/group__cudafilters.html](https://docs.opencv.org/3.4.0/dc/d66/group__cudafilters.html)

# 八、相机校正及3D重建

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/camera_calibration_and_3d_reconstruction.html](https://docs.opencv.org/2.4/modules/gpu/doc/camera_calibration_and_3d_reconstruction.html)

3.4.* [https://docs.opencv.org/3.4.0/dd/d47/group__cudastereo.html](https://docs.opencv.org/3.4.0/dd/d47/group__cudastereo.html)

# 九、光流

2.4.* [https://docs.opencv.org/2.4/modules/gpu/doc/video.html](https://docs.opencv.org/2.4/modules/gpu/doc/video.html)

3.4.* [https://docs.opencv.org/3.4.0/d7/d3f/group__cudaoptflow.html](https://docs.opencv.org/3.4.0/d7/d3f/group__cudaoptflow.html)

# 十、OpenCV3新增

视频编码、解码 [https://docs.opencv.org/3.4.0/d0/d61/group__cudacodec.html](https://docs.opencv.org/3.4.0/d0/d61/group__cudacodec.html)

背景分割 [https://docs.opencv.org/3.4.0/d6/d17/group__cudabgsegm.html](https://docs.opencv.org/3.4.0/d6/d17/group__cudabgsegm.html)

向前支持 [https://docs.opencv.org/3.4.0/d5/dc3/group__cudalegacy.html](https://docs.opencv.org/3.4.0/d5/dc3/group__cudalegacy.html)

CUDA开发层 [https://docs.opencv.org/3.4.0/df/dfc/group__cudev.html](https://docs.opencv.org/3.4.0/df/dfc/group__cudev.html)



