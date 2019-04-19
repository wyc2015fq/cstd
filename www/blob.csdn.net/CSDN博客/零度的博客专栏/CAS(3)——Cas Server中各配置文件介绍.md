# CAS(3)——Cas Server中各配置文件介绍 - 零度的博客专栏 - CSDN博客
2016年05月25日 13:39:42[零度anngle](https://me.csdn.net/zmx729618)阅读数：1332
                
Cas Server中所有的配置文件都是放在WEB-INF目录及其子目录下的。
       在WEB-INF/classes下的配置文件有：
l  **cas-theme-default.properties**：该文件存放的是css文件的路径，用户可以在这里指定使用的css文件为自定义的css文件，也可以更改配置文件中对应的css文件已更改默认的样式。
l  **系列messages_*.properties**：国际化支持对应的资源文件。
l  **default_views.properties和protocol_views.properties**：Cas Server中使用到的视图配置文件，用于定义对应视图的处理类及其文件路径。
l  **log4j.xml**：log4j的配置文件。
       在WEB-INF根目录下的配置文件有：
l  **web.xml**：这个文件就是大伙熟悉的web.xml文件。
l  **cas-servlet.xml**：该文件是Cas Server使用到的[spring](http://lib.csdn.net/base/17) MVC的配置文件，里面定义了一些Controller及View相关的内容。
l  **cas.properties**：该文件是Cas Server配置的主要文件。与Spring相关的所有配置使用的外部属性都定义在这个文件中。
l  **deployerConfigContext.xml**：该文件是专为Cas Server部署人员抽取出来的一个文件，其中的内容通常都是Cas Server部署人员需要进行更改的内容。
l  **login-webflow.xml**：登录使用到的Spring Web Flow配置文件。
l  **restlet-servlet.xml**：使用到的Restful API的相关配置。
l  **WEB-INF/spring-configuration**：该目录中存放的是Cas Server加载的Spring相关的配置文件。
l  **WEB-INF/unused-spring-configuration**：该目录中存放的是没有用到，但是有需要时可以将它们copy到spring-configuration目录下的文件。
     在WEB-INF/spring-configuration目录下的配置文件是Cas Server中使用到的所有与Spring相关的配置文件，具体有：
l  **applicationContext.xml**：该文件中定义了主要使用的bean，通常不需要修改。
l  **argumentExtractorsConfiguration.xml**：该文件中定义了需要使用的协议，默认只有CAS和SAML是可用的。
l  **log4jConfiguration.xml**：log4j初始化对应的配置。
l  **propertyFileConfigure.xml**：用于指定外部属性文件的位置，默认指定的就是前面提到的cas.properties文件。
l  **securityContext.xml**：这是Cas Server中使用到的Spring Security相关内容的配置文件，默认不需要进行修改。
l  **ticketExpirationPolicies.xml**：用于指定TicketGranting Ticket和Service Ticket的过期策略，一般不需要修改。
l  **ticketGrantingTicketCookieGenerator.xml**：TicketGranting Ticket对应的CookieGenerator的配置。
l  **ticketRegistry.xml**：DefaultTicketRegistry对应的配置。
l  **uniqueIdGenerators.xml**：唯一主键生成相关的配置。
l  **warnCookieGenerator.xml**：用于生成警告的CookieGenerator对应配置。
原文地址：[http://haohaoxuexi.iteye.com/blog/2128865](http://haohaoxuexi.iteye.com/blog/2128865)
