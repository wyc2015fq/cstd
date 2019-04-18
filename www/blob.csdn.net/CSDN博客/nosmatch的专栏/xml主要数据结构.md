# xml主要数据结构 - nosmatch的专栏 - CSDN博客
2012年04月24日 16:36:57[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：9734
下面介绍的是libxml主要的数据类型，对于应用编程来说，这些数据类型是需要了解和掌握的。
1．内部字符类型xmlChar
xmlChar是libxml2中的字符类型，库中所有字符、字符串都是基于这个数据类型。它的定义在xmlstring.h中，定义说明如下：
typedef unsigned char xmlChar;
使用unsigned char作为内部字符格式是考虑到它能很好适应UTF-8编码，而UTF-8编码正是libxml2的内部编码，其他格式的编码要转换为这个编码才能在libxml2中使用。
xmlChar *常在libxml2中作为字符串指针类型，很多函数会返回一个动态分配内存的xmlChar *变量，使用这样的函数时需要手工删除内存。
2．xmlChar相关函数
如同标准C中的char类型一样，xmlChar也有动态内存分配、字符串操作等相关函数。例如xmlMalloc是动态分配内存的函数，xmlFree是配套的释放内存函数，xmlStrcmp是字符串比较函数等。基本上xmlChar字符串相关函数都在xmlstring.h中定义，而动态内存分配函数在xmlmemory.h头文件中定义。
3．xmlChar*与其他类型之间的转换
在实际编程中，总是需要在xmlChar *和char *之间进行强制类型转换，所以定义了一个宏BAD_CAST，其定义如下：
#define BAD_CAST (xmlChar *)
4．XML中常用到的重定义
    在XML程序中，会经常看到xmlChildrenNode这个名称，其实这个名称是定义在tree.h中的重定义。其重定义如下：
#define xmlChildrenNode children
5．文档类型xmlDoc、指针xmlDocPtr
xmlDoc是一个struct，保存了一个xml的相关信息，例如文件名、文档类型、子节点等，xmlDocPtr等于xmlDoc * 。与文档指针相关函数有如下几个。
xmlNewDoc函数创建一个新的文档指针。
xmlParseFile函数以默认方式读入一个UTF-8格式的文档，并返回文档指针。
xmlReadFile函数读入一个带有某种编码的xml文档，并返回文档指针。
xmlFreeDoc释放文档指针。特别注意，当调用xmlFreeDoc时，该文档所有包含的节点内存都会被释放，所以一般来说不需要手工调用xmlFreeNode或者xmlFreeNodeList来释放动态分配的节点内存，除非把该节点从文档中移除了。一般来说，一个文档中所有节点都应该动态分配，然后加入文档，最后调用xmlFreeDoc一次释放所有节点申请的动态内存，这也是为什么我们在程序中很少看见xmlNodeFree的原因。
xmlSaveFile将文档以默认方式存入一个文件。
xmlSaveFormatFileEnc可将文档以某种编码格式存入一个文件中。
6．节点类型xmlNode、指针xmlNodePtr
节点是XML中最重要的元素，xmlNode代表XML文档中的一个节点，实现为一个struct，此结构内容很丰富也很重要，其定义在tree.h中，具体说明如下：
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
可以看到，节点之间是以链表和树两种方式同时组织起来的，next和prev指针可以组成链表，而parent和children可以组织为树。同时此结构还有以下重要成员：
        content：节点中的文字内容。
        doc：节点所属文档。
        name：节点名字。
        ns：节点的名字空间。
        properties：节点属性列表。
XML文档的操作其根本原理就是在节点之间移动、查询节点的各项信息，并进行增加、删除、修改等操作。
xmlDocSetRootElement函数可以将一个节点设置为某个文档的根节点，这是将文档与节点连接起来的重要手段，当有了根结点以后，所有子节点就可以依次连接上根节点，从而组织成为一个XML树。
7．XML属性
XML属性也是编程中经常用到的结构，其定义如下：
struct _xmlAttr {
    void *    _private;   /* application data */
    xmlElementType type;  /* XML_ATTRIBUTE_NODE, must be second !  */
    const xmlChar *    name ;  /*the name of the property */
    struct _xmlNode *  children;  /*the value of the property */
    struct _xmlNode *  last;  /*NULL */
    struct _xmlNode *  parent;  /*child->parent link */
    struct _xmlAttr *  next;  /*next sibling link */
    struct _xmlAttr *  prev;  /*previous sibling link */
    struct _xmlDoc *   doc;  /*the containing document */
    xmlNs *   ns;  /*pointer to the associated namespace */
    xmlAttributeType   atype;  /*the attribute type if validating */
    void *    psvi;  /*for type/PSVI informations */
}
8．节点集合类型xmlNodeSet、指针类型xmlNodeSetPtr
节点集合代表一个由节点组成的变量，节点集合只作为XPath的查询结果而出现，因此被定义在xpath.h中，其定义如下：
/* A node-set (an unordered collection of nodes without duplicates). */
typedef struct _xmlNodeSet xmlNodeSet;
typedef xmlNodeSet *xmlNodeSetPtr;
struct _xmlNodeSet {
    int nodeNr;          /* number of nodes in the set */
    int nodeMax;      /* size of the array as allocated */
    xmlNodePtr *nodeTab;/* array of nodes in no particular order */
    /* @@ with_ns to check wether namespace nodes should be looked at @@ */
};
可以看出，节点集合有三个成员，分别是节点集合的节点数、最大可容纳的节点数，以及节点数组头指针。对节点集合中各个节点的访问方法如下：
xmlNodeSetPtr nodeset = XPath查询结果;
for (int i = 0; i < nodeset->nodeNr; i++)
{
 nodeset->nodeTab[i];
}
