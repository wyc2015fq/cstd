# Hibernate拦截器(Interceptor)与事件监听器(Listener) - 零度的博客专栏 - CSDN博客
2016年04月28日 17:38:37[零度anngle](https://me.csdn.net/zmx729618)阅读数：2291
前言：
由于项目中（S2SH框架）用到了memcache缓存服务器，考虑到同步问题是用每个bean变更时同时更新缓存还是用类似数据库trigger（触发器）去实现呢，答案当然是用类似trigger的方式了，其优点不言而喻，毕竟这么写一劳永逸。
经调查发现，hibernate有两种方式可以实现：
        拦截器（Intercept）：与Struts2的拦截器机制基本一样，都是一个操作穿过一层层拦截器，每穿过一个拦截器就会触发相应拦截器的事件做预处理或善后处理。
　　监听器（Listener）：其实功能与拦截器是相似的，但它实现原理不同，它是为每一个事件注册一个或多个监听器，一旦事件发生，则事件源通知所有监听该事件的监听器，然后监听器处理通知（观察者模式）。
拦截器具体实现：
**[java]**[view
 plain](http://blog.csdn.net/liu251890347/article/details/34821715#)[copy](http://blog.csdn.net/liu251890347/article/details/34821715#)[print](http://blog.csdn.net/liu251890347/article/details/34821715#)[?](http://blog.csdn.net/liu251890347/article/details/34821715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/406775/fork)
- package fi.uum.cache.intercepter;  
- 
- import java.io.Serializable;  
- import java.util.Iterator;  
- 
- import org.hibernate.CallbackException;  
- import org.hibernate.EmptyInterceptor;  
- import org.hibernate.type.Type;  
- import org.springframework.stereotype.Component;  
- 
- @Component
- publicclass CacheIntercepter extends EmptyInterceptor{  
- 
- /**
-      * 
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- @Override
- publicvoid onDelete(Object entity, Serializable id, Object[] state,  
-             String[] propertyNames, Type[] types) {  
-         System.out.println("delete..............");  
- super.onDelete(entity, id, state, propertyNames, types);  
-     }  
- 
- @Override
- publicboolean onFlushDirty(Object entity, Serializable id,  
-             Object[] currentState, Object[] previousState,  
-             String[] propertyNames, Type[] types) {  
-         System.out.println("flushDirty..............");  
- returnsuper.onFlushDirty(entity, id, currentState, previousState,  
-                 propertyNames, types);  
-     }  
- 
- @Override
- publicboolean onSave(Object entity, Serializable id, Object[] state,  
-             String[] propertyNames, Type[] types) {  
-         System.out.println("save..............");  
- returnsuper.onSave(entity, id, state, propertyNames, types);  
-     }  
- 
- @Override
- publicvoid onCollectionRecreate(Object collection, Serializable key)  
- throws CallbackException {  
-         System.out.println("recreate..............");  
- super.onCollectionRecreate(collection, key);  
-     }  
- 
- @Override
- publicvoid onCollectionRemove(Object collection, Serializable key)  
- throws CallbackException {  
-         System.out.println("remove..............");  
- super.onCollectionRemove(collection, key);  
-     }  
- 
- @Override
- publicvoid onCollectionUpdate(Object collection, Serializable key)  
- throws CallbackException {  
-         System.out.println("collectionUpdate..............");  
- super.onCollectionUpdate(collection, key);  
-     }  
- 
- @Override
- publicboolean onLoad(Object entity, Serializable id, Object[] state,  
-             String[] propertyNames, Type[] types) {  
-         System.out.println("load..............");  
- returnsuper.onLoad(entity, id, state, propertyNames, types);  
-     }  
- 
- @Override
- publicvoid postFlush(Iterator entities) {  
-         System.out.println("flush..............");  
- super.postFlush(entities);  
-     }  
- 
- @Override
- public String onPrepareStatement(String sql) {  
-         System.out.println("statement.............."+sql);  
- returnsuper.onPrepareStatement(sql);  
-     }  
- 
- @Override
- publicvoid preFlush(Iterator entities) {  
-         System.out.println("preflush..............");  
- super.preFlush(entities);  
-     }  
- 
- }  
类继承EmptyInterceptor是官方推荐做法。
PS:@Component为spring组件（bean）声明方式，用法类似<bean id="" class=""> beanid 默认为类名（第一个字母小写），使用@Component前需要先在srping配置文件中声明<!-- 配置注入信息的包 --><context:component-scan base-package="*" /> 。
最后，Hibernate的拦截器有两种设置方式，一种是使用sessionFactory.openSession(Interceptor interceptor)，这样的拦截器只会针对该session有效，又叫做局部拦截器。另一种是使用Configuration的setInterceptor(Interceptor interceptor)方法设置，这样的拦截器对每一个session都有效，又称之为全局拦截器，全局拦截器还有种配置方法是在sessionFactory bean中加
**[java]**[view
 plain](http://blog.csdn.net/liu251890347/article/details/34821715#)[copy](http://blog.csdn.net/liu251890347/article/details/34821715#)[print](http://blog.csdn.net/liu251890347/article/details/34821715#)[?](http://blog.csdn.net/liu251890347/article/details/34821715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/406775/fork)
- <property name="entityInterceptor">  
-             <ref bean="cacheIntercepter"/>  
-         </property>  
事件监听器实现：
**[java]**[view
 plain](http://blog.csdn.net/liu251890347/article/details/34821715#)[copy](http://blog.csdn.net/liu251890347/article/details/34821715#)[print](http://blog.csdn.net/liu251890347/article/details/34821715#)[?](http://blog.csdn.net/liu251890347/article/details/34821715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/406775/fork)
- package fi.uum.cache.listener;  
- 
- import org.hibernate.event.spi.PostDeleteEvent;  
- import org.hibernate.event.spi.PostDeleteEventListener;  
- import org.hibernate.event.spi.PostInsertEvent;  
- import org.hibernate.event.spi.PostInsertEventListener;  
- import org.hibernate.event.spi.PostUpdateEvent;  
- import org.hibernate.event.spi.PostUpdateEventListener;  
- import org.hibernate.persister.entity.EntityPersister;  
- import org.springframework.stereotype.Component;  
- 
- @Component
- publicclass CacheEventListener implements PostUpdateEventListener ,  
-     PostInsertEventListener,PostDeleteEventListener{  
- 
- /**
-      * 
-      */
- privatestaticfinallong serialVersionUID = 1L;  
- 
- @Override
- publicvoid onPostDelete(PostDeleteEvent arg0) {  
-         System.out.println("delete...................");  
-     }  
- 
- @Override
- publicvoid onPostInsert(PostInsertEvent arg0) {  
-         System.out.println("insert...................");  
-     }  
- 
- @Override
- publicvoid onPostUpdate(PostUpdateEvent arg0) {  
-         System.out.println("update...................");  
-     }  
- 
- @Override
- publicboolean requiresPostCommitHanding(EntityPersister arg0) {  
-         System.out.println("here...................");  
- returnfalse;  
-     }  
- 
- 
- 
- 
- }  
配置方法：
在hibernate4中，查看LocalSessionFactroyBean源码去不支持EventListener。spring3.X 对hibernate4不支持这样的配置，hibernate4 改变了注册EventListener的方式，这里使用注解方式：
**[java]**[view
 plain](http://blog.csdn.net/liu251890347/article/details/34821715#)[copy](http://blog.csdn.net/liu251890347/article/details/34821715#)[print](http://blog.csdn.net/liu251890347/article/details/34821715#)[?](http://blog.csdn.net/liu251890347/article/details/34821715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/406775/fork)
- package fi.uum.common.config;  
- 
- import javax.annotation.PostConstruct;  
- 
- import org.hibernate.SessionFactory;  
- import org.hibernate.event.service.spi.EventListenerRegistry;  
- import org.hibernate.event.spi.EventType;  
- import org.hibernate.internal.SessionFactoryImpl;  
- import org.springframework.beans.factory.annotation.Autowired;  
- import org.springframework.stereotype.Component;  
- 
- import fi.uum.cache.listener.CacheEventListener;  
- 
- @Component
- publicclass HibernateIntegrator {  
- 
- @Autowired
- private CacheEventListener cacheEventListener;  
- 
- @Autowired
- private SessionFactory sessionFactory;  
- 
- 
- @PostConstruct
- publicvoid registerListeners() {  
-         EventListenerRegistry registry = ((SessionFactoryImpl) sessionFactory).getServiceRegistry().getService(  
-                 EventListenerRegistry.class);  
-         registry.getEventListenerGroup(EventType.POST_INSERT).appendListener(cacheEventListener);  
-         registry.getEventListenerGroup(EventType.POST_UPDATE).appendListener(cacheEventListener);  
-         registry.getEventListenerGroup(EventType.POST_DELETE).appendListener(cacheEventListener);  
-     }  
- 
- }  
如果你的项目使用的是hibernate3+，建议在sessionFactory bean中配置eventListener。
实际效果：
两种方法实现过发现在本项目中只有save obj的时候才触发了事件（拦截器），一直不解，后来想到hibernate是基于对象操作的，而在项目中除了save方法外，其他的update、delete、等方法都是通过hql语句实现的，所有更新、删除的事件未触发。这样一来必须通过监听statement语句时通过判断该如果是更新还是删除等执行更新缓存了。
