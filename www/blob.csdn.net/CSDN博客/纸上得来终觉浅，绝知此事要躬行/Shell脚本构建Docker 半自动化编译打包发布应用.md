# Shell脚本构建Docker 半自动化编译打包发布应用 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年10月25日 17:50:50[boonya](https://me.csdn.net/boonya)阅读数：646标签：[Shell																[Docker](https://so.csdn.net/so/search/s.do?q=Docker&t=blog)](https://so.csdn.net/so/search/s.do?q=Shell&t=blog)
个人分类：[Docker																[Shell](https://blog.csdn.net/boonya/article/category/8324499)](https://blog.csdn.net/boonya/article/category/6559498)







Docker 发布的方式为实现DevOps（自动化运维，需要配置 代码管理工具如Git的hook来实现提交编译、打包、发布等一系列事件控制）提供了诸多方便，辅助于Shell脚本可以很好的配合起来。Docker镜像可以通过自动化编译，也可以通过导入镜像的方式来获得。本文的题目叫：Shell脚本构建Docker 半自动化编译打包发布，因为我这里还没跟代码管理关联起来。

### Shell脚本不同实现

导入镜像方式（镜像文件rtvsweb-publish.tar）：

![](https://img-blog.csdn.net/20181025174112333?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

构建发布方式（依靠Dockerfile来构建）：

![](https://img-blog.csdn.net/20181025174207515?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###  Shell Docker构建发布脚本工具核心install.sh

```bash
#! /bin/bash
##file:rtvs.sh
##company:cvnavi.com
##author:Pengjunlin
echo "当前执行文件......$0"
##################################变量定义##################################
DOCKER_IMAGE_IS_MATCH_TAR_FILE="false"
DOCKER_RTVSWEB_CONTAINER_NAME="rtvsweb-publish"
DOCKER_RTVSWEB_NEW_VERSION=""
DOCKER_RTVSWEB_FIRST_VERSION=1
DOCKER_RTVSWEB_SECOND_VERSION=0
DOCKER_RTVSWEB_THIRD_VERSION=0
DOCKER_RTVSWEB_VERSION_TAG=""
DOCKER_CONTAINER_TEMP_HOST=""
DB_REDIS_CONNECTION_STRING=""
DB_MYSQL_CONNECTION_STRING=""
DB_MYSQL_HOST=""
MASTER_LAN_IP=""
SYSTEM_ALLOW_TO_SET="false"
###################################函数定义#######################################
function init_files()
{
	# 创建rtvs目录
	if [[ ! -d "/usr/local/rtvs" ]]; then
		echo "创建容器映射路径.... /usr/local/rtvs"
		mkdir /usr/local/rtvs
	fi
	# 复制VersionConfig.xml（第一次做完全复制，若有变动需要手动修改）
	if [[ -f "./VersionConfig.xml" ]]; then
		if [[ ! -f "/usr/local/rtvs/VersionConfig.xml" ]]; then
			echo "拷贝一份XML配置文件： ./VersionConfig.xml /usr/local/rtvs/VersionConfig.xml"
			cp VersionConfig.xml /usr/local/rtvs/VersionConfig.xml
		fi
	else
		echo "缺少./VersionConfig.xml文件...已退出安装!"
		exit
	fi
	# 复制SettingConfig.xml（第一次做完全复制，若有变动需要手动修改）
	if [[ -f "./SettingConfig.xml" ]]; then
		if [[ ! -f "/usr/local/rtvs/SettingConfig.xml" ]]; then
			echo "拷贝一份XML配置文件： ./SettingConfig.xml /usr/local/rtvs/SettingConfig.xml"
			cp SettingConfig.xml /usr/local/rtvs/SettingConfig.xml
		fi
	else
		echo "缺少./SettingConfig.xml文件...已退出安装!"
		exit
	fi
	# 复制log4.config（第一次做完全复制，若有变动需要手动修改）
	if [[ -f "./log4.config" ]]; then
		if [[ ! -f "/usr/local/rtvs/log4.config" ]]; then
			echo "拷贝一份日志配置文件： ./log4.config /usr/local/rtvs/log4.config"
		    cp log4.config /usr/local/rtvs/log4.config
		fi
	else
		echo "缺少./log4.config文件...已退出安装!"
		exit
	fi
}
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
		    DB_MYSQL_HOST="localhost"
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
   DOCKER_CONTAINER_TEMP_HOST=` docker inspect --format '{{ .NetworkSettings.IPAddress }}' $1`
}

function init_docker_master_ip(){
	out=0
	for i in  `ip a |grep inet[^6]|grep -E -o '([0-9]{1,3}\.){3}[0-9]{1,3}'`; do
		OLD_IFS="$IFS" 
		IFS="." 
		arr=($i) 
		IFS="$OLD_IFS" 
		for s in ${arr[@]} ;do 
			if [[ $s -eq "192" ]]; then
				 echo "$i" 
				 MASTER_LAN_IP=$i
				 out=1
				 break
			fi
		done
		if [[ $out -eq 1 ]]; then
			break
		fi
	done
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
		echo "当前mysql5.7实例IP=$DOCKER_CONTAINER_TEMP_HOST"
		#echo "MySQL容器network 相关信息:"
		#docker network inspect share_network
        #echo "MySQL容器link相关信息:"
		#cat /etc/hosts
	else
		echo "./docker_mysql_validator.sh 执行过程中出现错误，已退出安装!"
		exit
	fi
	#link 访问方式
	DB_MYSQL_HOST="mysql5.7"
}

function destory_docker_service()
{
	# 停止容器
	for i in [ `docker ps ` ]; do
		if [[ "$i" == "rtvsweb-publish" ]]; then
			echo "尝试停止$DOCKER_RTVSWEB_CONTAINER_NAME容器..."
		    docker stop $DOCKER_RTVSWEB_CONTAINER_NAME
		fi
	done
    # 删除容器
	for i in [ `docker ps -a` ]; do
		if [[ "$i" == "rtvsweb-publish" ]]; then
			echo "尝试删除$DOCKER_RTVSWEB_CONTAINER_NAME容器..."
		docker rm $DOCKER_RTVSWEB_CONTAINER_NAME
		fi
	done
}

function docker_build_image()
{
	# 清理容器(保留历史构建的镜像)
	destory_docker_service

	if [[ $? -eq 0 ]]; then
		echo "应用服务容器和镜像已处理，配置文件已完成复制!"
	else
		exit 1
	fi
   
	echo "Docker镜像构建......cmd:(docker build -t rtvsweb:$DOCKER_RTVSWEB_NEW_VERSION .)"
	docker build -t rtvsweb:$DOCKER_RTVSWEB_NEW_VERSION .

	# 判断是否有镜像,存在时创建相应的容器实例
	for i in [ `docker images` ]; do
		#statements
		if [[  "$i" == "$DOCKER_RTVSWEB_NEW_VERSION" ]]; then
			DOCKER_IMAGE_IS_MATCH_TAR_FILE="true"
			echo "已经找到最新构建的镜像!"
			run_docker_service_image
			break
		fi
	done
	if [[ $DOCKER_IMAGE_IS_MATCH_TAR_FILE == "false" ]]; then
		echo "构建镜像未匹配到最新版本，已退出安装!"
		exit 1
	fi

	echo "构建后的docker images镜像列表:"
	docker images

	echo "当前正在运行的Docker容器实例列表:"
	docker ps
}

function run_docker_service_image()
{
	echo "正在启动docker 服务容器......"
	# run 的参数设置参考VS项目启动的设置--privileged=true 获取管理员权限
	# ===bridge 桥接模式
	#docker run -it --name=$DOCKER_RTVSWEB_CONTAINER_NAME --net="bridge" --dns 8.8.8.8 --network share_network --network-alias $4  -v /usr/local/rtvs/error_log:/app/Error_log -v /usr/local/rtvs/log_file:/app/LogFile -v /usr/local/rtvs/mydata:/MyData -v /usr/local/rtvs/videocache:/MyVideo  -v /usr/local/rtvs/SettingConfig.xml:/app/SettingConfig.xml -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700:19700 -p 19702:19702 -p 19703:19703 -d $DOCKER_RTVSWEB_IMAGE_TAG_NAME
	if [[ $DB_MYSQL_HOST == "mysql5.7" ]]; then
		# ===link 链接模式
		echo "rtvsweb--容器运行---链接模式"
		docker run -it --name $DOCKER_RTVSWEB_CONTAINER_NAME  --privileged=true --link mysql5.7:mysql5.7 -v /usr/local/rtvs:/MyData -e MyDataPath=/MyData -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700-19719:19700-19719 -p 30888-30889:30888-30889  -d rtvsweb:$DOCKER_RTVSWEB_NEW_VERSION
	else
		# ===普通模式
		echo "rtvsweb--容器运行---普通模式"
		docker run -it --name $DOCKER_RTVSWEB_CONTAINER_NAME  --privileged=true -v /usr/local/rtvs:/MyData -e MyDataPath=/MyData -p 38067:80 -p 44383:443  -p 18000:18000 -p 18002:18002 -p 19700-19719:19700-19719  -p 30888-30889:30888-30889 -d rtvsweb:$DOCKER_RTVSWEB_NEW_VERSION
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

function init_mysql_conn()
{
	if [[ "$DB_MYSQL_HOST" == "localhost" ]]; then
		mysql="Database=filecache;Data Source=$MASTER_LAN_IP;port=3366;User Id=rtvsweb;Password=rtvs2018;charset=utf8;pooling=true"
		sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString>/g" /usr/local/rtvs/SettingConfig.xml
	    DB_MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	else
		mysql="Database=filecache;Data Source=mysql5.7;port=3306;User Id=rtvsweb;Password=rtvs2018;charset=utf8;pooling=true"
		sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString>/g" /usr/local/rtvs/SettingConfig.xml
	    DB_MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
	fi
}


function editSpecificConfig()
{
    init_mysql_conn
	echo "获取/usr/local/rtvs/SettingConfig.xml信息..."
	cat /usr/local/rtvs/SettingConfig.xml

    # 判断是否可以设置
	if [[ "$SYSTEM_ALLOW_TO_SET" == "true" ]]; then
		read -p ">>>是否修改Redis配置连接字符串？y/n：" ans
		echo "$ans"
		if [[ "$ans" == "y" ]]; then
			read -p "请输入Redis配置连接字符串:" redis
			echo "新的Redis连接字符串:$redis"
	        sed -i "s/<RedisExchangeHosts>.*<\/RedisExchangeHosts>/<RedisExchangeHosts>$redis<\/RedisExchangeHosts>/g" /usr/local/rtvs/SettingConfig.xml
			DB_REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
			echo "获取已修改的Redis配置连接字符串...RedisExchangeHosts=$DB_REDIS_CONNECTION_STRING"
		fi

		read -p ">>>是否修改Mysql配置连接字符串？y/n：" ans
		echo "$ans"
		if [[ "$ans" == "y" ]]; then
			read -p "请输入Mysql配置连接字符串:" mysql
			echo "新的Mysql连接字符串:$mysql"
	        sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString>/g" /usr/local/rtvs/SettingConfig.xml
			DB_MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
			echo "获取已修改的Mysql配置连接字符串...MysqlConnectionString=$DB_MYSQL_CONNECTION_STRING"
		fi
	fi
}

function remoteRedisAndMysqlConfig()
{

	echo "获取/usr/local/rtvs/SettingConfig.xml信息..."
	cat /usr/local/rtvs/SettingConfig.xml
	# 设置Redis连接字符串
	read -p "是否修改Redis配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Redis连接字符串:" redis
		echo "新的Redis连接字符串:$redis"
		sed -i "s/<RedisExchangeHosts>.*<\/RedisExchangeHosts>/<RedisExchangeHosts>$redis<\/RedisExchangeHosts>/g" /usr/local/rtvs/SettingConfig.xml
		DB_REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
		echo "获取已修改的Redis配置连接字符串...RedisExchangeHosts=$DB_REDIS_CONNECTION_STRING"
	fi
	

	read -p "是否修改Mysql配置连接字符串？y/n：" ans
	echo "$ans"
	if [[ "$ans" == "y" ]]; then
		read -p "请输入Mysql连接字符串:" mysql
		echo "新的Mysql连接字符串:$mysql"
		sed -i "s/<MysqlConnectionString>.*<\/MysqlConnectionString>/<MysqlConnectionString>$mysql<\/MysqlConnectionString>/g" /usr/local/rtvs/SettingConfig.xml
		DB_MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`
		echo "获取已修改的Mysql配置连接字符串...MysqlConnectionString=$DB_MYSQL_CONNECTION_STRING"
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
	version_step=1

    # 记录下一次版本号
	if [[ "$DOCKER_RTVSWEB_VERSION_TAG" == "VersionFirst" ]]; then
		DOCKER_RTVSWEB_FIRST_VERSION=$((DOCKER_RTVSWEB_FIRST_VERSION+version_step))
		sed -i "s/<VersionFirst>.*<\/VersionFirst>/<VersionFirst>$DOCKER_RTVSWEB_FIRST_VERSION<\/VersionFirst>/g" /usr/local/rtvs/VersionConfig.xml
		sed -i "s/<VersionSecond>.*<\/VersionSecond>/<VersionSecond>0<\/VersionSecond>/g" /usr/local/rtvs/VersionConfig.xml
		sed -i "s/<VersionThird>.*<\/VersionThird>/<VersionThird>0<\/VersionThird>/g" /usr/local/rtvs/VersionConfig.xml
	fi
	if [[ "$DOCKER_RTVSWEB_VERSION_TAG" == "VersionSecond" ]]; then
		DOCKER_RTVSWEB_SECOND_VERSION=$((DOCKER_RTVSWEB_SECOND_VERSION+version_step))
		sed -i "s/<VersionSecond>.*<\/VersionSecond>/<VersionSecond>$DOCKER_RTVSWEB_SECOND_VERSION<\/VersionSecond>/g" /usr/local/rtvs/VersionConfig.xml
		sed -i "s/<VersionThird>.*<\/VersionThird>/<VersionThird>0<\/VersionThird>/g" /usr/local/rtvs/VersionConfig.xml
	fi
	if [[ "$DOCKER_RTVSWEB_VERSION_TAG" == "VersionThird" ]]; then
		DOCKER_RTVSWEB_THIRD_VERSION=$((DOCKER_RTVSWEB_THIRD_VERSION+version_step))
		sed -i "s/<VersionThird>.*<\/VersionThird>/<VersionThird>$DOCKER_RTVSWEB_THIRD_VERSION<\/VersionThird>/g" /usr/local/rtvs/VersionConfig.xml
	fi

	VIDEO_CACHE_PATH=`grep -E -o -e '<VideoCachePath>.+</VideoCachePath>' /usr/local/rtvs/SettingConfig.xml | sed 's/<VideoCachePath>//g'|sed 's/<\/VideoCachePath>//g'`
	DB_REDIS_CONNECTION_STRING=`grep -E -o -e '<RedisExchangeHosts>.+</RedisExchangeHosts>' /usr/local/rtvs/SettingConfig.xml | sed 's/<RedisExchangeHosts>//g'|sed 's/<\/RedisExchangeHosts>//g'`
	DB_MYSQL_CONNECTION_STRING=`grep -E -o -e '<MysqlConnectionString>.+</MysqlConnectionString>' /usr/local/rtvs/SettingConfig.xml | sed 's/<MysqlConnectionString>//g'|sed 's/<\/MysqlConnectionString>//g'`

	echo "Redis连接字符串：$DB_REDIS_CONNECTION_STRING"
	echo "Mysql连接字符串：$DB_MYSQL_CONNECTION_STRING"
	echo "默认视频缓存目录：$VIDEO_CACHE_PATH"
	echo "正在运行的容器实例:"
	docker ps
    echo "容器对应的Ip地址:"
    docker_container_ip mysql5.7
    if [[ "$DOCKER_CONTAINER_TEMP_HOST" != "" ]]; then
        echo "mysql5.7     ---------------------$DOCKER_CONTAINER_TEMP_HOST" 
    fi
    docker_container_ip $DOCKER_RTVSWEB_CONTAINER_NAME
    echo "$DOCKER_RTVSWEB_CONTAINER_NAME  ---------------------$DOCKER_CONTAINER_TEMP_HOST"
    echo "验证rtvs访问：curl http://cvtsp.com:38067/"
	echo "******************安装完成!*****************"
}

function version_management()
{
    DOCKER_RTVSWEB_FIRST_VERSION=`grep -E -o -e '<VersionFirst>.+</VersionFirst>' /usr/local/rtvs/VersionConfig.xml | sed 's/<VersionFirst>//g'|sed 's/<\/VersionFirst>//g'`
	DOCKER_RTVSWEB_SECOND_VERSION=`grep -E -o -e '<VersionSecond>.+</VersionSecond>' /usr/local/rtvs/VersionConfig.xml | sed 's/<VersionSecond>//g'|sed 's/<\/VersionSecond>//g'`
	DOCKER_RTVSWEB_THIRD_VERSION=`grep -E -o -e '<VersionThird>.+</VersionThird>' /usr/local/rtvs/VersionConfig.xml | sed 's/<VersionThird>//g'|sed 's/<\/VersionThird>//g'`
	DOCKER_RTVSWEB_VERSION_TAG=`grep -E -o -e '<UpgradeTag>.+</UpgradeTag>' /usr/local/rtvs/VersionConfig.xml | sed 's/<UpgradeTag>//g'|sed 's/<\/UpgradeTag>//g'`

	DOCKER_RTVSWEB_NEW_VERSION="$DOCKER_RTVSWEB_FIRST_VERSION.$DOCKER_RTVSWEB_SECOND_VERSION.$DOCKER_RTVSWEB_THIRD_VERSION"
    echo "当前rtvsweb版本:$DOCKER_RTVSWEB_NEW_VERSION"
    echo "当前rtvsweb版本升级格式:$DOCKER_RTVSWEB_VERSION_TAG"
}

function by_local_mysql_installer()
{
	# Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_local_mysql_installer"
    # 镜像构建
    if [[ $? -eq 0 ]]; then
		docker_build_image
		if [[ $? -eq 0 ]]; then
			echo "镜像构建成功!"
		else
			echo "镜像构建失败!"
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
    # 镜像构建
    if [[ $? -eq 0 ]]; then
		docker_build_image
		if [[ $? -eq 0 ]]; then
			echo "镜像构建成功!"
		else
			echo "镜像构建失败!"
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

function by_smart_installer()
{
    # Docker 安装
    docker_install
    # Mysql 安装及其脚本创建
    mysql_install "by_smart_installer"
    # 镜像构建
    if [[ $? -eq 0 ]]; then
		docker_build_image
		if [[ $? -eq 0 ]]; then
			echo "镜像构建成功!"
		else
			echo "镜像构建失败!"
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

function by_simple_installer()
{
    # Docker 安装
    docker_install   
    # 镜像构建
    docker_build_image 
    if [[ $? -eq 0 ]]; then
		echo "镜像构建成功!"
	else
		echo "镜像构建失败!"
		exit 
	fi
    # 修改镜像使用的路径和数据源
    remoteRedisAndMysqlConfig
    # 完成后输出
    completed
}


if [[  $# -gt 0  ]]; then
	echo "安装所需文件检查...."
	init_files
	if [[ $? -eq 0 ]]; then
		echo "安装Docker服务的文件存在，验证通过！"
	else
		echo "安装Docker服务的文件存在缺失,已退出安装！"
		exit 
	fi

    echo "rtvsweb版本检查...."
    version_management

    echo "获取本地局域网IP地址...."
	init_docker_master_ip
	echo "本地IP地址....MASTER_LAN_IP=$MASTER_LAN_IP"
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

### 整体设计实现思路

1、Docker环境准备和安装

2、MySQL环境准备和安装

3、Docker的知识：构建、运行

4、Docker镜像的版本管理

![](https://img-blog.csdnimg.cn/2018102615000465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_27,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20181026145950538.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_27,color_FFFFFF,t_70)

5、容期间网络访问：--link

6、镜像打包历史

![](https://img-blog.csdn.net/20181025175257794?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

7、服务运行效果

![](https://img-blog.csdnimg.cn/20181026150048740.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Jvb255YQ==,size_27,color_FFFFFF,t_70)





