
# red head Linux 下安装jdk - 萌萌的It人 www.itmmd.com - CSDN博客


2014年11月16日 20:36:03[Jlins](https://me.csdn.net/dyllove98)阅读数：1250


1、分配读写权限(假设我的jdk-6u24-linux-i586.bin在我的/app根目录下):
chmod 755 jdk-6u24-linux-i586.bin
2、运行jdk-6u24-linux-i586.bin会自动解压
./jdk-6u24-linux-i586.bin
3、生命名（可选）
mv jdk-6u24-linux jdk
4、修改环境变量
vi /etc/profile
在最后添加如下（说明：这里是在app根目录）：
export JAVA_HOME=/app/jdk
export CLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
export PATH=.:$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH:$CLASSPATH
5、刷新环境变量生效
source /etc/profile
6、查看JDK是否安装成功
java -version出现如下（配置成功）：
java version "1.6.0_24"
Java(TM) SE Runtime Environment (build 1.6.0_24-b07)
Java HotSpot(TM) Client VM (build 19.1-b02, mixed mode)
![](http://images.cnitblog.com/blog/437282/201411/041405283617635.gif)
欢迎大家访问我的个人网站[萌萌的IT人](http://www.itmmd.com)

