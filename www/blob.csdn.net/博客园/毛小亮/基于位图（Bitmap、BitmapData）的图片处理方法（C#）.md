# 基于位图（Bitmap、BitmapData）的图片处理方法（C#） - 毛小亮 - 博客园
# [基于位图（Bitmap、BitmapData）的图片处理方法（C#）](https://www.cnblogs.com/xianerwonder/p/6124658.html)
**目前操作位图的主流方法有三种:**
　　1、基于Bitmap像素的处理方法，以GetPixel()和SetPixel()方法为主。方法调用简单，但是效率偏低。
　　2、基于内存的像素操作方法，以System.Runtime.InteropServices.Marshal.Copy()方法将数据变为非托管资源，操作后再写入内存。
　　3、基于指针的操作方式，效率最高，但是对使用者的能力有要求，能力不够者容易造成内存溢出。
**第二种方法的一个实例:**
```
1 //大图逐行遍历，y为行索引
 2             for (var y = 0; y < destHeight; y++)
 3             {
 4                 //小图。把一行数据读入数组。第一个参数是起始位。
 5                 System.Runtime.InteropServices.Marshal.Copy(srcScan0 + y * srcStride, srcBuffer, 0, srcStride);
 6                 //大图。
 7                 System.Runtime.InteropServices.Marshal.Copy(dstScan0 + y * dstStride, dstBuffer, 0, dstStride);
 8 
 9                 //大图逐列，rgb三色.(与源码相比做了修改,遍历长度减小，数值因为是灰度值，计算了变为了三分之一。)
10                 for (var x = 0; x < destWidth; x ++)
11                 {
12                     //字节总索引
13                     int fullIndex = 3*x;
14                     //相乘，再除以255。返回一个byte
15                     //dstBuffer[x] = channelProcessFunction(ref srcBuffer[x], ref dstBuffer[x]);
16                     var blendValue = channelProcessFunction(ref srcBuffer[fullIndex], ref dstBuffer[fullIndex]);
17                     dstBuffer[fullIndex + 2] = blendValue;
18                     dstBuffer[fullIndex + 1] = blendValue;
19                     dstBuffer[fullIndex] = blendValue;
20                 }
21                 //写回托管内存
22                 System.Runtime.InteropServices.Marshal.Copy(dstBuffer, 0, dstScan0 + y * dstStride, dstStride);
23             }
基于内存的像素操作
```
**而对于灰度图的处理，可以使用调整gamma值的方式。对于灰度图处理的一个实例：**
```
1 /// <summary>
 2         /// 把小图片按照权重，重设gamma值，重新渲染。可得到加权重后的小图，背景为白色，非透明。
 3         /// </summary>
 4         /// <param name="image">小图片</param>
 5         /// <param name="weight">权重</param>
 6         /// <returns>加权重后的小图，背景为白色，非透明</returns>
 7         private static Bitmap ApplyHeatValueToImage(Bitmap image, float weight)
 8         {
 9             //新建临时位图
10             var tempImage = new Bitmap(image.Width, image.Height, PixelFormat.Format32bppPArgb);
11 
12             using (var g = Graphics.FromImage(tempImage))
13             {
14                 //把权重参数映射为[0.1-5]，以便后面进行gamma的矫正。gamma值正常范围为1.0 - 2.2
15                 //此处拓展，可以让色彩更加intense
16                 //gamma值可用来进行灰度值层面的明暗矫正，改善失真。
17                 ////I want to make the color more intense (White/bright)
18                 if (weight < 0.02f) weight = 0.02f;//最小0.02
19                 weight *= 5f;
20                 if (weight > 5f) weight = 5f;
21 
22                 // Create ImageAttributes
23                 var ia = new ImageAttributes();
24 
25                 //Gamma values range from 0.1 to 5.0 (normally 0.1 to 2.2), with 0.1 being the brightest and 5.0 the darkest.
26                 //Convert the 100% to a range of 0.1-5 by multiplying it by 5
27                 ia.SetGamma(weight, ColorAdjustType.Bitmap);
28 
29                 //在image中 重绘
30                 // Draw Image with the attributes
31                 g.DrawImage(image,
32                             new Rectangle(0, 0, image.Width, image.Height),//这里如果size设小一点，可以对目标图像进行缩放。如果小于Graphics的尺寸，则会出现白边
33                             0, 0, image.Width, image.Height,//这里可以对源图像进行裁剪。
34                             GraphicsUnit.Pixel, ia);
35             }
36             //New dot with a different intensity
37             return tempImage;
38         }
```

