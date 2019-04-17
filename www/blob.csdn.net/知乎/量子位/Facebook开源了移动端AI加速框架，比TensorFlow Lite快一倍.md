# Facebook开源了移动端AI加速框架，比TensorFlow Lite快一倍 - 知乎
# 



> 方栗子 发自 凹非寺
量子位 出品 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-6117a004dab9a4cd0aca07923894106c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='225' height='225'></svg>)



Facebook发布了一个开源框架，叫**QNNPACK**，是手机端神经网络计算的加速包。

官方表示，它可以**成倍提升**神经网络的推理效率，几乎比**TensorFlow Lite**快一倍。

这个框架，能够为很多运算加速，比如**DW卷积** (Depthwise Convolution) ，许多先进的架构里面都用得到。

目前，QNNPACK已经是**PyTorch 1.0**的一部分，在Caffe2里就能直接使用。

其实，Facebook手机应用里面，已经部署了这个QNNPACK。也就是说，数以亿计的手机都在用了。

## **从哪个角度加速？**

QNNPACK，这个名字眼熟么？

两年前，Facebook就推出过一个加速包，叫做**NNPACK**，Caffe2Go用的就是它。

基于Winograd变换和傅里叶变换，有效减少卷积计算里的乘加运算 (**Multiply-Add**) 。这样一来，3x3卷积可以只比1x1慢一倍，而不会慢8倍。

不过，世界变化很快。现在的计算机视觉 (CV) 神经网络里，用到的很多卷积类型，**已经沾不到NNPACK的光**：

比如，1x1卷积，分组卷积 (Grouped Convolution) ，Strided Convolution，扩张卷积 (Dilated Convolution) ，DW卷积 (DepthWise Convolution) ，适用于精度/存储带宽受到限制的 (移动端等) 场景。



![](https://pic2.zhimg.com/v2-ea1f8fc658e7f2fd73e8957f5f648121_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)



而CV神经网络，大部分推理时间，都花在卷积和全连接运算上。

这样的运算，和**矩阵乘法**密切相关：

大内核的卷积，可以分解成im2col和一个矩阵乘法。

所以，有高效的矩阵乘法，才能有高效的卷积网络。

于是，QNNPACK出世了。

## **怎样加速矩阵乘法？**



![](https://pic1.zhimg.com/v2-97c20812b46023ef8beadd8cdfba150c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='705'></svg>)



矩阵乘法，A x B = C。C里面的每一个元素，都可以看成**A中某行**和**B中某列**的点乘。


但直接在点乘基础上计算的话，一点也不快，会受到存储带宽的限制。



![](https://pic2.zhimg.com/v2-a0bfe67473e35b61ecefa320ac41edb1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='442' height='584'></svg>)



如果，能同时计算**A中多行**和**B中多列**的点乘，即**MRxNR**，就能给运算速度带来猛烈的提升。

不需要太多，这样细小的改变就够了。

## **节省内存和缓存**

模型训练，可能更需要高精度。但在训练完成后，**推理**部分对精度的需求可能就没有那么高了。

**低精度**的计算，有时对推理的**准确性**不会产生明显的影响。

而这样的运算，不需要太大存储，并节省能源，有助于把AI部署在移动端。

**QNNPACK**用的线性量化 (Linear Quantization) 方案，与安卓的神经网络API兼容。

它假设量化值q[i]是用8比特的无符号整数 (Unsigned Integers) 来表示的，以及q[i]与真实值r[i]相关，关系如下：



![](https://pic1.zhimg.com/v2-0d7933f72c44eafc67db4daed960fe44_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='532' height='106'></svg>)



与其他库不同，QNNPACK把矩阵A、B都放进一级缓存 (L1 Cache) ，目标是把所有对运算过程并不非常必要的内存转换 (Memory Transformations) 都删掉。


QNNPACK可以在一次微内核调用 (Microkernel Call) 里，处理A和B。

不需要在微内核之外，累积32位的中间结果，QNNPACK把32位的中间值融合进微内核，然后写出8位值，节省了存储带宽和缓存。

## **赢了TensorFlow Lite**

开发团队用谷歌的视觉框架MobileNetV2里面的**图像分类模型**来测试。

拿**TensorFlow Lite**做对手，和**QNNPACK**比了一场。



![](https://pic4.zhimg.com/v2-6a41fb8be64c7b5b00265198ef022bf3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='780' height='472'></svg>)



结果是，QNNPACK比TensorFlow Lite几乎快一倍，不论是在高端智能机，还是普通智能机身上。

所以，各位也去试一下？

GitHub项目页：
[pytorch/QNNPACK​github.com![图标](https://pic4.zhimg.com/v2-2b69048deeb1ce5914dc891d7aa149e7_ipico.jpg)](https://link.zhihu.com/?target=https%3A//github.com/pytorch/QNNPACK)
博客原文：
[https://code.fb.com/ml-applications/qnnpack/​code.fb.com](https://link.zhihu.com/?target=https%3A//code.fb.com/ml-applications/qnnpack/)
所以，各位也去试一下？

— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


