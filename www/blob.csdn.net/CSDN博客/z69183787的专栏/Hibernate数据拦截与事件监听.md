# Hibernate数据拦截与事件监听 - z69183787的专栏 - CSDN博客
2015年02月03日 15:28:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：815
**拦截器(Interceptor)**
org.hibernate.Interceptor接口定义了Hibernate中通用拦截机制
创建Session对象的时候，所有的Session对象或者这个Session对象的所有持久化操作的动作都会被指定的拦截器进行拦截.
**Interceptor接口的方法**
- afterTransactionBegin()
当一个事务时候启动时，会立刻调用这个方法，这个方法可以改变这个事务的状态，例如：回滚事务
- instantiate()
创建对象，如果返回null,则Hibernate将调用实体类的默认构造方法创建持久化对象
- getEntity()
当一个持久化对象，通过标示符属性在Session对象的缓存中进行查找，并且没有找到时，会调用该方法
- getEntityName()
当session对象获取持久化对象的名字时，会调用这个方法
- onLoad()
该方法在持久化对象初始化之前加载，这个的持久化对象处于刚被创建的状态(对象的属性值都未赋值)
- findDirty()
当调用Session对象的flush()方法时，讲调用该方法判断对象是否为脏数据，这是脏数据检查的另外拦截的实现方式
- isTransient()
当调用Session对象的saveOrUpdate方法时，会调用该方法判断对象是否尚未保存
- onSave()
在对象被保存之前调用，通过这个方法可以对要保持的对象的属性进行修改
- onDelete()
该方法在持久化对象被删除之前调用
- preFlush()
该方法当调用Session对象的flush()方法之前被调用
- onFlushDirty()
当调用Session对象flush()方法进行脏数据检查时，如果发现持久化对象的状态发生了改变，会调用该方法
- postFlush()
该方法调用Session对象的flush()方法之后被调用
- beforeTransactionCompletion()
在完成一个事务之前，调用此方法，这个方法可以改变事务的状态，例如回滚事务
- afterTransactionCompletion()
当完成一个事务之后，立刻调用此方法
**使用拦截器实现审计日志**
审计日志指的是，在应用系统中，对所有的数据库的操作都做记录，记录所操作内容，操作的用户和操作的时间
demo
log4j.properties
Log4j.properties代码  ![收藏代码](http://fehly.iteye.com/images/icon_star.png)
- log4j.logger.com.rbh.examples=info,appender1  
- log4j.appender.appender1=org.apache.log4j.FileAppender  
- log4j.appender.appender1.layout=org.apache.log4j.TTCCLayout  
- log4j.appender.appender1.File=ligfile.txt  
LogEntityInterceptor
Java代码  ![收藏代码](http://fehly.iteye.com/images/icon_star.png)
- package com.rbh.examples;  
- import java.io.Serializable;  
- import org.apache.log4j.Logger;  
- import org.hibernate.EmptyInterceptor;  
- import org.hibernate.type.Type;  
- 
- publicclass LogEntityInterceptor extends EmptyInterceptor {  
- 
- privatestaticfinallong serialVersionUID = 1L;  
- 
- private Logger logger = Logger.getLogger(LogEntityInterceptor.class);  
- 
- publicvoid onDelete(Object entity,Serializable id, Object[] state,String[] propertyNames,  
-             Type[] types){  
-         logger.info("删除数据");  
-     }  
- 
- publicboolean onFlushDirty(Object entity,Serializable id, Object[] currentState,  
-             Object[] preState,String[] propertyNames,  
-             Type[] types){  
-         logger.info("修改数据");  
- returnfalse;  
-     }  
- 
- publicboolean onSave(Object entity,Serializable id, Object[] State,  
-             String[] propertyNames,  
-             Type[] types){  
-         logger.info("保存数据");  
- returnfalse;  
-     }  
- }  
HibernateTest
Java代码  ![收藏代码](http://fehly.iteye.com/images/icon_star.png)
- package com.rbh.examples;  
- 
- import java.util.Date;  
- import org.hibernate.Session;  
- import org.hibernate.SessionFactory;  
- import org.hibernate.cfg.Configuration;  
- 
- publicclass HibernateTest {  
- publicstaticvoid main(String[] args)   
-     {  
-         HibernateTest test =new HibernateTest();  
-         test.testInterceptor();  
-     }         
- 
- publicvoid testInterceptor()  
-     {  
-         LogEntityInterceptor interceptor=new LogEntityInterceptor();  
-         Configuration config=new Configuration();  
-         config.setInterceptor(interceptor);  
-         config.configure();  
- 
-         SessionFactory sf=config.buildSessionFactory();  
-         Session session=sf.getCurrentSession();  
- 
-         Guestbook gb= new Guestbook();  
-         gb.setName("Narcissus");  
-         gb.setEmail("javac.q@gmail.com");  
-         gb.setCreatedTime(new Date());  
-         gb.setPhone("11102121");  
-         gb.setTitle("test Interceptor");  
-         gb.setContent("test Interceptor,test Interceptor");  
- 
- 
-         session.beginTransaction();  
-         session.save(gb);  
-         session.getTransaction().commit();  
- 
-         session=sf.getCurrentSession();  
-         gb.setName("tom");  
-         session.beginTransaction();  
-         session.update(gb);  
-         session.getTransaction().commit();  
- 
-         session=sf.getCurrentSession();  
-         session.beginTransaction();  
-         session.delete(gb);  
-         session.getTransaction().commit();  
-     }  
- 
- }  
可以通过session方式加载拦截器对象,也可以通过Configuration对象加载拦截器
Configuration：对所有的session都会被拦截
session:只对当前的session进行拦截
**Hibernate的事件监听机制**
Hibernate中的事件监听机制可以对Session对象的动作进行监听，一旦发生了特殊的事件，Hibernate就会执行监听器中的事件处理方法
在某些功能的设计中，我们即可以使用Hibernate的拦截器实现，也可以使用Hibernate的事件监听来实现
**Hibernate中事件与对应的监听器接口**
**        事件类型                        监听器接口**
                     auto-flush                                                    AutoFlushEventListener
                        merge                                                       MergeEventListener
                        delete                                                       DeleteEventListener
                        persist                                                      PersistEventListener
                    dirty-check                                                    DirtyCheckEventListener
                         evice                                                        EvictEventListener
                          flush                                                        FlushEventListener
                    flush-entity                                                    FlushEntityEventListener
                          load                                                         LoadEventListener
                 load-collection                                                  InitializeCollectEventListener
                          lock                                                          LockEventListener
                      refresh                                                         RefreshEventListener
                      replicate                                                      ReplicateEventListener
                     save-update                                                 SaveOrUpdateEventListener
                     pre-load                                                        PreLoadEventListener
                     pre-update                                                    PreUpdateEventListener
                     pre-delete                                                     PreDeleteEventListener
                     pre-insert                                                      PreInsertEventListener
                     post-load                                                       PostLoadEventListener
                     post-update                                                   PostUpdateEventListener
                     post-delete                                                    PostDeleteEventListener
                     post-insert                                                     PostInsertEventListener
**应用Hibernate事件监听器**
用户制定的事件监听器首先需要实现与所需要处理的事件对应的接口，或者继承实现这个接口的类
通过使用Hibernate的配置文件(hibernate.cfg.xml)配置事件监听对象,或者使用Configuration对象注册这个定制的事件监听器对象
LogPostLoadEventListener
Java代码  ![收藏代码](http://fehly.iteye.com/images/icon_star.png)
- import org.hibernate.event.PostLoadEvent;  
- import org.hibernate.event.PostLoadEventListener;  
- publicclass LogPostLoadEventListener implements PostLoadEventListener {  
- privatestaticfinallong serialVersionUID = 404241098418965422L;  
- publicvoid onPostLoad(PostLoadEvent event) {  
-         System.out.println("Class:" + event.getEntity().getClass().getName() + ",id:"
-                 + event.getId());  
-     }  
- }  
修改Hibernate.cfg.xml文件
Xml代码  ![收藏代码](http://fehly.iteye.com/images/icon_star.png)
- <mappingresource="com/rbh/examples/Guestbook.hbm.xml"/>
- <listenertype="post-load"class="com.rbh.examples.LogPostLoadEventListener"/>
- </session-factory>
- </hibernate-configuration>
或者通过Configuration 对象注册这个监听器对象
Java代码  ![收藏代码](http://fehly.iteye.com/images/icon_star.png)
- Configuration config = new Configuration();  
- config.setListener("post-load", new LogPostLoadEventListener());  
- config.configure();  
- Session session = config.buildSessionFactory().getCurrentSession();  
 编写、配置好监听器以后，当通过Session对象的load()、get()方法或者Query对象的list方法加载持久化对象之后，LogPostEventListener对象中的onPostLoad()方法就会被执行.
**使用监听器实现审计日志**
利用Hibernate的事件机制，不仅能够精确追踪到持久化对象的字段的修改，持久化对象关联关系的变更，还能记录更新前的数值和更新后的数值
**监听器与拦截器的比较**
监听器可以实现更细化粒度的拦截
通过监听器获取所拦截的持久化对象的修改后喝修改前的状态值
能直接通过Event对象获取Session对象
 使用监听器实现审计日志的一个demo 见附件
简要说明：AuditLog表式记录 日志的表
AuditLog.java 和AuditLog.hbm.xml表的持久化映射
AuditLogEventListener 具体的实现类
