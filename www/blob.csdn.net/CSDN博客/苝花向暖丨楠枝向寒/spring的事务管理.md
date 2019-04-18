# spring的事务管理 - 苝花向暖丨楠枝向寒 - CSDN博客

2018年07月18日 20:33:54[苝花向暖丨楠枝向寒](https://me.csdn.net/weixin_40247263)阅读数：30标签：[spring																[tx																[事务管理](https://so.csdn.net/so/search/s.do?q=事务管理&t=blog)](https://so.csdn.net/so/search/s.do?q=tx&t=blog)](https://so.csdn.net/so/search/s.do?q=spring&t=blog)
个人分类：[框架](https://blog.csdn.net/weixin_40247263/article/category/7511186)


```
<bean class="com.zzh.core.MyPropertyPlaceholderConfigurer"> 
		<property name="location" value="classpath:config/db.properties"></property>
	</bean>	
	<bean id="dataSource" class="com.alibaba.druid.pool.DruidDataSource" destroy-            
    method="close">
		<property name="driverClassName" value="${driver}"></property>
		<property name="url" value="${url}"></property>
		<property name="username" value="${user}"></property>
		<property name="password" value="${pwd}"></property>
	</bean>
```

```
<bean id="txManager"         
    class="org.springframework.jdbc.datasource.DataSourceTransactionManager">
		<property name="dataSource" ref="dataSource"></property>
	</bean>
	<tx:annotation-driven transaction-manager="txManager"/>
```

上面配置的是JDBC的连接、下面配置的是事务管理，其中事务管理的 ref 为 我们配置的jdbc的bean节点的id

使用的时候只需要在需要事务控制的方法上面加上@Transactional 注释即可，如果在类名上面加，则是这个类中的所有的方法均进行事务管理。


