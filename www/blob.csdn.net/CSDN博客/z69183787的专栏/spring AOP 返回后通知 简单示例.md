# spring AOP 返回后通知 简单示例 - z69183787的专栏 - CSDN博客
2016年08月15日 13:41:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1079
开始前我们要导入spring需要的jar包；
还需要引入 aspectjweaver.jar 和 cglib.jar 这两个jar包
如果是maven项目的话，在pom.xml 的dependencies节点下添加如下引用即可：
Xml代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- 
<dependency>
<groupId>aspectj</groupId>
<artifactId>aspectjweaver</artifactId>
<version>1.5.3</version>
</dependency>
<dependency>
<groupId>cglib</groupId>
<artifactId>cglib</artifactId>
<version>2.2.2</version>
</dependency>
现在开始：
首先，我们定义一个工程中要用到的简单实体Bean-- User.java
Java代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- package org.xmy.ldq.entity;  
- import java.io.Serializable;  
- /**
-  * 测试用实体
-  * @author LiDuanqiang
-  */
- publicclass User implements Serializable{  
- privatestaticfinallong serialVersionUID = 5752197085695030514L;  
- 
- private String id;  
- private String name;  
- private String password;  
- private String address;  
- 
- public User() {}  
- public User(String id, String name,String password,String address) {  
- super();  
- this.id = id;  
- this.name = name;  
- this.password = password;  
- this.address = address;  
-     }  
- public String getId() {  
- return id;  
-     }  
- publicvoid setId(String id) {  
- this.id = id;  
-     }  
- public String getName() {  
- return name;  
-     }  
- publicvoid setName(String name) {  
- this.name = name;  
-     }  
- public String getAddress() {  
- return address;  
-     }  
- publicvoid setAddress(String address) {  
- this.address = address;  
-     }  
- public String getPassword() {  
- return password;  
-     }  
- publicvoid setPassword(String password) {  
- this.password = password;  
-     }  
- 
- }  
 接下来定义一个通知对象：UserAdvice.java
Java代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- package org.xmy.ldq.aop;  
- 
- import org.aspectj.lang.JoinPoint;  
- 
- /**
-  * 用户操作的通知对象
-  * @author LiDuanqiang
-  */
- publicclass UserAdvice {  
- /**
-      * 主体方法返回后将执行的通知方法
-      * @param JoinPoint 
-      * @param retValue主体方法传递到通知方法的返回值
-      * @return
-      */
- public Object afterReturning(JoinPoint joinPoint,Object retValue)throws Throwable{  
-         Object object = null;  
-         object = joinPoint.getThis();//返回代理对象
- /*切入点主体方法的名字*/
-         String methodName = joinPoint.getSignature().getName();  
- if (retValue instanceof Boolean && (Boolean)retValue && methodName.equals("login")) {  
-             System.out.println("方法:"+methodName+"()"+"成功执行;"+"登录成功!");  
-         }  
- return object;  
-     }  
- }  
 再定义一个目标类:AOPBean.java
Java代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- package org.xmy.ldq;  
- 
- import org.xmy.ldq.entity.User;  
- 
- 
- /**
-  * 主体Bean
-  * @author LiDuanqiang
-  */
- publicclass AOPBean{  
- public Boolean login(User user){  
-         Boolean ret = false;  
- /*用户名和密码都为ldq则成功登录*/
- if (user.getName().equals("ldq")&&user.getPassword().equals("ldq")) {  
-             ret = true;  
-         }  
- return ret;  
-     }  
- 
- }  
 最后定义一个测试主程序的实现类：App.java
Java代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- package org.xmy.ldq;  
- 
- import org.springframework.context.support.ClassPathXmlApplicationContext;  
- import org.xmy.ldq.entity.User;  
- 
- /**
-  * 测试
-  * @author LiDuanqiang
-  */
- publicclass App{  
- publicstaticvoid main( String[] args ){  
-         ClassPathXmlApplicationContext factory =  
- new ClassPathXmlApplicationContext("applicationContext.xml");  
-         AOPBean aopBean = (AOPBean) factory.getBean("aopBean");  
-         aopBean.login(new User("1","ldq","ldq","cs"));  
-         System.exit(0);  
-     }  
- }  
 配置文件很重要:applicationContext.xml
Xml代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:aop="http://www.springframework.org/schema/aop"
- xmlns:tx="http://www.springframework.org/schema/tx"
- xmlns:p="http://www.springframework.org/schema/p"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-            http://www.springframework.org/schema/beans/spring-beans-2.5.xsd  
-            http://www.springframework.org/schema/context  
-            http://www.springframework.org/schema/context/spring-context-2.5.xsd  
-            http://www.springframework.org/schema/aop http://www.springframework.org/schema/aop/spring-aop-2.5.xsd  
-            http://www.springframework.org/schema/tx http://www.springframework.org/schema/tx/spring-tx-2.5.xsd">
- <aop:config>
- <!-- 定义切点 -->
- <aop:pointcutid="login_PointcutTarget"expression="execution(* org.xmy.ldq.AOPBean.login(..))"/>
- <!-- 定义切面 -->
- <aop:aspectid="userAspect"ref="userAdvice">
- <aop:after-returning
- pointcut-ref="login_PointcutTarget"
- arg-names="joinPoint,retValue"
- returning="retValue"
- method="afterReturning"
- />
- </aop:aspect>
- </aop:config>
- 
- <beanid="userAdvice"class="org.xmy.ldq.aop.UserAdvice"></bean>
- <beanid="aopBean"class="org.xmy.ldq.AOPBean"></bean>
- 
- </beans>
 说明：arg-names="joinPoint,retValue" 被切目标方法的参数名称；
Xml代码  ![收藏代码](http://ldq2010-163-com.iteye.com/images/icon_star.png)
- returning="retValue" 目标方法成功执行后的返回值 一般类型定义为 Object；  
- method="afterReturning"目标方法名称  
如果程序中报出
java.lang.NoClassDefFoundError: org/aspectj/weaver/reflect/ReflectionWorld$ReflectionWorldException
是aspectjweaver.jar包丢失了，将其引入即可；
 Cannot proxy target class because CGLIB2 is not available. Add CGLIB to the class path or specify proxy interfaces.错误 是因为采用的CGLIB代理，应把相应的jar包引入。
