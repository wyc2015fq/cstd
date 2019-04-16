# Java 开源 CMS ：magnolia - 雷霄骅(leixiaohua1020)的专栏 - CSDN博客





2013年12月26日 00:09:00[雷霄骅](https://me.csdn.net/leixiaohua1020)阅读数：74









Magnolia 是一个开源基于Java的Web内容管理系统(CMS)，构建在Java内容知识库标准(JSR-170)。在使用它的过程中，我发现它的界面确实很有特色：给人一种Win8的感觉。在此记录一下它的使用方法。

注：查了一下Magnolia的意思，木兰的一种。

![](https://img-blog.csdn.net/20131226121411562)

首先记录一下安装方法，相当的简单：

1.安装JDK，设置JAVA_HOME环境变量。这是Java系统必须的一步。很多用Java开发的人可以跳过此步

2.下载Magnolia CMS Tomcat bundle压缩文件

3.解压缩zip后，运行magnolia\tomcat\bin目录下的startup.bat就可以了

服务器启动后，在浏览器里输入http://localhost:8080/。如果安装成功的话，会出现欢迎画面：

![](https://img-blog.csdn.net/20131226121449234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

先看一看位于右下角的Public Instance。有两个连接：

Visit>superuser，密码superuser

先来看看管理员页面，相当有Win8的风格，还有动画效果。

![](https://img-blog.csdn.net/20131226121458375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20131226121501718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

普通用户的页面如下图所示（有些地方的内容被我修改了）：

![](https://img-blog.csdn.net/20131226121454843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

现在回到管理员页面，单击“Edit”里面的“Page”，出现如下页面。可以对页面进行各种修改。

![](https://img-blog.csdn.net/20131226121517531?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以在页面上单击右键，进行修改，很方便，就像在Windows中操作文件一样。

![](https://img-blog.csdn.net/20131226121529265?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

注意：在这里不能修改页面的内容。

如果想要修改页面的内容的话，需要回到欢迎页面http://localhost:8080/，单击左下角的Login to the Admin Central（登入后的界面和上图管理员界面是一样的）。

在页面上右键->Edit Page，皆可以跳转到编辑页面。下图显示了在“About”页面的编辑页面。

![](https://img-blog.csdn.net/20131226121535687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

可以在页面里添加Component。在每个Component的右上角有一个铅笔形状的按钮，单击该按钮可以编辑该Component的内容，如下图所示：

![](https://img-blog.csdn.net/20131226121825234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGVpeGlhb2h1YTEwMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



暂时先记录到这里。

magnolia官网：[http://www.magnolia-cms.com/](http://www.magnolia-cms.com/)








