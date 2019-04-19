# Spring事务管理—AOP/Annotation - 零度的博客专栏 - CSDN博客
2016年04月21日 11:18:00[零度anngle](https://me.csdn.net/zmx729618)阅读数：509
### 1、spring AOP 的配置文件的配置：
```
<!-- 事务管理器 -->
  <bean id="transactionManager"
   class="org.springframework.orm.hibernate3.HibernateTransactionManager">
   <property name="sessionFactory" ref="sessionFactory" />
  </bean>
  <!-- 配置事务的传播特性 -->
  <tx:advice id="txAdvice" transaction-manager="transactionManager">
   <tx:attributes>
    <tx:method name="get*" propagation="REQUIRED" read-only="true" />
    <tx:method name="del*" propagation="REQUIRED" />
    <tx:method name="save*" propagation="REQUIRED" />
    <tx:method name="update*" propagation="REQUIRED" />
   </tx:attributes>
  </tx:advice>
  <!-- 配置事务拦截器拦截哪些类的哪些方法,一般设置成拦截Service -->
  <aop:config>
   <aop:pointcut expression="execution(* com.xy.service.*.*(..))"
    id="allMethod" />
   <aop:advisor advice-ref="txAdvice" pointcut-ref="allMethod" />
  </aop:config>
```
表示com.xy.service包下的所有方法为为事务管理。
### 2、Spring annotation事务管理
```
<!-- sessionFactory -->
    <bean id="sessionFactory" class="org.springframework.orm.hibernate4.LocalSessionFactoryBean">
		<property name="dataSource">
			<ref bean="dataSource" />
		</property>
		<property name="hibernateProperties">
			<props>
				<prop key="hibernate.dialect">${hibernate.dialect}</prop>
				<prop key="hibernate.show_sql">${hibernate.show_sql}</prop>
				<prop key="hibernate.format_sql">${hibernate.format_sql}</prop>
				<prop key="hibernate.cache.use_second_level_cache">${hibernate.use_second_level_cache}</prop>
				<prop key="hibernate.cache.use_query_cache">${hibernate.use_query_cache}</prop>
				<prop key="hibernate.cache.region.factory_class">org.hibernate.cache.ehcache.EhCacheRegionFactory</prop> 
				<prop key="hibernate.hbm2ddl.auto">${hibernate.hbm2ddl}</prop>
			</props>
		</property>
		<property name="packagesToScan">
			<list>
			      <value>org.nercita.bcp.**.domain**</value>  
			</list>
		</property>
	</bean>
    <!-- 事务管理器 -->
 	<bean id="transactionManager" class="org.springframework.orm.hibernate4.HibernateTransactionManager">
		<property name="sessionFactory" ref="sessionFactory" />
	</bean> 
	
    <!-- 开启注解事务 -->
	<tx:annotation-driven transaction-manager="transactionManager" />
```
方法中注解@transactional()实现事务
