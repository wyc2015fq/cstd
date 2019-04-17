# Python利用OpenCV来进行图片的位移和缩放 - zhusongziye的博客 - CSDN博客





2018年04月01日 18:51:53[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：335








                利用OpenCV来进行图片的位移
环境：Windows

python版本：3.6

安装anaconda，下载地址：https://www.anaconda.com/download/

![](https://img-blog.csdn.net/20180401184914484?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

32位电脑就选择32位，64位就选择64位。

安装之后，要下载OpenCV

![](https://img-blog.csdn.net/20180401184952120?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
代码部分
![](https://img-blog.csdn.net/20180401185004938?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

下面是用源代码实现图片的位移

![](https://img-blog.csdn.net/2018040118503096?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
效果图
![](https://img-blog.csdn.net/2018040118504183?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
利用OpenCV来进行图片的缩放
主要有四个步骤，

1.加载缩放的图片

2.获取图片信息

3.调用resize方法

4.检查最终结果

代码部分






![](https://img-blog.csdn.net/20180401185110100?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




最终效果




![](https://img-blog.csdn.net/20180401185123504?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




![](https://img-blog.csdn.net/20180401185144257?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3podXNvbmd6aXll/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



