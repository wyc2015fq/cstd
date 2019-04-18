# 查看ORACLE是共享还是专用服务器模式 - z69183787的专栏 - CSDN博客
2015年01月22日 11:13:52[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2591
1.select count(*) from v$shared_server;或不为O表示为共享服务器模式。
2.在tnsnames.ora里设定的
aaaa=
(DESCRIPTION =
(ADDRESS_LIST =
(ADDRESS = (PROTOCOL = TCP)(HOST = aaaa)(PORT = 1521))
)
(CONNECT_DATA =
(SID = aa)
(SERVER = DEDICATED) 
)
)
红色部分指定了用DEDICATED方式连接DB.
另外，Background process ,以及通过本地连接进来的，只能是DEDICATED
比如说sqlplus user/pass 形式
如果DB没有配置共享服务器，那么Client只能以DEDICATED方式连接DB.
3。判断一个已经连接的session的连接方式有两种方法
A
select server from v$session 
where ...
如果server = 'DEDICATED'则是DEDICATED方式
server='SHARED'则是shared方式，并且正有shared_server_process为其服务
server='NONE'的话，则是shared方式，并且当前没有shared_server_process为其服务。
B. 仅用于Unix 底下，似乎windown不行
连接v$session, v$process 看process中的program
1 select p.program,s.server from v$session s , v$process p
2* where s.paddr = p.addr
如果 program 为 。。(S0NN) 的，则是shared方式，并且正有shared_server_process为其服务
如果 program 为 。。(D0NN) 的，则是shared方式，并且当前没有shared_server_process为其服务
如果 program 为 其它的，则是'DEDICATED'方式
