# Tomcat启动超时问题Server Tomcat v7.0 Server at localhost was unable to start within 45 seconds - xqhrs232的专栏 - CSDN博客
2018年11月28日 14:00:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：37
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://www.cnblogs.com/qupengkun/p/4897100.html](https://www.cnblogs.com/qupengkun/p/4897100.html)
使用Eclipse启动Tomcat时出现启动超时的问题如下所示：
![](https://images2015.cnblogs.com/blog/820767/201510/820767-20151021101143864-925773388.png)
Server Tomcat v7.0 Server at localhost was unable to
start within 45 seconds . If the server requires more
time, try increasing the timeout in the server editor.
遇到此问题时，第一步先多启动几下试试，如果不行的话可以
把tomcat启动时间适当延长如下：
![](https://images2015.cnblogs.com/blog/820767/201510/820767-20151021102208270-401943005.png)
打开tomcat设置页，修改启动时间：
![](https://images2015.cnblogs.com/blog/820767/201510/820767-20151021102315239-353000488.png)
经上一步如果还是启动超时的话，可以对项目进行clean，它的
作用是将工程中的.class文件全部删除，同时重新编译工程：
![](https://images2015.cnblogs.com/blog/820767/201510/820767-20151021102733255-1389518778.png)
然后再次重新部署tomcat，启动即可。
