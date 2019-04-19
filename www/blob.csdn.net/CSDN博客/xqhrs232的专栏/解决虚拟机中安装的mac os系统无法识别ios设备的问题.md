# 解决虚拟机中安装的mac os系统无法识别ios设备的问题 - xqhrs232的专栏 - CSDN博客
2018年12月12日 16:30:34[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：218
原文地址::[https://blog.csdn.net/jiuluguoshidai/article/details/51583684](https://blog.csdn.net/jiuluguoshidai/article/details/51583684)
相关文章
1、我的iphone不能被虚拟机识别怎么办----[https://jingyan.baidu.com/article/f0e83a25877bcf22e59101ab.html](https://jingyan.baidu.com/article/f0e83a25877bcf22e59101ab.html)
2、虚拟机MAC OS X连接不上iphone或ipad的问题解决----[https://www.aliyun.com/jiaocheng/402655.html](https://www.aliyun.com/jiaocheng/402655.html)
3、mac虚拟机不能识别 iphone或u盘----[https://blog.csdn.net/u011872945/article/details/38416155](https://blog.csdn.net/u011872945/article/details/38416155)
之前没有在此黑苹果上编程，只是连接过U盘，U盘是可以识别的，但是连接我的ipad的时候却无法识别。搜索百度无果，只有翻墙搜万能的谷歌。辛亏英文还是混的过去的。经过搜索比较得出解决办法：
1、邮件系统，点击最下方的设置。
2、然后选到usb控制器，将usb兼容从USB3.0 调到USB2.0。点击确定。
（注意：更改设置时系统应该是关机状态，否则更改无效）
3、然后重启系统，完美解决。
虚拟机装mac os编写程序其实很快了，但是你的硬盘需要时固态，机械硬盘会很慢。
