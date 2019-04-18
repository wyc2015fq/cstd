# libxml的使用（1）--读取xml - nosmatch的专栏 - CSDN博客
2012年05月08日 16:47:11[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：10344
       libxml是一个用于解析xml文件的库，在各个平台下都能使用，也支持多种语言，如c，python等。这里是[官方网站](http://xmlsoft.org/)。上面有libxml的api和一些code examples，都是英文的。不过比较简单。
libxml的基础功能就是对xml的读和写。下面将简单介绍libxml的读的功能。（大部分内容是参照libxml tutorial 的文档）
0 编译程序
因为本人是在linux下用c语言来介绍libxml的，所以使用了gcc编译器。其他的编译命令请参照官网。
我们知道，gcc 最简单的编译命令为gcc filename.c 。只要给这个命令添加头文件和链接库的地址就可以使用libxml。而libxml也提供了一个脚本xml2-config来配置这些地址。所以编译时将命令改为gcc filename.c `xml2-config --cflags --libs`即可。cflags在编译时提供头文件的地址，而libs在链接时提供库文件的地址。
1 分析文件--parse file
先简单介绍xml文件。
<?xml version="1.0" encoding="UTF-8"?>
<root>
<node1>content1</node1>
<node2 attribute="yes">content2</node2>
<node3>
<subnode>go</subnode>
</node3>
</root>
上面是一个简单的xml文件。从文件中很容易就能看出整个文件的结构和要表达的意思。下面我会结合xml的基本类型介绍这个文件。
文件的第一行是xml文件的一些属性，可以看出编码方式是utf-8 。libxml只能处理uft-8 和 utf-16编码的文件，如果你的文件不是这两种编码，需要在使用前进行转换。
<root></root>是xml的一个节点，即xmlNode。而xmlNodePtr 表示指向xmlNode的指针--xmlNode*。<node1></node1>,<node2></node2>,<node3></node3>是这个node的子node，即xmlNodePtr->children或xmlNodePtr->xmlChildrenNode。所以<subnode></subnode>就是<node3></node3>的子node。
接下来就是解析文件了。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- #include <stdio.h>
- #include <string.h>
- #include <stdlib.h>
- #include <libxml/tree.h>
- #include <libxml/parser.h>
文件中需要包含这两个头文件。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- xmlDocPtr doc = NULL;  
- xmlNodePtr node = NULL;  
- 
- doc = xmlParseFile(filename);  
- 
- if(NULL  == doc) {  
-     fprintf(stderr, "parse error\n");  
-     exit(1);  
- }  
xmlDocPtr 表示指向xmlDoc的指针。从tutorial中我们可以知道，xmlDoc是一个包含了已解析的文件生成的节点树的结构体。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- node = xmlDocGetRootElement(doc);  
- 
- if(NULL == node) {  
-     fprintf(stderr, "doc has no content\n");  
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
-     xmlFreeDoc(doc);  
-     exit(1);  
- }  
xmlDocGetRootElement(doc)可以得到整个文件的根节点，所有的操作都是从根节点开始的。
现在我们已经有了一个包含这个节点树的结构体指针xmlDocPtr doc，有了一个可以操作节点的结构体指针xmlNodePtr node，我们就可以读取各个节点的信息了。
节点包含这么几个信息：
node->name:节点的名字，如node1,node2,subnode等
node->xmlChildrenNode:节点的子节点
node->last:节点的最后一个子节点
node->parent:节点的父节点
node->next:节点的兄弟节点，对于node1来说，node2和node3都是其兄弟节点，node1的next指向node2
由于节点的内容是其子节点(对于node1，content1可以说是其子节点)，所以我们需要使用xmlNodeGetContent来取出内容。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- node = node->xmlChildrenNode;  
- while(node != NULL) {  
-     printf("name=%s content=%s\n",  
-                node->name,  
-                (char*)xmlNodeGetContent(node));  
-     node=node->next;  
- }  
这是完整的源码。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- #include <stdio.h>
- #include <string.h>
- #include <stdlib.h>
- #include <libxml/parser.h>
- 
- staticint
- parseDoc(char* docname) {  
-     xmlDocPtr doc;  
-     xmlNodePtr cur;  
- //xmlKeepBlanksDefault(0);
- 
-     doc = xmlParseFile(docname);  
- 
- if(doc == NULL) {  
-         fprintf(stderr, "doc error!\n");  
- return 0;  
-     }  
- 
-     cur = xmlDocGetRootElement(doc);  
- 
- if(cur == NULL) {  
-         fprintf(stderr, "root error!\n");  
-         xmlFreeDoc(doc);  
- return 0;  
-     }  
- 
- if(xmlStrcmp(cur->name, (const xmlChar*)"root")) {  
-         printf("end\n");  
- return 0;  
-     }  
- 
-     cur = cur->children;  
- while(cur != NULL) {  
-         printf("name=%s content=%s\n",cur->name,   
-                 (char*)xmlNodeGetContent(cur));  
- //cur->content);
-         cur = cur->next;  
-     }  
-     xmlFreeDoc(doc);  
- return 0;  
- }  
- 
- int main() {  
- char* docname = "story.xml";  
-     parseDoc(docname);  
- return 1;  
- }  
编译链接后执行，会发现输出的结果如下：
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- name=text content=  
- 
- name=node1 content=content1  
- name=text content=  
- 
- name=node2 content=content2  
- name=text content=  
- 
- name=node3 content=  
-         go  
- 
- name=text content=  
这是因为libxml默认将各个节点间的空格当作一个节点，只要在调用xmlParseFile之前调用xmlKeepBlanksDefault(0)即可。
修改后结果如下：
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- name=node1 content=content1  
- name=node2 content=content2  
- name=node3 content=go  
2 读取内存中的xml文档。
很多时候我们需要对内存中的xml文档进行操作。比如在网络编程的时候经常会从server那里接受一些客户端的配置信息，这些配置信息大部分都是使用xml语言描述的。在你将这些信息读入到buffer中后，你无需将他们写入文件当中再分析，直接可以调用xmlReadMemory函数就可以得到一个xmlDocPtr。这个函数接受五个参数，第一个参数为缓存区的指针，第二个参数为缓存区大小，其他参数具体看API文档。当然另外一个函数xmlReadDoc也能实现这样的功能。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7256634#)
- staticchar* config = "<config />";  
- xmlDocPtr doc = NULL;  
- xmlKeepBlanksDefault(0);  
- 
- doc = xmlReadMemory(config, strlen(config), "in_memory.xml", NULL, 0);  
- 
- //handle doc
- 
- xmlFreeDoc(doc);
  
