# imageJ二次开发之旅 – imageJ开源项目介绍 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2015年12月01日 14:57:18[gloomyfish](https://me.csdn.net/jia20003)阅读数：8456标签：[JAVA																[imageJ																[图像处理](https://so.csdn.net/so/search/s.do?q=图像处理&t=blog)
个人分类：[图像处理](https://blog.csdn.net/jia20003/article/category/879324)





**imageJ二次开发之旅 – imageJ开源项目介绍**

**imageJ起源：**

imageJ项目最初是由NIH（NationalInstitutes of Health,美国国家卫生研究院）在1997发起的，其目的是提供一个跨平台、高效、稳定可运行的Java图像处理工具。经过多年发展在imageJ的基础上已经孕育出多个分支开源项目包括imageJ1.x、imageJ2.x、Fiji等成功的开源项目，

![](https://img-blog.csdn.net/20151201143737816)




imageJ的产生是地地道道的一个人的项目产物。最初的开发者只有一个人Wayne Rasband，他为美国国家卫生研究院做了40年的码农，此君已经于2010退休了。而imageJ则通过开源社区不断贡献出新插件从而功能更加的丰富、实用性更强，已经获得一大批的忠实用户与追随者。




**ImageJ项目特征：**

ImageJ能够走到今天，得到图像处理开发者认可与使用，主要原因在于开放源代码与插件机制，其中imageJ的插件机制允许开发者自己开发相关算法并集成到已经存在的imageJ应用程序中，这样就减轻了算法开发者的工作量同时避免了很多重复劳动。另外一个重要机制就是宏(macros)，通过宏机制，根据脚本语言，imageJ就可以借助分布式计算发挥Java跨平台的强大运算能力，特别适合处理大型多维图像。此外imageJ由于是早期的Java语言开发GUI都是借助于awt组件完成，跟现在的java swing有点格格不入，所以imageJ迁移到GitHub上之后有产生了imagej-swing的项目分支。如果GUI都能换成swing的，我相信imageJ将会在更多的项目中得到集成运用。imageJ项目主页：http://imagej.net/Home。同时ImageJ支持多种图像格式文件的IO读写，是一个非常好的图像文件读写支持库。所以imageJ主要特征可以总结如下：

-         基于插件机制调用所有的图像处理算法程序，实现了UI层与算法计算的分离

-         强大的插件功能与宏脚本定义功能

-         基于AWT的GUI库。

-         支持几乎所有常见的图像格式IO读写



**imageJ源代码下载与运行**

访问地址[https://github.com/imagej](https://github.com/imagej)选择ImageJA主页之后选择downloadzip即可，如果你已经安装Eclipse4.0以上版本，就可以选择maven项目之后直接导入解压缩的源代码即可，在IDE中直接运行ij包下的ImageJ.java即可。显示出现的界面如下：

![](https://img-blog.csdn.net/20151201145127053)




**点击文件菜单打开一张图像，显示如下：**
![](https://img-blog.csdn.net/20151201145222756)

选择[Process]->[Filters]-[Gaussian Blur](高斯模糊)，弹出高斯参数输入对话框，直接按确认即可得到模糊之后的图像如下：
![](https://img-blog.csdn.net/20151201145314421)

**完整的导入工程目录结构如下：**


![](https://img-blog.csdn.net/20151201145407965?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)](https://so.csdn.net/so/search/s.do?q=imageJ&t=blog)](https://so.csdn.net/so/search/s.do?q=JAVA&t=blog)




