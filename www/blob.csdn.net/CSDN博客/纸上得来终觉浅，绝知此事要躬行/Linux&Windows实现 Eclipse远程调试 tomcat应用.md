# Linux&Windows实现 Eclipse远程调试 tomcat应用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年12月30日 15:17:43[boonya](https://me.csdn.net/boonya)阅读数：797








原文地址：[http://blog.csdn.net/afgasdg/article/details/9236877](http://blog.csdn.net/afgasdg/article/details/9236877)根据我个人的喜好做了一下调整。


## **配置Tomcat使支持远程调试**

Linux配置:

修改/usr/local/tomcat/bin/catalina.sh文件：

CATALINA_OPTS="-Xdebug  -Xrunjdwp:transport=dt_socket,address=8000,server=y,suspend=n"![](https://img-blog.csdn.net/20161230150338183)




Windows配置：

修改$TOMCAT_HOME/bin/catalina.bat文件：

Set  “CATALINA_OPTS=-Xdebug  -Xrunjdwp:transport=dt_socket,address=8000,server=y,suspend=n"

如图：
![](https://img-blog.csdn.net/20130703190113109?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

==============参数说明：=====================

-Xdebug             ： 启用调试模式

-Xrunjdwp<sub-options>  ：加载JVM的JPDA参考实现库

transport=dt_socket     ： Socket连接，可选dt_shmem 通过共享内存的方式连接到调试服务器

address=8000         ：
调试服务器监听的端口

server=y            ：
是否是服务器端，n为客户端

suspend=n           ：
启动过程是否加载暂停，y为启动时暂停，方便调试启动过程

测试启动tomcat：

Linux启动:./catalina.sh run

Windows启动：直接双击catalina.bat


## Eclipse调试远程Tomcat应用
![](https://img-blog.csdn.net/20130703190215906?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20130703190253421?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


点击debug，就可进行调试了
![](https://img-blog.csdn.net/20130703190326031?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

剩下的就和普通调试一样了，断点直接在源代码中添加就行。

## **远程VM连接被拒绝问题**

Failed to connect to remote VM. Connection refused.

Connection refused: connect。

![](https://img-blog.csdn.net/20130703190417750?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


出现如图所示的情况可能是已经建立了一个连接了。

解决方法：去debug透视图中![](https://img-blog.csdn.net/20130703190347640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，

找到点击![](https://img-blog.csdn.net/20130703190400234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYWZnYXNkZw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)断开链接，就可以解决问题了。



