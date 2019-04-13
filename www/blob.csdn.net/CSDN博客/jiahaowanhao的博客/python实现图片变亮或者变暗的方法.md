
# python实现图片变亮或者变暗的方法 - jiahaowanhao的博客 - CSDN博客


2018年01月30日 19:56:59[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：1832


[python实现图片变亮或者变暗的方法](http://cda.pinggu.org/view/24603.html)
这篇文章主要介绍了python实现图片变亮或者变暗的方法,涉及Python中Image模块操作图片的相关技巧,分享给大家供大家参考。具体实现方法如下：
import Image
\# open an image file (.jpg or.png) you have in the working folder
im1 = Image.open("angelababy.jpg")
\# multiply each pixel by 0.9 (makes the image darker)
\# works best with .jpg and .png files, darker < 1.0 < lighter
\# (.bmp and .gif files give goofy results)
\# note that lambda is akin to a one-line function
im2 = im1.point(lambda p: p * 0.5)
\# brings up the modified image in a viewer, simply saves the image as
\# a bitmap to a temporary file and calls viewer associated with .bmp
\# make certain you have associated an image viewer with this file type
im2.show()
\# save modified image to working folder as Audi2.jpg
im2.save("angelababy2.jpg")
运行效果如下所示：
![](http://cda.pinggu.org/uploadfile/image/20180130/20180130065131_26363.png)
希望本文所述对大家的Python程序设计有所帮助。

