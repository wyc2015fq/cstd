# 怎么得到网页上Java Applet的源码 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2012年06月06日 17:39:04[gloomyfish](https://me.csdn.net/jia20003)阅读数：6098








**怎么得到网页上Java Applet的源码**


最近看一些东西，常看到有些算法demo是Java的Applet嵌套在网页里面运行，本来想通过

浏览器缓存获取源码一读，发现原来Applet的程序可能被加载到本地的JVM去啦，从缓存中

对我来说变成不可能完成任务了。心有不甘啊，于是通过ViewSource浏览一下对应的网页源码

发现一个屡试不爽的好方法，直接把对应网页的appletjar包与前缀加上，在浏览器中直接访问该

URL之后，浏览器就会自动下载对应的jar文件，然后我反编译了一下Class文件，ok，源码可以

看啦。要不然人家怎么总说Applet不安全呢，源代码这么轻松的被人搞到。

下面看看我的图解步骤：假设我要搞得是这个网页applet源码，网页URL如下：

[http://www.apropos-logic.com/nc/FPFormats.html](http://www.apropos-logic.com/nc/FPFormats.html)



第一步：打开Chrome，输入该URL访问该页面。

第二步，在页面上右键viewsource，在html中发现内嵌入的Applet代码如下：

![](https://img-my.csdn.net/uploads/201206/06/1338975610_7046.png)


其中codebase=”./”说明Jar文件在当前路径下与该html页面目录层次相同

Archive=”FPFormats.jar” 说明该Applet运行classes文件都在FPFormats.jar里面



所以最终指向该jar文件的URL为：[http://www.apropos-logic.com/nc/FPFormats.jar](http://www.apropos-logic.com/nc/FPFormats.jar)

第三步：知道URL之后，直接在浏览器中键入，jar文件就这样被轻松下载了。

对于没有archive标签的applet，同样可以分析其html代码

![](https://img-blog.csdn.net/20131227105237375)


得到URL为：

[http://www.imageprocessingbasics.com/imageprocessingbasics/histogramspecification/HistogramSpecification.class](http://www.imageprocessingbasics.com/imageprocessingbasics/histogramspecification/HistogramSpecification.class)


第四部：使用一种Java反编译工具得到源代码，开始阅读吧。这个就不错！

![](https://img-my.csdn.net/uploads/201206/06/1338975631_8849.png)

原来还是有很多applet的代码可以被轻松下载的。

最后说：这样不好吧！
            


