# Linux CentOs下安装RabbitMQ教程 - 菜鸟路上的小白 - CSDN博客





2017年01月04日 14:16:29[somnus_小凯](https://me.csdn.net/u012486840)阅读数：505








**1. 安装erlang **


安装依赖环境


yum -y install make gcc gcc-c++ kernel-devel m4 ncurses-devel openssl-devel unixODBC-devel



下载最新 Erlang 19.0



wget http://erlang.org/download/otp_src_19.0.tar.gz


解压


tar -xvzf otp_src_19.0.tar.gz



配置

./configure --prefix=/usr/local/erlang --with-ssl -enable-threads -enable-smmp-support -enable-kernel-poll --enable-hipe --without-javac



make


make install


配置/etc/profile


ERLANG_HOME=/usr/local/erlang

PATH=$PATH:$JAVA_HOME/bin:$ERLANG_HOME/bin

![](https://img-blog.csdn.net/20170104135937484?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





使其生效  


source /etc/profile


echo $PATH





检验erl
![](https://img-blog.csdn.net/20170104140127080?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



**2. 安装 rabbitmq**


wget http://www.rabbitmq.com/releases/rabbitmq-server/v3.6.3/rabbitmq-server-generic-unix-3.6.3.tar.xz


xz -drabbitmq-server-generic-unix-3.6.3.tar.xz

tar -xvfrabbitmq-server-generic-unix-3.6.3.tar


cd ./rabbitmq_server-3.6.3


cd sbin/

启用web管理界面


./rabbitmq-plugins enable rabbitmq_management




启动

./rabbitmq-server -detached


添加用户

./rabbitmqctl add_user admin admin123


设置权限

./rabbitmqctl set_user_tags admin administrator

浏览器访问

IP地址:15672

![](https://img-blog.csdn.net/20170104140310814?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20170104141558507?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjQ4Njg0MA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




