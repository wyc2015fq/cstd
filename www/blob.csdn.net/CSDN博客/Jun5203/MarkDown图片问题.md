# MarkDown图片问题 - Jun5203 - CSDN博客
2018年06月28日 14:47:19[贾俊浩_Ellen](https://me.csdn.net/Ellen5203)阅读数：490
## 前言
 我们在写博客的时候经常会插入图片来使博客一目了然，但是图片的大小往往会影响美观，图片太大，影响阅读的心情；图片太小，又显得过于乏味。下面给大家分享一个如何设置图片的小窍门，希望大家能受用。
## 过程
## ● 插入图片，设置大小
1、MarkDown插入图片快捷键Ctrl+G； 
2、上传图片后会出现如下图； 
![](https://img-blog.csdn.net/20180628142320602)
3、这时我们剪切其中的一部分，如下图； 
![](https://img-blog.csdn.net/20180628142450676)
4、固定图片显示大小，输入以下字符
```xml
<img src="https://img-blog.csdn.net/20180628142152770" width=60% height=50% />
```
其中src后面接的就是图像对象，width和height设置的是显示图像的尺寸。 
![](https://img-blog.csdn.net/20180628142152770)
## ● 如果想给图像价格标注，方法如下：
```xml
<center>
<img src="https://img-blog.csdn.net/20180628142152770" width=60% height=50% />
Figure 1. Lena
</center>
```
![](https://img-blog.csdn.net/20180628142152770)
Figure 1. Lena 
## ● 如果想让图片和标注间距离增大，方法如下：
```xml
<center>
<img src="https://img-blog.csdn.net/20180628142152770" width=60% height=50% />
$ $
Figure 1. Lena
</center>
```
![](https://img-blog.csdn.net/20180628142152770)
Figure 1. Lena 
## ● 图片位置——居左、居中、居右
![](https://img-blog.csdn.net/20180628142152770)
```xml
<div align=left><img src="https://img-blog.csdn.net/20180628142152770" width=60% height=50% />
```
![](https://img-blog.csdn.net/20180628142152770)
```xml
<div align=center><img src="https://img-blog.csdn.net/20180628142152770" width=60% height=50% />
```
![](https://img-blog.csdn.net/20180628142152770)
```xml
<div align=right><img src="https://img-blog.csdn.net/20180628142152770" width=60% height=50% />
```
## 后记
 顺便补充个首行缩进的小窍门` `
希望大家引用哈，有什么需要改进的地方，还需大神指教！
