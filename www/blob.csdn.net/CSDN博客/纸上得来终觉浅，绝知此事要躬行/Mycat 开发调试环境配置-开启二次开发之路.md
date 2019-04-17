# Mycat 开发调试环境配置-开启二次开发之路 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2017年07月19日 13:32:03[boonya](https://me.csdn.net/boonya)阅读数：5486








Mycat是 数据库分库分表中间件。mycat1.6分库分表特性只能选择其一，不能同时存在，对于特殊应用场景既需要分库有需要分表，这就需要修改mycat的源码来实现了。今次我们来准备一下Mycat的开发调试环境。官网参考地址：[http://www.mycat.io/](http://www.mycat.io/)

## 前期准备

1、环境配置：需要准备jdk、maven、git、STS(eclipse)；这些就不多说了。


2、下载项目源码与配置mycat MySQL主备分库：

![](https://img-blog.csdn.net/20170719134117428)

mycat是master上安装的，local-mycat是本地代码启动服务连接。


![](https://img-blog.csdn.net/20170719134143106)

mycat项目已放至svn。


## 项目配置

mycat启动需要配置schema.xml、rule.xml、server.xml。

### 修改schema.xml



```
<?xml version="1.0"?>
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">
<mycat:schema xmlns:mycat="http://io.mycat/">

        <schema name="TESTDB" checkSQLschema="false" sqlMaxLimit="100">
                <table name="t_user"   primaryKey="u_id" autoIncrement="true"   dataNode="dn1" rule="mod-single" />

                <table name="t_service" primaryKey="s_id" autoIncrement="true"   dataNode="dn2"  rule="mod-single"/>

               <table name="t_location" primaryKey="F_ID" autoIncrement="true"  dataNode="dn1,dn2" rule="mod-long" />

        </schema>
        <!-- <dataNode name="dn1$0-743" dataHost="localhost1" database="db$0-743"
                /> -->
        <dataNode name="dn1" dataHost="localhost1" database="m" />
        <dataNode name="dn2" dataHost="localhost1" database="n" />

        <dataHost name="localhost1" maxCon="1000" minCon="10" balance="1"  writeType="0" dbType="mysql" dbDriver="native" switchType="1"  slaveThreshold="100">
                <heartbeat>show slave status</heartbeat>
                <!-- can have multi write hosts -->
                <writeHost host="hostM1" url="10.0.0.129:3306" user="root"   password="123456">
                        <!-- can have multi read hosts -->
                        <readHost host="hostS2" url="10.0.0.128:3306" user="root" password="123456" />
                </writeHost>

                <!-- <writeHost host="hostM2" url="localhost:3316" user="root" password="123456"/> -->
        </dataHost>

</mycat:schema>
```

配置好逻辑表和数据库节点。



### 修改rule.xml



```
<?xml version="1.0" encoding="UTF-8"?>
<!-- - - Licensed under the Apache License, Version 2.0 (the "License"); 
        - you may not use this file except in compliance with the License. - You 
        may obtain a copy of the License at - - http://www.apache.org/licenses/LICENSE-2.0 
        - - Unless required by applicable law or agreed to in writing, software - 
        distributed under the License is distributed on an "AS IS" BASIS, - WITHOUT 
        WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. - See the 
        License for the specific language governing permissions and - limitations 
        under the License. -->
<!DOCTYPE mycat:rule SYSTEM "rule.dtd">
<mycat:rule xmlns:mycat="http://io.mycat/">
        <tableRule name="rule1">
                <rule>
                        <columns>id</columns>
                        <algorithm>func1</algorithm>
                </rule>
        </tableRule>

        <tableRule name="rule2">
                <rule>
                        <columns>user_id</columns>
                        <algorithm>func1</algorithm>
                </rule>
        </tableRule>

        <tableRule name="sharding-by-intfile">
                <rule>
                        <columns>sharding_id</columns>
                        <algorithm>hash-int</algorithm>
                </rule>
        </tableRule>
        <tableRule name="auto-sharding-long">
                <rule>
                        <columns>id</columns>
                        <algorithm>rang-long</algorithm>
                </rule>
        </tableRule>
        <tableRule name="mod-long">
                <rule>
                        <columns>id</columns>
                        <algorithm>mod-long</algorithm>
                </rule>
        </tableRule>

       <tableRule name="mod-single">
                <rule>
                        <columns>id</columns>
                        <algorithm>mod-single</algorithm>
                </rule>
        </tableRule>

        <tableRule name="sharding-by-murmur">
                <rule>
                        <columns>id</columns>
                        <algorithm>murmur</algorithm>
                </rule>
        </tableRule>
        <tableRule name="crc32slot">
                <rule>
                        <columns>id</columns>
                        <algorithm>crc32slot</algorithm>
                </rule>
        </tableRule>
        <tableRule name="latest-month-calldate">
                <rule>
                        <columns>calldate</columns>
                        <algorithm>latestMonth</algorithm>
                </rule>
        </tableRule>

        <tableRule name="auto-sharding-rang-mod">
                <rule>
                        <columns>id</columns>
                        <algorithm>rang-mod</algorithm>
                </rule>
        </tableRule>

        <tableRule name="jch">
                <rule>
                        <columns>id</columns>
                        <algorithm>jump-consistent-hash</algorithm>
                </rule>
        </tableRule>

        <function name="murmur"
                class="io.mycat.route.function.PartitionByMurmurHash">
                <property name="seed">0</property><!-- 默认是0 -->
                <property name="count">2</property><!-- 要分片的数据库节点数量，必须指定，否则没法分片 -->
                <property name="virtualBucketTimes">160</property><!-- 一个实际的数据库节点被映射为这么多虚拟节点，默认是160倍，也就是虚拟节点数是物理节点数的160倍 -->
        </function>

        <function name="crc32slot"
                          class="io.mycat.route.function.PartitionByCRC32PreSlot">
                <property name="count">2</property><!-- 要分片的数据库节点数量，必须指定，否则没法分片 -->
        </function>
        <function name="hash-int"
                class="io.mycat.route.function.PartitionByFileMap">
                <property name="mapFile">partition-hash-int.txt</property>
        </function>
        <function name="rang-long"
                class="io.mycat.route.function.AutoPartitionByLong">
                <property name="mapFile">autopartition-long.txt</property>
        </function>
        <function name="mod-long" class="io.mycat.route.function.PartitionByMod">
                <!-- how many data nodes -->
                <property name="count">2</property>
        </function>
        <function name="mod-single" class="io.mycat.route.function.PartitionByMod">
                <!-- how many data nodes -->
                <property name="count">1</property>
        </function>


        <function name="func1" class="io.mycat.route.function.PartitionByLong">
                <property name="partitionCount">8</property>
                <property name="partitionLength">128</property>
        </function>
        <function name="latestMonth"
                class="io.mycat.route.function.LatestMonthPartion">
                <property name="splitOneDay">24</property>
        </function>
        <function name="partbymonth"
                class="io.mycat.route.function.PartitionByMonth">
                <property name="dateFormat">yyyy-MM-dd</property>
                <property name="sBeginDate">2015-01-01</property>
        </function>

        <function name="rang-mod" class="io.mycat.route.function.PartitionByRangeMod">
                <property name="mapFile">partition-range-mod.txt</property>
        </function>

        <function name="jump-consistent-hash" class="io.mycat.route.function.PartitionByJumpConsistentHash">
                <property name="totalBuckets">3</property>
        </function>
</mycat:rule>
```

上面用以配置分片规则。



### 修改server.xml



```
<?xml version="1.0" encoding="UTF-8"?>
<!-- - - Licensed under the Apache License, Version 2.0 (the "License"); 
	- you may not use this file except in compliance with the License. - You 
	may obtain a copy of the License at - - http://www.apache.org/licenses/LICENSE-2.0 
	- - Unless required by applicable law or agreed to in writing, software - 
	distributed under the License is distributed on an "AS IS" BASIS, - WITHOUT 
	WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. - See the 
	License for the specific language governing permissions and - limitations 
	under the License. -->
<!DOCTYPE mycat:server SYSTEM "server.dtd">
<mycat:server xmlns:mycat="http://io.mycat/">
	<system>
	<property name="useSqlStat">0</property>  <!-- 1为开启实时统计、0为关闭 -->
	<property name="useGlobleTableCheck">0</property>  <!-- 1为开启全加班一致性检测、0为关闭 -->

		<property name="sequnceHandlerType">2</property>
      <!--  <property name="useCompression">1</property>--> <!--1为开启mysql压缩协议-->
        <!--  <property name="fakeMySQLVersion">5.6.20</property>--> <!--设置模拟的MySQL版本号-->
	<!-- <property name="processorBufferChunk">40960</property> -->
	<!-- 
	<property name="processors">1</property> 
	<property name="processorExecutor">32</property> 
	 -->
		<!--默认为type 0: DirectByteBufferPool | type 1 ByteBufferArena-->
		<property name="processorBufferPoolType">0</property>
		<!--默认是65535 64K 用于sql解析时最大文本长度 -->
		<!--<property name="maxStringLiteralLength">65535</property>-->
		<!--<property name="sequnceHandlerType">0</property>-->
		<!--<property name="backSocketNoDelay">1</property>-->
		<!--<property name="frontSocketNoDelay">1</property>-->
		<!--<property name="processorExecutor">16</property>-->
		<!--
			<property name="serverPort">8066</property> <property name="managerPort">9066</property> 
			<property name="idleTimeout">300000</property> <property name="bindIp">0.0.0.0</property> 
			<property name="frontWriteQueueSize">4096</property> <property name="processors">32</property> -->
		<!--分布式事务开关，0为不过滤分布式事务，1为过滤分布式事务（如果分布式事务内只涉及全局表，则不过滤），2为不过滤分布式事务,但是记录分布式事务日志-->
		<property name="handleDistributedTransactions">0</property>
		
			<!--
			off heap for merge/order/group/limit      1开启   0关闭
		-->
		<property name="useOffHeapForMerge">1</property>

		<!--
			单位为m
		-->
		<property name="memoryPageSize">1m</property>

		<!--
			单位为k
		-->
		<property name="spillsFileBufferSize">1k</property>

		<property name="useStreamOutput">0</property>

		<!--
			单位为m
		-->
		<property name="systemReserveMemorySize">384m</property>
	</system>
	
	<!-- 全局SQL防火墙设置 -->
	<!-- 
	<firewall> 
	   <whitehost>
	      <host host="127.0.0.1" user="mycat"/>
	      <host host="127.0.0.2" user="mycat"/>
	   </whitehost>
       <blacklist check="false">
       </blacklist>
	</firewall>
	-->
	
	<user name="root">
		<property name="password">123456</property>
		<property name="schemas">TESTDB</property>
		
		<!-- 表级 DML 权限设置 -->
		<!-- 		
		<privileges check="false">
			<schema name="TESTDB" dml="0110" >
				<table name="tb01" dml="0000"></table>
				<table name="tb02" dml="1111"></table>
			</schema>
		</privileges>		
		 -->
	</user>

	<user name="user">
		<property name="password">user</property>
		<property name="schemas">TESTDB</property>
		<property name="readOnly">true</property>
	</user>

</mycat:server>
```

注：修改root对应的密码即可。



### 启动MycatStartup.java

运行Run as Application:




```
2017-07-19 12:50:21,689 [INFO ][main] total resouces of dataHost localhost1 is :2  (io.mycat.backend.datasource.PhysicalDBPool:PhysicalDBPool.java:100) 
2017-07-19 12:50:21,749 [INFO ][main] create layer cache pool TableID2DataNodeCache of type encache ,default cache size 10000 ,default expire seconds18000  (io.mycat.cache.CacheService:CacheService.java:125) 
2017-07-19 12:50:21,750 [INFO ][main] create child Cache: TESTDB_ORDERS for layered cache TableID2DataNodeCache, size 50000, expire seconds 18000  (io.mycat.cache.DefaultLayedCachePool:DefaultLayedCachePool.java:80) 
2017-07-19 12:50:23,129 [INFO ][main] dyna class load from D:\workspace-sts-3.8.4.RELEASE\mycat\src\main\catlet,and auto check for class file modified every 60 seconds  (io.mycat.config.classloader.DynaClassLoader:DynaClassLoader.java:34) 
2017-07-19 12:50:23,129 [INFO ][main] ===============================================  (io.mycat.MycatServer:MycatServer.java:258) 
2017-07-19 12:50:23,129 [INFO ][main] MyCat is ready to startup ...  (io.mycat.MycatServer:MycatServer.java:259) 
2017-07-19 12:50:23,129 [INFO ][main] Startup processors ...,total processors:4,aio thread pool size:8    
 each process allocated socket buffer pool  bytes ,a page size:2097152  a page's chunk number(PageSize/ChunkSize) is:512  buffer page's number is:80  (io.mycat.MycatServer:MycatServer.java:271) 
2017-07-19 12:50:23,129 [INFO ][main] sysconfig params:SystemConfig [processorBufferLocalPercent=100, frontSocketSoRcvbuf=1048576, frontSocketSoSndbuf=4194304, backSocketSoRcvbuf=4194304, backSocketSoSndbuf=1048576, frontSocketNoDelay=1, backSocketNoDelay=1, maxStringLiteralLength=65535, frontWriteQueueSize=2048, bindIp=0.0.0.0, serverPort=8066, managerPort=9066, charset=utf8, processors=4, processorExecutor=8, timerExecutor=2, managerExecutor=2, idleTimeout=1800000, catletClassCheckSeconds=60, sqlExecuteTimeout=300, processorCheckPeriod=1000, dataNodeIdleCheckPeriod=300000, dataNodeHeartbeatPeriod=10000, clusterHeartbeatUser=_HEARTBEAT_USER_, clusterHeartbeatPass=_HEARTBEAT_PASS_, clusterHeartbeatPeriod=5000, clusterHeartbeatTimeout=10000, clusterHeartbeatRetry=10, txIsolation=3, parserCommentVersion=50148, sqlRecordCount=10, bufferPoolPageSize=2097152, bufferPoolChunkSize=4096, bufferPoolPageNumber=80, maxResultSet=524288, bigResultSizeSqlCount=10, bufferUsagePercent=80, flowControlRejectStrategy=0, clearBigSqLResultSetMapMs=600000, defaultMaxLimit=100, sequnceHandlerType=2, sqlInterceptor=io.mycat.server.interceptor.impl.DefaultSqlInterceptor, sqlInterceptorType=select, sqlInterceptorFile=D:\workspace-sts-3.8.4.RELEASE\mycat/logs/sql.txt, mutiNodeLimitType=0, mutiNodePatchSize=100, defaultSqlParser=druidparser, usingAIO=0, packetHeaderSize=4, maxPacketSize=16777216, mycatNodeId=1]  (io.mycat.MycatServer:MycatServer.java:272) 
2017-07-19 12:50:23,243 [INFO ][main] useOffHeapForMerge = 1  (io.mycat.memory.MyCatMemory:MyCatMemory.java:53) 
2017-07-19 12:50:23,243 [INFO ][main] memoryPageSize = 1m  (io.mycat.memory.MyCatMemory:MyCatMemory.java:54) 
2017-07-19 12:50:23,243 [INFO ][main] spillsFileBufferSize = 1k  (io.mycat.memory.MyCatMemory:MyCatMemory.java:55) 
2017-07-19 12:50:23,243 [INFO ][main] useStreamOutput = 0  (io.mycat.memory.MyCatMemory:MyCatMemory.java:56) 
2017-07-19 12:50:23,243 [INFO ][main] systemReserveMemorySize = 384m  (io.mycat.memory.MyCatMemory:MyCatMemory.java:57) 
2017-07-19 12:50:23,263 [INFO ][main] totalNetWorkBufferSize = 160MB  (io.mycat.memory.MyCatMemory:MyCatMemory.java:58) 
2017-07-19 12:50:23,263 [INFO ][main] dataNodeSortedTempDir = D:\workspace-sts-3.8.4.RELEASE\mycat  (io.mycat.memory.MyCatMemory:MyCatMemory.java:59) 
2017-07-19 12:50:23,279 [INFO ][main] mycat.memory.offHeap.size: 1010MB  (io.mycat.memory.MyCatMemory:MyCatMemory.java:122) 
2017-07-19 12:50:23,295 [INFO ][main] using nio network handler   (io.mycat.MycatServer:MycatServer.java:373) 
2017-07-19 12:50:24,659 [INFO ][main] $_MyCatManager is started and listening on 9066  (io.mycat.MycatServer:MycatServer.java:389) 
2017-07-19 12:50:24,659 [INFO ][main] $_MyCatServer is started and listening on 8066  (io.mycat.MycatServer:MycatServer.java:393) 
2017-07-19 12:50:24,660 [INFO ][main] ===============================================  (io.mycat.MycatServer:MycatServer.java:395) 
2017-07-19 12:50:24,660 [INFO ][main] Initialize dataHost ...  (io.mycat.MycatServer:MycatServer.java:399) 
2017-07-19 12:50:24,660 [INFO ][main] init backend myqsl source ,create connections total 10 for hostM1 index :0  (io.mycat.backend.datasource.PhysicalDBPool:PhysicalDBPool.java:294) 
2017-07-19 12:50:24,661 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema n  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,662 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema m  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,662 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema n  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,662 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema m  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,665 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema n  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,665 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema m  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,666 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema n  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,666 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema m  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,666 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema n  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:24,666 [INFO ][main] no ilde connection in pool,create new connection for hostM1 of schema m  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:25,547 [INFO ][$_NIOREACTOR-3-RW] connected successfuly MySQLConnection [id=7, lastTime=1500439825547, user=root, schema=m, old shema=m, borrowed=true, fromSlaveDB=false, threadId=26, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,547 [INFO ][$_NIOREACTOR-1-RW] connected successfuly MySQLConnection [id=1, lastTime=1500439825547, user=root, schema=m, old shema=m, borrowed=true, fromSlaveDB=false, threadId=24, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,548 [INFO ][$_NIOREACTOR-2-RW] connected successfuly MySQLConnection [id=6, lastTime=1500439825547, user=root, schema=n, old shema=n, borrowed=true, fromSlaveDB=false, threadId=25, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,793 [INFO ][$_NIOREACTOR-2-RW] connected successfuly MySQLConnection [id=2, lastTime=1500439825793, user=root, schema=m, old shema=m, borrowed=true, fromSlaveDB=false, threadId=30, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,793 [INFO ][$_NIOREACTOR-3-RW] connected successfuly MySQLConnection [id=3, lastTime=1500439825793, user=root, schema=m, old shema=m, borrowed=true, fromSlaveDB=false, threadId=28, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,795 [INFO ][$_NIOREACTOR-0-RW] connected successfuly MySQLConnection [id=8, lastTime=1500439825795, user=root, schema=n, old shema=n, borrowed=true, fromSlaveDB=false, threadId=31, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,795 [INFO ][$_NIOREACTOR-1-RW] connected successfuly MySQLConnection [id=5, lastTime=1500439825795, user=root, schema=m, old shema=m, borrowed=true, fromSlaveDB=false, threadId=27, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,796 [INFO ][$_NIOREACTOR-0-RW] connected successfuly MySQLConnection [id=4, lastTime=1500439825796, user=root, schema=n, old shema=n, borrowed=true, fromSlaveDB=false, threadId=29, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,808 [INFO ][$_NIOREACTOR-1-RW] connected successfuly MySQLConnection [id=9, lastTime=1500439825808, user=root, schema=n, old shema=n, borrowed=true, fromSlaveDB=false, threadId=32, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,810 [INFO ][$_NIOREACTOR-2-RW] connected successfuly MySQLConnection [id=10, lastTime=1500439825810, user=root, schema=n, old shema=n, borrowed=true, fromSlaveDB=false, threadId=33, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.GetConnectionHandler:GetConnectionHandler.java:67) 
2017-07-19 12:50:25,876 [INFO ][main] init result :finished 10 success 10 target count:10  (io.mycat.backend.datasource.PhysicalDBPool:PhysicalDBPool.java:319) 
2017-07-19 12:50:25,876 [INFO ][main] localhost1 index:0 init success  (io.mycat.backend.datasource.PhysicalDBPool:PhysicalDBPool.java:265) 
2017-07-19 12:50:25,876 [INFO ][main] save DataHost index  localhost1 cur index 0  (io.mycat.MycatServer:MycatServer.java:557) 
MyCAT Server startup successfully. see logs in logs/mycat.log
2017-07-19 12:50:28,061 [INFO ][Timer1] create connections ,because idle connection not enough ,cur is 0, minCon is 10 for hostS2  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:299) 
2017-07-19 12:50:28,076 [INFO ][Timer1] no ilde connection in pool,create new connection for hostS2 of schema n  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:413) 
2017-07-19 12:50:28,293 [INFO ][$_NIOREACTOR-3-RW] connectionAcquired MySQLConnection [id=11, lastTime=1500439828275, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=true, threadId=22, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 12:50:28,319 [INFO ][$_NIOREACTOR-2-RW] connectionAcquired MySQLConnection [id=14, lastTime=1500439828316, user=root, schema=m, old shema=m, borrowed=false, fromSlaveDB=true, threadId=25, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 12:50:28,322 [INFO ][$_NIOREACTOR-0-RW] connectionAcquired MySQLConnection [id=12, lastTime=1500439828316, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=true, threadId=23, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 12:53:16,500 [INFO ][$_NIOREACTOR-3-RW] ServerConnection [id=1, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]'root' login success  (io.mycat.net.handler.FrontendAuthenticator:FrontendAuthenticator.java:194) 
2017-07-19 12:53:17,663 [INFO ][$_NIOREACTOR-3-RW] close connection,reason:quit cmd ,ServerConnection [id=1, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]  (io.mycat.net.AbstractConnection:AbstractConnection.java:508) 
2017-07-19 12:53:20,226 [INFO ][$_NIOREACTOR-0-RW] ServerConnection [id=2, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]'root' login success  (io.mycat.net.handler.FrontendAuthenticator:FrontendAuthenticator.java:194) 
2017-07-19 12:53:21,555 [INFO ][$_NIOREACTOR-1-RW] ServerConnection [id=3, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]'root' login success  (io.mycat.net.handler.FrontendAuthenticator:FrontendAuthenticator.java:194) 
2017-07-19 12:53:24,426 [INFO ][$_NIOREACTOR-1-RW] close connection,reason:quit cmd ,ServerConnection [id=3, schema=TESTDB, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=TESTDB]  (io.mycat.net.AbstractConnection:AbstractConnection.java:508) 
2017-07-19 12:53:24,427 [INFO ][$_NIOREACTOR-0-RW] close connection,reason:quit cmd ,ServerConnection [id=2, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]  (io.mycat.net.AbstractConnection:AbstractConnection.java:508) 
2017-07-19 12:53:25,222 [INFO ][$_NIOREACTOR-2-RW] ServerConnection [id=4, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]'root' login success  (io.mycat.net.handler.FrontendAuthenticator:FrontendAuthenticator.java:194) 
2017-07-19 12:53:26,713 [INFO ][$_NIOREACTOR-3-RW] ServerConnection [id=5, schema=null, host=192.168.1.4, user=root,txIsolation=3, autocommit=true, schema=null]'root' login success  (io.mycat.net.handler.FrontendAuthenticator:FrontendAuthenticator.java:194) 
2017-07-19 12:55:28,039 [INFO ][Timer0] create connections ,because idle connection not enough ,cur is 5, minCon is 10 for hostM1  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:299) 
2017-07-19 12:55:28,040 [INFO ][Timer0] create connections ,because idle connection not enough ,cur is 4, minCon is 10 for hostS2  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:299) 
2017-07-19 12:55:28,143 [INFO ][$_NIOREACTOR-0-RW] connectionAcquired MySQLConnection [id=15, lastTime=1500440128137, user=root, schema=m, old shema=m, borrowed=false, fromSlaveDB=true, threadId=26, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 12:55:28,153 [INFO ][$_NIOREACTOR-2-RW] connectionAcquired MySQLConnection [id=17, lastTime=1500440128137, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=true, threadId=27, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 12:55:28,172 [INFO ][$_NIOREACTOR-1-RW] connectionAcquired MySQLConnection [id=16, lastTime=1500440128157, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=false, threadId=34, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 13:00:28,040 [INFO ][Timer0] too many ilde cons ,close some for datasouce  hostM1  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:280) 
2017-07-19 13:00:28,043 [INFO ][Timer0] close connection,reason:too many idle con ,MySQLConnection [id=1, lastTime=1500440128058, user=root, schema=m, old shema=m, borrowed=false, fromSlaveDB=false, threadId=24, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.net.AbstractConnection:AbstractConnection.java:508) 
2017-07-19 13:00:28,044 [INFO ][Timer0] create connections ,because idle connection not enough ,cur is 5, minCon is 10 for hostS2  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:299) 
2017-07-19 13:00:28,063 [INFO ][$_NIOREACTOR-3-RW] connectionAcquired MySQLConnection [id=18, lastTime=1500440428058, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=true, threadId=28, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 13:05:28,044 [INFO ][Timer0] create connections ,because idle connection not enough ,cur is 6, minCon is 10 for hostM1  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:299) 
2017-07-19 13:05:28,044 [INFO ][Timer0] create connections ,because idle connection not enough ,cur is 5, minCon is 10 for hostS2  (io.mycat.backend.datasource.PhysicalDatasource:PhysicalDatasource.java:299) 
2017-07-19 13:05:28,119 [INFO ][$_NIOREACTOR-0-RW] connectionAcquired MySQLConnection [id=19, lastTime=1500440728101, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=false, threadId=35, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.129, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45) 
2017-07-19 13:05:28,120 [INFO ][$_NIOREACTOR-1-RW] connectionAcquired MySQLConnection [id=20, lastTime=1500440728101, user=root, schema=n, old shema=n, borrowed=false, fromSlaveDB=true, threadId=29, charset=latin1, txIsolation=3, autocommit=true, attachment=null, respHandler=null, host=10.0.0.128, port=3306, statusSync=null, writeQueue=0, modifiedSQLExecuted=false]  (io.mycat.backend.mysql.nio.handler.NewConnectionRespHandler:NewConnectionRespHandler.java:45)
```




## IDE异常处理

在配置过程中遇到：Archive for required library: 'D:/DEVELOPERS/maven/apache-maven-3.5.0/repository/mysql/mysql-connector-java/5.1.35/mysql-connector-java-5.1.35.jar' in project 'mycat' cannot be read or is not a valid ZIP file    mycat        Build path    Build Path Problem



**处理方法：**删除maven下载目录，编辑pom.xml使之重新下载jar包。




## Mycat开发示例修改源码支持按天分库

这里我们只需要修改<table>标签的subTables属性的封装就可以实现。


### Java源码修改

TableConfig.java 修改代码部分：



```java
public TableConfig(String name, String primaryKey, boolean autoIncrement,boolean needAddLimit, int tableType,
			String dataNode,Set<String> dbType, RuleConfig rule, boolean ruleRequired,
			TableConfig parentTC, boolean isChildTable, String joinKey,
			String parentKey,String subTables) {
		if (name == null) {
			throw new IllegalArgumentException("table name is null");
		} else if (dataNode == null) {
			throw new IllegalArgumentException("dataNode name is null");
		}
		this.primaryKey = primaryKey;
		this.autoIncrement = autoIncrement;
		this.needAddLimit=needAddLimit;
		this.tableType = tableType;
		this.dbTypes=dbType;
		if (ruleRequired && rule == null) {
			throw new IllegalArgumentException("ruleRequired but rule is null");
		}

		this.name = name.toUpperCase();
		
		String theDataNodes[] = SplitUtil.split(dataNode, ',', '$', '-');
		if (theDataNodes == null || theDataNodes.length <= 0) {
			throw new IllegalArgumentException("invalid table dataNodes: " + dataNode);
		}
		dataNodes = new ArrayList<String>(theDataNodes.length);
		for (String dn : theDataNodes) {
			dataNodes.add(dn);
		}
		
		if(subTables!=null && !subTables.equals("")){
			// =====新增按日分表策略====start====
			String str=subTables.substring(subTables.indexOf("$")+1, subTables.length());
			String startChar="(";
			String splitChar=",";
			String endChar=")";
			boolean valid=str.contains(startChar)&&str.contains(splitChar)&&str.contains(endChar);
			// =====新增按日分表策略====end====
			if(!valid){
				// 原始按顺序分表方式
				String sTables[] = SplitUtil.split(subTables, ',', '$', '-');
				if (sTables == null || sTables.length <= 0) {
					throw new IllegalArgumentException("invalid table subTables");
				}
				this.distTables = new ArrayList<String>(sTables.length);
				for (String table : sTables) {
					distTables.add(table);
				}
			}else{
				// 按日分表方式（表需要自己创建好）
				str=str.substring(1, str.length()-1);
				String [] strDates=str.split(",");
				if(strDates.length!=2) {
					throw new IllegalArgumentException("invalid table subTables:array's length is little than 2 ,you should set array like $(20170717,20180716)");
				}
				String dateStart=strDates[0];//开始日期
				String dateEnd=strDates[1];// 结束日期
				if(dateStart.equals(dateEnd)){
					throw new IllegalArgumentException("invalid table subTables:array's start date can't be equal with end date,you should set array like $(20170717,20180716)");
				}
				String  dateFormat="yyyyMMdd";
				Date begin=null;
				Date end=null;
				try {
					begin=DateUtils.parseDate(dateStart, dateFormat);
					end=DateUtils.parseDate(dateEnd, dateFormat);
				} catch (ParseException e) {
					throw new IllegalArgumentException("invalid table subTables:array's  date format is incorrect,you should set array like $(20170717,20180716)");
				}
				// 生成对应的按日期表集合
				List<Date> dates=DateUtils.getBetweenDates(begin, end);
				this.distTables = new ArrayList<String>(dates.size());
				String tablePrex=subTables.substring(0, subTables.indexOf("$"));
				for (Date date : dates) {
					String table=tablePrex+DateUtils.getStrFormTime(dateFormat, date);
					System.out.println(table);
					distTables.add(table);
				}
			}
		}else{
			this.distTables = new ArrayList<String>();
		}	
		
		this.rule = rule;
		this.partitionColumn = (rule == null) ? null : rule.getColumn();
		partionKeyIsPrimaryKey=(partitionColumn==null)?primaryKey==null:partitionColumn.equals(primaryKey);
		this.ruleRequired = ruleRequired;
		this.childTable = isChildTable;
		this.parentTC = parentTC;
		this.joinKey = joinKey;
		this.parentKey = parentKey;
		if (parentTC != null) {
			locateRTableKeySql = genLocateRootParentSQL();
			secondLevel = (parentTC.parentTC == null);
		} else {
			locateRTableKeySql = null;
			secondLevel = false;
		}
	}
```




### 日期工具类

DateUtils.java



```java
package io.mycat.config.util;

import java.sql.Timestamp;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Calendar;
import java.util.Date;
import java.util.List;

import org.apache.log4j.Logger;

public class DateUtils extends org.apache.commons.lang.time.DateUtils {

	public static final String TIME_WITH_MINUTE_PATTERN = "HH:mm";

	public static final long DAY_MILLI = 24 * 60 * 60 * 1000; // 一天的MilliSecond

	// 取值范围：就是临界点包不包含等于

	public final static int LEFT_OPEN_RIGHT_OPEN = 1;
	public final static int LEFT_CLOSE_RIGHT_OPEN = 2;
	public final static int LEFT_OPEN_RIGHT_CLOSE = 3;
	public final static int LEFT_CLOSE_RIGHT_CLOSE = 4;
	/**
	 * 比较日期的模式 --只比较日期，不比较时间
	 */
	public final static int COMP_MODEL_DATE = 1;
	/**
	 * 比较日期的模式 --只比较时间，不比较日期
	 */
	public final static int COMP_MODEL_TIME = 2;
	/**
	 * 比较日期的模式 --比较日期，也比较时间
	 */
	public final static int COMP_MODEL_DATETIME = 3;

	private static Logger logger = Logger.getLogger(DateUtils.class);

	/**
	 * 要用到的DATE Format的定义
	 */
	public static String DATE_FORMAT_DATEONLY = "yyyy-MM-dd"; // 年/月/日
	public static String DATE_FORMAT_DATETIME = "yyyy-MM-dd HH:mm:ss"; // 年/月/日
	public static SimpleDateFormat sdfDateTime = new SimpleDateFormat(DateUtils.DATE_FORMAT_DATETIME);
	public static SimpleDateFormat sdfDateOnly = new SimpleDateFormat(DateUtils.DATE_FORMAT_DATEONLY);
	public static final SimpleDateFormat formatTimestamp = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	public static final SimpleDateFormat SHORTDATEFORMAT = new SimpleDateFormat("yyyyMMdd");
	public static final SimpleDateFormat SHORT_DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd");
	public static final SimpleDateFormat LONG_DATE_FORMAT = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
	public static final SimpleDateFormat LONG_DATE_FORMAT_SSS = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss.SSS");
	public static final SimpleDateFormat HMS_FORMAT = new SimpleDateFormat("HH:mm:ss");

	/**
	 * 根据日期格式字符串解析日期字符串
	 *
	 * @param str
	 *            日期字符串
	 * @param parsePatterns
	 *            日期格式字符串
	 * @return 解析后日期
	 * @throws ParseException
	 */
	public static Date parseDate(String str, String parsePatterns) throws ParseException {
		return parseDate(str, new String[] { parsePatterns });
	}

	/**
	 * 根据单位字段比较两个日期
	 *
	 * @param date
	 *            日期1
	 * @param otherDate
	 *            日期2
	 * @param withUnit
	 *            单位字段，从Calendar field取值
	 * @return 等于返回0值, 大于返回大于0的值 小于返回小于0的值
	 */
	public static int compareDate(Date date, Date otherDate, int withUnit) {
		Calendar dateCal = Calendar.getInstance();
		dateCal.setTime(date);
		Calendar otherDateCal = Calendar.getInstance();
		otherDateCal.setTime(otherDate);

		switch (withUnit) {
		case Calendar.YEAR:
			dateCal.clear(Calendar.MONTH);
			otherDateCal.clear(Calendar.MONTH);
		case Calendar.MONTH:
			dateCal.set(Calendar.DATE, 1);
			otherDateCal.set(Calendar.DATE, 1);
		case Calendar.DATE:
			dateCal.set(Calendar.HOUR_OF_DAY, 0);
			otherDateCal.set(Calendar.HOUR_OF_DAY, 0);
		case Calendar.HOUR:
			dateCal.clear(Calendar.MINUTE);
			otherDateCal.clear(Calendar.MINUTE);
		case Calendar.MINUTE:
			dateCal.clear(Calendar.SECOND);
			otherDateCal.clear(Calendar.SECOND);
		case Calendar.SECOND:
			dateCal.clear(Calendar.MILLISECOND);
			otherDateCal.clear(Calendar.MILLISECOND);
		case Calendar.MILLISECOND:
			break;
		default:
			throw new IllegalArgumentException("withUnit 单位字段 " + withUnit + " 不合法！！");
		}
		return dateCal.compareTo(otherDateCal);
	}

	/**
	 * 根据单位字段比较两个时间
	 *
	 * @param date
	 *            时间1
	 * @param otherDate
	 *            时间2
	 * @param withUnit
	 *            单位字段，从Calendar field取值
	 * @return 等于返回0值, 大于返回大于0的值 小于返回小于0的值
	 */
	public static int compareTime(Date date, Date otherDate, int withUnit) {
		Calendar dateCal = Calendar.getInstance();
		dateCal.setTime(date);
		Calendar otherDateCal = Calendar.getInstance();
		otherDateCal.setTime(otherDate);

		dateCal.clear(Calendar.YEAR);
		dateCal.clear(Calendar.MONTH);
		dateCal.set(Calendar.DATE, 1);
		otherDateCal.clear(Calendar.YEAR);
		otherDateCal.clear(Calendar.MONTH);
		otherDateCal.set(Calendar.DATE, 1);
		switch (withUnit) {
		case Calendar.HOUR:
			dateCal.clear(Calendar.MINUTE);
			otherDateCal.clear(Calendar.MINUTE);
		case Calendar.MINUTE:
			dateCal.clear(Calendar.SECOND);
			otherDateCal.clear(Calendar.SECOND);
		case Calendar.SECOND:
			dateCal.clear(Calendar.MILLISECOND);
			otherDateCal.clear(Calendar.MILLISECOND);
		case Calendar.MILLISECOND:
			break;
		default:
			throw new IllegalArgumentException("withUnit 单位字段 " + withUnit + " 不合法！！");
		}
		return dateCal.compareTo(otherDateCal);
	}

	/**
	 * 获得当前的日期毫秒
	 *
	 * @return
	 */
	public static long nowTimeMillis() {
		return System.currentTimeMillis();
	}

	/**
	 * 获得当前的时间戳
	 *
	 * @return
	 */
	public static Timestamp nowTimeStamp() {
		return new Timestamp(nowTimeMillis());
	}

	/**
	 * yyyy-MM-dd 当前日期
	 *
	 */
	public static String getReqDate() {
		return SHORTDATEFORMAT.format(new Date());
	}

	/**
	 * yyyy-MM-dd 传入日期
	 *
	 * @param date
	 * @return
	 */
	public static String getReqDate(Date date) {
		return SHORT_DATE_FORMAT.format(date);
	}

	/**
	 * yyyyMMdd 传入日期
	 *
	 * @param date
	 * @return
	 */
	public static String getReqDateyyyyMMdd(Date date) {
		return SHORTDATEFORMAT.format(date);
	}

	/**
	 * yyyy-MM-dd 传入的时间戳
	 *
	 * @param date
	 * @return
	 */
	public static String TimestampToDateStr(Timestamp tmp) {
		return SHORT_DATE_FORMAT.format(tmp);
	}

	/**
	 * HH:mm:ss 当前时间
	 *
	 * @return
	 */
	public static String getReqTime() {
		return HMS_FORMAT.format(new Date());
	}

	/**
	 * 得到时间戳格式字串
	 *
	 * @param date
	 * @return
	 */
	public static String getTimeStampStr(Date date) {
		return LONG_DATE_FORMAT.format(date);
	}

	/**
	 * 得到长日期格式字串
	 *
	 * @return
	 */
	public static String getLongDateStr() {
		return LONG_DATE_FORMAT.format(new Date());
	}

	public static String getLongDateStr(Timestamp time) {
		return LONG_DATE_FORMAT.format(time);
	}

	/**
	 * 得到短日期格式字串
	 *
	 * @param date
	 * @return
	 */
	public static String getShortDateStr(Date date) {
		return SHORT_DATE_FORMAT.format(date);
	}

	public static String getShortDateStr() {
		return SHORT_DATE_FORMAT.format(new Date());
	}

	/**
	 * 计算 second 秒后的时间
	 *
	 * @param date
	 * @param second
	 * @return
	 */
	public static Date addSecond(Date date, int second) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		;
		calendar.add(Calendar.SECOND, second);
		return calendar.getTime();
	}

	/**
	 * 计算 minute 分钟后的时间
	 *
	 * @param date
	 * @param minute
	 * @return
	 */
	public static Date addMinute(Date date, int minute) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.add(Calendar.MINUTE, minute);
		return calendar.getTime();
	}

	/**
	 * 计算 hour 小时后的时间
	 *
	 * @param date
	 * @param minute
	 * @return
	 */
	public static Date addHour(Date date, int hour) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.add(Calendar.HOUR, hour);
		return calendar.getTime();
	}

	/**
	 * 得到day的起始时间点。
	 *
	 * @param date
	 * @return
	 */
	public static Date getDayStart(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.HOUR_OF_DAY, 0);
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		return calendar.getTime();
	}

	/**
	 * 得到day的终止时间点.
	 *
	 * @param date
	 * @return
	 */
	public static Date getDayEnd(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.HOUR_OF_DAY, 0);
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		calendar.add(Calendar.DAY_OF_MONTH, 1);
		calendar.add(Calendar.MILLISECOND, -1);
		return calendar.getTime();
	}

	/**
	 * 计算 day 天后的时间
	 *
	 * @param date
	 * @param day
	 * @return
	 */
	public static Date addDay(Date date, int day) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.add(Calendar.DAY_OF_MONTH, day);
		return calendar.getTime();
	}

	/**
	 * 得到month的终止时间点.
	 *
	 * @param date
	 * @return
	 */
	public static Date getMonthEnd(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.DAY_OF_MONTH, 1);
		calendar.set(Calendar.HOUR_OF_DAY, 0);
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		calendar.add(Calendar.MONTH, 1);
		calendar.add(Calendar.MILLISECOND, -1);
		return calendar.getTime();
	}

	public static Date addYear(Date date, int year) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.add(Calendar.DAY_OF_YEAR, 365 * year);
		return calendar.getTime();
	}

	public static Timestamp strToTimestamp(String dateStr) {
		return Timestamp.valueOf(dateStr);
	}

	public static Timestamp strToTimestamp(Date date) {
		return Timestamp.valueOf(formatTimestamp.format(date));
	}

	public static Timestamp getCurTimestamp() {
		return Timestamp.valueOf(formatTimestamp.format(new Date()));
	}

	/**
	 * 取得两个日期之间的日数
	 *
	 * @return t1到t2间的日数，如果t2 在 t1之后，返回正数，否则返回负数
	 */
	public static long daysBetween(java.sql.Timestamp t1, java.sql.Timestamp t2) {
		return (t2.getTime() - t1.getTime()) / DAY_MILLI;
	}

	/**
	 * 返回java.sql.Timestamp型的SYSDATE
	 *
	 * @return java.sql.Timestamp型的SYSDATE
	 * @since 1.0
	 * @history
	 */
	public static java.sql.Timestamp getSysDateTimestamp() {
		return new java.sql.Timestamp(System.currentTimeMillis());
	}

	/**
	 * 利用缺省的Date格式(YYYY/MM/DD)转换String到java.sql.Timestamp
	 *
	 * @param sDate
	 *            Date string
	 * @return
	 * @since 1.0
	 * @history
	 */
	public static java.sql.Timestamp toSqlTimestamp(String sDate) {
		if (sDate == null) {
			return null;
		}
		if (sDate.length() != DateUtils.DATE_FORMAT_DATEONLY.length()
				&& sDate.length() != DateUtils.DATE_FORMAT_DATETIME.length()) {
			return null;
		}
		return toSqlTimestamp(sDate, sDate.length() == DateUtils.DATE_FORMAT_DATEONLY.length()
				? DateUtils.DATE_FORMAT_DATEONLY : DateUtils.DATE_FORMAT_DATETIME);

	}

	/**
	 * 利用缺省的Date格式(YYYY/MM/DD hh:mm:ss)转化String到java.sql.Timestamp
	 *
	 * @param sDate
	 *            Date string
	 * @param sFmt
	 *            Date format DATE_FORMAT_DATEONLY/DATE_FORMAT_DATETIME
	 * @return
	 * @since 1.0
	 * @history
	 */
	public static java.sql.Timestamp toSqlTimestamp(String sDate, String sFmt) {
		String temp = null;
		if (sDate == null || sFmt == null) {
			return null;
		}
		if (sDate.length() != sFmt.length()) {
			return null;
		}
		if (sFmt.equals(DateUtils.DATE_FORMAT_DATETIME)) {
			temp = sDate.replace('/', '-');
			temp = temp + ".000000000";
		} else if (sFmt.equals(DateUtils.DATE_FORMAT_DATEONLY)) {
			temp = sDate.replace('/', '-');
			temp = temp + " 00:00:00.000000000";
		} else {
			return null;
		}
		return java.sql.Timestamp.valueOf(temp);
	}

	/**
	 * 以YYYY/MM/DD HH24:MI:SS格式返回系统日期时间
	 *
	 * @return 系统日期时间
	 * @since 1.0
	 * @history
	 */
	public static String getSysDateTimeString() {
		return toString(new java.util.Date(System.currentTimeMillis()), DateUtils.sdfDateTime);
	}

	/**
	 * 根据指定的Format转化java.util.Date到String
	 *
	 * @param dt
	 *            java.util.Date instance
	 * @param sFmt
	 *            Date format , DATE_FORMAT_DATEONLY or DATE_FORMAT_DATETIME
	 * @return
	 * @since 1.0
	 * @history
	 */
	public static String toString(java.util.Date dt, String sFmt) {
		if (dt == null || sFmt == null || "".equals(sFmt)) {
			return "";
		}
		return toString(dt, new SimpleDateFormat(sFmt));
	}

	/**
	 * 利用指定SimpleDateFormat instance转换java.util.Date到String
	 *
	 * @param dt
	 *            java.util.Date instance
	 * @param formatter
	 *            SimpleDateFormat Instance
	 * @return
	 * @since 1.0
	 * @history
	 */
	private static String toString(java.util.Date dt, SimpleDateFormat formatter) {
		String sRet = null;

		try {
			sRet = formatter.format(dt).toString();
		} catch (Exception e) {
			logger.error(e);
			sRet = null;
		}

		return sRet;
	}

	/**
	 * 转换java.sql.Timestamp到String，格式为YYYY/MM/DD HH24:MI
	 *
	 * @param dt
	 *            java.sql.Timestamp instance
	 * @return
	 * @since 1.0
	 * @history
	 */
	public static String toSqlTimestampString2(java.sql.Timestamp dt) {
		if (dt == null) {
			return null;
		}
		String temp = toSqlTimestampString(dt, DateUtils.DATE_FORMAT_DATETIME);
		return temp.substring(0, 16);
	}

	public static String toString(java.sql.Timestamp dt) {
		return dt == null ? "" : toSqlTimestampString2(dt);
	}

	/**
	 * 根据指定的格式转换java.sql.Timestamp到String
	 *
	 * @param dt
	 *            java.sql.Timestamp instance
	 * @param sFmt
	 *            Date 格式，DATE_FORMAT_DATEONLY/DATE_FORMAT_DATETIME/
	 *            DATE_FORMAT_SESSION
	 * @return
	 * @since 1.0
	 * @history
	 */
	public static String toSqlTimestampString(java.sql.Timestamp dt, String sFmt) {
		String temp = null;
		String out = null;
		if (dt == null || sFmt == null) {
			return null;
		}
		temp = dt.toString();
		if (sFmt.equals(DateUtils.DATE_FORMAT_DATETIME) || // "YYYY/MM/DD
		// HH24:MI:SS"
				sFmt.equals(DateUtils.DATE_FORMAT_DATEONLY)) { // YYYY/MM/DD
			temp = temp.substring(0, sFmt.length());
			out = temp.replace('/', '-');
			// }else if( sFmt.equals (DateUtils.DATE_FORMAT_SESSION ) ){
			// //Session
			// out =
			// temp.substring(0,4)+temp.substring(5,7)+temp.substring(8,10);
			// out += temp.substring(12,14) + temp.substring(15,17);
		}
		return out;
	}

	// 得到当前日期的星期
	public static int getWeek() {
		Calendar cal = Calendar.getInstance();
		cal.setTime(new Date());
		int w = cal.get(Calendar.DAY_OF_WEEK);
		return w;
	}

	/**
	 * Timestamp 格式转换成yyyy-MM-dd timestampToSql(Timestamp 格式转换成yyyy-MM-dd)
	 *
	 * @param timestamp
	 *            时间
	 * @return createTimeStr yyyy-MM-dd 时间
	 * @Exception 异常对象
	 * @since V1.0
	 */
	public static String timestampToStringYMD(java.sql.Timestamp timestamp) {
		SimpleDateFormat sdf = new SimpleDateFormat(DateUtils.DATE_FORMAT_DATEONLY);
		String createTimeStr = sdf.format(timestamp);
		return createTimeStr;
	}

	/**
	 * 判断一个时间是否在某个时间区间内
	 *
	 * @param now
	 *            目标时间
	 * @param start
	 *            时间区间开始
	 * @param end
	 *            时间区间结束
	 * @param model
	 *            区间模式
	 * @return 是否在区间内
	 */
	public static boolean isBetween(Date now, Date start, Date end, int model) {
		return isBetween(now, start, end, model, LEFT_OPEN_RIGHT_OPEN);
	}

	/**
	 * 判断时间是否在制定的时间段之类
	 *
	 * @param date
	 *            需要判断的时间
	 * @param start
	 *            时间段的起始时间
	 * @param end
	 *            时间段的截止时间
	 * @param interModel
	 *            区间的模式
	 *
	 *            <pre>
	* 取值：
	* LEFT_OPEN_RIGHT_OPEN
	* LEFT_CLOSE_RIGHT_OPEN
	* LEFT_OPEN_RIGHT_CLOSE
	* LEFT_CLOSE_RIGHT_CLOSE
	 *            </pre>
	 * 
	 * @param compModel
	 *            比较的模式
	 *
	 *            <pre>
	* 取值：
	* COMP_MODEL_DATE 只比较日期，不比较时间
	* COMP_MODEL_TIME 只比较时间，不比较日期
	* COMP_MODEL_DATETIME 比较日期，也比较时间
	 *            </pre>
	 * 
	 * @return
	 */
	public static boolean isBetween(Date date, Date start, Date end, int interModel, int compModel) {
		if (date == null || start == null || end == null) {
			throw new IllegalArgumentException("日期不能为空");
		}
		SimpleDateFormat format = null;
		switch (compModel) {
		case COMP_MODEL_DATE: {
			format = new SimpleDateFormat("yyyyMMdd");
			break;
		}
		case COMP_MODEL_TIME: {
			format = new SimpleDateFormat("HHmmss");
			break;
		}
		case COMP_MODEL_DATETIME: {
			format = new SimpleDateFormat("yyyyMMddHHmmss");
			break;
		}
		default: {
			throw new IllegalArgumentException(String.format("日期的比较模式[%d]有误", compModel));
		}
		}
		long dateNumber = Long.parseLong(format.format(date));
		long startNumber = Long.parseLong(format.format(start));
		long endNumber = Long.parseLong(format.format(end));
		switch (interModel) {
		case LEFT_OPEN_RIGHT_OPEN: {
			if (dateNumber <= startNumber || dateNumber >= endNumber) {
				return false;
			} else {
				return true;
			}
		}
		case LEFT_CLOSE_RIGHT_OPEN: {
			if (dateNumber < startNumber || dateNumber >= endNumber) {
				return false;
			} else {
				return true;
			}
		}
		case LEFT_OPEN_RIGHT_CLOSE: {
			if (dateNumber <= startNumber || dateNumber > endNumber) {
				return false;
			} else {
				return true;
			}
		}
		case LEFT_CLOSE_RIGHT_CLOSE: {
			if (dateNumber < startNumber || dateNumber > endNumber) {
				return false;
			} else {
				return true;
			}
		}
		default: {
			throw new IllegalArgumentException(String.format("日期的区间模式[%d]有误", interModel));
		}
		}
	}

	/**
	 * 得到当前周起始时间
	 *
	 * @param date
	 * @return
	 */
	public static Date getWeekStart(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.get(Calendar.WEEK_OF_YEAR);
		int firstDay = calendar.getFirstDayOfWeek();
		calendar.set(Calendar.DAY_OF_WEEK, firstDay);
		calendar.set(Calendar.HOUR_OF_DAY, 0);
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		return calendar.getTime();
	}

	/**
	 * 得到当前周截止时间
	 *
	 * @param date
	 * @return
	 */
	public static Date getWeekEnd(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.get(Calendar.WEEK_OF_YEAR);
		int firstDay = calendar.getFirstDayOfWeek();
		calendar.set(Calendar.DAY_OF_WEEK, 8 - firstDay);
		calendar.set(Calendar.HOUR_OF_DAY, 23);
		calendar.set(Calendar.MINUTE, 59);
		calendar.set(Calendar.SECOND, 59);
		calendar.set(Calendar.MILLISECOND, 0);
		return calendar.getTime();
	}

	/**
	 * 得到当月起始时间
	 *
	 * @param date
	 * @return
	 */
	public static Date getMonthStart(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.DAY_OF_MONTH, 1);
		calendar.set(Calendar.HOUR_OF_DAY, 0);
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		return calendar.getTime();
	}

	/**
	 * 得到当前年起始时间
	 *
	 * @param date
	 * @return
	 */
	public static Date getYearStart(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.YEAR, calendar.get(Calendar.YEAR));
		calendar.set(Calendar.MONTH, 0);
		calendar.set(Calendar.DAY_OF_MONTH, 1);
		calendar.set(Calendar.HOUR_OF_DAY, 0);
		calendar.set(Calendar.MINUTE, 0);
		calendar.set(Calendar.SECOND, 0);
		calendar.set(Calendar.MILLISECOND, 0);
		return calendar.getTime();
	}

	/**
	 * 得到当前年最后一天
	 *
	 * @param date
	 * @return
	 */
	public static Date getYearEnd(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.YEAR, calendar.get(Calendar.YEAR));
		calendar.set(Calendar.MONTH, 11);
		calendar.set(Calendar.DAY_OF_MONTH, 31);
		calendar.set(Calendar.HOUR_OF_DAY, 23);
		calendar.set(Calendar.MINUTE, 59);
		calendar.set(Calendar.SECOND, 59);
		calendar.set(Calendar.MILLISECOND, 0);
		return calendar.getTime();
	}

	/**
	 * 取得月天数
	 *
	 * @param date
	 * @return
	 */
	public static int getDayOfMonth(Date date) {
		Calendar c = Calendar.getInstance();
		c.setTime(date);
		return c.getActualMaximum(Calendar.DAY_OF_MONTH);
	}

	/**
	 * 取得月第一天
	 *
	 * @param date
	 * @return
	 */
	public static Date getFirstDateOfMonth(Date date) {
		Calendar c = Calendar.getInstance();
		c.setTime(date);
		c.set(Calendar.DAY_OF_MONTH, c.getActualMinimum(Calendar.DAY_OF_MONTH));
		return c.getTime();
	}

	/**
	 * 取得月最后一天
	 *
	 * @param date
	 * @return
	 */
	public static Date getLastDateOfMonth(Date date) {
		Calendar c = Calendar.getInstance();
		c.setTime(date);
		c.set(Calendar.DAY_OF_MONTH, c.getActualMaximum(Calendar.DAY_OF_MONTH));
		return c.getTime();
	}

	/**
	 * 取得季度第一天
	 *
	 * @param date
	 * @return
	 */
	public static Date getSeasonStart(Date date) {
		return getDayStart(getFirstDateOfMonth(getSeasonDate(date)[0]));
	}

	/**
	 * 取得季度最后一天
	 *
	 * @param date
	 * @return
	 */
	public static Date getSeasonEnd(Date date) {
		return getDayEnd(getLastDateOfMonth(getSeasonDate(date)[2]));
	}

	/**
	 * 取得季度月
	 *
	 * @param date
	 * @return
	 */
	public static Date[] getSeasonDate(Date date) {
		Date[] season = new Date[3];

		Calendar c = Calendar.getInstance();
		c.setTime(date);

		int nSeason = getSeason(date);
		if (nSeason == 1) {// 第一季度
			c.set(Calendar.MONTH, Calendar.JANUARY);
			season[0] = c.getTime();
			c.set(Calendar.MONTH, Calendar.FEBRUARY);
			season[1] = c.getTime();
			c.set(Calendar.MONTH, Calendar.MARCH);
			season[2] = c.getTime();
		} else if (nSeason == 2) {// 第二季度
			c.set(Calendar.MONTH, Calendar.APRIL);
			season[0] = c.getTime();
			c.set(Calendar.MONTH, Calendar.MAY);
			season[1] = c.getTime();
			c.set(Calendar.MONTH, Calendar.JUNE);
			season[2] = c.getTime();
		} else if (nSeason == 3) {// 第三季度
			c.set(Calendar.MONTH, Calendar.JULY);
			season[0] = c.getTime();
			c.set(Calendar.MONTH, Calendar.AUGUST);
			season[1] = c.getTime();
			c.set(Calendar.MONTH, Calendar.SEPTEMBER);
			season[2] = c.getTime();
		} else if (nSeason == 4) {// 第四季度
			c.set(Calendar.MONTH, Calendar.OCTOBER);
			season[0] = c.getTime();
			c.set(Calendar.MONTH, Calendar.NOVEMBER);
			season[1] = c.getTime();
			c.set(Calendar.MONTH, Calendar.DECEMBER);
			season[2] = c.getTime();
		}
		return season;
	}

	/**
	 *
	 * 1 第一季度 2 第二季度 3 第三季度 4 第四季度
	 *
	 * @param date
	 * @return
	 */
	public static int getSeason(Date date) {

		int season = 0;

		Calendar c = Calendar.getInstance();
		c.setTime(date);
		int month = c.get(Calendar.MONTH);
		switch (month) {
		case Calendar.JANUARY:
		case Calendar.FEBRUARY:
		case Calendar.MARCH:
			season = 1;
			break;
		case Calendar.APRIL:
		case Calendar.MAY:
		case Calendar.JUNE:
			season = 2;
			break;
		case Calendar.JULY:
		case Calendar.AUGUST:
		case Calendar.SEPTEMBER:
			season = 3;
			break;
		case Calendar.OCTOBER:
		case Calendar.NOVEMBER:
		case Calendar.DECEMBER:
			season = 4;
			break;
		default:
			break;
		}
		return season;
	}

	public static int getWeekOfYear(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setFirstDayOfWeek(Calendar.MONDAY);
		calendar.setTime(date);
		return calendar.get(Calendar.WEEK_OF_YEAR);
	}

	public static int getMonthOfYear(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setFirstDayOfWeek(Calendar.MONDAY);
		calendar.setTime(date);
		return calendar.get(Calendar.WEEK_OF_MONTH);
	}

	/**
	 * 字符串转date
	 *
	 * @param dateString
	 * @return
	 */
	public static Date StringToDate(String dateString) {
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy/MM/dd");
		Date date = null;
		try {
			date = sdf.parse(dateString);
		} catch (ParseException e) {
			logger.error(e);
		}
		return date;
	}

	/**
	 * 判断输入日期是一个星期中的第几天(星期天为一个星期第一天)
	 *
	 * @param date
	 * @return
	 */
	public static int getWeekIndex(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		return calendar.get(Calendar.DAY_OF_WEEK);
	}

	/**
	 * 当前时间的前几天，并且以例如2013/12/09 00:00:00 形式输出
	 */
	public static Date subDays(int days) {
		Date date = addDay(new Date(), -days);
		String dateStr = getReqDate(date);
		Date date1 = null;
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
		try {
			date1 = sdf.parse(dateStr);
		} catch (ParseException e) {
			logger.error(e);
		}
		return date1;
	}

	/**
	 * 判断开始时间和结束时间，是否超出了当前时间的一定的间隔数限制 如：开始时间和结束时间，不能超出距离当前时间90天
	 *
	 * @param startDate
	 *            开始时间
	 * @param endDate
	 *            结束时间按
	 * @param interval
	 *            间隔数
	 * @param dateUnit
	 *            单位(如：月，日),参照Calendar的时间单位
	 * @return
	 */
	public static boolean isOverIntervalLimit(Date startDate, Date endDate, int interval, int dateUnit) {
		Calendar cal = Calendar.getInstance();
		cal.setTime(new Date());
		cal.add(dateUnit, interval * (-1));
		Date curDate = getDayStart(cal.getTime());
		if (getDayStart(startDate).compareTo(curDate) < 0 || getDayStart(endDate).compareTo(curDate) < 0) {
			return true;
		}
		return false;
	}

	/**
	 * 判断开始时间和结束时间，是否超出了当前时间的一定的间隔数限制, 时间单位默认为天数 如：开始时间和结束时间，不能超出距离当前时间90天
	 *
	 * @param startDate
	 *            开始时间
	 * @param endDate
	 *            结束时间按
	 * @param interval
	 *            间隔数
	 * @return
	 */
	public static boolean isOverIntervalLimit(Date startDate, Date endDate, int interval) {
		Calendar cal = Calendar.getInstance();
		cal.setTime(new Date());
		cal.add(Calendar.DAY_OF_MONTH, interval * (-1));
		Date curDate = getDayStart(cal.getTime());
		if (getDayStart(startDate).compareTo(curDate) < 0 || getDayStart(endDate).compareTo(curDate) < 0) {
			return true;
		}
		return false;
	}

	/**
	 * 判断开始时间和结束时间，是否超出了当前时间的一定的间隔数限制, 时间单位默认为天数 如：开始时间和结束时间，不能超出距离当前时间90天
	 *
	 * @param startDate
	 *            开始时间
	 * @param endDate
	 *            结束时间按
	 * @param interval
	 *            间隔数
	 * @return
	 */
	public static boolean isOverIntervalLimit(String startDateStr, String endDateStr, int interval) {
		Date startDate = null;
		Date endDate = null;
		try {
			startDate = DateUtils.parseDate(startDateStr, DateUtils.DATE_FORMAT_DATEONLY);
			endDate = DateUtils.parseDate(endDateStr, DateUtils.DATE_FORMAT_DATEONLY);
		} catch (ParseException e) {
			logger.error(e);
			return false;
		}
		return isOverIntervalLimit(startDate, endDate, interval);
	}

	/**
	 * 传入时间字符串及时间格式，返回对应的Date对象
	 *
	 * @param src
	 *            时间字符串
	 * @param pattern
	 *            时间格式
	 * @return Date
	 */
	public static java.util.Date getDateFromString(String src, String pattern) {
		SimpleDateFormat f = new SimpleDateFormat(pattern);
		try {
			return f.parse(src);
		} catch (ParseException e) {
			return null;
		}
	}

	/**
	 * 取季度
	 *
	 * @param date
	 * @return
	 */
	@SuppressWarnings("deprecation")
	public static int getQuarter(Date date) {
		if (date.getMonth() == 0 || date.getMonth() == 1 || date.getMonth() == 2) {
			return 1;
		} else if (date.getMonth() == 3 || date.getMonth() == 4 || date.getMonth() == 5) {
			return 2;
		} else if (date.getMonth() == 6 || date.getMonth() == 7 || date.getMonth() == 8) {
			return 3;
		} else if (date.getMonth() == 9 || date.getMonth() == 10 || date.getMonth() == 11) {
			return 4;
		} else {
			return 0;

		}
	}

	/**
	 * 取得通用日期时间格式字符串
	 *
	 * @param Date
	 * @return String
	 */
	public static String formatDate(Date date) {
		if (date == null) {
			return "";
		}
		SimpleDateFormat dateFormat = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		return dateFormat.format(date);
	}

	/**
	 * 获取当日的日期格式串
	 *
	 * @param
	 * @return String
	 */
	public static String today() {
		return formatDate(new Date(), "yyyy-MM-dd");
	}

	/**
	 * 获取当前时间格式串
	 *
	 * @param
	 * @return String
	 */
	public static String currentTime() {
		return formatDate(new Date(), "yyyyMMddhhmmssSSS");
	}

	/**
	 * 取得指定日期格式的字符串
	 *
	 * @param Date
	 * @return String
	 */
	public static String formatDate(Date date, String format) {
		SimpleDateFormat dateFormat = new SimpleDateFormat(format);
		return dateFormat.format(date);
	}

	/**
	 * 获取昨日的日期格式串
	 *
	 * @return Date
	 */
	public static String getYesterday() {
		Calendar calendar = Calendar.getInstance();
		calendar.add(Calendar.DATE, -1);
		return formatDate(calendar.getTime(), "yyyy-MM-dd");
	}

	/**
	 * 判断当前时间是否在一定的时间范围内
	 *
	 * @param String
	 * @return boolean
	 */
	public static boolean isInBetweenTimes(String startTime, String endTime) {
		Date nowTime = new Date();
		SimpleDateFormat sdf = new SimpleDateFormat("HH:mm:ss");
		String time = sdf.format(nowTime);
		if (time.compareTo(startTime) >= 0 && time.compareTo(endTime) <= 0) {
			return true;
		} else {
			return false;
		}
	}

	/**
	 * 字符转日期
	 *
	 * @param dateStr
	 * @return
	 */
	public static Date getDateByStr(String dateStr) {
		SimpleDateFormat formatter = null;
		if (dateStr == null) {
			return null;
		} else if (dateStr.length() == 10) {
			formatter = new SimpleDateFormat("yyyy-MM-dd");
		} else if (dateStr.length() == 16) {
			formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm");
		} else if (dateStr.length() == 19) {
			formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		} else if (dateStr.length() > 19) {
			dateStr = dateStr.substring(0, 19);
			formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
		} else {
			return null;
		}
		try {
			return formatter.parse(dateStr);
		} catch (ParseException e) {
			logger.error(e);
			return null;
		}
	}

	/**
	 * 根据传入的数字，输出相比现在days天的数据
	 *
	 * @param days
	 * @return Date
	 */
	public static Date getDate(int days) {
		Calendar calendar = Calendar.getInstance();
		calendar.add(Calendar.DATE, days);
		return calendar.getTime();
	}

	/**
	 * 日期最大时间
	 *
	 * @param dt
	 * @return
	 */
	public static Date getMaxTime(Date dt) {

		Date dt1 = null;
		Calendar ca = Calendar.getInstance();
		ca.setTime(dt);
		ca.add(Calendar.DAY_OF_MONTH, 1);
		dt1 = ca.getTime();
		dt1 = DateUtils.getMinTime(dt1);
		ca.setTime(dt1);
		ca.add(Calendar.SECOND, -1);
		dt1 = ca.getTime();
		return dt1;
	}

	/**
	 * 日期最小时间
	 *
	 * @param dt
	 * @return
	 */
	public static Date getMinTime(Date dt) {
		Date dt1 = null;
		dt1 = DateUtils.getDateByStr(DateUtils.formatDate(dt, "yyyy-MM-dd"));
		return dt1;
	}

	/**
	 * 月的最后一天
	 *
	 * @param sDate1
	 * @return
	 */
	@SuppressWarnings("deprecation")
	public static Date getLastDayOfMonth(Date date) {
		Calendar cDay1 = Calendar.getInstance();
		cDay1.setTime(date);
		int lastDay = cDay1.getActualMaximum(Calendar.DAY_OF_MONTH);
		Date lastDate = cDay1.getTime();
		lastDate.setDate(lastDay);
		return lastDate;
	}

	/**
	 * 月的第一天
	 *
	 * @param date
	 * @return
	 */
	public static Date getFirstDayOfMonth(Date date) {
		Calendar calendar = Calendar.getInstance();
		calendar.setTime(date);
		calendar.set(Calendar.DATE, calendar.getActualMinimum(Calendar.DATE));
		return calendar.getTime();
	}

	/**
	 * 上月第一天
	 *
	 * @return
	 */
	public static Date getPreviousMonthFirstDay() {
		Calendar lastDate = Calendar.getInstance();
		lastDate.set(Calendar.DATE, 1);// 设为当前月的1号
		lastDate.add(Calendar.MONTH, -1);// 减一个月，变为下月的1号
		return getMinTime(lastDate.getTime());
	}

	/**
	 * 上月最后一天
	 *
	 * @return
	 */
	public static Date getPreviousMonthLastDay() {
		Calendar lastDate = Calendar.getInstance();
		lastDate.set(Calendar.DATE, 1);// 设为当前月的1号
		lastDate.add(Calendar.DATE, -1);
		return getMinTime(lastDate.getTime());
	}

	/**
	 * 两个日期相关天数
	 *
	 * @param startDate
	 * @param endDate
	 * @return
	 */
	public static long getDateDiff(String startDate, String endDate) {
		long diff = 0;
		try {
			Date date1 = new SimpleDateFormat("yyyy-MM-dd").parse(startDate);
			Date date2 = new SimpleDateFormat("yyyy-MM-dd").parse(endDate);

			diff = (date1.getTime() - date2.getTime()) / (24 * 60 * 60 * 1000) > 0
					? (date1.getTime() - date2.getTime()) / (24 * 60 * 60 * 1000)
					: (date2.getTime() - date1.getTime()) / (24 * 60 * 60 * 1000);
		} catch (ParseException e) {
		}
		return diff;
	}
	
	public static boolean isEmpty(Object obj){
		return null==obj;
	}

	public static long getDateDiff(Date date1, Date date2) {
		if (DateUtils.isEmpty(date1) || DateUtils.isEmpty(date1)) {
			return 0L;
		}
		long diff = (date1.getTime() - date2.getTime()) / (24 * 60 * 60 * 1000) > 0
				? (date1.getTime() - date2.getTime()) / (24 * 60 * 60 * 1000)
				: (date2.getTime() - date1.getTime()) / (24 * 60 * 60 * 1000);
		return diff;
	}

	/*
	 * 判断两个时间是不是在一个周中
	 */
	public static boolean isSameWeekWithToday(Date date) {

		if (date == null) {
			return false;
		}

		// 0.先把Date类型的对象转换Calendar类型的对象
		Calendar todayCal = Calendar.getInstance();
		Calendar dateCal = Calendar.getInstance();

		todayCal.setTime(new Date());
		dateCal.setTime(date);
		int subYear = todayCal.get(Calendar.YEAR) - dateCal.get(Calendar.YEAR);
		// subYear==0,说明是同一年
		if (subYear == 0) {
			if (todayCal.get(Calendar.WEEK_OF_YEAR) == dateCal.get(Calendar.WEEK_OF_YEAR))
				return true;
		} else if (subYear == 1 && dateCal.get(Calendar.MONTH) == 11 && todayCal.get(Calendar.MONTH) == 0) {
			if (todayCal.get(Calendar.WEEK_OF_YEAR) == dateCal.get(Calendar.WEEK_OF_YEAR))
				return true;
		} else if (subYear == -1 && todayCal.get(Calendar.MONTH) == 11 && dateCal.get(Calendar.MONTH) == 0) {
			if (todayCal.get(Calendar.WEEK_OF_YEAR) == dateCal.get(Calendar.WEEK_OF_YEAR))
				return true;
		}
		return false;
	}

	/**
	 * getStrFormTime:
	 *
	 * @param form
	 *            格式时间
	 * @param date
	 *            时间
	 * @return
	 */
	public static String getStrFormTime(String form, Date date) {
		SimpleDateFormat sdf = new SimpleDateFormat(form);
		return sdf.format(date);
	}

	/**
	 * 获取几天内日期 return 2014-5-4、2014-5-3
	 */
	public static List<String> getLastDays(int countDay) {
		List<String> listDate = new ArrayList<String>();
		for (int i = 0; i < countDay; i++) {
			listDate.add(DateUtils.getReqDateyyyyMMdd(DateUtils.getDate(-i)));
		}
		return listDate;
	}

	/**
	 * 对时间进行格式化
	 *
	 * @param date
	 * @return
	 */
	public static Date dateFormat(Date date) {
		SimpleDateFormat sdf = new SimpleDateFormat("yyyy-MM-dd");
		Date value = new Date();

		try {
			value = sdf.parse(sdf.format(date));
		} catch (ParseException e) {
			e.printStackTrace();
		}

		return value;

	}
	
	/**
     * 获取两个日期之间的所有日期（yyyy-MM-dd）
     * @Description TODO
     * @param begin
     * @param end
     * @return
     * @author XuJD
     * @date 2017-4-1
     */
	public static List<Date> getBetweenDates(Date begin, Date end) {
		List<Date> result = new ArrayList<Date>();
		Calendar tempStart = Calendar.getInstance();
		tempStart.setTime(begin);
		/*Calendar tempEnd = Calendar.getInstance();
		tempStart.add(Calendar.DAY_OF_YEAR, 1);
		tempEnd.setTime(end);
		while (tempStart.before(tempEnd)) {
			result.add(tempStart.getTime());
			tempStart.add(Calendar.DAY_OF_YEAR, 1);
		}*/
		while (begin.getTime() <= end.getTime()) {
			result.add(tempStart.getTime());
			tempStart.add(Calendar.DAY_OF_YEAR, 1);
			begin = tempStart.getTime();
		}
		return result;
	}

	public static void main(String[] args) {
		//Date date3 = DateUtils.addMinutes(new Date(), 5);
		System.out.println(new Date().after(DateUtils.addMinute(new Date(), -10)));
		Date date1 = DateUtils.addMinutes(new Date(), -1);
		Date date2 = DateUtils.addMinutes(date1, 5);
		System.out.println(date1);
		System.out.println(date2);
		System.out.println(DateUtils.isBetween(new Date(), date1, date2, DateUtils.COMP_MODEL_TIME));

		Integer a = 1;
		int b = 1;
		System.out.println(a == b);
	}
}
```




### 修改schema.xml子表配置

我们简单创建一个表t_sutable,里面只有两个字段：id（主键自增）和name。




```
<!-- 注意这里只支持一个dataNode值配置 注释错误写法-->
<!-- <table name="t_subtable" primaryKey="id" autoIncrement="true" subTables="t_subtable_$(20170730,20170805)"  dataNode="dn1,dn2"  /> -->
```
`<table name="t_subtable" primaryKey="id" autoIncrement="true" subTables="t_subtable_$(20170730,20170805)"  dataNode="dn1"  />`
注：mycat1.6目前不支持既分库又分表。


### 按日分表实现效果测试

下面是通过Mycat查询的结果：


![](https://img-blog.csdn.net/20170720123106541)





我们来看子表：

![](https://img-blog.csdn.net/20170720123254628)

为了让你对既分库又分表有深刻的认识，我们配置dataNode="dn1,dn2":



```
<!-- 注意这里只支持一个dataNode值配置 注释错误写法-->
<!-- <table name="t_subtable" primaryKey="id" autoIncrement="true" subTables="t_subtable_$(20170730,20170806)"  dataNode="dn1,dn2"  /> -->
```
`<table name="t_subtable" primaryKey="id" autoIncrement="true" subTables="t_subtable_$(20170730,20170806)"  dataNode="dn1"  />`



得到如下图结果：

![](https://img-blog.csdn.net/20170720123511932)

由此可见，mycat只支持单库分表。


### 结合MongoDB测试

修改schema.xml配置



```
<?xml version="1.0"?>
<!DOCTYPE mycat:schema SYSTEM "schema.dtd">
<mycat:schema xmlns:mycat="http://io.mycat/">

        <schema name="TESTDB" checkSQLschema="false" sqlMaxLimit="100">
             <table name="people" primaryKey="_ID"  dataNode="dn1" />
             <table name="t_subtable" primaryKey="_ID" subTables="t_subtable_$(20170730,20170805)" dataNode="dn1" />
        </schema>

        <dataNode name="dn1" dataHost="localhost1" database="mongo-test-db" /> 

        <dataHost name="localhost1" maxCon="1000" minCon="1" balance="0" writeType="0" dbType="mongodb" dbDriver="jdbc">
			<heartbeat>select user()</heartbeat>
			<writeHost host="hostM" url="mongodb://192.168.1.4/" user="admin" password="123456" ></writeHost>
		</dataHost>
		
</mycat:schema>
```

创建主表及子表并添加测试数据：


```java
/**
	 * 创建集合
	 */
	@Test
	public void createCollection() throws Exception {
		// 连接到 mongodb 服务
		@SuppressWarnings("resource")
		MongoClient mongoClient = new MongoClient("127.0.0.1", 27017);
		// 连接到数据库
		MongoDatabase mongoDatabase = mongoClient.getDatabase("mongo-test-db");
		System.out.println("Connect to database successfully");
		//mongoDatabase.createCollection("people");
		//mongoDatabase.createCollection("t_subtable");
		int [] arrays=new int []{20170730,20170731,20170801,20170802,20170803,20170804,20170805};
		for (int i = 0,j=arrays.length; i < j; i++) {
			mongoDatabase.createCollection("t_subtable_"+arrays[i]);
		}
		System.out.println("集合创建成功");
	}
```


```java
/**
	 * 插入文档
	 */
	@Test
	public void addDocument() throws Exception {
		// 连接到 mongodb 服务
		@SuppressWarnings("resource")
		MongoClient mongoClient = new MongoClient("127.0.0.1", 27017);
		// 连接到数据库
		MongoDatabase mongoDatabase = mongoClient.getDatabase("mongo-test-db");
		System.out.println("Connect to database successfully");
		MongoCollection<Document> collection = mongoDatabase.getCollection("test");
		System.out.println("集合 test 选择成功");
		// 插入文档
		/**
		 * 1. 创建文档 org.bson.Document 参数为key-value的格式 2. 创建文档集合List<Document> 3.
		 * 将文档集合插入数据库集合中 mongoCollection.insertMany(List<Document>) 插入单个文档可以用
		 * mongoCollection.insertOne(Document)
		 **/
		Document document = new Document("title", "mongo").append("description", "database").append("likes", 100)
				.append("by", "Fly");
		List<Document> documents = new ArrayList<Document>();
		documents.add(document);
		System.out.println("文档插入成功");
		collection.insertMany(documents);
		
		int [] arrays=new int []{20170730,20170731,20170801,20170802,20170803,20170804,20170805};
		for (int i = 0,j=arrays.length; i < j; i++) {
			String table="t_subtable_"+arrays[i];
			Document document2 = new Document("title", "mongo")
					.append("name", "boonya")
					.append("table", table)
					.append("description", "database")
					.append("likes", 100)
					.append("by", "Fly");
			MongoCollection<Document> collection2 = mongoDatabase.getCollection(table);
			collection2.insertOne(document2); 
		}
		
	}
```

Mycat SQL语句查询结果：

![](https://img-blog.csdn.net/20170720170833554)

通过mycat用代码来测试：

![](https://img-blog.csdn.net/20170720171304094)


唯一不足的是Mycat对客户端的支持不是很好，双击的时候不能正常显示主表的数据：

![](https://img-blog.csdn.net/20170720171226191)





## 参考资料

[MyCAT源码分析：调试环境搭建](https://github.com/YunaiV/Blog/blob/master/Database/MyCAT/1001-MyCAT%E6%BA%90%E7%A0%81%E5%88%86%E6%9E%90%EF%BC%9A%E8%B0%83%E8%AF%95%E7%8E%AF%E5%A2%83%E6%90%AD%E5%BB%BA.md)




[源代码方式调试Mycat](http://www.cnblogs.com/756623607-zhang/p/6850904.html?utm_source=itdadao&utm_medium=referral)





[Mycat源码篇 : 起步，Mycat源码阅读调试环境搭建](http://blog.csdn.net/d6619309/article/details/52746430)










