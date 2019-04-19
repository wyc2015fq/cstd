# SQL语句汇总 - HJ - CSDN博客
2018年03月13日 12:40:56[FZH_SYU](https://me.csdn.net/feizaoSYUACM)阅读数：114
**SQL语句汇总**
一、**数据库**
1. 创建数据库     
`CREATE DATABASE NAME;`
2. 查看数据库
`SHOW DATABASES;`
3. 选择数据库
`USE NAME;`
4. 删除数据库库
` DROP DATABASE NAME;`
二、**表**
1. 创建表
```
CREATE TABLE NAME (
    属性名 数据类型;
    属性名 数据类型;
    ....
    ....
    属性名 数据类型
)
```
2. 查看表     
`DESCRIBE NAME;`
3. 删除表
`DROP TABLE NAME;`
4. 修改表
①修改表名
`ALTER TABLE OLD_NAME RENAME (TO) NEW_NAME;`
②增加字段
a. 在最后一个位置增加字段：
`ALTER TABLE NAME ADD 属性名 属性类型;`
b. 在第一个位置增加字段：
`ALTER TABLE NAME ADD 属性名 属性类型 FIRST;`
c. 在指定字段之后增加字段：
`ALTER TABLE NAME ADD 属性名 属性类型 AFTER 属性名;`
5. 修改字段的内容  
` ALTER TABLE NAME MODIFY 属性名 修改后的数据类型;`
6. 修改字段的顺序
```
ALTER TABLE NAME MODIFY
    属性名1 数据类型 FIRST|AFTER 属性名2;
```
注：“属性名1”表示要调整顺序的字段名；
       “FIRST”表示将字段调整到表的第一个位置；
       “AFTER 属性名2”表示将字段调整到属性名2字段位置之后；
7. 表的约束
```
CREATE TABLE NAME (
    属性名 数据类型	NOT NULL,	（设置非空约束）
    属性名 数据类型	DEFAULT 默认值,	（设置字段的默认值）
    属性名 数据类型	UNIQUE L,	（设置唯一约束）
    属性名 数据类型	PRIMARY KEY,	（设置主键约束）
    属性名 数据类型	AUTO_INCREMENT,	（设置字段值自动增加）
    CONSTRAINT 约束名 PRIMARY KEY(属性名,属性名,...),
 	                                （设置多字段主键）
    CONSTRAINT 外键约束名 FOREING KEY(属性名1)
        REFERENCES 表名(属性名2),
                                        （设置外键约束）
)
```
三、**索引**
1. 创建索引
①普通索引
`ALTER TABLE NAME ADD INDEX|KEY 索引名(属性名);`
②唯一索引
`ALTER TABLE NAME ADD UNIQUE INDEX|KEY 索引名(属性名);`
③全文索引
`ALTER TABLE NAME ADD FULLTEXT INDEX|KEY 索引名(属性名);`
④多列索引
```
ALTER TABLE NAME ADD INDEX|KEY 索引名( 属性名1,
                                       属性名2,
                                       ......
                                       属性名n);
```
2. 删除索引
`DROP INDEX 索引名 ON NAME;`
四、**视图**
1. 创建视图
`CREATE VIEW NAME AS 查询语句;`
2. 查看视图
```
SHOW TABLE STATUS FROM VIEW LIKE ‘pattern’;
```
或者
`DESCRIBE|DESC NAME;`
3. 删除视图
`DROP VIEW NAME;`
4. 修改视图
`CREATE OR REPLACE VIEW NAME AS 查询语句;`
或者
`ALTER VIEW NAME AS 查询语句;`
五、**触发器**
1. 创建触发器
```
CREATE TRIGGER NAME
    BEFORE|AFTER	[DELETE|INSERT|UPDATE 语句]
        ON TABLE_NAME FOR EACH ROW 激活后被执行的语句;
```
或者
```
DELIMITER $$
CREATE TRIGGER NAME
    BEFORE|AFTER	[DELETE|INSERT|UPDATE 语句]
        ON TABLE_NAME FOR EACH ROW
            BEGIN
                激活后被执行的语句;
            END
    $$
DELIMITER;
```
2. 查看触发器
`SELECT * FROM TRIGGERS;`
3. 删除触发器
`DROP TRGGER NAME;`
六、**数据**
1. 插入数据
```
INSERT INTO NAME(field 1,field 2,...,field n)
  VALUES(value 11,value 21,...,value n1);
          ...
          ...
        (value 1m,value 2m,...,value nm);
```
或者
```
INSERT INTO NAME(field 1,field 2,...,field n)
  SELECT (field 21,field 22,...,field 2n) FROM NAME WHERE语句;
```
2. 更新数据
```
UPDATE NAME SET
    field 1=value 1,field 2=value 2,...
        WHERE CONDITION（指定更新满足条件的特定数据记录）
```
注：如果在更新语句中，没有关键字WHERE语句，则将更新表中所有的数据记录；
3. 删除数据
`DELETE FROM NAME WHERE CONDITION;`
