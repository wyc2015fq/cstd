# gSOAP 源码分析(二) - 建建的博客 - CSDN博客
2017年02月17日 15:45:37[纪建](https://me.csdn.net/u013898698)阅读数：319
 gSOAP 源码分析(二)
2012-5-24 邵盛松
一 gSOAP XML介绍
Xml的全称是EXtensible Markup Language。可扩展标记语言。仅仅是一个纯文本。适合用于数据的存储和传输。里面的标签由作者赋予它含义
比如
<Email>
<From>a@domain.com/From>
<To>b@domain.com</To>
<Head>**Hello**</Head>
<Body>**Hello World**</Body>
</Email>
可以赋予这个xml文档含义是 a发送给b一封邮件,邮件标题是“Hello”，正文是“Hello World”
 <ns:add>
   <a>0.0</a>
   <b>0.0</b>
  </ns:add>
gSOAP赋予**add为一个接口函数的名称，a和b是接口函数参数**
二 命名空间
<?xml version="1.0" encoding="UTF-8"?>
<**SOAP-ENV:Envelope**
 xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/"
 xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/"
 xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
 xmlns:xsd="http://www.w3.org/2001/XMLSchema"
 xmlns:ns="urn:calc">
 <**SOAP-ENV:Body** SOAP-ENV:encodingStyle="http://schemas.xmlsoap.org/soap/encoding/">
  <ns:add>
   <a>0.0</a>
   <b>0.0</b>
  </ns:add>
 <**/SOAP-ENV:Body**>
<**/SOAP-ENV:Envelope**>
  由于标签是我们自定义的，多个XML文件中可能存在重复命名，但表示的含义却不一样，为了解决XML文档中命名的冲突问题 就出现了命名空间。所以需要加上一个namespace来区分多个xml文件之间相同的标签。
使用语法：xmlns:namespace-prefix="namespaceURI"
**xmlns的意思是XML Namespace**
URI是Uniform Resource Identifier 的缩写，作用是**赋予命名空间一个惟一的名称用来表示URI 一个是URL，一个是****URN**，gSOAP中URL和URN都使用了：xmlns:ns="urn:calc 
其中**calc是一个接口文件的文件名**。这句是gsoap定义的默认命名空间有了默认的命名空间，直接是<a>0.0</a>就可以，而不用<ns:a>0.0</ns:a>
关于命名空间的处理代码如下
SOAP_FMAC1 int SOAP_FMAC2 soap_match_namespace(struct soap*, const char *, const char*, size_t n1, size_t n2);
SOAP_FMAC1 int SOAP_FMAC2 soap_set_namespaces(struct soap*, const struct Namespace*);
SOAP_FMAC1 void SOAP_FMAC2 soap_set_local_namespaces(struct soap*);
SOAP_FMAC1 void SOAP_FMAC2 soap_pop_namespace(struct soap*);
SOAP_FMAC1 struct soap_nlist* SOAP_FMAC2 soap_push_namespace(struct soap*, const char *,const char *);
SOAP_FMAC1 const char* SOAP_FMAC2 soap_current_namespace(struct soap *soap, const char *tag);
SOAP_FMAC1 struct soap_nlist* SOAP_FMAC2 soap_lookup_ns(struct soap *soap, const char *tag, size_t n);
三 编码
gSOAP默认使用的是使用UTF-8编码
关于编码的处理函数
SOAP_FMAC1 int SOAP_FMAC2 soap_pututf8(struct soap*, unsigned long);
SOAP_FMAC1 soap_wchar SOAP_FMAC2 soap_getutf8(struct soap*);
SOAP_FMAC1 int SOAP_FMAC2 soap_putbase64(struct soap*, const unsigned char*, int);
SOAP_FMAC1 unsigned char* SOAP_FMAC2 soap_getbase64(struct soap*, int*, int);
SOAP_FMAC1 int SOAP_FMAC2 soap_puthex(struct soap*, const unsigned char*, int);
SOAP_FMAC1 unsigned char* SOAP_FMAC2 soap_gethex(struct soap*, int*);
XML指令处理（XML PI）
目的是为了给XML解析器提供额外的信息。这里处理XML文件的编码信息 <?xml version="1.0" encoding="UTF-8"?>
gSOAP关于指令处理代码如下
soap_get_pi(struct soap *soap)
{ char buf[64];
  register char *s = buf;
  register int i = sizeof(buf);
  register soap_wchar c = soap_getchar(soap);
  /* This is a quick way to parse XML PI and we could use a callback instead to
   * enable applications to intercept processing instructions */
  while ((int)c != EOF && c != '?')
  { if (--i > 0)
    { if (soap_blank(c))
        c = ' ';
      *s++ = (char)c;
    }
    c = soap_getchar(soap);
  }
  *s = '\0';
  DBGLOG(TEST, SOAP_MESSAGE(fdebug, "XML PI <?%s?>\n", buf));
  if (!strncmp(buf, "xml ", 4))
  { s = strstr(buf, " encoding=");
    if (s && s[10])
    { if (!soap_tag_cmp(s + 11, "iso-8859-1*")
       || !soap_tag_cmp(s + 11, "latin1*"))
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Switching to latin1 encoding\n"));
        soap->mode |= SOAP_ENC_LATIN;
      }
      else if (!soap_tag_cmp(s + 11, "utf-8*"))
      { DBGLOG(TEST, SOAP_MESSAGE(fdebug, "Switching to utf-8 encoding\n"));
        soap->mode &= ~SOAP_ENC_LATIN;
      }
    }
  }
  if ((int)c != EOF)
    c = soap_getchar(soap);
  return c;
}
四 XML Schema
定义了文档中的元素，属性，数据类型，默认值等
因为WSDL 是一种基于 schema 的语言，所以这个看WDSL文档更清楚
 <schema targetNamespace="urn:calc"
  xmlns:SOAP-ENV="http://schemas.xmlsoap.org/soap/envelope/"
  xmlns:SOAP-ENC="http://schemas.xmlsoap.org/soap/encoding/"
  xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
  xmlns:xsd="http://www.w3.org/2001/XMLSchema"
  xmlns:ns="urn:calc"
  xmlns="http://www.w3.org/2001/XMLSchema"
  elementFormDefault="unqualified"
  attributeFormDefault="unqualified">
  <import namespace="http://schemas.xmlsoap.org/soap/encoding/"/>
 </schema>
<message name="addRequest">
 <part name="a" type="xsd:double"/><!-- ns__add::a -->
 <part name="b" type="xsd:double"/><!-- ns__add::b -->
</message>
<message name="addResponse">
 <part name="result" type="xsd:double"/><!-- ns__add::result -->
</message>
定义简易元素的语法：
<part name="xxx" type="yyy"/>
 上面示例中a,b,result指的是元素的名称 double是数据类型,xsd:double 表示是XMLSchema
内建的数据类型
elementFormDefault="unqualified"
elementFormDefault这一属性用来指示 XML Schema 处理程序把这个 XML schema 中定义的元素或者类型放到哪个命名空间
Unqualified 表示目标命名空间下的这个属性不需要带命名空间前缀，子元素不必使用命名空间前缀
  attributeFormDefault="unqualified"
表示目标命名空间下的这个属性不需要带命名空间前缀.
<import namespace="http://schemas.xmlsoap.org/soap/encoding/"/> 表示需要导入的命名空间
 五 xml数据的绑定
soapcalcService.h文件中的接口声明函数
virtual int add(double a, double b, double *result) SOAP_PURE_VIRTUAL;
soapcalcService.cpp文件中的实现函数
int calcService::add(double a, double b, double *result)
{ 
*result = a + b;
return SOAP_OK;
} 
Req.Xml文件中 body元素中的内容
  <ns:add>
   <a>0.0</a>
   <b>0.0</b>
  </ns:add>
res.xml文件中 body元素中的内容
<ns:addResponse>
   <result>0.0</result>
  </ns:addResponse>
WSDL文件中add部分
<message name="addRequest">
 <part name="a" type="xsd:double"/><!-- ns__add::a -->
 <part name="b" type="xsd:double"/><!-- ns__add::b -->
</message>
<message name="addResponse">
 <part name="result" type="xsd:double"/><!-- ns__add::result -->
</message>
存根文件soapStub.h文件中
struct ns__add
{
public:
double a; /* required element of type xsd:double */
double b; /* required element of type xsd:double */
};
struct ns__addResponse
{
public:
double *result; /* SOAP 1.2 RPC return element (when namespace qualified) *//* optional element of type xsd:double */
};
对比上述各个文件关于add的描述
目的是将数据解析到一个对象中，这些对象可以应用到我们的程序中。不需要编写代码搜索xml节点元素，只需要调用add这个函数，就可以得到double返回值
数据绑定，就是将xml数据转换成C++对象，这个C++对象可以表达出xml的数据块
六 xml数据的传输
soapC.cpp文件中关于add部分
add发送请求部分
soap_default_ns__add
soap_serialize_ns__add
soap_out_ns__add
soap_in_ns__add
soap_put_ns__add
soap_get_ns__add
soap_instantiate_ns__add
soap_copy_ns__add
add应答部分
soap_default_ns__addResponse
soap_serialize_ns__addResponse
soap_out_ns__addResponse
soap_in_ns__addResponse
soap_put_ns__addResponse
soap_get_ns__addResponse
soap_instantiate_ns__addResponse
soap_copy_ns__addResponse
每个函数都类似以上格式 一个发送请求的 default,serialize,out，in，put,get,instantiate,copy。
