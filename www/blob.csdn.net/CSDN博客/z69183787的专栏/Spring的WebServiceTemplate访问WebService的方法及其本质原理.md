# Spring的WebServiceTemplate访问WebService的方法及其本质原理 - z69183787的专栏 - CSDN博客
2016年12月06日 16:38:49[OkidoGreen](https://me.csdn.net/z69183787)阅读数：3186
WebService客户端调用的本质就是将SAOP格式的XML通过通信协议发送到WebService的服务器端,然后接收服务器端返回的XML.
本文简单介绍一下如何通过Spring提供的WebServiceTemplate访问Webservice,WebServiceTemplate与调用webservice的客户端已及webservice服务器端示意图如下(图片来源于Spring in Action):
![](http://hi.csdn.net/attachment/201202/24/0_1330067140447V.gif)
这里以SOAP over HTTP为例,开发步骤如下:
1,在Spring的配置文件中配置WebServiceTemplate,最简单的配置如下:
**[html]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- <beanid="webServiceTemplate"class="org.springframework.ws.client.core.WebServiceTemplate">
- <propertyname="defaultUri"value="http://localhost:8080/prjCXFWS/services/SimpleServicePort"/>
- </bean>
*这种配置省略了MessageFactory和messageSender的配置,Spring默认会使用SaajSoapMessageFactory和HttpUrlConnectionMessageSender.等同于下面的配置
**[html]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- <beanid="messageFactory"class="org.springframework.ws.soap.saaj.SaajSoapMessageFactory"/>
- <beanid="webServiceTemplate"class="org.springframework.ws.client.core.WebServiceTemplate">
- <constructor-argref="messageFactory"/>
- <propertyname="messageSender">
- <beanclass="org.springframework.ws.transport.http.HttpUrlConnectionMessageSender"/>
- </property>
- <propertyname="defaultUri"value="http://localhost:8080/prjCXFWS/services/SimpleServicePort"/>
- </bean>
还可以使用CommonsHttpMessageSender作为messageSender,它提供了设置timeout,用户名,密码等选项的功能.(需要使用commons-httpclient.jar和commons-codec.jar)
MessageFactory还可以使用AxiomSoapMessageFactory和DomPoxMessageFactory.
**[html]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- <beanid="webServiceTemplate"class="org.springframework.ws.client.core.WebServiceTemplate">
- <propertyname="messageSender">
- <beanclass="org.springframework.ws.transport.http.CommonsHttpMessageSender">
- <propertyname="readTimeout"value="0"/>
- </bean>
- </property>
- <propertyname="defaultUri"value="http://localhost:8080/prjCXFWS/services/SimpleServicePort"/>
- </bean>
2,Java调用代码:传输的是SOAP XML.
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- privatestaticfinal String MESSAGE =  
- "<queryPeopleByID  xmlns=\"http://test.cxfws.com\">1231ss</queryPeopleByID> ";  
- publicstaticvoid test() {  
-     ApplicationContext ac = new ClassPathXmlApplicationContext("conf/wsAppcontext.xml");  
-     WebServiceTemplate simpleService = (WebServiceTemplate) ac.getBean("webServiceTemplate");  
-     StreamSource source = new StreamSource(new StringReader(MESSAGE));  
-        StreamResult result = new StreamResult(System.out);          
-        simpleService.sendSourceAndReceiveToResult(source, result);  
- }  
*MESSAGE为需要发送到webservice服务器端的XML payload内容,SOAP body之内的XML内容.
Spring调用Webservice的另一种方法是通过Spring提供的JaxWsPortProxyFactoryBean,示意图如下(图片来源于Spring in Action):
![](http://hi.csdn.net/attachment/201202/24/0_1330069334u76m.gif)
Spring的配置如下:
**[html]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- <beanid="simpleService"
- class="org.springframework.remoting.jaxws.JaxWsPortProxyFactoryBean">
- <propertyname="serviceInterface"
- value="com.cxfclient.test.SimpleService"/>
- <propertyname="wsdlDocumentUrl"
- value="http://localhost:8080/prjCXFWS/services/SimpleServicePort?WSDL"/>
- <propertyname="namespaceUri"value="http://test.cxfws.com/"/>
- <propertyname="serviceName"value="SimpleServiceService"/>
- <propertyname="portName"value="SimpleServicePort"/>
- </bean>
**这种方式需要用工具通过Webservice 的wsdl文件生成客户端需要的一些Java类,如service的interface,参数类等等(如下面代码中的SimpleService,People类).
Java调用代码如下
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- ApplicationContext ac = new ClassPathXmlApplicationContext("conf/wsAppcontext.xml");  
- SimpleService simpleService = (SimpleService) ac.getBean("simpleService");  
- 
- People people = simpleService.queryPeopleByID("test");  
- System.out.println(people.getAge() + people.getName() + people.getPid());  
关于更多客户端如何调用webservice,参照http://blog.csdn.net/kkdelta/article/details/3987591
对于服务器端,其本质也是接收符合SOAP规范的XML消息,解析XML,返回符合SOAP规范的XML,这里用一个servlet模拟webservice,代码如下:
**[java]**[view
 plain](http://blog.csdn.net/kkdelta/article/details/7290769#)[copy](http://blog.csdn.net/kkdelta/article/details/7290769#)
![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/166906/fork)
- publicclass WSSimulator extends HttpServlet {  
- publicvoid doGet(HttpServletRequest request, HttpServletResponse response)  
- throws IOException {  
-         System.out.println("doGet");  
-         BufferedReader in = new BufferedReader(new InputStreamReader( request.getInputStream()));  
-         String str;  
- while ((str = in.readLine()) != null) {  
-             System.out.println(str); ##1
-         }  
-         in.close();  
- 
-         String soapHeader = "<SOAP-ENV:Envelope xmlns:SOAP-ENV=\"http://schemas.xmlsoap.org/soap/envelope/\">"
-             +"<SOAP-ENV:Header/><SOAP-ENV:Body>";  
-         String soapPayload = "<xxx>yyy</xxx>";  
-         String soapTail = "</SOAP-ENV:Body></SOAP-ENV:Envelope>";         
-         response.getWriter().write(soapHeader + soapPayload + soapTail);  ##2
-     }  
- publicvoid doPost(HttpServletRequest request, HttpServletResponse response) throws IOException {  
-         System.out.println("doPost");  
-         doGet(request, response);  
-     }  
- }  
通过WebServiceTemplate将消息发送到这个servlet监听的url,可以更深理解Webservice的本质.
##1 str就是从客户端传输到服务器端的XML.##2将SOAP消息返回给客户端.
更多服务器端webservice的开发请参照http://blog.csdn.net/kkdelta/article/details/3984312
