# 图像滤镜艺术---Swirl滤镜


  Swirl Filter
Swirl 滤镜是实现图像围绕中心点(cenX,cenY)扭曲旋转的效果，效果图如下：


原图



效果图

代码如下：

          //
            ///
            /// Swirl Filter
            ///
            /// Source image.
            /// The X position of Swirl.
            /// The Y position of Swirl.
            /// The degree of swirl,0-360.
            /// The result image.
            private Bitmap SwirlFilterProcess(Bitmap srcBitmap, int cenX, int cenY, int swilDegree)
            {
                Bitmap a = new Bitmap(srcBitmap);
                int w = a.Width;
                int h = a.Height;
                int radius = 0;
                swilDegree = Math.Max(0, Math.Min(360, swilDegree));
                double k = swilDegree / 3600.0;
                Bitmap dst = new Bitmap(w, h);
                System.Drawing.Imaging.BitmapData srcData = a.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
    
                System.Drawing.Imaging.BitmapData dstData = dst.LockBits(new Rectangle(0, 0, w, h), System.Drawing.Imaging.ImageLockMode.ReadWrite, System.Drawing.Imaging.PixelFormat.Format32bppArgb);
    
                unsafe
                {
                    byte* pIn = (byte*)srcData.Scan0.ToPointer();
                    byte* pOut = (byte*)dstData.Scan0.ToPointer();
                    byte* p = null;
                    int stride = srcData.Stride - w * 4;
                    int offsetX = 0, offsetY = 0;
                    int newX = 0, newY = 0;
                    double radian = 0;
                    for (int y = 0; y < h; y++)
                    {
                        for (int x = 0; x < w; x++)
                        {
                            offsetX = x - cenX;
                            offsetY = y - cenY;
                            radian = Math.Atan2(offsetY, offsetX);
                            radius = (int)(Math.Sqrt(offsetX * offsetX + offsetY * offsetY));
                            newX = (int)(radius * Math.Cos(radian + k * radius)) + cenX;
                            newY = (int)(radius * Math.Sin(radian + k * radius)) + cenY;
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



最后放上一个完整的C#程序Demo下载地址：http://www.zealpixel.com/thread-58-1-1.html，另外，推荐一个网址www.zealpixel.com，有很多不错的图像处理开源代码！

作者：Trent1985 
来源：CSDN 
原文：https://blog.csdn.net/Trent1985/article/details/48464727 
版权声明：本文为博主原创文章，转载请附上博文链接！