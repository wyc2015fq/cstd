# TinyXML的简单用法 - L_Andy的专栏 - CSDN博客

2015年04月10日 17:59:28[卡哥](https://me.csdn.net/L_Andy)阅读数：1622



TinyXML最新的发布版本是2.5.3，一直想用，今天拿出点时间用了用，现在把所掌握的初步简单用法整理一下。

首先编译库文件，TinyXML有两个编译版本，取决于是否使用STL。Build之后在新程序代码中include库中的tinyxml.h即可。下面是我的代码：

#include <cstdio>

#include <iostream>

#include "../tinyxml.h"

int main()

{

        TiXmlDocument myDoc( "TestTinyXML.xml");

        myDoc.InsertEndChild( TiXmlDeclaration( "1.0", "gb2312", "yes") );

        myDoc.InsertEndChild( TiXmlElement( "RootNode") );

        TiXmlElement* rootNode = myDoc.RootElement();

        TiXmlElement* subElem = rootNode->InsertEndChild( TiXmlElement( "SubNode:Size") )->ToElement();

        subElem->SetAttribute( "value", 16);

        subElem->InsertEndChild( TiXmlText( "Text size") );

        myDoc.SaveFile();

        myDoc.Clear();

        myDoc.LoadFile( "TestTinyXML.xml");

        TiXmlHandle docHandle( &myDoc);

        TiXmlDeclaration* pDec = myDoc.FirstChild()->ToDeclaration();
if( pDec)

                std::cout << "Version: " << pDec->Version() << "\nEncoding: " << pDec->Encoding() << "\nStandalong: " << pDec->Standalone() << "\n";

        subElem = docHandle.FirstChildElement().Element();
char* pch = ( char*)subElem->GetText();
if( !pch)

        {

                subElem = subElem->FirstChild()->ToElement();
if( subElem)

                {

                        pch = ( char*)subElem->GetText();
if( pch)

                                std::cout << pch << " is ";

                        std::cout << subElem->Attribute( "value") << "\n";

                }// if

        }// if

return 0;

}

程序运行输出如下：

Version: 1.0

Encoding: gb2312

Standalong: yes

Text size is 16

执行后会生成一个TestTinyXML.xml文件，其内容如下：

<?xmlversion="1.0" encoding="gb2312" standalone="yes" ?>
<RootNode>
<SubNode:Sizevalue="16">Text
 size</SubNode:Size>
</RootNode>

这个小玩意儿别看小，但可以说是短小精悍呢，足够我目前用了。今天先整理出这些，有时间再写。

