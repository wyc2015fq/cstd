# oracle 执行被锁表 - 菜鸟路上的小白 - CSDN博客





2018年01月24日 10:49:21[somnus_小凯](https://me.csdn.net/u012486840)阅读数：174
个人分类：[java](https://blog.csdn.net/u012486840/article/category/6382670)









                
1.下面的语句用来查询哪些对象被锁：

select object_name,machine,s.sid,s.serial#

from v$locked_object l,dba_objects o ,v$session s

where l.object_id　=　o.object_id and l.session_id=s.sid;



2.下面的语句用来杀死一个进程：

alter system kill session '24,111'; (其中24,111分别是上面查询出的sid,serial#)

【注】以上两步，可以通过Oracle的管理控制台来执行。
            


