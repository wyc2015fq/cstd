# ubuntu安装jdk - weixin_33985507的博客 - CSDN博客
2012年10月05日 08:32:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
一 。。首先去sun官网（oracl官网？）下载jdk6的bin安装文件。
1）将jdk.bin拷贝到/opt目录，执行
#sudo sh jdk-6u13-linux-i586.bin 
或者
#chmod 755 jdk-6u13-linux-i586.bin
#./jdk-6u13-linux-i586.bin
2）配置环境变量
#sudo gedit /etc/profile
添加
JAVA_HOME=/home/owner/2012/monitor/jdk/jdk1.6.0_13
JRE_HOME=/home/owner/2012/monitor/jdk/jdk1.6.0_13/jre
PATH=$PATH:$JAVA_HOME/bin:JRE_HOME/bin
CLASSPATH=.:$JAVA_HOME/lib/dt.jar:$JAVA_HOME/lib/tools.jar:$JRE_HOME/lib:
export JAVA_HOME JRE_HOME PATH CLASSPATH
3）使环境变量生效
#source /etc/profile
4）测试z
#java -version
二 。。卸载jdk
#sudo rm -rf /opt/jdk1.6.0_22
#sudo gedit /etc/profile
删除环境变量
三 。。由于myeclipse官网国内访问不能。所以决定下载netbeans安装
安装方法
#sudo sh netbeans-6.9.1-ml-linux.sh
