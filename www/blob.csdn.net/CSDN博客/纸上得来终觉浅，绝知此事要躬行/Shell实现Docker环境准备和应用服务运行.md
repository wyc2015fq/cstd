# Shell实现Docker环境准备和应用服务运行 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月11日 14:09:53[boonya](https://me.csdn.net/boonya)阅读数：313








### 考虑问题

**还是根据之前的Shell编写经验，本文需要注意几个问题：**
- Docker 的容器IP获取
- 如何进入MySQL容器执行数据库脚本
- Docker应用镜像的导入导出
- Shell 脚本sed -i 修改XML参数空格问题导致参数个数解析多于预期
- 如何设计脚本执行的命令方式入口及帮助文档

### 示例脚本

**下面是回答上面问题的一个脚本：install.sh**

```bash
#! /bin/bash
#file:rtvs.sh
#company:cvnavi.com
#author:Pengjunlin
echo "当前执行文件......$0"
##################################变量定义##################################
docker_image_id="f37318a5729b"
docker_image_file="rtvsweb-dev.tar"
docker_mysql_remote_user_name="rtvsweb"
docker_mysql_remote_user_pwd="rtvs2018"
docker_mysql_container_name="mysql5.7"
docker_mysql_gateway="172.17.0.1"
docker_mysql_host="172.17.0.x"
install_docker_mysql_db="true"
docker_rtvsweb_image_tag_name="rtvsweb:dev"
docker_rtvsweb_container_name="rtvsweb-dev"

REDIS_CONNECTION_STRING=""
MYSQL_CONNECTION_STRING=""
VIDEO_CACHE_PATH=""
MYSQL_HOST=""
CONTAINER_HOST=""
###################################函数定义#######################################
function mysql_create_table()
{
	if [[ -f "./mysql_create_table.sh" ]]; then
		echo "数据库脚本赋值权限......"
		# 为执行文件添加权限
		chmod a+x mysql_create_table.sh
	else
		echo "缺少./mysql_create_table.sh文件...已退出安装!"
	    exit
	fi
	# 执行mysql数据库表创建
	./mysql_create_table.sh
	if [[ $? -eq 0 ]]; then
		echo "./mysql_docker_install.sh 执行完成!"
	else
		exit
	fi
}

function docker_install()
{
	echo "检查Docker......"
	docker -v
    if [ $? -eq  0 ]; then
        echo "检查到Docker已安装!"
    else
    	echo "安装docker环境..."
        curl -sSL https://get.daocloud.io/docker | sh
        echo "安装docker环境...安装完成!"
    fi
    # 创建公用网络==bridge模式
    #docker network create share_network
}

function mysql_install(){
	install_type=$1

	if [[ "$install_type" == "by_local_mysql_installer" ]]; then
		#statements
		echo "暂不支持wget本地下载安装，已退出！"
		exit 1
	fi

	if [[ "$install_type" == "by_docker_mysql_installer" ]]; then
		#statements
		echo "安装Docker Mysql环境..."
		if [[ -f "./docker_mysql_install.sh" ]]; then
			if [[ -f "./docker_mysql_create_table.sh" ]]; then
					echo "拷贝一份容器执行文件： ./docker_mysql_create_table.sh /usr/local/docker_mysql_create_table.sh"
					cp docker_mysql_create_table.sh /usr/local/docker_mysql_create_table.sh
			else
					echo "缺少./docker_mysql_create_table.sh文件...已退出安装!"
		            exit
			fi
			# 为执行文件添加权限
			chmod a+x docker_mysql_install.sh
			# Dokcer方式安装Mysql
			./docker_mysql_install.sh
			if [[ $? -eq 0 ]]; then
				echo "./docker_mysql_install.sh 执行完成!"
				# 休10秒钟
				echo "休眠等待10s,等待Docker执行完成......"
				sleep 10s
				# 测试打印mysql信息
				print_docker_mysql_info
			else
				echo "./docker_mysql_install.sh 执行过程中出现错误，已退出安装!"
				exit
			fi
		else
		        echo "缺少./docker_mysql_install.sh文件...已退出安装!"
		        exit
		fi
	fi

	if [[ "$install_type" == "by_smart_installer" ]]; then
		#statements
		if [[ `netstat -lanp|grep tcp|grep 3306|wc -l` == 1 ]]; then
			echo "检测到mysql已在本机安装！"
			# 打印一下mysql版本
			echo "检测到mysql版本:"
			mysql --version
			# 执行创建Mysql数据库脚本
		    mysql_create_table
		    # 设置数据库地址为localhost
			MYSQL_HOST="localhost"
		else
			echo "安装Docker Mysql环境..."
			if [[ -f "./docker_mysql_install.sh" ]]; then
				if [[ -f "./docker_mysql_create_table.sh" ]]; then
					echo "拷贝一份容器执行文件： ./docker_mysql_create_table.sh /usr/local/docker_mysql_create_table.sh"
					cp docker_mysql_create_table.sh /usr/local/docker_mysql_create_table.sh
				else
					echo "缺少./docker_mysql_create_table.sh文件...已退出安装!"
		            exit
				fi
				# 为执行文件添加权限
				chmod a+x docker_mysql_install.sh
				# Dokcer方式安装Mysql
				./docker_mysql_install.sh
				if [[ $? -eq 0 ]]; then
					echo "./docker_mysql_install.sh 执行完成!"
					# 休10秒钟
					echo "休眠等待10s,等待Docker执行完成......"
					sleep 10s
					# 测试打印mysql信息
					print_docker_mysql_info 
				else
					echo "./docker_mysql_install.sh 执行过程中出现错误，已退出安装!"
					exit
				fi
			else
		        echo "缺少./docker_mysql_install.sh文件...已退出安装!"
		        exit
		    fi
		fi
	fi
}

function docker_container_ip() {
   CONTAINER_HOST=` docker inspect --format '{{ .NetworkSettings.IPAddress }}' $1`
}

function print_docker_mysql_info()
{
	echo "脚本执行Mysql信息验证:..."
	if [[ -f "./docker_mysql_validator.sh" ]]; then
		echo "拷贝一份容器执行文件： ./docker_mysql_validator.sh /usr/local/docker_mysql_validator.sh"
		cp docker_mysql_validator.sh /usr/local/docker_mysql_validator.sh
	else
		echo "缺少./docker_mysql_validator.sh文件...已退出安装!"
		exit
 	fi

	# 映射copy文件路径到docker容器
	docker cp /usr/local/docker_mysql_validator.sh mysql5.7:/usr/local/docker_mysql_validator.sh

	docker exec -it mysql5.7  /bin/bash -c "sh /usr/local/docker_mysql_validator.sh"
	if [[ $? -eq 0 ]]; then
		echo "./docker_mysql_validator.sh 执行完成!"
		echo "MySQL容器Host:"
		docker_container_ip mysql5.7 
		MYSQL_HOST="mysql5.7"
		#echo "MySQL容器network 相关信息:"
		#docker network inspect share_network
        echo "MySQL容器link相关信息:"
		cat /etc/hosts
	else
		echo "./docker_mysql_validator.sh 执行过程中出现错误，已退出安装!"
		exit
	fi
}

function docker_image_need_file_path()
{
	# 创建rtvs目录
	if [[ ! -d "/usr/local/rtvs" ]]; then
		mkdir /usr/local/rtvs
	fi
	# 创建mydata目录
	if [[ ! -d "/usr/local/rtvs/mydata" ]]; then
		mkdir /usr/local/rtvs/mydata
	fi

	# 创建video目录
	if [[ ! -d "/usr/local/rtvs/videocache" ]]; then
		mkdir /usr/local/rtvs/videocache
	fi

	if [[ -f "./SettingConfig.xml" ]]; then
		echo "拷贝一份XML配置文件： ./SettingConfig.xml /usr/local/rtvs/SettingConfig.xml"
		cp SettingConfig.xml /usr/local/rtvs/SettingConfig.xml
	else
		echo "缺少./SettingConfig.xml文件...已退出安装!"
		exit
	fi
}

function docker_image_load()
{
	#处理镜像需要的文件
	docker_image_need_file_path

	echo "Docker镜像加载......"
	echo "Docker镜像导入......$1  执行指令: sudo docker load < $2 "
	sudo docker load < $2

	echo "导入镜像后的docker images列表:"
	docker images

	# 判断是否有镜像,存在时创建相应的容器实例
	for i in [ `docker images` ]; do
		#statements
		if [[  "$i" == "$1" ]]; then
			docker tag $1 $3
			echo "已经找到导入的镜像!"
			echo "尝试停止$4容器..."
			docker stop $4
			echo "尝试删除$4容器..."
			docker rm $4
			echo "启动容器..."
			# run 的参数设置参考VS项目启动的设置

			# ===bridge 桥接模式
			#docker run -it --name=$4 --net="bridge" --dns 8.8.8.8 --network share_network --network-alias $4 -v /usr/local/rtvs/mydata:/MyData -v /usr/local/rtvs/videocache:/MyVideo  -v /usr/local/rtvs/SettingConfig.xml:/app/SettingConfig.xml -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 -d $3
			# ===link 链接模式
			docker run -it --name $4 --link mysql5.7:mysql5.7 -v /usr/local/rtvs/mydata:/MyData -v /usr/local/rtvs/videocache:/MyVideo  -v /usr/local/rtvs/SettingConfig.xml:/app/SettingConfig.xml -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 -d $3
			break
		fi
	done
	echo "当前正在运行的Docker容器实例列表:"
	docker ps
}

####################工具类和流程定制################################

function editXml()
{
    val=`echo ${@:3}`
    echo "正在修改XML文件:$1...."
    echo "正在修改XML文件:[0]=$1,[1]=$2,[2]=$val"
    echo "XML文件$2标签 value=$val"
    sed -i "s/<$2>.*<\/$2>/<$2>${val}<\/$2></g" $1
}


function editSpecificConfig()
{
	echo "获取/usr/local/rtvs/SettingConfig.xml信息..."
	
	cat /usr/local/rtvs/SettingConfig.xml
	echo ""
	echo "修改Redis和MySQL连接字符串..."

    #设置视频缓存路径并打印默认信息
	#editXml VideoCachePath /MyVideo /usr/local/rtvs/SettingConfig.xml
    sed -i "s/<VideoCachePath>.*<\/VideoCachePath>/<VideoCachePath>\/MyVideo<\/VideoCachePath></g" /usr/local/rtvs/SettingConfig.xml


	VIDEO_CACHE_PATH=`grep -E -o -e '<VideoCachePath>.+</VideoCachePath>' /usr/local/rtvs/SettingConfig.xml | sed 's/<VideoCachePath>//g'|sed 's/<\/VideoCachePath>//g'`
	echo "默认视频缓存目录：VideoCachePath=$VIDEO_CACHE_PATH"

	REDIS_CONNECTON_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
	echo "默认Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTON_STRING"

	MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	echo "默认Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"

	read -p "是否修改Redis配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Redis配置连接字符串:" redis
		echo "新的Redis连接字符串:$redis"
        sed -i "s/<RedisExchangeHosts>.*<\/RedisExchangeHosts>/<RedisExchangeHosts>$redis<\/RedisExchangeHosts></g" /usr/local/rtvs/SettingConfig.xml
		REDIS_CONNECTON_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
		echo "获取已修改的Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTON_STRING"
	fi

	#Mysql配置连接字符串
	echo "根据安装的Mysql配置连接字符串...MySQL Host:$MYSQL_HOST"
	mysql="Database=filecache;Data Source=$MYSQL_HOST;port=3306;User Id=rtvsweb;Password=rtvs2018;charset=utf8;pooling=true"
    echo "安装MySQL的连接字符串：$mysql" 
    sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
    
    MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	echo "根据安装条件修改Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"

	read -p "是否修改Mysql配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Mysql配置连接字符串:" mysql
		echo "新的Mysql连接字符串:$mysql"
                sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
		MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
		echo "获取已修改的Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"
	fi
}

function editRedisAndMysqlConfig()
{
	echo "获取/usr/local/rtvs/SettingConfig.xml信息..."
	
	cat /usr/local/rtvs/SettingConfig.xml
	echo ""
	echo "修改Redis和MySQL连接字符串..."

    #设置视频缓存路径并打印默认信息
	#editXml VideoCachePath /MyVideo /usr/local/rtvs/SettingConfig.xml
    sed -i "s/<VideoCachePath>.*<\/VideoCachePath>/<VideoCachePath>\/MyVideo<\/VideoCachePath></g" /usr/local/rtvs/SettingConfig.xml


	VIDEO_CACHE_PATH=`grep -E -o -e '<VideoCachePath>.+</VideoCachePath>' /usr/local/rtvs/SettingConfig.xml | sed 's/<VideoCachePath>//g'|sed 's/<\/VideoCachePath>//g'`
	echo "默认视频缓存目录：VideoCachePath=$VIDEO_CACHE_PATH"

	REDIS_CONNECTON_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
	echo "默认Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTON_STRING"

	MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	echo "默认Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"

	# 设置Redis连接字符串
	read -p "是否修改Redis配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Redis连接字符串:" redis
		echo "新的Redis连接字符串:$redis"
		sed -i "s/<RedisExchangeHosts>.*<\/RedisExchangeHosts>/<RedisExchangeHosts>$redis<\/RedisExchangeHosts></g" /usr/local/rtvs/SettingConfig.xml
		REDIS_CONNECTON_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
		echo "获取已修改的Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTON_STRING"
	fi
	

	read -p "是否修改Mysql配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Mysql连接字符串:" mysql
		echo "新的Mysql连接字符串:$mysql"
		sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
		MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
		echo "获取已修改的Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"
	fi
	
}

function help()
{
	echo "*******************************************"
	echo "*********       调用方法说明    ************"
	echo "*******************************************"
	echo "查看核心配置文件：cat ./SettingConfig.xml"
	echo "帮助方法说明：sh ./install.sh help"
	echo "修改Redis和MySQL连接字符串：sh ./install.sh editXml [XMLFilePath] [tag] ['value'] "
    echo "********************************************"
	echo "*********       快速定制安装    *************"
	echo "********************************************"
	echo "1、执行本地Mysql安装(wget下载文件安装...比较费时): sh ./install.sh by_local_mysql_installer"
	echo "2、执行Docker Mysql安装 : sh ./install.sh by_docker_mysql_installer"
	echo "3、智能检测决定Mysql安装方式(如果本机已有MySQL则直接使用): sh ./install.sh by_smart_installer"
	echo "4、简化安装只需要给出Redis和MySQL连接字符串即可(极简方式): sh ./install.sh by_simple_installer"
	echo "说明：\r 以上4种方式已包含了Docker应用的安装!"
}

function completed()
{
	echo "******************安装结果:*****************"
	# bridge 桥接模式
	#echo "network实例, share_network相关容器信息:"
	#docker network inspect share_network
	echo "正在运行的容器实例:"
	docker ps
        echo "容器对应的Ip地址:"
        docker_container_ip mysql5.7
        echo "mysql5.7     ---------------------$CONTAINER_HOST" 
        docker_container_ip rtvsweb-dev
        echo "rtvsweb-dev  ---------------------$CONTAINER_HOST"
        echo "验证rtvs访问：curl http://localhost:38067/"
	echo "******************安装完成!*****************"
}

function by_local_mysql_installer()
{
	# Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_local_mysql_installer"
    # 镜像导入和实例构建
    if [[ $? -eq 0 ]]; then
		docker_image_load $1 $2 $3 $4
    else
		exit 1
    fi
    # 修改镜像使用的路径和数据源
    editSpecificConfig
    # 完成后输出
    completed
}

function by_docker_mysql_installer()
{
    # Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_docker_mysql_installer"
    # 镜像导入和实例构建
    if [[ $? -eq 0 ]]; then
		docker_image_load $1 $2 $3 $4
	else
		exit 1
    fi
    # 修改镜像使用的路径和数据源
    editSpecificConfig
    # 完成后输出
    completed
}

function by_smart_installer()
{
    # Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_smart_installer"
    # 镜像导入和实例构建
    if [[ $? -eq 0 ]]; then
		docker_image_load $1 $2 $3 $4
    else
		exit 1
    fi
    # 修改镜像使用的路径和数据源
    editSpecificConfig
    # 完成后输出
    completed
}

function by_simple_installer()
{
    # Docker 安装
    docker_install   
    # 镜像导入和实例构建
    docker_image_load $1 $2 $3 $4
    # 修改镜像使用的路径和数据源
    editRedisAndMysqlConfig
    # 完成后输出
    completed
}


if [[  $# -gt 0  ]]; then
	#statements 
	if [[ "$1" == "by_local_mysql_installer" ]]; then
		#statements
		by_local_mysql_installer  $docker_image_id $docker_image_file $docker_rtvsweb_image_tag_name $docker_rtvsweb_container_name
	fi
	if [[ "$1" == "by_docker_mysql_installer" ]]; then
		#statements
		by_docker_mysql_installer  $docker_image_id $docker_image_file $docker_rtvsweb_image_tag_name $docker_rtvsweb_container_name
	fi
	if [[ "$1" == "by_smart_installer" ]]; then
		#statements
		by_smart_installer  $docker_image_id $docker_image_file $docker_rtvsweb_image_tag_name $docker_rtvsweb_container_name
	fi	
	if [[ "$1" == "by_simple_installer" ]]; then
		#statements
		by_simple_installer  $docker_image_id $docker_image_file $docker_rtvsweb_image_tag_name $docker_rtvsweb_container_name
	fi	
	if [[ "$1" == "editXml" ]]; then

		if [[  $# -eq 4  ]]; then
			#statements
			editXml  $2 $3 $4
		else
			echo "editXml参数个数不匹配！"

	    fi
	fi	
	if [[ "$1" == "help" ]]; then
		#statements
		help
	fi	
else
	help
fi
```

### 示例效果

帮助指令：

![](https://img-blog.csdn.net/20181011142804725?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

执行结果：

![](https://img-blog.csdn.net/20181011142714839?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

关于脚本和应用服务的说明：

```
上面缺失的文件我在这里就不提供了，主要看流程设计和帮助文档。我上面实现的是ASP.NET core web的
docker应用程序的部署环境，因为涉及到Nugget包，项目发布的时候采用的部署方式是框架依赖模式，它可以将
需要的*.dll文件都生成到publish目录下。其他语言如Java的应该比这个更简单（如采用微服务框架Spring 
Boot 打成jar包即可）。
```

### 示例改进

代码改进：

```bash
#! /bin/bash
##file:rtvs.sh
##company:cvnavi.com
##author:Pengjunlin
echo "当前执行文件......$0"
##################################变量定义##################################
DOCKER_IMAGE_ID="f37318a5729b"
DOCKER_IMAGE_FILE="rtvsweb-publish.tar"
DOCKER_RTVSWEB_IMAGE_TAG_NAME="rtvsweb:publish"
DOCKER_RTVSWEB_CONTAINER_NAME="rtvsweb-publish"
REDIS_CONNECTION_STRING=""
MYSQL_CONNECTION_STRING=""
VIDEO_CACHE_PATH=""
MYSQL_HOST=""
CONTAINER_HOST=""
LINK_MYSQL="false"
###################################函数定义#######################################
function mysql_create_table()
{
	if [[ -f "./mysql_create_table.sh" ]]; then
		echo "数据库脚本赋值权限......"
		# 为执行文件添加权限
		chmod a+x mysql_create_table.sh
	else
		echo "缺少./mysql_create_table.sh文件...已退出安装!"
	    exit
	fi
	# 执行mysql数据库表创建
	./mysql_create_table.sh
	if [[ $? -eq 0 ]]; then
		echo "./mysql_docker_install.sh 执行完成!"
	else
		exit
	fi
}

function docker_install()
{
	echo "检查Docker......"
	docker -v
    if [ $? -eq  0 ]; then
        echo "检查到Docker已安装!"
    else
    	echo "安装docker环境..."
        curl -sSL https://get.daocloud.io/docker | sh
        echo "安装docker环境...安装完成!"
    fi
    # 创建公用网络==bridge模式
    #docker network create share_network
}

function mysql_install(){
	install_type=$1

	if [[ "$install_type" == "by_local_mysql_installer" ]]; then
		#statements
		echo "暂不支持wget本地下载安装，已退出！"
		exit 1
	fi

	if [[ "$install_type" == "by_docker_mysql_installer" ]]; then
		#statements
		docker_mysql_install
	fi

	if [[ "$install_type" == "by_smart_installer" ]]; then
		#statements
		if [[ `netstat -lanp|grep tcp|grep 3306|wc -l` == 1 ]]; then
			echo "检测到mysql已在本机安装！"
			# 打印一下mysql版本
			echo "检测到mysql版本:"
			mysql --version
			# 执行创建Mysql数据库脚本
		    mysql_create_table
			if [[ $? -eq 0 ]]; then
				echo "本地MySQL脚本初始化完成!"
			else
				echo "本地MySQL脚本初始化失败!"
				exit 1
			fi
		    MYSQL_HOST="localhost"
		else
			docker_mysql_install
		fi
	fi
}

function docker_mysql_install(){
	echo "安装Docker Mysql环境..."
	if [[ -f "./docker_mysql_install.sh" ]]; then
		if [[ -f "./docker_mysql_create_table.sh" ]]; then
			echo "拷贝一份容器执行文件： ./docker_mysql_create_table.sh /usr/local/docker_mysql_create_table.sh"
			cp docker_mysql_create_table.sh /usr/local/docker_mysql_create_table.sh
		else
			echo "缺少./docker_mysql_create_table.sh文件...已退出安装!"
		    exit 1
		fi
		# 为执行文件添加权限
		chmod a+x docker_mysql_install.sh
		# Dokcer方式安装Mysql
		./docker_mysql_install.sh
		if [[ $? -eq 0 ]]; then
			echo "./docker_mysql_install.sh 执行完成!"
			# 休10秒钟
			echo "休眠等待10s,等待Docker执行完成......"
			sleep 10s
			# 测试打印mysql信息
			print_docker_mysql_info
		else
			echo "./docker_mysql_install.sh 执行过程中出现错误，已退出安装!"
		    exit 1
		fi
	else
		    echo "缺少./docker_mysql_install.sh文件...已退出安装!"
		    exit 1
	fi
}

function docker_container_ip() {
   CONTAINER_HOST=` docker inspect --format '{{ .NetworkSettings.IPAddress }}' $1`
}

function print_docker_mysql_info()
{
	echo "脚本执行Mysql信息验证:..."
	if [[ -f "./docker_mysql_validator.sh" ]]; then
		echo "拷贝一份容器执行文件： ./docker_mysql_validator.sh /usr/local/docker_mysql_validator.sh"
		cp docker_mysql_validator.sh /usr/local/docker_mysql_validator.sh
	else
		echo "缺少./docker_mysql_validator.sh文件...已退出安装!"
		exit
 	fi

	# 映射copy文件路径到docker容器
	docker cp /usr/local/docker_mysql_validator.sh mysql5.7:/usr/local/docker_mysql_validator.sh

	docker exec -it mysql5.7  /bin/bash -c "sh /usr/local/docker_mysql_validator.sh"
	if [[ $? -eq 0 ]]; then
		echo "./docker_mysql_validator.sh 执行完成!"

		echo "MySQL容器Host:"
		docker_container_ip mysql5.7 
		echo "当前mysql5.7实例IP=$CONTAINER_HOST"
		#echo "MySQL容器network 相关信息:"
		#docker network inspect share_network
        #echo "MySQL容器link相关信息:"
		#cat /etc/hosts
	else
		echo "./docker_mysql_validator.sh 执行过程中出现错误，已退出安装!"
		exit
	fi
	#link 访问方式
	MYSQL_HOST="mysql5.7"
}

function docker_image_need_file_path()
{
	# 创建rtvs目录
	if [[ ! -d "/usr/local/rtvs" ]]; then
		mkdir /usr/local/rtvs
	fi
	# 创建mydata目录
	if [[ ! -d "/usr/local/rtvs/mydata" ]]; then
		mkdir /usr/local/rtvs/mydata
	fi

	# 创建video目录
	if [[ ! -d "/usr/local/rtvs/videocache" ]]; then
		mkdir /usr/local/rtvs/videocache
	fi

	if [[ -f "./SettingConfig.xml" ]]; then
		echo "拷贝一份XML配置文件： ./SettingConfig.xml /usr/local/rtvs/SettingConfig.xml"
		cp SettingConfig.xml /usr/local/rtvs/SettingConfig.xml
	else
		echo "缺少./SettingConfig.xml文件...已退出安装!"
		exit
	fi
}

function docker_image_load()
{
	#处理镜像需要的文件
	docker_image_need_file_path

    #验证镜像是否存在
	exists_image_name="false"
	exists_image_tag="false"
	image_id_is_equal="false"
	for i in [ `docker images ` ]; do
		
		if [[ "$i" == "rtvsweb" ]]; then
			echo "$i"
			exists_image_name="true"
		fi
		if [[ "$i" == "publish" ]]; then
			echo "$i"
			exists_image_tag="true"
		fi
	done
	if [[ $exists_image_name == "true" && $exists_image_tag == "true" ]]; then
		echo "本地已存在镜像：$DOCKER_RTVSWEB_IMAGE_TAG_NAME"
		echo "尝试删除镜像：$DOCKER_RTVSWEB_IMAGE_TAG_NAME..."
		docker rmi $DOCKER_RTVSWEB_IMAGE_TAG_NAME
	fi
	echo "Docker镜像加载......"
	if [[ ! -f "./$DOCKER_IMAGE_FILE" ]]; then
		echo "Docker镜像文件$DOCKER_IMAGE_FILE ......不存在，已退出安装!"
		exit 1
	fi
	echo "Docker镜像导入......$DOCKER_IMAGE_ID  执行指令: sudo docker load < $DOCKER_IMAGE_FILE "
	sudo docker load < $DOCKER_IMAGE_FILE

	echo "导入镜像后的docker images列表:"
	docker images

	# 判断是否有镜像,存在时创建相应的容器实例
	for i in [ `docker images` ]; do
		#statements
		if [[  "$i" == "$DOCKER_IMAGE_ID" ]]; then
			image_id_is_equal="true"
			echo "已经找到导入的镜像!"
			run_service_image
			break
		fi
	done
	if [[ $image_id_is_equal == "false" ]]; then
		echo "导入镜像的ID不是$DOCKER_IMAGE_ID，已退出安装!"
		exit 1
	fi
	echo "当前正在运行的Docker容器实例列表:"
	docker ps
}

function run_service_image()
{
	docker tag $DOCKER_IMAGE_ID $DOCKER_RTVSWEB_IMAGE_TAG_NAME	

	echo "尝试停止$DOCKER_RTVSWEB_CONTAINER_NAME容器..."
	docker stop $DOCKER_RTVSWEB_CONTAINER_NAME

	echo "尝试删除$DOCKER_RTVSWEB_CONTAINER_NAME容器..."
	docker rm $DOCKER_RTVSWEB_CONTAINER_NAME

	echo "启动容器..."
	# run 的参数设置参考VS项目启动的设置
	# ===bridge 桥接模式
	#docker run -it --name=$DOCKER_RTVSWEB_CONTAINER_NAME --net="bridge" --dns 8.8.8.8 --network share_network --network-alias $4 -v /usr/local/rtvs/mydata:/MyData -v /usr/local/rtvs/videocache:/MyVideo  -v /usr/local/rtvs/SettingConfig.xml:/app/SettingConfig.xml -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 -d $DOCKER_RTVSWEB_IMAGE_TAG_NAME
	if [[ $LINK_MYSQL == "true" ]]; then
		# ===link 链接模式
		docker run -it --name $DOCKER_RTVSWEB_CONTAINER_NAME --link mysql5.7:mysql5.7 -v /usr/local/rtvs/mydata:/MyData -v /usr/local/rtvs/videocache:/MyVideo  -v /usr/local/rtvs/SettingConfig.xml:/app/SettingConfig.xml -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 -d $DOCKER_RTVSWEB_IMAGE_TAG_NAME
	else
		# ===普通方式
		docker run -it --name $DOCKER_RTVSWEB_CONTAINER_NAME  -v /usr/local/rtvs/mydata:/MyData -v /usr/local/rtvs/videocache:/MyVideo  -v /usr/local/rtvs/SettingConfig.xml:/app/SettingConfig.xml -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 -d $DOCKER_RTVSWEB_IMAGE_TAG_NAME
	fi
}

####################工具类和流程定制################################

function editXml()
{
    val=`echo ${@:3}`
    echo "正在修改XML文件:$1...."
    echo "正在修改XML文件:[0]=$1,[1]=$2,[2]=$val"
    echo "XML文件$2标签 value=$val"
    sed -i "s/<$2>.*<\/$2>/<$2>${val}<\/$2></g" $1
}


function editSpecificConfig()
{
	echo "获取/usr/local/rtvs/SettingConfig.xml信息..."
	
	cat /usr/local/rtvs/SettingConfig.xml
	echo ""
	echo "修改Redis和MySQL连接字符串..."

    #设置视频缓存路径并打印默认信息
	#editXml VideoCachePath /MyVideo /usr/local/rtvs/SettingConfig.xml
    sed -i "s/<VideoCachePath>.*<\/VideoCachePath>/<VideoCachePath>\/MyVideo<\/VideoCachePath></g" /usr/local/rtvs/SettingConfig.xml


	VIDEO_CACHE_PATH=`grep -E -o -e '<VideoCachePath>.+</VideoCachePath>' /usr/local/rtvs/SettingConfig.xml | sed 's/<VideoCachePath>//g'|sed 's/<\/VideoCachePath>//g'`
	echo "默认视频缓存目录：VideoCachePath=$VIDEO_CACHE_PATH"

	REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
	echo "默认Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTION_STRING"

	MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	echo "默认Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"

	read -p "是否修改Redis配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Redis配置连接字符串:" redis
		echo "新的Redis连接字符串:$redis"
        sed -i "s/<RedisExchangeHosts>.*<\/RedisExchangeHosts>/<RedisExchangeHosts>$redis<\/RedisExchangeHosts></g" /usr/local/rtvs/SettingConfig.xml
		REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
		echo "获取已修改的Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTION_STRING"
	fi

	#Mysql配置连接字符串
	echo "mysql---------settings----------init----------------start!"
	echo "根据安装的Mysql配置连接字符串...MySQL Host:$MYSQL_HOST"
	if [[ $MYSQL_HOST == "localhost" ]]; then
		mysql="Database=filecache;Data Source=$MYSQL_HOST;port=3306;User Id=rtvsweb;Password=rtvs2018;charset=utf8;pooling=true"
    	echo "安装MySQL的连接字符串：$mysql" 
    	sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
	else
		mysql="Database=filecache;Data Source=$MYSQL_HOST;port=3366;User Id=rtvsweb;Password=rtvs2018;charset=utf8;pooling=true"
    	echo "安装MySQL的连接字符串：$mysql" 
    	sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
	fi
	echo "mysql---------settings-----------init---------------end!"
    
    MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	echo "根据安装条件修改Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"

	read -p "是否修改Mysql配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Mysql配置连接字符串:" mysql
		echo "新的Mysql连接字符串:$mysql"
                sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
		MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
		echo "获取已修改的Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"
	fi
}

function editRedisAndMysqlConfig()
{
	echo "获取/usr/local/rtvs/SettingConfig.xml信息..."
	
	cat /usr/local/rtvs/SettingConfig.xml
	echo ""
	echo "修改Redis和MySQL连接字符串..."

    #设置视频缓存路径并打印默认信息
	#editXml VideoCachePath /MyVideo /usr/local/rtvs/SettingConfig.xml
    sed -i "s/<VideoCachePath>.*<\/VideoCachePath>/<VideoCachePath>\/MyVideo<\/VideoCachePath></g" /usr/local/rtvs/SettingConfig.xml


	VIDEO_CACHE_PATH=`grep -E -o -e '<VideoCachePath>.+</VideoCachePath>' /usr/local/rtvs/SettingConfig.xml | sed 's/<VideoCachePath>//g'|sed 's/<\/VideoCachePath>//g'`
	echo "默认视频缓存目录：VideoCachePath=$VIDEO_CACHE_PATH"

	REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
	echo "默认Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTION_STRING"

	MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	echo "默认Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"

	# 设置Redis连接字符串
	read -p "是否修改Redis配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Redis连接字符串:" redis
		echo "新的Redis连接字符串:$redis"
		sed -i "s/<RedisExchangeHosts>.*<\/RedisExchangeHosts>/<RedisExchangeHosts>$redis<\/RedisExchangeHosts></g" /usr/local/rtvs/SettingConfig.xml
		REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
		echo "获取已修改的Redis配置连接字符串...RedisExchangeHosts=$REDIS_CONNECTION_STRING"
	fi
	

	read -p "是否修改Mysql配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Mysql连接字符串:" mysql
		echo "新的Mysql连接字符串:$mysql"
		sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString></g" /usr/local/rtvs/SettingConfig.xml
		MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
		echo "获取已修改的Mysql配置连接字符串...MysqlConnectionString=$MYSQL_CONNECTION_STRING"
	fi
	
}

function help()
{
	echo "*******************************************"
	echo "*********       调用方法说明    ************"
	echo "*******************************************"
	echo "查看核心配置文件：cat ./SettingConfig.xml"
	echo "帮助方法说明：sh ./install.sh help"
	echo "修改Redis和MySQL连接字符串：sh ./install.sh editXml [XMLFilePath] [tag] ['value'] "
    echo "********************************************"
	echo "*********       快速定制安装    *************"
	echo "********************************************"
	echo "1、执行本地Mysql安装(wget下载文件安装...比较费时): sh ./install.sh by_local_mysql_installer"
	echo "2、执行Docker Mysql安装 : sh ./install.sh by_docker_mysql_installer"
	echo "3、智能检测决定Mysql安装方式(如果本机已有MySQL则直接使用): sh ./install.sh by_smart_installer"
	echo "4、简化安装只需要给出Redis和MySQL连接字符串即可(极简方式): sh ./install.sh by_simple_installer"
	echo "说明：\r 以上4种方式已包含了Docker应用的安装!"
}

function completed()
{
	echo "******************安装结果:*****************"
	# bridge 桥接模式
	#echo "network实例, share_network相关容器信息:"
	#docker network inspect share_network
	echo "Redis连接字符串：$REDIS_CONNECTION_STRING"
	echo "Mysql连接字符串：$MYSQL_CONNECTION_STRING"
	echo "默认视频缓存目录：$VIDEO_CACHE_PATH"
	echo "正在运行的容器实例:"
	docker ps
    echo "容器对应的Ip地址:"
    docker_container_ip mysql5.7
    if [[ "$CONTAINER_HOST" != "" ]]; then
        echo "mysql5.7     ---------------------$CONTAINER_HOST" 
    fi
    docker_container_ip $DOCKER_RTVSWEB_CONTAINER_NAME
    echo "$DOCKER_RTVSWEB_CONTAINER_NAME  ---------------------$CONTAINER_HOST"
    echo "验证rtvs访问：curl http://localhost:38067/"
	echo "******************安装完成!*****************"
}

function by_local_mysql_installer()
{
	# Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_local_mysql_installer"
    # 镜像导入和实例构建
    if [[ $? -eq 0 ]]; then
		docker_image_load
		if [[ $? -eq 0 ]]; then
			echo "镜像导入成功!"
		else
			echo "镜像导入失败!"
			exit 
		fi
    else
		exit
    fi
    # 修改镜像使用的路径和数据源
    editSpecificConfig
    # 完成后输出
    completed
}

function by_docker_mysql_installer()
{
    # Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_docker_mysql_installer"
    # 镜像导入和实例构建
    if [[ $? -eq 0 ]]; then
		docker_image_load
	else
		exit
    fi
    # 修改镜像使用的路径和数据源
    editSpecificConfig
    # 完成后输出
    completed
}

function by_smart_installer()
{
    # Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_smart_installer"
    # 镜像导入和实例构建
    if [[ $? -eq 0 ]]; then
		docker_image_load
    else
		exit
    fi
    # 修改镜像使用的路径和数据源
    editSpecificConfig
    # 完成后输出
    completed
}

function by_simple_installer()
{
    # Docker 安装
    docker_install   
    # 镜像导入和实例构建
    docker_image_load 
    # 修改镜像使用的路径和数据源
    editRedisAndMysqlConfig
    # 完成后输出
    completed
}


if [[  $# -gt 0  ]]; then
	#statements 
	if [[ "$1" == "by_local_mysql_installer" ]]; then
		#statements
		by_local_mysql_installer 
	fi
	if [[ "$1" == "by_docker_mysql_installer" ]]; then
		#statements
		by_docker_mysql_installer 
	fi
	if [[ "$1" == "by_smart_installer" ]]; then
		#statements
		by_smart_installer 
	fi	
	if [[ "$1" == "by_simple_installer" ]]; then
		#statements
		by_simple_installer 
	fi	
	if [[ "$1" == "editXml" ]]; then

		if [[  $# -eq 4  ]]; then
			#statements
			editXml  $2 $3 $4
		else
			echo "editXml参数个数不匹配！"

	    fi
	fi	
	if [[ "$1" == "help" ]]; then
		#statements
		help
	fi	
else
	help
fi
```

最终Docker应用跑起来了：

![](https://img-blog.csdn.net/20181015150929793?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



