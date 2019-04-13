
# libxml解析xml文件的一些总结 - 高科的专栏 - CSDN博客

2012年12月19日 21:27:27[高科](https://me.csdn.net/pbymw8iwm)阅读数：6857


libxml -- 解析 XML 文档
[介绍：XML 和 DOM](http://jianlee.ylinux.org/Computer/C/libxml.html#sec3)[libxml](http://jianlee.ylinux.org/Computer/C/libxml.html#sec4)[介绍](http://jianlee.ylinux.org/Computer/C/libxml.html#sec5)[数据类型 — xmlChar](http://jianlee.ylinux.org/Computer/C/libxml.html#sec6)[数据结构](http://jianlee.ylinux.org/Computer/C/libxml.html#sec7)[创建 XML 文档](http://jianlee.ylinux.org/Computer/C/libxml.html#sec8)[解析 XML 文档](http://jianlee.ylinux.org/Computer/C/libxml.html#sec9)[修改 xml 文档](http://jianlee.ylinux.org/Computer/C/libxml.html#sec10)[Xpath — 处理大型 XML 文档](http://jianlee.ylinux.org/Computer/C/libxml.html#sec11)[libxml2 库函数](http://jianlee.ylinux.org/Computer/C/libxml.html#sec12)[要注意的函数](http://jianlee.ylinux.org/Computer/C/libxml.html#sec13)[读取 xml 文件](http://jianlee.ylinux.org/Computer/C/libxml.html#sec14)[xml 操作基本结构及其指针类型](http://jianlee.ylinux.org/Computer/C/libxml.html#sec15)[根节点相关函数](http://jianlee.ylinux.org/Computer/C/libxml.html#sec16)[创建子节点相关函数](http://jianlee.ylinux.org/Computer/C/libxml.html#sec17)[添加子节点相关函数](http://jianlee.ylinux.org/Computer/C/libxml.html#sec18)[属性相关函数](http://jianlee.ylinux.org/Computer/C/libxml.html#sec19)
## 参考
[http://xmlsoft.org/](http://xmlsoft.org/)
[http://www.miidoo.cn/info_detail-204.html](http://www.miidoo.cn/info_detail-204.html)
[http://www.blogjava.net/wxb_nudt/archive/2007/11/28/161340.html](http://www.blogjava.net/wxb_nudt/archive/2007/11/28/161340.html)
[http://www.ibm.com/developerworks/cn/aix/library/au-libxml2.html](http://www.ibm.com/developerworks/cn/aix/library/au-libxml2.html)
[http://www.cppblog.com/lymons/archive/2009/03/30/37553.html](http://www.cppblog.com/lymons/archive/2009/03/30/37553.html)
[XPath 教程](http://www.zvon.org/xxl/XPathTutorial/Output_chi/introduction.html)
## XML
### 介绍：XML 和 DOM
XML是eXtensible Markup Language的缩写，它是一种可扩展性标识语言, 能够让你自己创造标识，标识你所表示的内容。DOM全称是Document Object Model(文档对象模型)，定义了一组与平台和语言无关的接口，以便程序和脚本能够动态访问和修改XML文档内容、结构及样式。XML创建了标识，而 DOM的作用就是告诉程序如何操作和显示这些标识。
XML将数据组织成为一棵树，DOM通过解析XML文档，为XML文档在逻辑上建立一个树模型，树的节点是一个个的对象。这样通过操作这棵树和这些对象就可以完成对XML文档的操作，为处理文档的所有方面提供了一个完美的概念性框架。
XML 中共有12种节点类型，其中最常见的节点类型有5种：
**元素**元素是 XML 的基本组成单元。，描述XML的基本信息。**属性**属性节点包含关于元素节点的信息，通常包含在元素里面，描述元素的属性。**文本**包含许多文本信息或者只是空白。**文档**文档节点是整个文档中所有其它节点的父节点。**注释**注释是对相关的信息进行描述、注释。
## libxml
### 介绍
本文所介绍的 libxml 是针对 C 语言的一套 API 接口。其他如 ruby，python 亦有对应的基于 libxml 开发的绑定库接口。
### 数据类型 — xmlChar
在 libXml 中用 xmlChar 替代 char , XML 使用 UTF-8 编码的一字节字符串。如果你的数据使用其它编码，它必须被转换到 UTF-8 才能使用libxml的函数。
如同标准 C 中的 char 类型一样， xmlChar 也有动态内存分配、字符串操作等相关函数。例如 xmlMalloc 是动态分配内存的函数； xmlFree 是配套的释放内存函数； xmlStrcmp 是字符串比较函数等等。基本上 xmlChar 字符串相关函数都在xmlstring.h 中定义；而动态内存分配函数在 xmlmemory.h 中定义。另外要注意，因为总是要在 xmlChar* 和 char* 之间进行类型转换，所以定义了一个宏 BAD_CAST ，其定义如下： xmlstring.h
\#define BAD_CAST (xmlChar *)原则上来说， unsigned char 和 char 之间进行强制类型转换是没有问题的。
### 数据结构
**xmlDoc**代表DOM结构中的文档类型。包含由解析文档建立的树结构， xmlDocPtr 是指向这个结构的指针。**xmlNode**代表DOM结构中的除文档类型类型外的其它节点类型。包含单一结点的结构， xmlNodePtr 是指向这个结构的指针，它被用于遍历文档树。节点应该是xml中最重要的元素了， xmlNode 代表了xml文档中的一个节点，实现为一个 struct ，内容很丰富： tree.htypedefstruct_xmlNodexmlNode;typedefxmlNode*xmlNodePtr;struct_xmlNode{void*_private;/*application data*/xmlElementTypetype;/*type number, must be second !*/constxmlChar*name;/*the name of the node, or the entity*/struct_xmlNode*children;/*parent->childs link*/struct_xmlNode*last;/*last child link*/struct_xmlNode*parent;/*child->parent link*/struct_xmlNode*next;/*next sibling link*/struct_xmlNode*prev;/*previous sibling link*/struct_xmlDoc*doc;/*the containing document*//*End of common part*/xmlNs*ns;/*pointer to the associated namespace*/xmlChar*content;/*the content*/struct_xmlAttr*properties;/*properties list*/xmlNs*nsDef;/*namespace definitions on this node*/void*psvi;/*for type/PSVI informations*/unsignedshortline;/*line number*/unsignedshortextra;/*extra data for XPath/XSLT*/};可以看到，节点之间是以链表和树两种方式同时组织起来的，next和prev指针可以组成链表，而parent和children可以组织为树。所有节点都是文档 xmlDoc 节点的直接或间接子节点。同时还有以下重要元素：
节点中的文字内容： content ；
节点所属文档： doc ；
节点名字： name ；
节点的 namespace： ns ；
节点属性列表： properties ；
xml 文档的操作其根本原理就是在节点之间移动、查询节点的各项信息，并进行增加、删除、修改的操作。 xmlDocSetRootElement 函数可以将一个节点设置为某个文档的根节点，这是将文档与节点连接起来的重要手段，当有了根结点以后，所有子节点就可以依次连接上根节点，从而组织成为一个 xml 树。
### 创建 XML 文档
创建一个 XML 文档流程如下：
用 xmlNewDoc 函数创建一个文档指针 doc;
用 xmlNewNode 函数创建一个节点指针 root_node;
用 xmlDocSetRootElement 将 root_node 设置为 doc 的根节点;
给 root_node 添加一系列子节点，并设置字节点的内容和属性;
用 xmlSaveFile 保存 xml 到文件;
用 xmlFreeDoc 函数关闭文档指针，清除内存。
#### 示例
下面用一个例子说明一些函数的使用，和创建一个 XML 文档的大致步骤：
\#include<stdio.h>\#include<stdlib.h>\#include<libxml/parser.h>\#include<libxml/tree.h>intmain(intargc,char**argv)
{xmlDocPtrpdoc=NULL;xmlNodePtrproot_node=NULL,pnode=NULL,pnode1=NULL;//创建一个新文档并设置 root 节点//一个 XML 文件只有一个 root 节点pdoc = xmlNewDoc (BAD_CAST"1.0");
  proot_node = xmlNewNode (NULL, BAD_CAST"根节点");
  xmlNewProp (proot_node, BAD_CAST"版本", BAD_CAST"1.0");
  xmlDocSetRootElement (pdoc, proot_node);
  pnode = xmlNewNode (NULL, BAD_CAST"子节点1");//创建上面 pnode 的子节点xmlNewChild (pnode,NULL, BAD_CAST"子子节点1", BAD_CAST"信息");//添加子节点到 root 节点xmlAddChild (proot_node, pnode);
  pnode1 = xmlNewNode (NULL, BAD_CAST"子子节点1");
  xmlAddChild (pnode, pnode1);
  xmlAddChild (pnode1,xmlNewText (BAD_CAST"这是更低的节点,子子子节点1"));//还可以这样直接创建一个子节点到 root 节点上xmlNewTextChild (proot_node,NULL, BAD_CAST"子节点2", BAD_CAST"子节点2的内容");
  xmlNewTextChild (proot_node,NULL, BAD_CAST"子节点3", BAD_CAST"子节点3的内容");//保存 xml 为文件,如果没有给出文件名参数，就输出到标准输出xmlSaveFormatFileEnc (argc > 1 ? argv[1]:"-", pdoc,"UTF-8", 1);//释放资源xmlFreeDoc (pdoc);
  xmlCleanupParser ();
  xmlMemoryDump ();return0;
}编译这个例子，先看看系统里面的 libxml2 库的 pkgconfig 信息：
root@jianlee:~/lab/xml\# cat /usr/lib/pkgconfig/libxml-2.0.pc
prefix=/usr
exec_prefix=${prefix}
libdir=${exec_prefix}/lib
includedir=${prefix}/include
modules=1
Name: libXML
Version: 2.6.32
Description: libXML library version2.
Requires:
Libs: -L${libdir} -lxml2
Libs.private:  -lz  -lm
Cflags: -I${includedir}/libxml2
root@jianlee:~/lab/xml\# pkg-config libxml-2.0 --cflags --libs
-I/usr/include/libxml2  -lxml2编译：
root@jianlee:~/lab/xml\# gcc -Wall `pkg-config libxml-2.0 --cflags --libs` create_xml.c如果没有修改源程序，输出应该是这样：
root@jianlee:~/lab/xml\# ./a.out
<?xml version="1.0" encoding="UTF-8"?>
<根节点 版本="1.0">
  <子节点1>
    <子子节点1>信息</子子节点1>
    <子子节点1>这是更低的节点,子子子节点1</子子节点1>
  </子节点1>
  <子节点2>子节点2的内容</子节点2>
  <子节点3>子节点3的内容</子节点3>
</根节点>
#### 示例补充说明
##### 输出的各节点不要在一行
上面使用下面方式保存 xml 文档，输出的文件各子节点间自动加入回车：
// 保存 xml 为文件,如果没有给出文件名参数，就输出到标准输出
  xmlSaveFormatFileEnc (argc > 1 ? argv[1]:"-", pdoc, "UTF-8", 1);如果把上面的 1 换成 0 ，输出格式是放在一行。
##### 用到的函数说明
上面涉及几个函数和类型定义，不过意思很明了，下面解释一个（重要的是自己动手写程序，反复实验，所谓熟能生巧）。
**xmlDocPtr**指向 XML 文档对象的指针**xmlNodePtr**指向 XML 文档对象中的节点对象（根节点和子节点都是一样的）**xmlNewDoc**创建一个 XML 文档对象**xmlNewNode**创建一个 XML 文档的指针对象**xmlNewProp**给一个节点增加属性信息，包括在 <> 中，如：xmlNewProp (proot_node, BAD_CAST "版本", BAD_CAST "1.0");
> 最后显示是这个样子：
<根节点 版本="1.0">**xmlDocSetRootElement**设置 XML 文档对象的根节点，只有一个根节点**xmlNewChild**指定一个节点，会创建这个节点的子节点。这样不需要使用 xmlNewNode 创建一个节点，再使用 xmlAddChild 添加到其父节点中。**xmlAddChild**把一个节点设置为另外一个节点的子节点。**xmlNewText**创建一个描述节点，没有 <> 符号，需要添加到其他节点上。比如上例中的：xmlAddChild (pnode1,xmlNewText (BAD_CAST "这是更低的节点,子子子节点1"));
> 会出现下面的结果：
<子子节点1>这是更低的节点,子子子节点1</子子节点1>**xmlNewTextChild**和 xmlNewText 的区别如同 xmlNewNodeChild 和 xmlNewNode 的区别一样！**xmlSaveFormatFileEnc**保存 xml 对象为文件。**xmlFreeDoc**释放 xml 对象**xmlCleanupParser**清理**xmlMemoryDump**清理
### 解析 XML 文档
解析一个xml文档，从中取出想要的信息，例如节点中包含的文字，或者某个节点的属性，其流程如下：
用 xmlReadFile 函数读出一个文档指针 doc ；
用 xmlDocGetRootElement 函数得到根节点 curNode ；
curNode->xmlChildrenNode 就是根节点的子节点集合 ；
轮询子节点集合，找到所需的节点，用 xmlNodeGetContent 取出其内容 ；
用 xmlHasProp 查找含有某个属性的节点 ；
取出该节点的属性集合，用 xmlGetProp 取出其属性值 ；
用 xmlFreeDoc 函数关闭文档指针，并清除本文档中所有节点动态申请的内存。
注意： 节点列表的指针依然是 xmlNodePtr ，属性列表的指针也是 xmlAttrPtr ，并没有 xmlNodeList 或者 xmlAttrList 这样的类型 。看作列表的时候使用它们的 next 和 prev 链表指针来进行轮询 。只有在 Xpath 中有 xmlNodeSet 这种类型。
#### 示例
\#include<stdio.h>\#include<stdlib.h>\#include<libxml/parser.h>\#include<libxml/tree.h>intmain(intargc,char**argv)
{xmlDocPtrpdoc=NULL;xmlNodePtrproot=NULL,curNode=NULL;char*psfilename;if(argc < 1)
    {
      printf ("用法: %s xml文件名\n", argv[0]);
      exit (1);
    }
  psfilename = argv[1];//打开 xml 文档//xmlKeepBlanksDefault(0);pdoc = xmlReadFile (psfilename,"UTF-8", XML_PARSE_RECOVER);if(pdoc ==NULL)
    {
      printf ("打开文件 %s 出错!\n", psfilename);
      exit (1);
    }//获取 xml 文档对象的根节对象proot = xmlDocGetRootElement (pdoc);if(proot ==NULL)
    {
      printf("错: %s 是空文档（没有root节点）！\n", psfilename);
      exit (1);
    }/*我使用上面程序创建的 xml 文档，它的根节点是“根节点”，这里比较是否
     正确。*/if(xmlStrcmp (proot->name, BAD_CAST"根节点") != 0)
    {
      printf ("错误文档");
      exit (1);
    }/*如果打开的 xml 对象有 version 属性，那么就输出它的值。*/if(xmlHasProp (proot, BAD_CAST"版本"))
    {xmlChar*szAttr= xmlGetProp (proot, BAD_CAST"版本");
      printf ("版本: %s \n根节点：%s\n", szAttr, proot->name);
    }else{
      printf (" xml 文档没有版本信息\n");
    }
  curNode = proot->xmlChildrenNode;charn=0;while(curNode !=NULL)
    {if(curNode->name != BAD_CAST"text")
        {
          printf ("子节点%d： %s\n", n++,curNode->name);
        }
      curNode = curNode->next;
    }/*关闭和清理*/xmlFreeDoc (pdoc);
  xmlCleanupParser ();return0;
}编译运行（使用上例创建的 my.xml 文件）：
root@jianlee:~/lab/xml\# cat my.xml
<?xml version="1.0" encoding="UTF-8"?>
<根节点 版本="1.0">
  <子节点1>
    <子子节点1>信息</子子节点1>
    <子子节点1>这是更低的节点,子子子节点1</子子节点1>
  </子节点1>
  <子节点2>子节点2的内容</子节点2>
  <子节点3>子节点3的内容</子节点3>
</根节点>
root@jianlee:~/lab/xml\# gcc -Wall `pkg-config libxml-2.0 --cflags --libs` read_xml.c
root@jianlee:~/lab/xml\# ./a.out my.xml
版本: 1.0
根节点：根节点
子节点0： text
子节点1： 子节点1
子节点2： text
子节点3： 子节点2
子节点4： text
子节点5： 子节点3
子节点6： text为什么 my.xml 文件中显示只有 ”子节点1“、 ”子节点2“和 “子节点3”三个子节点，而程序显示有 7 个子节点呢？！而且 0、2、4、6 都是 text 名字？
这是因为其他四个分别是元素前后的空白文本符号，而 XML 把它们也当做一个 Node ！元素是 Node 的一种类型。XML 文档对象模型 (DOM) 定义了几种不同的 Nodes 类型，包括 Elements（如 files 或者 age）、Attributes（如 units）和 Text（如 root 或者 10）。元素可以具有子节点。
在打开 xml 文档之前加上一句（取消上面程序中的此句注释就可以）：
xmlKeepBlanksDefault(0);或者使用下面参数读取 xml 文档：
//读取xml文件时忽略空格
doc = xmlReadFile(docname, NULL, XML_PARSE_NOBLANKS);这样就可以按我们所想的运行了：
root@jianlee:~/lab/xml\# gcc -Wall `pkg-config libxml-2.0 --cflags --libs` read_xml.c
root@jianlee:~/lab/xml\# ./a.out my.xml
版本: 1.0
根节点：根节点
子节点0： 子节点1
子节点1： 子节点2
子节点2： 子节点3还有一点注意： my.xml 文件中的子节点名字一次是 “子节点1”、“子节点2”、 “子节点3”。程序中的 n 值确是从 0 开始计算。从 0 还是 1 是个人喜好。我有时候喜好从 0 开始，有时候喜好从 1 开始。
**xmlFreeDoc**释放文档指针。特别注意，当你调用 xmlFreeDoc 时，该文档所有包含的节点内存都被释放，所以一般来说不需要手动调用 xmlFreeNode 或者 xmlFreeNodeList 来释放动态分配的节点内存，除非你把该节点从文档中移除了。一般来说，一个文档中所有节点都应该动态分配，然后加入文档，最后调用 xmlFreeDoc 一次释放所有节点申请的动态内存，这也是为什么我们很少看见 xmlNodeFree 的原因。**xmlSaveFile**将文档以默认方式存入一个文件。**xmlSaveFormatFileEnc**可将文档以某种编码/格式存入一个文件中,创建 xml 文档是的示例中用到
### 修改 xml 文档
首先打开一个已经存在的xml文档，顺着根结点找到需要添加、删除、修改的地方，调用相应的xml函数对节点进行增、删、改操作。
#### 删除节点
删除节点使用下面方法：
if(!xmlStrcmp(curNode->name, BAD_CAST"newNode1"))
  {xmlNodePtrtempNode;
    tempNode = curNode->next;
    xmlUnlinkNode(curNode);
    xmlFreeNode(curNode);
    curNode = tempNode;continue;
   }即将当前节点从文档中断链（unlink），这样本文档就不会再包含这个子节点。这样做需要使用一个临时变量来存储断链节点的后续节点，并记得要手动删除断链节点的内存。
#### 示例
\#include<stdio.h>\#include<stdlib.h>\#include<libxml/parser.h>intmain(intargc,char*argv[])
{xmlDocPtrdoc;//定义解析文档指针xmlNodePtrcurNode;//定义结点指针(你需要它为了在各个结点间移动)char*szDocName;if(argc <= 1)
    {
      printf("Usage: %s docname\n", argv[0]);return(0);
    }
  szDocName = argv[1];
  xmlKeepBlanksDefault(0);
  doc = xmlReadFile(szDocName,"UTF-8",XML_PARSE_RECOVER);//解析文件if(NULL== doc)
    {
      fprintf(stderr,"Document not parsed successfully. \n");return-1;
    }
  curNode = xmlDocGetRootElement(doc);/*检查确认当前文档中包含内容*/if(NULL== curNode)
    {
      fprintf(stderr,"empty document\n");
      xmlFreeDoc(doc);return-1;
    }
  curNode = curNode->children;while(NULL!= curNode)
    {//删除 "子节点1"if(!xmlStrcmp(curNode->name, BAD_CAST"子节点1"))
        {xmlNodePtrtempNode;
          tempNode = curNode->next;
          xmlUnlinkNode(curNode);
          xmlFreeNode(curNode);
          curNode = tempNode;continue;
        }//修改 "子节点2" 的属性值if(!xmlStrcmp(curNode->name, BAD_CAST"子节点2"))
        {
          xmlSetProp(curNode,BAD_CAST"属性1", BAD_CAST"设置");
        }//修改 “子节点2” 的内容if(!xmlStrcmp(curNode->name, BAD_CAST"子节点2"))
        {
          xmlNodeSetContent(curNode, BAD_CAST"内容变了");
        }//增加一个属性if(!xmlStrcmp(curNode->name, BAD_CAST"子节点3"))
        {
          xmlNewProp(curNode, BAD_CAST"新属性", BAD_CAST"有");
        }//增加 "子节点4"if(!xmlStrcmp(curNode->name, BAD_CAST"子节点3"))
        {
          xmlNewTextChild(curNode,NULL, BAD_CAST"新子子节点1", BAD_CAST"新内容");
        }
      curNode = curNode->next;
    }//保存文件xmlSaveFormatFileEnc (szDocName, doc,"UTF-8",1);
  xmlFreeDoc (doc);
  xmlCleanupParser ();
  xmlMemoryDump ();return0;
}编译运行：
root@jianlee:~/lab/xml\# cat my.xml
<?xml version="1.0" encoding="UTF-8"?>
<根节点 版本="1.0">
  <子节点1>
    <子子节点1>信息</子子节点1>
    <子子节点1>这是更低的节点,子子子节点1</子子节点1>
  </子节点1>
  <子节点2>子节点2的内容</子节点2>
  <子节点3>子节点3的内容</子节点3>
</根节点>
root@jianlee:~/lab/xml\# gcc -Wall `pkg-config libxml-2.0 --cflags --libs` modify_xml.c
root@jianlee:~/lab/xml\# ./a.out my.xml
root@jianlee:~/lab/xml\# cat my.xml
<?xml version="1.0" encoding="UTF-8"?>
<根节点 版本="1.0">
  <子节点2 属性1="设置">内容变了</子节点2>
  <子节点3 新属性="有">子节点3的内容<新子子节点1>新内容</新子子节点1></子节点3>
</根节点>
root@jianlee:~/lab/xml\# ./a.out my.xml  \# 看看再运行一次的结果！
root@jianlee:~/lab/xml\# cat my.xml
<?xml version="1.0" encoding="UTF-8"?>
<根节点 版本="1.0">
  <子节点2 属性1="设置">内容变了</子节点2>
  <子节点3 新属性="有" 新属性="有">子节点3的内容<新子子节点1>新内容</新子子节点1><新子子节点1>新内容</新子子节点1></子节点3>
</根节点>
## Xpath — 处理大型 XML 文档
## libxml2 库函数
### 要注意的函数
#### xmlKeepBlanksDefault
int xmlKeepBlanksDefault (int val)设置是否忽略空白节点,比如空格,在分析前必须调用,默认值是0,最好设置成1.
xmlKeepBlanksDefault(0) 除了在读入xml文件时忽略空白之外，还会在写出xml 文件时在每行前面放置缩进（indent）。如果使用xmlKeepBlanksDefault(1) 则你会发现每行前面的缩进就没有了，但不会影响回车换行。
#### xmlSaveFormatFile
// 保存 xml 为文件,如果没有给出文件名参数，就输出到标准输出
  xmlSaveFormatFileEnc (argc > 1 ? argv[1]:"-", pdoc, "UTF-8", 1);xmlSaveFormatFile 的 format 参数设置成 0，保存后的 xml 文档里是会把所有的结点都放到一行里显示。设置为 1,就可以自动添加回车。
### 读取 xml 文件
#### xmlParseFile
xmlDocPtr xmlParseFile (const char * filename)以默认方式读入一个 UTF-8 格式的 xml 文档, 并返回一个文档对象指针 <libxml/tree.h>
#### xmlReadFile
指定编码读取一个 xml 文档，返回指针。
### xml 操作基本结构及其指针类型
#### xmlDoc, xmlDocPtr
文档对象的结构体及其指针
#### xmlNode, xmlNodePtr
节点对象的结构体及其指针
#### xmlAttr, xmlAttrPtr
节点属性的结构体及其指针
#### xmlNs, xmlNsPtr
节点命名空间的结构及其指针
### 根节点相关函数
#### xmlDocGetRootElement
xmlNodePtr xmlDocGetRootElement (xmlDocPtr doc)获取文档根节点
#### xmlDocSetRootElement
xmlNodePtr xmlDocSetRootElement (xmlDocPtr doc, xmlNodePtr root)设置文档根节点
### 创建子节点相关函数
#### xmlNewNode
xmlNodePtr xmlNewNode (xmlNsPtr ns, const xmlChar * name)创建新节点
#### xmlNewChild
xmlNodePtr xmlNewChild (xmlNodePtr parent, xmlNsPtr ns, const xmlChar * name, const xmlChar * content)创建新的子节点
#### xmlCopyNode
xmlNodePtr xmlCopyNode (const xmlNodePtr node, int extended)复制当前节点
### 添加子节点相关函数
#### xmlAddChild
xmlNodePtr xmlAddChild (xmlNodePtr parent, xmlNodePtr cur)给指定节点添加子节点
#### xmlAddNextSibling
xmlNodePtr xmlAddNextSibling (xmlNodePtr cur, xmlNodePtr elem)添加后一个兄弟节点
#### xmlAddPrevSibling
xmlNodePtr xmlAddPrevSibling (xmlNodePtr cur, xmlNodePtr elem)添加前一个兄弟节点
#### xmlAddSibling
xmlNodePtr xmlAddSibling (xmlNodePtr cur, xmlNodePtr elem)添加兄弟节点
### 属性相关函数
#### xmlNewProp
xmlAttrPtr xmlNewProp (xmlNodePtr node, const xmlChar * name, const xmlChar * value)创建新节点属性
#### xmlGetProp
xmlChar * xmlGetProp (xmlNodePtr node, const xmlChar * name)读取节点属性
#### xmlSetProp
xmlAttrPtr xmlSetProp (xmlNodePtr node, const xmlChar * name, const xmlChar * value)设置节点属性


