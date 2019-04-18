# linux 配置全用户的环境变量，profile.d文件夹的作用 - z69183787的专栏 - CSDN博客
2017年09月28日 17:23:58[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1134
# Linux 环境下/etc/profile和/etc/profile.d 的区别
> **区别：**
> 
> 1. 两个文件都是设置环境变量文件的，/etc/profile是永久性的环境变量,是全局变量，/etc/profile.d/设置所有     用户生效
> 2. /etc/profile.d/比/etc/profile好维护，不想要什么变量直接删除/etc/profile.d/下对应的shell脚本即可，不用      像/etc/profile需要改动此文件
> 
**CentOS 中每个用户都要指定各自的环境变量，这样会比较麻烦，那么如何配置一个环境变量，所有的用户都可以使用呢？**
> 
下面以jdk的安装为例
vi  /etc/profile.d/java.sh
> 在新的java.sh中输入以下内容：
#set java environment
> JAVA_HOME=/var/mysoft/jdk1.7.0_80
> PATH=＄JAVA_HOME/bin:＄PATH
> export JAVA_HOME PATH
> 
保存退出，然后给java.sh分配权限：chmod 755 /etc/profile.d/java.sh
