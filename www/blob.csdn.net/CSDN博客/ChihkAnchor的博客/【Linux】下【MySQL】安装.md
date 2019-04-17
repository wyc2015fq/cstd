# 【Linux】下【MySQL】安装 - ChihkAnchor的博客 - CSDN博客





2018年10月31日 17:10:36[Chihk-Anchor](https://me.csdn.net/weixin_40871455)阅读数：16标签：[linux																[mysql](https://so.csdn.net/so/search/s.do?q=mysql&t=blog)](https://so.csdn.net/so/search/s.do?q=linux&t=blog)






- 下载：
	- [https://dev.mysql.com/downloads/file/?id=481117](https://dev.mysql.com/downloads/file/?id=481117)
- 我这里下载的Linux通用版：mysql-5.7.24-linux-glibc2.12-x86_64.tar.gz，刚开始下载ubuntu对应的版本安装出问题

- 解压：
	- 解压命令：tar -zxvf mysql-5.7.24-linux-glibc2.12-x86_64.tar.gz
- 复制解压后的mysql目录：cp -r mysql-5.7.24-linux-glibc2.12-x86_64.tar.gz  /usr/local/mysql
- 若提示权限不够，在 cp 前加上 sudo

- 添加用户和用户组
	- 添加用户组： groupadd mysql
- 添加用户mysql 到用户组mysql： useradd -g mysql mysql

- 
安装
- 
获取根权限 sudo su

- 
cd  /usr/local/mysql/

- 
mkdir -p ./data/mysql  (注意：一定要加-p）

- 
chown -R mysql:mysql ./    （/前有个点）

- 
./bin/mysql_install_db --user=mysql --datadir=/usr/local/mysql/data/mysql   （前面有个点）
- 
 安装时发现上面这个命令过时了，提示（mysql_install_db is deprecated. Please consider switching to mysqld --initialize），也就是说之后官方不再支持这个命令

- 
当不支持上面的命令时，可是使用   ./bin/mysqld  --user=mysql --datadir=/usr/local/mysql/data/mysql

- 
![](https://img-blog.csdnimg.cn/20181029223648935.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dlaXhpbl80MDg3MTQ1NQ==,size_16,color_FFFFFF,t_70)


- 
cp support-files/mysql.server /etc/init.d/mysqld

- 
chmod 755 /etc/init.d/mysqld

- 
cp support-files/my-default.cnf /etc/my.cnf
- 
若my-default.cnf不存在，使用  vim /etc/my.cnf   然后粘贴如下代码

- 
```
[client]
port = 3306
socket = /tmp/mysql.sock

[mysqld]
character_set_server=utf8
init_connect='SET NAMES utf8'
basedir=/usr/local/mysql
datadir=/usr/local/mysql/data
socket=/tmp/mysql.sock
log-error=/var/log/mysqld.log
pid-file=/var/run/mysqld/mysqld.pid
#不区分大小写
lower_case_table_names = 1

sql_mode=STRICT_TRANS_TABLES,NO_ZERO_IN_DATE,NO_ZERO_DATE,ERROR_FOR_DIVISION_BY_ZERO,NO_AUTO_CREATE_USER,NO_ENGINE_SUBSTITUTION

max_connections=5000

default-time_zone = '+8:00'
```

- 
按Esc，输入 wq 退出


- 
修改启动脚本  vim /etc/init.d/mysqld

- 
修改项：

		　　　　basedir=/usr/local/mysql/

		　　　　datadir=/usr/local/mysql/data/mysql

- 
启动服务：  service mysqld start

- 
测试连接：  ./mysql/bin/mysql -uroot

- 
加入环境变量，编辑 /etc/profile，使得可以在任何地方用mysql命令：

		　　　　export PATH=$PATH:/usr/local/mysql//bin

		              source /etc/profile

- 
`启动mysql：service mysqld start`

- 
`关闭mysql：service mysqld stop`

- 
`查看运行状态：service mysqld status`






