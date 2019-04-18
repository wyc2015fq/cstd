# Noise2Noise：图像降噪，无需干净样本，原因非常简单 - 知乎
# 



最近nVidia几个月前的一篇文章突然火了：
[https://arxiv.org/pdf/1803.04189.pdf​arxiv.org](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1803.04189.pdf)
这个文章写了很长，其实它说的事情非常简单。

本来我们做图像降噪，需要输入的噪音图像 x，和 “干净样本” y。

例如，x 是路径跟踪渲染用少数光束渲染的图片，y 是长期渲染后的图片。那么如果用 y 作为训练目标，生成 y 是个非常费时费力的过程。

但其实你如果仔细想想，可以用另一次快速渲染生成的另一个噪音图像（它相当于 y + 另一个不同的噪音）作为训练目标（所以叫Noise2Noise）。只要训练样本够多，最终也相当于用 y 作为训练目标。原因是简单的统计学原理。

下面是效果，可见用另一个噪音图像作为训练目标 (b)，效果和用干净图像作为训练目标 (c) 最终的效果几乎一样好。
![](https://pic3.zhimg.com/v2-eb5d20e56786071b370a346cc7b256ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1507' height='385'></svg>)
而且训练过程和速度也几乎一样，例如：
![](https://pic1.zhimg.com/v2-c033a46c0b6cfa332a8b6fca610ef3ac_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='492' height='281'></svg>)
严格说来，这个需要两次独立的观测，还是没有 Deep Image Prior 神奇，当然它的运行速度比 Deep Image Prior 快多了。


