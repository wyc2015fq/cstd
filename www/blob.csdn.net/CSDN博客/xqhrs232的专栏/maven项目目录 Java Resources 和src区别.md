# maven项目目录 Java Resources 和src区别 - xqhrs232的专栏 - CSDN博客
2018年10月30日 14:40:06[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：74
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/wang_sht/article/details/79171796](https://blog.csdn.net/wang_sht/article/details/79171796)
相关文章
1、Java Resources是什么----[https://blog.csdn.net/rui_xiao/article/details/80013259?utm_source=blogxgwz1](https://blog.csdn.net/rui_xiao/article/details/80013259?utm_source=blogxgwz1)
2、maven项目下目录结构源文件src/main/java(src/main/resources)与src文件有什么差别？----[https://blog.csdn.net/qq_38941937/article/details/81026462?utm_source=blogxgwz0](https://blog.csdn.net/qq_38941937/article/details/81026462?utm_source=blogxgwz0)
使用maven管理java web项目时，我们往往会被eclipse上展示的目录结构所迷惑，明明有个Java Resources包，为啥又冒出个src文件夹呢？
我们先看看整体的目录结构：
经过对比，可以发现src下多了一个webapp文件夹，这个文件夹里存放着项目所有页面资源。
项目右键>Properties>Deployment Assembly。
可以看到src/main/java，src/main/resources，src/test/java目录编译后的部署路径都是
WEB-INF/classes，而编译前WEB-INF的父目录就是webapp，webapp编译后的父目录又是根目录，所以最直观的体现是，编译后项目名下所有的文件夹名称其实就是编译前webapp下的所有文件夹名。而Java Resources包其实只是方便开发人员开发，特地从src包中分离出来的。
编译前的webapp目录结构
编译后的项目目录结构
而现在WEB-INF下多了原项目java resources的编译文件，就在classes文件夹中
--------------------- 
作者：wang_sht 
来源：CSDN 
原文：https://blog.csdn.net/wang_sht/article/details/79171796 
版权声明：本文为博主原创文章，转载请附上博文链接！
