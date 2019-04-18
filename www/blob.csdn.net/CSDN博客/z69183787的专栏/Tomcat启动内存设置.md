# Tomcat启动内存设置 - z69183787的专栏 - CSDN博客
2013年05月07日 10:20:32[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3593
## Tomcat启动内存设置
Tomcat的启动分为startupo.bat启动和注册为windows服务的启动，下面一一说明。
**1.startup.bat启动**
在tomcat_home/bin目录下找到catalina.bat，用文本编辑器打开，加上下面一行：
set JAVA_OPTS= -Xms1024M -Xmx1024M -XX:PermSize=256M -XX:MaxNewSize=256M -XX:MaxPermSize=256M
解释一下各个参数：
-Xms1024M：初始化堆内存大小（注意，不加M的话单位是KB）
-Xmx1029M：最大堆内存大小
-XX:PermSize=256M：初始化类加载内存池大小
-XX:MaxPermSize=256M：最大类加载内存池大小
-XX:MaxNewSize=256M：这个还不清楚哈，有知道的说声
还有一个-server参数，是指启动jvm时以服务器方式启动，比客户端启动慢，但性能较好，大家可以自己选择。
**2.windows服务启动**
       如果你的tomcat是注册为windows服务并且是以服务方式启动的，那么上面的方法就无效了，因为这时tomcat启动是读取注册表的参数，而不是读取批处理文件的参数，这时我们有两种方法来设置jvm参数。
第一种比较简单，tomcat为我们提供了一个设置启动参数的窗体，双击tomcat_home/bin目录下的tomcat6w.exe，如图
![](http://hiphotos.baidu.com/zn_stu/pic/item/5bde76d4912397dd6f714bac5982b2b7d1a2877c.jpg)
下方的Initial memory pool就是初始化堆内存大小，Maximun memory pool是最大堆内存大小。
而要设置Perm Gen池的大小就要在Java Option里面加参数了，在里面加上：
-Dcatalina.base=%tomcat_home%
-Dcatalina.home=%tomcat_home%
-Djava.endorsed.dirs=%tomcat_home%\endorsed
-Djava.io.tmpdir=%tomcat_home%\temp
-XX:PermSize=256M
-XX:MaxPermSize=256M
-XX:ReservedCodeCacheSize=48M
-Duser.timezone=GMT+08
（PS：网上说每一行后面不要有空格，没试过）
第二种方法是打开注册表->HKEY_LOCAL_MACHINE\SOFTWARE\Apache Software Foundation\Procrun 2.0\Tomcat6\Parameters\Java(路径可能有一点点差别)
![](http://hiphotos.baidu.com/zn_stu/pic/item/cef9402a2df5e0fe62745aa95c6034a85fdf7210.jpg)
修改Options的值，把刚才上面那些参数加进去就OK了。（别忘了先备份一下注册表）
