# 图像滤镜艺术---(Nostalgla Filter)老照片滤镜


  (Nostalgla Filter)老照片滤镜
Nostalgla Filter主要是通过算法来模拟一种复古，陈旧的照片风格，以此来达到新照片显示老照片的效果。目前这个算法有很多，本文在这里介绍一种更逼真，效率更高的新算法：
1，老照片一般都具有泛黄的纸张特征，这一点，我们构建一张暗黄色的地板图层A：RGB(200,120,10)；
2，老照片还具有一些裂纹特征，当然，并非所有老照片都这样，这里添加裂纹特征，更形象更逼真，因此，我们这里可以使用PS构建一张裂纹模板B，如下图所示：

3，将原始照片C进行灰度化处理得到灰度图D，这里可以使用最简单的灰度公式：
Gray = (R + G + B) / 3;

4，将D与A进行柔光操作，得到图E，柔光算法如下：

```csharp
       int ModeSmoothLight(int basePixel, int mixPixel)
        {
            int res = 0;
            res = mixPixel > 128 ? ((int)((float)basePixel + ((float)mixPixel + (float)mixPixel - 255.0f) * ((Math.Sqrt((float)basePixel / 255.0f)) * 255.0f - (float)basePixel) / 255.0f)) :
                  ((int)((float)basePixel + ((float)mixPixel + (float)mixPixel - 255.0f) * ((float)basePixel - (float)basePixel * (float)basePixel / 255.0f) / 255.0f));
            return Math.Min(255, Math.Max(0, res));
        }
```

5，将E与B进行柔光操作得到最后的效果图F；
以上就是老照片滤镜的算法实现了，算法比较简单，但是效果很不错！
核心代码如下：

```csharp
private Bitmap NostalglaFilterProcess(Bitmap src,Bitmap mask)
        {
            Bitmap dst = new Bitmap(src);
            int w = dst.Width;
            int h = dst.Height;
            BitmapData maskData = mask.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData srcData = dst.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            byte* p = (byte*)srcData.Scan0;
            byte* pMask = (byte*)maskData.Scan0;
            int offset = srcData.Stride - w * 4;
            int b = 10, g = 130, r = 200, gray = 0;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    gray = (p[0] + p[1] + p[2]) / 3;
                    b = ModeSmoothLight(gray, b);
                    g = ModeSmoothLight(gray, g);
                    r = ModeSmoothLight(gray, r);
                    p[0] = (byte)ModeSmoothLight(b,pMask[0]);
                    p[1] = (byte)ModeSmoothLight(g, pMask[1]);
                    p[2] = (byte)ModeSmoothLight(r, pMask[2]);
                    p += 4;
                    pMask += 4;
                }
                p += offset;
                pMask += offset;
            }
            dst.UnlockBits(srcData);
            mask.UnlockBits(maskData);
            return dst;
        }
```

效果图如下：

[![img](http://www.zealpixel.com/data/attachment/portal/201507/29/101414z8vjqro79djycfix.jpg)](http://www.zealpixel.com/data/attachment/portal/201507/29/101414z8vjqro79djycfix.jpg)

原图

[![img](http://www.zealpixel.com/data/attachment/portal/201507/29/101413ozht0hqlsu94yddu.png)](http://www.zealpixel.com/data/attachment/portal/201507/29/101413ozht0hqlsu94yddu.png)

老照片滤镜效果图

最后放上一个完整C#程序DEMO的下载地址：<http://www.zealpixel.com/thread-69-1-1.html>