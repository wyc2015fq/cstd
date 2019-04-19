# Row-Oriented Database 、Column-Oriented Database 、Key-Value Store Database 、Document-Oriented Databas - 三少GG - CSDN博客
2014年05月26日 08:19:45[三少GG](https://me.csdn.net/scut1135)阅读数：2312

# [Row-Oriented Database 、Column-Oriented Database 、Key-Value Store Database 、Document-Oriented Database](http://www.yeeach.com/post/600)
[chuanliang](http://www.yeeach.com/post/author/chuanliang) 于 2009-10-4,20:50 [Comments
 (0)](http://www.yeeach.com/post/600#respond)
归类于：[技术相关](http://www.yeeach.com/post/category/%E6%8A%80%E6%9C%AF-%E8%BD%AF%E4%BB%B6)  Tags: [Column-Oriented
 Database](http://www.yeeach.com/post/tag/column-oriented-database), [Document-Oriented Database](http://www.yeeach.com/post/tag/document-oriented-database), [key-value
 store，Row-Oriented Database](http://www.yeeach.com/post/tag/key-value-store%EF%BC%8Crow-oriented-database), [列数据库](http://www.yeeach.com/post/tag/%E5%88%97%E6%95%B0%E6%8D%AE%E5%BA%93), [文档数据库](http://www.yeeach.com/post/tag/%E6%96%87%E6%A1%A3%E6%95%B0%E6%8D%AE%E5%BA%93), [行数据库](http://www.yeeach.com/post/tag/%E8%A1%8C%E6%95%B0%E6%8D%AE%E5%BA%93).
    在研究Key-Value Database数据库时候，不可避免地遇见了几种类型的数据库划分方式：Row-Oriented Database 、Column-Oriented Database 、Key-Value Database 、Document-Oriented Database，简单总结一下。
### **1、Row-Oriented Database：**
    行导向的数据库实际上就是传统的关系数据库（RDBMS），关系数据库的数据是按行来存储的，即把一条记录的所有属性（列）存储在一起。传统关系数据库的典型应用场合是OLTP（On-Line Transaction Processing ）领域，这些领域以事务处理为主，包括数据的增加、删除、修改、查询。
![key-value store，Row-Oriented Database ,Column-Oriented Database,Document-Oriented Database,行数据库,列数据库,文档数据库](http://www.yeeach.com/wp-content/uploads/2009/10/thumb.png)
### **2、Column-Oriented Database：**
    列导向的数据库的典型应用场合是OLAP（On-Line Analysis Processing），因此以列导向的架构设计非常适合于数据仓库的应用。对于大多数数据仓库的分析场景中，实际关注的数据很多时候都只有那么一列或者少数几列的数据。所以在以列为导向的设计中，大部分的分析查询都只需要读取某一个（或者几个）表的几列，而不需要像传统以行为导向的数据库那样需要扫描整个表的数据，这两者IO量的差距是非常大的。
    要理解列导向的数据库可以参考下图（摘自[What’s a Column-Oriented DBMS?](http://marklogic.blogspot.com/2007/03/whats-column-oriented-dbms.html)）。
![key-value store，Row-Oriented Database ,Column-Oriented Database,Document-Oriented Database,行数据库,列数据库,文档数据库](http://www.yeeach.com/wp-content/uploads/2009/10/example-thumb.jpg)
    参考[文章](http://internetmindmap.com/database_software)及Wikipedia的划分方法，常见的Column-Oriented Database如下（不一定准确，仅供参考）
- 
[http://www.luciddb.org/](http://www.luciddb.org/)
- 
[http://www.infobright.org/](http://www.infobright.org/)
- 
[http://monetdb.cwi.nl/](http://monetdb.cwi.nl/)
- 
[http://www.equi4.com/metakit/](http://www.equi4.com/metakit/)
- 
[http://incubator.apache.org/cassandra/](http://incubator.apache.org/cassandra/) （也可以归成Key-Value Store）
- 
[http://hadoop.apache.org/hbase/](http://hadoop.apache.org/hbase/) （也可以归成Key-Value Store）
- 
[http://hypertable.org/](http://hypertable.org/) （也可以归成Key-Value Store）
- 
[http://kosmosfs.sourceforge.net/](http://kosmosfs.sourceforge.net/) （也可以归成Key-Value Store）
- 
[http://www.openneptune.com/](http://www.openneptune.com/) （也可以归成Key-Value Store）
- 
[http://www.vermontdatabase.com/rc21home.htm](http://www.vermontdatabase.com/rc21home.htm)
- 
[http://www.jhterbekke.net/XplainDBMS.html](http://www.jhterbekke.net/XplainDBMS.html)
- 
[http://db.lcs.mit.edu/projects/cstore/](http://db.lcs.mit.edu/projects/cstore/)
- 
[http://www.ingres.com/vectorwise/](http://www.ingres.com/vectorwise/)
- 
[http://cloudbase.sourceforge.net](http://cloudbase.sourceforge.net/)
- 
[https://codeforge.lbl.gov/projects/fastbit/](https://codeforge.lbl.gov/projects/fastbit/)
### **3、Column-Oriented Database VS. Row-Oriented Database**
![key-value store，Row-Oriented Database ,Column-Oriented Database,Document-Oriented Database,行数据库,列数据库,文档数据库](http://www.yeeach.com/wp-content/uploads/2009/10/columsbasedvsrowbased-thumb.png)
![OLTP, OLAP,key-value store，Row-Oriented Database ,Column-Oriented Database,Document-Oriented Database,行数据库,列数据库,文档数据库](http://www.yeeach.com/wp-content/uploads/2009/10/oltpvsolap-thumb.png)
### **4、Key-Value Store Database**
    在[distributed key-value store 杂思](http://www.yeeach.com/2009/09/27/distributed-key-value-store-%E6%9D%82%E6%80%9D/)对一些常见Key-Value Store数据库进行了总结，其实精确来说，不应当都叫Key-Value
 Store Database，而应当叫做[Anti-RDBMS](http://www.metabrew.com/article/anti-rdbms-a-list-of-distributed-key-value-stores/)或NOSQL Database。
### **5、Document-Oriented Database**
    文档数据库一般用于存储半结构化的数据，在文档数据库中文档是处理信息的基本单位。文档数据库允许创建许多不同类型的非结构化的或任意格式的字段。文档数据库一般采用XML、YAML 、JSON作为存储协议。由于文档数据库中的文档也需要一个唯一的key来标识文档（当然key可以是JSON、XML结构），文档的内容可以看做value，因此一般把文档数据库也归到Key-Value Store Database中，当然所有的XML数据库其实也可以归成文档数据库。
    文档数据库很适合CMS、知识库、OA系统，用于存储新闻稿件、电子邮件、书籍、Web页面、多媒体文件等半结构化的数据。   
    除了Lotus Notes 、Amazon SimpleDB 这样的商用文档数据库外，常见的开源文档数据库：
- 
[http://couchdb.apache.org/](http://couchdb.apache.org/)
- 
[http://www.mongodb.org/](http://www.mongodb.org/)
- 
[http://jackrabbit.apache.org/](http://jackrabbit.apache.org/)
- <p
>[http://riak.basho.com/](http://riak.basho.com/)
- 
[http://rddb.rubyforge.org/](http://rddb.rubyforge.org/)
- 
[http://github.com/yrashk/strokedb](http://github.com/yrashk/strokedb)
- 
[http://code.google.com/p/featherdb/](http://code.google.com/p/featherdb/)
- 
[http://code.google.com/p/thrudb/](http://code.google.com/p/thrudb/)
### **6、参考文档**
[http://ronaldbradford.com/data-store-products/](http://ronaldbradford.com/data-store-products/)
[http://internetmindmap.com/database_software](http://internetmindmap.com/database_software)
[http://en.wikipedia.org/wiki/Column-oriented_DBMS](http://en.wikipedia.org/wiki/Column-oriented_DBMS)
[http://en.wikipedia.org/wiki/Document-oriented_database](http://en.wikipedia.org/wiki/Document-oriented_database)
Technorati 标签: [key-value store](http://technorati.com/tags/key-value+store),[Row-Oriented
 Database](http://technorati.com/tags/Row-Oriented+Database),[Column-Oriented Database](http://technorati.com/tags/Column-Oriented+Database),[Document-Oriented
 Database](http://technorati.com/tags/Document-Oriented+Database),[行数据库](http://technorati.com/tags/%E8%A1%8C%E6%95%B0%E6%8D%AE%E5%BA%93),[列数据库](http://technorati.com/tags/%E5%88%97%E6%95%B0%E6%8D%AE%E5%BA%93),[文档数据库](http://technorati.com/tags/%E6%96%87%E6%A1%A3%E6%95%B0%E6%8D%AE%E5%BA%93)
