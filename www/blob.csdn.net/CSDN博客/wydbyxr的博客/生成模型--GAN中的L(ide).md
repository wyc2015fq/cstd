# 生成模型--GAN中的L(ide) - wydbyxr的博客 - CSDN博客
2018年10月29日 10:34:25[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：27
个人分类：[生成模型(VAE,GAN,GLOW)](https://blog.csdn.net/wydbyxr/article/category/8260790)
# L(ide)， target domain identity constraint
  是一种提升generator效果通用方法。
  这个loss的含义是：对于target to source生成器F，如果输入是source domain的图片x（而不是target domain的y），那么也要生成出于x相似的图片。加了这个限制之后，图片颜色就相对稳定了些，不至于很夸张。"
