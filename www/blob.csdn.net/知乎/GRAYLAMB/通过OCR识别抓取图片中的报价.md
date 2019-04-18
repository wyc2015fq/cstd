# 通过OCR识别抓取图片中的报价 - 知乎
# 

我们还是采用无所不能的Python来进行价格抓取。
主要思路是：截屏--》OCR识别截屏图片中的价格，将价格数据变成文本

截屏与OCR识别，需要用到Python的两个库，一个是PIL（里面主要是图形处理的方法），一个是pytesseract（主要是OCR处理的方法）。

其中Python会自带PIL库，因此可以直接import其中的方法。但是pytesseract则需要先安装再使用。因此，首先我们来介绍pytesseract的安装过程。

先运行：sudo pip install pytesseract语句，完成安装动作。

![](https://pic3.zhimg.com/a50821a0db57920749f5824afa1a44ce_b.png)然后会惊喜的发现: import pytesseract成功

然后我们开始测试pytesseract的功能：

找到一张很简单的带报价的图片 1.jpg：
![](https://pic2.zhimg.com/321bef2b3da4a62559aa0ba5e5bbfeb1_b.png)
为了在Python中打开这个图片，需要从PIL中引入Image，然后用Image.Open打开图片。打开后，用pytesseract库中的pytesseract.image_to_string进行OCR。

```
import pytesseract
from PIL import Image
image=Image.open('/路径/1.jpg')
price=pytesseract.image_to_string(image)
print price
```

问题来了，执行到price=pytesseract.image_to_string(image)的时候居然报错了：

```
Traceback (most recent call last):
  File "<stdin>", line 1, in <module>
  File "/Users/graylamb/anaconda2/lib/python2.7/site-packages/pytesseract/pytesseract.py", line 161, in image_to_string
    config=config)
  File "/Users/graylamb/anaconda2/lib/python2.7/site-packages/pytesseract/pytesseract.py", line 94, in run_tesseract
    stderr=subprocess.PIPE)
  File "/Users/graylamb/anaconda2/lib/python2.7/subprocess.py", line 710, in __init__
    errread, errwrite)
  File "/Users/graylamb/anaconda2/lib/python2.7/subprocess.py", line 1335, in _execute_child
    raise child_exception
OSError: [Errno 2] No such file or directory
```

这个问题困扰了我一阵子，后来仔细阅读了pytesseract的文档，发现还必须安装google的tesseract-ocr才可以：

![](https://pic2.zhimg.com/30c934c10ea577b372af578ba0ed055d_b.png)google已经被墙了，无法下载安装包。无奈下求助了GitHub，找到了MacOS下的brew的安装方式，太赞了！

![](https://pic3.zhimg.com/4fd9d9333c2c60878797b82e0881f4fa_b.png)在Mac下输入brew install tesseract直接安装解决问题！

![](https://pic4.zhimg.com/b0581566b0f49b3b38d252b5dde02797_b.png)安装好以后，再执行刚才的语句：

```
>>> price=pytesseract.image_to_string(image)
>>> print price
XAG/USD 17.218
```

搞定！

下面就是截屏了，截屏主要靠PIL的ImageGrab。

做法很简单：

```
>>> im=ImageGrab.grab()
>>> im
<PIL.PngImagePlugin.PngImageFile image mode=RGBA size=1366x768 at 0x10313CD50>
```

这时，im已经是截屏的内容了。

最牛的是，ImageGrab.grab()可以指定截屏区域。比如屏幕中已经出现了报价：
![](https://pic4.zhimg.com/5b9747200404c8c9169f030ba2cc7203_b.png)

这时你可以输入报价区域的像素位置：

```
>>> im=ImageGrab.grab((43,427,360,455))
>>> price=pytesseract.image_to_string(im)
>>> print price
XAG/USD 17.200 17.246
```

完美~

(完)

