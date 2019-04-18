# Lucene - 深之JohnChen的专栏 - CSDN博客

2010年08月16日 15:09:00[byxdaz](https://me.csdn.net/byxdaz)阅读数：4225


**Lucene****是什么？**

Lucene是Apache组织的一个用JAVA实现全文搜索引擎的开源项目。后来有人将Lucene移植到。Net语言。

Lucene是一个信息检索的函数库(Library)，利用它你可以为你的应用加上索引和搜索的功能。

Lucene的使用者不需要深入了解有关全文检索的知识，仅仅学会使用库中的一个类，你就为你的应用实现全文检索的功能。

不过千万别以为Lucene是一个象google那样的搜索引擎，Lucene甚至不是一个应用程序，它仅仅是一个工具，一个Library。你也可以把它理解为一个将索引、搜索功能封装的很好的一套简单易用的API。利用这套API你可以做很多有关搜索的事情，而且很方便。

**Lucene****能做什么？**

Lucene可以对任何的数据做索引和搜索。 Lucene不管数据源是什么格式，只要它能被转化为文字的形式，就可以被Lucene所分析利用。也就是说不管是MS word， Html ，pdf还是其他什么形式的文件只要你可以从中抽取出文字形式的内容就可以被Lucene所用。你就可以用Lucene对它们进行索引以及搜索。

**如何使用****Lucene**

**示例介绍**

1.为作为输入参数的文件夹下的所有txt类型的文件做索引，做好的索引文件放入index文件夹。

2.然后在索引的基础上对文件进行全文搜索。

**1. ****建立索引**

**IndexWriter writer = new IndexWriter("index",new StandardAnalyzer(),true);  **

**Document doc = new Document(); **

**doc.Add(new Field(“city”, ”****上海****”,Field.Store.YES, Field.Index.TOKENIZED));**

**doc.Add(new Field(“shopname”,”****东北菜****”,Field.Store.YES,Field.Index.TOKENIZED));**

**writer.AddDocument(doc);**

**doc = new Document(); **

**doc.Add(new Field(“city”, ”****北京****”,Field.Store.YES, Field.Index.TOKENIZED));**

**doc.Add(new Field(“shopname”,”****东北菜菜馆****”,Field.Store.YES,Field.Index.TOKENIZED));**

**writer.AddDocument(doc);**

**writer.SetUseCompoundFile(true);**

**writer.Optimize(); **

**writer.Close();**

**2. ****对索引进行搜索**

**IndexSearcher indexSearcher= ****new**** IndexSearcher(indexDir); **

**QueryParser parser = new QueryParser("shopname", **new StandardAnalyzer()**);**

**Query query = parser.Parse(****“东北”****);**

**Hits hits = indexSearcher.Search(query);**

**Document doc;**

**For (int i = 0 ; i < hits.length();i++)**

**{**

**   doc = hits.Doc(i);**

**   doc.Get(“shopname”).ToString()**

**}**

这样就实现了简单的全文搜索了。

**Lucene****的包结构**

Lucene源码中共包括7个子包，每个包完成特定的功能：

Lucene.Net.Util

一些公用类

**建立索引时最重要的几个概念**

**Document**：一个要进行索引的单元，相当于数据库的一行纪录，任何想要被索引的数据，都必须转化为Document对象存放。

**Field**：Document中的一个字段，相当于数据库中的Column。

**IndexWriter**：负责将Document写入索引文件。通常情况下,IndexWriter的构造函数包括了以下3个参数：索引存放的路径,分析器和是否重新创建索引。特别注意的一点，当IndexWriter执行完addDocument方法后，一定要记得调用自身的close方法来关闭它。只有在调用了close方法后，索引器才会将存放在内在中的所有内容写入磁盘并关闭输出流。

**Analyzer**：分析器，主要用于文本分词。常用的有StandardAnalyzer分析器，StopAnalyzer分析器，WhitespaceAnalyzer分析器等。

**Directory**：索引存放的位置。Lucene提供了两种索引存放的位置，一种是磁盘，一种是内存。一般情况将索引放在磁盘上；相应地Lucene提供了FSDirectory和RAMDirectory两个类。

**Segment**：段，是Lucene索引文件的最基本的一个单位。Lucene说到底就是不断加入新的Segment，然后按一定的规则算法合并不同的Segment以合成新的Segment。

Lucene建立索引的过程就是将待索引的对象转化为Lucene的Document对象，使用IndexWriter将其写入Lucene 自定义格式的索引文件中。待索引的对象可以来自文件、数据库等任意途径，用户自行编码遍历目录读取文件或者查询数据库表取得ResultSet，Lucene的API只负责和字符串打交道。

**Field****对象**

从Lucene的源代码中，可以看出Field 典型构造函数如下：

**Field(String name, String value, Field.Store store, Field.Index index)**

其中：

**Field.Index**有四种属性，分别是：

**Field.Index.TOKENIZED**：分词索引

**Field.Index.UN_TOKENIZED****：**不分词进行索引，如作者名，日期等，不再需要分词。

**Field.Index.NO**：不进行索引，存放不能被搜索的内容如文档的一些附加属性如文档类型, URL等。

**Field.Index.NO_NORMS**：不分词,建索引.但是Field的值不像通常那样被保存，而是只取一个byte，这样节约存储空间。

**Field.Store****也有三个属性，分别是：**

**Field.Store.YES**：索引文件本来只存储索引数据, 此设计将原文内容直接也存储在索引文件中，如文档的标题。

**Field.Store.NO**：原文不存储在索引文件中，搜索结果命中后，再根据其他附加属性如文件的Path，数据库的主键等，重新连接打开原文，适合原文内容较大的情况。

**Field.Store.COMPRESS**压缩存储。

**Lucene的基本原理和代码分析:**

其中总体架构和索引文件格式是Lucene 2.9的，索引过程分析是Lucene 3.0的。

鉴于索引文件格式没有太大变化，因而原文没有更新，原理和架构的文章中引用了前辈的一些图，可能属于早期的Lucene，但不影响对原理和架构的理解。

本系列文章尚在撰写之中，将会有分词器，段合并，QueryParser，查询语句与查询对象，搜索过程，打分公式的推导等章节。

提前给大家分享，希望大家批评指正。 

[Lucene学习总结之一：全文检索的基本原理](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623594.html)

[http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623594.html](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623594.html)

[](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623596.html)

[Lucene学习总结之二：Lucene的总体架构](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623596.html)

[http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623596.html](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623596.html)

[Lucene学习总结之三：Lucene的索引文件格式(1)](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623597.html)

[http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623597.html](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623597.html)

[Lucene学习总结之三：Lucene的索引文件格式(2)](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623599.html)

[http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623599.html](http://www.cnblogs.com/forfuture1978/archive/2009/12/14/1623599.html)

[Lucene学习总结之三：Lucene的索引文件格式(3)](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661436.html)

[http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661436.html](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661436.html)

[Lucene学习总结之四：Lucene索引过程分析(1)](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661439.html)

[http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661439.html](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661439.html)

[Lucene学习总结之四：Lucene索引过程分析(2)](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661440.html)

[http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661440.html](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661440.html)

[Lucene学习总结之四：Lucene索引过程分析(3)](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661441.html)

[http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661441.html](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661441.html)

[Lucene学习总结之四：Lucene索引过程分析(4)](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661442.html)

[http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661442.html](http://www.cnblogs.com/forfuture1978/archive/2010/02/02/1661442.html)

#### 本系列文章已在javaeye制作成电子书，可提供下载，谢谢关注。

#### http://forfuture1978.javaeye.com/blog/pdf

