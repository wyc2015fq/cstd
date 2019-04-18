# Oracle Dedicated server 和 Shared server（专用模式 和 共享模式) 说明 - z69183787的专栏 - CSDN博客
2014年04月12日 10:26:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：676
个人分类：[Oracle](https://blog.csdn.net/z69183787/article/category/2175483)
## 一.  官网说明
       在DBCA 建库的时候，有提示让我们选择连接类型，这里有两种类型：专用服务器模式和共享服务器模式。默认使用专用模式。如下图：
## ![](http://hi.csdn.net/attachment/201108/21/0_1313911556f7g1.gif)
Oracle 官方文档对这两种文档的说明如下：
About Dedicated andShared Server Processes
[http://download.oracle.com/docs/cd/B28359_01/server.111/b28310/manproc001.htm](http://download.oracle.com/docs/cd/B28359_01/server.111/b28310/manproc001.htm)
       OracleDatabase creates server processes to handle the requests of user processesconnected to an instance. A server process can be either of the following:
       （1）A dedicated server process, which services only one userprocess
       （2）A shared server process, which can service multiple userprocesses
    Oracle 创建Server process 来处理user processes连接实例的的请求。 server process 分两种： dedicated server process和 sharedserver process。
       Yourdatabase is always enabled to allow dedicated server processes, but you mustspecifically configure and enable shared server by setting one ormore initialization parameters.
### 1.1 Dedicated Server Processes
[Figure4-1, "Oracle Database Dedicated Server Processes"](http://download.oracle.com/docs/cd/B28359_01/server.111/b28310/manproc001.htm#i1007374) illustrateshow dedicated server
 processes work. In this diagram two user processes areconnected to the database through dedicated server processes.
       Ingeneral, it is better to be connected through a dispatcher and use ashared server process. This is illustrated in [Figure4-2,
 "Oracle Database Shared Server Processes"](http://download.oracle.com/docs/cd/B28359_01/server.111/b28310/manproc001.htm#i1006182). A shared serverprocess can be more efficient because it keeps the number of processes requiredfor the running instance low.
       Inthe following situations, however, users and administrators should explicitlyconnect to an instance using a dedicated server process:
 在以上两种情况，需要显示的使用dedicated server process 去连接实例：
       （1）To submit a batch job (for example, when a job can allow little orno idle time for the server process)
       （2）To use Recovery Manager (RMAN) to back up, restore, or recover a database
       Torequest a dedicated server connection when Oracle Database is configured forshared server, users must connect using a net service name that is configuredto use a dedicated server. Specifically, the net service name value shouldinclude the SERVER=DEDICATED clause
 in the connect descriptor.
 如果数据库配置的是shared server，想要用dedicated server 去连接实例，就必须配置在net service name（tnsnames.ora）里指定SERVER=DEDICATED。
![](http://hi.csdn.net/attachment/201108/21/0_1313911603cS6x.gif)
Figure 4-1 OracleDatabase Dedicated Server Processes
### 1.2 Shared Server Processes
       Consideran order entry system with dedicated server processes. A customer phones theorder desk and places an order, and the clerk taking the call enters the orderinto the database. For most of the transaction, the clerk is on the telephonetalking to
 the customer. A server process is not needed during this time, sothe server process dedicated to the clerk's user process remains idle. Thesystem is slower for other clerks entering orders, because the idle serverprocess is holding system resources.
       Sharedserver architecture eliminates the need for a dedicated server process for eachconnection (see [Figure4-2](http://download.oracle.com/docs/cd/B28359_01/server.111/b28310/manproc001.htm#i1006182)).
### ![](http://hi.csdn.net/attachment/201108/21/0_13139116554I41.gif)
Figure 4-2 OracleDatabase Shared Server Processes
       Ina shared server configuration, client user processes connect to a dispatcher.The dispatcher can support multiple client connections concurrently. Eachclient connection is bound to a virtual circuit, which is a piece ofshared memory used by the dispatcher
 for client database connection requestsand replies. The dispatcher places a virtual circuit on a common queue when arequest arrives.
       Anidle shared server process picks up the virtual circuit from the common queue,services the request, and relinquishes the virtual circuit before attempting toretrieve another virtual circuit from the common queue. This
 approach enables a small pool of server processes to serve a largenumber of clients. A significant advantage of shared server architectureover the dedicated server model is the reduction of system resources, enablingthe support of an increased number
 of users.
       Foreven better resource management, shared server can be configuredfor connection pooling. Connection pooling lets a dispatcher support moreusers by enabling the database server to time-out protocol connections and touse those connections to service
 an active session. Further, shared server canbe configured for session multiplexing, which combines multiple sessionsfor transmission over a single network connection in order to conserve the operatingsystem's resources.
       Sharedserver architecture requires Oracle Net Services. User processes targeting theshared server must connect through Oracle Net Services, even if they are on thesame machine as the Oracle Database instance.
## 二. 说明
### 2.1 Dedicated server 说明
### ![](http://hi.csdn.net/attachment/201108/21/0_13139117277kfl.gif)
       对于专用服务器配置，在登录时，Oracle 总会创建一个新的进程，这个服务器进程会在会话生存期中专门这个连接服务。对于每个会话，都会出现一个新的专用服务器，会话与专用服务器之间存在一对一的映射。按照定义，这个专用服务器不是实例的一部分。客户进程（也就是想要连接数据库的程序）会通过某种网络通道（如TCP/IP socket）与这个专用服务器直接通信，并由这个服务器进程接收和执行我的SQL。如果必要，它会读取数据文件，并在数据库的缓存中查找我要的数据。也许它会完成我的更新语句，也可能会运行我的PL/SQL
 代码。这个服务器进程的主要目标就是对我提交的SQL 调用做出响应。
### 2.2 Shared server 说明
### 2.2.1  说明
       共享服务器（shared server），正式的说法是多线程服务器（Multi-Threaded Server）或MTS。如果采用这种方式，就不会对每条用户连接创建另外的线程或新的UNIX 进程。
      在共享服务器中，Oracle 使用一个“共享进程”池为大量用户提供服务。共享服务器实际上就是一种连接池机制。利用共享服务器，我们不必为10,000 个数据库会话创建10,000 个专用服务器（这样进程或线程就太多了），而只需建立很少的一部分进程/线程，顾名思义，这些进程/线程将由所有会话共享。这样Oracle 就能让更多的用户与数据库连接，否则很难连接更多用户。如果让我的机器管理10,000个进程，这个负载肯定会把它压垮，但是管理100
 个或者1,000 个进程还是可以的。采用共享服务器模式，共享进程通常与数据库一同启动，使用ps命令可以看到这个进程。
       共享服务器连接和专用服务器连接之间有一个重大区别，与数据库连接的客户进程不会与共享服务器直接通信，但专用服务器则不然，客户进程会与专用服务器直接通信。之所以不能与共享服务器直接对话，原因就在于这个服务器进程是共享的。为了共享这些进程，还需要另外一种机制，通过这种机制才能与服务器进程“对话”。为此，Oracle 使用了一个或一组称为调度器（dispatcher，也称分派器）的进程。
       客户进程通过网络与一个调度器进程通信。这个调度器进程将客户的请求放入SGA中的请求队列（这也是SGA 的用途之一）。第一个空闲的共享服务器会得到这个请求，并进行处理（例如，请求可能是UPDATE T SETX = X+5 WHERE Y = 2）。完成这个命令后，共享服务器会把响应放在原调度器（即接收请求的调度器）的响应队列中。调度器进程一直在监听这个队列，发现有结果后，就会把结果传给客户。从概念上讲，共享服务器请求的流程如图2-3 所示。
![](http://hi.csdn.net/attachment/201108/21/0_1313911897TlJA.gif)
       如图2-3 所示，客户连接向调度器发送一个请求。调度器首先将这个请求放在SGA 中的请求队列中①。第一个可用的共享服务器从请求队列中取出这个请求②并处理。共享服务器的处理结束后，再把响应（返回码、数据等）放到响应队列中③，接下来调度器拿到这个响应④，传回给客户。
### 2.2.2 共享服务器具有以下一些缺点
       一般我们以oracle默认的专用服务器方式就行了，没必要使用共享服务器模式。一个是我们是使用中间件（如：weblogic）去连oracle的，中间件本身有连接池机制，另外就是oracle的这个共享服务器方式也做的不够好了，有诸多缺点。
       1)共享服务器的代码路径比专用服务器长，所以它天生就比专用服务器慢。
       2)存在人为死锁的可能，因为它是串行的，只要一个连接阻塞，则该服务器进程上的所有用户都被阻塞，并且极可能死锁。
       3)存在独占事务的可能，因为如果一个会话的事务运行时间过长，它独占共享资源，其它用户只能等待，而专用服务器，每个客户端是一个会话。
       4)共享服务器模式限制了某些数据库特性，例如：不能单独启动和关闭实例，不能进行介质恢复，不能使用Log Miner，并且SQL_TRACE没有意义(因为是共享而不是当前会话的)。
       MTS减少的内存实际上是专用服务器模式下每个用户连接到操作系统进程所需的内存，但它却使用SGA的Large_Pool来分配UGA，拆东墙补西墙，所减少的内存是很少的。如果用户会话的连接和断开很频繁，数据库进程的创建和删除的开销会非常大，这种情况最好采用共享服务器模式(否则，应该使用连接池技术)。如果客户端一次连接终身使用(会话生命周期内)，使用共享服务器模式的意义不大。因为大部分时间，一个会话就连接到一个服务器进程，无法共享服务器进程。
### 2.2.3 共享服务初始化参数的一些说明
shared_servers ：指定了当instance 启动的时候 shared server process 启动的数量，不要将这个参数设置得太大，否者启动数据库instance 的时候 就会花更多时间,Oracle启动过后会根据负载来动态调整shared_servers。如果为0，表示数据库没有启动共享服务模式。 这个参数是配置shared server 必须的，而且只有这个参数是必须的。
       修改参数：  alter system set shared_servers=1;
max_shared_servers：ORACLE在同一个时刻最大能够使用的 shared server process.不要将这个参数设置小于 shared_servers,如果动态修改shared_servers大于max_shared_servers，ORACLE会覆盖max_shared_servers的值，此时你需要修改max_shared_servers.同时也不能大于processes。这个参数是为了给占用很大资源操作而设的(批处理)，为了预留一些process
 给DBA任务(rman备份)，
shared_server_sesions: 指定了总共允许的的shared server session 的数量。如果设置了这个参数，那么就不要将这个值超过sessions，如果没有设置这个值，那么只要还有空闲的session,就可以被使用。设置这个值是为专有连接预留 user sessions.
dispatchers：配置 dispatcher process .如果不设置这个参数，只要设置了shared_servers ，oracle 也会自动设置一个基于tcp协议的dispatcher。还需要查看操作系统支持一个dispatcher能处理多少个connections
   SQL> select * from v$dispatcher;
max_dispatchers: 设置同一时刻能够同时运行的dispatchers的数量，必须大于等于 dispatchers ，小于processes。这个参数也会被dispatchers覆盖。
circuits：指定了virtual circuits 的总数量。
### 2.2.4关闭共享模式
       将shared_servers参数置为0，那么所有以共享方式连接到数据库都不能成功，但是未释放的共享连接会继续保持连接，直到断开。如果将shared_servers 和max_shared_servers都设为0，那么共享连接将被终结。所有的共享方式连接都断开了的话，就可以使用alter system set dispatcher=’’; 将dispatcher清除，防止下次启动数据库又打开了共享连接方式。
### 2.3 TCP/IP 连接的基本原理
       这里将分析网络上最常见的一种情形：在TCP/IP 连接上建立一个基于网络的连接请求。在这种情况下，客户在一台机器上，而服务器驻留在另一台机器上，这两台机器通过一个TCP/IP 网络连接。客户率先行动，使用Oracle 客户软件（Oracle 提供的一组应用程序接口，或API）建立一个请求，力图连接数据库。
例如，客户可以发出以下命令：
C:\Users\Administrator.DavidDai>sqlplus sys/oracle@dave2_202as sysdba;
SQL*Plus: Release 11.2.0.1.0 Production onSun Aug 21 14:36:47 2011
Copyright (c) 1982, 2010, Oracle.  All rights reserved.
Connected to:
Oracle Database 10g Enterprise EditionRelease 10.2.0.1.0 - Production
With the Partitioning, OLAP and Data Miningoptions
SQL>
       这里，客户是程序SQL*Plus，dave2_202是一个TNS 服务名。TNS 代表透明网络底层（Transparent Network Substrate），这是Oracle 客户中处理远程连接的“基础”软件，有了它才有可能建立对等通信。TNS 连接串告诉Oracle 软件如何与远程数据库连接。
       一般地，机器上运行的客户软件会读取一个tnsnames.ora 文件。这是一个纯文本的配置文件，通常放在[ORACLE_HOME]\network\admin目录下。配置如下：
dave2_202 =
 (DESCRIPTION =
   (ADDRESS_LIST =
     (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.3.202)(PORT = 1521))
    )
   (CONNECT_DATA =
     (SERVER = DEDICATED)
     (SERVICE_NAME = dave2)
    )
  )
       根据这个配置信息，Oracle 客户软件可以把我们使用的TNS 连接串dave2_202映射到某些有用的信息，也就是主机名、该主机上“监听器”进程接受（监听）连接的端口、该主机上所连接数据库的服务名，等等。
       服务名表示具有公共属性、服务级阈值和优先级的应用组。提供服务的实例数量对应用是透明的，每个数据库实例可以向监听器注册，表示要提供多个服务。  所以，服务就映射到物理的数据库实例，并允许DBA 为之关联阈值和优先级。这个串（dave2_202）还可以用其他方式来解析。例如，可以使用Oracle Internet 目录（Oracle Internet Directory，OID），这是一个分布式轻量级目录访问协议（Lightweight Directory Access Protocol，LDAP）服务器，其作用就相当于解析主机名的DNS。不过，tnsnames.ora文件通常只适用于大多数小到中型安装，在这些情况下，这个配置文件的副本不算太多，尚可管理。
       既然客户软件知道要连接到哪里，它会与主机名为192.168.3.202 的服务器在端口1521 上打开一条TCP/IP socket 连接。如果服务器DBA 安装并配置了Oracle Net，并且有一个监听器在端口1521上监听连接请求，就会收到这个连接。 在网络环境中，我们会在服务器上运行一个称为TNS 监听器的进程。就是这个监听器进程能让我们与数据库物理连接。当它收到入站连接请求时，它会使用自己的配置文件检查这个请求，可能会拒绝请求（例如，因为没有这样的数据库，或者可能我们的IP 地址受到限制，不允许连接这个主机），也可能会接受请求，并真正建立连接。
      如果建立一条专用服务器连接，监听器进程就会为我们创建一个专用服务器。在UNIX上，这是通过fork()和exec()系统调用做到的（在UNIX 中，要在初始化之后创建新进程，惟一的办法就是通过fork()）。      这个新的专用服务器进程继承了监听器建立的连接，现在就与数据库物理地连接上了。
       在Windows 上，监听器进程请求数据库进程为连接创建一个新线程。一旦创建了这个线程，客户就会“重定向”到该线程，相应地就能建立物理连接。图2-4 显示了UNIX 上的监听器进程和专用服务器连接。
### ![](http://hi.csdn.net/attachment/201108/21/0_1313911990uhJF.gif)
       另一方面，如果我们发出共享服务器连接请求，监听器的表现则会有所不同。监听器进程知道实例中运行了哪些调度器。接收到连接请求后，监听器会从可用的调度器池中选择一个调度器进程。监听器会向客户返回连接信息，其中说明了客户如何与调度器进程连接；如果可能的话，还可以把连接“转发”给调度器进程（这依赖于不同的操作系统和数据库版本，不过实际效果是一样的）。
       监听器发回连接信息后，它的工作就结束了，因为监听器一直在特定主机的特定端口上运行（主机名和端口号大家都知道），而调度器会在服务器上随意指派的端口上接受连接。监听器要知道调度器指定的这些随机端口号，并为我们选
择一个调度器。客户再与监听器断开连接，并与调度器直接连接。现在就与数据库有了一个物理连接。这个过程如图2-5 所示。
![](http://hi.csdn.net/attachment/201108/21/0_1313912011S582.gif)
## 三. 判断oracle是共享模式还是专用模式的方法
### 3.1. showparameter shared_server;
 (注：8i应为：showparameter mts_servers;)
SQL> show parameter shared_server;
NAME                    TYPE       VALUE
------------------------------------ ----------- ------------------------------
max_shared_servers           integer     20
shared_server_sessions        integer     330
shared_servers               integer     1
### 3.2 查看v$session 视图
SQL> Select username,server,program fromv$session where username is not null;
USERNAME          SERVER    PROGRAM
------------------------------ ------------------------------------------------
SYS                  DEDICATED rman.exe
RMAN                DEDICATED rman.exe
SYS                  DEDICATED rman.exe
SYS                  DEDICATED sqlplus.exe
SYS                  DEDICATED rman.exe
DAVE                DEDICATED toad.exe
SYS                  DEDICATED toad.exe
### 3.3 查看监听： lsnrctl service
C:\Users\Administrator.DavidDai>lsnrctlservice
LSNRCTL for 32-bit Windows: Version11.2.0.1.0 - Production on 26-6月 -2010 11:7:40 Copyright (c) 1991, 2010, Oracle.  All rights reserved.
正在连接到(DESCRIPTION=(ADDRESS=(PROTOCOL=IPC)(KEY=EXTPROC1521)))
服务摘要..
服务"orcl" 包含 1 个实例。
  实例 "orcl", 状态 READY, 包含此服务的 1 个处理程序...
    处理程序:
      "DEDICATED" 已建立:34 已拒绝:0 状态:ready
        LOCAL SERVER
服务"orclXDB" 包含 1 个实例。
  实例 "orcl", 状态 READY, 包含此服务的 1 个处理程序...
    处理程序:
     "D000" 已建立:0 已被拒绝:0 当前: 0 最大: 1022 状态: ready
        DISPATCHER <machine: DAVIDDAI, pid:10884>
        (ADDRESS=(PROTOCOL=tcp)(HOST=DavidDai)(PORT=58400))
命令执行成功
### 3.4.查看TNSNAMES.ora 文件。如：
ORCL =
 (DESCRIPTION =
   (ADDRESS_LIST =
     (ADDRESS = (PROTOCOL = TCP)(HOST = DavidDai)(PORT = 1521))
    )
   (CONNECT_DATA =
      (SERVER = DEDICATED)
     (SERVICE_NAME = orcl)
    )
  )
       这里是以DEDICATED 专用模式连接 orcl 实例。写上 (SERVER = SHARED) 则是使用共享服务器模式，但是这时shared_server_process需要打开，要不然会出错连不上oracle。要是这段放空没写，那么系统会根据服务器模式自动调节，不过根据实测结果，就算服务器是定义成共享服务器模式，shared_server_process没打开的情况下，在v$session中查到的连接依然是SERVER = DEDICATED。
       所以基本上我们这段话我们都是可以放空着不写的，但是有时候要连上我们的共享服务器模式的数据库，放空有可能系统认为要用共享服务器方式去连，那时就要声明 SERVER = DEDICATED采用专用服务器方式去连接。
       在数据库启动的时候，如果没有指定shared_servers,但是设置了dispatchers，那么ORACLE就认为启动了shared server ,并且设置shared_servers为1. 在数据库启动的时候，没有设置shared_servers,没有设置dispatchers，即使以后修改了dispatchers，也不能启动shared server，必须从新启动数据库。
       另外，Background process ,以及通过本地连接进来的，只能是DEDICATED .比如说sqlplus user/pass形式。如果数据库没有配置共享服务器，那么客户端只能以DEDICATED方式连接数据库.

-------------------------------------------------------------------------------------------------------
Blog： http://blog.csdn.net/tianlesoftware
Weibo: http://weibo.com/tianlesoftware
Email: dvd.dba@gmail.com
DBA1 群：62697716(满);   DBA2 群：62697977(满)  DBA3 群：62697850(满)  
DBA 超级群：63306533(满);  DBA4 群： 83829929(满) DBA5群： 142216823(满) 
DBA6 群：158654907(满)  聊天 群：40132017(满)   聊天2群：69087192(满)
--加群需要在备注说明Oracle表空间和数据文件的关系，否则拒绝申请
