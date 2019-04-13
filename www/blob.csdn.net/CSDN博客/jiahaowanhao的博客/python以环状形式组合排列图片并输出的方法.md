
# python以环状形式组合排列图片并输出的方法 - jiahaowanhao的博客 - CSDN博客


2018年04月02日 22:42:35[数据分析技术](https://me.csdn.net/jiahaowanhao)阅读数：232


python以环状形式组合排列图片并输出的方法
本文实例讲述了python以环状形式组合排列图片并输出的方法。分享给大家供大家参考。具体分析如下：
这段代码可以自定义一个空白画板，然后将指定的图片以圆环状的方式排列起来，用到了pil库，可以通过：
pip install pil 的方式安装。
具体代码如下：
代码如下:
\# -*- coding: utf-8 -*-
__author__ = 'www.jb51.net'
import math
from PIL import Image
def arrangeImagesInCircle(masterImage, imagesToArrange):
imgWidth, imgHeight = masterImage.size
\#we want the circle to be as large as possible.
\#but the circle shouldn't extend all the way to the edge of the image.
\#If we do that, then when we paste images onto the circle, those images will partially fall over the edge.
\#so we reduce the diameter of the circle by the width/height of the widest/tallest image.
diameter = min(
imgWidth  - max(img.size[0] for img in imagesToArrange),
imgHeight - max(img.size[1] for img in imagesToArrange)
)
radius = diameter / 2
circleCenterX = imgWidth  / 2
circleCenterY = imgHeight / 2
theta = 2*math.pi / len(imagesToArrange)
for i in range(len(imagesToArrange)):
curImg = imagesToArrange[i]
angle = i * theta
dx = int(radius * math.cos(angle))
dy = int(radius * math.sin(angle))
\#dx and dy give the coordinates of where the center of our images would go.
\#so we must subtract half the height/width of the image to find where their top-left corners should be.
pos = (
circleCenterX + dx - curImg.size[0]/2,
circleCenterY + dy - curImg.size[1]/2
)
masterImage.paste(curImg, pos)
img = Image.new("RGB", (500,500), (255,255,255))
\#下面的三个图片是3个 50x50 的pngs 图片，使用了绝对路径，需要自己进行替换成你的图片路径
imageFilenames = ["d:/www.jb51.net/images/1.png", "d:/www.jb51.net/images/2.png", "d:/www.jb51.net/images/3.png"] * 5
images = [Image.open(filename) for filename in imageFilenames]
arrangeImagesInCircle(img, images)
img.save("output.png")
希望本文所述对大家的Python程序设计有所帮助。

