# Oracle 按时间段分组统计 （使用LEVEL） - z69183787的专栏 - CSDN博客
2013年04月01日 14:56:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：8052

想要按时间段分组查询，首先要了解level,connect by,oracle时间的加减. 
关于level这里不多说，我只写出一个查询语句:
**[sql]**[view
 plain](http://blog.csdn.net/wanglipo/article/details/6556665#)[copy](http://blog.csdn.net/wanglipo/article/details/6556665#)
- ---level 是一个伪例
- selectlevelfrom dual connectbylevel <=10  
- ---结果：1    
- 2  
- 3  
- 4  
- 5  
- 6  
- 7  
- 8  
- 9  
- 10  
关于connect by可以看 
[http://www.cnblogs.com/johnnyking39/articles/1155497.html](http://www.cnblogs.com/johnnyking39/articles/1155497.html)
oracle时间的加减看看试一下以下sql语句就会知道:
**[sql]**[view
 plain](http://blog.csdn.net/wanglipo/article/details/6556665#)[copy](http://blog.csdn.net/wanglipo/article/details/6556665#)
- select sysdate -1 from dual  
- ----结果减一天,也就24小时
- select sysdate-(1/2) from dual  
- -----结果减去半天，也就12小时
- select sysdate-(1/24) from dual  
- -----结果减去1 小时
- select sysdate-((1/24)/12) from dual   
- ----结果减去5分钟
- select sydate-(level-1) from dual connectbylevel<=10  
- ---结果是10间隔1天的时间
下面是本次例子：
**[sql]**[view
 plain](http://blog.csdn.net/wanglipo/article/details/6556665#)[copy](http://blog.csdn.net/wanglipo/article/details/6556665#)
- select dt, count(satisfy_degree) as num from T_DEMO  i ,  
- (select sysdate - (level-1) * 2 dt  
- from dual connectbylevel <= 10) d  
- where i.satisfy_degree='satisfy_1'and
- i.insert_time<dt and i.insert_time> d.dt-2  
- groupby d.dt    
例子中的sysdate - (level-1) * 2得到的是一个间隔是2天的时间 
group by d.dt  也就是两天的时间间隔分组查询
自己实现例子：
create table A_HY_LOCATE1
(
  MOBILE_NO          VARCHAR2(32),
  LOCATE_TYPE        NUMBER(4),
  AREA_NO            VARCHAR2(32),
  CREATED_TIME       DATE,
  AREA_NAME          VARCHAR2(512),
)；
select (sysdate-13)-(level-1)/4 from dual connect by level<=34  --从第一条时间记录开始（sysdate-13）为表中的最早的日期，“34”出现的分组数（一天按每六个小时分组 就应该为4）
一下是按照每6个小时分组   
select mobile_no,area_name,max(created_time ),dt, count(*) as num from a_hy_locate1  i ,
(select (sysdate-13)-(level-1)/4  dt
from dual connect by level <= 34) d
where i.locate_type = 1 and
i.created_time<dt and i.created_time> d.dt-1/4
group by mobile_no,area_name,d.dt
另外一个方法：
--按六小时分组
select trunc(to_number(to_char(created_time, 'hh24')) / 6),count(*)
  from t_test
 where created_time > trunc(sysdate - 40)
 group by trunc(to_number(to_char(created_time, 'hh24')) / 6)
--按12小时分组
select trunc(to_number(to_char(created_time, 'hh24')) / 6),count(*)
  from t_test
 where created_time > trunc(sysdate - 40)
 group by trunc(to_number(to_char(created_time, 'hh24')) / 6)
**分享到：**
