
# InfluxDB 下载安装 - leofionn的博客 - CSDN博客


2018年05月01日 08:36:33[leofionn](https://me.csdn.net/qq_36142114)阅读数：3367


下一个1.5版本的，我去看他的web页面，半天打不开，去看教程，配置里面【admin】的配置要手动开启，默认是false，找了半天找不到，听说web页面版本的被干掉了，卸载后。下了一个1.2版本的。
1、基本环境
\#yum install go
2、安装
可以直接rpm安装
到http://repos.influxdata.com下载相应的版本
|1
|2
|\#wget https:|//repos.influxdata.com/rhel/6Server/x86_64/stable/influxdb-1.2.0.x86_64.rpm
|\#rpm -ivh influxdb-1.2.0.x86_64.rpm
|

或者
添加yum 源安装
|1
|2
|3
|4
|5
|6
|7
|8
|\#cat << EOF | sudo tee /etc/yum.repos.d/influxdb.repo
|[influxdb]
|name = InfluxDB Repository - RHEL \|$releasever
|baseurl = https:|//repos.influxdata.com/rhel/\$releasever/\$basearch/stable
|enabled = 1
|gpgcheck = 1
|gpgkey = https:|//repos.influxdata.com/influxdb.key
|EOF
|

|1
|2
|3
|4
|5
|6
|7
|8
|\# cat influxdb.repo
|[influxdb]
|name = InfluxDB Repository - RHEL|$releasever
|baseurl = https:|//repos.influxdata.com/rhel/$releasever/$basearch/stable
|enabled = 1
|gpgcheck = 1
|gpgkey = https:|//repos.influxdata.com/influxdb.key
|\#yum install influxdb -y
|

3、配置
InfluxDB 1.1开始WEB管理默认是禁用的，所以装完并没有启用8083端口了，需要到配置文件里启用。
|1
|2
|3
|vi /etc/influxdb/influxdb.conf
|[admin]
|enabled = true ​​​​
|

4、启动 InfluxDB
\# service influxdb start
启动后打开 web 管理界面 http://本机IP:8083/
InfluxDB 的 Web 管理界面端口是 8083，HTTP API 监听端口是 8086，如果需要更改这些默认设定，修改 InfluxDB 的配置文件 /etc/influxdb/influxdb.conf 后重启 InfluxDB 就可以了
5、基本用法
|1
|2
|3
|4
|5
|6
|7
|\#influx
|>help    \#查看基本用法
|>show databases
|name: databases
|name
|----
|_internal
|

新旧版本要注意的事项：
1、是否启用web管理
1.1版本开始是默认没有启用web管理的，所以新版装完之后，启动服务，8083端口是没有的。要么通过命令直接管理，要么通过API管理。
不过还是可以手动开起来的。
|1
|2
|3
|vim /etc/influxdb/influxdb.conf
|[admin]
|enabled = true ​​​​
|

官网说明
The built-in web administration GUI is deprecated in InfluxDB 1.1 and is disabled by default. We recommend using the HTTP API or the Command Line Interface to interact with InfluxDB.
2、默认管理账号
0.8及以前版本安装完后，是有一个账号为root、密码为root的管理账号。可作为一个默认管理权限的账号。
0.9版本及以后版本是没有默认账号的，都要自己创建。
3、HTTP API接口的不同
0.9开始跟以前的版本完全不一样，所以如果是从老版本升级到0.9或者更高版本，要注意了，接口文件要完全重新写过。
0.8以前是类似这样的请求：
http://localhost:8086/db/mydb/
而新版的是这样的：
http://localhost:8086/query?db=test&pretty=true
新版基本上都是用/query来执行，而旧版是没有的。
网上很多都是旧版代码，如果用来查询新版的，是得不到数据的。


