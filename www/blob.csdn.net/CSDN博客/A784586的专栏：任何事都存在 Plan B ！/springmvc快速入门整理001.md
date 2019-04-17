# springmvc快速入门整理001 - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年03月12日 17:28:21[QuJack](https://me.csdn.net/A784586)阅读数：327








**springmvc快速入门整理001**

**1.web mvc:**

**1.1 web开发中需要了解的****请求--响应模型**；（新技术HTML5
 websocket和服务器推（Comet）可以实现服务器通知客户端）；




**1.2 MVC：**

**![](https://img-blog.csdn.net/20170312172613391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)**



**Model（模型）：**数据模型，提供要展示的数据，因此包含数据和行为，可以认为是领域模型或JavaBean组件（包含数据和行为），不过现在一般都分离开来：Value Object（数据）
 和 服务层（行为）。也就是模型提供了模型数据查询和模型数据的状态更新等功能，包括数据和业务。

**View（视图）：**负责进行模型的展示，一般就是我们见到的用户界面，客户想看到的东西。

**Controller（控制器）：**接收用户请求，委托给模型进行处理（状态改变），处理完毕后把返回的模型数据返回给视图，由视图负责展示。 也就是说控制器做了个调度员的工作。




在标准的MVC中模型能主动推数据给视图进行更新（观察者设计模式，在模型上注册视图，当模型更新时自动更新视图），但在Web开发中模型是无法主动推给视图（无法主动更新用户界面），因为在Web开发是请求-响应模型。Web
 MVC :少了推模型那一条线。




**1.3 Web 端开发发展历史：**

**CGI-----> Servlet -----> JSP -----> Model1 / Model2(Web MVC ) -----> Front Controller+Page Controller;**




**CGI**：（Common Gateway Interface）公共网关接口，一种在web服务端使用的脚本技术，使用C或Perl语言编写，用于接收web用户请求并处理，最后动态产生响应给用户，但每次请求将产生一个进程，重量级。

**Servlet**：一种JavaEE web组件技术，是一种在服务器端执行的web组件，用于接收web用户请求并处理，最后动态产生响应给用户。但每次请求只产生一个线程（而且有线程池），轻量级。而且能利用许多JavaEE技术（如JDBC等）。本质就是在java代码里面
 输出 html流。但表现逻辑、控制逻辑、业务逻辑调用混杂。这种做法是绝对不可取的，控制逻辑、表现代码、业务逻辑对象调用混杂在一起，最大的问题是直接在Java代码里面输出Html，这样前端开发人员无法进行页面风格等的设计与修改，即使修改也是很麻烦，因此实际项目这种做法不可取。

**JSP：**（Java Server Page）：一种在服务器端执行的web组件，是一种运行在标准的HTML页面中嵌入脚本语言（现在只支持Java）的模板页面技术。本质就是在html代码中嵌入java代码。JSP最终还是会被编译为Servlet，只不过比纯Servlet开发页面更简单、方便。但表现逻辑、控制逻辑、业务逻辑调用还是混杂。

**Model1：**可以认为是JSP的增强版，可以认为是jsp+javabean

特点：使用<jsp:useBean>标准动作，自动将请求参数封装为JavaBean组件；还必须使用java脚本执行控制逻辑。在model1中JSP负责控制逻辑、表现逻辑、业务对象（javabean）的调用，只是比纯JSP简化了获取请求参数和封装请求参数。

**Model2**：架构其实可以认为就是我们所说的Web MVC模型，只是控制器采用Servlet、模型采用JavaBean、视图采用JSP。视图和模型分离了，控制逻辑和展示逻辑分离。

**服务到工作者：Front Controller + Application Controller + Page Controller + Context**

即，前端控制器+应用控制器+页面控制器（也有称其为动作）+上下文，也是Web MVC，只是责任更加明确。







------------------------------------------------------------------------------------------------------------------------------------------------------------------

**html与jsp区别:**

HTML（Hypertext Markup Language）文本标记语言，它是静态页面，和JavaScript一样解释性语言，为什么说是解释性 语言呢？因为，只要你有一个浏览器那么它就可以正常显示出来，而不需要指定的编译工具，只需在TXT文档中写上HTML标记就OK。

JSP（Java Server Page）看这个意思就知道是Java服务端的页面，所以它是动态的，它是需要经过JDK编译后把内容发给客户端去显 示，我们都知道，Java文件编译后会产生一个class文件，最终执行的就是这个class文件，JSP也一样，它也要编译成class文件！JSP不止要编译，它还得要转译，首先把JSP转译成一个Servlet文件，然后在编译成class文件。当用户访问JSP时就执行了class文件，最
 终......

1.最简单的区别就是，HTML能直接打开，jsp只能发布到Tomact等服务器上才能打开 。

  2.定义上HTML页面是静态页面可以直接运行，JSP页面是动态页它运行时需要转换成servlet。 

 3.他们的表头不同，这个是JSP的头“ <%@ page language="java" import="java.util.*" pageEncoding="gbk"%>”在表头中有编码格式和倒入包等。

 4.也是很好区分的在jsp中用<%%>就可以写Java代码了，而html没有<%%>

------------------------------------------------------------------------------------------------------------------------------------------------------------------




-----------------------------------------------------------------------------------------------------------------------------------------------

3.springmvc

意图：想替代struts2.

Struts1封装了很多参数；

Struts2不是Struts1的简单升级；解决了Struts单例多例问题；

Stuts2类级别，例如：model,request,response，类中所有的方法都共享

Struts2使用model对象来从页面封装参数到action；从action封装数据到页面；（双向）

SpringMVC方法级别，例如：model（MAP），只从action封装数据到页面（单向）




2.工作原理




基于Filter：



![](https://img-blog.csdn.net/20170312172654065?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


基于servlet：



![](https://img-blog.csdn.net/20170312172711767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


前端核心处理器：DispatcherServlet

处理器映射器：handlerMapping

控制器：Contronller

模型和视图对象：ModelAndView

视图解析器：ViewResolver

视图层：View(jsp)




常用的处理器映射器（handlerMapping）：

<bean class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping"/>

<bean class="org.springframework.web.servlet.handler.SimpleUrlHandlerMapping">
支持多个url，访问同一个Controller，按id属性访问；

<bean class="org.springframework.web.servlet.mvc.support.ControllerClassNameHandlerMapping"/> 通过类名访问控制器；

常见控制器（控制器：Contronller）：

 CommandController命令控制器 public class MyCommandController extends
AbstractCommandController {。。。。。。} 
命令控制器，用来封装页面的对象

表单控制器 public class MyFormController extends
SimpleFormController{ }

向导表单控制器 public class MyWizardController extends
AbstractWizardFormController { }




注解开发：

配置web.xml DispatcherServlet,


配置 ：springmvc-servlet.xml

 <!--
对所有的Controller进行包扫描
-->

    <context:component-scan base-package="cn.itcast.controller"/>



    <!--
内部资源视图解析器 -->

    <bean id="jspViewResolver" class="org.springframework.web.servlet.view.InternalResourceViewResolver">

     <property name="prefix" value="/WEB-INF/pages/"/>

     <property name="suffix" value=".jsp"/>

    </bean>




-----------------------------------------------------------------------------------------------------------------------------------------

真实流程：

![](https://img-blog.csdn.net/20170312172731064?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




核心架构的**具体流程**步骤如下：

1、  首先用户发送请求——>DispatcherServlet，前端控制器收到请求后自己不进行处理，而是委托给其他的解析器进行处理，作为统一访问点，进行全局的流程控制；

2、  DispatcherServlet——>HandlerMapping， HandlerMapping处理器映射器将会把请求映射为HandlerExecutionChain对象（包含一个Handler处理器（页面控制器）对象、多个HandlerInterceptor拦截器）对象，通过这种策略模式，很容易添加新的映射策略；

3、  DispatcherServlet——>HandlerAdapter，HandlerAdapter将会把处理器包装为适配器，从而支持多种类型的处理器，即适配器设计模式的应用，从而很容易支持很多类型的处理器；

4、  HandlerAdapter——>处理器功能处理方法的调用，HandlerAdapter将会根据适配的结果调用真正的处理器的功能处理方法，完成功能处理；并返回一个ModelAndView对象（包含模型数据、逻辑视图名）；

5、  ModelAndView的逻辑视图名--> ViewResolver， ViewResolver视图解析器将把逻辑视图名解析为具体的View，这种策略模式，很容易更换其他视图技术；

6、  View——>渲染，View会根据传进来的Model模型数据进行渲染，此处的Model实际是一个Map数据结构，因此很容易支持其他视图技术；

7、返回控制权给DispatcherServlet，由DispatcherServlet返回响应给用户，到此一个流程结束。



此处我们只是讲了核心流程，没有考虑拦截器、本地解析、文件上传解析等。

1、  请求如何给前端控制器？这个应该在web.xml中进行部署描述，在HelloWorld中详细讲解。

2、  前端控制器如何根据请求信息选择页面控制器进行功能处理？ 我们需要配置HandlerMapping进行映射

3、  如何支持多种页面控制器呢？配置HandlerAdapter从而支持多种类型的页面控制器

4、  如何页面控制器如何使用业务对象？可以预料到，肯定利用Spring IoC容器的依赖注入功能

5、  页面控制器如何返回模型数据？使用ModelAndView返回

6、  前端控制器如何根据页面控制器返回的逻辑视图名选择具体的视图进行渲染？ 使用ViewResolver进行解析

7、  不同的视图技术如何使用相应的模型数据？ 因为Model是一个Map数据结构，很容易支持其他视图技术



在此我们可以看出具体的**核心开发步骤**：

1、  DispatcherServlet在web.xml中的部署描述，从而拦截请求到Spring Web MVC

2、  HandlerMapping的配置，从而将请求映射到处理器

3、  HandlerAdapter的配置，从而支持多种类型的处理器

4、  ViewResolver的配置，从而将逻辑视图名解析为具体视图技术

5、处理器（页面控制器）的配置，从而进行功能处理

由此可以看出主要开发工作任务：控制器Controller+视图层JSP




**Spring Web MVC优势**

1、清晰的角色划分：前端控制器（DispatcherServlet）、请求到处理器映射（HandlerMapping）、处理器适配器（HandlerAdapter）、视图解析器（ViewResolver）、处理器或页面控制器（Controller）、验证器（  
 Validator）、命令对象（Command  请求参数绑定到的对象就叫命令对象）、表单对象（Form
 Object 提供给表单展示和提交到的对象就叫表单对象）。

2、分工明确，而且扩展点相当灵活，可以很容易扩展，虽然几乎不需要；

3、由于命令对象就是一个POJO，无需继承框架特定API，可以使用命令对象直接作为业务对象；

4、和Spring 其他框架无缝集成，是其它Web框架所不具备的；

5、可适配，通过HandlerAdapter可以支持任意的类作为处理器；

6、可定制性，HandlerMapping、ViewResolver等能够非常简单的定制；

7、功能强大的数据验证、格式化、绑定机制；

8、利用Spring提供的Mock对象能够非常简单的进行Web层单元测试；

9、本地化、主题的解析的支持，使我们更容易进行国际化和主题的切换。

10、强大的JSP标签库，使JSP编写更容易。

………………还有比如RESTful风格的支持、简单的文件上传、约定大于配置的契约式编程支持、基于注解的零配置支持等等。

---------------------------------------------------------------------------------------------------------------

 开发springMVC程序：导包到中

1.在web.xml配置DispatcherServlet前端控制器，需要拦截哪些请求；
- <servlet>  
-     <servlet-name>**myservlet**</servlet-name>  
-     <servlet-**class**>org.springframework.web.servlet.DispatcherServlet</servlet-**class**>  
-     <load-on-startup>1</load-on-startup>  
- </servlet>  
- <servlet-mapping>  
-     <servlet-name>**myservlet**</servlet-name>  
-     <url-pattern>/</url-pattern>  
- </servlet-mapping> 




2.
在 WEB-INF/ myservlet-servlet.xml 配置HandlerMapping处理器映射器、HandlerAdapter处理器适配器；

<!-- HandlerMapping -->

<bean class="org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping"/>



<!-- HandlerAdapter -->

<bean class="org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter"/>




3.配置ViewResolver

<!-- ViewResolver -->

<bean class="org.springframework.web.servlet.view.InternalResourceViewResolver">

<property name="viewClass" value="org.springframework.web.servlet.view.JstlView"/>

<property name="prefix" value="/WEB-INF/jsp/"/>

<property name="suffix" value=".jsp"/>

</bean>




4.开发控制器 【开发重点】

org.springframework.web.servlet.mvc.Controller：页面控制器/处理器必须实现Controller接口；

public ModelAndView
handleRequest(HttpServletRequest req, HttpServletResponse resp) ：功能处理方法，实现相应的功能处理，比如收集参数、验证参数、绑定参数到命令对象、将命令对象传入业务对象进行业务处理、最后返回ModelAndView对象；

ModelAndView：包含了视图要实现的模型数据和逻辑视图名；“mv.addObject("message",
 "Hello World!");”表示添加模型数据，此处可以是任意POJO对象；“mv.setViewName("hello");”表示设置逻辑视图名为“hello”，视图解析器会将其解析为具体的视图，如前边的视图解析器InternalResourceVi。wResolver会将其解析为“WEB-INF/jsp/hello.jsp”。

将开发的控制器配置到文件中，接受IOC管理。

<!-- 处理器 -->  

<bean name="/hello"**class**="cn.javass.chapter2.web.controller.HelloWorldController"/>  




5.开发视图页面 (JSP)【开发重点】




中文乱码：

<filter>  
-     <filter-name>CharacterEncodingFilter</filter-name>  
-     <filter-**class**>org.springframework.web.filter.CharacterEncodingFilter</filter-**class**>  
-     <init-param>  
-         <param-name>encoding</param-name>  
-         <param-value>utf-8</param-value>  
-     </init-param>  
- </filter>  
- <filter-mapping>  
-     <filter-name>CharacterEncodingFilter</filter-name>  
-     <url-pattern>/*</url-pattern>  
- </filter-mapping>  

**-------------------------------------------------------------**

**附：**

**Spring 新特性**

**一、Spring2.5之前，我们都是通过实现Controller接口或其实现来定义我们的处理器类。**



**二、Spring2.5引入注解式处理器支持，通过@Controller 和 @RequestMapping注解定义我们的处理器类。并且提供了一组强大的注解：**



需要通过处理器映射DefaultAnnotationHandlerMapping和处理器适配器AnnotationMethodHandlerAdapter来开启支持@Controller 和 @RequestMapping注解的处理器。



@Controller：用于标识是处理器类；

@RequestMapping：请求到处理器功能方法的映射规则；

@RequestParam：请求参数到处理器功能处理方法的方法参数上的绑定；

@ModelAttribute：请求参数到命令对象的绑定；

@SessionAttributes：用于声明session级别存储的属性，放置在处理器类上，通常列出模型属性（如@ModelAttribute）对应的名称，则这些属性会透明的保存到session中；

@InitBinder：自定义数据绑定注册支持，用于将请求参数转换到命令对象属性的对应类型；



**三、Spring3.0引入RESTful架构风格支持(通过@PathVariable注解和一些其他特性支持),且又引入了更多的注解支持：**

@CookieValue：cookie数据到处理器功能处理方法的方法参数上的绑定；

@RequestHeader：请求头（header）数据到处理器功能处理方法的方法参数上的绑定；

@RequestBody：请求的body体的绑定（通过HttpMessageConverter进行类型转换）；

@ResponseBody：处理器功能处理方法的返回值作为响应体（通过HttpMessageConverter进行类型转换）；

@ResponseStatus：定义处理器功能处理方法/异常处理器返回的状态码和原因；

@ExceptionHandler：注解式声明异常处理器；

@PathVariable：请求URI中的模板变量部分到处理器功能处理方法的方法参数上的绑定，从而支持RESTful架构风格的URI；



**四、还有比如：**

JSR-303验证框架的无缝支持（通过@Valid注解定义验证元数据）；

使用Spring 3开始的ConversionService进行类型转换（PropertyEditor依然有效），支持使用@NumberFormat
 和 @DateTimeFormat来进行数字和日期的格式化；

HttpMessageConverter（Http输入/输出转换器，比如JSON、XML等的数据输出转换器）；

ContentNegotiatingViewResolver，内容协商视图解析器，它还是视图解析器，只是它支持根据请求信息将同一模型数据以不同的视图方式展示（如json、xml、html等），RESTful架构风格中很重要的概念（同一资源，多种表现形式）；

Spring 3 引入 一个  mvc XML的命名空间用于支持mvc配置，包括如：

    <mvc:annotation-driven>：

      自动注册基于注解风格的处理器需要的DefaultAnnotationHandlerMapping、AnnotationMethodHandlerAdapter

      支持Spring3的ConversionService自动注册

      支持JSR-303验证框架的自动探测并注册（只需把JSR-303实现放置到classpath）

      自动注册相应的HttpMessageConverter（用于支持@RequestBody  和 @ResponseBody）（如XML输入输出转换器（只需将JAXP实现放置到classpath）、JSON输入输出转换器（只需将Jackson实现放置到classpath））等。

    <mvc:interceptors>：注册自定义的处理器拦截器；

    <mvc:view-controller>：和ParameterizableViewController类似，收到相应请求后直接选择相应的视图；

<mvc:resources>：逻辑静态资源路径到物理静态资源路径的支持；

<mvc:default-servlet-handler>：当在web.xml 中DispatcherServlet使用<url-pattern>/</url-pattern> 映射时，能映射静态资源（当Spring
 Web MVC框架没有处理请求对应的控制器时（如一些静态资源），转交给默认的Servlet来响应静态文件，否则报404找不到资源错误，）。



……等等。



**五、Spring3.1新特性：**

对Servlet 3.0的全面支持。



@EnableWebMvc：用于在基于Java类定义Bean配置中开启MVC支持，和XML中的<mvc:annotation-driven>功能一样；



新的@Contoller和@RequestMapping注解支持类：处理器映射RequestMappingHandlerMapping 和 处理器适配器RequestMappingHandlerAdapter组合来代替Spring2.5开始的处理器映射DefaultAnnotationHandlerMapping和处理器适配器AnnotationMethodHandlerAdapter，提供更多的扩展点，它们之间的区别我们在处理器映射一章介绍。



新的@ExceptionHandler 注解支持类：ExceptionHandlerExceptionResolver来代替Spring3.0的AnnotationMethodHandlerExceptionResolver，在异常处理器一章我们再详细讲解它们的区别。



@RequestMapping的"consumes" 和 "produces" 条件支持：用于支持@RequestBody 和 @ResponseBody，

1consumes指定请求的内容是什么类型的内容，即本处理方法消费什么类型的数据，如consumes="application/json"表示JSON类型的内容，Spring会根据相应的HttpMessageConverter进行请求内容区数据到@RequestBody注解的命令对象的转换；

2produces指定生产什么类型的内容，如produces="application/json"表示JSON类型的内容，Spring的根据相应的HttpMessageConverter进行请求内容区数据到@RequestBody注解的命令对象的转换，Spring会根据相应的HttpMessageConverter进行模型数据（返回值）到JSON响应内容的转换

3以上内容，本章第×××节详述。



URI模板变量增强：URI模板变量可以直接绑定到@ModelAttribute指定的命令对象、@PathVariable方法参数在视图渲染之前被合并到模型数据中（除JSON序列化、XML混搭场景下）。



@Validated：JSR-303的javax.validation.Valid一种变体（非JSR-303规范定义的，而是Spring自定义的），用于提供对Spring的验证器（org.springframework.validation.Validator）支持，需要Hibernate Validator
 4.2及更高版本支持；



@RequestPart：提供对“multipart/form-data”请求的全面支持，支持Servlet 3.0文件上传（javax.servlet.http.Part）、支持内容的HttpMessageConverter（即根据请求头的Content-Type，来判断内容区数据是什么类型，如JSON、XML，能自动转换为命令对象），比@RequestParam更强大（只能对请求参数数据绑定，key-alue格式），而@RequestPart支持如JSON、XML内容区数据的绑定；详见本章的第×××节；



Flash 属性 和 RedirectAttribute：通过FlashMap存储一个请求的输出，当进入另一个请求时作为该请求的输入，典型场景如重定向（POST-REDIRECT-GET模式，1、POST时将下一次需要的数据放在FlashMap；2、重定向；3、通过GET访问重定向的地址，此时FlashMap会把1放到FlashMap的数据取出放到请求中，并从FlashMap中删除；从而支持在两次请求之间保存数据并防止了重复表单提交）。

Spring Web MVC提供FlashMapManager用于管理FlashMap，默认使用SessionFlashMapManager，即数据默认存储在session中。




可以参考：[http://jinnianshilongnian.iteye.com/blog/1752171](http://jinnianshilongnian.iteye.com/blog/1752171)

--------------------------------------------------------------------------------------------------------------------------------------------------------------------

































