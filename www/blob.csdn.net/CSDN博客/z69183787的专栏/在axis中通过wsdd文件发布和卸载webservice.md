# 在axis中通过wsdd文件发布和卸载webservice - z69183787的专栏 - CSDN博客
2014年07月14日 09:36:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1249
**用Axis开发基于Java的Web服务****概述**
本文介绍了一个比较简单实用的基于Java的SOAP引擎——Apache组织推出的Axis——的安装、配置和应用开发方法，希望对有兴趣做基于Java的Web Services应用开发的朋友有所帮助。 
Axis简介 
　　Axis是Apache组织推出的SOAP引擎，Axis项目是Apache组织著名的SOAP项目的后继项目，目前最新版本是采用Java开发的1.1版本，C++的版本正在开发之中。Axis v1.1软件包可以从[http://ws.apache.org/axis/dist/1_1/](http://ws.apache.org/axis/dist/1_1/)下载得到。 
Axis的安装 
　  应用Axis开发Web Services，你需要有一个支持Servlet的服务器引擎，比如广为人知的Tomcat（Tomcat也是由Apache组织所提供的，目前的最新版本为4.1.24，下载地址为[http://jakarta.apache.org/builds/jakarta-tomcat-4.0/release/v4.1.24/](http://jakarta.apache.org/builds/jakarta-tomcat-4.0/release/v4.1.24/)。在此之前当然你机器必须已经装好了JDK1.3以上的版本，当安装好Tomcat之后，只需将下载的Axis软件包解压缩，将其中的“webapps”目录下的“axis”目录整个拷贝到Tomcat安装目录下的“web
 apps”目录下即可。 
Axis的配置 
　　Axis基于Java开发，可以部署于多种操作系统，使用前需要配置一系列的系统变量，在此假定你已经在本机上装好了Tomcat 4.0以上的版本，需要配置的系统变量如下表所示：
CATALINA_HOME 
C:\Tomcat_4_1 （此处应为Tomcat的安装位置，注意路径名中不要有空格） 
AXIS_HOME 
%CATALINA_HOME%\webapps\axis 
AXIS_LIB 
%AXIS_HOME%\lib 
AXISCLASSPATH 
%AXIS_LIB%\axis.jar;%AXIS_LIB%\commons-discovery.jar; %AXIS_LIB%\commons-logging.jar;%AXIS_LIB%\jaxrpc.jar; %AXIS_LIB%\saaj.jar;%AXIS_LIB%\log4j-1.2.8.jar; %AXIS_LIB%\xml-apis.jar;%AXIS_LIB%\xercesImpl.jar 
　　如果你不愿做这么繁琐的类路径设置，可以直接将axis软件包中“lib”目录下的所有.jar文件加入到系统环境变量CLASSPATH中即可。 
Axis的测试 
　　安装配置完毕后，应测试一下是否Axis可以正确运行了。启动Tomcat服务器，在浏览器中访问[http://localhost:8080/axis/happyaxis.jsp](http://localhost:8080/axis/happyaxis.jsp)，如果页面显示有错误，则需要回头检查一下相关配置是否正确，如果浏览页面能正确显示出系 
统组件、属性等参数配置信息，则表示安装成功，如下图所示： 
![](http://images.cnblogs.com/cnblogs_com/mingzi/60714.jpg)
服务的发布 
　　Axis提供了两种服务发布方式，一种是即时发布(Instant Deployment)，一种是定制发布(Custom Deployment)。 
使用即时发布 
　　对即时发布的支持是Axis的特色之一，使用即时发布使用户只需有提供服务的Java类的源代码，即可将其迅速发布成Web服务。每当用户调用这类服务的时候，Axis会自动进行编译，即使服务器重启了也不必对其做任何处理，使用非常简单快捷。使用即时发布首先需要一个实现服务功能的Java源文件，将其扩展名改为.jws（Java Web Service的缩写），然后将该文件放到“……\webapps\axis”目录下即可。在此给出一个从英里到公里的长度单位转换的服务，其源码如下： 
文件Distance.jws 
/**
* @author 飞鹰
*/
**public **class Distance
{
**public ****double **convertMile2Kilometre( **double **mile )
{
//实现英里到公里的距离转换
**return **mile * 1.609;
}//convertMile2Kilometre()
}/* Distance */
　　将其放到“……\webapps\axis”目录，通过访问[http://localhost:8080/axis/Distance.jws?wsdl](http://localhost:8080/axis/Distance.jws?wsdl)可以看到这个服务的WSDL描述文件，这说明Distance服务被成功发布了。如下图所示：
![](http://images.cnblogs.com/cnblogs_com/mingzi/6071.jpg)
使用定制发布 
　　即时发布是一项令人激动的技术，它使Web服务的开发变得如此简单；然而即时发布并不总是最好的选择，比如有些应用系统是第三方提供的，我们没有购买源代码，只有.class文件，但我们又希望将这个应用系统的一些功能对外发布成Web服务，使其能够在更大范围内产生作用，这个时候即时发布技术就无能为力了。此外，即时发布技术并不灵活，无法进行更多的服务配置，这使得它并不能满足一些特定系统的需求。因此，Axis提供了另一种服务发布方式，这就是定制发布。在此给出一个从加仑到升的容积单位转换的服务，其源码如下： 
文件Capacity.java
**package samples.capacity;/*** @author 飞鹰*/public class Capacity{public double convertGallon2Litre( double gallon ){//实现加仑到升的容积转换return gallon * 4.546;}//convertGallon2Litre()}/* Capacity */**
　　将其编译成.class文件，放置到“……\webapps\axis\samples\capacity”目录下，即可着手进行发布。 
　　定制发布不需要构建.jws文件，但必须创建服务发布描述文件deploy.wsdd（Web Service Deployment Descriptor的缩写），这个文件负责描述服务的名称、入口等信息，其内容如下： 
文件deploy.wsdd 
<deployment xmlns="http://xml.apache.org/axis/wsdd/"
xmlns:java="http://xml.apache.org/axis/wsdd/providers/java">
<service name="Capacity" provider="java:RPC">
<parameter name="className" value="samples.capacity.Capacity"/>
<parameter name="allowedMethods" value="*"/>
</service>
</deployment>
将该文件也放到“……\webapps\axis\samples\capacity”目录下，然后可以采用Axis提供的一个客户端管理工具——AdminClient来完成服务的定制发布。在“……\webapps\axis\samples\capacity”目录下，运行： 
java -cp %AXISCLASSPATH% org.apache.axis.client.AdminClient deploy.wsdd
可以看到以下运行结果： 
Processing file deploy.wsdd 
<Admin>Doneprocessing</Admin> 
这表明Capacity服务定制发布完成。 通过访问[http://localhost:8080/axis/services/Capacity?wsdl](http://localhost:8080/axis/services/Capacity?wsdl)可以看到这个服务的WSDL描述文件，这说明Capacity服务被成功发布了。如下图所示： 
![](http://images.cnblogs.com/cnblogs_com/mingzi/60716.jpg)
服务的访问 
GET方式的服务访问 
一般的SOAP消息都是采用POST方式实现传递，但也可以通过GET方式来访问。比如以下给出的一个服务——“HelloWorld”，其源码如下： 
文件HelloWorld.jws 
**public class HelloWorld{public String helloWorld(){//在服务器端打印输出System.out.println( "Hello World!" );//返回相应字符串return "Hello World!";}//helloWorld()}/* HelloWorld */**
这个服务给出一个名位“helloWorld”的无入口参数的操作，返回一个内容为“Hello World！的字符串”，同时在服务器端打印“Hello World！”，将该文件放到“……\webapps\axis”目录下，即可通过GET方法直接访问该服务，访问的地址为[http://localhost:8080/axis/HelloWorld.jws?method=helloWorld](http://localhost:8080/axis/HelloWorld.jws?method=helloWorld)，可以看到返回的SOAP信封消息，同时服务器端给出了相应的显示信息“Hello
 World！”这表明HelloWorld服务被成功访问了。如下图所示： 
![](http://images.cnblogs.com/cnblogs_com/mingzi/60717.gif)
客户端服务访问编程 
Axis提供了一套API来实现SOAP，从[http://localhost:8080/axis/docs/apiDocs/index.html](http://localhost:8080/axis/docs/apiDocs/index.html)可以看到Axis的API文档。 
其中，org.apache.axis.client.Call和org.apache.axis.client.Service是两个比较常用的类，一般的客户端程序欲访问一个Web Service时，都要生成一个客户端的Service对象和Call对象，在访问服务之前，首先要对Call对象设置相应的参数，包括服务的位置、操作名、入口参数、返回值类型等，最后调用Call对象的invoke方法访问服务。 
以下给出了一个客户端访问Web服务的例程——AXISTest.java： 
文件AXISTest.java 
package axisexercise;
import org.apache.axis.client.Call;
import org.apache.axis.client.Service;
import org.apache.axis.encoding.XMLType;
import javax.xml.rpc.ParameterMode;
/**
* @author 飞鹰
*/
public class AXISTest
{
public static void main( String[] args ) throws Exception
{
Service service = new Service();
Call call = ( Call ) service.createCall();
//////////访问即时发布的Distance服务
//设置访问点
call.setTargetEndpointAddress( "http://localhost:8080/axis/Distance.jws" );
//设置操作名
call.setOperationName( "convertMile2Kilometre" );
//设置入口参数
call.addParameter( "op1", XMLType.XSD_DOUBLE, ParameterMode.IN );
call.setReturnType( XMLType.XSD_double );
double d1 = new Double( 190 );
//调用服务
System.out.println( d1 + " 英里相当于 " + 
call.invoke( new Object[] {d1} ) + " 公里！" );
//////////访问定制发布的Capacity服务
call = ( Call ) service.createCall();
//设置访问点
call.setTargetEndpointAddress( "http://localhost:8080/axis/services/Capacity" );
//设置操作名
call.setOperationName( "convertGallon2Litre" );
//设置入口参数
call.addParameter( "op1", XMLType.XSD_DOUBLE, ParameterMode.IN );
call.setReturnType( XMLType.XSD_double );
d1 = new Double( 10.00 );
//调用服务
System.out.println( d1 + " 加仑相当于 " + 
call.invoke( new Object[] {d1} ) + " 升！" );
} //main()
}/* AXISTest */
编译运行后运行可以看到以下的结果：
　　190.0英里相当于305.71公里！
　　10.0加仑相当于45.46升！
注意程序在访问即时发布的Distance服务和定制发布的Capacity服务时的不同，前者的服务访问点地址为 http://localhost:8080/axis/HelloWorld.jws，而后者的则为http://localhost:8080 /axis/services/Capacity。
　　总结
　　Web  Services是未来网络应用的发展方向，SOAP和WSDL是Web  Services的核心协议，Axis给出了一个很好的SOAP实现，它使得开发Web  Services应用变得轻松而有趣。
=====================================
在axis中通过wsdd文件发布和卸载webservice
在上一篇<>中我介绍了一种简单webservice的发布方式-jws。
这种发布方式非常的敏捷！但有它的局限性—你必须知道webservice的源代码。有时候这个的确很让人郁闷！比如：的webservice被打成了一个jar包，你并没有源代码。或者这个webservice根本就不是你的公司开发的。这时你发布这个webservice就会变得很困难。axis针对这种情况提供了wsdd文件的方式来发布和撤销webservice。
现在我们来通过一个wsdd文件来对wsdd有个初步的认识。注意：在开始下面的步骤前请安装axis.具体安装请看上一篇
<deployment xmlns="http://xml.apache.org/axis/wsdd/" 
 xmlns:java="http://xml.apache.org/axis/wsdd/providers/java"> 
 <service name="MyService" provider="java:RPC"> 
 <parameter name="className" value="samples.userguide.example3.MyService"/> 
 <parameter name="allowedMethods" value="*"/> 
 </service> 
</deployment>
最外面的元素告诉我们这是一个wsdd发布项，并且定义了java的命名空间，service 元素定义了我们的service.java:RPC定义了axis的provider。parameter name定义了webservice类名及所在包的位置。allowedMethods value等于*，表示MyService的所有方法都可以被调用。
了解了wsdd文件，我们来认识一个非常有用的工具 AdminClient.正式通过它我们才能通过wsdd文件发布我们的应用。它的全路径是 org.apache.axis.client.AdminClient，在axis.jar包中。在使用这个工具前我们必须把相应的jar包加入 classpath。如：axis.jar..。如果你做了<>的练习则这一步就不用再做了。
我们首先进入webservice所在源文件的目录找到deploy.wsdd文件。然后键入如下命令：
java org.apache.axis.client.AdminClient deploy.wsdd
（如果提示错误，请检查你是否将必须的jar包加入classpath）
如果成功则会显示 ： 
<Admin>Done processing</Admin>
下面来解释AdminClient 所做的工作。AdminClient 首先会在axis.jar所在应用下的WEB-INF文件夹下创建一个webservice配置文件server-config.wsdd，并且根据 wsdd文件的配置将我们的webservice加入到这个配置文件中。如果server-config.wsdd已经存在则不会创建，会修改这个文件将新的service 配置到server-config.wsdd文件中。
<xml version="1.0" encoding="UTF-8"?> 
<deployment xmlns="http://xml.apache.org/axis/wsdd/" xmlns:java="http://xml.apache.org/axis/wsdd/providers/java"> 
 <globalConfiguration> 
 <parameter name="sendMultiRefs" value="true"/> 
 <parameter name="disablePrettyXML" value="true"/> 
 <parameter name="adminPassword" value="admin"/> 
 <parameter name="attachments.Directory" value="C:\tomcat5\webapps\axis\WEB-INF\attachments"/> 
 <parameter name="dotNetSoapEncFix" value="true"/> 
 <parameter name="enableNamespacePrefixOptimization" value="false"/> 
 <parameter name="sendXMLDeclaration" value="true"/> 
 <parameter name="sendXsiTypes" value="true"/> 
 <parameter name="attachments.implementation" value="org.apache.axis.attachments.AttachmentsImpl"/> 
 <requestFlow> 
 <handler type="java:org.apache.axis.handlers.JWSHandler"> 
 <parameter name="scope" value="session"/> 
 </handler> 
 <handler type="java:org.apache.axis.handlers.JWSHandler"> 
 <parameter name="scope" value="request"/> 
 <parameter name="extension" value=".jwr"/> 
 </handler> 
 </requestFlow> 
 </globalConfiguration> 
 <handler name="LocalResponder" type="java:org.apache.axis.transport.local.LocalResponder"/> 
 <handler name="URLMapper" type="java:org.apache.axis.handlers.http.URLMapper"/> 
 <handler name="Authenticate" type="java:org.apache.axis.handlers.SimpleAuthenticationHandler"/> 
 <service name="AdminService" provider="java:MSG"> 
 <parameter name="allowedMethods" value="AdminService"/> 
 <parameter name="enableRemoteAdmin" value="false"/> 
 <parameter name="className" value="org.apache.axis.utils.Admin"/> 
 <namespace>http://xml.apache.org/axis/wsdd/namespace> 
 </service> 
 <service name="Version" provider="java:RPC"> 
 <parameter name="allowedMethods" value="getVersion"/> 
 <parameter name="className" value="org.apache.axis.Version"/> 
 </service> 
 <service name="MyService" provider="java:RPC"> 
 <parameter name="allowedMethods" value="*"/> 
 <parameter name="className" value="samples.userguide.example3.MyService"/> 
 </service> 
 <transport name="http"> 
 <requestFlow> 
 <handler type="URLMapper"/> 
 <handler type="java:org.apache.axis.handlers.http.HTTPAuthHandler"/> 
 </requestFlow> 
 <parameter name="qs:list" value="org.apache.axis.transport.http.QSListHandler"/> 
 <parameter name="qs:wsdl" value="org.apache.axis.transport.http.QSWSDLHandler"/> 
 <parameter name="qs.list" value="org.apache.axis.transport.http.QSListHandler"/> 
 <parameter name="qs.method" value="org.apache.axis.transport.http.QSMethodHandler"/> 
 <parameter name="qs:method" value="org.apache.axis.transport.http.QSMethodHandler"/> 
 <parameter name="qs.wsdl" value="org.apache.axis.transport.http.QSWSDLHandler"/> 
 </transport> 
 <transport name="local"> 
 <responseFlow> 
 <handler type="LocalResponder"/> 
 </responseFlow> 
 </transport> 
</deployment>
我们来访问一下看看：
java samples.userguide.example3.Client -lhttp://localhost:8080/axis/services/MyService "test me!"
You typed : test me! 
返回正确，部署成功！
如果你要卸载一个webservice也很简单！我们首先定义一个undeploy.wsdd
<undeployment xmlns="http://xml.apache.org/axis/wsdd/"> 
 <service name="MyService"/> 
</undeployment>
进入webservice所在源文件的目录找到undeploy.wsdd文件。然后键入如下命令：
java org.apache.axis.client.AdminClient undeploy.wsdd
（如果提示错误，请检查你是否将必须的jar包加入classpath） 
如果成功则会显示 ： 
<Admin>Done processing</Admin>
在看server-config.wsdd已经删除了原来service的定义
<?xml version="1.0" encoding="UTF-8"?> 
<deployment xmlns="http://xml.apache.org/axis/wsdd/" xmlns:java="http://xml.apache.org/axis/wsdd/providers/java"> 
 <globalConfiguration> 
 <parameter name="sendMultiRefs" value="true"/> 
 <parameter name="disablePrettyXML" value="true"/> 
 <parameter name="adminPassword" value="admin"/> 
 <parameter name="attachments.Directory" value="C:\tomcat5\webapps\axis\WEB-INF\attachments"/> 
 <parameter name="dotNetSoapEncFix" value="true"/> 
 <parameter name="enableNamespacePrefixOptimization" value="false"/> 
 <parameter name="sendXMLDeclaration" value="true"/> 
 <parameter name="sendXsiTypes" value="true"/> 
 <parameter name="attachments.implementation" value="org.apache.axis.attachments.AttachmentsImpl"/> 
 <requestFlow> 
 <handler type="java:org.apache.axis.handlers.JWSHandler"> 
 <parameter name="scope" value="session"/> 
 </handler> 
 <handler type="java:org.apache.axis.handlers.JWSHandler"> 
 <parameter name="scope" value="request"/> 
 <parameter name="extension" value=".jwr"/> 
 </handler> 
 </requestFlow> 
 </globalConfiguration> 
 <handler name="LocalResponder" type="java:org.apache.axis.transport.local.LocalResponder"/> 
 <handler name="URLMapper" type="java:org.apache.axis.handlers.http.URLMapper"/> 
 <handler name="Authenticate" type="java:org.apache.axis.handlers.SimpleAuthenticationHandler"/> 
 <service name="AdminService" provider="java:MSG"> 
 <parameter name="allowedMethods" value="AdminService"/> 
 <parameter name="enableRemoteAdmin" value="false"/> 
 <parameter name="className" value="org.apache.axis.utils.Admin"/> 
 <namespace>http://xml.apache.org/axis/wsdd/</namespace> 
 </service> 
 <service name="Version" provider="java:RPC"> 
 <parameter name="allowedMethods" value="getVersion"/> 
 <parameter name="className" value="org.apache.axis.Version"/> 
 </service> 
<!-- </span> </li> <li class="alt"><span><span class="comments"> 原来的service的定义已经删除！ </span> </span></li> <li class=""><span><span class="comments">--> 
 <transport name="http"> 
 <requestFlow> 
 <handler type="URLMapper"/> 
 <handler type="java:org.apache.axis.handlers.http.HTTPAuthHandler"/> 
 </requestFlow> 
 <parameter name="qs:list" value="org.apache.axis.transport.http.QSListHandler"/> 
 <parameter name="qs:wsdl" value="org.apache.axis.transport.http.QSWSDLHandler"/> 
 <parameter name="qs.list" value="org.apache.axis.transport.http.QSListHandler"/> 
 <parameter name="qs.method" value="org.apache.axis.transport.http.QSMethodHandler"/> 
 <parameter name="qs:method" value="org.apache.axis.transport.http.QSMethodHandler"/> 
 <parameter name="qs.wsdl" value="org.apache.axis.transport.http.QSWSDLHandler"/> 
 </transport> 
 <transport name="local"> 
 <responseFlow> 
 <handler type="LocalResponder"/> 
 </responseFlow> 
 </transport> 
</deployment>
