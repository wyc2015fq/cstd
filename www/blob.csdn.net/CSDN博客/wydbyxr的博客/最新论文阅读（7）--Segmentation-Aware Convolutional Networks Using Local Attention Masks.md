# 最新论文阅读（7）--Segmentation-Aware Convolutional Networks Using Local Attention Masks - wydbyxr的博客 - CSDN博客
2018年06月06日 22:03:27[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：307
所属专栏：[深度学习--最新论文](https://blog.csdn.net/column/details/23683.html)
# Segmentation-Aware Convolutional Networks Using Local Attention Masks
```
- 2017年8月
- 用于语义分割；提出Im2dist
- 卡内基梅隆；瑞尔森大学【加】；Facebook
```
　　DNN中，网络越深，神经元对图像进行“审查”的部分就越大，这可能会导致局部性（localized）变差以及模糊（神经元需要对图像中非常大的区域进行“审查”）。 
　　本文是通过每个神经元只出现在自己感兴趣的区域，进而锐化这种反应，即segmentation-aware CNN。 
　　官网地址：[http://www.cs.cmu.edu/~aharley/segaware/](http://www.cs.cmu.edu/~aharley/segaware/)
### 网络结构：
![网络结构](https://img-blog.csdn.net/20180606215251721?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[https://github.com/aharley/segaware/blob/master/scripts/segaware/config/vgg/deploy.prototxt](https://github.com/aharley/segaware/blob/master/scripts/segaware/config/vgg/deploy.prototxt)
### 各个网络子模块解释：
　　1）embedding-net 
　　VGG16，有两次pooling。 
　　在embedding-net后，先upsample到同一个尺寸（type: “Interp”，双线性），再concat，最后1*1的卷积。 
　　2）im2dist 
　　在matlab中，有imdistline，它是计算图像上两点的坐标距离。caffe代码在[https://github.com/aharley/segaware/blob/a758ed39aed2861895d07ac9b5c41f9a992f8e53/caffe/src/caffe/util/im2dist.cpp](https://github.com/aharley/segaware/blob/a758ed39aed2861895d07ac9b5c41f9a992f8e53/caffe/src/caffe/util/im2dist.cpp)
　　im2dist计算过程：当前中心点分别减去周围8个点的数值，得到8个值；然后求和。
```
输入：64 * 321 * 321
stride：1；pad：1；kernel的移动方式与卷积核一致
输出：9 * 321 * 321
```
　　3）im2dist之后 
　　im2dist之后会依次进行元素级的Scale、AbsVal、exp。得到mask（9*321*321）。 
　　该mask根据情况进行“tile”运算，扩大3倍或64倍，得到（3*9*321*321）或（64*9*321*321）。
　　4）im2col 
　　详情见caffe中的im2col。会将map变成矩阵，会扩大矩阵。另外，在caffe中，一般是im2col+caffe_cpu_gemm连用进行卷积运算。
　　输入是3*321*321（即C*H*W）
```
对于layer {
  bottom: ""data""
  top: ""im2col_conv1_1""
  name: ""im2col_conv1_1""
  type: ""Im2col""
  convolution_param {
    num_output: 64 kernel_size: 3 pad: 1
  }
}
```
　　输出是3*9*321*321(即C*K*K*H*W)，而不是64*9*321*321。因为num_output: 64不会有任何影响，该参数只说明之后的conv_layer会有64个channel的输出。
### 理解mask
![理解mask](https://img-blog.csdn.net/20180606220302419?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
