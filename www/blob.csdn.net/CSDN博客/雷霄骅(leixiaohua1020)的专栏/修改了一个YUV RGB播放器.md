# 修改了一个YUV/RGB播放器 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2016年01月06日 00:21:26[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：39921标签：[YUV																[RGB																[像素数据																[YUV播放器																[开源](https://so.csdn.net/so/search/s.do?q=开源&t=blog)
个人分类：[我的开源项目](https://blog.csdn.net/leixiaohua1020/article/category/1843731)




最近在学习过程中查看YUV/RGB像素数据的时候，发现找不到一个合适的播放器。主流的YUV播放器大多只支持YUV格式播放，却不支持RGB格式数据播放。而我正好需要查看RGB像素数据。在Sourceforge上找到了一个比较好用的既支持YUV又支持RGB的播放器，但是发现有一些小毛病，于是把这个播放器源代码下载下来并且做了一些修改，感觉好用了很多。在这里写文章记录一下。
该播放器使用比较简单，可以通过菜单栏打开像素数据文件，也可以通过拖拽方式打开文件。如果文件名称中包含了“{w}x{h}”这样的字符串（例如“test_320x420.yuv”），系统会自动解析为该像素数据的宽和高。

![](https://img-blog.csdn.net/20160106001409609)

软件在原版的基础上做了如下修改：

> (1) 修正了2处内存泄露。
(2) 在“打开文件”对话框中添加“RGB类型”，“所有类型”两种文件类型。
(3) 从文件名中自动解析宽高信息。文件名格式应该是“XXX_{width}x{height}.yuv”，例如“test_1920x1080.yuv”。
(4) 添加中文语言
(5) 添加了8:1放大


**YUV Player**


原版项目地址：[https://sourceforge.net/projects/raw-yuvplayer/](https://sourceforge.net/projects/raw-yuvplayer/)

修改版源码地址：[https://github.com/leixiaohua1020/YUVplayer](https://github.com/leixiaohua1020/YUVplayer)

编译后可以直接运行的程序：[http://download.csdn.net/detail/leixiaohua1020/9391876](http://download.csdn.net/detail/leixiaohua1020/9391876)](https://so.csdn.net/so/search/s.do?q=YUV播放器&t=blog)](https://so.csdn.net/so/search/s.do?q=像素数据&t=blog)](https://so.csdn.net/so/search/s.do?q=RGB&t=blog)](https://so.csdn.net/so/search/s.do?q=YUV&t=blog)




