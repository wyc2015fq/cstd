# 利用MSXML解析XML文本(VC) - 深之JohnChen的专栏 - CSDN博客

2006年04月17日 16:55:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：2458


一、引言

　　当前Web上流行的剧本语言是以HTML为主的语言结构，HTML是一种标记语言，而不是一种编程语言，主要的标记是针对显示，而不是针对文档内容本身结构的描述的。也就是说，机器本身是不能够解析它的内容的，所以就出现了XML语言。XML (eXtensible Markup Language)语言是SGML语言的子集，它保留了SGML主要的使用功能，同时大大缩减了SGML的复杂性。XML语言系统建立的目的就是使它不仅能够表示文档的内容，而且可以表示文档的结构，这样在同时能够被人类理解的同时，也能够被机器所理解。XML要求遵循一定的严格的标准。XML分析程序比HTML浏览器更加要挑剔语法和结构，XML要求正在创建的网页正确的使用语法和结构，而不是象HTML一样，通过浏览器推测文档中应该是什么东西来实现HTML的显示，XML使得分析程序不论在性能还是稳定性方面都更容易实现。XML文档每次的分析结果都是一致的，不象HTML，不同的浏览器可能对同一个HTML作出不同的分析和显示。

　　同时因为分析程序不需要花时间重建不完整的文档，所以它们能比同类HTML能更有效地执行其任务。它们能全力以赴地根据已经包含在文档中的那个树结构建造出相应的树来，而不用在信息流中的混合结构的基础上进行显示。XML标准是对数据的处理应用，而不是只针对Web网页的。任何类型的应用都可以在分析程序的上面进行建造，浏览器只是XML的一个小的组成部分。当然，浏览仍旧极其重要，因为它为XML工作人员提供用于阅读信息的友好工具。但对更大的项目来说它就不过是一个显示窗口。因为XML具有严格的语法结构，所以我们甚至可以用XML来定义一个应用层的通讯协议，比如互联网开放贸易协议(Internet Open Trading Protocol)就是用XML来定义的。从某种意义上说，以前我们用BNF范式定义的一些协议和格式从原则上说都可以用XML来定义。实际上，如果我们有足够的耐心，我们完全可以用XML来定义一个C++语言的规范。

　　当然，XML允许大量HTML样式的形式自由的开发，但是它对规则的要求更加严格。XML主要有三个要素：DTD(Document Type Declaration——文档类型声明)或XML Schema(XML大纲)、XSL(eXtensible Stylesheet Language——可扩展样式语言)和XLink(eXtensible Link Language——可扩展链接语言)。DTD和XML大纲规定了XML文件的逻辑结构，定义了XML文件中的元素、元素的属性以及元素和元素的属性之间的关系；Namespace(名域)实现统一的XML文档数据表示以及数据的相互集成；XSL是用于规定XML文档呈现样式的语言，它使得数据与其表现形式相互独立，比如XSL能使Web浏览器改变文档的表示法，例如数据的显示顺序的变化，不需要再与服务器进行通讯。通过改变样式表，同一个文档可以显示得更大，或者经过折叠只显示外面得一层，或者可以变为打印得格式。而XLink将进一步扩展目前Web上已有的简单链接。

　二、实现XML解析的说明

　　当然，从理论上说，根据XML的格式定义，我们可以自己编写一个XML的语法分析器，但是实际上微软已经给我们提供了一个XML语法解析器，如果你安装了IE5.0以上版本的话，实际上你就已经安装了XML语法解析器。可以从微软站点(www.microsoft.com)下载最新的MSXML的SDK和Parser文件。它是一个叫做MSXML.DLL的动态链接库，最新版本为msxml3，实际上它是一个COM对象库，里面封装了所有进行XML解析所需要的所有必要的对象。因为COM是一种以二进制格式出现的和语言无关的可重用对象。所以你可以用任何语言(比如VB，VC，DELPHI，C++ Builder甚至是剧本语言等等)对它进行调用，在你的应用中实现对XML文档的解析。下面的关于XML文档对象模型的介绍是基于微软最新的msxml3为基础进行的。

　　三、XML文档对象(XML DOM)模型分析

　　XML DOM对象提供了一个标准的方法来操作存储在XML文档中的信息，DOM应用编程接口(API)用来作为应用程序和XML文档之间的桥梁。

　　DOM可以认为是一个标准的结构体系用来连接文档和应用程序(也可以是剧本语言)。MSXML解析器允许你装载和创建一个文档，收集文档的错误信息，得到和操作文档中的所有的信息和结构，并把文档保存在一个XML文件中。DOM提供给用户一个接口来装载、到达和操作并序列化XML文档。DOM提供了对存储在内存中的XML文档的一个完全的表示，提供了可以随机访问整个文档的方法。DOM允许应用程序根据MSXML解析器提供的逻辑结构来操作XML文档中的信息。利用MSXML所提供的接口来操作XML。

　　实际上MSXML解析器根据XML文档生成一个DOM树结构，它能够读XML文档并根据XML文档内容创建一个节点的逻辑结构，文档本身被认为是一个包含了所有其他节点的节点。

　　DOM使用户能够把文档看成是一个有结构的信息树，而不是简单的文本流。这样应用程序或者是剧本即使不知道XML的语义细节也能够方便的操作该结构。DOM包含两个关键的抽象：一个树状的层次、另一个是用来表示文档内容和结构的节点集合。树状层次包括了所有这些节点，节点本身也可以包含其他的节点。这样的好处是对于开发人员来说，他可以通过这个层次结构来找到并修改相应的某一个节点的信息。DOM把节点看成是一个通常的对象，这样就有可能创建一个剧本来装载一个文档，然后遍历所有的节点，显示感兴趣的节点的信息。注意节点可以有很多中具体的类型，比如元素、属性和文本都可以认为是一个节点。

　　微软的MSXML解析器读一个XML文档，然后把它的内容解析到一个抽象的信息容器中称为节点(NODES)。这些节点代表文档的结构和内容，并允许应用程序来读和操作文档中的信息而不需要显示的知道XML的语义。在一个文档被解析以后，它的节点能够在任何时候被浏览而不需要保持一定的顺序。

　　对开发人员来说，最重要的编程对象是DOMDocument。DOMDocument对象通过暴露属性和方法来允许你浏览，查询和修改XML文档的内容和结构，每一个接下来的对象暴露自己的属性和方法，这样你就能够收集关于对象实例的信息，操作对象的值和结构，并导航到树的其他对象上去。

　MSXML.DLL所包括的主要的COM接口有：

　　(1)DOMDocument

　　DOMDocument对象是XML DOM的基础，你可以利用它所暴露的属性和方法来允许你浏览、查询和修改XML文档的内容和结构。DOMDocument表示了树的顶层节点。它实现了DOM文档的所有的基本的方法并且提供了额外的成员函数来支持XSL和XSLT。它创建了一个文档对象，所有其他的对象都可以从这个文档对象中得到和创建。

　　(2)IXMLDOMNode

　　IXMLDOMNode是文档对象模型(DOM)中的基本的对象，元素，属性，注释，过程指令和其他的文档组件都可以认为是IXMLDOMNode，事实上，DOMDocument对象本身也是一个IXMLDOMNode对象。

　　(3)IXMLDOMNodeList

　　IXMLDOMNodeList实际上是一个节点(Node)对象的集合，节点的增加、删除和变化都可以在集合中立刻反映出来，可以通过“for...next”结构来遍历所有的节点。

　　(4)IXMLDOMParseError

　　IXMLDOMParseError接口用来返回在解析过程中所出现的详细的信息，包括错误号，行号，字符位置和文本描述。

　　下面主要描述一个DOMDocument对象的创建过程，这里我们用VC描述创建一个文档对象的过程。

　　HRESULT hr;

　　IXMLDomDocument* pXMLDoc;

　　IXMLDOMNode* pXDN;

　　Hr=CoInitialize(NULL); //COM的初始化

　　//得到关于IXMLDOMDocument接口的指针pXMLDOC。

　　hr=CoCreateInstance(CLSID_DOMDocument,NULL,CLSCTX_INPPROC_SERVER,

　　IID_IXMLDOMDocument,(void**)&pXMLDoc); 

　　//得到关于IXMLDOMNode接口的指针pXDN。

　　hr=pXMLDoc->QueryInterface(IID_IXMLDOMNode,(void**)&pXDN);

　　在MSXML解析器使用过程中，我们可以使用文档中的createElement方法来创建一个节点装载和保存XML文件。通过load或者是loadXML方法可以从一个指定的URL来装载一个XML文档。Load(LoadXML)方法带有两个参数：第一个参数xmlSource表示需要被解析的文档，第二个参数isSuccessful表示文档装载是否成功。Save方法是用来把文档保存到一个指定的位置。Save方法有一个参数destination用来表示需要保存的对象的类型，对象可以是一个文件，一个ASP Response方法，一个XML文档对象，或者是一个能够支持持久保存(persistence)的客户对象。下面是save方法使用的一个简单的例子(具体程序请参见[http://www.swm.com.cn/swm/200101/利用MSXML解析XML文本)。](http://www.swm.com.cn/swm/200101/%E5%88%A9%E7%94%A8MSXML%E8%A7%A3%E6%9E%90XML%E6%96%87%E6%9C%AC)%E3%80%82)

　　同时，在解析过程中，我们需要得到和设置解析标志。利用不同的解析标志，我们可能以不同的方法来解析一个XML文档。XML标准允许解析器验证或者不验证文档，允许不验证文档的解析过程跳过对外部资源的提取。另外，你可能设置标志来表明你是否要从文档中移去多余的空格。

　　为了达到这个目的，DOMDocument对象暴露了下面几个属性，允许用户在运行的时候改变解析器的行为：

　　(1)Async(相对于C++是两个方法，分别为get_async和put_async)

　　(2)ValidateOnparse (相对于C++是两个方法，分别为get_validateOnParse和 put_validateOnParse)

　　(3)ResolveExternals(相对于C++是两个方法，分别为get_ ResolveExternals和put_ ResolveExternals)

　　(4)PersercveWhiteSpace(相对于C++是两个方法，分别为get_ PersercveWhiteSpace和put_ PersercveWhiteSpace)

　　每一个属性可以接受或者返回一个Boolean值。缺省的，anync,validateOnParse,resolveExternals的值为TRUE，perserveWhiteSpace的值跟XML文档的设置有关，如果XML文档中设置了xml:space属性的话，该值为FALSE。

　　同时在文档解析过程中可以收集一些和文档信息的信息，实际上在文档解析过程中可以得到以下的信息：

　　(1)doctype(文档类型)：实际上是和用来定义文档格式的DTD文件。如果XML文档没有相关的DTD文档的话，它就返回NULL。

　　(2)implementation(实现)：表示该文档的实现，实际上就是用来指出当前文档所支持的XML的版本。

　　(3)parseError(解析错误)：在解析过程中最后所发生的错误。

　　(4)readyState(状态信息)：表示XML文档的状态信息，readyState对于异步使用微软的XML解析器来说的重要作用是提高了性能，当异步装载XML文档的时候，你的程序可能需要检查解析的状态，MSXML提供了四个状态，分别为正在状态，已经状态，正在解析和解析完成。

　　(5)url(统一资源定位)：关于正在被装载和解析的XML文档的URL的情况。注意如果该文档是在内存中建立的话，这个属性返回NULL值。

　　在得到文档树结构以后，我们可以操作树中的每一个节点，可以通过两个方法得到树中的节点，分别为nodeFromID和getElementsByTagName。

　　nodeFromID包括两个参数，第一个参数idString用来表示ID值，第二个参数node返回指向和该ID相匹配的NODE节点的接口指针。注意根据XML的技术规定，每一个XML文档中的ID值必须是唯一的而且一个元素(element)仅且只能和一个ID相关联。

　　getElementsByTagName方法有两个参数，第一个参数tagName表示需要查找的元素(Element)的名称，如果tagName为“*”的话返回文档中所有的元素(Element)。第二个参数为resultList,它实际是指向接口IXMLDOMNodeList的指针，用来返回和tagName(标签名字)相关的所有的Node的集合。

　下面是一个简单的例子 

　　下面是save方法使用的一个简单的例子：

　　BOOL DOMDocSaveLocation()

　　{

　　BOOL bResult = FALSE;

　　IXMLDOMDocument *pIXMLDOMDocument = NULL;

　　HRESULT hr;

　　try

　　{

　　_variant_t varString = _T("D://sample.xml");

　　// 这里需要创建一个DOMDocument对象和装载XML文档，代码省略.

　　hr = pIXMLDOMDocument->save(varString); //保存文档到D://sample.xml中去。

　　if(SUCCEEDED(hr))

　　bResult = TRUE;

　　}

　　catch(...)

　　{

　　DisplayErrorToUser();

　　// 这里需要释放对IXMLDOMDocument接口的引用，代码省略。

　　}

　　return bResult;

　　}

　　例子2

　　IXMLDOMDocument *pIXMLDOMDocument = NULL; 

　　wstring strFindText (_T("author"));

　　IXMLDOMNodeList *pIDOMNodeList = NULL;

　　IXMLDOMNode *pIDOMNode = NULL;

　　long value;

　　BSTR bstrItemText;

　　HRESULT hr;

　　try

　　{

　　// 创建一个DOMDocument文档对象，并装载具体文档，相关代码省略。

　　//下面的代码用来得到一个和标签名称author相关的所有的节点集合

　　hr = pIXMLDOMDocument->getElementsByTagName( 

　　(TCHAR*)strFindText.data(), &pIDOMNodeList);

　　SUCCEEDED(hr) ? 0 : throw hr; 

　　//是否正确的得到了指向IDOMNodeList的指针。

　　hr = pIDOMNodeList->get_length(&value); //得到所包含的NODE节点的个数

　　if(SUCCEEDED(hr))

　　{

　　pIDOMNodeList->reset();

　　for(int ii = 0; ii < value; ii++)

　　{

　　//得到具体的一个NODE节点

　　pIDOMNodeList->get_item(ii, &pIDOMNode); 

　　if(pIDOMNode )

　　{

　　pIDOMNode->get_text(&bstrItemText); //得到该节点相关的文本信息

　　::MessageBox(NULL, bstrItemText,strFindText.data(), MB_OK);

　　pIDOMNode->Release();

　　pIDOMNode = NULL;

　　}

　　}

　　}

　　pIDOMNodeList->Release();

　　pIDOMNodeList = NULL;

　　}

　　catch(...)

　　{

　　if(pIDOMNodeList)

　　pIDOMNodeList->Release();

　　if(pIDOMNode)

　　pIDOMNode->Release();

　　DisplayErrorToUser();

　　}

　　简单的实例程序

　　#include < atlbase.h>

　　//下面的.h文件是在安装了最新的XML Parser以后所包含的.h文件。

　　#include "C:/Program Files/Microsoft XML Parser SDK/inc/msxml2.h"

　　#include < iostream>

　　void main()

　　{

　　// 初始化COM接口

　　CoInitialize(NULL);

　　//在程序中，我们假定我们装载的XML文件名称为xmldata.xml,它缺省的和可执行文

　　//件在同一个目录中。该文件的内容如下：

　　// < ?xml version="1.0"?>

　　// < xmldata>

　　// < xmlnode />

　　// < xmltext>Hello, World!< / xmltext>

　　// < /xmldata>

　　//

　　//程序将寻找名为“xmlnode”的节点，然后插入一个新的名称为“xmlchildnode”的

　　//节点，然后它去寻找一个名为“xmltest”的节点，然后提取包含在节点中的文本并显

　　//示它。最后它把新的改变过的XML文档保存在名称为“updatexml.xml”的文档中。

　　try {

　　// 通过智能指针创建一个解析器的实例。

　　CComPtr< IXMLDOMDocument> spXMLDOM;

　　HRESULT hr = spXMLDOM.CoCreateInstance(__uuidof(DOMDocument));

　　if ( FAILED(hr) ) throw "不能创建XML Parser对象";

　　if ( spXMLDOM.p == NULL ) throw "不能创建XML Parser对象";

　　// 如果对象创建成功的话，就开始装载XML文档

　　VARIANT_BOOL bSuccess = false;

　　hr = spXMLDOM->load(CComVariant(L"xmldata.xml"),&bSuccess);

　　if ( FAILED(hr) ) throw "不能够在解析器中装载XML文档";

　　if ( !bSuccess ) throw "不能够在解析器中装载XML文档";

　　// 检查并搜索"xmldata/xmlnode"

　　CComBSTR bstrSS(L"xmldata/xmlnode");

　　CComPtr< IXMLDOMNode> spXMLNode;

　　//用接口IXMLDOMDocument的方法selectSingleNode方法定位该节点

　　hr = spXMLDOM->selectSingleNode(bstrSS,&spXMLNode);

　　if ( FAILED(hr) ) throw "不能在XML节点中定位'xmlnode' ";

　　if ( spXMLNode.p == NULL ) throw "不能在XML节点中定位'xmlnode' ";

　　//DOM对象“spXMLNode”现在包含了XML节点< xmlnode>，所以我们可以在

　　//它下面创建一个子节点并把找到的该节点作为它的父节点。

　　CComPtr< IXMLDOMNode> spXMLChildNode;

　　//用接口IXMLDOMDocument的方法createNode方法创建一个新节点。

　　hr = spXMLDOM->createNode(

　　CComVariant(NODE_ELEMENT),

　　CComBSTR("xmlchildnode"),

　　NULL,&spXMLChildNode);

　　if ( FAILED(hr) ) throw "不能创建'xmlchildnode' 节点";

　　if ( spXMLChildNode.p == NULL ) 

　　throw "不能创建'xmlchildnode' 节点";

　　//添加新节点到spXMLNode节点下去。

　　CComPtr< IXMLDOMNode> spInsertedNode;

　　hr = spXMLNode->appendChild(spXMLChildNode,&spInsertedNode);

　　if ( FAILED(hr) ) throw "不能创建'xmlchildnode' 节点";

　　if ( spInsertedNode.p == NULL ) throw "不能移动'xmlchildnode' 节点";

　　//对新节点添加属性。

　　CComQIPtr< IXMLDOMElement> spXMLChildElement;

　　spXMLChildElement = spInsertedNode;

　　if ( spXMLChildElement.p == NULL )

　　throw "不能在XML元素接口中查询到'xmlchildnode' ";

　　//设置新节点的属性

　　hr = spXMLChildElement->setAttribute(CComBSTR(L"xml"),CComVariant(L"fun"));

　　if ( FAILED(hr) ) throw "不能插入新的属性";

　　//下面的程序段用来寻找一个节点并显示该节点的相关信息

　　// 查找"xmldata/xmltext"节点

　　spXMLNode = NULL; // 释放先前的节点

　　bstrSS = L"xmldata/xmltext";

　　hr = spXMLDOM->selectSingleNode(bstrSS,&spXMLNode);

　　if ( FAILED(hr) ) throw "不能定位'xmltext'节点";

　　if ( spXMLNode.p == NULL ) throw "不能定位'xmltext'节点";

　　// 得到该节点包含的文本并显示它

　　CComVariant varValue(VT_EMPTY);

　　hr = spXMLNode->get_nodeTypedValue(&varValue);

　　if ( FAILED(hr) ) throw "不能提取'xmltext'文本";

　　if ( varValue.vt == VT_BSTR ) {

　　// 显示结果，注意这里要把字符串从形式BSTR转化为ANSI

　　USES_CONVERSION;

　　LPTSTR lpstrMsg = W2T(varValue.bstrVal);

　　std::cout < < lpstrMsg < < std::endl;

　　} // if

　　else {

　　// 如果出现错误

　　throw "不能提取'xmltext'文本";

　　} // else

　　//保存修改过的XML文档到指定的文档名

　　hr = spXMLDOM->save(CComVariant("updatedxml.xml"));

　　if ( FAILED(hr) ) throw "不能保存修改过的XML文档";

　　std::cout < < "处理完成..." < < std::endl < < std::endl;

　　} // try

　　catch(char* lpstrErr) {

　　// 出现错误

　　std::cout < < lpstrErr < < std::endl < < std::endl;

　　} // catch

　　catch(...) {

　　// 未知错误

　　std::cout < < "未知错误..." < < std::endl < < std::endl;

　　} // catch

　　// 结束对COM的使用

　　CoUninitialize();

　　}

　　最后我们讨论一下如何来创建新的节点，实际上可以通过方法createNode来创建一个新的节点。CreateNode包括四个参数，第一个参数Type表示要创建的节点的类型，第二个参数name表示新节点的nodeName的值，第三个参数namespaceURI表示该节点相关的名字空间，第四个参数node表示新创建的节点。注意可以通过使用已经提供的类型(Type)，名称(name)和名字空间(nodeName)来创建一个节点。

　　当一个节点被创建的时候，它实际上是在一个名字空间范围(如果已经提供了名字空间的话)内创建的。如果没有提供名字空间的话，它实际上是在文档的名字空间范围内创建的。

　　四、利用MSXML进行XML文档分析的简单实例

　　为了说明如何在VC中使用XML DOM模型，这里我们显示了一个简单的实例程序(具体程序请参见www.swm.com.cn/swm/200101/利用MSXML解析XML文本)，是一个Console Application。下面是主要的程序代码，本代码用来在一个XML文档中定位一个特殊的Node节点，并插入一个新的子节点。

　　五、总结

　　XML文档因为有着比HTML严格的多的语法要求，所以使用和编写一个XML解析器要比编写一个HTML的解析器要容易的多。同时因为XML文档不仅可以标记文档的显示属性，更重要的是它标记了文档的结构和包含信息的特征，所以我们可以方便的通过XML解析器来获取特定节点的信息并加以显示或修改，方便了用户对XML文档的操作和维护。同时我们需要注意的是XML是一种开放的结构体系并不依赖于任何一家公司，所以开发基于XML的应用必然会得到绝大多数软件开发平台的支持。另外，我们也可以看到，象微软这样的软件开发主流企业也把目光定位在基于XML+COM的体系上，无论是微软的Office系列、Web服务器和浏览器还是数据库产品(SQL Server)都已经开始支持基于XML的应用。通过XML来定制应用程序的前端，COM来实现具体的业务对象和数据库对象，使系统具有更加灵活的扩展性和维护性。

