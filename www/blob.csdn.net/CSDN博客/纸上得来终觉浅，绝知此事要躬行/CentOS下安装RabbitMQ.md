# CentOS下安装RabbitMQ - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年03月15日 16:42:57[boonya](https://me.csdn.net/boonya)阅读数：417









## 环境准备

yum -y install make gcc gcc-c++ kernel-devel m4 ncurses-devel openssl-devel 

yum -y install make gcc gcc-c++ kernel m4 ncurses openssl

## **erlang安装包**

![这里写图片描述](https://img-blog.csdn.net/20170214093613459?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd194X3pf/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

erlang的下载地址:[http://www.erlang.org/](http://www.erlang.org/)

## **RabbitMQ安装包**

![这里写图片描述](https://img-blog.csdn.net/20170214093621990?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd194X3pf/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **解压缩erlang otp包**

tar -xvf otp_src_19.2.tar.gz

## **安装erlang环境**

./configure –prefix=/usr/local/erlang –with-ssl -enable-threads -enable-smmp-support -enable-kernel-poll –enable-hipe –without-javac

make

make install

## **配置erlang环境变量**

vi /etc/profile 

ERLANG_HOME=/usr/local/erlang 

PATH=$ERLANG_HOME/bin:$PATH

## **解压RabbitMQ安装包**

tar -xvf rabbitmq-server-generic-unix-3.6.5.tar.xz

## RbiitMQ操作

### **启动节点**

cd sbin 

./rabbitmq-server

以守护程序的方式在后台运行： 

./rabbitmq -server -detached

### **停止节点**

cd sbin 

./rabbitmqctl stop

### **添加用户**

./rabbitmqctl add_user test test

### **设置权限**

./rabbitmqctl set_user_tags test administrator

./rabbitmqctl list_users

./rabbitmqctl set_permissions -p / test ‘.*’ ‘.*’ ‘.*’

### 管理界面访问

![这里写图片描述](https://img-blog.csdn.net/20170214110749094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd194X3pf/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




