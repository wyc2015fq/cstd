# libxml2主要函数说明 - nosmatch的专栏 - CSDN博客
2012年04月24日 10:03:05[nosmatch](https://me.csdn.net/HDUTigerkin)阅读数：3554
许多事物符合80/20法则，libxml中也是20%的函数提供了80%的功能。下面的列表列出了libxml的主要函数及其用法说明。
#### 1.   全局函数说明
|头文件引用|xml2config --cflags|
|----|----|
|库文件引用|xml2config --libs|
|主要头文件|parse.htree.hxpath.h|
|简单例程|Makefile:INCLUDE=-I./ -I$HOME/include `xml2config --cflags`LIBRARY=-L./ -L$HOME/lib `xml2config --libs`*.c或*.cpp#include <libxml/parse.h>#include <libxml/tree.h>#include <libxml/xpath.h>|
|函数功能|在分析XML数据是，去除空白字符。如果不去除空白字符，则这些字符也会被当做一个node来处理|
|----|----|
|函数接口|int  xmlKeepBlanksDefault(int val)|
|参数说明|val：0或者1。0表示去除空白字符，1表示不去除返回值：0表示设置失败，1表示设置成功，一般不用判断|
|简单例程|xmlKeepBlanksDefault(0);|
#### 2.   XML文件载入和保存函数
|函数功能|将XML文件从硬盘上载入到内存中，并且生成DOM树。使用完毕之后，需要用xmlFreeDoc()来释放资源|
|----|----|
|函数接口|xmlDocPtr xmlParseFile(const char * filename)|
|参数说明|filename：XML文件名称。返回值：如果载入成功，则返回这个文档的根节点。否则返回NULL|
|简单例程|xmlDodPtr pdoc;pdoc = xmlParseFile("123.xml");if( pdoc == NULL ){     printf("Fail to parse XML file.\n");}|
|函数功能|将内存中的DOM树，保存到硬盘上，生成一个带格式的XML文件|
|----|----|
|函数接口|int  xmlSaveFormatFileEnc(const char * filename,  xmlDocPtr cur,     const char * encoding, int format)|
|参数说明|filename：需要保存的文件的名称cur：需要保存的XML文档encoding：导出文件的编码类型，或者为NULLformat：是否格式化。0表示不格式化，1表示需要格式化。注意：只有当xmlIndentTreeOutput设置为1，或者xmlKeepBlanksDefault(0)时，format设置为1才能生效返回值：写入文件中的字节数量|
|简单例程|xmlDodPtr pdoc;pdoc = xmlParseFile("123.xml");if( pdoc == NULL ){     printf("Fail to parse XML file.\n");}Do_something_with_pdoc();int filesize;filesize = xmlSaveFormatFileEnc("321.xml", pdoc, "gb2312",1);if(filesize == -1){     printf("Fail to save XML to file.\n");}|
#### 3.   XML内存载入和输出函数
|函数功能|将一块内存中的XML数据生成一个DOM树。使用完毕之后，需要用xmlFreeDoc()来释放资源|
|----|----|
|函数接口|xmlDocPtr xmlParseMemory(const char * buffer, int size)|
|参数说明|buffer：存放XML格式数据的内存区size：内存中XML格式数据的长度返回值：如果载入成功，则返回这个文档的根节点；否则返回NULL|
|简单例程|char *buffer;int size;xmlDocPtr pdoc;// read_data_to_bufferbuffer = ……size = strlen(buffer);pdoc = xmlParseMemory(buffer, size);if( pdoc == NULL){     printf("Fail to parse XML buffer.\n");}|
|函数功能|将DOM树导出到内存中，形成一个XML格式的数据|
|----|----|
|函数接口|void xmlDocDumpFormatMemoryEnc(xmlDocPtr out_doc, xmlChar ** doc_txt_ptr,     int * doc_txt_len, const char * txt_encoding, int format)|
|参数说明|out_doc：需要输出成为一个buffer的XML文档doc_txt_ptr：输出文档的内存区。由该函数在内部申请。使用完成之后，必须调用xmlFree()函数来释放该内存块doc_txt_len：输出文档内存区的长度txt_encoding：输出文档的编码类型format：是否格式化。0表示不格式化，1表示需要格式化。注意只有当xmlIndentTreeOutput设置为1，或者xmlKeepBlanksDefault(0)时，format设置为1才能生效|
|简单例程|xmlChar *outbuf;int outlen;xmlDocPtr pdoc;pdoc = ……xmlDocDumpFormatMemoryEnc(pdoc, &outbuf, &outlen, "gb2312", 1);xmlFree(outbuf);|
#### 4.   创建和释放XML文档函数
|函数功能|在内存中创建一个新的XML文档。所创建的文档需要使用xmlFreeDoc()来释放资源|
|----|----|
|函数接口|xmlDocPtr xmlNewDoc     (const xmlChar * version)|
|参数说明|version：XML标准的版本，目前只能指定为“1.0”|
|简单例程|xmlDocPtr pdoc ;pdoc = xmlNewDoc((const xmlChar*)"1.0");if( pdoc == NULL ){     printf(“Fail to create new XML doc.\n”);}|
|函数功能|释放内存中的XML文档|
|----|----|
|函数接口|void xmlFreeDoc(xmlDocPtr cur)|
|参数说明|cur：需要释放的XML文档|
|简单例程|xmlDocPtr pdoc ;pdoc = xmlNewDoc((const xmlChar*)"1.0");if( pdoc == NULL ){     printf(“Fail to create new XML doc.\n”);}xmlFreeDoc(podc);|
#### 5.   XML节点操作函数
|函数功能|获得根节点|
|----|----|
|函数接口|xmlNodePtr    xmlDocGetRootElement(xmlDocPtr doc)|
|参数说明|doc：XML文档句柄。返回值：XML文档的根节点，或者NULL。|
|使用流程|① 解析好文档的根节点指针，应用使用该指针可以遍历XML文件② xmlNodePtr的next字段，指向下一个同级XML节点③ properties字段为xmlAttr类型，指向该XML节点的属性④ children字段为xmlNodePtr类型，指向该XML节点的子节点|
|简单例程|xmlDocPtr pdoc ;xmlNodePtr root ;pdoc = xmlParseFile("123.xml");if( pdoc == NULL ){     printf("Fail to parse XML File.\n");     return ;}root = xmlDocGetRootElement(pdoc);if( root == NULL){     printf("Fail to get root element\n");     return;}|
|函数功能|设置根节点|
|----|----|
|函数接口|xmlNodePtr    xmlDocSetRootElement(xmlDocPtr doc, xmlNodePtr root)|
|参数说明|doc：XML文档句柄root：XML文档的新的根节点返回值：如果该文档原来有根节点，则返回根节点，否则返回NULL|
|简单例程|xmlDocPtr pdoc ;xmlNodePtr root;pdoc = xmlNewDoc((const xmlChar*)"1.0");if( pdoc == NULL ){     printf("Fail to create new XML doc.\n");     return;}root = xmlNewDocNode(pdoc, NULL, (const xmlChar*)"root", NULL);if( root == NULL ){     printf("Fail to create doc node.\n");     return ;}xmlDocSetRootElement(pdoc, root);|
|函数功能|获得节点的内容|
|----|----|
|函数接口|xmlChar * xmlNodeGetContent  (xmlNodePtr cur)|
|参数说明|cur：节点的指针返回值：节点的文本内容。如果该节点没有文本内容，则返回NULL。当返回值不为NULL时，需要用xmlFree()函数来释放返回的资源|
|简单例程|xmlNodePtr node;xmlChar* content;node = ….content = xmlNodeGetContent(node);xmlFree(content);|
|函数功能|设置节点的内容长度|
|----|----|
|函数接口|void xmlNodeSetContentLen(xmlNodePtr cur, const xmlChar * content,  int len)|
|参数说明|cur：节点的指针content：节点的新文本内容len：节点新文本内容的长度|
|简单例程|xmlNodePtr node;xmlChar* content;int len;content = (xmlChar*)"1234567890";len = strlen((char*)content);xmlNodeSetContentLen(node, content, len);|
|函数功能|在节点的内容后面添加新的内容|
|----|----|
|函数接口|void xmlNodeAddContentLen(xmlNodePtr cur,  const xmlChar * content,  int len)|
|参数说明|cur：节点的指针content：节点的新加的文本内容len：节点新加的文本内容的长度|
|简单例程|xmlNodePtr node;xmlChar* content;int len;content = (xmlChar*)"1234567890";len = strlen((char*)content);xmlNodeAddContentLen(node, content, len);|
|函数功能|获得节点的属性|
|----|----|
|函数接口|xmlChar * xmlGetProp(xmlNodePtr node, const xmlChar * name)|
|参数说明|node：XML节点的指针name：该节点的属性的名称返回值：该属性的值或者为NULL。如果不为NULL，则需要用xmlFree()来释放资源|
|简单例程|xmlNodePtr node;xmlChar* prop;node = ….prop = xmlGetProp(node, (const xmlChar*)"name");if( prop != NULL)     xmlFree(prop);|
|函数功能|设置节点的属性（如果该属性已经存在，则替换其值）|
|----|----|
|函数接口|xmlAttrPtr xmlSetProp(xmlNodePtr node, const xmlChar * name, const xmlChar * value)|
|参数说明|node：需要设置属性的节点name：属性的名称value：属性的值返回值：该属性节点的指针|
|简单例程|xmlNodePtr node;xmlAttrPtr attr;node = ….attr = xmlSetProp(node, (cosnt xmlChar*)"Dept-Name", (const xmlChar*)"ES");if( attr == NULL){     printf("Fail to set prop of this node.\n");}|
#### 6.   XPath函数
|函数功能|生成xpath的上下文关系句柄|
|----|----|
|函数接口|xmlXPathContextPtr xmlXPathNewContext(xmlDocPtr doc)|
|参数说明|doc：需要处理的XML文档返回值：该文档的XPath上下文关系句柄或者NULL。该返回句柄由函数内部申请，此函数调用者需要用xmlXPathFreeContext来释放|
|简单例程|xmlDocPtr pdoc;xmlXPathContextPtr xpathctx;pdoc = ….xpathctx = xmlXPathNewContext(pdoc);if( xpathctx != NULL)     xmlXPathFreeContext(xpathctx);|
|函数功能|释放xpath的上下文关系句柄|
|----|----|
|函数接口|void xmlXPathFreeContext(xmlXPathContextPtr ctxt)|
|参数说明|ctxt：需要释放的xpath上下文关系句柄|
|简单例程|参见xmlXPathNewContext()的例程|
|函数功能|执行xpath的表达式，返回结果内容节点集合XPath表达式的表示方法，请参考：http://www.zvon.org/xxl/XPathTutorial/General/examples.html|
|----|----|
|函数接口|xmlXPathObjectPtr  xmlXPathEvalExpression (const xmlChar * str,                             xmlXPathContextPtr ctxt)|
|参数说明|str：xpath表达式ctxt：xpath的上下文关系句柄返回值：满足表达式的结果集合或者为NULL。该返回句柄由函数内部申请，此函数调用者需要用xmlXPathFreeObject()来释放|
|简单例程|xmlDocPtr pdoc;xmlXPathContextPtr xpathctx;xmlXPathObjectPtr xpathobj;pdoc = ….xpathctx = xmlXPathNewContext(pdoc);if( xpathctx == NULL){     printf("Fail to create xpath context.\n");     return ;}Xpathobj = xmlXPathEvalExpression(BAD_CAST "//@value", xpathctx);if( xpathobj == NULL){     printf("Fail to evaluate xpath expression.\n");     xmlXPathFreeContext(xpathctx);     return;}xmlXPathFreeObject(xpathobj);xmlXPathFreeContext(xpathctx);结果集说明：xpathobj-> nodesetval：存储结果列表，如果为NULL，表示无结果xpathObj->nodesetval->nodeNr：表示结果的数量xpathObj->nodesetval->nodeTab：表示结果的数组，可以通过下标访问例如：int number;xmlNodePtr node;if( xpathobj-> nodesetval == NULL)     number = 0;else     number = xpathObj->nodesetval->nodeNr;for(int i=0;i<number;i++){     node = xpathObj->nodesetval->nodeTab[i];     do_some_work_with_node();}|
|函数功能|释放xpath表达式运算结果集|
|----|----|
|函数接口|void xmlXPathFreeObject(xmlXPathObjectPtr obj)|
|参数说明|obj：需要释放的xpath表达式运算结果集合|
|简单例程|参见xmlXPathEvalExpression()的例程|
#### 7.   XML常见使用函数列表
下文是对XML常见使用函数的简要说明，有些函数的具体说明见上面的表格。
<libxml/parser.h>
 int xmlKeepBlanksDefault (int val) //设置是否忽略空白节点,比如空格,在分析前必须调用,默认值是0,最好设置成1。
xmlDocPtr xmlParseFile (const char * filename) //分析一个xml文件,并返回一个文档对象指针
<libxml/tree.h>
//xml操作的基础结构提及其指针类型
//xmlDoc  xmlDocPtr 文档对象的结构体及其指针
//xmlNode xmlNodePtr 节点对象的结构体及其指针
//xmlAttr xmlAttrPtr 节点属性的结构体及其指针
//xmlNs xmlNsPtr 节点命名空间的结构及其指针
//根节点相关函数
xmlNodePtr xmlDocGetRootElement (xmlDocPtr doc) //获取文档根节点
xmlNodePtr xmlDocSetRootElement (xmlDocPtr doc, xmlNodePtr root) //设置文档根节点
//创建子节点相关函数
xmlNodePtr xmlNewNode (xmlNsPtr ns, const xmlChar * name) //创建新节点
xmlNodePtr xmlNewChild (xmlNodePtr parent, xmlNsPtr ns, const xmlChar * name, const xmlChar * content) //创建新的子节点
xmlNodePtr xmlCopyNode (const xmlNodePtr node, int extended) //复制当前节点
//添加子节点相关函数
xmlNodePtr xmlAddChild (xmlNodePtr parent, xmlNodePtr cur) //给指定节点添加子节点
xmlNodePtr xmlAddNextSibling (xmlNodePtr cur, xmlNodePtr elem) //添加后一个兄弟节点
xmlNodePtr xmlAddPrevSibling (xmlNodePtr cur, xmlNodePtr elem) //添加前一个兄弟节点
xmlNodePtr xmlAddSibling (xmlNodePtr cur, xmlNodePtr elem) //添加兄弟节点
//属性相关函数
xmlAttrPtr xmlNewProp (xmlNodePtr node, const xmlChar * name, const xmlChar * value) //创建新节点属性
xmlChar * xmlGetProp (xmlNodePtr node, const xmlChar * name) //读取节点属性
xmlAttrPtr xmlSetProp (xmlNodePtr node, const xmlChar * name, const xmlChar * value) //设置节点属性
//作用同尾部同名的字符串函数。只不过针对相应的xml节点
xmlChar* xmlStrcat (xmlChar *cur, const xmlChar * add)
const xmlChar *xmlStrchr(const xmlChar * str, xmlChar val)
int xmlStrcmp (const xmlChar * str1, const xmlChar * str2)
int xmlStrlen (const xmlChar * str)
xmlChar *xmlStrncat (xmlChar * cur, const xmlChar * add, int len)
int xmlStrncmp (const xmlChar * str1, const xmlChar * str2, int len)
const xmlChar *xmlStrstr (const xmlChar * str, const xmlChar * val
