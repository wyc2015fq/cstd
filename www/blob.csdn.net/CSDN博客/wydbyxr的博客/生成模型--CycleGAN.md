# 生成模型--CycleGAN - wydbyxr的博客 - CSDN博客
2018年10月26日 10:15:28[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：104
# CycleGAN
## CycleGAN和pix2pix的比较
  pix2pix也可以做图像变换，它和CycleGAN的区别在于，pix2pix模型必须要求成对数据（paired data），而CycleGAN利用非成对数据也能进行训练(unpaired data)。
  比如，我们希望训练一个将白天的照片转换为夜晚的模型。如果使用pix2pix模型，那么我们必须在搜集大量地点在白天和夜晚的两张对应图片，而使用CycleGAN只需同时搜集白天的图片和夜晚的图片，不必满足对应关系。
  因此CycleGAN的用途要比pix2pix更广泛，利用CycleGAN就可以做出更多有趣的应用。
