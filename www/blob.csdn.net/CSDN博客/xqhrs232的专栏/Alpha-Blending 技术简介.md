# Alpha-Blending 技术简介 - xqhrs232的专栏 - CSDN博客
2013年06月07日 21:52:49[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：1141
原文地址::[http://www.rosoo.net/a/201004/9314.html](http://www.rosoo.net/a/201004/9314.html)

## Alpha-Blending 技术简介
|jackyhwei 发布于 2010-04-30 12:00 点击:502次|![](http://share.baidu.com/static/images/type-button-1.jpg)来自：百度博客|
现在的游戏，不管是 2D 还是 3D 的，为了追求透明光影效果，通常都会使用到 Alpha-Blending 技术。下面，我们将谈论什么是 Alpha-Blending 技术并简单介绍 Alpha-Blending 技术的具体算法。
TAG: [特效](http://www.rosoo.net/tags.php?/%CC%D8%D0%A7/)[Blending](http://www.rosoo.net/tags.php?/Blending/)[游戏编程](http://www.rosoo.net/tags.php?/%D3%CE%CF%B7%B1%E0%B3%CC/)
现在的游戏，不管是 2D 还是 3D 的，为了追求透明光影效果，通常都会使用到 Alpha-Blending 技术。下面，我们将谈论什么是 Alpha-Blending 技术并简单介绍 Alpha-Blending 技术的具体算法。
所谓 Alpha-Blending，其实就是按照“Alpha”混合向量的值来混合源像素和目标像素。为了便于理解，我们就从两个像素间的 Alpha-Blending 过程谈起吧。
第一步，先把源像素和目标像素的 RGB 三个颜色分量分离，然后把源像素的三个颜色分量分别乘上 Alpha 的值，并把目标像素的三个颜色分量分别乘上 Alpha 的反值，接下来把结果按对应颜色分量相加，再对最后求得的每个分量结果除以 Alpha 的最大值（通常这一步都是用移位来完成，这亦是 Alpha 的最大值为何总是二的幂的原因），最后把三个颜色分量重新合成为一个像素输出。
在这个过程中，为了分离像素中的 RGB 三个颜色分量，我们一般会使用掩膜的方法。至于具体的三个掩膜值：RMask、GMask 和 BMask，可以从 DirectDraw 中的 DDPixelFormat 返回得到（当然，你也可以根据自己的需要做各种灵活处理）。
在下面给出的说明性例程中，我们假设 RMask、GMask 和 BMask 已经存放的是 RGB 三个颜色分量的位掩膜值，Alpha 的最大值为 256，而 source 和 dest 是指向源像素和目标像素的指针。
- int ialpha = 256 - *alpha; // Alpha 的反值
- 
- *dest = (RMask & (((*source & RMask) * *alpha + 
-   (*dest & RMask) * ialpha) >>8)) | (GMask & (((*source & GMask) * *alpha
-   + (*dest & GMask) * ialpha) >>8)) | (BMask & (((*source & BMask) * *alpha
-   + (*dest & BMask) * ialpha) >>8)); 
将2个图像中对应的2点进行混色的算法过程如下：
设a=Alpha值; b=256-a (b是a的补值); 
MR,MG,MB分别为R,G,B的掩膜值，用二进制表示，即 
MR=00000000 11111111 00000000 00000000 
MG=00000000 00000000 11111111 00000000 
MB=00000000 00000000 00000000 11111111 
它们与图像的像素逻辑乘，即可从32位色彩中分离出R,G,B分量。 
通常，在一个32位色彩的像素值数据中，前8位就是alpha值，故 
每个像素可以有自己的Alpha值，它也要利用掩膜值MA来分离: 
MA=11111111 00000000 00000000 00000000 
如果用C代表32位色彩值，则Alpha值 a=C & MA。 
混合后的像素值为： 
dest = (R & (((Csource & MR)*a + (Cdest & MR)*b) > > 8))   
       | (G & (((Csource & MG)*a + (Cdest & MG)*b) > > 8))   
       | (B & (((Csource & MB)*a + (Cdest & MB)*b) > > 8)); 
Csource,Cdest分别为待混合的像素色彩值。
最快的alpha blending算法
- unsigned int blend565( unsigned int src, unsigned int dst, unsigned char alpha ) 
- { 
- unsigned long src2; 
- unsigned long dst2; 
- 
- //Convert to 32bit |-----GGGGGG-----RRRRR------BBBBB|
- src2=((src<<16)|src)&0x07E0F81F; 
- dst2=((dst<<16)|dst)&0x07E0F81F; 
- 
- //Perform blending R:G:B with alpha in range 0..32
- //Note that the reason that alpha may not exceed 32 is that there are only
- //5bits of space between each R:G:B value, any higher value will overflow
- //into the next component and deliver ugly result.
- dst2=((((dst-src)*alpha)>>5)+src)&0x07E0F81F; 
- return (dst>>16)|dst; 
- } 
[另一篇](http://bbs.rosoo.net/thread-749-1-1.html)解释的很清楚。
朴素的单指令多数据原理，移位使得RGB变成一个xGxxRxB的32为的数据，以前算R,G,B的需要3步，现在只需要一步了。
当然了仅仅适用于16位565的模式。
(chenzhuoyou)

