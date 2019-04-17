# Coursera视频无法观看的三种不同解决方法（亲测有效） - Machine Learning with Peppa - CSDN博客





2018年01月11日 23:27:05[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：61758
所属专栏：[用Python玩转数据](https://blog.csdn.net/column/details/18811.html)









最近在coursera上课时出现了视频黑屏，网页缓冲，无法观看等问题，经过查询发现很多人也有同样的问题。对于不同的原因，一般来说解决方法也不同。这里有三种办法，大家可以挨个尝试，肯定有一个能用。




### 1. 浏览器原因

在win7和部分WIN10上用的chrome会出现黑屏现象。有两个办法：
- 可以换浏览器。Chrome自从22开始，在win7上的html5视频就放不了了。
- 将鼠标放在右上方你的名字上，等下拉菜单出现后，选择course preferences。在video player下选择flash player。







### 2.网络原因

这种原因占了大多数，基本的方法就是改HOST






1. **用管理员权限记事本打开host文件**

很多同学问hosts是啥，在哪。看下图(路径就是图片上的)

![](https://img-blog.csdn.net/20180313143434563?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzk1MjE1NTQ=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)








2. **将如下内容复制到文件末尾**


```
52.84.246.90    d3c33hcgiwev3.cloudfront.net
52.84.246.252    d3c33hcgiwev3.cloudfront.net
52.84.246.144    d3c33hcgiwev3.cloudfront.net
52.84.246.72    d3c33hcgiwev3.cloudfront.net
52.84.246.106    d3c33hcgiwev3.cloudfront.net
52.84.246.135    d3c33hcgiwev3.cloudfront.net
52.84.246.114    d3c33hcgiwev3.cloudfront.net
52.84.246.90    d3c33hcgiwev3.cloudfront.net
52.84.246.227    d3c33hcgiwev3.cloudfront.net
```


3. **打开命令行，输入如下命令**

`ipconfig/flushdns`




还有同学问命令行是啥，怎么打开？可以有如下两种方式选择：

1）在桌面空白，按住shift键并单击右键，会出现一系列选项，选择【用命令行打开】选项，然后输入上面的命令即可

2）按键盘上的【windows】和【R】，要同时按，然后会出现这个框，在框里输入cmd即可打开命令行窗口了。
![](https://pic3.zhimg.com/50/v2-499071f23b02cef59a66db31c6d4b668_hd.jpg)



### 3. 究极办法

如果实在懒得动手或者前面两个办法也没卵用，那就用VPN翻墙吧，顺便还能当加速器，steam无压力。

VPN工具和使用教程在我的Github：[VPN-for-Coursera](https://github.com/Y1ran/Free-VPN-for-Coursera)，目前是免费开源，大家可以直接下载使用（下载前记得star哦）

如果有问题或VPN连接失效，可以Github或者CSDN上联系我~








