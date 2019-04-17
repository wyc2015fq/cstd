# Shell 基于Docker操作InfluxDB安装建库创建用户并授权 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月14日 18:25:29[boonya](https://me.csdn.net/boonya)阅读数：273








InfluxDB提供了一些辅助命令，使得我们可以不用进入到influxdb控制台里面就可以操作，这点很像mysql，所以实现起来很方便快捷。

**目录**

[安装脚本](#%E5%AE%89%E8%A3%85%E8%84%9A%E6%9C%AC)

[数据库操作脚本](#%E6%95%B0%E6%8D%AE%E5%BA%93%E6%93%8D%E4%BD%9C%E8%84%9A%E6%9C%AC)

[执行输出](#%E6%89%A7%E8%A1%8C%E8%BE%93%E5%87%BA)

### 安装脚本

docker_influxdb_install.sh

```bash
#! /bin/bash
#file:docker_influxdb_install.sh
#company:cvnavi.com
#author:Pengjunlin
echo "当前执行文件......$0"
IS_EXISTS_INFLUXDB_IMAGE_NAME="false"
IS_EXISTS_INFLUXDB_CONTAINER="false"
IS_EXISTS_INFLUXDB_CONTAINER_RUNGING="false"
START_CONTAINER_CHECK_MAX_TIMES=3
START_CONTAINER_CHECK_CURRENT=1
# ========================下载镜像======================================
for i in [ `docker images ` ]; do
	
	if [[ "$i" == "docker.io/influxdb" ||  "$i" == "influxdb" ]]; then
		echo "$i"
		IS_EXISTS_INFLUXDB_IMAGE_NAME="true"
		break
	fi
done
if [[ $IS_EXISTS_INFLUXDB_IMAGE_NAME == "true"  ]]; then
	echo "本地已存在influxdb:latest镜像，不再重新下载......."
else
	echo "本地不存在influxdb:latest镜像，正在下载......."
	docker pull influxdb:latest
fi

# ====================创建镜像===========================================
if [[ $IS_EXISTS_INFLUXDB_CONTAINER == "false" ]]; then
	echo "检查influxdb容器是否创建......"
	for i in [ `docker ps -a` ]; do
		if [[ "$i" == "influxdb" ]]; then
			IS_EXISTS_INFLUXDB_CONTAINER="true"
			break
		fi
	done
	if [[ $IS_EXISTS_INFLUXDB_CONTAINER == "false" ]]; then
		cp docker_influxdb_create_user.sh /etc/influxdb/scripts/docker_influxdb_create_user.sh
	    chmod a+x /etc/influxdb/scripts/docker_influxdb_create_user.sh
		if [[ -f "/etc/influxdb/scripts/docker_influxdb_create_user.sh" ]]; then
			echo "检查到influxdb容器尚未创建!"
	        # 执行容器创建
			# 运行容器实例 --privileged=true 获取管理员权限
			echo "创建influxdb容器实例..."
			sudo docker run -d -p 38083:8083 -p 38086:8086  --name influxdb  --restart always --privileged=true -v /etc/influxdb/scripts:/etc/influxdb/scripts  influxdb:latest
			# 休10秒钟
			echo "休眠等待10s以便Docker完成容器运行......"
			sleep 10s
	        echo "进入influxdb容器: docker exec -it influxdb  /bin/bash -c 'sh /etc/influxdb/scripts/docker_influxdb_create_user.sh'"
			# 进入容器并执行脚本：
			docker exec -it influxdb  /bin/bash -c "sh /etc/influxdb/scripts/docker_influxdb_create_user.sh"
			# 删除执行文件
			rm -f /etc/influxdb/scripts/docker_influxdb_create_user.sh

			echo "influxdb容器已创建完毕!"

			IS_EXISTS_INFLUXDB_CONTAINER_RUNGING=true
		else
			echo "/etc/influxdb/scripts/docker_influxdb_create_user.sh文件不存在，docker需要用此文件创建influxdb容器实例并创建用户."
			exit 1
		fi
	else
		echo "检查到influxdb容器已创建!"
	fi
fi
# ===================启动或重启容器================================
if [[ $IS_EXISTS_INFLUXDB_CONTAINER == "true" && $IS_EXISTS_INFLUXDB_CONTAINER_RUNGING == "false" ]]; then
    echo "下面最多执行三次influxdb容器检查重启..."
	while [[ $START_CONTAINER_CHECK_CURRENT -le $START_CONTAINER_CHECK_MAX_TIMES ]]; do
		echo "检查influxdb容器状态......$START_CONTAINER_CHECK_CURRENT"
		for i in [ `docker ps ` ]; do
			if [[ "$i" == "influxdb" ]]; then
				IS_EXISTS_INFLUXDB_CONTAINER_RUNGING="true"
				break
			fi
		done
		if [[ $IS_EXISTS_INFLUXDB_CONTAINER_RUNGING == "false" ]]; then
			echo "检查到influxdb容器当前不在运行状态!"
			echo "启动influxdb容器...."
			docker start influxdb
			for i in [ `docker ps ` ]; do
				if [[ "$i" == "influxdb" ]]; then
					IS_EXISTS_INFLUXDB_CONTAINER_RUNGING="true"
					break
				fi
			done
			if [[ $IS_EXISTS_INFLUXDB_CONTAINER_RUNGING == "true" ]]; then
				echo "influxdb容器已经在运行!"
				break
			fi
		else
			echo "influxdb容器已经在运行!"
			break
		fi
		START_CONTAINER_CHECK_CURRENT=$((START_CONTAINER_CHECK_CURRENT+1))
	done
	if [[ $IS_EXISTS_INFLUXDB_CONTAINER_RUNGING == "false" ]]; then
		echo "检查到influxdb容器当前仍未运行,请联系相关人员进行处理!"
		exit 1
	fi
fi
```

### 数据库操作脚本

docker_influxdb_create_user.sh

```bash
#！/bin/bash
#file:docker_influxdb_create_user.sh
#company:cvnavi.com
#author:Pengjunlin
echo "当前执行文件......$0"
INFLUXDB_DATABASE_NAME="rtvsweb"
INFLUXDB_USER_NAME="admin"
INFLUXDB_USER_PWD="admin"
# influxdb数据库相关配置
echo "influxdb数据库相关配置"
influx -version
# 查询数据库列表
echo "查询数据库列表"
influx  -execute "show databases"
# 删除数据库
echo "删除数据库$INFLUXDB_DATABASE_NAME"
influx  -execute "drop database $INFLUXDB_DATABASE_NAME"
# 创建数据库
echo "创建数据库$INFLUXDB_DATABASE_NAME"
influx  -execute "create database $INFLUXDB_DATABASE_NAME"
# 创建用户并授权
echo "创建$INFLUXDB_USER_NAME用户并授权"
influx -execute "create user "$INFLUXDB_USER_NAME" with password '$INFLUXDB_USER_PWD' with all privileges"
# 查询用户列表
echo "查询用户列表"
influx -execute "use $INFLUXDB_DATABASE_NAME" -execute  "show users"
```

### 执行输出

```bash
=================InfluxDB检查安装:======================start
安装Docker Influxdb环境...
当前执行文件......./docker_influxdb_install.sh
influxdb
本地已存在influxdb:latest镜像，不再重新下载.......
检查influxdb容器是否创建......
检查到influxdb容器尚未创建!
创建influxdb容器实例...
22979c1f0600d218105f8665a6c0b21dc867b04d7873ad2b17d9f92cf5a2ed5d
休眠等待10s以便Docker完成容器运行......
进入influxdb容器: docker exec -it influxdb  /bin/bash -c 'sh /etc/influxdb/scripts/docker_influxdb_create_user.sh'
当前执行文件....../etc/influxdb/scripts/docker_influxdb_create_user.sh
influxdb数据库相关配置
InfluxDB shell version: 1.7.0
Enter an InfluxQL query
查询数据库列表
name: databases
name
----
删除数据库rtvsweb
创建数据库rtvsweb
创建admin用户并授权
查询用户列表
user  admin
----  -----
admin true
influxdb容器已创建完毕!
./docker_influxdb_install.sh 执行完成!
=================InfluxDB检查安装:======================end
InfluxDB时序数据库安装成功!
```

注意：如果程序采用link连接的话端口号仍然是8086，想让Docker重启了容器自动启动运行需要配置 --restart always。



