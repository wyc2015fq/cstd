# C++之tinyXML使用 - L_Andy的专栏 - CSDN博客

2014年10月30日 10:22:23[卡哥](https://me.csdn.net/L_Andy)阅读数：39703


 tinyXML一款很优秀的操作C++类库，文件不大，但方法很丰富，和apache的Dom4j可以披靡啊！习惯了使用java类库的我看到这么丰富的c++类库，很高兴！它使用很简单，只需要拷贝几个文件到你的工程中，没有STL也可以编译运行。 

    下面我从这几个方面谈谈对tinyXML类库的使用以及理解。 

    首先在sourceforge上下载tinyXML类库，地址：[http://sourceforge.net/projects/tinyxml/](http://sourceforge.net/projects/tinyxml/)

    然后解压缩tinyXML后，将这六个文件添加到你的c++工程中，分别是**tinystr.h、tinystr.cpp、tinyxml.h、tinyxml.cpp、tinyxmlerror.cpp、tinyxmlparser.cpp**。在需要操作xml文件的地方，使用如下代码，就可以引入tinyXML类库。

C++代码  ![收藏代码](http://qaohao.iteye.com/images/icon_star.png)

- #include<tinyxml>

或者

C++代码  ![收藏代码](http://qaohao.iteye.com/images/icon_star.png)

- #include "tinyxml.h"

    下面我用个简单的例子说明如何使用tinyXML操作xml文件。在讲例子之前我先说说tinyXML中主要类和xml文档之间的对应关系。下面是tinyXML中主要class的类图，反应各个类之间的静态关系。 

引用来自tinyXML文档![](http://dl.iteye.com/upload/attachment/158691/fd4f4ec6-61fe-34ac-acab-d946c7678029.png)

    TiXmlBase是所有类的基类，TiXmlNode、TiXmlAttribute两个类都继承来自TiXmlBase类，其中TiXmlNode类指的是所有被<...>...<.../>包括的内容，而xml中的节点又具体分为以下几方面内容，分别是声明、注释、节点以及节点间的文本，因此在TiXmlNode的基础上又衍生出这几个类TiXmlComment、TiXmlDeclaration、TiXmlDocument、TiXmlElement、TiXmlText、TiXmlUnknown，分别用来指明具体是xml中的哪一部分。TiXmlAttribute类不同于TiXmlNode，它指的是在尖括号里面的内容，像<...
 ***=...>，其中*******就是一个属性。这块我具体用一个xml文档说明一下，内容如下： 

Xml代码  ![收藏代码](http://qaohao.iteye.com/images/icon_star.png)

- <?xmlversion="1.0"encoding="UTF-8"?>
- <phonebook>
- <!--one item behalfs one contacted person.-->
- <item>
- <name>miaomaio</name>
- <addr>Shaanxi Xi'an</addr>
- <tel>13759911917</tel>
- <email>miaomiao@home.com</email>
- </item>
- <item>
- <name>gougou</name>
- <addr>Liaoning Shenyang</addr>
- <tel>15840330481</tel>
- <email>gougou@home.com</email>
- </item>
- <!--more contacted persons.-->
- </phonebook>

- 像TiXmlDeclaration指的就是<?xml version="1.0" encoding="UTF-8"?>，
- 像TiXmlComment指的就是<!--one item behalfs one contacted person.-->、 <!--more contacted persons.-->，
- 像TiXmlDocument指的就是整个xml文档，
- 像TiXmlElement指的就是<phonebook>、<item>、<name>、<addr>等等这些节点，
- 像TiXmlText指的就是‘gougou’、‘15840330481’这些夹在<item>与</item>、<name>与</name>、<addr>与</addr>之间的文本文字，
- 像TiXmlAttribute指的就是<?xml version="1.0" encoding="UTF-8"?>节点中version、encoding，
- 除此之外就是TiXmlUnknown。

下面是我自己写的一段读xml文件的c++代码，以及再往xml写入一个item的源代码，其中phonebookdata.xml中的内容就是上面xml，仅供参考。 

C++代码  ![收藏代码](http://qaohao.iteye.com/images/icon_star.png)

- //______________________________________________________________________
- // Read information from xml file.
- 
- // define xml file path, as follow , we use relative path,
- // but you can use absolute path also.
- constchar* filepath = "phonebookdata.xml";  
- TiXmlDocument doc(filepath);  
- bool loadOkay = doc.LoadFile();  
- // faile to load 'phonebookdata.xml'.
- if (!loadOkay) {      
-     printf( "Could not load test file %s. Error='%s'. Exiting.\n", filepath,doc.ErrorDesc() );  
-     exit( 1 );  
- }  
- 
- // get dom root of 'phonebookdata.xml', here root should be 'phonebook'.
- TiXmlElement* root = doc.RootElement();  
- 
- printf("_______________________________________\n\n");  
- printf("     contacted person information      \n\n");  
- // trace every items below root.
- for( TiXmlNode*  item = root->FirstChild( "item" );  
-          item;  
-          item = item->NextSibling( "item" ) ) {  
-     printf("_______________________________________\n");  
- 
- // read name.
-     TiXmlNode* child = item->FirstChild();  
- constchar* name = child->ToElement()->GetText();  
- if (name) {  
-         printf("name:%s\n",name);  
-     } else {  
-         printf("name:\n");  
-     }  
- 
- // read address.
-     child = item->IterateChildren(child);  
- constchar* addr = child->ToElement()->GetText();  
- if (addr) {  
-         printf("addr:%s\n",addr);  
-     } else {  
-         printf("addr:\n");  
-     }  
- 
- 
- // read telephone no.
-     child = item->IterateChildren(child);  
- constchar* tel = child->ToElement()->GetText();  
- if (tel) {  
-         printf("tel:%s\n",tel);  
-     } else {  
-         printf("tel:\n");  
-     }  
- 
- // read e-mail.
-     child = item->IterateChildren(child);  
- constchar* email = child->ToElement()->GetText();  
- if(email) {  
-         printf("email:%s\n",email);  
-     } else {  
-         printf("email:\n");  
-     }  
- 
-     printf("\n");  
- 
- }  
- //______________________________________________________________________
- 
- 
- //______________________________________________________________________
- // Add information to xml file and save it.
- TiXmlElement* writeRoot = doc.RootElement();  
- TiXmlNode* newNode = new TiXmlElement("item");  
- 
- const TiXmlNode* name4NewNode = new TiXmlElement("name");  
- newNode->InsertEndChild(*name4NewNode)->InsertEndChild(TiXmlText("pipi"));  
- 
- const TiXmlNode* addr4NewNode = new TiXmlElement("addr");  
- newNode->InsertEndChild(*addr4NewNode)->InsertEndChild(TiXmlText("Shaanxi Xianyang"));  
- 
- const TiXmlNode* tel4NewNode = new TiXmlElement("tel");  
- newNode->InsertEndChild(*tel4NewNode)->InsertEndChild(TiXmlText("02937310627"));  
- 
- const TiXmlNode* email4NewNode = new TiXmlElement("email");  
- newNode->InsertEndChild(*email4NewNode)->InsertEndChild(TiXmlText("pipi@home.com"));  
- 
- writeRoot->InsertEndChild(*newNode);  
- doc.SaveFile();  
- //______________________________________________________________________

具体使用可以参考tinyxml随带的文档。

