# Python Opencv Canny 边缘检测算法 - YZXnuaa的博客 - CSDN博客
2018年01月24日 15:16:07[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1307
Canny边缘检测算法可以分为以下5个步骤：
- 
应用高斯滤波来平滑图像，目的是去除噪声
- 
找寻图像的强度梯度（intensity gradients）
- 
应用非最大抑制（non-maximum suppression）技术来消除边误检（本来不是但检测出来是）
- 
应用双阈值的方法来决定可能的（潜在的）边界
- 
利用滞后技术来跟踪边界
**1. 图像平滑（去噪声）**
任何[边缘检测](https://baike.baidu.com/item/%E8%BE%B9%E7%BC%98%E6%A3%80%E6%B5%8B)算法都不可能在未经处理的[原始数据](https://baike.baidu.com/item/%E5%8E%9F%E5%A7%8B%E6%95%B0%E6%8D%AE)上很好地工作，所以第一步是对原始数据与高斯
 mask 作[卷积](https://baike.baidu.com/item/%E5%8D%B7%E7%A7%AF)，得到的图像与原始图像相比有些轻微的模糊（blurred）。这样，单独的一个像素噪声在经过高斯平滑的图像上变得几乎没有影响。以下为一个5X5高斯滤波器（高斯核，标准差\delta=1.4），其中A为原始图像，B为平滑后的图像。
![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D250/sign=417daa653a12b31bc36cca2cb6193674/54fbb2fb43166d227dba23b6452309f79152d2c6.jpg)
![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=3328b5cf18178a82ca3c78a2c603737f/908fa0ec08fa513d979923dd3b6d55fbb2fbd986.jpg)
**2. 寻找图像中的强度梯度**
Canny算法的基本思想是找寻一幅图相中灰度强度变化最强的位置。所谓变化最强，即指梯度方向。平滑后的图像中每个像素点的梯度可以由Sobel算子（一种卷积运算）来获得（opencv中有封装好的函数，可以求图像中每个像素点的n阶导数）。首先，利用如下的核来分别求得沿水平（x）和垂直（y）方向的梯度G_X和G_Y。
K_{GX} = [-1 0 1 ; -2 0 2 ; -1 0 1], K_{GY} = {1 2 1 ; 0 0 0 ; -1 -2 -1}
之后便可利用公式来求得每一个像素点的梯度度量值（gradient magnitude，可能翻译得不准确）。
![](https://gss1.bdstatic.com/-vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=75e4f90d4234970a4373172da5cad1c0/d50735fae6cd7b8943660ba10c2442a7d9330e8d.jpg)
，有时为了计算简便，也会使用G_X和G_Y的无穷大范数来代替二范数。把平滑后的图像中的每一个点用G代替，可以获得如下图像。从下图可以看出，在变化剧烈的地方（边界处），将获得较大的梯度度量值G，对应的颜色为白色。然而，这些边界通常非常粗，难以标定边界的真正位置。为了做到这一点（参考非极大抑制Non-maximum suppression一节），还必须存储梯度方向，其公式如下图所示。也就是说在这一步我们会存数两块数据，一是梯度的强度信息，另一个是梯度的方向信息。
![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=647af60dbd096b63851959523c338733/30adcbef76094b36c2932acfa0cc7cd98d109d8a.jpg)
![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=635bf9e5217f9e2f74351a0a2f31e962/0b46f21fbe096b6334f415530a338744eaf8acc0.jpg)
**3. 非极大抑制Non-maximum suppression**
这一步的目的是将模糊（blurred）的边界变得清晰（sharp）。通俗的讲，就是保留了每个像素点上梯度强度的极大值，而删掉其他的值。对于每个像素点，进行如下操作：
a) 将其梯度方向近似为以下值中的一个（0,45,90,135,180,225,270,315）（即上下左右和45度方向）
b) 比较该像素点，和其梯度方向正负方向的像素点的梯度强度
c) 如果该像素点梯度强度最大则保留，否则抑制（删除，即置为0）
为了更好的解释这个概念，看下图。
![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=1d74151091dda144de096bb082b6d009/e1fe9925bc315c60250bdaec8bb1cb1349547712.jpg)
图中的数字代表了像素点的梯度强度，箭头方向代表了梯度方向。以第二排第三个像素点为例，由于梯度方向向上，则将这一点的强度（7）与其上下两个像素点的强度（5和4）比较，由于这一点强度最大，则保留。处理后效果如下图所示。
![](https://gss2.bdstatic.com/9fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=1c0a176bd709b3deefbfe36afcbf6cd3/7c1ed21b0ef41bd5e72a58f757da81cb39db3db3.jpg)
上图中，可以想象，边界处的梯度方向总是指向垂直于边界的方向，即最后会保留一条边界处最亮的一条细线。
**4.双阈值（Double Thresholding）**
经过非极大抑制后图像中仍然有很多噪声点。Canny算法中应用了一种叫双阈值的技术。即设定一个阈值上界和阈值下界（opencv中通常由人为指定的），图像中的像素点如果大于阈值上界则认为必然是边界（称为强边界，strong edge），小于阈值下界则认为必然不是边界，两者之间的则认为是候选项（称为弱边界，weak edge），需进行进一步处理。经过双阈值处理的图像如下图所示
![](https://gss1.bdstatic.com/9vo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D220/sign=0639410e76cf3bc7ec00caeee100babd/ac4bd11373f082023b9f2c3c4dfbfbedab641ba7.jpg)
上图中右侧强边界用白色表示，弱边界用灰色表示。
**5.利用滞后的边界跟踪**
这里就不细作解释了。大体思想是，和强边界相连的弱边界认为是边界，其他的弱边界则被抑制。
