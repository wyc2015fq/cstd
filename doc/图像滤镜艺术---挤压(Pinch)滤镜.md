# 图像滤镜艺术---挤压(Pinch)滤镜


  Pinch滤镜
Pinch滤镜是通过坐标变换来实现以某个点(cenX,cenY)为中心，某个半径R内图像向其挤压变形的效果。实现这个滤镜的算法很多，主要是数学公式的不同，大家可以自行设计，这里给个代码示例，大家可以直接使用。
代码如下：


```csharp
    //
        /// <summary>
        /// Pinch Filter
        /// </summary>
        /// <param name="src">Source image.</param>
        /// <param name="cenX">The X position of sun.</param>
        /// <param name="cenY">The Y position of sun.</param>
        /// <returns>The result image.</returns>
        private Bitmap PinchFilterProcess(Bitmap srcBitmap, int cenX, int cenY)
        {
            Bitmap a = new Bitmap(srcBitmap);
            int w = a.Width;
            int h = a.Height;
            int radius = 0;
            Bitmap dst = new Bitmap(w, h);
            System.Drawing.Imaging.BitmapData srcData = a.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            System.Drawing.Imaging.BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            unsafe
            {
                byte* pIn = (byte)srcData.Scan0.ToPointer();
                byte pOut = (byte)dstData.Scan0.ToPointer();
                byte p = null;
                int sWidth = srcData.Stride;
                int stride = sWidth - w * 4;
                int offsetX = 0, offsetY = 0;
                int newX = 0, newY = 0;
                double radian = 0,degree = 10;
                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        offsetX = x - cenX;
                        offsetY = y - cenY;
                        radian = Math.Atan2(offsetY, offsetX);
                        radius = (int)(Math.Sqrt(offsetX * offsetX + offsetY * offsetY));
                        radius = (int)(Math.Sqrt(radius) * degree);
                        newX = (int)(radius * Math.Cos(radian)) + cenX;
                        newY = (int)(radius * Math.Sin(radian)) + cenY;
                        newX = Math.Min(w - 1, Math.Max(0, newX));
                        newY = Math.Min(h - 1, Math.Max(0, newY));
                        p = pIn + newY * srcData.Stride + newX * 4;
                        pOut[0] = (byte)p[0];
                        pOut[1] = (byte)p[1];
                        pOut[2] = (byte)p[2];
                        pOut[3] = (byte)255;                     
                        pOut += 4;
                    }
                    pOut += stride;
                }
                a.UnlockBits(srcData);
                dst.UnlockBits(dstData);
            }
            return dst;
    }
```
效果图如下：


原图



Pinch滤镜效果图

最后放上一个完整的C#版程序Demo给大家下载使用：
http://www.zealpixel.com/forum.php?mod=viewthread&tid=55&extra=page%3D1
--------------------- 
作者：Trent1985 
来源：CSDN 
原文：https://blog.csdn.net/Trent1985/article/details/48464801 
版权声明：本文为博主原创文章，转载请附上博文链接！