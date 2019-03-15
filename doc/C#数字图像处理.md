# C#数字图像处理

## C#数字图像处理--1 图像打开，保存

所属专栏： [Win8Metro图像处理（C#）](https://blog.csdn.net/column/details/image-c.html)


#### 作为本专栏的第一篇，必不可少的需要介绍一下图像的打开与保存，一便大家后面DEMO的制作。

**图像**

图像有多种含义,通常定义是指各种图形和影像的总称。古人也有对图像的描述：“既铭勒於钟鼎，又图像於丹青。” ---傅咸，“其神图像，皆合青石为之。” ---郦道元。数字图像又称数码图像或数位图像，是二维图像用有限数字数值像素的表示。而像素则是计算数字图像的一种单位，是计算机屏幕上所能显示的最小单位。如果将一张图像不断地放大，我们会发现，其实图像是由一个一个正方形点所组成，而这些点就是像素。如果这张图象大小为320*240，那么这就表明该图像的宽为320个像素的长度，高为240个像素的长度。

**图像格式**

目前的图像格式主要有：BMP格式，JPEG格式，GIF格式，PCX格式，TIFF格式，PNG格式，TGA格式，EXIF格式，FPX格式，SVG格式，PSD格式，CDR格式，PCD格式，DXF格式，UFO格式，EPS格式等。

**数字图像处理**

   数字图像处理(Digital Image Processing)又称为计算机图像处理，它是指将图像信号转换成数字信号并利用计算机对其进行处理的过程。


这里我们仅对位图Bitmap进行说明。

位图Bitmap又称栅格图，是使用像素阵列表示的图像，每个像素的颜色信息由RGB组合或者灰度值表示。根据颜色信息所需的数据位分为1、4、8、16、24及32位等，位数越高颜色越丰富，相应的数据量越大。其中使用1位表示一个像素颜色的位图因为一个数据位只能表示两种颜色，所以又称为二值位图。通常使用24位RGB组合数据位表示的的位图称为真彩色位图。

对于24位位图而言，它的每一个像素都是由3个颜色分量R，G，B所表示，每一个分量的大小为0-255，所占空间为一个字节即8位，这样每一张图像都可以表示为这么一个颜色矩阵，矩阵的长宽分别代表图像的高宽，以320*240大小图像为例，它有320*240个像素组成，每个像素都有R，G，B三个颜色分量，每一个分量用一个字节8位表示，则矩阵大小定义为byte[320*3,240]，表示图像高位240，宽为320*3个字节。

对于32位位图，则是在24位位图的基础上，每个像素多加了一个透明度Alpha通道，即多加了一个字节8位。

对于24位图像，RGB分量分别为0-255大小，它所代表的颜色空间如图1所示：


图1 RGB颜色空间

其中(0,0,0)代表黑色(255,255,255)代表白色。



  Win8Metro编程中，图像相关的操作基本都是以流的形式进行的，图像对象类型在Metro主要表现为两种形式：BitmapImage和WriteableBitmap，图像的显示控件为Image。

  我们可以用如下方式打开和显示一幅图像对象。

BitmapImage srcImage=newBitmapImage (new Uri(“UriPath”), UriKind.Relative)

  其中UriPath为图像的Uri地址，UriKind表示路径的选择，Urikind.Relative表示是相对路径，也可以选择绝对路径Urikind.Absolute，或者Urikind. RelativeOrAbsolute。

Image imageBox=newImage ();

imageBox.Source=srcImage;//将图像显示在imageBox控件中

  还有一种方法则是使用WriteableBitmap对象，这也是我这里要详细介绍的方法。

## 1.图像打开

```csharp
private static BitmapImage srcImage = newBitmapImage();
privatestaticWriteableBitmap wbsrcImage;
//open image fuctiondefinition
private async void OpenImage()
{
    FileOpenPicker imagePicker = newFileOpenPicker

      {
        ViewMode = PickerViewMode.Thumbnail,
		  SuggestedStartLocation = PickerLocationId.PicturesLibrary,
		  FileTypeFilter = { ".jpg",".jpeg",".png",".bmp" }
    };
    Guid decoderId;
    StorageFile imageFile = await imagePicker.PickSingleFileAsync();
    if (imageFile != null)
    {
        srcImage = newBitmapImage();
        using (IRandomAccessStream stream = await imageFile.OpenAsync(FileAccessMode.Read))
        {
            srcImage.SetSource(stream);
            switch (imageFile.FileType.ToLower())
            {
                case ".jpg":
                case ".jpeg":
                    decoderId = Windows.Graphics.Imaging.BitmapDecoder.JpegDecoderId;
                    break;
                case ".bmp":
                    decoderId = Windows.Graphics.Imaging.BitmapDecoder.BmpDecoderId;
                    break;
                case ".png":
                    decoderId = Windows.Graphics.Imaging.BitmapDecoder.PngDecoderId;
                    break;
                default:
                    return;
            }
            Windows.Graphics.Imaging.BitmapDecoder decoder = awaitWindows.Graphics.Imaging.BitmapDecoder.CreateAsync(decoderId, stream);
            int width = (int)decoder.PixelWidth;
            int height = (int)decoder.PixelHeight;
            Windows.Graphics.Imaging.PixelDataProvider dataprovider = await decoder.GetPixelDataAsync();
            byte[] pixels = dataprovider.DetachPixelData();
            wbsrcImage = newWriteableBitmap(width, height);
            Stream pixelStream = wbsrcImage.PixelBuffer.AsStream();
            //rgba in original
            //to display ,convert tobgra
            for (int i = 0; i < pixels.Length; i += 4)
            {
                byte temp = pixels[i];
                pixels[i] = pixels[i + 2];
                pixels[i + 2] = temp;
            }
            pixelStream.Write(pixels, 0, pixels.Length);
            pixelStream.Dispose();
            stream.Dispose();
        }
        ImageOne.Source = wbsrcImage;
        ImageOne.Width = wbsrcImage.PixelWidth;
        ImageOne.Height = wbsrcImage.PixelHeight;
    }
}
```



## 2.图像保存

```csharp
//save image fuction definition
private async void SaveImage(WriteableBitmap src)
{
	FileSavePicker save = newFileSavePicker();
	save.SuggestedStartLocation = PickerLocationId.PicturesLibrary;
	save.DefaultFileExtension = ".jpg";
	save.SuggestedFileName = "newimage";
	save.FileTypeChoices.Add(".bmp", newList<string>() { ".bmp" });
	save.FileTypeChoices.Add(".png", newList<string>() { ".png" });
	save.FileTypeChoices.Add(".jpg", newList<string>() { ".jpg", ".jpeg" });
	StorageFile savedItem = await save.PickSaveFileAsync();
	try
	{
		Guid encoderId;
		switch (savedItem.FileType.ToLower())
		{
		case".jpg":
			encoderId = BitmapEncoder.JpegEncoderId;
			break;
		case".bmp":
			encoderId = BitmapEncoder.BmpEncoderId;
			break;
		case".png":
		default:
			encoderId = BitmapEncoder.PngEncoderId;
			break;
		}
		IRandomAccessStream fileStream = await savedItem.OpenAsync(Windows.Storage.FileAccessMode.ReadWrite);
		BitmapEncoder encoder = awaitBitmapEncoder.CreateAsync(encoderId, fileStream);
		Stream pixelStream = src.PixelBuffer.AsStream();
		byte[] pixels = newbyte[pixelStream.Length];
		pixelStream.Read(pixels, 0, pixels.Length);
		//pixal format shouldconvert to rgba8
		for (int i = 0; i < pixels.Length; i += 4)
		{
			byte temp = pixels[i];
			pixels[i] = pixels[i + 2];
			pixels[i + 2] = temp;
		}
		encoder.SetPixelData(
			BitmapPixelFormat.Rgba8,
			BitmapAlphaMode.Straight,
			(uint)src.PixelWidth,
			(uint)src.PixelHeight,
			96, // Horizontal DPI
			96, // Vertical DPI
			pixels
		);
		await encoder.FlushAsync();
	}
	catch (Exception e)
	{
		throw e;
	}
}
```





## C#数字图像处理--2.1图像灰度化



**[函数说明]**

图像灰度化函数GrayProcess(WriteableBitmap src)

**[算法说明]**

  图像灰度化就是去掉彩色图像的彩色信息。对于一张图像，其中的每一个像素都

存在B,G,R三个颜色分量(这里不考虑透明度分量)，这三个分量在C#中是按照B→G

→R的顺序进行存储的，这三个分量的值分别取在0-255范围之内，对于不同取值，

相应的也就产生了不同的颜色信息。如果以X,Y,Z轴分别取代R,G,B分量构建三维坐

标系，则颜色分布如图2.1所示。

![img](https://img-my.csdn.net/uploads/201211/09/1352439302_4843.JPG)

公式2-(1)是根据我们人眼对颜色感知的特点以及人眼对色彩分辨的范围计算得到

的一个比较适于人眼感官的公式。

​    这里我们采用公式2-(1)，对每个像素运用该公式，得到的值即为像素的灰度值。

**[函数代码]**

```csharp
/// <summary>
/// Gray process.
/// </summary>
/// <param name="src">Source image.</param>
/// <returns></returns>
public static WriteableBitmap GrayProcess(WriteableBitmap src) ////1 灰度化处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap grayImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			byte tempByte = (byte)((int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299));
			temp[i] = tempByte;
			temp[i + 1] = tempByte;
			temp[i + 2] = tempByte;
		}
		Stream sTemp = grayImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return grayImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

![img](https://img-my.csdn.net/uploads/201211/09/1352439621_3515.JPG)



## C#数字图像处理--2.2图像二值化函数


![img](https://img-my.csdn.net/uploads/201211/20/1353398253_1029.JPG)

**[函数代码]**

```csharp
/// <summary>
/// Binary process.
/// </summary>
/// <param name="src">Source image.</param>
/// <param name="threshould">Define a threshould value for binary processing, from 0 to 255.</param>
/// <returns></returns>
public static WriteableBitmap BinaryProcess(WriteableBitmap src, int threshould)////2 二值化处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap binaryImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			byte tempByte = (byte)(((temp[i] + temp[i + 1] + temp[i + 2]) / 3) < threshould ? 0 : 255);
			temp[i] = tempByte;
			temp[i + 1] = tempByte;
			temp[i + 2] = tempByte;
		}
		Stream sTemp = binaryImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return binaryImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-my.csdn.net/uploads/201211/20/1353398389_5700.JPG)







## C#数字图像处理--2.3图像反色



**[函数名称]**

图像反色函数*ContraryProcess(**WriteableBitmap* *src)*

[算法说明]

​    反色公式如下：       P'(x,y) = 255 - P(x,y);

​    P'(x,y)为反色后的像素值，P(x,y)是原始像素值。



**[函数代码]**

```csharp

///<summary>
/// Contrary process.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static WriteableBitmap ContraryProcess(WriteableBitmap src)////3反色处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap contraryImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			temp[i] = (byte)(255 - temp[i]);
			temp[i + 1] = (byte)(255 - temp[i + 1]);
			temp[i + 2] = (byte)(255 - temp[i + 2]);
		}
		Stream sTemp = contraryImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return contraryImage;
	}
	else
	{
		returnnull;
	}
}

```

**![img](https://img-blog.csdn.net/20150410103230838?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**





## C#数字图像处理--2.4图像颜色聚类



**[函数名称]**

图像颜色聚类函数*ClusterProcess(**WriteableBitmap* *src,**int* *value)*





**[算法说明]**

  图像颜色聚类的方法有很多，但是对于视频监控而言，现有方法很难满足实时性的要求，这里介绍一种位屏蔽压缩的方法实现颜色聚类，可以满足实时性的要求。

  位屏蔽法就是在3D的RGB真彩空间中近似均匀采样的颜色压缩方法，即将屏蔽的颜色位置设置为0，具体可以采用移位运算来实现。

  一个像素的RGBA分量各占一个字节(这里指32位格式)，对于每一个字节，它的大小为[0,255]，用二进制位表示如下：

​                        0000 0000——1111 1111

  比如11111111移位3位为11111000，按这个方法，我们可以将其移位N(0<N<=7)位，在C#中表现为&操作，如11111111位移3位可表示为：255&248。

  用上述这个移位屏蔽算法,我们可以对彩色图像中的每一个像素进行这样的移位操作,那么图像像素值就会得到相应的颜色空间的压缩,从而达到聚类的效果。









**[函数代码]**



```csharp
///<summary>
/// Cluster process (a fast method be introduced here).
///</summary>
///<param name="src">Source image.</param>
///<param name="value">Choose one of {254-128}</param>
///<returns></returns>
public static WriteableBitmap ClusterProcess(WriteableBitmap src, int value)////4聚类处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap clusterImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			temp[i] = (byte)(temp[i] & value);
			temp[i + 1] = (byte)(temp[i + 1] & value);
			temp[i + 2] = (byte)(temp[i + 2] & value);
		}
		Stream sTemp = clusterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return clusterImage;
	}
	else
	{
		returnnull;
	}
}

```

**![img](https://img-blog.csdn.net/20150410103443267?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**









## C#数字图像处理--2.5图像亮度调整



## *2.5*图像亮度调整函数



**[函数名称]**

  图像亮度调整函数*BrightnessAdjustProcess(**WriteableBitmap* *src,* *int**brightnessValue)*

*![img](https://img-blog.csdn.net/20150410103624152?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*



**[函数代码]**

```csharp
///<summary>
/// Bright adjust process.
///</summary>
///<param name="src">Source image.</param>
///<param name="brightnessValue">Brightness value, from -255 to 255.</param>
///<returns></returns>
public static WriteableBitmap BrightnessAdjustProcess(WriteableBitmap src, int brightnessValue)////5亮度调整
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap brightImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			temp[i] = (byte)(((brightnessValue + temp[i]) > 255 ? 255 : (brightnessValue + temp[i])) < 0 ? 0 : ((brightnessValue + temp[i]) > 255 ? 255 : (brightnessValue + temp[i])));
			temp[i + 1] = (byte)(((brightnessValue + temp[i + 1]) > 255 ? 255 : (brightnessValue + temp[i + 1])) < 0 ? 0 : ((brightnessValue + temp[i + 1]) > 255 ? 255 : (brightnessValue + temp[i + 1])));
			temp[i + 2] = (byte)(((brightnessValue + temp[i + 2]) > 255 ? 255 : (brightnessValue + temp[i + 2])) < 0 ? 0 : ((brightnessValue + temp[i + 2]) > 255 ? 255 : (brightnessValue + temp[i + 2])));
		}
		Stream sTemp = brightImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return brightImage;
	}
	else
	{
		returnnull;
	}
}

```

**![img](https://img-blog.csdn.net/20150410103602035?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## C#数字图像处理--2.6图像对比度调整

﻿﻿

## *2.6*图像对比度调整函数

**[函数名称]**

图像对比度调整函数*ContrastAdjustProcess(**WriteableBitmap* *src,* *double**contrastValue)*



**![img](https://img-blog.csdn.net/20150407105536817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

**[函数代码]**

```csharp
///<summary>
/// Contrast adjust process.
///</summary>
///<param name="src">Source image.</param>
///<param name="contrastValue">Contrast value, from -1 to 1.</param>
///<returns></returns>
public static WriteableBitmap ContrastAdjustProcess(WriteableBitmap src, double contrastValue)////6对比度调整
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap contrastImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			temp[i] = Convert.ToByte((((temp[i] - 127.5) * contrastValue + temp[i]) > 255 ? 255 : ((temp[i] - 127.5) * contrastValue + temp[i])) < 0 ? 0 : (((temp[i] - 127.5) * contrastValue + temp[i]) > 255 ? 255 : ((temp[i] - 127.5) * contrastValue + temp[i])));
			temp[i + 1] = Convert.ToByte((((temp[i + 1] - 127.5) * contrastValue + temp[i + 1]) > 255 ? 255 : ((temp[i + 1] - 127.5) * contrastValue + temp[i + 1])) < 0 ? 0 : (((temp[i + 1] - 127.5) * contrastValue + temp[i + 1]) > 255 ? 255 : ((temp[i + 1] - 127.5) * contrastValue + temp[i + 1])));
			temp[i + 2] = Convert.ToByte((((temp[i + 2] - 127.5) * contrastValue + temp[i + 2]) > 255 ? 255 : ((temp[i + 2] - 127.5) * contrastValue + temp[i + 2])) < 0 ? 0 : (((temp[i + 2] - 127.5) * contrastValue + temp[i + 2]) > 255 ? 255 : ((temp[i + 2] - 127.5) * contrastValue + temp[i + 2])));
		}
		Stream sTemp = contrastImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return contrastImage;
	}
	else
	{
		returnnull;
	}
}

```

**![img](https://img-blog.csdn.net/20150410103920027?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**





﻿﻿Win8Metro(C#)数字图像处理--2.7图像伪彩色

﻿﻿

## *2.7*图像伪彩色函数

**[函数名称]**

图像伪彩色函数*PseudoColorProcess(**WriteableBitmap* *src)*



**[算法说明]**

  伪彩色是为改善视觉效果，利用计算机图像增强技术对图像的灰度赋予的不同假色彩，即，将一张灰度图转化为彩色图。主要原理是把灰度图像的各个不同灰度级按照线性或非线性的映射函数变换成为不同的彩色空间。

  本文采用基于RGB颜色空间的伪彩色映射算法。过程如下：

![img](https://img-blog.csdn.net/20150407105747639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



 **[函数代码]**

	 ```csharp
	 ///<summary>
	 /// Pseudo color process.
	 ///</summary>
	 ///<param name="src">Source image.</param>
	 ///<returns></returns>
	 public static WriteableBitmap PseudoColorProcess(WriteableBitmap src)////7伪彩色处理
 {
	 if (src != null)
	 {
		 int w = src.PixelWidth;
		 int h = src.PixelHeight;
		 WriteableBitmap pseudoImage = newWriteableBitmap(w, h);
		 byte[] temp = src.PixelBuffer.ToArray();
		 int tGray = 0;
		 for (int i = 0; i < temp.Length; i += 4)
		 {
			 tGray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			 if (tGray >= 0 && tGray <= 63)
			 {
				 temp[i] = (byte)255;
				 temp[i + 1] = (byte)(254 - 4 * tGray);
				 temp[i + 2] = 0;
			 }
			 if (tGray >= 64 && tGray <= 127)
			 {
				 temp[i] = (byte)(510 - 4 * tGray);
				 temp[i + 1] = (byte)(4 * tGray - 254);
				 temp[i + 2] = (byte)0;
			 }
			 if (tGray >= 128 && tGray <= 191)
			 {
				 temp[i] = (byte)0;
				 temp[i + 1] = (byte)255;
				 temp[i + 2] = (byte)(4 * tGray - 510);
			 }
			 if (tGray >= 192 && tGray <= 255)
			 {
				 temp[i] = (byte)0;
				 temp[i + 1] = (byte)(1022 - 4 * tGray);
				 temp[i + 2] = (byte)255;
			 }
			 tGray = 0;
		 }
		 Stream sTemp = pseudoImage.PixelBuffer.AsStream();
		 sTemp.Seek(0, SeekOrigin.Begin);
		 sTemp.Write(temp, 0, w * 4 * h);
		 return pseudoImage;
	 }
	 else
	 {
		 returnnull;
	 }
 }

```

**![img](https://img-blog.csdn.net/20150410104125030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



 Win8Metro(C#)数字图像处理--2.8图像线性变换
﻿﻿

**2.8图像线性变换**

**[函数名称]**

图像线性变换函数*LinearTransformProcess(**WriteableBitmap* *src,* *double* *k,**int* *v)*

![img](https://img-blog.csdn.net/20150407105917401?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**[函数代码]**

​```csharp
///<summary>
/// Linear transform process(f=kf+v).
///</summary>
///<param name="src">Source image.</param>
///<param name="k">Parameter,from 0 to 5.</param>
///<param name="v">Parameter,from -128 to 128.</param>
///<returns></returns>
public static WriteableBitmap LinearTransformProcess(WriteableBitmap src, double k, int v)////8线性变换处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap linearImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			temp[i] = (byte)(((k * temp[i] + v + 0.5) > 255 ? 255 : (k * temp[i] + v + 0.5)) < 0 ? 0 : ((k * temp[i] + v + 0.5) > 255 ? 255 : (k * temp[i] + v + 0.5)));
			temp[i + 1] = (byte)(((k * temp[i + 1] + v + 0.5) > 255 ? 255 : (k * temp[i + 1] + v + 0.5)) < 0 ? 0 : ((k * temp[i + 1] + v + 0.5) > 255 ? 255 : (k * temp[i + 1] + v + 0.5)));
			temp[i + 2] = (byte)(((k * temp[i + 2] + v + 0.5) > 255 ? 255 : (k * temp[i + 2] + v + 0.5)) < 0 ? 0 : ((k * temp[i + 2] + v + 0.5) > 255 ? 255 : (k * temp[i + 2] + v + 0.5)));
		}
		Stream sTemp = linearImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return linearImage;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

**![img](https://img-blog.csdn.net/20150410104646363?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

﻿﻿



## C#数字图像处理--2.9图像均值滤波


**[函数名称]**

图像均值滤波函数*MeanFilterProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150407110144697?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
///<summary>
/// Mean filter process.
///</summary>
///<param name="src">Source image</param>
///<returns></returns>
public static WriteableBitmap MeanFilterProcess(WriteableBitmap src)////9均值滤波处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				temp[i + j * w * 4] = (byte)((tempMask[i - 4 + (j - 1) * w * 4] + tempMask[i + (j - 1) * w * 4] + tempMask[i + 4 + (j - 1) * w * 4]
					+ tempMask[i - 4 + j * w * 4] + tempMask[i + 4 + j * w * 4] + tempMask[i - 4 + (j + 1) * w * 4] + tempMask[i + (j + 1) * w * 4]
					+ tempMask[i + 4 + (j + 1) * w * 4]) / 8);
				temp[i + 1 + j * w * 4] = (byte)((tempMask[i - 4 + 1 + (j - 1) * w * 4] + tempMask[i + 1 + (j - 1) * w * 4] + tempMask[i + 1 + 4 + (j - 1) * w * 4]
					+ tempMask[i + 1 - 4 + j * w * 4] + tempMask[i + 1 + 4 + j * w * 4] + tempMask[i + 1 - 4 + (j + 1) * w * 4] + tempMask[i + 1 + (j + 1) * w * 4]
					+ tempMask[i + 1 + 4 + (j + 1) * w * 4]) / 8);
				temp[i + 2 + j * w * 4] = (byte)((tempMask[i + 2 - 4 + (j - 1) * w * 4] + tempMask[i + 2 + (j - 1) * w * 4] + tempMask[i + 2 + 4 + (j - 1) * w * 4]
					+ tempMask[i + 2 - 4 + j * w * 4] + tempMask[i + 2 + 4 + j * w * 4] + tempMask[i + 2 - 4 + (j + 1) * w * 4] + tempMask[i + 2 + (j + 1) * w * 4]
					+ tempMask[i + 2 + 4 + (j + 1) * w * 4]) / 8);
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

![Win8Metro(C)数字图像处理--图像均值滤波 - CSharp - C](http://img8.ph.126.net/KyM6qnmnG1-oQHwuBcWCdg==/1006273041758331758.jpg)







## C#数字图像处理--2.10图像中值滤波


**[函数名称]**

图像中值滤波函数*MedianFilterProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150407110352708?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







**[函数代码]**

```csharp
///<summary>
/// Median filter process.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static Writeable Bitmap MedianFilterProcess(Writeable Bitmap src)////10中值滤波处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int v1 = 0, v2 = 0, v3 = 0, v4 = 0, v5 = 0, v6 = 0, v7 = 0, v8 = 0, t = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				v1 = (int)(temp[i - 4 + (j - 1) * w * 4] * 0.114 + temp[i - 4 + 1 + (j - 1) * w * 4] * 0.587 + temp[i - 4 + 2 + (j - 1) * w * 4] * 0.299);
				v2 = (int)(temp[i + (j - 1) * w * 4] * 0.114 + temp[i + 1 + (j - 1) * w * 4] * 0.587 + temp[i + 2 + (j - 1) * w * 4] * 0.299);
				v3 = (int)(temp[i + 4 + (j - 1) * w * 4] * 0.114 + temp[i + 4 + 1 + (j - 1) * w * 4] * 0.587 + temp[i + 4 + 2 + (j - 1) * w * 4] * 0.299);
				v4 = (int)(temp[i - 4 + j * w * 4] * 0.114 + temp[i - 4 + 1 + j * w * 4] * 0.587 + temp[i - 4 + 2 + j * w * 4] * 0.299);
				v5 = (int)(temp[i + 4 + j * w * 4] * 0.114 + temp[i + 4 + 1 + j * w * 4] * 0.587 + temp[i + 4 + 2 + j * w * 4] * 0.299);
				v6 = (int)(temp[i - 4 + (j + 1) * w * 4] * 0.114 + temp[i - 4 + 1 + (j + 1) *w * 4] * 0.587 + temp[i - 4 + 2 + (j + 1) * w * 4] * 0.299);
				v7 = (int)(temp[i + (j + 1) * w * 4] * 0.114 + temp[i + 1 + (j + 1) * w * 4] * 0.587 + temp[i + 2 + (j + 1) * w * 4] * 0.299);
				v8 = (int)(temp[i + 4 + (j + 1) * w * 4] * 0.114 + temp[i + 4 + 1 + (j + 1) * w * 4] * 0.587 + temp[i + 4 + 2 + (j + 1) * w * 4] * 0.299);
				t = GetMedianValue(v1, v2, v3, v4, v5, v6, v7, v8);
				if (t == v1)
				{
					temp[i + j * w * 4] = (byte)tempMask[i - 4 + (j - 1) * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i - 4 + 1 + (j - 1) * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i - 4 + 2 + (j - 1) * w * 4];
				}

				elseif(t == v2)
				{
					temp[i + j * w * 4] = (byte)tempMask[i + (j - 1) * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i + 1 + (j - 1) * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i + 2 + (j - 1) * w * 4];
				}
				elseif(t == v3)
				{
					temp[i + j * w * 4] = (byte)tempMask[i + 4 + (j - 1) * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i + 1 + 4 + (j - 1) * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i + 2 + 4 + (j - 1) * w * 4];
				}
				elseif(t == v4)
				{
					temp[i + j * w * 4] = (byte)tempMask[i - 4 + j * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i - 4 + 1 + j * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i - 4 + 2 + j * w * 4];
				}
				elseif(t == v5)
				{
					temp[i + j * w * 4] = (byte)tempMask[i + 4 + j * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i + 4 + 1 + j * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i + 4 + 2 + j * w * 4];
				}
				elseif(t == v6)
				{
					temp[i + j * w * 4] = (byte)tempMask[i - 4 + (j + 1) * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i - 4 + 1 + (j + 1) * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i - 4 + 2 + (j + 1) * w * 4];
				}
				elseif(t == v7)
				{
					temp[i + j * w * 4] = (byte)tempMask[i + (j + 1) * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i + 1 + (j + 1) * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i + 2 + (j + 1) * w * 4];
				}
				else
				{
					temp[i + j * w * 4] = (byte)tempMask[i + 4 + (j + 1) * w * 4];
					temp[i + 1 + j * w * 4] = (byte)tempMask[i + 4 + 1 + (j + 1) * w * 4];
					temp[i + 2 + j * w * 4] = (byte)tempMask[i + 4 + 2 + (j + 1) * w * 4];
				}
				v1 = 0; v2 = 0; v3 = 0; v4 = 0; v5 = 0; v6 = 0; v7 = 0; v8 = 0; t = 0;
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		returnnull;
	}
}
privatestaticint GetMedianValue(paramsint[] src)
{
	int w = src.Length;
	int temp = src[0], m = 0;
	for (int i = 1; i < (int)(w / 2); i++)
	{
		if (src[i] < temp)
		{
			m = src[i];
			src[i] = temp;
			temp = m;
		}
		else
			continue;
	}
	return (int)((src[(int)(w / 2)] + src[(int)(-1 + w / 2)]) / 2);
}

```

**![img](https://img-blog.csdn.net/20150410104914388?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## C#数字图像处理--2.11图像锐化


**[函数名称]**

图像锐化函数*SharpeningProcess(**WriteableBitmap* *src,**double* *sharpenValue)*

![img](https://img-blog.csdn.net/20150407110414426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







**[函数代码]**

```csharp
///<summary>
/// Sharpening process.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static WriteableBitmap SharpeningProcess(WriteableBitmap src)////11锐化处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap sharpenImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double b = 0, g = 0, r = 0, srR = 0, srG = 0, srB = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{

				srB = tempMask[i + j * w * 4];
				srG = tempMask[i + 1 + j * w * 4];
				srR = tempMask[i + 2 + j * w * 4];
				b = Math.Abs(5 * srB - tempMask[i - 4 + j * w * 4] - tempMask[i + 4 + j * w * 4] - tempMask[i + (j - 1) * w * 4] - tempMask[i + (j + 1) * w * 4]);
				g = Math.Abs(5 * srG - tempMask[i - 4 + 1 + j * w * 4] - tempMask[i + 4 + 1 + j * w * 4] - tempMask[i + 1 + (j - 1) * w * 4] - tempMask[i + 1 + (j + 1) * w * 4]);
				r = Math.Abs(5 * srR - tempMask[i - 4 + 2 + j * w * 4] - tempMask[i + 4 + 2 + j * w * 4] - tempMask[i + 2 + (j - 1) * w * 4] - tempMask[i + 2 + (j + 1) * w * 4]);
				temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
				temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
				temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				b = 0; g = 0; r = 0; srR = 0; srG = 0; srB = 0;
			}
		}
		Stream sTemp = sharpenImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return sharpenImage;
	}
	else
	{
		returnnull;
	}
}

```

 **[图像效果]**

**![img](https://img-blog.csdn.net/20150410105030813?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## C#数字图像处理--2.12Sobel边缘检测

﻿﻿

**[函数名称]**

图像*Sobel*边缘检测函数*SobelEdgeProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150407110612421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**[函数代码]**

```csharp
///<summary>
/// Sobel edge detection.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static WriteableBitmap SobelEdgeProcess(WriteableBitmap src)////12 Sobel边缘检测
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap sobelImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int b = 0, g = 0, r = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				if (i == 0 || i == w - 4 || j == 0 || j == h - 1)
				{
					temp[i + j * w * 4] = (byte)0;
					temp[i + 1 + j * w * 4] = (byte)0;
					temp[i + 2 + j * w * 4] = (byte)0;
				}
				else
				{
					b = Math.Abs(tempMask[i - 4 + (j - 1) * w * 4] + 2 * tempMask[i - 4 + j * w * 4] + tempMask[i - 4 + (j + 1) * w * 4] - tempMask[i + 4 + (j - 1) * w * 4]
						- 2 * tempMask[i + 4 + j * w * 4] - tempMask[i + 4 + (j + 1) * w * 4]) + Math.Abs(tempMask[i - 4 + (j - 1) * w * 4] + 2 * tempMask[i + (j - 1) * w * 4]
							+ tempMask[i + 4 + (j - 1) * w * 4] - tempMask[i - 4 + (j + 1) * w * 4] - 2 * tempMask[i + (j + 1) * w * 4] - tempMask[i + 4 + (j + 1) * w * 4]);
					g = Math.Abs(tempMask[i - 4 + 1 + (j - 1) * w * 4] + 2 * tempMask[i - 4 + 1 + j * w * 4] + tempMask[i - 4 + 1 + (j + 1) * w * 4] - tempMask[i + 4 + 1 + (j - 1) * w * 4]
						- 2 * tempMask[i + 4 + 1 + j * w * 4] - tempMask[i + 4 + 1 + (j + 1) * w * 4]) + Math.Abs(tempMask[i - 4 + 1 + (j - 1) * w * 4] + 2 * tempMask[i + 1 + (j - 1) * w * 4]
							+ tempMask[i + 4 + 1 + (j - 1) * w * 4] - tempMask[i - 4 + 1 + (j + 1) * w * 4] - 2 * tempMask[i + 1 + (j + 1) * w * 4] - tempMask[i + 1 + 4 + (j + 1) * w * 4]);
					r = Math.Abs(tempMask[i - 4 + 2 + (j - 1) * w * 4] + 2 * tempMask[i - 4 + 2 + j * w * 4] + tempMask[i - 4 + 2 + (j + 1) *w * 4] - tempMask[i + 4 + 2 + (j - 1) * w * 4]
						- 2 * tempMask[i + 4 + 2 + j * w * 4] - tempMask[i + 4 + 2 + (j + 1) * w * 4]) + Math.Abs(tempMask[i - 4 + 2 + (j - 1) * w * 4] + 2 * tempMask[i + 2 + (j - 1) * w * 4]
							+ tempMask[i + 4 + 2 + (j - 1) * w * 4] - tempMask[i - 4 + 2 + (j + 1) * w * 4] - 2 * tempMask[i + 2 + (j + 1) * w * 4] - tempMask[i + 2 + 4 + (j + 1) * w * 4]);
					temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
					temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
					temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				}
				b = 0; g = 0; r = 0;
			}
		}
		Stream sTemp = sobelImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return sobelImage;
	}
	else
	{
		returnnull;
	}
}

```

**![img](https://img-blog.csdn.net/20150410105218297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



## C#数字图像处理--2.13Roberts边缘检测


**[函数名称]**

图像*Roberts*边缘检测函数*RobertEdgeProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150407110707193?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
///<summary>
/// Roberts edge detection.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static WriteableBitmap RobertEdgeProcess(WriteableBitmap src)////13 Robert边缘检测
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap robertImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int b = 0, g = 0, r = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				if (i == 0 || i == w - 4 || j == 0 || j == h - 1)
				{
					temp[i + j * w * 4] = (byte)0;
					temp[i + 1 + j * w * 4] = (byte)0;
					temp[i + 2 + j * w * 4] = (byte)0;
				}
				else
				{
					b = Math.Abs(tempMask[i + j * w * 4] - tempMask[i - 4 + (j + 1) * w * 4]) + Math.Abs(tempMask[i - 4 + j * w * 4] - tempMask[i + (j + 1) * w * 4]);
					g = Math.Abs(tempMask[i + 1 + j * w * 4] - tempMask[i - 4 + 1 + (j + 1) * w * 4]) + Math.Abs(tempMask[i - 4 + 1 + j * w * 4] - tempMask[i + 1 + (j + 1) * w * 4]);
					r = Math.Abs(tempMask[i + 2 + j * w * 4] - tempMask[i - 4 + 2 + (j + 1) * w * 4]) + Math.Abs(tempMask[i - 4 + 2 + j * w * 4] - tempMask[i + 2 + (j + 1) * w * 4]);
					temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
					temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
					temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				}
				b = 0; g = 0; r = 0;
			}
		}
		Stream sTemp = robertImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return robertImage;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410105456141?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## C#数字图像处理--2.14Prewitt 边缘检测

﻿﻿

**[函数名称]**

图像*Prewitt*边缘检测函数*PrewittEdgeProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150407110801169?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**[函数代码]**

```
       ///<summary>
       /// Smooth edge detection.
       ///</summary>
       ///<param name="src">Source image.</param>
       ///<returns></returns>
       public static WriteableBitmap PrewittEdgeProcess(WriteableBitmap src)////14 Prewitt边缘检测
       {
           if(src!=null )
           {
           int w = src.PixelWidth;
           int h = src.PixelHeight;
           WriteableBitmap smoothImage =newWriteableBitmap(w,h);
           byte[] temp = src.PixelBuffer.ToArray();
           byte[] tempMask = (byte[])temp.Clone();
           int b = 0, g = 0, r = 0;
           for (int j = 1; j < h - 1; j++)
           {
               for (int i = 4; i < w * 4 - 4; i += 4)
               {
                   if (i == 0 || i == w - 4 || j == 0 || j == h - 1)
                   {
                       temp[i + j * w * 4] = (byte)0;
                       temp[i + 1 + j * w * 4] = (byte)0;
                       temp[i + 2 + j * w * 4] = (byte)0;
                   }
                   else
                   {
                       b =Math.Abs(tempMask[i - 4 + (j - 1) * w * 4] + tempMask[i + (j - 1) * w * 4] + tempMask[i + 4 + (j - 1) * w * 4]
                           - tempMask[i - 4 + (j + 1) * w * 4] - tempMask[i + (j + 1) * w * 4] - tempMask[i + 4 + (j + 1) * w * 4]) +
                           Math.Abs(tempMask[i + 4 + (j - 1) * w * 4] + tempMask[i + 4 + j * w * 4] + tempMask[i + 4 + (j + 1) * w * 4]
                           - tempMask[i - 4 + (j - 1) * w * 4] - tempMask[i - 4 + j * w * 4] - tempMask[i - 4 + (j + 1) * w * 4]);
                       g =Math.Abs(tempMask[i - 4 + 1 + (j - 1) * w * 4] + tempMask[i + 1 + (j - 1) * w * 4] + tempMask[i + 1 + 4 + (j - 1) * w * 4]
                           - tempMask[i - 4 + 1 + (j + 1) * w * 4] - tempMask[i + 1 + (j + 1) * w * 4] - tempMask[i + 1 + 4 + (j + 1) * w * 4]) +
                           Math.Abs(tempMask[i + 1 + 4 + (j - 1) * w * 4] + tempMask[i + 1 + 4 + j * w * 4] + tempMask[i + 1 + 4 + (j + 1) * w * 4]
                           - tempMask[i - 4 + 1 + (j - 1) * w * 4] - tempMask[i - 4 + 1 + j * w * 4] - tempMask[i - 4 + 1 + (j + 1) * w * 4]);
                       r =Math.Abs(tempMask[i - 4 + 2 + (j - 1) * w * 4] + tempMask[i + 2 + (j - 1) * w * 4] + tempMask[i + 4 + 2 + (j - 1) * w * 4]
                           - tempMask[i - 4 + 2 + (j + 1) * w * 4] - tempMask[i + 2 + (j + 1) * w * 4] - tempMask[i + 4 + 2 + (j + 1) * w * 4]) +
                           Math.Abs(tempMask[i + 4 + 2 + (j - 1) * w * 4] + tempMask[i + 4 + 2 + j * w * 4] + tempMask[i + 4 + 2 + (j + 1) * w * 4]
                           - tempMask[i - 4 + 2 + (j - 1) * w * 4] - tempMask[i - 4 + 2 + j * w * 4] - tempMask[i - 4 + 2 + (j + 1) * w * 4]);
                       temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
                       temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
                       temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
                   }
                   b = 0; g = 0; r = 0;
               }
           }
           Stream sTemp = smoothImage.PixelBuffer.AsStream();
           sTemp.Seek(0,SeekOrigin.Begin);
           sTemp.Write(temp, 0, w * 4 * h);
           return smoothImage;
           }
           else
           {
               returnnull;
           }
       }

```

**![img](https://img-blog.csdn.net/20150410105842260?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

## C#数字图像处理--2.15图像霓虹效果


**[函数名称]**

图像霓虹效果函数*NeonProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150407110825680?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

上述公式进行开方即可。



**[函数代码]**

```csharp
///<summary>
/// Neon process.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static WriteableBitmap NeonProcess(WriteableBitmap src)////15霓虹处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap neonImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int b = 0, g = 0, r = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				if (i == 0 || i == w - 4 || j == 0 || j == h - 1)
				{
					temp[i + j * w * 4] = (byte)0;
					temp[i + 1 + j * w * 4] = (byte)0;
					temp[i + 2 + j * w * 4] = (byte)0;
				}
				else
				{
					b = (int)Math.Sqrt((tempMask[i + j * w * 4] - tempMask[i + 4 + j * w * 4]) * (tempMask[i + j * w * 4] - tempMask[i + 4 + j * w * 4])
						+ (tempMask[i + j * w * 4] - tempMask[i + (j + 1) * w * 4]) * (tempMask[i + j * w * 4] - tempMask[i + (j + 1) * w * 4]));
					g = (int)Math.Sqrt((tempMask[i + 1 + j * w * 4] - tempMask[i + 4 + 1 + j * w * 4]) * (tempMask[i + 1 + j * w * 4] - tempMask[i + 4 + 1 + j * w * 4])
						+ (tempMask[i + 1 + j * w * 4] - tempMask[i + 1 + (j + 1) * w * 4]) * (tempMask[i + 1 + j * w * 4] - tempMask[i + 1 + (j + 1) * w * 4]));
					r = (int)Math.Sqrt((tempMask[i + 2 + j * w * 4] - tempMask[i + 4 + 2 + j * w * 4]) * (tempMask[i + 2 + j *w * 4] - tempMask[i + 4 + 2 + j * w * 4])
						+ (tempMask[i + 2 + j * w * 4] - tempMask[i + 2 + (j + 1) * w * 4]) * (tempMask[i + 2 + j * w * 4] - tempMask[i + 2 + (j + 1) * w * 4]));
					temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
					temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
					temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				}
				b = 0; g = 0; r = 0;
			}
		}
		Stream sTemp = neonImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return neonImage;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410110040118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.16图像浮雕效果


**[函数名称]**

图像浮雕效果函数*ReliefProcess(**WriteableBitmap* *src)*

![img](https://img-blog.csdn.net/20150408093320925?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
///<summary>
/// Relief process.
///</summary>
///<param name="src">The source image.</param>
///<param name="reliefValue">A value to adjust the relief processing, from 0 to 255.</param>
///<returns></returns>
public static WriteableBitmap ReliefProcess(WriteableBitmap src, int reliefValue)////16浮雕处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap reliefImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int b = 0, g = 0, r = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				b = Math.Abs(tempMask[i + j * w * 4] - tempMask[i + 4 + j * w * 4] + reliefValue);
				g = Math.Abs(tempMask[i + 1 + j * w * 4] - tempMask[i + 1 + 4 + j * w * 4] + reliefValue);
				r = Math.Abs(tempMask[i + 2 + j * w * 4] - tempMask[i + 2 + 4 + j * w * 4] + reliefValue);
				temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
				temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
				temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				b = 0; g = 0; r = 0;
			}
		}
		Stream sTemp = reliefImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return reliefImage;
	}
	else
	{
		returnnull;
	}
}

```

 **[图像效果]**

![img](https://img-blog.csdn.net/20150410110127920?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.17图像木刻效果


**[函数名称]**

图像木刻效果函数*WoodCutProcess(**WriteableBitmap* *src)*

*![img](https://img-blog.csdn.net/20150408093524817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*



**[函数代码]**

```
       ///<summary>
       /// Wood cut process.
       ///</summary>
       ///<param name="src">Source image.</param>
       ///<returns></returns>
       public static WriteableBitmap WoodCutProcess(WriteableBitmap src)////17木刻处理
       {
           if(src!=null )
           {
           int w = src.PixelWidth;
           int h = src.PixelHeight;
           WriteableBitmap woodcutImage =newWriteableBitmap(w,h);
           byte[] temp = src.PixelBuffer.ToArray();
           int tempRGB = 0;
           for (int i = 0; i < temp.Length; i += 4)
           {
               tempRGB = (int)((temp[i] + temp[i + 1] + temp[i + 2]) / 3);
               temp[i] = (byte)(tempRGB > 122.5 ? 0 : 255);
               temp[i + 1] = (byte)(tempRGB > 122.5 ? 0 : 255);
               temp[i + 2] = (byte)(tempRGB > 122.5 ? 0 : 255);
               tempRGB = 0;
           }
           Stream sTemp = woodcutImage.PixelBuffer.AsStream();
           sTemp.Seek(0, SeekOrigin.Begin);
           sTemp.Write(temp, 0, w * 4 * h);
           return woodcutImage;
           }
           else
           {
               returnnull;
           }
       }
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410110324059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.18图像平移变换

﻿﻿

**[函数名称]**

图像平移变换函数*TranslationProcess(**WriteableBitmap* *src,**int* *x,**int* *y)*

*![img](https://img-blog.csdn.net/20150408093623429?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*

**[函数代码]**

```csharp
///<summary>
/// Translation process.
///</summary>
///<param name="src">Source image.</param>
///<param name="x">Translate value of x.</param>
///<param name="y">Translate value of y.</param>
///<returns></returns>
public static WriteableBitmap TranslationProcess(WriteableBitmap src, int x, int y)////18平移变换
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap translateImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = newbyte[w * h * 4];
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				if (i + x < 0 || i + x >= w || j + y < 0 || j + y >= h)
				{
					tempMask[i * 4 + j * w * 4] = (byte)0;
					tempMask[i * 4 + 1 + j * w * 4] = (byte)0;
					tempMask[i * 4 + 2 + j * w * 4] = (byte)0;
				}
				else
				{
					tempMask[i * 4 + j * w * 4] = (byte)(temp[(i + x) * 4 + (j + y) * w * 4]);
					tempMask[i * 4 + 1 + j * w * 4] = (byte)(temp[(i + x) * 4 + 1 + (j + y) * w * 4]);
					tempMask[i * 4 + 2 + j * w * 4] = (byte)(temp[(i + x) * 4 + 2 + (j + y) * w * 4]);
					tempMask[i * 4 + 3 + j * w * 4] = (byte)(temp[(i + x) * 4 + 3 + (j + y) * w * 4]);
				}
			}
		}
		Stream sTemp = translateImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(tempMask, 0, w * 4 * h);
		return translateImage;
	}
	else
	{
		returnnull;
	}
}
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410110445132?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.19图像水平镜像

**[函数名称]**

图像水平镜像函数*MirrorXProcess(**WriteableBitmap* *src)*

*![img](https://img-blog.csdn.net/20150408093829958?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*



**[函数代码]**

```csharp
       ///<summary>
       /// Horizontal mirror process.
       ///</summary>
       ///<param name="src">Source image.</param>
       ///<returns></returns>
       public static WriteableBitmap MirrorXProcess(WriteableBitmap src)////19水平镜像
       {
           if(src!=null )
           {
           int w = src.PixelWidth;
           int h = src.PixelHeight;
           WriteableBitmap mirrorImage =newWriteableBitmap(w,h);
           byte[] temp = src.PixelBuffer.ToArray();
           byte[] tempMask =newbyte[w * h * 4];
           for (int j = 0; j < h; j++)
           {
               for (int i = 0; i < w; i++)
               {
                   tempMask[i * 4 + j * w * 4] = temp[(w - 1 - i) * 4 + j * w * 4];
                   tempMask[i * 4 + 1 + j * w * 4] = temp[(w - 1 - i) * 4 + 1 + j * w * 4];
                   tempMask[i * 4 + 2 + j * w * 4] = temp[(w - 1 - i) * 4 + 2 + j * w * 4];
                   tempMask[i * 4 + 3 + j * w * 4] = temp[(w - 1 - i) * 4 + 3 + j * w * 4];
               }
           }
           Stream sTemp = mirrorImage.PixelBuffer.AsStream();
           sTemp.Seek(0, SeekOrigin.Begin);
           sTemp.Write(tempMask, 0, w * 4 * h);
           return mirrorImage;
           }
           else
           {
               returnnull;
           }
       }

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410110834016?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## C#数字图像处理--2.20图像垂直镜像


**[函数名称]**

图像垂直镜像函数*MirrorYProcess(**WriteableBitmap* *src)*

*![img](https://img-blog.csdn.net/20150408094014884?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)*



**[函数代码]**

```csharp
///<summary>
/// Vertical mirror process.
///</summary>
///<param name="src">Source image.</param>
///<returns></returns>
public static WriteableBitmap MirrorYProcess(WriteableBitmap src)////20垂直镜像
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap mirrorImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = newbyte[w * h * 4];
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				tempMask[i * 4 + j * w * 4] = temp[i * 4 + (h - 1 - j) * w * 4];
				tempMask[i * 4 + 1 + j * w * 4] = temp[i * 4 + 1 + (h - 1 - j) * w * 4];
				tempMask[i * 4 + 2 + j * w * 4] = temp[i * 4 + 2 + (h - 1 - j) * w * 4];
				tempMask[i * 4 + 3 + j * w * 4] = temp[i * 4 + 3 + (h - 1 - j) * w * 4];
			}
		}
		Stream sTemp = mirrorImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(tempMask, 0, w * 4 * h);
		return mirrorImage;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410110847077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## C#数字图像处理--2.21二值图像腐蚀


**[函数名称]**

二值图像腐蚀函数*CorrosionProcess(WriteableBitmap src)*



**[算法说明]**

 二值图像腐蚀操作属于图像形态学的范畴，形态学运算是只针对二值图像进行，并依据数学形态学*(Mathermatical Morphogy)*集合论方法发展起来的数字图像处理方法，它主要包括腐蚀，膨胀，开，闭，击中，击不中等。

 图像形态学运算，要使用结构元素，所谓结构元素是指具有某种确定形状的基本结构，它的选择一般要求其具有旋转不变性或者镜像不变性，即：结构元素的原点在其几何中心处，周围像素关于原点对称。

 在这里我们选取如下的结构元素：



![img](https://img-blog.csdn.net/20150410110950069?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 其中，*F*为二值图像原图，*X*为结构元素原点所在的二值图像中的连通域。

 假设*F*中目标像素为*255(*白色*)*，非目标为*0(*黑色*)*，当结构元素*S*原点移动到点*(x,y)*时，如果*S*中所有点均包含在*X*中*(X*中对应在*S*中所有点的位置均为*255)*，则在腐蚀后的二值图像中，对应于*S*原点的位置为*255(*白色*)*，否则为*0(*黑色*)*。

 用通俗的话来说就是：用结构元素作为模板在原始二值图像种平滑一遍，扫描图像的每一个像素，用结构元素中的每一个元素与其覆盖的二值图像做“与”操作*(*假设结构元素都为*1)*，如果结果都为*1*，则二值图像中对应结构元素原点位置的像素值为*1*，否则为*0*。



**[函数代码]**

```csharp
       ///<summary>
       /// Corrosion process.
       ///</summary>
       ///<param name="src">The source image(It should be the binary image).</param>
       ///<returns></returns>
       public static WriteableBitmap CorrosionProcess(WriteableBitmap src)////21图像腐蚀运算
       {
           if (src !=null)
           {
               int w = src.PixelWidth;
               int h = src.PixelHeight;
               WriteableBitmap corrosionImage =newWriteableBitmap(w, h);
               byte[] temp = src.PixelBuffer.ToArray();
               byte[] tempMask = (byte[])temp.Clone();
               for (int j = 0; j < h; j++)
               {
                   for (int i = 0; i < w ; i ++)
                   {
                       if (i == 0 || i == w - 1 || j == 0 || j == h - 1)
                       {
                           temp[i * 4 + j * w * 4] = (byte)255;
                           temp[i * 4 + 1 + j * w * 4] = (byte)255;
                           temp[i * 4 + 2 + j * w * 4] = (byte)255;
                       }
                       else
                       {
                           if (tempMask[i * 4 - 4 + j * w * 4] == 255 && tempMask[i * 4 + j * w * 4] == 255 && tempMask[i * 4 + 4 + j * w * 4] == 255
                               && tempMask[i * 4 + (j - 1) * w * 4] == 255 && tempMask[i * 4 + (j + 1) * w * 4] == 255)
                           {
                               temp[i * 4 + j * w * 4] = (byte)255;
                               temp[i * 4 + 1 + j * w * 4] = (byte)255;
                               temp[i * 4 + 2 + j * w * 4] = (byte)255;
                           }
                           else
                           {
                               temp[i * 4 + j * w * 4] = 0;
                               temp[i * 4 + 1 + j * w * 4] = 0;
                               temp[i * 4 + 2 + j * w * 4] = 0;
                           }
                       }
                   }
               }
               Stream sTemp = corrosionImage.PixelBuffer.AsStream();
               sTemp.Seek(0, SeekOrigin.Begin);
               sTemp.Write(temp, 0, w * 4 * h);
               return corrosionImage;
           }
           else
           {
               returnnull;
           }
       }

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410111112434?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.22二值图像膨胀

﻿﻿

**[函数名称]**

二值图像膨胀函数*DilationProcess**(WriteableBitmap src)*







**[算法说明]**

 膨胀算法也是属于形态学算法的范畴，前一节已经简单介绍了形态学，这里不再累赘。

 我们这里介绍的膨胀算法依旧采用上一节腐蚀中的结构元素*S*，则算法过程如下：



![img](https://img-blog.csdn.net/20150410111144589?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

 用通俗的话讲就是，用结构元素作为模板在原始二值图像种平滑一遍，扫描图像的每一个像素，用结构元素中的每一个元素与其覆盖的二值图像做“或”操作*(*假设结构元素都为*1)*，如果结果为*1*，则二值图像中对应结构元素原点位置的像素值为*1*，否则为*0*。



**[函数代码]**

```csharp
       ///<summary>
       /// Dilation process.
       ///</summary>
       ///<param name="src">The source image(It should be the binary image).</param>
       ///<returns></returns>
       public static WriteableBitmap DilationProcess(WriteableBitmap src)////22图像膨胀运算
       {
           if (src !=null)
           {
               int w = src.PixelWidth;
               int h = src.PixelHeight;
               WriteableBitmap dilationImage =newWriteableBitmap(w, h);
               byte[] temp = src.PixelBuffer.ToArray();
               byte[] tempMask = (byte[])temp.Clone();
               for (int j = 0; j < h; j++)
               {
                   for (int i = 0; i < w; i++)
                   {
                       if (i == 0 || i == w - 1 || j == 0 || j == h - 1)
                       {
                           temp[i * 4 + j * w * 4] = (byte)255;
                           temp[i * 4 + 1 + j * w * 4] = (byte)255;
                           temp[i * 4 + 2 + j * w * 4] = (byte)255;
                       }
                       else
                       {
                           if (tempMask[i * 4 - 4 + j * w * 4] == 255 || tempMask[i * 4 + j * w * 4] == 255 || tempMask[i * 4 + 4 + j * w * 4] == 255
                               || tempMask[i * 4 + (j - 1) * w * 4] == 255 || tempMask[i * 4 + (j + 1) * w * 4] == 255)
                           {
                               temp[i * 4 + j * w * 4] = (byte)255;
                               temp[i * 4 + 1 + j * w * 4] = (byte)255;
                               temp[i * 4 + 2 + j * w * 4] = (byte)255;
                           }
                           else
                           {
                               temp[i * 4 + j * w * 4] = 0;
                               temp[i * 4 + 1 + j * w * 4] = 0;
                               temp[i * 4 + 2 + j * w * 4] = 0;
                           }
                       }
                   }
               }
               Stream sTemp = dilationImage.PixelBuffer.AsStream();
               sTemp.Seek(0, SeekOrigin.Begin);
               sTemp.Write(temp, 0, w * 4 * h);
               return dilationImage;
           }
           else
           {
               returnnull;
           }
       }

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410111305441?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.23二值图像开运算


**[函数名称]**

二值图像开运算函数*OpenOperateProcess(WriteableBitmap src)*



**[算法说明]**

 开运算就是先进性一次腐蚀后进行一次膨胀。算法过程如公式*2-(27)*。

![img](https://img-blog.csdn.net/20150410111343571?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
///<summary>
/// Open operate process.
///</summary>
///<param name="src">The source image(It should be the binary image).</param>
///<returns></returns>
public static WriteableBitmap OpenOperateProcess(WriteableBitmap src)////23图像开运算
{
	if (src != null)
	{
		WriteableBitmap temp = DilationProcess(CorrosionProcess(src));
		return temp;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410111441475?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.24二值图像闭运算

﻿﻿

**[函数名称]**

二值图像闭运算函数*CloseOperateProcess(WriteableBitmap src)*



**[算法说明]**

 闭运算就是先进性一次膨胀后进行一次腐蚀。算法过程如公式*2-(28)*。

![img](https://img-blog.csdn.net/20150410111520540?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
///<summary>
/// Close operate process.
///</summary>
///<param name="src">The source image(It should be the binary image).</param>
///<returns></returns>
public static WriteableBitmap CloseOperateProcess(WriteableBitmap src)////24图像闭运算
{
	if (src != null)
	{
		WriteableBitmap temp = CorrosionProcess(DilationProcess(src));
		return temp;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410111627352?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.25二值图像距离变换

﻿﻿

**[函数名称]**

二值图像距离变换函数*DistanceTransformProcess(**WriteableBitmap* *src)*



**[算法说明]**

 二值图像的距离变换实际上就是将二值图像转换为灰度图像，在二值图像中我们将图像分为目标图像和背景图像，假设目标图像像素值为*1*，即为白色，背景像素为*0*即为黑色。在转换后的幅灰度图像中，每个连通域的各个像素点的灰度级与该像素点到其背景像素的最近距离有关。其中灰度级最大点的集合为目标图像的骨架，就是目标图像中心部分的像素的集合，灰度级反应了背景像素与目标图像边界的影响关系。用数学语言表示如下：

 假设二值图像*I*包含一个连通域*S*，其中有目标*O*和背景*B*，距离图为*D*，则距离变换定义如下：





  ![img](https://img-blog.csdn.net/20150410111742625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



距离变换的具体步骤为：

​     *1*，将图像中的目标像素点分类，分为内部点，外部点和孤立点。

以中心像素的四邻域为例，如果中心像素为目标像素*(*值为*1)*且四邻域都为目标像素*(*值为*1)*，则该点为内部点。如果该中心像素为目标像素，四邻域为背景像素*(*值为*0)*，则该中心点为孤立点，如下图所示。除了内部点和孤立点之外的目标区域点为边界点。

![img](https://img-blog.csdn.net/20150410112155967?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​       *6*，对于孤立点保持不变。

 以上的距离变换方法由于计算量大，比较耗时，因此在实际应用中，我们采用一种倒角模版算法，只需要对图像进行两次扫描就可以实现距离变换。该方法称为*Chamfer*倒角距离变换法。

 该方法使用两个模版，分别为前向模版和后向模板，如下图所示:



![img](https://img-blog.csdn.net/20150410112127765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



 计算步骤如下*:*

 *1*，使用前向模板，对图像从上到下，从左到右进行扫描，模板中心*0*点对应的像素值如果为*0*则跳过，如果为*1*则计算模板中每个元素与其对应的像素值的和，分别为*Sum1,Sum2,Sum3,Sum4*，*Sum5*，而中心像素值为这五个和值中的最小值。

 *2*，使用后向模板，对图像从下到上，从右到左进行扫描，方法同上。

 *3*，一般我们使用的模板为*3\*3*和*5\*5*，分别如下图所示：





​                         ![img](https://img-blog.csdn.net/20150410112200416?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
///<summary>
/// Distance transform of binary image.
///</summary>
///<param name="src">The source image.</param>
///<returns></returns>
public static WriteableBitmap DistanceTransformProcess(WriteableBitmap src)////25二值图像距离变换
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap expansionImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int t1, t2, t3, t4, t5, min = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w * 4 - 4; x += 4)
			{
				if (y == 0 || x == 0)
				{
					temp[x + y * w * 4] = 0;
					temp[x + 1 + y * w * 4] = 0;
					temp[x + 2 + y * w * 4] = 0;
				}
				else
				{
					if (temp[x + y * w * 4] != 0)
					{
						t1 = temp[x - 3 + (y - 1) * w * 4] + 4;
						t2 = temp[x + (y - 1) * w * 4] + 3;
						t3 = temp[x + 3 + (y - 1) * w * 4] + 4;
						t4 = temp[x - 3 + y * w * 4] + 3;
						t5 = temp[x + y * w * 4];
						min = GetMin(t1, t2, t3, t4, t5);
						temp[x + y * w * 4] = (byte)min;
						temp[x + 1 + y * w * 4] = (byte)min; temp[x + 2 + y * w * 4] = (byte)min;
					}
					t2 = 0; t3 = 0; t4 = 0; t5 = 0; min = 0;
				}
			}
		}
		for (int y = h - 2; y > 0; y--)
		{
			for (int x = w * 4 - 4; x > 0; x -= 4)
			{
				if (y == 1 || x == 3)
				{
					temp[x + y * w * 4] = 0;
					temp[x + 1 + y * w * 4] = 0;
					temp[x + 2 + y * w * 4] = 0;
				}
				else
				{
					if (temp[x + y * w * 4] != 0)
					{
						t1 = temp[x - 3 + (y + 1) * w * 4] + 4;
						t2 = temp[x + (y + 1) * w * 4] + 3;
						t3 = temp[x + 3 + (y + 1) * w * 4] + 4;
						t4 = temp[x + 3 + y * w * 4] + 3;
						t5 = temp[x + y * w * 4];
						min = GetMin(t1, t2, t3, t4, t5);
						temp[x + y * w * 4] = (byte)min;
						temp[x + 1 + y * w * 4] = (byte)min; temp[x + 2 + y * w * 4] = (byte)min;
					}
					t2 = 0; t3 = 0; t4 = 0; t5 = 0; min = 0;
				}
			}
		}
		Stream sTemp = expansionImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return expansionImage;
	}
	else
	{
		returnnull;
	}
}
privatestaticint GetMin(int a, int b, int c, int d, int e)
{
	int t = (a < b ? a : b) < c ? (a < b ? a : b) : c;
	return ((t < d ? t : d) < e ? (t < d ? t : d) : e);
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150410112321782?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## C#数字图像处理--2.26图像减法


**[函数名称]**

图像减法函数*SubtractionProcess(**WriteableBitmap* *src,* *WriteableBitmap* *subSrc)*



**[算法说明]**

 图像减法就是将两幅图像对应像素值进行减法运算，这里要求两幅图像大小最好相同。

 算法过程如下：



![img](https://img-blog.csdn.net/20150410112354951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
///<summary>
/// Subtraction of two images(Both of them should be in same size).
///</summary>
///<param name="src">The first source image.</param>
///<param name="subSrc">The second source image.</param>
///<returns></returns>
public static WriteableBitmap SubtractionProcess(WriteableBitmap src, WriteableBitmap subSrc)////26图像减法
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap subImage = newWriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] subTemp = subSrc.PixelBuffer.ToArray();
		byte[] dst = newbyte[w * h * 4];
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				dst[i * 4 + j * w * 4] = (byte)Math.Abs(temp[i * 4 + (h - 1 - j) * w * 4] - subTemp[i * 4 + (h - 1 - j) * w * 4]);
				dst[i * 4 + 1 + j * w * 4] = (byte)Math.Abs(temp[i * 4 + 1 + (h - 1 - j) * w * 4] - subTemp[i * 4 + 1 + (h - 1 - j) * w * 4]);
				dst[i * 4 + 2 + j * w * 4] = (byte)Math.Abs(temp[i * 4 + 2 + (h - 1 - j) * w * 4] - subTemp[i * 4 + 2 + (h - 1 - j) * w * 4]);
				dst[i * 4 + 3 + j * w * 4] = 0;
			}
		}
		Stream sTemp = subImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(dst, 0, w * 4 * h);
		return subImage;
	}
	else
	{
		returnnull;
	}
}

```

**[图像效果]**





![img](https://img-blog.csdn.net/20150410112410926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.27图像加法运算

﻿﻿

**[函数名称]**

图像加法函数AddProcess(WriteableBitmap src, WriteableBitmap addSrc)

![img](https://img-blog.csdn.net/20150410125201598?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
        /// <summary>
        /// Add procession of two images(Both of them should be in same size).
        /// </summary>
        /// <param name="src">The first source image.</param>
        /// <param name="addSrc">The second source iamge.</param>
        /// <returns></returns>
        public static WriteableBitmap AddProcess(WriteableBitmap src,WriteableBitmap addSrc)////27图像加法
        {
            if (src != null)
            {
                int w = src.PixelWidth;
                int h = src.PixelHeight;
                WriteableBitmap addImage = new WriteableBitmap(w, h);
                byte[] temp = src.PixelBuffer.ToArray();
                byte[] addTemp = addSrc.PixelBuffer.ToArray();
                byte[] dst = new byte[w * h * 4];
                int r = 0, g = 0, b = 0;
                for (int i = 0; i < w; i++)
                {
                    for (int j = 0; j < h; j++)
                    {
                        b = temp[i * 4 + (h - 1 - j) * w * 4] + addTemp[i * 4 + (h - 1 - j) * w * 4];
                        g = temp[i * 4 + 1 + (h - 1 - j) * w * 4] + addTemp[i * 4 + 1 + (h - 1 - j) * w * 4];
                        r = temp[i * 4 + 2 + (h - 1 - j) * w * 4] + addTemp[i * 4 + 2 + (h - 1 - j) * w * 4];
                        dst[i * 4 + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
                        dst[i * 4 + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
                        dst[i * 4 + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
                        dst[i * 4 + 3 + j * w * 4] = 0;
                        b = 0; g = 0; r = 0;
                    }
                }
                Stream sTemp = addImage.PixelBuffer.AsStream();
                sTemp.Seek(0, SeekOrigin.Begin);
                sTemp.Write(dst, 0, w * 4 * h);
                return addImage;
            }
            else
            {
                return null;
            }
        }

```

![img](https://img-blog.csdn.net/20150410125315476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.28图像乘法运算


**[函数名称]**

图像乘法函数MultiplicationProcess(WriteableBitmap src, WriteableBitmap mulSrc)

![img](https://img-blog.csdn.net/20150410125550073?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
/// Multiplication of two images(Both of them should be in same size).
/// </summary>
/// <param name="src">The first source image.</param>
/// <param name="maskSrc">The second source image(It is an mask that each pixel of it  is 0 or 1).</param>
/// <returns></returns>
public static WriteableBitmap MultiplicationProcess(WriteableBitmap src, WriteableBitmap maskSrc)////28图像乘法
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap mulImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] mulTemp = maskSrc.PixelBuffer.ToArray();
		byte[] dst = new byte[w * h * 4];
		int r = 0, g = 0, b = 0;
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				b = temp[i * 4 + (h - 1 - j) * w * 4] * mulTemp[i * 4 + (h - 1 - j) * w * 4];
				g = temp[i * 4 + 1 + (h - 1 - j) * w * 4] * mulTemp[i * 4 + 1 + (h - 1 - j) * w * 4];
				r = temp[i * 4 + 2 + (h - 1 - j) * w * 4] * mulTemp[i * 4 + 2 + (h - 1 - j) * w * 4];
				dst[i * 4 + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
				dst[i * 4 + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
				dst[i * 4 + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				dst[i * 4 + 3 + j * w * 4] = 0;
				b = 0; g = 0; r = 0;
			}
		}
		Stream sTemp = mulImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(dst, 0, w * 4 * h);
		return mulImage;
	}
	else
	{
		return null;
	}
}

```

![img](https://img-blog.csdn.net/20150410125643596?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.29图像除法运算


**[函数名称]**

图像除法函数DivisionProcess(WriteableBitmap src, WriteableBitmap divSrc)

![img](https://img-blog.csdn.net/20150410125956288?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
/// Division of two images.
/// </summary>
/// <param name="src">The frist source image.</param>
/// <param name="divSrc">The second source image.</param>
/// <returns></returns>
public static WriteableBitmap DivisionProcess(WriteableBitmap src, WriteableBitmap divSrc)////29图像除法
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap divImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] divTemp = divSrc.PixelBuffer.ToArray();
		byte[] dst = new byte[w * h * 4];
		int r = 0, g = 0, b = 0, graySrc = 0, grayDiv = 0;
		for (int i = 0; i < w; i++)
		{
			for (int j = 0; j < h; j++)
			{
				graySrc = temp[i * 4 + (h - 1 - j) * w * 4] + temp[i * 4 + 1 + (h - 1 - j) * w * 4] + temp[i * 4 + 2 + (h - 1 - j) * w * 4];
				grayDiv = divTemp[i * 4 + (h - 1 - j) * w * 4] + divTemp[i * 4 + 1 + (h - 1 - j) * w * 4] + divTemp[i * 4 + 2 + (h - 1 - j) * w * 4];
				if (grayDiv + graySrc != 0)
				{
					b = temp[i * 4 + (h - 1 - j) * w * 4] * (temp[i * 4 + (h - 1 - j) * w * 4] - divTemp[i * 4 + (h - 1 - j) * w * 4]) / (temp[i * 4 + (h - 1 - j) * w * 4] + divTemp[i * 4 + (h - 1 - j) * w * 4]);
					g = temp[i * 4 + 1 + (h - 1 - j) * w * 4] * (temp[i * 4 + 1 + (h - 1 - j) * w * 4] - divTemp[i * 4 + 1 + (h - 1 - j) * w * 4]) / (temp[i * 4 + 1 + (h - 1 - j) * w * 4] + divTemp[i * 4 + 1 + (h - 1 - j) * w * 4]);
					r = temp[i * 4 + 2 + (h - 1 - j) * w * 4] * (temp[i * 4 + 2 + (h - 1 - j) * w * 4] - divTemp[i * 4 + 2 + (h - 1 - j) * w * 4]) / (temp[i * 4 + 2 + (h - 1 - j) * w * 4] + divTemp[i * 4 + 2 + (h - 1 - j) * w * 4]);
					dst[i * 4 + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
					dst[i * 4 + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
					dst[i * 4 + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
					dst[i * 4 + 3 + j * w * 4] = 0;
					b = 0; g = 0; r = 0;
				}
			}
		}
		Stream sTemp = divImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(dst, 0, w * 4 * h);
		return divImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150410130008085?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

#  Win8Metro(C#)数字图像处理--2.30直方图均衡化


**[函数名称]**

直方图均衡化函数HistogramEqualProcess(WriteableBitmap src)

**[算法说明]**

  直方图均衡化，又叫做直方图修平，是对图像进行非线性拉伸，重新分配图像像素值，把原始图像的灰度直方图从比较集中的某个灰度区间转换为全部灰度范围内的均匀分布，这样就增加了像素灰度值的动态范围，达到增强图像整体对比度的效果。

![img](https://img-blog.csdn.net/20150411090125422?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**[函数代码]**

```csharp
        /// <summary>
        /// Histogram equalization process.
        /// </summary>
        /// <param name="src">The source image.</param>
        /// <returns></returns>
        public static WriteableBitmap HistogramEqualProcess(WriteableBitmap src)////30图像直方图均衡化
        {
            if (src != null)
            {
                int w = src.PixelWidth;
                int h = src.PixelHeight;
                WriteableBitmap histogramEqualImage = new WriteableBitmap(w, h);
                byte[] temp = src.PixelBuffer.ToArray();
                byte gray;
                int[] tempArray = new int[256];
                int[] countPixel = new int[256];
                byte[] pixelMap = new byte[256];
                for (int i = 0; i < temp.Length; i += 4)
                {
                    gray = (byte)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
                    countPixel[gray]++;
                }
                for (int i = 0; i < 256; i++)
                {
                    if (i != 0)
                    {
                        tempArray[i] = tempArray[i - 1] + countPixel[i];
                    }
                    else
                    {
                        tempArray[0] = countPixel[0];
                    }
                    pixelMap[i] = (byte)(255 * tempArray[i] * 4 / temp.Length + 0.5);
                }
                for (int i = 0; i < temp.Length; i+=4)
                {
                    gray = temp[i];
                    temp[i] = pixelMap[gray];
                    gray = temp[i+1];
                    temp[i+1] = pixelMap[gray];
                    gray = temp[i+2];
                    temp[i+2] = pixelMap[gray];
                }
                Stream sTemp = histogramEqualImage.PixelBuffer.AsStream();
                sTemp.Seek(0, SeekOrigin.Begin);
                sTemp.Write(temp, 0, w * 4 * h);
                return histogramEqualImage;
            }
            else
            {
                return null;
            }
        }

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150411090221613?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.31灰度拉伸算法


**[函数名称]**

灰度拉伸函数GrayStretchProcess(WriteableBitmap src)

**[算法说明]**

  直方图灰度拉伸也叫做对比度拉伸，是一种特殊的线性点运算，使用的是分段线性变换函数，它的主要思想是提高图像灰度级的动态范围；它的作用是扩展图像的直方图，使其充满整个灰度等级的范围内，从而改善输出图像。

  如图Fig.1所示，变换函数的运算结果是将原图在a-b之间的灰度级拉伸到c-d之间。如果一幅图像的灰度级集中在较暗的区域从而导致图像偏暗，或者一幅图像的灰度级集中在较亮的区域从而导致图像偏亮，则可以使用灰度拉伸来改善图像质量。

![img](https://img-blog.csdn.net/20150411090430142?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**

```csharp
/// <summary>
/// Gray stretch process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap GrayStretchProcess(WriteableBitmap src)////31图像灰度拉伸
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap grayStretchImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int min = 0;
		int max = 0;
		int gray = 0;
		int res = 0;
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			min = min < gray ? min : gray;
			max = max > gray ? max : gray;
		}
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			res = (255 / (max - min)) * (gray - min);
			temp[i] = (byte)res;
			temp[i + 1] = (byte)res;
			temp[i + 2] = (byte)res;
		}
		Stream sTemp = grayStretchImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return grayStretchImage;
	}
	else
	{
		return null;
	}
}

```

![img](https://img-blog.csdn.net/20150411090519360?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.32图像曝光算法


**[函数名称]**

图像曝光函数ExposureProcess(WriteableBitmap src,int exposureValue)

**![img](https://img-blog.csdn.net/20150411090740661?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**[函数代码]**

```csharp
/// <summary>
/// Exposure process.
/// </summary>
/// <param name="src">Source image.</param>
/// <param name="exposureValue">To adjust exposure lavel, from 0 to 255.</param>
/// <returns></returns>
public static WriteableBitmap ExposureProcess(WriteableBitmap src, int exposureValue)////35图像曝光
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap exposureImage = new WriteableBitmap(w, h);
		int r = 0, g = 0, b = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			byte tempByte = (byte)((int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299));
			b = temp[i];
			g = temp[i + 1];
			r = temp[i + 2];
			if (tempByte < 128)
			{
				temp[i] = (byte)(255 - b);
				temp[i + 1] = (byte)(255 - g);
				temp[i + 2] = (byte)(255 - r);
			}
		}
		Stream sTemp = exposureImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return exposureImage;
	}
	else
	{
		return null;
	}
}

```

**[图像效果]**

![img](https://img-blog.csdn.net/20150411090819506?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.33图像非线性变换


**[函数名称]**

图像非线性变换函数NonlinearTransformProcess(WriteableBitmap src,int k )

**![img](https://img-blog.csdn.net/20150412104414206?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

```csharp
/// <summary>
/// Nonlinear transform process.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="k">Param to adjust nonlinear transform, from 0 to 255.</param>
/// <returns></returns>
public static WriteableBitmap NonlinearTransformProcess(WriteableBitmap src, int k)////37
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap linearImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int r = 0, g = 0, b = 0;
		for (int i = 0; i < temp.Length; i += 4)
		{
			b = (int)(k * Math.Log10(1 + temp[i]));
			g = (int)(k * Math.Log10(1 + temp[i + 1]));
			r = (int)(k * Math.Log10(1 + temp[i + 2]));
			temp[i] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
			temp[i + 1] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
			temp[i + 2] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
		}
		Stream sTemp = linearImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return linearImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150412104537522?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.34直方图规定化


**[函数名称]**

WriteableBitmap HistogramSpecificateProcess(WriteableBitmap src, WriteableBitmap dst)

**[算法说明]**

**![img](https://img-blog.csdn.net/20150412104701157?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

[函数代码]

```csharp
/// <summary>
/// Histogram specification process.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="dst">The image to get histogram to use.</param>
/// <returns></returns>
public static WriteableBitmap HistogramSpecificateProcess(WriteableBitmap src, WriteableBitmap dst)////38图像直方图规定化
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		byte[]histMap = HistogramMap(dst);
		WriteableBitmap histImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			temp[i] = (byte)(255 * histMap[temp[i]]);
			temp[i + 1] = (byte)(255 * histMap[temp[i + 1]]);
			temp[i + 2] = (byte)(255 * histMap[temp[i + 2]]);
		}
		Stream sTemp = histImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return histImage;
	}
	else
	{
		return null;
	}
}
public static byte[] HistogramMap(WriteableBitmap src)
{
	if (src != null)
	{
		byte[] temp = src.PixelBuffer.ToArray();
		byte gray;
		int[] tempArray = new int[256];
		int[] countPixel = new int[256];
		byte[] pixelMap = new byte[256];
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (byte)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			countPixel[gray]++;
		}
		for (int i = 0; i < 256; i++)
		{
			if (i != 0)
			{
				tempArray[i] = tempArray[i - 1] + countPixel[i];
			}
			else
			{
				tempArray[0] = countPixel[0];
			}
			pixelMap[i] = (byte)(255 * tempArray[i] * 4 / temp.Length + 0.5);
		}
		return pixelMap;
	}
	else
		return null;
}

```

  <img src="https://img-blog.csdn.net/20150412104816739?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center" alt="" />

## C#数字图像处理--2.35图像肤色检测算法

﻿﻿

**[函数名称]**

肤色检测函数SkinDetectProcess(WriteableBitmap src)

**[算法说明]**

  这个算法是一篇学术论文算法的实现，论文名字为“基于韧脸检测和颜色分析的红眼自动消除”(作者金秋明，王朔中)，主要是采集并统计肤色像素，得到肤色像素在RGB颜色空间中的分布范围，以此作为像素是否为肤色像素的判断标准及约束条件。具体内容大家可以在网络中搜索，由统计结果得到的肤色范围如下公式2-(45),2-(46)所示:

![img](https://img-blog.csdn.net/20150412105217959?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// Skin detection.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap SkinDetectProcess(WriteableBitmap src)////36肤色检测
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int R, G, B, S;
		double r, g, b;
		for (int i = 0; i < temp.Length; i += 4)
		{
			B = tempMask[i];
			G = tempMask[i + 1];
			R = tempMask[i + 2];
			S = R + G + B;
			r = (double)R / (double)(R + G + B + 1.0);
			g = (double)G / (double)(R + G + B + 1.0);
			b = (double)B / (double)(R + G + B + 1.0);
			if (S != 0)
			{
				if ((r > (double)(95.0 / (double)S)) && (r < 1 - (double)(90.0 / (double)S)) && (g > (double)(50.0 / (double)S)) && ((r - g) > (double)(30.0 / (double)S)) && (r - g < 0.2))
				{
					temp[i] = (byte)B;
					temp[i + 1] = (byte)G;
					temp[i + 2] = (byte)R;
				}
				else
				{
					temp[i] = 0;
					temp[i + 1] = 0;
					temp[i + 2] = 0;
				}
			}
			else
			{
				temp[i] = 0;
				temp[i + 1] = 0;
				temp[i + 2] = 0;
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}

```

![img](https://img-blog.csdn.net/20150412105222440?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.36角点检测算法


**[函数名称]**

Harris角点检测函数    HarrisDetect(WriteableBitmap src, int CRF)

**[算法说明]**

  目前的角点检测算法可归纳为3类：基于灰度图像的角点检测、基于二值图像的角点检测、基于轮廓曲线的角点检测。基于灰度图像的角点检测又可分为基于梯度、基于模板和基于模板梯度组合3类方法，其中基于模板的方法主要考虑像素领域点的灰度变化，即图像亮度的变化，将与邻点亮度对比足够大的点定义为角点。本文将介绍一种改进的Harris角点检测算法，该算法是一种基于模板与梯度组合的方法。

![img](https://img-blog.csdn.net/20150413110327952?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[函数代码]

```csharp
/// <summary>
/// Harris counter-detect.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="v">The threshould to control counters number.</param>
/// <returns></returns>
public static int[, ] HarrisDetect(WriteableBitmap src, int CRF)
{
	int x = src.PixelWidth;
	int y = src.PixelHeight;
	double[, ] Ix = new double[x, y];
	double[, ] Iy = new double[x, y];
	double[, ] Ixy = new double[x, y];
	double[, ] cim = new double[x, y];
	int[, ] re = new int[x, y];
	double[, ] srcBytes = GetImageBytes(src);
	GetIV(srcBytes, Ix, Iy, Ixy, x, y);
	GaussFilter(Ix, Iy, Ixy, x, y);
	cim = GetCim(Ix, Iy, Ixy, x, y);
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			if ((cim[i, j] == GetMax(cim[i - 1, j - 1], cim[i, j - 1], cim[i + 1, j - 1], cim[i - 1, j], cim[i, j], cim[i + 1, j], cim[i - 1, j + 1], cim[i, j + 1], cim[i + 1, j + 1])) && (cim[i, j] > CRF))
			{
				re[i, j] = 1;
			}
		}
	}
	return re;
}
//获得角点图像的 原始 信息
public static double[, ] GetImageBytes(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[, ] imageBytes = new double[w, h];
		int b = 0, g = 0, r = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w * 4; x += 4)
			{
				b = temp[x + y * w * 4];
				g = temp[x + 1 + y * w * 4];
				r = temp[x + 2 + y * w * 4];
				imageBytes[x, y] = (b * 0.114 + g * 0.587 + r * 0.299);
			}
		}
		return imageBytes;
	}
	else
	{
		return null;
	}
}
//梯度求取函数
private static void GetIV(double[, ] src, double[, ] Ix, double[, ] Iy, double[, ] Ixy, int x, int y)
{
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			Ix[i, j] = Math.Abs(src[i + 1, j - 1] + src[i + 1, j] + src[i + 1, j + 1] - src[i - 1, j - 1] - src[i - 1, j] - src[i - 1, j + 1]);
			Iy[i, j] = Math.Abs(src[i - 1, j + 1] + src[i, j + 1] + src[i + 1, j + 1] - src[i - 1, j - 1] - src[i, j - 1] - src[i + 1, j - 1]);
			Ixy[i, j] = Math.Abs(Ix[i, j] * Iy[i, j]);
		}
	}
}
//高斯滤波函数(对梯度图像进行高斯滤波，这里采用的是3*3的高斯滤波模板)
private static void GaussFilter(double[, ] Ix, double[, ] Iy, double[, ] Ixy, int x, int y)
{
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			Ix[i, j] = (Ix[i - 1, j - 1] + Ix[i, j - 1] * 2 + Ix[i + 1, j - 1] + 2 * Ix[i - 1, j] + 4 * Ix[i, j] + 2 * Ix[i + 1, j] + Ix[i - 1, j + 1] + 2 * Ix[i, j + 1] + Ix[i + 1, j + 1]) / 16;
			Iy[i, j] = (Iy[i - 1, j - 1] + Iy[i, j - 1] * 2 + Iy[i + 1, j - 1] + 2 * Iy[i - 1, j] + 4 * Iy[i, j] + 2 * Iy[i + 1, j] + Iy[i - 1, j + 1] + 2 * Iy[i, j + 1] + Ix[i + 1, j + 1]) / 16;
			Ixy[i, j] = (Ixy[i - 1, j - 1] + Ixy[i, j - 1] * 2 + Ixy[i + 1, j - 1] + 2 * Ixy[i - 1, j] + 4 * Ixy[i, j] + 2 * Ixy[i + 1, j] + Ixy[i - 1, j + 1] + 2 * Ixy[i, j + 1] + Ix[i + 1, j + 1]) / 16;
		}
	}
}
//图像角点求取函数
private static double[, ] GetCim(double[, ] Ix, double[, ] Iy, double[, ] Ixy, int x, int y)
{
	double cim = 0;
	double[, ] results = new double[x, y];
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			if (Ix[i, j] != 0 || Iy[i, j] != 0)
			{
				cim = Math.Abs(Ix[i, j] * Iy[i, j] - Ixy[i, j] * Ixy[i, j]) / (Ix[i, j] * Ix[i, j] + Iy[i, j] * Iy[i, j]);
				results[i, j] = cim;
			}
		}
	}
	return results;
}
//最大值获取函数
private static double GetMax(params double[] src)
{
	double tMax = 0;
	for (int i = 0; i < src.Length; i++)
	{
		if (tMax < src[i])
		{
			tMax = src[i];
		}
	}
	return tMax;
}
```


[图像效果]

![img](https://img-blog.csdn.net/20150413110409623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## C#数字图像处理--2.37Wallis图象锐化


**[函数名称]**

Wallis图象锐化        WallisSharpen(WriteableBitmap src)

**[算法说明]**

 Wallis锐化算法是在拉普拉斯算子的基础上，考虑人的视觉特性中包含一个对数环节，因此在锐化时，采用对数处理的方法进行改进，公式如下：

![img](https://img-blog.csdn.net/20150413125726521?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

[函数代码]

```csharp
/// <summary>
/// Wallis sharpen process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap WallisSharpen(WriteableBitmap src)////37Wallis锐化函数
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap sharpenImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double b = 0, g = 0, r = 0, srR = 0, srG = 0, srB = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{

				srB = tempMask[i + j * w * 4];
				srG = tempMask[i + 1 + j * w * 4];
				srR = tempMask[i + 2 + j * w * 4];
				b = 46 * Math.Abs(5 * Math.Log(srB + 1) - Math.Log(tempMask[i - 4 + j * w * 4] + 1) - Math.Log(tempMask[i + 4 + j * w * 4] + 1) - Math.Log(tempMask[i + (j - 1) * w * 4] + 1) - Math.Log(tempMask[i + (j + 1) * w * 4] + 1));
				g = 46 * Math.Abs(5 * Math.Log(srG + 1) - Math.Log(tempMask[i - 4 + 1 + j * w * 4] + 1) - Math.Log(tempMask[i + 4 + 1 + j * w * 4] + 1) - Math.Log(tempMask[i + 1 + (j - 1) * w * 4] + 1) - Math.Log(tempMask[i + 1 + (j + 1) * w * 4] + 1));
				r = 46 * Math.Abs(5 * Math.Log(srR + 1) - Math.Log(tempMask[i - 4 + 2 + j * w * 4] + 1) - Math.Log(tempMask[i + 4 + 2 + j * w * 4] + 1) - Math.Log(tempMask[i + 2 + (j - 1) * w * 4] + 1) - Math.Log(tempMask[i + 2 + (j + 1) * w * 4] + 1));
				temp[i + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
				temp[i + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
				temp[i + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
				b = 0; g = 0; r = 0; srR = 0; srG = 0; srB = 0;
			}
		}
		Stream sTemp = sharpenImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return sharpenImage;
	}
	else
	{
		return null;
	}
}
```

[图象效果]

![img](https://img-blog.csdn.net/20150413125804011?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.38Hough变换直线检测


**[函数名称]**

Hough 变换直线检测         HoughLineDetect(WriteableBitmap src, int threshould)

**[算法说明]**

  Hough变换是数字图像处理中一种常用的几何形状识别方法，它可以识别直线，圆，椭圆，弧线等

等几何形状，其基本原理是利用图像二维空间和Hough参数空间的点-线对偶性，把图像空间中的形

状检测问题转换到Hough的参数空间中去，最终以寻找参数空间中的峰值问题，得到形状检测的最优

结果。

![img](https://img-blog.csdn.net/20150413130036595?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
 /// Hough transform of line detectting process.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <param name="threshould">The threshould to adjust the number of lines.</param>
 /// <returns></returns>
	public static WriteableBitmap HoughLineDetect(WriteableBitmap src, int threshould)////2 Hough 变换直线检测
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int roMax = (int)Math.Sqrt(w * w + h * h) + 1;
		int[, ] mark = new int[roMax, 180];
		double[] theta = new double[180];
		for (int i = 0; i < 180; i++)
		{
			theta[i] = (double)i * Math.PI / 180.0;
		}
		double roValue = 0.0;
		int transValue = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				if (temp[x * 4 + y * w * 4] == 0)
				{
					for (int k = 0; k < 180; k++)
					{
						roValue = (double)x * Math.Cos(theta[k]) + (double)y * Math.Sin(theta[k]);
						transValue = (int)Math.Round(roValue / 2 + roMax / 2);
						mark[transValue, k]++;
					}
				}
			}
		}
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				int T = x * 4 + y * w * 4;
				if (temp[T] == 0)
				{
					for (int k = 0; k < 180; k++)
					{
						roValue = (double)x * Math.Cos(theta[k]) + (double)y * Math.Sin(theta[k]);
						transValue = (int)Math.Round(roValue / 2 + roMax / 2);
						if (mark[transValue, k] > threshould)
						{
							temp[T + 2] = (byte)255;
						}
					}
				}
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```


[图像效果]
![img](https://img-blog.csdn.net/20150413130155609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

注意：图中没有标红的线，是因为threshold=80，如果这个值改变，会影响检测结果，这个值足够小，另外两条直线也将被标红。

## C#数字图像处理--2.39二值图像投影


**[函数名称]**

  二值图像投影         ImageProjection(WriteableBitmap src)

**[算法说明]**

**![img](https://img-blog.csdn.net/20150414123121470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**





**[函数代码]**

```csharp
/// <summary>
/// Get projections of X and Y direction.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static int[][] ImageProjection(WriteableBitmap src)////二值图像投影
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		int[][] Centerpoint = new int[2][];
		Centerpoint[0] = new int[w + 1];
		Centerpoint[1] = new int[h + 1];
		int b = 0, g = 0, r = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w * 4; x += 4)
			{
				b = temp[x + y * w * 4];
				g = temp[x + 1 + y * w * 4];
				r = temp[x + 2 + y * w * 4];
				if (r + g + b == 0)
				{
					Centerpoint[1][y]++;
				}
			}
		}
		for (int x = 0; x < w * 4; x += 4)
		{
			for (int y = 0; y < h; y++)
			{
				b = temp[x + y * w * 4];
				g = temp[x + 1 + y * w * 4];
				r = temp[x + 2 + y * w * 4];
				if (r + g + b == 0)
				{
					Centerpoint[0][(int)(x / 4)]++;
				}
			}
		}
		return Centerpoint;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150414123415129?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.40二值图像轮廓提取算法


**[函数名称]**

  二值图像轮廓提取         ContourExtraction(WriteableBitmap src)

**[算法说明]**

  二值图像的轮廓提取对于图像识别，图像分割有着重要意义。该算法的核心就是将图像目标的内部点消除。所谓内部点，我们要根据当前像素点的邻域来进行判断，假设邻域窗口为3*3窗口，如果当前像素P(x,y)的八个邻域像素满足如下条件，则该点即内部点：

  1，P(x,y)为目标像素，假设目标像素为黑色0，背景像素为白色255，那么P(x,y)=0;

  2，P(x,y)的八个邻域像素均为目标像素0；

  我们把满足条件的内部点删除，换为背景点255，即可得到图像轮廓。

  内部点如下图所示：

![img](https://img-blog.csdn.net/20150414123735137?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
/// Contour Extraction process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap ContourExtraction(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 4; i < w * 4 - 4; i += 4)
			{
				if ((tempMask[i + j * w * 4] == 0) && (tempMask[i - 4 + j * w * 4] == 0) && (tempMask[i + 4 + j * w * 4] == 0) && (tempMask[i - 4 + (j - 1) * w * 4] == 0)
					&& (tempMask[i - 4 + (j + 1) * w * 4] == 0) && (tempMask[i + (j - 1) * w * 4] == 0) && (tempMask[i + (j + 1) * w * 4] == 0)
					&& (tempMask[i + 4 + (j - 1) * w * 4] == 0) && (tempMask[i + 4 + (j + 1) * w * 4] == 0))
				{
					temp[i + j * w * 4] = (byte)255;
					temp[i + 1 + j * w * 4] = (byte)255;
					temp[i + 2 + j * w * 4] = (byte)255;
				}
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```



![img](https://img-blog.csdn.net/20150414123931323?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.41彩色图像密度分割算法


**[函数名称]**

  彩色图像密度分割函数      DensitySegmentProcess(WriteableBitmap src)

**[算法说明]**

  图像密度分割又叫做彩色等密度分割处理，一般图像（或影像）上色调的明暗是以附着在片基上的银粒子密度来计量的。因此，为了突出某一密度等级的色调（或相应地物），即将图像（或影像）的色调密度分划成若干个等级，并用不同的颜色分别表示这不同的密度等级，得到一幅彩色的等密度分割图像。这一技术过程就叫作密度分割处理，或简称密度分割。密度分割可使影像轮廓更清晰，突出某些具有一定色调特征的地物及分布状态，在显示环境污染范围，隐伏构造，以及寻找地下水等方面有广泛的应用，并取得较好的效果。密度分割后得到的彩色图像的色彩是人为加于的，一般并不代表地物的实际颜色，所以一般也称密度分割为假彩色密度分割。

  这里我们列举的是基于颜色灰度的密度分割。

**[函数代码]**

```csharp
/// <summary>
 /// Density segmentation.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <returns></returns>
	public static WriteableBitmap DensitySegmentProcess(WriteableBitmap src)////彩色图像密度分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int color = 0;
		for (int i = 0; i < temp.Length; i += 4)
		{
			byte tempByte = (byte)((int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299));
			color = GetColor(tempByte);
			switch (color)
			{
			case 1:
				temp[i] = (byte)255; temp[i + 1] = (byte)255; temp[i + 2] = (byte)0;
				break;
			case 2:
				temp[i] = (byte)255; temp[i + 1] = (byte)0; temp[i + 2] = (byte)255;
				break;
			case 3:
				temp[i] = (byte)0; temp[i + 1] = (byte)255; temp[i + 2] = (byte)255;
				break;
			case 4:
				temp[i] = (byte)255; temp[i + 1] = (byte)0; temp[i + 2] = (byte)0;
				break;
			case 5:
				temp[i] = (byte)0; temp[i + 1] = (byte)0; temp[i + 2] = (byte)255;
				break;
			case 0:
				temp[i] = (byte)0; temp[i + 1] = (byte)0; temp[i + 2] = (byte)0;
				break;
			default:
				break;
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
//定义密度等级获得函数
private static int GetColor(int v)
{
	int t = 0;
	if (v == 0)
	{
		t = 0;
	}
	else if (v > 0 && v < 50)
	{
		t = 1;
	}
	else if (v >= 50 && v < 100)
	{
		t = 2;
	}
	else if (v >= 100 && v < 150)
	{
		t = 3;
	}
	else if (v >= 150 && v < 200)
	{
		t = 4;
	}
	else
	{
		t = 5;
	}
	return t;
}
```

![img](https://img-blog.csdn.net/20150414124156154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.42图像光照效果算法


**[函数名称]**

图像光照效果  SunlightProcess(WriteableBitmap src,int X,int Y,float thresould)

**[算法说明]**

  图像光照效果就是在图像中添加上一个太阳光源，以此模仿光照条件。主要算法包括：

1光源选择；2光照像素值求取。

  1，光源选择。我们假设光源位置坐标为S(X,Y)，其中光源坐标一定要保证在图像大小

范围内。有了光源位置，我们就可以来构建一个圆形区域模拟光照了。我们设定光源半

径为R，那么，光照范围就是以S(X,Y)为圆心，以R为半径的圆了。

  根据光源特性，在图像中表现为中间靠近圆心最亮，亮度延半径方向向四周逐渐减弱。

因此，我们根据光源圆形的范围内像素距离圆心的欧几里得距离来构建线性变换的公式，

假设欧几里得距离为D，变换后的像素值为f，则公式如下：

![img](https://img-blog.csdn.net/20150415123145681?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
 /// Sun light process.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <param name="A">X location of light source.</param>
 /// <param name="B">Y location of light source.</param>
 /// <param name="thresould">Light intensity value.</param>
 /// <returns></returns>
	public static WriteableBitmap SunlightProcess(WriteableBitmap src, int X, int Y, float thresould)////41图像光照函数
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double b = 0, g = 0, r = 0;
		if (X >= w || Y >= h || X < 0 || Y < 0)
		{
			X = w / 2;
			Y = h / 2;
		}
		Point Cen = new Point(X, Y);
		int R = Math.Min(X, Y);
		float curR = 0;
		float pixelValue = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				b = tempMask[i * 4 + j * w * 4];
				g = tempMask[i * 4 + 1 + j * w * 4];
				r = tempMask[i * 4 + 2 + j * w * 4];
				curR = (float)Math.Sqrt(Math.Pow((i - Cen.X), 2) + Math.Pow((j - Cen.Y), 2));
				if (curR < R)
				{
					pixelValue = thresould * (1.0f - curR / R);
					b = b + pixelValue;
					g = g + pixelValue;
					r = r + pixelValue;
					temp[i * 4 + j * w * 4] = (byte)(b > 0 ? (b < 255 ? b : 255) : 0);
					temp[i * 4 + 1 + j * w * 4] = (byte)(g > 0 ? (g < 255 ? g : 255) : 0);
					temp[i * 4 + 2 + j * w * 4] = (byte)(r > 0 ? (r < 255 ? r : 255) : 0);
					b = 0; g = 0; r = 0;
				}
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150415123214934?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.43图像马赛克效果算法


**[函数名称]**

  图像马赛克效果        MosaicProcess(WriteableBitmap src, int v)

**[算法说明]**

  图像马赛克效果其实就是将图像分成大小一致的图像块，每一个图像块都是一个正方形，并且在这个正方形中所有像素值都相等。我们可以将这个正方形看作是一个模板窗口，模板中对应的所有图像像素值都等于该模板的左上角第一个像素的像素值，这样的效果就是马赛克效果，而正方形模板的大小则决定了马赛克块的大小，即图像马赛克化的程度。

**[函数代码]**

```csharp
/// <summary>
 /// Mosaic process.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <param name="v">The threshould to control the result of mosaic process.</param>
 /// <returns></returns>
	public static WriteableBitmap MosaicProcess(WriteableBitmap src, int v)////图像马赛克效果
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int dR = 0;
		int dG = 0;
		int dB = 0;
		int dstX = 0;
		int dstY = 0;
		dR = tempMask[2];
		dG = tempMask[1];
		dB = tempMask[0];
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				dstX = i;
				dstY = j;
				if (j % v == 0)
				{
					if (i % v == 0)
					{
						dB = tempMask[dstX * 4 + dstY * w * 4];
						dG = tempMask[dstX * 4 + 1 + dstY * w * 4];
						dR = tempMask[dstX * 4 + 2 + dstY * w * 4];
					}
					else
					{
						temp[dstX * 4 + dstY * w * 4] = (byte)dB;
						temp[dstX * 4 + 1 + dstY * w * 4] = (byte)dG;
						temp[dstX * 4 + 2 + dstY * w * 4] = (byte)dR;
					}
				}
				else
				{
					temp[dstX * 4 + dstY * w * 4] = temp[dstX * 4 + (dstY - 1) * w * 4];
					temp[dstX * 4 + 1 + dstY * w * 4] = temp[dstX * 4 + 1 + (dstY - 1) * w * 4];
					temp[dstX * 4 + 2 + dstY * w * 4] = temp[dstX * 4 + 2 + (dstY - 1) * w * 4];
				}
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```



**[图像效果]**

![img](https://img-blog.csdn.net/20150415123458014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.44图像油画效果算法

﻿﻿

**[函数名称]**

  图像油画效果      OilpaintingProcess(WriteableBitmap src)

**[算法说明]**

  图像油画效果可以看作是轻度的雾化，因此我们只需将雾化的阈值设置为2即可，雾化效果在下一小节有详细介绍。

**[函数代码]**

```csharp
/// <summary>
/// Oil painting process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap OilpaintingProcess(WriteableBitmap src)////图像油画效果
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		Random ran = new Random();
		int k = 0;
		int dx = 0;
		int dy = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 1; i < w; i++)
			{
				k = ran.Next(2);
				dx = (i + k) >= w ? w - 1 : (i + k);
				dy = (j + k) >= h ? h - 1 : (j + k);
				temp[i * 4 + j * w * 4] = (byte)tempMask[dx * 4 + dy * w * 4];
				temp[i * 4 + 1 + j * w * 4] = (byte)tempMask[dx * 4 + 1 + dy * w * 4];
				temp[i * 4 + 2 + j * w * 4] = (byte)tempMask[dx * 4 + 2 + dy * w * 4];
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150416094529563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.45图像雾化效果算法


**[函数名称]**

  图像雾化         AtomizationProcess(WriteableBitmap src,int v)

**[算法说明]**

**![img](https://img-blog.csdn.net/20150416094905174?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

```csharp
/// <summary>
/// Atomization process.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="v">The threshould to control the effect of atomization.</param>
/// <returns></returns>
public static WriteableBitmap AtomizationProcess(WriteableBitmap src, int v)////45图像雾化
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		Random ran = new Random();
		int k = 0;
		int dx = 0;
		int dy = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				k = ran.Next(v);
				dx = (i + k) >= w ? w - 1 : (i + k);
				dy = (j + k) >= h ? h - 1 : (j + k);
				temp[i * 4 + j * w * 4] = (byte)tempMask[dx * 4 + dy * w * 4];
				temp[i * 4 + 1 + j * w * 4] = (byte)tempMask[dx * 4 + 1 + dy * w * 4];
				temp[i * 4 + 2 + j * w * 4] = (byte)tempMask[dx * 4 + 2 + dy * w * 4];
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150416095022987?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.46图像RGB分量增强效果

﻿﻿

**[函数名称]**

RGB分量调整         RGBAdjustProcess(WriteableBitmap src, int value,int threshould)

**[算法说明]**

  RGB分量调整实际上是分别对每个像素的RGB三个分量进行调整，公式如下：

![img](https://img-blog.csdn.net/20150417132503394?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
/// R,G,B value adjusting.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="value">To judge which one to adjust, R is 3, G is 2, B is 1.</param>
/// <param name="threshould">It is a value to adjust the result image.</param>
/// <returns></returns>
public static WriteableBitmap RGBAdjustProcess(WriteableBitmap src, int value, int threshould)////41 RGB分量调整
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		if (value == 1)
		{
			for (int i = 0; i < temp.Length; i += 4)
			{
				temp[i] = (byte)(Math.Max(0, Math.Min((temp[i] + threshould), 255)));
			}
		}
		if (value == 2)
		{
			for (int i = 0; i < temp.Length; i += 4)
			{
				temp[i + 1] = (byte)(Math.Max(0, Math.Min((temp[i + 1] + threshould), 255)));
			}
		}
		if (value == 3)
		{
			for (int i = 0; i < temp.Length; i += 4)
			{
				temp[i + 2] = (byte)(Math.Max(0, Math.Min((temp[i + 2] + threshould), 255)));
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150417132624405?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.47人脸红眼去除算法

﻿﻿

**[函数名称]**

  红眼去除     RedeyeRemoveProcess(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150417132656737?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Redeye remove Process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap RedeyeRemoveProcess(WriteableBitmap src)////红眼去除
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int r, g, b;
		int Rc, Gc, Bc;
		for (int i = 0; i < temp.Length; i += 4)
		{
			b = tempMask[i];
			g = tempMask[i + 1];
			r = tempMask[i + 2];
			if (r > (int)(g + b))//这里 只是简单的判断一下红眼像素只为说明红眼去除算法，实际上要使用一定的红眼判断算法决策
			{
				Rc = (int)((g + b) / 2);
				Gc = (int)((g + Rc) / 2);
				Bc = (int)((b + Rc) / 2);
				temp[i] = (byte)Bc;
				temp[i + 1] = (byte)Gc;
				temp[i + 2] = (byte)Rc;
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

**![img](https://img-blog.csdn.net/20150417132816235?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**

## C#数字图像处理--2.48Canny边缘检测算法

﻿﻿

**[算法说明]**

Canny边缘检测算法可以分为4步：高斯滤波器平滑处理、梯度计算、非极大值抑制、双阈值边缘检

测和边缘连接。

  1，高斯滤波器平滑处理。由于图像中经常包含一些高斯噪声，因此在边缘检测前我们要先用高斯

滤波器对其进行滤波，为了方便，通常是使用一些高斯模板，这里我们使用如下的高斯滤波器模板。

![img](https://img-blog.csdn.net/20150417133143503?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// Canny edge detect process.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="highThreshould">The high threshould value. </param>
/// <param name="lowThreshould">The low threshould value. </param>
/// <returns></returns>
public static WriteableBitmap CannyedgedetectProcess(WriteableBitmap src, int highThreshould, int lowThreshould)////图像油画效果
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int[, ] srcBytes = new int[w, h];
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcBytes[i, j] = (int)(tempMask[i * 4 + j * w * 4] * 0.114 + tempMask[i * 4 + 1 + j * w * 4] * 0.587 + tempMask[i * 4 + 2 + j * w * 4] * 0.299);
			}
		}
		float gradientMax = 0;
		float[, ] gradient = new float[w, h];
		byte[, ] degree = new byte[w, h];
		GaussFilter(ref srcBytes, w, h);
		GetGradientDegree(srcBytes, ref gradient, ref degree, ref gradientMax, w, h);
		NonMaxMini(gradient, ref srcBytes, gradientMax, w, h, degree);
		TwoThreshouldJudge(highThreshould, lowThreshould, ref srcBytes, w, h);
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)srcBytes[i, j];
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
//高斯滤波
private static void GaussFilter(ref int[, ] src, int x, int y)
{
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			src[i, j] = (4 * src[i, j] + src[i - 1, j - 1] + src[i + 1, j - 1] + src[i - 1, j + 1] + src[i + 1, j + 1] + 2 * src[i, j - 1] + 2 * src[i - 1, j] + 2 * src[i, j + 1] + 2 * src[i + 1, j]) / 16;
		}
	}
}
//梯度相位角获取
private static void GetGradientDegree(int[, ] srcBytes, ref float[, ] gradient, ref byte[, ] degree, ref float GradientMax, int x, int y)
{
	gradient = new float[x, y];
	degree = new byte[x, y];
	int gx, gy;
	int temp;
	double div;
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			gx = srcBytes[i + 1, j - 1] + 2 * srcBytes[i + 1, j] + srcBytes[i + 1, j + 1] - srcBytes[i - 1, j - 1] - 2 * srcBytes[i - 1, j] - srcBytes[i - 1, j + 1];
			gy = srcBytes[i - 1, j - 1] + 2 * srcBytes[i, j - 1] + srcBytes[i + 1, j - 1] - srcBytes[i - 1, j + 1] - 2 * srcBytes[i, j + 1] - srcBytes[i + 1, j + 1];
			gradient[i, j] = (float)Math.Sqrt((double)(gx * gx + gy * gy));
			if (GradientMax < gradient[i, j])
			{
				GradientMax = gradient[i, j];
			}
			if (gx == 0)
			{
				temp = (gy == 0) ? 0 : 90;
			}
			else
			{
				div = (double)gy / (double)gx;
				if (div < 0)
				{
					temp = (int)(180 - Math.Atan(-div) * 180 / Math.PI);
				}
				else
				{
					temp = (int)(Math.Atan(div) * 180 / Math.PI);
				}
				if (temp < 22.5)
				{
					temp = 0;
				}
				else if (temp < 67.5)
				{
					temp = 45;
				}
				else if (temp < 112.5)
				{
					temp = 90;
				}
				else if (temp < 157.5)
				{
					temp = 135;
				}
				else
					temp = 0;
			}
			degree[i, j] = (byte)temp;
		}
	}
}
//非极大值抑制
private static void NonMaxMini(float[, ] gradient, ref int[, ] srcBytes, float GradientMax, int x, int y, byte[, ] degree)
{
	float leftPixel = 0, rightPixel = 0;
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			switch (degree[i, j])
			{
			case 0:
				leftPixel = gradient[i - 1, j];
				rightPixel = gradient[i + 1, j];
				break;
			case 45:
				leftPixel = gradient[i - 1, j + 1];
				rightPixel = gradient[i + 1, j - 1];
				break;
			case 90:
				leftPixel = gradient[i, j + 1];
				rightPixel = gradient[i, j - 1];
				break;
			case 135:
				leftPixel = gradient[i + 1, j + 1];
				rightPixel = gradient[i - 1, j - 1];
				break;
			default:
				break;
			}
			if ((gradient[i, j] < leftPixel) || (gradient[i, j] < rightPixel))
			{
				srcBytes[i, j] = 0;
			}
			else
			{
				srcBytes[i, j] = (int)(255 * gradient[i, j] / GradientMax);
			}
		}
	}
}
//双阈值边缘判断
private static void TwoThreshouldJudge(int highThreshold, int lowThreshould, ref int[, ] srcBytes, int x, int y)
{
	for (int j = 1; j < y - 1; j++)
	{
		for (int i = 1; i < x - 1; i++)
		{
			if (srcBytes[i, j] > highThreshold)
			{
				srcBytes[i, j] = 255;
			}
			else if (srcBytes[i, j] < lowThreshould)
			{
				srcBytes[i, j] = 0;
			}
			else
			{
				if (srcBytes[i - 1, j - 1] < highThreshold && srcBytes[i, j - 1] < highThreshold && srcBytes[i + 1, j - 1] < highThreshold && srcBytes[i - 1, j] < highThreshold
					&& srcBytes[i + 1, j] < highThreshold && srcBytes[i - 1, j + 1] < highThreshold && srcBytes[i, j + 1] < highThreshold && srcBytes[i + 1, j + 1] < highThreshold)
				{
					srcBytes[i, j] = 0;
				}
				else
					srcBytes[i, j] = 255;
			}
		}
	}
}
```

![img](https://img-blog.csdn.net/20150417133259210?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.49Zhang二值图像细化算法


**[函数名称]**

  二值图像细化算法      WriteableBitmap ThinningProcess(WriteableBitmap src)

**[算法说明]**

  图像细化（Image Thinning），一般指二值图像的骨架化（Image Skeletonization）的一种操作运算。

所谓的细化就是经过一层层的剥离，从原来的图中去掉一些点，但仍要保持原来的形状，直到得到图

像的骨架。骨架，可以理解为图象的中轴。

  细化算法有很多，我们这里介绍一种二值图像的快速细化算法—Zhang 细化算法，该算法是Zhang于

1984年提出。

  算法过程如下：

  1，设二值图像中0为背景，1为目标。目标像素的8邻域如下图所示：

![img](https://img-blog.csdn.net/20150417133417117?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// Zhang's fast thinning process for binary image.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap ThinningProcess(WriteableBitmap src)////二值图像细化(Zhang快速细化算法)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int[, ] srcBytes = new int[w, h];
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcBytes[i, j] = (tempMask[i * 4 + j * w * 4] * 0.114 + tempMask[i * 4 + 1 + j * w * 4] * 0.587 + tempMask[i * 4 + 2 + j * w * 4] * 0.299 < 128 ? 0 : 1);
			}
		}
		Thinning(ref srcBytes, w, h);
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcBytes[i, j] * 255);
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
private static void Thinning(ref int[, ] srcBytes, int w, int h)
{
	int[] srcTemp;
	int countNumber;
	do
	{
		countNumber = 0;
		for (int y = 1; y < h - 1; y++)
		{
			for (int x = 1; x < w - 1; x++)
			{
				srcTemp = new int[9]{ srcBytes[x, y], srcBytes[x - 1, y - 1], srcBytes[x, y - 1], srcBytes[x + 1, y - 1], srcBytes[x + 1, y], srcBytes[x + 1, y + 1], srcBytes[x, y + 1], srcBytes[x - 1, y + 1], srcBytes[x - 1, y] };
				if (srcBytes[x, y] != 1)
				{
					if (CountN(srcTemp) >= 2 && CountN(srcTemp) <= 6)
					{
						if (CountT(srcTemp) == 1)
						{
							if (srcBytes[x, y - 1] * srcBytes[x + 1, y] * srcBytes[x, y + 1] == 0)
							{
								if (srcBytes[x - 1, y] * srcBytes[x + 1, y] * srcBytes[x, y + 1] == 0)
								{
									srcBytes[x, y] = (byte)1;
									countNumber++;
								}
							}
							else
							{
								if (srcBytes[x, y - 1] * srcBytes[x + 1, y] * srcBytes[x - 1, y] == 0)
								{
									if (srcBytes[x, y - 1] * srcBytes[x, y + 1] * srcBytes[x - 1, y] == 0)
									{
										srcBytes[x, y] = (byte)1;
										countNumber++;
									}
								}
							}
						}
					}
				}
			}
		}
	} while (countNumber != 0);
}
private static int CountN(params int[] src)
{
	int count = 0;
	for (int i = 0; i < src.Length; i++)
	{
		if (src[i] == 0)
		{
			count++;
		}
	}
	return count;
}
private static int CountT(params int[] src)
{
	int count = 0;
	for (int i = 1; i < src.Length; i++)
	{
		if (src[i] == 1 && src[i - 1] == 0)
		{
			count++;
		}
	}
	if (src[src.Length - 1] == 0 && src[0] == 1)
	{
		count++;
	}
	return count;
}
```

![img](https://img-blog.csdn.net/20150417133359248?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.50图像运动模糊

﻿﻿

**[函数名称]**

  图像运动模糊算法    MotionblurProcess(WriteableBitmap src,int k,int direction)

**[算法说明]**

  运动模糊是指在摄像机获取图像时,由于景物和相机之间的相对运动而造成的图像上的模糊。这里

我们主要介绍匀速直线运动所造成的模糊，由于非匀速直线运动在某些条件下可以近似为匀速直线

运动，或者可以分解为多个匀速直线运动的合成，因此，在摄像机较短的图像曝光时间内，造成图

像模糊的运动情况可以近似为匀速直线运动。

  对于匀速直线运动，图像的运动模糊可以用以下公式表示：

![img](https://img-blog.csdn.net/20150417133609929?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20150417133645685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
	 /// Motion blur process.
	 /// </summary>
	 /// <param name="src">The source image.</param>
	 /// <param name="k">The offset of motion, from 0 to 200.</param>
	 /// <param name="direction">The direction of motion, x:1, y:2.</param>
	 /// <returns></returns>
	public static WriteableBitmap MotionblurProcess(WriteableBitmap src, int k, int direction)////运动模糊处理
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int b, g, r;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = g = r = 0;
				switch (direction)
				{
				case 1:
					if (x >= k)
					{
						for (int i = 0; i <= k; i++)
						{
							b += (int)tempMask[(x - i) * 4 + y * w * 4];
							g += (int)tempMask[(x - i) * 4 + 1 + y * w * 4];
							r += (int)tempMask[(x - i) * 4 + 2 + y * w * 4];
						}
						temp[x * 4 + y * w * 4] = (byte)(b / (k + 1));
						temp[x * 4 + 1 + y * w * 4] = (byte)(g / (k + 1));
						temp[x * 4 + 2 + y * w * 4] = (byte)(r / (k + 1));
					}
					else
					{
						if (x > 0)
						{
							for (int i = 0; i < x; i++)
							{
								b += (int)tempMask[(x - i) * 4 + y * w * 4];
								g += (int)tempMask[(x - i) * 4 + 1 + y * w * 4];
								r += (int)tempMask[(x - i) * 4 + 2 + y * w * 4];
							}
							temp[x * 4 + y * w * 4] = (byte)(b / (x + 1));
							temp[x * 4 + 1 + y * w * 4] = (byte)(g / (x + 1));
							temp[x * 4 + 2 + y * w * 4] = (byte)(r / (x + 1));
						}
						else
						{
							temp[x * 4 + y * w * 4] = (byte)(tempMask[x * 4 + y * w * 4] / k);
							temp[x * 4 + 1 + y * w * 4] = (byte)(tempMask[x * 4 + 1 + y * w * 4] / k);
							temp[x * 4 + 2 + y * w * 4] = (byte)(tempMask[x * 4 + 2 + y * w * 4] / k);
						}
					}
					break;
				case 2:
					if (y >= k)
					{
						for (int i = 0; i <= k; i++)
						{
							b += (int)tempMask[x * 4 + (y - i) * w * 4];
							g += (int)tempMask[x * 4 + 1 + (y - i) * w * 4];
							r += (int)tempMask[x * 4 + 2 + (y - i) * w * 4];
						}
						temp[x * 4 + y * w * 4] = (byte)(b / (k + 1));
						temp[x * 4 + 1 + y * w * 4] = (byte)(g / (k + 1));
						temp[x * 4 + 2 + y * w * 4] = (byte)(r / (k + 1));
					}
					else
					{
						if (y > 0)
						{
							for (int i = 0; i < y; i++)
							{
								b += (int)tempMask[x * 4 + (y - i) * w * 4];
								g += (int)tempMask[x * 4 + 1 + (y - i) * w * 4];
								r += (int)tempMask[x * 4 + 2 + (y - i) * w * 4];
							}
							temp[x * 4 + y * w * 4] = (byte)(b / (y + 1));
							temp[x * 4 + 1 + y * w * 4] = (byte)(g / (y + 1));
							temp[x * 4 + 2 + y * w * 4] = (byte)(r / (y + 1));
						}
						else
						{
							temp[x * 4 + y * w * 4] = (byte)(tempMask[x * 4 + y * w * 4] / k);
							temp[x * 4 + 1 + y * w * 4] = (byte)(tempMask[x * 4 + 1 + y * w * 4] / k);
							temp[x * 4 + 2 + y * w * 4] = (byte)(tempMask[x * 4 + 2 + y * w * 4] / k);
						}
					}
					break;
				default:
					break;
				}
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150417133744653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.51图像统计滤波算法


**[函数名称]**

  图像统计滤波   WriteableBitmap StatisticalFilter(WriteableBitmap src,double T)

![img](https://img-blog.csdn.net/20150418092915678?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
	/// Statistical filtering process.
	/// </summary>
	/// <param name="src">The source image.</param>
	/// <param name="T">The threshould to adjust filter effect.</param>
	/// <returns></returns>
	public static WriteableBitmap StatisticalFilter(WriteableBitmap src, double T)////图像统计滤波
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double mean = 0;
		double std = 0;
		int[] windowValue = new int[9];
		double mR = 0;
		double mG = 0;
		double mB = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				windowValue[0] = (int)(tempMask[(i - 1) * 4 + (j - 1) * w * 4] * 0.114 + tempMask[(i - 1) * 4 + 1 + (j - 1) * w * 4] * 0.587 + tempMask[(i - 1) * 4 + 2 + (j - 1) * w * 4] * 0.299);
				windowValue[1] = (int)(tempMask[i * 4 + (j - 1) * w * 4] * 0.114 + tempMask[i * 4 + 1 + (j - 1) * w * 4] * 0.587 + tempMask[i * 4 + 2 + (j - 1) * w * 4] * 0.299);
				windowValue[2] = (int)(tempMask[(i + 1) * 4 + (j - 1) * w * 4] * 0.114 + tempMask[(i + 1) * 4 + 1 + (j - 1) * w * 4] * 0.587 + tempMask[(i + 1) * 4 + 2 + (j - 1) * w * 4] * 0.299);
				windowValue[3] = (int)(tempMask[(i - 1) * 4 + j * w * 4] * 0.114 + tempMask[(i - 1) * 4 + 1 + j * w * 4] * 0.587 + tempMask[(i - 1) * 4 + 2 + j * w * 4] * 0.299);
				windowValue[4] = (int)(tempMask[i * 4 + j * w * 4] * 0.114 + tempMask[i * 4 + 1 + j * w * 4] * 0.587 + tempMask[i * 4 + 2 + j * w * 4] * 0.299);
				windowValue[5] = (int)(tempMask[(i + 1) * 4 + j * w * 4] * 0.114 + tempMask[(i + 1) * 4 + 1 + j * w * 4] * 0.587 + tempMask[(i + 1) * 4 + 2 + j * w * 4] * 0.299);
				windowValue[6] = (int)(tempMask[(i - 1) * 4 + (j + 1) * w * 4] * 0.114 + tempMask[(i - 1) * 4 + 1 + (j + 1) * w * 4] * 0.587 + tempMask[(i - 1) * 4 + 2 + (j + 1) * w * 4] * 0.299);
				windowValue[7] = (int)(tempMask[i * 4 + (j + 1) * w * 4] * 0.114 + tempMask[i * 4 + 1 + (j + 1) * w * 4] * 0.587 + tempMask[i * 4 + 2 + (j + 1) * w * 4] * 0.299);
				windowValue[8] = (int)(tempMask[(i + 1) * 4 + (j + 1) * w * 4] * 0.114 + tempMask[(i + 1) * 4 + 1 + (j + 1) * w * 4] * 0.587 + tempMask[(i + 1) * 4 + 2 + (j + 1) * w * 4] * 0.299);
				for (int n = 0; n < 9; n++)
				{
					mean += (double)((double)(windowValue[n]) / 9);
				}
				for (int n = 0; n < 9; n++)
				{
					std += Math.Pow((double)(windowValue[n]) - mean, 2) / 9;
				}
				if (windowValue[4] >= T * Math.Sqrt(std))
				{
					for (int k = -1; k < 2; k++)
					{
						for (int m = -1; m < 2; m++)
						{
							mB += (double)tempMask[(i + m) * 4 + (j + k) * w * 4] / 9;
							mG += (double)tempMask[(i + m) * 4 + 1 + (j + k) * w * 4] / 9;
							mR += (double)tempMask[(i + m) * 4 + 2 + (j + k) * w * 4] / 9;
						}
					}
					temp[i * 4 + j * w * 4] = (byte)mB;
					temp[i * 4 + 1 + j * w * 4] = (byte)mG;
					temp[i * 4 + 2 + j * w * 4] = (byte)mR;
				}
				mean = 0;
				std = 0;
				mR = mG = mB = 0;
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150418093012306?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.52图像K均值聚类


**[函数名称]**

  图像KMeans聚类      KMeansCluster(WriteableBitmap src,int k)

![img](https://img-blog.csdn.net/20150418093158340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

​```csharp
/// <summary>
	/// KMeans Cluster process.
	/// </summary>
	/// <param name="src">The source image.</param>
	/// <param name="k">Cluster threshould, from 2 to 255.</param>
	/// <returns></returns>
	public static WriteableBitmap KMeansCluster(WriteableBitmap src, int k)////KMeansCluster
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int b = 0, g = 0, r = 0;
		//定义灰度图像信息存储变量
		byte[] imageData = new byte[w * h];
		//定义聚类均值存储变量(存储每一个聚类的均值)
		double[] meanCluster = new double[k];
		//定义聚类标记变量(标记当前像素属于哪一类)
		int[] markCluster = new int[w * h];
		//定义聚类像素和存储变量(存储每一类像素值之和)
		double[] sumCluster = new double[k];
		//定义聚类像素统计变量(存储每一类像素的数目)
		int[]countCluster = new int[k];
		//定义聚类RGB分量存储变量(存储每一类的RGB三分量大小)
		int[] sumR = new int[k];
		int[] sumG = new int[k];
		int[] sumB = new int[k];
		//临时变量
		int sum = 0;
		//循环控制变量
		bool s = true;
		double[] mJduge = new double[k];
		double tempV = 0;
		int cou = 0;
		//获取灰度图像信息
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				b = tempMask[i * 4 + j * w * 4];
				g = tempMask[i * 4 + 1 + j * w * 4];
				r = tempMask[i * 4 + 2 + j * w * 4];
				imageData[i + j * w] = (byte)(b * 0.114 + g * 0.587 + r * 0.299);
			}
		}
		while (s)
		{
			sum = 0;
			//初始化聚类均值
			for (int i = 0; i < k; i++)
			{
				meanCluster[i] = i * 255.0 / (k - 1);
			}
			//计算聚类归属
			for (int i = 0; i < imageData.Length; i++)
			{
				tempV = Math.Abs((double)imageData[i] - meanCluster[0]);
				cou = 0;
				for (int j = 1; j < k; j++)
				{
					double t = Math.Abs((double)imageData[i] - meanCluster[j]);
					if (tempV > t)
					{
						tempV = t;
						cou = j;
					}
				}
				countCluster[cou]++;
				sumCluster[cou] += (double)imageData[i];
				markCluster[i] = cou;
			}
			//更新聚类均值
			for (int i = 0; i < k; i++)
			{
				meanCluster[i] = sumCluster[i] / (double)countCluster[i];
				sum += (int)(meanCluster[i] - mJduge[i]);
				mJduge[i] = meanCluster[i];
			}
			if (sum == 0)
			{
				s = false;
			}
		}
		//计算聚类RGB分量
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				sumB[markCluster[i + j * w]] += tempMask[i * 4 + j * w * 4];
				sumG[markCluster[i + j * w]] += tempMask[i * 4 + 1 + j * w * 4];
				sumR[markCluster[i + j * w]] += tempMask[i * 4 + 2 + j * w * 4];
			}
		}
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * 4 * w] = (byte)(sumB[markCluster[i + j * w]] / countCluster[markCluster[i + j * w]]);
				temp[i * 4 + 1 + j * 4 * w] = (byte)(sumG[markCluster[i + j * w]] / countCluster[markCluster[i + j * w]]);
				temp[i * 4 + 2 + j * 4 * w] = (byte)(sumR[markCluster[i + j * w]] / countCluster[markCluster[i + j * w]]);
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150418093305404?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.53图像傅立叶变换


**[函数名称]**

1,一维FFT变换函数         Complex[] FFT(Complex[] sourceData, int countN)

  2,二维FFT变换函数           Complex[] FFT2(byte[] imageData,bool inv)

  3,图像傅立叶变换幅度函数     WriteableBitmap FFTImage()

  4,图像傅立叶变换相位函数     WriteableBitmap FFTPhaseImage()

**[算法说明]**

  **关于傅立叶变换这一小节，由于算法较为复杂，因此在后面专门介绍，这里略去。**

**[函数代码]**

  **代码包括两个类库：1,复数运算类Complex.cs   2,FFT变换类DFT.cs**

```csharp
1.Complex.cs
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Win8ImageProcess
{
    class Complex
    {
        //复数实部
        private double real = 0.0;
        //复数虚部
        private double imaginary = 0.0;

        public double Real
        {
            get
            {
                return real;
            }
            set
            {
                real = value;
            }
        }

        public double Imaginary
        {
            get
            {
                return imaginary;
            }
            set
            {
                imaginary = value;
            }
        }

        public Complex()
        {
        }

        public Complex(double dbreal, double dbimag)
        {
            real = dbreal;
            imaginary = dbimag;
        }

        public Complex(Complex other)
        {
            real = other.real;
            imaginary = other.imaginary;
        }
        //复数加运算
        public static Complex operator +(Complex comp1, Complex comp2)
        {
            return comp1.Add(comp2);
        }
        //复数减运算
        public static Complex operator -(Complex comp1, Complex comp2)
        {
            return comp1.Subtract(comp2);
        }
        //复数乘运算
        public static Complex operator *(Complex comp1, Complex comp2)
        {
            return comp1.Multiply(comp2);
        }

        public Complex Add(Complex comp)
        {
            double x = real + comp.real;
            double y = imaginary + comp.imaginary;

            return new Complex(x, y);
        }

        public Complex Subtract(Complex comp)
        {
            double x = real - comp.real;
            double y = imaginary - comp.imaginary;

            return new Complex(x, y);
        }

        public Complex Multiply(Complex comp)
        {
            double x = real * comp.real - imaginary * comp.imaginary;
            double y = real * comp.imaginary + imaginary * comp.real;

            return new Complex(x, y);
        }
        //幅值
        public double Abs()
        {
            double x = Math.Abs(real);
            double y = Math.Abs(imaginary);

            if (real == 0)
            {
                return y;
            }
            if (imaginary == 0)
            {
                return x;
            }

            if (x > y)
            {
                return (x * Math.Sqrt(1 + (y / x) * (y / x)));
            }
            else
            {
                return (y * Math.Sqrt(1 + (x / y) * (x / y)));
            }
        }
        //相位角
        public double Angle()
        {
            if (real == 0 && imaginary == 0)
                return 0;

            if (real == 0)
            {
                if (imaginary > 0)
                    return Math.PI / 2;
                else
                    return -Math.PI / 2;
            }
            else
            {
                if (real > 0)
                    return Math.Atan2(imaginary, real);
                else
                {
                    if (imaginary >= 0)
                        return Math.Atan2(imaginary, real) + Math.PI;
                    else
                        return Math.Atan2(imaginary, real) - Math.PI;
                }
            }
        }
        //共轭
        public Complex Conjugate()
        {
            return new Complex(this.real, -this.imaginary);
        }
    }
}
2.DFT.cs
using System;
using System.Collections.Generic;
using System.Text;
using System.Threading.Tasks;
using System.IO;
using Windows.UI.Xaml.Media.Imaging;
using System.Runtime.InteropServices.WindowsRuntime;


namespace Win8ImageProcess
{
    public sealed class DFTClass
    {
        private static WriteableBitmap srcImage;
        private static byte[] imageData;
        private static int w = 0;
        private static int h = 0;

        public DFTClass(WriteableBitmap image)
        {
            srcImage = image;
            w = image.PixelWidth;
            h = image.PixelHeight;
        }
        //一维FFT变换
        private Complex[] FFT(Complex[] sourceData, int countN)
        {
            //求fft的级数
            int r = Convert.ToInt32(Math.Log(countN, 2));
            //定义加权系数W
            Complex[] w = new Complex[countN / 2];
            Complex[] interVar1 = new Complex[countN];
            Complex[] interVar2 = new Complex[countN];
            interVar1 = (Complex[])sourceData.Clone();
            //求取加权系数W
            double angle = 0;
            for (int i = 0; i < countN / 2; i++)
            {
                angle = -i * Math.PI * 2 / countN;
                w[i] = new Complex(Math.Cos(angle), Math.Sin(angle));
                angle = 0;
            }
            int interval = 0;
            int halfN = 0;
            int gap = 0;
            //核心部分
            for (int i = 0; i < r; i++)
            {
                interval = 1 << i;
                halfN = 1 << (r - i);
                for (int j = 0; j < interval; j++)
                {
                    gap = j * halfN;
                    for (int k = 0; k < halfN / 2; k++)
                    {
                        interVar2[k + gap] = interVar1[k + gap] + interVar1[k + gap + halfN / 2];
                        interVar2[k + halfN / 2 + gap] = (interVar1[k + gap] - interVar1[k + gap + halfN / 2]) * w[k * interval];
                    }
                }
                interVar1 = (Complex[])interVar2.Clone();
            }
            uint rev = 0;
            uint num = 0;
            for (uint j = 0; j < countN; j++)
            {
                rev = 0;
                num = j;
                for (int i = 0; i < r; i++)
                {
                    rev <<= 1;
                    rev |= num & 1;
                    num >>= 1;
                }
                interVar2[rev] = interVar1[j];
            }
            return interVar2;
        }
        //二维fft变换
        private Complex[] FFT2(byte[] imageData,bool inv)
        {
            int bytes = w * h;
            byte[] bmpValues = new byte[bytes];
            Complex[] tempCom1 = new Complex[bytes];
            bmpValues = (byte[])imageData.Clone();
            for (int i = 0; i < bytes; i++)
            {
                if (inv == true)
                {
                    if ((i / w + i % w) % 2 == 0)
                    {
                        tempCom1[i] = new Complex(bmpValues[i], 0);
                    }
                    else
                    {
                        tempCom1[i] = new Complex(-bmpValues[i], 0);
                    }
                }
                else
                {
                    tempCom1[i] = new Complex(bmpValues[i], 0);
                }
            }
            Complex[] tempCom2 = new Complex[w];
            Complex[] tempCom3 = new Complex[w];
            for (int i = 0; i < h; i++)//水平方向
            {
                for (int j = 0; j < w; j++)
                {
                    tempCom2[j] = tempCom1[i * w + j];
                }
                tempCom3 = FFT(tempCom2, w);
                for (int j = 0; j < w; j++)
                {
                    tempCom1[i * w + j] = tempCom3[j];
                }
            }
            Complex[] tempCom4 = new Complex[h];
            Complex[] tempCom5 = new Complex[h];
            for (int i = 0; i < w; i++)//垂直方向
            {
                for (int j = 0; j < h; j++)
                {
                    tempCom4[j] = tempCom1[j * w + i];
                }
                tempCom5 = FFT(tempCom4, h);
                for (int j = 0; j < h; j++)
                {
                    tempCom1[j * w + i] = tempCom5[j];
                }
            }
            return tempCom1;
        }
        /// <summary>
        /// 图像FFT幅度图函数
        /// </summary>
        /// <returns></returns>
        public WriteableBitmap FFTImage()
        {
            WriteableBitmap grayImage = new WriteableBitmap(w, h);
            byte[] temp = srcImage.PixelBuffer.ToArray();
            imageData = new byte[w * h];
            byte tempByte = 0;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    tempByte = (byte)((int)(temp[i * 4 + j * w * 4] * 0.114 + temp[i * 4 + 1 + j * w * 4] * 0.587 + temp[i * 4 + 2 + j * w * 4] * 0.299));
                    imageData[i + j * w] = tempByte;
                }
            }
            int bytes = w * h;
            Complex[] freDom = new Complex[bytes];
            double[] tempArray = new double[bytes];
            freDom = FFT2(imageData, true);
            for (int i = 0; i < bytes; i++)
            {
                tempArray[i] = Math.Log(1 + freDom[i].Abs(), 2);
            }
            //灰度级拉伸
            double a = 1000.0, b = 0.0;
            double p;
            for (int i = 0; i < bytes; i++)
            {
                if (a > tempArray[i])
                {
                    a = tempArray[i];
                }
                if (b < tempArray[i])
                {
                    b = tempArray[i];
                }
            }
            p = 255.0 / (b - a);
            for (int i = 0; i < bytes; i++)
            {
                imageData[i] = (byte)(p * (tempArray[i] - a) + 0.5);
            }
            byte[] dstData = new byte[w * h * 4];
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    dstData[i * 4 + j * w * 4] = (byte)imageData[i + j * w];
                    dstData[i * 4 + 1 + j * w * 4] = (byte)imageData[i + j * w];
                    dstData[i * 4 + 2 + j * w * 4] = (byte)imageData[i + j * w];
                }
            }
            Stream sTemp = grayImage.PixelBuffer.AsStream();
            sTemp.Seek(0, SeekOrigin.Begin);
            sTemp.Write(dstData, 0, w * 4 * h);
            return grayImage;
        }
        /// <summary>
        /// 图像FFT相位图函数
        /// </summary>
        /// <returns></returns>
        public WriteableBitmap FFTPhaseImage()
        {
            WriteableBitmap grayImage = new WriteableBitmap(w, h);
            byte[] temp = srcImage.PixelBuffer.ToArray();
            imageData = new byte[w * h];
            byte tempByte = 0;
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    tempByte = (byte)((int)(temp[i * 4 + j * w * 4] * 0.114 + temp[i * 4 + 1 + j * w * 4] * 0.587 + temp[i * 4 + 2 + j * w * 4] * 0.299));
                    imageData[i + j * w] = tempByte;
                }
            }
            int bytes = w * h;
            Complex[] freDom = new Complex[bytes];
            double[] tempArray = new double[bytes];
            freDom = FFT2(imageData, true);
            for (int i = 0; i < bytes; i++)
            {
                tempArray[i] = freDom[i].Angle() + 2 * Math.PI;
            }
            //灰度级拉伸
            double a = 1000.0, b = 0.0;
            double p;
            for (int i = 0; i < bytes; i++)
            {
                if (a > tempArray[i])
                {
                    a = tempArray[i];
                }
                if (b < tempArray[i])
                {
                    b = tempArray[i];
                }
            }
            p = 255.0 / (b - a);
            for (int i = 0; i < bytes; i++)
            {
                imageData[i] = (byte)(p * (tempArray[i] - a) + 0.5);
            }
            byte[] dstData = new byte[w * h * 4];
            for (int j = 0; j < h; j++)
            {
                for (int i = 0; i < w; i++)
                {
                    dstData[i * 4 + j * w * 4] = (byte)imageData[i + j * w];
                    dstData[i * 4 + 1 + j * w * 4] = (byte)imageData[i + j * w];
                    dstData[i * 4 + 2 + j * w * 4] = (byte)imageData[i + j * w];
                }
            }
            Stream sTemp = grayImage.PixelBuffer.AsStream();
            sTemp.Seek(0, SeekOrigin.Begin);
            sTemp.Write(dstData, 0, w * 4 * h);
            return grayImage;
        }
    }
}
```

![img](https://img-blog.csdn.net/20150420125354715?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.54迭代法图像二值化

﻿﻿

**[函数名称]**

  迭代法图像二值化      int IterativeThSegment(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150420125712312?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
 /// Iterative method of image segmention.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <returns></returns>
	public static WriteableBitmap IterativeThSegment(WriteableBitmap src) ////迭代法阈值分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		//定义灰度图像信息存储变量
		int[] srcData = new int[w * h];
		//定义背景和目标像素个数变量C1,C2，总体灰度和变量sum
		int C1 = 0, C2 = 0, sum = 0;
		//定义背景和目标的灰度和变量G1,G2，前后两次灰度均值变量t0,t
		double G1 = 0, G2 = 0, t0 = 256, t = 0;
		//定义阈值变量
		int Th = 0;
		//定义循环控制变量
		bool s = true;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcData[i + j * w] = (int)((double)tempMask[i * 4 + j * w * 4] * 0.114 + (double)tempMask[i * 4 + 1 + j * w * 4] * 0.587 + (double)tempMask[i * 4 + 2 + j * w * 4] * 0.299);
				sum += srcData[i + j * w];
			}
		}
		//初始化阈值
		Th = sum / (w*h);
		while (s)
		{
			for (int i = 0; i < srcData.Length; i++)
			{
				if (srcData[i] < Th)
				{
					C1++;
					G1 += srcData[i];
				}
				else
				{
					C2++;
					G2 += srcData[i];
				}
			}
			t = (double)((G1 / C1 + G2 / C2) / 2);
			if (Math.Abs(t - t0) < 1)
			{
				s = false;
				for (int j = 0; j < h; j++)
				{
					for (int i = 0; i < w; i++)
					{
						temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcData[i + j * w] < Th ? 0 : 255);
					}
				}
			}
			else
			{
				t0 = t;
				C1 = 0; C2 = 0; G1 = 0; G2 = 0;
				//更新阈值
				Th = (int)t;
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}

```

[图像效果]

![img](https://img-blog.csdn.net/20150420125932057?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.55OSTU法图像二值化

﻿﻿

**[函数名称]**

  Ostu法图像二值化      WriteableBitmap OstuThSegment(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150421124553926?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
/// Ostu method of image segmention.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap OstuThSegment(WriteableBitmap src) ////Ostu法阈值分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		//定义灰度图像信息存储变量
		int[] srcData = new int[w * h];
		//定义阈值变量
		int Th = 0; ;
		//定义背景和目标像素数目变量N1,N2，灰度变量U1,U2，灰度和变量Sum1,Sum2，临时缓存变量Temp
		int N1 = 0, N2 = 0, Sum1 = 0, Sum2 = 0;
		//定义背景和目标像素比例变量W1,W2，图像整体平均灰度变量U，方差变量g，对比阈值变量TT
		double W1 = 0, W2 = 0, U1 = 0, U2 = 0, g = 0, TT = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcData[i + j * w] = (int)((double)tempMask[i * 4 + j * w * 4] * 0.114 + (double)tempMask[i * 4 + 1 + j * w * 4] * 0.587 + (double)tempMask[i * 4 + 2 + j * w * 4] * 0.299);
			}
		}
		//寻找最大类间方差
		for (int T = 0; T <= 255; T++)
		{
			for (int i = 0; i < srcData.Length; i++)
			{
				if (srcData[i] > T)
				{
					N2++;
					Sum2 += srcData[i];
				}
				else
				{
					N1++;
					Sum1 += srcData[i];
				}
			}
			W1 = (double)(N1 / (N1 + N2));
			W2 = (double)(1.0 - W1);
			U1 = (N1 == 0 ? 0.0 : (Sum1 / N1));
			U2 = (N2 == 0 ? 0.0 : (Sum2 / N2));
			g = N1 * N2 * (U1 - U2) * (U1 - U2);
			if (g > TT)
			{
				TT = g;
				Th = T;
			}
			N1 = 0; N2 = 0;
			Sum1 = 0; Sum2 = 0; W1 = 0.0; W2 = 0.0; U1 = 0.0; U2 = 0.0; g = 0.0;
		}
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcData[i + j * w] < Th ? 0 : 255);
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150421124707324?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.56简单统计法图像二值化


**[函数名称]**

  简单统计法图像二值化 WriteableBitmap StatisticalThSegment(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150421124932410?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
  /// Statistical method of image segmention.
  /// </summary>
  /// <param name="src">The source image.</param>
  /// <returns></returns>
	public static WriteableBitmap StatisticalThSegment(WriteableBitmap src) ////Ostu法阈值分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		//定义灰度图像信息存储变量
		int[] srcData = new int[w * h];
		int eX = 0;
		int eY = 0;
		int sumEF = 0;
		int sumE = 0;
		int eMax = 0;
		//定义阈值变量
		int Th = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcData[i + j * w] = (int)((double)tempMask[i * 4 + j * w * 4] * 0.114 + (double)tempMask[i * 4 + 1 + j * w * 4] * 0.587 + (double)tempMask[i * 4 + 2 + j * w * 4] * 0.299);
			}
		}
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				eX = srcData[i - 1 + j * w] - srcData[i + 1 + j * w];
				eY = srcData[i + (j - 1) * w] - srcData[i + (j + 1) * w];
				eMax = Math.Max(eX, eY);
				sumE += eMax;
				sumEF += eMax * srcData[i + j * w];
			}
		}
		Th = (int)(sumEF / sumE);
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcData[i + j * w] < Th ? 0 : 255);
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150421124905759?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.57一维最大熵法图像二值化

﻿﻿

**[函数名称]**

  一维最大熵法图像二值化WriteableBitmap EntropymaxThSegment(WriteableBitmap src)

**[算法说明]**

  一维最大熵法图像分割就是利用图像的灰度分布密度函数定义图像的信息熵，通过优化一定的熵

准则得到熵最大时对应的阈值，从而进行图像分割的方法。

  算法过程：

  1，对于一幅灰度图像，灰度范围为[0,L-1]，求取图像的最小灰度级min，最大灰度级max；

![img](https://img-blog.csdn.net/20150421125203372?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**

```csharp
/// <summary>
 /// Entropy max method of image segmention.
 /// </summary>
 /// <param name="src">The source iamge.</param>
 /// <returns></returns>
	public static WriteableBitmap EntropymaxThSegment(WriteableBitmap src) ////一维熵最大法阈值分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		//定义灰度图像信息存储变量
		int[] srcData = new int[w * h];
		//定义阈值变量
		int Th = 0;
		//定义直方图存储变量
		int[] histogram = new int[256];
		//定义熵值变量
		double Ht = 0.0;
		double Hl = 0.0;
		double sigma = 0.0;
		//定义灰度最值变量
		int max = 0;
		int min = 255;
		//定义临时变量
		double t = 0.0, pt = 0.0, tempMax = 0.0;
		int tempV = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				tempV = (int)((double)tempMask[i * 4 + j * w * 4] * 0.114 + (double)tempMask[i * 4 + 1 + j * w * 4] * 0.587 + (double)tempMask[i * 4 + 2 + j * w * 4] * 0.299);
				srcData[i + j * w] = tempV;
				histogram[tempV]++;
				if (tempV > max)
				{
					max = tempV;
				}
				if (tempV < min)
				{
					min = tempV;
				}
			}
		}
		for (int i = min; i < max; i++)
		{
			t = (double)((double)histogram[i] / (double)(w * h));
			if (t > 0.00000001)
			{
				Hl += -t * Math.Log10(t);
			}
			else
				continue;
		}
		for (int i = min; i < max; i++)
		{
			t = (double)((double)histogram[i] / (double)(w * h));
			pt += t;
			if (t > 0.00000001)
			{
				Ht += -t * Math.Log10(t);
				sigma = Math.Log10(pt * (1 - pt)) * Ht / pt + (Hl - Ht) / (1 - pt);
				if (sigma > tempMax)
				{
					tempMax = (int)sigma;
					Th = i;
				}
			}
			else
				continue;
		}
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcData[i + j * w] < Th ? 0 : 255);
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150421125154505?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.58双峰法图像二值化

﻿﻿

**[函数名称]**

  双峰法图像二值化 WriteableBitmap  PeakshistogramThSegment(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150421125322894?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// Peaks histogram method of image segmention.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap  PeakshistogramThSegment(WriteableBitmap src) ////双峰法阈值分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		//定义灰度图像信息存储变量
		int[] srcData = new int[w * h];
		//定义直方图存取变量
		int[] histValues = new int[256];
		//定义双峰位置变量h1,h2，对应的灰度变量t1,t2,谷底灰度变量t
		int h1 = 0, h2 = 0, t1 = 0, t2 = 0, t = 255;
		//定义阈值变量
		int Th = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcData[i + j * w] = (int)((double)tempMask[i * 4 + j * w * 4] * 0.114 + (double)tempMask[i * 4 + 1 + j * w * 4] * 0.587 + (double)tempMask[i * 4 + 2 + j * w * 4] * 0.299);
				histValues[srcData[i + j * w]]++;
			}
		}
		for (int i = 0; i < 256; i++)
		{
			if (i < 129)
			{
				if (histValues[i] > t1)
				{
					h1 = i;
					t1 = histValues[i];
				}
			}
			else
			{
				if (histValues[i] > t2)
				{
					h2 = i;
					t2 = histValues[i];
				}
			}
		}
		for (int n = h1; n <= h2; n++)
		{
			if (histValues[n] < t)
			{
				Th = n;
				t = histValues[n];
			}
		}
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcData[i + j * w] < Th ? 0 : 255);
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150421125550196?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.59 P分位法图像二值化

﻿﻿

**[函数名称]**

  P分位法图像二值化

**[算法说明]**

  所谓P分位法图像分割，就是在知道图像中目标所占的比率Ratio时，循环不同的灰度值对图像进行

分割，并计算对应的目标所占的比率，如果该比率与Ratio的差值足够小，那么该阈值就是所求的最

佳分割阈值。

![img](https://img-blog.csdn.net/20150421125630609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// P-Parameter method of image segmention.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="P">The ratio of object, from 0 to 1.</param>
/// <returns></returns>
public static WriteableBitmap PParameterThSegment(WriteableBitmap src, double P) ////P参数法阈值分割
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap dstImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		//定义灰度图像信息存储变量
		int[] srcData = new int[w * h];
		//定义背景和目标像素个数变量
		int C1 = 0, C2 = 0;
		//定义阈值变量
		int Th = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				srcData[i + j * w] = (int)((double)tempMask[i * 4 + j * w * 4] * 0.114 + (double)tempMask[i * 4 + 1 + j * w * 4] * 0.587 + (double)tempMask[i * 4 + 2 + j * w * 4] * 0.299);
			}
		}
		for (int T = 0; T <= 255; T++)
		{
			for (int i = 0; i < srcData.Length; i++)
			{
				if (srcData[i] > T)
				{
					C1++;
				}
				else
				{
					C2++;
				}
			}
			double t = Math.Abs((double)((double)C1 / ((double)C1 + (double)C2)) - P);
			if (t < 0.01)
			{
				Th = T;
				break;
			}
			C1 = 0;
			C2 = 0;
		}
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)(srcData[i + j * w] < Th ? 0 : 255);
			}
		}
		Stream sTemp = dstImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return dstImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150421125729297?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.60部分彩色保留算法

﻿﻿

**[函数名称]**

  部分彩色保留函数       WriteableBitmap PartialcolorProcess(WriteableBitmap src,int rValue,int gValue,int bValue)

**[算法说明]**

  这个算法有多种，可以基于RGB，HSV等多种颜色空间来实现，本文介绍最简单的基于RGB颜色空间的彩色保留算法。

  对于我们需要保留的像素P(x,y)，它的RGB值分别为R(x,y),G(x,y),B(x,y)，各个分量之间的关系有如下几种：

  1--R>G>B;2—R>B>G;3—B>R>G;4—B>G>R;5—G>R>B;6—G>B>R;

  对于当前像素f(x,y)：

  如果该像素的RGB分量关系情况与P(x,y)像素的分量关系情况相同，则该像素颜色保留，否则，将该像素做灰度化处理。

  这样，就可以保留我们需要的颜色像素了。

 **[函数代码]**

```csharp
/// <summary>
/// Partial color process.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="rValue">R channel of pixel.</param>
/// <param name="gValue">G channel of pixel.</param>
/// <param name="bValue">B channel of pixel.</param>
/// <returns></returns>
public static WriteableBitmap PartialcolorProcess(WriteableBitmap src, int rValue, int gValue, int bValue)////部分彩色
  {
	  if (src != null)
	  {
		  int w = src.PixelWidth;
		  int h = src.PixelHeight;
		  WriteableBitmap srcImage = new WriteableBitmap(w, h);
		  byte[] temp = src.PixelBuffer.ToArray();
		  byte[] tempMask = (byte[])temp.Clone();
		  int r = 0, g = 0, b = 0;
		  int grayValue = 0;
		  for (int j = 0; j < h; j++)
		  {
			  for (int i = 0; i < w; i++)
			  {
				  b = tempMask[i * 4 + j * w * 4];
				  g = tempMask[i * 4 + 1 + j * w * 4];
				  r = tempMask[i * 4 + 2 + j * w * 4];
				  if (((r > b && b > g && rValue > gValue && gValue > bValue) || (r > g && g > b && rValue > gValue && gValue > bValue) || (g > r && r > b && gValue > rValue && rValue > bValue)
					  || (g > b && b > r && gValue > bValue && bValue > rValue) || (b > g && g > r && bValue > gValue && gValue > rValue) || (b > r && r > g && bValue > rValue && rValue > gValue)))
				  {
				  }
				  else
				  {
					  grayValue = (int)((tempMask[i * 4 + j * w * 4] + tempMask[i * 4 + 1 + j * w * 4] + tempMask[i * 4 + 2 + j * w * 4]) / 3);
					  temp[i * 4 + j * w * 4] = temp[i * 4 + 1 + j * w * 4] = temp[i * 4 + 2 + j * w * 4] = (byte)grayValue;
				  }
			  }
		  }
		  Stream sTemp = srcImage.PixelBuffer.AsStream();
		  sTemp.Seek(0, SeekOrigin.Begin);
		  sTemp.Write(temp, 0, w * 4 * h);
		  return srcImage;
	  }
	  else
	  {
		  return null;
	  }
  }
```

![img](https://img-blog.csdn.net/20150422123355195?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.61哈哈镜效果


**[函数名称]**

  哈哈镜效果函数  WriteableBitmap DistortingMirrorProcess(WriteableBitmap src, int x, int y)

**[算法说明]**

  哈哈镜效果图像实际上是一种图像形变特效而已，对于这个特效，我们可以通过三角变换来实现。

  1，对于哈哈镜效果变换，首先它有两个参数，原点坐标和特效影响因子。

  对于图像中的像素点P(x,y)，假设原点坐标为X，Y，那么，根据三角函数变换可以得到：

  当前像素P的相对坐标cX,cY:

![img](https://img-blog.csdn.net/20150422123413425?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Distorting mirror process.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="x">Origin of coordinates in x direction.</param>
/// <param name="y">Origin of coordinates in y direction.</param>
/// <returns></returns>
	public static WriteableBitmap DistortingMirrorProcess(WriteableBitmap src, int x, int y)////哈哈镜特效
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		int radius = 0;
		double theta = 0;
		int tX = 0;
		int tY = 0;
		int mapX = 0;
		int mapY = 0;
		int mapR = 0;
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				tX = i - x;
				tY = j - y;
				theta = Math.Atan2((double)tY, (double)tX);
				radius = (int)Math.Sqrt((double)(tX * tX + tY * tY));
				mapR = (int)(Math.Sqrt((double)radius * 100));
				mapX = x + (int)(mapR * Math.Cos(theta));
				mapY = y + (int)(mapR * Math.Sin(theta));
				temp[i * 4 + j * w * 4] = tempMask[mapX * 4 + mapY * w * 4];
				temp[i * 4 + 1 + j * w * 4] = tempMask[mapX * 4 + 1 + mapY * w * 4];
				temp[i * 4 + 2 + j * w * 4] = tempMask[mapX * 4 + 2 + mapY * w * 4];
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150422123625033?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.62图像对数增强

﻿﻿

**[函数名称]**

  对数增强      WriteableBitmap LogenhanceProcess(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150424124141526?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Logarithm enhancement process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap LogenhanceProcess(WriteableBitmap src)////对数增强
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		for (int i = 0; i < tempMask.Length; i++)
		{
			temp[i] = (byte)(255.0 / Math.Log(256) * Math.Log(tempMask[i] + 1));
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150424124232647?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.63图像指数增强


**[函数名称]**

  指数增强      WriteableBitmap IndexenhanceProcess(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150424124552553?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**[函数代码]**



```csharp
/// <summary>
/// Enhanced index process.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap IndexenhanceProcess(WriteableBitmap src)////指数增强
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		for (int i = 0; i < tempMask.Length; i++)
		{
			temp[i] = (byte)((1.0 / 255.0) * (double)(tempMask[i] * tempMask[i]));
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```



**[图像效果]**

![img](https://img-blog.csdn.net/20150424124539161?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.64图像高斯滤波算法


**[函数名称]**

  高斯平滑滤波器      GaussFilter(WriteableBitmap src,int radius,double sigma)

**[算法说明]**

  高斯滤波器实质上是一种信号的滤波器，其用途是信号的平滑处理。它是一类根据高斯函数的

形状来选择权重的线性平滑滤波器，该滤波器对于抑制服从正态分布的噪声非常有效。高斯函数

的公式如下所示：

![img](https://img-blog.csdn.net/20150424125223676?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







```csharp
private static double[, ] GaussFuc(int r, double sigma)
{
	int size = 2 * r + 1;
	double[, ] gaussResult = new double[size, size];
	double k = 0.0;
	for (int y = -r, h = 0; y <= r; y++, h++)
	{
		for (int x = -r, w = 0; x <= r; x++, w++)
		{
			gaussResult[w, h] = (1.0 / (2.0 * Math.PI * sigma * sigma)) * (Math.Exp(-((double)x * (double)x + (double)y * (double)y) / (2.0 * sigma * sigma)));
			k += gaussResult[w, h];
		}
	}
	return gaussResult;
}
```



我们设置参数r=1,sigma=1.0，则得到一个3*3的高斯模板如下：

![img](https://img-blog.csdn.net/20150424125749795?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这个公式可以理解为先对图像按行进行一次一维高斯滤波，在对结果图像按列进行一次一维高斯滤波，这样速度将大大提高。

一维高斯滤波代码如下(包含归一化)：



```csharp
private static double[] GaussKernel1D(int r, double sigma)
{
	double[] filter = new double[2 * r + 1];
	double sum = 0.0;
	for (int i = 0; i < filter.Length; i++)
	{
		filter[i] = Math.Exp((double)(-(i - r) * (i - r)) / (2.0 * sigma * sigma));
		sum += filter[i];
	}
	for (int i = 0; i < filter.Length; i++)
	{
		filter[i] = filter[i] / sum;
	}
	return filter;
}
```



**[函数代码]**



```csharp
private static double[] GaussKernel(int radius, double sigma)
{
	int length = 2 * radius + 1;
	double[] kernel = new double[length];
	double sum = 0.0;
	for (int i = 0; i < length; i++)
	{
		kernel[i] = Math.Exp((double)(-(i - radius) * (i - radius)) / (2.0 * sigma * sigma));
		sum += kernel[i];
	}
	for (int i = 0; i < length; i++)
	{
		kernel[i] = kernel[i] / sum;
	}
	return kernel;
}
/// <summary>
/// Gauss filter process
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="radius">The radius of gauss kernel,from 0 to 100.</param>
/// <param name="sigma">The convince of gauss kernel, from 0 to 30.</param>
/// <returns></returns>
public static WriteableBitmap GaussFilter(WriteableBitmap src, int radius, double sigma) ////高斯滤波
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] srcValue = src.PixelBuffer.ToArray();
		byte[] tempValue = (byte[])srcValue.Clone();
		double[] kernel = GaussKernel(radius, sigma);
		double tempB = 0.0, tempG = 0.0, tempR = 0.0;
		int rem = 0;
		int t = 0;
		int v = 0;
		double K = 0.0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				tempB = tempG = tempR = 0.0;
				for (int k = -radius; k <= radius; k++)
				{
					rem = (Math.Abs(x + k) % w);
					t = rem * 4 + y * w * 4;
					K = kernel[k + radius];
					tempB += srcValue[t] * K;
					tempG += srcValue[t + 1] * K;
					tempR += srcValue[t + 2] * K;
				}
				v = x * 4 + y * w * 4;
				tempValue[v] = (byte)tempB;
				tempValue[v + 1] = (byte)tempG;
				tempValue[v + 2] = (byte)tempR;
			}
		}
		for (int x = 0; x < w; x++)
		{
			for (int y = 0; y < h; y++)
			{
				tempB = tempG = tempR = 0.0;
				for (int k = -radius; k <= radius; k++)
				{
					rem = (Math.Abs(y + k) % h);
					t = rem * w * 4 + x * 4;
					K = kernel[k + radius];
					tempB += tempValue[t] * K;
					tempG += tempValue[t + 1] * K;
					tempR += tempValue[t + 2] * K;
				}
				v = x * 4 + y * w * 4;
				srcValue[v] = (byte)tempB;
				srcValue[v + 1] = (byte)tempG;
				srcValue[v + 2] = (byte)tempR;
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(srcValue, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150424125757042?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.65形态学轮廓提取算法


**[函数名称]**

  形态学轮廓提取函数

​      WriteableBitmap MorcontourextractionProcess(WriteableBitmap src)

![img](https://img-blog.csdn.net/20150425085040927?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
 /// Morgraphy contour extraction process.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <returns></returns>
	public static WriteableBitmap MorcontourextractionProcess(WriteableBitmap src)////形态学轮廓提取
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap corrosionImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		for (int j = 0; j < h; j++)
		{
			for (int i = 0; i < w; i++)
			{
				if (i == 0 || i == w - 1 || j == 0 || j == h - 1)
				{
					temp[i * 4 + j * w * 4] = (byte)255;
					temp[i * 4 + 1 + j * w * 4] = (byte)255;
					temp[i * 4 + 2 + j * w * 4] = (byte)255;
				}
				else
				{
					if (tempMask[i * 4 - 4 + j * w * 4] == 255 && tempMask[i * 4 + j * w * 4] == 255 && tempMask[i * 4 + 4 + j * w * 4] == 255
						&& tempMask[i * 4 + (j - 1) * w * 4] == 255 && tempMask[i * 4 + (j + 1) * w * 4] == 255)
					{
						temp[i * 4 + j * w * 4] = (byte)255;
						temp[i * 4 + 1 + j * w * 4] = (byte)255;
						temp[i * 4 + 2 + j * w * 4] = (byte)255;
					}
					else
					{
						temp[i * 4 + j * w * 4] = 0;
						temp[i * 4 + 1 + j * w * 4] = 0;
						temp[i * 4 + 2 + j * w * 4] = 0;
					}
				}
			}
		}
		for (int i = 0; i < temp.Length; i++)
		{
			temp[i] = (byte)(tempMask[i] - temp[i]);
		}
		Stream sTemp = corrosionImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return corrosionImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150425085150846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.66FloodFill算法


**[函数名称]**

  洪水填充算法函数

WriteableBitmap FloodfillProcess(WriteableBitmap src,Point location, Color fillColor, int threshold)

![img](https://img-blog.csdn.net/20150425085350639?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



2，以这个点为起点，将它压入栈中，假设我们要填充的颜色为A，则将该点颜色设置为A，然后判断它的四邻域像素，这里我们设置一个颜色阈值T，假设当前像素灰度值为P(x,y)，四邻域像素为M(n),n=1,2,3,4，那么判断当前像素与四邻域像素的灰度差值D=|P-M|，如果D小于T， 那么我们将该像素M作为下一个种子点，压入栈中，否则继续判断。如图中黑色像素的四邻域内有一灰色点，与其差值小于T，则把它作为新的种子点压入栈中，继续判断。

3，当栈为空时，种子填充结束，否则重做步骤2。

**[函数代码]**



```csharp
/// <summary>
 /// Flood fill.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <param name="location">The start point to be filled.</param>
 /// <param name="fillColor">The color to be filled.</param>
 /// <param name="threshold">One parameter to control fill effect, from 0 to 255.</param>
 /// <returns></returns>
	public static WriteableBitmap FloodfillProcess(WriteableBitmap src, Point location, Color fillColor, int threshold)////洪水填充算法
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		Stack<Point> fillPoints = new Stack<Point>(w * h);
		int[, ] mask = new int[w, h];
		WriteableBitmap fillImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		Color backColor = Color.FromArgb(0, tempMask[(int)location.X * 4 + 2 + (int)location.Y * w * 4], tempMask[(int)location.X * 4 + 1 + (int)location.Y * w * 4], tempMask[(int)location.X * 4 + (int)location.Y * w * 4]);
		int gray = (int)((backColor.R + backColor.G + backColor.B) / 3);
		if (location.X < 0 || location.X >= w || location.Y < 0 || location.Y >= h) return null;
		fillPoints.Push(new Point(location.X, location.Y));
		while (fillPoints.Count > 0)
		{
			Point p = fillPoints.Pop();
			mask[(int)p.X, (int)p.Y] = 1;
			tempMask[4 * (int)p.X + (int)p.Y * w * 4] = (byte)fillColor.B;
			tempMask[4 * (int)p.X + 1 + (int)p.Y * w * 4] = (byte)fillColor.G;
			tempMask[4 * (int)p.X + 2 + (int)p.Y * w * 4] = (byte)fillColor.R;
			if (p.X > 0 && (Math.Abs(gray - (int)((tempMask[4 * ((int)p.X - 1) + (int)p.Y * w * 4] + tempMask[4 * ((int)p.X - 1) + 1 + (int)p.Y * w * 4] + tempMask[4 * ((int)p.X - 1) + 2 + (int)p.Y * w * 4]) / 3)) < threshold) && (mask[(int)p.X - 1, (int)p.Y] != 1))
			{
				tempMask[4 * ((int)p.X - 1) + (int)p.Y * w * 4] = (byte)fillColor.B;
				tempMask[4 * ((int)p.X - 1) + 1 + (int)p.Y * w * 4] = (byte)fillColor.G;
				tempMask[4 * ((int)p.X - 1) + 2 + (int)p.Y * w * 4] = (byte)fillColor.R;
				fillPoints.Push(new Point(p.X - 1, p.Y));
				mask[(int)p.X - 1, (int)p.Y] = 1;
			}
			if (p.X < w - 1 && (Math.Abs(gray - (int)((tempMask[4 * ((int)p.X + 1) + (int)p.Y * w * 4] + tempMask[4 * ((int)p.X + 1) + 1 + (int)p.Y * w * 4] + tempMask[4 * ((int)p.X + 1) + 2 + (int)p.Y * w * 4]) / 3)) < threshold) && (mask[(int)p.X + 1, (int)p.Y] != 1))
			{
				tempMask[4 * ((int)p.X + 1) + (int)p.Y * w * 4] = (byte)fillColor.B;
				tempMask[4 * ((int)p.X + 1) + 1 + (int)p.Y * w * 4] = (byte)fillColor.G;
				tempMask[4 * ((int)p.X + 1) + 2 + (int)p.Y * w * 4] = (byte)fillColor.R;
				fillPoints.Push(new Point(p.X + 1, p.Y));
				mask[(int)p.X + 1, (int)p.Y] = 1;
			}
			if (p.Y > 0 && (Math.Abs(gray - (int)((tempMask[4 * (int)p.X + ((int)p.Y - 1) * w * 4] + tempMask[4 * (int)p.X + 1 + ((int)p.Y - 1) * w * 4] + tempMask[4 * (int)p.X + 2 + ((int)p.Y - 1) * w * 4]) / 3)) < threshold) && (mask[(int)p.X, (int)p.Y - 1] != 1))
			{
				tempMask[4 * (int)p.X + ((int)p.Y - 1) * w * 4] = (byte)fillColor.B;
				tempMask[4 * (int)p.X + 1 + ((int)p.Y - 1) * w * 4] = (byte)fillColor.G;
				tempMask[4 * (int)p.X + 2 + ((int)p.Y - 1) * w * 4] = (byte)fillColor.R;
				fillPoints.Push(new Point(p.X, p.Y - 1));
				mask[(int)p.X, (int)p.Y - 1] = 1;
			}
			if (p.Y < h - 1 && (Math.Abs(gray - (int)((tempMask[4 * (int)p.X + ((int)p.Y + 1) * w * 4] + tempMask[4 * (int)p.X + 1 + ((int)p.Y + 1) * w * 4] + tempMask[4 * (int)p.X + 2 + ((int)p.Y + 1) * w * 4]) / 3)) < threshold) && (mask[(int)p.X, (int)p.Y + 1] != 1))
			{
				tempMask[4 * (int)p.X + ((int)p.Y + 1) * w * 4] = (byte)fillColor.B;
				tempMask[4 * (int)p.X + 1 + ((int)p.Y + 1) * w * 4] = (byte)fillColor.G;
				tempMask[4 * (int)p.X + 2 + ((int)p.Y + 1) * w * 4] = (byte)fillColor.R;
				fillPoints.Push(new Point(p.X, p.Y + 1));
				mask[(int)p.X, (int)p.Y + 1] = 1;
			}
		}
		fillPoints.Clear();
		temp = (byte[])tempMask.Clone();
		Stream sTemp = fillImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return fillImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150425085342068?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.67图像最大值滤波器


**[函数名称]**

  最大值滤波器WriteableBitmap MaxFilterProcess(WriteableBitmap src)





**[算法说明]**

  最大值滤波属于非线性滤波方法，它是一种基于排序统计理论的可有效抑制噪声的非线性平滑滤波

器，基本原理是把数字图像或数字序列中一点的值用该点的一个临域中各点值的最大值替换。

![img](https://img-blog.csdn.net/20150426131437563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





**[函数代码]**



```csharp
/// <summary>
/// Max value filter.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap MaxFilterProcess(WriteableBitmap src)////最大值滤波器
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double[] Gray = new double[9];
		double gray = 0;
		int tempr = 0, tempb = 0, tempg = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				tempb = 0;
				tempg = 0;
				tempr = 0;
				gray = 0;
				int[] B = new int[9]{ tempMask[i * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j - 1) * w * 4], tempMask[i * 4 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + j * w * 4], tempMask[(i + 1) * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j + 1) * w * 4], tempMask[i * 4 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + (j + 1) * w * 4] };
				int[] G = new int[9]{ tempMask[i * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j - 1) * w * 4], tempMask[i * 4 + 1 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 1 + j * w * 4], tempMask[(i + 1) * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j + 1) * w * 4], tempMask[i * 4 + 1 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j + 1) * w * 4] };
				int[] R = new int[9]{ tempMask[i * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j - 1) * w * 4], tempMask[i * 4 + 2 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 2 + j * w * 4], tempMask[(i + 1) * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j + 1) * w * 4], tempMask[i * 4 + 2 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j + 1) * w * 4] };
				for (int n = 0; n < 9; n++)
				{
					Gray[n] = (double)B[n] * 0.114 + (double)G[n] * 0.587 + (double)R[n] * 0.299;
					if (gray < Gray[n])
					{
						gray = Gray[n];
						tempb = B[n];
						tempr = R[n];
						tempg = G[n];
					}
				}
				temp[i * 4 + j * w * 4] = (byte)tempb;
				temp[i * 4 + 1 + j * w * 4] = (byte)tempg;
				temp[i * 4 + 2 + j * w * 4] = (byte)tempr;
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150426131442642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.68图像最小值滤波器


![img](https://img-blog.csdn.net/20150427115931956?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Min value filter.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap MinFilterProcess(WriteableBitmap src)////最小值滤波器
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double[] Gray = new double[9];
		double gray = 255;
		int tempr = 0, tempb = 0, tempg = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				tempb = 0;
				tempg = 0;
				tempr = 0;
				gray = 255;
				int[] B = new int[9]{ tempMask[i * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j - 1) * w * 4], tempMask[i * 4 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + j * w * 4], tempMask[(i + 1) * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j + 1) * w * 4], tempMask[i * 4 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + (j + 1) * w * 4] };
				int[] G = new int[9]{ tempMask[i * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j - 1) * w * 4], tempMask[i * 4 + 1 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 1 + j * w * 4], tempMask[(i + 1) * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j + 1) * w * 4], tempMask[i * 4 + 1 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j + 1) * w * 4] };
				int[] R = new int[9]{ tempMask[i * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j - 1) * w * 4], tempMask[i * 4 + 2 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 2 + j * w * 4], tempMask[(i + 1) * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j + 1) * w * 4], tempMask[i * 4 + 2 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j + 1) * w * 4] };
				for (int n = 0; n < 9; n++)
				{
					Gray[n] = (double)B[n] * 0.114 + (double)G[n] * 0.587 + (double)R[n] * 0.299;
					if (gray > Gray[n])
					{
						gray = Gray[n];
						tempb = B[n];
						tempr = R[n];
						tempg = G[n];
					}
				}
				temp[i * 4 + j * w * 4] = (byte)tempb;
				temp[i * 4 + 1 + j * w * 4] = (byte)tempg;
				temp[i * 4 + 2 + j * w * 4] = (byte)tempr;
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		return null;
	}
}
```

**[图像效果]**

![img](https://img-blog.csdn.net/20150427115917935?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

﻿﻿Win8 Metro(C#)数字图像处理--2.69中点滤波器


![img](https://img-blog.csdn.net/20150427120127357?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

**[函数代码]**











```csharp
/// <summary>
/// Mid-point filter.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap MidPointFilterProcess(WriteableBitmap src)////中点滤波器
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double[] Gray = new double[9];
		double gray = 255, graymax = 0;
		int tempr = 0, tempb = 0, tempg = 0, temprMax = 0, tempgMax = 0, tempbMax = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				tempb = 0;
				tempg = 0;
				tempr = 0;
				gray = 255;
				graymax = 0;
				int[] B = new int[9]{ tempMask[i * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j - 1) * w * 4], tempMask[i * 4 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + j * w * 4], tempMask[(i + 1) * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j + 1) * w * 4], tempMask[i * 4 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + (j + 1) * w * 4] };
				int[] G = new int[9]{ tempMask[i * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j - 1) * w * 4], tempMask[i * 4 + 1 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 1 + j * w * 4], tempMask[(i + 1) * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j + 1) * w * 4], tempMask[i * 4 + 1 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j + 1) * w * 4] };
				int[] R = new int[9]{ tempMask[i * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j - 1) * w * 4], tempMask[i * 4 + 2 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 2 + j * w * 4], tempMask[(i + 1) * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j + 1) * w * 4], tempMask[i * 4 + 2 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j + 1) * w * 4] };
				for (int n = 0; n < 9; n++)
				{
					Gray[n] = (double)B[n] * 0.114 + (double)G[n] * 0.587 + (double)R[n] * 0.299;
					if (gray > Gray[n])
					{
						gray = Gray[n];
						tempb = B[n];
						tempr = R[n];
						tempg = G[n];
					}
					if (graymax < Gray[n])
					{
						graymax = Gray[n];
						tempbMax = B[n];
						tempgMax = G[n];
						temprMax = R[n];
					}
				}
				temp[i * 4 + j * w * 4] = (byte)((tempb + tempbMax) / 2);
				temp[i * 4 + 1 + j * w * 4] = (byte)((tempg + tempgMax) / 2);
				temp[i * 4 + 2 + j * w * 4] = (byte)((tempr + temprMax) / 2);
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		return null;
	}
}
```



**[图像效果]**

![img](https://img-blog.csdn.net/20150427120239268?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



## C#数字图像处理--2.70修正后的阿尔法滤波器


![img](https://img-blog.csdn.net/20150427120345015?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// Alpha filter.
/// </summary>
/// <param name="src">The source image.</param>
/// <param name="tNumber">One parameter set from 0 to 9.</param>
/// <returns></returns>
public static WriteableBitmap AlphaFilterProcess(WriteableBitmap src, int tNumber)////修正的阿尔法滤波器
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double[] Gray = new double[9];
		double gray = 255;
		int tempr = 0, tempb = 0, tempg = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				tempb = 0;
				tempg = 0;
				tempr = 0;

				int[] B = new int[9]{ tempMask[i * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j - 1) * w * 4], tempMask[i * 4 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + j * w * 4], tempMask[(i + 1) * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j + 1) * w * 4], tempMask[i * 4 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + (j + 1) * w * 4] };
				int[] G = new int[9]{ tempMask[i * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j - 1) * w * 4], tempMask[i * 4 + 1 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 1 + j * w * 4], tempMask[(i + 1) * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j + 1) * w * 4], tempMask[i * 4 + 1 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j + 1) * w * 4] };
				int[] R = new int[9]{ tempMask[i * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j - 1) * w * 4], tempMask[i * 4 + 2 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 2 + j * w * 4], tempMask[(i + 1) * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j + 1) * w * 4], tempMask[i * 4 + 2 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j + 1) * w * 4] };
				for (int n = 0; n < 9; n++)
				{
					Gray[n] = (double)B[n] * 0.114 + (double)G[n] * 0.587 + (double)R[n] * 0.299;
				}
				for (int x = 0; x < 9; x++)
				{
					for (int y = x + 1; y < 9; y++)
					{
						gray = Gray[x];
						tempb = B[x];
						tempg = G[x];
						tempr = R[x];
						if (Gray[x] > Gray[y])
						{
							Gray[x] = Gray[y];
							Gray[y] = gray;
							B[x] = B[y];
							G[x] = G[y];
							R[x] = R[y];
							B[y] = tempb;
							G[y] = tempg;
							R[y] = tempr;
						}
					}
				}
				if (tNumber < 0 || tNumber > 8) return null;
				tNumber = (tNumber % 2 == 0 ? tNumber : tNumber - 1);
				for (int m = 0; m < (int)(tNumber / 2); m++)
				{
					R[m] = 0;
					G[m] = 0;
					B[m] = 0;
					R[8 - m] = 0;
					G[8 - m] = 0;
					B[8 - m] = 0;
				}
				temp[i * 4 + j * w * 4] = (byte)(B.Sum() / (9 - tNumber));
				temp[i * 4 + 1 + j * w * 4] = (byte)(G.Sum() / (9 - tNumber));
				temp[i * 4 + 2 + j * w * 4] = (byte)(R.Sum() / (9 - tNumber));
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150427120322138?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.71Sigma平滑滤波器


**[算法说明]**

  Sigma平滑滤波器是构造一个模板，比如3*3大小的模板，计算这个模板对应的像素的标准差d，然后根据统计学原理，得到一个置信区间v，假设3*3模板中心像素为p，则v大小范围为[p-d,p+d]。

我们判断3*3模板中对应像素是否落在置信区间v内，对于在v内的像素，我们求取他们的均值，然后，当前模板对应的中心像素的滤波值就等于这个均值。

图像表示如下：

![img](https://img-blog.csdn.net/20150427163029637?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Sigma filter.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static WriteableBitmap SigmaFilterProcess(WriteableBitmap src)////Sigma滤波器
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap filterImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		byte[] tempMask = (byte[])temp.Clone();
		double[] Gray = new double[9];
		double variance = 0;
		int count = 0;
		for (int j = 1; j < h - 1; j++)
		{
			for (int i = 1; i < w - 1; i++)
			{
				variance = 0;
				count = 0;
				int[] B = new int[9]{ tempMask[i * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j - 1) * w * 4], tempMask[i * 4 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + j * w * 4], tempMask[(i + 1) * 4 + j * w * 4], tempMask[(i - 1) * 4 + (j + 1) * w * 4], tempMask[i * 4 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + (j + 1) * w * 4] };
				int[] G = new int[9]{ tempMask[i * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j - 1) * w * 4], tempMask[i * 4 + 1 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 1 + j * w * 4], tempMask[(i + 1) * 4 + 1 + j * w * 4], tempMask[(i - 1) * 4 + 1 + (j + 1) * w * 4], tempMask[i * 4 + 1 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 1 + (j + 1) * w * 4] };
				int[] R = new int[9]{ tempMask[i * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j - 1) * w * 4], tempMask[i * 4 + 2 + (j - 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j - 1) * w * 4], tempMask[(i - 1) * 4 + 2 + j * w * 4], tempMask[(i + 1) * 4 + 2 + j * w * 4], tempMask[(i - 1) * 4 + 2 + (j + 1) * w * 4], tempMask[i * 4 + 2 + (j + 1) * w * 4], tempMask[(i + 1) * 4 + 2 + (j + 1) * w * 4] };
				for (int n = 0; n < 9; n++)
				{
					Gray[n] = (double)B[n] * 0.114 + (double)G[n] * 0.587 + (double)R[n] * 0.299;
				}
				for (int m = 0; m < 9; m++)
				{
					variance += ((double)Gray[m] - Gray.Average()) * ((double)Gray[m] - Gray.Average()) / 9;
				}
				variance = Math.Sqrt(variance);
				for (int m = 0; m < 9; m++)
				{
					if (Gray[m] < Gray[0] - variance && Gray[m] > variance + Gray[0])
					{
						R[m] = 0;
						G[m] = 0;
						B[m] = 0;
					}
					else count++;
				}
				temp[i * 4 + j * w * 4] = (byte)(B.Sum() / count);
				temp[i * 4 + 1 + j * w * 4] = (byte)(G.Sum() / count);
				temp[i * 4 + 2 + j * w * 4] = (byte)(R.Sum() / count);
			}
		}
		Stream sTemp = filterImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return filterImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150427163223816?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.72图像噪声


**[函数名称]**

  噪声函数WriteableBitmap NoiseProcess(WriteableBitmap src, byte flagNoise, double meanValue, double varianceValue)

![img](https://img-blog.csdn.net/20150427163421523?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20150427163410081?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





```csharp
/// <summary>
/// Noise process.
/// </summary>
/// <param name="src">The source image.</param>
	 /// <param name="flagNoise">One parameter to choose noise mode,0--Gauss noise,1--Rayleigh noise,2--Noise index,3--alt and pepper noise.</param>
/// <param name="meanValue">Mean value or alt value.</param>
/// <param name="varianceValue">Variance value or pepper value.</param>
/// <returns></returns>
	public static WriteableBitmap NoiseProcess(WriteableBitmap src, byte flagNoise, double meanValue, double varianceValue)////噪声
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		double tempValue = 0;
		Random r1, r2;
		double v1 = 0, v2 = 0;
		r1 = new Random(unchecked((int)DateTime.Now.Ticks));
		r2 = new Random(~unchecked((int)DateTime.Now.Ticks));
		for (int i = 0; i < temp.Length; i++)
		{
			switch (flagNoise)
			{
			case 0:
				//Gauss noise 0 20
				do
				{
					v1 = r1.NextDouble();
				} while (v1 <= 0.00000000001);
				v2 = r2.NextDouble();
				tempValue = Math.Sqrt(-2 * Math.Log(v1)) * Math.Cos(2 * Math.PI * v2) * varianceValue + meanValue;
				break;
			case 1:
				do
					//Rayleigh noise 0 200
				{
					v1 = r1.NextDouble();
				} while (v1 >= 0.9999999999);
				tempValue = meanValue + Math.Sqrt(-1 * varianceValue * Math.Log(1 - v1));
				break;
			case 2:
				//Noise index  0.1
				do
				{
					v1 = r1.NextDouble();
				} while (v1 >= 0.9999999999);
				tempValue = -1 * Math.Log(1 - v1) / meanValue;
				break;
			case 3:
				//Salt and pepper noise 0.02 0.02
				v1 = r1.NextDouble();
				if (v1 <= meanValue)
					tempValue = -500;
				else if (v1 >= (1 - varianceValue))
					tempValue = 500;
				else
					tempValue = 0;
				break;
			default:
				break;
			}
			tempValue = tempValue + temp[i];

			if (tempValue > 255)
			{
				temp[i] = 255;
			}
			else if (tempValue < 0)
			{
				temp[i] = 0;
			}
			else
				temp[i] = Convert.ToByte(tempValue);
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```



![img](https://img-blog.csdn.net/20150427163507473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20150427163651736?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.73一种背景图像融合特效


![img](https://img-blog.csdn.net/20150427163803287?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Image merge process.
/// </summary>
/// <param name="bacImage">The background image.</param>
/// <param name="dstImage">The source image.</param>
/// <param name="k">One parameter, from 0 to 1.</param>
/// <returns></returns>
public static WriteableBitmap ImageMerge(WriteableBitmap bacImage, WriteableBitmap dstImage, double k)
{
	if (bacImage != null && dstImage != null)
	{
		int w = dstImage.PixelWidth;
		int h = dstImage.PixelHeight;
		int sw = bacImage.PixelWidth;
		int sh = bacImage.PixelHeight;
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] dstValue = dstImage.PixelBuffer.ToArray();
		byte[] bacValue = bacImage.PixelBuffer.ToArray();
		byte[] tempValue = new byte[dstValue.Length];
		int r = 0, g = 0, b = 0, R = 0, G = 0, B = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				int tx = x % sw;
				int ty = y % sh;
				b = bacValue[tx * 4 + ty * w * 4];
				g = bacValue[tx * 4 + 1 + ty * w * 4];
				r = bacValue[tx * 4 + 2 + ty * w * 4];
				B = dstValue[x * 4 + y * w * 4];
				G = dstValue[x * 4 + 1 + y * w * 4];
				R = dstValue[x * 4 + 2 + y * w * 4];
				double xr = 0.0, xb = 0.0, xg = 0.0;
				xr = ((double)r - ((double)R - (double)k * 255.0)) / (2.0 * 255.0 * k);
				xg = ((double)g - ((double)G - (double)k * 255.0)) / (2.0 * 255.0 * k);
				xb = ((double)b - ((double)B - (double)k * 255.0)) / (2.0 * 255.0 * k);
				tempValue[x * 4 + y * w * 4] = (byte)(255.0 * (1.0 - 3.0 * xb * xb + 2.0 * xb * xb * xb));
				tempValue[x * 4 + 1 + y * w * 4] = (byte)(255.0 * (1.0 - 3.0 * xg * xg + 2.0 * xg * xg * xg));
				tempValue[x * 4 + 2 + y * w * 4] = (byte)(255.0 * (1.0 - 3.0 * xr * xr + 2.0 * xr * xr * xr));
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(tempValue, 0, w * 4 * h);
		return srcImage;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150427163854387?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.74图像凸包计算


![img](https://img-blog.csdn.net/20150428123650270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



​```csharp
/// <summary>
/// Convex Hull compute.
/// </summary>
/// <param name="points">The source image points.</param>
/// <param name="startPoints"></param>
/// <param name="endPoints"></param>
private static void ConvexhullCompute(ref Point[] srcPoints, ref List<Point>startPoints, ref List<Point> endPoints)
{
	Point start;
	Point end;
	double a = 0, b = 0;
	int signa = 0, signb = 0;
	int max = srcPoints.Length;
	double temp = 0.0;
	int t = 0;
	double tx = 0.0;
	for (int i = 0; i < max; i++)
	{
		start = srcPoints[i];
		for (int k = i + 1; k < max; k++)
		{
			end = srcPoints[k];
			if (start.X == end.X)
			{
				for (int j = 0; j < max; j++)
				{
					if ((!(signa > 0 && signb > 0)) && (j != i && j != k))
					{
						t = srcPoints[j].X;
						if (t > start.X)
							signa++;
						if (t < start.X)
							signb++;
					}
				}
			}
			else
			{
				a = (double)(end.Y - start.Y) / (double)(end.X - start.X);
				b = (double)start.Y - (double)start.X * a;
				for (int j = 0; j < max; j++)
				{
					if ((!(signa > 0 && signb > 0)) && (j != i && j != k))
					{
						temp = a * (double)srcPoints[j].X + b;
						tx = (double)srcPoints[j].Y;
						if (temp > tx)
							signa++;
						if (temp < tx)
							signb++;
					}
				}
			}
			if (signa * signb == 0)
			{
				startPoints.Add(start);
				endPoints.Add(end);
			}
			signb = 0;
			signa = 0;
		}
	}
}
```

![img](https://img-blog.csdn.net/20150428123740517?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

## C#数字图像处理--2.75灰度图像的形态学算法


前面章节中介绍了二值图像的形态学算法，这里讲一下灰度图的形态学算法，主要是公式，代码略。

1，膨胀算法

![img](https://img-blog.csdn.net/20150509142654187?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

2，腐蚀算法

![img](https://img-blog.csdn.net/20150509142538446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

3，开运算

开运算：先进行一次腐蚀运算，接着进行一次膨胀运算。

4，闭运算

闭运算即：先进行一次膨胀运算，接着进行一次腐蚀运算。

以上就是灰度图像的形态学算法。

## C#数字图像处理--3.1图像均值计算


![img](https://img-blog.csdn.net/20150428123921543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Mean value computing.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double GetMeanProcess(WriteableBitmap src) ////32 图像均值计算
{
	if (src != null)
	{
		double mean = 0.0;
		int sum = 0;
		int gray = 0, number = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			sum += gray;
			number++;
		}
		mean = (double)(sum / number);
		return mean;
	}
	else
	{
		return 0;
	}
}
```

## C#数字图像处理--3.2图像方差计算


![img](https://img-blog.csdn.net/20150428124202580?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
 ///
 /// </summary>Variance computing.
 /// <param name="src">The source image.</param>
 /// <returns></returns>
	public static double GetVarianceProcess(WriteableBitmap src) ////33 图像方差计算
{
	if (src != null)
	{
		double mean = GetMeanProcess(src);
		double sum = 0;
		double variance = 0;
		int gray = 0, number = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			sum += (gray - mean)*(gray - mean);
			number++;
		}
		variance = (double)(sum / number);
		return variance;
	}
	else
	{
		return 0;
	}
}
```

## C#数字图像处理--3.3图像直方图计算


![img](https://img-blog.csdn.net/20150429115010331?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Get the array of histrgram.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static int[] GetHistogramArray(WriteableBitmap src) ////34 图像直方图计算
{
	if (src != null)
	{
		int[] histogram = new int[256];
		int gray = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			histogram[gray]++;
		}
		return histogram;
	}
	else
	{
		return null;
	}
}
```

## C#数字图像处理--3.4图像信息熵计算


![img](https://img-blog.csdn.net/20150429115148876?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
[函数代码]
/// <summary>
/// Entropy of one image.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double GetEntropy(WriteableBitmap src)
{
	double entropy = 0;
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		byte[] temp = src.PixelBuffer.ToArray();
		int[] countTemp = new int[256];
		int gray = 0;
		for (int i = 0; i < temp.Length; i += 4)
		{
			gray = (int)(temp[i] * 0.114 + temp[i + 1] * 0.587 + temp[i + 2] * 0.299);
			countTemp[gray]++;
		}
		for (int i = 0; i < 256; i++)
		{
			if (countTemp[i] != 0)
			{
				entropy += (double)(-((countTemp[i] / (w * h)) * Math.Log10(countTemp[i] / (w * h))));
			}
			else
				continue;
		}
		return entropy;
	}
	else
	{
		return 0;
	}
}
```

## C#数字图像处理--3.5图像形心计算


![img](https://img-blog.csdn.net/20150429115217754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Get the center of the object in an image.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static int[] CenterPoints(WriteableBitmap src)
{
	if (src != null)
	{
		int[] Centerpoint = new int[2];
		int M00 = 0, M01 = 0, M10 = 0;
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		int b = 0, g = 0, r = 0;
		byte[] temp = src.PixelBuffer.ToArray();
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w * 4; x += 4)
			{
				b = temp[x + y * w * 4];
				g = temp[x + 1 + y * w * 4];
				r = temp[x + 2 + y * w * 4];
				if (r + g + b != 0)
				{
					M00++;
					M01 += y;
					M10 += x;
				}
			}
		}
		Centerpoint[0] = (int)(M10 / M00);
		Centerpoint[1] = (int)(M01 / M00);
		return Centerpoint;
	}
	else
	{
		return null;
	}
}
```

## C#数字图像处理--4图像颜色空间描述


图像颜色空间是图像颜色集合的数学表示，本小节将针对几种常见颜色空间做个简单介绍。

![img](https://img-blog.csdn.net/20150429123419670?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







```csharp
/// <summary>
 /// Get rgba value from source image.
 /// </summary>
 /// <param name="src">The source image.</param>
 /// <returns></returns>
	public static byte[] RGBValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		byte[] rgbaValue = new byte[w * h * 3];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				rgbaValue[x * 3 + y * w * 3] = (byte)temp[x * 4 + y * w * 4];
				rgbaValue[x * 3 + 1 + y * w * 3] = (byte)temp[x * 4 + 1 + y * w * 4];
				rgbaValue[x * 3 + 2 + y * w * 3] = (byte)temp[x * 4 + 2 + y * w * 4];
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return rgbaValue;
	}
	else
	{
		return null;
	}
}
```



## **4.2 XYZ颜色空间**

**[空间解析]**

XYZ颜色空间：国际照明委员会(CIE)在进行了大量正常人视觉测量和统计,1931年建立了“标准色度观察者”， 从而奠定了现代CIE标准色度学的定量基础。由于"标准色度观察者"用来标定光谱色时出现负 刺激值，计算不便，也不易理解，因此1931年CIE在RGB系统基础上，改用三个假想的原色X、Y、 Z建立了一个新的色度系统。将它匹配等能光谱的三刺激值，定名为"CIE1931 标准色度观察者 光谱三刺激值"，简称为"CIE1931标准色度观察者"。这一系统叫做"CIE1931标准色度系统"或称为“2”视场XYZ色度系统"。其中Y表示亮度，X、Y反映颜色的色度特性，色度图如下所示。

![img](https://img-blog.csdn.net/20150429123707524?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Get XYZ value from source image.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double[] XYZValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[] xyzValue = new double[w * h * 3];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = temp[x * 4 + y * w * 4];
				g = temp[x * 4 + 1 + y * w * 4];
				r = temp[x * 4 + 2 + y * w * 4];
				xyzValue[x * 3 + y * w * 3] = (double)(0.5767309 * (double)r + 0.1855540 * (double)g + 0.1881852 * (double)b);
				xyzValue[x * 3 + 1 + y * w * 3] = (double)(0.2973769 * (double)r + 0.6273491 * (double)g + 0.0752741 * (double)b);
				xyzValue[x * 3 + 2 + y * w * 3] = (double)(0.0270343 * (double)r + 0.0706872 * (double)g + 0.9911085 * (double)b);
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return xyzValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert rgb to xyz.
/// </summary>
/// <param name="rgbValue">The rgb value.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] RGBtoXYZ(byte[]rgbValue, int w, int h)
{
	if (rgbValue != null)
	{
		double[] xyzValue = new double[w * h * 3];
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = rgbValue[x * 3 + y * w * 3];
				g = rgbValue[x * 3 + 1 + y * w * 3];
				r = rgbValue[x * 3 + 2 + y * w * 3];
				xyzValue[x * 3 + y * w * 3] = (double)(0.5767309 * (double)r + 0.1855540 * (double)g + 0.1881852 * (double)b);
				xyzValue[x * 3 + 1 + y * w * 3] = (double)(0.2973769 * (double)r + 0.6273491 * (double)g + 0.0752741 * (double)b);
				xyzValue[x * 3 + 2 + y * w * 3] = (double)(0.0270343 * (double)r + 0.0706872 * (double)g + 0.9911085 * (double)b);
			}
		}
		return xyzValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert xyz to rgb.
/// </summary>
/// <param name="xyzValue">The xyz value.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] XYZtoRGB(byte[] xyzValue, int w, int h)
{
	if (xyzValue != null)
	{
		double[] rgbValue = new double[w * h * 3];
		double xV = 0, yV = 0, zV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				xV = xyzValue[x * 3 + y * w * 3];
				yV = xyzValue[x * 3 + 1 + y * w * 3];
				zV = xyzValue[x * 3 + 2 + y * w * 3];
				rgbValue[x * 3 + y * w * 3] = (byte)(0.0134474 * xV - 0.1183897 * yV + 1.0154096 * zV);
				rgbValue[x * 3 + 1 + y * w * 3] = (byte)(-0.9692660 * xV + 1.8760108 * yV + 0.0415560 * zV);
				rgbValue[x * 3 + 2 + y * w * 3] = (byte)(2.0413690 * xV - 0.5649464 * yV - 0.3446944 * zV);
			}
		}
		return rgbValue;
	}
	else
	{
		return null;
	}
}
```

![img](https://img-blog.csdn.net/20150429123805993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```csharp
/// <summary>
/// Get yuv information.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double[] YUVValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[] yuvValue = new double[w * h * 3];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = temp[x * 4 + y * w * 4];
				g = temp[x * 4 + 1 + y * w * 4];
				r = temp[x * 4 + 2 + y * w * 4];
				yuvValue[x * 3 + y * w * 3] = (double)(0.299 * (double)r + 0.587 * (double)g + 0.114 * (double)b);
				yuvValue[x * 3 + 1 + y * w * 3] = (double)(-0.148 * (double)r - 0.289 * (double)g + 0.437 * (double)b);
				yuvValue[x * 3 + 2 + y * w * 3] = (double)(0.615 * (double)r - 0.515 * (double)g - 0.100 * (double)b);
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return yuvValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert RGB to YUV.
/// </summary>
/// <param name="rgbValue">The rgb information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] RGBtoYUV(byte[] rgbValue, int w, int h)
{
	if (rgbValue != null)
	{
		double[] yuvValue = new double[w * h * 3];
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = rgbValue[x * 3 + y * w * 3];
				g = rgbValue[x * 3 + 1 + y * w * 3];
				r = rgbValue[x * 3 + 2 + y * w * 3];
				yuvValue[x * 3 + y * w * 3] = (double)(0.299 * (double)r + 0.587 * (double)g + 0.114 * (double)b);
				yuvValue[x * 3 + 1 + y * w * 3] = (double)(-0.148 * (double)r - 0.289 * (double)g + 0.437 * (double)b);
				yuvValue[x * 3 + 2 + y * w * 3] = (double)(0.615 * (double)r - 0.515 * (double)g - 0.100 * (double)b);
			}
		}
		return yuvValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert yuv to rgb.
/// </summary>
/// <param name="yuvValue">The yuv information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] YUVtoRGB(byte[] yuvValue, int w, int h)
{
	if (yuvValue != null)
	{
		double[] rgbValue = new double[w * h * 3];
		double yV = 0, uV = 0, vV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				yV = yuvValue[x * 3 + y * w * 3];
				uV = yuvValue[x * 3 + 1 + y * w * 3];
				vV = yuvValue[x * 3 + 2 + y * w * 3];
				rgbValue[x * 3 + y * w * 3] = (byte)(1 * yV + 2.032 * uV + 0 * vV);
				rgbValue[x * 3 + 1 + y * w * 3] = (byte)(1 * yV - 0.395 * uV - 0.581 * vV);
				rgbValue[x * 3 + 2 + y * w * 3] = (byte)(1 * yV + 0 * uV + 1.140 * vV);
			}
		}
		return rgbValue;
	}
	else
	{
		return null;
	}
}
```



## **4.4 HIS颜色空间**

 **[空间解析]**

  HIS颜色空间是从人的视觉系统出发，用色调(Hue)、色饱和度(Saturation或Chroma)和亮度 (Intensity或Brightness)来描述色彩。HSI色彩空间可以用一个圆锥空间模型来描述。用这种 描述HIS色彩空间的圆锥模型相当复杂，但确能把色调、亮度和色饱和度的变化情形表现得很清楚。通常把色调和饱和度通称为色度，用来表示颜色的类别与深浅程度，H和S包含了颜色信息，而I则与颜色信息无关。

  HIS颜色空间模型如下图所示：

![img](https://img-blog.csdn.net/20150429123839286?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20150429123906320?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Get his information.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
	public static double[] HISValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[] hisValue = new double[w * h * 3];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		double r = 0, g = 0, b = 0;
		double hV = 0, degree = 0, iV = 0, sV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = (double)temp[x * 4 + y * w * 4] / 255.0;
				g = (double)temp[x * 4 + 1 + y * w * 4] / 255.0;
				r = (double)temp[x * 4 + 2 + y * w * 4] / 255.0;
				degree = Math.Acos(0.5 * ((r - g) + (r - b)) / Math.Sqrt((r - g) * (r - g) + (r - b) * (g - b) + 0.0000001)) / (2 * Math.PI);
				hV = (b <= g) ? degree : 1.0 - degree;
				iV = (double)(r + g + b) / 3.0;
				sV = 1.0 - 3.0 * (double)Math.Min(r, Math.Min(g, b)) / (double)(r + g + b + 0.00000001);
				hisValue[x * 3 + y * w * 3] = hV;
				hisValue[x * 3 + 1 + y * w * 3] = iV;
				hisValue[x * 3 + 2 + y * w * 3] = sV;
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return hisValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert rgb to his.
/// </summary>
/// <param name="rgbValue">The rgb information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] RGBtoHIS(byte[] rgbValue, int w, int h)
{
	if (rgbValue != null)
	{
		double[] hisValue = new double[w * h * 3];
		double r = 0, g = 0, b = 0;
		double hV = 0, degree = 0, iV = 0, sV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = (double)rgbValue[x * 3 + y * w * 3] / 255.0;
				g = (double)rgbValue[x * 3 + 1 + y * w * 3] / 255.0;
				r = (double)rgbValue[x * 3 + 2 + y * w * 3] / 255.0;
				degree = Math.Acos(0.5 * ((r - g) + (r - b)) / Math.Sqrt((r - g) * (r - g) + (r - b) * (g - b) + 0.0000001)) / (2 * Math.PI);
				hV = (b <= g) ? degree : 1.0 - degree;
				iV = (double)(r + g + b) / 3.0;
				sV = 1.0 - 3.0 * (double)Math.Min(r, Math.Min(g, b)) / (double)(r + g + b + 0.00000001);
				hisValue[x * 3 + y * w * 3] = hV;
				hisValue[x * 3 + 1 + y * w * 3] = iV;
				hisValue[x * 3 + 2 + y * w * 3] = sV;
			}
		}
		return hisValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert his to rgb.
/// </summary>
/// <param name="hisValue">The his information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] HIStoRGB(byte[] hisValue, int w, int h)
{
	if (hisValue != null)
	{
		double[] rgbValue = new double[w * h * 3];
		double hV = 0, iV = 0, sV = 0;
		double r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				hV = hisValue[x * 3 + y * w * 3];
				iV = hisValue[x * 3 + 1 + y * w * 3];
				sV = hisValue[x * 3 + 2 + y * w * 3];
				hV = hV * 2 * Math.PI;
				if (hV >= 0 && hV < 2 * Math.PI / 3)
				{
					r = (double)(iV * (1.0 + (sV * Math.Cos(hV) / Math.Cos(Math.PI / 3 - hV))));
					b = (double)(iV * (1.0 - sV));
					g = (double)(3.0 * iV - r - b);
				}
				else if (hV >= 2 * Math.PI / 3 && hV < 4 * Math.PI / 3)
				{
					g = (double)(iV * (1.0 + sV * Math.Cos(hV - 2 * Math.PI / 3) / Math.Cos(Math.PI - hV)));
					r = (double)(iV * (1.0 - sV));
					b = (double)(3.0 * iV - r - g);
				}
				else
				{
					g = (double)(iV * (1.0 - sV));
					b = (double)(iV * (1.0 + sV * Math.Cos(hV - 4 * Math.PI / 3) / Math.Cos(5 * Math.PI / 3 - hV)));
					r = (double)(3.0 * iV - g - b);
				}
				rgbValue[x * 3 + y * w * 3] = (byte)(Math.Min(255, b * 255.0));
				rgbValue[x * 3 + 1 + y * w * 3] = (byte)(Math.Min(255, g * 255.0));
				rgbValue[x * 3 + 2 + y * w * 3] = (byte)(Math.Min(255, r * 255.0));
			}
		}
		return rgbValue;
	}
	else
	{
		return null;
	}
}
```





![img](https://img-blog.csdn.net/20150429124059060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Get YIQ information.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double[] YIQValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[] yiqValue = new double[w * h * 3];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = temp[x * 4 + y * w * 4];
				g = temp[x * 4 + 1 + y * w * 4];
				r = temp[x * 4 + 2 + y * w * 4];
				yiqValue[x * 3 + y * w * 3] = (double)(0.299 * (double)r + 0.587 * (double)g + 0.114 * (double)b);
				yiqValue[x * 3 + 1 + y * w * 3] = (double)(0.596 * (double)r - 0.275 * (double)g - 0.321 * (double)b);
				yiqValue[x * 3 + 2 + y * w * 3] = (double)(0.212 * (double)r - 0.523 * (double)g + 0.311 * (double)b);
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return yiqValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert rgb to yiq.
/// </summary>
/// <param name="rgbValue">The rgb information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] RGBtoYIQ(byte[] rgbValue, int w, int h)
{
	if (rgbValue != null)
	{
		double[] yiqValue = new double[w * h * 3];
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = rgbValue[x * 3 + y * w * 3];
				g = rgbValue[x * 3 + 1 + y * w * 3];
				r = rgbValue[x * 3 + 2 + y * w * 3];
				yiqValue[x * 3 + y * w * 3] = (double)(0.299 * (double)r + 0.587 * (double)g + 0.114 * (double)b);
				yiqValue[x * 3 + 1 + y * w * 3] = (double)(0.596 * (double)r - 0.275 * (double)g - 0.321 * (double)b);
				yiqValue[x * 3 + 2 + y * w * 3] = (double)(0.212 * (double)r - 0.523 * (double)g + 0.311 * (double)b);
			}
		}
		return yiqValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert yiq to rgb.
/// </summary>
/// <param name="yiqValue">The yiq information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] YIQtoRGB(byte[] yiqValue, int w, int h)
{
	if (yiqValue != null)
	{
		double[] rgbValue = new double[w * h * 3];
		double yV = 0, iV = 0, qV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				yV = yiqValue[x * 3 + y * w * 3];
				iV = yiqValue[x * 3 + 1 + y * w * 3];
				qV = yiqValue[x * 3 + 2 + y * w * 3];
				rgbValue[x * 3 + 2 + y * w * 3] = (byte)(1 * yV + 0.9560 * iV + 0.6210 * qV);
				rgbValue[x * 3 + 1 + y * w * 3] = (byte)(1 * yV - 0.2720 * iV - 0.6470 * qV);
				rgbValue[x * 3 + y * w * 3] = (byte)(1 * yV - 1.1070 * iV + 1.7040 * qV);
			}
		}
		return rgbValue;
	}
	else
	{
		return null;
	}
}
```



## **4.6 HSV颜色空间**

**[空间解析]**

  HSV彩色空间是一种适合肉眼分辨的模型。
  H—色相，表示色彩信息，即所处的光谱颜色的位置。该参数用角度量来表示，红、绿、蓝分别相隔120度。互补色分别相差180度。
  S—饱和度，该参数为一比例值，范围从0到1，它表示成所选颜色的纯度和该颜色最大的纯度之间的比率。S=0时，只有灰度。
  V—亮度，表示色彩的明亮程度，范围从0到1。

  HSV颜色空间模型如下图所示：

![img](https://img-blog.csdn.net/20150429124234362?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



​```csharp
/// <summary>
/// Get HSV information.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double[] HSVValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[] hsvValue = new double[w * h * 3];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		double r = 0, g = 0, b = 0;
		double min = 0, max = 0;
		double hV = 0, sV = 0, vV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = (double)temp[x * 4 + y * w * 4] / 255.0;
				g = (double)temp[x * 4 + 1 + y * w * 4] / 255.0;
				r = (double)temp[x * 4 + 2 + y * w * 4] / 255.0;
				min = Math.Min(r, Math.Min(g, b));
				max = Math.Max(r, Math.Max(g, b));
				if (max == min)
					hV = 0;
				if (max == r && g >= b)
					hV = 60.0 * (g - b) / (max - min);
				if (max == r && g < b)
					hV = 60.0 * (g - b) / (max - min) + 360.0;
				if (max == g)
					hV = 60.0 * (b - r) / (max - min) + 120.0;
				if (max == b)
					hV = 60.0 * (r - g) / (max - min) + 240.0;
				if (max == 0)
					sV = 0;
				else
					sV = (max - min) / max;
				vV = max;
				hsvValue[x * 3 + y * w * 3] = (double)(hV);
				hsvValue[x * 3 + 1 + y * w * 3] = (double)(sV);
				hsvValue[x * 3 + 2 + y * w * 3] = (double)(vV);
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return hsvValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert rgb to hsv.
/// </summary>
/// <param name="rgbValue">The rgb information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] RGBtoHSV(byte[] rgbValue, int w, int h)
{
	if (rgbValue != null)
	{
		double[] hsvValue = new double[w * h * 3];
		double r = 0, g = 0, b = 0;
		double min = 0, max = 0;
		double hV = 0, sV = 0, vV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				r = (double)rgbValue[x * 3 + 2 + y * w * 3] / 255.0;
				g = (double)rgbValue[x * 3 + 1 + y * w * 3] / 255.0;
				b = (double)rgbValue[x * 3 + y * w * 3] / 255.0;
				min = Math.Min(r, Math.Min(g, b));
				max = Math.Max(r, Math.Max(g, b));
				if (max == min)
					hV = 0;
				if (max == r && g >= b)
					hV = 60.0 * (g - b) / (max - min);
				if (max == r && g < b)
					hV = 60.0 * (g - b) / (max - min) + 360.0;
				if (max == g)
					hV = 60.0 * (b - r) / (max - min) + 120.0;
				if (max == b)
					hV = 60.0 * (r - g) / (max - min) + 240.0;
				if (max == 0)
					sV = 0;
				else
					sV = (max - min) / max;
				vV = max;
				hsvValue[x * 3 + y * w * 3] = (double)(hV);
				hsvValue[x * 3 + 1 + y * w * 3] = (double)(sV);
				hsvValue[x * 3 + 2 + y * w * 3] = (double)(vV);
			}
		}
		return hsvValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert hsv to rgb.
/// </summary>
/// <param name="hsvValue">The hsv information.</param>
/// <param name="w">The width of souce image.</param>
/// <param name="h">The height of souce image.</param>
/// <returns></returns>
public static double[] HSVtoRGB(byte[] hsvValue, int w, int h)
{
	if (hsvValue != null)
	{
		double[] rgbValue = new double[w * h * 3];
		double hV = 0, sV = 0, vV = 0, r = 0, g = 0, b = 0, p = 0, q = 0, t = 0;
		int hN = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				hV = hsvValue[x * 3 + y * w * 3];
				sV = hsvValue[x * 3 + 1 + y * w * 3];
				vV = hsvValue[x * 3 + 2 + y * w * 3];
				if (hV < 0)
					hV = 360 + hV;
				hN = (int)(hV / 60);
				p = vV * (1.0 - sV);
				q = vV * (1.0 - (hV / 60.0 - hN) * sV);
				t = vV * (1.0 - (1.0 - (hV / 60.0 - hN)) * sV);
				switch (hN)
				{
				case 0:
					r = vV;
					g = t;
					b = p;
					break;
				case 1:
					r = q;
					g = vV;
					b = p;
					break;
				case 2:
					r = p;
					g = vV;
					b = t;
					break;
				case 3:
					r = p;
					g = q;
					b = vV;
					break;
				case 4:
					r = t;
					g = p;
					b = vV;
					break;
				case 5:
					r = vV;
					g = p;
					b = q;
					break;
				default:
					break;
				}
				rgbValue[x * 3 + y * w * 3] = (byte)(255.0 * b);
				rgbValue[x * 3 + 1 + y * w * 3] = (byte)(255.0 * g);
				rgbValue[x * 3 + 2 + y * w * 3] = (byte)(255.0 * r);
			}
		}
		return rgbValue;
	}
	else
	{
		return null;
	}
}
```



![img](https://img-blog.csdn.net/20150429124444231?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvVHJlbnQxOTg1/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```csharp
/// <summary>
/// Get cmyk information.
/// </summary>
/// <param name="src">The source image.</param>
/// <returns></returns>
public static double[] CMYKValue(WriteableBitmap src)
{
	if (src != null)
	{
		int w = src.PixelWidth;
		int h = src.PixelHeight;
		double[] cmykValue = new double[w * h * 4];
		WriteableBitmap srcImage = new WriteableBitmap(w, h);
		byte[] temp = src.PixelBuffer.ToArray();
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = temp[x * 4 + y * w * 4];
				g = temp[x * 4 + 1 + y * w * 4];
				r = temp[x * 4 + 2 + y * w * 4];
				cmykValue[x * 4 + y * w * 4] = (double)(g + b);
				cmykValue[x * 4 + 1 + y * w * 4] = (double)(r + b);
				cmykValue[x * 4 + 2 + y * w * 4] = (double)(r + g);
				cmykValue[x * 4 + 3 + y * w * 4] = (double)(Math.Min((g + b), Math.Min((r + b), (r + g))));
			}
		}
		Stream sTemp = srcImage.PixelBuffer.AsStream();
		sTemp.Seek(0, SeekOrigin.Begin);
		sTemp.Write(temp, 0, w * 4 * h);
		return cmykValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert rgb to cmyk.
/// </summary>
/// <param name="rgbValue">The rgb information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] RGBtoCMYK(byte[] rgbValue, int w, int h)
{
	if (rgbValue != null)
	{
		double[] cmykValue = new double[w * h * 4];
		int r = 0, g = 0, b = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				b = rgbValue[x * 3 + y * w * 3];
				g = rgbValue[x * 3 + 1 + y * w * 3];
				r = rgbValue[x * 3 + 2 + y * w * 3];
				cmykValue[x * 4 + y * w * 4] = (double)(g + b);
				cmykValue[x * 4 + 1 + y * w * 4] = (double)(r + b);
				cmykValue[x * 4 + 2 + y * w * 4] = (double)(r + g);
				cmykValue[x * 4 + 3 + y * w * 4] = (double)(Math.Min((g + b), Math.Min((r + b), (r + g))));
			}
		}
		return cmykValue;
	}
	else
	{
		return null;
	}
}
/// <summary>
/// Convert cmyk to rgb.
/// </summary>
/// <param name="cmykValue">The cmyk information.</param>
/// <param name="w">The width of source image.</param>
/// <param name="h">The height of source image.</param>
/// <returns></returns>
public static double[] CMYKtoRGB(byte[] cmykValue, int w, int h)
{
	if (cmykValue != null)
	{
		double[] rgbValue = new double[w * h * 3];
		double cV = 0, mV = 0, yV = 0, kV = 0;
		for (int y = 0; y < h; y++)
		{
			for (int x = 0; x < w; x++)
			{
				cV = cmykValue[x * 4 + y * w * 3];
				mV = cmykValue[x * 4 + 1 + y * w * 3];
				yV = cmykValue[x * 4 + 2 + y * w * 3];
				kV = cmykValue[x * 4 + 3 + y * w * 3];
				rgbValue[x * 3 + y * w * 3] = (byte)(0.5 * (mV + cV - yV));
				rgbValue[x * 3 + 1 + y * w * 3] = (byte)(0.5 * (yV + cV - mV));
				rgbValue[x * 3 + 2 + y * w * 3] = (byte)(0.5 * (mV + yV - cV));
			}
		}
		return rgbValue;
	}
	else
	{
		return null;
	}
}
```

**最后，分享一个专业的图像处理网站（微像素），里面有很多源代码下载：**

<http://www.zealpixel.com/portal.php>

































