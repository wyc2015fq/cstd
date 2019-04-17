# 我的python学习笔记(2) - GDAL与Python - 长歌行 - CSDN博客





2015年09月26日 21:16:13[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：3377
所属专栏：[Python与GIS](https://blog.csdn.net/column/details/pygis.html)









可以使用Python来调用GDAL

GDAL相关库下载地址： 
[http://www.gisinternals.com/release.php](http://www.gisinternals.com/release.php)
- 
根据自己的编译器与平台选择对于的release版本 
![这里写图片描述](https://img-blog.csdn.net/20150926210728083)- 
下载GDAL核心库，并安装 
![这里写图片描述](https://img-blog.csdn.net/20150926210830479)
一般会安装到这个路径，x86与x64略有不同 

C:\Program Files\GDAL
- 
下载GDAL Python依赖库，并安装 
![这里写图片描述](https://img-blog.csdn.net/20150926210958435)
一般会安装到这个路径C:\Python27\Lib\site-packages
- 
将GDAL的包括动态库和exe的目录加入到系统变量的Path中（可不加）

- 
在环境变量中创建“GDAL_DATA”并加入gdal-data的路径，例如C:\Program Files\GDAL\gdal-data。里面主要是一些空间参考系的描述。

- 
打开对于的python， 输入import ogr测试










