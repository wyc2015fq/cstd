# 利用Hibernate 3 Interceptor生成审计日志 - z69183787的专栏 - CSDN博客
2015年02月03日 19:48:03[OkidoGreen](https://me.csdn.net/z69183787)阅读数：2385
在实际应用中，有可能需要审计对数据库中重要数据的更新历史。如：每当发生向T_USER表中插入或更新记录的事件时，就向审计日志表插入一条记录。 
对于此种业务场景，可以采用数据库的触发器来生成审计日志，这种方法较为简便，并且有很好的性能，但是不支持跨数据库平台，缺乏灵活性。因此我们可以采用Hibernate 3以后提供的拦截器来实现相应的功能。当Session执行save()、update()、saveOrUpdate()、delete()以及flush()方法时，就会调用拦截器的相关方法，然后在这些方法中实现审计日志的业务逻辑。 
用户定义的拦截器必须实现org.hibernate.Interceptor接口，在这个接口中主要定义了以下方法。 
onDelete()：当Session删除一个对象之前调用此方法。 
onFlushDirty()：当Session的flush()方法检查到脏对象时调用此方法。 
onSave()：当Session保存一个对象之前调用本方法，如果在这个方法中修改了持久化对象的数据，就返回true，否则返回null。 
postFlush()：当Session的flush()方法执行完所有SQL语句后调用此方法。 
在org.hibernate包中还提供了Interceptor接口的一个实现类EmptyInterceptor，这个类中的所有方法实际上什么也不做，用户自定义的拦截器类也可以扩展此类。 
Java代码  ![收藏代码](http://edisonlv2010.iteye.com/images/icon_star.png)
- package com.jintoufs.common.hibernate;  
- 
- import java.io.Serializable;  
- import java.sql.Timestamp;  
- import java.util.Date;  
- 
- import org.cosmos.core.orm.entity.AuditableEntity;  
- import org.hibernate.EmptyInterceptor;  
- import org.hibernate.type.Type;  
- import org.slf4j.Logger;  
- import org.slf4j.LoggerFactory;  
- 
- publicclass AuditInterceptor extends EmptyInterceptor {  
- 
- privatestaticfinallong serialVersionUID = -7481545873785342485L;  
- privatestatic Logger logger = LoggerFactory.getLogger(AuditInterceptor.class);  
- 
- /**
-      *  拦截hibernate save方法(即保存对象之前回调此方法),添加审计信息
-      *  entity - POJO Instance
-      *  id - POJO OID
-      *  state - POJO Instance中每一个属性的值所组成的集合(OID属性除外)     
-      *  propertyNames - POJO Instance中每一个属性的属性名组成的集合(OID属性除外)    
-      *  types - POJO Instance中每一个属性所属类型所对应的Hibernate类型组成的集合(OID属性除外)
-      */
- @Override
- publicboolean onSave(Object entity, Serializable id, Object[] state, String[] propertyNames, Type[] types) {  
- 
-         String operUserStr = getCurrentUser();  
- 
- if (entity instanceof AuditableEntity) {  
- for (int index = 0; index < propertyNames.length; index++) {  
- if ("createTime".equals(propertyNames[index])) {  
-                     state[index] = new Timestamp(new Date().getTime());  
-                 }  
- if ("createBy".equals(propertyNames[index])) {  
-                     state[index] = getCurrentUser();  
-                 }  
-             }  
-             logger.info("{}对象(ID:{}) 被 {} 在 {} 创建", new Object[] { entity.toString(), id, operUserStr, new Date() });  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- 
- /**
-      *  拦截hibernate flush方法(即检查到脏对象时回调此方法),添加审计信息
-      *  entity - POJO Instance
-      *  id - POJO OID
-      *  state - POJO Instance中每一个属性的值所组成的集合(OID属性除外)     
-      *  propertyNames - POJO Instance中每一个属性的属性名组成的集合(OID属性除外)    
-      *  types - POJO Instance中每一个属性所属类型所对应的Hibernate类型组成的集合(OID属性除外)
-      */
- publicboolean onFlushDirty(Object entity, Serializable id, Object[] state, Object[] previousState,  
-             String[] propertyNames, Type[] types) {  
- 
-         String operUserStr = getCurrentUser();  
- 
- if (entity instanceof AuditableEntity) {  
- for (int index = 0; index < propertyNames.length; index++) {  
- if ("lastModifyTime".equals(propertyNames[index])) {  
-                     state[index] = new Timestamp(new Date().getTime());  
-                 }  
- if ("lastModifyBy".equals(propertyNames[index])) {  
-                     state[index] = getCurrentUser();  
-                 }  
-             }  
-             logger.info("{}对象(ID:{}) 被 {} 在 {} 修改", new Object[] { entity.toString(), id, operUserStr, new Date() });  
- returntrue;  
-         }  
- returnfalse;  
-     }  
- 
- /**
-      * @description 得到当前的操作用户
-      * @return 
-      */
- private String getCurrentUser() {  
-         String loginName = "test_user";  
- //String loginName = SpringSecurityUtils.getCurrentUserName();
- return loginName;  
-     }  
- }  
applicationContext.xml 
Java代码  ![收藏代码](http://edisonlv2010.iteye.com/images/icon_star.png)
- <!-- sessionFactory -->  
- <bean id="sessionFactory"
- class="org.springframework.orm.hibernate4.LocalSessionFactoryBean">  
- <property name="dataSource" ref="dataSource" />  
- ......  
- <property name="entityInterceptor" ref="auditInterceptor"></property>  
- ......  
- </bean>  
- 
- <!-- hibernate审计拦截器 -->  
- <bean id="auditInterceptor"class="com.jintoufs.common.hibernate.AuditInterceptor" />  
以上为Spring 3和Hibernate 4整合时的配置方式 
将审计拦截器作为sessionFactory的属性进行配置，即表示此拦截器可以被所有Session实例共享。等同的代码表示方式为：Configuration.setInterceptor(Interceptor inter)。 
