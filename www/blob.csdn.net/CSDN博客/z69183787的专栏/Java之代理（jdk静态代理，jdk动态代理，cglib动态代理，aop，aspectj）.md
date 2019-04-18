# Java之代理（jdk静态代理，jdk动态代理，cglib动态代理，aop，aspectj） - z69183787的专栏 - CSDN博客
2015年07月05日 11:57:41[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1353

## 一．概念
代理是什么呢？举个例子，一个公司是卖摄像头的，但公司不直接跟用户打交道，而是通过代理商跟用户打交道。如果：公司接口中有一个卖产品的方法，那么公司需要实现这个方法，而代理商也必须实现这个方法。如果公司卖多少钱，代理商也卖多少钱，那么代理商就赚不了钱。所以代理商在调用公司的卖方法后，加上自己的利润然后再把产品卖给客户。而客户部直接跟公司打交道，或者客户根本不知道公司的存在，然而客户最终却买到了产品。
专业点说：代理模式是对象的结构型模式，代码模式给某一个对象提供代理，并由代理对象控制原对象（目标对象，被代理对象）的引用。简单点说，就是通过一个工厂生成一个类的代理对象，当客户端使用的时候不直接使用目标对象，而是直接使用代理对象。
## 二．jdk的静态代理
Jdk的静态代理要求，目标对象和代理对象都要实现相同的接口。然后提供给客户端使用。这个代理对客户端是可见的，其结果图如下：![](http://hi.csdn.net/attachment/201110/7/0_13179544654uqU.gif)
下面给出一个例子：
首先建立1个接口：UserService.java定义如下方法：
**[java]**[view plain](http://blog.csdn.net/centre10/article/details/6847828#)[copy](http://blog.csdn.net/centre10/article/details/6847828#)[print](http://blog.csdn.net/centre10/article/details/6847828#)[?](http://blog.csdn.net/centre10/article/details/6847828#)
- package com.xie.service;  
- 
- publicinterface UserService {  
- publicvoid addUser(String userId,String userName);  
- publicvoid delUser(String userId);  
- publicvoid modfiyUser(String userId,String userName);  
- public String findUser(String userId);  
- }  
```java
package com.xie.service;
public interface UserService {
    public void addUser(String userId,String userName);
    public void delUser(String userId);
    public void modfiyUser(String userId,String userName);
    public String findUser(String userId);
}
```
然后实现这个接口的目标对象：UserServiceImpl.java
**package** com.xie.serviceimpl;
**import** com.xie.service.UserService;
**public class** UserServiceImpl **implements** UserService {
    @Override
**public void** addUser(String userId, String userName) {
       System.*out*.println("UserServiceImpl addUser userId->>"+userId);
    }
    @Override
**public void** delUser(String userId) {
       System.*out*.println("UserServiceImpl delUser userId->>"+userId);
    }
    @Override
**public void** modfiyUser(String userId, String userName) {
       System.*out*.println("UserServiceImpl modfiyUser userId->>"+userId);
    }
    @Override
**public** String findUser(String userId) {
       System.*out*.println("UserServiceImpl findUser userId->>"+userId);
**return** "张山";
    }
}
为目标对象创建代理对象：UserServiceImplProxy.java代理对象持有目标对象的引用。
**package** com.xie.serviceproxy;
**import** com.xie.service.UserService;
**public class** UserServiceImplProxy **implements** UserService {
**private** UserService userService;
**public** UserServiceImplProxy(UserService userService){
**this**.userService = userService;
    }
    @Override
**public void** addUser(String userId, String userName) {
**try** {
           System.*out*.println("开始执行：addUser");
           userService.addUser(userId, userName);
           System.*out*.println("addUser执行成功。");
       } **catch** (Exception e) {
           System.*out*.println("addUser执行失败。");
       }
    }
    @Override
**public void** delUser(String userId) {
    }
    @Override
**public void** modfiyUser(String userId, String userName) {
    }
    @Override
**public** String findUser(String userId) {
**return null**;
    }
}
最后调用代理对象完成功能：Client.java
package com.xie.client;
import com.xie.service.UserService;
import com.xie.serviceimpl.UserServiceImpl;
import com.xie.serviceproxy.UserServiceImplProxy;
public class Client {
         public static void main(String[] args) {
          UserService userService = new UserServiceImplProxy(new UserServiceImpl());
          userService.addUser("001", "centre");
         }
}
## 一．jdk动态代理
静态代理要为每个目标类创建一个代理类，当需要代理的对象太多，那么代理类也变得很多。同时代理类违背了可重复代理只写一次的原则。jdk给我们提供了动态代理。其原理图如下：
![](http://hi.csdn.net/attachment/201110/7/0_1317954717zSNU.gif)
Jdk的动态要求目标对象必须实现接口，因为它创建代理对象的时候是根据接口创建的。如果不实现接口，jdk无法给目标对象创建代理对象。被代理对象可以可以实现多个接口，创建代理时指定创建某个接口的代理对象就可以调用该接口定义的方法了。
首先定义2个接口：Service接口和UserService接口（上面的接口）
Service.java
**package** com.xie.service;
**public interface** Service {
**public void** sayHello(String name);
**public int** addOperter(**int** num,**int** num2);
}
然后定义实现这2个接口的目标对象：UserServiceImpl.java
package com.xie.serviceimpl;
import com.xie.service.Service;
import com.xie.service.UserService;
public class UserServiceImpl implements UserService ,Service{
         @Override
         public void addUser(String userId, String userName) {
                   System.out.println("UserServiceImpl addUser userId->>"+userId);
         }
         @Override
         public void delUser(String userId) {
                   System.out.println("UserServiceImpl delUser userId->>"+userId);
         }
         @Override
         public void modfiyUser(String userId, String userName) {
                   System.out.println("UserServiceImpl modfiyUser userId->>"+userId);
         }
         @Override
         public String findUser(String userId) {
                   System.out.println("UserServiceImpl findUser userId->>"+userId);
                   return "张山";
         }
         @Override
         public void sayHello(String name) {
                   System.out.println("你好："+name);
         }
         @Override
         public int addOperter(int num, int num2) {
                   return num+num2;
         }
}
提供一个生成代理对象的的类：LogHandler.java
package com.xie.serviceproxy;
import java.lang.reflect.InvocationHandler;
import java.lang.reflect.Method;
import java.lang.reflect.Proxy;
public class LogHandler implements InvocationHandler {
         private Object targertObject;
         public Object newInstance(Object targertObject){
                   this.targertObject = targertObject;
                   Class targertClass = targertObject.getClass();
                   return Proxy.newProxyInstance(targertClass.getClassLoader(), targertClass.getInterfaces(),this);
         }
         @Override
         public Object invoke(Object proxy, Method method, Object[] args)
                            throws Throwable {
                   System.out.println("调用方法"+method.getName());
                   Object ret = null;
                   try {
                            ret = method.invoke(targertObject, args);
                            System.out.print("成功调用方法："+method.getName()+";参数为：");
                            for (int i = 0; i < args.length; i++) {
                                     System.out.println(args[i]);
                            }
                   } catch (Exception e) {
                            e.printStackTrace();
                            System.out.print("调用方法："+method.getName()+"失败;参数为：");
                            for (int i = 0; i < args.length; i++) {
                                     System.out.print(args[i]);
                            }
                   }
                   return ret;
         }
}
编写一个客户端类来使用工厂类生成目标类的代理：Client.java
package com.xie.client;
import com.xie.service.Service;
import com.xie.service.UserService;
import com.xie.serviceimpl.UserServiceImpl;
import com.xie.serviceproxy.LogHandler;
public class Client {
         public static void main(String[] args) {
                   Service Service = (Service)new LogHandler().newInstance(new UserServiceImpl());
                   UserService userService = (UserService)new LogHandler().newInstance(new UserServiceImpl());
                   userService.addUser("001", "centre");
                   String name = userService.findUser("002");
                   System.out.println(name);
                   int num = Service.addOperter(12, 23);
                   System.out.println(num);
                   Service.sayHello("centre");
         }
}
## 一．cglib 动态代理
jdk给目标类提供动态要求目标类必须实现接口，当一个目标类不实现接口时，jdk是无法为其提供动态代理的。cglib 却能给这样的类提供动态代理。Spring在给某个类提供动态代理时会自动在jdk动态代理和cglib动态代理中动态的选择。
使用cglib为目标类提供动态代理：需要导入cglib.jar和asm.jar
如果出现asm中的类无法找到的异常，在java工程中是真的缺少asm.jar，而在web工程中很可能是asm.jar和spring提供的org.springframework.asm-3.0.4.RELEASE.jar包冲突。
首先编写一个目标类：UserServiceImpl.java（上面的类），
然后为其创建一个代理工厂，用于生成目标类的代理对象：CglibProxy.java
注意：如果一个类继承了某个类，在子类中没有一个方法，用cglib生成该子类的动态代理类中将没有一个方法。
package com.xie.serviceproxy;
import java.lang.reflect.Method;
import net.sf.cglib.proxy.MethodInterceptor;
import net.sf.cglib.proxy.MethodProxy;
public class CglibProxy implements MethodInterceptor{
         @Override
         public Object intercept(Object obj, Method method, Object[] args,
                            MethodProxy proxy) throws Throwable {
        System.out.println("调用的方法是：" + method.getName());
        Object ret = null;
        try {
            ret = proxy.invokeSuper(obj, args);
                            System.out.print("成功调用方法："+method.getName()+";参数为：");
                            for (int i = 0; i < args.length; i++) {
                                     System.out.print(args[i]);
                            }
                   } catch (Exception e) {
                            e.printStackTrace();
                            System.out.print("调用方法："+method.getName()+"失败;参数为：");
                            for (int i = 0; i < args.length; i++) {
                                     System.out.print(args[i]);
                            }
                   }
                   return ret;
         }
}
编写一个客户端使用代理工厂生成代理对象：CglibClient.java
package com.xie.client;
import net.sf.cglib.proxy.Enhancer;
import com.xie.serviceimpl.UserServiceImpl;
import com.xie.serviceproxy.CglibProxy;
public class CglibClient {
         public static void main(String[] args) {
                   cglibUse1();
         }
         public static void cglibUse1(){ 
                   Enhancer enhancer = new Enhancer();
                   // 设置被代理的类（目标类）
                   enhancer.setSuperclass(UserServiceImpl.class);
                   //使用回调
                   enhancer.setCallback(new CglibProxy());
                   // 创造 代理 （动态扩展了UserServiceImpl类）
                   UserServiceImpl my = (UserServiceImpl) enhancer.create();
                   //my.addUser("001", "centre");
                   int ret = my.addOperter(15, 22);
                   System.out.println("返回的结果是："+ret);
         }
}
## 五． jdk动态和cglib动态代理比较
Jdk动态代理要求被代理的类要实现接口，而cglib不需要，cglib能根据内存中为其创建子类（代理对象）。那么它们的效率是怎么样的呢？可以做如下测试：
我们用jdk和cglib动态代理分别为同一个代理创建10000个代理对象。
代码1：
**public static void** testFlexiable(){
       UserService test = **new** UserServiceImpl();
**long** nums = 1000;
       Date start = **new** Date();
**for** (**int** i = 0; i < nums; i++) {
           UserService userService = (UserService)**new** LogHandler().newInstance(test);
       }
       Date end = **new** Date();
       System.*out*.println("创建"+nums+"个代理代理对象用时："+(end.getTime()-start.getTime())+"毫秒。");
    }
测试结果：
创建1000个代理代理对象用时：32毫秒。
创建1000个代理代理对象用时：31毫秒。
创建1000个代理代理对象用时：31毫秒。
创建1000个代理代理对象用时：31毫秒。
创建10000个代理代理对象用时：94毫秒。
创建10000个代理代理对象用时：78毫秒。
创建10000个代理代理对象用时：78毫秒。
创建10000个代理代理对象用时：78毫秒。
代码2：
**public static void** testFlexiable(){
       Enhancer enhancer = **new** Enhancer();
       // 设置被代理的类（目标类）
       enhancer.setSuperclass(UserServiceImpl.**class**);
       //使用回调
       enhancer.setCallback(**new** CglibProxy());
**long** nums = 1000;
       Date start = **new** Date();
**for** (**int** i = 0; i < nums; i++) {
           UserServiceImpl my = (UserServiceImpl) enhancer.create();
       }
       Date end = **new** Date();
       System.*out*.println("创建"+nums+"个代理代理对象用时："+(end.getTime()-start.getTime())+"毫秒。");
    }
测试结果：
创建1000个代理代理对象用时：47毫秒。
创建1000个代理代理对象用时：62毫秒。
创建1000个代理代理对象用时：62毫秒。
创建1000个代理代理对象用时：47毫秒。
创建1000个代理代理对象用时：47毫秒。
创建1000个代理代理对象用时：47毫秒。
创建10000个代理对象会抛异常，cglib运行速度明显比jdk动态代理慢，由于是通过类创建的子类，比jdk通过接口创建代理更耗内存。因此在s2sh框架中，spring通过为类提供代理采用jdk比cglib应该要好一些吧。
## 六． 面向切面编程
面向切面编程是继面向对象后，又一种重要的思维方式。面向对象比较重要的是通过继承实现代码重用。而面向切面编程，则注重纵向编程，他能将2个不同的功能分开，实现最大程度的解耦，比如我们现在有业务逻辑层和日志层，如果不分开，那么在每个业务逻辑方法中除了要实现业务外还要加上日志代码，如果某一天我不需要日志了，而有很多这样的类的，很多方法都加上日志代码，那改动的工作量是可想而知的。有没有一种方法让我们只写一次日志代码，而把他应用在需要写日志的方法前面，当不需要的时候直接删除呢？面向切面编程给我们提供了办法。
Struts2的拦截器就是采用这种思想编写的。下面模拟实现一个拦截器，设计图如下：
![](http://hi.csdn.net/attachment/201110/7/0_13179548637yLD.gif)
首先定义一个拦截器接口：Interceptor.java
**package** com.xie.interceptor;
**public interface** Interceptor {
**public void** intercept(ActionInvocation invocation);
}
然后实现拦截器接口，实现3个拦截器：
FirstInterceptor.java,SecondInterceptor.java,ThirdInterceptor.java
**package** com.xie.interceptor;
**public class** FirstInterceptor **implements** Interceptor {
    @Override
**public void** intercept(ActionInvocation invocation) {
        System.*out*.println(1);
        invocation.invoke();
        System.*out*.println(-1);
    }
}
**package** com.xie.interceptor;
**public class** SecInterceptor **implements** Interceptor {
    @Override
**public void** intercept(ActionInvocation invocation) {
        System.*out*.println(2);
        invocation.invoke();
        System.*out*.println(-2);
    }
}
**package** com.xie.interceptor;
**public class** ThirInterceptor **implements** Interceptor{
    @Override
**public void** intercept(ActionInvocation invocation) {
       System.*out*.println(3);
       invocation.invoke();
       System.*out*.println(-3);
    }     
}
接下来定义一个InvokeInterceptory.java
package com.xie.interceptor;
import java.util.ArrayList;
import java.util.List;
public class ActionInvocation {
         List<Interceptor> interceptors=new ArrayList<Interceptor>();
         int index=-1;
         Action action=new Action();
         public ActionInvocation(){
                   this.interceptors.add(new FirstInterceptor());
                   this.interceptors.add(new SecInterceptor());
                   this.interceptors.add(new ThirInterceptor());
         }
    public void invoke(){
             if (index+1>=this.interceptors.size()) {
                            action.execute();
                   }else {
                            index++;
                            this.interceptors.get(index).intercept(this);
                   }
    }
}
定义一个action：Action.java
**package** com.xie.interceptor;
**public class** Action {
**public void** execute(){
     System.*out*.println("execute action.");
     }
}
最后定义Main类来调用action的execute方法：
**package** com.xie.interceptor;
**public class** Main {
**public static void** main(String[] args) {
**new** ActionInvocation().invoke();
    }
}
运行结果如下：
1
2
3
execute action.
-3
-2
-1
在javaEE中，像Filter（过滤器），Intercepetor（拦截器），spring的事务管理都采用了面向切面的编程思想。
### 1.几个应用
写一个过滤器：这个过滤器的功能是实现web页面访问权限：
package com.ibeifeng.filter;
import java.io.IOException;
import javax.servlet.Filter;
import javax.servlet.FilterChain;
import javax.servlet.FilterConfig;
import javax.servlet.ServletException;
import javax.servlet.ServletRequest;
import javax.servlet.ServletResponse;
import javax.servlet.http.HttpServletRequest;
import javax.servlet.http.HttpServletResponse;
import javax.servlet.http.HttpSession;
public class LoginFilter implements Filter {
         public void destroy() {
         }
         public void doFilter(ServletRequest req, ServletResponse res,
                            FilterChain chain) throws IOException, ServletException {
                   HttpServletRequest request = (HttpServletRequest) req;
                   HttpSession session = request.getSession();
                   String username = (String) session.getAttribute("username");
                   HttpServletResponse response = (HttpServletResponse) res;
                   String uri = request.getRequestURI();
                   //如果用户请求了index.html，这时就必须做登录判断，判断用户是否登录。
                   if("/Pfms/index.html".equals(uri)) {
                            if(username == null || "".equals(username)) {
                                     response.sendRedirect("login.html");
                            } else {
                                     chain.doFilter(request, response);
                            }
                   }else {
                            chain.doFilter(request, response);
                   }
         }
         public void init(FilterConfig filterConfig) throws ServletException {
         }
}
再写一个struts2的拦截器：
package interceptor;
import com.opensymphony.xwork2.ActionInvocation;
import com.opensymphony.xwork2.interceptor.*;
import com.opensymphony.xwork2.*;
import java.util.*;
public class AuthorizationInterceptor extends AbstractInterceptor
{
         private String ignoreActions;
         // ignoreActions属性的getter方法
         public String getIgnoreActios()
         {
                   return ignoreActions;
         }
         // ignoreActions属性的setter方法
         public void setIgnoreActions(String ignoreActions)
         {
                   this.ignoreActions = ignoreActions;
         }
         @Override
         public String intercept(ActionInvocation invocation) throws Exception
         {
                   ActionContext ctx = invocation.getInvocationContext();
                   Map session = ctx.getSession();
                   String user = (String) session.get("username");
                   boolean ignore = false;
                   String currentAction = invocation.getProxy().getActionName();
                   String[] actions = ignoreActions.split(",");
                   for (String action : actions)
                   {
                            if (currentAction.matches(action.trim()))
                            {
                                     ignore = true;
                                     break;
                            }
                   }
                   if (user != null || ignore == true)
                   {
                            return invocation.invoke();
                   }
                   else
                   {
                            return Action.LOGIN;
                   }
         }
}
看看spring是如何配置事物的（xml）：
    <!-- 配置事务管理器 -->
    <bean id=*"txManager"*
       class=*"org.springframework.orm.hibernate3.HibernateTransactionManager"*>
       <property name=*"sessionFactory"*>
           <ref local=*"sessionFactory"* />
       </property>
       <property name=*"nestedTransactionAllowed"* value=*"true"* />
    </bean>
    <!-- 配置事务的传播特性 -->
    <tx:advice id=*"txAdvice"* transaction-manager=*"txManager"*>
       <tx:attributes>
           <!-- 在开发的时候可以这样定义，但部署的时候一定要详细定义 -->
           <tx:method name=*"*"* propagation=*"REQUIRED"* />
           <tx:method name=*"get*"* read-only=*"true"* />
           <tx:method name=*"load*"* read-only=*"true"* />
           <tx:method name=*"find*"* read-only=*"true"* />
           <tx:method name=*"query*"* read-only=*"true"* />
           <tx:method name=*"pagedQuery*"* read-only=*"true"* />
           <tx:method name=*"*"* />
       </tx:attributes>
    </tx:advice>
    <!-- 以AspectJ方式 定义 AOP -->
    <aop:config proxy-target-class=*"true"* expose-proxy=*"true"*>
       <aop:advisor pointcut=*"execution(public * com.hikvision.webclient8100.manager.*.*(..))"*
           advice-ref=*"txAdvice"* />
    </aop:config>
### 2.面向切面的概念
1）aspect（切面）：实现了cross-cutting功能，是针对切面的模块。最常见的是logging模块，这样，程序按功能被分为好几层，如果按传统的继承的话，商业模型继承日志模块的话根本没有什么意义，而通过创建一个logging切面就可以使用AOP来实现相同的功能了。
将横切多个业务对象的程序对了出来，模块化，该模块可以无侵入式的集成到业务对象中，如：事务，日志，权限等。
2）jointpoint（连接点）：连接点是切面插入应用程序的地方，该点能被方法调用，而且也会被抛出意外。连接点是应用程序提供给切面插入的地方，可以添加新的方法。比如以上我们的切点可以认为是findInfo(String)方法。
通知执行的时机，如方法调用，抛出异常时。 
3）advice（处理逻辑）：advice是我们切面功能的实现，它通知程序新的行为。如在logging里，logging advice包括logging的实现代码，比如像写日志到一个文件中。advice在jointpoint处插入到应用程序中。以上我们在MyHandler.java中实现了advice的功能 。
Advice（通知）：指切面的具体实现，如记录日志，验证权限。通知有各种类型，其中包括“before”，“after”，“around”和“throw”等通知
4）pointcut（切入点）：pointcut可以控制你把哪些advice应用于jointpoint上去，通常你使用pointcuts通过正则表达式来把明显的名字和模式进行匹配应用。决定了那个jointpoint会获得通知。
切入点：是感兴趣的连接点。通知和一个切入点表达式关联，并在满足这个切入点上运行，（如：执行某个特定名称的方法时。）切入点表达式如何和连接点匹配时AOP的核心：spring缺省使用AspectJ切入点语法。
　　5）introduction：允许添加新的方法和属性到类中。 
6）target（目标类）：是指那些将使用advice的类，一般是指独立的那些商务模型。比如以上的StudentInfoServiceImpl. 
是一个被代理对象，被通知对象，被一个或者多个切面所通知的对象。
7)proxy（代理类）：使用了proxy的模式。是指应用了advice的对象，看起来和target对象很相似。 
AOP代理的对象，用来实现切面的功能，在spring中，AOP代理可以使用jdk动态代理和cglib动态代理。
8）weaving(插入）：是指应用aspects到一个target对象创建proxy对象的过程：complie time，classload time，runtime。把切面连接到其它应用程序类型或者对象上，并创建一个被通知的对象，在运行时完成织入。
### 3．使用aspectj实现aop编程
使用aspectj编写系统日志。
Aspectj是一个使用面向切面，底层采用动态代理的框架。AspectJ是一个面向切面的框架，它扩展了Java语言。AspectJ定义了AOP语法所以它有一个专门的编译器用来生成遵守Java字节编码规范的Class文件。spring的AOP实现使用了Aspectj框架。
1.       采用annotation
a)首先开启aspectj支持，如下：
<?xml version=*"1.0"* encoding=*"UTF-8"*?>
<beans default-lazy-init=*"false"*
    xmlns=*"http://www.springframework.org/schema/beans"* xmlns:xsi=*"http://www.w3.org/2001/XMLSchema-instance"*
    xmlns:context=*"http://www.springframework.org/schema/context"*
    xmlns:aop=*"http://www.springframework.org/schema/aop"* xmlns:tx=*"http://www.springframework.org/schema/tx"*
    xsi:schemaLocation=*"*
*           http://www.springframework.org/schema/beans*
*           http://www.springframework.org/schema/beans/spring-beans-3.0.xsd*
*           http://www.springframework.org/schema/context*
*           http://www.springframework.org/schema/context/spring-context-3.0.xsd*
*           http://www.springframework.org/schema/tx*
*           http://www.springframework.org/schema/tx/spring-tx-3.0.xsd*
*           http://www.springframework.org/schema/aop*
*[http://www.springframework.org/schema/aop/spring-aop-3.0.xsd](http://www.springframework.org/schema/aop/spring-aop-3.0.xsd)"*>
    <aop:aspectj-autoproxy></aop:aspectj-autoproxy>
 </beans>
如果采用dtd：
b)定义切面对象
@Aspect
c)申明切入点
任何public方法的执行:
execution(public * *(..))
任何以set开头的方法：
execution(* set*(..))
任何在接口AccountService的方法:
execution(* com.xyz.service.AccountService.*(..))
任何在service包的方法:
execution(* com.xyz.service.*.*(..))
任何在service包和其子包的方法:
execution(* com.xyz.service..*.*(..))
any join point (执行方法必须在Spring Aop方式下) within the service package:
within(com.xyz.service.*)
any join point (执行方法必须在Spring Aop方式下) within the service package or a sub-package:
within(com.xyz.service..*)
any join point (执行方法必须在Spring Aop方式下) where the proxy implements the AccountService interface:
this(com.xyz.service.AccountService)
any join point (method execution only in Spring AOP) where the target object implements the AccountService interface:
target(com.xyz.service.AccountService)
'target' is more commonly used in a binding form :- see the following section on advice for how to make the target object available in the advice body.
any join point (method execution only in Spring AOP) which takes a single parameter, and where the argument passed at runtime is Serializable:
args(java.io.Serializable)
'args' is more commonly used in a binding form :- see the following section on advice for how to make the method arguments available in the advice body.
any join point (method execution only in Spring AOP) where the target object has an @Transactional annotation:
@target(org.springframework.transaction.annotation.Transactional)
'@target' can also be used in a binding form :- see the following section on advice for how to make the annotation object available in the advice body.
any join point (method execution only in Spring AOP) where the declared type of the target object has an @Transactional annotation:
@within(org.springframework.transaction.annotation.Transactional)
'@within' can also be used in a binding form :- see the following section on advice for how to make the annotation object available in the advice body.
any join point (method execution only in Spring AOP) where the executing method has an @Transactional annotation:
@annotation(org.springframework.transaction.annotation.Transactional)
'@annotation' can also be used in a binding form :- see the following section on advice for how to make the annotation object available in the advice body.
any join point (method execution only in Spring AOP) which takes a single parameter, and where the runtime type of the argument passed has the @Classified annotation:
@args(com.xyz.security.Classified)
'@args' can also be used in a binding form :- see the following section on advice for how to make the annotation object(s) available in the advice body.
any join point (method execution only in Spring AOP) on a Spring bean named 'tradeService':
bean(tradeService)
any join point (method execution only in Spring AOP) on Spring beans having names that match the wildcard expression '*Service':
bean(*Service)
d)切入点表达式：
execution(modifiers-pattern? ret-type-pattern declaring-type-pattern? 
name-pattern(param-pattern) throws-pattern?)
--modifiers-pattern 访问修饰符，public，private等
-- ret-type-pattern 返回类型，void，String等
-- declaring-type-pattern 申明类型
-- name-pattern 方法名称 
-- param-pattern 参数名称
-- throws-pattern 异常名称
编写一个接口：ServiceAspectjAnnotation.java
**package** com.xie.aspectj.test;
**import** java.util.List;
**import** com.hikvision.webclient8100.domain.Depart;
**public interface** ServiceAspectjAnnotation {
**public boolean** delete(List<Depart> list);
**public boolean** change(List<Depart> list);
**public** List<Depart> queryAll();
**public** Depart queryByName(String pname);
**public** Depart queryBySequenceNo(String sequenceNo);
**public** String pageDivide(**int** index,
**int** pageSize);
**public** String findParentOrgsName() **throws** Exception;
**boolean** save(List<Depart> list);
}
实现该接口：ServiceAspectjAnnotationImpl.java
**package** com.xie.aspectj.test;
**import** java.util.List;
**import** org.springframework.stereotype.Component;
**import** com.hikvision.webclient8100.domain.Depart;
@Component(value="serviceAspectjAnnotationImpl")
**public class** ServiceAspectjAnnotationImpl //implements ServiceAspectjAnnotation
{
**public boolean** save(List<Depart> list) {
       System.*out*.println("保存方法执行成功。");
**return false**;
    }
**public boolean** delete(List<Depart> list) {
       System.*out*.println("删除方法执行成功。");
**return false**;
    }
**public boolean** change(List<Depart> list) {
       System.*out*.println("修改方法执行成功。");
**return false**;
    }
**public** List<Depart> queryAll() {
       System.*out*.println("查询成功。");
**return null**;
    }
**public** Depart queryByName(String pname) {
       System.*out*.println("通过名字查询方法执行成功。");
**return null**;
    }
**public** Depart queryBySequenceNo(String sequenceNo) {
       System.*out*.println("通过序列号查询成功。");
**return null**;
    }
**public** String pageDivide(**int** index,
**int** pageSize) {
       System.*out*.println("分页方法成功返回。");
**return null**;
    }
**public** String findParentOrgsName() **throws** Exception {
       System.*out*.println("查找上级组织名称成功。");
**throw new** Exception();
    }
}
建立一个日志类：LogHandler.java
**package** com.hikvision.webclient8100.util;
**import** org.apache.log4j.Logger;
**import** org.aspectj.lang.ProceedingJoinPoint;
**import** org.aspectj.lang.annotation.After;
**import** org.aspectj.lang.annotation.AfterReturning;
**import** org.aspectj.lang.annotation.AfterThrowing;
**import** org.aspectj.lang.annotation.Around;
**import** org.aspectj.lang.annotation.Aspect;
**import** org.aspectj.lang.annotation.Before;
**import** org.springframework.stereotype.Component;
@Component(value = "logHandler")
@Aspect
// 切面
**public class** LogHandler {
**private static** Logger *logger* = Logger.*getLogger*(LogHandler.**class**);
    @Before// 通知
    // * com.hikvision.webclient8100.manager.*.*(..))切点
    ("execution(* com.hikvision.webclient8100.manager.*.*(..))")
    // 连接点，连接点是切面插入应用程序的地方
**public void** manager() {// 通知，面向切面加入的业务逻辑
*logger*.info("manager层的某个方法开始执行");
    }
    @After("execution(* com.xie.aspectj.test.*.*(..))")
**public void** dao() {
*logger*.info("dao层的某个方法执行完成");
    }
    @Around("execution(* com.xie.aspectj.test.ServiceAspectjAnnotationImpl.change(..))")
**public void** arund(ProceedingJoinPoint pjp) **throws** Throwable {
*logger*.info("方法开始执行....................");
       pjp.proceed();
*logger*.info("方法执行完成。");
    }
    @AfterThrowing(pointcut = "execution(* com.xie.aspectj.test.*.*(..))", throwing = "ex")
**public void** doRecovery() {
*logger*.error("出现异常。");
    }
    @AfterReturning("execution(* com.xie.aspectj.test.ServiceAspectjAnnotationImpl.delete(..))")
**public void** reOpertion() {
*logger*.info("操作完成");
    }
}
编写测试方法：ServiceAspectjAnnotationImplTest.java
package com.xie.aspectj.test;
import java.util.ArrayList;
import java.util.List;
import org.junit.AfterClass;
import org.junit.BeforeClass;
import org.junit.Test;
import org.springframework.context.support.ClassPathXmlApplicationContext;
import com.hikvision.webclient8100.dao.DepartDao;
import com.hikvision.webclient8100.domain.Depart;
public class ServiceAspectjAnnotationImplTest {
         private static ClassPathXmlApplicationContext atc = new 
ClassPathXmlApplicationContext("applicationContext.xml");
         @BeforeClass
         public static void setUpBeforeClass() throws Exception {
         }
         @AfterClass
         public static void tearDownAfterClass() throws Exception {
         }
         @Test
         public void testSave(){
                   ServiceAspectjAnnotation serviceAspectjAnnotation = (ServiceAspectjAnnotation) atc.getBean("serviceAspectjAnnotationImpl");
                   System.out.println(serviceAspectjAnnotation.save(new ArrayList<Depart>()));
         }
         @Test
         public void testSave1(){
                   ServiceAspectjAnnotationImpl serviceAspectjAnnotation = 
(ServiceAspectjAnnotationImpl) atc.getBean("serviceAspectjAnnotationImpl");
                   System.out.println(serviceAspectjAnnotation.save(new ArrayList<Depart>()));
         }
         @Test
         public void testDelete(){
                   ServiceAspectjAnnotationImpl serviceAspectjAnnotation = 
(ServiceAspectjAnnotationImpl) atc.getBean("serviceAspectjAnnotationImpl");
                   System.out.println(serviceAspectjAnnotation.delete(new ArrayList<Depart>()));
         }
         @Test
         public void testChange(){
                   ServiceAspectjAnnotationImpl serviceAspectjAnnotation = 
(ServiceAspectjAnnotationImpl) atc.getBean("serviceAspectjAnnotationImpl");
                   System.out.println(serviceAspectjAnnotation.change(new ArrayList<Depart>()));
         }
         @Test
         public void testFind() throws Exception{
                   ServiceAspectjAnnotationImpl serviceAspectjAnnotation = 
(ServiceAspectjAnnotationImpl) atc.getBean("serviceAspectjAnnotationImpl");
                   serviceAspectjAnnotation.findParentOrgsName();
         }
}
2．使用xml：
    <!-- <bean id="logHandler1" class="com.hikvision.webclient8100.util.LogHandler1"></bean> -->
<!—采用了spring的注解生成bean-->
    <aop:config><!-- spring 的配置 -->
       <aop:aspect ref=*"logHandler1"*><!-- 定义切面 -->
           <aop:pointcut id=*"manager1"*
              expression=*"execution(* com.hikvision.webclient8100.manager.*.*(..))"* /><!-- 定义切点 -->
           <aop:before pointcut-ref=*"manager1"* method=*"reOpertion"* /><!-- 通知执行时机，这是我们要在主业务逻辑出要加入的代码 -->
       </aop:aspect>
       <aop:aspect ref=*"logHandler1"*>
           <aop:pointcut id=*"test"*
              expression=*"execution(* com.xie.aspectj.test.*.*(..))"* />
           <aop:after pointcut-ref=*"test"* method=*"dao"* />
       </aop:aspect>
       <aop:aspect ref=*"logHandler1"*>
           <aop:pointcut id=*"around"*
              expression=*"execution(* com.xie.aspectj.test.ServiceAspectjAnnotationImpl.change(..))"* />
           <aop:around pointcut-ref=*"around"* method=*"arund"* />
       </aop:aspect>
       <aop:aspect ref=*"logHandler1"*>
           <aop:pointcut id=*"throw"*
              expression=*"execution(* com.xie.aspectj.test.*.*(..))"* />
           <aop:after-throwing pointcut-ref=*"throw"* method=*"doRecovery"*
              throwing=*"ex"* />
       </aop:aspect>
       <aop:aspect ref=*"logHandler1"*>
           <aop:pointcut id=*"afterreturn"*
              expression=*"execution(* com.xie.aspectj.test.ServiceAspectjAnnotationImpl.delete(..))"* />
           <aop:after-returning pointcut-ref=*"afterreturn"*
              method=*"reOpertion"* />
       </aop:aspect>
    </aop:config>
## 七．总结
面向切面（方面）编程：实际上就是在不修改原来代码的前提下为原来的业务增加新的逻辑代码。它可以方便的添加和删除，让代码维护起来更加方便。Aspectj是一个AOP框架，底层实现采用动态代理。Spring的AOP采用aspectj，动态代理采用jdk和cglib中切换。
