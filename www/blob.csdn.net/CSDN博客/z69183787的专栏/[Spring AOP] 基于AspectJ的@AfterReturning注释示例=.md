# [Spring AOP] 基于AspectJ的@AfterReturning注释示例= - z69183787的专栏 - CSDN博客
2015年01月07日 12:04:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：9800
**环境** ：系统开发过程中，我们都曾实现过将系统元数据或字典表添加到缓存中，以便程序调用，减少数据库访问IO。
**问题** ：在用户通过前端页面更新系统字典表时，需手工刷新系统缓存，操作很不友好。
**解决方案** ：监听持久层DAO方法的调用，对于目标表的insert,update,delete操作进行相应的系统缓存更新。
**示例环境**：Spring2.5 + iBatis + AspectJ
**参考书目**：Spring 2.5 Aspect-Oriented Programming
![Spring 2.5 Aspect-Oriented Programming](http://seraph115.iteye.com/upload/picture/pic/76518/cc554e41-6cd0-3d9b-a84a-d705f7d6b255.jpg)
Spring AOP自动代理的XML配置
Xml代码  ![收藏代码](http://seraph115.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:aop="http://www.springframework.org/schema/aop"xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
- http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd">
- 
- <aop:aspectj-autoproxy/>
- 
- </beans>
被监测类的代码：
Java代码  ![收藏代码](http://seraph115.iteye.com/images/icon_star.png)
- publicinterface ScDbInfoDAO {  
- 
-     BigDecimal insert(ScDbInfo record);  
- 
- int updateByPrimaryKey(ScDbInfo record);  
- 
- int updateByPrimaryKeySelective(ScDbInfo record);  
- 
-     List selectByExample(ScDbInfoExample example, String orderByClause);  
- 
-     List selectByExample(ScDbInfoExample example);  
- 
-     ScDbInfo selectByPrimaryKey(BigDecimal dbId);  
- 
- int deleteByExample(ScDbInfoExample example);  
- 
- int deleteByPrimaryKey(BigDecimal dbId);  
- 
- int selectCountByExample(ScDbInfoExample example);  
- 
- }  
然后是AspectJ实现：
Java代码  ![收藏代码](http://seraph115.iteye.com/images/icon_star.png)
- import org.apache.log4j.Logger;  
- import org.aspectj.lang.JoinPoint;  
- import org.aspectj.lang.Signature;  
- import org.aspectj.lang.annotation.AfterReturning;  
- import org.aspectj.lang.annotation.Aspect;  
- import org.springframework.stereotype.Service;  
- 
- /**
-  * @author seraph
-  * 
-  */
- @Service
- @Aspect
- publicclass JdbcSourceInterceptor {  
- 
- privatestaticfinal Logger log = Logger.getLogger(JdbcSourceInterceptor.class);  
- 
- @AfterReturning(value="execution(* com.longtop.data.switching.db.dao.ScDbInfoDAO.*(..))", argNames="rtv", returning="rtv")  
- publicvoid afterInsertMethod(JoinPoint jp, Object rtv) throws Throwable {  
- 
-         Signature signature = jp.getSignature();  
-         log.debug("DeclaringType:" + signature.getDeclaringType());   
-         log.debug("DeclaringTypeName:" + signature.getDeclaringTypeName());  
-         log.debug("Modifiers:" + signature.getModifiers());  
-         log.debug("Name:" + signature.getName());  
-         log.debug("LongString:" + signature.toLongString());  
-         log.debug("ShortString:" + signature.toShortString());  
- 
- for (int i = 0; i < jp.getArgs().length; i++) {  
-             Object arg = jp.getArgs()[i];  
- if(null != arg) {  
-                 log.debug("Args:" + arg.toString());   
-             }  
-         }  
- 
-         log.debug("Return:" + rtv);   
-     }  
- 
- }  
运行时的监测日志：
Java代码  ![收藏代码](http://seraph115.iteye.com/images/icon_star.png)
- JdbcSourceInterceptor  - DeclaringType:class dao.impl.ScDbInfoDAOImpl  
- JdbcSourceInterceptor  - DeclaringTypeName:dao.impl.ScDbInfoDAOImpl  
- JdbcSourceInterceptor  - Modifiers:1
- JdbcSourceInterceptor  - Name:selectByPrimaryKey  
- JdbcSourceInterceptor  - LongString:ScDbInfoDAOImpl.selectByPrimaryKey(BigDecimal)  
- JdbcSourceInterceptor  - ShortString:selectByPrimaryKey  
- JdbcSourceInterceptor  - Args:1
- JdbcSourceInterceptor  - Return:ScDbInfo: [dbId=1, dbName=oracle, dbDesc=oracle驱动, dbType=2, dbIp=10.1.7.19, dbPortNo=1521, dbInstName=dc, dbUserName=cgst, dbPwd=cgst, maxConnNum=100, minConnNum=20, initConnNum=26]  
通过以上的日志我们可以看出，@AfterReturning注释AOP中，通过JoinPoint和返回参数我们可以得到类运行时的所有相关信息，如通过方法名我们可以鉴别出是insert, update还是delete操作，针对不同的操作实现不同的处理方法，如调用缓存的add()，remove()，refresh()方法。我们还可以获取方法的调用参数及返回值，这极大的方便了我们对原业务逻辑的AOP处理。
**一些相关概念：**
- Aspect - 切面
- Pointcut - 切入点
- Joinpoint - 连接点
- Pointcut Designators (PCD)
Spring AOP中, 切入点(Pointcut)注释符在使用execution方法时以下的连接点(joinpoint)是可用的。
•    execution
•    within
•    this
•    target
•    args
•    @target
•    @args
•    @within
•    @annotation
•    bean
以下的切入点(pointcut)仅支持基于XML的Spring AOP配置，不支持AspectJ注释形式。如使用将会导致IllegalArgumentException异常。他们是:
•    call
•    get
•    set
•    preinitialization
•    staticinitialization
•    initialization
•    handler
•    adviceexecution
•    withincode
•    cflow
•    cflowbelow
•    if
•    @this
•    @withincode
