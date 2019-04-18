# Spring 学习笔记（十）渲染 Web 视图 (Apache Tilesa 和 Thymeleaf) - weixin_33985507的博客 - CSDN博客
2018年12月29日 19:17:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：12
## 使用Apache Tiles视图定义布局
为了在Spring中使用Tiles，需要配置几个bean。我们需要一个TilesConfigurer bean，它会负责定位和加载Tile定义并协调生成Tiles。除此之外，还需要TilesViewResolver bean将逻辑视图名称解析为Tile定义。
配置TilesConfigurer来解析Tile定义。
```
@Bean
public TilesConfigurer tilesConfigurer(){
    TilesConfigurer tiles = new TilesConfigurer();
    tiles.setDefinitions(new String[] {
        "/WEB-INF/layout/tiles.xml"
    });
    tiles.setCheckRefresh(true);
    return tiles;
}
```
当配置TilesConfigurer的时候，所要设置的最重要的属性就是definitions。这个属性接受一个String类型的数组，其中每个条目都指定一个Tile定义的XML文件。对于Spittr应用来讲，我们让它在“/WEB-INF/layout/”目录下查找tiles.xml。
其实我们还可以指定多个Tile定义文件，甚至能够在路径位置上使用通配符，当然在上例中我们没有使用该功能。例如，我们要求TilesConfigurer加载“/WEB-INF/”目录下的所有名字为tiles.xml的文件，那么可以按照如下的方式设置definitions属性：
```
tiles.setDefinitons(new String[] {
    "/WEB-INF/**/tiles.xml"
});
```
让我们来配置TilesViewResolver，可以看到，这是一个很基本的bean定义，没有什么要设置的属性：
```
@Bean
public ViewResolver viewResolver(){
    return new TilesViewResolver();
}
```
如果你更喜欢XML配置的话，那么可以按照如下的形式配置TilesConfigurer和TilesViewResolver：
```
<bean id="tilesConfigurer" class="org.springframework.web.servlet.view.tiles.TilesConfigurer">
    <property name="definitions">
        <list>
            <value>/WEB-INF/layout/tiles.xml.xml</value>
            <value>/WEB-INF/views/**/tiles.xml</value>
        </list>
    </property>
</bean>
<bean id="viewResolver" class="org.springframework.web.servlet.view.tiles.TilesViewResolver"/>
```
TilesConfigurer会加载Tile定义并与Apache Tiles协作，而TilesViewRe-solver会将逻辑视图名称解析为引用Tile定义的视图。它是通过查找与逻辑视图名称相匹配的Tile定义实现该功能的。我们需要创建几个Tile定义以了解它是如何运转的。
**定义 Tiles**
Apache Tiles提供了一个文档类型定义（document type definition，DTD），用来在XML文件中指定Tile的定义。每个定义中需要包含一个<definition>元素，这个元素会有一个或多个<putattribute>元素。例如，如下的XML文档为 Spittr 应用定义了几个 tile。
```
<?xml version="1.0" encoding="UTF-8" ?>
<!DOCTYPE tiles-definitions PUBLIC
        "-//Apache Software Foundation//DTD Tiles Configuration 3.0//EN"
        "http://tiles.apache.org/dtds/tiles-config_3_0.dtd">
<tiles-definitions>
    <definition name="base" template="/WEB-INF/layout/page.jsp">
        <put-attribute name="header" value="/WEB-INF/layout/header.jsp"/>
        <put-attribute name="footer" value="/WEB-INF/layout/footer.jsp"/>
    </definition>
    <definition name="home" extends="base">
        <put-attribute name="body" value="/WEB-INF/view/home.jsp"/>
    </definition>
    <definition name="registerForm" extends="base">
        <put-attribute name="body" value="/WEB-INF/view/registerForm.jsp"/>
    </definition>
    <definition name="prefile" extends="base">
        <put-attribute name="body" value="/WEB-INF/views/profile.jsp"/>
    </definition>
    <definition name="spittles" extends="base">
        <put-attribute name="body" value="/WEB-INF/views/spittles.jsp"/>
    </definition>
    <definition name="spittle" extends="base">
        <put-attribute name="body" value="/WEB-INF/views/spittle.jsp"/>
    </definition>
</tiles-definitions>
```
每个<definition>元素都定义了一个Tile，它最终引用的是一个JSP模板。在名为base的Tile中，模板引用的是“/WEBINF/layout/page.jsp”。某个Tile可能还会引用其他的JSP模板，使这些JSP模板嵌入到主模板中。对于base Tile来讲，它引用的是一个头部JSP模板和一个底部JSP模板。
base Tile所引用的page.jsp模板如下面程序清单所示。
```
<%@ taglib uri="http://www.springframework.org/tags" prefix="s" %>
<%@ taglib uri="http://tiles.apache.org/tags-tiles" prefix="t" %>
<%@ page session="false" %>
<html>
<head>
    <title>Spittr</title>
    <link rel="stylesheet" type="text/css" href="<s:uri value="/resource/style.css"/>">
</head>
<body>
    <div id="header">
        <t:insertAttribute name="header"/>
    </div>
    <div id="content">
        <t:insertAttribute name="body"/>
    </div>
    <div id="footer">
        <t:insertAttribute name="footer"/>
    </div>
</body>
</html>
```
在程序清单6.3中，需要重点关注的事情就是如何使用Tile标签库中的<t:insert Attribute> JSP标签来插入其他的模板。在这里，用它来插入名为header、body和footer的模板。最终，它会形成图6.4所示的布局。
![7155422-9e55d4f1a09a9c80.png](https://upload-images.jianshu.io/upload_images/7155422-9e55d4f1a09a9c80.png)
image.png
现在，我们关注一下home Tile，它扩展了base。因为它扩展了base，因此它会继承base中的模板和所有的属性。尽管home Tile定义相对来说很简单，但是它实际上包含了如下的定义：
```
<definition name="home" template="/WEB-INF/layout/page.jsp">
        <put-attribute name="header" value="/WEB-INF/layout/header.jsp"/>
        <put-attribute name="footer" value="/WEB-INF/layout/footer.jsp"/>
        <put-attribute name="body" value="/WEB-INF/views/home.jsp"/>
    </definition>
```
属性所引用的每个模板是很简单的，如下是header.jsp模板：
```
<%@ taglib uri="http://www.springframework.org/tags" prefix="s" %>
<a href="<s:uri value="/" />">
<img scr="<s:uri value="/resources"/>/images/spittr_logo_50.png" border="0"/></a>
```
footer.jsp模板更为简单：
```
Copyright © Craig Walls
```
每个扩展自base的Tile都定义了自己的主体区模板，所以每个都会与其他的有所区别。但是为了完整地了解home Tile，如下展现了home.jsp：
```
<%@ taglib uri="http://java.sun.com/jsp/jstl/core" prefix="c"%>
<%@ page session="false"%>
<h1>Welcome to Spittr</h1>
<a href="<c:url value="/spittles"/>">Spittles</a>
<a href="<c:url value="spitter/register"/>">Register</a>
```
这里的关键点在于通用的元素放到了page.jsp、header.jsp以及footer.jsp中，其他的Tile模板中不再包含这部分内容。这使得它们能够跨页面重用，这些元素的维护也得以简化。
## 使用Thymeleaf
JSP 缺点：
JSP缺乏良好格式的一个副作用就是它很少能够与其产生的HTML类似。所以，在Web浏览器或HTML编辑器中查看未经渲染的JSP模板是非常令人困惑的，而且得到的结果看上去也非常丑陋。
JSP规范是与Servlet规范紧密耦合的。这意味着它只能用在基于Servlet的Web应用之中。JSP模板不能作为通用的模板（如格式化Email），也不能用于非Servlet的Web应用。
### 配置Thymeleaf视图解析器
为了要在Spring中使用Thymeleaf，我们需要配置三个启用Thymeleaf与Spring集成的bean：
-ThymeleafViewResolver：将逻辑视图名称解析为Thymeleaf模板视图；
-SpringTemplateEngine：处理模板并渲染结果；
-TemplateResolver：加载Thymeleaf模板。
如下为声明这些bean的Java配置。
配置Spring对Thymeleaf的支持：
```
@Bean
    public viewReslover viewResolver(SpringTemplateEngine templateEngine) {
        ThymeleafViewResolver viewResolver = new ThymeleafViewResolver();
        viewResolver.setTemplateEngine(templateEngine);
        return viewResolver;
    }
    
    @Bean
    public TemplateEngine templateEngine(TemplateResolver templateResolver) {
        SpringTemplateEngine templateEngine = new SpringTemplateEngine();
        templateEngine.setTemplateResolver(templateResolver);
        return templateEngine;
    }
    
    @Bean
    public TemplateReslover templateReslover() {
        TemplateReslover templateReslover = new ServlteContextTemplateResolver();
        templateReslover.setPrefix("/WEB-INF/templates/");
        templateReslover.setSuffix(".html");
        templateReslover.setTemplate("HTML5");
        return templateReslover;
    }
```
不管使用哪种配置方式，Thymeleaf都已经准备就绪了，它可以将响应中的模板渲染到Spring MVC控制器所处理的请求中。
ThymeleafViewResolver是Spring MVC中ViewResolver的一个实现类。像其他的视图解析器一样，它会接受一个逻辑视图名称，并将其解析为视图。不过在该场景下，视图会是一个Thymeleaf模板。
需要注意的是ThymeleafViewResolver bean中注入了一个对SpringTemplate Engine bean的引用。SpringTemplateEngine会在Spring中启用Thymeleaf引擎，用来解析模板，并基于这些模板渲染结果。可以看到，我们为其注入了一个TemplateResolver bean的引用。
TemplateResolver会最终定位和查找模板。与之前配置InternalResource-ViewResolver类似，它使用了prefix和suffix属性。前缀和后缀将会与逻辑视图名组合使用，进而定位Thymeleaf引擎。它的templateMode属性被设置成了HTML 5，这表明我们预期要解析的模板会渲染成HTML 5输出。
所有的Thymeleaf bean都已经配置完成了，那么接下来我们该创建几个视图了。
### 定义Thymeleaf模板
使用Thymeleaf命名空间的首页模板引擎:
```
<html xmlns="http://www.w3.org/1999/xhtml"
        xmlns="http://www.thymeleaf.org">
<link>
    <title>Spittr</title>
    <link rel="stylesheet" type="/text/css" th:href="@{/resources/style.css}"></link>
</head>
<body>
    <h1>Welcome to Spittr</h1>
    <a th:href="@{/spittles}">Spittles</a>
    <a th:href="@{/spitter/register}">Register</a>
</body>
</html>
```
首页模板相对来讲很简单，**只使用了th:href属性。这个属性与对应的原生HTML属性很类似，也就是href属性，**并且可以按照相同的方式来使用。th:href属性的特殊之处在于它的值中可以包含Thymeleaf表达式，用来计算动态的值。**它会渲染成一个标准的href属性，其中会包含在渲染时动态创建得到的值。**这是Thymeleaf命名空间中很多属性的运行方式：它们对应标准的HTML属性，并且具有相同的名称，但是会渲染一些计算后得到的值。在本例中，使用th:href属性的三个地方都用到了“@{}”表达式，用来计算相对于URL的路径（就像在JSP页面中，我们可能会使用的JSTL <c:url>标签或Spring<s:url>标签类似）。
### 借助Thymeleaf实现表单绑定
表单绑定是Spring MVC的一项重要特性。**它能够将表单提交的数据填充到命令对象中，并将其传递给控制器，**而在展现表单的时候，表单中也会填充命令对象中的值。如果没有表单绑定功能的话，我们需要确保HTML表单域要映射后端命令对象中的属性，并且在校验失败后展现表单的时候，还要负责确保输入域中值要设置为命令对象的属性。但是，如果有表单绑定的话，它就会负责这些事情了。
请参考如下的Thymeleaf模板片段，它会渲染FirstName输入域：
```
<label th:class="${#fields.hasErrors('firstName')}? 'error'">First Name</label>
<input type="text" th:field="*{fileName}" th:class="${#fields.hasErrors('firstName')}? 'error'"/><br/>
```
th:class属性会渲染为一个class属性，它的值是根据给定的表达式计算得到的。在上面的这两个th:class属性中，它会直接检查firstName域有没有校验错误。如果有的话，class属性在渲染时的值为error。如果这个域没有错误的话，将不会渲染class属性。
