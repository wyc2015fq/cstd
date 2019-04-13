
# Clojure 学习入门（7）—— 连接mysql - 阳光岛主 - CSDN博客

2013年12月22日 22:30:53[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：8929个人分类：[Script																](https://blog.csdn.net/sunboy_2050/article/category/694817)
所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



Clojure的contrib包中实现了对现有JDBC的封装，在wiki上有连接，该页面上包含了对不同的数据库的连接方法和基本的操作，但是这个页面上没有提供足够的信息，足够一个初学者能够使用Clojure来完成一个SQL操作。本文中我们将利用Clojure和Lein工具，简介一下做开发的基础。当然网上也有非常多对Clojure的SQL操作进行封装的库，我们这里没有使用。
首先，配置你的MySQL数据库，创建数据库、表、数据。
其次，在和MySQL的操作中，我们需要两个库，一个是clojure.contrib，另外一个是mysql-connector-java-xxx.jar。为了管理和配置这两个库，我们需要配置一下project.clj文件，在depedencies中加上两条：org.clojure/clojure-contrib “1.2.0” 和 mysql/mysql-connector-java “5.1.6”。这样运行 lein deps，就会自动下载并且安装到Leiningen dependencies lib目录下。
接着，在需要使用sql库的地方要导入这些库
在ns宏中加载如下内容：(:use [clojure.contrib.sql :as sql :only ()])，这样在本文件中就可以直接使用sql作为前缀。需要主意的是，虽然我们并没有直接使用到mysql-connector-java，但是如果在lib目录下没有该文件，就会报错没有mysql的driver。
配置上两条之后，就可以进行sql连接了。
首先要定义一个database specification，这是一个map，几个必须配置的key包括了：classname, subprotocol, subname, user, password。
|(|def|db {:classname|"com.mysql.jdbc.Driver"
|:subprotocol|"mysql"
|:subname|"//127.0.0.1:3306/test"
|:user|"root"
|:password|"pass"|})
|

这个db-spec在后面几乎所有的sql操作都是需要的。
接下来我们主要描述几个常用API的格式，具体的内容可以参考手册。
1，新建表
sql/create-table :table-name [:key :type “”] [:key :type “”])
|(defn create|-|users []
|(sql|/|create|-|table
|:user
|[:|id|:integer|"PRIMARY KEY"|"AUTO_INCREMENT"|]
|[:fname|"varchar(25)"|]))
|

2， 插入数据
两个api，一个是insert-rows，一个是insert-values。insert-rows需要形成一个完整的行数据插入到表中，insert-values可以按照key来插入值。
|(defn insert|-|user [fname]
|(sql|/|insert|-|values :users [:fname] [fname]))
|

使用insert-values可以利用primary-key的auto_increment的属性
|(defn insert|-|rows
|[]
|(sql|/|insert|-|rows :user [|1|"Tim"|] [|2|"Tom"|]))
|

3，丢弃表
|(defn drop|-|user
|[]
|(|try
|(sql|/|drop|-|table :fruit)
|(catch Exception _)))
|

4， with-query-results 选择查询的数据
with-query-results 类似于 let 语句，会将查询结果绑定到rs上，rs是一个map构成的sequence。
|(sql|/|with|-|query|-|results rs [|"select * from user"|]
|(dorun (|map|\#(println %) rs)))
|

5， 更新表
update-values 用来更新表的内容，使用该函数需要指定查询条件，如下图所示
|(defn update|-|user [|id|attribute|-|map|]
|(sql|/|update|-|values :user [|"id=?"|id|] attribute|-|map|))
|

6， 删除行
|(sql|/|delete|-|rows :user [|"id=?"|id|])
|

7，SQL准备
clojure支持由用户输入构造sql语句，利用的是sql prepared语句，略

8. 示例
创建表 fruit，并插入两条记录，然后条件查询和全部查询
1） 在project.clj 中，添加sql依赖：

```python
(defproject myClojure "0.1.0-SNAPSHOT"
  :description "FIXME: write description"
  :url "http://example.com/FIXME"
  :license {:name "Eclipse Public License"
            :url "http://www.eclipse.org/legal/epl-v10.html"}
  :dependencies [
                 [org.clojure/clojure "1.5.1"]
                 [org.clojure/java.jdbc "0.0.6"]
                 [org.clojure/clojure-contrib "1.2.0"]
                 [mysql/mysql-connector-java "5.1.26"]
                 ])
```

2） mysql.cli 代码

```python
; clojure connect to mysql
; ithomer.net
; 2013.12.10
(ns myClojure.mysql2
  (:require [clojure.java.jdbc :as sql]))
; 配置参数
(def mysql-db{
               :classname "com.mysql.jdbc.Driver"
               :subprotocol "mysql"
               :subname "//172.27.9.104:3306/tmptest"
               :user "root"
               :password "server2011"})
; 创建表
(defn create-fruit
  "Create a table" 
  []
   (sql/create-table
     :fruit
     [:name "varchar(32)" "PRIMARY KEY"]
     [:appearance "varchar(32)"]
     [:cost :int]
     [:grade :real]))
; 删除表
(defn drop-fruit
  "Drop a table" 
  []
  (try
    (sql/drop-table :fruit)
    (catch Exception _)))
; 连接数据库
(sql/with-connection
  mysql-db
  (drop-fruit)
  (create-fruit))
; 插入数据
(sql/with-connection mysql-db
  (sql/insert-records :fruit
                      {:name "Apple" :appearance "rosy" :cost 24 :grade 1.2}
                      {:name "Orange" :appearance "round" :cost 49}))
; 条件查询
(println (sql/with-connection mysql-db
           (sql/with-query-results rows
             ["SELECT * FROM fruit WHERE appearance = ?" "rosy"]
             (:cost (first rows)))))
; 全量查询
(println (sql/with-connection mysql-db
           (sql/with-query-results rows
             ["SELECT * FROM fruit"]
             (dorun (map #(println %) rows)))))
```
运行结果：
24
{:name Apple, :appearance rosy, :cost 24, :grade 1.2}
{:name Orange, :appearance round, :cost 49, :grade nil}
nil
创建的数据库表内容：
![](https://img-blog.csdn.net/20131210103844250)
注：所有的sql操作都需要在with-connection宏下包裹着

**参考推荐：**
[Clojure SQL 基础](http://www.cnblogs.com/mmjx/archive/2011/10/28/2228253.html)
[Clojure 连接mysql](http://blog.csdn.net/baiyangfu_love/article/details/8478026)
[Connecting Clojure and MySQL](http://corfield.org/blog/post.cfm/connecting-clojure-and-mysql)



