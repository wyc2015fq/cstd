# 【Android studio】 unable to start the daemon process - u013366022的专栏 - CSDN博客
2016年08月25日 09:58:59[slitaz](https://me.csdn.net/u013366022)阅读数：465
**这几天在做一个安卓桌面项目时，突然发现android studio 不能用了。**
**提示：**
**![](https://img-blog.csdn.net/20160825094745535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**
**网上的一些方法，要不就是：**
**1、删除C:\Users\<username>\.gradle 文件夹**
**2、File Menu - > Invalidate Caches/ Restart->Invalidate and Restart**
**3、C:\Users\<username>\.gradle 文件夹下增加gradle.properties**
**并不能解决我的问题。**
**尝试卸载android studio，重新安装 又或者安装其他版本的android studio，更新windows系统同样不能解决。**
**后来同事给了一个解决的办法，好奇葩的。**
**在系统环境变量中添加：    _JAVA_OPTIONS值为：    -Djava.net.preferIPv4Stack=true之后重启android studio，得以解决。**
**参考：**
**http://ask.csdn.net/questions/253654原因为从Java6更新到Java7后，Java7默认启用ipv6导致的：http://www.oracle.com/technetwork/java/javase/7u6-relnotes-1729681.html**
