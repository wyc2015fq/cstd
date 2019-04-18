# 安卓SDK之YUV-Image - wishchinYang的专栏 - CSDN博客
2015年06月23日 11:27:49[wishchin](https://me.csdn.net/wishchin)阅读数：3265
       类参考链接：[http://api.apkbus.com/reference/android/graphics/YuvImage.html](http://api.apkbus.com/reference/android/graphics/YuvImage.html)
**1. 引入包：    **
```java
import java.io.ByteArrayOutputStream;
import android.hardware.Camera;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.YuvImage;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.Region;
```
       安卓YUV_IMage包含四元组的YUV数据，contains YUV data and provides a method that compresses a region of the YUV data to a Jpeg，提供了一个向jpeg格式压缩的方法。       
[YuvImage](http://api.apkbus.com/reference/android/graphics/YuvImage.html#YuvImage%28byte%5B%5D,%20int,%20int,%20int,%20int%5B%5D%29)(byte[] yuv, int format,
 int width, int height, int[] strides)
Construct an YuvImage.
int[getYuvFormat](http://api.apkbus.com/reference/android/graphics/YuvImage.html#getYuvFormat%28%29)()
**释义：  **
Construct an YuvImage.
##### 参数
strides(Optional) Row bytes of each image plane. If yuv contains padding, the stride of each image must be provided. If strides is null, the method assumes no padding and derives the row bytes by format and width itself.
##### 抛出
[IllegalArgumentException](http://api.apkbus.com/reference/java/lang/IllegalArgumentException.html)if format is not support; width or height <= 0; or yuv is null.
**2.  使用到的缓存类**
参考链接：[http://www.apihome.cn/api/java/ByteArrayOutputStream.html](http://www.apihome.cn/api/java/ByteArrayOutputStream.html)
参考链接：[http://blog.chinaunix.net/uid-9789791-id-1997411.html](http://blog.chinaunix.net/uid-9789791-id-1997411.html)
**3. 转换为矩阵数据使用的代码**
参考链接：[http://eyehere.net/2011/android-camera-2/](http://eyehere.net/2011/android-camera-2/)
```java
//@Override
	public void changeYUV(byte[] data, Bitmap bmp)
	{
		final int Width  = myCamera.getParameters().getPreviewSize().width;
		final int Height = myCamera.getParameters().getPreviewSize().height;
		 YuvImage image = new YuvImage(data, ImageFormat.NV21,Width, Height, null);
		
		 if(image!=null){
             ByteArrayOutputStream stream = new ByteArrayOutputStream();
             image.compressToJpeg(new Rect(0, 0, Width, Height), 80, stream);
             //Bitmap bmp = BitmapFactory.decodeByteArray(stream.toByteArray(), 0, stream.size());
             bmp = BitmapFactory.decodeByteArray(stream.toByteArray(), 0, stream.size());
             stream.close();
             
         }
	}
```
