# Maven入门 - 三少GG - CSDN博客
2017年05月01日 15:00:04[三少GG](https://me.csdn.net/scut1135)阅读数：731
本文介绍下在Ubuntu上安装Apache的Maven包，我们有几种不同的安装方式,
默认情况下, Apache Maven 3.0.5其实已经安装到你的Ubuntu系统下了 
Maven安装到Ubuntu 14.x, 15.x系统去 
$ sudo  apt-get install maven3
检测版本,
$ ** mvn --version**Apache Maven 3.0.5
Maven home: /usr/share/maven
Java version: 1.8.0_25, vendor: Oracle Corporation
Java home: /usr/java/jdk1.8.0_25/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "3.13.0-57-generic", arch: "amd64", family: "unix"
maven作为一个项目管理工具确实非常好用，但是在国内这个网络条件下实在是让人恼火。之前oschina的中央仓库可用，现在oschina的maven服务器关了，一直没找到国内镜像来替代。今天发现阿里云公开了一个中央仓库，大家可以试试。
## 配置
修改maven根目录下的conf文件夹中的setting.xml文件，** sudo vi /usr/share/maven3/conf/settings.xml **
内容如下：
```xml
<mirrors>
**    <mirror>        <id>alimaven</id>        <name>aliyun maven</name>        <url>http://maven.aliyun.com/nexus/content/groups/public/</url>        <mirrorOf>central</mirrorOf>            </mirror>**
</mirrors>
```
之后就能享受如飞的maven下载速度。
如果你想安装3.3.x版本的话只需要这几步, 
从[这里](https://maven.apache.org/download.cgi)下载 apache-Maven包: 
然后使用wget下载,
$ sudo wget http://mirror.fibergrid.in/apache/maven/maven-3/3.3.9/binaries/apache-maven-3.3.9-bin.tar.gz
解压tar.gz文件,
$ sudo tar -zxvf apache-maven-3.3.9-bin.tar.gz
然后进入解压后的文件夹,
$  sudo mv apache-maven-3.3.9 /usr/local/.
设置路径并编辑文件  ~/.profile or /etc/profile  ，在后面添加下面代码:  
  
export M2_HOME=/usr/local/apache-maven-3.3.9
export M2=$M2_HOME/bin
export MAVEN_OPTS="-Xms256m -Xmx512m"
export PATH=$M2:$PATH
$ sudo  source ~/.profile
$ sudo source /etc/profile
现在再检查下maven的版本
$  sudo mvn --version
Apache Maven 3.3.9 (bb52d8502b132ec0a5a3f4c09453c07478323dc5; 2015-11-10T11:41:47-05:00)
Maven home: /usr/local/apache-maven-3.3.9
Java version: 1.8.0_25, vendor: Oracle Corporation
Java home: /usr/java/jdk1.8.0_25/jre
Default locale: en_US, platform encoding: UTF-8
OS name: "linux", version: "3.13.0-57-generic", arch: "amd64", family: "unix"
