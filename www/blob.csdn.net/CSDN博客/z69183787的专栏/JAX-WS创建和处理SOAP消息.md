# JAX-WS创建和处理SOAP消息 - z69183787的专栏 - CSDN博客
2014年05月09日 12:55:46[OkidoGreen](https://me.csdn.net/z69183787)阅读数：901
JAX-WS规范是一组XML web services的JAVA API，在 JAX-WS中，一个远程调用可以转换为一个基于XML的协议例如SOAP，在使用JAX-WS过程中，开发者不需要编写任何生成和处理SOAP消息的代码。JAX-WS的运行时实现会将这些API的调用转换成为对应的SOAP消息。
 JAX-WS 也提供了一组针对底层消息进行操作的API调用，你可以通过Dispatch 直接使用SOAP消息或XML消息发送请求或者使用Provider处理SOAP或XML消息。通过web service所提供的互操作环境，我们可以用JAX-WS轻松实现JAVA平台与其他编程环境（.net等）的互操作。
     现在WebService的主要成熟的框架有axis、cxf等，都很好的支持了SOAP协议，而且CXF也提供了对 JAX-WS 全面的支持。可以算是JAX-WS进行封装的框架。
     这篇文章就对处理web services的JAVA基础API JAX-WS进行介绍。
     首先看如何发布web services服务。
     1、编写接口，使用@WebService注解
**[java]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/9219565#)[copy](http://blog.csdn.net/wanghuan203/article/details/9219565#)
- <span style="font-family:SimSun;font-size:14px;">package webservice.wh;  
- 
- import javax.jws.WebParam;  
- import javax.jws.WebResult;  
- import javax.jws.WebService;  
- 
- @WebService
- publicinterface IMyService {  
- @WebResult(name="addResult")  
- //对返回值和参数进行名字定义，否则默认为arg0，arg1...
- publicint add(@WebParam(name="firstA")int a ,@WebParam(name="SecondB")int b);  
- //与上面方法进行对比
- publicint minus(int a,int b);  
- }  
- </span>  
    2、编写编写实现类，标明@WebService注解
**[java]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/9219565#)[copy](http://blog.csdn.net/wanghuan203/article/details/9219565#)
- <span style="font-family:SimSun;font-size:14px;">package webservice.wh;  
- 
- import javax.jws.WebResult;  
- import javax.jws.WebService;  
- 
- @WebService(endpointInterface="webservice.wh.IMyService")  
- publicclass MyServiceImpl implements IMyService {  
- 
- @Override
- @WebResult(name="addResult")  
- publicint add(int a, int b) {  
-         System.out.print(a+"+"+b+"="+(a+b));  
- return a+b;  
-     }  
- 
- @Override
- publicint minus(int a, int b) {  
-         System.out.print(a+"-"+b+"="+(a-b));  
- return a-b;  
-     }  
- 
- }  
- </span>  
     3、编写服务类，根据url和是实现类启动服务
**[java]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/9219565#)[copy](http://blog.csdn.net/wanghuan203/article/details/9219565#)
- <span style="font-family:SimSun;font-size:14px;">package webservice.wh;  
- import javax.xml.ws.Endpoint;  
- publicclass MyService {  
- publicstaticvoid main(String[] args){  
-         String address="http://localhost:6666/ns";  
-         Endpoint.publish(address, new MyServiceImpl());  
-     }  
- }</span>  
     4、运行3中代码，发布服务，在浏览器可查看
![](https://img-blog.csdn.net/20130701204025328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ2h1YW4yMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     5、使用jax-ws封装好的服务进行客户端调用
**[java]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/9219565#)[copy](http://blog.csdn.net/wanghuan203/article/details/9219565#)
- <span style="font-family:SimSun;font-size:14px;">package webservice.wh;  
- 
- import java.net.MalformedURLException;  
- import java.net.URL;  
- 
- import javax.xml.namespace.QName;  
- import javax.xml.ws.Service;  
- 
- publicclass TestClient {  
- publicstaticvoid main(String[] args) {  
- try {  
-             URL url = new URL("http://localhost:7777/ns?wsdl");  
-             QName sname = new QName("http://wh.webservice/", "MyServiceImplService");  
-             Service service = Service.create(url,sname);  
-             IMyService ms = service.getPort(IMyService.class);  
-             System.out.println(ms.add(12,33));  
-         } catch (MalformedURLException e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }  
- </span>  
    这样的客户端调用直接使用封装好的service进行操作，在调用过程中将我们的代码转换为SOAP“信件”与服务进行交互，我们不需要编写任何生成和处理SOAP消息的代码。很是方便。不过既然是JAVA
 底层API，它也提供了底层的SOAP操作API，我们来用易用，了解一下SOAP是怎样生成的。
     6、创建SOAP消息
**[java]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/9219565#)[copy](http://blog.csdn.net/wanghuan203/article/details/9219565#)
- <span style="font-family:SimSun;font-size:14px;">publicclass TestSoap {  
- 
- private String ns = "http://service.soap.org/";  
- private String wsdlUrl = "http://localhost:6666/ms?wsdl";  
- 
- /**
-      * 创建SOAP消息
-      * @author WHuan
-      */
- @Test
- 
- publicvoid createSoap() {  
- try {  
- // 创建消息工厂
-             MessageFactory factory = MessageFactory.newInstance();  
- // 根据消息工厂创建SoapMessage
-             SOAPMessage message = factory.createMessage();  
- // 创建SOAPPart
-             SOAPPart part = message.getSOAPPart();  
- // 获取SOAPEnvelope
-             SOAPEnvelope envelope = part.getEnvelope();  
- // 通过SoapEnvelope可以获取到相应的Body和Header等信息
-             SOAPBody body = envelope.getBody();  
- // 根据Qname创建相应的节点,Qname是一个带有命名空间的节点
-             QName qname = new QName("http://java.wh/webservice/", "add", "ns");  
- // <ns:add xmlns="http://java.wh/webservice/"
-             SOAPBodyElement ele = body.addBodyElement(qname);  
-             ele.addChildElement("a").setValue("11");  
-             ele.addChildElement("b").setValue("22");  
- // 打印消息信息
-             message.writeTo(System.out);  
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }  
-     }  
- }</span>  
     代码比较容易理解，首先通过消息工厂MessageFactory创建SOAPMessage，然后创建SOAPPart，我的理解相当于创建了一个邮箱，然后创建信件SOAPEnvelope，之后是SOAP的BODY和Header部分，其中Header部分可以省略。不过现在的API可以省略SOAPPart，直接创建BODY和Header，鉴于Part是经典方式，所以还是按照传统来了。
     这里涉及到SOAP消息的格式，很形象的可以理解为一封信件，鉴于是基础知识，这里便不再赘述，不清楚的可以先补充一下SOAP格式，理解了SOAP的格式之后，上面那段代码是很容易理解的。
     这样进行运行后，我们便可以得到一个完整的SOAP消息：
![](https://img-blog.csdn.net/20130701212358500?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ2h1YW4yMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
     得到这样的SOAP消息后，怎么与我们发布的WSDL进行交互呢？
     7、SOAP消息传递和处理，使用Dispatch 
**[java]**[view
 plain](http://blog.csdn.net/wanghuan203/article/details/9219565#)[copy](http://blog.csdn.net/wanghuan203/article/details/9219565#)
- <span style="font-family:SimSun;font-size:14px;">   /**
-      * 创建SOAP消息,并提交得到返回值
-      * @author WHuan
-      */
- @Test
- publicvoid soapDispatch() {  
- try {  
- // 创建服务service
-             URL url = new URL(wsdlUrl);  
-             QName sname= new QName(ns,"MyServiceImplService");  
-             Service service =Service.create(url,sname);  
- 
- //创建DIspatch
-             Dispatch<SOAPMessage> dispatch=service.createDispatch(new QName(ns,"MyServiceImplPort"),  
- SOAPMessage.class, Service.Mode.MESSAGE);  
- 
- //创建SOAPMessage
-             SOAPMessage msg=MessageFactory.newInstance().createMessage();  
-             SOAPEnvelope envelope =msg.getSOAPPart().getEnvelope();  
-             SOAPBody body=envelope.getBody();  
- 
- //创建QName来指定消息中传递数据
-             QName ename=new QName(ns,"add","nn");  
- //<nn:add xmlns="xx"/>
-             SOAPBodyElement ele=body.addBodyElement(ename);  
-             ele.addChildElement("firstA").setValue("22");  
-             ele.addChildElement("SecondB").setValue("33");  
-             msg.writeTo(System.out);  
-             System.out.println("\n invoking....");  
- 
- //通过Dispatch传递消息,会返回相应消息
-             SOAPMessage response = dispatch.invoke(msg);  
-             response.writeTo(System.out);  
-             System.out.println();  
- 
- //将相应的消息转换为doc对象
-             Document doc= response.getSOAPPart().getEnvelope().getBody().extractContentAsDocument();  
-             String str=doc.getElementsByTagName("addResult").item(0).getTextContent();  
-             System.out.println(str);  
- 
-         } catch (Exception e) {  
-             e.printStackTrace();  
-         }</span>  
    这样直接创建了SOAP消息后使用dispatch便可以进行传递，通过extractConentAsDocument方法得到Document类型的返回值，从返回值的SOAP消息中取得我们想要的标签信息。输出结果如下：
![](https://img-blog.csdn.net/20130701212609843?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ2h1YW4yMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20130701212614234?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvd2FuZ2h1YW4yMDM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
 本文主要从使用JAVA自带API出发，介绍一些稍微底层些的SOAP消息的创建和传递处理。理解了底层，也对web Service的框架使用多一份熟悉，也当给大家复习下SOAP消息吧。
