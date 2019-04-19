# MyCAT 数据分片入门实战 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [fullstackyang](http://www.jobbole.com/members/2653yy) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
近日尝试了关于Mycat分表分库的特性，这里做一些整理，作为入门的参考。
本文不对Mycat配置的每一项都进行详细解释，这些内容在其官方的权威指南中都有介绍，这里阐述部署mycat的实际操作过程，也算是对文档没有交代部分的一个补充吧。
## 一、方案规划
部署模型如图
![mycat-model](http://www.fullstackyang.com/wp-content/uploads/2016/12/mycat-model.png)
其中，有两台虚拟机：192.168.1.21和192.168.1.22，前者用于部署Mycat-server服务和1个mysql实例，后者部署2个mysql实例，这里为了简化部署模型，未考虑mysql的主从复制，3个实例均独立。
现在假设系统的数据库为messagedb，里面只有2张表，一张表为消息表：message，一张表示消息来源的字典表：source，本案实现的是按自然月分片的规则，因此上述3个mysql实例各自需要创建4个数据库，即：
|192.168.1.21:3307|message201601|192.168.1.22:3307|message201605|192.168.1.22:3308|message201609|
|----|----|----|----|----|----|
|message201602|message201606|message201610| | | |
|message201603|message201607|message201611| | | |
|message201604|message201608|message201612| | | |
> 
说明：如果是刚接触Mycat的小伙伴对分片不太理解，简单地说，对于Mycat，一个分片表示某一个MySQL实例上的某一个数据库，即schema@host，于是当我们原先的一张大表需要分片的时候，mycat就会按照我们设定的规则，把这张大表中的数据分散到各个分片上，即所谓的分表分库，因此我们需要在每个对应的分片上创建相同名称的数据库，相同结构的表。
##  二、环境准备
首先部署mysql实例，假设mysql的安装包已经下载完成，放置在/application/tools目录下，部署的工作用脚本实现：


```
#!/bin/sh
[ -f /etc/init.d/functions ] && . /etc/init.d/functions
port=$1
tarDir="/application/tools"
tarname="mysql-5.7.17-linux-glibc2.5-x86_64"
BaseDir="/application/mysql"
DataDir="/data/mysql/$port/"
# 检查安装的依赖包，并创建mysql用户 
function checkEnv(){
        res=`rpm -qa|grep libaio|wc -l`
        if [ $res -eq 0 ];then
                yum install -y libaio > /dev/null 2>&1
                res=`rpm -qa | grep libaio | wc -l`
                [ $res -ne 1 ] && echo "libaio package install failed..."&& exit 3
        fi
        if [ ! -L $BaseDir ];then
                if [ -f $tarDir/$tarname.tar.gz ];then
                        cd $tarDir && tar -zxf $tarname.tar.gz && mv $tarname ../
                        cd .. && ln -s $tarname $BaseDir
                else
                        echo "$tarname.tar is not found..."
                        exit 10
                fi
        fi
        [ `id mysql|wc -l` -eq 0 ] && /usr/sbin/useradd -s /sbin/nologin -M mysql
}
# 接收端口号作为参数，并使用mysql命令初始化数据库
function install(){
        [ -d $DataDir/data ] && rm -rf $DataDir/data/* || mkdir -p $DataDir/data
        $BaseDir/bin/mysqld \
        --initialize \
        --user=mysql \
        --basedir=$BaseDir \
        --datadir=$DataDir/data \
        --character-set-server=utf8 \
        --collation-server=utf8_general_ci \
        --initialize-insecure >/dev/null 2>&1
}
# 修改配置文件中的端口，修改配置文件权限等
function config(){
        cp $tarDir/my.cnf $DataDir && sed -i "s#3306#$port#g" $DataDir/my.cnf
        cp $tarDir/mysql.sh $DataDir && sed -i "s#3306#$port#g" $DataDir/mysql.sh
        sed -i 's#/usr/local/mysql#/application/mysql#g' $BaseDir/bin/mysqld_safe
        chmod 500 $DataDir/my.cnf
        chmod 400 $DataDir/mysql.sh #mysql.sh文件是用于启动和停止mysql服务的脚本，见下文
        touch $DataDir/mysql-$port.err
        chown -R mysql.mysql $DataDir
        chown -R mysql.mysql $BaseDir
        echo -e "mysql install success..."
}
# 由于mysql5.7默认给root用户分配随机密码，因此这里使用mysqld_safe启动mysql，并手动修改密码
function setPasswd(){
        $BaseDir/bin/mysqld_safe --defaults-file=$DataDir/my.cnf >/dev/null 2>&1 &
        while [ `netstat -natp|grep ":${port}"|wc -l` -eq 0 ]
        do
                sleep 1
        done
        echo "set password for user root ..."
        $BaseDir/bin/mysqladmin -uroot -S $DataDir/mysql.sock password 'root123' >/dev/null 2>&1
}
# 判断该端口是否被使用
if [[ $1 =~ ^[0-9]+$  ]];then
        temp=`netstat -natp| grep "mysqld"| grep "LISTEN" | awk '{print $4}'| cut -b 4- `
        if [[ -z $temp ]];then
                echo "initialize envionment..."
                checkEnv
        else
                for myport in $temp
                do
                        if [ $myport -eq "$1" ];then
                                echo -e "$1 instance has already existed..."
                                exit 1
                        fi
                done
        fi
      
        echo "Starting install $tarname..."
        install
        echo "configuration $1 instance..."
        config
        echo "mysql $port is booting..."
        setPasswd
        echo "done!!!"   
else
        echo "usage: $0 port[integer]";
fi
```
在脚本中，我们为每个mysql实例配置了一个管理脚本：mysql.sh，代码如下：


```
#!/bin/sh
[ -f /etc/init.d/functions ] && source /etc/init.d/functions
port=3306
MYUSER=root
MYPASSWD=root123
CmdPath="/application/mysql/bin"
DataPath="/data/mysql/$port"
MYADMIN="mysqladmin -u$MYUSER -p$MYPASSWD -S $DataPath/mysql.sock"
result() {
    RETVAL=$?
    if [ $RETVAL -eq 0 ];then
        action "$1 mysql" /bin/true
    else
        action "$1 mysql" /bin/false
    fi
}
start() {
        if [ ! -e $DataPath/mysql.sock ];then
                $CmdPath/mysqld_safe --defaults-file=/data/mysql/$port/my.cnf &>/dev/null &
                sleep 2
                result start
        else
                echo "MySQL $port is running..."
        fi
}
stop() {
        if [ ! -e $DataPath/mysql.sock ];then
                echo "MySQl $port is stopped..."
        else
                $CmdPath/$MYADMIN shutdown >/dev/null 2>&1
                result stop
        fi
}
case "$1" in
    start)
        start
        ;;
    stop)
        stop
        ;;
    restart)
        stop
        sleep 5
        start
        ;;
    *)
        echo "usage: $0 start|stop|restart"
        exit $RETVAL
esac
```
原理很简单，即使用mysqld_safe启动服务，使用mysqladmin停止服务。
将所有实例都启动起来之后，就可以开始创建数据库了，因为都是相同的数据库名和表结构，所以依然用一段脚本来完成工作：

Shell
```
db_host=( "192.168.1.21" "192.168.1.22" "192.168.1.22" )
db_port=( "3307" "3307" "3308" )
mysql_user=root
mysql_passwd=root123
mysql_cmd="/application/mysql/bin/mysql -u$mysql_user -p$mysql_passwd -e"
for((i=0;i<${#db_host[@]};i++))
do
        db="-h${db_host[$i]} -P${db_port[$i]}";
        for (( j=1;j<=4;j++ ))
        do
                n=$(( $i*4+$j ))
                [ $n -lt 10 ] && db_name="message20160"$n || db_name="message2016"$n
                $mysql_cmd "drop database if exists $db_name" $db >/dev/null 2>&1
                echo "creating database "$db_name"..."
                $mysql_cmd "create database $db_name;" $db >/dev/null 2>&1
 
                $mysql_cmd "use $db_name;source /data/tb.sql;" $db 2>/dev/null
        done
 
        $mysql_cmd "show databases;" $db 2> /dev/null
done
```
其中tb.sql的内容如下：

Transact-SQL
```
create table source (
        id int(11) not null auto_increment primary key comment 'pk',
        name varchar(10) default '' comment 'source name'
);
create table message (
        id int(11) not null auto_increment primary key comment 'pk',
        content varchar(255) default '' comment 'message content',
        create_time date default null,
        source_id int(11) not null,
        foreign key(source_id) references source(id)
);
insert into `source`(`id`,`name`) values(1,'weibo');
insert into `source`(`id`,`name`) values(2,'weixin');
insert into `source`(`id`,`name`) values(3,'qq');
insert into `source`(`id`,`name`) values(4,'email');
insert into `source`(`id`,`name`) values(5,'sms');
```
在message表中，总共有4个字段：
- id：主键
- content：消息的内容
- create_time：创建时间，这也是mycat进行分片时的参考字段
- source_id：source表的外键
另外，我们在source表插入了5条记录，用于测试。到这里，后端数据库的环境就搭建完成了。
## 三、安装和配置Mycat
安装Mycat的过程比较简单，在这个地址就可以下载安装包：https://github.com/MyCATApache/Mycat-download/tree/master/1.6-RELEASE。下载完之后，就进行解压，创建软链接，创建相应的用户，修改目录权限等套路操作，这里就不细说了。
安装完之后，简单地看一下mycat目录结构：

Shell
```
# tree mycat -L 1
mycat
|-- bin
|-- catlet
|-- conf
|-- lib
|-- logs
`-- version.txt
```

我们可以通过bin目录下的mycat可执行文件启动服务：bin/mycat start。启动之后可以看一下，端口打开的情况：


Shell
```
# netstat -tunlp
Active Internet connections (only servers)
Proto Recv-Q Send-Q Local Address           Foreign Address         State       PID/Program name    
tcp        0      0 0.0.0.0:22              0.0.0.0:*               LISTEN      1146/sshd           
tcp        0      0 127.0.0.1:25            0.0.0.0:*               LISTEN      2249/master         
tcp        0      0 127.0.0.1:32000         0.0.0.0:*               LISTEN      12842/java          
tcp6       0      0 :::9066                 :::*                    LISTEN      12842/java          
tcp6       0      0 :::42346                :::*                    LISTEN      12842/java          
tcp6       0      0 :::3307                 :::*                    LISTEN      3323/mysqld         
tcp6       0      0 :::22                   :::*                    LISTEN      1146/sshd           
tcp6       0      0 ::1:25                  :::*                    LISTEN      2249/master         
tcp6       0      0 :::34141                :::*                    LISTEN      12842/java          
tcp6       0      0 :::1984                 :::*                    LISTEN      12842/java          
tcp6       0      0 :::8066                 :::*                    LISTEN      12842/java
```
其中，9066端口是管理端口，提供查看当前系统节点的情况，报告心跳状态等相关系统监控的功能，8066是数据端口，相当于数据库的访问端口。我们可以使用mysql命令访问这里两个端口:


Shell
```
mysql -h[mycat_host] -u[mycat_user] -p[mycat_passwd] -P [8066|9066]
```
那么mycat_user和mycat_passwd是如何配置呢，下面就需要介绍mycat中最主要的3个配置文件：server.xml，schema.xml和rule.xml。
### 3.1 server.xml
该配置文件是用于配置mycat的系统信息，主要有两个标签：system和user。这里的user就是上述访问mycat服务的用户，不是后端数据库的用户。如果我们使用默认的配置，server.xml大概是这样的：


XHTML
```
<?xml version="1.0" encoding="UTF-8"?>
<!DOCTYPE mycat:server SYSTEM "server.dtd">
<mycat:server xmlns:mycat="http://io.mycat/">
        <system>
                <property name="useSqlStat">0</property>  
                <property name="useGlobleTableCheck">0</property> 
                <property name="sequnceHandlerType">2</property>
                <property name="processorBufferPoolType">0</property>
                <property name="useOffHeapForMerge">1</property>
                <property name="memoryPageSize">1m</property>
                <property name="spillsFileBufferSize">1k</property>
                <property name="useStreamOutput">0</property>
                <property name="systemReserveMemorySize">384m</property>
        </system>
 
        <user name="admin">
                <property name="password">admin123</property>
                <property name="schemas">messagedb</property>
        </user>
</mycat:server>
```
user标签下schemas属性表示该用户可以访问的数据库，可以定义多个数据库，用英文逗号隔开。schemas定义的数据库，一定要配置在后面的schema.xml文件对应的逻辑库，否则会提示无法访问。
system标签暂时使用默认的配置，不做过多的讨论。总之，先让你的猫先跑起来再考虑其他玩法。
### 3.2 schema.xml
schema配置文件比较复杂，也是最关键的一个配置文件，定义了mycat中的逻辑库、逻辑表，和分片的相关信息。配置如下：


XHTML
```
<?xml version="1.0"?>
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">
<mycat:schema xmlns:mycat="http://io.mycat/">
 
        <schema name="messagedb" checkSQLschema="false" sqlMaxLimit="100">
                <table name="message" dataNode="dn1,dn2,dn3,dn4,dn5,dn6,dn7,dn8,dn9,dn10,dn11,dn12" rule="sharding-by-month" />
                <!-- global table is auto cloned to all defined data nodes ,so can join
                        with any table whose sharding node is in the same data node -->
                <table name="source" primaryKey="id" type="global" dataNode="dn1,dn2,dn3,dn4,dn5,dn6,dn7,dn8,dn9,dn10,dn11,dn12" />
        </schema>
        <dataNode name="dn1" dataHost="mysql-01" database="message201601" />
        <dataNode name="dn2" dataHost="mysql-01" database="message201602" />
        <dataNode name="dn3" dataHost="mysql-01" database="message201603" />
        <dataNode name="dn4" dataHost="mysql-01" database="message201604" />
        <dataNode name="dn5" dataHost="mysql-02" database="message201605" />
        <dataNode name="dn6" dataHost="mysql-02" database="message201606" />
        <dataNode name="dn7" dataHost="mysql-02" database="message201607" />
        <dataNode name="dn8" dataHost="mysql-02" database="message201608" />
        <dataNode name="dn9" dataHost="mysql-03" database="message201609" />
        <dataNode name="dn10" dataHost="mysql-03" database="message201610" />
        <dataNode name="dn11" dataHost="mysql-03" database="message201611" />
        <dataNode name="dn12" dataHost="mysql-03" database="message201612" />
 
        <dataHost name="mysql-01" maxCon="1000" minCon="10" balance="0"
                          writeType="0" dbType="mysql" dbDriver="native" switchType="-1">
                <heartbeat>select user()</heartbeat>
                <writeHost host="hostM1" url="192.168.1.21:3307" user="root"
                                   password="root123">
                </writeHost>
        </dataHost>
 
        <dataHost name="mysql-02" maxCon="1000" minCon="10" balance="0"
                          writeType="0" dbType="mysql" dbDriver="native" switchType="-1">
                <heartbeat>select user()</heartbeat>
                <writeHost host="hostM2" url="192.168.1.22:3307" user="root"
                                   password="root123">
                </writeHost>
        </dataHost>
 
        <dataHost name="mysql-03" maxCon="1000" minCon="10" balance="0"
                          writeType="0" dbType="mysql" dbDriver="native" switchType="-1">
                <heartbeat>select user()</heartbeat>
                <writeHost host="hostM3" url="192.168.1.22:3308" user="root"
                                   password="root123">
                </writeHost>
        </dataHost>
</mycat:schema>
```
几点要说明一下：
- schema标签定义逻辑库，其下table子标签定义逻辑表，datanode属性定义该逻辑表需要分布到哪几个分片上，rule属性表示使用何种分片规则，这里我们选择sharding-by-month，这个规则的名称是自定义的，只要和后面的rule.xml对应起来即可
- source表是一张全局表，这里需要使用type=”global”来定义，这样mycat就可以帮我们在指定的分片上克隆相同的数据，这对join查询是非常有好处的。
- datanode标签定义了分片，datahost是主机名，对应dataHost标签的name属性值，database定义该主机数据库实例上的具体数据库名。
- dataHost标签定义数据库实例，其下heartbeart标签表示心跳检测所使用的方法，writeHost标签定义写数据的实例，另外还有readHost标签可以定义读数据的实例，这里不考虑读写分离，仅使用写实例，因此需要把balance属性设置为0
- 其他属性可以自行查阅官方权威指南
- 最后，出于规范和安全考虑，最好不使用数据库的root用户，而是另外再创建一个用于mycat访问的用户。
### 3.3 rule.xml
rule.xml中定义了很多分片的规则，具体规则的算法可以参考官方权威指南，这里我们直接使用默认的就可以了，其中按自然月的分片规则配置如下：

XHTML
```
<mycat:rule xmlns:mycat="http://io.mycat/">
...
        <tableRule name="sharding-by-month">
                <rule>
                        <columns>create_time</columns>
                        <algorithm>partbymonth</algorithm>
                </rule>
        </tableRule>
...
        <function name="partbymonth"
                class="io.mycat.route.function.PartitionByMonth">
                <property name="dateFormat">yyyy-MM-dd</property>
                <property name="sBeginDate">2016-01-01</property>
        </function>
...
</mycat:rule>
```
- tableRule标签定义分片规则的，其下columns标签表示对数据库表中的哪个字段应用规则，algorithm指定实现算法的名称，对应的是function标签中的name属性值
- function标签定义对应的实现类，以及参数，包括dateFormat（日期格式）和sBeginDate（起始日期）
说明：起始日期是用来计算数据所在的分片位置，例如2016年1月的message就会找到第1个分片，即dn1，2016年12月的message就会找到第12个分片，即dn12，但是如果出现了2017年1月的message，mycat就会去找第13个分片，但是配置文件中又没有对应的配置，那么就会抛出无法找到分片的错误。
最后再来总结一下配置文件的关系
![mycat-config](http://www.fullstackyang.com/wp-content/uploads/2016/12/mycat-config.png)
如图所示，server.xml定义了访问mycat服务的用户，以及该用户授权的数据库（逻辑库），schema.xml定义了具体的逻辑库，逻辑表，以及分片和数据库实例的信息，rule.xml分片规则和实现类。
## 四、测试
到这里已经完成了mycat的配置文件，但先不急着往里面灌数据，我们先访问管理端口9066，看一下运行情况：

Shell
```
mysql -h127.0.0.1 -uadmin -p -P 9066          
Enter password: 
Welcome to the MySQL monitor.  Commands end with ; or \g.
Your MySQL connection id is 12
Server version: 5.6.29-mycat-1.6-RELEASE-20161028204710 MyCat Server (monitor)
 
Copyright (c) 2000, 2016, Oracle and/or its affiliates. All rights reserved.
 
Oracle is a registered trademark of Oracle Corporation and/or its
affiliates. Other names may be trademarks of their respective
owners.
 
Type 'help;' or '\h' for help. Type '\c' to clear the current input statement.
 
mysql> show @@datanode;
+------+------------------------+-------+-------+--------+------+------+---------+------------+----------+---------+---------------+
| NAME | DATHOST                | INDEX | TYPE  | ACTIVE | IDLE | SIZE | EXECUTE | TOTAL_TIME | MAX_TIME | MAX_SQL | RECOVERY_TIME |
+------+------------------------+-------+-------+--------+------+------+---------+------------+----------+---------+---------------+
| dn1  | mysql-01/message201601 |     0 | mysql |      0 |    9 | 1000 |   12282 |          0 |        0 |       0 |            -1 |
| dn10 | mysql-03/message201610 |     0 | mysql |      0 |    0 | 1000 |    2639 |          0 |        0 |       0 |            -1 |
| dn11 | mysql-03/message201611 |     0 | mysql |      0 |    1 | 1000 |    2640 |          0 |        0 |       0 |            -1 |
| dn12 | mysql-03/message201612 |     0 | mysql |      0 |    9 | 1000 |   12283 |          0 |        0 |       0 |            -1 |
| dn2  | mysql-01/message201602 |     0 | mysql |      0 |    0 | 1000 |    2638 |          0 |        0 |       0 |            -1 |
| dn3  | mysql-01/message201603 |     0 | mysql |      0 |    0 | 1000 |    2642 |          0 |        0 |       0 |            -1 |
| dn4  | mysql-01/message201604 |     0 | mysql |      0 |    0 | 1000 |    2638 |          0 |        0 |       0 |            -1 |
| dn5  | mysql-02/message201605 |     0 | mysql |      0 |    9 | 1000 |   12289 |          0 |        0 |       0 |            -1 |
| dn6  | mysql-02/message201606 |     0 | mysql |      0 |    0 | 1000 |    2640 |          0 |        0 |       0 |            -1 |
| dn7  | mysql-02/message201607 |     0 | mysql |      0 |    1 | 1000 |    2645 |          0 |        0 |       0 |            -1 |
| dn8  | mysql-02/message201608 |     0 | mysql |      0 |    0 | 1000 |    2638 |          0 |        0 |       0 |            -1 |
| dn9  | mysql-03/message201609 |     0 | mysql |      0 |    0 | 1000 |    2638 |          0 |        0 |       0 |            -1 |
+------+------------------------+-------+-------+--------+------+------+---------+------------+----------+---------+---------------+
12 rows in set (2.17 sec)
 
mysql> show @@heartbeat;
+--------+-------+--------------+------+---------+-------+--------+---------+--------------+---------------------+-------+
| NAME   | TYPE  | HOST         | PORT | RS_CODE | RETRY | STATUS | TIMEOUT | EXECUTE_TIME | LAST_ACTIVE_TIME    | STOP  |
+--------+-------+--------------+------+---------+-------+--------+---------+--------------+---------------------+-------+
| hostM2 | mysql | 192.168.1.22 | 3307 |       1 |     0 | idle   |       0 | 2,2,1        | 2016-12-21 20:03:02 | false |
| hostM1 | mysql | 192.168.1.21 | 3307 |       1 |     0 | idle   |       0 | 0,0,0        | 2016-12-21 20:03:02 | false |
| hostM3 | mysql | 192.168.1.22 | 3308 |       1 |     0 | idle   |       0 | 0,1,1        | 2016-12-21 20:03:02 | false |
+--------+-------+--------------+------+---------+-------+--------+---------+--------------+---------------------+-------+
3 rows in set (0.21 sec)
```
如果看到各个节点都已经出现，并且心跳状态RS_CODE=1，则表示后端数据库连接正常。
现在我们用JDBC的方式批量插入1000万数据：


Java
```
public class TestMyCat {
	private static final String driver = "com.mysql.jdbc.Driver";
	private static final String url = "jdbc:mysql://192.168.1.21:8066/message?useServerPrepStmts=false&rewriteBatchedStatements=true";
	private static final String username = "admin";
	private static final String password = "admin123";
 
	@Test
	public void test() throws SQLException {
		Calendar calendar = Calendar.getInstance();
		Random random = new Random();
		calendar.set(2016, 0, 1, 0, 0, 0);
	
		Connection connection = null;
		PreparedStatement ps = null;
		try {
			Class.forName(driver);
			connection = (Connection) DriverManager.getConnection(url, username, password);
			connection.setAutoCommit(false);
			String sql = "insert into message(`content`, `create_time`, `source_id`) values(?,?,?)";
			ps = connection.prepareStatement(sql);
			long start = System.currentTimeMillis();
			for (int i = 0; i < 10000000; i++) {
				ps.setString(1, System.currentTimeMillis() + "");
				long randomtime = calendar.getTimeInMillis() + (random.nextInt(365) + 1) * 86400 * 1000l;
				Date date = new Date(randomtime);
				int source_id = random.nextInt(5) + 1;
				ps.setDate(2, date);
				ps.setInt(3, source_id);
				ps.addBatch();
				if (i != 0 && i % 10000 == 0) {
					System.out.println("execute batch : " + i);
					ps.executeBatch();
				}
			}
			ps.executeBatch();
			connection.commit();
			System.out.println(System.currentTimeMillis() - start);
		} catch (SQLException | ClassNotFoundException e) {
			e.printStackTrace();
		} finally {
			if (ps != null)
				ps.close();
			if (connection != null)
				connection.close();
		}
	}
 
}
```
如果运行的时候报错： Multi-statement transaction required more than ‘max_binlog_cache_size’ bytes of storage;，可以适当调大一下my.cnf下的max_binlog_cache_size参数。
最后我们来检验一下分片的结果，其中message表中的数据根据create_time的值按月进行了分片，而source表作为全局表，则其数据出现在了每个分片上，下面贴出部分结果：


Shell
```
mysql -h192.168.1.21 -uroot -proot123 -P3307 -e "select min(create_time),max(create_time) from message201602.message;": 
+------------------+------------------+
| min(create_time) | max(create_time) |
+------------------+------------------+
| 2016-02-01       | 2016-02-29       |
+------------------+------------------+
 
mysql -h192.168.1.22 -uroot -proot123 -P3307 -e "select min(create_time),max(create_time) from message201605.message;": 
+------------------+------------------+
| min(create_time) | max(create_time) |
+------------------+------------------+
| 2016-05-01       | 2016-05-31       |
+------------------+------------------+
 
mysql -h192.168.1.22 -uroot -proot123 -P3308 -e "select min(create_time),max(create_time) from message201609.message;": 
+------------------+------------------+
| min(create_time) | max(create_time) |
+------------------+------------------+
| 2016-09-01       | 2016-09-30       |
+------------------+------------------+
```

Shell
```
mysql -h192.168.1.21 -uroot -proot123 -P3307 -e "select * from message201601.source"
+----+--------+
| id | name   |
+----+--------+
|  1 | weibo  |
|  2 | weixin |
|  3 | qq     |
|  4 | email  |
|  5 | sms    |
+----+--------+
 
mysql -h192.168.1.22 -uroot -proot123 -P3307 -e "select * from message201607.source"
+----+--------+
| id | name   |
+----+--------+
|  1 | weibo  |
|  2 | weixin |
|  3 | qq     |
|  4 | email  |
|  5 | sms    |
+----+--------+
 
mysql -h192.168.1.22 -uroot -proot123 -P3308 -e "select * from message201611.source"
+----+--------+
| id | name   |
+----+--------+
|  1 | weibo  |
|  2 | weixin |
|  3 | qq     |
|  4 | email  |
|  5 | sms    |
+----+--------+
```
## 五、总结
本文就mycat分片的特性进行一次实战操作，完成了部署mycat-server以及后端mysql数据库，并以按自然月为分片规则进行了相关的配置，最后做了一个小的测试来验证分片功能的正确性。
mycat还有其他比较强大的特性还有待进一步的研究使用，下一步的工作：
- 完成读写分离的配置和测试
- 整合zookeerp实现高可用集群
## 六、参考资料
- 周继峰 冯钻优 陈胜尊 左越宗. 分布式数据库架构及企业实践：基于Mycat中间件[M]. 电子工业出版社, 2016.
- mycat开源项目组. Mycat权威指南. http://mycat.io/document/Mycat_V1.6.0.pdf
