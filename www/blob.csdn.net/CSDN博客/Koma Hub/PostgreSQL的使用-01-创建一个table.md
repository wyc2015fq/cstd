# PostgreSQL的使用-01-创建一个table - Koma Hub - CSDN博客
2018年06月07日 20:53:51[Koma_Wong](https://me.csdn.net/Rong_Toa)阅读数：259
# PostgreSQL的使用（1）
## [](https://github.com/rtoax/ML/blob/master/SQL/PostgreSQL/PostgreSQL-study-01.md#%E4%B8%80%E5%A6%82%E4%BD%95%E4%BD%BF%E7%94%A8postgresql)一.如何使用PostgreSQL
- 安装-略
- 登陆PostgreSQL
$ su
# su - postgres
$ 
这时就进入了PostgreSQL的安装目录/opt/PostgreSQL/10/
- 创建一个数据库，并连接数据库
$ createdb testdb
$ psql testdb
testdb=#
之后你就会一直在testdb=#下操作，当一个语句没有写完时为“testdb-#”，写完时为“testdb=#”，写完的标志为语句以“;”结束。
## [](https://github.com/rtoax/ML/blob/master/SQL/PostgreSQL/PostgreSQL-study-01.md#%E4%BA%8C%E7%AE%80%E5%8D%95%E7%9A%84%E6%95%B0%E6%8D%AE%E5%BA%93%E6%93%8D%E4%BD%9C)二.简单的数据库操作
- 创建一个TABLE
createtablet(
    name textnot null,
    age integer,
    sex text,
    score numericcheck (score <100.0)
);
createtablet2(
    name textprimary key,
    age integer,
    sex text,
    score numericcheck(score <100.0)
);
createtablet3(
    name textnot nullprimary key,
    age integer,
    sex text,
    score numericcheck(score <100.0)
);
createtablet4(
    name text,
    age integer default 18,
    sex text,
    score numericcheck(score <100.0),
    primary key(name, age)
);
上面的not null、primary key和check为约束，default为缺省值。
- 显示一个TABLE
table t;
 name | age | sex | score 
------+-----+-----+-------
(0 rows)
- 删除一个TEBLE
droptable t;
- 增加字段
altertable t4 add column degree text;
altertable t4 drop score;
table t4;
输出为：
```
name | age | sex | degree 
------+-----+-----+--------
(0 rows)
```
- 修改字段名称、表名
altertable t4 rename column name to rong;
altertable t4 rename to t5;
table t5;
输出为：
```
rong | age | sex | degree 
------+-----+-----+--------
(0 rows)
```
- 选择一个列
select rong from t5;
 rong 
------
(0 rows)
select rong,sex from t5;
 rong | sex 
------+-----
(0 rows)
``
