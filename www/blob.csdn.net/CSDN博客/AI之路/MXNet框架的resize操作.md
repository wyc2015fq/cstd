# MXNet框架的resize操作 - AI之路 - CSDN博客





2017年10月13日 08:12:31[AI之路](https://me.csdn.net/u014380165)阅读数：1668
所属专栏：[MXNet使用及源码解读](https://blog.csdn.net/column/details/19485.html)









MXNet框架的**图像预处理**操作基本上都在/mxnet/python/mxnet/image.py脚本中，这里的/mxnet指的是官方github的mxnet项目。这篇博客介绍在image.py脚本中的resize操作，主要包含两个类：**mxnet.image.ResizeAug和mxnet.image.ForceResizeAug**。

**mxnet.image.ResizeAug(size,interp=2)**
**这里参数size是一个int值**，表示resize到哪个尺寸，interp表示resize时候的插值方式，默认是2。**这个类是将短边resize到一个指定值，因此这个类处理后一般还要跟一个crop操作，因为你resize后只是短边变成指定size，长边是按照相应比例缩放**，比如你原来是448*1000，要resize到224，那么最后就变成了224*500，这样的尺寸还是不能直接输入模型（比如你要的输入是224*224），因此一般模型都要求输入图像的长宽尺寸一致，比如都是224*224，因此要做crop操作，将224*500再crop成需要的尺寸，比如224*224。在image.py中有CenterCropAug，RandomCropAug两种crop操作，从类名称也可以很好理解二者的差异，前者crop时候的中心点是指定的，后者是随机的。**因此这种resize操作不会使得图像中的物体变形，但是crop后可能会只保留原图像的一部分。**

**mxnet.image.ForceResizeAug(size,interp=2)**
**这里的size是一个tuple，也就是(int,int)**，这个类是强制将输入图像resize到你指定的(int,int)大小，一般会设置这两个int是个相同的值，比如都是224，**，因为是强制resize到一个指定的长宽，因此一般会引起原来图像中的物体的变形，比如物体被拉长或拉宽了，但是整个物体都在，这是和前面一个resize类的区别**。同时这个类就不需要再跟一个crop操作了，因为已经达到需要的尺寸了。



