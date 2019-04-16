# Python图像处理基本操作 - 白马负金羁 - CSDN博客





2019年03月14日 06:20:12[白马负金羁](https://me.csdn.net/baimafujinji)阅读数：720








在Python中进行图像处理可以使用的库有很多，本文主要介绍下面三个：
- OpenCV（Open Source Computer Vision Library）
- PIL（Python Imaging Library）
- skimage（scikit-image）

*如下代码的实验环境为Jupyter Notebook.

欢迎关注白马负金羁的博客 [http://blog.csdn.net/baimafujinji](http://blog.csdn.net/baimafujinji)，为保证公式、图表得以正确显示，强烈建议你从该地址上查看原版博文。本博客主要关注方向包括：数字图像处理、算法设计与分析、数据结构、机器学习、数据挖掘、统计分析方法、自然语言处理。

## 1. OpenCV

OpenCV是图像处理中最强大的一个库，它的源代码是由C\C++写成的，所以原版的OpenCV可以与C、C++无缝结合。Python版的OpenCV主要依赖于cv2这个包来实现。

### 1.1  imread()

```python
import cv2
import numpy as np

#读入图片：默认彩色图，cv2.IMREAD_GRAYSCALE灰度图，cv2.IMREAD_UNCHANGED包含alpha通道
img = cv2.imread('Lena.png')
print(img.shape)
print(type(img))
```

上述代码的执行结果如下：

(512, 512, 3)
<class 'numpy.ndarray'>

此时，被读入的图像以ndarray格式存在，取值范围是 [0, 255]。

### 1.2  imshow()

```python
cv2.imshow('Lena',img)
cv2.waitKey(0)
cv2.destroyAllWindows()
```

函数imshow()用于显示图像，但只使用它并不会得到任何结果，必须结合后面的waitKey(delay)，参数delay表示延迟多少毫秒。默认情况为0。当delay≤0，可以理解为延迟无穷大毫秒。调用destroyAllWindows()函数可以释放由 OpenCV创建的所有窗口。注意上面三条语句必须写在Jupyter notebook中的同一Cell里，然后按任意键，系统会关闭图像显示窗口。

### 1.3  imwrite()

```python
cv2.imwrite('lena.jpg',img)
```

函数imwrite()用于存储图像，第一个参数是file name，第二个参数是要存储的图像对象。



参考链接[【点击链接】](https://opencv-python-tutroals.readthedocs.io/en/latest/py_tutorials/py_gui/py_image_display/py_image_display.html)

欢迎关注白马负金羁的博客 [http://blog.csdn.net/baimafujinji](http://blog.csdn.net/baimafujinji)，为保证公式、图表得以正确显示，强烈建议你从该地址上查看原版博文。本博客主要关注方向包括：数字图像处理、算法设计与分析、数据结构、机器学习、数据挖掘、统计分析方法、自然语言处理。

## 2. PIL

Python里面自带一个PIL（python images library), 但这个库现在已经停止更新了，所以使用Pillow, 它是由PIL发展而来的。

### 1.1  open()

```python
from PIL import Image
import numpy as np

img = Image.open('Lena.png')
print(img.size)
print(img.mode)
print(type(img))
```

上述代码的执行结果如下：

(512, 512)
RGB
<class 'PIL.PngImagePlugin.PngImageFile'>

### 1.2 show()

下面的代码可以用来显示图像，程序会弹出一个单独的图像窗口。

```python
img.show()
```

除此之外，还可以使用matplotlib来绘制图像，它是一个专业绘图的库，相当于matlab中的plot，可以设置多个figure，设置figure的标题，甚至可以使用subplot在一个figure中显示多张图片。

```python
import matplotlib.pyplot as plt
%matplotlib inline

plt.imshow(img)
```

inline一句的作用是在Jupyter Notebook的网页中直接绘图，即不会单独弹出绘图窗口，上述代码的执行结果如下：

![](https://img-blog.csdnimg.cn/20190314055009111.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JhaW1hZnVqaW5qaQ==,size_16,color_FFFFFF,t_70)

如果不希望显示坐标轴，则可以使用下面的代码：

```python
plt.axis('off')
plt.imshow(img)
```

下面的代码演示了显示多行、多列图像的方法，注意其中显示灰度图像的语法。

```python
fig=plt.figure(figsize=(28, 28))
columns = 5
rows = 5
for i in range(1, columns*rows +1):
    img = test_only_success_adv[650+i].reshape(28,28)+0.5
    fig.add_subplot(rows, columns, i)
    plt.imshow(img, cmap='gray')
    plt.axis('off')
```

代码执行结果如下：

![](https://img-blog.csdnimg.cn/20190314055633579.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2JhaW1hZnVqaW5qaQ==,size_16,color_FFFFFF,t_70)

### 1.3  save()

图像保存可以使用：

```python
img.save('lena2.jpg')
```

如果要把以ndarray格式存储的矩阵保存成图像，则需要使用：

```python
im = Image.fromarray(np.uint8(data_jsma_0*255))
im.save("000.png")
```



参考链接[【点击链接】](https://pillow.readthedocs.io/en/stable/)

欢迎关注白马负金羁的博客 [http://blog.csdn.net/baimafujinji](http://blog.csdn.net/baimafujinji)，为保证公式、图表得以正确显示，强烈建议你从该地址上查看原版博文。本博客主要关注方向包括：数字图像处理、算法设计与分析、数据结构、机器学习、数据挖掘、统计分析方法、自然语言处理。

## 3. skimage



### 1.1  imread()

参考示例代码：

```python
from skimage import io
import numpy as np

img = io.imread('Lena.png')
print(img.shape) # numpy矩阵，(h,w,c)

print(type(img))
```

上述代码执行结果如下：

(512, 512, 3)
<class 'numpy.ndarray'>

被读入的图像以ndarray格式存在。

与之前类似，io.imshow() 和 io.imsave() 分别用于显示和存储图像。

### 1.2  img_as_ubyte()

该函数的作用是convert an image to 8-bit unsigned integer format，也就是把图像像素灰度的取值范围转化到0~255之间的整数。

例如，

```python
print(np.max(adv_test_cw0[0]))
print(np.min(adv_test_cw0[0]))

byte_adv_test_cw0 = img_as_ubyte(adv_test_cw0)

print(np.max(byte_adv_test_cw0[0]))
print(np.min(byte_adv_test_cw0[0]))
```

上述代码的执行结果如下：

0.9997719

0.00054621696

255

0



参考链接[【点击链接】](https://scikit-image.org/)



欢迎关注白马负金羁的博客 [http://blog.csdn.net/baimafujinji](http://blog.csdn.net/baimafujinji)，为保证公式、图表得以正确显示，强烈建议你从该地址上查看原版博文。本博客主要关注方向包括：数字图像处理、算法设计与分析、数据结构、机器学习、数据挖掘、统计分析方法、自然语言处理。

**【全文完】**



