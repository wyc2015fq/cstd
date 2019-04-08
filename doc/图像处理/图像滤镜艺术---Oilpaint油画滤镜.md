# 图像滤镜艺术---Oilpaint油画滤镜

2015年09月13日 08:17:26

 

Trent1985

 

阅读数：2974

更多

所属专栏： [图像滤镜艺术](https://blog.csdn.net/column/details/zphotoimagefilter.html)



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Trent1985/article/details/48412609

 Oilpaint油画滤镜

​    图像油画效果实际上是将图像边缘产生一种朦胧，雾化的效果，同时，将一定的边缘模糊化，这样图像整体上看去像素与像素之间就像雾一样随机呈现。

 算法过程如下：

 假设当前像素为P(x,y)，他的随机位置为Pd(dx,dy)，那么算法公式如下：

[![img](http://www.zealpixel.com/data/attachment/portal/201507/19/124640k94d1gpdecvpctte.png)](http://www.zealpixel.com/data/attachment/portal/201507/19/124640k94d1gpdecvpctte.png)

 其中，K(v)为最大值不大于v的随机数正数，v为雾化阈值，v值越大，雾化程度越明显，反之，雾化程度越小，v=0时，图像无变化效果。

 核心代码如下：

```csharp
        /// <summary>
        /// Mosaic filter.
        /// </summary>
        /// <param name="src">Source  image.</param>
        /// <param name="blockSize">The size of mosaic effect.</param>
        /// <returns>Resullt image.</returns>
        public Bitmap OilpaintFilter(Bitmap src, int intensity)
        {
            Bitmap srcBitmap = new Bitmap(src);
            int w = srcBitmap.Width;
            int h = srcBitmap.Height;
            System.Drawing.Imaging.BitmapData srcData = srcBitmap.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            IntPtr ptr = srcData.Scan0;
            int bytes = h * srcData.Stride;
            byte[] srcValues = new byte[bytes];
            System.Runtime.InteropServices.Marshal.Copy(ptr, srcValues, 0, bytes);
            byte[] tempValues = (byte[])srcValues.Clone();
            int stride = srcData.Stride;
            Random ran = new Random();
            int k = 0;
            int dx = 0;
            int dy = 0;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    k = ran.Next(intensity);
                    dx = (i + k) >= w ? w - 1 : (i + k);
                    dy = (j + k) >= h ? h - 1 : (j + k);
                    tempValues[i * 4 + j * w * 4] = (byte)srcValues[dx * 4 + dy * w * 4];
                    tempValues[i * 4 + 1 + j * w * 4] = (byte)srcValues[dx * 4 + 1 + dy * w * 4];
                    tempValues[i * 4 + 2 + j * w * 4] = (byte)srcValues[dx * 4 + 2 + dy * w * 4];
                }
            }
            srcValues = (byte[])tempValues.Clone();
            System.Runtime.InteropServices.Marshal.Copy(srcValues, 0, ptr, bytes);
            srcBitmap.UnlockBits(srcData);
            return srcBitmap;
        }
```

 图像油画滤镜效果如下：

[![img](http://www.zealpixel.com/data/attachment/portal/201507/19/124819mlyntu43nlt3rt38.jpg)](http://www.zealpixel.com/data/attachment/portal/201507/19/124819mlyntu43nlt3rt38.jpg)

原图

[![img](http://www.zealpixel.com/data/attachment/portal/201507/19/124820l5tvmt2pggmzbvkb.png)](http://www.zealpixel.com/data/attachment/portal/201507/19/124820l5tvmt2pggmzbvkb.png)

Oilpaint滤镜效果

最后，放上一个完整的C#版程序Demo下载链接：<http://www.zealpixel.com/forum.php?mod=viewthread&tid=52&extra=page%3D1>