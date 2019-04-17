# Shell 实现Docker MySQL5.7安装及SQL脚本运行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月11日 13:49:15[boonya](https://me.csdn.net/boonya)阅读数：250








Linux Shell 实现Docker MySQL安装及脚本运行的目标：

1、实现MySQL5.7安装

2、安装完成后创建对应的账号和数据库实例表等

### 创建docker mysql容器实例

定义docker_mysql_install.sh

```bash
#! /bin/bash
#file:mysql_docker_install.sh
#company:cvnavi.com
#author:Pengjunlin
echo "当前执行文件......$0"
exists_name=""
exists_tag=""
# 下载 mysql
for i in [ `docker images ` ]; do
	
	if [[ $i == "docker.io/mysql" ]]; then
		echo "$i"
		exists_name="1"
	fi
	if [[ $i == "5.7" ]]; then
		echo "$i"
		exists_tag="1"
	fi
done
if [[ $exists_name == "1" && $exists_tag == "1" ]]; then
	echo "本地已存在mysql:5.7镜像."
else
	docker pull mysql:5.7
fi
# 停止mysql容器
for i in [ `docker ps ` ]; do
	if [[ $i == "mysql5.7" ]]; then
		echo "停止mysql5.7容器 "
		docker stop mysql5.7
		break
	fi
done
# 删除mysql容器
for i in [ `docker ps -a` ]; do
	if [[ $i == "mysql5.7" ]]; then
		echo "删除mysql5.7容器 "
		docker rm mysql5.7
		break
	fi
done

if [[ -f "/usr/local/docker_mysql_create_table.sh" ]]; then
	# 创建mysql映射目录
	if [[ ! -d "/etc/mysql" ]]; then
		mkdir /etc/mysql
	fi
	if [[ ! -d "/etc/mysql/conf.d" ]]; then
		mkdir /etc/mysql/conf.d
	fi
	if [[ ! -d "/etc/mysql/logs" ]]; then
		mkdir /etc/mysql/logs
	fi
	if [[ ! -d "/etc/mysql/data" ]]; then
		mkdir /etc/mysql/data
	fi
	# 运行容器实例
	docker run -d -p 3306:3306 --name mysql5.7 -v /etc/mysql/conf.d:/etc/mysql/conf.d -v /etc/mysql/logs:/logs -v /etc/mysql/data:/var/lib/mysql -e MYSQL_ROOT_PASSWORD=root  mysql:5.7
	# 映射copy文件路径到docker容器
	docker cp /usr/local/docker_mysql_create_table.sh mysql5.7:/usr/local/docker_mysql_create_table.sh
	# 打印即将执行的指令
	echo " docker exec -it mysql5.7  /bin/bash -c 'sh /usr/local/docker_mysql_create_table.sh'"
	# 进入容器并执行脚本：
	docker exec -it mysql5.7  /bin/bash -c "sh /usr/local/docker_mysql_create_table.sh"
else
	echo "/usr/local/docker_mysql_create_table.sh文件不存在，docker需要用此文件创建mysql容器实例并创建用户和数据库表."
	exit 1
fi
```

注意：mysql容器的 账号密码都是root，使用了/bin/bash/ -c 来执行指令文件。

### 创建用户和执行SQL语句

定义docker_mysql_create_table.sh

```bash
#！/bin/bash
#file:docker_mysql_create_table.sh
#company:cvnavi.com
#author:Pengjunlin
echo "当前执行文件......$0"
# Mysql数据库相关配置
mysql_database_host="localhost"
mysql_database_name="filecache"
mysql_root_user_name="root"
mysql_root_user_pwd="root"
mysql_remote_user_name="rtvsweb"
mysql_remote_user_pwd="rtvs2018"
mysql_table_name="avinfo";
# 进入mysql控制台 默认用户是root
echo "Docker Mysql控制台创建数据库...."
echo "***********************************以下容执行可能会有错误提示，请忽略**************************"
# 为root用户设置密码
echo "Docker Mysql控制台为root用户设置密码....$mysql_root_user_pwd"
sql_alter_user="ALTER USER '$mysql_root_user_name'@'$mysql_database_host' IDENTIFIED BY '$mysql_root_user_pwd'"
mysql -u$mysql_root_user_name -p$mysql_root_user_pwd -e"$sql_alter_user"


# 添加远程登录用户
echo "Docker Mysql控制台创建远程用户....$mysql_remote_user_name"
sql_create_user="CREATE USER '$mysql_remote_user_name'@'%' IDENTIFIED WITH mysql_native_password BY '$mysql_remote_user_pwd'"
mysql -u$mysql_root_user_name -p$mysql_root_user_pwd -e"$sql_create_user"

#为远程账号添加权限
echo "Docker Mysql控制台为远程账号添加权限.... *.* TO '$mysql_remote_user_name'@'%'"
sql_grant_user="GRANT ALL PRIVILEGES ON *.* TO '$mysql_remote_user_name'@'%'"
mysql -u$mysql_root_user_name -p$mysql_root_user_pwd -e"$sql_grant_user"

# 创建数据库
echo "Docker Mysql控制台创建数据库.... $mysql_database_name"
sql_create_database="create database IF NOT EXISTS $mysql_database_name"
mysql -u$mysql_root_user_name -p$mysql_root_user_pwd -e"$sql_create_database"

#用户创建Mysql数据库脚本
echo "Docker Mysql控制台创建数据库表.... $mysql_table_name"
sql_create_table="create table IF NOT EXISTS $mysql_table_name (
  F_ID varchar(255) NOT NULL,
  F_SIM varchar(20) NOT NULL,
  F_CHANNEL int(11) NOT NULL,
  F_ALARM decimal(18,0) NOT NULL,
  F_STORAGE_TYPE int(11) NOT NULL,
  F_STREAM_TYPE int(11) NOT NULL,
  F_START_TIME decimal(20,0) NOT NULL,
  F_END_TIME decimal(20,0) NOT NULL,
  F_FILE_PATH varchar(255) NOT NULL,
  F_FILE_SIZE decimal(18,0) NOT NULL,
  F_DATA_TYPE int(11) default NULL,
  PRIMARY KEY (F_ID)
) ENGINE=InnoDB DEFAULT CHARSET=utf8"
mysql -u$mysql_root_user_name -p$mysql_root_user_pwd -d$mysql_database_name -e"$sql_create_table"
echo "***********************************上内容执行可能会有错误提示，请忽略**************************"
```

这里创建了远程账号rtvsweb,数据库实例filecache,以及数据库表avinfo。

注意：mysql是运行在容器中的，我们在外部不能直接使用mysql -uroot -proot 来验证访问，因为它是一个微型的容器实例有自己的ip地址，如172.17.0.2 ，容器默认的网关是172.17.0.1。我们访问容器实例可以通过外部的mysql客户端如navicat来验证远程账号。如果本地已经安装过mysql 可以使用如：mysql -h172.17.0.2  -urtvsweb -prtvs2018 来访问容器版本的mysql。

非容器管理的MySQL安装请参考：[shell脚本实现msyql5.7全自动安装](https://blog.csdn.net/huoyuanshen/article/details/76922175)



