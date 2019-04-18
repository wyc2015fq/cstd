# 最新论文阅读（23）--CrescendoNet: A Simple Deep Convolutional Neural Network with Ensemble Behavior - wydbyxr的博客 - CSDN博客
2018年06月09日 18:58:31[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：41
# CrescendoNet: A Simple Deep Convolutional Neural Network with Ensemble Behavior
```
- 2017年10月
- CrescendoNet
- 克莱姆森大学【美】
```
　　用于分类；在CIFAR10，CIFAR100和SVHN上，与FractalNet、DenseNet-BC的性能相当，且参数还少； 
　　提出CrescendoNet，整体结构是由crescendo block堆叠而成，且没有采用残差结构。 
　　要点有二，一是Crescendo Block（有若干独立的深度线性递增的路径，类似googlenet-block），二是Path-wise training（每次只训练block中的一条path，其余path固定，可以减少训练时内存）。
### 整体结构
![整体结构](https://img-blog.csdn.net/20180609185803299?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### path-wise training
![这里写图片描述](https://img-blog.csdn.net/20180609185821534?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
