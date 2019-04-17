# InfluxDB时序数据库 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2018年11月09日 17:59:39[boonya](https://me.csdn.net/boonya)阅读数：399








### **前言**

在我们很多应用中会遇到有一种基于一系列时间的数据需要处理，通过时间的顺序可以将这些数据点连成线，再通过数据统计后可以做成多纬度的报表，也可通过机器学习来实现数据的预测告警。而时序数据库就是用于存放管理这种有着时间顺序数据的，时序数据库一般都支持时序数据的快速写入、持久化、多纬度的聚合查询等基本功能。

### **InfluxDB简介**

InfluxDB是一个基于时间序列数据而开发的高性能数据存储平台，它可以对时序数据进行高吞吐量的摄取、压缩和实时查询。InfluxDB是用Go语言编写的，它会编译成一个没有外部依赖的二进制文件来运行，支持Java、JavaScript、c#等语言。InfluxDB支持类似SQL的查询语言，同时还支持正则表达式、算术表达式和时间序列特定函数以加速数据的处理效率。如下是跟InfluxDB**相关的网址：**

InfluxDB官网：[https://www.influxdata.com/](https://www.influxdata.com/)

InfluxDB官方文档：[https://docs.influxdata.com/influxdb/](https://docs.influxdata.com/influxdb/)

InfluxDB官方下载：[https://portal.influxdata.com/downloads](https://portal.influxdata.com/downloads)

InfluxDB客户端工具下载：[https://docs.influxdata.com/influxdb/v1.6/tools/api_client_libraries/](https://docs.influxdata.com/influxdb/v1.6/tools/api_client_libraries/)

**特点：**
- 无结构(无模式)：可以是任意数量的列
- 可以设置metric的保存时间
- 支持与时间有关的相关函数(如min、max、sum、count、mean、median等)，方便统计
- 支持存储策略:可以用于数据的删改。(influxDB没有提供数据的删除与修改方法)
- 支持连续查询:是数据库中自动定时启动的一组语句，和存储策略搭配可以降低InfluxDB的系统占用量。
- 原生的HTTP支持，内置HTTP API
- 支持类似sql语法
- 支持设置数据在集群中的副本数
- 支持定期采样数据，写入另外的measurement，方便分粒度存储数据。
- 自带web管理界面，方便使用(登入方式：http://:8083)

### InfluxDB操作

这里将会简单的介绍下如何操作InfluxDB，通过这些操作基本也能满足工作上的需要了。操作InfluxDB可以通过命令行工具，也可借助开源的客户端工具，我这里使用的是一款名叫“InfluxDBStudio”基于C#编写的开源工具。常用操作的代码如下：

```
#显示用户
show users
 
#创建用户
create user "username" with password 'password'
 
#创建管理员权限用户
create user "username" with password 'password' with all privileges
 
#删除用户
drop user "username"
 
#创建数据库
create database "db_name"
 
#显示所有的数据库
show databases
 
#删除数据库
drop database "db_name"
 
#使用数据库
use db_name
 
#显示该数据库中所有的表
show measurements
 
#创建表，直接在插入数据的时候指定表名，其中test为表名
insert test,host=127.0.0.1,monitor_name=test count=1
 
#删除表
drop measurement "measurement_name"
 
#查询数据
select * from test order by time desc
 
#查看当前数据库的数据保存策略（Retention Policies）
show retention policies on "db_name"
 
#创建新的数据保存策略
#rp_name：策略名
#db_name：具体的数据库名；
#3w：保存3周，3周之前的数据将被删除，influxdb具有各种事件参数，比如：h（小时），d（天），w（星期）
#replication 1：副本个数，一般为1就可以了
#default：设置为默认策略
create retention policy "rp_name" on "db_name" duration 3w replication 1 default
 
#修改数据保存策略
alter retention policy "rp_name" on "db_name" duration 30d default
 
#删除数据保存策略
drop retention policy "rp_name"
 
#查看数据库的连续查询（Continous Queries）
show continuous queries
 
#创建新的连续查询（Continous Queries）
#cq_name：连续查询名字
#db_name：数据库名字
#sum(count)：计算总和
#table_name：当前表名
#new_table_name：存新的数据的表名
#30m：时间间隔为30分钟
create continous query cq_name on db_name begin select sum(count) into new_table_name from table_name group by time(30m) end
 
#删除连续查询
drop continous query cp_name on db_name
```

### 编程

**.net core**

```
using System;
using System.Collections.Generic;
using System.Linq;
using System.Threading.Tasks;
using InfluxData.Net.Common.Enums;
using InfluxData.Net.InfluxDb;
using InfluxData.Net.InfluxDb.Models;
using Microsoft.AspNetCore.Mvc;
 
namespace WebApplication1.Controllers
{
 public class InfoController : Controller
 {
 //声明InfluxDbClient
 private InfluxDbClient clientDb;
 
 public InfoController()
 {
  //连接InfluxDb的API地址、账号、密码
  var infuxUrl = "http://localhost:8086/";
  var infuxUser = "admin";
  var infuxPwd = "admin";
 
  //创建InfluxDbClient实例
  clientDb = new InfluxDbClient(infuxUrl, infuxUser, infuxPwd, InfluxDbVersion.Latest);
 }
 
 
 /// <summary>
 /// 从InfluxDB中读取数据
 /// </summary>
 public async void GetData()
 {
  //传入查询命令，支持多条
  var queries = new[]
  {
  " SELECT * FROM Reading WHERE time> now() - 24h "
  };
  var dbName = "code-hub";
 
  //从指定库中查询数据
  var response = await clientDb.Client.QueryAsync(queries, dbName);
  //得到Serie集合对象（返回执行多个查询的结果）
  var series = response.ToList();
  //取出第一条命令的查询结果，是一个集合
  var list = series[0].Values;
  //从集合中取出第一条数据
  var info_model = list.FirstOrDefault();
 }
 
 
 
 /// <summary>
 /// 往InfluxDB中写入数据
 /// </summary>
 public async void AddData()
 {
  //基于InfluxData.Net.InfluxDb.Models.Point实体准备数据
  var point_model = new Point()
  {
  Name = "Reading",//表名
  Tags = new Dictionary<string, object>()
  {
   { "Id", 158}
  },
  Fields = new Dictionary<string, object>()
  {
   { "Val", "webInfo" }
  },
  Timestamp = DateTime.UtcNow
  };
  var dbName = "code-hub";
 
  //从指定库中写入数据，支持传入多个对象的集合
  var response = await clientDb.Client.WriteAsync(point_model, dbName);
 }
 }
}
```

**Java spring-boot**

pom配置

```
<!-- https://mvnrepository.com/artifact/org.influxdb/influxdb-java -->
<dependency>
    <groupId>org.influxdb</groupId>
    <artifactId>influxdb-java</artifactId>
    <version>2.14</version>
</dependency>
```

主方法

```java
package com.example.demo;


import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.scheduling.annotation.EnableScheduling;
import org.springframework.scheduling.annotation.Scheduled;

import java.util.Random;

@SpringBootApplication
@EnableScheduling
public class DemoApplication {

    public static void main(String[] args) {
        SpringApplication.run(DemoApplication.class, args);
    }


    @Scheduled(fixedRate = 1000)
    public void doInsert(){
        Random random = new Random();
        InfluxDBDemo.insert(random.nextInt(1000));
    }

}
```

工具类：

```java
package com.example.demo;

import org.influxdb.InfluxDB;
import org.influxdb.InfluxDBFactory;
import org.influxdb.dto.Point;


public class InfluxDBDemo {

    public static void insert(int num){
        InfluxDB db = InfluxDBFactory.connect("http://192.168.192.128:8086", "admin", "admin");
        db.setDatabase("my_test");  // 设置数据库
        Point.Builder builder = Point.measurement("test_measurement");  // 创建Builder，设置表名
        builder.addField("count",num);  // 添加Field
        builder.tag("TAG_CODE","TAG_VALUE_" + num);    // 添加Tag
        Point point = builder.build();
        db.write(point);
    }
}
```

### **总结**

1、InfluxDB是个专业的时序数据库，通过时序库可帮助我们更高效的处理应用中的时序数据。

2、使用InfluxDB库时需先了解该库的一些特色功能，如数据保存策略、连续查询等。

3、通过“InfluxData.Net”类库可快速简便的帮助我们在ASP.NET Core2程序中实现对InfluxDB的读写操作，Java也是支持的。

4、提供Docker版本的InfluxDB镜像。

### 参考文章

[http://www.cnblogs.com/LUA123/p/9507029.html](http://www.cnblogs.com/LUA123/p/9507029.html)

[https://www.jb51.net/article/149972.htm](https://www.jb51.net/article/149972.htm)



