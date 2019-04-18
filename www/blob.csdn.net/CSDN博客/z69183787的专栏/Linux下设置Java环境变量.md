# Linux下设置Java环境变量 - z69183787的专栏 - CSDN博客
2017年09月28日 17:31:04[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3275
**关于如何在linux下设置 环境变量，可参考：**
**http://blog.csdn.net/z69183787/article/details/78126122**
**介绍一种自己使用的方法，**
**在 /etc/profile,d 目录下 增加 shell 脚本，添加环境变量**
**如下 **
**export JBOSS_HOME=/usr/local/jbossexport JBOSS_HOST="0.0.0.0"export TOMCAT_HOME=/usr/local/tomcatexport CATALINA_HOME=/usr/local/tomcatexport CATALINA_BASE=/usr/local/tomcatexport PATH=$PATH:$TOMCAT_HOME/binexport JAVA_HOME=/usr/local/jdkexport JAVAPTH=/usr/local/jdk/binexport CLASSPATH=.:$CATALINA_HOME/lib:$JAVA_HOME/lib:$JAVA_HOME/jre/libexport PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin**
xxx.sh
今天开始简单的学习了一下在Linux下安装jdk
写下来总结一下以便后来的查找和复习
首先下载Linux版的jdk我这里使用的jdk1.7：http://download.oracle.com/otn-pub/java/jdk/7u45-b18/jdk-7u45-linux-i586.tar.gz
下载完成后在usr文件夹下新建一个文件夹文件夹名为“Java”便于和其他的程序区分，然后将下载的jdk文件拷贝到/usr/java文件夹下
然后打开命令窗口 通过ls命令查看我们实在那个文件夹下
cd .. 返回上一层查看有我们查找的usr文件夹没有，没有继续 cd ..到上一层
找到usr后
运行命令:cd usr到该目录
ls查看该目录下的所有子目录
然后运行命令:cd java到java目录下
运行ls就可以看到我们拷贝进来的jdk了 
如图所示：
![](http://images.cnitblog.com/blog/468688/201311/02153149-726efede763946f7908f784c28f1ba89.png)
然后运行命令：tar -vxzf jdk-7u45-linux-i586.tar.gz
将jdk解压
解压完成后我们再次运行:ls 命令可以看到多出来了一个文件夹如图：
![](http://images.cnitblog.com/blog/468688/201311/02155835-5486bd00bfe4420ebbe57ca06310113a.png)
我这里使用的是root用户，如果你在操作的时候没有权限的换可以换成root用户
解压完成后我们到 : etc 目录下查找profile文件
![](http://images.cnitblog.com/blog/468688/201311/02160327-70d49fd0022c4b7fa29a3ba26620e667.png)
打开文件
![](http://images.cnitblog.com/blog/468688/201311/02160401-991b2ffdc97543dabe927ae9cdb6ca09.png)
添加上画红线方框内的内容
注意：我这里的/usr/java/jdk     jdk是我将jdk1.7.0_45重命名之后的名称这里不能完全照抄要按照你的javahome的文件名称填写跟在Window下配置是差不多的
然后我们保存文件。
运行命令：source /etc/profile 重新加载profile使配置立即生效
然后运行：java -version可以看到如图的显示：
![](http://images.cnitblog.com/blog/468688/201311/02161801-e72f1f1468c24c6f94ede3765144c686.png)
这就说明环境的配置已经成功！
