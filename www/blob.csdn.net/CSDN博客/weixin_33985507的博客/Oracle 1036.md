# Oracle 1036 - weixin_33985507的博客 - CSDN博客
2010年11月12日 16:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：3
Kernel修订
/etc/sysctl.conf:
net.ipv4.tcp_keepalive_time = 600
net.ipv4.tcp_keepalive_intvl = 60
net.ipv4.tcp_keepalive_probes = 20
**在 sqlnet.oraSQLNET.INBOUND_CONNECT_TIMEOUT = 0SQLNET.RECV_TIMEOUT = 0SQLNET.SEND_TIMEOUT = 0在 listener.ora 增加：INBOUND_CONNECT_TIMEOUT_LISTENER = 30重启监听器：lsnrctl reload**
在警告日志文件中(*alert.log*)持续出现如下错误:
Tue Jul 18 23:09:22 2006
WARNING: inbound connection timed out (**ORA-3136**)
Tue Jul 18 23:09:23 2006
WARNING: inbound connection timed out (**ORA-3136**)
Tue Jul 18 23:09:25 2006
WARNING: inbound connection timed out (**ORA-3136**)
Tue Jul 18 23:09:30 2006
WARNING: inbound connection timed out (**ORA-3136**)
Tue Jul 18 23:12:15 2006
WARNING: inbound connection timed out (**ORA-3136**)
同时在sqlnet.log中记录了如下错误:
Fatal NI connect error 12170.
VERSION INFORMATION:
TNS for Linux: Version 10.2.0.2.0 - Production
Oracle Bequeath NT Protocol Adapter for Linux: Version 10.2.0.2.0 - Production
TCP/IP NT Protocol Adapter for Linux: Version 10.2.0.2.0 - Production
Time: 19-JUL-2006 11:25:26
Tracing not turned on.
Tns error struct:
ns main err code: 12535
TNS-12535: TNS:operation timed out
ns secondary err code: 12606
nt main err code: 0
nt secondary err code: 0
nt OS err code: 0
Client address: (ADDRESS=(PROTOCOL=tcp)(HOST=192.168.1.123)(PORT=58147))
这是和网络连接相关的一个错误,Metalink上给出了如下的解决方案:
> 
**1.set INBOUND_CONNECT_TIMEOUT_<listenername>=0 in listener.ora 2. set SQLNET.INBOUND_CONNECT_TIMEOUT = 0 in sqlnet.ora of server.3. stop and start both listener and database.4. Now try to connect to DB and observe the behaviour**
这里重起数据库和Listener我认为是没有必要的,我们reload一下Listner应该就可以了.
[oracle@order admin]$ lsnrctl 
LSNRCTL for Linux: Version 10.2.0.2.0 - Production on 19-JUL-2006 15:26:33
Copyright (c) 1991, 2005, Oracle. All rights reserved.
Welcome to LSNRCTL, type "help" for information.
LSNRCTL> reload 
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=172.16.9.11)(PORT=1521)))
The command completed successfully
LSNRCTL> services
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=172.16.9.11)(PORT=1521)))
Services Summary...
Service "order" has 2 instance(s).
Instance "order", status UNKNOWN, has 1 handler(s) for this service...
Handler(s):
"DEDICATED" established:0 refused:0
LOCAL SERVER
Instance "order", status READY, has 1 handler(s) for this service...
Handler(s):
"DEDICATED" established:0 refused:0 state:ready
LOCAL SERVER
The command completed successfully 
LSNRCTL> show inbound_connect_timeout
Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=172.16.9.11)(PORT=1521)))
LISTENER parameter "inbound_connect_timeout" set to 0
The command completed successfully
LSNRCTL> exit
修改之后,观察了一段时间,目前正常.
关于SQLNET.INBOUND_CONNECT_TIMEOUT参数,Oracle建议修改该参数,以避免denial-of-service攻击.
引用一段Oracle文档说明如下:
**SQLNET.INBOUND_CONNECT_TIMEOUT**
***Purpose***Use the SQLNET.INBOUND_CONNECT_TIMEOUT parameter to specify the time, in seconds, for a client to connect with the database server and provide the necessary authentication information.
If the client fails to establish a connection and complete authentication in the time specified, then the database server terminates the connection. In addition, the database server logs the IP address of the client and an ORA-12170: TNS:Connect timeout occurred error message to the sqlnet.log file. The client receives either an ORA-12547: TNS:lost contact or an ORA-12637: Packet receive failed error message.
Without this parameter, a client connection to the database server can stay open indefinitely without authentication. Connections without authentication can introduce possible denial-of-service attacks, whereby malicious clients attempt to flood database servers with connect requests that consume resources.
To protect both the database server and the listener, Oracle Corporation recommends setting this parameter in combination with the INBOUND_CONNECT_TIMEOUT_listener_name parameter in the listener.ora file. When specifying values for these parameters, consider the following recommendations:
Set both parameters to an initial low value. 
Set the value of the INBOUND_CONNECT_TIMEOUT_listener_name parameter to a lower value than the SQLNET.INBOUND_CONNECT_TIMEOUT parameter. 
For example, you can set INBOUND_CONNECT_TIMEOUT_listener_name to 2 seconds and INBOUND_CONNECT_TIMEOUT parameter to 3 seconds. If clients are unable to complete connections within the specified time due to system or network delays that are normal for the particular environment, then increment the time as needed.
*See Also: *Oracle9i Net Services Administrator's Guide for information about configuring these parameters 
***Default***
None
***Example***
SQLNET.INBOUND_CONNECT_TIMEOUT=3
