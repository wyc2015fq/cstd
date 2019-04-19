# Java web的三种打包方式：jar、war、ear - xqhrs232的专栏 - CSDN博客
2018年09月29日 14:31:16[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：57
个人分类：[JavaWeb技术](https://blog.csdn.net/xqhrs232/article/category/8231541)
原文地址::[https://blog.csdn.net/qq_28379809/article/details/77075076](https://blog.csdn.net/qq_28379809/article/details/77075076)
转载自：[http://www.jb51.net/article/35881.htm](http://www.jb51.net/article/35881.htm)
  以最终客户的角度来看，JAR文件就是一种封装，他们不需要知道jar文件中有多少个.class文件，每个文件中的功能与作用，同样可以得到他们希望的结果。除jar以外对于J2EE来说还有war和ear。区别见下表：
||JAR|WAR|EAR|
|----|----|----|----|
|**英文**|Java Archive file|Web Archive file|Enterprise Archive file|
|**包含内容**|class、properties文件，是文件封装的最小单元；包含Java类的普通库、资源（resources）、辅助文件（auxiliary files）等|Servlet、JSP页面、JSP标记库、JAR库文件、HTML/XML文档和其他公用资源文件，如图片、音频文件等|除了包含JAR、WAR以外，还包括EJB组件|
|**部署文件**|application-client.xml|web.xml|application.xml|
|**容器**|应用服务器（application servers）|小型服务程序容器（servlet containers）|EJB容器（EJB containers）|
|**级别**|小|中|大|
**(1)EAR文件的打包**
EAR文件包括整个项目，内含多个 EJB Module（JAR文件）和 Web Module（WAR文件）。
EAR文件的生成可以使用winrar以zip压缩方式或者从命令行以jar命令来生成。
步骤：
1>、先打包成war和jar，并写好application.xml，放到 META-INF 目录下。
2>、运行 jar cf your_application.ear your_war.war your_jar.jar META-INF\application.xml （假设都在当前目录下）。
当然也可以用jar xf your_application.ear来解压。
application.xml是用来描述你的ear中包括的war和jar
weblogic自带的petstore的application.xml例子：
复制代码代码如下:
<!DOCTYPE application PUBLIC '-//Sun Microsystems, Inc.//DTD J2EE Application 1.2//EN' 'http://java.sun.com/j2ee/dtds/application_1_2.dtd'>  
 <application>  
     <display-name>estore</display-name>  
     <description>Application description</description>  
     <module>  
         <ejb>customerEjb.jar</ejb>  
     </module>  
     <module>  
         <ejb>mailerEjb.jar</ejb>  
     </module>  
     <module>  
         <web>  
             <web-uri>petstore.war</web-uri>  
             <context-root>estore</context-root>  
         </web>  
     </module>  
     <module>  
         <ejb>petstoreEjb.jar</ejb>  
     </module>  
     <module>  
         <ejb>signonEjb.jar</ejb>  
     </module>  
     <module>  
         <ejb>personalizationEjb.jar</ejb>  
     </module>  
     <module>  
         <ejb>inventoryEjb.jar</ejb>  
     </module>  
     <module>  
         <ejb>shoppingcartEjb.jar</ejb>  
     </module>  
     <security-role>  
         <description>the gold customer role</description>  
         <role-name>gold_customer</role-name>  
     </security-role>  
     <security-role>  
         <description>the customer role</description>  
         <role-name>customer</role-name>  
     </security-role>  
     <security-role>  
         <role-name>administrator</role-name>  
     </security-role>  
 </application> 
(2)WAR的使用
1>、生成war文件使用如下命令：jar  -cvf   web1.war *
2>、查看web1.war中都有哪些文件，可以使用命令：jar  -tf  web1.war
3>、解压web1.war文件，可以使用命令：jar  -xvf  web1.war
另外，也可使用winrar软件选择zip压缩方式，并将压缩文件后缀名改为war即可压缩生成war文件；同样使用winrar软件可以强行打开war 文件，或者强行解压war文件
使用jar命令与winrar软件的区别在于前者在压缩文件的同时会生成 META-INF 文件夹，内包含MANIFEST.MF文件。
**(3)何时使用war或者jar文件**
当你的项目在没有完全竣工的时候，不适合使用war文件，因为你的类会由于调试之类的经常改，这样来回删除、创建war文件很不爽，最好是你的项目已经完成了，不改了，那么就打个war包吧，这个时候一个war文件就相当于一个web应用程序；而jar文件就是把类和一些相关的资源封装到一个包中，便于程序中引用。
+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
**Java的打包jar、war、ear包的作用、区别、打包方式：**
**1、 作用与区别**
i.  jar：通常是开发时要引用通用(JAVA)类，打成包便于存放管理；
ii. war：是做好一个(web)应用后，通常是网站，打成包部署到容器中；
iii. ear： 企业级应用，实际上EAR包中包含WAR包和几个企业级项目的配置文件而已，一般服务器选择WebSphere等，都会使用EAR包。通常是EJB打成ear包。
**2、打包方式**
i. 所有的包都是用jar打的，只不过目标文件的扩展名不一样；
ii. 也可以用Ant来安成构建。
**3、JET编译成EXE**
i.  JET 是要用钱买的，而且据说 JET 也不是能把所有的 Java 程序都编译成执行文件，性能也要打些折扣。所以，使用制作可执行 JAR 文件包的方法就是最佳选择了，何况它还能保持 Java 的跨平台特性。
**注意：**
将web项目以war的方式导出后，直接放在tomcat容器的webapps下 ，启动服务，即可运行该项目，该war包会自动解压出一个同名的文件夹。
--------------------- 本文来自 风与其火 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/qq_28379809/article/details/77075076?utm_source=copy
