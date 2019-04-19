# CentOS6.5_X64下安装配置MongoDB数据库 - fanyun的博客 - CSDN博客
2016年06月04日 09:55:23[老樊Lu码](https://me.csdn.net/fanyun_01)阅读数：7865
所属专栏：[数据库](https://blog.csdn.net/column/details/database-01.html)[大数据](https://blog.csdn.net/column/details/bigdata01.html)
**环境准备：**
硬件要求：50G硬盘 8G内存 4核CPU
软件要求：Linux操作系统：[CentOS](http://www.linuxidc.com/topicnews.aspx?tid=14)6.5_X64
  mongodb-linux-x86_64-2.6.10.tgz
**目的：**
安装配置MongoDB数据库
**具体操作：**
一、关闭SElinux、配置防火墙
1.vi /etc/selinux/config
#SELINUX=enforcing #注释掉
#SELINUXTYPE=targeted #注释掉
SELINUX=disabled #增加
:wq!  #保存退出
setenforce 0 #使配置立即生效
2.vi /etc/sysconfig/iptables  #编辑
-A RH-Firewall-1-INPUT -m state --state NEW -m tcp -p tcp --dport 27017 -j ACCEPT  #允许27017端口通过防火墙
:wq! #保存退出
/etc/init.d/iptables restart #重启防火墙使配置生效
二、安装MongoDB
（1）.下载MongoDB安装包：mongodb-linux-x86_64-2.6.10.tgz
（按照以下官网链接中的流程安装MongoDB，需要安装的版本是 v2.6.9：
[http://docs.mongodb.org/manual/tutorial/install-mongodb-on-red-hat/](http://docs.mongodb.org/manual/tutorial/install-mongodb-on-red-hat/)）
# tar -zxvf mongodb-linux-x86_64-2.6.10.tgz
在/root下建立mongodb运行时的文件夹并放入mongodb组件
# mkdir -p mongodb
# cp -R -n mongodb-linux-x86_64-2.6.10/ mongodb
mv mongodb-linux-x86_64-2.6.10 /usr/local/mongodb  #移动解压文件夹到MongoDB安装目录
mkdir  -p  /data/mongodb/mongodb_data/                  #创建MongoDB数据库存放路径
mkdir  -p  /data/mongodb/mongodb_log/                        #创建MongoDB数据库日志存放路径
设置环境变量
# vi ~/.bashrc
在该文件最后一行添加：
export PATH=/usr/local/mongodb/mongodb-linux-x86_64-2.6.10/bin:$PATH
保存并退出文件
输入命令使环境变量生效：
# source ~/.bashrc
建立默认数据存储位置：
mkdir-p /data/db
建立mongodb日志存储位置：
mkdir 
/usr/local/mongodb/logs
启动数据库命令：
# mongod--logpath=/usr/local/mongodb/logs/mongodb.log --fork
#mongod --logpath=/usr/local/mongodb/logs/mongodb.log --fork --nojournal(第二种启动方法：有，之前起mongodb的时候journal也一起启动，这是mongodb在宕机时回复写操作用的，但是要多占一些硬盘，内存资源。
（2）.启动MongoDB
/usr/local/mongodb/bin/mongod --port 27017 --fork --dbpath=/data/mongodb/mongodb_data/ --logpath=/data/mongodb/mongodb_log/mongodb.log --logappend
about to fork child process, waiting until server is ready for connections.
forked process: 2102
child process started successfully, parent exiting
安装完毕后数据库默认监听端口为27017，数据库默认存储路径为/var/lib/mongo
安装完毕后使用如下命令启动MongoDB：
#service mongod start
（3）.查看你启动状态
netstat -ntpl    #查看MongoDB是否启动
用这个命令查看启动状态：ps aux | grep mongod
![](https://img-blog.csdn.net/20160602100758478?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
（4）.添加系统服务和守护进程
配置MongoDB开机自启动：chkconfig
 mongod on
# echo“/usr/local/mongodb/mongodb-linux-x86_64-2.6.10/bin/mongod--logpath=/root/mongodb/logs/mongodb.log
 --fork” >> /etc/rc.local
1）.修改mongodb系统文件设置开机自启动
       i. 先停止mongodb服务：service mongod stop 
       ii. 重新启动mongodb服务，用以下命令：
#mongod --logpath=/usr/local/mongodb/logs/mongodb.log --fork –nojournal
      iii. 最后修改下开机启动文件里mongodb的部分：vi /etc/rc.local，在这行添加：/usr/local/mongodb/mongodb-linux-x86_64-2.6.10/bin/mongod--logpath=/usr/local/mongodb/logs/mongodb.log--fork，后面加上--nojournal，保存。
（5）.测试MongoDB数据库
a)  执行导入命令：
    # 
/usr/local/mongodb/mongodb-linux-x86_64-2.6.10/bin/mongorestore  --db project /root/newDump/project
b)  导入完以后，进行如下操作验证数据数否有了：
# mongo登入数据库
# >useproject
# >db.stock.getIndexes()  (备注：如果有乱码，设置shell终端为utf8)，若如下图所示，表明导入数据成功。
![](https://img-blog.csdn.net/20160602101246154?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
