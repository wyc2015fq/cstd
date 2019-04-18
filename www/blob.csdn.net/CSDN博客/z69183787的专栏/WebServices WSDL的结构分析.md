# WebServices:WSDL的结构分析 - z69183787的专栏 - CSDN博客
2016年12月06日 13:35:48[OkidoGreen](https://me.csdn.net/z69183787)阅读数：981

   WSDL（Web Services Description Language，Web服务描述语言）是为描述Web Services发布的XML格式。W3C组织没有批准1.1版的WSDL，但是2.0版本已经在製訂中，2.0版将被作为推荐标准（recommendation）（一种官方标准），并将被W3C组织批准为正式标准。WSDL描述Web服务的公共接口。这是一个基于XML的关于如何与Web服务通讯和使用的服务描述；也就是描述与目录中列出的Web服务进行交互时需要绑定的协议和信息格式。通常采用抽象语言描述该服务支持的操作和信息，使用的时候再将实际的网络协议和信息格式绑定给该服务。
      WSDL 文档仅仅是一个简单的 XML 文档。它包含一系列描述某个 web service 的定义。
      WebMthod的定义：
 1:  [WebService(Namespace = "http://tempuri.org/")] 2:  [WebServiceBinding(ConformsTo = WsiProfiles.BasicProfile1_1)] 3:  [System.ComponentModel.ToolboxItem(false)] 4:  public class WebService2 : System.Web.Services.WebService 5:  { 6:      [WebMethod] 7:  public bool Add(TestClass testClass,int id) 8:      { 9:  return true;10:      }11:  }12:  13:  public class TestClass14:  {15:  public int a;16:  public string b;17:  public DateTime c;18:  }19:  
       WSDL的结构：
![image](http://images.cnblogs.com/cnblogs_com/zhaozhan/WindowsLiveWriter/WebServicesWSDL_130BF/image_thumb_1.png)
       一个WSDL文档通常包含有以下元素，即types、message、portType、operation、binding、 service元素。这些元素嵌套在definitions元素中。
definitions是WSDL文档的根元素，definitions还声明各命名空间。
types，数据类型定义的容器，它使用某种类型系统(一般地使用XML Schema中的类型系统)。
 1:  <wsdl:types> 2:      <s:schema elementFormDefault="qualified" targetNamespace="http://tempuri.org/"> 3:        <s:element name="Add"> 4:          <s:complexType> 5:            <s:sequence> 6:              <s:element minOccurs="0" maxOccurs="1" name="testClass" type="tns:TestClass" /> 7:              <s:element minOccurs="1" maxOccurs="1" name="id" type="s:int" /> 8:            </s:sequence> 9:          </s:complexType>10:        </s:element>11:        <s:complexType name="TestClass">12:          <s:sequence>13:            <s:element minOccurs="1" maxOccurs="1" name="a" type="s:int" />14:            <s:element minOccurs="0" maxOccurs="1" name="b" type="s:string" />15:            <s:element minOccurs="1" maxOccurs="1" name="c" type="s:dateTime" />16:          </s:sequence>17:        </s:complexType>18:        <s:element name="AddResponse">19:          <s:complexType>20:            <s:sequence>21:              <s:element minOccurs="1" maxOccurs="1" name="AddResult" type="s:boolean" />22:            </s:sequence>23:          </s:complexType>24:        </s:element>25:      </s:schema>26:    </wsdl:types>
     types描述WebMethod的名称（Add），传入参数（testClass——包括对TestClass的详细描述,id）,响应信息（AddResponse）。
message描述通信消息的数据结构的抽象类型化定义，使用types的描述的类型来定义整个消息的数据结构。
1:  <wsdl:message name="AddSoapIn">2:      <wsdl:part name="parameters" element="tns:Add" />3:    </wsdl:message>4:    <wsdl:message name="AddSoapOut">5:      <wsdl:part name="parameters" element="tns:AddResponse" />6:    </wsdl:message>
portType和operation描述服务和服务的方法。operation包括输入和输出（使用message的描述）。
1:  <wsdl:portType name="WebService2Soap">2:      <wsdl:operation name="Add">3:        <wsdl:input message="tns:AddSoapIn" />4:        <wsdl:output message="tns:AddSoapOut" />5:      </wsdl:operation>6:    </wsdl:portType>
binding描述Web Services的通信协议。 <soap:binding/>描述使用SOAP协议，binding还描述Web Services的方法、输入、输出。
 1:  <wsdl:binding name="WebService2Soap" type="tns:WebService2Soap"> 2:      <soap:binding transport="http://schemas.xmlsoap.org/soap/http" /> 3:      <wsdl:operation name="Add"> 4:        <soap:operation soapAction="http://tempuri.org/Add" style="document" /> 5:        <wsdl:input> 6:          <soap:body use="literal" /> 7:        </wsdl:input> 8:        <wsdl:output> 9:          <soap:body use="literal" />10:        </wsdl:output>11:      </wsdl:operation>12:    </wsdl:binding>
service描述Web Services访问点的集合。因为包括SOAP1.1和SOAP1.2的描述，所以一个方法有对应两描述。
1:  <wsdl:service name="WebService2">2:      <wsdl:port name="WebService2Soap" binding="tns:WebService2Soap">3:        <soap:address location="http://localhost:1552/WebService2.asmx" />4:      </wsdl:port>5:      <wsdl:port name="WebService2Soap12" binding="tns:WebService2Soap12">6:        <soap12:address location="http://localhost:1552/WebService2.asmx" />7:      </wsdl:port>8:    </wsdl:service>
