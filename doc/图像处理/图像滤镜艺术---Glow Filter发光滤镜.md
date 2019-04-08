# 图像滤镜艺术---Glow Filter发光滤镜


  Glow Filter发光滤镜
Glow Filter发光滤镜是一种让图像产生发光效果的滤镜，它的实现算法如下：
1，对原图P进行高斯模糊得到图像A；
2，将P和A进行“叠加”图层混合处理，公式如下：
Result(x,y) = ((basePixel(x,y) <= 128) ? (mixPixel(x,y) * basePixel(x,y) / 128):(255 - (255 - mixPixel(x,y)) * (255 - basePixel(x,y)) / 128));
注意：Result(x,y)属于[0-255];
以上就是发光滤镜的原理。
核心代码如下：

```csharp
private Bitmap GlowFilterProcess(Bitmap src)
        {
            Bitmap gaussBitmap = gf.Apply(src, 15);   
            Bitmap dst = new Bitmap(src);
            int w = dst.Width;
            int h = dst.Height;
            BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData gaussData = gaussBitmap.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            byte* pGauss = (byte*)gaussData.Scan0;
            byte* pDst = (byte*)dstData.Scan0;
            int offset = dstData.Stride - w * 4;
            int gray;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    gray = ((pDst[0] <= 128) ? (pGauss[0] * pDst[0] / 128) : (255 - (255 - pGauss[0]) * (255 - pDst[0]) / 128));
                    gray = Math.Min(255, Math.Max(0, gray));
                    pDst[0] = (byte)gray;
                    gray = ((pDst[1] <= 128) ? (pGauss[1] * pDst[1] / 128) : (255 - (255 - pGauss[1]) * (255 - pDst[1]) / 128));
                    gray = Math.Min(255, Math.Max(0, gray));
                    pDst[1] = (byte)gray;
                    gray = ((pDst[2] <= 128) ? (pGauss[2] * pDst[2] / 128) : (255 - (255 - pGauss[2]) * (255 - pDst[2]) / 128));
                    gray = Math.Min(255, Math.Max(0, gray));
                    pDst[2] = (byte)gray;
                    pDst[3] = (byte)255;
                    pGauss += 4;
                    pDst += 4;
                }
                pGauss += offset;
                pDst += offset;
            }
            dst.UnlockBits(dstData);
            gaussBitmap.UnlockBits(gaussData);
            return dst;
        }
```

效果图如下：

效果图如下：


原图



Glow Filter效果图

最后放上一个完整的 C# 程序Demo下载地址：
http://www.zealpixel.com/thread-65-1-1.html
--------------------- 
作者：Trent1985 
来源：CSDN 
原文：https://blog.csdn.net/Trent1985/article/details/48391517 
版权声明：本文为博主原创文章，转载请附上博文链接！