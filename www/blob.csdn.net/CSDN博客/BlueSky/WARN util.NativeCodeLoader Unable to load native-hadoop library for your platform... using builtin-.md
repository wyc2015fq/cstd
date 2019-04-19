# WARN util.NativeCodeLoader: Unable to load native-hadoop library for your platform... using builtin- - BlueSky - CSDN博客
2015年11月17日 16:34:12[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：432标签：[hadoop																[library																[native](https://so.csdn.net/so/search/s.do?q=native&t=blog)](https://so.csdn.net/so/search/s.do?q=library&t=blog)](https://so.csdn.net/so/search/s.do?q=hadoop&t=blog)
个人分类：[Spark Learning](https://blog.csdn.net/ASIA_kobe/article/category/5912555)
目录 
前言 
1.相关环境 
2.获取代码 
3.安装Hadoop 
4.安装Native Lib 
5.安装完成 
6.相关链接
前言  
最近需要安装CDH4.5, 在重启后提示缺失native lib, 经过一番折腾后,终于安装成功,下面记录下native lib的获取过程.
@Author  duangr
@Website [http://www.linuxidc.com/Linux/2014-03/98599.htm](http://www.linuxidc.com/Linux/2014-03/98599.htm)
1.相关环境
项   值 
OS  CentOS6.4 
Arch    x86_64 
Hadoop  hadoop-2.0.0-cdh4.5.0 
[主]duangr-1 192.168.56.10 
[主]duangr-2 192.168.56.11 
[主]duangr-3 192.168.56.12 
2.获取代码
hadoop-2.0.0-cdh4.5.0.tar.gz 
hadoop-2.0.0+1518-1.cdh4.5.0.p0.24.el6.x86_64.rpm
3.安装hadoop
安装hadoop-2.0.0-cdh4.5.0.tar.gz, 此处略
4.安装Native Lib
将hadoop-2.0.0+1518-1.cdh4.5.0.p0.24.el6.x86_64.rpm在PC上使用压缩工具打开后,取出\usr\lib\hadoop\lib\native 下面的代码,上传到主机 ${HADOOP_HOME}/lib/native 下面
cd ${HADOOP_HOME}/lib/native 
rm libhadoop.so 
rm libsnappy.so 
rm libsnappy.so.1 
ln -s libhadoop.so.1.0.0 libhadoop.so 
ln -s libsnappy.so.1.1.3 libsnappy.so.1 
ln -s libsnappy.so.1.1.3 libsnappy.so
cd .. 
tar -zcf native.tar.gz native
scp native.tar.gz duangr-2:${HADOOP_HOME}/lib  ssh duangr-2 “cd ${HADOOP_HOME}/lib; tar -zxf native.tar.gz;”
scp native.tar.gz duangr-3:${HADOOP_HOME}/lib  ssh duangr-3 “cd ${HADOOP_HOME}/lib; tar -zxf native.tar.gz;”
5.安装完成
重启hadoop即可
