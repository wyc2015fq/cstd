# 最新论文阅读（17）--Pyramidal RoR for Image Classification - wydbyxr的博客 - CSDN博客
2018年06月09日 18:23:38[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：42
个人分类：[深度学习论文阅读](https://blog.csdn.net/wydbyxr/article/category/7151097)
# Pyramidal RoR for Image Classification
```
- 2017年10月
- RoR和Pyramidal RoR
- 华北电力大学
```
　　用于图像分类； 
　　提出The Residual Networks of Residual Networks (RoR)，即大的残差模块中会有小的残差模块。 
　　整体结构其实就是从输入到输出，通道数不断递增的残差模块 进行stack形成的网络。
### 两种结构的整体结构
![这里写图片描述](https://img-blog.csdn.net/2018060918225865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### RoR network architecture
![这里写图片描述](https://img-blog.csdn.net/20180609182213137?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
### Pyramidal RoR
![这里写图片描述](https://img-blog.csdn.net/20180609182317550?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
