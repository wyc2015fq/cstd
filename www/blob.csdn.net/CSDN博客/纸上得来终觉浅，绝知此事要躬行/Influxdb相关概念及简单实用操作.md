# Influxdb相关概念及简单实用操作 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年12月04日 10:05:04[boonya](https://me.csdn.net/boonya)阅读数：144








转自：[https://blog.csdn.net/Jailman/article/details/78427896?locationNum=6&fps=1](https://blog.csdn.net/Jailman/article/details/78427896?locationNum=6&fps=1)

新的infludb版本已经取消了页面的访问方式，只能使用客户端来查看数据

###  一、influxdb与传统数据库的比较

库、表等比较：
|influxDB|传统数据库中的概念|
|----|----|
|database|数据库|
|measurement|数据库中的表|
|points|表里面的一行数据|

influxdb数据的构成：



Point由时间戳（time）、数据（field）、标签（tags）组成。
|Point属性|传统数据库中的概念|
|----|----|
|time|每个数据记录时间，是数据库中的主索引(会自动生成)|
|fields|各种记录值（没有索引的属性）也就是记录的值：温度， 湿度|
|tags|各种有索引的属性：地区，海拔|



这里不得不提另一个名词：series：

所有在数据库中的数据，都需要通过图表来展示，而这个series表示这个表里面的数据，可以在图表上画成几条线：通过tags排列组合算出来。具体可以通过[SHOW SERIES](https://docs.influxdata.com/influxdb/v1.1/query_language/schema_exploration/#show-series) FROM "表名" 进行查询。

influxdb使用的端口如下：

```
8083: Web admin管理服务的端口, http://localhost:8083
8086: HTTP API的端口
8088: 集群端口(目前还不是很清楚, 配置在全局的bind-address，默认不配置就是开启的)
```



###  二、安装及增删改查

1、安装

```
# wget https://dl.influxdata.com/influxdb/releases/influxdb-1.1.0.x86_64.rpm
# rpm -ivh influxdb-1.1.0.x86_64.rpm
# systemctl enable influxdb
# systemctl start influxdb
```

注：本示例是在centos7/rhel7中配置的服务，如果在rhel6下，可以使用service进行服务配置和启动。

2、命令行下创建及查询

通过influx命令进行管理，可以使用如下命令创建数据库：

```
> CREATE DATABASE "testDB"
> show databases //查询当前的所有数据库
> show databases
name: databases
---------------
name
_internal
testDB
> use testDB //使用某个数据库
```

建库的操作可以发现非常类似于mysql下的操作。而在influxdb下没有细分的表的概念，influxdb下的表在插入数据库的时候自动会创建。可以通过SHOW measurements命令查看所有的表，这个类似于mysql下的show tables; 。

```
> INSERT cpu,host=serverA,region=us_west value=0.64 //在cpu表中插入相关的数据
> SELECT * FROM cpu ORDER BY time DESC LIMIT 3 //查询最近的三条数据
> SELECT * FROM /.*/ LIMIT 1 //正则表达式查询
> delete from cpu where time=1480235366557373922 //删除某条数据
> DROP MEASUREMENT "measurementName" //删除表
```

update更新语句没有，不过有alter命令，在influxdb中，删除操作用和更新基本不用到 。在针对数据保存策略方面，有一个特殊的删除方式，这个后面再提。

关于用户的操作如下：

```
# 显示用户
SHOW USERS
# 创建用户
CREATE USER "username" WITH PASSWORD 'password'
# 创建管理员权限的用户
CREATE USER "username" WITH PASSWORD 'password' WITH ALL PRIVILEGES
# 删除用户
DROP USER "username"
```

influxdb的权限设置比较简单，只有读、写、ALL几种。更多用户权限设置可以参看官方文档：[https://docs.influxdata.com/influxdb/v1.0/query_language/authentication_and_authorization/](https://docs.influxdata.com/influxdb/v1.0/query_language/authentication_and_authorization/) 。默认情况下，influxdb类似与mongodb，是不开启用户认证的，可以修改其 conf文件，配置http块内容如下：

```
[http]
enable = true
bind-address = ":8086"
auth-enabled = true # 开启认证
```

###  三、数据保存策略及连续查询

1、数据保存策略

一般情况下基于时间序列的point数据不会进行直接删除操作，一般我们平时只关心当前数据，历史数据不需要一直保存，不然会占用太多空间。这里可以配置数据保存策略（Retention Policies），当数据超过了指定的时间之后，就会被删除。

```
SHOW RETENTION POLICIES ON "testDB" //查看当前数据库的Retention Policies
CREATE RETENTION POLICY "rp_name" ON "db_name" DURATION 30d REPLICATION 1 DEFAULT //创建新的Retention Policies
#注释如下：
rp_name：策略名
db_name：具体的数据库名
30d：保存30天，30天之前的数据将被删除
它具有各种时间参数，比如：h（小时），w（星期）
REPLICATION 1：副本个数，这里填1就可以了
DEFAULT 设为默认的策略
```

也可以通过如下命令修改和删策略：

```
ALTER RETENTION POLICY "rp_name" ON db_name" DURATION 3w DEFAULT
DROP RETENTION POLICY "rp_name" ON "db_name"
```

2、连续查询（Continuous Queries）

当数据超过保存策略里指定的时间之后，就会被删除。如果我们不想完全删除掉，比如做一个数据统计采样：把原先每秒的数据，存为每小时的数据，让数据占用的空间大大减少（以降低精度为代价）。这就需要InfluxDB提供的：连续查询（Continuous Queries）。

查看当前的查询策略
`SHOW CONTINUOUS QUERIES`
创建新的Continuous Queries

```
CREATE CONTINUOUS QUERY cq_30m ON testDB BEGIN SELECT mean(temperature) INTO weather30m FROM weather GROUP BY time(30m) END
注释如下：
cq_30m：连续查询的名字
testDB：具体的数据库名
mean(temperature): 算平均温度
weather： 当前表名
weather30m： 存新数据的表名
30m：时间间隔为30分钟
```

当我们插入新数据之后，通过SHOW MEASUREMENTS查询发现。可以发现数据库中多了一张名为weather30m(里面已经存着计算好的数据了)。这一切都是通过Continuous Queries自动完成的。

删除Continuous Queries
`DROP CONTINUOUS QUERY <cq_name> ON <database_name>`


###  四、RESTful 接口操作

插入及查询操作：

```
# code from www.361way.com
now=$(date '+%FT%T.000Z')
tomorrow=$(date -v +1d '+%FT%T.000Z')
echo "creating database"
curl -G http://localhost:8086/query --data-urlencode "q=CREATE DATABASE foo"
echo "creating retention policy"
curl -G http://localhost:8086/query --data-urlencode "q=CREATE RETENTION POLICY bar ON foo DURATION INF REPLICATION 1 DEFAULT"
echo "inserting data"
curl -d "{\"database\" : \"foo\", \"retentionPolicy\" : \"bar\", \"points\": [{\"measurement\": \"cpu\", \"tags\": {\"region\":\"uswest\",\"host\": \"server01\"},\"time\": \"$now\",\"fields\": {\"value\": 100}}]}" -H "Content-Type: application/json" http://localhost:8086/write
echo "inserting data"
curl -d "{\"database\" : \"foo\", \"retentionPolicy\" : \"bar\", \"points\": [{\"measurement\": \"cpu\", \"tags\": {\"region\":\"uswest\",\"host\": \"server01\"},\"time\": \"$tomorrow\",\"fields\": {\"value\": 200}}]}" -H "Content-Type: application/json" http://localhost:8086/write
sleep 1
echo "querying data"
curl -G http://localhost:8086/query --data-urlencode "db=foo" --data-urlencode "q=SELECT count(value) FROM \"foo\".\"bar\".cpu"
echo "querying data"
curl -G http://localhost:8086/query --data-urlencode "db=foo" --data-urlencode "q=SELECT count(value) FROM \"foo\".\"bar\".cpu where time < now() + 10d"
```

GZIP压缩数据插入及查询：

```
# code from www.361way.com
echo "inserting data"
curl -v -i -H "Content-encoding: gzip" -H "Content-Type: application/json" -X POST -T foo.json.gz http://localhost:8086/write
rm foo.json.gz
echo "querying data with gzip encoding"
curl -v -G --compressed http://localhost:8086/query --data-urlencode "db=foo" --data-urlencode "q=SELECT sum(value) FROM \"foo\".\"bar\".cpu GROUP BY time(1h)"
```

还可以指定数据保存策略及表的各关键值插入：

```
echo "creating retention policy"
curl -G http://localhost:8086/query --data-urlencode "q=CREATE RETENTION POLICY bar ON foo DURATION 300d REPLICATION 3 DEFAULT"
curl -d '{"database" : "foo", "retentionPolicy" : "bar", "points": [{"measurement": "network", "tags": {"host": "server01","region":"uswest"},"time": "2015-02-26T22:01:11.703Z","fields": {"rx": 2342,"tx": 9804}}]}' -H "Content-Type: application/json" http://localhost:8086/write
curl -d '{"database" : "foo", "retentionPolicy" : "bar", "points": [{"measurement": "network", "tags": {"host": "server01","region":"useast"},"time": "2015-02-27T22:01:11.703Z","fields": {"rx": 4324,"tx": 7930}}]}' -H "Content-Type: application/json" http://localhost:8086/write
```

更多可以参考[github上的测试内容](https://github.com/influxdata/influxdb/tree/master/tests)。

无论是[influx](https://docs.influxdata.com/influxdb/v1.1/tools/shell/)还是通过RESETful 参数进行查询，其还有pertty参数可以使输出的json格式更加美观 。除此之外，influx命令还有column、CSV等格式输出，influx命令支持通过import参数输入类csv类格式的数据。

###  五、influxdb.conf配置web管理

当前版本的influxdb，默认情况下未开启web管理功能。可以通过修改influxdb.conf文件中admin项的配置开启web管理界面，具体如下：

```
[admin]
# Determines whether the admin service is enabled.
enabled = true
# The default bind address used by the admin service.
bind-address = ":8083"
```

![influxdb](http://www.361way.com/wp-content/uploads/2016/11/influxdb-webadmin.png)





