# VC使用MSXML解析XML文档 - 深之JohnChen的专栏 - CSDN博客

2006年04月17日 17:00:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：8387


**VC使用MSXML解析XML文档**

第一部分：DOM解析：

      概述：DOM解析将会把一个完整的XML文档读进来，生成一个结构树。这样会要把XML文档全部都加载到内在中。所以解析起来的速度会要慢一些。

    1、如何加载xml文件：

         //创建DOM,加载XML文档

         MSXML::IXMLDOMDocumentPtr pCommandDoc;

         pCommandDoc.CreateInstance(__uuidof(MSXML::DOMDocument));

          pCommandDoc->put_async(VARIANT_FALSE);

          pCommandDoc->put_validateOnParse(VARIANT_FALSE);

          pCommandDoc->put_resolveExternals(VARIANT_FALSE);

          pCommandDoc->put_preserveWhiteSpace(VARIANT_TRUE);

          pCommandDoc->load(file.GetBuffer(0));

     2、在XML文档中查找指定的结点：

      //找到

     MSXML::IXMLDOMNodePtr pRootNode=pCommandDoc->selectSingleNode("root/record");

       if (pRootNode==NULL)

     {

          return ;

      }

    3、得到XML文档中，结点的属性

        CString strTemp;

     MSXML::IXMLDOMNamedNodeMapPtr pAttrs = NULL;

     pRootNode->get_attributes(&pAttrs);

     if (pAttrs==NULL)

     {

          return;

     }

     MSXML::IXMLDOMNodePtr pRequestTypeAttr=pAttrs->getQualifiedItem("name","");

     _bstr_t strRequestType=pRequestTypeAttr->Gettext();

     strTemp=strRequestType.operator char *();

    4、得到结点的内容

    _bstr_t strVisiPort=pNode->Gettext();

    5、设置结点的内容

     HRESULT hr=pNode->put_text(_bstr_t(m_strGatewayPassword));

   6、设置一个属性内容

    IXMLDOMAttribute *pa=NULL;

    bstr = SysAllocString(L"属性1");

    pXMLDom->createAttribute(bstr,&pNode);

    var = VariantString(L"strin");

    pa->put_value(var);

    pRoot->setAttributeNode(pa, &pa1);

第二部分、如何使用SAX解析

        概述：SAX使用的是加载式的，将会把XML文档分断，加载到内存中。使用事件通知的方式，来表示找到结点。好像没有写文档的能力吧。它的速度要比DOM快不少。

       使用SAX的时候，就需要重载MSXML4.0中的一个接口ISAXContentHandler。

      有几个函数重载了之后，当找到了结点之后，就会回调这一些函数。

对于这一种SAX解析的模式有一些不太了解如何应用。只是感觉这种方式可能不太好用于那种

对结构很重要的xml文档的解析　。

**vc中操作Xml--使用CMarkup类**

[http://www.firstobject.com/dn_markup.htm](http://www.firstobject.com/dn_markup.htm)

DotNet中使用xml相对比较轻松，在vc中操作xml，有些繁琐。最近在网络上得知CMarkup比较好用，于是试了下，还好，就把部分例子写出来。以后有时间的话，再写清楚点

//----------UserInfo.xml--------------

<?xml version="1.0" encoding="UTF-8" ?>

<UserInfo>

<UserID>**luo**</UserID>

<UserID>**lin**</UserID>

</UserInfo>

1。生成UserInfo.xml

CMarkup xml;

xml.SetDoc("<?xml version=/"1.0/" encoding=/"UTF-8/"?>/r/n");

xml.AddElem("UserInfo");

xml.IntoElem();

xml.AddElem("UserID","luo");

xml.AddElem("UserID","lin");

xml.OutOfElem();

xml.Save("UserInfo.xml");

2。浏览特定元素

//----查看所有UserID----

CMarkup xml;

xml.Load("UserInfo.xml");

BOOL bFind = true;

xml.ResetMainPos();

while (xml.FindChildElem("UserID");)

{

       //此时接点还是父接点

         CString strTagName = _T("");

         CString strData = _T("");

         strTagName = xml.GetChildTagName();

         strData = xml.GetChildData();

         TRACE("/n---tagName:%s,Data:%s--/n",strTagName,strData);

}

       /********************这样也可以******************

       CMarkup xml;

       xml.Load("UserInfo.xml");

       BOOL bFind = true;

       xml.ResetMainPos();

       while (xml.FindChildElem("UserID");)

       {

                     xml.IntoElem();

                     CString strTagName = _T("");

                     CString strData = _T("");

                     strTagName = xml.GetTagName();

                     strData = xml.GetData();

                     TRACE("/n---tagName:%s,Data:%s--/n",strTagName,strData);

                     xml.OutOfElem();

       }*/

       /**************这样也可以**********************

       CMarkup xml;

       xml.Load("UserInfo.xml");

       BOOL bFind = true;

       xml.ResetMainPos();

       xml.FindElem();  //UserInfo

       xml.IntoElem();

       while (xml.FindElem("UserID");)

       {

                     CString strTagName = _T("");

                     CString strData = _T("");

                     strTagName = xml.GetTagName();

                     strData = xml.GetData();

                     TRACE("/n---tagName:%s,Data:%s--/n",strTagName,strData);

       }

       */

//-------------结果----------

---tagName:UserID,Data:luo--

---tagName:UserID,Data:lin—

3。修改

//--------把UserID为”luo”改为”flypigluo”-----------

BOOL bLoadXml = false;

CMarkup xml;

bLoadXml = xml.Load("UserInfo.xml");

       if (bLoadXml)

       {

              CString strUserID = _T("");

              xml.ResetMainPos();

              xml.FindElem();

              xml.IntoElem();

              while (xml.FindElem("UserID"))

              {

                     strUserID = xml.GetData();

                     if (strUserID=="luo")

                     {

                            xml.SetData(CString("flypig")+strUserID);

                            xml.Save("UserInfo.xml");

                            break;

                     }

              }

       }


4。添加

4。1   添在最后面（使用的是AddElem）

//加在最后面

       BOOL bLoadXml = false;

       CMarkup xml;

       bLoadXml = xml.Load("UserInfo.xml");

       if (bLoadXml)

       {

              xml.ResetMainPos();

              xml.FindElem();

              xml.IntoElem();

              xml.AddElem("UserID","luoluo");

              xml.OutOfElem();

              xml.Save("UserInfo.xml");

       }

//--------原来为------

<?xml version="1.0" encoding="UTF-8" ?>

<UserInfo>

<UserID>**luo**</UserID>

<UserID>**lin**</UserID>

</UserInfo>

//---------添加后为-------------

<?xml version="1.0" encoding="UTF-8" ?>

<UserInfo>

<UserID>**luo**</UserID>

<UserID>**lin**</UserID>

<UserID>**luoluo**</UserID>

</UserInfo>

4。2  加在最前面（使用InsertElem）

BOOL bLoadXml = false;

CMarkup xml;

bLoadXml = xml.Load("UserInfo.xml");

if (bLoadXml)

{

       xml.ResetMainPos();

       xml.FindElem();

       xml.IntoElem();

       xml.InsertElem("UserID","AddUserIDHead");

       xml.OutOfElem();

       xml.Save("UserInfo.xml");

}

//----原来为—

<?xml version="1.0" encoding="UTF-8" ?>

<UserInfo>

<UserID>**luo**</UserID>

<UserID>**lin**</UserID>

</UserInfo>

//------修改后-----

<?xml version="1.0" encoding="UTF-8" ?>

<UserInfo>

<UserID>**AddUserIDHead**</UserID>

<UserID>**luo**</UserID>

<UserID>**lin**</UserID>

</UserInfo>

5。删除

CMarkup xml;

xml.Load("UserInfo.xml");

BOOL bFind = true;

xml.ResetMainPos();      

while (bFind)

{

       bFind = xml.FindChildElem("UserID");

       if (bFind)

       {

              //此时接点还是父接点

              CString strData = _T("");

              strData = xml.GetChildData();

              if (strData=="luo")

              {

                     xml.RemoveChildElem();

                     xml.Save("UserInfo.xml");

                     break;

              }

       }

}

6。查找

见第二点浏览。类似


上面的例子里面没有属性这些东西，下面的一个例子是有属性的

//UserInfo1.xml

<?xml version="1.0" encoding="UTF-8" ?>

<UserInfo>

<UserUserID="**UserID00**" UserPwd="**UserPwd00**" UserLevel="**158**" />

<UserUserID="**UserID01**" UserPwd="**UserPwd01**" UserLevel="**162**" />

<UserUserID="**UserID02**" UserPwd="**UserPwd02**" UserLevel="**165**" />

<UserUserID="**UserID03**" UserPwd="**UserPwd03**" UserLevel="**168**" />

<UserUserID="**UserID04**" UserPwd="**UserPwd04**" UserLevel="**171**" />

<UserUserID="**UserID05**" UserPwd="**UserPwd05**" UserLevel="**175**" />

<UserUserID="**UserID06**" UserPwd="**UserPwd06**" UserLevel="**178**" />

<UserUserID="**UserID07**" UserPwd="**UserPwd07**" UserLevel="**181**" />

<UserUserID="**UserID08**" UserPwd="**UserPwd08**" UserLevel="**184**" />

<UserUserID="**UserID09**" UserPwd="**UserPwd09**" UserLevel="**188**" />

</UserInfo>

7。产生

CString strID = _T("");

       CString strPwd = _T("");

       CMarkup xml;

       xml.SetDoc("<?xml version=/"1.0/" encoding=/"UTF-8/"?>");

       xml.AddElem("UserInfo");

       xml.IntoElem();

       for (int i=0; i<10; i++)

       {

              //产生一个随机数，作为权限

              srand(UINT(time(NULL)+i));

              int nLevel = rand()%256;

              strID.Format("UserID%02d",i);

              strPwd.Format("UserPwd%02d",i);

              xml.AddElem("User");

              xml.AddAttrib("UserID",strID);

              xml.AddAttrib("UserPwd",strPwd);

              xml.AddAttrib("UserLevel",nLevel);

       }

       xml.OutOfElem();

       xml.Save("UserInfo1.xml");

8。查找

CMarkup xml;

BOOL bLoad = false;

BOOL bFind = false;

bLoad = xml.Load("UserInfo1.xml");

       if (bLoad)

       {

              CString strID;

              CString strPwd;

              xml.ResetMainPos();

              xml.FindElem();    //UserInfo

              while (xml.FindChildElem("User"))

              {

                     strID = xml.GetChildAttrib("UserID");

                     strPwd = xml.GetChildAttrib("UserPwd");

                     TRACE("/n----id:%s,pwd:%s-------/n",strID,strPwd);

                     if (strID=="UserID00"&&strPwd=="UserPwd00")

                     {

                            bFind = true;

                            break;

                     }

              }

       }

       if (bFind)

       {

              TRACE("/n---- find------/n");

       }

       else

       {

              TRACE("/n----no find------/n");

       }

9。修改

CMarkup xml;

       BOOL bLoad = false;

       BOOL bFind = false;

       bLoad = xml.Load("UserInfo1.xml");

       if (bLoad)

       {

              CString strID;

              CString strPwd;

              xml.ResetMainPos();

              xml.FindElem();    //UserInfo

              while (xml.FindChildElem("User"))

              {

                     strID = xml.GetChildAttrib("UserID");

                     strPwd = xml.GetChildAttrib("UserPwd");

                     TRACE("/n----id:%s,pwd:%s-------/n",strID,strPwd);

                     if (strID=="UserID00"&&strPwd=="UserPwd00")

                     {

                            bFind = true;

                            xml.SetChildAttrib("UserID",strID+CString("Modify"));

                            xml.Save("UserInfo1.xml");

                            break;

                     }

              }

       }

       if (bFind)

       {

              TRACE("/n---- find------/n");

       }

       else

       {

              TRACE("/n----no find------/n");

       }

