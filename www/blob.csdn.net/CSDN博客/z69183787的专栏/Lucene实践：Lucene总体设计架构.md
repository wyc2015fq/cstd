# Lucene实践：Lucene总体设计架构 - z69183787的专栏 - CSDN博客
2015年08月23日 16:22:57[OkidoGreen](https://me.csdn.net/z69183787)阅读数：847
Lucene总的来说是：
- 一个高效的，可扩展的，全文检索库。
- 全部用Java实现，无须配置。
- 仅支持纯文本文件的索引(Indexing)和搜索(Search)。
- 不负责由其他格式的文件抽取纯文本文件，或从网络中抓取文件的过程。
在Lucene in action中，Lucene 的构架和过程如下图，
![[图]Lucene的构架和过程](http://hi.csdn.net/attachment/201002/1/3634917_1265049061Uiu0.png)
[](https://p-blog.csdn.net/images/p_blog_csdn_net/forfuture1978/608405/o_image_2_633924612942231250.png)
**说明Lucene 是有索引和搜索的两个过程，包含索引创建，索引，搜索三个要点。**
让我们更细一些看Lucene的各组件：
![[图]Lucene各组件](http://hi.csdn.net/attachment/201002/1/3634917_1265049062fG9p.jpg)
- **被索引的文档用Document对象 表示。**
- **IndexWriter 通过函数addDocument 将文档添加到索引中，实现创建索引的过程。**
- **Lucene 的索引是应用反向索引。**
- **当用户有请求时，Query 代表用户的查询语句。**
- **IndexSearcher 通过函数search 搜索Lucene Index 。**
- **IndexSearcher 计算term weight 和score 并且将结果返回给用户。**
- **返回给用户的文档集合用TopDocsCollector 表示。**
那么如何应用这些组件呢？
让我们再详细到对Lucene API 的调用实现索引和搜索过程。
![[图]Lucene API的调用](http://hi.csdn.net/attachment/201002/1/3634917_12650490643655.jpg)
- **索引过程如下：**- **创建一个IndexWriter 用来写索引文件，它有几个参数，INDEX_DIR 就是索引文件所存放的位置，Analyzer 便是用来对文档进行词法分析和语言处理的。**
- **创建一个Document 代表我们要索引的文档。**
- **将不同的Field 加入到文档中。我们知道，一篇文档有多种信息，如题目，作者，修改时间，内容等。不同类型的信息用不同的Field 来表示，在本例子中，一共有两类信息进行了索引，一个是文件路径，一个是文件内容。其中FileReader 的SRC_FILE 就表示要索引的源文件。**
- **IndexWriter 调用函数addDocument 将索引写到索引文件夹中。**
- **搜索过程如下：**- **IndexReader 将磁盘上的索引信息读入到内存，INDEX_DIR 就是索引文件存放的位置。**
- **创建IndexSearcher 准备进行搜索。**
- **创建Analyer 用来对查询语句进行词法分析和语言处理。**
- **创建QueryParser 用来对查询语句进行语法分析。**
- **QueryParser 调用parser 进行语法分析，形成查询语法树，放到Query 中。**
- **IndexSearcher 调用search 对查询语法树Query 进行搜索，得到结果TopScoreDocCollector 。**
以上便是Lucene API函数的简单调用。
然而当进入Lucene的源代码后，发现Lucene有很多包，关系错综复杂。
然而通过下图，我们不难发现，Lucene的各源码模块，都是对普通索引和搜索过程的一种实现。
此图是上一节介绍的全文检索的流程对应的Lucene实现的包结构。(参照[http://www.lucene.com.cn/about.htm](http://www.lucene.com.cn/about.htm) 中文章《开放源代码的全文检索引擎Lucene》)
![[图]Lucene包结构](http://hi.csdn.net/attachment/201002/1/3634917_12650490657464.jpg)
- **Lucene 的analysis 模块主要负责词法分析及语言处理而形成Term 。**
- **Lucene 的index 模块主要负责索引的创建，里面有IndexWriter 。**
- **Lucene 的store 模块主要负责索引的读写。**
- **Lucene 的QueryParser 主要负责语法分析。**
- **Lucene 的search 模块主要负责对索引的搜索。**
- **Lucene 的similarity 模块主要负责对相关性打分的实现。**
了解了Lucene的整个结构，我们便可以开始Lucene的源码之旅了。
