# 深入了解ApusicAS服务器配置系列之——配置Web上下文根 - z69183787的专栏 - CSDN博客
2013年11月12日 18:39:28[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1602
虽然AAS是最常见也是实施最多的产品，但是，实际中使用到的往往是最常见的一些配置内容，遇到某些特殊情况，往往有些不知所措。针对这一状况，决定将AAS的配置文件做一个较深入的研究，从而能在最大程度上了解AAS各种配置文件及其参数的实际意义。如果可能的话，本系列会慢慢持续，直至基本介绍完成所有的配置为止。
实际应用中，配置应用的上下文根是最常见也是看起来信手拈来的操作与配置（需求往往是设置默认应用，即不需要添加应用标识，直接可以访问应用）。但是，实际上工作中使用的配置往往是很多种方法中的一种，而且可能并不是最优的方法。
本文便以设置默认应用为例，说明每种配置的方法。
实际上，配置应用的上下文根，在AAS下，目前看来有两种方式：
- 首先，是配置apusic-application.xml文件
这种方式，印象中Weblogic也可以用，只是当时是8.1.x的版本，不知道升级到11g以后是不是还继续支持。
其实很简单，可在web应用(war包或目录)内建一个META-INF目录，此目录与WEB-INF目录同级，并将apusic-application.xml文件放置在其中，即可生效。至于apusic-application.xml文件的内容，可以参考AAS默认安装的default应用。具体内容，贴出来：
**[html]**[view
 plain](http://blog.csdn.net/cyq1984/article/details/6929615#)[copy](http://blog.csdn.net/cyq1984/article/details/6929615#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- 
- <!DOCTYPE apusic-application PUBLIC '-//Apusic//DTD Apusic Application 3.0//EN'  
-     'http://www.apusic.com/dtds/apusic-application_3_0.dtd'>
- 
- <apusic-application>
- <moduleuri="public_html">
- <web>
- <context-root>/</context-root>
- </web>
- </module>
- </apusic-application>
- 其次，为应用添加base-context属性
大家都知道，无论是AAS自己检测到的应用还是手工创建或者使用AAS管理工具创建的应用。都是在%APUSIC_DOMAIN_CONFIG_HOME%下的server.xml文件中自动加上应用的一些基本配置信息，一般情况下是：
**[html]**[view
 plain](http://blog.csdn.net/cyq1984/article/details/6929615#)[copy](http://blog.csdn.net/cyq1984/article/details/6929615#)
- <applicationname="appName"base="path"start="auto"/>
默认情况下，添加的应用属性是为了能够识别出应用并启动起来，但是，实际上相关的属性还是比较丰富的，本文提到的上下文根便是其中一个。此属性的名称为“base-context”，具体值可以根据实际需要设定。如果需要设置默认应用，则可以添加base-context="/"。
注意：由于AAS默认在安装时会添加一个默认的应用，因此，无论按照哪种方式设置的默认应用，都需要将AAS默认安装的“default”应用做相应调整。需要将default下的apusic-application.xml文件中的
**[html]**[view
 plain](http://blog.csdn.net/cyq1984/article/details/6929615#)[copy](http://blog.csdn.net/cyq1984/article/details/6929615#)
- <context-root>/</context-root>
做一些调整，比如改成
**[html]**[view
 plain](http://blog.csdn.net/cyq1984/article/details/6929615#)[copy](http://blog.csdn.net/cyq1984/article/details/6929615#)
- <context-root>/originalApp</context-root>
本文在ApusicAS 6.0及以上版本上试验通过。
本文引出AAS的一个隐藏问题，就是直接把应用扔在AAS的"APPLICATION_HOME"下识别出来的应用，应用名是什么的问题？其实，关于本问题，AAS的操作手册有非常详细的说明：
**如果base-context和context-root都未配置，那么apusic会按照以下规则指定web应用的上下文根路径：如果应用是ear模块，那么指定apusic-application.xml中该web module的uri值为根路径；如果应用是web模块，那么web应用的根路径为server.xml中指定的应用名，即"name"属性的值。如果上述值以".war"结尾，那么去除".war"后的值为context-root。**
