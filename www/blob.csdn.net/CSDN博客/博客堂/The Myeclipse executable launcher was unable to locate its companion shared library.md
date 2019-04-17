# The Myeclipse executable launcher was unable to locate its companion shared library - 博客堂 - CSDN博客





2015年03月28日 18:24:16[最小森林](https://me.csdn.net/u012052268)阅读数：15948标签：[myeclipse																[launcher																[plugins																[Common](https://so.csdn.net/so/search/s.do?q=Common&t=blog)
个人分类：[java](https://blog.csdn.net/u012052268/article/category/3058811)





**打开myeclipse的时候弹出**：The Myeclipse executable launcher was unable to locate its companion shared library




**解决办法**：在安装目录下打开 myeclipse.ini

发现** /MyEclipse/Common/的路径**找不到了，因为修改了文件夹的名字。 因为上次装MyEclipse装在E盘 这次装在D盘 所以会有两个地方。

所以：

1.根据ini文件的路径找到MyEclipse/Common/ 

2.或者更改ini文件到新的路径。



```
#utf8 (do not remove)
#utf8 (do not remove)
-startup
E:/本机常用软件/MyEclipse/Common/plugins/org.eclipse.equinox.launcher_1.2.0.v20110502.jar
--launcher.library
E:/本机常用软件/MyEclipse/Common/plugins/org.eclipse.equinox.launcher.i18n.win32.win32.x86_4.2.0.v201201111650
-install
D:\Program Files\MyEclipse
-vm
E:\本机常用软件\MyEclipse\Common\binary\com.sun.java.jdk.win32.x86_1.6.0.013\jre\bin\javaw.exe
-configuration
D:\Program Files\MyEclipse\configuration
-vmargs
-Xmx512m
-XX:MaxPermSize=256m
-XX:ReservedCodeCacheSize=64m
-Dosgi.nls.warnings=ignore
```](https://so.csdn.net/so/search/s.do?q=plugins&t=blog)](https://so.csdn.net/so/search/s.do?q=launcher&t=blog)](https://so.csdn.net/so/search/s.do?q=myeclipse&t=blog)




