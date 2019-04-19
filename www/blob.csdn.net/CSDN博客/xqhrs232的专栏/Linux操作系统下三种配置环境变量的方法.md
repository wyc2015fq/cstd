# Linux操作系统下三种配置环境变量的方法 - xqhrs232的专栏 - CSDN博客
2012年04月26日 15:03:11[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：511标签：[linux																[java																[path																[文本编辑																[shell																[jdk](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=shell&t=blog)](https://so.csdn.net/so/search/s.do?q=文本编辑&t=blog)](https://so.csdn.net/so/search/s.do?q=path&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[Linux/Ubuntu/Fedora](https://blog.csdn.net/xqhrs232/article/category/906931)
原文地址::[http://www.linuxeden.com/html/sysadmin/20080424/56879.html](http://www.linuxeden.com/html/sysadmin/20080424/56879.html)
相关网帖
1.  Linux(Ubuntu)下设置环境变量的各种方法----[http://dawn-sky.iteye.com/blog/1333246](http://dawn-sky.iteye.com/blog/1333246)
现在使用linux的朋友越来越多了，在linux下做开发首先就是需要配置环境变量，下面以配置java环境变量为例介绍三种配置环境变量的方法。
1.修改/etc/profile文件 
如果你的计算机仅仅作为开发使用时推荐使用这种方法，因为所有用户的shell都有权使用这些环境变量，可能会给系统带来安全性问题。
(1)用文本编辑器打开/etc/profile 
(2)在profile文件末尾加入： 
JAVA_HOME=/usr/share/jdk1.5.0_05 
PATH=$JAVA_HOME/bin:$PATH 
CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export JAVA_HOME 
export PATH 
export CLASSPATH 
(3)重新登录 
注解： 
a. 你要将 /usr/share/jdk1.5.0_05jdk 改为你的jdk安装目录 
b. linux下用冒号“:”来分隔路径 
c. $PATH / $CLASSPATH / $JAVA_HOME 是用来引用原来的环境变量的值,在设置环境变量时特别要注意不能把原来的值给覆盖掉了，这是一种常见的错误。
d. CLASSPATH中当前目录“.”不能丢,把当前目录丢掉也是常见的错误。 
e. export是把这三个变量导出为全局变量。 
f. 大小写必须严格区分。 
2. 修改.bashrc文件　　 
这种方法更为安全，它可以把使用这些环境变量的权限控制到用户级别，如果你需要给某个用户权限使用这些环境变量，你只需要修改其个人用户主目录下的.bashrc文件就可以了。
(1)用文本编辑器打开用户目录下的.bashrc文件 
(2)在.bashrc文件末尾加入：　　 
set JAVA_HOME=/usr/share/jdk1.5.0_05 
export JAVA_HOME 
set PATH=$JAVA_HOME/bin:$PATH 
export PATH 
set CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
export CLASSPATH 
(3)重新登录 
3. 直接在shell下设置变量 
不赞成使用这种方法，因为换个shell，你的设置就无效了，因此这种方法仅仅是临时使用，以后要使用的时候又要重新设置，比较麻烦。
只需在shell终端执行下列命令： 
export JAVA_HOME=/usr/share/jdk1.5.0_05 
export PATH=$JAVA_HOME/bin:$PATH 
export CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar
