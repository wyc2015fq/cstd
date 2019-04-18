# Linux操作系统Ubuntu部署J2EE篇 - 专注于数据挖掘算法研究和应用 - CSDN博客





2011年09月11日 15:27:52[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：3625标签：[ubuntu																[linux																[java																[jdk																[eclipse																[图形](https://so.csdn.net/so/search/s.do?q=图形&t=blog)
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)





## Ubuntu下J2EE部署

### 1 安装JDK环境

离线手动安装：

1）下载地址：[http://www.oracle.com/technetwork/java/javase/downloads/index.html](http://www.oracle.com/technetwork/java/javase/downloads/index.html)

下载版本：jdk-6u27-linux-i586.bin

2）管理员主文件夹fjs下创建java文件夹，将jdk-6u27-linux-i586.bin放置其中，进入java目录的终端，执行命令：

sudo sh jdk-6u27-linux-i586.bin

3）设置环境系统变量：该设置方法验证未通过；

——执行命令：sudo gedit /etc/environment

在文件中添加：
     export JAVA_HOME=/home/fjs/java/jdk1.6.0_27      export JRE_Home=/home/fjs/java/jdk1.6.0_27/jre      export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib 
——执行命令：sudo gedit /etc/profile

在文件中umask022之前添加：
     export JAVA_HOME=/home/fjs/java/jdk1.6.0_27      export JRE_HOME=/home/fjs/java/jdk1.6.0_27/jre      export CLASSPATH=$CLASSPATH:$JAVA_HOME/lib:$JAVA_HOME/jre/lib      export PATH=$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH:$HOME/bin 
4）注销用户fjs（管理员），检测JDK版本，命令：java –version

=================================================================================================================================


1）安装后注销用户，登陆界面输入用户名和密码后无法进入图形化系统而重新进入登陆界面，强制关闭后重启也不可以，提示：*Stopping System V runlevel compatibility [OK]

2）执行ctrl+alt+f1切换到tty（ctrl+alt+f7切换到图形化界面），输入用户名和密码后，进入文本化系统；执行命令gdm查看具体原因， 

提示：/home/fjs/java/jdk1.6.0_27/lib，/home/fjs/java/jdk1.6.0_27/jre/lib，bad variable name；

3）执行命令startx进入图形化系统，将/etc/environment和/etc/profile上所添加的java环境变量信息删除，重新执行命令gdm，

提示：(gdm-binary:2390):warning **:could not acquire name;bailing out；

4）以root身份执行重配置gdm命令：dpkg-reconfigure gdm；

提示：(gdm-binary:2480):warniing**:could not acquire name;bailing out；

5）强制退出，重新启动Ubuntu发现可以正常进入图形化系统；

6）定位原因：jdk环境变量配置导致错误，修改配置方法为：

   sudo vi /etc/environment

   在environment中修改：

PATH=”……:/home/fjs/java/ jdk1.6.0_27/bin”

JAVA_HOME=/home/fjs/java/ jdk1.6.0_27

CLASSPATH=.:/home/fjs/java/ jdk1.6.0_27/lib

   重启即配置成功。

===============================================================================================================================


### 2 安装Eclipse

1．下载地址：[http://www.eclipse.org/downloads](http://www.eclipse.org/downloads)

   下载版本：eclipse-jee-indigo-linux-gtk.tar.gz

2．解压到指定目录即可，这里和jdk目录一致，放在java内，解压后路径：/home/fjs/java/eclipse；

3．启动eclipse，设置工作路径为：/home/fjs/java/workspace；

### 3 安装Tomcat

1．下载地址：[http://tomcat.apache.org](http://tomcat.apache.org/)

   下载版本：apche-tomcat-7.0.21.tar.gz

2．解压到指定目录即可，这里和jdk目录一致，放在java内，解压后路径：/home/fjs/java/ apche-tomcat-7.0.21；

3．进入/home/fjs/java/ apche-tomcat-7.0.21/bin，执行命令./ startup.sh启动，执行命令./ shutdown.sh关闭；](https://so.csdn.net/so/search/s.do?q=eclipse&t=blog)](https://so.csdn.net/so/search/s.do?q=jdk&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)](https://so.csdn.net/so/search/s.do?q=ubuntu&t=blog)




