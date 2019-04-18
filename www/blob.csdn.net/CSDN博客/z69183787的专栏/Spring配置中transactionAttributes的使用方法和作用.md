# Spring配置中transactionAttributes的使用方法和作用 - z69183787的专栏 - CSDN博客
2013年12月05日 10:00:07[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1083
最近碰到这个问题，在使用spring提供的JpaTemplate进行查询时，如果数据量超过100 条，查询效率就会明显降低。由于开始时使用JPA内部的双向关联，造成各实体内部关联过多，从而影响所有的操作，因此怀疑是因为JPA的关联关系所致。但 是去掉关联关系后的效果不显著。 
查找spring的相关配置，发现原来关于“transactionAttributes”有问题。原来的配置如下： 
<bean id="baseTransactionProxy" class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean" 
lazy-init="true" abstract="true"> 
<property name="transactionManager"> 
<ref bean="transactionManager" /> 
</property> 
<property name="transactionAttributes"> 
<props> 
<prop key="sav*">PROPAGATION_REQUIRED</prop> 
<prop key="update*">PROPAGATION_REQUIRED</prop> 
<prop key="delete*">PROPAGATION_REQUIRED</prop> 
<prop key="get*">PROPAGATION_REQUIRED,readOnly</prop> 
<prop key="find*">PROPAGATION_REQUIRED,readOnly</prop> 
</props> 
</property> 
</bean> 
使用上述配置，在JPA打出的日志中显示每次查询时都要进行更新操作，查阅相关spring 的资料后发现transactionAttributes的各种属性的意义，现把资料分享如下： 
PROPAGATION_REQUIRED--支持当前事务，如果当前没有事务，就新建一个事务。这是最常见的选择。 
PROPAGATION_SUPPORTS--支持当前事务，如果当前没有事务，就以非事务方式执行。 
PROPAGATION_MANDATORY--支持当前事务，如果当前没有事务，就抛出异常。 
PROPAGATION_REQUIRES_NEW--新建事务，如果当前存在事务，把当前事务挂起。 
PROPAGATION_NOT_SUPPORTED--以非事务方式执行操作，如果当前存在事务，就把当前事务挂起。 
PROPAGATION_NEVER--以非事务方式执行，如果当前存在事务，则抛出异常。 
PROPAGATION_NESTED--如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则进行与PROPAGATION_REQUIRED类似的操作。
当前所有的事务都使用“PROPAGATION_REQUIRED”属性值，并且控制事务的操作权限为只读，以保证查询时不会更新数据。根据上述 定义 “PROPAGATION_REQUIRED”属性会造成为所有的操作都创建事务，从而会出现JPA的日志中查询时也会进行更新操作的现象，也就造成了效 率的低下。将所有查询的操作改成事务类型为“PROPAGATION_NEVER”（不使用事务），则查询效率立即提升，但是此时担心一个问题：比如在一
 个saveXXX()的方法中，如果方法内部使用更新、查询、再更新的操作流程，会不会造成调用查询时，由于上述配置造成的抛出异常。 
另外，如果出现 
〈prop key="myMethod"〉PROPAGATION_REQUIRED,readOnly,-Exception〈/prop〉 
这样的配置，其中: 
-Exception表示有Exception抛出时,事务回滚. -代表回滚+就代表提交 
readonly 就是read only, 设置操作权限为只读,一般用于查询的方法,优化作用.
