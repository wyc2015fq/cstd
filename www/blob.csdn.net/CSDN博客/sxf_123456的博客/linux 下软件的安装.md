# linux 下软件的安装 - sxf_123456的博客 - CSDN博客
2017年02月23日 11:57:43[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：193标签：[linux																[软件](https://so.csdn.net/so/search/s.do?q=软件&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)
个人分类：[项目开发](https://blog.csdn.net/sxf_123456/article/category/6742554)
Linux 下软件的安装主要有两种安装方式。第一种是安装文件名为 XXX.tar.gz，另外一种安装文件名为 XXX.i386.rpm。
第一种安装方法如下：
1、将要安装的文件拷贝到目录中，如 以root用户登录，将文件拷贝到/usr/src
# cp  XXX.tar.gz   /usr/src
2、解压文件
#tar  zxvf   XXX.tar.gz  
如果是XXX.tar.bz2，使用tar  jxvf   XXX.tar.bz2解压
3、解压后的文件夹中，会产生一个configure的文件。configure 作用是指定安装目录
进入解压后的文件夹目录下。如果想安装到/opt/pp目录中，使用下边命令
# cd  XXX   
#./configure  --prefix=/opt/pp
4、使用make进行编译
#make
5、编译成功后进行安装
#make install
6、安装完毕，应清除编译过程产生的临时文件和配置过程中产生的文件
#make clean
