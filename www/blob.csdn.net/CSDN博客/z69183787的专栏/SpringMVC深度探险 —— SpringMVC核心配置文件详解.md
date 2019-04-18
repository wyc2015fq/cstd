# SpringMVC深度探险 —— SpringMVC核心配置文件详解 - z69183787的专栏 - CSDN博客
2016年10月19日 16:00:38[OkidoGreen](https://me.csdn.net/z69183787)阅读数：686
在上一篇文章中，我们从DispatcherServlet谈起，最终为读者详细分析了SpringMVC的初始化主线的全部过程。整个初始化主线的研究，其实始终围绕着DispatcherServlet、WebApplicationContext和组件这三大元素之间的关系展开。 
在文章写完之后，也陆续收到了一些反馈，其中比较集中的问题，是有关WebApplicationContext对组件进行初始化的过程交代的不够清楚。所以，本文作为上一篇文章的续文，就试图来讲清楚这个话题。 
**SpringMVC的核心配置文件**
SpringMVC的核心配置文件，我们从整个专栏的第一篇文章就开始接触。所以，我们在这里首先对SpringMVC的核心配置文件做一些概括性的回顾。 
downpour 写道
**结论** SpringMVC的核心配置文件是构成SpringMVC应用程序的必要元素之一。
这是我们在讲有关SpringMVC的构成要素时就曾经提到过的一个重要结论。当时我们所说的另外两大必要元素就是DispatcherServlet和Controller。因而，SpringMVC的核心配置文件在整个应用程序中所起到的作用也是举足轻重的。这也就是我们在这里需要补充对这个文件进行详细分析的原因。 
downpour 写道
**结论** SpringMVC的核心配置文件与传统的Spring Framework的配置文件是一脉相承的。
这个结论很容易理解。作为Spring Framework的一部分，我们可以认为SpringMVC是整个Spring Framework的一个组件。因而两者的配置体系和管理体系完全相同也属情理之中。实际上，SpringMVC所采取的策略，就是借用Spring Framework强大的容器（ApplicationContext）功能，而绝非自行实现。 
downpour 写道
**结论** SpringMVC的核心配置文件是架起DispatcherServlet与WebApplicationContext之间的桥梁。
我们在web.xml中指定SpringMVC的入口程序DispatcherServlet时，实际上蕴含了一个对核心配置文件的指定过程（[servlet-name]-servlet.xml）。当然，我们也可以明确指定这个核心配置文件的位置。这些配置选项，我们已经在上一篇文章中详细介绍过，这里不再重复。 
而上面这一结论，除了说明两者之间的配置关系之外，还包含了一层运行关系：DispatcherServlet负责对WebApplicationContext进行初始化，而初始化的依据，就是这个SpringMVC的核心配置文件。所以，SpringMVC的核心配置文件的内容解读将揭开整个SpringMVC初始化主线的全部秘密。 
如果我们把这个结论与上一个结论结合起来来看，也正因为SpringMVC的核心配置文件使用了与Spring Framework相同的格式，才使其成为DispatcherServlet驾驭Spring的窗口。 
downpour 写道
**结论** SpringMVC的核心配置文件是SpringMVC中所有组件的定义窗口，通过它我们可以指定整个SpringMVC的行为方式。
这个结论告诉了我们SpringMVC核心配置文件在整个框架中的作用。组件行为模式的多样化，决定了我们必须借助一个容器（WebApplicationContext）来进行统一的管理。而SpringMVC的核心配置文件，就是我们进行组件管理的窗口。 
**核心配置文件概览**
说了那么多有关SpringMVC核心配置文件的结论，我们不妨来看一下这个配置文件的概况： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:mvc="http://www.springframework.org/schema/mvc"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-             http://www.springframework.org/schema/beans/spring-beans-3.1.xsd  
-             http://www.springframework.org/schema/context   
-             http://www.springframework.org/schema/context/spring-context-3.1.xsd  
-             http://www.springframework.org/schema/mvc  
-             http://www.springframework.org/schema/mvc/spring-mvc-3.1.xsd">
- 
- <!-- Enables the Spring MVC @Controller programming model -->
- <mvc:annotation-driven/>
- 
- <context:component-scanbase-package="com.demo2do.sample.web.controller"/>
- 
- <!-- Handles HTTP GET requests for /static/** by efficiently serving up static resources in the ${webappRoot}/static/ directory -->
- <mvc:resourcesmapping="/static/**"location="/static/"/>
- 
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="prefix"value="/"/>
- <propertyname="suffix"value=".jsp"/>
- </bean>
- 
- </beans>
这是一个非常典型的SpringMVC核心配置文件。虽然我们在这里几乎对每一段重要的配置都做了注释，不过可能对于毫无SpringMVC开发经验的读者来说，这段配置基本上还无法阅读。所以接下来，我们就试图对这个文件中的一些细节加以说明。 
**【头部声明】**
配置文件中首先进入我们眼帘的是它的头部的一大段声明： 
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <beansxmlns="http://www.springframework.org/schema/beans"
- xmlns:mvc="http://www.springframework.org/schema/mvc"
- xmlns:context="http://www.springframework.org/schema/context"
- xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
- xsi:schemaLocation="http://www.springframework.org/schema/beans  
-             http://www.springframework.org/schema/beans/spring-beans-3.1.xsd  
-             http://www.springframework.org/schema/context   
-             http://www.springframework.org/schema/context/spring-context-3.1.xsd  
-             http://www.springframework.org/schema/mvc  
-             http://www.springframework.org/schema/mvc/spring-mvc-3.1.xsd">
-       ......  
- </beans>
这个部分是整个SpringMVC核心配置文件的关键所在。这一段声明，被称之为Schema-based XML的声明部分。有关Schema-based
 XML的概念，读者可以参考Spring官方的reference： 
[Appendix C. XML Schema-based configuration](http://static.springsource.org/spring/docs/3.1.0.RELEASE/reference/html/xsd-config.html)
[Appendix D. Extensible XML authoring](http://static.springsource.org/spring/docs/3.1.0.RELEASE/reference/html/extensible-xml.html)
为了帮助读者快速理解，我们稍后会专门开辟章节针对Schema-based XML的来龙去脉进行讲解。 
**【组件定义】**
除了头部声明部分的其他配置部分，就是真正的组件定义部分。在这个部分中，我们可以看到两种不同类型的配置定义模式： 
1. 基于Schema-based XML的配置定义模式
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <mvc:annotation-driven/>
2. 基于Traditional XML的配置定义模式
Xml代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- <beanclass="org.springframework.web.servlet.view.InternalResourceViewResolver">
- <propertyname="prefix"value="/"/>
- <propertyname="suffix"value=".jsp"/>
- lt;/bean>
两种不同的组件定义模式，其目的是统一的：对SpringMVC中的组件进行声明，指定组件的行为方式。
虽然两种不同的组件定义模式的外在表现看上去有所不同，但是SpringMVC在对其进行解析时最终都会将其转化为组件的定义而加载到WebApplicationContext之中进行管理。所以我们需要理解的是蕴藏在配置背后的目的而非配置本身的形式。 
至于这两种不同的配置形式之间的关系，我们稍后会在Schema-based XML的讲解中详细展开。 
**Schema-based XML**
**【基本概念】**
Schema-based XML本身并不是SpringMVC或者Spring Framework独创的一种配置模式。我们可以看看W3C对于其用途的一个大概解释： 
W3C 写道
The purpose of an XSD schema is to define and describe a class of XML documents by using schema components to constrain and document the meaning, usage and relationships of their constituent parts: datatypes, elements and their content and attributes and their
 values. Schemas can also provide for the specification of additional document information, such as normalization and defaulting of attribute and element values. Schemas have facilities for self-documentation. Thus, XML Schema Definition Language: Structures
 can be used to define, describe and catalogue XML vocabularies for classes of XML documents.
这个解释稍微有点抽象。所以我们可以来看看Spring官方reference对于引入Schema-based XML的说法： 
Spring Reference 写道
The central motivation for moving to XML Schema based configuration files was to make Spring XML configuration easier. The 'classic' <bean/>-based approach is good, but its generic-nature comes with a price in terms of configuration overhead.
也就是说，我们引入Schema-based XML是为了对Traditional的XML配置形式进行简化。通过Schema的定义，把一些原本需要通过几个bean的定义或者复杂的bean的组合定义的配置形式，用另外一种简单而可读的配置形式呈现出来。
所以，我们也可以由此得出一些有用的推论： 
downpour 写道
Schema-based XML可以代替Traditional的XML配置形式，在Spring容器中进行组件的定义。
这里的代替一词非常重要，这就意味着传统的XML配置形式在这里会被颠覆，我们在对Schema-based XML进行解读时，需要使用一种全新的语义规范来理解。 
downpour 写道
Schema-based XML可以极大改善配置文件的可读性并且缩小配置文件的规模。
这是从引入Schema-based XML的目的反过来得出的推论。因为如果引入Schema-based XML之后，整个配置变得更加复杂，那么Schema-based XML的引入也就失去了意义。 
同时，笔者在这里需要特别强调的是Schema-based XML的引入，实际上是把原本静态的配置动态化、过程化。有关这一点，我们稍后会有说明。 
**【引入目的】**
在早期的Spring版本中，只有Traditional XML一种组件定义模式。当时，XML作为Java最好的朋友，自然而然在整个框架中起到了举足轻重的作用。根据Spring的设计原则，所有纳入WebApplicationContext中管理的对象，都被映射为XML中的一个<bean>节点，通过对于<bean>节点的一个完整描述，我们可以有效地将整个应用程序中所有的对象都纳入到一个统一的容器中进行管理。 
这种统一化的描述，带来的是管理上的便利，不过同时也带来了逻辑上的困扰。因为统一的节点，降低了配置的难度，我们几乎只需要将<bean>节点与Java的对象模型对应起来即可。（有一定经验的Spring程序员可以回忆一下，我们在编写Spring配置文件时，是否也是一个将配置选项与Java对象中属性或者方法对应起来的过程）但是这样的配置形式本身并不具备逻辑语义，也就是说我们无法非常直观地看出某一个特定的<bean>定义，从逻辑上它到底想说明什么问题？ 
这也就是后来Schema-based XML开始被引入并流行开来的重要原因。从形式上看，Schema-based XML相比较Traditional XML至少有三个方面的优势： 
- **namespace —— 拥有很明确的逻辑分类**
- element —— 拥有很明确的过程语义
- attributes —— 拥有很简明的配置选项
这三方面的优势，我们可以用一幅图来进行说明： 
![](http://dl.iteye.com/upload/attachment/0063/6548/168ebe76-c51f-35ab-859b-14d2c8fde863.png)
在图中，我们分别用上下两层来说明某一个配置节点的结构名称以及它们的具体作用。由此可见，Schema-based XML中的配置节点拥有比较鲜明的功能特性，通过namespace、element和attributes这三大元素之间的配合，共同完成对一个动态过程的描述。
例如，<mvc:annotation-driven />这段配置想要表达的意思，就是在mvc的空间内实现Annotation驱动的配置方式。其中，mvc表示配置的有效范围，annotation-driven则表达了一个动态的过程，实际的逻辑含义是：整个SpringMVC的实现是基于Annotation模式，请为我注册相关的行为模式。 
这种配置方式下，可读性大大提高：我们无需再去理解其中的实现细节。同时，配置的简易性也大大提高：我们甚至不用去关心哪些bean被定义了。 
所以总体来说，Schema-based XML的引入，对于配置的简化是一个极大的进步。 
**【构成要素】**
在Spring中，一个Schema-based XML有两大构成要素：过程实现和配置定义。 
先谈谈过程实现。所谓过程实现，其实就是我们刚才所举的那个例子中，实现实际背后逻辑的过程。这个过程由两个Java接口来进行表述： 
- NamespaceHandler —— 对Schema定义中namespace的逻辑处理接口
- BeanDefinitionParser —— 对Schema定义中element的逻辑处理接口
很显然，NamespaceHandler是入口程序，它包含了所有的属于该namespace定义下所有element的处理调用，所以BeanDefinitionParser的实现就成为了NamespaceHandler的调用对象了。这一点，我们可以通过NamesapceHandler的MVC实现类来加以证明： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- publicvoid init() {  
-     registerBeanDefinitionParser("annotation-driven", new AnnotationDrivenBeanDefinitionParser());  
-     registerBeanDefinitionParser("default-servlet-handler", new DefaultServletHandlerBeanDefinitionParser());  
-     registerBeanDefinitionParser("interceptors", new InterceptorsBeanDefinitionParser());         
-     registerBeanDefinitionParser("resources", new ResourcesBeanDefinitionParser());  
-     registerBeanDefinitionParser("view-controller", new ViewControllerBeanDefinitionParser());  
- }  
我们可以看到，MvcNamespaceHandler的执行，只不过依次调用了不同的BeanDefinitionParser的实现类而已，而每一个BeanDefinitionParser的实现，则对应于Schema定义中的element逻辑处理。例如，AnnotationDrivenBeanDefinitionParser对应于：<mvc:annotation-driven
 />这个element实现；ResourcesBeanDefinitionParser则对应于<mvc:resources />的实现等等。 
所以，要具体了解每个element的行为过程，只要研究每一个BeanDefinitionParser的实现类即可。我们以整个MVC空间中最重要的一个节点<mvc:annotation-driven />为例，对AnnotationDrivenBeanDefinitionParser进行说明，其源码如下： 
Java代码  ![收藏代码](http://downpour.iteye.com/images/icon_star.png)
- public BeanDefinition parse(Element element, ParserContext parserContext) {  
-     Object source = parserContext.extractSource(element);  
- 
-     CompositeComponentDefinition compDefinition = new CompositeComponentDefinition(element.getTagName(), source);  
-     parserContext.pushContainingComponent(compDefinition);  
- 
-     RootBeanDefinition methodMappingDef = new RootBeanDefinition(RequestMappingHandlerMapping.class);  
-     methodMappingDef.setSource(source);  
-     methodMappingDef.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-     methodMappingDef.getPropertyValues().add("order", 0);  
-     String methodMappingName = parserContext.getReaderContext().registerWithGeneratedName(methodMappingDef);  
- 
-     RuntimeBeanReference conversionService = getConversionService(element, source, parserContext);  
-     RuntimeBeanReference validator = getValidator(element, source, parserContext);  
-     RuntimeBeanReference messageCodesResolver = getMessageCodesResolver(element, source, parserContext);  
- 
-     RootBeanDefinition bindingDef = new RootBeanDefinition(ConfigurableWebBindingInitializer.class);  
-     bindingDef.setSource(source);  
-     bindingDef.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-     bindingDef.getPropertyValues().add("conversionService", conversionService);  
-     bindingDef.getPropertyValues().add("validator", validator);  
-     bindingDef.getPropertyValues().add("messageCodesResolver", messageCodesResolver);  
- 
-     ManagedList<?> messageConverters = getMessageConverters(element, source, parserContext);  
-     ManagedList<?> argumentResolvers = getArgumentResolvers(element, source, parserContext);  
-     ManagedList<?> returnValueHandlers = getReturnValueHandlers(element, source, parserContext);  
- 
-     RootBeanDefinition methodAdapterDef = new RootBeanDefinition(RequestMappingHandlerAdapter.class);  
-     methodAdapterDef.setSource(source);  
-     methodAdapterDef.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-     methodAdapterDef.getPropertyValues().add("webBindingInitializer", bindingDef);  
-     methodAdapterDef.getPropertyValues().add("messageConverters", messageConverters);  
- if (element.hasAttribute("ignoreDefaultModelOnRedirect")) {  
-         Boolean ignoreDefaultModel = Boolean.valueOf(element.getAttribute("ignoreDefaultModelOnRedirect"));  
- 
- methodAdapterDef.getPropertyValues().add("ignoreDefaultModelOnRedirect", ignoreDefaultModel);  
-     }  
- if (argumentResolvers != null) {  
-         methodAdapterDef.getPropertyValues().add("customArgumentResolvers", argumentResolvers);  
-     }  
- if (returnValueHandlers != null) {  
-             methodAdapterDef.getPropertyValues().add("customReturnValueHandlers", returnValueHandlers);  
-     }  
-     String methodAdapterName = parserContext.getReaderContext().registerWithGeneratedName(methodAdapterDef);  
- 
-     RootBeanDefinition csInterceptorDef = new RootBeanDefinition(ConversionServiceExposingInterceptor.class);  
-     csInterceptorDef.setSource(source);  
-     csInterceptorDef.getConstructorArgumentValues().addIndexedArgumentValue(0, conversionService);  
-     RootBeanDefinition mappedCsInterceptorDef = new RootBeanDefinition(MappedInterceptor.class);  
-     mappedCsInterceptorDef.setSource(source);  
-         mappedCsInterceptorDef.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-         mappedCsInterceptorDef.getConstructorArgumentValues().addIndexedArgumentValue(0, (Object) null);  
-         mappedCsInterceptorDef.getConstructorArgumentValues().addIndexedArgumentValue(1, csInterceptorDef);  
-     String mappedInterceptorName = parserContext.getReaderContext().registerWithGeneratedName(mappedCsInterceptorDef);  
- 
-     RootBeanDefinition methodExceptionResolver = new RootBeanDefinition(ExceptionHandlerExceptionResolver.class);  
-     methodExceptionResolver.setSource(source);  
-         methodExceptionResolver.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-         methodExceptionResolver.getPropertyValues().add("messageConverters", messageConverters);  
-         methodExceptionResolver.getPropertyValues().add("order", 0);  
-     String methodExceptionResolverName =  
- parserContext.getReaderContext().registerWithGeneratedName(methodExceptionResolver);  
- 
-     RootBeanDefinition responseStatusExceptionResolver = new RootBeanDefinition(ResponseStatusExceptionResolver.class);  
-     responseStatusExceptionResolver.setSource(source);  
-         responseStatusExceptionResolver.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-         responseStatusExceptionResolver.getPropertyValues().add("order", 1);  
-     String responseStatusExceptionResolverName =  
-                 parserContext.getReaderContext().registerWithGeneratedName(responseStatusExceptionResolver);  
- 
-     RootBeanDefinition defaultExceptionResolver = new RootBeanDefinition(DefaultHandlerExceptionResolver.class);  
-     defaultExceptionResolver.setSource(source);  
-         defaultExceptionResolver.setRole(BeanDefinition.ROLE_INFRASTRUCTURE);  
-         defaultExceptionResolver.getPropertyValues().add("order", 2);  
-     String defaultExceptionResolverName =  
-                 parserContext.getReaderContext().registerWithGeneratedName(defaultExceptionResolver);  
- 
-     parserContext.registerComponent(new BeanComponentDefinition(methodMappingDef, methodMappingName));  
-     parserContext.registerComponent(new BeanComponentDefinition(methodAdapterDef, methodAdapterName));  
-     parserContext.registerComponent(new BeanComponentDefinition(methodExceptionResolver, methodExceptionResolverName));  
-     parserContext.registerComponent(new BeanComponentDefinition(responseStatusExceptionResolver, responseStatusExceptionResolverName));  
-     parserContext.registerComponent(new BeanComponentDefinition(defaultExceptionResolver, defaultExceptionResolverName));  
-     parserContext.registerComponent(new BeanComponentDefinition(mappedCsInterceptorDef, mappedInterceptorName));  
- 
- // Ensure BeanNameUrlHandlerMapping (SPR-8289) and default HandlerAdapters are not "turned off" 
-         MvcNamespaceUtils.registerDefaultComponents(parserContext, source);  
- 
-     parserContext.popAndRegisterContainingComponent();  
- 
- returnnull;  
- }  
整个过程看上去稍显凌乱，不过我们发现其中围绕的一条主线就是：使用编程的方式来对bean进行注册。也就是说，<mvc:annotation-driven
 />这样一句配置，顶上了我们如此多的bean定义。难怪Schema-based XML被誉为是简化XML配置的绝佳帮手了。 
有了过程实现，我们再来谈谈配置定义。配置定义的目的非常简单，就是通过一些配置文件，将上述的过程实现类串联起来，从而完成整个Schema-based XML的定义。 
整个配置定义，也分为两个部分： 
- **Schema定义 —— 一个xsd文件，描述整个Schema空间中element和attribute的定义**
- **注册配置文件 —— 由META-INF/spring.handlers和META-INF/spring.schemas构成，用以注册Schema和Handler**
Schema定义是由一个xsd文件完成的。这个文件在Spring发布的时候同时发布在网络上。例如SpringMVC的Schema定义，就发布在这个地址： 
[http://www.springframework.org/schema/mvc/spring-mvc-3.1.xsd](http://www.springframework.org/schema/mvc/spring-mvc-3.1.xsd)
同时，这个地址在Spring的发布包中也存有一个备份。这个备份位于SpringMVC的分发包spring-webmvc的JAR包之中。 
![](http://dl.iteye.com/upload/attachment/0063/6666/38811a12-3a04-37b6-81ac-c1fd78199f28.png)
这样做的好处在于，我们在对Schema进行引用时，可以通过本地寻址来加快加载速度。 
**注**：如果我们回顾一下之前的核心配置文件中的头部声明部分。其中的xsi:schemaLocation声明就是用于指定映射于本地的XSD文件。所以xsi:schemaLocation的定义不是必须的，不过声明它能够使Spring自动查找本地的缓存来进行schema的寻址。
我们在这里不对XSD文件做过多的内容分析，因为其中不外乎是对element的定义、attributes的定义等等。这些内容是我们进行Schema-based XML配置的核心基础。 
配置定义的另外一个元素构成是META-INF/spring.handlers和META-INF/spring.schemas这两个文件。它们同样位于SpringMVC的分发包下。当我们在XML的头部声明中引用了相关的Schema定义之后，Spring会自动查找spring.schemas和spring.handlers的定义，根据其中指定的NamespaceHandler实现类加载执行。 
有关这个过程，我们在之后的日志分析中还会涉及。 
**初始化日志的再分析**
有了Schema Based XML的相关知识，就可以对DispatcherServlet的初始化启动日志做进一步的详细分析。而这次的分析，我们试图弄清楚以下问题： 
- **Where —— 组件的声明在哪里？**
- **How —— 组件是如何被注册的？**
- **What —— 究竟哪些组件被注册了？**
对于这三个问题的研究，我们需要结合日志和Schema based XML的运行机理来共同进行分析。 
引用
**[main] INFO /sample - Initializing Spring FrameworkServlet 'dispatcher' 19:49:48,670  INFO XmlWebApplicationContext:495 - Refreshing WebApplicationContext for namespace 'dispatcher-servlet': startup date [Thu Feb 16 19:49:48 CST 2012]; parent: Root WebApplicationContext 19:49:48,674  INFO XmlBeanDefinitionReader:315 - Loading XML bean definitions from class path resource [web/applicationContext-dispatcher.xml] ## Schema定位和加载 (开始) ##**
19:49:48,676 DEBUG DefaultDocumentLoader:72 - Using JAXP provider [com.sun.org.apache.xerces.internal.jaxp.DocumentBuilderFactoryImpl] 
19:49:48,678 DEBUG PluggableSchemaResolver:140 - Loading schema mappings from [META-INF/spring.schemas] 
19:49:48,690 DEBUG PluggableSchemaResolver:118 - Found XML schema [http://www.springframework.org/schema/beans/spring-beans-3.1.xsd] in classpath: org/springframework/beans/factory/xml/spring-beans-3.1.xsd 
19:49:48,710 DEBUG PluggableSchemaResolver:118 - Found XML schema [http://www.springframework.org/schema/mvc/spring-mvc-3.1.xsd] in classpath: org/springframework/web/servlet/config/spring-mvc-3.1.xsd 
19:49:48,715 DEBUG PluggableSchemaResolver:118 - Found XML schema [http://www.springframework.org/schema/tool/spring-tool-3.1.xsd] in classpath: org/springframework/beans/factory/xml/spring-tool-3.1.xsd 
19:49:48,722 DEBUG PluggableSchemaResolver:118 - Found XML schema [http://www.springframework.org/schema/context/spring-context-3.1.xsd] in classpath: org/springframework/context/config/spring-context-3.1.xsd 
**## Schema定位和加载 (结束) #### NamespaceHandler执行阶段 (开始) ##**
19:49:48,731 DEBUG DefaultBeanDefinitionDocumentReader:108 - Loading bean definitions 
19:49:48,742 DEBUG DefaultNamespaceHandlerResolver:156 - Loaded NamespaceHandler mappings: {...} 
19:49:48,886 DEBUG PathMatchingResourcePatternResolver:550 - Looking for matching resources in directory tree [D:\Work\Demo2do\Sample\target\classes\com\demo2do\sample\web\controller] 
19:49:48,896 DEBUG XmlBeanDefinitionReader:216 - **Loaded 18 bean definitions from location pattern [classpath:web/applicationContext-dispatcher.xml]**
19:49:48,897 DEBUG XmlWebApplicationContext:525 - Bean factory for WebApplicationContext for namespace 'dispatcher-servlet': org.springframework.beans.factory.support.DefaultListableBeanFactory@495c998a: defining beans [[ 
**1. org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerMapping#0, 2. org.springframework.format.support.FormattingConversionServiceFactoryBean#0, 3. org.springframework.web.servlet.mvc.method.annotation.RequestMappingHandlerAdapter#0, 4. org.springframework.web.servlet.handler.MappedInterceptor#0, 5. org.springframework.web.servlet.mvc.method.annotation.ExceptionHandlerExceptionResolver#0,6. org.springframework.web.servlet.mvc.annotation.ResponseStatusExceptionResolver#0, 7. org.springframework.web.servlet.mvc.support.DefaultHandlerExceptionResolver#0, 8. org.springframework.web.servlet.handler.BeanNameUrlHandlerMapping, 9. org.springframework.web.servlet.mvc.HttpRequestHandlerAdapter, 10. org.springframework.web.servlet.mvc.SimpleControllerHandlerAdapter, 11. blogController, 12. userController, 13. org.springframework.context.annotation.internalConfigurationAnnotationProcessor, 14. org.springframework.context.annotation.internalAutowiredAnnotationProcessor, 15. org.springframework.context.annotation.internalRequiredAnnotationProcessor, 16. org.springframework.context.annotation.internalCommonAnnotationProcessor, 17. org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0, 18. org.springframework.web.servlet.handler.SimpleUrlHandlerMapping#0**
]; parent: org.springframework.beans.factory.support.DefaultListableBeanFactory@6602e323 
19:49:48,949 DEBUG XmlWebApplicationContext:794 - Unable to locate MessageSource with name 'messageSource': using default [org.springframework.context.support.DelegatingMessageSource@4b2922f6] 
19:49:48,949 DEBUG XmlWebApplicationContext:818 - Unable to locate ApplicationEventMulticaster with name 'applicationEventMulticaster': using default [org.springframework.context.event.SimpleApplicationEventMulticaster@79b66b06] 
19:49:48,949 DEBUG UiApplicationContextUtils:85 - Unable to locate ThemeSource with name 'themeSource': using default [org.springframework.ui.context.support.DelegatingThemeSource@372c9557] 
19:49:49,154 DEBUG RequestMappingHandlerMapping:98 - Looking for request mappings in application context: WebApplicationContext for namespace 'dispatcher-servlet': startup date [Thu Feb 16 19:49:48 CST 2012]; parent: Root WebApplicationContext 
19:49:49,175  INFO RequestMappingHandlerMapping:188 - Mapped "{[/blog],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto 
public org.springframework.web.servlet.ModelAndView com.demo2do.sample.web.controller.BlogController.index() 
19:49:49,177  INFO RequestMappingHandlerMapping:188 - Mapped "{[/register],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto 
public org.springframework.web.servlet.ModelAndView com.demo2do.sample.web.controller.UserController.register(com.demo2do.sample.entity.User) 
19:49:49,180  INFO RequestMappingHandlerMapping:188 - Mapped "{[/login],methods=[],params=[],headers=[],consumes=[],produces=[],custom=[]}" onto 
public org.springframework.web.servlet.ModelAndView com.demo2do.sample.web.controller.UserController.login(java.lang.String,java.lang.String) 
19:49:49,632 DEBUG BeanNameUrlHandlerMapping:71 - Looking for URL mappings in application context: WebApplicationContext for namespace 'dispatcher-servlet': startup date [Thu Feb 16 19:49:48 CST 2012]; parent: Root WebApplicationContext 
19:49:49,924  INFO SimpleUrlHandlerMapping:314 - Mapped URL path [/static/**] onto handler 'org.springframework.web.servlet.resource.ResourceHttpRequestHandler#0' 
**## NamespaceHandler执行阶段 (结束) ##**
19:49:49,956 DEBUG DispatcherServlet:627 - Unable to locate RequestToViewNameTranslator with name 'viewNameTranslator': using default [org.springframework.web.servlet.view.DefaultRequestToViewNameTranslator@4d16318b] 
19:49:49,980 DEBUG DispatcherServlet:667 - No ViewResolvers found in servlet 'dispatcher': using default
19:49:49,986 DEBUG DispatcherServlet:689 - Unable to locate FlashMapManager with name 'flashMapManager': using default [org.springframework.web.servlet.support.DefaultFlashMapManager@1816daa9] 
**19:49:49,986 DEBUG DispatcherServlet:523 - Published WebApplicationContext of servlet 'dispatcher' as ServletContext attribute with name [org.springframework.web.servlet.FrameworkServlet.CONTEXT.dispatcher] 19:49:49,986  INFO DispatcherServlet:463 - FrameworkServlet 'dispatcher': initialization completed in 1320 ms 19:49:49,987 DEBUG DispatcherServlet:136 - Servlet 'dispatcher' configured successfully**
在上面的启动日志中，笔者还是把不同的日志功能使用不同的颜色进行了区分。这里进行逐一分析： 
1. 黑色加粗标记区域 —— 容器的启动和结束标志
这个部分的日志比较明显，位于容器的启动阶段和结束阶段，在之前的讨论中我们已经分析过，这里不再重复。
2. 黄色注释段落 —— Schema定位和加载
这个部分的日志反应出刚才我们所分析的Schema-based XML的工作原理。这是其中的第一步：读取META-INF/spring.schemas的内容，加载schema定义。然后找到相应的NamespaceHandler，执行其实现类。
3. 蓝色注释部分 —— NamespaceHandler执行阶段
这个部分的日志，可以帮助我们回答本节一开始所提出的两个问题。**绝大多数的组件，都是在BeanDefinitionParser的实现类中使用编程的方式注册的。**
4. 红色标记区域 —— 组件注册细节
这个部分的日志区域彻底回答了本节一开始所提出的最后一个问题：一共有18个组件被注册，就是红色标记的那18个bean。
**小结**
本文所涉及到的话题，主要围绕着SpringMVC的核心配置问题展开。读者可以将本文作为上一篇文章的续篇，将两者结合起来阅读。因为从宏观上说，本文的话题实际上也属于初始化主线的一个部分。 
