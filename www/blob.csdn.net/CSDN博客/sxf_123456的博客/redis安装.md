# redis安装 - sxf_123456的博客 - CSDN博客
2018年01月11日 19:50:19[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：125
个人分类：[安装文档](https://blog.csdn.net/sxf_123456/article/category/7396488)
**安装redis**
1、  从redis官网下载
https://redis.io/
2、  解压
3、  编译
make
4、  安装
make install
5、  创建bin和etc文件夹
mkdir -p/usr/local/redis/bin
mkdir -p/usr/local/redis/etc
6、  将解压后存在的redis.conf移动到/usr/local/redis/etc/
mv  redis.conf /usr/local/redis/etc
7、  进入解压后的src文件夹
         mvmkreleasedhdr.sh redis-benchmark redis-check-aof redis-check-rdb redis-cliredis-server /usr/local/redis/bin
8、  修改配置文件，设置后台运行(将daemonize
设置为yes)
[root@ELK2etc]# vi /usr/local/redis/etc/redis.conf
9、  复制配置文件
1.在/etc下新建redis文件夹
[root@ELK2etc]# mkdir /etc/redis
2.把安装redis目录里面的redis.conf文件复制到/etc/redis/6379.conf里面,6379.conf是取的文件名称,启动脚本里面的变量会读取这个名称,所以要是redis的端口号改了,这里也要修改
[root@ELK2 etc]# cp /usr/local/redis/etc/redis.conf/etc/redis/6379.conf
10、             复制redis启动脚本
#1.redis启动脚本一般在redis根目录的utils,如果不知道路径,可以先查看路径
[root@ELK2etc]# find / -name redis_init_script
/home/sctele/redis-4.0.6/utils/redis_init_script
#2.复制启动脚本到/etc/init.d/redis文件中
[root@localhostredis]# cp /home/sctele/redis-4.0.6/utils/redis_init_script  /etc/init.d/redis
11、             修改启动脚本
[root@ELK2etc]#  vi /etc/init.d/redis
#在/etc/init.d/redis文件的头部添加下面两行注释代码,也就是在文件中#!/bin/sh的下方添加
# chkconfig:2345 10 90  
#description: Start and Stop redis 
修改参数,指定redis的安装路径
EXEC=/usr/local/redis/bin/redis-server
CLIEXEC=/usr/local/redis/bin/redis-cli
  修改完后的截图
12、             启动redis
打开redis命令:serviceredis start
关闭redis命令:service redis stop
设为开机启动:chkconfig redis on
设为开机关闭:chkconfig redis off
方法二：
设置redis开机启动
1.编写脚本
```
[root@localhost ~]# vi /etc/init.d/redis
```
复制下面代码到脚本中(注意要修改里面redis的安装路径,以/usr/redis/redis-3.2.4路径为例)(这段代码就是redis根目录 /utils/redis_init_script 启动脚本的代码)
```
#!/bin/sh
# chkconfig: 2345 10 90  
# description: Start and Stop redis   
REDISPORT=6379
EXEC=/usr/redis/redis-3.2.4/src/redis-server
CLIEXEC=/usr/redis/redis-3.2.4/src/redis-cli
PIDFILE=/var/run/redis_${REDISPORT}.pid
CONF="/usr/redis/redis-3.2.4/redis.conf"
case "$1" in
    start)
        if [ -f $PIDFILE ]
        then
                echo "$PIDFILE exists, process is already running or crashed"
        else
                echo "Starting Redis server..."
                $EXEC $CONF &
        fi
        ;;
    stop)
        if [ ! -f $PIDFILE ]
        then
                echo "$PIDFILE does not exist, process is not running"
        else
                PID=$(cat $PIDFILE)
                echo "Stopping ..."
                $CLIEXEC -p $REDISPORT shutdown
                while [ -x /proc/${PID} ]
                do
                    echo "Waiting for Redis to shutdown ..."
                    sleep 1
                done
                echo "Redis stopped"
        fi
        ;;
    restart)
        "$0" stop
        sleep 3
        "$0" start
        ;;
    *)
        echo "Please use start or stop or restart as first argument"
        ;;
esac
```
3.保存退出,设置权限
```
[root@localhost ~]# chmod 777 /etc/init.d/redis
```
4.启动redis
```
[root@localhost ~]# service redis start
```
方法二来源:
http://www.cnblogs.com/skyessay/p/6433349.html
