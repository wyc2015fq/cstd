# sql limit offset 区别 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年11月14日 15:57:45[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：175
select * from table limit 2,1;                 
//含义是跳过2条取1条数据，即读取第3条数据
select * from table
limit 2 offset 1;  //这种更容易理解一些
//含义是从第1条（不包括）数据开始取出2条数据，limit后面跟的是2条数据，offset后面是从第1条开始读取，即读取第2,3条
先写order by再limit
SELECT article FROM table1 ORDER BY publish_date LIMIT 20
It will order first, then get the first 20. A database will also process anything in the WHERE clause before ORDER BY.
[https://stackoverflow.com/questions/4708708/mysql-order-by-limit](https://stackoverflow.com/questions/4708708/mysql-order-by-limit)
