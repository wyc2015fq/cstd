# oracle的日期蛋 - 左直拳的马桶_日用桶 - CSDN博客
2017年01月10日 19:48:25[左直拳](https://me.csdn.net/leftfist)阅读数：283标签：[oracle](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[oracle](https://blog.csdn.net/leftfist/article/category/5734215)
一切都是扯鸡巴蛋。 
在网上查oracle的日期函数用法，得到一大堆语法，林林总总，都是扯鸡巴蛋，没能解决我的问题。
其实，我想写这么一条语句：查找某个日期（不含时分秒）产生或有关的记录。咋写？
SQL SERVER下就容易咯，
`select * from table1 where createdate='2017-01-10'`
多么容易，新手拈来。但是，在ORACLE下可不行。
找来找去，是这样的：
`select * from table1 where to_char(createdate,'yyyy-MM-dd')='2017-01-10';`
如果是当天，就简单一些：
`select * from table1 where trunc(createdate,'dd')=trunc(sysdate,'dd');`
