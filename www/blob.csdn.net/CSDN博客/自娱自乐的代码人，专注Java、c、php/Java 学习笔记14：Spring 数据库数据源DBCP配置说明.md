# Java 学习笔记14：Spring 数据库数据源DBCP配置说明 - 自娱自乐的代码人，专注Java、c、php - CSDN博客





2012年12月04日 14:30:53[initphp](https://me.csdn.net/initphp)阅读数：13666
所属专栏：[Java学习笔记](https://blog.csdn.net/column/details/javago.html)









连接数据库都必须要有数据连接，一般Spring中采用DBCP配置源，通过在bean.xml中配置相应的数据配置，就可以连接数据库了，可以看一下以下相关配置：



```
<bean id="jdbc" class="org.springframework.beans.factory.config.PropertyPlaceholderConfigurer" >
    	<property name="locations" value="/WEB-INF/jdbc.properties"/>    
    </bean>
    <bean id="dataSource"  class="org.apache.commons.dbcp.BasicDataSource"  destroy-method="close">        
     <property name="driverClassName" value="${jdbc.driverClassName}" />       
     <property name="url" value="${jdbc.url}" />       
     <property name="username" value="${jdbc.username}" />       
     <property name="password" value="${jdbc.password}" />    
      <property name="testOnBorrow" value="false" />      
        <property name="testWhileIdle" value="true" />   
          <!-- 连接池启动时的初始值 -->
	  <property name="initialSize" value="10" />
	  <!-- 连接池的最大值 -->
	  <property name="maxActive" value="100" />
	  <!-- 最大空闲值.当经过一个高峰时间后，连接池可以慢慢将已经用不到的连接慢慢释放一部分，一直减少到maxIdle为止 -->
	  <property name="maxIdle" value="50" />
	  <!-- 最小空闲值.当空闲的连接数少于阀值时，连接池就会预申请去一些连接，以免洪峰来时来不及申请 -->
	  <property name="minIdle" value="10" />
	  <!--#给出一条简单的sql语句进行验证-->
	  <property name="validationQuery" value="select getdate()" />
	  <!--#在取出连接时进行有效验证-->
	  <property name="removeAbandonedTimeout" value="120" />
	  <property name="removeAbandoned" value="true" />
	  <!-- #运行判断连接超时任务的时间间隔，单位为毫秒，默认为-1，即不执行任务。 -->
	  <property name="timeBetweenEvictionRunsMillis" value="3600000" />
	  <!-- #连接的超时时间，默认为半小时。 -->
	  <property name="minEvictableIdleTimeMillis" value="3600000" />  
	</bean> 
	<bean id="JdbcTemplate" class="org.springframework.jdbc.core.JdbcTemplate">
        <property name="dataSource" ref="dataSource"/>
    </bean>
```


以上的配置，就直接可以使用JdbcTemplate来做数据库操作了。

下面是一个详细的配置表：

**常用链接配置：**



|参数|描述|
|----|----|
|username|传递给JDBC驱动的用于建立连接的用户名|
|password|传递给JDBC驱动的用于建立连接的密码|
|url|传递给JDBC驱动的用于建立连接的URL|
|driverClassName|使用的JDBC驱动的完整有效的java 类名|
|connectionProperties|当建立新连接时被发送给JDBC驱动的连接参数，格式必须是 [propertyName=property;]*注意 ：参数user/password将被明确传递，所以不需要包括在这里。|

**事务属性配置：**
|参数|默认值|描述|
|----|----|----|
|defaultAutoCommit|true|连接池创建的连接的默认的auto-commit状态|
|defaultReadOnly|driver default|连接池创建的连接的默认的read-only状态. 如果没有设置则setReadOnly方法将不会被调用. (某些驱动不支持只读模式,比如:Informix)|
|defaultTransactionIsolation|driver default|连接池创建的连接的默认的TransactionIsolation状态. 下面列表当中的某一个: (参考javadoc)- NONE - READ_COMMITTED - READ_UNCOMMITTED - REPEATABLE_READ - SERIALIZABLE|
|defaultCatalog||连接池创建的连接的默认的catalog|


**数据源连接数量配置：**


|参数|默认值|描述|
|----|----|----|
|initialSize|0|初始化连接:连接池启动时创建的初始化连接数量,1.2版本后支持|
|maxActive|8|最大活动连接:连接池在同一时间能够分配的最大活动连接的数量, 如果设置为非正数则表示不限制|
|maxIdle|8|最大空闲连接:连接池中容许保持空闲状态的最大连接数量,超过的空闲连接将被释放,如果设置为负数表示不限制|
|minIdle|0|最小空闲连接:连接池中容许保持空闲状态的最小连接数量,低于这个数量将创建新的连接,如果设置为0则不创建|
|maxWait|无限|最大等待时间:当没有可用连接时,连接池等待连接被归还的最大时间(以毫秒计数),超过时间则抛出异常,如果设置为-1表示无限等待|

**数据源连接健康状况检查：**


|参数|默认值|描述|
|----|----|----|
|validationQuery||SQL查询,用来验证从连接池取出的连接,在将连接返回给调用者之前.如果指定,则查询必须是一个SQL SELECT并且必须返回至少一行记录|
|testOnBorrow|true|指明是否在从池中取出连接前进行检验,如果检验失败,则从池中去除连接并尝试取出另一个.注意: 设置为true后如果要生效,validationQuery参数必须设置为非空字符串|
|testOnReturn|false|指明是否在归还到池中前进行检验 注意: 设置为true后如果要生效,validationQuery参数必须设置为非空字符串|
|testWhileIdle|false|指明连接是否被空闲连接回收器(如果有)进行检验.如果检测失败,则连接将被从池中去除.注意: 设置为true后如果要生效,validationQuery参数必须设置为非空字符串|
|timeBetweenEvictionRunsMillis|-1|在空闲连接回收器线程运行期间休眠的时间值,以毫秒为单位. 如果设置为非正数,则不运行空闲连接回收器线程|
|numTestsPerEvictionRun|3|在每次空闲连接回收器线程(如果有)运行时检查的连接数量|
|minEvictableIdleTimeMillis|1000 * 60 * 30|连接在池中保持空闲而不被空闲连接回收器线程(如果有)回收的最小时间值，单位毫秒|

**缓存语句：**


|参数|默认值|描述|
|----|----|----|
|poolPreparedStatements|false|开启池的prepared statement 池功能|
|maxOpenPreparedStatements|不限制|statement池能够同时分配的打开的statements的最大数量, 如果设置为0表示不限制|

**连接泄露回收：**


|参数|默认值|描述|
|----|----|----|
|removeAbandoned|false|标记是否删除泄露的连接,如果他们超过了removeAbandonedTimout的限制.如果设置为true, 连接被认为是被泄露并且可以被删除,如果空闲时间超过removeAbandonedTimeout. 设置为true可以为写法糟糕的没有关闭连接的程序修复数据库连接.|
|removeAbandonedTimeout|300|泄露的连接可以被删除的超时值, 单位秒|
|logAbandoned|false|标记当Statement或连接被泄露时是否打印程序的stack traces日志。被泄露的Statements和连接的日志添加在每个连接打开或者生成新的Statement,因为需要生成stack trace。|




**注意：**
- Java数据库连接有“8小时问题”，所以destroy-method="close"一定要加上。“8小时问题”是指一个连接空闲8小时数据库会自动关闭，而数据源并不知道。
- 高并发下，可以testOnBorrow设置false，testWhileIdle设置为true，这样就会定时对后台空链接进行检测发现无用连接就会清除掉，不会每次都去都去检测是否8小时的空链接。






