# SpringMVC集成Tiles2 - z69183787的专栏 - CSDN博客
2014年10月24日 10:11:54[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1091
个人分类：[SpringMvc																[SpringMvc-Tiles网页模板](https://blog.csdn.net/z69183787/article/category/2656935)](https://blog.csdn.net/z69183787/article/category/2176803)
**介绍Tiles2**
如今，网站一般被划分成片正在呈现不同的网页之间的可重复使用的模板。例如，一个网站中包含头，页脚，菜单等，这项目仍然通过了该网站，并给它一个共同的外观和感觉。这是非常困难的硬编码在每一个网页，如果以后需要改变，不是所有的页面需要进行修改。因此，我们使用模板化机制。我们创建了一个共同的页眉，页脚，菜单页面，包括在每个页面。
瓷砖插件允许模板和组件。事实上，这两种机制是相似的：
定义组装兴建另一部分或一整页页（“Tiles”）。的一部分
带参数，允许动态的内容，并且可以被看作是一种方法，用JAVA语言。Tiles是保持一致的外观和感觉一个web应用程序在所有的网页模板系统使用。它增加模板的可重用性，减少代码重复。
中央配置文件中定义一个常见的​​网页布局，这种布局可以扩展到所有的Web应用程序的网页。
**应用程序布局**
我们的目标是我们Spring3HelloWorld应用程序添加页眉，页脚和菜单。以下将是相同的布局。
**![http://img.viralpatel.net/2009/12/struts2-tiles-layout.gif](http://img.viralpatel.net/2009/12/struts2-tiles-layout.gif)**
**所需JAR包**
**![spring-3-tiles-framework-jar](http://img.viralpatel.net/2010/07/spring-3-tiles-framework-jar2.png)**
上述表中突出显示的jar文件要添加新的Tiles集成项目。
在Spring mvc 中配置Tiles框架
要配置Tiles，在spring-servlet.xml中配置一个TilesConfig的<bean>。打开spring-servlet.xml，并添加下面的代码<beans></beans>标记之间。
WEB-INF/spring-servlet.xml:
**[html]**[view
 plain](http://blog.csdn.net/zemochen/article/details/9569451#)[copy](http://blog.csdn.net/zemochen/article/details/9569451#)[print](http://blog.csdn.net/zemochen/article/details/9569451#)[?](http://blog.csdn.net/zemochen/article/details/9569451#)
- <beanid="viewResolver"
- class="org.springframework.web.servlet.view.UrlBasedViewResolver">
- <propertyname="viewClass">
- <value>
-             org.springframework.web.servlet.view.tiles2.TilesView 
- </value>
- </property>
- </bean>
- <beanid="tilesConfigurer"
- class="org.springframework.web.servlet.view.tiles2.TilesConfigurer">
- <propertyname="definitions">
- <list>
- <value>/WEB-INF/tiles.xml</value>
- </list>
- </property>
- </bean>
通过上面定义的参数.在WEB-INF中写一个tiles的配置文件"tiles.xml"这个xml为我们的web项目定义Tiles
![tiles-xml-spring-mvc](http://img.viralpatel.net/2009/12/struts2-tiles-xml.png)
WEB-INF/tiles.xml:
**[html]**[view
 plain](http://blog.csdn.net/zemochen/article/details/9569451#)[copy](http://blog.csdn.net/zemochen/article/details/9569451#)[print](http://blog.csdn.net/zemochen/article/details/9569451#)[?](http://blog.csdn.net/zemochen/article/details/9569451#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <!DOCTYPE tiles-definitions PUBLIC 
-        "-//Apache Software Foundation//DTD Tiles Configuration 2.0//EN" 
-        "http://tiles.apache.org/dtds/tiles-config_2_0.dtd">
- <tiles-definitions>
- <definitionname="base.definition"
- template="/WEB-INF/jsp/layout.jsp">
- <put-attributename="title"value=""/>
- <put-attributename="header"value="/WEB-INF/jsp/header.jsp"/>
- <put-attributename="menu"value="/WEB-INF/jsp/menu.jsp"/>
- <put-attributename="body"value=""/>
- <put-attributename="footer"value="/WEB-INF/jsp/footer.jsp"/>
- </definition>
- 
- <definitionname="contact"extends="base.definition">
- <put-attributename="title"value="Contact
 Manager"/>
- <put-attributename="body"value="/WEB-INF/jsp/contact.jsp"/>
- </definition>
- 
- </tiles-definitions>
在这里，我们有tiles.xml定义模板base.definition。该布局包含的属性，如头，标题，主体，菜单和页脚。然后扩展和布局的新关联页面。我们覆盖默认的布局改变Body和Title的内容。
**创建我们的JSP文件**
**![spring-tiles-jsp-files](http://img.viralpatel.net/2010/07/spring-tiles-jsp-files.png)**
我们写一个layout.jsp作为我们web项目的布局页面然后引入相应的页面
WEB-INF/jsp/layout.jsp
**[html]**[view
 plain](http://blog.csdn.net/zemochen/article/details/9569451#)[copy](http://blog.csdn.net/zemochen/article/details/9569451#)[print](http://blog.csdn.net/zemochen/article/details/9569451#)[?](http://blog.csdn.net/zemochen/article/details/9569451#)
- <%@
 taglib uri="http://tiles.apache.org/tags-tiles"prefix="tiles"%>
- <!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" 
- "http://www.w3.org/TR/html4/loose.dtd">
- <html>
- <head>
- <metahttp-equiv="Content-Type"content="text/html;
 charset=UTF-8">
- <title><tiles:insertAttributename="title"ignore="true"/></title>
- </head>
- <body>
- <tableborder="1"cellpadding="2"cellspacing="2"align="center">
- <tr>
- <tdheight="30"colspan="2"><tiles:insertAttributename="header"/>
- </td>
- </tr>
- <tr>
- <tdheight="250"><tiles:insertAttributename="menu"/></td>
- <tdwidth="350"><tiles:insertAttributename="body"/></td>
- </tr>
- <tr>
- <tdheight="30"colspan="2"><tiles:insertAttributename="footer"/>
- </td>
- </tr>
- </table>
- </body>
- </html>
WEB-INF/jsp/header.jsp
**[html]**[view
 plain](http://blog.csdn.net/zemochen/article/details/9569451#)[copy](http://blog.csdn.net/zemochen/article/details/9569451#)[print](http://blog.csdn.net/zemochen/article/details/9569451#)[?](http://blog.csdn.net/zemochen/article/details/9569451#)
- <h1>Header</h1>
**[html]**[view
 plain](http://blog.csdn.net/truong/article/details/17339907#)[copy](http://blog.csdn.net/truong/article/details/17339907#)[print](http://blog.csdn.net/truong/article/details/17339907#)[?](http://blog.csdn.net/truong/article/details/17339907#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/113568/fork)
- <h1>Header</h1>
WEB-INF/jsp/menu.jsp
**[html]**[view
 plain](http://blog.csdn.net/zemochen/article/details/9569451#)[copy](http://blog.csdn.net/zemochen/article/details/9569451#)[print](http://blog.csdn.net/zemochen/article/details/9569451#)[?](http://blog.csdn.net/zemochen/article/details/9569451#)
- <spanstyle="font-size:
 14px;"><p>Menu</p></span>
*WEB-INF/jsp/footer.jsp*
**[html]**[view
 plain](http://blog.csdn.net/zemochen/article/details/9569451#)[copy](http://blog.csdn.net/zemochen/article/details/9569451#)[print](http://blog.csdn.net/zemochen/article/details/9569451#)[?](http://blog.csdn.net/zemochen/article/details/9569451#)
- <spanstyle="font-size:
 14px;"><p>Copyright <codeclass="xml
 plain">©</code> Zemo.com</p></span>
所有工作准备就绪部署就行了
**![spring-tiles-demo-screen-contact-manager](http://img.viralpatel.net/2010/07/spring-tiles-demo-screen-contact-manager.png)**
