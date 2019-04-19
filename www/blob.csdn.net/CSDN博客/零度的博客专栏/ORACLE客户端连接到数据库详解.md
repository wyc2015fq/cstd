# ORACLE客户端连接到数据库详解 - 零度的博客专栏 - CSDN博客
2019年04月17日 11:07:29[零度anngle](https://me.csdn.net/zmx729618)阅读数：40标签：[oracle连接](https://so.csdn.net/so/search/s.do?q=oracle连接&t=blog)
个人分类：[数据库](https://blog.csdn.net/zmx729618/article/category/3108441)
原文出处：[https://www.cnblogs.com/andy6/p/5877327.html](https://www.cnblogs.com/andy6/p/5877327.html)
Oracle 客户端连接到Oracle 数据库服务器貌似不同于SQL serve中的网络配置，其实不然，只不过所有的SQL server 运行于Windows平台，故很多配置直接集成到了操作系统之中。所以无需配置客户端即可连接到服务器。Oracle 客户端连接到数据库依赖于Oracle Net。Oracle提供了很多基于客户端或服务器的配置工具，需要搞清的是Oracle Net 中的相关术语。术语明白了，其实配置方法与SQL server 大同小异。
一、什么是Oracle Net
    Oracle Net 用于客户端和服务器之间创建一个连接会话，并负责维护该会话。
    Oracle Net 在客户端作为应用程序的一个后台进程组件，而在服务器端则包含称为侦听器的活动进程，
负责客户端和服务器之间的交互。
    Oracle Net 同时也支持异构数据库的连接，如连接到Sybase，Informix，DB2，SQL Server等
    通常基于下列配置来实现
        网络配置(网络必须是联通的)
        节点所在的位置(IP/Hostname)
        应用程序
        所使用的协议(TCP/IP、/TCP/IP with SSL、SDP、Named Pipes)
    Oracle Net支持的连接类型
        客户端-服务器模式(如SQLPlus)
        Java 应用程序(JDBC等)
        Web 客户端应用程序
使用基于Web的应用程序(App Server)作为中间件来实现，可以配置JDBC Oracle Call Interface (OCI) driver 或thin JDBC driver
            通过HTTP直接连接到Oracle 服务器，如OEM
    Oracle Net实现机制
        用户发出连接请求实现与用户进程进行交互
        用户进程通过Oracle Net 来与服务器进程交互
        服务器进程则与实例进行交互(由Oracle Net维护)
        实例通过后台进程交互来完成数据库的读写操作
    Oracle Net 连接方式
        本地客户端连接的数据库(即客户端与数据库位于同一台服务器)。
            本地连接同样使用到了Oracle Net，因为任何与数据的交互都通过Oracle Net来完成。
不需要使用到Listener，因为本地连接使用的是IPC协议，而IPC协议允许主机内进程间相互通信，由操作系统提供。
            本地连接不需要进行任何配置，且本地服务器上可以运行若干个实例。
            本地连接是唯一一个不需要侦听器的连接类型。
            连接方法：CONNECT username/password
        所有非本地客户端连接到数据库
            服务器端启用Listener进程
            客户端通过TNSNAME或Easy connect或LDAP等名称解析方式来解析连接字符串
            一旦连接成功，即是Listener服务停止，不影响该会话。
    Oracle Net Listener(侦听器)
        运行于服务器端，用于侦听所有来自客户端的连接请求，并提供处理数据库服务方面的请求
            位置：$ORACLE_HOME/network/admin/listerer.ora
 --对应于netmgr中(GUI界面)的Listeners项下建立的缺省侦听器(Listener)
                  $ORACLE_HOME/network/admin/sqlnet.ora  
 --对应于netmgr中(GUI界面)的Profile
        服务器端配置tnsnames
            位置：ORACLE_HOME/network/admin/tnsnames.ora
--对应于netmgr中(GUI界面)的Service Naming
        Listener提供的信息包括，IP地址、端口号、协议、服务名(service_name)等
        一个Listener能够为多个数据库实例提供服务
二、客户端到服务器建立会话的过程及相关术语
    执行如下的命令来建立连接
        SQL> CONNECT username/password@net_service_name  
        连接字符串(connect string)：username/password@net_service_name
包含了用户名、密码、连接标识符
            如：SQL> conn scott/tiger@orcl
            字符串分解
                用户名scott
                密码为tiger
                "/"    用于分割用户与密码
                "@"   指示网络连接所需的用户进程
                orcl  连接标识符
        连接标识符(connect identifier)
            @后面的即为连接标识符，如上例中的orcl。
            连接标识符的名字是一个映射到连接描述符的简化
            连接标识符可以为网络服务名/别名、数据库服务名等
(net service name, database service name, or net service alias.)
        连接描述符(connect discriptor)
            使用一种特殊格式来描述连接的具体信息，信息包含了、数据库所在的位置(IP地址)、
服务名、端口号等
            如tnsnames中的：
            orcl =  --网络服务名,数据库别名,等同于连接标识符(屏蔽客户端如何连接到服务器端的细节)
              (DESCRIPTION =           
                (ADDRESS_LIST =
                  (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
--IP地址,端口号,协议
                )
                (CONNECT_DATA =
                  (SERVICE_NAME = orcl.robinson.com)  --服务名
                )
              )
            以上所有DESCRIPTION内的信息即为连接描述信息
            描述符之上等于号左边的可以为网络服务名，网络服务别名等，此等同于连接标识符。
        服务名(service_name)
客户端连接到实例的服务名，可以为该参数指定一个或多个服务名。该参数从9i引入，service_name通常可以使用SID代替。
9i之后，Oracle推荐使用service_name而不是SID。可以通过定义多不不同的服务名来区分不同的用户连接，该参数缺省的格式为db_name.domain_name。
实例将所定义的服务名注册到侦听器，当客户端请求服务时，侦听器根据服务名决定将使用哪个实例提供所请求的服务并与之建立连接
            基于相同的数据库可以定义多不不同的服务名来区分不同的使用情况
            如定义service_name为
                service_names = sales.robinson.com,hr.robinsn.com
            则sales部门通过sales服务名来建立连接，hr部门通过hr服务名来建立连接，
而无须关心连接的是哪一个数据库。
        演示连接过程(非本地连接)
            环境：服务器为RHEL 5 + Oracle 10g ，客户端Winxp + 10g Client
            查看服务器listener是否启动
            [oracle@robinson admin]$ lsnrctl   --启动lsnrctl 侦听器程序
            LSNRCTL for Linux: Version 10.2.0.1.0 - Production on 03-SEP-2010 13:31:06
            Copyright (c) 1991, 2005, Oracle.  All rights reserved.
            Welcome to LSNRCTL, type "help" for information.
            LSNRCTL> status      --查看侦听器状态，侦听器没有启动
            Connecting to (ADDRESS=(PROTOCOL=tcp)(HOST=)(PORT=1521))
            TNS-12541: TNS:no listener
             TNS-12560: TNS:protocol adapter error
              TNS-00511: No listener
               Linux Error: 111: Connection refused
            --启动缺省的侦听器
            --在下面列出的信息中没有侦听器的路径及listener.ora文件，此处listener.ora 不存在
            LSNRCTL> start    
            Starting /u01/app/oracle/10g/bin/tnslsnr: please wait...
            TNSLSNR for Linux: Version 10.2.0.1.0 - Production
            Log messages written to /u01/app/oracle/10g/network/log/listener.log
            Listening on: (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=robinson.com)(PORT=1521)))
            Connecting to (ADDRESS=(PROTOCOL=tcp)(HOST=)(PORT=1521))
            STATUS of the LISTENER
            ------------------------
            Alias                     LISTENER
            Version                   TNSLSNR for Linux: Version 10.2.0.1.0 - Production
            Start Date                03-SEP-2010 13:31:14
            Uptime                    0 days 0 hr. 0 min. 0 sec
            Trace Level               off
            Security                  ON: Local OS Authentication
            SNMP                      OFF
            Listener Log File         /u01/app/oracle/10g/network/log/listener.log
            Listening Endpoints Summary...
              (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=robinson.com)(PORT=1521)))
            The listener supports no services
            The command completed successfully
            LSNRCTL> service  --已经看到了已注册的实例信息，因为10g支持动态注册
            Connecting to (ADDRESS=(PROTOCOL=tcp)(HOST=)(PORT=1521))
            Services Summary...
            Service "orcl.robinson.com" has 1 instance(s).
              Instance "orcl", status READY, has 1 handler(s) for this service...
                Handler(s):
                  "DEDICATED" established:0 refused:0 state:ready
                     LOCAL SERVER
            Service "orclXDB.robinson.com" has 1 instance(s).
              Instance "orcl", status READY, has 1 handler(s) for this service...
                Handler(s):
                  "D000" established:0 refused:0 current:0 max:1022 state:ready
                     DISPATCHER <machine: robinson, pid: 3246>
                     (ADDRESS=(PROTOCOL=tcp)(HOST=robinson.com)(PORT=51473))
            Service "orcl_XPT.robinson.com" has 1 instance(s).
              Instance "orcl", status READY, has 1 handler(s) for this service...
                Handler(s):
                  "DEDICATED" established:0 refused:0 state:ready
                     LOCAL SERVER
            The command completed successfully 
            --查看客户端tnsnames.ora的信息
            C:/>type F:/oracle/product/10.2.0/client_1/NETWORK/ADMIN/tnsnames.ora
            # tnsnames.ora Network Configuration File: F:/oracle/product/10.2.0/client_1/NET
            WORK/ADMIN/tnsnames.ora
            # Generated by Oracle configuration tools.
            orcl =
              (DESCRIPTION =
                (ADDRESS_LIST =
                  (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                )
                (CONNECT_DATA =
                  (SERVICE_NAME = orcl.robinson.com)
                )
              )
            C:/>sqlplus /nolog
            SQL*Plus: Release 10.2.0.1.0 - Production on 星期五9月3 13:42:02 2010
            Copyright (c) 1982, 2005, Oracle.  All rights reserved.
            SQL> conn scott/tiger@orcl
            Connected.
            --将tnsnames.ora中服务别名改为oral后再连接，可以正常连接，即网络服务名可以随便取。
            SQL> conn system/redhat@oral
            Connected.
            --查看数据库中的服务名         
            SQL> show parameter service_names;
            NAME                                 TYPE        VALUE
            ------------------------------------ ----------- --------------------------
            service_names                        string      orcl.robinson.com
三、服务注册与名称解析         
    服务注册(service registration)
        实例将数据库将所提供的服务名及相关信息告知listener的过程称之为服务注册
        注册通常包含下列信息
            数据库的服务名
            实例名
            可用的服务处理程序(service handlers)，用于调度(共享模式)和派生子程序(专用模式)
            端口号等
        注册方法
            静态注册: 指将实例的相关信息手动告知listener侦听器，可以使用netmgr,netca,oem以及直接vi listener.ora文件来实现
                --下面是通过netmgr配置后产生的listener.ora文件的内容
                [oracle@robinson ~]$ cat $ORACLE_HOME/network/admin/listener.ora
                # listener.ora Network Configuration File: /u01/app/oracle/10g/network/admin/listener.ora
                # Generated by Oracle configuration tools.
                SID_LIST_LISTENER =       -->这里定义LISTENER进程提供的数据库服务列表
                  (SID_LIST =
                    (SID_DESC =
                      (GLOBAL_DBNAME = orcl.robinson.com)
                      (ORACLE_HOME = /u01/app/oracle/10g)
                      (SID_NAME = ORCL)
                    )
                  )
                LISTENER =               -->监听器的名字，一台数据库可以有多个监听器
                  (DESCRIPTION =
                    (ADDRESS = (PROTOCOL = TCP)(HOST = robinson)(PORT = 1521))
                  )
动态注册：实例在启动时使用PMON进程自动将instance_name和service_names等信息注册到已启动的缺省侦听器listener.ora,在9i之后,不需要任何配置即可实现动态注册.实例启动后会自动在本地主机默认的号端口上查找侦听器,并在查找到时进行注册
                可以在实例启动后的任何时间段执行下面命令实现重新注册
                    alter system register;
                    如果侦听器在非默认端口上运行，可以通过设置local_listener参数来制定侦听器所在的位置并手动进行注册
                    SQL> alter system set local_listener = 'list2';
                    SQL> alter system register;
                建议配置两个参数保证成功注册：service_names,instance_name
                    service_names缺省为global database name
                    alter system set service_names = 'testdb.robinson.com' scope =spfile;
                    alter system set instance_name = 'testdb' scope = spfile;
                --下面启动缺省的侦听器来实现动态注册
                [oracle@robinson ~]$ lsnrctl start
                LSNRCTL for Linux: Version 10.2.0.1.0 - Production on 03-SEP-2010 16:30:04
                Copyright (c) 1991, 2005, Oracle.  All rights reserved.
                Starting /u01/app/oracle/10g/bin/tnslsnr: please wait...
                TNSLSNR for Linux: Version 10.2.0.1.0 - Production
                System parameter file is /u01/app/oracle/10g/network/admin/listener.ora
                Log messages written to /u01/app/oracle/10g/network/log/listener.log
                Listening on: (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=robinson.com)(PORT=1521)))
                Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=robinson)(PORT=1521)))
                STATUS of the LISTENER
                -----------------------------部分信息省略---------------------------------------
                Listener Parameter File   /u01/app/oracle/10g/network/admin/listener.ora
                Listener Log File         /u01/app/oracle/10g/network/log/listener.log
                Listening Endpoints Summary...
                  (DESCRIPTION=(ADDRESS=(PROTOCOL=tcp)(HOST=robinson.com)(PORT=1521)))
                Services Summary...
                Service "orcl.robinson.com" has 1 instance(s).
--启动了orcl.robinson.com服务且为静态注册
                  Instance "ORCL", status UNKNOWN, has 1 handler(s) for this service...
                The command completed successfully
                [oracle@robinson ~]$ export ORACLE_SID=testdb  --设置ORACLE_SID并启动testdb
                [oracle@robinson ~]$ sqlplus /nolog
                SQL*Plus: Release 10.2.0.1.0 - Production on Fri Sep 3 16:33:00 2010
                Copyright (c) 1982, 2005, Oracle.  All rights reserved.
                SQL> conn / as sysdba
                Connected to an idle instance.
                SQL> startup
                ORACLE instance started.
                Total System Global Area  469762048 bytes
                Fixed Size                  1220048 bytes
                Variable Size             138412592 bytes
                Database Buffers          327155712 bytes
                Redo Buffers                2973696 bytes
                Database mounted.
                Database opened.
                SQL> ho lsnrctl status     --再次查看侦听器的状态
                                           --可以看到testdb已经被动态注册
                LSNRCTL for Linux: Version 10.2.0.1.0 - Production on 03-SEP-2010 16:34:06
                Copyright (c) 1991, 2005, Oracle.  All rights reserved.
                Connecting to (DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=robinson)(PORT=1521)))
                STATUS of the LISTENER
                -----------------------------部分信息省略---------------------------------------
                Services Summary...
                Service "orcl.robinson.com" has 2 instance(s).
                  Instance "ORCL", status UNKNOWN, has 1 handler(s) for this service...
                Service "orclXDB.robinson.com" has 1 instance(s).
                Service "orcl_XPT.robinson.com" has 1 instance(s).
                Service "testdb" has 1 instance(s).    --下面是和testdb相关的服务名，实例名
                  Instance "testdb", status READY, has 1 handler(s) for this service...
                Service "testdb.robinson.com" has 1 instance(s).
                  Instance "testdb", status READY, has 1 handler(s) for this service...
                Service "testdbXDB" has 1 instance(s).
                  Instance "testdb", status READY, has 1 handler(s) for this service...
                Service "testdb_XPT" has 1 instance(s).
                  Instance "testdb", status READY, has 1 handler(s) for this service...
                The command completed successfully             
                SQL> conn scott/tiger@192.168.1.128/testdb.robinson.com                                     Connected.  --动态注册testdb,Win客户端可以正常连接
                SQL> conn system/redhat@orcl         --原来静态注册的orcl也可以正常连接
                Connected.
                --注意服务名中有些状态为UNKNOWN，有些为READY
                --对于动态注册的服务名，因为监听器知道实例的状态，所以正常状态通常显示为READY。
                --对于静态注册的服务名，通常显示为UNKNOWN。
        当客户端的请求到达listener时，listener选择一个合适的service handler为之服务。
之后listener就不再参与相关处理。
        对于受限模式的实例，PMON阻塞所有到实例的连接。当客户端尝试连接将收到如下错误信息：
            ORA-12526：TNS：listener: all appropriate instances are in restricted mode
            ORA-12527: TNS：listener: all appropriate instances are in restricted mode or blocking new connections
            C:/Documents and Settings/Robinson Cheng>sqlplus scott/tiger@orcl
            SQL*Plus: Release 10.2.0.1.0 - Production on 星期三9月1 17:14:41 2010
            Copyright (c) 1982, 2005, Oracle.  All rights reserved.
            ERROR:
            ORA-12526: TNS:listener: all appropriate instances are in restricted mode
        动态注册时的几个常用参数以及需要$ORACLE_HOME目录
            SQL> select name,value,isdefault from v$parameter
              2  where name in ('instance_name','db_domain','service_names');
            NAME                      VALUE                          ISDEFAULT
            ------------------------- ------------------------------ ---------
            db_domain                 robinson.com                   FALSE
            instance_name             orcl                           FALSE
            service_names             orcl.robinson.com              FALSE
        更多关于动态注册的更多描述：[配置非默认端口的动态服务注册](http://blog.csdn.net/robinson_0612/archive/2010/10/07/5925526.aspx)
    名称解析方式(Naming Method)
        用于客户端连接到数据库服务时如何解析连接字符串。连接字符串形式：c:/sqlplus scott/tiger@orcl
        常用的字符串解析方法
            Easy connect
                缺省的连接方式为Easy connect ，仅仅限制于使用TCP协议，不支持负载均衡
                不支持connect-time failover、source routing、load balancing
                SQL> connect username/password@host[:port][/service_name]
                C:/>sqlplus scott/tiger@192.168.192.128:1521/orcl
            Local naming(本地名称解析)
                使用本地配置文件tnsnames.ora
                支持所有的Oracle Net 协议
                支持高级连接选项：connect-time failover、source routing、load balancing
                在服务器端缺省路径为$ORACLE_HOME/network/admin
                tnsnames.ora可以使用TNS_ADMIN环境变量设定到任意位置
            Directory naming(目录名称解析)
                目录名称解析技术使得解析实现集中化管理，用户会被指向一个解析别名的LDAP目录服务器
                支持所有的Oracle Net协议及高级选项
            External Naming Method(外部名称解析方式)
                第三方名称解析方法如SUN的NIS等。
四、常用的配置文件
    ldqp.ora         -->用于配置LDAP目录名称解析
    cman.ora         -->用于配置Oracle 连接管理参数
    tnsnames.ora     -->用于配置本地客户端或远程客户端的本地名称解析
    listener.ora     -->用于配置侦听器的相关注册信息
    sqlnet.ora       -->用于配置服务器或客户端所支持的解析方式
    netmgr(Net Manager)中的几个配置模块
        Profile 分支       -->对应于sqlnet.ora文件，可以设置客户端和服务器端同时应用的Oracle选项
        Sevice Naming 分支 -->用于配置客户端的名称解析，对应于tnsnames.ora文件
        Listener 分支      -->用于配置服务器端的数据库侦听器，对应于listener.ora文件
五、侦听器配置程序(lsnrctl) 及TNSPING命令  
    [oracle@robinson admin]$ lsnrctl
    LSNRCTL for Linux: Version 10.2.0.1.0 - Production on 03-SEP-2010 21:30:52
    Copyright (c) 1991, 2005, Oracle.  All rights reserved.
    Welcome to LSNRCTL, type "help" for information.
    LSNRCTL> help   --使用help 查看lsnrctl 的扩展命令
    The following operations are available
    An asterisk (*) denotes a modifier or extended command:
    start               stop                status    -->停止、启动、查看状态等(常用)         
    services            version             reload             
    save_config         trace               spawn              
    change_password     quit                exit                
    set*                show*                  
    lsnrctl命令用法
        LSNRCTL> command [listener_name]
    lsnrctl命令演示参照前面例子中的使用
    tnsping命令用于测试网络的可联通性
        C:/>tnsping orcl
        TNS Ping Utility for 32-bit Windows: Version 10.2.0.1.0 - Production on 03-9月-
        2010 21:52:45
        Copyright (c) 1997, 2005, Oracle.  All rights reserved.
        Used parameter files:
        F:/oracle/product/10.2.0/client_1/network/admin/sqlnet.ora
        Used TNSNAMES adapter to resolve the alias
        Attempting to contact (DESCRIPTION = (ADDRESS_LIST = (ADDRESS = (PROTOCOL = TCP)
        (HOST = 192.168.1.128)(PORT = 1521))) (CONNECT_DATA = (SERVICE_NAME = orcl.robin
        son.com)))
        OK (0 msec)
六、总结       
    1.客户端连接时的五个重要参数：用户名、密码、IP地址、端口号、服务名
    2.从客户端成功连接到数据库服务器
        客户端要求
            需要安装适当的客户端软件(Oracle Client)
            正确配置sqlnet.ora  
                NAMES.DIRECTORY_PATH = (TNSNAMES,EZCONNECT ….)      
                SQLNET.AUTHENTICATION_SERVICES = (NTS)   关于认证方式，请参考：[Oracle 密码文件](http://blog.csdn.net/robinson_0612/archive/2010/05/20/5611672.aspx)
                NAMES.DEFAULT_DOMAIN=robinson.com
--表示客户端在请求连接时将设定的缺省值追加网络服务名(net_service_name)之后
                    --如果db_domain为robinson.com,使用scott/tiger@orcl连接,则等同于使用scott@orcl.robinson.com进行连接
            正确配置tnsnames.ora
        服务器端要求
            listener已经启动
            数据库已经启动
    3.手动配置listener.ora和tnsnames.ora
        配置tnsname.ora文件：
            可用的配置工具
                oracle Net Configuration Assistant(GUI)       -->实际上修改tnsnames.ora文件
                oracle Net Manager(GUI)                       -->实际上修改tnsnames.ora文件
        给出下面的示例：
            orcl =                                           -->网络服务别名，可以修改，对应于连接标识符
              (DESCRIPTION =
                (ADDRESS_LIST =
                  (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                )
                (CONNECT_DATA =
                  (SERVICE_NAME = orcl.robinson.com)
                )
              )
            红色的内容为根据实际需要进行修改的内容，现描述如下：
            PROTOCOL：客户端与服务器端通讯的协议，一般为TCP，该内容一般不用改。
HOST：数据库侦听所在的主机的主机机名或IP地址，通常数据库侦听与数据库位于同一个机器上。建议使用IP地址。在UNIX或WINDOWS下可以使用hostname来获得主机的机器名或使用ifconfig(UNIX)、ipconfgi(Windows)来获得主机的IP地址。
            PORT：数据库侦听使用的端口，缺省为，可以查看服务器端的listener.ora文件获得端口号
                  或在数据库服务器命令提示符下使用nsrctl status [listener name]命令查看获得端口号。
                  此处Port的值一定要与数据库侦听正在侦听的端口一样。
            SERVICE_NAME：在服务器端，使用sqlplus> show parameter service_name命令查看。
            上述配置时应保证客户机能够ping通服务器，之后可以使用
tnsping orcl(网络服务名)来测试网络的联通情况
                SQL> conn scott/tiger@orcl
                Connected.
            使用上面的连接方式进行连接时，则连接标识符orcl被解释为tnsnames.ora中orcl对应
的描述信息,根据描述信息来解析连接服务器
        配置listener.ora  -->同样可以使用tnsnames.ora配置时所使用的GUI工具来完成
            给出下面的示例System parameter file is /u01/app/oracle/10g/network/admin/listener.ora
            LISTENER =
              (DESCRIPTION =
                (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
              )   --LISTENER部分描述了主机地址、端口及协议
            SID_LIST_LISTENER =
              (SID_LIST =
                (SID_DESC =
                  (GLOBAL_DBNAME = orcl1)
                  (ORACLE_HOME = /u01/app/oracle/10g)
                  (SID_NAME = ORCL)
                )
                (SID_DESC =
                  (GLOBAL_DBNAME = orcl2)
                  (ORACLE_HOME = /u01/app/oracle/10g)
                  (SID_NAME = ORCL)
                )
              )
            a. LISTENER部分描述了主机地址、端口及协议
            b. SID_LIST_LISTENER描述对外提供数据库服务的列表
               两个SID_DESC是对同一个SID数据库设置的两个service_names所设置的监听服务
            启动该Listener后的信息如下
                Services Summary...
                Service "orcl1" has 1 instance(s).
                  Instance "ORCL", status UNKNOWN, has 1 handler(s) for this service...
                Service "orcl2" has 1 instance(s).
                  Instance "ORCL", status UNKNOWN, has 1 handler(s) for this service...
                The command completed successfully
            配置客户端的tnsnames.ora如下
                orcl1 =
                  (DESCRIPTION =
                    (ADDRESS_LIST =
                      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                    )
                    (CONNECT_DATA =
                      (SERVICE_NAME = orcl1)
                    )
                  )
                orcl2 =
                  (DESCRIPTION =
                    (ADDRESS_LIST =
                      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                    )
                    (CONNECT_DATA =
                      (SERVICE_NAME = orcl2)
                    )
                  )
            从客户端进行连接     
                C:/>sqlplus /nolog
                SQL*Plus: Release 10.2.0.1.0 - Production on 星期六9月4 09:47:22 2010
                Copyright (c) 1982, 2005, Oracle.  All rights reserved.
                SQL> conn scott/tiger@orcl1
                Connected.
                SQL> conn scott/tiger@orcl2
- 
            注意：
                笔者首次连接时收到如下错误提示,将listener.ora中的SID_NAME的值改为小写,即orcl,问题解决
                所以尽可能的统一大小写。
                    SQL> conn scott/tiger@orcl1;    --下面是未统一大小写时的错误提示
                    ERROR:
                    ORA-01034: ORACLE not available
                    ORA-27101: shared memory realm does not exis
                    Linux Error: 2: No such file or directory
    4.关于GLOBAL_DBNAME
        下面是GLOBAL_DBNAME的官方描述
            GLOBAL_DBNAME
                Purpose        
    Use the parameter GLOBAL_DBNAME to identify the database service.
                While processing a client connection request, the listener tries to match the value of
                this parameter with the value of the SERVICE_NAME parameter in the client connect
                descriptor. If the client connect descriptor uses the SID parameter, then the listener
                does not attempt to map the values. This parameter is primarily intended for
                configurations with Oracle8 release 8.0 or Oracle7 databases (where dynamic service
                registration is not supported for dedicated servers). This parameter may also be
                required for use with Oracle9i and Oracle8 database services by some configurations
                and management tools.
                The value for this parameter is typically obtained from the combination of the DB_
                NAME and DB_DOMAIN parameters (DB_NAME.DB_DOMAIN) in the initialization
                parameter file, but the value can also contain any valid name used by clients to
                identify the service.
                Embed this parameter under the SID_DESC parameter.
        大致意思如下：
            当客户端发出连接请求时，监听器首先使用客户端连接描述符中的SERVICE_NAME相匹配。
            如果客户端连接描述符中使用的是SID,则不再尝试匹配该值,而是对监听器中设置的SID_NAME进行匹配
            该参数的值主要从初始化参数文件中的db_name和db_domian中组合得到。
        演示GOLBAL_DBNAME使用
            假定服务器listener.ora配置如下：
                SID_LIST_LISTENER =
                  (SID_LIST =
                    (SID_DESC =
                      (GLOBAL_DBNAME = orcl.robinson.com)
                      (ORACLE_HOME = /u01/app/oracle/10g)
                      (SID_NAME = orcl)
                    )
                  )
                LISTENER =
                  (DESCRIPTION =
                    (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                  )
            客户端的tnsnames.ora配置如下：
                orcl =
                  (DESCRIPTION =
                    (ADDRESS_LIST =
                      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                    )
                    (CONNECT_DATA =
                      (SERVICE_NAME = orcl.robinson.com)
                    )
                  )
            且spfileorcl中db_domain为空值
                [oracle@robinson ~]$ strings $ORACLE_HOME/dbs/spfileorcl.ora | grep db_domain
                *.db_domain=''
            数据库中的相关设置为空值
                SQL> col name format a20
                SQL> col value format a35
                SQL> select name,value,isdefault from v$parameter
                  2  where name in ('instance_name','db_domain','service_names');
                NAME                 VALUE                               ISDEFAULT
                -------------------- ----------------------------------- ---------
                db_domain                                                FALSE
                instance_name                                            FALSE
                service_names                                            FALSE 
            能够成功连接
                SQL> conn system/redhat@orcl
                Connected.
            ------------------------------------------------------------------------------------------
            修改客户端tnsnames.ora中的service_name项
                即去掉SERVICE_NAME = orcl.robinson.com这项内容且添加为SID = ORCL
            修改后的内容如下：
                ORCL =
                  (DESCRIPTION =
                    (ADDRESS_LIST =
                      (ADDRESS = (PROTOCOL = TCP)(HOST = 192.168.1.128)(PORT = 1521))
                    )
                    (CONNECT_DATA =
                      (SID = ORCL)
                    )
                  )
            SQL> conn scott/tiger@orcl   --能够成功连接
            Connected.
        关于GOLBAL_DBNAME总结：
当客户端使用的是SID连接参数，则匹配侦听器配置文件中的SID_NAME项    (主要用于兼容以前的老版本,8i或更早版本)
否则当使用的是SERVICE_NAME，则匹配侦听器配置文件中的SERVICE_NAME项 (现在多用SERVICE_NAME项)
