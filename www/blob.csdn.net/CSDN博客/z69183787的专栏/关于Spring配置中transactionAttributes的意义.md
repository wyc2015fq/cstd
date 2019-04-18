# 关于Spring配置中transactionAttributes的意义 - z69183787的专栏 - CSDN博客
2013年12月05日 09:57:05[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2414
各位大虾好，小弟最近在看Spring，有谁能解释一下下边的配置么？
<bean id="persister" 
    class="org.springframework.transaction.interceptor.TransactionProxyFactoryBean">
     <property name="transactionManager"><ref bean="transactionManager"/></property>
     <property name="target"><ref bean="refTarget"/></property>
     <property name="transactionAttributes">
         <props>
                    <prop key="myMethod">PROPAGATION_REQUIRED, -Exception</prop>
         </props>
     </property>
</bean>
最不明白的是这句话：
<prop key="myMethod">PROPAGATION_REQUIRED, -Exception</prop>
后边的-Exception是什么意思啊，另外，有的代码这行里边还有readonly,都代表什么意思？
#### 解决方案 »
- 
是事物控制的配置吧....
prop key是方法名,可以包含*等通配符,PROPAGATION_REQUIRED是常用选择,可以理解为greate or replace事物,readonly是设置操作权限为只读,
- 
-Exception忘记说了,这个代表抛出异常,则该方法回滚...-代表回滚+就代表提交....
- 
后边的-Exception是什么意思啊，另外，有的代码这行里边还有readonly,都代表什么意思？ 表示有Exception抛出时,事务回滚. readonly 就是read only, 一般用于查询的方法,优化作用.
**事务传播行为种类**
Spring在TransactionDefinition接口中规定了7种类型的事务传播行为，
它们规定了事务方法和事务方法发生嵌套调用时事务如何进行传播：
表1事务传播行为类型
|事务传播行为类型|说明|
|----|----|
|PROPAGATION_REQUIRED|如果当前没有事务，就新建一个事务，如果已经存在一个事务中，加入到这个事务中。这是最常见的选择。|
|PROPAGATION_SUPPORTS|支持当前事务，如果当前没有事务，就以非事务方式执行。|
|PROPAGATION_MANDATORY|使用当前的事务，如果当前没有事务，就抛出异常。|
|PROPAGATION_REQUIRES_NEW|新建事务，如果当前存在事务，把当前事务挂起。|
|PROPAGATION_NOT_SUPPORTED|以非事务方式执行操作，如果当前存在事务，就把当前事务挂起。|
|PROPAGATION_NEVER|以非事务方式执行，如果当前存在事务，则抛出异常。|
|PROPAGATION_NESTED|如果当前存在事务，则在嵌套事务内执行。如果当前没有事务，则执行与PROPAGATION_REQUIRED类似的操作。|
