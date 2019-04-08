# 图像滤镜艺术---Wave滤镜


  Wave Filter水波滤镜
水波滤镜是通过坐标变换来模拟水波效果，使图像呈现出水波的特效。这个滤镜有一个可调参数：水波的扭曲程度。
代码如下;

```csharp
//
        ///
        /// Wave Filter
        ///
        /// Source image.
        /// The degree of wave,0-100.
        /// The result image.
        private Bitmap WaveFilterProcess(Bitmap srcBitmap, int degree)
        {
            Bitmap a = new Bitmap(srcBitmap);
            int w = a.Width;
            int h = a.Height;
            degree = degree * 32 / 100;
            degree = Math.Max(0, Math.Min(32, degree));
            Bitmap dst = new Bitmap(w, h);
            System.Drawing.Imaging.BitmapData srcData = a.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            System.Drawing.Imaging.BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
            unsafe
            {
                byte* pIn = (byte)srcData.Scan0.ToPointer();
                byte pOut = (byte)dstData.Scan0.ToPointer();
                byte p = null;
                int stride = srcData.Stride - w * 4;
                int newX = 0, newY = 0;
                double PI2 = Math.PI * 2.0;  
                for (int y = 0; y < h; y++)
                {
                    for (int x = 0; x < w; x++)
                    {
                        newX = (int)(degree * Math.Sin(PI2 * y / 128.0)) + x;
                        newY = (int)(degree * Math.Cos(PI2 * x / 128.0)) + y;
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



水波滤镜效果图

最后放上一个完整的C#版程序Demo下载地址：http://www.zealpixel.com/thread-59-1-1.html
--------------------- 
作者：Trent1985 
来源：CSDN 
原文：https://blog.csdn.net/Trent1985/article/details/48464753 
版权声明：本文为博主原创文章，转载请附上博文链接！