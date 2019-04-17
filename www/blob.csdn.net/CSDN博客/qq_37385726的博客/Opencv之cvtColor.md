# Opencv之cvtColor - qq_37385726的博客 - CSDN博客





2018年08月23日 11:47:27[啧啧啧biubiu](https://me.csdn.net/qq_37385726)阅读数：206
所属专栏：[Opencv](https://blog.csdn.net/column/details/26515.html)











转载自[这里](https://blog.csdn.net/on2way/article/details/46801063)





## API Definition

我们从 OpenCV官网 的[Miscellaneous Image Transformations](http://docs.opencv.org/2.4/modules/imgproc/doc/miscellaneous_transformations.html) 上，可查到 **cv2.cvtColor** 这个api的定义如下：

> 
### cvtColor

Converts an image from one color space to another.

**C++**: void cvtColor(InputArray src, OutputArray dst, int code, int dstCn=0 ) 
**Python**: cv2.cvtColor(src, code[, dst[, dstCn]]) → dst 
**C**: void cvCvtColor(const CvArr* src, CvArr* dst, int code) 
**Python**: cv.CvtColor(src, dst, code) → None

**Parameters**: 
**src** – input image: 8-bit unsigned, 16-bit unsigned ( CV_16UC… ), or single-precision floating-point. 
**dst** – output image of the same size and depth as src. 
**code** – color space conversion code (see the description below). 
**dstCn** – number of channels in the destination image; if the parameter is 0, the number of the channels is derived automatically from src and code.

The function converts an input image from one color space to another. In case of a transformation to-from RGB color space, the order of the channels should be specified explicitly (RGB or BGR). Note that the default color format in OpenCV is often referred to as RGB but it is actually BGR (the bytes are reversed). So the first byte in a standard (24-bit) color image will be an 8-bit Blue component, the second byte will be Green, and the third byte will be Red. The fourth, fifth, and sixth bytes would then be the second pixel (Blue, then Green, then Red), and so on.


## 参数探究

在探究的过程中，我发现 `code` 参数的 **输入类型** 是 `int` 型，于是写代码进行验证：

```python
import cv2
color_types = [cv2.COLOR_BGR2RGB, cv2.COLOR_BGR2GRAY]
for color_type in color_types:
    print ('{}    {}'.format(color_type, type(color_type)))
```

结果证明了，原来 `code` 参数的 **输入** 不管是`cv2.COLOR_BGR2RGB`、`cv2.COLOR_BGR2GRAY`，或是其他 **颜色转换空间（color space conversion）**，均是 `int` 型的：

```python
<type 'int'>
<type 'int'>
```

## 颜色空间转换探究

于是我另外编写了一小段代码，探究哪些整数可以作为 **cv2.cvtColor** 中 `code` 参数的 **替代输入值** ，并看看在 **转换了颜色空间** 后，会生成什么样的图像。

（自己写的实验源码附在文章末尾）

验证得知，以下整数可以作为 **cv2.cvtColor** 中 `code` 参数的 **替代输入值**：

```python
Valid index in cv2.cvtColor:
[0, 1, 2, 3, 4, 5, 6, 7, 10, 11, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 44, 45, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 127, 128, 129, 130, 131, 132, 133, 134]
```

## 效果图

### 原图像

在进行 **转换颜色空间** 之前的原图（./pic/origin_pic.jpg）： 
![这里写图片描述](https://img-blog.csdn.net/20170830204528255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## 生成的图像

./generated_pics/1.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830204603685?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/2.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205004014?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/6.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205103648?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/32.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205124780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/34.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205144106?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/35.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205208584?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/36.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205241457?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/38.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205304118?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/41.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205319780?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/53.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205348257?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/54.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205404380?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/55.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205423063?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/69.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205613476?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/72.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205649366?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/73.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205708804?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/79.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205736636?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/82.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205803464?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

./generated_pics/85.jpg： 
![这里写图片描述](https://img-blog.csdn.net/20170830205844428?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSk5pbmdXZWk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## Code

附上自己写的实验代码：

```python
# coding=utf-8

origin_pic = './pic/origin_pic.jpg'
save_folder = './generated_pics'

import os
try:
    os.makedirs(save_folder)
except OSError:
    pass

import cv2
img = cv2.imread(origin_pic)
valid_index = []
for color_type in range(-300, 1000, 1):
    try:
        img_new = cv2.cvtColor(img, color_type)
        cv2.imwrite(os.path.join(save_folder, str(color_type)+'.jpg'), img_new)
        valid_index.append(color_type)
    except:
        pass
print ('Valid index in cv2.cvtColor:\n{}'.format(valid_index))
```



