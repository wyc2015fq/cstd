# No Hibernate Session bound to thread, and configuration does not allow creation of non-transactional - z69183787的专栏 - CSDN博客
2013年01月29日 17:01:22[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3486
getHibernateTemplate().getSessionFactory().getCurrentSession()的意思是得到当前线程绑定的session，而当前线程绑定的session是通过当前的事务产生的，如果你没有配置事务的话，当前线程threadlocal中就不存在session，这样就出现no session错误。
而execute方法，看源码HibernateTemplate中写道
Java code
[?](http://bbs.csdn.net/topics/280063179#clipboardWindow)
```
```java
public
```
```java
Object execute(HibernateCallback action,
```
```java
boolean
```
```java
exposeNativeSession)
```
```java
throws
```
```java
DataAccessException {
```
```java
```
```java
Assert.notNull(action,
```
```java
"Callback object must not be null"
```
```java
);
```
```java
```
```java
Session session = getSession();
```
```java
```
```java
boolean
```
```java
existingTransaction = (!isAlwaysUseNewSession() &&
```
```java
```
```java
(!isAllowCreate() || SessionFactoryUtils.isSessionTransactional(session, getSessionFactory())));
```
```
其中getSession，代码如下
Java code
[?](http://bbs.csdn.net/topics/280063179#clipboardWindow)
```
```java
protected
```
```java
Session getSession() {
```
```java
```
```java
if
```
```java
(isAlwaysUseNewSession()) {
```
```java
```
```java
return
```
```java
SessionFactoryUtils.getNewSession(getSessionFactory(), getEntityInterceptor());
```
```java
```
```java
}
```
```java
```
```java
else
```
```java
if
```
```java
(isAllowCreate()) {
```
```java
```
```java
return
```
```java
SessionFactoryUtils.getSession(
```
```java
```
```java
getSessionFactory(), getEntityInterceptor(), getJdbcExceptionTranslator());
```
```java
```
```java
}
```
```java
```
```java
else
```
```java
{
```
```java
```
```java
try
```
```java
{
```
```java
```
```java
return
```
```java
getSessionFactory().getCurrentSession();
```
```java
```
```java
}
```
```java
```
```java
catch
```
```java
(HibernateException ex) {
```
```java
```
```java
throw
```
```java
new
```
```java
DataAccessResourceFailureException(
```
```java
"Could not obtain current Hibernate Session"
```
```java
, ex);
```
```java
```
```java
}
```
```java
```
```java
}
```
```java
```
```java
}
```
```
注意这里：else if (isAllowCreate()),其中在HibernateTemplate类中默认private boolean allowCreate = true;
意思说如果当前线程中的session不存在的话，是否允许创建，而默认是允许的，通过函数名字就很清楚，接下来是创建当前线程中的session的代码。
[http://bbs.csdn.net/topics/280063179](http://bbs.csdn.net/topics/280063179)
1: @Transactional声明的方法执行时,Spring的TransactionManager会自动Open Sesion,自动开启事务,并且将此Sesion绑定到SpringSessionContext(实际上是TransactionSynchronizationManager的ThreadLocal的Map)中..
2:SessionFactory.getCurrentSession()方法执行时,调用SpringSessionContext.currentSession()从TransactionSynchronizationManager的上下文中查找 当前的Session
3:找到后返回当前的Session,找不到,则返回HibernateException("No Sessionfound for current thread")
PS: 从中,我们也知道了,执行SessionFactoryImpl.openSession()时,只是简单地new 一个SessionBuilder,然后调用SessionBuilder.openSession(),得到的Session是不会绑定到任何 org.hibernate.context.spi.CurrentSessionContext 在上下文中的.
