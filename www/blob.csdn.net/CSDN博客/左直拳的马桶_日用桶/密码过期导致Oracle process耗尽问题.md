# 密码过期导致Oracle process耗尽问题 - 左直拳的马桶_日用桶 - CSDN博客
2016年12月15日 20:05:10[左直拳](https://me.csdn.net/leftfist)阅读数：934
oracle忽然连不上！
大致是报这样的错：
```
ORA-12516: TNS: 监听程序找不到符合协议堆栈要求的可用处理程序
ORA-12520: TNS: 监听程序无法为请求的服务器类型找到可用的处理程序
```
但又不是全部时间都报错，有时候又正常，尤其是将数据库刚重启之后。
用oracle管理员sys进去看，直接原因是oracle的进程数process爆表了：
```
SQL> show parameter processes;
NAME                                 TYPE        VALUE
------------------------------------ ----------- ------------
aq_tm_processes                      integer     1
db_writer_processes                  integer     1
gcs_server_processes                 integer     0
global_txn_processes                 integer     1
job_queue_processes                  integer     1000
log_archive_max_processes            integer     4
processes                            integer     300
```
可知processes的限定值为300;而当前使用数量为
```
SQL> select count(*) from v$process;
  COUNT(*)
----------
        299
```
所以连接不进去。那为啥process用了这么多？以前又没发现有这个问题。 
查看会话数，发现与process严重不对等，只有区区30多个：
```
SQL> select count(*) from v$session where status='ACTIVE';
  COUNT(*)
----------
        34
```
所谓的会话，就是已经连接到数据库了，我的oracle是DEDICATED（专用）模式，通常process与session是一对一关系。所以这里process比session多出来那么多，除去oracle本身的开销，说明引起oracle资源耗尽的原因，不是数据库里面的操作，而是在外面。
再看是啥占用process最多：
```
SQL> select count(*),program from v$process group by program having count(*) > 1
;
  COUNT(*) PROGRAM
---------- ----------------------------------------------------------------
        255 ORACLE.EXE (SHAD)
```
这个ORACLE.EXE (SHAD)与oracle连接有关。所以，最有可能是**有个东西在连接我们的oracle，但又连不上，于是不停地尝试，oracle不断创建process进行响应，最后导致资源耗尽，死翘翘了**。
那个鬼东西是啥？我逐一排查。停掉一些服务器上的程序，没啥效果，最后停掉arcgis，process一下子降下来了。罪魁祸首原来是arcgis！
为啥arcgis会不断连接oracle呢？查看它的错误日志，发现有提示密码过期！
`The Layer:'****项目' in Map:'图层' is invalid. The base table definition string "SDE.****" is invalid. User password has expired.`
奇怪啊，咋会过期呢？我好奇地用连接串中指定的账号登录oracle，结果真的提示密码过期！
```
SQL> conn sde/sde@pdbhnhy;
ERROR:
ORA-28001: the password has expired
更改 sde 的口令
新口令:
```
我靠。几天来的问题元凶终于找到了，我禁不住老泪纵横。
还没有结束。为啥密码会过期？原来，oracle11g和之后（我的oracle是12C），默认账号密码期限是180天。
```
SQL> select * from dba_profiles where profile='DEFAULT' and resource_name='PASSWORD_LIFE_TIME';
PROFILE              RESOURCE_NAME        RESOURCE LIMIT                COM
-------------------- -------------------- -------- -------------------- -
DEFAULT              PASSWORD_LIFE_TIME   PASSWORD 180            NO
```
为一劳永逸，将密码改为没有期限限制：
`ALTER PROFILE DEFAULT LIMIT PASSWORD_LIFE_TIME UNLIMITED;`
修改之后不需要重启动数据库，会立即生效。
修改后，还没有被提示ORA-28002警告的帐户不会再碰到同样的提示; 已经被提示的帐户必须再改一次密码，举例如下：
```
$sqlplus / as sysdba
sql> alter user smsc identified by <原来的密码> ----不用换新密码
```
网上有文章说遇到这种错误，可以将process的限定值设大一点，我认为这是没有搞清错误根源的应对之道。像我这种情况，就算你将process设得再大，也经不起损耗。更何况，修改oracle的系统参数，风险太大了，搞不好整个数据库都起不来，到时哭都没有眼泪。我上次就是因为看了网上一篇文章的指引，想着调大一点内存设置，结果数据库再也没能启动过。要不是有备份，我都不知道怎么交代。
不过，oracle12C号称支持插件数据库，几个插件数据库（PDB）共用一个容器数据库（CDB），也就是实例了，但是这个process数量也是共用的。所以PDB数量一多，process也有可能出现不够的问题。这时的话，或许也可以将proess设大一点。不过，窃以为，新增一个实例会更合适。
另外，oracle为啥要不断新增process来响应这种无谓的连接请求呢？岂不是很容易被饱和攻击？也太逊了点吧，这就是号称数据库头把交椅的oracle？我估计，如果是账号密码不对的请求，oracle是不会有什么回应的；但对于密码过期，可能比较特殊：有反应，能连，但操作不了，造成客户端不断的去请求？
参考文章： 
[数据库连接进程过高的问题](http://blog.csdn.net/liufeng1980423/article/details/49001347)
[11g oracle 用户密码过期问题](http://www.blogjava.net/freeman1984/archive/2013/04/23/398301.html)
================================= 
如果系统已经提示密码过期，或即将过期，则修改配置文件还是不够的。仍然要修改一次密码。
参考文章： 
[Oracle 11g中修改默认密码过期天数和锁定次数](https://my.oschina.net/junn/blog/322986)
