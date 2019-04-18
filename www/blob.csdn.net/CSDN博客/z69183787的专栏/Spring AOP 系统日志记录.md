# Spring AOP 系统日志记录 - z69183787的专栏 - CSDN博客
2015年01月07日 12:03:17[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1964
Spring AOP 系统日志记录
AOP是Aspect Oriented Programming的缩写，意思是面向方面编程，AOP实际是GoF设计模式的延续
   关于Spring AOP的一些术语
- **切面（Aspect）** ：在Spring AOP中，切面可以使用通用类或者在普通类中以@Aspect 注解（@AspectJ风格）来实现
- **连接点（Joinpoint）** ：在Spring AOP中一个连接点代表一个方法的执行
- **通知（Advice）** ：在切面的某个特定的连接点（Joinpoint）上执行的动作。通知有各种类型，其中包括"around"、"before”和"after"等通知。许多AOP框架，包括Spring，都是以拦截器做通知模型， 并维护一个以连接点为中心的拦截器链
- **切入点（Pointcut）** ：定义出一个或一组方法，当执行这些方法时可产生通知，Spring缺省使用AspectJ切入点语法。
   通知类型
- **前置通知（@Before）** ：在某连接点（join point）之前执行的通知，但这个通知不能阻止连接点前的执行（除非它抛出一个异常）
- **返回后通知（@AfterReturning）** ：在某连接点（join point）正常完成后执行的通知：例如，一个方法没有抛出任何异常，正常返回
- **抛出异常后通知（@AfterThrowing）** ：方法抛出异常退出时执行的通知
- **后通知（@After）** ：当某连接点退出的时候执行的通知（不论是正常返回还是异常退出）
- **环绕通知（@Around）** ：包围一个连接点（join point）的通知，如方法调用。这是最强大的一种通知类型，环绕通知可以在方法调用前后完成自定义的行为，它也会选择是否继续执行连接点或直接返回它们自己的返回值或抛出异常来结束执行
      @AspectJ风格的AOP配置
Spring AOP配置有两种风格：
- XML风格 = 采用声明形式实现Spring AOP 
- AspectJ风格 = 采用注解形式实现Spring AOP
   创建日志记录表(MySQL)
```
CREATE TABLE `t_log` (
  `id` bigint(20) unsigned NOT NULL AUTO_INCREMENT,
  `userid` bigint(20) unsigned NOT NULL,
  `createdate` datetime NOT NULL COMMENT '创建日期',
  `content` varchar(8000) NOT NULL DEFAULT '' COMMENT '日志内容',
  `operation` varchar(250) NOT NULL DEFAULT '' COMMENT '用户所做的操作',
  PRIMARY KEY (`id`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;
```
   日志记录POJO类：
```java
**package com.hz.yiliao.orm;import java.io.Serializable;import java.util.Date;/** * 系统日志 * @author Administrator * */public class Log implements Serializable{  private static final long serialVersionUID = -7372303516713218870L;    private Long id;//  private Long userId;//  private Date createDate;//创建日期  private String content;//日志内容  private String operation;//用户所做的操作    //get set方法省略，自己生成    }**
```
日志记录Mapper接口与XML配置文件：
```java
**package com.hz.yiliao.dao;import java.util.List;import java.util.Map;import com.hz.yiliao.orm.Log;/** * 系统日志接口 * @author Administrator * */public interface LogMapper {  /**   * 新增   * @param log   */  public void insert(Log log);  }**
```
    LogMapper.xml配置文件
**<?xml version="1.0" encoding="UTF-8"?><!DOCTYPE mapper PUBLIC "-//mybatis.org//DTD Mapper 3.0//EN" "http://mybatis.org/dtd/mybatis-3-mapper.dtd"><mappernamespace="com.hz.yiliao.dao.LogMapper"><resultMaptype="com.hz.yiliao.orm.Log"id="LogMapper"><idproperty="id"column="id"/><resultproperty="userId"column="userid" /><resultproperty="createDate"column="createdate" /><resultproperty="content"column="content" /><resultproperty="operation"column="operation" /></resultMap><sqlid="LogSQL">userid,createdate,content,operation</sql><insertid="insert"parameterType="com.hz.yiliao.orm.Log"useGeneratedKeys="true"keyProperty="id"keyColumn="id">insert into t_log(<includerefid="LogSQL"/>) value(#{userId},NOW(),#{content},#{operation})</insert></mapper>**
 日志记录Service层:
```java
**package com.hz.yiliao.bo;import java.util.List;import java.util.Map;import org.springframework.beans.factory.annotation.Autowired;import org.springframework.stereotype.Service;import org.springframework.transaction.annotation.Transactional;import com.hz.yiliao.dao.LogMapper;import com.hz.yiliao.orm.AdminUser;import com.hz.yiliao.orm.Log;/** * 系统日志逻辑层 * @author Administrator * */@Servicepublic class LogBO {    @Autowired  private LogMapper logMapper;  /**   * 新增   * @param log   */  @Transactional("transactionManager")  public void insertLog(Log log){    logMapper.insert(log);  }    }**
```
在MyBatis配置文件mybatis-config.xml中配置POJO，根据自己的要求配置，如果不知道怎么配置，看一下spring+mybatis整合
创建utils包，在utils包下创建切面类LogAspect:
```java
**package com.hz.yiliao.utils;import java.lang.reflect.Method;import org.aspectj.lang.JoinPoint;import org.aspectj.lang.annotation.AfterReturning;import org.aspectj.lang.annotation.Aspect;import org.aspectj.lang.annotation.Pointcut;import org.springframework.beans.factory.annotation.Autowired;import com.hz.yiliao.bo.LogBO;import com.hz.yiliao.orm.Log;/** * 系统日志AOP * @author Administrator * */@Aspectpublic class LogAspect {  @Autowired  private LogBO logBO;    /**   * 添加业务逻辑方法切入点    */  @Pointcut("execution(* com.hz.yiliao.bo.*.insert(..))")  public void insertCell(){}    /**   * 修改业务逻辑方法切入点    */  @Pointcut("execution(* com.hz.yiliao.bo.*.update(..))")  public void updateCell(){}    /**   * 删除业务逻辑方法切入点    */  @Pointcut("execution(* com.hz.yiliao.bo.*.delete(..))")  public void deleteCell(){}    /**   * 添加操作日志(后置通知)    * @param joinPoint   * @param rtv   */  @SuppressWarnings("unused")  @AfterReturning(value="insertCell()",argNames="rtv", returning="rtv")  public void insertLog(JoinPoint joinPoint, Object rtv) throws Throwable{    Long userId = 1L;        if(userId == null){//没有管理员登录        return ;    }    //判断参数      if(joinPoint.getArgs() == null){//没有参数        return ;    }    //获取方法名     String methodName = joinPoint.getSignature().getName();    //获取操作内容    String opContent = optionContent(joinPoint.getArgs(),methodName);        Log log = new Log();    log.setUserId(userId);    log.setContent(opContent);    log.setOperation("添加");    logBO.insertLog(log);  }     /**      * 管理员修改操作日志(后置通知)      * @param joinPoint      * @param rtv      * @throws Throwable      */      @SuppressWarnings("unused")  @AfterReturning(value="updateCell()", argNames="rtv", returning="rtv")      public void updateLog(JoinPoint joinPoint, Object rtv) throws Throwable{      	Long userId = 1L;                  if(userId == null){//没有登录              return;          }          //判断参数          if(joinPoint.getArgs() == null){//没有参数              return;          }          //获取方法名          String methodName = joinPoint.getSignature().getName();          //获取操作内容          String opContent = optionContent(joinPoint.getArgs(), methodName);                    //创建日志对象          Log log = new Log();      log.setUserId(userId);    log.setContent(opContent);        log.setOperation("修改");//操作          logBO.insertLog(log);    }          /**      * 删除操作     * @param joinPoint      * @param rtv      */      @SuppressWarnings("unused")  @AfterReturning(value="deleteCell()",argNames="rtv", returning="rtv")    public void deleteLog(JoinPoint joinPoint, Object rtv) throws Throwable{    	Long userId = 1L;    	if(userId == null){//没有登录              return;          }          //判断参数          if(joinPoint.getArgs() == null){//没有参数              return;          }          //获取方法名          String methodName = joinPoint.getSignature().getName();                  StringBuffer rs = new StringBuffer();    rs.append(methodName);    String className = null;    for(Object info : joinPoint.getArgs()){        	//获取对象类型      className = info.getClass().getName();      className = className.substring(className.lastIndexOf(".") + 1);      rs.append("[参数1，类型:" + className + "，值:(id:" + joinPoint.getArgs()[0]+")");        }                  //创建日志对象          Log log = new Log();      log.setUserId(userId);    log.setContent(rs.toString());        log.setOperation("删除");//操作          logBO.insertLog(log);    }    /**   * 使用Java反射来获取被拦截方法(insert、update)的参数值，      * 将参数值拼接为操作内容   * @param args   * @param mName   * @return   */  public String optionContent(Object[] args, String mName){    if(args == null){      return null;    }    StringBuffer rs = new StringBuffer();    rs.append(mName);    String className = null;    int index = 1;    //遍历参数对象     for(Object info : args){      //获取对象类型      className = info.getClass().getName();      className = className.substring(className.lastIndexOf(".") + 1);      rs.append("[参数"+index+"，类型:" + className + "，值:");      //获取对象的所有方法      Method[] methods = info.getClass().getDeclaredMethods();      // 遍历方法，判断get方法       for(Method method : methods){        String methodName = method.getName();        // 判断是不是get方法        if(methodName.indexOf("get") == -1){//不是get方法           continue;//不处理        }        Object rsValue = null;        try{          // 调用get方法，获取返回值          rsValue = method.invoke(info);        }catch (Exception e) {          continue;        }        //将值加入内容中        rs.append("(" + methodName+ ":" + rsValue + ")");      }      rs.append("]");      index ++;    }    return rs.toString();  }  }**
```
   在applicationContext.xml中加入新的配置
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:mvc="http://www.springframework.org/schema/mvc"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"xmlns:aop="http://www.springframework.org/schema/aop"xmlns:tx="http://www.springframework.org/schema/tx"xmlns:jee="http://www.springframework.org/schema/jee"xsi:schemaLocation="
http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-3.0.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-3.0.xsd
http://www.springframework.org/schema/mvc
http://www.springframework.org/schema/mvc/spring-mvc-3.0.xsd
http://www.springframework.org/schema/aop
http://www.springframework.org/schema/aop/spring-aop-3.0.xsd
http://www.springframework.org/schema/tx
http://www.springframework.org/schema/tx/spring-tx-3.0.xsd
http://www.springframework.org/schema/jee
http://www.springframework.org/schema/jee/spring-tx-3.0.xsd"><!-- 加入Aspectj配置 --><aop:aspectj-autoproxy /><beanid="logAspect"class="com.hz.yiliao.utils.LogAspect" /><context:component-scanbase-package="com.hz.yiliao.bo,com.hz.yiliao.orm" /><beanid="yiliaoDataSource"class="org.apache.commons.dbcp.BasicDataSource"destroy-method="close"><propertyname="driverClassName"value="com.mysql.jdbc.Driver" /><propertyname="url"value="jdbc:mysql://localhost:3306/yiliao?useUnicode=true&characterEncoding=UTF-8&zeroDateTimeBehavior=convertToNull" /><propertyname="username"value="root" /><propertyname="password"value="root" /><propertyname="defaultAutoCommit"value="false" /><propertyname="timeBetweenEvictionRunsMillis"value="3600000" /><propertyname="minEvictableIdleTimeMillis"value="3600000" /></bean><beanid="yiliaoSessionFactory"class="org.mybatis.spring.SqlSessionFactoryBean"><propertyname="dataSource"ref="yiliaoDataSource" /><propertyname="typeAliasesPackage"value="com.hz.yiliao.orm" /></bean><beanid="mailSender"class="com.hz.yiliao.utils.MailSender"><propertyname="javaMailSender"ref="javaMailSender" /></bean><beanid="javaMailSender"class="org.springframework.mail.javamail.JavaMailSenderImpl"><propertyname="host"value="smtp.163.com" /><propertyname="username"value="yiliao_2014@163.com" /><propertyname="password"value="yiliao2014" /><propertyname="defaultEncoding"value="UTF-8" /></bean><importresource="mybatis-config.xml" /><beanid="transactionManager"class="org.springframework.jdbc.datasource.DataSourceTransactionManager"><propertyname="dataSource"><reflocal="yiliaoDataSource" /></property></bean><mvc:interceptors><mvc:interceptor><mvc:mappingpath="/**" /><beanclass="com.hz.yiliao.filt.YiliaoSysInterceptor" /></mvc:interceptor></mvc:interceptors><tx:annotation-driventransaction-manager="transactionManager"proxy-target-class="true" /></beans>
配置成功后分别进行登录、添加、修改、删除 等 操作，日志记录表的内容如下：
![](https://img-blog.csdn.net/20170623103859497?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvejY5MTgzNzg3/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
