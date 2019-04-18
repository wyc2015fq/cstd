# wsdd方式发布web service - z69183787的专栏 - CSDN博客
2014年07月14日 09:32:34[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1431
我主要参见这个地方[http://203.208.33.132/search?q=cache:m2OzEZLkhkEJ:qzone.qq.com/blog/82899476-1235122037+MyEclipse+web+service+helloworld+wsdd&cd=3&hl=zh-CN&ct=clnk&gl=cn&client=aff-cs-360se&st_usg=ALhdy29fzXT_S_o7qkihSqGY2hd8QfhB6w](http://203.208.33.132/search?q=cache:m2OzEZLkhkEJ:qzone.qq.com/blog/82899476-1235122037+MyEclipse+web+service+helloworld+wsdd&cd=3&hl=zh-CN&ct=clnk&gl=cn&client=aff-cs-360se&st_usg=ALhdy29fzXT_S_o7qkihSqGY2hd8QfhB6w)
2.定制发布 Custom Deployment - Introducing **WSDD**
      比起即时发布定制发布更加烦琐也更复杂，但是换来的却是更大的灵活性，因此在实际项目中定制发布还是不二的选择。定制发布需要你自己编写一个**WSDD**（**Web Service** Deployment Descriptor）文件，这个东东类似与XML稍后会做出介绍。废话不多说，我们来看代码： 
java 代码 
package  com.chnic.webservice;    
   public   class  **HelloWorld** {  注意，这里的代码不太对，不要用，用下一篇编写wsdd文档里的代码
  
             public  **HelloWorld**(){    
                }    
             public  String hello(String str){    
         return   "Hello "  + str;        }    
             public   int  add( int  a,  int  b){    
         return  a + b;        }    
        }    
        一个带包的很简单的类，在eclipse下编译后按照包名拷到 %TOMCAT_HOME%\webapps\axis\WEB-INF\classes 目录下。以这个类为例，拷贝完之后这个**HelloWorld**.class的路径就是 %TOMCAT_HOME%\webapps\axis\**WEB**-INF\classes\com\chnic\webservice。PS：
 如果嫌这样太麻烦，可以另外建一个Java **Web**工程用**myeclipse**的发布工具发布到Tomcat之后，整体一次性拷贝到websericve的工程中。 
       接下来就需要编写发布文件deploy.**wsdd**。到%TOMCAT_HOME%\webapps\axis\WEB-INF 目录下建立这个文件并在其中添加如下内容： 
xml 代码 注意，这里的代码不太对，不要用，用下一篇编写wsdd文档里的代码
      <deployment   xmlns = [http://xml.apache.org/axis/**wsdd**/](http://xml.apache.org/axis/wsdd/)   xmlns:java = [http://xml.apache.org/axis/**wsdd**/providers/java](http://xml.apache.org/axis/wsdd/providers/java)>    
    <**service**   name = "**HelloWorld**"   provider = "java:RPC" >    
   <parameter   name = "className"   value = "com.chnic.webservice.**HelloWorld**"/>            
<parameter   name = "allowedMethods"   value = "*" />        
    </**service**>    
</deployment>    
       简单的介绍下各个节点的含义，"**HelloWorld**"当然是这个webservice的名字，后面紧跟的java：RPC指的是服务类型。这里一共有有4种类型，分别是：RPC, Document, Wrapped 和 Message。有兴趣可以看下 org.apache.axis.providers这个包和子包下面的类的API文档。之后的parameter节点第一个当然是指出具体的类，第二个从字面上也很好理解：允许调用的方法。这里的配置告诉引擎可以调用所有的public方法，当然你也可以自己指定。 
编写完配置发布文件之后，cmd打开windows的控制台，进入%TOMCAT_HOME%\webapps\axis\WEB-INF目录下键入如下命令   
java -Djava.ext.dirs=lib org.apache.axis.client.AdminClient deploy.wsdd     注意如果端口是8888，应该键入：
java -Djava.ext.dirs=lib org.apache.axis.client.AdminClient -p8888 deploy.wsdd
很怪异的一件事是，wsdd文档只能用一次，再用就说找不到文件了，需要重新建立文件才行
       之后控制台返回Processing file deploy.**wsdd** 和 Done processing 这两段话即说明发布成功。（此时会在同级目录生成一个server-config.**wsdd**文件 ）在这里的AdminClient是AXIS提供的一个客户端管理工具。至于java.ext.dirs的含义可以去了解一下classloader和JVM类装载机制方面的知识，在这里就不多解释。 还有一点要注意的是在发布的时候Tomcat服务必须处于启动状态，否则就会抛出一堆无法连接的异常信息。发布成功之后你可以通过访问 [http://localhost:8080/axis/servlet/AxisServlet](http://localhost:8080/axis/servlet/AxisServlet) 来查看你所有的定制发布的服务。
要卸载发布的服务，只需要编辑这样的文档：
<undeployment xmlns="http://xml.apache.org/axis/wsdd/">   
 <service name="HelloAXIS_Service"/>   
</undeployment> 
存为undepoly.wsdd,位置和depoly.wsdd的一样，运行的命令行也一样：
java -Djava.ext.dirs=lib org.apache.axis.client.AdminClient -p8888 undeploy.wsdd

客户端测试代码 
import java.net.URL; 
import javax.xml.namespace.QName; 
import javax.xml.rpc.ServiceException; 
import org.apache.axis.client.Call; 
import org.apache.axis.client.**Service**; 
String targetEendPoint =  "http://localhost:8080/axis/services/**HelloWorld**" ;    
**Service service** =  new  **Service**();    Call call = (Call) **service**.createCall();    
call.setTargetEndpointAddress( new  URL(targetEendPoint));      
call.setOperationName( new  QName(targetEendPoint,  "hello" ));         String result = (String) call.invoke( new  Object[]{ "Robert" });    
System.out.println(result);      
call.setOperationName( new  QName(targetEendPoint,  "add" ));    Integer res = (Integer) call.invoke( new  Object[]{ new  Integer( 1 ),  new  Integer( 2 )});    
System.out.println( "The result is: "  + res);  
运行测试代码，控制台显示 Hello Robert 和 The result is: 3 这两句话，说明发布成功。仔细观察下发现其实除了那个targetEndpoint 之外，即时发布和定制发布的客户端调用代码基本上都是一样的。定制发布的URL可以在WSDL文件里找到。 
使用java调用webservice，可以使用Axis（[http://ws.apache.org/axis/](http://ws.apache.org/axis/)），调用列车 时刻表服务的代码： 
String url="[http://www.webxml.com.cn/WebServices/TrainTimeWebService.asmx](http://www.webxml.com.cn/WebServices/TrainTimeWebService.asmx)"; //endpiont 
String namespace = "[http://WebXml.com.cn/](http://webxml.com.cn/)"; //wsdl中的namespace 
String methodName = "getStationAndTimeByTrainCode"; //调用哪个方法，这里是 getStationAndTimeByTrainCode，因为返回的 是数组，比较方便，其他都是返回dataSet，需要额外解析 
String soapActionURI = "[http://WebXml.com.cn/getStationAndTimeByTrainCode](http://webxml.com.cn/getStationAndTimeByTrainCode)"; //wsdl中方法的soapActionURI 
Service service = new Service(); 
Call call = (Call) service.createCall(); 
call.setTargetEndpointAddress(new java.net.URL(url)); 
call.setUseSOAPAction(true); 
call.setSOAPActionURI(soapActionURI); 
call.setOperationName(new QName(namespace, methodName)); 
call.addParameter("TrainCode", XMLType.XSD_STRING, ParameterMode.IN); //设置列车代码参数 
call.addParameter("UserID", XMLType.XSD_STRING, ParameterMode.IN); //设置商业用户ID 
call.setReturnType(XMLType.XSD_ANYTYPE); //返回值类型，这里为String数组 
Object ret = call.invoke(new Object[] { "D32","" }); //这里是调用，参数为D32次列车 
System.out.println("返回结果：" + ret); //这里需要根据返回类型不同，做不同处理 
