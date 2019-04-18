# C的xml编程-libxml2 - ljx0305的专栏 - CSDN博客
2008年01月30日 23:18:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：2591标签：[xml																[编程																[c																[文档																[null																[struct](https://so.csdn.net/so/search/s.do?q=struct&t=blog)](https://so.csdn.net/so/search/s.do?q=null&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=c&t=blog)](https://so.csdn.net/so/search/s.do?q=编程&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)
  这里主要讲述libxml2在linux下的使用。
（以下内容除了linux下的安装步骤是自己写的，其余均出自[http://www.blogjava.net/wxb_nudt/archive/2007/11/28/161340.html](http://www.blogjava.net/wxb_nudt/archive/2007/11/28/161340.html)，由于该篇文章讲解的非常详细，我也没必要重写一遍了。如果该篇文章早出些时候，早期开发我也不会走很多弯路了。虽然在该篇文章出现之前我已经对libxml2较熟悉了并开发完毕，但为了备忘，现在将该篇文章作为收藏之用。在此再次感谢这篇文章的大侠为大家整理了这篇文章^_^）
# 1.下载与安装LIBXML2
Libxml2是一个C语言的XML程序库，可以简单方便的提供对XML文档的各种操作，并且支持XPATH查询，以及部分的支持XSLT转换等功能。Libxml2的下载地址是[http://xmlsoft.org/](http://xmlsoft.org/)，完全版的库是开源的，并且带有例子程序和说明文档。最好将这个库先下载下来，因为这样可以查看其中的文档和例子。
由于我是在linux下用C语言进行开发的，所以我下载的是libxml2-2.6.20.tar.gz版本的源码包。
具体安装步骤：
1、解压：$tar zxvf libxml2-2.6.20.tar.gz
2、进入解压后的安装目录：$cd libxml2-2.6.20
3、安装三部曲：1）$./configure
              2)$make
              3)$make install
安装完毕。
# 2.Libxml2中的数据类型和函数
一个函数库中可能有几百种数据类型以及几千个函数，但是记住大师的话，90%的功能都是由30%的内容提供的。对于libxml2，我认为搞懂以下的数据类型和函数就足够了。
## 2.1内部字符类型xmlChar
xmlChar是Libxml2中的字符类型，库中所有字符、字符串都是基于这个数据类型。事实上它的定义是：xmlstring.h
typedef unsigned char xmlChar;
使用unsigned char作为内部字符格式是考虑到它能很好适应UTF-8编码，而UTF-8编码正是libxml2的内部编码，其它格式的编码要转换为这个编码才能在libxml2中使用。
还经常可以看到使用xmlChar*作为字符串类型，很多函数会返回一个动态分配内存的xmlChar*变量，使用这样的函数时记得要手动删除内存。
## 2.2xmlChar相关函数
如同标准c中的char类型一样，xmlChar也有动态内存分配、字符串操作等相关函数。例如xmlMalloc是动态分配内存的函数；xmlFree是配套的释放内存函数；xmlStrcmp是字符串比较函数等等。
基本上xmlChar字符串相关函数都在xmlstring.h中定义；而动态内存分配函数在xmlmemory.h中定义。
## 2.3xmlChar*与其它类型之间的转换
另外要注意，因为总是要在xmlChar*和char*之间进行类型转换，所以定义了一个宏BAD_CAST，其定义如下：xmlstring.h
#define BAD_CAST (xmlChar *)
原则上来说，unsigned char和char之间进行强制类型转换是没有问题的。
## 2.4文档类型xmlDoc、指针xmlDocPtr
xmlDoc是一个struct，保存了一个xml的相关信息，例如文件名、文档类型、子节点等等；xmlDocPtr等于xmlDoc*，它搞成这个样子总让人以为是智能指针，其实不是，要手动删除的。
xmlNewDoc函数创建一个新的文档指针。
xmlParseFile函数以默认方式读入一个UTF-8格式的文档，并返回文档指针。
xmlReadFile函数读入一个带有某种编码的xml文档，并返回文档指针；细节见libxml2参考手册。
xmlFreeDoc释放文档指针。特别注意，当你调用xmlFreeDoc时，该文档所有包含的节点内存都被释放，所以一般来说不需要手动调用xmlFreeNode或者xmlFreeNodeList来释放动态分配的节点内存，除非你把该节点从文档中移除了。一般来说，一个文档中所有节点都应该动态分配，然后加入文档，最后调用xmlFreeDoc一次释放所有节点申请的动态内存，这也是为什么我们很少看见xmlNodeFree的原因。
xmlSaveFile将文档以默认方式存入一个文件。
xmlSaveFormatFileEnc可将文档以某种编码/格式存入一个文件中。
## 2.5节点类型xmlNode、指针xmlNodePtr
节点应该是xml中最重要的元素了，xmlNode代表了xml文档中的一个节点，实现为一个struct，内容很丰富：tree.h
typedef struct _xmlNode xmlNode;
typedef xmlNode *xmlNodePtr;
struct _xmlNode {
    void           *_private;/* application data */
    xmlElementType   type;   /* type number, must be second ! */
    const xmlChar   *name;      /* the name of the node, or the entity */
    struct _xmlNode *children; /* parent->childs link */
    struct _xmlNode *last;   /* last child link */
    struct _xmlNode *parent;/* child->parent link */
    struct _xmlNode *next;   /* next sibling link */
    struct _xmlNode *prev;   /* previous sibling link */
    struct _xmlDoc *doc;/* the containing document */
    /* End of common part */
    xmlNs           *ns;        /* pointer to the associated namespace */
    xmlChar         *content;   /* the content */
    struct _xmlAttr *properties;/* properties list */
    xmlNs           *nsDef;     /* namespace definitions on this node */
    void            *psvi;/* for type/PSVI informations */
    unsigned short   line;   /* line number */
    unsigned short   extra; /* extra data for XPath/XSLT */
};
可以看到，节点之间是以链表和树两种方式同时组织起来的，next和prev指针可以组成链表，而parent和children可以组织为树。同时还有以下重要元素：
l节点中的文字内容：content；
l节点所属文档：doc；
l节点名字：name；
l节点的namespace：ns；
l节点属性列表：properties；
Xml文档的操作其根本原理就是在节点之间移动、查询节点的各项信息，并进行增加、删除、修改的操作。
xmlDocSetRootElement函数可以将一个节点设置为某个文档的根节点，这是将文档与节点连接起来的重要手段，当有了根结点以后，所有子节点就可以依次连接上根节点，从而组织成为一个xml树。
## 2.6节点集合类型xmlNodeSet、指针xmlNodeSetPtr
节点集合代表一个由节点组成的变量，节点集合只作为Xpath的查询结果而出现（XPATH的介绍见后面），因此被定义在xpath.h中，其定义如下：
/*
 * A node-set (an unordered collection of nodes without duplicates).
 */
typedef struct _xmlNodeSet xmlNodeSet;
typedef xmlNodeSet *xmlNodeSetPtr;
struct _xmlNodeSet {
    int nodeNr;          /* number of nodes in the set */
    int nodeMax;      /* size of the array as allocated */
    xmlNodePtr *nodeTab;/* array of nodes in no particular order */
    /* @@ with_ns to check wether namespace nodes should be looked at @@ */
};
可以看出，节点集合有三个成员，分别是节点集合的节点数、最大可容纳的节点数，以及节点数组头指针。对节点集合中各个节点的访问方式很简单，如下：
xmlNodeSetPtr nodeset = XPATH查询结果;
for (int i = 0; i < nodeset->nodeNr; i++) 
{
 nodeset->nodeTab[i];
}
注意，libxml2是一个c函数库，因此其函数和数据类型都使用c语言的方式来处理。如果是c++，我想我宁愿用STL中的vector来表示一个节点集合更好，而且没有内存泄漏或者溢出的担忧。
# 3.简单xml操作例子
了解以上基本知识之后，就可以进行一些简单的xml操作了。当然，还没有涉及到内码转换（使得xml中可以处理中文）、xpath等较复杂的操作。
## 3.1创建xml文档
有了上面的基础，创建一个xml文档显得非常简单，其流程如下：
l用xmlNewDoc函数创建一个文档指针doc；
l用xmlNewNode函数创建一个节点指针root_node；
l用xmlDocSetRootElement将root_node设置为doc的根结点；
l给root_node添加一系列的子节点，并设置子节点的内容和属性；
l用xmlSaveFile将xml文档存入文件；
l用xmlFreeDoc函数关闭文档指针，并清除本文档中所有节点动态申请的内存。
注意，有多种方式可以添加子节点：第一是用xmlNewTextChild直接添加一个文本子节点；第二是先创建新节点，然后用xmlAddChild将新节点加入上层节点。
源代码文件是CreateXmlFile.cpp，如下：
/********************************************************************
    created:   2007/11/09
    created:   9:11:2007   15:34
    filename: CreateXmlFile.cpp
    author:       Wang xuebin 
    depend:       libxml2.lib 
    build:     nmake TARGET_NAME=CreateXmlFile
    purpose:   创建一个xml文件
*********************************************************************/
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream.h>
int main()
{
    //定义文档和节点指针
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");
    //设置根节点
    xmlDocSetRootElement(doc,root_node);
    //在根节点中直接创建节点
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");
    //创建一个节点，设置其内容和属性，然后加入根结点
    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
    xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
    xmlAddChild(root_node,node);
    xmlAddChild(node,content);
    xmlNewProp(node,BAD_CAST"attribute",BAD_CAST "yes");
    //创建一个儿子和孙子节点
    node = xmlNewNode(NULL, BAD_CAST "son");
    xmlAddChild(root_node,node);
    xmlNodePtr grandson = xmlNewNode(NULL, BAD_CAST "grandson");
    xmlAddChild(node,grandson);
    xmlAddChild(grandson, xmlNewText(BAD_CAST "This is a grandson node"));
    //存储xml文档
    int nRel = xmlSaveFile("CreatedXml.xml",doc);
    if (nRel != -1)
    {
       cout<<"一个xml文档被创建,写入"<<nRel<<"个字节"<<endl;
    }
    //释放文档内节点动态申请的内存
    xmlFreeDoc(doc);
    return 1;
}
编译链接命令如下：
nmake TARGET_NAME=CreateXmlFile
然后执行可执行文件CreateXmlFile.exe，会生成一个xml文件CreatedXml.xml，打开后如下所示：
<?xml version="1.0"?>
<root>
<newNode1>newNode1 content</newNode1>
<newNode2>newNode2 content</newNode2>
<newNode3>newNode3 content</newNode3>
<node2 attribute="yes">NODE CONTENT</node2>
<son>
<grandson>This is a grandson node</grandson>
</son>
</root>
最好使用类似XMLSPY这样的工具打开，因为这些工具可以自动整理xml文件的栅格，否则很有可能是没有任何换行的一个xml文件，可读性较差。
## 3.2解析xml文档
解析一个xml文档，从中取出想要的信息，例如节点中包含的文字，或者某个节点的属性，其流程如下：
l用xmlReadFile函数读出一个文档指针doc；
l用xmlDocGetRootElement函数得到根节点curNode；
lcurNode->xmlChildrenNode就是根节点的子节点集合；
l轮询子节点集合，找到所需的节点，用xmlNodeGetContent取出其内容；
l用xmlHasProp查找含有某个属性的节点；
l取出该节点的属性集合，用xmlGetProp取出其属性值；
l用xmlFreeDoc函数关闭文档指针，并清除本文档中所有节点动态申请的内存。
注意：节点列表的指针依然是xmlNodePtr，属性列表的指针也是xmlAttrPtr，并没有xmlNodeList或者xmlAttrList这样的类型。看作列表的时候使用它们的next和prev链表指针来进行轮询。只有在Xpath中有xmlNodeSet这种类型，其使用方法前面已经介绍了。
源代码如下：ParseXmlFile.cpp
/********************************************************************
    created:   2007/11/15
    created:   15:11:2007   11:47
    filename: ParseXmlFile.cpp
    author:       Wang xuebin 
    depend:       libxml2.lib
    build:     nmake TARGET_NAME=ParseXmlFile
    purpose:   解析xml文件
*********************************************************************/
#include <libxml/parser.h>
#include <iostream.h>
int main(int argc, char* argv[])
{
    xmlDocPtr doc;           //定义解析文档指针
    xmlNodePtr curNode;      //定义结点指针(你需要它为了在各个结点间移动) 
    xmlChar *szKey;          //临时字符串变量
    char *szDocName;
    if (argc <= 1) 
    {
       printf("Usage: %s docname"n", argv[0]);
       return(0);
    }
    szDocName = argv[1];
    doc = xmlReadFile(szDocName,"GB2312",XML_PARSE_RECOVER); //解析文件
    //检查解析文档是否成功，如果不成功，libxml将指一个注册的错误并停止。
    //一个常见错误是不适当的编码。XML标准文档除了用UTF-8或UTF-16外还可用其它编码保存。
    //如果文档是这样，libxml将自动地为你转换到UTF-8。更多关于XML编码信息包含在XML标准中.
    if (NULL == doc) 
    {  
       fprintf(stderr,"Document not parsed successfully. "n");     
       return -1; 
    } 
    curNode = xmlDocGetRootElement(doc); //确定文档根元素
    /*检查确认当前文档中包含内容*/ 
    if (NULL == curNode)
    { 
       fprintf(stderr,"empty document"n"); 
       xmlFreeDoc(doc); 
       return -1; 
    } 
    /*在这个例子中，我们需要确认文档是正确的类型。“root”是在这个示例中使用文档的根类型。*/
    if (xmlStrcmp(curNode->name, BAD_CAST "root")) 
    {
       fprintf(stderr,"document of the wrong type, root node != root"); 
       xmlFreeDoc(doc); 
       return -1; 
    } 
    curNode = curNode->xmlChildrenNode;
    xmlNodePtr propNodePtr = curNode;
    while(curNode != NULL) 
    {
       //取出节点中的内容
       if ((!xmlStrcmp(curNode->name, (const xmlChar *)"newNode1"))) 
       {
           szKey = xmlNodeGetContent(curNode);
           printf("newNode1: %s"n", szKey); 
           xmlFree(szKey); 
       } 
       //查找带有属性attribute的节点
       if (xmlHasProp(curNode,BAD_CAST "attribute"))
       {
           propNodePtr = curNode;
       }
       curNode = curNode->next; 
    } 
    //查找属性
    xmlAttrPtr attrPtr = propNodePtr->properties;
    while (attrPtr != NULL)
    {
       if (!xmlStrcmp(attrPtr->name, BAD_CAST "attribute"))
       {
           xmlChar* szAttr = xmlGetProp(propNodePtr,BAD_CAST "attribute");
           cout<<"get attribute = "<<szAttr<<endl;
           xmlFree(szAttr);
       }
       attrPtr = attrPtr->next;
    }
    xmlFreeDoc(doc);
    return 0;
}
编译链接命令如下：
nmake TARGET_NAME=ParseXmlFile
执行命令如下，使用第一次创建的xml文件作为输入：
ParseXmlFile.exe CreatedXml.xml
观察源代码可发现，所有以查询方式得到的xmlChar*字符串都必须使用xmlFree函数手动释放。否则会造成内存泄漏。
## 3.3修改xml文档
有了上面的基础，修改xml文档的内容就很简单了。首先打开一个已经存在的xml文档，顺着根结点找到需要添加、删除、修改的地方，调用相应的xml函数对节点进行增、删、改操作。源代码见ChangeXmlFile，编译链接方法如上。执行下面的命令：
ChangeXmlFile.exe CreatedXml.xml
可以得到一个修改后的xml文档ChangedXml.xml，如下：
<?xml version="1.0"?>
<root>
<newNode2>content changed</newNode2>
<newNode3 newAttr="YES">newNode3 content</newNode3>
<node2 attribute="no">NODE CONTENT</node2>
<son>
<grandson>This is a grandson node</grandson>
<newGrandSon>new content</newGrandSon>
</son>
</root>
需要注意的是，并没有xmlDelNode或者xmlRemoveNode函数，我们删除节点使用的是以下一段代码：
       if (!xmlStrcmp(curNode->name, BAD_CAST "newNode1"))
       {
           xmlNodePtr tempNode;
           tempNode = curNode->next;
           xmlUnlinkNode(curNode);
           xmlFreeNode(curNode);
           curNode = tempNode;
           continue;
       }
即将当前节点从文档中断链（unlink），这样本文档就不会再包含这个子节点。这样做需要使用一个临时变量来存储断链节点的后续节点，并记得要手动删除断链节点的内存。
## 3.4使用XPATH查找xml文档
简而言之，XPATH之于xml，好比SQL之于关系数据库。要在一个复杂的xml文档中查找所需的信息，XPATH简直是必不可少的工具。XPATH语法简单易学，并且有一个很好的官方教程，见[http://www.zvon.org/xxl/XPathTutorial/Output_chi/introduction.html](http://www.zvon.org/xxl/XPathTutorial/Output_chi/introduction.html)。这个站点的XML各种教程齐全，并且有包括中文在内的各国语言版本，真是让我喜欢到非常！
使用XPATH之前，必须首先熟悉几个数据类型和函数，它们是使用XPATH的前提。在libxml2中使用Xpath是非常简单的，其流程如下：
l定义一个XPATH上下文指针xmlXPathContextPtr context，并且使用xmlXPathNewContext函数来初始化这个指针；
l定义一个XPATH对象指针xmlXPathObjectPtr result，并且使用xmlXPathEvalExpression函数来计算Xpath表达式，得到查询结果，将结果存入对象指针中；
l使用result->nodesetval得到节点集合指针，其中包含了所有符合Xpath查询结果的节点；
l使用xmlXPathFreeContext释放上下文指针；
l使用xmlXPathFreeObject释放Xpath对象指针；
具体的使用方法可以看XpathForXmlFile.cpp的这一段代码，其功能是查找符合某个Xpath语句的对象指针：
xmlXPathObjectPtr getNodeSet(xmlDocPtr doc, const xmlChar *szXpath) 
{
    xmlXPathContextPtr context;    //XPATH上下文指针
    xmlXPathObjectPtr result;       //XPATH对象指针，用来存储查询结果
    context = xmlXPathNewContext(doc);     //创建一个XPath上下文指针
    if (context == NULL) 
    {   
       printf("context is NULL"n");
       return NULL; 
    }
    result = xmlXPathEvalExpression(szXpath, context); //查询XPath表达式，得到一个查询结果
    xmlXPathFreeContext(context);             //释放上下文指针
    if (result == NULL) 
    {
       printf("xmlXPathEvalExpression return NULL"n"); 
       return NULL; 
    }
    if (xmlXPathNodeSetIsEmpty(result->nodesetval))   //检查查询结果是否为空
    {
       xmlXPathFreeObject(result);
       printf("nodeset is empty"n");
       return NULL;
    }
    return result;    
}
一个完整的使用Xpath的例子在代码XpathForXmlFile.cpp中，它查找一个xml文件中符合"/root/node2[@attribute='yes']"语句的结果，并且将找到的节点的属性和内容打印出来。编译链接命令如下：
nmake TARGET_NAME=XpathForXmlFile
执行方式如下：
XpathForXmlFile.exe CreatedXml.xml
观察结果可以看出找到了一个节点，即root下面node2节点，它的attribute属性值正好等于yes。更多关于Xpath的内容可以参考XPATH官方手册。只有掌握了XPATH，才掌握了使用大型XML文件的方法，否则每寻找一个节点都要从根节点找起，会把人累死。
# 4.用ICONV解决XML中的中文问题
Libxml2中默认的内码是UTF-8，所有使用libxml2进行处理的xml文件，必须首先显式或者默认的转换为UTF-8编码才能被处理。
要在xml中使用中文，就必须能够在UTF-8和GB2312内码（较常用的一种简体中文编码）之间进行转换。Libxml2提供了默认的内码转换机制，并且在libxml2的Tutorial中有一个例子，事实证明这个例子并不适合用来转换中文。
所以需要我们显式的使用ICONV来进行内码转换，libxml2本身也是使用ICONV进行转换的。ICONV是一个专门用来进行编码转换的库，基本上支持目前所有常用的编码。它是glibc库的一个部分，常常被用于UNIX系统中。当然，在windows下面使用也没有任何问题。前面已经提到了ICONV的安装和使用方法，这里主要讲一下编程相关问题。
本节其实和xml以及libxml2没有太大关系，你可以把它简单看作是一个编码转换方面的专题。我们仅仅需要学会使用两个函数就可以了，即从UTF-8转换到GB2312的函数u2g，以及反向转换的函数g2u，源代码在wxb_codeConv.c中：
/********************************************************************
    created:   2007/11/15
    created:   15:11:2007   10:30
    filename: wxb_codeConv.c
    author:       Wang xuebin 
    depend:       iconv.lib
    build:     不需要build，被包含到其它源代码中
    purpose:   提供从UTF-8到GB2312的内码转换，以及反向的转换
*********************************************************************/
#include "iconv.h"
#include <string.h>
//代码转换:从一种编码转为另一种编码
int code_convert(char* from_charset, char* to_charset, char* inbuf,
               int inlen, char* outbuf, int outlen)
{
    iconv_t cd;
    char** pin = &inbuf;   
    char** pout = &outbuf;
    cd = iconv_open(to_charset,from_charset);   
    if(cd == 0)
       return -1;
    memset(outbuf,0,outlen);   
    if(iconv(cd,(const char**)pin,(unsigned int *)&inlen,pout,(unsigned int*)&outlen)
       == -1)
       return -1;   
    iconv_close(cd);
    return 0;   
}
//UNICODE码转为GB2312码   
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* u2g(char *inbuf)   
{
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("utf-8","gb2312",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}   
//GB2312码转为UNICODE码
//成功则返回一个动态分配的char*变量，需要在使用完毕后手动free，失败返回NULL
char* g2u(char *inbuf)   
{
    int nOutLen = 2 * strlen(inbuf) - 1;
    char* szOut = (char*)malloc(nOutLen);
    if (-1 == code_convert("gb2312","utf-8",inbuf,strlen(inbuf),szOut,nOutLen))
    {
       free(szOut);
       szOut = NULL;
    }
    return szOut;
}   
使用的时候将这个c文件include到其它源文件中。include一个c文件并不奇怪，在c语言的年代我们常常这么干，唯一的害处的编译链接出来的可执行程序体积变大了。当然这时因为我们这段代码很小的原因，再大一点我就要用dll了。
从UTF-8到GB2312的一个典型使用流程如下：
l得到一个UTF-8的字符串szSrc；
l定义一个char*的字符指针szDes，并不需要给他动态审判内存；
lszDes = u2g(szSrc)，这样就可以得到转换后的GB2312编码的字符串；
l使用完这个字符串后使用free(szDes)来释放内存。
本文并不准备讲述iconv中的函数细节，因为那几个函数以及数据类型都非常简单，我们还是重点看一下如何在libxml2中使用编码转换来处理带有中文的xml文件。下面是使用以上方法来创建一个带有中文的XML文件的例子程序CreateXmlFile_cn.cpp，源代码如下：
/********************************************************************
    created:   2007/11/17
    created:   9:11:2007   15:34
    filename: CreateXmlFile.cpp
    author:       Wang xuebin 
    depend:       libxml2.lib iconv.lib
    build:     nmake TARGET_NAME=CreateXmlFile_cn
    purpose:   创建一个xml文件，其中包含中文
*********************************************************************/
#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <iostream.h>
#include "wxb_codeConv.c" //自己写的编码转换函数
int main(int argc, char **argv)
{
    //定义文档和节点指针
    xmlDocPtr doc = xmlNewDoc(BAD_CAST"1.0");
    xmlNodePtr root_node = xmlNewNode(NULL,BAD_CAST"root");
    //设置根节点
    xmlDocSetRootElement(doc,root_node);
    //一个中文字符串转换为UTF-8字符串，然后写入
    char* szOut = g2u("节点1的内容");
    //在根节点中直接创建节点
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode1", BAD_CAST "newNode1 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode2", BAD_CAST "newNode2 content");
    xmlNewTextChild(root_node, NULL, BAD_CAST "newNode3", BAD_CAST "newNode3 content");
    xmlNewChild(root_node, NULL, BAD_CAST "node1",BAD_CAST szOut);
    free(szOut);
    //创建一个节点，设置其内容和属性，然后加入根结点
    xmlNodePtr node = xmlNewNode(NULL,BAD_CAST"node2");
    xmlNodePtr content = xmlNewText(BAD_CAST"NODE CONTENT");
    xmlAddChild(root_node,node);
    xmlAddChild(node,content);
    szOut = g2u("属性值");
    xmlNewProp(node,BAD_CAST"attribute",BAD_CAST szOut);
    free(szOut);
    //创建一个中文节点
    szOut = g2u("中文节点");
    xmlNewChild(root_node, NULL, BAD_CAST szOut,BAD_CAST "content of chinese node");
    free(szOut);
    //存储xml文档
    int nRel = xmlSaveFormatFileEnc("CreatedXml_cn.xml",doc,"GB2312",1);
    if (nRel != -1)
    {
       cout<<"一个xml文档被创建,写入"<<nRel<<"个字节"<<endl;
    }
    xmlFreeDoc(doc);
    return 1;
}
编译链接命令如下：
nmake TARGET_NAME=CreateXmlFile_cn
完成后执行CreateXmlFile_cn.exe可以生成一个xml文件CreatedXml_cn.xml，其内容如下：
<?xml version="1.0" encoding="GB2312"?>
<root>
<newNode1>newNode1 content</newNode1>
<newNode2>newNode2 content</newNode2>
<newNode3>newNode3 content</newNode3>
<node1>节点1的内容</node1>
<node2 attribute="属性值">NODE CONTENT</node2>
<中文节点>content of chinese node</中文节点>
</root>
观察可知，节点的名称、内容、属性都可以使用中文了。在解析、修改和查找XML文档时都可以使用上面的方法，只要记住，进入xml文档之前将中文编码转换为UTF-8编码；从XML中取出数据时，不管三七二十一都可以转换为GB2312再用，否则你很有可能见到传说中的乱码！
# 5.用XML来做点什么
有了以上的基础，相信已经可以顺利的在c/c++程序中使用XML文档了。那么，我们到底要用XML来做什么呢？我随便说一说自己的想法：
第一，可以用来作为配置文件。例如很多组件就是用XML来做配置文件；当然，我们知道用INI做配置文件更简单，只要熟悉两个函数就可以了；不过，复杂一点的配置文件我还是建议采用XML；
第二，可以用来作为在程序之间传送数据的格式，这样的话最好给你的xml先定义一个XML Schema，这样的数据首先可以做一个良构校验，还可以来一个Schema校验，如此的话出错率会比没有格式的数据小得多。目前XML已经广泛作为网络之间的数据格式了；
第三，可以用来作为你自定义的数据存储格式，例如对象持久化之类的功能；
最后，可以用来显示你的技术很高深，本来你要存储一个1，结果你这样存储了：
<?xml version="1.0" encoding="GB2312"?>
<root>
<My_Program_Code content="1"></My_Program_Code>
</root>
 引用:http://blog.chinaunix.net/u1/56834/showart_441723.html
