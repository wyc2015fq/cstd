# MongoDB 开机自启动 - 左直拳的马桶_日用桶 - CSDN博客
2014年10月30日 21:11:49[左直拳](https://me.csdn.net/leftfist)阅读数：18432
MongoDB安装了以后，应当设置开机自启动。
假设启动命令如下：
sudo /db/mongodb/265/bin/mongod --config /db/conf/mongodb/mongodb.conf
方法有二：
1、把这句话写到把 /etc/rc.local，即可开机启动。
第二种：
2、将一个脚本放到/etc/init.d/
比如，命名为/etc/init.d/mongodb，则执行以下步骤即可：
1）要给它们赋予执行权限：
chmod +x /etc/init.d/mongodb
2）接着试一下是否可以启动、停止：
service mongodb start
service mongodb stop
3)最后设为开机启动：
chkconfig mongodb on
这个脚本，有样例如下：
```
#!/bin/bash
# mongod - Startup script for mongod
# chkconfig: 35 80 15
# description: Mongo is a scalable, document-oriented database.
# processname: mongod
#config: /db/conf/mongodb/mongod.conf
# pidfile: /var/run/mongo/mongo.pid
source /etc/rc.d/init.d/functions
# things from mongod.conf get there by mongod reading it
if [ $(id -u) != "0" ]; then
    echo "Permission Denied! Please use root to run again!"
    exit 1
fi
test -d /var/run/mongodb || (mkdir -p /var/run/mongodb ; chown mongod:mongod /var/run/mongodb)
# NOTE: if you change any OPTIONS here, you get what you pay for:
# this script assumes all options are in the config file.
CONFIGFILE="/db/conf/mongodb/mongodb.conf"
SYSCONFIG="/etc/sysconfig/mongod"
export PATH=$PATH:/db/mongodb/265/bin
DBPATH=`awk -F= '/^dbpath/{print $2}' "$CONFIGFILE"`
OPTIONS=" --config $CONFIGFILE"
mongod=${MONGOD-/db/mongodb/265/bin/mongod}
echo "db path is: "$DBPATH
echo $mongod
MONGO_USER=leftfist
MONGO_GROUP=leftfist
[ -r "$SYSCONFIG" ] && source "$SYSCONFIG"
super() {
    su - $MONGO_USER -c "PATH=$PATH:/db/mongodb/265/bin; $*"
}
start()
{
  echo -n $"Starting mongod: "
#  daemon --user "$MONGO_USER" "numactl --interleave=all" $mongod $OPTIONS
#daemon --user "$MONGO_USER" $mongod $OPTIONS
#  
#   su - $MONGO_USER -c "$mongod $OPTIONS" -m -p
#  su - $MONGO_USER
  $mongod $OPTIONS
#  super $mongod $OPTIONS
  echo $mongod$OPTIONS
  RETVAL=$?
  echo
  [ $RETVAL -eq 0 ] && touch /var/lock/subsys/mongod
}
stop()
{
  echo -n $"Stopping mongod: "
  killproc -p "$DBPATH"/mongod.lock -d 300 /db/mongodb/265/bin/mongod
  RETVAL=$?
  echo
  [ $RETVAL -eq 0 ] && rm -f /var/lock/subsys/mongod
}
restart () {
        stop
        start
}
ulimit -n 12000
RETVAL=0
case "$1" in
  start)
    start
    ;;
  stop)
    stop
    ;;
  restart|reload|force-reload)
    restart
    ;;
  condrestart)
    [ -f /var/lock/subsys/mongod ] && restart || :
    ;;
  status)
    status $mongod
    RETVAL=$?
    ;;
  *)
    echo "Usage: $0 {start|stop|status|restart|reload|force-reload|condrestart}"
    RETVAL=1
esac
exit $RETVAL
```
这种脚本，甚是难懂。据说是Shell。。。。并且它是怎么来的？不可能是由我们自己一句一句地码出来的吧？听说是安装应用系统的时候，由系统自己设置的。
但我的mongodb，是从官网上下载的二进制文件，解压以后就可以使用了，仅为了方便，配置了一下PATH而已，根本没有什么启动脚本。上面那个示例，是从其他地方抄过来，然后修改了一下，有效。
按照自己的理解，解释一下里面一些句子：
#配置文件
CONFIGFILE="/db/conf/mongodb/mongodb.conf"
#此为启动mongodb的参数（配置文件）。$CONFIGFILE的意思，应该就是将上面那一句代入其中
OPTIONS=" --config $CONFIGFILE"
#定义了mongod，但{}里的MONGOD就不知道是什么意思了，可能就是mongod命令？但为什么是大写？也许是环境变量？如果真是，那么中间那一横就是或者的意思了
#即在有PATH的条件下，直接是mongod命令，否则就是明确指示具体路径的mongod命令
mongod=${MONGOD-/db/mongodb/265/bin/mongod}
#账户，但在我的例子中，它们不起作用
MONGO_USER=leftfist
MONGO_GROUP=leftfist
#因为我是这样子启动mongoDB的：
#并没有指定由什么账户来运行。其实，启动脚本都由超级管理员来启动(?)
  $mongod $OPTIONS
