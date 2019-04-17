# Pomer  Flex J2EE (Flex+BlazeDS+Hibernate + spring )框架 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2014年05月29日 12:44:05[boonya](https://me.csdn.net/boonya)阅读数：1383








### 1.为什么使用POMER



Pomer是一 个开源的企业信息管理系统基础框架，它提供了一系列企业信息管理系统的基础功能，包括企业组织架构管理，角色权限管理，菜单管理，数据权限控制等功能。 Pomer提供了简洁高效的Flex应用程序开发框架，丰富实用的Flex控件以及常见的Flex应用模式，和一个基于Pomer框架的代码生成器，基于 Pomer，可以快速开发出优秀的管理信息系统，而不用太关注业务以外的技术细节。

![](http://www.oschina.net/uploads/img/201006/18113452_LFiq.jpg)

## Pomer 有哪些组件
- Flex 开发框架


Pomer 主要采用spring和hibernate做为后台框架，通过blazeds(LCDS)RemoteObject与java后台通信，前台采用 Cairngorm MVC框架， Pomer框架主要作用就是简化blazeds(LCDS)RemoteObject的访问方式，提高开发效率。Pomer通过注解 (Annotation)将普通java对象，Spring Bean和EJB发布为远程对象（RemoteObject）提供给flex访问，而不需要进行任何配置 。
- 代码生成器


Pomer 代码生成器包括两个部分，一个是强大灵活通用代码生成器模型和一套基于Pomer的默认模板。 Pomer通用代码生成器采用freemark做为模板引擎，可以生成任何基于数据表或javaPojo和java接口的代码，用户只需提供 freemark的模板即可。 Pomer提供的默认生成功能和默认模板可以通过数据库生成基于Pomer框架可运行的增删改查代码，也可以从javaBean生成 RemoteObject的数据传输对象、表单和列表，还能通过java接口生成Cairngorm的框架代码。
- Flex 组件


提供一系列高效用户交互组件，渲染极佳用户体验
- 组织架构管理


Pomer提供一个简单的企业组织架构管理。主要是为了演示FLEX和JAVA的各种应用模式。
- 安全管理


Pomer提供一个简单的通用安全框架。
- SAM 单点登录集成


基于行业标准的开源单点登录集成

Pomer Functional Architecture

![](http://www.oschina.net/uploads/img/201006/18113453_Pd9V.jpg)

## Pomer 能做什么
- Pomer是一个基于Flex & Java的基础开发框架，是经检验过的开发Flex应用程序的高效实践。使用Pomer开发Flex应用程序，您不必再为Flex与Java通信方式的选择、项目组织架构的建设、企业应用通用组件模块的开发而花费宝贵时间。
- Pomer可以做为企业信息管理系统的基础开发框架。

> 
Pomer已经完成了企业信息管理系统中大部分的常见功能，如组织机构管理，访问控制管理、菜单权限管理、数据安全管理等。基于Pomer，可以快速开发出企业信息管理系统。
- Pomer提供一组与SAM（单点登录）集成的接口，可以做为企业的安全管理中心。
- Pomer提供一套代码生成器，您能减少大量时间使用复制和粘贴建立样板代码，减少代码出错率。

> 
用 户使用Pomer代码生成器，可根据数据库快速生成基于Pomer项目框架的项目工程代码，您可以立即将此工程部署到符合Java EE规范的Web容器中并立即运行，此过程无需编写任何代码。生成的代码基于Flex、Struts2.x、Spring2.x和 Hibernate3.x。用户还可以自定义代码生成模板生成符合您业务需要的代码。
- Pomer提供丰富实用的Flex控件，实现高效RIA应用，极大增强了用户体验。
- Pomer提供一个简易高效的Flex应用程序开发框架，您可以快速搭建起符合企业应用的项目框架，它提供的实用程序包使得Flex与Java交互应用开发变得简洁高效。

## 如何使用 Pomer

## Pomer 名称的由来

Pomer 取自英文单词Pomeranian的缩写形式。Pomeranian的意思是波美拉尼亚犬或博美犬，它是狐狸犬家族中最小的犬种，其名出自原产地波美拉尼亚，在波兰西北部和德国东北部。


博 美是一种活跃的犬种，聪明勇敢并且是忠实的同伴。它们可能不适于与幼儿互动，因为它们的小体型可能会让它们被小孩虐待。它借着宏亮尖锐的吠声，通知闯入者 的到来，证明了它们为优秀的看守犬。它们能轻易的适应都市生活，并且就乡村生活而言，也是优秀的犬种，拥有野生祖先传下来的强烈狩猎本能。


博 美犬体型小，食量也小，但却十分的忠诚与机警，你无需花费过多时间照看它，它会很好的融入你的生活，带给你无穷的欢乐。博美犬短小精悍的优良特性赋予了 Pomer同样的特征，Pomer宣扬简单、快速、实用的原则，你无需花费过高的学习曲线，就能带来极大的产出。我们对博美犬的热爱铸就了Pomer（音 译博美）名称的由来（两位项目成员各养了一只白色的博美犬）。

This article from:[http://www.oschina.net/p/pomer/](http://www.oschina.net/p/pomer/)

### 2.POMER的下载与安装
- Pomer开发环境
- [MyEclipse6.5](http://downloads.myeclipseide.com/downloads/products/eworkbench/6.5.1GA/MyEclipse_6.5.1GA_E3.3.2_Installer.exe)
- [flex3.0插件版](http://daved.ims.uaf.edu/~cermak/whirled/windows/FB3_WWEJ_Plugin.exe)
- JDK1.6.0_05
- Mysql5.0
- Tomcat 6.0


Pomer在上叙环境开发，并测试通过，但不说明以上版本环境是必须的，更高或更低的版本，或其他IDE、数据库、J2EE容器都可运行，但或许会有些小的改动。


- 下载
[立即下载Pomer](http://code.google.com/p/pomer/wiki/help_doc_zh),源代码和war包：[http://code.google.com/p/pomer/wiki/downloads?tm=2](http://code.google.com/p/pomer/wiki/downloads?tm=2)
- 用户下载Pomer和Pomer代码生成器源代码，也可以下载war文件运行体验
- 安装
- 

- 运行War，体验Pomer效果

     1、在MySql上创建数据库pomer

     2、运行建表脚本%pomer%\doc\db\mysql_createSQL.sql

     3、 修改pomer\WEB-INF\classes\config\jdbc.properties属性文件

*复制内容到剪贴板*
##### 代码:

```
hibernate.connection.url=jdbc:mysql://127.0.0.1:3306/pomer

hibernate.connection.username=root

hibernate.connection.password=111111
```




     4、启动Tomcat

     5、[http://localhost:8080/pomer/ui/PomerUserIndex.html](http://localhost:8080/pomer/ui/PomerUserIndex.html)

     6、Pomer 用户管理模块运行效果图

![](http://lh4.ggpht.com/yulinlincom/SOGFRdmtH5I/AAAAAAAAAcA/OZkaarZMgFU/s576/pomerUserIndex.JPG)


- 

- 导入源代码

请确认已经在MyEclipse中安装Flex插件，并运行正常。

Pomer是一个基于MyEclipse的Web项目，用户只需解压后导入即可

1、下载Pomer和PomerGenerator

2、解压缩到各自文件夹

3、导入到Myeclipse中

启动MyEclipse->File->Inport如图一

![](http://lh3.ggpht.com/yulinlincom/SOGV6NfRD_I/AAAAAAAAAcI/8UKiqtoN-U0/s640/eclipse_inport_1.JPG)



［图一］



![](http://lh6.ggpht.com/yulinlincom/SOGV6YcjNAI/AAAAAAAAAcQ/ysR6PQr8wSU/s400/eclipse_inport_2.JPG)



［图二］



![](http://lh3.ggpht.com/yulinlincom/SOGV6VBIr-I/AAAAAAAAAcY/16i_UBJK0-c/s400/eclipse_inport_3.JPG)



［图三］



![](http://lh3.ggpht.com/yulinlincom/SOGV6tlXaMI/AAAAAAAAAco/VwDLfWKCShU/s640/setting_0.JPG)



［图四］


导入完成，等待系统编译完成，如纺译完成后，只出现图四那样的一个错误“Target runtime Apache Tomcat v6.0 is not defined”，说明导入成功，运行环境配置成功。这个错误应该是Adobe Flex的一个设计问题，不影响运行。

4、检查和修改配置环境

![](http://lh4.ggpht.com/yulinlincom/SOGWJ3vvlxI/AAAAAAAAAcw/rvC_u5CRwkw/s576/setting_2.JPG)



［图五］



![](http://lh5.ggpht.com/yulinlincom/SOGWJ-0wbxI/AAAAAAAAAc4/ZxHEBZyzYKE/s576/setting_3.JPG)



［图六］



![](http://lh3.ggpht.com/yulinlincom/SOGWKDnZT7I/AAAAAAAAAdA/jQgAHf1-pXg/s576/setting_4.JPG)



［图七］



![](http://lh5.ggpht.com/yulinlincom/SOGWKKULM-I/AAAAAAAAAdI/lNXaeGoZvoY/s512/setting_5.JPG)



［图八］



    这个修改非常重要，也是adobe flex的一个恶心设计，services-config.xml的路径指定一定要正确。

![](http://lh3.ggpht.com/yulinlincom/SOGWKdc4TAI/AAAAAAAAAdQ/ujMnAg0rLhQ/s576/setting_6.JPG)



［图九］




5、运行PomerUserIndex

      右击flex_src\PomerUserIndex.mxml->Run as->Flex Application,如果出现如下错误提示选Procced

![](http://lh5.ggpht.com/yulinlincom/SOGblAIDCjI/AAAAAAAAAdY/qovrKhq37ds/setting_8.JPG)



［图十］


      如正常运行，刚出现如下图，如有错误请重新配置

![](http://lh4.ggpht.com/yulinlincom/SOGFRdmtH5I/AAAAAAAAAcA/OZkaarZMgFU/s576/pomerUserIndex.JPG)

- 

This article from: [http://vissul.iteye.com/blog/353663](http://vissul.iteye.com/blog/353663)





