# Ubuntu14.04 快速安装JDK1.8 - 三少GG - CSDN博客
2015年09月02日 22:03:33[三少GG](https://me.csdn.net/scut1135)阅读数：3200

1. 首先去Oracle官网下载适合你机器位数的JDK 1.8，http://www.oracle.com/technetwork/java/javase/downloads/jdk8-downloads-2133151.html 
这里以jdk-8u25-linux-x64.tar.gz为例。
2. 使用终端命令进入JDK下载后的目录，执行解压命令： sudo tar -xzf jdk-8u25-linux-x64.tar.gz
    Copy解压后的文件夹到/usr/lib/jvm: sudo mv jdk1.8.0_25 /usr/lib/jvm 
3. 配置环境变量 vim ~/.bashrc 为如下内容：
*export JAVA_HOME=/usr/lib/jvm/jdk1.8.0_25*
*export JRE_HOME=${JAVA_HOME}/jre*
*export CLASSPATH=.:${JAVA_HOME}/lib:${JRE_HOME}/lib*
*export PATH=${JAVA_HOME}/bin:$PATH*
然后按esc键退出编辑模式，并输入命令"：wq"，最后在终端输入命令“source ~/.bashrc 保存配置并退出
4 配置默认JDK版本
sudo update-alternatives  --install /usr/bin/java  java  /usr/lib/jvm/jdk1.8.0_25/bin/java 300
sudo update-alternatives  --install /usr/bin/javac  javac  /usr/lib/jvm/jdk1.8.0_25/bin/javac 300
sudo update-alternatives  --install /usr/bin/jar  jar  /usr/lib/jvm/jdk1.8.0_25/bin/jar 300
检查：
sudo update-alternatives  --config   java
5 查看JAVA版本
java  -version
系统会列出
java version "1.8.0"  等信息。
