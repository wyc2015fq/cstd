# 图像滤镜艺术---LOMO Filter


  LOMO Filter
LOMO是一种概念，即强调感受、机缘，弱化摄影技巧，不确定性和随意性是LOMO最大特点。LOMO源于Lomography，LOMO相机，它原先是苏联的产物，由列宁格勒光学与机械联盟于1980年制造出品，是一台为了大众需求而制造的傻瓜相机。常见的lomo照片构图没有章法，色彩浓郁，焦点不实，曝光不准，晃动糊片也很普遍。
LOMO的风格多种多样，本文简单介绍一种风格。
具体算法实现如下：
1，原图A与原图使用“柔光”图层混合，得到图B，公式如下：
        

```csharp
private int ModeSmoothLight(int basePixel, int mixPixel)
        {
            int res = 0;
            res = mixPixel > 128 ? ((int)((float)basePixel + ((float)mixPixel + (float)mixPixel - 255.0f) * ((Math.Sqrt((float)basePixel / 255.0f)) * 255.0f - (float)basePixel) / 255.0f)) :
                  ((int)((float)basePixel + ((float)mixPixel + (float)mixPixel - 255.0f) * ((float)basePixel - (float)basePixel * (float)basePixel / 255.0f) / 255.0f));
            return Math.Min(255, Math.Max(0, res)); 
        }
```

2，B与一种自己设定的风格色(比如蓝色：R-200,G-37,B-11)进行“排除”图层混合，设定40%透明度，得到图C，具体公式如下：
        

```csharp
private int ModeExclude(int basePixel, int mixPixel)
        {
            int res = 0;
            res = (mixPixel + basePixel) - mixPixel * basePixel / 128;
            return Math.Min(255, Math.Max(0, res));
        }
```

3，选则一种暗角模板，与C进行“叠加”图层混合，得到最终的LOMO效果，当然，如果不喜欢暗角效果，此步骤可以忽略；
核心代码如下：
        

```csharp
private Bitmap LOMOFilterProcess(Bitmap src)
        {
            Bitmap srcBitmap = new Bitmap(src);
            Bitmap dst = new Bitmap(src);
            int w = dst.Width;
            int h = dst.Height;
            BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadWrite, PixelFormat.Format32bppArgb);
            BitmapData srcData = srcBitmap.LockBits(new Rectangle(0, 0, w, h), ImageLockMode.ReadOnly, PixelFormat.Format32bppArgb);
            byte* pSrc = (byte*)srcData.Scan0;
            byte* pDst = (byte*)dstData.Scan0;
            int offset = dstData.Stride - w * 4;
            int r,g,b;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    b = ModeSmoothLight(pSrc[0], pDst[0]);
                    g = ModeSmoothLight(pSrc[1], pDst[1]);
                    r = ModeSmoothLight(pSrc[2], pDst[2]);
                    b = ModeExclude(b, 80);
                    g = ModeExclude(g, 15);
                    r = ModeExclude(r, 5);
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


原图



LOMO Filter效果图

最后，放上一个完整C#程序DEMO的下载链接：
http://www.zealpixel.com/thread-66-1-1.html
--------------------- 
作者：Trent1985 
来源：CSDN 
原文：https://blog.csdn.net/Trent1985/article/details/48371111 
版权声明：本文为博主原创文章，转载请附上博文链接！