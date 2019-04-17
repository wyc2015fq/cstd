# 搞定Volley，秒秒钟！ - DEVELOPER - CSDN博客





2014年04月05日 20:55:39[学术袁](https://me.csdn.net/u012827205)阅读数：2103标签：[android																[jar包打包																[volley使用](https://so.csdn.net/so/search/s.do?q=volley使用&t=blog)




由于这个新下来的项目要用到Volley，所以我学习了一下。然后把我的小小总结给大家分享一下，希望能有所用处：

首先，对于Volley的简介和使用，[点击打开链接](http://blog.csdn.net/t12x3456/article/details/9221611)

在这里还可以自定义你自己的请求方式：[点击打开链接网页](http://blog.csdn.net/ttdevs/article/details/17586205)






下面就来介绍一下使用Volley之前要做的工作：

这个Volley使用之前要有volley源码，因为Volley的使用要用到Volley源码来完成。那下面就要先下载源码了。

强调一点，在你运行的项目中的项目文件中一定要注明权限：

```java
<uses-permission android:name="android.permission.INTERNET" />
    <uses-permission android:name="android.permission.ACCESS_WIFI_STATE" />
    <uses-permission android:name="android.permission.ACCESS_NETWORK_STATE" />
    <uses-permission android:name="android.permission.READ_CONTACTS" />
    <uses-permission android:name="android.permission.WRITE_EXTERNAL_STORAGE" />
    <uses-permission android:name="android.permission.MOUNT_UNMOUNT_FILESYSTEMS" />
    <uses-permission android:name="android.permission.CHANGE_WIFI_STATE" />
    <uses-permission android:name="android.permission.CHANGE_NEWWORK_STATE" />
```


下载方法一：从git库先克隆一个下来<这种方式要安装git插件>：

```java
git clone https://android.googlesource.com/platform/frameworks/volley
```
然后编译为jar包，再把jar包放到自己的工程的libs目录。然后就可以随意使用了。
下载方法二：使用我已下载过的Volley源码  [点击下载Volley源码](http://download.csdn.net/detail/u012827205/7150715)  下载之后解压，打包成jar包导入你的安卓项目中即可使用。

关于怎么具体使用其中的类和方法，网络上有很多我就不过多解释了。其实在这里我是想要介绍一下怎么把安卓工程Volley打包成jar包，这一技术我研究了一整个下午才搞定。

具体的文字说明：<这是一般的打包jar包非可运行jar包，即为导入另一安卓项目使用的>

1）在要打包的项目上右击，选择Export

2）在弹出的窗口中，选择Java -> Runnable JAR File，然后点击next按

3）在Runnable JAR File Specification窗口中，选择Launch configuration和Export destination

4）点击Finish按钮，打包完成。


在我的其中某个安卓项目中大概就是这样操作打包的

![](https://img-blog.csdn.net/20140405201934984?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140405202050906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







![](https://img-blog.csdn.net/20140405202223765?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





























































![](https://img-blog.csdn.net/20140405202255546?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)























































![](https://img-blog.csdn.net/20140405202314609?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)























































这就是从前到后的操作步骤，其中没有显示出来的操作就是直接finish的。

在打包过程中，要注意几点：

1.jar包中一般只包含代码而不包含资源，仅仅是java类。

2.如果要在jar包中包含资源，则文件不应该放在默认的目录下<因为导入其他项目后可能出现重名冲突>，应该在package目录下创建专用的资源目录，如此通过package的不重名来确保导入后的资源部发生冲突。同时，jar中代码不能直接使用R类中资源id进行访问，应该通过getClass().getClassLoader.getResourceAsStream()进行加载。

3.字符串资源可以定义成常量的方式来使用。

4.对于打包的安卓项目类<有Activity>，有需要资源的，应该在被导入jar包的项目中的resource中补上<或者把jar包中的资源文件复制过来>。






然后看一下，使用Volley的效果图

![](https://img-blog.csdn.net/20140405185230687?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvanVuaHVhaG91c2U=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


对于这个效果实现可以下载我的测试源码：[点击打开链接并下载源码](http://download.csdn.net/detail/u012827205/7150655)](https://so.csdn.net/so/search/s.do?q=jar包打包&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)




