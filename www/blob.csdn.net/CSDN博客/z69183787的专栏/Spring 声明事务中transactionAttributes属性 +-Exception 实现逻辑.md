# Spring 声明事务中transactionAttributes属性 + - Exception 实现逻辑 - z69183787的专栏 - CSDN博客
2013年12月06日 09:30:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1944
下面是一段典型的Spring **声明事务**的配置：
[查看文本](http://www.verydemo.com/demo_c143_i31690.html#)[打印](http://www.verydemo.com/demo_c143_i31690.html#)[?](http://www.verydemo.com/demo_c143_i31690.html#)
- <beanid="userDAOProxy"
- class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
- <propertyname="transactionManager">
- <refbean="transactionManager"/>
- </property>
- <propertyname="target">
- <reflocal="userDAO"/>
- </property>
- <propertyname="transactionAttributes">
- <props>
- <propkey="insert*">PROPAGATION_REQUIRED</prop>
- <propkey="get*">PROPAGATION_REQUIRED,readOnly</prop>
- <propkey="save*">PROPAGATION_REQUIRED,-ApplicationException,+BusinessException</prop>
- </props>
- </property>
- </bean>
在Spring**声明事务**中，我们可以自定义方法的哪些Exception需要回滚，哪些Exception可以直接提交。
通过下面的配置：
[查看文本](http://www.verydemo.com/demo_c143_i31690.html#)[打印](http://www.verydemo.com/demo_c143_i31690.html#)[?](http://www.verydemo.com/demo_c143_i31690.html#)
- <propkey="save*">PROPAGATION_REQUIRED,-ApplicationException,+BusinessException</prop>
- 表示抛出该异常时需要回滚
+表示即使抛出该异常**事务**同样要提交
-ApplicationException ：表示抛出ApplicationException 时，**事务**需要回滚。但不是说只抛出ApplicationException 异常时，**事务**才回滚，如果程序抛出RuntimeException和Error时，**事务**一样会回滚，即使这里没有配置。因为Spring中默认对所有的RuntimeException和Error都会回滚**事务**。
Spring中是如何**实现**这段**逻辑**的：
调用的是
**org.springframework.transaction.interceptor.RuleBasedTransactionAttribute.rollbackOn(Throwable ex)**
[查看文本](http://www.verydemo.com/demo_c143_i31690.html#)[打印](http://www.verydemo.com/demo_c143_i31690.html#)[?](http://www.verydemo.com/demo_c143_i31690.html#)
- publicboolean rollbackOn(Throwable ex) {  
- if (logger.isTraceEnabled()) {  
-             logger.trace("Applying rules to determine whether transaction should rollback on " + ex);  
-         }  
- 
-         RollbackRuleAttribute winner = null;  
- int deepest = Integer.MAX_VALUE;  
- 
- //配置文件中的回滚异常列表，当然去掉了-，只有name，commit的规则是另外一个对象
- if (this.rollbackRules != null) {  
- for (RollbackRuleAttribute rule : this.rollbackRules) {  
- //使用抛出exception的className(全路径className)进行indexOf match
- //如果没有match上会继续搜索superClass name进行match，到Throwable class为止
- int depth = rule.getDepth(ex);  
- if (depth >= 0 && depth < deepest) {  
-                     deepest = depth;  
-                     winner = rule;  
-                 }  
-             }  
-         }  
- 
- if (logger.isTraceEnabled()) {  
-             logger.trace("Winning rollback rule is: " + winner);  
-         }  
- 
- // User superclass behavior (rollback on unchecked) if no rule matches.
- if (winner == null) {  
-             logger.trace("No relevant rollback rule found: applying default rules");  
- //如果没有match上，调用此方法继续match，判断instance of RuntimeException or Error
- returnsuper.rollbackOn(ex);  
-         }  
- 
- return !(winner instanceof NoRollbackRuleAttribute);  
-     }  
**rule.getDepth**方法代码
        因为使用的是className的全路径进行indexOf匹配，所以如果自定义异常是：com.abc.ApplicationException，你在xml配置文件中定义为：-abc，同样会match上，**事务**也会回滚，这一点需要注意。
       另外一点，如何在xml中定义的是-Exception，这样只要class的全路径中包含Exception字段，如包名，也会匹配上。
[查看文本](http://www.verydemo.com/demo_c143_i31690.html#)[打印](http://www.verydemo.com/demo_c143_i31690.html#)[?](http://www.verydemo.com/demo_c143_i31690.html#)
- publicint getDepth(Throwable ex) {  
- return getDepth(ex.getClass(), 0);  
- }  
- 
- 
- privateint getDepth(Class exceptionClass, int depth) {  
- if (exceptionClass.getName().indexOf(this.exceptionName) != -1) {  
- // Found it!
- return depth;  
-     }  
- // If we've gone as far as we can go and haven't found it...
- if (exceptionClass.equals(Throwable.class)) {  
- return -1;  
-     }  
- return getDepth(exceptionClass.getSuperclass(), depth + 1);  
- }  
super.rollbackOn(Throwable ex**)** 方法代码
很简单的一行代码，这就是为什么RuntimeException和Error也会回滚啦。
[查看文本](http://www.verydemo.com/demo_c143_i31690.html#)[打印](http://www.verydemo.com/demo_c143_i31690.html#)[?](http://www.verydemo.com/demo_c143_i31690.html#)
- publicboolean rollbackOn(Throwable ex) {  
- return (ex instanceof RuntimeException || ex instanceof Error);  
-     }  
**几次测试输出的debug日志**：
```
[08/24 03:35:39] [DEBUG] RuleBasedTransactionAttribute: Applying rules to determine whether transaction should rollback on usertest.exception.BusinessException: Error
[08/24 03:35:39] [DEBUG] RuleBasedTransactionAttribute: Winning rollback rule is: RollbackRuleAttribute with pattern [BusinessException]
[08/24 03:35:39] [DEBUG] DataSourceTransactionManager: Triggering beforeCompletion synchronization
[08/24 03:35:39] [DEBUG] DataSourceTransactionManager: Initiating transaction rollback
[08/24 03:35:39] [DEBUG] DataSourceTransactionManager: Rolling back JDBC transaction on Connection [org.apache.commons.dbcp.PoolableConnection@1db05b2]
[08/24 03:35:39] [DEBUG] DataSourceTransactionManager: Triggering afterCompletion synchronization
[08/24 03:35:39] [DEBUG] TransactionSynchronizationManager: Clearing transaction synchronization
[08/24 03:35:39] [DEBUG] TransactionSynchronizationManager: Removed value [org.springframework.jdbc.datasource.ConnectionHolder@1833eca] for key [org.apache.commons.dbcp.BasicDataSource@4aa0ce] from thread [main]
[08/24 03:35:39] [DEBUG] DataSourceTransactionManager: Releasing JDBC Connection [org.apache.commons.dbcp.PoolableConnection@1db05b2] after transaction
[08/24 03:35:39] [DEBUG] DataSourceUtils: Returning JDBC Connection to DataSource
[08/24 03:39:16] [DEBUG] TransactionInterceptor: Completing transaction for [usertest.dao.UsersDAO.testInsertAndUpdate] after exception: java.lang.Exception: Error
[08/24 03:39:16] [DEBUG] RuleBasedTransactionAttribute: Applying rules to determine whether transaction should rollback on java.lang.Exception: Error
[08/24 03:39:16] [DEBUG] RuleBasedTransactionAttribute: Winning rollback rule is: null
[08/24 03:39:16] [DEBUG] RuleBasedTransactionAttribute: No relevant rollback rule found: applying superclass default
[08/24 03:39:16] [DEBUG] DataSourceTransactionManager: Triggering beforeCommit synchronization
[08/24 03:39:16] [DEBUG] DataSourceTransactionManager: Triggering beforeCompletion synchronization
[08/24 03:39:16] [DEBUG] DataSourceTransactionManager: Initiating transaction commit
[08/24 03:39:16] [DEBUG] DataSourceTransactionManager: Committing JDBC transaction on Connection [org.apache.commons.dbcp.PoolableConnection@1db05b2]
[08/24 03:39:16] [DEBUG] DataSourceTransactionManager: Triggering afterCommit synchronization
[08/24 03:41:40] [DEBUG] TransactionInterceptor: Completing transaction for [usertest.dao.UsersDAO.testInsertAndUpdate] after exception: usertest.exception.BusinessException: Error
[08/24 03:41:40] [DEBUG] RuleBasedTransactionAttribute: Applying rules to determine whether transaction should rollback on usertest.exception.BusinessException: Error
[08/24 03:41:40] [DEBUG] RuleBasedTransactionAttribute: Winning rollback rule is: RollbackRuleAttribute with pattern [Exception]
[08/24 03:41:40] [DEBUG] DataSourceTransactionManager: Triggering beforeCompletion synchronization
[08/24 03:41:40] [DEBUG] DataSourceTransactionManager: Initiating transaction rollback
[08/24 03:41:40] [DEBUG] DataSourceTransactionManager: Rolling back JDBC transaction on Connection [org.apache.commons.dbcp.PoolableConnection@1db05b2]
[08/24 03:41:40] [DEBUG] DataSourceTransactionManager: Triggering afterCompletion synchronization
[08/24 03:41:40] [DEBUG] TransactionSynchronizationManager: Clearing transaction synchronization
```
