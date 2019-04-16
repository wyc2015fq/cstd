# [数据库] Navicat for MySQL事件Event实现数据每日定期操作 - 杨秀璋的专栏 - CSDN博客





2017年03月10日 12:26:10[Eastmount](https://me.csdn.net/Eastmount)阅读数：13531
所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)[数据库实战开发设计与优化](https://blog.csdn.net/column/details/14842.html)









        在我们操作数据库过程中，通常会遇到一些某个时间点操作数据库的问题，例如：
        (1).每天凌晨12点对数据库进行定时备份，结算和汇总；
        (2).每天凌晨2点删除数据库前三天的数据；
        (3).插入某个数据超过一定时间改变某个值的状态，比如预警系统。
        这里就需要通过Event事件进行简单操作，下面将详细处理。你可能会想到通过触发器实现，但是如果是同一张表Insert插入数据后，但是触发器再进行Update更新操作是不行的，所以需要尝试通过Event事件解决。
        在前面讲过一篇文章：[[数据库] Navicat for MySQL触发器更新和插入操作](http://blog.csdn.net/eastmount/article/details/52344036)        希望这篇文章对你有所帮助，如果文章中出现错误或不足之处，还请海涵~




## 一. Event实现每日某个时刻删除事件

        首先通过Navicat for MySQL查看Event事件是否开启，具体代码：
`show variables like '%sche%';`        通常事件是开启的，如下图所示：


![](https://img-blog.csdn.net/20170309234242260)


        如果没有开启，需要数据库超级权限设置。
`set global event_scheduler =1;`
        其中event_scheduler为ON表示开启，OFF表示关闭，如下：

+-----------------+-------+

| Variable_name   | Value |

+-----------------+-------+

| event_scheduler | OFF   |

+-----------------+-------+
        下面开始写事件进行操作。
        假设现在存在一张表loginuser，包括用户名、密码、权限三个字段。如下图：


![](https://img-blog.csdn.net/20170309234800435)



        当前时间点为2017年3月9日 23:48，我们设置23:50执行删除eastmount数据操作。其中SQL语句的Event代码如下：

```
CREATE EVENT testevent
	ON SCHEDULE EVERY 1 DAY STARTS '2017-03-09 23:50:00'
  DO
		delete from loginuser where Username='eastmount';
```
        创建事件可以查看到的，如下图所示：


![](https://img-blog.csdn.net/20170309235055749)


        然后过了23:50执行查询语句就会发现eastmount用户已经被删除。


![](https://img-blog.csdn.net/20170309235215453)


        点开事件如下图所示，但我更推荐大家写SQL语句。



![](https://img-blog.csdn.net/20170309235748596)


![](https://img-blog.csdn.net/20170309235809049)

        通过上面这段SQL语句，我们知道了最简单的Event事件，下面进一步加深介绍。备份数据加个备份SQL语句执行即可。
        参考官网：[https://dev.mysql.com/doc/refman/5.7/en/event-scheduler.html](https://dev.mysql.com/doc/refman/5.7/en/event-scheduler.html)







## 二. Event实现每隔某段时间定时插入事件

        在使用事件Event中，需要开启和关闭事件，使用的代码如下：
        关闭事件：ALTER EVENT testevent DISABLE;
        开启事件：ALTER EVENT testevent ENABLE;
        查看事件是否开启的代码如下：

`SELECT * FROM mysql.event;`        输出如下图所示：


![](https://img-blog.csdn.net/20170310084326922)

        下面定义每隔10秒插入一行数据的事件，比如实时接收交通数据信息等实时处理，通常会使用该方法进行插入操作。创建一个表logininfo，记录登录信息，字段包括：ID序号（主键、递增INT型）、NAME用户名、STARTTIME（登录时间）、ENDTIME（登出时间）、STATE（状态 0-离线 1-在线）。


![](https://img-blog.csdn.net/20170310085328160)

        SQL语句创建EVENT事件如下：

```
CREATE EVENT insertevent
    ON SCHEDULE EVERY 10 SECOND 
    DO
				INSERT INTO logininfo(NAME,STARTTIME,STATE) values('test01',now(),'1');
```
        运行结果如下图所示：


![](https://img-blog.csdn.net/20170310090901995)

        从图中可以看到，每隔10秒就插入一组数据，该事件正确执行。
        注意：在真实的开发过程中，会遇到mysql服务重启或断电等情况，此时则会出现时间调度器被关闭的情况，所有事件都不起作用，解决方法，需要在mysql.ini文件中加入 "event_scheduler = ON;" 语句。








## 三. Event实现超过某个时间更新状态

        假设现在有一个报警系统，当用户进入房间开始计时，当用户在放在中待了2小时就开始报警，设置状态。数据如下：



![](https://img-blog.csdn.net/20170310120521984)

        SQL语句查询返回结果如下：

```
select current_timestamp, starttime, timestampdiff(SECOND,starttime,current_timestamp) as T from logininfo;
```
        输出结果如下图所示，每隔10秒会插入一个数据，所以不断更新。


![](https://img-blog.csdn.net/20170310120656625)



        现在的更新UPDATE的语句如下所示：


```
CREATE EVENT updateevent
    ON SCHEDULE EVERY 10 SECOND 
    DO
				update logininfo set STATE=0 WHERE 
					timestampdiff(SECOND,starttime,current_timestamp)>300 and STATE=1;
```
        然后查询实时更新状态的SQL语句如下：

```
select ID, STATE, current_timestamp, starttime, 
				timestampdiff(SECOND,starttime,current_timestamp) as T 
from logininfo;
```
        输出如下图所示，发现302可能是查过300秒，可能实时更新的延误，但不影响效果的。


![](https://img-blog.csdn.net/20170310121208997)



        讲到这里，一个简单预警系统的数据库就设置完成，总共我们设置了3个事件，如下图所示：



![](https://img-blog.csdn.net/20170310121703207)






        补充一段代码：


```
delimiter //
CREATE EVENT e
ON SCHEDULE
    EVERY 5 SECOND
DO
BEGIN
    DECLARE v INTEGER;
    DECLARE CONTINUE HANDLER FOR SQLEXCEPTION BEGIN END;
    SET v = 0;
    WHILE v < 5 DO
        INSERT INTO t1 VALUES (0);    
        UPDATE t2 SET s1 = s1 + 1;
        SET v = v + 1;
    END WHILE;
END //
delimiter ;
```

        最后希望文字对您有所帮助，基础性文章，如果存在错误或不足之处，还请海涵~
        最近自己和她太操劳，自己需要多付出点，多学点，多为她做点，forever。
(By:Eastmount 2017-03-10 中午12点[http://blog.csdn.net//eastmount/](http://blog.csdn.net//eastmount/) )







