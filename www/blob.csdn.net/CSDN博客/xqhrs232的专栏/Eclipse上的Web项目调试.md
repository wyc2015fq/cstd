# Eclipse上的Web项目调试 - xqhrs232的专栏 - CSDN博客
2018年11月05日 14:12:22[xqhrs232](https://me.csdn.net/xqhrs232)阅读数：97
原文地址::[https://blog.csdn.net/boral_li/article/details/5995651](https://blog.csdn.net/boral_li/article/details/5995651)
相关文章
1、eclipse调试web项目----[https://blog.csdn.net/zhao1949/article/details/54709958](https://blog.csdn.net/zhao1949/article/details/54709958)
2、[在eclipse中配置server和database](https://www.cnblogs.com/qrlozte/p/3175170.html)----[http://www.cnblogs.com/qrlozte/p/3175170.html](http://www.cnblogs.com/qrlozte/p/3175170.html)
3、Debug-如何在Eclipse中调试JAVA Web程序----[https://blog.csdn.net/wo_shi_LTB/article/details/79251975](https://blog.csdn.net/wo_shi_LTB/article/details/79251975)
 在Eclipse中开发Web项目的首要难题就是如何进行代码调试。本文简要说明一下在Eclipse中使用Tomcat和Jetty调试Java Web项目的方法。
     Tomcat插件方式。Eclipse自身可以通过下载安装Tomcat插件的方式进行调试，由于插件中集成了tomcat的运行环境，所以不需要独立安装Tomcat服务器。该插件网址http://www.eclipsetotale.com/tomcatPlugin.html，插件下载地址：http://www.eclipsetotale.com/tomcatPlugin/tomcatPluginV321.zip，下载以后以普通插件的方式安装到Eclipse中(拷贝方式或link方式)即可。但这种方式的调试环境和真实运行环境还是有一些差别，而且运行目录缺省是在Eclipse的插件目录中，运行环境不太直观，所以这里不推荐此方式。
    Tomcat Server方式。首先下载安装Tomcat(地址http://tomcat.apache.org/)。然后打开Eclipse，在Eclipse中添加Tomcat Server。步骤如下：
    Window->Preferences->Server->Runtime Environments->Add->选择Apache Tomcat v6.0->next->选择Tomcat的安装目录->finish。
当Tomcat被添加到Eclipse的Server以后，Eclipse会自动创建一个名称为“Servers”的项目，其中包含了context.xml、server.xml、tomcat-users.xml、web.xml等基本配置文件。下面开始配置web项目调试选项：
    在web项目上右键->Debug As->Debug Configurations->在Apache Tomcat上右键->New,创建调试器，然后使用缺省配置选项即可。接下来就可以在项目上右键->Debug On Server开始调试web项目了。
   Jetty Server方式。该配置与Tomcat大同小异。首先还是下载安装Jetty(地址:http://jetty.codehaus.org/jetty/)。然后打开Eclipse，在Eclipse中添加Jetty Server。步骤如下：
    Window->Preferences->Server->Runtime Environments->Add->选择Jetty 6->next->选择Jetty的安装目录->next->设置端口->finish。
    下面开始配置web项目调试选项：
    在web项目上右键->Debug As->Debug Configurations->在Generic Server上右键->New,创建调试器，然后使用缺省配置选项即可。(配置信息中的jre一般选择缺省jre)。接下来就可以在项目上右键->Debug On Server开始调试web项目了。
    Jetty调试多个web项目。当项目比较庞大时，经常会把web项目分解成多个子web项目，分别开发，然后聚合成一个完整的web系统。由于子web项目不能独立运行，这样分解开发带来的一个问题是开发过程中的调试问题，Jetty支持以指定的配置文件启动web服务器，并且支持多文件夹聚合web站点，因此可以配置Jetty来解决这个问题，在调试时Jetty把分布于不同位置的各个子项目在逻辑上自动合并成一个完整项目。配置方法：
   首先创建包含Jetty包的User Library：Window->Preferences->User Libraries->New(输入名称Jetty)->Add Jars->在Jetty的安装目录下选择要包含的Jetty包，这些jar包包括：{Jetty目录}/lib/ext/*.jar;：{Jetty目录}/lib/jsp-2.1/*.jar。
   在项目分解时，web项目被分解为一个主Java Web项目和若干个普通Java项目，其中的主Web项目用来做配合文件维护管理和启动项目。
   1.在主web项目上点击右键，选择Debug As->Debug Configurations->在Java Application上右键->New， 然后开始配置该调试器：
   2.在选项卡“Main”中，设置Project选项为主Web项目；Main Class设置为org.mortbay.xml.XmlConfiguration；
   3.在“Arguments”选项卡中，设置Program arguments为${workspace_loc:xxxx}/jetty.xml，其中的xxxx为主web项目名称，Working Directory中的“other”选项设置为${workspace_loc}；
   4.Classpath选项卡，Bootstrap Entries中首先添加上面创建的包含Jetty的jar包的User Library，然后添加项目所用到的其它User Library，User Entries中添加主web项目和其他所有的子java项目；
   5.在Source中添加需要调试的源码项目；
   6.以上配置完成以后，开始配置Jetty启动配置文件jetty.xml，以上的设置“${workspace_loc:xxxx}/jetty.xml”是把jetty.xml置于主web项目的根目录下。该文件可以从Jetty安装目录中的ext/jetty.xml文件夹中拷贝，略作修改即可。可以在jetty.xml中配置各个子项目中web资源文件的存储路径，也可以在WEB-INF中新建一个jetty-web.xml文件来单独配置子项目中的web资源。示例jetty-web.xml：
<?xml version="1.0"  encoding="ISO-8859-1"?>
<!DOCTYPE Configure PUBLIC "-//Mort Bay Consulting//DTD Configure//EN" "http://jetty.mortbay.org/configure.dtd">
<Configure class="org.mortbay.jetty.webapp.WebAppContext">
<Set name="baseResource"><New class="org.mortbay.resource.ResourceCollection">
      <Arg>
        <Array type="java.lang.String">    
          <Item>MainProject/WebContent</Item>     
          <Item>SubProject1/WebContent</Item> 
          <Item>SubProject2/WebContent</Item>  
         </Array>
      </Arg>
    </New>
  </Set>
  <Set name="contextPath">/vpath</Set>
</Configure>
   web项目一般包含jar引用包、class文件、web资源文件(jsp、js、css、图片等)，jar文件是在Classpath选项卡的Bootstrap Entries中进行配置；class文件是在Classpath选项卡中的User Entries进行配置；调试时的java源码文件是在Source选项卡中配置；web资源文件是在jetty-web.xml文件中进行配置。这样Jetty即可根据这些配置选项启动完整的web项目了。
