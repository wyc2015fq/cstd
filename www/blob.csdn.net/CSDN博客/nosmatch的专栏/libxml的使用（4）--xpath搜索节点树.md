# libxml的使用（4）--xpath搜索节点树 - nosmatch的专栏 - CSDN博客
2012年05月08日 16:49:16[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：4336
在libxml的tutorial中介绍了一种用关键字查找节点的方法，这种方法将使用打xpath系列API。由于我才刚刚接触libxml，所以我对xpath的认识也仅仅是在tutorial提供的功能之内了。废话少说，直接进入整体。
我们在操作xml文件是经常需要根据特定的条件查找一系列的节点，为了实现这样的功能，我们需要一个xmlXPathContextPtr和一个expression。我们调用xmlXPathEvalExpression函数来得到一个xmlXPathObjectPtr指针，这个指针包含了一个xmlNodeSetPtr，其中有一个变量nodeTab是我们所需要的节点数组。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)
- xmlXPathObjectPtr ret = NULL;  
- xmlXPathContextPtr con = NULL;  
- 
- con = xmlXPathNewContext(doc);  
- ret = xmlXPathEvalExpression((xmlChar*)expr, con);  
- xmlXPathFreeContext(con);  
这样我们就得到了查询的结果了。expr是查询的条件，tutorial给的例子里，这个条件是“//keyword”，表示找出所有名称为keyword的节点。至于其他的条件，我现在还不知道。
得到了查询的结果，我们就要对结果进行处理。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)
- if(NULL == ret) {  
-     fprintf(stderr, "eval func error\n");  
-     exit(1);  
- }  
- if(xmlXPathNodeSetIsEmpty(ret->nodesetval)){  
-     fprintf(stderr, "node set empty\n");  
-     xmlXPathFreeObject(ret);  
-     exit(1);  
- }  
- xmlNodeSetPtr nodeset = ret->nodesetval;  
- int i;  
- for(i = 0; i < nodeset->nodeNr; i ++) {  
- //handle the node
- }  
- xmlXPathFreeObject(ret);  
下面是一个程序的实例。用于提取出网页中的链接：
web.html
**[html]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)
- <html>
- <head>
- <title>web</title>
- </head>
- <body>
- <ahref="www.baidu.com">baidu</a>
- <ahref="www.google.com">Google</a>
- </body>
- </html>
link.c
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)
- #include <stdio.h>
- #include <string.h>
- #include <stdlib.h>
- #include <libxml/parser.h>
- #include <libxml/xpath.h>
- 
- static xmlDocPtr  
- getDocPtr(char* docname) {  
-     xmlDocPtr doc = NULL;  
-     xmlKeepBlanksDefault(0);  
- 
-     doc = xmlParseFile(docname);  
- if(NULL == doc) {  
-         fprintf(stderr, "document cannot be parsed!\n");  
-         exit(1);  
-     }  
- return doc;  
- }  
- 
- static xmlXPathObjectPtr  
- getXPathObjectPtr(xmlDocPtr doc, xmlChar* xpath_exp) {  
-     xmlXPathObjectPtr result;  
-     xmlXPathContextPtr context;  
- 
-     context = xmlXPathNewContext(doc);  
-     result = xmlXPathEvalExpression((const xmlChar*)xpath_exp, context);  
-     xmlXPathFreeContext(context);  
- 
- if(NULL == result) {  
-         fprintf(stderr, "eval expression error!\n");  
- return NULL;  
-     }  
- 
- if(xmlXPathNodeSetIsEmpty(result->nodesetval)) {  
-         fprintf(stderr, "empty node set!\n");  
-         xmlXPathFreeObject(result);  
- return NULL;  
-     }  
- return result;  
- }  
- 
- int main() {  
- char* docname = "web.html";  
-     xmlDocPtr doc = NULL;  
-     xmlXPathObjectPtr xpath_obj = NULL;  
-     xmlNodeSetPtr nodeset = NULL;  
-     xmlChar* xpath_exp = (xmlChar*)"//a";  
-     xmlChar* uri;  
- 
-     doc = getDocPtr(docname);  
- 
-     xpath_obj = getXPathObjectPtr(doc, xpath_exp);  
- 
- if(NULL != xpath_obj) {  
-         nodeset = xpath_obj->nodesetval;  
- int i = 0;  
- for(i = 0; i < nodeset->nodeNr; i ++) {  
-             uri = xmlGetProp(nodeset->nodeTab[i],(const xmlChar*)"href");  
-             printf("link address:%s\n",uri);  
-             xmlFree(uri);  
-         }  
-         xmlXPathFreeObject(xpath_obj);  
-     }  
-     xmlFreeDoc(doc);  
-     xmlCleanupParser();  
- 
- return 1;  
- }  
输出结果为：
**[html]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262645#)
- link address:www.baidu.com  
- link address:www.google.com  
