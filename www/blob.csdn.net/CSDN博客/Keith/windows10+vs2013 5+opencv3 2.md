# windows10+vs2013/5+opencv3.2 - Keith - CSDN博客





2017年04月30日 00:56:28[ke1th](https://me.csdn.net/u012436149)阅读数：1295








# window下配置vc3.2

由于网上已经有了关于如何在window配置opencv3.2的资料，对于重复的事情，本博客就直接引用，并对自己配置时碰到的问题做个总结。

## 配置

这篇博客[http://www.cnblogs.com/zangdalei/p/5339316.html](http://www.cnblogs.com/zangdalei/p/5339316.html)对于vs2013和vs2015都适用，但有几个问题需要注意
- vs2013可能在属性管理器中没有`Debug|x64`和`Release|x64`，碰到这种情况时，点击`配置管理器->活动平台解决方案->x64` 然后就会出现`Debug|x64`和`Release|x64`了。看下图。 
![这里写图片描述](https://img-blog.csdn.net/20170430004355657?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170430004413091?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQzNjE0OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
- 
如果opencv的版本为3.2.*的话，那么`连接器->输入->附加依赖项`中添加的文件名应该为`opencv_world320d.lib`（debug），`opencv_world320.lib`(release).

- 
在运行opencv程序的时候可能会碰到`xxx.dll`文件丢失.这时应该到[https://www.microsoft.com/zh-CN/download/details.aspx?id=48145](https://www.microsoft.com/zh-CN/download/details.aspx?id=48145)(vs2015), [https://www.microsoft.com/zh-CN/download/details.aspx?id=40784](https://www.microsoft.com/zh-CN/download/details.aspx?id=40784)(vs2013) 下载和自己平台对应的运行库，然后双击安装。

- 
运行程序时，还会出`无法查找或打开 PDB 文件`，这问题不影响程序运行。可以不用关心。

- 
`OpenCV Error: Assertion failed (size.width>0 && size.height>0) in cv::imshow` 如果出现这种错误，那就是说明传入`imshow`中的图片是空的。


## 参考资料

[http://blog.csdn.net/hellousb2010/article/details/38052825](http://blog.csdn.net/hellousb2010/article/details/38052825)

[http://www.cnblogs.com/zangdalei/p/5339316.html](http://www.cnblogs.com/zangdalei/p/5339316.html)




