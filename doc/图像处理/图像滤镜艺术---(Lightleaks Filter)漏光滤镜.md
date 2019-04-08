# 图像滤镜艺术---(Lightleaks Filter)漏光滤镜

  (Lightleaks Filter)漏光滤镜
漏光拍摄其实就是一种摄影手法，最初是因为强烈光照导致相片交卷的过分曝光,最终在成像时的出现局部漏光。而漏光滤镜实际上就是模拟这种拍摄效果而已。漏光的效果颜色多种多样，我们可以人为的添加各种颜色的漏光模板，然后通过算法将这种模板与真实照片融合起来，从而呈现出我们想要的漏光效果。
我们的漏光滤镜算法如下：
1，选择漏光模板A；
2，将漏光模板A与原图B进行“叠加”图层混合，即可得到漏光效果图C；
叠加算法如下：

```csharp
int ModeSuperposition(int basePixel,int mixPixel)//基色 < = 128：结果色 = 混合色 * 基色 / 128；基色 > 128：结果色 = 255 - （255 - 混合色）* (255 - 基色) / 128
{
    int res = 0;
    res = ((basePixel <= 128) ? (mixPixel * basePixel / 128):(255 - (255 - mixPixel) * (255 - basePixel) / 128));
    return CheckRange(res);
};
```

这个算法其实很简单，这里我们给出核心代码如下：

```csharp
private Bitmap LightleaksFilterProcess(Bitmap src,Bitmap mask)
        {
            Bitmap srcBitmap = new Bitmap(mask);
            Bitmap dst = new Bitmap(src);
            int w = dst.Width;
            int h = dst.Height;
            BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData srcData = srcBitmap.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            byte* pSrc = (byte*)srcData.Scan0;
            byte* pDst = (byte*)dstData.Scan0;
            int offset = dstData.Stride - w * 4;
            int r,g,b,gray;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    b = ((pDst[0] <= 128) ? (pSrc[0] * pDst[0] / 128) : (255 - (255 - pSrc[0]) * (255 - pDst[0]) / 128));
                    b = Math.Min(255, Math.Max(0, b));
                    g = ((pDst[1] <= 128) ? (pSrc[1] * pDst[1] / 128) : (255 - (255 - pSrc[1]) * (255 - pDst[1]) / 128));
                    g = Math.Min(255, Math.Max(0, g));
                    r = ((pDst[2] <= 128) ? (pSrc[2] * pDst[2] / 128) : (255 - (255 - pSrc[2]) * (255 - pDst[2]) / 128));
                    r = Math.Min(255, Math.Max(0, r));
                    pDst[0] = (byte)b;
                    pDst[1] = (byte)g;
                    pDst[2] = (byte)r;
                    pDst[3] = (byte)255;
                    pSrc += 4;
                    pDst += 4;
                }
                pSrc += offset;
                pDst += offset;
            }
            dst.UnlockBits(dstData);
            srcBitmap.UnlockBits(srcData);
            return dst;
        }
```

效果图如下：

[![img](http://www.zealpixel.com/data/attachment/portal/201507/28/104729vdvdawttzt5avsxl.jpg)](http://www.zealpixel.com/data/attachment/portal/201507/28/104729vdvdawttzt5avsxl.jpg)

原图

[![img](http://www.zealpixel.com/data/attachment/portal/201507/28/104917x6oaic6aooyoxaoa.jpg)](http://www.zealpixel.com/data/attachment/portal/201507/28/104917x6oaic6aooyoxaoa.jpg)

漏光模板图

[![img](http://www.zealpixel.com/data/attachment/portal/201507/28/104727uvvu2qyc1usumhy9.png)](http://www.zealpixel.com/data/attachment/portal/201507/28/104727uvvu2qyc1usumhy9.png)

Lightleaks Filter效果图

最后放上一个完整的C#程序DEMO下载地址：<http://www.zealpixel.com/thread-67-1-1.html>

