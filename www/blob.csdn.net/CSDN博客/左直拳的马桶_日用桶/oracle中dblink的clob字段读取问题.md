# oracle中dblink的clob字段读取问题 - 左直拳的马桶_日用桶 - CSDN博客
2018年01月24日 19:09:54[左直拳](https://me.csdn.net/leftfist)阅读数：627
oracle中，使用数据库连接（DbLink），源表中的CLOB等大字段是不能在select语句中展现出来的。比如说，有个源表tsource，里面有个大字段photo，那么运行以下语句会报错：
`select * from tsource@dblink1;`
那怎么办呢？可以将该表的数据拷贝到本地库。就是这么怪，不能展示，但能传输：
```
create table mysource as select * from tsource@dblink1;
select * from mysource;--有了，爽歪歪
```
当然，现实场景中，可能会搞个JOB，同步表之类。但原理都一样，将数据复制到本地。
