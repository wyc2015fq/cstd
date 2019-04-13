
# Spring 入门 AOP - 萌萌的It人 www.itmmd.com - CSDN博客


2013年12月04日 13:25:27[Jlins](https://me.csdn.net/dyllove98)阅读数：1111


通过一个小例子演视怎么使用 Spring 现实面向切面编程。
##### 导入 Spring 所需要的包
spring-framework-2.5.6 版需要导入以下包：
1.----- spring.jar
2.----- commons-logging.jar
3.----- aspectjrt.jar
4.----- aspectjweaver.jar
5.----- cglib-nodep-2.1_3.jar
spring-framework-3.2.4 版需要导入以下包：
1.----- spring-core-3.2.4.RELEASE.jar
2.----- spring-beans-3.2.4.RELEASE.jar
3.----- spring-context-3.2.4.RELEASE.jar
4.----- spring-expression-3.2.4.RELEASE.jar
5.----- commons-logging.jar
6.----- aspectjweaver.jar
7.----- aspectjrt.jar
8.----- aopalliance.jar
**代码：**
Book.java：
publicclassBook {privateintid;privateString name;//省略get set方法....}
BookService.java：
publicclassBookService {publicvoidsave(Book book){
        System.out.println("save:"+book.getName());
    }
    
}
main方法：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicstaticvoidmain(String[] args) {
    ApplicationContext appctx=newClassPathXmlApplicationContext("applicationContext.xml");
    Book b= (Book)appctx.getBean("book");
    b.setName("j2ee");
    BookService bs= (BookService)appctx.getBean("bookService");//调用BookService的save方法。bs.save(b);
    ((ClassPathXmlApplicationContext)appctx).close();
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
通过使用Spring AOP 在调用BookService的save方法前后各加上一些业务（如记录日志，时间等等）方便起见，这里简单输出两句话。

## 使用配置文件配置AOP(Schema)
添加切面类 Log.java ：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
publicclassLog {//在目标方法执行前调用此方法，JoinPoint:封装了目标方法的一些信息（如类名，参数等等）publicvoidbefore(JoinPoint jp){
        
        System.out.println(jp.getSignature().getName()+":开始执行----------");//获取被代理对象System.out.println("被代理对象："+jp.getTarget().getClass());//获取被代理方法System.out.println("被代理方法:"+jp.getSignature());//获取方法参数System.out.println("方法参数:"+jp.getArgs());
    }//在目标方法执行完后调用此方法，Object rn:目标方法的返回值（void为null）publicvoidafterreturning(JoinPoint jp,Object rn){
        System.out.println(jp.getSignature().getName()+":执行完毕----------");
        
        System.out.println(rn);
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
Spring配置文件:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:aop="http://www.springframework.org/schema/aop"xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
http://www.springframework.org/schema/aop
http://www.springframework.org/schema/aop/spring-aop-2.5.xsd"><beanid="book"class="com.startspring.Book"/><beanid="bookService"class="com.startspring.BookService"/><beanid="log"class="com.startspring.aop.Log"/><aop:config><!--指定切入点--><aop:pointcutid="bookSave"expression="execution(public void com.startspring.BookService.save(com.startspring.Book))"/><!--指定切面--><aop:aspectid="aspect"ref="log"><!--前置通知（增强）:指明切入点方法执行前执行 “before”方法--><aop:beforemethod="before"pointcut-ref="bookSave"/><!--后置通知（增强）:指明切入点方法完成后执行 “afterreturning”方法,参数rn是目标方法的返回值--><aop:after-returningmethod="afterreturning"pointcut-ref="bookSave"returning="rn"/></aop:aspect></aop:config></beans>![复制代码](http://common.cnblogs.com/images/copycode.gif)
注：要使用aop:标签要先引入aop命名空间（第4、7、8行）

## 使注解配置AOP(Annotation)
Spring配置文件:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:context="http://www.springframework.org/schema/context"xmlns:aop="http://www.springframework.org/schema/aop"xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
http://www.springframework.org/schema/context
http://www.springframework.org/schema/context/spring-context-2.5.xsd
http://www.springframework.org/schema/aop
http://www.springframework.org/schema/aop/spring-aop-2.5.xsd"><context:component-scanbase-package="com.startspring"/><!--要使用aspect的注解，先要在配置文件中添加如下代码:启动自动匹配注解类--><aop:aspectj-autoproxy/></beans>![复制代码](http://common.cnblogs.com/images/copycode.gif)
添加切面类 Log.java ：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
@Component
@Aspect//声明该类为切面类publicclassLog {//前置通知（增强）:指明切入点方法执行前执行该方法//后面参数指定了插入点@Before("execution(public void com.startspring.BookService.save(com.startspring.Book))")publicvoidbefore(JoinPoint jp){
        System.out.println(jp.getSignature().getName()+":开始执行----------");//获取被代理对象System.out.println("被代理对象："+jp.getTarget().getClass());//获取被代理方法System.out.println("被代理方法:"+jp.getSignature());//获取方法参数System.out.println("方法参数:"+jp.getArgs());
    }//后置通知（增强）:指明切入点方法执行完毕执行该方法//returning="rn" ：声明参数rn是目标方法的返回值(void为null)@AfterReturning(returning="rn",value="execution(public void com.startspring.BookService.save(com.startspring.Book))")publicvoidafterreturning(JoinPoint jp,Object rn){
        System.out.println(jp.getSignature().getName()+":开始完毕----------");
        System.out.println(rn);
        
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
这里Bean的声明也采用了注解，要在Book.java和BookService.java里添加相应的注解。或在配置文件中写<bean>也是可以的。

## 通过现实接口配置AOP
添加切面类 Log.java ：
![复制代码](http://common.cnblogs.com/images/copycode.gif)
/** 通过实现 MethodBeforeAdvice 实现前置处理。AfterReturningAdvice：后置处理。
 * 分别重写方法:before，afterReturning*/publicclassLogimplementsMethodBeforeAdvice,AfterReturningAdvice{//method:表示切入点方法.args：切入点方法的参数.target:目标对象publicvoidbefore(Method method, Object[] args, Object target)throwsThrowable {
        System.out.println(method.getName()+":开始执行----------");//获取被代理对象System.out.println("被代理对象："+target);//获取被代理方法System.out.println("被代理方法:"+method);//获取方法参数System.out.println("方法参数:"+args);
        
    }//result表示目标方法的返回值publicvoidafterReturning(Object result, Method method, Object[] args,Object target)throwsThrowable {
        System.out.println(method.getName()+":执行完毕----------");
        
        System.out.println(result);
        
    }
}![复制代码](http://common.cnblogs.com/images/copycode.gif)
Spring配置文件:
![复制代码](http://common.cnblogs.com/images/copycode.gif)
<?xml version="1.0" encoding="UTF-8"?><beansxmlns="http://www.springframework.org/schema/beans"xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"xmlns:aop="http://www.springframework.org/schema/aop"xsi:schemaLocation="http://www.springframework.org/schema/beans
http://www.springframework.org/schema/beans/spring-beans-2.5.xsd
http://www.springframework.org/schema/aop
http://www.springframework.org/schema/aop/spring-aop-2.5.xsd"><beanid="book"class="com.startspring.Book"/><beanid="bookService"class="com.startspring.BookService"/><beanid="log"class="com.startspring.aop.Log"/><aop:config><!--指定切入点--><aop:pointcutid="bookSave"expression="execution(public void com.startspring.BookService.save(com.startspring.Book))"/><!--advice-ref:指定切面   pointcut-ref:指定切入点--><aop:advisoradvice-ref="log"pointcut-ref="bookSave"/></aop:config></beans>![复制代码](http://common.cnblogs.com/images/copycode.gif)






