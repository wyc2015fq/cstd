# oracle procedure 心得 - 左直拳的马桶_日用桶 - CSDN博客
2017年06月24日 09:46:14[左直拳](https://me.csdn.net/leftfist)阅读数：325标签：[oracle](https://so.csdn.net/so/search/s.do?q=oracle&t=blog)
个人分类：[oracle](https://blog.csdn.net/leftfist/article/category/5734215)
近日在写oracle的存储过程，有一点心得，虽然那个存储过程尚未写完，但先记录如下：
**1、权限**
我们为账户赋权限的时候，往往用的是角色。但是，在存储过程中，角色是用不上的。比如说，你已经
`grant connect,resource to myuser;`
按道理，用户myuser可以访问其他表空间里的表了。在一般的SQL窗口完全没有问题，但在存储过程里却不行，查了下，说是oracle的存储过程里不承认权限角色？ 
所以，如果oracle的存储过程里要访问别的表空间的话，还要再赋一个权限：
`grant select any table to myuser;`
**2、into 变量**
时间太长了，不记得自己有关这个知识点的心得了。可能我想记下来的是这个： 
如 `select f1,f2 into 变量1，变量2 from dual;`
3、序列号与group by 
将查询结果插入表，使用了分组group by，则某字段值如果采用序列号（sequence）是不行的，比如
```
insert into t(id,m)
select seq1.nextval,max(create_date) 
from t2 group by code;
```
我想到的办法只能是将ID从自增整型改为GUID：
```
insert into t(id,m)
select sys_guid(),max(create_date) 
from t2 group by code;
```
4、日期字符串格式
`to_date('2017/06/24 9:45:00','yyyy/MM/dd hh24:mi:ss')`
24小时格式为 hh24，分钟为mi，而不是我们习惯的小写mm
