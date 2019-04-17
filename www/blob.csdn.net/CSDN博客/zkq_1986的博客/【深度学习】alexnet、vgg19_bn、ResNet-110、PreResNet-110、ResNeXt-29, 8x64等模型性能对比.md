# 【深度学习】alexnet、vgg19_bn、ResNet-110、PreResNet-110、ResNeXt-29, 8x64等模型性能对比 - zkq_1986的博客 - CSDN博客





2018年11月16日 16:47:58[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：351
个人分类：[神经网络](https://blog.csdn.net/zkq_1986/article/category/6338867)









alexnet、vgg19_bn、ResNet-110、PreResNet-110、WRN-28-10 (drop 0.3)、ResNeXt-29, 8x64、ResNeXt-29, 16x64、DenseNet-BC (L=100, k=12)、DenseNet-BC (L=190, k=40)等模型性能对比：

### CIFAR

Top1 error rate on the CIFAR-10/100 benchmarks are reported. You may get different results when training your models with different random seed. Note that the number of parameters are computed on the CIFAR-10 dataset.
|Model|Params (M)|CIFAR-10 (%)|CIFAR-100 (%)|
|----|----|----|----|
|alexnet|2.47|22.78|56.13|
|vgg19_bn|20.04|6.66|28.05|
|ResNet-110|1.70|6.11|28.86|
|PreResNet-110|1.70|4.94|23.65|
|WRN-28-10 (drop 0.3)|36.48|3.79|18.14|
|ResNeXt-29, 8x64|34.43|3.69|17.38|
|ResNeXt-29, 16x64|68.16|3.53|17.30|
|DenseNet-BC (L=100, k=12)|0.77|4.54|22.88|
|DenseNet-BC (L=190, k=40)|25.62|3.32|17.17|

![cifar](https://github.com/bearpaw/pytorch-classification/raw/master/utils/images/cifar.png)

### ImageNet

Single-crop (224x224) validation error rate is reported.
|Model|Params (M)|Top-1 Error (%)|Top-5 Error (%)|
|----|----|----|----|
|ResNet-18|11.69|30.09|10.78|
|ResNeXt-50 (32x4d)|25.03|22.6|6.29|

![Validation curve](https://github.com/bearpaw/pytorch-classification/raw/master/utils/images/imagenet.png)

## 参考文献：

[https://github.com/bearpaw/pytorch-classification](https://github.com/bearpaw/pytorch-classification)





