# 人像美妆---妆容迁移算法研究(Makeup transfer)

2017年04月18日 14:15:35

 

Trent1985

 

阅读数：9361

更多

所属专栏： [图像滤镜艺术](https://blog.csdn.net/column/details/zphotoimagefilter.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/70226779

对于人像美妆算法，现在的美妆相机、玩美彩妆之类的app已经做的比较成熟了，但是具体算法，基本网络上是杳无可查，今天本人介绍一种自动的人像美妆算法----(Makeup Transfer)妆容迁移

妆容迁移相关的论文不多，有如下几篇：



1.Example-Based cosmetic transfer

2.Makeup Transfer using Multi-example

3.A new digtial face makeup method

4.An automatic framework for example vased virtual makeup

本人主要介绍第一篇《Example-Based cosmetic transfer》，论文效果图如下：

![img](https://img-blog.csdn.net/20170418130209547?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意：A和A*是两张输入，B是原始图像，B*是根据A*迁移过来的妆容效果图；

本文的算法流程如下：

**1，Face warp**

若要讲A*的妆容迁移到B上，文中有几个条件：

①背景单一；

②肤色相近；

这两个条件也是为了最后的效果更加自然；

首先，Face warp的过程需要有人脸特征点，因此，这一步之前需要进行人脸检测和点位对齐，拿到A*和B的特征点；

然后，根据变形算法讲A*和B的特征点对齐，从而将A*变形到与B一致；

变形算法有以下几种(个人总结)：



①最小二乘MLS变形算法：Image Deformation Using Moving Least Squares.

详细介绍参考博客：http://blog.csdn.net/hjimce/article/details/46550001

②基于线的变形算法：As-Rigid-As-Possible Shape Manipulation.

详细介绍参考博客：http://blog.csdn.net/hjimce/article/details/45766321

③三角网格仿射变换

给出这一步的效果图如下：

![img](https://img-blog.csdn.net/20170418135155881?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

左边是原图B(为避免 侵权，眼睛做了处理)，中间是妆容图A*，右边是Face warp之后的效果图(我这里采用的是三角网格变形)；

**2，Cosmetic Map计算**

文中介绍的重点也就是这一步，计算Cosmetic Map,即CP，其实算法很简单，公式如下：

Cp = ap / ap*

ap:妆容图像A*对应的原图A

ap*：妆容图像A*

就这么一个简单的公式，就可以化腐朽为神奇。

文中所给CP效果图如下：

![img](https://img-blog.csdn.net/20170418131503962?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**3，Makeup transfer**

得到了CP之后，我们就可以来进行妆容迁移了，具体算法如下：

![img](https://img-blog.csdn.net/20170418131744135?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**4，Others**

实际上论文中还介绍一些其他内容，这里我没有写出，因为我的重点是妆容迁移，所以就主要提取了这块内容。

论文中实际上还进行了Freckle remove雀斑去除，眉毛眼睛纹理细节提取等等，如下所示：

![img](https://img-blog.csdn.net/20170418132135777?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![img](https://img-blog.csdn.net/20170418132150496?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这些内容，我这里不关心，实际上，就是为了让最后的效果更加自然更加逼真；

以上整个过程就是这篇论文的核心算法；

**现在，好东东才刚刚开始：**

**本人对这篇论文提出一下几个问题，实际上也是应用中的缺陷：**

①，论文要求三个输入(A, A*, B)，一个输出B*

这一点，实际应用中就有很大限制，一般而言，我们能拿到A*，也就是好看的妆容效果图，然后想对自己的照片B进行化妆，这个逻辑中是没有未化妆的原图A的。

②，论文中要求肤色相近，背景单一

这一点，普适性太低，很难应用；

③，按照论文的逻辑，嘴巴区域是闭合的，无法适应于各种大笑等开口的情况，或者是效果太差；

**介于以上三点，本人对算法进行了改进：**

①，根据A*，对A进行估计，估算得到A，这样就只要求用户输入一张好看的效果图，即可对自己的自拍照等进行妆容迁移了；

②，根据人脸特征点，获取A*中的肤色特征，构建精准的人脸Mask，去除背景，这样就避免了背景的影响，同时，进行肤色转换，将A*中的肤色转换到B*中去,从而避免肤色差异过大造成的影响；

③获取A*中的唇色特征，对B进行唇色转换，即将A*的肤色和唇色迁移到B*中去，从而使用于各种大笑等开口场景；

**根据上述三点，本人改进算法，得到如下的结果：**

![img](https://img-blog.csdn.net/20170418134414041?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

原图B

![img](https://img-blog.csdn.net/20170418134509871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

三个目标妆容B*

上述三个妆容B*分别对应的效果图如下：

![img](https://img-blog.csdn.net/20170418134754625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在给一组测试图：

![img](https://img-blog.csdn.net/20170418140555344?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

以上效果本人做了化妆程度自适应，所以没有出现很饱满的艳妆，这样是为了看起来更自然一点。

注意：本人使用的测试图来自美颜相机和互联网，若有侵权敬请告知。

本人提供简单的代码调用如下：



```csharp
      private void pictureBox4_Click(object sender, EventArgs e)
      {
          if (pictureBox1.Image != null)
          {
              Graphics g = Graphics.FromImage(curBitmap);
              int[] eyePoints = { 
                                 173, 370, 177, 441, 191, 509, 212, 577, 236, 640,
                                 267, 698, 303, 748, 347, 793, 399, 826, 465, 837,
                                 526, 825, 575, 792, 611, 748, 643, 699, 671, 643,
                                 695, 578, 714, 507, 727, 435, 728, 364, 214, 316,
                                 245, 284, 285, 273, 328, 274, 370, 281, 402, 308,
                                 363, 309, 325, 304, 287, 302, 251, 307, 513, 307,
                                 544, 282, 583, 275, 623, 274, 660, 284, 688, 313,
                                 654, 306, 620, 302, 585, 304, 549, 309, 269, 390,
                                 282, 373, 300, 364, 323, 361, 347, 366, 365, 380,
                                 378, 401, 360, 406, 342, 410, 321, 412, 300, 408,
                                 283, 401, 533, 399, 544, 378, 562, 365, 585, 359,
                                 607, 362, 625, 371, 638, 386, 624, 398, 608, 406,
                                 588, 410, 567, 408, 550, 404, 424, 394, 424, 453,
                                 417, 512, 386, 542, 398, 580, 446, 588, 480, 588,
                                 528, 576, 536, 539, 506, 511, 494, 452, 490, 393,
                                 363, 653, 394, 643, 429, 637, 462, 642, 495, 636,
                                 527, 643, 557, 654, 535, 687, 506, 713, 461, 726,
                                 415, 715, 384, 688, 373, 656, 417, 657, 462, 661,
                                 504, 657, 546, 657, 505, 676, 460, 686, 414, 676,
                                 322, 389, 586, 387, 457, 392, 461, 502, 463, 554,
                                 463, 588 };
 
              eyePoints[2 * 50 + 1] -= 2;
              eyePoints[2 * 49 + 1] -= 3;
              eyePoints[2 * 48 + 1] -= 4;
              eyePoints[2 * 47 + 1] -= 3;
              eyePoints[2 * 46 + 1] -= 2;
 
              eyePoints[2 * 52 + 1] -= 1;
              eyePoints[2 * 61 + 1] -= 2;
              eyePoints[2 * 60 + 1] -= 3;
              eyePoints[2 * 59 + 1] -= 2;
              eyePoints[2 * 58 + 1] -= 1;
              for (int i = 0; i < 101; i++)
              {
                  g.DrawRectangle(new Pen(Color.Red, 1), new Rectangle(eyePoints[2 * i] - 1, eyePoints[2 * i + 1] - 1, 2, 2));
              }
              g.Dispose();
              DateTime start = DateTime.Now;
              curBitmap = ip.SoftSkin(srcBitmap, new Bitmap(startPath + "\\MakeUp\\MAP.png"), null, skinRatio, 30);
 
              curBitmap = ip.MKMakeupTransfer(curBitmap, new Bitmap(Application.StartupPath + "\\M3.JPG"), curFacePoints, eyePoints);
              DateTime end = DateTime.Now;
                  label1.Text = "TC: " + (end - start).ToString();
              pictureBox1.Image = curBitmap;
          }
      }
 [DllImport("TestDemo_C.dll", CallingConvention = CallingConvention.Cdecl, CharSet = CharSet.None, ExactSpelling = true)]
        private static extern int IN_Pic_MakeupTransfer(byte* srcData, int width, int height, int stride, int[] srcFacePointsAll, byte* maskData, int mWidth, int mHeight, int mStride, int[] mKeyPointsAll);
        public Bitmap MKMakeupTransfer(Bitmap src, Bitmap mask, int[] srcFacePointsAll, int[] mskFacePointsAll)
        {
            Bitmap a = new Bitmap(src);
            int w = a.Width;
            int h = a.Height;
            BitmapData srcData = a.LockBits(new Rectangle(0, 0, a.Width, a.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData mskData = mask.LockBits(new Rectangle(0, 0, mask.Width, mask.Height), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            IN_Pic_MakeupTransfer((byte*)srcData.Scan0, w, h, srcData.Stride, srcFacePointsAll,(byte*)mskData.Scan0, mask.Width, mask.Height, mskData.Stride,  mskFacePointsAll);
            a.UnlockBits(srcData);
            mask.UnlockBits(mskData);
            return a;
        }
```

最后，给一个测试DEMO：[点击打开链接](http://download.csdn.net/detail/trent1985/9817706)



如果问题，请联系QQ：1358009172