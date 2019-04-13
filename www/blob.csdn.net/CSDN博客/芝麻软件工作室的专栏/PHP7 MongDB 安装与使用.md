
# PHP7 MongDB 安装与使用 -  芝麻软件工作室的专栏 - CSDN博客


2016年07月31日 08:50:20[seven-soft](https://me.csdn.net/softn)阅读数：2016


本文教程只适合在 PHP7 的环境，如果你是 PHP5 环境，你可以参阅[PHP MongDB 安装与使用](http://www.runoob.com/mongodb/mongodb-php.html)。
## PHP7 Mongdb 扩展安装
我们使用 pecl 命令来安装：
$/usr/local/php7/bin/pecl install mongodb执行成功后，会输出以下结果：
……Buildprocess completed successfullyInstalling'/usr/local/php7/lib/php/extensions/no-debug-non-zts-20151012/mongodb.so'install ok:channel://pecl.php.net/mongodb-1.1.7configuration option"php_ini"isnotsetto php.ini locationYoushould add"extension=mongodb.so"to php.ini接下来我们打开 php.ini 文件，添加**extension=mongodb.so**配置。
可以直接执行以下命令来添加。
$ echo"extension=mongodb.so">>`/usr/local/php7/bin/php --ini | grep "Loaded Configuration" | sed -e "s|.*:\s*||"`
> 注意：
> 以上执行的命令中 php7 的安装目录为 /usr/local/php7/，如果你安装在其他目录，需要相应修改 pecl 与 php 命令的路径。

---

## Mongodb 使用
PHP7 连接 MongoDB 语法如下：
$manager=newMongoDB\Driver\Manager("mongodb://localhost:27017");
### 插入数据
将 name 为"菜鸟教程" 的数据插入到 test 数据库的 runoob 集合中。
<?php
$bulk=newMongoDB\Driver\BulkWrite;$document=['_id'=>newMongoDB\BSON\ObjectID,'name'=>'菜鸟教程'];$_id=$bulk->insert($document);var_dump($_id);$manager=newMongoDB\Driver\Manager("mongodb://localhost:27017");$writeConcern=newMongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY,1000);$result=$manager->executeBulkWrite('test.runoob',$bulk,$writeConcern);?>
### 读取数据
这里我们将三个网址数据插入到 test 数据库的 sites 集合，并读取迭代出来：
<?php
$manager=newMongoDB\Driver\Manager("mongodb://localhost:27017");// 插入数据$bulk=newMongoDB\Driver\BulkWrite;$bulk->insert(['x'=>1,'name'=>'菜鸟教程','url'=>'http://www.runoob.com']);$bulk->insert(['x'=>2,'name'=>'Google','url'=>'http://www.google.com']);$bulk->insert(['x'=>3,'name'=>'taobao','url'=>'http://www.taobao.com']);$manager->executeBulkWrite('test.sites',$bulk);$filter=['x'=>['$gt'=>1]];$options=['projection'=>['_id'=>0],'sort'=>['x'=>-1],];// 查询数据$query=newMongoDB\Driver\Query($filter,$options);$cursor=$manager->executeQuery('test.sites',$query);foreach($cursoras$document){print_r($document);}?>输出结果为：
stdClassObject([x]=>3[name]=>taobao[url]=>http://www.taobao.com)stdClassObject([x]=>2[name]=>Google[url]=>http://www.google.com)
### 更新数据
接下来我们将更新 test 数据库 sites 集合中 x 为 2 的数据：
<?php
$bulk=newMongoDB\Driver\BulkWrite;$bulk->update(['x'=>2],['$set'=>['name'=>'菜鸟工具','url'=>'tool.runoob.com']],['multi'=>false,'upsert'=>false]);$manager=newMongoDB\Driver\Manager("mongodb://localhost:27017");$writeConcern=newMongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY,1000);$result=$manager->executeBulkWrite('test.sites',$bulk,$writeConcern);?>接下来我们使用 "db.sites.find()" 命令查看数据的变化，x 为 2 的数据已经变成了菜鸟工具：
![](http://www.runoob.com/wp-content/uploads/2016/06/EFFDBCDE-1E7E-40AD-BE88-37CD57070E25.jpg)
### 删除数据
以下实例删除了 x 为 1 和 x 为 2的数据，注意 limit 参数的区别：
<?php
$bulk=newMongoDB\Driver\BulkWrite;$bulk->delete(['x'=>1],['limit'=>1]);// limit 为 1 时，删除第一条匹配数据$bulk->delete(['x'=>2],['limit'=>0]);// limit 为 0 时，删除所有匹配数据$manager=newMongoDB\Driver\Manager("mongodb://localhost:27017");$writeConcern=newMongoDB\Driver\WriteConcern(MongoDB\Driver\WriteConcern::MAJORITY,1000);$result=$manager->executeBulkWrite('test.sites',$bulk,$writeConcern);?>更多使用方法请参考：[http://php.net/manual/en/book.mongodb.php](http://php.net/manual/en/book.mongodb.php)。

