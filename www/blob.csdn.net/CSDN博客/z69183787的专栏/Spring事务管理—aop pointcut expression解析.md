# Spring事务管理—aop:pointcut expression解析 - z69183787的专栏 - CSDN博客
2013年12月05日 10:11:29[OkidoGreen](https://me.csdn.net/z69183787)阅读数：5306
**先来看看这个spring的配置文件的配置：**
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
   id="allDaoMethod" />
  <aop:advisor advice-ref="txAdvice" pointcut-ref="allDaoMethod" />
 </aop:config>
表示com.xy.service包下的所有方法为为事务管理。
**execution(* com.aptech.jb.epet.dao.hibimpl.*.*(..)) **
这样写应该就可以了 这是com.aptech.jb.epet.dao.hibimpl 包下所有的类的所有方法。。
第一个*代表所有的返回值类型 
第二个*代表所有的类
第三个*代表类所有方法 最后一个..代表所有的参数。
下面给出一些常见切入点表达式的例子：
- 
任意公共方法的执行：
execution(public * *(..))
- 
任何一个以“set”开始的方法的执行：
execution(* set*(..))
- 
`AccountService` 接口的任意方法的执行：
execution(* com.xyz.service.AccountService.*(..))
- 
定义在service包里的任意方法的执行：
execution(* com.xyz.service.*.*(..))
- 
定义在service包或者子包里的任意类的任意方法的执行：
execution(* com.xyz.service..*.*(..))
