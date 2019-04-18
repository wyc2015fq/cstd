# 数据预处理--数据扩增/Data Augmentation/图像增强 - wydbyxr的博客 - CSDN博客
2018年12月03日 11:00:26[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：708
  若增加训练数据，则能够提升算法的准确率，因为这样可以避免过拟合，更好地泛化；而避免了过拟合你就可以增大你的网络结构了。
  可以大量使用数据增广。
# 1）几何变换
  包括：弹性变换（Elastic Transform）、透视变换（Perspective Transform）、分段仿射变换（Piecewise Affine transforms）、枕形畸变（Pincushion Distortion）。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181203105432644.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
  a）随机改变大小（resize）,随机缩放、旋转、翻转
  b）从原始图像（256,256）中，随机的crop出一些图像（224,224）
  作者说，不做随机crop，大型网络基本都过拟合(under substantial overfitting)。
  先crop，后padding再resize，这样输入图片会保持长宽比。我们做过类似的实验，抠图直接resize到输入尺寸，这样图片会损失长宽比，结果会差一点。然后，我们会先做加入Random scales，把框进行随机扩大或者缩小。
  c）水平/竖直翻转，flip。mirror，即水平翻转图像。
```
transform_param { 
# 测试的时候就不做镜像了
    mirror: false
    crop_size: 227
    mean_file: ""ilsvrc12/imagenet_mean.binaryproto""
}
```
  d）Rotation变换/旋转变换
  目前主流的处理方式rotate负30度到正30度，这次我们用的是负45度到正45度。这个Rotation一般来说会有提升但是不会很大，主要是用在一些比较极端的情况下，比如人是斜着的。
  在后面做实验的时候，发现了另外一种思路：直接把人旋转，检测出人的头，或某个部位之后，按照一定的角度把人转正。
# 2）加噪声
  对主成分做一个(0, 0.1)的高斯扰动。
# 3）PCA Jittering（颜色改变）
  最早是由Alex在他2012年赢得的ImageNet竞赛的那篇NIPS中提出的。
  a）首先按照RGB三个颜色通道计算均值和标准差，对网络的输入数据进行规范化，
  b）随后我们在整个训练集上计算了协方差矩阵，进行特征分解，得到特征向量和特征值，用来做PCA Jittering。
  对RGB空间做PCA，然后对主成分做一个(0, 0.1)的高斯扰动。结果让错误率又下降了1%。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181203105702574.png)
# 4）对比度和亮度
  给图像增加一些随机的光照；
  对比度受限自适应直方图均衡化算法（Clahe），锐化（Sharpen），凸点（Emboss）；
# 5）随机色相、饱和度、明度（HSV）变换
# 6）彩图到灰度转换（Color to Gray）
# 7）将灰度图重新映射到随机颜色的图像中
# 8）模糊（Blur）、一般模糊（Median Blur）、非常模糊（Motion Blur）
# 9）通道重排
  由于数据的天然性，这一点非常重要
# 10）图像上细胞核的复制？
  这样就创造了大量重叠的细胞核，似乎有助于网络更好地学到重叠细胞核的边界。
