# libxml的使用（2）--读取节点属性 - nosmatch的专栏 - CSDN博客
2012年05月08日 16:48:07[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：5800
在上一篇文章当中，我读取了各个节点的名字和内容，现在我将读取各个节点的属性。
**[html]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)
- <?xmlversion="1.0"encoding="UTF-8"?>
- <root>
- <node1>content1</node1>
- <node2attribute="yes">content2</node2>
- <node3>
- <subnode>go</subnode>
- </node3>
- </root>
这是上一篇文章中提到的xml文件。
在node2这个节点上有一个属性attribute，其值是yes。我们可以使用xmlGetProp这个函数将其提取出来。这个函数包含了两个变量，一个是将要提取属性的节点指针xmlNodePtr，另一个是属性的名称。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7257148#)
- xmlChar* attr_value = NULL;  
- if(!xmlStrcmp(node->name, (const xmlChar*)"node2")) {  
-     attr_value = xmlGetProp(node, "attribute");  
-     printf("attribute value:%s\n",attr_value);  
-     xmlFree(attr_value);  
- }  
这样我们就提取出了yes这个字符串了！
