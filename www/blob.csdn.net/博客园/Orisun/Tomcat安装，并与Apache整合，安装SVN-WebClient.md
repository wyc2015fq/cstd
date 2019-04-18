# Tomcat安装，并与Apache整合，安装SVN-WebClient - Orisun - 博客园







# [Tomcat安装，并与Apache整合，安装SVN-WebClient](https://www.cnblogs.com/zhangchaoyang/articles/2101811.html)





直接去
[http://tomcat.apache.org](http://tomcat.apache.org/)下载二进制包(Binary Distributions)就不需要安装了，解压后直接运行bash文件即可运行Tomcat。

事先要设置好JAVA_HOME

echo $JAVA_HOME

/opt/java/jdk1.6.0_25

开启：

$ /home/orisun/develop/apache-tomcat-7.0.16/bin/startup.sh

停止：

/home/orisun/develop/apache-tomcat-7.0.16/bin/shutdown.sh

当然每次这么启动也太麻烦了，搞个简短一点的命令，在~/.bashrc中添加：

alias tomcat_start='/home/orisun/develop/apache-tomcat-7.0.16/bin/startup.sh'

alias tomcat_stop='/home/orisun/develop/apache-tomcat-7.0.16/bin/shutdown.sh'

alias tomcat_restart='tomcat_stop && tomcat_start'

在浏览器中输入
[http://localhost:8080/](http://localhost:8080/)

![](https://pic002.cnblogs.com/images/2011/103496/2011070915303452.png)

比我三四年前装Tomcat的时候界面漂亮多了。

要把Tomcat整合到Apache里面去，需要给Apache安装mod_jk.so，JK是Tomcat和Apache的连接器，到
[http://www.apache.org/dist/tomcat/tomcat-connectors/jk/](http://www.apache.org/dist/tomcat/tomcat-connectors/jk/)上面下载Linux版本的JK。

下面就是给Apache安装动态模块的一般步骤了，如果你是ubuntu采用apt-get自动安装的apache,需要再apt-get install apache2-dev把apxs装上，默认装在了/usr/lib/apache2/modules/apxs2.

cd download/tomcat-connectors-1.2.32-src/native


./buildconf.sh

./configure --with-apxs=

/usr/lib/apache2/modules/apxs2

make

sudo make install

然后就有了/usr/lib/apache2/modules/mod_jk.so

**配置Apache**

# vi /etc/apache2/httpd.conf

1、在文件加载模块的位置加上如下语句

LoadModule jk_module modules/mod_jk.so

2、在文件最末尾加上如下语句

# Where to find workers.properties

JkWorkersFile workers.properties

# Where to put jk logs

JkLogFile logs/mod_jk.log

# Set the jk log level [debug/error/info]

JkLogLevel info

# Select the log format

JkLogStampFormat "[%a %b %d %H:%M:%S %Y] "

# JkOptions indicate to send SSL KEY SIZE,

JkOptions +ForwardKeySize +ForwardURICompat -ForwardDirectories

# JkRequestLogFormat set the request format

JkRequestLogFormat "%w %V %T"

# Send servlet for context /examples to worker named ajp13

# JkMount /servlet/* ajp13

# Send JSPs for context /examples to worker named ajp13

JkMount /*.jsp ajp13

# Apache 虚拟主机配置

NameVirtualHost *:80

<VirtualHost *:80>

　　ServerAdmin webmaster@localhost

　　DocumentRoot "/home/orisun/develop/apache-tomcat-7.0.16/webapps"

　　ServerName 127.0.0.1

　　ErrorLog /var/log/apache2/tomcat_error_log

　　CustomLog /var/log/apache2/tomcat_access_log common

</VirtualHost>

**配置workers.properties文件**

#vi /

etc/apache2/workers.properties

在文件中添加如下内容：

worker.list=ajp13

worker.ajp13.port=8009

# 本机,若上面Tomcat主机不为localhost,作相应修改

worker.ajp13.host=127.0.0.1

# 类型

worker.ajp13.type=ajp13

# 代理数,不用修改

worker.ajp13.lbfactor=1

**8009端口的中文支持**

# vi /

/home/orisun/develop/apache-tomcat-7.0.16/conf/server.xml

设置URIEncode为UTF-8，以支持中文

<Connector port="8009" URIEncoding="UTF-8" .../>

默认你SVN已经装好了
[http://www.cnblogs.com/zhangchaoyang/articles/2101589.html](http://www.cnblogs.com/zhangchaoyang/articles/2101589.html)

下载svnwebclient.tar.gz，解压后把

svnwebclient.war放到tomcat的webapps目录下。

然后修改

svnwebclient/WEB-INF/web.xml文件：

设置 RepositoryUrl 属性为SVN版本库路径：

<context-param>
<param-name>RepositoryUrl</param-name>
<param-value>http://127.0.0.1/svn_reops/test/</param-value>
</context-param>

说明：

svn_reops是我已经建好的一个SVN库的虚拟路径，并且向库中提交了一个test工程（svn import -m "import test project to test" ./test/ http://zcypc/svn_repos/test）

添加 ParentRepositoryDirectory 属性为SVN版本库父路径（路径最后一定要以“/”结尾）：

<context-param>
<param-name>ParentRepositoryDirectory</param-name>
<param-value>http://

127.0.0.1/svn_reops/</param-value>
</context-param>

设置 DefaultEncoding 属性为 UTF-8：

<context-param>
<param-name>DefaultEncoding</param-name>
<param-value>UTF-8</param-value>
</context-param>

重启tomcat，重启apache。












