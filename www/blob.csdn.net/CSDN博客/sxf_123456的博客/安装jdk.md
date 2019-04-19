# 安装jdk - sxf_123456的博客 - CSDN博客
2018年01月11日 19:52:08[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：108
个人分类：[安装文档](https://blog.csdn.net/sxf_123456/article/category/7396488)
**安装jdk**
1、在/usr/java下解压
tar-zxvf  jdk-8u111-linux-x64.tar.gz
2、配置jdk环境变量
vi/etc/profile
设置环境变量
#--java--
export JAVA_HOME=/usr/java/jdk1.8.0_111/
export PATH=$PATH:$JAVA_HOME/bin:$JAVA_HOME/jre/bin:$PATH
export CLASSPATH=.:$JAVA_HOME/lib:$JAVA_HOME/jre/lib
3、环境变量生效
source /etc/profile
4、创建软连接
[root@ELK1 usr]# ln -s   /usr/java/jdk1.8.0_111/bin/java    /usr/bin/java
5、  查看jdk安装
java -version
6、修改/usr/java 权限
chmod  755 /usr/java
