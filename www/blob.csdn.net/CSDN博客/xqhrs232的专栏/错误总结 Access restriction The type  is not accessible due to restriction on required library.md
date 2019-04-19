# 错误总结:Access restriction: The type ** is not accessible due to restriction on required library - xqhrs232的专栏 - CSDN博客
2018年09月30日 09:28:39[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：42
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/lk_blog/article/details/45371813](https://blog.csdn.net/lk_blog/article/details/45371813)
来到新公司,下载代码查看后发现有很多地方标红报错了.
    仔细一看都是Access restriction: The type ** is not accessible due to restriction on required library类型的错误,这种错误的原因一般都是引用了jdk中非标准的类导致的.
例如:import sun.misc.BASE64Decoder;和import sun.misc.BASE64Encoder;
这两个类都在sun.misc包下,在jdk提供的rt.jar中,但是非标准的JavaApi接口.
处理的一般方法有两种:
    方法1. 修改eclipse的错误级别设置:
Windows -> Preferences -> Java -> Compiler -> Errors/Warnings -> 
Deprecated and trstricted API -> Forbidden reference (access rules): -> change to warning
sun.**包里的类，在eclipse里默认的级别是Error,修改为warning后错误即可消失。
    方法2.自定义access rules
工程上右键->工程属性->java builder path->Libraries标签，点击JRE System Library里面的Access rules，add sun/** 为accessible,见下图
![](https://img-blog.csdn.net/20150429212218612?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvbGtfYmxvZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
    按照方法1修改后很多地方标红的地方已经不再报错,但是引用这个类(sun.net.ftp.FtpClient;)的地方还是报错.经查阅后发现,原来jdk7已经将FtpClient修改为了abstract类,而且其中的方法也有不少变动.以前在Jdk6中可以用的openServer/login/binary/closeServer等方法在jdk7中已经不能用了.将项目所使用的Jdk版本从7降为6后,所有的报错消失.
扩展:
    为了项目可运行暂时调整为JDK6是可以的,但这绝对不是一个好办法,现在JDK8都已经出现好长时间了,我们为了以后平滑升级,最好不要用sun.**下的类.
    对于使用sun.misc.BASE64Decoder类的地方可以考虑使用org.apache.commons.codec.binary.Base64替换,对于使用sun.net.ftp.FtpClient类的地方可以考虑使用org.apache.commons.net.ftp.FTPClient替换. --------------------- 本文来自 天赋吉运李坤 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/lk_blog/article/details/45371813?utm_source=copy
