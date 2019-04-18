# Oracle session连接数和inactive的问题记录 - z69183787的专栏 - CSDN博客
2014年04月12日 19:29:42[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1384
从上周起，服务器Oracle数据库出现问题，用不到半天，就会报maxsession（150）的问题，肯定是数据库的会话超过最大数了。 
  由于服务器跑的是文件传输应用，占用的请求和会话肯定很大，因此用户数不大就已经让oracle的会话数达到最大值。 
  处理方式不外乎两种：扩大oracle最大session数以及清除inactive会话，当然还有，就是从数据库连接池和程序bug上面下手。 
从各处收集了一些查看当前会话的语句，记录一下： 
1.select count(*) from v$session； 
  select count(*) from v$process； 
  查看当前总会话数和进程数，这两个视图就是跟会话及进程有关的重要视图啦，信息都是从这里面取的。 
2.查询那些应用的连接数此时是多少 
select  b.MACHINE, b.PROGRAM , count(*) from v$process a, v$session b where a.ADDR = b.PADDR and  b.USERNAME is not null   group by  b.MACHINE  ,
 b.PROGRAM order by count(*) desc; 
3.查询是否有死锁 
select * from v$locked_object; 
如果查询结果为no rows selected，说明数据库中没有死锁。否则说明数据库中存在死锁。 
接下来说明一下会话的状态： 
1.active 处于此状态的会话，表示正在执行，处于活动状态。 
2.killed 处于此状态的会话，表示出现了错误，正在回滚，当然，也是占用系统资源的。还有一点就是，killed的状态一般会持续较长时间，而且用windows下的工具pl/sql developer来kill掉，是不管用的，要用命令：alter system kill session 'sid,serial#'
 ; 
3.inactive 处于此状态的会话表示不是正在执行的，比如select语句已经完成。我一开始以为，只要是inactive状态的会话，就是该杀，为什么不释放呢。其实，inactive对数据库本身没有什么影响，但是如果程序没有及时commit，那么就会造成占用过多会话。解决inactive的方法最好的就是在oracle中直接设置超时时间，也是有两种方法，区别暂时还不清楚： 
1.修改sqlnet.ora文件，新增expire_time=x(单位是分钟)   
我的sqlnet.ora位置在D:\oracle\ora92\network\admin 
