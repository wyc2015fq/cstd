# 超十亿样本炼就的CNN助力图像质量增强，Adobe推出新功能「增强细节」 - 知乎
# 



> 近日，Adobe 宣布推出一项名为 Enhance Details（增强细节）的照片编辑应用程序新功能，该功能使用机器学习来增强图像的分辨率和图像质量。新功能将在 Lightroom CC 中提供，Adobe 称该功能可以将某些图像的分辨率提高多达 30％。
选自Adobe，作者：Sharad Mangalick，机器之心编译，参与：Nurhachu Null、路。

近日，Adobe 宣布推出一项名为 Enhance Details（增强细节）的照片编辑应用程序新功能，该功能使用机器学习——一个经过大量训练的[卷积神经网络](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757267%26idx%3D3%26sn%3D65d1d8db4fa24920ef86951f91af3b85%26chksm%3D871a9cedb06d15fb1bf813c05c9feb2004a946a867aecbcc84af233e41cac320b4bc5bf8017d%26token%3D1491778879%26lang%3Dzh_CN)——为那些真正需要重视的图片提供最高的质量。增强细节功能在 Bayer 格式（佳能、尼康、索尼等）和 X-Trans 格式（富士胶片）的原始马赛克文件上都能很好地使用。
![](https://pic2.zhimg.com/v2-a9e2a241f2ceb77e2146c1b8dcf81e7d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='704' height='473'></svg>)![](https://pic2.zhimg.com/v2-1de3f7febbeb53098fa2e9cd23702049_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='467'></svg>)上图展示了 Fuji X-Trans 文件中细节区域放大到 200% 时的区别。请注意窗户和街灯中细节的清晰度得到了增强
**照相机是如何看世界的**

要想理解「增强细节」的原理，我们先来了解一下普通的数码相机传感器是如何看世界的。

人眼可以区分数百万种颜色。我们绝大多数人都是有三色视觉的，我们的视网膜上有不同类型的锥形感光细胞，可以感知红色、绿色和蓝色。每类视锥细胞都可以让眼睛区分大约 100 种色度，然后人类视觉系统将信号混合在一起来看见这数百万种不同色度的颜色。

但是，照相机并不是这么看世界的。

所有的数字照片都是从单色开始的。然后通过逆马赛克变换过程将照片变成彩色。

数码相机传感器由两部分组成。首先是主要的光传感器阵列。微观光敏腔测量给定像素的光强度。只是光强，并没有颜色。例如你在沙滩上，观看太平洋上的日落。

你看到的景象是这样的：
![](https://pic3.zhimg.com/v2-fda5d7776dd6094007fa48515b3885de_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='534'></svg>)你记忆中的样子
相机上的光传感器阵列只能看到一张单色图片（可能更暗一些，因为光传感器感知光线的方式与人眼不同，原始处理（raw processing）关心的就是这个问题，但是一张超级黯淡的图片并不会让人感兴趣，所以......
![](https://pic3.zhimg.com/v2-e6b8e5b258935a5f800f03031f4488b2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='534'></svg>)没有色彩滤波阵列的相机看到的是这样的
在光传感器阵列上面加上色彩滤波阵列后会产生不同的结果。色彩滤波阵列让传感器记录每一个像素的颜色，例如：
![](https://pic4.zhimg.com/v2-034b08f01c3cefe51cfd9ff773822b0b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='525'></svg>)色彩滤波阵列将每个像素记为单个的红色、绿色或者蓝色。这幅图呈绿色的原因是拜耳阵列（Bayer array）的绿色像素是红色和蓝色像素的两倍，这是为了模仿人眼感知颜色的方式
数码照相机对每一个给定像素只会记录三种颜色之中的一个。例如对于红色像素，色彩滤波阵列会移除所有的蓝色和绿色信息，最终导致这个像素仅仅记录红色。因此原始图像中的每个像素都会缺失其他两种颜色的信息。
![](https://pic4.zhimg.com/v2-59413e6adaf227abcc60f3258ecc52c7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='394'></svg>)
**软件是如何重建图像的**

数字照片中每个像素复合的红、绿、蓝色的值是通过逆马赛克变换过程来创建的。

逆马赛克变换方法的出现是艺术与科学。有很多种方式可以对一张照片进行逆马赛克处理。逆马赛克设计的选择可以影响一切，从照片的整体分辨率到较小颜色区域的保真度，以及精细细节的准确再现。

在其最基本的形式中，逆马赛克过程会将邻近像素的颜色值平均化。例如，具有红色滤波器的像素将仅提供这个像素关于红色光强的记录。逆马赛克算法会对所有四个相邻蓝色像素的值取平均，来决定最可能的蓝色值，然后对周围的绿色像素也进行同样的操作得到绿色值。猜测最可能的值是什么的过程叫做[插值](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757267%26idx%3D3%26sn%3D65d1d8db4fa24920ef86951f91af3b85%26chksm%3D871a9cedb06d15fb1bf813c05c9feb2004a946a867aecbcc84af233e41cac320b4bc5bf8017d%26token%3D1491778879%26lang%3Dzh_CN)，它是逆马赛克过程的一个重要部分。

在具有平滑色彩梯度或者恒定颜色的图像区域中，逆马赛克过程相对简单一些，比如蓝天和白云。然而，在具有更复杂的图像区域中，这个过程会变得特别棘手。在具有纹理、精细细节、重复图案和尖锐边缘的区域中，标准的逆马赛克方法会遇到麻烦，从而产生较低分辨率和有问题的图像。
![](https://pic1.zhimg.com/v2-fcf4c1a46805d33f6b0f89e9f1b1e7c0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='467'></svg>)
高级的逆马赛克方法能够处理这些复杂区域，但是这些方法消耗大量计算资源。构建一幅图像需要大量数学计算来执行必需的插值操作。即使在最强大的计算机硬件上，这也需要时间。

因此，像 Lightroom 这样的软件一直在不断地进行图像保真度和速度之间的权衡。

**逆马赛克问题**

实际上需要解决的逆马赛克问题并不多。但是他们一次又一次，以新的和更复杂的形式，在一张又一张的图像中出现。
- 小尺寸细节：如果图像具有逼近相机传感器分辨率极限的小细节，那么这是一个大问题。如果幸运的话，你仅仅会把这些细节变成混乱的颜色。如果不幸，你会遇到莫尔图案，颜色伪影排列成迷宫一样的图案。
- 假色：当逆马赛克算法跨尖锐的边缘而不是沿着边缘进行错误插值时，你可能看到颜色的突变或者不自然的变化。
- Zippering：图像边缘缺少通常用来插值颜色数据的一半像素，所以可能出现边缘模糊。

Adobe 一直在努力提升逆马赛克算法。多年来，Adobe 已经将算法改进到能够在大多数图像上表现得非常好的程度。但是这些特殊的难题仍需要我们以不同的方式思考这个问题。

**增强细节**

进入 Adobe Sensei。Sensei 集成了人工智能的所有分支，包括机器学习。

增强细节使用一个经过大量训练的卷积神经网络进行图片质量最大化的优化。Adobe 使用有问题的示例训练一个神经网络来对原始图像进行逆马赛克处理，然后利用内置于最新 Mac OS 和 win10 操作系统中的机器学习框架来运行这个网络。增强细节使用的神经网络已经在超过十亿示例上进行了训练。

这十亿示例中的每一个都包含一或多个上述问题，它们给标准的逆马赛克方法带来了严重的麻烦。Adobe 训练了两个模型：一个用于 Bayer 传感器，另一个用于 Fujifilm X-Trans 传感器。

所以，增强细节带来了令人惊叹的效果，包括更高的分辨率、更准确的边缘和细节渲染、更少的伪影（如假色和莫尔图案）。
![](https://pic1.zhimg.com/v2-4a6db83e71e4ba015aec122058032ec8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='700' height='467'></svg>)
Adobe 用西门子星分辨率测试卡进行了计算，发现「增强细节」在 Bayer 和 X-Trans 原始图像上能够提供高达 30% 的分辨率提升。如果你想自己尝试一下，请自行下载富士胶片原始文件（[https://theblogimages.adobe.com/wp-content/uploads/2019/02/Fuji-X-Trans-Sample-File.zip](https://link.zhihu.com/?target=https%3A//theblogimages.adobe.com/wp-content/uploads/2019/02/Fuji-X-Trans-Sample-File.zip)）。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*原文链接：*[https://theblog.adobe.com/enhance-details/](https://link.zhihu.com/?target=https%3A//theblog.adobe.com/enhance-details/)


