# 使用TensorFlow进行简单的图像处理 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2018年01月21日 11:18:09[gloomyfish](https://me.csdn.net/jia20003)阅读数：5090








## 使用TensorFlow进行简单的图像处理

### 概述

作为计算机视觉开发者，使用TensorFlow进行简单的图像处理是基本技能，而TensorFlow在tf.image包中支持对图像的常见的操作包括：
- 亮度调整
- 对比度调整
- 饱和度调整
- 图像采样插值放缩
- 色彩空间转换
- Gamma校正
- 标准化

图像的读入与显示我们通过OpenCV来实现，这里需要注意一点，OpenCV中图像三个通道是BGR，如果你是通过tensorflow读取的话三个通过顺序是RGB。图像读取的代码如下：

1.opencv方式
`src = cv.imread("D:/vcprojects/images/meinv.png")`
2.tensorflow方式

```
jpg = tf.read_file("D:/vcprojects/images/yuan_test.png")
img = tf.image.decode_jpeg(jpg, channels=3)
```

3.使用OpenCV显示图像

```python
def show_image(image, title='input'):
    print("result : \n", image)
    cv.namedWindow(title, cv.WINDOW_AUTOSIZE)
    cv.imshow(title, image)
    cv.waitKey(0)
    cv.destroyAllWindows()
```

原图显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121110705871?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 1.放缩图像

支持三种方式，分别是临界点插值、双线性插值与双立方插值，不过我发现在使用双立方插值的时候，tensorflow处理之后图像总是会出现一些噪点，这个算不算它的BUG 

 -  tf.image.resize_nearest_neighbor # 临界点插值 

 -  tf.image.resize_bilinear # 双线性插值 

 -  tf.image.resize_bicubic # 双立方插值算法 

演示代码如下：
```
src = cv.imread("D:/vcprojects/images/meinv.png")
cv.imshow("input", src)
h, w, depth = src.shape
src = np.expand_dims(src, 0)
print(src.shape)
bi_image = tf.image.resize_bilinear(src, size=[h*2, w*2])
bi_image = tf.squeeze(bi_image)
bi_result = sess.run(bi_image)
bi_result = np.uint8(bi_result)
show_image(bi_result,"bilinear-zoom")
```

显示图像如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121110839104?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 2.图像亮度调整

图像亮度是图像基本属性之一，tensorflow支持两种方式API对图像亮度进行调整 

- tf.image.adjust_brightness 

- tf.image.random_brightness 

使用上述API的时候需要对图像进行维度添加为四维的tensor数据，完整的图像亮度调整的代码如下：
```
src = cv.imread("D:/vcprojects/images/meinv.png")
src = np.expand_dims(src, 0)
brightness = tf.image.adjust_brightness(src, delta=.5)
brightness = tf.squeeze(brightness)
result = sess.run(brightness)
result = np.uint8(result)
show_image(result, "brightness demo")
```

显示图像如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121110922591?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 3.图像对比度调整

图像对比度是图像基本属性之一，tensorflow支持两种方式API对图像对比度进行调整 

- tf.image.adjust_contrast 

- tf.image.random_contrast 

前面一种全局调整，后面一种方式是随机调整，对比度调整的代码演示如下：
```
src = cv.imread("D:/vcprojects/images/meinv.png")
src = np.expand_dims(src, 0)
contrast = tf.image.adjust_contrast(src, contrast_factor=2.2)
contrast = tf.squeeze(contrast)
result = sess.run(contrast)
result = np.uint8(result)
show_image(result, "contrast demo")
```

显示图像如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121111044077?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 4.图像gamma校正

伽玛校正就是对图像的伽玛曲线进行编辑，以对图像进行非线性色调编辑的方法，检出图像信号中的深色部分和浅色部分，并使两者比例增大，从而提高图像的对比度。相关API为： 

- tf.image.adjust_gamma 

常见gamma的取值范围为0.05~5之间，tensorflow实现gamma校正的代码演示如下：
```
src = cv.imread("D:/vcprojects/images/meinv.png")
src = np.expand_dims(src, 0)
contrast = tf.image.adjust_gamma(src, gain=1.0, gamma=4.2)
contrast = tf.squeeze(contrast)
result = sess.run(contrast)
result = np.uint8(result)
show_image(result, "gamma demo")
```

显示图像如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121111133320?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 5.图像饱和度调整

图像饱和度是图像HSV色彩空间最常见的指标之一，通过调整图像饱和度可以得到更加自然光泽的图像，tensorflow中饱和度调整的API如下：
- tf.image.adjust_saturation

常见的饱和度调整范围在0～5之间取值即可，演示代码如下：

```
src = cv.imread("D:/vcprojects/images/meinv.png")
contrast = tf.image.adjust_saturation(src, saturation_factor=2.2)
result = sess.run(contrast)
result = np.uint8(result)
show_image(result, "saturation demo")
```

这里要特别说明一下，饱和度调整不支持4D tensor对象，所以读入的RGB图像即可“`。无需再次进行维度增加操作。最终调整之后的演示图像如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121111600074?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

6.图像标准化
这个在tensorflow中对图像数据训练之前，经常会进行此步操作，它跟归一化是有区别的。归一化的图像直方图不会改变，标准化会改变图像直方图分布，标准化API如下： 

- tf.image.per_image_standardization 

图像标准化实现代码如下：
```
src = cv.imread("D:/vcprojects/images/meinv.png")
contrast = tf.image.per_image_standardization(src)
result = sess.run(contrast)
result = np.uint8(result)
show_image(result, "standardization demo")
```

演示结果如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121111525625?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

7.图像色彩空间转换
tensorflow支持常见图像色彩空间转换，包括RGB、HSV、灰度色彩空间，相关API如下： 

- tf.image.rgb_ to_hsv 

- tf.image.rgb_ to_grayscale 

- tf.image.hsv_ to_rgb 

将图像从RGB色彩空间转换到灰度空间的代码演示如下：
```
src = cv.imread("D:/vcprojects/images/meinv.png")
gray = tf.image.rgb_to_grayscale(src)
result = sess.run(gray)
result = np.uint8(result)
show_image(result, "gray - demo")
```

结果显示如下： 
![这里写图片描述](https://img-blog.csdn.net/20180121111412468?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvamlhMjAwMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
#### 小结

tensorflow中还提供一些其他的图像操作相关API，比如裁剪、填充、随机调整亮度、对比度等，还有非最大信号压制等操作，感兴趣的可以自己进一步学习。 
**欢迎关注微信公众号**
**【OpenCV学堂】**

















