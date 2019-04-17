# AutoLine开源平台安装部署教程 - zhusongziye的博客 - CSDN博客





2018年05月20日 16:37:55[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：226








# 安装手册



- 
安装手册
- 
MySQL安装及配置

- 
AutoLine依赖包安装

- 
初始化数据库

- 
初始化数据

- 
运行

- 
可能碰到的问题






github地址：https://github.com/small99/AutoLine

## MySQL安装及配置

下载最新版的Mysql安装，具体安装方法，请自行参照相关文档。
- 
使用utf-8编码创建一个名为autoline的数据库




- 
修改.env配置文件中的数据库连接字符串，如下：





> 
DATABASE_URL=mysql+pymysql://root:123456@127.0.0.1/autoline

TRIGGER_DATABASE_URL=mysql+pymysql://root:123456@127.0.0.1/autoline


注：
- 
root:123456 改为你的MySQL账号和密码

- 
127.0.0.1 改为你的MySQL服务器的IP地址





## AutoLine依赖包安装

如何安装AutoLine相关依赖包？
- 
直接在 https://github.com/small99/AutoLine 下载




- 
或通过git命令clone：





> 
git clone https://github.com/small99/AutoLine


等待下载完成， 在AutoLine根目录下的requirements.txt即为相关依赖包文件，

使用下面的命令安装依赖
- 
需要先安装好Python3，最好将pip升级到最新的版本（python）,先升级pip





> 
python -m pip install --upgrade pip

- 
安装AutoLine依赖包：





> 
pip install -r requirements.txt





## 初始化数据库

首次启动时，需要初始化数据库建表和数据信息：

使用下面命令初始化建表
- 
初始化


> 
python manage.py db init

- 
migrate


> 
python  manage.py db migrate

- 
upgrade


> 
python manage.py db upgrade


如果你修改了数据库模型,即修改了models.py中的表字段，运行上述的2和3即可修改表结构

## 初始化数据

第一次运行时，需要初始化数据，使用下面的命令即可：

> 
python manage.py deploy


## 运行

运行方式分为两种
- 
默认方式，只能在本机访问


> 
python manage.py runserver
此时只能通过http://127.0.0.1:5000来访问

- 
外网访问模式


> 
python manage.py runserver -h 0.0.0.0 -p 8080


-h 用于绑定本机IP

-p 用于指定端口

这是你可以通过http://ip:端口 来访问平台了，只要能ping到你IP地址的机器均可访问平台




## 可能碰到的问题
- 
提示pip不是最新版，请根据上面的提示先更新你的pip版本

- 
提示缺依赖包，请根据提示信息，手动pip install xxx 来安装缺的依赖包

- 
发现任何问题，请优先重新到github拉取最新的代码




