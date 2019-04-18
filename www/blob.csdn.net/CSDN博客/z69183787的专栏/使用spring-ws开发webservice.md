# 使用spring-ws开发webservice - z69183787的专栏 - CSDN博客
2016年12月05日 20:17:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：976
官方文档：[http://docs.spring.io/spring-ws/site/reference/html/client.html](http://docs.spring.io/spring-ws/site/reference/html/client.html)
spring是一个优雅的框架，将常用的技术粘合在一起。spring-ws相对于其他框架最大的优点是对web服务涉及技术的抽象非常的好，为webservice处理提供统一的模型，而又不限制实现时使用的技术。
# 发布webservice
发布webservice有以下几个步骤： 
1. 编写服务接口定义，包括类型定义，服务方法定义。 
2. 实现服务接口定义的方法。 
3. 发布服务。 
本文已一个订单服务为例
## 为什么契约优先
开发webservice可以定义java代码，通过框架生产wsdl/xsd内容。也可以采用契约优先方式，先定义wsdl/xsd，然后生成java代码。通过契约优先的方式有以下原因： 
1. Object/XML之间的转换。对象与xml转换不能完全的匹配，xsd可以定义对象的限制，比如字符串必须是大写，而java对象没有办法定义限制。 
2. java类不是可移植的类型。 
3. 环形依赖。如果一个类之间相互依赖，则生成的xml会不停的嵌套。 
详情可以参考《Spring Web Services Reference》
## 编写服务接口定义
定义数据类型，对于请求和反馈，尽量增加一个基础类，这样可以随时增加公共的内容。 
xsd文件内容
```xml
<?xml version="1.0" encoding="UTF-8"?>
<schema targetNamespace="http://www.nxmax.net/hello"
    elementFormDefault="qualified" xmlns="http://www.w3.org/2001/XMLSchema"
    xmlns:tns="http://www.nxmax.net/hello">
    <complexType name="BaseRespType" abstract="true">
        <sequence>
            <element name="success" type="boolean" minOccurs="1"
                maxOccurs="1" />
            <element name="message" type="string" minOccurs="0" />
        </sequence>
    </complexType>
    <complexType name="BaseReqType" abstract="true">
        <sequence>
            <element name="callerIndentifier" type="string" minOccurs="1" maxOccurs="1"></element>
        </sequence>
    </complexType>
    <element name="CreateOrderReq">
        <complexType>
            <complexContent>
                <extension base="tns:BaseReqType">
                    <sequence>
                        <element name="GoodCode" type="string" />
                        <element name="Amount" type="int" />
                        <element name="Remarks" type="string" minOccurs="0" />
                    </sequence>
                </extension>
            </complexContent>
        </complexType>
    </element>
    <element name="CreateOrderResp">
        <complexType>
            <complexContent>
                <extension base="tns:BaseRespType">
                    <sequence>
                        <element name="OrderNum" type="string" minOccurs="1"
                            maxOccurs="1"></element>
                    </sequence>
                </extension>
            </complexContent>
        </complexType>
    </element>
    <element name="InquireReq">
        <complexType>
            <complexContent>
                <extension base="tns:BaseReqType">
                    <sequence>
                        <element name="orderNum" type="string" minOccurs="1" maxOccurs="1"></element>
                    </sequence>
                </extension>
            </complexContent>
        </complexType></element>
    <element name="InquireResp">
        <complexType>
            <complexContent>
                <extension base="tns:BaseRespType">
                    <sequence>
                        <element name="orderInfo" type="tns:OrderInfoType" minOccurs="0" maxOccurs="1"></element>
                    </sequence>
                </extension>
            </complexContent>
        </complexType>
    </element>
    <complexType name="OrderInfoType">
        <all>
            <element name="orderNum" type="string" minOccurs="1"
                maxOccurs="1">
            </element>
            <element name="date" type="date" minOccurs="1"
                maxOccurs="1">
            </element>
            <element name="time" type="time" minOccurs="1"
                maxOccurs="1">
            </element>
            <element name="goodCode" type="string" minOccurs="1"
                maxOccurs="1">
            </element>
            <element name="remarks" type="string" minOccurs="0" maxOccurs="1"></element>
        </all>
    </complexType>
</schema>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 71
- 72
- 73
- 74
- 75
- 76
- 77
- 78
- 79
- 80
- 81
- 82
- 83
编写wsdl文件
```xml
<?xml version="1.0" encoding="UTF-8" standalone="no"?>
<wsdl:definitions name="hello" targetNamespace="http://www.nxmax.net/hello/"
    xmlns:soap="http://schemas.xmlsoap.org/wsdl/soap/" xmlns:tns="http://www.nxmax.net/hello/"
    xmlns:wsdl="http://schemas.xmlsoap.org/wsdl/" xmlns:xsd="http://www.w3.org/2001/XMLSchema"
    xmlns:xsd1="http://www.nxmax.net/hello" xmlns:mime="http://schemas.xmlsoap.org/wsdl/mime/"
    xmlns:http="http://schemas.xmlsoap.org/wsdl/http/">
    <wsdl:types>
        <xsd:schema xmlns:xsd="http://www.w3.org/2001/XMLSchema">
            <xsd:import namespace="http://www.nxmax.net/hello"
                schemaLocation="hello.xsd">
            </xsd:import>
        </xsd:schema>
    </wsdl:types>
    <wsdl:message name="createRequest">
        <wsdl:part element="xsd1:CreateOrderReq" name="parameters" />
    </wsdl:message>
    <wsdl:message name="createResponse">
        <wsdl:part element="xsd1:CreateOrderResp" name="parameters" />
    </wsdl:message>
    <wsdl:message name="inquireRequest">
        <wsdl:part name="parameters" element="xsd1:InquireReq"></wsdl:part>
    </wsdl:message>
    <wsdl:message name="inquireResponse">
        <wsdl:part name="parameters" element="xsd1:InquireResp"></wsdl:part>
    </wsdl:message>
    <wsdl:portType name="Order">
        <wsdl:operation name="create">
            <wsdl:input message="tns:createRequest" />
            <wsdl:output message="tns:createResponse" />
        </wsdl:operation>
        <wsdl:operation name="inquire">
            <wsdl:input message="tns:inquireRequest"></wsdl:input>
            <wsdl:output message="tns:inquireResponse"></wsdl:output>
        </wsdl:operation>
    </wsdl:portType>
    <wsdl:binding name="OrderBinding" type="tns:Order">
        <soap:binding style="document"
            transport="http://schemas.xmlsoap.org/soap/http" />
        <wsdl:operation name="create">
            <soap:operation
                soapAction="http://www.nxmax.net/hello/create" />
            <wsdl:input>
                <soap:body use="literal" />
            </wsdl:input>
            <wsdl:output>
                <soap:body use="literal" />
            </wsdl:output>
        </wsdl:operation>
        <wsdl:operation name="inquire">
            <soap:operation
                soapAction="http://www.nxmax.net/hello/inquire" />
            <wsdl:input>
                <soap:body use="literal" />
            </wsdl:input>
            <wsdl:output>
                <soap:body use="literal" />
            </wsdl:output>
        </wsdl:operation>
    </wsdl:binding>
    <wsdl:service name="OrderService">
        <wsdl:port binding="tns:OrderBinding" name="OrderPort">
            <soap:address location="http://www.example.org/" />
        </wsdl:port>
    </wsdl:service>
</wsdl:definitions>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 68
- 69
- 70
## spring处理webservice请求原理
一个webservice请求如下，
```
POST http://localhost:8081/sws/ HTTP/1.1
Content-Type: text/xml;charset=UTF-8
SOAPAction: "http://www.nxmax.net/hello/create"
User-Agent: Jakarta Commons-HttpClient/3.1
Host: localhost:8081
Content-Length: 467
<soapenv:Envelope xmlns:soapenv="http://schemas.xmlsoap.org/soap/envelope/" xmlns:hel="http://www.nxmax.net/hello">
   <soapenv:Header/>
   <soapenv:Body>
      <hel:CreateOrderReq>
         <hel:callerIndentifier>ewewe</hel:callerIndentifier>
         <hel:GoodCode>2323</hel:GoodCode>
         <hel:Amount>3</hel:Amount>
         <hel:Remarks>2323</hel:Remarks>
         <!--Optional:-->
      </hel:CreateOrderReq>
   </soapenv:Body>
</soapenv:Envelope>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
spring接受到请求后通过EndpointMapping获取分发目标。默认spring支持3种分发方式： 
1. 通过xml携带的内容确定。soapenv:Body保护了本次请求的内容。通过@PayloadRoot指明，如@PayloadRoot(namespace = “[http://www.nxmax.net/hello](http://www.nxmax.net/hello)“, localPart = “CreateOrderReq”) 
2. 通过http头中的SOAPAction确定。通过@SoapAction指明，如@SoapAction(“[http://www.nxmax.net/hello/create](http://www.nxmax.net/hello/create)“) 
3. 通过WS-Addressing中的Action确定。通过@Action指明，如@Action(“[http://globalplatform.org/globaleligibilityinfo/GlobalEligibilityInfo/CheckGlobalEligibilityRequest](http://globalplatform.org/globaleligibilityinfo/GlobalEligibilityInfo/CheckGlobalEligibilityRequest)“)
spring-ws提供灵活处理输入输出的方式，可以手动处理xml，可以通过jaxb将xml转换为对象之后再由应用处理。支持的类型非常广泛，具体可以参考《Spring Web Services Reference》的5.4 Endpoints。
## 手工处理xml内容
手工处理xml内容需要使用 @PayloadRoot修饰方法，方法的参数类型可以使用org.w3c.dom.Element，示例如下：
```
package net.nxmax.sws.impl;
import java.io.ByteArrayInputStream;
import java.io.StringWriter;
import javax.xml.parsers.DocumentBuilderFactory;
import javax.xml.transform.OutputKeys;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerFactory;
import javax.xml.transform.dom.DOMSource;
import javax.xml.transform.stream.StreamResult;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.ws.server.endpoint.annotation.Endpoint;
import org.springframework.ws.server.endpoint.annotation.PayloadRoot;
import org.springframework.ws.server.endpoint.annotation.RequestPayload;
import org.springframework.ws.server.endpoint.annotation.ResponsePayload;
import org.w3c.dom.Document;
import org.w3c.dom.Element;
// 指定是一个ws入口
@Endpoint
public class XmlEndpoint {
    private static final String NAMESPACE_URI = "http://www.nxmax.net/hello";
    /** Logger */
    private static final Logger LOG = LoggerFactory
            .getLogger(XmlEndpoint.class);
    private TransformerFactory transFactory = TransformerFactory.newInstance();
    private DocumentBuilderFactory cocumentBuilderFactory = DocumentBuilderFactory
            .newInstance();
    // 使用内容作为分发标识
    // @ResponsePayload 指明返回值需要转换
    // @RequestPayload 指明是输入的参数
    @PayloadRoot(namespace = NAMESPACE_URI, localPart = "CreateOrderReq")
    @ResponsePayload
    public Element create(@RequestPayload Element req) throws Exception {
        LOG.info("REQ XML:{}", e2s(req));
        Element rt = getReturn();
        LOG.info("RESP XML:{}", e2s(rt));
        return rt;
    }
    private Object e2s(Element req) throws Exception {
        Transformer transformer = transFactory.newTransformer();
        StringWriter buffer = new StringWriter();
        transformer.setOutputProperty(OutputKeys.OMIT_XML_DECLARATION, "yes");
        transformer.transform(new DOMSource(req), new StreamResult(buffer));
        String str = buffer.toString();
        return str;
    }
    private Element getReturn() throws Exception {
        StringBuilder sb = new StringBuilder();
        sb.append("<hel:CreateOrderResp xmlns:hel=\"http://www.nxmax.net/hello\">");
        sb.append("<hel:success>true</hel:success>");
        sb.append("<hel:message>3232323</hel:message>");
        sb.append("<hel:OrderNum>2222222222</hel:OrderNum>");
        sb.append("</hel:CreateOrderResp>");
        Document x = cocumentBuilderFactory.newDocumentBuilder().parse(
                new ByteArrayInputStream(sb.toString().getBytes()));
        return x.getDocumentElement();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 43
- 44
- 45
- 46
- 47
- 48
- 49
- 50
- 51
- 52
- 53
- 54
- 55
- 56
- 57
- 58
- 59
- 60
- 61
- 62
- 63
- 64
- 65
- 66
- 67
函数的参数可以可以包含其他内容，比如MessageContext。比如方法可以定义为
```java
public Element create(@RequestPayload Element req, MessageContext context,
            SoapMessage message) throws Exception
```
- 1
- 2
- 1
- 2
MessageContext,SoapMessage是默认启用的，不需要@RequestPayload修饰。
## 将定义转换为java代码
现在还在使用xml处理webservice，太out了吧。没问题，我们来使用java的标准工具。 
java有自带的工具将wsdl/xsd转换为java代码。导入wsdl的是wsimport，导入xsd的是xjc。分别使用以下命令导入。 
wsimport -p net.nxmax.sws.def -d ..\java -Xnocompile -verbose -target 2.2 hello.wsdl 
xjc -p net.nxmax.sws.def -d ..\java -verbose -no-header hello.xsd 
对于spring的调用，xjc就足够了，因为操作可以通过请求确定，完整的wsdl定义就不需要了。 
下面是方法的定义
```java
// 下面是使用jaxb，使用soapation的方式
    protected ObjectFactory factory = new ObjectFactory();
    @SoapAction("http://www.nxmax.net/hello/create")
    @ResponsePayload
    public CreateOrderResp create(@RequestPayload CreateOrderReq req) {
        return factory.createCreateOrderResp();
    }
    @SoapAction("http://www.nxmax.net/hello/inquire")
    @ResponsePayload
    public InquireResp inquire(@RequestPayload InquireReq req) {
        return factory.createInquireResp();
    }
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
这样就非常方便了。
# 引用
spring可以使用wsimport生成的定义，但是在spring-ws中使用org.springframework.ws.client.core.WebServiceTemplate功能更强大，灵活。测试代码如下：
```xml
<?xml version="1.0" encoding="UTF-8"?>
<beans xmlns="http://www.springframework.org/schema/beans"
    xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance" xmlns:context="http://www.springframework.org/schema/context"
    xmlns:ws="http://www.springframework.org/schema/web-services" xmlns:p="http://www.springframework.org/schema/p"
    xsi:schemaLocation="http://www.springframework.org/schema/beans http://www.springframework.org/schema/beans/spring-beans.xsd
        http://www.springframework.org/schema/web-services http://www.springframework.org/schema/web-services/web-services.xsd
        http://www.springframework.org/schema/context http://www.springframework.org/schema/context/spring-context-4.0.xsd">
    <bean id="messageFactory" class="org.springframework.ws.soap.saaj.SaajSoapMessageFactory" />
    <!-- 定义jaxb对象转换的包名 -->
    <bean id="jaxb" class="org.springframework.oxm.jaxb.Jaxb2Marshaller">
        <property name="contextPath" value="net.nxmax.sws.def"></property>
    </bean>
    <!-- 定义模板 -->
    <bean id="webServiceTemplate" class="org.springframework.ws.client.core.WebServiceTemplate"
        p:defaultUri="http://localhost:8081/sws/" p:marshaller-ref="jaxb"
        p:unmarshaller-ref="jaxb">
        <constructor-arg ref="messageFactory" />
    </bean>
</beans>
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
```
package net.nxmax.sws.client;
import net.nxmax.sws.def.CreateOrderReq;
import net.nxmax.sws.def.CreateOrderResp;
import net.nxmax.sws.def.ObjectFactory;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;
import org.springframework.context.ConfigurableApplicationContext;
import org.springframework.context.support.GenericXmlApplicationContext;
import org.springframework.ws.client.core.WebServiceMessageCallback;
import org.springframework.ws.client.core.WebServiceTemplate;
import org.springframework.ws.soap.client.core.SoapActionCallback;
public class SwsClient {
    /** Logger */
    private static final Logger LOG = LoggerFactory.getLogger(SwsClient.class);
    public static void main(String[] args) {
        new SwsClient().go();
    }
    private void go() {
        ObjectFactory factory = new ObjectFactory();
        ConfigurableApplicationContext ctx = new GenericXmlApplicationContext(
                "classpath:/client.xml");
        WebServiceTemplate template = ctx.getBean(WebServiceTemplate.class);
        // 指定soapation，与wsdl中的定义一致。如果
        WebServiceMessageCallback callback = new SoapActionCallback(
                "http://www.nxmax.net/hello/create");
        CreateOrderReq req = factory.createCreateOrderReq();
        CreateOrderResp resp = (CreateOrderResp) template
                .marshalSendAndReceive(req, callback);
        LOG.info("Result:{}", resp.isSuccess());
        ctx.close();
    }
}
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
- 33
- 34
- 35
- 36
- 37
- 38
- 39
- 40
- 41
- 42
[示例代码](http://download.csdn.net/detail/nullo_cns/9161607)
