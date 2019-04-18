# 深度学习基础--loss与激活函数--Total variation loss - wydbyxr的博客 - CSDN博客
2018年11月19日 09:59:43[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：257
# Total variation loss/total variation regularization/Total variation denoising
  参考资料：[https://en.wikipedia.org/wiki/Total_variation_denoising](https://en.wikipedia.org/wiki/Total_variation_denoising)
  总变差有点像衡量一个函数变化程度的函数。BV模型一定程度上是L1图像模型。
  用在图像上，total variation loss可以使图像变得平滑"	"信号处理中，总变差去噪，也称为总变差正则化，是最常用于数字图像处理的过程，其在噪声去除中具有应用。
  它基于这样的原理：具有过多和可能是虚假细节的信号具有高的总变化，即，信号的绝对梯度的积分是高的。根据该原理，减小信号的总变化，使其与原始信号紧密匹配，去除不需要的细节，同时保留诸如边缘的重要细节。该概念由Rudin，Osher和Fatemi于1992年开创，因此被称为ROF模型。
  这种噪声消除技术优于简单的技术，例如线性平滑或中值滤波，这些技术可以降低噪声，但同时可以更大或更小程度地消除边缘。相比之下，即使在低信噪比下，总变差去噪在同时保留边缘同时平滑掉平坦区域中的噪声方面也非常有效。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181119095933431.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
## 一些解释
  首先是数学上的，实际上，在数学上更容易掌握的是L2图像模型。假设噪声（这里仅以去噪为例）为高斯噪声，则使用L2图像模型将导出3*3的模板卷积，相当于3*3模板的高斯低通滤波。这种卷积操作在数学上也称为磨光，也就是说，通过卷积得到的函数被磨光了——所谓光滑意味着无穷可微性。这实际上与自然图像有相当的距离，自然图像应该不是光滑的。BV模型正式为了解决这个问题。或者说，BV模型的产生不是为了光滑，而是为了解决过于光滑的问题——显然的，正如绝对值函数一样，它容许一些点的不可导性，或者说不光滑的点的存在。
  BV模型使得图像是分段（片）光滑的，而不是无穷可微（光滑）的。正是由于不可导点的容许性，从而BV模型比L2图像模型更接近自然图像，比如对图像边的保持。回到Lp空间，BV模型一定程度上是L1图像模型。
