# vc6里调用WebService - 深之JohnChen的专栏 - CSDN博客

2009年05月26日 15:31:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4191


vc6里调用WebService

1、安装soapsdk3.0开发包

([http://download.microsoft.com/download/2/e/0/2e068a11-9ef7-45f5-820f-89573d7c4939/soapsdk.exe](http://download.microsoft.com/download/2/e/0/2e068a11-9ef7-45f5-820f-89573d7c4939/soapsdk.exe))

2、写代码

// WSWrapper.h: interface for the WSWrapper class.

//

//////////////////////////////////////////////////////////////////////

#if !defined(AFX_WSWRAPPER_H__842F2D22_1EC1_4391_922D_C4E51CC65077__INCLUDED_)

#define AFX_WSWRAPPER_H__842F2D22_1EC1_4391_922D_C4E51CC65077__INCLUDED_

#if _MSC_VER > 1000

#pragma once

#endif // _MSC_VER > 1000

#import "msxml4.dll" 

#import "C:/Program Files/Common Files/MSSoap/Binaries/mssoap30.dll" exclude("IStream", "IErrorInfo", "ISequentialStream", "_LARGE_INTEGER", "_ULARGE_INTEGER", "tagSTATSTG", "_FILETIME")

#include <string>

#include <Windows.h>

using namespace MSXML2;

using namespace MSSOAPLib30;

using std::string;

class WSWrapper

{

public:

WSWrapper(const char *wsURL, const char *wsNameSapce,const char *wsMethodName);

virtual ~WSWrapper();

string MisUserLogin(const string &strUserName,const string &strUserPassWord);

private:

const string _wsURL;

const string _wsNameSapce;

const string _wsMethodName;

};

#endif // !defined(AFX_WSWRAPPER_H__842F2D22_1EC1_4391_922D_C4E51CC65077__INCLUDED_)

// WSWrapper.cpp: implementation of the WSWrapper class.

//

//////////////////////////////////////////////////////////////////////

#include "WSWrapper.h"

//////////////////////////////////////////////////////////////////////

// Construction/Destruction

//////////////////////////////////////////////////////////////////////

WSWrapper::WSWrapper(const char *wsURL, const char *wsNameSapce, const char *wsMethodName): _wsURL(wsURL), _wsNameSapce(wsNameSapce),_wsMethodName(wsMethodName)

{

}

WSWrapper::~WSWrapper()

{

}

string WSWrapper::MisUserLogin(const string &strUserName,const string &strUserPassWord)

{

try

{

HRESULT hr = CoInitialize(NULL);//初始化com环境

if(FAILED(hr))

{

//出错了

}

ISoapSerializerPtr Serializer;

ISoapReaderPtr Reader;

ISoapConnectorPtr Connector;

//连接到WebService

hr = Connector.CreateInstance(__uuidof(HttpConnector30));

if(FAILED(hr))

{

//创建com对象出错，一般是因为没有安装com

}

Connector->Property["EndPointURL"] = _wsURL.c_str(); 

hr = Connector->Connect();

if(FAILED(hr))

{

//连接失败

return "error";

}

Connector->Property["SoapAction"] = (_wsNameSapce + _wsMethodName).c_str();

//开始创建webservice的请求Soap包

hr = Connector->BeginMessage();

hr = Serializer.CreateInstance(__uuidof(SoapSerializer30));

if(FAILED(hr))

{

//创建com对象出错，一般是因为没有安装com

}

hr = Serializer->Init(_variant_t((IUnknown*)Connector->InputStream));

Serializer->StartEnvelope("SOAP", "http://schemas.xmlsoap.org/soap/envelope/", "");

Serializer->SoapAttribute("xsi", "", "http://www.w3.org/2001/XMLSchema-instance", "xmlns");

Serializer->SoapAttribute("xsd", "", "http://www.w3.org/2001/XMLSchema", "xmlns");

Serializer->StartBody(L"NONE");

Serializer->StartElement(_wsMethodName.c_str(), _wsNameSapce.c_str(), "NONE", "");

Serializer->StartElement(L"LoginName", _wsNameSapce.c_str(), "NONE", "");

Serializer->SoapAttribute("xsi:type", "", "xsd:string", "");

Serializer->WriteString(strUserName.c_str());

Serializer->EndElement();

Serializer->StartElement(L"PassWord", _wsNameSapce.c_str(), "NONE", "");

Serializer->SoapAttribute("xsi:type", "", "xsd:string", "");

Serializer->WriteString(strUserPassWord.c_str());

Serializer->EndElement();

Serializer->EndElement();

Serializer->EndBody();

Serializer->EndEnvelope();

hr = Connector->raw_EndMessage();

if(FAILED(hr))

{

//发送SOAP MESSAGE 失败

return "error";

}

hr = Connector->EndMessage();

//解析返回的soap包

hr = Reader.CreateInstance(__uuidof(SoapReader30));

if(FAILED(hr))

{

//创建com对象出错，一般是因为没有安装com

}

VARIANT_BOOL bRead;

bRead = Reader->Load(_variant_t((IUnknown*)Connector->OutputStream), "");

string strResultXml((const char*)Reader->Body->xml); 

string strResult;

if(Reader->GetRpcResult())

{

string strResult4 = (const char*)Reader->RpcResult->xml;

strResult = (const char*)Reader->RpcResult->text;

}

else

{

if(strResultXml.c_str()>0)

{

strResult = "RpcNoResult And BodyHasResult";

}

else

{

strResult = "RpcNoResult And BodyNoResult";

}

}

return strResult;

}

catch(ISoapConnectorPtr Connector)

{

//got a exception 

}

return "error";

}


3、使用方法

//调用[http://192.168.10.186:89/MisUserWS.asmx](http://192.168.10.186:89/MisUserWS.asmx)下的“MIS_UserLogin”WEB服务。

//查看WSDL内容[http://192.168.10.186:89/MisUserWS.asmx?WSDL](http://192.168.10.186:89/MisUserWS.asmx?WSDL)

<?xml version="1.0" encoding="utf-8" ?>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:definitions xmlns:soap="**http://schemas.xmlsoap.org/wsdl/soap/**" xmlns:tm="**http://microsoft.com/wsdl/mime/textMatching/**" xmlns:soapenc="**http://schemas.xmlsoap.org/soap/encoding/**" xmlns:mime="**http://schemas.xmlsoap.org/wsdl/mime/**" xmlns:tns="**http://tempuri.org/**" xmlns:s="**http://www.w3.org/2001/XMLSchema**" xmlns:soap12="**http://schemas.xmlsoap.org/wsdl/soap12/**" xmlns:http="**http://schemas.xmlsoap.org/wsdl/http/**" targetNamespace="**http://tempuri.org/**" xmlns:wsdl="**http://schemas.xmlsoap.org/wsdl/**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:types>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:schema elementFormDefault="**qualified**" targetNamespace="**http://tempuri.org/**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**MIS_UserLogin**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**LoginName**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**PassWord**" type="**s:string**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**MIS_UserLoginResponse**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**MIS_UserLoginResult**" type="**tns:MisUserInfo**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType name="**MisUserInfo**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**YPassWord**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**YUserName**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**WorkExePath**" type="**s:string**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**ID**" type="**s:int**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**uname**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**upassword**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**utype**" type="**s:string**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**EmployeeID**" type="**s:int**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**IsLocked**" type="**s:boolean**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**TeamID**" type="**s:int**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**LastLoginTime**" type="**s:dateTime**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**DepartmentID**" type="**s:int**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**GroupID**" type="**s:int**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**Comm_Input_By**" type="**s:int**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**Comm_Input_Date**" type="**s:dateTime**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**Comm_Modi_By**" type="**s:int**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**Comm_Modi_Date**" type="**s:dateTime**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**RoleID**" type="**s:int**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**MachineIP**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**WorkStartPath**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**WorkEndPath**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**Memory**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**SystemUser**" type="**s:string**" />

</s:sequence>

</s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**UserLogin**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**LoginName**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**PassWord**" type="**s:string**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**UserLoginResponse**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**UserLoginResult**" type="**s:int**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**UserLoginOut**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**LoginName**" type="**s:string**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**UserLoginOutResponse**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**UserLoginOutResult**" type="**s:int**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**MIS_Select_Userconfiguration**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**LoginID**" type="**s:int**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**MIS_Select_UserconfigurationResponse**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**MIS_Select_UserconfigurationResult**" type="**tns:MisUserInfo**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**MIS_Set_Userconfiguration**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**MachineIP**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**WorkStartPath**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**WorkEndPath**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**SystemUser**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**WorkExePath**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**YUserName**" type="**s:string**" />

<s:elementminOccurs="**0**" maxOccurs="**1**" name="**YPassWord**" type="**s:string**" />

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**id**" type="**s:int**" />

</s:sequence>

</s:complexType>

</s:element>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:element name="**MIS_Set_UserconfigurationResponse**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:complexType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<s:sequence>

<s:elementminOccurs="**1**" maxOccurs="**1**" name="**MIS_Set_UserconfigurationResult**" type="**s:int**" />

</s:sequence>

</s:complexType>

</s:element>

</s:schema>

</wsdl:types>

[**+**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**MIS_UserLoginSoapIn**">

<wsdl:partname="**parameters**" element="**tns:MIS_UserLogin**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**MIS_UserLoginSoapOut**">

<wsdl:partname="**parameters**" element="**tns:MIS_UserLoginResponse**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**UserLoginSoapIn**">

<wsdl:partname="**parameters**" element="**tns:UserLogin**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**UserLoginSoapOut**">

<wsdl:partname="**parameters**" element="**tns:UserLoginResponse**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**UserLoginOutSoapIn**">

<wsdl:partname="**parameters**" element="**tns:UserLoginOut**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**UserLoginOutSoapOut**">

<wsdl:partname="**parameters**" element="**tns:UserLoginOutResponse**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**MIS_Select_UserconfigurationSoapIn**">

<wsdl:partname="**parameters**" element="**tns:MIS_Select_Userconfiguration**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**MIS_Select_UserconfigurationSoapOut**">

<wsdl:partname="**parameters**" element="**tns:MIS_Select_UserconfigurationResponse**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**MIS_Set_UserconfigurationSoapIn**">

<wsdl:partname="**parameters**" element="**tns:MIS_Set_Userconfiguration**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:message name="**MIS_Set_UserconfigurationSoapOut**">

<wsdl:partname="**parameters**" element="**tns:MIS_Set_UserconfigurationResponse**" />

</wsdl:message>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:portType name="**MisUserWSSoap**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_UserLogin**">

<wsdl:documentation xmlns:wsdl="**http://schemas.xmlsoap.org/wsdl/**">**用户登录**</wsdl:documentation>

<wsdl:inputmessage="**tns:MIS_UserLoginSoapIn**" />

<wsdl:outputmessage="**tns:MIS_UserLoginSoapOut**" />

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**UserLogin**">

<wsdl:documentation xmlns:wsdl="**http://schemas.xmlsoap.org/wsdl/**">**用户登录即保存信息到临时表里,判断是否重复登录:0用户不存在,1用户已登录,2临时表已存在并插入用户记录,3创建临时表并插入用户记录**</wsdl:documentation>

<wsdl:inputmessage="**tns:UserLoginSoapIn**" />

<wsdl:outputmessage="**tns:UserLoginSoapOut**" />

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**UserLoginOut**">

<wsdl:documentation xmlns:wsdl="**http://schemas.xmlsoap.org/wsdl/**">**用户退出,即删除临时表记录.返回 0成功,-1 表中无此用户,-1 表不存在**</wsdl:documentation>

<wsdl:inputmessage="**tns:UserLoginOutSoapIn**" />

<wsdl:outputmessage="**tns:UserLoginOutSoapOut**" />

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_Select_Userconfiguration**">

<wsdl:documentation xmlns:wsdl="**http://schemas.xmlsoap.org/wsdl/**">**查找用户初始配置信息**</wsdl:documentation>

<wsdl:inputmessage="**tns:MIS_Select_UserconfigurationSoapIn**" />

<wsdl:outputmessage="**tns:MIS_Select_UserconfigurationSoapOut**" />

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_Set_Userconfiguration**">

<wsdl:documentation xmlns:wsdl="**http://schemas.xmlsoap.org/wsdl/**">**设置用户初始配置信息**</wsdl:documentation>

<wsdl:inputmessage="**tns:MIS_Set_UserconfigurationSoapIn**" />

<wsdl:outputmessage="**tns:MIS_Set_UserconfigurationSoapOut**" />

</wsdl:operation>

</wsdl:portType>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:binding name="**MisUserWSSoap**" type="**tns:MisUserWSSoap**">

<soap:bindingtransport="**http://schemas.xmlsoap.org/soap/http**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_UserLogin**">

<soap:operationsoapAction="**http://tempuri.org/MIS_UserLogin**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**UserLogin**">

<soap:operationsoapAction="**http://tempuri.org/UserLogin**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**UserLoginOut**">

<soap:operationsoapAction="**http://tempuri.org/UserLoginOut**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_Select_Userconfiguration**">

<soap:operationsoapAction="**http://tempuri.org/MIS_Select_Userconfiguration**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_Set_Userconfiguration**">

<soap:operationsoapAction="**http://tempuri.org/MIS_Set_Userconfiguration**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

</wsdl:binding>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:binding name="**MisUserWSSoap12**" type="**tns:MisUserWSSoap**">

<soap12:bindingtransport="**http://schemas.xmlsoap.org/soap/http**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_UserLogin**">

<soap12:operationsoapAction="**http://tempuri.org/MIS_UserLogin**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap12:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap12:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**UserLogin**">

<soap12:operationsoapAction="**http://tempuri.org/UserLogin**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap12:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap12:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**UserLoginOut**">

<soap12:operationsoapAction="**http://tempuri.org/UserLoginOut**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap12:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap12:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_Select_Userconfiguration**">

<soap12:operationsoapAction="**http://tempuri.org/MIS_Select_Userconfiguration**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap12:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap12:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:operation name="**MIS_Set_Userconfiguration**">

<soap12:operationsoapAction="**http://tempuri.org/MIS_Set_Userconfiguration**" style="**document**" />

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:input>

<soap12:bodyuse="**literal**" />

</wsdl:input>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:output>

<soap12:bodyuse="**literal**" />

</wsdl:output>

</wsdl:operation>

</wsdl:binding>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:service name="**MisUserWS**">

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:port name="**MisUserWSSoap**" binding="**tns:MisUserWSSoap**">

<soap:addresslocation="**http://192.168.10.186:89/MisUserWS.asmx**" />

</wsdl:port>

[**-**](http://192.168.10.186:89/MisUserWS.asmx?WSDL#)<wsdl:port name="**MisUserWSSoap12**" binding="**tns:MisUserWSSoap12**">

<soap12:addresslocation="**http://192.168.10.186:89/MisUserWS.asmx**" />

</wsdl:port>

</wsdl:service>

</wsdl:definitions>

//VC 调用WEB service

CString strResult = "",strWebSercivePath = "[http://192.168.10.186:89/MisUserWS.asmx](http://192.168.10.186:89/MisUserWS.asmx)

",strNamespace = "http://tempuri.org/";

WSWrapper wsWrapper(strWebSercivePath,strNamespace,"MIS_UserLogin");

string strName = "admin";

string strPassword = "123";

string strResult = wsWrapper.MisUserLogin(strName,strPassword);

4、注意事项

1）、查看WSDL内容，分析出WS的服务名（函数名）、参数列表。

2）、出现“webservice 服务器未能识别 HTTP 头 SOAPAction 的值…”问题，一般是由于命名空间不对，默认的命名空间是“[http://tempuri.org/](http://tempuri.org/)”。

3）、如果想分析详细的错误信息，可以查看ISoapReaderPtr Reader; (const char*)Reader->Body->xml的内容，得到详细的信息，具体操作请参考上面的代码。

4）、(const char*)Reader->RpcResult->text 表示接受返回信息；(const char*)Reader->RpcResult->xml，表示接受得到信息，以XML格式存储。可以通过CMarkup类得到每个项的内容，可以映射到一个对应的结构体，从而得到所有返回的信息。

