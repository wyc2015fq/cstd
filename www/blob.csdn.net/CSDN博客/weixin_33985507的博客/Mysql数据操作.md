# Mysql数据操作 - weixin_33985507的博客 - CSDN博客
2017年10月23日 20:22:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：8
# 一 介绍
MySQL数据操作： DML
========================================================
在MySQL管理软件中，可以通过SQL语句中的DML语言来实现数据的操作，包括
使用INSERT实现数据的插入
UPDATE实现数据的更新
使用DELETE实现数据的删除
使用SELECT查询数据以及。
========================================================
# 二 插入数据INSERT
```
1. 插入完整数据（顺序插入）
    语法一：
    INSERT INTO 表名(字段1,字段2,字段3…字段n) VALUES(值1,值2,值3…值n);
    语法二：
    INSERT INTO 表名 VALUES (值1,值2,值3…值n);
2. 指定字段插入数据
    语法：
    INSERT INTO 表名(字段1,字段2,字段3…) VALUES (值1,值2,值3…);
3. 插入多条记录
    语法：
    INSERT INTO 表名 VALUES
        (值1,值2,值3…值n),
        (值1,值2,值3…值n),
        (值1,值2,值3…值n);
        
4. 插入查询结果
    语法：
    INSERT INTO 表名(字段1,字段2,字段3…字段n) 
                    SELECT (字段1,字段2,字段3…字段n) FROM 表2
                    WHERE …;
```
# 三 更新数据UPDATE
```
语法：
    UPDATE 表名 SET
        字段1=值1,
        字段2=值2,
        WHERE CONDITION;
示例：
    UPDATE mysql.user SET password=password(‘123’) 
        where user=’root’ and host=’localhost’;
```
# 四 删除数据DELETE
```
语法：
    DELETE FROM 表名 
        WHERE CONITION;
示例：
    DELETE FROM mysql.user 
        WHERE password=’’;
练习：
    更新MySQL root用户密码为mysql123
    删除除从本地登录的root用户以外的所有用户
```
