# Weblogic10.3.5 数据库连接问题 - z69183787的专栏 - CSDN博客
2014年07月25日 10:54:31[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2264
解决方式：
、看看你是不是连接有没有释放的地方，在http://localhost:7001/console部署里面可以看到 
二、设置weblogic的连接池属性 inactive Connection Timeout； 
三、要不你换换jdbc的jar包
就是Weblogic的inactive Connection Timeout属性设置问题，数据库的数据量太大，SQL执行的时间过长，而该属性配置值是180  让我改完0 就好了  谢谢拉
0为不超时。
附上官方文档：
The number of inactive seconds on a reserved connection before WebLogic Server reclaims the connection and releases it back into the connection pool.
You can use the Inactive Connection Timeout feature to reclaim leaked connections - connections that were not explicitly closed by the application. Note that this feature is not intended to be used in place of properly closing connections.
When set to `0`, the feature is disabled.
MBean Attribute (Does not apply to application modules) : 
`[JDBCConnectionPoolParamsBean.InactiveConnectionTimeoutSeconds](http://download.oracle.com/docs/cd/E21764_01/apirefs.1111/e13951/mbeans/JDBCConnectionPoolParamsBean.html#InactiveConnectionTimeoutSeconds)`
Minimum value: `0`
Maximum value: `2147483647`
近期发现了Weblogic10.3.5 数据库连接问题，首先介绍一个JDBC参数InactiveConnectionTImeout，该参数用于强制回收那些被泄露的连接(长时间不使用的连接、未被程序正确释放的连接)，避免连接池因为泄露导致无连接可用。当然该参数只是起辅助作用，解决问题知道还是在于完善应用程序。
　　本文和该参数有关，早期的8.1\9.2中，设定该参数时，定时器(内部的连接池维护Task)只会清理那些长期未被使用的空闲连接(从名字上可以看出来)，即对于运行一个长时间执行SQL的连接而言，它是不受影响的。但不知道从10.3哪个版本开始(我测试的是10.3.5)，该参数对于长时间执行SQL的连接也进行强制回收了（这个比较不合理，虽然对于正在执行的Tx影响不大）。Debug了一下，发现SQL执行时间大于4*InactiveConnectionTimeout时，这个SQL执行完成时，会出现
 <BEA-001153> <Forcibly releasing inactive connection "weblogic.jdbc.wrapper.PoolConnection_oracle_jdbc_driver_T4CConnection@1" back into the connection pool "TestDS", currently reserved by: java.lang.Exception这样的错误。
　　//该线程为内部Task执行线程，该线程目前处于被阻塞状态，等待SQL执行结束后回收连接
　　"[STUCK] ExecuteThread: '20' for queue: 'weblogic.kernel.Default (self-tuning)'" daemon prio=2 tid=0x2c4d3400 nid=0x1a14 waiting for monitor entry [0x
　　319bf000]
　　java.lang.Thread.State: BLOCKED (on object monitor)
　　at oracle.jdbc.driver.PhysicalConnection.rollback(PhysicalConnection.java:3896)
　　- waiting to lock <0x0e6191d8> (a oracle.jdbc.driver.T4CConnection)
　　at weblogic.jdbc.wrapper.Connection.forcedCleanup(Connection.java:156)
　　at weblogic.common.resourcepool.ResourcePoolImpl.timeoutInactiveResources(ResourcePoolImpl.java:1955)
　　at weblogic.common.resourcepool.ResourcePoolImpl.access$8(ResourcePoolImpl.java:1916)
　　at weblogic.common.resourcepool.ResourcePoolImpl$ResourcePoolMaintanenceTask.timerExpired(ResourcePoolImpl.java:2680)
　　at weblogic.timers.internal.TimerImpl.run(TimerImpl.java:273)
　　at weblogic.work.SelfTuningWorkManagerImpl$WorkAdapterImpl.run(SelfTuningWorkManagerImpl.java:528)
　　at weblogic.work.ExecuteThread.execute(ExecuteThread.java:209)
　　at weblogic.work.ExecuteThread.run(ExecuteThread.java:178)
　　//该线程为应用执行线程，目前该线程SQL正在运行，他阻塞（阻塞对象为T4CConnection）了内部Task执行线程
　　"[STUCK] ExecuteThread: '0' for queue: 'weblogic.kernel.Default (self-tuning)'" daemon prio=2 tid=0x2c7b4c00 nid=0x1100 runnable [0x2cf7e000]
　　java.lang.Thread.State: RUNNABLE
　　at java.net.SocketInputStream.socketRead0(Native Method)
　　......
　　at oracle.jdbc.driver.OracleStatement.doExecuteWithTimeout(OracleStatement.java:1315)
　　at oracle.jdbc.driver.OraclePreparedStatement.executeInternal(OraclePreparedStatement.java:3576)
　　at oracle.jdbc.driver.OraclePreparedStatement.execute(OraclePreparedStatement.java:3677)
　　- locked <0x0e6191d8> (a oracle.jdbc.driver.T4CConnection)
　　at oracle.jdbc.driver.OraclePreparedStatementWrapper.execute(OraclePreparedStatementWrapper.java:1374)
　　at weblogic.jdbc.wrapper.PreparedStatement.execute(PreparedStatement.java:102)
　　at com.bea.cs.test.jdbc.DriverTest.inactiveTimeoutTest(DriverTest.java:537)
　　at jsp_servlet.__ds._jspService(__ds.java:86)
　　从上面的线程堆栈可以看到，内部Task线程正在调用connection的rollback，即回滚连接上的本地事务。如果应用线程上使用的连接设为auto commit的话，该SQL执行不会被rollback，否则该SQL虽然执行完成也会被rollback调(全局事务中，某个事务分支使用的连接一定是非auto commit的)，所以该问题对涉及全局事务的应用影响比较大。
　　下面我们在看看连接被回收的时间点为什么会出现在3-4个InactiveConnectionTimeout之间。当内部Task执行，发现到了InactiveConnectionTimeout时间点后，它会调用timeoutInactiveResources()方法开始进行连接清理。那么为什么正在执行SQL的连接会被标志为非活动连接呢？这里涉及连接状态的三个标志位，分别为：conn_in_use，connUsed，hang_state，这三个标志位是其中的关键（getUsed()和setUsed()方法中会进行标志位操作及读取，如果对于某个连接getUsed()返回false的话，该连接会被清理）。下面我们看看其中标志位的变化过程:
 SQL开始执行时，preInvokeHandler会被这三个标志位进行设定。
　　初始值如下：
　　conn_in_use-->true
　　hang_state-->conn_state_in_use
　　connUsed-->true
　　第一次InactiveConnectionTimeout检查，getUsed返回true，同时调用setUsed(false)，标志位变化如下：
　　conn_in_use-->true
　　hang_state-->conn_state_idle_suspect
　　connUsed-->true
　　第二次InactiveConnectionTimeout检查，getUsed返回true，同时调用setUsed(false)，标志位变化如下：
　　conn_in_use-->false
　　hang_state-->conn_state_hang_suspect
　　connUsed-->false
　　第三次InactiveConnectionTimeout检查，getUsed返回true，
　　hang_state-->conn_state_in_use（从conn_state_hang_suspect变成conn_state_in_use）
　　同时调用setUsed(false)，标志位变化如下：
　　conn_in_use-->false
　　connUsed-->false
　　hang_state-->conn_state_idle_suspect（从conn_state_in_use变成conn_state_idle_suspect）
　　第四次InactiveConnectionTimeout检查，getUsed返回false，开始回收该连接。
　　从上面的变化时间点可以看到从第一次到第四次检查经历了完整的3个InactiveConnectionTimout周期，而从SQL执行到第一次检查点，这个时间小于一个InactiveConnectionTimeout，所以连接开始被回收的时间点介于3-4个InactiveConnectionTimeout之间。
　　java软件学习中心与你共同的学习java开发，如果还有其他的发现可以和我联系。
