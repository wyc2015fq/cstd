# 基于注解的Spring MVC（上篇） - 建建的博客 - CSDN博客
2017年12月21日 09:42:34[纪建](https://me.csdn.net/u013898698)阅读数：100
个人分类：[java学习路线](https://blog.csdn.net/u013898698/article/category/7152763)
什么是Spring MVC
Spring MVC框架是一个MVC框架，通过实现Model-View-Controller模式来很好地将数据、业务与展现进行分离。从这样一个角度来说，Spring MVC和Structs、Structs2非常类似。Spring MVC的设计是围绕DispatcherServlet展开的，DispatcherServlet负责将请求派发到特定的handler。通过可配置的hander mappings、view
 resolution、locale以及theme resolution来处理请求并且转到对应的视图。Spring MVC请求处理的整体流程如图：
![](http://images2015.cnblogs.com/blog/801753/201603/801753-20160331220949910-293084349.png)
Spring MVC有基于注解版与基础.xml版的两种用法，不过现在的企业级开发基本都使用的是注解版，没别的原因，就是方便而已。因此后面的代码示例，都是基于注解版本的，想了解基于.xml版本的Spring MVC的朋友可以自行上网查询。
Spring MVC环境搭建
要开始本文后面的内容，自然要搭建一个Spring MVC的环境，那么首先建立一个Java Web的工程，我建立的工程名字叫做SpringMVC，要搭建一个基础功能的Spring MVC环境，必须引入的jar包是beans、context、core、expression、web、webmvc以及commons-logging。
然后，对web.xml添加一些内容：
```
<?xml version="1.0" encoding="UTF-8"?>
<web-app version="2.5" 
    xmlns="http://java.sun.com/xml/ns/javaee" 
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" 
    xsi:schemaLocation="http://java.sun.com/xml/ns/javaee 
    http://java.sun.com/xml/ns/javaee/web-app_2_5.xsd">
    
    <display-name></display-name>    
    <welcome-file-list>
        <welcome-file>index.jsp</welcome-file>
    </welcome-file-list>
      
      <!-- 该监听器将在Web容器启动时激活Spring -->
    <listener>
        <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
    </listener>
    
    <!-- 处理由JavaBeans,Introspector的使用而引起的缓冲泄露,建议配置此监听器 -->
    <listener>
        <listener-class>org.springframework.web.util.IntrospectorCleanupListener</listener-class>
    </listener>
  
      <!--configure the setting of springmvcDispatcherServlet and configure the mapping-->
    <servlet>
        <servlet-name>springmvc</servlet-name>
        <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
        <init-param>
              <param-name>contextConfigLocation</param-name>
              <param-value>classpath:springmvc-servlet.xml</param-value>
          </init-param>
          <load-on-startup>1</load-on-startup> 
    </servlet>
 
    <servlet-mapping>
        <servlet-name>springmvc</servlet-name>
        <url-pattern>/</url-pattern>
    </servlet-mapping>
</web-app>
```
两个listener不是必须的，但是servlet是必须的，url-pattern用于开发者选择哪些路径是需要让Spring MVC来处理的。接着在classpath下按照我们约定的名字springmvc-servlet.xml写一个xml文件：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"   
    xmlns="http://www.springframework.org/schema/beans"  
    xmlns:mvc="http://www.springframework.org/schema/mvc"  
    xmlns:context="http://www.springframework.org/schema/context"  
    xmlns:aop="http://www.springframework.org/schema/aop"  
    xmlns:tx="http://www.springframework.org/schema/tx"  
    xsi:schemaLocation="http://www.springframework.org/schema/beans 
        http://www.springframework.org/schema/beans/spring-beans-4.2.xsd
        http://www.springframework.org/schema/mvc
        http://www.springframework.org/schema/mvc/spring-mvc-4.2.xsd
        http://www.springframework.org/schema/context
        http://www.springframework.org/schema/context/spring-context-4.2.xsd">    
    
    <context:annotation-config />    
    <context:component-scan base-package="com.xrq.controller"/>
    
    <!-- 配置视图解析器 -->
    <bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">  
        <!-- WebRoot到一指定文件夹文件路径 -->  
        <property name="prefix" value="/" />  
        <!-- 视图名称后缀  -->  
        <property name="suffix" value=".jsp" />  
    </bean>  
</beans>
```
另外，由于使用了Spring，所以Tomcat启动的时候默认会去WEB-INF下找applicationContext.xml，所以放一个空的applicationContext.xml到WEB-INF下：
```
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
    xsi:schemaLocation="http://www.springframework.org/schema/beans
    http://www.springframework.org/schema/beans/spring-beans-4.2.xsd">
    
</beans>
```
写一个Java POJO，用于处理具体的请求：
```
@Controller
@RequestMapping(value = "/test")
public class TestController
{
    @RequestMapping
    public String dispatchTest()
    {
        System.out.println("Enter TestController.dispatchTest");
        return "test";
    }
}
```
注意，这里有一个@Controller，这个注解和@Service注解意思差不多，都表示该类是一个Spring Bean，这样就不需要再在Spring文件中为这个类定义Bean了。
另外，由于我前面在springmvc-servlet.xml中配置了prefix和suffix，因此return的时候就可以方便一些，不需要写前缀和后缀，Spring MVC默认会转发到（请求转发是Spring MVC默认的页面跳转方式）"/test.jsp"路径下。
最后别忘了，因为在web.xml中设置了启动激活Spring，因此还需要写一个applicationContext.xml（Spring文件的默认名字），当然，里面除了基本的声明，什么实际内容都不需要。最终，WebRoot文件夹应该是这么一个结构：
![](http://images2015.cnblogs.com/blog/801753/201603/801753-20160331203407879-836578753.png)
最后，启动容器，访问"localhost:8080/SpringMVC/test"，容器就会把该请求转发到"localhost:8080/SpringMVC/test.jsp"页面下了。
@RequestMapping注解
Spring MVC中最重要的注解应该就是@RequestMapping了，它是用于处理请求映射的。继续看刚才的TestController：
```
@Controller
@RequestMapping(value = "/test")
public class TestController
{
    @RequestMapping
    public String dispatchTest()
    {
        System.out.println("Enter TestController.dispatchTest()");
        return "test";
    }
}
```
类上的RequestMapping是用于第一层匹配的。"localhost:8080/SpringMVC/test"和"localhost:8080/SpringMVC/add"，value是test，自然走的是前者。
接着看，比如我在TestController中又定义了三个方法，此时类上不使用RequestMapping注解：
```
@RequestMapping(value = "/add")
public String dispatchAddTest()
{
    System.out.println("Enter TestControll.dispatchAddTest()");
    return "test";
}
    
@RequestMapping(value = "/add/add")
public String dispatchAddAddTest()
{
    System.out.println("Enter TestControll.dispatchAddAddTest()");
    return "test";
}
@RequestMapping(value = "/del")
public String dispatchDelTest()
{
    System.out.println("Enter TestControll.dispatchDelTest()");
    return "test";
}
```
那么这三个方法分别匹配的路径是：
```
"localhost:8080/SpringMVC/add"
"localhost:8080/SpringMVC/add/add"
"localhost:8080/SpringMVC/del"
```
关于路径匹配，再提一点，假如在类上和方法上都加了RequestMapping，那么将会以类路径为基准，再向方法路径做匹配，比如：
```
@Controller
@RequestMapping(value = "/test/")
public class TestController
{
    @RequestMapping(value = "common")
    public String root()
    {
        System.out.println("Enter TestController.root()!");
        return "result";
    }
}
```
这种写法，匹配的应当是：
```
"localhost:8080/SpringMVC/test/common"
"localhost:8080/SpringMVC/test/common/"
"localhost:8080/SpringMVC/test/common.html"
"localhost:8080/SpringMVC/test/common.jsp"
"localhost:8080/SpringMVC/test/common.vm"
```
类似这种的路径，如果还想往"localhost:8080/SpringMVC/test/common/"再添加内容，那么root()这个方法就无法匹配到了，必须再添加方法。多说一句，"/"一直是一个容易弄混的东西，我自己试验的时候发现，RequestMapping里面的value属性，只要路径不存在多级的关系，加不加"/"是没有什么影响的。
另外，@RequestMapping还可以匹配请求类型，到底是GET还是POST还是其他的，这么做：
```
@RequestMapping(method = RequestMethod.POST)
public String dispatchTest()
{
    System.out.println("Enter TestController.dispatchTest()");
    return "test";
}
```
这样就指定了该方法只匹配"localhost:8080/SpringMVC/test"且请求方式为POST的请求。
前面页面跳转的方式都是转发（dispatch）的方式，转发在我看来未必是一种很好的方式，典型的就是处理表单的时候会有表单重复提交的问题，那么如何使用重定向（redirect）的方式进行页面跳转？可以这么写Controller的方法，差别在于return部分：
```
@RequestMapping
public String dispatchTest(Test test)
{
    System.out.println("Enter TestController.dispatchTest(), test: " + test);
    return "redirect:/test.jsp";
}
```
最后，@RequestMapping中还有params、headers、consumes等几个属性，不过这几个都不重要，也不常用，就不讲了。
参数匹配
处理url也好、表单提交也好，参数匹配是非常重要的一个内容，万幸，Spring MVC对参数请求的支持做得非常好----它会自动根据url或者表单中参数的名字和方法中同名形参进行匹配并赋值。
举一个例子：
```
@Controller
@RequestMapping(value = "/test")
public class TestController
{
    @RequestMapping
    public String dispatchTest(String testId, String ttestId)
    {
        System.out.println("Enter TestController.dispatchTest(), testId = " + testId + 
                ", ttestId = " + ttestId);
        return "test";
    }
}
```
此时，我访问"localhost:8080/SpringMVC/test?testId=1&ttestId=2"，控制台打印出：
```
Enter TestController.dispatchTest(), testId = 1, ttestId = 2
```
不仅如此，方法中还可以放入一个实体类对象：
```
public class Test
{
    private String tid;
    private String nid;
    private String bid;
    
    public void setTid(String tid)
    {
        this.tid = tid;
    }
    
    public void setNid(String nid)
    {
        this.nid = nid;
    }
    
    public void setBid(String bid)
    {
        this.bid = bid;
    }
    
    public String toString()
    {
        return "tid = " + tid + ", nid = " + nid + ", bid = " + bid;
    }
}
```
注意，实体类对象中如果私有属性不打算对外提供，getter可以没有，但是为了Spring MVC可以将对应的属性根据属性名称进行匹配并赋值，setter必须要有。把TestController稍作改造，传入一个对象：
```
@RequestMapping
public String dispatchTest(Test test)
{
    System.out.println("Enter TestController.dispatchTest(), test: " + test);
    return "test";
}
```
此时我访问"http://localhost:8080/SpringMVC/test?tid=0&bid=1&nid=2"，控制台上打印出：
```
Enter TestController.dispatchTest(), test: tid = 0, nid = 2, bid = 1
```
看到，参数完全匹配。
不仅如此，再多试验一点：
```
@RequestMapping
public String dispatchTest(Test test1, Test test2, String tid, String nid)
{
    System.out.println("Enter TestController.dispatchTest(), test1：" + test1 + 
            "; test2：" + test2 + "; tid：" + tid + "; nid：" + nid);
    return "test";
}
```
访问一样地址"http://localhost:8080/SpringMVC/test?tid=0&bid=1&nid=2"，结果是：
```
Enter TestController.dispatchTest(), test1：tid = 0, nid = 2, bid = 1; test2：tid = 0, nid = 2, bid = 1; tid：0; nid：2
```
结论就是：
- 假如方法的参数是普通的字符串，只要字符串名字有和请求参数中的key完全匹配的，Spring
 MVC就会将完全匹配的自动赋值
- 假如方法的参数是实体类，只要实体类中的参数有和请求参数中的key完全匹配的，Spring MVC就会将完全匹配的自动赋值
对于url如此，应用到表单中也是一样的，有兴趣的可以自己试验一下。
