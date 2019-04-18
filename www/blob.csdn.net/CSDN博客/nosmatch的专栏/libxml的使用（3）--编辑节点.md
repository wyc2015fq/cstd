# libxml的使用（3）--编辑节点 - nosmatch的专栏 - CSDN博客
2012年05月08日 16:48:33[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：3912
libxml读取的基本功能已经介绍过了，现在将介绍libxml编写的基本功能。
编写操作包含节点的添加，删除和修改。
对于添加，我们需要调用xmlNewTextChild函数来添加节点，需要xmlNewProp函数来添加属性。
xmlNewTextChild接受四个参数，第一个参数为新添加节点的父节点，第二个参数为新节点的命名空间namespace，第三个参数为节点的名称，第四个参数为节点的内容。函数返回指向新节点的指针xmlNodePtr。
如果要在根节点下添加一个新的节点node，他的内容设为content，则：
**[html]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)
- xmlDocPtr doc = NULL;  
- xmlNodePtr cur = NULL;  
- xmlNodePtr newNode = NULL;  
- 
- xmlKeepBlanksDefault(0);  
- 
- doc = xmlParseFile(docname);  
- cur = xmlDocGetRootElement(doc);  
- 
- newNode = xmlNewTextChild(cur, NULL, "node", "content");  
在添加后需要调用xmlSaveFormatFile函数将结果保存在硬盘上。xmlSaveFormatFile接收三个参数，第一个参数是要保存文件的文件名，第二个参数是xmlDocPtr，第三个参数是格式。如果第一个参数为“-”，结果将会输出到标准输出上。第三个参数现在没有什么研究，libxml 的 tutorial上的例子将其设为1，我就默认将其设为1了。
当然，还有多种方式可以添加一个新的node。如函数xmlAddChild，这个函数接收两个变量，第一个是父节点，第二个是新节点。这个函数在构造玩节点之后再将其添加，所以我们需要能生成节点的函数。在API文档中搜索后会发现函数xmlNewNode。这个函数接受两个变量，第一个是命名空间，第二个是节点名称。我们再调用函数xmlNodeAddContent或者xmlNodeSetContent就可以对内容进行操作了。
给新添加的节点添加属性，名称为“attr”，值为“yes”：
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7257261#)
- xmlAttrPtr newAttr = NULL;  
- newAttr = xmlNewProp(newNode, "attr", "yes");  
对于修改和删除，API中已经很清楚地提到了这些函数。
如果你要删除node，则调用xmlReplaceNode，用NULL来替换节点。（在API当中没有发现有remove node之类的函数，不知道有没有别的办法？）
如果要修改node的name，则调用xmlNodeSetName函数。
对于属性也一样，函数都在API当中。在tree.h目录中。
