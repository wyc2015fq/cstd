# libxml读取xml文件的其他方法 - nosmatch的专栏 - CSDN博客
2012年05月08日 17:12:09[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：3162
      在前面一篇文章我提到了一种分析文档的方法，即调用xmlParseFile函数。当我看了libxml官网上的code examples之后，我发现libxml还提供了更加精准的分析方式。
1 调用xmlReadFile函数
xmlReadFile函数接收三个参数，第一个参数是文件名，第二个参数是编码类型，第三个参数是分析时的选项。由于提供了更多的参数，所以这个函数能更精确的处理文件。这个函数返回一个xmlDocPtr，得到这个指针后我们可以对各个节点进行操作了。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)
- xmlDocPtr doc = NULL;  
- doc = xmlReadFile(docname, NULL, XML_PARSE_DTDVALID);  
2 使用xmlParseCtxtPtr解析文件
libxml提供了这样一个结构体xmlParserCtxt，这个结构体可以深入控制解析的工程，先看看他是如何工作的。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)
- xmlParserCtxtPtr ctxt = NULL;  
- xmlDocPtr doc = NULL;  
- 
- ctxt = xmlNewParserCtxt();  
- doc = xmlCtxtReadFile(ctxt, docnamen,NULL, 0);  
不要忘了最后释放ctxt。
使用ctxt的一个操作就是分块处理文件，即读进文件的一部分，边读边分析。
**[cpp]**[view
 plain](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[copy](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[print](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)[?](http://blog.csdn.net/linjianfengqrh/article/details/7262756#)
- xmlParserCtxtPtr ctxt  = NULL;  
- xmlDocPtr doc = NULL;  
- staticchar chunk[1024];  
- int num = 0;  
- FILE* fd = NULL;  
- 
- fd = fopen(docname, "rb");  
- if(NULL == fd) {  
-     fprintf(stderr, "open error!\n");  
-     exit(1);  
- }  
- 
- num = fread(chunk, 1, 1024, fd);  
- 
- if(num <= 0) {  
-     fprintf(stderr, "read error!\n");  
-     fclose(fd);  
-     exit(2);  
- }  
- 
- ctxt = xmlCreatePushParserCtxt(NULL, NULL, chunk, num, docname);  
- 
- if(NULL == ctxt) {  
-     fprintf(stderr, "cannot create ctxt\n");  
-     fclose(fd);  
-     exit(3);  
- }  
- 
- while((num = fread(chunk,1,1024,fd)) >0) {  
-     xmlParseChunk(ctxt, chunk, num, 0);  
- }  
- xmlParseChunk(ctxt, chunk,0,1);  
- 
- doc = ctxt->myDoc;  
- num = ctxt->wellFormed;  
- 
- xmlFreeParserCtxt(ctxt);  
- 
- if(0 == num) {  
-     fprintf(stderr, "fail to parse!\n");  
-     fclose(fd);  
-     exit(4);  
- }  
- // handle doc
- 
- xmlFreeDoc(doc);  
- fclose(fd);  
