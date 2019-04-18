# 【数据平台】同一mysql主机不同数据库建复制表 - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年09月27日 12:49:53[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：584








场景：mysql服务器同一台，要从 a 数据库中把表 t 复制到数据库 b 中。




进入数据库b，执行：


语句：


`create table t like a.t;`
`INSERT t SELECT * FROM a.t;`




