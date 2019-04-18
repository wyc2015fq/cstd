# ubuntu15.10避免图形界面无法登录的jdk配置 - 专注于数据挖掘算法研究和应用 - CSDN博客





2015年12月18日 15:43:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2212
个人分类：[Linux](https://blog.csdn.net/fjssharpsword/article/category/6480442)









jdk配置etc/profile文件后会出现图形界面无法登录，正确的配置方法如下，配置后重启即可。

#Java Environment Path

export JAVA_HOME=/usr/local/java/jdk1.7.0_79

export JRE_HOME=${JAVA_HOME}/jre

export PATH=$PATH:${JAVA_HOME}/bin

export CLASSPATH=$CLASSPATH.:${JAVA_HOME}/lib:${JRE_HOME}/lib
            


