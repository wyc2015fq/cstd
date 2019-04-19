# SpringMVC深度探险-DispatcherServlet与初始化主线 - 零度的博客专栏 - CSDN博客
2017年02月20日 11:37:22[零度anngle](https://me.csdn.net/zmx729618)阅读数：840
在上一篇文章中，我们给出了构成SpringMVC应用程序的三要素以及三要素的设计过程。让我们来归纳一下整个设计过程中的一些要点： 
- **SpringMVC将Http处理流程抽象为一个又一个处理单元**
- **SpringMVC定义了一系列组件（接口）与所有的处理单元对应起来**
- **SpringMVC由DispatcherServlet贯穿始终，并将所有的组件串联起来**
在整个过程中，组件和DispatcherServlet总是维持着一个相互支撑的关系： 
- **DispatcherServlet —— 串联起整个逻辑主线，是整个框架的心脏**
- **组件 —— 逻辑处理单元的程序化表示，起到承上启下的作用，是SpringMVC行为模式的实际承载者**
在本系列接下来的两篇文章中，我们将分别讨论DispatcherServlet和组件的相关内容。本文讨论DispatcherServlet，而下一篇则重点分析组件。 
有关DispatcherServlet，我们想从构成DispatcherServlet的体系结构入手，再根据不同的逻辑主线分别加以分析，希望能够帮助读者整理出学习SpringMVC核心类的思路。 
**DispatcherServlet的体系结构**
通过不同的角度来观察DispatcherServlet会得到不同的结论。我们在这里选取了三个不同的角度：运行特性、继承结构和数据结构。 
**【运行主线】**
从DispatcherServlet所实现的接口来看，DispatcherServlet的核心本质：是一个Servlet。这个结论似乎很幼稚，不过这个幼稚的结论却蕴含了一个对整个框架都至关重要的内在原则：Servlet可以根据其特性进行运行主线的划分。 
根据Servlet规范的定义，Servlet中的两大核心方法init方法和service方法，它们的运行时间和触发条件都截然不同： 
1. init方法
**在整个系统启动时运行，且只运行一次。**因此，在init方法中我们往往会对整个应用程序进行初始化操作。这些初始化操作可能包括对容器（WebApplicationContext）的初始化、组件和外部资源的初始化等等。
2. service方法
**在整个系统运行的过程中处于侦听模式，侦听并处理所有的Web请求。**因此，在service及其相关方法中，我们看到的则是对Http请求的处理流程。
因而在这里，Servlet的这一特性就被SpringMVC用于对不同的逻辑职责加以划分，从而形成两条互不相关的逻辑运行主线： 
- **初始化主线 —— 负责对SpringMVC的运行要素进行初始化**
- **Http请求处理主线 —— 负责对SpringMVC中的组件进行逻辑调度完成对Http请求的处理**
对于一个MVC框架而言，运行主线的划分非常重要。因为只有弄清楚不同的运行主线，我们才能针对不同的运行主线采取不同的研究策略。而我们在这个系列中的绝大多数分析的切入点，也是围绕着不同的运行主线进行的。 
**注**：SpringMVC运行主线的划分依据是Servlet对象中不同方法的生命周期。事实上，几乎所有的MVC都是以此为依据来进行运行主线的划分。这进一步可以证明所有的MVC框架的核心基础还是Servlet规范，而设计理念的差异也导致了不同的框架走向了完全不同的发展道路。
**【继承结构】**
除了运行主线的划分以外，我们再关注一下DispatcherServlet的继承结构： 
![](http://dl.iteye.com/upload/attachment/0062/6368/a5ae8920-b395-322b-a023-3278418992e0.png)
在这个继承结构中，我们可以看到DispatcherServlet在其继承树中包含了2个Spring的支持类：HttpServletBean和FrameworkServlet。我们分别来讨论一下这两个Spring的支持类在这里所起到的作用。 
HttpServletBean是Spring对于Servlet最低层次的抽象。在这一层抽象中，Spring会将这个Servlet视作是一个Spring的bean，并将init-param中的值作为bean的属性注入进来： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- publicfinalvoid init() throws ServletException {  
- if (logger.isDebugEnabled()) {  
-         logger.debug("Initializing servlet '" + getServletName() + "'");  
-     }  
- 
- // Set bean properties from init parameters.
- try {  
-         PropertyValues pvs = new ServletConfigPropertyValues(getServletConfig(), this.requiredProperties);  
-         BeanWrapper bw = PropertyAccessorFactory.forBeanPropertyAccess(this);  
-         ResourceLoader resourceLoader = new ServletContextResourceLoader(getServletContext());  
-         bw.registerCustomEditor(Resource.class, new ResourceEditor(resourceLoader, this.environment));  
-         initBeanWrapper(bw);  
-         bw.setPropertyValues(pvs, true);  
-     }  
- catch (BeansException ex) {  
-         logger.error("Failed to set bean properties on servlet '" + getServletName() + "'", ex);  
- throw ex;  
-     }  
- 
- // Let subclasses do whatever initialization they like.
-     initServletBean();  
- 
- if (logger.isDebugEnabled()) {  
-         logger.debug("Servlet '" + getServletName() + "' configured successfully");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
从源码中，我们可以看到HttpServletBean利用了Servlet的init方法的执行特性，将一个普通的Servlet与Spring的容器联系在了一起。在这其中起到核心作用的代码是：BeanWrapper bw = PropertyAccessorFactory.forBeanPropertyAccess(this);将当前的这个Servlet类转化为一个BeanWrapper，从而能够以Spring的方式来对init-param的值进行注入。BeanWrapper的相关知识属于Spring
 Framework的内容，我们在这里不做详细展开，读者可以具体参考HttpServletBean的注释获得更多的信息。 
FrameworkServlet则是在HttpServletBean的基础之上的进一步抽象。通过FrameworkServlet真正初始化了一个Spring的容器（WebApplicationContext），并引入到Servlet对象之中： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- protectedfinalvoid initServletBean() throws ServletException {  
-     getServletContext().log("Initializing Spring FrameworkServlet '" + getServletName() + "'");  
- if (this.logger.isInfoEnabled()) {  
- this.logger.info("FrameworkServlet '" + getServletName() + "': initialization started");  
-     }  
- long startTime = System.currentTimeMillis();  
- 
- try {  
- this.webApplicationContext = initWebApplicationContext();  
-         initFrameworkServlet();  
-     } catch (ServletException ex) {  
- this.logger.error("Context initialization failed", ex);  
- throw ex;  
-     } catch (RuntimeException ex) {  
- this.logger.error("Context initialization failed", ex);  
- throw ex;  
-     }  
- 
- if (this.logger.isInfoEnabled()) {  
- long elapsedTime = System.currentTimeMillis() - startTime;  
- this.logger.info("FrameworkServlet '" + getServletName() + "': initialization completed in " +  
-                 elapsedTime + " ms");  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
上面的这段代码就是FrameworkServlet初始化的核心代码。从中我们可以看到这个FrameworkServlet将调用其内部的方法initWebApplicationContext()对Spring的容器（WebApplicationContext）进行初始化。同时，FrameworkServlet还暴露了与之通讯的结构可供子类调用： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- publicabstractclass FrameworkServlet extends HttpServletBean {  
- 
- /** WebApplicationContext for this servlet */
- private WebApplicationContext webApplicationContext;  
- 
- // 这里省略了其他所有的代码
- 
- /**
-      * Return this servlet's WebApplicationContext.
-      */
- publicfinal WebApplicationContext getWebApplicationContext() {  
- returnthis.webApplicationContext;  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
我们在这里暂且不对Spring容器（WebApplicationContext）的初始化过程详加探查，稍后我们会讨论一些WebApplicationContext初始化过程中的配置选项。不过读者可以在这里体会到：FrameworkServlet在其内部初始化了一个Spring的容器（WebApplicationContext）并暴露了相关的操作接口，因而继承自FrameworkServlet的DispatcherServlet，也就直接拥有了与WebApplicationContext进行通信的能力。
通过对DispatcherServlet继承结构的研究，我们可以明确： 
downpour 写道
**结论** DispatcherServlet的继承体系架起了DispatcherServlet与Spring容器进行沟通的桥梁。
**【数据结构】**
在上一篇文章中，我们曾经提到过DispatcherServlet的数据结构： 
![](http://dl.iteye.com/upload/attachment/0062/6933/6ea52d3d-fe87-3579-976e-d6edd1f0deb3.png)
我们可以把在上面这张图中所构成DispatcherServlet的数据结构主要分为两类（我们在这里用一根分割线将其分割开来）： 
- **配置参数 —— 控制SpringMVC组件的初始化行为方式**
- **核心组件 —— SpringMVC的核心逻辑处理组件**
可以看到，这两类数据结构都与SpringMVC中的核心要素组件有关。因此，我们可以得出这样一个结论： 
downpour 写道
**结论** 组件是整个DispatcherServlet的灵魂所在：它不仅是初始化主线中的初始化对象，同样也是Http请求处理主线中的逻辑调度载体。
**注**：我们可以看到被我们划为配置参数的那些变量都是boolean类型的，它们将在DispatcherServlet的初始化主线中起到一定的作用，我们在之后会使用源码进行说明。而这些boolean值可以通过web.xml中的init-param值进行设定覆盖（这是由HttpServletBean的特性带来的）。
**SpringMVC的运行体系**
DispatcherServlet继承结构和数据结构，实际上表述的是DispatcherServlet与另外两大要素之间的关系： 
- **继承结构 —— DispatcherServlet与Spring容器（WebApplicationContext）之间的关系**
- **数据结构 —— DispatcherServlet与组件之间的关系**
所以，其实我们可以这么说：SpringMVC的整个运行体系，是由DispatcherServlet、组件和容器这三者共同构成的。
在这个运行体系中，DispatcherServlet是逻辑处理的调度中心，组件则是被调度的操作对象。而容器在这里所起到的作用，是协助DispatcherServlet更好地对组件进行管理。这就相当于一个工厂招了一大批的工人，并把工人划分到一个统一的工作车间而便于管理。在工厂要进行生产活动时，只需要从工作车间把工人分派到相应的生产流水线上即可。 
笔者在这里引用Spring官方reference中的一幅图，对三者之间的关系进行简单的描述： 
![](http://dl.iteye.com/upload/attachment/0062/6980/17a6f37b-340c-31a7-9352-944f87bb6a01.png)
**注**：在这幅图中，我们除了看到在图的左半边DispatcherServlet、组件和容器这三者之间的调用关系以外，还可以看到SpringMVC的运行体系与其它运行体系之间存在着关系。有关这一点，我们在之后的讨论中会详细展开。
既然是三个元素之间的关系表述，我们必须以两两关系的形式进行归纳： 
- **DispatcherServlet - 容器 —— DispatcherServlet对容器进行初始化**
- **容器 - 组件 —— 容器对组件进行全局管理**
- **DispatcherServlet - 组件 —— DispatcherServlet对组件进行逻辑调用**
值得注意的是，在上面这幅图中，三大元素之间的两两关系其实表现得并不明显，尤其是“容器 - 组件”和“DispatcherServlet - 组件”之间的关系。这主要是由于Spring官方reference所给出的这幅图是一个静态的关系表述，如果从动态的观点来对整个过程加以审视，我们就不得不将SpringMVC的运行体系与之前所提到的运行主线联系在一起，看看这些元素在不同的逻辑主线中所起到的作用。 
接下来，我们就分别看看DispatcherServlet的两条运行主线。 
**DispatcherServlet的初始化主线**
对于DispatcherServlet的初始化主线，我们首先应该明确几个基本观点： 
- **初始化主线的驱动要素 —— servlet中的init方法**
- **初始化主线的执行次序 —— HttpServletBean -> FrameworkServlet -> DispatcherServlet**
- **初始化主线的操作对象 —— Spring容器（WebApplicationContext）和组件**
这三个基本观点，可以说是我们对之前所有讨论的一个小结。明确了这些内容，我们就可以更加深入地看看DispatcherServlet初始化主线的过程： 
![](http://dl.iteye.com/upload/attachment/0062/9586/61b32fbb-1c8f-35ae-91cd-05dfd027b123.png)
在这幅图中，我们站在一个动态的角度将DispatcherServlet、容器（WebApplicationContext）和组件这三者之间的关系表述出来，同时给出了这三者之间的运行顺序和逻辑过程。读者或许对其中的绝大多数细节还很陌生，甚至有一种无从下手的感觉。这没有关系，大家可以首先抓住图中的执行线，回忆一下之前有关DispatcherServlet的继承结构和数据结构的内容。接下来，我们就图中的内容逐一进行解释。 
**【WebApplicationContext的初始化】**
之前我们讨论了DispatcherServlet对于WebApplicationContext的初始化是在FrameworkServlet中完成的，不过我们并没有细究其中的细节。在默认情况下，这个初始化过程是由web.xml中的入口程序配置所驱动的： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <!-- Processes application requests -->
- <servlet>
- <servlet-name>dispatcher</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>dispatcher</servlet-name>
- <url-pattern>/**</url-pattern>
- </servlet-mapping>
![](http://static.blog.csdn.net/images/save_snippets.png)
我们已经不止一次提到过这段配置，不过在这之前都没有对这段配置做过什么很详细的分析。事实上，这段入口程序的配置中隐藏了SpringMVC的两大要素（核心分发器Dispatcher和核心配置文件[servlet-name]-servlet.xml）之间的关系表述： 
downpour 写道
在默认情况下，web.xml配置节点中<servlet-name>的值就是建立起核心分发器DispatcherServlet与核心配置文件之间联系的桥梁。DispatcherServlet在初始化时会加载位置在/WEB-INF/[servlet-name]-servlet.xml的配置文件作为SpringMVC的核心配置。
SpringMVC在这里采用了一个“命名约定”的方法进行关系映射，这种方法很廉价也很管用。以上面的配置为例，我们就必须在/WEB-INF/目录下，放一个名为dispatcher-servlet.xml的Spring配置文件作为SpringMVC的核心配置用以指定SpringMVC的基本组件声明定义。 
这看上去似乎有一点别扭，因为在实际项目中，我们通常喜欢把配置文件放在classpath下，并使用不同的package进行区分。例如，在基于Maven的项目结构中，所有的配置文件应置于src/main/resources目录下，这样才比较符合配置文件统一化管理的最佳实践。 
于是，Spring提供了一个初始化的配置选项，通过指定contextConfigLocation选项来自定义SpringMVC核心配置文件的位置： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <!-- Processes application requests -->
- <servlet>
- <servlet-name>dispatcher</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:web/applicationContext-dispatcherServlet.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>dispatcher</servlet-name>
- <url-pattern>/</url-pattern>
- </servlet-mapping>
![](http://static.blog.csdn.net/images/save_snippets.png)
这样一来，DispatcherServlet在初始化时，就会自动加载在classpath下，web这个package下名为applicationContext-dispatcherServlet.xml的文件作为其核心配置并用以初始化容器（WebApplicationContext）。 
当然，这只是DispatcherServlet在进行WebApplicationContext初始化过程中的配置选项之一。我们可以在Spring的官方reference中找到相应的配置选项，有兴趣的读者可以参照reference的说明进行尝试： 
![](http://dl.iteye.com/upload/attachment/0062/9591/2a2f0d65-b1de-3dcc-8c54-3c4e9d87f6c2.png)
所有的这些配置选项，实际上都是为了让DispatcherServlet对WebApplicationContext的初始化过程显得更加自然。不过这只是完成了容器（WebApplicationContext）的构建工作，那么容器所管理的那些组件，又是如何进行初始化的呢？ 
downpour 写道
**结论** SpringMVC核心配置文件中所有的bean定义，就是SpringMVC的组件定义，也是DispatcherServlet在初始化容器（WebApplicationContext）时，所要进行初始化的组件。
在上一篇文章我们谈到组件的时候就曾经提到，SpringMVC自身对于组件并未实现一套完整的管理机制，而是借用了Spring Framework核心框架中容器的概念，将所有的组件纳入到容器中进行管理。所以，SpringMVC的核心配置文件使用与传统Spring Framework相同的配置形式，而整个管理的体系也是一脉相承的。 
**注**：Spring3.0之后，单独为SpringMVC建立了专用的schema，从而使得我们可以使用Schema Based XML来对SpringMVC的组件进行定义。不过我们可以将其视作是传统Spring配置的一个补充，而不要过于纠结不同的配置格式。
我们知道，SpringMVC的组件是一个个的接口定义，当我们在SpringMVC的核心配置文件中定义一个组件时，使用的却是组件的实现类： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="prefix"value="/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
![](http://static.blog.csdn.net/images/save_snippets.png)
这也就是Spring管理组件的模式：用具体的实现类来指定接口的行为方式。不同的实现类，代表着不同的组件行为模式，它们在Spring容器中是可以共存的： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="prefix"value="/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
- 
- <beanclass="org.springframework.web.servlet.view.freemarker.FreeMarkerViewResolver">
- <propertyname="prefix"value="/"/>
- <propertyname="suffix"value=".ftl"/>
- </bean>
![](http://static.blog.csdn.net/images/save_snippets.png)
所以，Spring的容器就像是一个聚宝盆，它只负责承载对象，管理好对象的生命周期，而并不关心一个组件接口到底有多少种实现类或者行为模式。这也就是我们在上面那幅图中，画了多个HandlerMappings、HandlerAdapters和ViewResolvers的原因：一个组件的多种行为模式可以在容器中共存，容器将负责对这些实现类进行管理。而具体如何使用这些对象，则由应用程序自身来决定。 
如此一来，我们可以大致概括一下WebApplicationContext初始化的两个逻辑层次： 
- **DispatcherServlet负责对容器（WebApplicationContext）进行初始化。**
- **容器（WebApplicationContext）将读取SpringMVC的核心配置文件进行组件的实例化。**
整个过程，我们把应用程序的日志级别调低，可以进行非常详细的观察： 
引用
14:15:27,037 DEBUG StandardServletEnvironment:100 - Initializing new StandardServletEnvironment 
14:15:27,128 DEBUG DispatcherServlet:115 - Initializing servlet 'dispatcher' 
14:15:27,438  INFO DispatcherServlet:444 - FrameworkServlet 'dispatcher': initialization started
14:15:27,449 DEBUG DispatcherServlet:572 - Servlet with name 'dispatcher' will try to create custom WebApplicationContext context of class 'org.springframework.web.context.support.XmlWebApplicationContext', using parent context [null] 
1571 [main] INFO /sample - Initializing Spring FrameworkServlet 'dispatcher' 
14:15:27,505 DEBUG StandardServletEnvironment:100 - Initializing new StandardServletEnvironment 
14:15:27,546  INFO XmlWebApplicationContext:495 - Refreshing WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:15:27 CST 2012]; root of context hierarchy 
14:15:27,689  INFO XmlBeanDefinitionReader:315 - Loading XML bean definitions from class path resource [web/applicationContext-dispatcherServlet.xml]
14:15:27,872 DEBUG PluggableSchemaResolver:140 - Loading schema mappings from [META-INF/spring.schemas] 
14:15:28,442 DEBUG PathMatchingResourcePatternResolver:550 - Looking for matching resources in directory tree [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller] 
14:15:28,442 DEBUG PathMatchingResourcePatternResolver:612 - Searching directory [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller] for files matching pattern [D:/Work/Demo2do/Sample/target/classes/com/demo2do/sample/web/controller/**/*.class] 
14:15:28,450 DEBUG PathMatchingResourcePatternResolver:351 - Resolved location pattern [classpath*:com/demo2do/sample/web/controller/**/*.class] to resources [file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\BlogController.class],
 file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\UserController.class]] 
14:15:28,569 DEBUG ClassPathBeanDefinitionScanner:243 - Identified candidate component class: file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\BlogController.class] 
14:15:28,571 DEBUG ClassPathBeanDefinitionScanner:243 - Identified candidate component class: file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\UserController.class] 
14:15:28,634 DEBUG BeanDefinitionParserDelegate:497 - Neither XML 'id' nor 'name' specified - using generated bean name [org.springframework.web.servlet.view.InternalResourceViewResolver#0] 
14:15:28,635 DEBUG XmlBeanDefinitionReader:216 - Loaded 19 bean definitions from location pattern [classpath:web/applicationContext-dispatcherServlet.xml]
14:15:28,635 DEBUG XmlWebApplicationContext:525 - Bean factory for WebApplicationContext for namespace 'dispatcher-servlet': org.springframework.beans.factory.support.DefaultListableBeanFactory@2321b59a: defining beans [org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping#0,org.springframework.format.support.FormattingConversionServiceFactoryBean#0,org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter#0,org.springframework.web.servlet.handler.MappedInterceptor#0,org.springframework.web.servlet.mvc.method.annotation.ExceptionHandlerExceptionResolver#0,org.springframework.web.servlet.mvc.annotation.ResponseStatusExceptionResolver#0,org.springframework.web.servlet.mvc.support.DefaultHandlerExceptionResolver#0,org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping,org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter,org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter,blogController,userController,org.springframework.context.annotation.internalConfigurationAnnotationProcessor,org.springframework.context.annotation.internalAutowiredAnnotationProcessor,org.springframework.context.annotation.internalRequiredAnnotationProcessor,org.springframework.context.annotation.internalCommonAnnotationProcessor,org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0,org.springframework.web.servlet.handler.SimpleUrlHandlerMapping#0,org.springframework.web.servlet.view.InternalResourceViewResolver#0];
 root of factory hierarchy 
14:15:29,015 DEBUG RequestMappingHandlerMapping:98 - Looking for request mappings in application context: WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:15:27 CST 2012]; root of context hierarchy 
14:15:29,037  INFO RequestMappingHandlerMapping:188 - Mapped "{[/blog],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto public org.springframework.web.servlet.ModelAndView com.demo2do.station.web.controller.BlogController.index()
14:15:29,039  INFO RequestMappingHandlerMapping:188 - Mapped "{[/login],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto public org.springframework.web.servlet.ModelAndView com.demo2do.station.web.controller.UserController.login(java.lang.String,java.lang.String) 
14:15:29,040 DEBUG DefaultListableBeanFactory:458 - Finished creating instance of bean 'org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping#0' 
14:15:29,460 DEBUG BeanNameUrlHandlerMapping:71 - Looking for URL mappings in application context: WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:15:27 CST 2012]; root of context hierarchy 
14:15:29,539 DEBUG DefaultListableBeanFactory:458 - Finished creating instance of bean 'org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0' 
14:15:29,540 DEBUG DefaultListableBeanFactory:217 - Creating shared instance of singleton bean 'org.springframework.web.servlet.handler.SimpleUrlHandlerMapping#0' 
14:15:29,555  INFO SimpleUrlHandlerMapping:314 - Mapped URL path [/static/**] onto handler 'org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0' 
14:15:29,556 DEBUG DefaultListableBeanFactory:458 - Finished creating instance of bean 'org.springframework.web.servlet.handler.SimpleUrlHandlerMapping#0' 
14:15:29,827 DEBUG DispatcherServlet:523 - Published WebApplicationContext of servlet 'dispatcher' as ServletContext attribute with name [org.springframework.web.servlet.FrameworkServlet.CONTEXT.dispatcher] 
14:15:29,827  INFO DispatcherServlet:463 - FrameworkServlet 'dispatcher': initialization completed in 2389 ms 
14:15:29,827 DEBUG DispatcherServlet:136 - Servlet 'dispatcher' configured successfully
4047 [main] INFO org.mortbay.log - Started SelectChannelConnector@0.0.0.0:8080 
Jetty Server started, use 4267 ms 
在这段启动日志（笔者进行了一定删减）中，笔者刻意将WebApplicationContext的初始化的标志日志使用红色的标进行区分，而将核心配置文件的读取位置和组件定义初始化的标志日志使用蓝色标记加以区分。相信有了这段日志的帮助，读者应该可以对WebApplicationContext的初始化过程有了更加直观的认识。 
**注**：启动日志是我们研究SpringMVC的主要途径之一，之后我们还将反复使用这种方法对SpringMVC的运行过程进行研究。读者应该仔细品味每一条日志的作用，从而能够与之后的分析讲解呼应起来。
或许读者对WebApplicationContext对组件进行初始化的过程还有点困惑，大家不妨先将这个过程省略，把握住整个DispatcherServlet的大方向。我们在之后的文章中，还将对SpringMVC的组件、这些组件的定义以及组件的初始化方式做进一步的分析和探讨。 
到此为止，图中顺着FrameworkServlet的那些箭头，我们已经交代清楚，读者可以回味一下整个过程。 
**【独立的WebApplicationContext体系】**
独立的WebApplicationContext体系，是SpringMVC初始化主线中的一个非常重要的概念。回顾一下刚才曾经提到过的DispatcherServlet、容器和组件三者之间的关系，我们在引用的那副官方reference的示意图中，实际上已经包含了这一层意思： 
downpour 写道
**结论** 在DispatcherServlet初始化的过程中所构建的WebApplicationContext独立于Spring自身的所构建的其他WebApplicationContext体系而存在。
稍有一些Spring编程经验的程序员，对于下面的配置应该非常熟悉： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:context/applicationContext-*.xml</param-value>
- </context-param>
- 
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
![](http://static.blog.csdn.net/images/save_snippets.png)
在上面的代码中，我们定义了一个Listener，它会在整个Web应用程序启动的时候运行一次，并初始化传统意义上的Spring的容器。这也是一般情况下，当并不使用SpringMVC作为我们的表示层解决方案，却希望在我们的Web应用程序中使用Spring相关功能时所采取的一种配置方式。 
如果我们要在这里引入SpringMVC，整个配置看上去就像这样： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <context-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:context/applicationContext-*.xml</param-value>
- </context-param>
- 
- <listener>
- <listener-class>org.springframework.web.context.ContextLoaderListener</listener-class>
- </listener>
- 
- <!-- Processes application requests -->
- <servlet>
- <servlet-name>dispatcher</servlet-name>
- <servlet-class>org.springframework.web.servlet.DispatcherServlet</servlet-class>
- <init-param>
- <param-name>contextConfigLocation</param-name>
- <param-value>classpath:web/applicationContext-dispatcherServlet.xml</param-value>
- </init-param>
- <load-on-startup>1</load-on-startup>
- </servlet>
- 
- <servlet-mapping>
- <servlet-name>dispatcher</servlet-name>
- <url-pattern>/</url-pattern>
- </servlet-mapping>
![](http://static.blog.csdn.net/images/save_snippets.png)
在这种情况下，DispatcherServlet和ContextLoaderListener会分别构建不同作用范围的容器（WebApplicationContext）。我们可以引入两个不同的概念来对其进行表述：ContextLoaderListener所初始化的容器，我们称之为Root
 WebApplicationContext；而DispatcherServlet所初始化的容器，是SpringMVC
 WebApplicationContext。 
同样采取日志分析的方法，加入了ContextLoaderListener之后，整个启动日志变成了这样： 
引用
[main] INFO /sample - Initializing Spring root WebApplicationContext 
14:56:42,261  INFO ContextLoader:272 - Root WebApplicationContext: initialization started
14:56:42,343 DEBUG StandardServletEnvironment:100 - Initializing new StandardServletEnvironment 
14:56:42,365  INFO XmlWebApplicationContext:495 - Refreshing Root WebApplicationContext: startup date [Mon Feb 06 14:56:42 CST 2012]; root of context hierarchy 
14:56:42,441 DEBUG PathMatchingResourcePatternResolver:550 - Looking for matching resources in directory tree [D:\Work\Demo2do\Sample\target\classes\context] 
14:56:42,442 DEBUG PathMatchingResourcePatternResolver:612 - Searching directory [D:\Work\Demo2do\Sample\target\classes\context] for files matching pattern [D:/Work/Demo2do/Sample/target/classes/context/applicationContext-*.xml]
14:56:42,446 DEBUG PathMatchingResourcePatternResolver:351 - Resolved location pattern [classpath:context/applicationContext-*.xml] to resources [file [D:\Work\Demo2do\Sample\target\classes\context\applicationContext-configuration.xml]] 
14:56:42,447  INFO XmlBeanDefinitionReader:315 - Loading XML bean definitions from file [D:\Work\Demo2do\Sample\target\classes\context\applicationContext-configuration.xml] 
14:56:42,486 DEBUG PluggableSchemaResolver:140 - Loading schema mappings from [META-INF/spring.schemas] 
14:56:42,597 DEBUG DefaultBeanDefinitionDocumentReader:108 - Loading bean definitions 
14:56:42,658 DEBUG PathMatchingResourcePatternResolver:550 - Looking for matching resources in directory tree [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample] 
14:56:42,699 DEBUG ClassPathBeanDefinitionScanner:243 - Identified candidate component class: file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\service\impl\BlogServiceImpl.class] 
14:56:42,750 DEBUG XmlBeanDefinitionReader:216 - Loaded 5 bean definitions from location pattern [classpath:context/applicationContext-*.xml]
14:56:42,750 DEBUG XmlWebApplicationContext:525 - Bean factory for Root WebApplicationContext: org.springframework.beans.factory.support.DefaultListableBeanFactory@478e4327: defining beans [blogService,org.springframework.context.annotation.internalConfigurationAnnotationProcessor,org.springframework.context.annotation.internalAutowiredAnnotationProcessor,org.springframework.context.annotation.internalRequiredAnnotationProcessor,org.springframework.context.annotation.internalCommonAnnotationProcessor];
 root of factory hierarchy 
14:56:42,860 DEBUG ContextLoader:296 - Published root WebApplicationContext as ServletContext attribute with name [org.springframework.web.context.WebApplicationContext.ROOT] 
14:56:42,860  INFO ContextLoader:301 - Root WebApplicationContext: initialization completed in 596 ms
14:56:42,935 DEBUG DispatcherServlet:115 - Initializing servlet 'dispatcher' 
14:56:42,974  INFO DispatcherServlet:444 - FrameworkServlet 'dispatcher': initialization started
14:56:42,974 DEBUG DispatcherServlet:572 - Servlet with name 'dispatcher' will try to create custom WebApplicationContext context of class 'org.springframework.web.context.support.XmlWebApplicationContext', using parent context [Root WebApplicationContext:
 startup date [Mon Feb 06 14:56:42 CST 2012]; root of context hierarchy] 
14:56:42,979  INFO XmlWebApplicationContext:495 - Refreshing WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:56:42 CST 2012]; parent: Root WebApplicationContext
14:56:42,983  INFO XmlBeanDefinitionReader:315 - Loading XML bean definitions from class path resource [web/applicationContext-dispatcherServlet.xml]
14:56:42,987 DEBUG PluggableSchemaResolver:140 - Loading schema mappings from [META-INF/spring.schemas] 
14:56:43,035 DEBUG DefaultBeanDefinitionDocumentReader:108 - Loading bean definitions 
14:56:43,075 DEBUG PathMatchingResourcePatternResolver:550 - Looking for matching resources in directory tree [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller] 
14:56:43,075 DEBUG PathMatchingResourcePatternResolver:612 - Searching directory [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller] for files matching pattern [D:/Work/Demo2do/Sample/target/classes/com/demo2do/sample/web/controller/**/*.class] 
14:56:43,077 DEBUG PathMatchingResourcePatternResolver:351 - Resolved location pattern [classpath*:com/demo2do/sample/web/controller/**/*.class] to resources [file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\BlogController.class],
 file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\UserController.class]] 
14:56:43,079 DEBUG ClassPathBeanDefinitionScanner:243 - Identified candidate component class: file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\BlogController.class] 
14:56:43,080 DEBUG ClassPathBeanDefinitionScanner:243 - Identified candidate component class: file [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller\UserController.class] 
14:56:43,089 DEBUG XmlBeanDefinitionReader:216 - Loaded 19 bean definitions from location pattern [classpath:web/applicationContext-dispatcherServlet.xml]
14:56:43,089 DEBUG XmlWebApplicationContext:525 - Bean factory for WebApplicationContext for namespace 'dispatcher-servlet': org.springframework.beans.factory.support.DefaultListableBeanFactory@5e6458a6: defining beans [org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping#0,org.springframework.format.support.FormattingConversionServiceFactoryBean#0,org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter#0,org.springframework.web.servlet.handler.MappedInterceptor#0,org.springframework.web.servlet.mvc.method.annotation.ExceptionHandlerExceptionResolver#0,org.springframework.web.servlet.mvc.annotation.ResponseStatusExceptionResolver#0,org.springframework.web.servlet.mvc.support.DefaultHandlerExceptionResolver#0,org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping,org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter,org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter,blogController,userController,org.springframework.context.annotation.internalConfigurationAnnotationProcessor,org.springframework.context.annotation.internalAutowiredAnnotationProcessor,org.springframework.context.annotation.internalRequiredAnnotationProcessor,org.springframework.context.annotation.internalCommonAnnotationProcessor,org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0,org.springframework.web.servlet.handler.SimpleUrlHandlerMapping#0,org.springframework.web.servlet.view.InternalResourceViewResolver#0];
 parent: org.springframework.beans.factory.support.DefaultListableBeanFactory@478e4327 
14:56:43,323 DEBUG RequestMappingHandlerMapping:98 - Looking for request mappings in application context: WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:56:42 CST 2012]; parent: Root WebApplicationContext 
14:56:43,345  INFO RequestMappingHandlerMapping:188 - Mapped "{[/blog],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto public org.springframework.web.servlet.ModelAndView com.demo2do.sample.web.controller.BlogController.index() 
14:56:43,346  INFO RequestMappingHandlerMapping:188 - Mapped "{[/login],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto public org.springframework.web.servlet.ModelAndView com.demo2do.sample.web.controller.UserController.login(java.lang.String,java.lang.String) 
14:56:43,707 DEBUG BeanNameUrlHandlerMapping:71 - Looking for URL mappings in application context: WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:56:42 CST 2012]; parent: Root WebApplicationContext 
14:56:43,828  INFO SimpleUrlHandlerMapping:314 - Mapped URL path [/static/**] onto handler 'org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0' 
14:56:43,883 DEBUG DispatcherServlet:523 - Published WebApplicationContext of servlet 'dispatcher' as ServletContext attribute with name [org.springframework.web.servlet.FrameworkServlet.CONTEXT.dispatcher] 
14:56:43,883  INFO DispatcherServlet:463 - FrameworkServlet 'dispatcher': initialization completed in 909 ms 
14:56:43,883 DEBUG DispatcherServlet:136 - Servlet 'dispatcher' configured successfully
2687 [main] INFO org.mortbay.log - Started SelectChannelConnector@0.0.0.0:8080 
Jetty Server started, use 2901 ms
整个启动日志被我们分为了2段。第一段的过程初始化的是Root WebApplicationContext；而第二段的过程初始化的是SpringMVC的WebApplicationContext。我们还是使用了红色的标记和蓝色标记指出了在整个初始化过程中的一些重要事件。其中，有这样一段内容值得我们注意： 
引用
14:56:42,979  INFO XmlWebApplicationContext:495 - Refreshing WebApplicationContext for namespace 'dispatcher-servlet': startup date [Mon Feb 06 14:56:42 CST 2012]; parent: Root WebApplicationContext
在这段日志中，非常明确地指出了SpringMVC WebApplicationContext与Root WebApplicationContext之间的关系：从属关系。因为根据这段日志的表述，SpringMVC
 WebApplicationContext能够感知到Root WebApplicationContext的存在，并且将其作为parent容器。
Spring正是使用这种Parent-Child的容器关系来对不同的编程层次进行划分。这种我们俗称的父子关系实际上不仅仅是一种从属关系，更是一种引用关系。从刚才的日志分析中，我们可以看出：SpringMVC中所定义的一切组件能够无缝地与Root
 WebApplicationContext中的组件整合。
到此为止，我们针对图中以web.xml为核心的箭头分支进行了讲解，读者可以将图中的内容与上面的文字说明对照再次加以理解。 
**【组件默认行为的指定】**
DispatcherServlet的初始化主线的执行体系是顺着其继承结构依次进行的，我们在之前曾经讨论过它的执行次序。所以，只有在FrameworkServlet完成了对于WebApplicationContext和组件的初始化之后，执行权才被正式转移到DispatcherServlet中。我们可以来看看DispatcherServlet此时究竟干了哪些事： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- /**
-  * This implementation calls {@link #initStrategies}.
-  */
- @Override
- protectedvoid onRefresh(ApplicationContext context) {  
-     initStrategies(context);  
- }  
- 
- /**
-  * Initialize the strategy objects that this servlet uses.
-  * <p>May be overridden in subclasses in order to initialize further strategy objects.
-  */
- protectedvoid initStrategies(ApplicationContext context) {  
-     initMultipartResolver(context);  
-     initLocaleResolver(context);  
-     initThemeResolver(context);  
-     initHandlerMappings(context);  
-     initHandlerAdapters(context);  
-     initHandlerExceptionResolvers(context);  
-     initRequestToViewNameTranslator(context);  
-     initViewResolvers(context);  
-     initFlashMapManager(context);  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
onRefresh是FrameworkServlet中预留的扩展方法，在DispatcherServlet中做了一个基本实现：initStrategies。我们粗略一看，很容易就能明白DispatcherServlet到底在这里干些什么了：初始化组件。 
读者或许会问，组件不是已经在WebApplicationContext初始化的时候已经被初始化过了嘛？这里所谓的组件初始化，指的又是什么呢？让我们来看看其中的一个方法的源码： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- /**
-  * Initialize the MultipartResolver used by this class.
-  * <p>If no bean is defined with the given name in the BeanFactory for this namespace,
-  * no multipart handling is provided.
-  */
- privatevoid initMultipartResolver(ApplicationContext context) {  
- try {  
- this.multipartResolver = context.getBean(MULTIPART_RESOLVER_BEAN_NAME, MultipartResolver.class);  
- if (logger.isDebugEnabled()) {  
-             logger.debug("Using MultipartResolver [" + this.multipartResolver + "]");  
-         }  
-     } catch (NoSuchBeanDefinitionException ex) {  
- // Default is no multipart resolver.
- this.multipartResolver = null;  
- if (logger.isDebugEnabled()) {  
-             logger.debug("Unable to locate MultipartResolver with name '" + MULTIPART_RESOLVER_BEAN_NAME +  
- "': no multipart request handling provided");  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
原来，这里的初始化，指的是DispatcherServlet从容器（WebApplicationContext）中读取组件的实现类，并缓存于DispatcherServlet内部的过程。还记得我们之前给出的DispatcherServlet的数据结构吗？这些位于DispatcherServlet内部的组件实际上只是一些来源于容器缓存实例，不过它们同样也是DispatcherServlet进行后续操作的基础。 
**注**：我们在第一篇文章中就曾经提到过Servlet实例内部的属性的访问有线程安全问题。而在这里，我们可以看到所有的组件都以Servlet内部属性的形式被调用，充分证实了这些组件本身也都是无状态的单例对象，所以我们在这里不必考虑线程安全的问题。
如果对上面的代码加以详细分析，我们会发现initMultipartResolver的过程是查找特定MultipartResolver实现类的过程。因为在容器中查找组件的时候，采取的是根据特定名称（MULTIPART_RESOLVER_BEAN_NAME）进行查找的策略。由此，我们可以看到DispatcherServlet进行组件初始化的特点： 
downpour 写道
**结论** DispatcherServlet中对于组件的初始化过程实际上是应用程序在WebApplicationContext中选择和查找组件实现类的过程，也是指定组件在SpringMVC中的默认行为方式的过程。
除了根据特定名称进行查找的策略以外，我们还对DispatcherServlet中指定SpringMVC默认行为方式的其他的策略进行的总结： 
- **名称查找 —— 根据bean的名字在容器中查找相应的实现类**
- **自动搜索 —— 自动搜索容器中所有某个特定组件（接口）的所有实现类**
- **默认配置 —— 根据一个默认的配置文件指定进行实现类加载**
这三条策略恰巧在initHandlerMappings的过程中都有体现，读者可以从其源码中找到相应的线索： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- privatevoid initHandlerAdapters(ApplicationContext context) {  
- this.handlerAdapters = null;  
- 
- if (this.detectAllHandlerAdapters) {  
- // Find all HandlerAdapters in the ApplicationContext, including ancestor contexts.
-         Map<String, HandlerAdapter> matchingBeans = BeanFactoryUtils.beansOfTypeIncludingAncestors(context, HandlerAdapter.class, true, false);  
- if (!matchingBeans.isEmpty()) {  
- this.handlerAdapters = new ArrayList<HandlerAdapter>(matchingBeans.values());  
- // We keep HandlerAdapters in sorted order.
-             OrderComparator.sort(this.handlerAdapters);  
-         }  
-     }  
- else {  
- try {  
-             HandlerAdapter ha = context.getBean(HANDLER_ADAPTER_BEAN_NAME, HandlerAdapter.class);  
- this.handlerAdapters = Collections.singletonList(ha);  
-         }  
- catch (NoSuchBeanDefinitionException ex) {  
- // Ignore, we'll add a default HandlerAdapter later.
-         }  
-     }  
- 
- // Ensure we have at least some HandlerAdapters, by registering
- // default HandlerAdapters if no other adapters are found.
- if (this.handlerAdapters == null) {  
- this.handlerAdapters = getDefaultStrategies(context, HandlerAdapter.class);  
- if (logger.isDebugEnabled()) {  
-             logger.debug("No HandlerAdapters found in servlet '" + getServletName() + "': using default");  
-         }  
-     }  
- }  
![](http://static.blog.csdn.net/images/save_snippets.png)
这里有必要对“默认策略”做一个简要的说明。SpringMVC为一些核心组件设置了默认行为方式的说明，这个说明以一个properties文件的形式位于SpringMVC分发包（例如spring-webmvc-3.1.0.RELEASE.jar）的内部： 
![](http://dl.iteye.com/upload/attachment/0062/9596/52159265-14d6-38ed-a4a3-6766a9876c53.png)
我们可以观察一下DispatcherServlet.properties的内容： 
引用
# Default implementation classes for DispatcherServlet's strategy interfaces. 
# Used as fallback when no matching beans are found in the DispatcherServlet context. 
# Not meant to be customized by application developers. 
org.springframework.web.servlet.LocaleResolver=org.springframework.web.servlet.i18n.AcceptHeaderLocaleResolver
org.springframework.web.servlet.ThemeResolver=org.springframework.web.servlet.theme.FixedThemeResolver
org.springframework.web.servlet.HandlerMapping=org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping,\
org.springframework.web.servlet.mvc.annotation.DefaultAnnotationHandlerMapping 
org.springframework.web.servlet.HandlerAdapter=org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter,\
org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter,\ 
org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerAdapter 
org.springframework.web.servlet.HandlerExceptionResolver=org.springframework.web.servlet.mvc.annotation.AnnotationMethodHandlerExceptionResolver,\
org.springframework.web.servlet.mvc.annotation.ResponseStatusExceptionResolver,\ 
org.springframework.web.servlet.mvc.support.DefaultHandlerExceptionResolver 
org.springframework.web.servlet.RequestToViewNameTranslator=org.springframework.web.servlet.view.DefaultRequestToViewNameTranslator
org.springframework.web.servlet.ViewResolver=org.springframework.web.servlet.view.InternalResourceViewResolver
org.springframework.web.servlet.FlashMapManager=org.springframework.web.servlet.support.DefaultFlashMapManager
结合刚才initHandlerMappings的源码，我们可以发现如果没有开启detectAllHandlerAdapters选项或者根据HANDLER_ADAPTER_BEAN_NAME的名称没有找到相应的组件实现类，就会使用DispatcherServlet.properties文件中对于HandlerMapping接口的实现来进行组件默认行为的初始化。 
由此可见，DispatcherServlet.properties中所指定的所有接口的实现方式在Spring的容器WebApplicationContext中总有相应的定义。这一点，我们在组件的讨论中还会详谈。 
这个部分我们的侧重点是图中DispatcherServlet与容器之间的关系。读者需要理解的是图中为什么会有两份组件定义，它们之间的区别在哪里，以及DispatcherServlet在容器中查找组件的三种策略。 
**小结**
在本文中，我们对SpringMVC的核心类：DispatcherServlet进行了一番梳理。也对整个SpringMVC的两条主线之一的初始化主线做了详细的分析。 
对于DispatcherServlet而言，重要的其实并不是这个类中的代码和逻辑，而是应该掌握这个类在整个框架中的作用以及与SpringMVC中其他要素的关系。 
对于初始化主线而言，核心其实仅仅在于那张笔者为大家精心打造的图。读者只要掌握了这张图，相信对整个SpringMVC的初始化过程会有一个全新的认识。 

