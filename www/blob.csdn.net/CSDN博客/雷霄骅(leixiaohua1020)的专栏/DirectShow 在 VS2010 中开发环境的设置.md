# DirectShow 在 VS2010 中开发环境的设置 - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年10月16日 15:40:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：36
个人分类：[DirectShow](https://blog.csdn.net/leixiaohua1020/article/category/1645657)










心血来潮想玩玩DirectShow，在网上查了很多资料都是讲VC6的，所以照着资料慢慢配置。大致过程和VC6下的是一样的，但差别还是有的。



具体过程可以参照这篇文章：[在vs2005中配置directshow开发环境](http://blog.163.com/wuyuanhere@126/blog/static/173323728201181910123054/)



由于现在directShow没有和direcxtx一起发布，而是和windows sdk 打包发布了，可以到官网下载最新的windows sdk 开发包。

windows sdk 地址：[http://www.microsoft.com/download/en/details.aspx?id=8442](http://www.microsoft.com/download/en/details.aspx?id=8442)

按系统类型下载。



下载安装好之后编译baseclasses项目，在C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses。

选择不同的编译环境可以得到四个类型的strmbasd.lib，开发项目时选择相应版本的strmbasd.lib。

在编译的时候没错误提示，看来选择新版本还是有好处的。



在项目中包含include：

C:\Program Files\Microsoft SDKs\Windows\v7.1\Include

C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses



以及lib：

C:\Program Files\Microsoft SDKs\Windows\v7.1\Lib

C:\Program Files\Microsoft SDKs\Windows\v7.1\Samples\multimedia\directshow\baseclasses\Debug（这个要根据所需要的strmbasd.lib版本进行选择）



最后提示一下大小写问题，像WinMM.Lib这种库文件在link的时候要分清大小写，后面的Lib也是，不然会出现link error的错误，当时我就找了找了很久才发现这个问题。网上关于directShow配置的文章都用的小写，copy进去后就出现了错误。不知道是以前库的名字都是小写的还是vc版本的问题，害死人啊。



原文地址：[http://blog.163.com/wuyuanhere@126/blog/static/1733237282011819111560/](http://blog.163.com/wuyuanhere@126/blog/static/1733237282011819111560/)




