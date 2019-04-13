
# caffe-matalb-python中数据的存储方式 - 机器学习的小学生 - CSDN博客


2017年03月23日 16:57:45[机器学习的小学生](https://me.csdn.net/xuluhui123)阅读数：529



## caffe/matalb/python中数据的存储方式
下面的代码摘自：classification_demo.m
```python
% Usage:
%  im = imread(
```
```python
'../../examples/images/cat.jpg'
```
```python
);
%  scores = classification_demo(im,
```
```python
1
```
```python
);
%  [score,
```
```python
class
```
```python
] = max(scores);
% Five things
```
```python
to
```
```python
be aware
```
```python
of
```
```python
:
%   caffe
```
```python
uses
```
```python
row-major
```
```python
order
```
```python
%   matlab
```
```python
uses
```
```python
column-major
```
```python
order
```
```python
%   caffe
```
```python
uses
```
```python
BGR color channel
```
```python
order
```
```python
%   matlab
```
```python
uses
```
```python
RGB color channel
```
```python
order
```
```python
%   images need
```
```python
to
```
```python
have the data mean subtracted
% Data coming
```
```python
in
```
```python
from
```
```python
matlab needs
```
```python
to
```
```python
be
```
```python
in
```
```python
the
```
```python
order
```
```python
%   [width, height, channels, images]
%
```
```python
where
```
```python
width
```
```python
is
```
```python
the fastest dimension.
% Here
```
```python
is
```
```python
the rough matlab
```
```python
for
```
```python
putting image data
```
```python
into
```
```python
the correct
% format
```
```python
in
```
```python
W x H x C
```
```python
with
```
```python
BGR channels:
%   % permute channels
```
```python
from
```
```python
RGB
```
```python
to
```
```python
BGR
%   im_data = im(:, :, [
```
```python
3
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
]);
%   % flip width
```
```python
and
```
```python
height
```
```python
to
```
```python
make width the fastest dimension
%   im_data = permute(im_data, [
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
]);
%   % convert
```
```python
from
```
```python
uint8
```
```python
to
```
```python
single
%   im_data = single(im_data);
%   % reshape
```
```python
to
```
```python
a fixed size (e.g.,
```
```python
227
```
```python
x227).
%   im_data = imresize(im_data, [IMAGE_DIM IMAGE_DIM],
```
```python
'bilinear'
```
```python
);
%   % subtract mean_data (already
```
```python
in
```
```python
W x H x C
```
```python
with
```
```python
BGR channels)
%   im_data = im_data - mean_data;
```
1.python中数组是按行为主存储的。
2.matlab中数组是按列为主存储的
3.caffe使用c++，也是按行存储的。
1.python 中的图像的三通道是RGB
2.matlab中图像的通道是RGB
3.caffe使用opencv库，图像的通道为BGR
**caffe C++中的blobs**
caffe是基于blobs存储和交换数据。blobs是一个四维数组：
```python
图像数量
```
```python
N
```
```python
* 通道数
```
```python
K
```
```python
* 图像高度H * 图像宽度W
```
在一个4位的blobs，坐标为(n,k,h,w)的物理位置为：
```python
(
```
```python
(
```
```python
n*K+k
```
```python
)
```
```python
*H+h)*
```
```python
W+w
```
**caffe的matlab接口中，blobs的存储方式为：**
```python
width
```
```python
*
```
```python
height
```
```python
* channels * num
```
三通道的形式为BGR.
上述blobs不同的原因是，matlab和c的存储方式的差异。matlab中存储是按照: 列 * 行 * 页 * 卷 …的形式进行存储的。而c语言数组的存储方式可以理解为：…卷 * 页 * 行 * 列，其中这两种存储方式中，…卷、页相当于索引，行和列维实际存储值。
通过地址的形式访问，越后面的维度存储越是连续的，具体参考:
《[一个例子弄明白C语言动态多维数组的创建，存储，引用及降维操作](http://blog.sina.com.cn/s/blog_62c832270101bdrp.html)》
**caffe提供了读取图像的接口**：
```python
im_data = caffe.io.load_image(
```
```python
'./examples/images/cat.jpg');
```
```python
im_data = imresize(im_data, [width, height]); % resize
```
```python
using
```
```python
Matlab
```
```python
's imresize
```
img_data是：width * height * C 的三维数组，并且提供的接口读取的图像的通道顺序为：BGR。
**使用matlab自带的读取图像的接口：**
此时需要对图像进行额外的处理操作才能供caffe使用。
```python
im_data = imread(
```
```python
'./examples/images/cat.jpg'
```
```python
);
```
```python
% read image
```
```python
im_data = im_data(:, :,
```
```python
[
```
```python
3
```
```python
,
```
```python
2
```
```python
,
```
```python
1
```
```python
]
```
```python
);
```
```python
% 从 RGB 转换为 BGR
```
```python
im_data =
```
```python
permute
```
```python
(im_data,
```
```python
[
```
```python
2
```
```python
,
```
```python
1
```
```python
,
```
```python
3
```
```python
]
```
```python
);
```
```python
% 改变 width 与 height 位置
```
```python
im_data = single(im_data);
```
```python
% 转换为单精度
```
## 单通道图像和单标签转化为hdf5格式
例子：[caffe-mnist-hdf5](https://github.com/mravendi/caffe-mnist-hdf5)
在这个例子中，我们关心的是图像是如何转换为hdf5供caffe使用的，但是这个例子中只涉及到单通道和单标签图像。
核心代码如下：
```python
% train-images.idx3-ubyte / train-labels.idx1-ubyte
```
```python
% images: 784*60000,784为28*28大小的图像,60000是训练样本的数量
```
```python
% labels:　60000*1 的列矢量。
```
```python
images = loadMNISTImages(
```
```python
'train-images.idx3-ubyte'
```
```python
);
labels = loadMNISTLabels(
```
```python
'train-labels.idx1-ubyte'
```
```python
);
```
```python
% reshape images to 4-D: [rows,col,channel,numbers]
```
```python
% trainData: 28 * 28 * 1 * 60000 [height,width,channels,numbers]
```
```python
trainData=
```
```python
reshape
```
```python
(images,
```
```python
[
```
```python
28
```
```python
28
```
```python
1
```
```python
size(images,
```
```python
2
```
```python
)]
```
```python
);
```
```python
% permute to [cols,rows,channel,numbers]
```
```python
% trainData: 28 * 28 * 1 * 60000 [width,height,channels,numbers]，因为高和宽大小相同
```
```python
trainData=
```
```python
permute
```
```python
(trainData,
```
```python
[
```
```python
2
```
```python
1
```
```python
3
```
```python
4
```
```python
]
```
```python
);
```
```python
% permute lables to [labels, number of labels ]
```
```python
% trainLabel: 1  * 60000
```
```python
trainLabels=
```
```python
permute
```
```python
(labels,
```
```python
[
```
```python
2
```
```python
,
```
```python
1
```
```python
]
```
```python
);
h5create(
```
```python
'train.hdf5'
```
```python
,
```
```python
'/data'
```
```python
,
```
```python
size
```
```python
(trainData),
```
```python
'Datatype'
```
```python
,
```
```python
'double'
```
```python
);
h5create(
```
```python
'train.hdf5'
```
```python
,
```
```python
'/label'
```
```python
,
```
```python
size
```
```python
(trainLabels),
```
```python
'Datatype'
```
```python
,
```
```python
'double'
```
```python
);
h5write(
```
```python
'train.hdf5'
```
```python
,
```
```python
'/data'
```
```python
,trainData);
h5write(
```
```python
'train.hdf5'
```
```python
,
```
```python
'/label'
```
```python
,trainLabels);
```
上面两次用到了permute函数，是因为matlab以列为主，c语言以行为主。
## 特征向量 和单标签转换为 hdf5 格式
例子：[caffe HDF5Data 层使用及数据生成](http://blog.csdn.net/shuzfan/article/details/52503683)
问题：输入数字的大小为，label是单标签。
```python
% 创建HDF5文件，包含data和label两个变量，数据类型是caffe支持的float型数据
```
```python
h5create(
```
```python
'train.h5'
```
```python
,
```
```python
'/data'
```
```python
,
```
```python
[
```
```python
1
```
```python
1
```
```python
512
```
```python
1000
```
```python
]
```
```python
,
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
h5create(
```
```python
'train.h5'
```
```python
,
```
```python
'/label'
```
```python
,
```
```python
[
```
```python
1
```
```python
1
```
```python
1
```
```python
1000
```
```python
]
```
```python
,
```
```python
'Datatype'
```
```python
,
```
```python
'single'
```
```python
);
```
```python
%reshape: width x height x channels x num，注意MATLAB读数据是列优先，是和C++里面相反的。所以写数据的时候也要倒着写。
```
```python
train_data  =
```
```python
reshape
```
```python
(train_data,
```
```python
[
```
```python
1
```
```python
1
```
```python
512
```
```python
1000
```
```python
]
```
```python
);
train_label =
```
```python
reshape
```
```python
(train_label,
```
```python
[
```
```python
1
```
```python
1
```
```python
1
```
```python
1000
```
```python
]
```
```python
);
h5write(
```
```python
'train.h5'
```
```python
,
```
```python
'/data'
```
```python
, single(train_data));
h5write(
```
```python
'train.h5'
```
```python
,
```
```python
'/label'
```
```python
, single(train_label));
```
上面的情况来至于网络，
下面的情况根据上面的原则应该如此，还需要进行验证。
## 多通道图像（彩色）和单标签转化为hdf5格式
只需要将\#\# 单通道图像和单标签转化为hdf5格式中：
```python
%
```
```python
permute channels from RGB to BGR
```
```python
%
```
```python
trainData = trainData(:,:,[3,2,1],:);
```
## 多通道图像和多标签转换为hdf5格式
此时2个label 应该为，n个标签，修改matlab中的第3维，或者是下面表示形式的第1维。
```python
train_label =
```
```python
reshape
```
```python
(train_label,
```
```python
[
```
```python
1
```
```python
1
```
```python
2
```
```python
1000
```
```python
]
```
```python
);
```
```python
% 或
```
```python
train_label =
```
```python
reshape
```
```python
(train_label,
```
```python
[
```
```python
2
```
```python
1000
```
```python
]
```
```python
);
```
## 验证
参考文献：
[http://blog.sina.com.cn/s/blog_62c832270101bdrp.html](http://blog.sina.com.cn/s/blog_62c832270101bdrp.html)[一个例子弄明白C语言动态多维数组的创建，存储，引用及降维操作；]

