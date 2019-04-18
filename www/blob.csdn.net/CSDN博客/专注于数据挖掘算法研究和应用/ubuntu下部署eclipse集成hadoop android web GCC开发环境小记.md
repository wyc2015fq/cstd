# ubuntu下部署eclipse集成hadoop\android\web\GCC开发环境小记 - 专注于数据挖掘算法研究和应用 - CSDN博客





2016年02月03日 08:44:34[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：2062








                
Hadoop环境：

1、部署：

   1)ubuntu上创建新用户账号：hadoop/123，并设置为管理员；

   2)ubuntu上安装SSH：创建rsa无密码ssh-key；

   3)ubuntu上部署hadoop2.7.1：home/hadoop下解压；

   4)hadoop2.7.1配置：http://my.oschina.net/laigous/blog/478438 参考；

2、开发：

   1)eclipse兼容问题：ubuntu为64位，需下载64位eclipse版本；

   2)hadoop2.7.1/lib/native查看64位：file libhadoop.so.1.0.0；

   3)eclipse集成hadoop插件：

     —先安装ant：sudo apt-get install ant

     —下载hadoop2x-eclipse-plugin

     —编译hadoop-eclipse-plugin插件  

       https://github.com/winghc/hadoop2x-eclipse-plugin 参考

       备注：2.7.1版本没编译组件成功，应该和版本有关系，直接用2.6.0版本；

     —hadoop-eclipse-plugin-2.6.0.jar拷贝到eclipse的plugins目录下；

     —eclipse配置：Window-->Show View -->MapReduce Tools

       参考：http://my.oschina.net/muou/blog/408543

                   http://www.cnblogs.com/shishanyuan/p/4178732.html

===========================================================================

web开发：

   1)eclipse安装并加载web开发插件；

   2)安装tomcat8，配置到eclipse；

   3)tomcat8 server无法创建

     —原因是ubuntu下tomcat目录的文件夹权限不够

     —sudo chmod -R 777 XXX（tomcat目录名）即可；

=============================================================================

Android开发：android-studio/eclipse+ADT

   1)ubuntu下安装android-studio开发环境：

     —sudo apt-add-repository ppa:paolorotolo/android-studio

     —sudo apt-get update

     —sudo apt-get install android-studio

    和eclipse在线安装ADT插件一样， google服务器无法到达；

   2)下载android-studio安装：

     —android-studio中文社区百度网盘下载；

     —启动后设置代理还是无法下载sdk；

   3)下载adt bundle完整包，含sdk+elicpse+adt解压运行；

   4)总结，android开发环境无论是android-stuio还是eclipse-adt都主要是 google网络无法访问导致无法正常安装android的sdk环境；

============================================================================================================

GCC/G++开发：

   1)sudo apt-get install build-essential安装；

     —gcc -g -Wall编译hello.c通过；

     —g++ 编译test.cpp通过；   

   2)集成开发环境eclipse+CDT：在线安装

     —网址http://download.eclipse.org/tools/cdt/releases/8.8
            


