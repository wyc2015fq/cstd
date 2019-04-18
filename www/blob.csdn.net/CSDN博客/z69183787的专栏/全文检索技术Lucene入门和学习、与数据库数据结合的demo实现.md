# 全文检索技术Lucene入门和学习、与数据库数据结合的demo实现 - z69183787的专栏 - CSDN博客
2015年08月23日 22:22:02[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1202
** 背景**：其实，我前两年就开始接触过Lucene了，当时是准备把做一个内容管理系统CMS，那么搜索这块就需要使用到Lucene技术了，当时，由于项目经费问题，技术方案确定，项目没有立项，最后没能实现..
      前个月，现在的这家公司需要实现有个检索的功能，我当时就想到了Lucene技术, 我就去确认数据量有多大？，检索的范围有多大？精确度等，.才700多条数据，而且还只是检索一个字段...以后数据量会每天的递增，研究Lucene刻不容缓啊..
**首先**，要非常感谢 博客园的 “觉先”先生的分享， 他的博客带我进入了Lucene的大门 :[http://www.cnblogs.com/forfuture1978/category/300665.html](http://www.cnblogs.com/forfuture1978/category/300665.html) ，通过他的博
客对Lucene分析文章，让我对Lucene有个完整的认识，虽然都是简单的认识，但受益匪浅..
       我在这就简单的说下，lucene是什么？
##       Lucene是一个高效的，基于Java的全文检索库。
所以在了解Lucene 之前要费一番工夫了解一下全文检索。
          那么什么叫做全文检索呢？这要从我们生活中的数据说起。
          我们生活中的数据总体分为两种：**结构化数据和非结构化数据**。
**结构化数据：指具有固定格式或有限长度的数据，如数据库，元数据等。**
**       非结构化数据：指不定长或无固定格式的数据，如邮件，word文档等。**
     当然有的地方还会提到第三种，半结构化数据，如XML，HTML 等，当根据需要可按结构化数据来处理，也可抽取出纯文本按非结构化数据来处理。非结构化数据又一种叫
法叫全文数据。
 按照数据的分类，搜索也分为两种：
**对结构化数据的搜索**：如对数据库的搜索，用SQL语句。再如对元数据的搜索，如利用windows 搜索对文件名，类型，修改时间进行搜索等。
**对非结构化数据的搜索**：如利用windows 的搜索也可以搜索文件内容，Linux 下的grep命令，再如用Google 和百度可以搜索大量内容数据。
 对非结构化数据也即对全文数据的搜索主要有两种方法：
        一种是顺序扫描法(Serial Scanning)：**所谓顺序扫描**，比如要找内容包含某一个字符串的文件，就是一个文档一个文档的看，对于每一个文档，从头看到尾，如果此文档包含此字符串，则此文档为我们要找的文件，接着看下一个文件，直到扫描完所有的文件。如利用windows的搜索也可以搜索文件内容，只是相当的慢。如果你有一个80G 硬盘，如果想在上面找到一个内容包含某字符串的文件，不花他几个小时，怕是做不到。Linux 下的grep 命令也是这一种方式。大家可能觉得这种方法比较原始，但对于小数 据量的文件，这种方法还是最直接，最方便的。但是对于大量的文件，这种方法就很慢了。有人可能会说，对非结构化数据顺序扫描很慢，对结构化数据的搜索却相对较快（由于结构化数据有一定的结构可以采取一定的搜索算法加快速度），那么把我们的非结构化数据想办法弄得有一定结构不就行了吗？
         想法很天然，却构成了全文检索的基本思路，也即将非结构化数据中的一部分信息提取出来，重新组织，使其变得有一定结构，然后对此有一定结构的数据进行搜
索，从而达到搜索相对较快的目的。这部分从非结构化数据中提取出的然后重新组织的信息，我们称之索引。这种说法比较抽象，举几个例子就很容易明白，比如字典，字典的
拼音表和部首检字表就相当于字典的索引，对每一个字的解释是非结构化的，如果字典没有音节表和部首检字表，在茫茫辞海中找一个字只能顺序扫描。然而字的某些信息可以
提取出来进行结构化处理，比如读音，就比较结构化，分声母和韵母，分别只有几种可以一一列举，于是将读音拿出来按一定的顺序排列，每一项读音都指向此字的详细解释的
页数。我们搜索时按结构化的拼音搜到读音，然后按其指向的页数，便可找到我们的非结构化数据——也即对字的解释。
 首先建立索引，再对索引进行搜索的过程就叫全文检索(Full-text Search)。
下面这幅图来自《Lucene in action》，但却不仅仅描述了Lucene 的检索过程，而是描述了全文检索的一般过程。
![](https://img-blog.csdn.net/20140920103423375?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb3V5aG9uZzEyMw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
## 全文检索大体分两个过程，索引创建(Indexing)和搜索索引(Search)。       索引创建：将现实世界中所有的结构化和非结构化数据提取信息，创建索引的过程。       搜索索引：就是得到用户的查询请求，搜索创建的索引，然后返回结果的过程
 关于 索引里面是什么结构？请看:[http://www.cnblogs.com/forfuture1978/category/300665.html](http://www.cnblogs.com/forfuture1978/category/300665.html) 介绍
          下面 我们就来实现全文检索demo （**本文代码引用的是lucene 3.0.3jar包**）：
####       DBDataIndexer 工具类： 是我针对数据库数据来生成索引文件，其实如果是先实现demo,直接看下面一个java代码，
**[java]**[view
 plain](http://blog.csdn.net/ouyhong123/article/details/39429715#)[copy](http://blog.csdn.net/ouyhong123/article/details/39429715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/472015/fork)
- import java.io.File;  
- import java.io.IOException;  
- 
- import org.apache.lucene.analysis.Analyzer;  
- import org.apache.lucene.analysis.standard.StandardAnalyzer;  
- import org.apache.lucene.document.Document;  
- import org.apache.lucene.document.Field;  
- import org.apache.lucene.index.IndexWriter;  
- import org.apache.lucene.store.Directory;  
- import org.apache.lucene.store.FSDirectory;  
- import org.apache.lucene.util.Version;  
- 
- import com.zte.its.app.lucene.model.ItsCommProblems;  
- 
- /**
-  * 针对数据库表数据-索引生成 工具类
-  * 
-  * @author 欧阳鸿
-  * 
-  */
- publicclass DBDataIndexer {  
- 
- privatestatic String INDEX_DIR = "D://LuceneTest//index";// 索引存放目录  
- 
- private Directory directory = null;  
- 
- /***
-      * 初始化索引文件目录
-      * 
-      * @return
-      * @throws Exception
-      */
- public Directory initLuceneDirctory() throws Exception {  
- if (directory == null) {  
-             File indexDir = new File(INDEX_DIR);  
- // 文件目录
- // 把索引文件存储到磁盘目录
- // 索引文件可放的位置：索引可以存放在两个地方1.硬盘，2.内存；
- // 放在硬盘上可以用FSDirectory()，放在内存的用RAMDirectory()不过一关机就没了
-             directory = FSDirectory.open(indexDir);  
-         }  
- return directory;  
-     };  
- 
- /***
-      * 初始化 Lucene 创建、增量索引的对象
-      * 
-      * @param cOra
-      *            true:表示创建索引，false表示在原有的索引基础上增量
-      * @return
-      * @throws IOException
-      */
- publicstatic IndexWriter initLuceneObj(Directory directory, boolean cOra) {  
-         Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_CURRENT); // 创建一个语法分析器
-         IndexWriter writer = null;  
- try {  
- 
- // 创建一个IndexWriter(存放索引文件的目录,分析器,Field的最大长度)
-             System.out.println(IndexWriter.MaxFieldLength.UNLIMITED);  
- // 可见构造它需要一个索引文件目录，一个分析器(一般用标准的这个)，一个参数是标识是否清空索引目录
-             writer = new IndexWriter(directory, analyzer, cOra,  
-                     IndexWriter.MaxFieldLength.UNLIMITED);  
- // 索引合并因子
- // 一、SetMergeFactor（合并因子）
- // SetMergeFactor是控制segment合并频率的，其决定了一个索引块中包括多少个文档，当硬盘上的索引块达到多少时，
- // 将它们合并成一个较大的索引块。当MergeFactor值较大时，生成索引的速度较快。MergeFactor的默认值是10，建议在建立索引前将其设置的大一些。
-             writer.setMergeFactor(100);  
- // 二、SetMaxBufferedDocs（最大缓存文档数）
- // SetMaxBufferedDocs是控制写入一个新的segment前内存中保存的document的数目，
- // 设置较大的数目可以加快建索引速度，默认为10。
-             writer.setMaxMergeDocs(1000);  
- // 三、SetMaxMergeDocs（最大合并文档数）
- // SetMaxMergeDocs是控制一个segment中可以保存的最大document数目，值较小有利于追加索引的速度，默认Integer.MAX_VALUE，无需修改。
- // 在创建大量数据的索引时，我们会发现索引过程的瓶颈在于大量的磁盘操作，如果内存足够大的话，
- // 我们应当尽量使用内存，而非硬盘。可以通过SetMaxBufferedDocs来调整，增大Lucene使用内存的次数。
- return writer;  
-         } catch (IOException e) {  
-             e.printStackTrace();  
-         } catch (Exception e) {  
- // TODO Auto-generated catch block
-             e.printStackTrace();  
-         }  
- returnnull;  
-     }  
- 
- /**
-      * 对单个Entity对象（自定义对应数据库数据对象）进行索引
-      * 
-      * @param writer
-      * @param e  自定义javaBean对象，
-      * @throws IOException
-      */
- publicstaticvoid indexEntity(IndexWriter writer, ItsCommProblems e)  
- throws IOException {  
- if (e == null) {  
- return;  
-         }  
-         Document doc = new Document();  
-         doc.add(new Field("requestId", e.getRequestId(), Field.Store.YES,  
-                 Field.Index.NO));  
-         doc.add(new Field("docid", e.getDocid(), Field.Store.YES,  
-                 Field.Index.NO));  
-         doc.add(new Field("articleTitle", e.getArticletitle(), Field.Store.YES,  
-                 Field.Index.ANALYZED));  
-         writer.addDocument(doc);  
-     }  
- 
- /***
-      * 
-      * 获得本次创建/增量索引的原数据数量（条）
-      * 
-      * @param writer
-      * @return
-      */
- publicstaticint getNumDocs(IndexWriter writer) throws IOException {  
- int numIndexed = 0;  
- if (writer != null) {  
-             numIndexed = writer.numDocs();  
-         }  
- return numIndexed;  
-     }  
- 
- /***
-      * 关闭Lucene相关Io对象
-      */
- publicstaticvoid closeIndexWriter(IndexWriter writer, Directory directory) {  
- if (writer != null) {  
- try {  
-                 writer.close(); // 关闭IndexWriter时,才把内存中的数据写到文件
-             } catch (IOException e) {  
-                 e.printStackTrace();  
-             }  
-         }  
-     }  
- 
- }  
###      下面就是检索索引的代码：
**[java]**[view
 plain](http://blog.csdn.net/ouyhong123/article/details/39429715#)[copy](http://blog.csdn.net/ouyhong123/article/details/39429715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/472015/fork)
- import java.io.IOException;  
- import java.util.ArrayList;  
- import java.util.List;  
- 
- import org.apache.lucene.analysis.Analyzer;  
- import org.apache.lucene.analysis.standard.StandardAnalyzer;  
- import org.apache.lucene.document.Document;  
- import org.apache.lucene.index.CorruptIndexException;  
- import org.apache.lucene.index.Term;  
- import org.apache.lucene.queryParser.MultiFieldQueryParser;  
- import org.apache.lucene.queryParser.ParseException;  
- import org.apache.lucene.search.BooleanClause;  
- import org.apache.lucene.search.BooleanQuery;  
- import org.apache.lucene.search.FuzzyQuery;  
- import org.apache.lucene.search.IndexSearcher;  
- import org.apache.lucene.search.MultiSearcher;  
- import org.apache.lucene.search.PrefixQuery;  
- import org.apache.lucene.search.Query;  
- import org.apache.lucene.search.ScoreDoc;  
- import org.apache.lucene.search.TermQuery;  
- import org.apache.lucene.search.TopScoreDocCollector;  
- import org.apache.lucene.search.WildcardQuery;  
- import org.apache.lucene.store.Directory;  
- import org.apache.lucene.util.Version;  
- 
- /**
-  * @author 查询
-  * 
-  */
- publicclass Searcher {  
- 
- // private static String INDEX_DIR = "D://LuceneTest//index"; // 索引所在的路径
- // private static String KEYWORD = "领导";// 关键词
- privatestatic String K_FEILD = "articleTitle"; // 关键词 对应 索引的域
- 
- privatestaticint TOP_NUM = 5;// 显示前5条结果
- 
- /***
-      * 多种匹配词--查询
-      * 
-      * @param keywords
-      * @throws Exception
-      */
- publicstatic List<String> searchIndex(Directory diretory, String keywords)  
- throws Exception {  
-         List<String> requestIdList = new ArrayList<String>();  
- if (keywords != null && !"".equals(keywords)) {  
-             IndexSearcher indexSearcher = null;  
-             MultiSearcher searcher = null;  
- 
- /* 创建一个搜索，搜索刚才创建的目录下的索引 */
- try {  
-                 indexSearcher = new IndexSearcher(diretory, true);  
- // read-only
- /* 在这里我们只需要搜索一个目录 */
-                 IndexSearcher indexSearchers[] = { indexSearcher };  
- /* 我们需要搜索两个域ArticleTitle, ArticleText里面的内容 */
-                 String[] fields = { K_FEILD };  
- /*
-                  * 下面这个表示要同时搜索这两个域，而且只要一个域里面有满足我们搜索的内容就行
-                  * BooleanClause.Occur[]数组,它表示多个条件之间的关系
-                  * ,BooleanClause.Occur.MUST表示and,
-                  * BooleanClause.Occur.MUST_NOT表示not
-                  * ,BooleanClause.Occur.SHOULD表示or. 1、MUST和MUST表示“与”的关系，即“并集”。
-                  * 2、MUST和MUST_NOT前者包含后者不包含。 3、MUST_NOT和MUST_NOT没意义
-                  * 4、SHOULD与MUST表示MUST，SHOULD失去意义；
-                  * 5、SHOUlD与MUST_NOT相当于MUST与MUST_NOT。 6、SHOULD与SHOULD表示“或”的概念
-                  */
-                 BooleanClause.Occur[] clauses = { BooleanClause.Occur.SHOULD };  
- /*
-                  * MultiFieldQueryParser表示多个域解析， 同时可以解析含空格的字符串，如果我们搜索"上海 中国"
-                  */
-                 Analyzer analyzer = new StandardAnalyzer(Version.LUCENE_CURRENT); // 创建一个语法分析器
- // ，Lucene3.0之后
- // 有变化的地方
-                 Query multiFieldQuery = MultiFieldQueryParser.parse(  
-                         Version.LUCENE_CURRENT, keywords, fields, clauses,  
-                         analyzer);  
-                 Query termQuery = new TermQuery(new Term(K_FEILD, keywords));// 词语搜索,完全匹配,搜索具体的域
- 
-                 Query wildqQuery = new WildcardQuery(  
- new Term(K_FEILD, keywords));// 通配符查询
- 
-                 Query prefixQuery = new PrefixQuery(new Term(K_FEILD, keywords));// 字段前缀搜索
- 
-                 Query fuzzyQuery = new FuzzyQuery(new Term(K_FEILD, keywords));// 相似度查询,模糊查询比如OpenOffica，OpenOffice
- 
- /* Multisearcher表示多目录搜索，在这里我们只有一个目录 */
-                 searcher = new MultiSearcher(indexSearchers);  
- // 多条件搜索
-                 BooleanQuery multiQuery = new BooleanQuery();  
- 
-                 multiQuery.add(wildqQuery, BooleanClause.Occur.SHOULD);  
-                 multiQuery.add(multiFieldQuery, BooleanClause.Occur.SHOULD);  
-                 multiQuery.add(termQuery, BooleanClause.Occur.SHOULD);  
-                 multiQuery.add(prefixQuery, BooleanClause.Occur.SHOULD);  
-                 multiQuery.add(fuzzyQuery, BooleanClause.Occur.SHOULD);  
- 
- /* 开始搜索 */
-                 TopScoreDocCollector collector = TopScoreDocCollector.create(  
-                         TOP_NUM, false);// Lucene3.0之后 有变化的地方
- 
-                 searcher.search(multiQuery, collector);  
-                 ScoreDoc[] hits = collector.topDocs().scoreDocs;  
- 
- for (int i = 0; i < hits.length; i++) {  
- 
-                     Document doc = searcher.doc(hits[i].doc);// new method
- // is.doc()
-                     requestIdList.add(doc.getField("requestId").stringValue());  
- // System.out.println("常见问题Id:"
- // + doc.getField("requestId").stringValue() + "   "
- // + hits[i].toString() + "  ");
-                 }  
-             } catch (CorruptIndexException e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             } catch (IOException e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             } catch (ParseException e) {  
- // TODO Auto-generated catch block
-                 e.printStackTrace();  
-             } finally {  
- if (searcher != null) {  
- try {  
- /* 关闭 */
-                         searcher.close();  
-                     } catch (IOException e) {  
-                         e.printStackTrace();  
-                     }  
-                 }  
- if (indexSearcher != null) {  
- try {  
-                         indexSearcher.close();  
-                     } catch (IOException e) {  
-                         e.printStackTrace();  
-                     }  
-                 }  
-             }  
-         }  
- return requestIdList;  
-     }  
- 
- publicstaticvoid main(String[] args) throws Exception {  
- // searchIndex(KEYWORD);// 调用searchIndex方法进行查询
-     }  
- }  
有个Searcher查询类， 我们就可以根据关键字来查询..![大笑](http://static.blog.csdn.net/xheditor/xheditor_emot/default/laugh.gif) 我这有个main方法 是用来调试使用的。大家可以试试
对了，还少了个调用的代码
**[java]**[view
 plain](http://blog.csdn.net/ouyhong123/article/details/39429715#)[copy](http://blog.csdn.net/ouyhong123/article/details/39429715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/472015/fork)
- //
**[java]**[view
 plain](http://blog.csdn.net/ouyhong123/article/details/39429715#)[copy](http://blog.csdn.net/ouyhong123/article/details/39429715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/472015/fork)
- publicvoid initLuceneIndex() throws Exception {  
-     Map map = new HashMap();  
- 
- long start = new Date().getTime();  
- //这里 我是调用了Dao持久层数据方法，使用了MVC的朋友应该知道.
-     List<ItsCommProblems> list = luceneDao.initLuceneIndex();  
- 
- // 得到初始化索引文件目录
-     Directory directory = new DBDataIndexer().initLuceneDirctory();  
- if (directory != null) {  
- 
-         IndexWriter writer = DBDataIndexer.initLuceneObj(directory, true);  
- if (list != null && list.size() > 0) {  
- for (ItsCommProblems object : list) {  
-                 DBDataIndexer.indexEntity(writer, object);  
-             }  
-         }  
-         DBDataIndexer.closeIndexWriter(writer, directory);  
-     } else { // 索引存放路径不存在
- thrownew IOException(directory  
-                 + " does not exist or is not a directory");  
-     }  
- long end = new Date().getTime();  
- 
-     System.out.println(" InitIndexed: took " + (end - start)  
-             + " milliseconds");  
- }  
**[java]**[view
 plain](http://blog.csdn.net/ouyhong123/article/details/39429715#)[copy](http://blog.csdn.net/ouyhong123/article/details/39429715#)![在CODE上查看代码片](https://code.csdn.net/assets/CODE_ico.png)[](https://code.csdn.net/snippets/472015/fork)
- //根据关键字查询对应的记录： 思路是，同关键字去索引查询到对应的记录Id,然而再通过id查询记录
- ublic List<ItsCommProblems> rearcherBykeyWord(String keywords)  
- throws Exception {  
- Map map = new HashMap();  
- 
- long start = new Date().getTime();  
- List<ItsCommProblems> commProblemList = null;  
- 
- // 得到初始化索引文件目录
- Directory directory = new DBDataIndexer().initLuceneDirctory();  
- List<String> requestIdList = Searcher.searchIndex(directory, keywords);  
- if (requestIdList != null && requestIdList.size() > 0) {  
-     map.put("requestIdList", requestIdList);  
-     commProblemList = luceneDao.rearcherBykeyWord(map);  
- }  
- 
- long end = new Date().getTime();  
- System.out.println(" rearcherBykeyWord 《 " + keywords + "》  共花费："
-         + (end - start) + " milliseconds");  
- if (commProblemList != null && commProblemList.size() > 0) {  
- for (ItsCommProblems itsCommProblems : commProblemList) {  
-         System.out.println(" 查询获得:  "
-                 + itsCommProblems.getArticletitle());  
-     }  
- }  
- return commProblemList;  
     其实，我这只是完成了简单的检索，很多策略问题，中英文等。都还没来的及实现...Lucene这个技术是很深的.需要花大量时间和精力研究和学习的..
版权声明：本文为博主原创文章，未经博主允许不得转载。

