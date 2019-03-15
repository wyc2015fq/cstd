# 图像滤镜艺术---(Sketch Filter)素描滤镜


  (Sketch Filter)素描滤镜

素描滤镜的实现方法比较简单，这里我们直接写出算法过程如下：

1，对原图S进行去色命令得到灰度图A；

2，对A进行反色得到图像B；

3，对B进行高斯模糊得到图C；

4，将C与B进行颜色减淡的图层混合算法：

P(x,y) = Pb(x,y) + (Pb(x,y)* Pc(x,y)) / (256 - Pc(x,y));

在步骤3中，可以添加变量edgeIntensity(edgeIntensity>=0)，来控制素描的边缘强度；

以上就是素描的算法实现。

核心代码如下：

```csharp
private Bitmap SketchFilterProcess(Bitmap src, int edgeIntensity)
        {
            Bitmap gaussBitmap = gf.Apply(src, edgeIntensity);   
            Bitmap dst = new Bitmap(src);
            int w = dst.Width;
            int h = dst.Height;
            BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData edgeData = gaussBitmap.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            byte* pGauss = (byte*)edgeData.Scan0;
            byte* pDst = (byte*)dstData.Scan0;
            int offset = dstData.Stride - w * 4;
            int gray, graySrc, grayGauss;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    graySrc = (pDst[0] + pDst[1] + pDst[2]) / 3;
                    grayGauss = 255 - (pGauss[0] + pGauss[1] + pGauss[2]) / 3;
                    gray = graySrc + (graySrc * grayGauss) / (256 - grayGauss);
                    gray = Math.Min(255, Math.Max(0, gray));
                    pDst[0] = (byte)gray;
                    pDst[1] = (byte)gray;
                    pDst[2] = (byte)gray;
                    pDst[3] = (byte)255;
                    pGauss += 4;
                    pDst += 4;
                }
                pGauss += offset;
                pDst += offset;
            }
            dst.UnlockBits(dstData);
            gaussBitmap.UnlockBits(edgeData);
            return dst;
        }
```

效果图如下所示：



原图



素描滤镜效果图

最后放上完整C#/C程序DEMO下载连接：

http://www.zealpixel.com/thread-64-1-1.html

作者：Trent1985 
来源：CSDN 
原文：https://blog.csdn.net/Trent1985/article/details/48393395 
版权声明：本文为博主原创文章，转载请附上博文链接！