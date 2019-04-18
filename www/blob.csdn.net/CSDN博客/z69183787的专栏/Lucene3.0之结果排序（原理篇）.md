# Lucene3.0之结果排序（原理篇） - z69183787的专栏 - CSDN博客
2015年08月23日 16:29:11[OkidoGreen](https://me.csdn.net/z69183787)阅读数：949
Lucene3.0之结果排序（原理篇）
传统上，人们将信息检索系统返回结果的排序称为"相关排序"（relevance
 ranking），隐含其中各条目的顺序反映结果和查询的相关程度。
1、基本排序原理
①向量空间模型
Gerald Salton 等在 30 多年前提出的"向量空间模型"（Vector
 Space Model，VSM）[Salton and Lesk,1968, Salton,1971]。该模型的基础是如下假设：文档d和查询q的相关性可以由它们包含的共有词汇情况来刻画。
经典的TF*IDF词项权重的计算公式：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436388701.gif)
给定某种权重的定量设计，求文档和查询的相关性就变成了求 d 和 q 向量的
某种距离，最常用的是余弦（cos）距离
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436381766.gif)
②链接分析PageRank原理
链接分析技术主要基于两个假设：1）一个网页被多次引用，则它可能是很重要的，如果被重要的网页引用，说明自身也是重要的，网页的重要性在网页之间可以传递。
2）随机冲浪模型：认为假定用户一开始随机地访问网页集合中的一个网页，然和跟随网页的链接向前浏览网页，不会退浏览，那么浏览下一个网页的概率是被浏览网页的量化的重要程度值。
按照以上的用户行为模型，每个网页可能被访问到的次数越多就越重要，这样的"可能被访问的次数"也就定义为网页的权值，PageRank值。如何计算这个权值呢？PageRank采用以下公式进行计算：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/20111019143639686.gif)
其中wj代表第j个网页的权值；lij只取0、1值，代表从网页i到网页j是否存在链接；ni代表网页i有多少个连向其它网页的链接；d代表"随机冲浪"中沿着链接访问网页的平均次数。选择合适的初始数值，递归的使用上述公式，即可得到理想的网页权值。
2、Lucene排序计算公式
Lucene的排序公式如下：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436396259.gif)
1)![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436391276.gif)，协调因子，表示文档（d）中Term(t)出现的百分比，也就是计算查询条件（q）中不同Term(t)，以及在文档中出现的数量之和，两者的数量之比。通常在文档中出现查询Term种类越多，分值越高。
2)![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436398244.gif)，调节因子，不影响索引排序情况，只在检索时使用，主要是用来让排序结果在不同的查询条件之间可以比较。这个条件是在搜索时候计算。数值是根据每一个查询项权重的平方和计算得到。计算公式如下：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436393260.gif)
3) ![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436398277.gif)，文档频率，表示查询词中，每个Term在对应的结果文档中(d)中出现的次数。查询词出现的次数越多，表示出现频率越高，文档的检索得分就越高。为了避免获得更大的相关性函数，实际中，使用次数的平方跟作为文档频率tf的值，避免数值过度放大。
4) ![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436407438.gif)，逆文档频率，检索匹配文档数量的反向函数。按照信息理论，文档出现的次数越少，每一篇文档的信息量就会越大。所以匹配的文档数越少，得分就越高。而索引库中文档总数越多，找到一篇目标文档难度越大，相应的信息量也会比较大。
5) ![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436409946.gif)，长度因子，每个索引词汇在域中的总体长度决定的，这个参数在索引建立时确定。数值根据文档中实际具有的索引项个数确定。检索词长度在文档总长度中占的比例越大，长度因子的数值也越大。
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/20111019143640503.gif)
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436401931.gif)
Lucene3.0之结果排序（操作篇）
1、Lucene相关排序流程
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436405868.png)
2、Lucene相关类
①Query类：一个抽象类，Lucene检索结果最终评分的总控制中心。其它评分有关的类和对象都是由Query类来管理和生产。
②Weight类接口：定义Query权重计算的一个实现接口，可以被重用。Weight类可以用来生成Scorer类，也可以解析评分的详细信息，另外还定义了获取Query权值的方法。
③Scorer类：Lucene评分机制的核心类。类的定义是抽象类，提供的一些抽象基本的计分功能方法提供所有的评分类实现，同时还定义了评分的详细解析方法，Scorer类内部有一个Similarity对象，用来指明计算公式。
④Scorer类：Lucene相似度计算的核心抽象类。Similarity类主要处理评分计算，系统缺省使用类DefaultSimilarity类对象
3、排序控制
使用Sort对象定制排序，通过改变文档Boost值来改变排序结果以及使用自定义的Similarity方法更改排序
4、文档Boost加权排序
①Boost是指索引建立过程中，给整篇文档或者文档的某一特定域设定的权值因子，在检索时，优先返回分数高的。
Document和Field两重Boosting参数。通过Document对象的setBoost()方法和Field对象的setBoost()方法。不同在于前者对文档中每一个域都修改了参数，而后者只针对指定域进行修改。
文档加权=Document-boosting*Field-boosting，默认情况下为1，一般不做修改。
②Sort对象检索排序
Sort使用时通过实例化对象作为参数，通过Searcher类的search接口来实现。Sort支持的排序功能以文档当中的域为单位，通过这种方法，可以实现一个或者多个不同域的多形式的值排序。
实际使用排序对象Sort进行排序。主要有两种模式，一种是以字符串表示文档域的名称作为参数指定域排序，一种是直接以排序域的包装域的包装类作为参数进行排序。
Sort对象使用比较简单，只需要在对文档索引进行检索时，在检索器的Search方法中带Sort对象作为参数即可。
1)Sort对象相关性排序
按照相关性排序时最基本的结果排序方法，使用Sort对象无参数构造函数完成的排序效果相当于Lucene默认的按相关性降序排序。
2)Sort对象文档编号排序
某些应用场合需要对所有符合匹配度的结果，按照文档内部编号排序输出。使用Sort对象的静态实例Sort.INDEXORDER来实现
3)Sort对象独立域排序
在检索过程中，把检索结果按照某一个特定域排序，非常重要。在使用搜索引擎过程中，有时会选择使用时间排序，而在搜索引擎库中，检索词完全是另外一个域的内容，与时间没有任何关系。这种应用中，检索关键词的匹配仍然是首要因素，匹配太低或者不匹配的文档直接不必处理，而匹配的文档则需进一步排序输出。
指定的排序域并没有进行特别限制，可以是检索词的关联域，也可以是文档中的任意其它域。
4)Sort对象联合域排序
多个文档域联合排序时，需要注意文档域的添加次序。排序的结果先按照第一个域排序，然后第二个域作为次要关键字排序。开发时，需要根据自己的需要选择合适的次序。
5)Sort对象逆向排序
Sort(field,true)或者Sort(field,false)实现升降序排序。
Lucene3.0之结果排序（示例篇）
这个例子是根据《开发自己的搜索引擎：Lucene2.0+Heritrix》中的例子改的，由于原书中是使用Lucene2.0，所以代码有部分改动。
package sortApp;
import java.io.File;
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.Term;
import org.apache.lucene.index.IndexWriter.MaxFieldLength;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.Sort;
import org.apache.lucene.search.TermQuery;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.FSDirectory;
import org.apache.lucene.util.Version;
public class SortTest {
public static void makeItem(IndexWriter writer, String bookNumber,
    String bookName, String publishDate) throws Exception {
   writer.setUseCompoundFile(false);
   Document doc = new Document();
   Field f1 = new Field("bookNumber", bookNumber, Field.Store.YES,
     Field.Index.NOT_ANALYZED);
   Field f2 = new Field("bookName", bookName, Field.Store.YES,
     Field.Index.ANALYZED);
   Field f3 = new Field("publishDate", publishDate, Field.Store.YES,
     Field.Index.NOT_ANALYZED);
   doc.add(f1);
   doc.add(f2);
   doc.add(f3);
   writer.addDocument(doc);
}
public static void main(String[] args) {
   String Index_Store_Path = "D:/index/1";
   File file = new File(Index_Store_Path);
   try {
    Directory Index = FSDirectory.open(file);
    IndexWriter writer = new IndexWriter(Index, new StandardAnalyzer(Version.LUCENE_CURRENT), true,
      MaxFieldLength.LIMITED);
    writer.setUseCompoundFile(false);
    Document doc1 = new Document();
    Field f11 = new Field("bookNumber", "0000001", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f12 = new Field("bookName", "钢铁是怎样炼成的", Field.Store.YES, Field.Index.ANALYZED);
    Field f13 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc1.add(f11);
    doc1.add(f12);
    doc1.add(f13);
    Document doc2 = new Document();
    Field f21 = new Field("bookNumber", "0000002", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f22 = new Field("bookName", "钢铁战士", Field.Store.YES, Field.Index.ANALYZED);
    Field f23 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc2.add(f21);
    doc2.add(f22);
    doc2.add(f23);
    Document doc3 = new Document();
    Field f31 = new Field("bookNumber", "0000003", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f32 = new Field("bookName", "篱笆女人和狗", Field.Store.YES, Field.Index.ANALYZED);
    Field f33 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc3.add(f31);
    doc3.add(f32);
    doc3.add(f33);
    Document doc4 = new Document();
    Field f41 = new Field("bookNumber", "0000004", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f42 = new Field("bookName", "女人是水做的", Field.Store.YES, Field.Index.ANALYZED);
    Field f43 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc4.add(f41);
    doc4.add(f42);
    doc4.add(f43);
    Document doc5 = new Document();
    Field f51 = new Field("bookNumber", "0000005", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f52 = new Field("bookName", "英雄儿女", Field.Store.YES, Field.Index.ANALYZED);
    Field f53 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc5.add(f51);
    doc5.add(f52);
    doc5.add(f53);
    Document doc6 = new Document();
    Field f61 = new Field("bookNumber", "0000006", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f62 = new Field("bookName", "白毛女", Field.Store.YES, Field.Index.ANALYZED);
    Field f63 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc6.add(f61);
    doc6.add(f62);
    doc6.add(f63);
    Document doc7 = new Document();
    Field f71 = new Field("bookNumber", "0000007", Field.Store.YES, Field.Index.NOT_ANALYZED);
    Field f72 = new Field("bookName", "我的兄弟和女儿", Field.Store.YES, Field.Index.ANALYZED);
    Field f73 = new Field("publishDate", "1970-01-01", Field.Store.YES, Field.Index.NOT_ANALYZED);
    doc7.add(f71);
    doc7.add(f72);
    doc7.add(f73);
    writer.addDocument(doc1);
    writer.addDocument(doc2);
    writer.addDocument(doc3);
    writer.addDocument(doc4);
    writer.addDocument(doc5);
    writer.addDocument(doc6);
    writer.addDocument(doc7);
    writer.optimize();
    writer.close();
    IndexSearcher searcher = new IndexSearcher(Index);
    TermQuery q = new TermQuery(new Term("bookName", "女"));   
    ScoreDoc[] hits = searcher.search(q, null, 1000, Sort.RELEVANCE).scoreDocs;
    for (int i = 0; i < hits.length; i++) {
     Document hitDoc = searcher.doc(hits[i].doc);
     System.out.print("书名：");
     System.out.println(hitDoc.get("bookName"));
     System.out.print("得分：");
     System.out.println(hits[i].score);
     System.out.print("内部ID ：");
     System.out.println(hits[i].doc);
     System.out.print("书号：");
     System.out.println(hitDoc.get("bookNumber"));
     System.out.print("发行日期：");
     System.out.println(hitDoc.get("publishDate"));
    }
   } catch (Exception e) {
    e.printStackTrace();
   }
}
}
运行结果：
Sort属性设为RELEVANCE：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436402520.png)
Sort属性设为INDEXORDE：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/201110191436419489.png)
去除Sort参数后：
![](http://images.cnblogs.com/cnblogs_com/ibook360/201110/20111019143641917.png)
