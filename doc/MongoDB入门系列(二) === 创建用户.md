# MongoDB入门系列(二) ===> 创建用户

2019年01月05日 13:00:11 [刘翾](https://me.csdn.net/c_kite) 阅读数：115



 版权声明：原创文章，如想转载，请注明原文网址，注明出处；否则，禁止转载；谢谢配合！	https://blog.csdn.net/c_kite/article/details/85845983

# 全系列目录

- MongoDB入门系列(一) ===> 概念解析 <https://blog.csdn.net/c_kite/article/details/85777134>
- MongoDB入门系列(二) ===> 创建用户 <https://blog.csdn.net/c_kite/article/details/85845983>
- MongoDB入门系列(三) ===> 基本操作:增删改查 <https://blog.csdn.net/c_kite/article/details/85853719>
- MongoDB入门系列(四) ===> 应用补充 <https://blog.csdn.net/c_kite/article/details/86361302>



### 文章目录

- [全系列目录](https://blog.csdn.net/c_kite/article/details/85845983#_1)

- - [1. 创建用户](https://blog.csdn.net/c_kite/article/details/85845983#1__21)

  - - [1.1 角色介绍](https://blog.csdn.net/c_kite/article/details/85845983#11__42)

  - [2. 修改密码](https://blog.csdn.net/c_kite/article/details/85845983#2__53)

  - [3. 删除用户](https://blog.csdn.net/c_kite/article/details/85845983#3__60)

  - [4. 接着文章开头](https://blog.csdn.net/c_kite/article/details/85845983#4__67)

  - [5. 登录](https://blog.csdn.net/c_kite/article/details/85845983#5__83)

本文代码测试环境: win10, MongoDB 4.0.5



mongo安装教程不出, 可以自行在网络中搜索
刚安装好的mongodb会提示:

```
2019-01-05T11:22:42.470+0800 I CONTROL  [initandlisten] ** WARNING: Access control is not enabled for the database.
2019-01-05T11:22:42.470+0800 I CONTROL  [initandlisten] **          Read and write access to data and configuration is unrestricted.
12
```

我们可以创建用户来解决这个问题.

## 1. 创建用户

由于刚安装完, 没有权限校验因此可以先创建一个超级管理员

```
use admin

db.createUser(
   {
        user:"root", // 用户名
        pwd:"root", // 密码
        roles:[ // 角色
         {role:"root",db:"admin"} // 超级管理员
        ]
   }
)

// 输入命令, 提示Successfully added user:
// 之后可以输入show users命令, 查看详情
1234567891011121314
```

### 1.1 角色介绍

```
1.数据库用户角色：read、readWrite;
2.数据库管理角色：dbAdmin、dbOwner、userAdmin；
3.集群管理角色：clusterAdmin、clusterManager、clusterMonitor、hostManager；
4.备份恢复角色：backup、restore；
5.所有数据库角色：readAnyDatabase、readWriteAnyDatabase、userAdminAnyDatabase、dbAdminAnyDatabase
6.超级用户角色：root  
123456
```

## 2. 修改密码

```
db.changeUserPassword("用户名", "密码")
1
```

## 3. 删除用户

```
db.dropUser("用户名");
1
```

## 4. 接着文章开头

创建完超级管理员之后最后再使用`dbAdmin`, 或者`dbAdminAnyDatabase`创建一个日常开发角色. 命令还是`createUser`, 只需要把roles里面的role更改一下就好.

创建完之后在MongoDB(博主的版本为4.0.5)的安装目录下找到bin文件夹, 打开里面的`mongod.cfg`.
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190105124133817.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nfa2l0ZQ==,size_16,color_FFFFFF,t_70)

增加字段`authorization: enabled`, 开启权限校验
![在这里插入图片描述](https://img-blog.csdnimg.cn/20190105124159194.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nfa2l0ZQ==,size_16,color_FFFFFF,t_70)

之后使用`net stop mongodb`, `net start mongodb` 重启一下mongo的服务

![在这里插入图片描述](https://img-blog.csdnimg.cn/20190105124345465.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2Nfa2l0ZQ==,size_16,color_FFFFFF,t_70)

再次进入的时候就会发现, 之前的warning不见了.

## 5. 登录

登录用户有两种方法

```
// 1
输入命令 mongo
输入命令 use admin(切换到刚刚创建用户的数据库, 如不切换这时的数据库默认为test)
输入命令db.auth("用户名", "密码") 返回值为1代表登录成功
1234
// 2
mongo -u "用户名" -p "密码" --authenticationDatabase 指定数据库 --port 指定端口

--authenticationDatabase 不写默认test数据库
--port 不写默认为27017, 如果想修改可以在mongod.cfg文件中修改
```