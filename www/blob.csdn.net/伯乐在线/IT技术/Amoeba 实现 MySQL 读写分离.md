# Amoeba 实现 MySQL 读写分离 - 文章 - 伯乐在线
原文出处： [杨一](http://bestvivi.com/2015/09/06/Amobe实现MySQL读写分离/)
# Amoeba 介绍
[Amoeba(变形虫)](http://docs.hexnova.com/amoeba/index.html)项目，该开源框架于2008年 开始发布一款 Amoeba for Mysql软件。
这个软件基于Java致力于**MySQL的分布式数据库前端代理层**，处于在应用和数据库之间，对客户端透明，它主要在应用层访问MySQL的时候充当**SQL路由功能**，解析应用传递过来的SQL语句，专注于分布式数据库代理层（Database Proxy）开发。具有负载均衡、高可用性、SQL 过滤、读写分离、可路由相关的到目标数据库、可并发请求多台数据库合并结果。 通过Amoeba能够完成多数据源的高可用、负载均衡、数据切片的功能，目前Amoeba已在很多企业的生产线上面使用。
优点：Amoeba已经具有Query路由，Query过滤，读写分离，负载均衡以及HA机制等相关内容。
缺点：Amoeba暂不支持事务；amoeba不支持跨库join和排序；amoeba不支持分库分表等；
Amoeba 主要解决的以下几个问题：
- 数据切分后复杂数据源整合；
- 提供数据切分规则并降低数据切分规则给数据库带来的影响；
- 降低数据库与客户端的连接数；
- 读写分离路由；
### Amoeba 实现读写分离配置
安装环境如下
master 192.168.1.5 rac1-node.tp-link.net
slave 192.168.1.6 rac2-node.tp-link.net
Amoeba 192.168.1.2 poprodbak.tp-link.net
安装amoeba前要安装jdk，因为amoeba是由java开发的。同时设置JAVA_HOME
**下载 ameba 软件**
使用[amoeba稳定版](http://sourceforge.net/projects/amoeba/files/Amoeba%20for%20mysql/2.2.x/)


```
# mkdir amoeba 
# cd amoeba
# tar -zxvf amoeba-mysql-binary-2.2.0.tar.gz
```
**配置 Amoeba**
解压完以后，下面进行配置。在conf目录下可以看到很多.xml后缀的文件。其中
- amoeba.xml是对amoeba代理的配置，定义读写分离的节点管理信息；
- dbServers.xml是对后端service的配置，定义数据库的信息；
- rule.xml可以定义高级设置比如数据的水平、垂直切分等；
- log4j.xml定义日志等。
配置dbServers.xml


```
<!--  
                        Each dbServer needs to be configured into a Pool, 
每个dbServer需要配置一个pool,如果多台平等的mysql需要进行loadBalance,平台已经提供一个具有负载均衡能力的objectPool:    简单的配置是属性加上virtual="true",该Pool不允许配置factoryConfig或者自己写一个ObjectPool
                         such as 'multiPool' dbServer    
                --> 
        <dbServer name="abstractServer" abstractive="true"> 
                <factoryConfig class="com.meidusa.amoeba.mysql.net.MysqlServerConnectionFactory"> 
                        <property name="manager">${defaultManager}</property> 
                        <property name="sendBufferSize">64</property> 
                        <property name="receiveBufferSize">128</property> 
                        <!-- mysql port --> 
                        <property name="port">3306</property> 
                                  \\这个是后端数据的端口
                        <!-- mysql schema --> 
                        <property name="schema">test</property> 
                                    \\这个是后端默认的数据库
                        <!-- mysql user --> 
                        <property name="user">root</property> 
                        <!--  mysql password 
                        <property name="password">password</property> 
                        --> 
                </factoryConfig> 
        </dbServer> 
 \\下面的配置是定义一个主节点和一个从节点。
        <dbServer name="master"  parent="abstractServer">   \\定义一个主节点
                <factoryConfig> 
                        <!-- mysql ip --> 
                        <property name="ipAddress">192.168.1.5</property> 
                        <property name="user">root</property>   \\连接数据库的用户名
                        <property name="password">root</property>  \\连接数据库的密码，此处如果写库跟读库密码一致，可以写在上面公共段，这里会继承上面的设置
                </factoryConfig> 
        </dbServer> 
        <dbServer name="slave"  parent="abstractServer">  \\定义一个从节点
                <factoryConfig> 
                        <!-- mysql ip --> 
                        <property name="ipAddress">192.168.1.6</property> 
                        <property name="user">root</property> 
                        <property name="password">root</property> 
                </factoryConfig> 
        </dbServer> 
             \\定义池，把master和slave加入
        <dbServer name="server1" virtual="true">  \\server1是要把master节点加入
                <poolConfig class="com.meidusa.amoeba.server.MultipleServerPool"> 
                        <!-- Load balancing strategy: 1=ROUNDROBIN , 2=WEIGHTBASED , 3=HA-->                               < !  -- 负载均衡参数1=ROUNDROBIN , 2=WEIGHTBASED , 3=HA-->
                        <property name="loadbalance">1</property> 
                        <!-- Separated by commas,such as: server1,server2,server1 --> 
                        <property name="poolNames">master</property>  <!--
参与该pool负载均衡的poolName列表以逗号分割 这里只一个主节点所以就一个
 -->
                </poolConfig> 
        </dbServer> 
        <dbServer name="readPool" virtual="true">
                <poolConfig class="com.meidusa.amoeba.server.MultipleServerPool">
                        <!-- Load balancing strategy: 1=ROUNDROBIN , 2=WEIGHTBASED , 3=HA-->
                        <property name="loadbalance">1</property>
                        <!-- Separated by commas,such as: server1,server2,server1 -->
                        <property name="poolNames">slave,slave,slave,master</property>
                </poolConfig>
        </dbServer> 
</amoeba:dbServers>
```
配置Amoeba.xml


```
<!-- port --> 
                        <property name="port">8066</property> 
                        \\定义amoeba读写分离proxy对外代理的端口
                        <!-- bind ipAddress --> 
                        <!--  
                        <property name="ipAddress">127.0.0.1</property> \\这个是绑定端口的ip，注释掉了，说明8066端口绑定在0.0.0.0/0.0.0.0 上面          --> 
                         <property name="manager">${clientConnectioneManager}</property> 
                        <property name="authenticator"> 
                                <bean class="com.meidusa.amoeba.mysql.server.MysqlClientAuthenticator"> 
                                        <property name="user">root</property> 
                                                        \\定义通过amoeba登录的用户名       
                                        <property name="password">root</property> 
                                                         \\相应的这个就是密码了.定义proxy的管理帐号密码，客户端和程序只需要连接proxy的帐号密码即可，相当于中间件封装
                <property name="LRUMapSize">1500</property> 
                <property name="defaultPool">master </property> 
 \\定义默认的池，一些除了SELECT\UPDATE\INSERT\DELETE的语句都会在defaultPool执行
                <property name="writePool">master</property>
\\定义写的池，这里的master就是在dbServer.xml中的master
                <property name="readPool"> readPool </property> 
                   \\定义读的池，这里的readPool就是在dbserver.xml中的readPool
                <property name="needParse">true</property>
```
**启动 Amoeba**


```
# ./amoeba start &
[1] 2381
# netstat -tlnp | grep 8066
tcp        0      0 :::8066                     :::*                        LISTEN      2381/java
```
以上说明amoeb已经正常启动了。


```
# mysql -h 192.168.1.2 -ulibai --port 8066 –p
```
连接报错
java.util.NoSuchElementException: Could not create a validated object, cause: ValidateObject failed
修改dbServers.xml配置，factoryConfig中 password默认是被注释掉的，取消注释
还是报错，后来发现原来是schema设置不对，这个是要设置为数据库名。修改后通过amoeba连接数据库正常。
至此，amoeba正常启动，客户端可以通过amoeba正常连接MySQL数据库。
**验证测试**
使用tcpdump抓包工具分析，读写是否分离到master和slave上。
在master和slave上分别执行


```
# tcpdump -i eth2 -s 0 -A -n -p port 3306 and src 192.168.1.2 | grep -i 'SELECT\|INSERT'
```
其中192.168.1.2是amoeba服务器
**在amoeba服务器上执行select操作**
slave上可以抓取到如下信息，查询三次以后，可以从master抓取到查询信息（因为此时master和slave节点数据刚好不一致，可以非常清晰地确实是在master上执行的）。因此可以大概验证amoeba设置的读操作slave和master 3比1的比例。


```
...p(Y.......select * from t
13:59:36.355619 IP 192.168.1.2.24489 > 192.168.1.6.mysql: Flags [.], ack 869, win 96, options [nop,nop,TS val 85451120 ecr 676978448], length 0
```
**在amoeba服务器上执行insert操作**
master抓包如下信息，slave上没有任何变化。因此可以验证写操作完全在master节点上完成。


```
14:06:47.578469 IP 192.168.1.2.58575 > 192.168.1.5.mysql: Flags [P.], seq 68:90, ack 94, win 46, options [nop,nop,TS val 85882307 ecr 677373872], length 22
E..JF.@.@.p|............>K....AM...........
..u.(_.......set names latin1.
14:06:47.578830 IP 192.168.1.2.58575 > 192.168.1.5.mysql: Flags [.], ack 105, win 46, options [nop,nop,TS val 85882308 ecr 677413229], length 0
E..4F.@.@.p.............>K.+..AX.....d.....
..u.(`.m
14:06:47.579712 IP 192.168.1.2.58575 > 192.168.1.5.mysql: Flags [P.], seq 90:135, ack 105, win 46, options [nop,nop,TS val 85882309 ecr 677413229], length 45
E..aF.@.@.pc............>K.+..AX...........
..u.(`.m)....insert into t values(5,'this_is_amoeba')
14:06:47.622539 IP 192.168.1.2.58575 > 192.168.1.5.mysql: Flags [.], ack 116, win 46, options [nop,nop,TS val 85882352 ecr 677413233], length 0
E..4F.@.@.p.............>K.X..Ac...........
..u.(`.q
```
**性能对比**
针对MySQL Proxy和Amoeba进行了简单的性能对比测试
分别查询14W条数据，proxy和amoeba表现几无差别。
第一次(s) 第二次 3 4 5 平均(s)
Mysql-proxy 0.27 0.27 0.28 0.29 0.26 0.274
Amoeba 0.23 0.26 0.33 0.25 0.34 0.282
分别插入14W条数据，proxy平均为1.8S，amoeba平均为1.95S。
可以看出mysqlproxy和Amoeba几无差别，可能是因为数据量比较小。
