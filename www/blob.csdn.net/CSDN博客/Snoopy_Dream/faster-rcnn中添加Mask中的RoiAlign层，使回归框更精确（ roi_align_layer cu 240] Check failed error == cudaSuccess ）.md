# faster-rcnn中添加Mask中的RoiAlign层，使回归框更精确（ roi_align_layer.cu:240] Check failed: error == cudaSuccess *） - Snoopy_Dream - CSDN博客





2018年05月28日 09:49:43[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：1158








具体的操作为什么这样做，可参照：

**[1.Caffe学习之自定义创建新的Layer层](https://blog.csdn.net/bvl10101111/article/details/74837156)**



###### [2.如何在caffe中自定义网络层](https://blog.csdn.net/thesby/article/details/50822250)

### ROI pooling层

说起ROI Alignment，就要说道faster-rcnn的ROI pooling， 


ROIpooling层结构是为了将**原图像的rois映射**到**固定大小的feature map**上。而此方法有一些缺点，会**带来边缘像素的缺失**

**[ROI Pooling层解析](https://blog.csdn.net/lanran2/article/details/60143861)**。

### ROI Align的作用
**[详解 ROI Align 的基本原理和实现细节](http://blog.leanote.com/post/afanti.deng@gmail.com/b5f4f526490b)**
![](https://img-blog.csdn.net/20180528114758517?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**如果roi大小为(7,6)，而roipooling是分成了(6,6)的部分，(7,6)到(6,6)的转换必然带来了边缘某像素的损失。而roialign利用双线性插值，将roi(7,6)插值扩充到(12,12)，此时再做(6,6)的roipooling，会提高精度，充分利用了roi的像素。**

用到的文件链接: [https://pan.baidu.com/s/1-f98btUdxX5h8bFTKRxLmA](https://pan.baidu.com/s/1-f98btUdxX5h8bFTKRxLmA) 密码: mypk

1.**~/py-faster-rcnn/caffe-fast-rcnn/include/caffe/layers** 添加**roi_align_layer.hpp**

2.**~/py-faster-rcnn/caffe-fast-rcnn/src/caffe/layers** 添加**roi_align_layer.cpp **roi_align_layer**.cu**

3**.~/py-faster-rcnn/caffe-fast-rcnn/src/caffe/proto** 中**caffe.proto**添加**add for roi align**以下的

```cpp
// Message that stores parameters used by ROIPoolingLayer
message ROIPoolingParameter {
  // Pad, kernel size, and stride are all given as a single value for equal
  // dimensions in height and width or as Y, X pairs.
  optional uint32 pooled_h = 1 [default = 0]; // The pooled output height
  optional uint32 pooled_w = 2 [default = 0]; // The pooled output width
  // Multiplicative spatial scale factor to translate ROI coords from their
  // input scale to the scale used when pooling
  optional float spatial_scale = 3 [default = 1];

  // add for roi align
  optional float pad_ratio = 4[default = 0];
  optional uint32 bi_type = 5 [default = 0];
  optional bool is_multi_interpolate = 6 [default = true];
```


4.**返回caffe-fast-rcnn路径**下 make clean ,make -j64,make pycaffe

5.修改model文件夹下面的prototxt模型文件，将ROIPooling换成ROIAlign

![](https://img-blog.csdn.net/20180516193531194?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





----------------------------------------      更新与2018年5月16日     --------------------------------------

如果你电脑出现下面这样的问题


```cpp
roi_align_layer.cu:240] Check failed: error == cudaSuccess (7 vs. 0)  too many resources requested for launch
```


![](https://img-blog.csdn.net/20180516100512446?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

**【解决方法】：**

1. 删除 **roi_align_layer.cu**文件

2. 打开**roi_align_layer.hpp**文件**注释掉**需要**gpu加速**的语句.（如下图）

3. **重新编译** make clean ,make -j64,make pycaffe


注意：如果不注释hpp的那一段，直接删除cu文件，重新编译会报错


![](https://img-blog.csdn.net/2018051610062828?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2UwMTUyOA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




