# 基于JDK动态代理和CGLIB动态代理的实现Spring注解管理事务（@Trasactional）到底有什么区别。 - z69183787的专栏 - CSDN博客
2013年12月05日 10:35:18[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2744
### 基于JDK动态代理和CGLIB动态代理的实现Spring注解管理事务（@Trasactional）到底有什么区别。
### 我还是喜欢基于Schema风格的Spring事务管理，但也有很多人在用基于@Trasactional注解的事务管理，但在通过基于JDK动态代理和CGLIB动态代理的实现Spring注解管理事务是有区别的，我们接下来看看到底有哪些区别。
## 一、基础工作
### 首先修改我们上一次做的 [SpringMVC + spring3.1.1 + hibernate4.1.0 集成及常见问题总结](http://sishuok.com/forum/blogPost/list/2625.html)，如下所示：
    将xml声明式事务删除
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- <aop:config expose-proxy="true">  
-         <!-- 只对业务逻辑层实施事务 -->  
-         <aop:pointcut id="txPointcut" expression="execution(* cn.javass..service..*.*(..))" />  
-         <aop:advisor advice-ref="txAdvice" pointcut-ref="txPointcut"/>  
-     </aop:config>  
    并添加注解式事务支持：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- <tx:annotation-driven transaction-manager="txManager"/>  
    在我们的BaseService接口上添加 @Transactional 使该方法开启事务
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package cn.javass.common.service;  
- publicinterface IBaseService<M extends java.io.Serializable, PK extends java.io.Serializable> {  
- @Transactional//开启默认事务
- publicint countAll();  
- }    
在我们的log4j.properties中添加如下配置，表示输出spring的所有debug信息
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- log4j.logger.org.springframework=INFO,CONSOLE  
在我们的resources.properties里将hibernate.show_sql=true 改为true，为了看到hibernate的sql。
单元测试类：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package cn.javass.ssonline.spider.service.impl;  
- 
- import org.junit.Test;  
- import org.junit.runner.RunWith;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.test.context.ContextConfiguration;  
- import org.springframework.test.context.junit4.SpringJUnit4ClassRunner;  
- import org.springframework.test.context.transaction.TransactionConfiguration;  
- 
- import cn.javass.demo.service.UserService;  
- @RunWith(SpringJUnit4ClassRunner.class)  
- @ContextConfiguration(locations = {"classpath:spring-config.xml"})  
- publicclass UserServiceTest2 {  
- 
- @Autowired
- private UserService userService;  
- @Test
- publicvoid testCreate() {  
-        userService.countAll();  
-     }  
- }  
基础工作做好，接下来我们详细看看 Spring基于 JDK动态代理 和 CGLIB类级别代理到底有什么区别。
## 二、基于JDK动态代理：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- <tx:annotation-driven transaction-manager="txManager"/>  
   该配置方式默认就是JDK动态代理方式
运行单元测试，核心日志如下：                                     
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- 2012-03-0709:58:44 [main] DEBUG org.springframework.orm.hibernate4.HibernateTransactionManager - Creating new transaction with name [cn.javass.common.service.impl.BaseService.countAll]: PROPAGATION_REQUIRED,ISOLATION_DEFAULT; ''//开启事务
- 2012-03-0709:58:44 [main] DEBUG org.springframework.orm.hibernate4.HibernateTransactionManager - Opened new Session   
- 
- 2012-03-0709:58:44 [main] DEBUG org.springframework.transaction.support.TransactionSynchronizationManager - Bound value [org.springframework.orm.hibernate4.SessionHolder@1184a4f] for key [org.hibernate.internal.SessionFactoryImpl@107b56e] to thread [main] //绑定session到ThreadLocal
- 2012-03-0709:58:44 [main] DEBUG org.springframework.transaction.support.TransactionSynchronizationManager - Initializing transaction synchronization  
- 2012-03-0709:58:44 [main] DEBUG org.springframework.transaction.interceptor.TransactionInterceptor - Getting transaction for [cn.javass.common.service.impl.BaseService.countAll]  
- 2012-03-0709:58:44 [main] DEBUG org.springframework.transaction.support.TransactionSynchronizationManager - Retrieved value [org.springframework.orm.hibernate4.SessionHolder@1184a4f] for key [org.hibernate.internal.SessionFactoryImpl@107b56e] bound to thread [main]  
- 2012-03-0709:58:44 [main] DEBUG org.springframework.orm.hibernate4.HibernateTransactionManager - Found thread-bound Session   
- 
- 2012-03-0709:58:44 [main] DEBUG org.springframework.transaction.support.TransactionSynchronizationManager - Retrieved value [org.springframework.orm.hibernate4.SessionHolder@1184a4f] for key [org.hibernate.internal.SessionFactoryImpl@107b56e] bound to thread [main]  
- Hibernate:   
-     select  
-         count(*) as col_0_0_   
-     from  
-         tbl_user usermodel0_  
- 
- 2012-03-0709:58:44 [main] DEBUG org.springframework.orm.hibernate4.HibernateTransactionManager - Committing Hibernate transaction on Session    //提交事务
- 
- 2012-03-0709:58:44 [main] DEBUG org.springframework.transaction.support.TransactionSynchronizationManager - Removed value [org.springframework.orm.hibernate4.SessionHolder@1184a4f] for key [org.hibernate.internal.SessionFactoryImpl@107b56e] from thread [main] //解除绑定session到ThreadLocal
到此我们可以看到事务起作用了，也就是说即使把@Transactional放到接口上 基于JDK动态代理也是可以工作的。
## 三、基于CGLIB类代理：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- <tx:annotation-driven transaction-manager="txManager" proxy-target-class="true"/>  
   该配置方式是基于CGLIB类代理
启动测试会报错，No Session found for current thread，说明事务没有起作用
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- org.hibernate.HibernateException: No Session found for current thread  
-     at org.springframework.orm.hibernate4.SpringSessionContext.currentSession(SpringSessionContext.java:97)  
-     at org.hibernate.internal.SessionFactoryImpl.getCurrentSession(SessionFactoryImpl.java:1024)  
-     at cn.javass.common.dao.hibernate4.BaseHibernateDao.getSession(BaseHibernateDao.java:63)  
-     at cn.javass.common.dao.hibernate4.BaseHibernateDao.aggregate(BaseHibernateDao.java:238)  
-     at cn.javass.common.dao.hibernate4.BaseHibernateDao.countAll(BaseHibernateDao.java:114)  
-     at cn.javass.common.service.impl.BaseService.countAll(BaseService.java:60)  
-     at cn.javass.common.service.impl.BaseService$$FastClassByCGLIB$$5b04dd69.invoke(<generated>)  
-     at net.sf.cglib.proxy.MethodProxy.invoke(MethodProxy.java:149)  
-     at org.springframework.aop.framework.Cglib2AopProxy$DynamicAdvisedInterceptor.intercept(Cglib2AopProxy.java:618)  
-     at cn.javass.demo.service.impl.UserServiceImpl$$EnhancerByCGLIB$$7d46c567.countAll(<generated>)  
-     at cn.javass.ssonline.spider.service.impl.UserServiceTest2.testCreate(UserServiceTest2.java:20)  
如果将注解放在具体类上或具体类的实现方法上才会起作用。
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package cn.javass.common.service.impl;  
- publicabstractclass BaseService<M extends java.io.Serializable, PK extends java.io.Serializable> implements IBaseService<M, PK> {  
- 
- @Transactional()   //放在抽象类上
- @Override
- publicint countAll() {  
- return baseDao.countAll();  
-     }  
- }  
运行测试类，将发现成功了，因为我们的UserService继承该方法，但如果UserService覆盖该方法，如下所示，也将无法织入事务(报错)：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package cn.javass.demo.service.impl;  
- publicclass UserServiceImpl extends BaseService<UserModel, Integer> implements UserService {  
- //没有@Transactional
- @Override
- publicint countAll() {  
- return baseDao.countAll();  
-     }  
- }  
**四、基于aspectj的**
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- <tx:annotation-driven transaction-manager="txManager" mode="aspectj" proxy-target-class="true"/>  
在此就不演示了，我们主要分析基于JDK动态代理和CGLIB类代理两种的区别。
## 五、结论：
基于JDK动态代理 ，可以将@Transactional放置在接口和具体类上。
基于CGLIB类代理，只能将@Transactional放置在具体类上。
因此 在实际开发时全部将@Transactional放到具体类上，而不是接口上。
## 六、分析
1、  JDK动态代理
1.1、Spring使用JdkDynamicAopProxy实现代理：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package org.springframework.aop.framework;  
- finalclass JdkDynamicAopProxy implements AopProxy, InvocationHandler, Serializable {  
- //注意此处的method 一定是接口上的method（因此放置在接口上的@Transactional是可以发现的）
- public Object invoke(Object proxy, Method method, Object[] args) throws Throwable {  
-     }  
- }  
注意此处的method 一定是接口上的method（因此放置在接口上的@Transactional是可以发现的）
1.2、如果<tx:annotation-driven 中 proxy-target-class="true" ，Spring将使用CGLIB动态代理，而内部通过Cglib2AopProxy实现代理，而内部通过DynamicAdvisedInterceptor进行拦截：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package org.springframework.aop.framework;  
- finalclass Cglib2AopProxy implements AopProxy, Serializable {  
- privatestaticclass DynamicAdvisedInterceptor implements MethodInterceptor, Serializable {  
- //注意此处的method 一定是具体类上的method（因此只用放置在具体类上的@Transactional是可以发现的）
- public Object intercept(Object proxy, Method method, Object[] args, MethodProxy methodProxy) throws Throwable {  
-             }  
-        }  
- }  
1.3、Spring使用AnnotationTransactionAttributeSource通过查找一个类或方法是否有@Transactional注解事务来返回TransactionAttribute（表示开启事务）：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package org.springframework.transaction.annotation;  
- publicclass AnnotationTransactionAttributeSource extends AbstractFallbackTransactionAttributeSource implements Serializable {  
- protected TransactionAttribute determineTransactionAttribute(AnnotatedElement ae) {  
- for (TransactionAnnotationParser annotationParser : this.annotationParsers) {  
-             TransactionAttribute attr = annotationParser.parseTransactionAnnotation(ae);  
- if (attr != null) {  
- return attr;  
-             }  
-         }  
- returnnull;  
-     }  
- }  
而AnnotationTransactionAttributeSource又使用SpringTransactionAnnotationParser来解析是否有@Transactional注解：
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package org.springframework.transaction.annotation;  
- 
- publicclass SpringTransactionAnnotationParser implements TransactionAnnotationParser, Serializable {  
- 
- public TransactionAttribute parseTransactionAnnotation(AnnotatedElement ae) {  
-         Transactional ann = AnnotationUtils.getAnnotation(ae, Transactional.class);  
- if (ann != null) {  
- return parseTransactionAnnotation(ann);  
-         }  
- else {  
- returnnull;  
-         }  
-     }  
- 
- public TransactionAttribute parseTransactionAnnotation(Transactional ann) {  
- 
-     }  
- 
- }  
    此处使用AnnotationUtils.getAnnotation(ae, Transactional.class)； 这个方法只能发现当前方法/类上的注解，不能发现父类的注解。 Spring还提供了一个 AnnotationUtils.findAnnotation()方法 可以发现父类/父接口中的注解（但spring没有使用该接口）。
   如果Spring此处换成AnnotationUtils.findAnnotation()，将可以发现父类/父接口中的注解。

这里还一个问题，描述如下：
在接口中删除@Transactional   //开启默认事务
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package cn.javass.common.service;  
- publicinterface IBaseService<M extends java.io.Serializable, PK extends java.io.Serializable> {  
- publicint countAll();  
- }    
在具体类中添加@Transactional
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package cn.javass.common.service.impl;  
- publicabstractclass BaseService<M extends java.io.Serializable, PK extends java.io.Serializable> implements IBaseService<M, PK> {  
- 
- @Transactional()   //开启默认事务
- @Override
- publicint countAll() {  
- return baseDao.countAll();  
-     }  
- }  

问题：
    我们之前说过，基于JDK动态代理时， method 一定是接口上的method（因此放置在接口上的@Transactional是可以发现的），但现在我们放在具体类上，那么Spring是如何发现的呢？？
    还记得发现TransactionAttribute是通过AnnotationTransactionAttributeSource吗？具体看步骤1.3：
而AnnotationTransactionAttributeSource 继承AbstractFallbackTransactionAttributeSource
java代码：
[查看](http://sishuok.com/forum/blogPost/list/0/3845.html#)[复制到剪贴板](http://sishuok.com/forum/blogPost/list/0/3845.html#)[打印](http://sishuok.com/forum/blogPost/list/0/3845.html#)
- package org.springframework.transaction.interceptor;  
- publicabstractclass AbstractFallbackTransactionAttributeSource implements TransactionAttributeSource {  
- 
- public TransactionAttribute getTransactionAttribute(Method method, Class<?> targetClass) {  
- //第一次 会委托给computeTransactionAttribute
- }  
- 
- //计算TransactionAttribute的
- private TransactionAttribute computeTransactionAttribute(Method method, Class<?> targetClass) {  
- 
- //省略
- 
- // Ignore CGLIB subclasses - introspect the actual user class.
-         Class<?> userClass = ClassUtils.getUserClass(targetClass);  
- // The method may be on an interface, but we need attributes from the target class.
- // If the target class is null, the method will be unchanged.
- //①此处将查找当前类覆盖的方法
-         Method specificMethod = ClassUtils.getMostSpecificMethod(method, userClass);  
- // If we are dealing with method with generic parameters, find the original method.
-         specificMethod = BridgeMethodResolver.findBridgedMethod(specificMethod);  
- 
- // First try is the method in the target class.
-         TransactionAttribute txAtt = findTransactionAttribute(specificMethod);  
- if (txAtt != null) {  
- return txAtt;  
-         }  
- 
- //找类上边的注解
- // Second try is the transaction attribute on the target class.
-         txAtt = findTransactionAttribute(specificMethod.getDeclaringClass());  
- if (txAtt != null) {  
- return txAtt;  
-         }  
- //②如果子类覆盖的方法没有 再直接找当前传过来的
- if (specificMethod != method) {  
- // Fallback is to look at the original method.
-             txAtt = findTransactionAttribute(method);  
- if (txAtt != null) {  
- return txAtt;  
-             }  
- // Last fallback is the class of the original method.
- return findTransactionAttribute(method.getDeclaringClass());  
-         }  
- returnnull;  
-     }  
- }  
//①此处将查找子类覆盖的方法
       Method specificMethod = ClassUtils.getMostSpecificMethod(method, userClass);
        // ClassUtils.getMostSpecificMethod
public static Method getMostSpecificMethod(Method method, Class<?> targetClass) {
       Method specificMethod = null;
if (method != null && isOverridable(method, targetClass) &&
              targetClass != null && !targetClass.equals(method.getDeclaringClass())) {
try {
              specificMethod = ReflectionUtils.findMethod(targetClass, method.getName(), method.getParameterTypes());
           } catch (AccessControlException ex) {
              // security settings are disallowing reflective access; leave
              // 'specificMethod' null and fall back to 'method' below
           }
       }
return (specificMethod != null ? specificMethod : method);
    }
   可以看出将找到当前类的那个方法。因此我们放置在BaseService countAll方法上的@Transactional起作用了。
      //②如果子类覆盖的方法没有 再直接找当前传过来的
if (specificMethod != method) {
           // Fallback is to look at the original method.
           txAtt = findTransactionAttribute(method);
if (txAtt != null) {
return txAtt;
           }
           // Last fallback is the class of the original method.
return findTransactionAttribute(method.getDeclaringClass());
       }
       查找子类失败时直接使用传过来的方法。
因此，建议大家使用基于Schema风格的事务（不用考虑这么多问题，也不用考虑是类还是方法）。而@Transactional建议放置到具体类上，不要放置到接口。
作者原创【[http://sishuok.com/forum/blogPost/list/0/3845.html#9317](http://sishuok.com/forum/blogPost/list/0/3845.html#9317)】
