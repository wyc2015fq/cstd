# postgres初级介绍，与mysql的一些比较 - Arthur的随笔 - CSDN博客
2012年05月09日 16:28:56[largetalk](https://me.csdn.net/largetalk)阅读数：1888
apt-get 安装好postgres之后会生成一个postgres系统用户，虽然pg中的role和系统用户没有关系，但开始时必须用postgres登录系统才能连上pg，这也算是一个不人性的地方吧
1. edit /etc/postgresql/9.1/main/pg_hba.conf 添加如下行
host    test       test     127.0.0.1         255.255.255.255    md5
host    test      test     192.168.1.0     255.255.255.0          password
edit /etc/postgresql/9.1/main/postgresql.conf 修改如下行
listen_addresses = "*"
/etc/init.d/postgres restart
2.  su postgres
3.  createuser -ADPE test   #-D no createdb -E encrypt -P password prompt
4. createdb -O test test   #-O owner
5. createlang plpgsql test
# psql -h ip  -U test test
能连上就算基本配置好了。
翻了一遍pg的文档，给我感触有：
1.数据类型更丰富，支持数组， 几何数据， ip数据类型等等，我觉得数组和一些数组函数非常有用
2. 多个schema，mysql需要多db才能实现的功能pg可以一个db就完成了，好处是可以共享链接池，应用更简单
3. 查询可以用正则。 mysql好像也有这个功能，记的不是太清楚
4. 有sequence， table space等概念，以前在oracle中见过，table space 很少用到，但sequence确实是个好东西
5. 表可以继承，文档有个使用表继承进行分区的例子，和mysql相比有好处也有坏处
6. index不只有b-tree，还有hash ， Gist， GIN，虽然其他的很少用，而且后两者我现在也没搞懂
习惯了mysql再来使用postgres，虽然基本sql是一样的，但像备份，还原，repliaction等等都不同了，原来对mysql的调优也是非常清楚，对于postgres也不太清楚，这也是个风险。
贴一些命令，备忘
```
show all;
set search_path "xxxx";
CREATE DATABASE dbname TEMPLATE template0;
CREATE TABLESPACE fastspace LOCATION '/mnt/sda1/postgresql/data';
CREATE TABLE foo(i int) TABLESPACE space1;
SET default_tablespace = space1;
CREATE TABLE foo(i int);
pg_dump dbname > outfile
psql dbname < infile
psql --set ON_ERROR_STOP=on dbname < infile
pg_dump -h host1 dbname | psql -h host2 dbname
pg_dumpall > outfile
psql -f infile postgres
pg_dump dbname | gzip > filename.gz
gunzip -c filename.gz | psql dbname
pg_dump dbname | split -b 1m - filename
cat filename* | psql dbname
```
