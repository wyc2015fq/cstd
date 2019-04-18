# centos下实现程序开机自启动(tomcat为例) - 专注于数据挖掘算法研究和应用 - CSDN博客





2012年04月08日 08:55:10[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：16001








vim /etc/rc.d/rc.local  

打开后用下文覆盖即可

--------------------------------------------------------------------------------------------------------------------------------

# This script will be executed *after* all the other init scripts.

# You can put your own initialization stuff in here if you don't

# want to do the full Sys V style init stuff.

export JAVA_HOME=/home/hostname/jdk1.6.0_31

export CLASSPATH=.:$JAVA_HOME/jre/lib/rt.jar:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar

export PATH=$PATH:$JAVA_HOME/bin

export CATALINA_HOME=/home/hostname/apache-tomcat-6.0.35

#tomcat自启动

/home/hostname/apache-tomcat-6.0.35/bin/startup.sh   

#其他程序自启动例子

/usr/bin/程序名

touch /var/lock/subsys/local



-----------------------------------------------------------------------------------------------------------------------------------



