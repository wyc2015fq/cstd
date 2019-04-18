# Lucene五分钟教程 - z69183787的专栏 - CSDN博客
2015年08月23日 20:03:36[OkidoGreen](https://me.csdn.net/z69183787)阅读数：573
Lucene大大简化了在应用中集成全文搜索的功能。但实际上Lucene十分简单，我可以在五分钟之内向你展示如何使用Lucene。
### 1. 建立索引
为了简单起见，我们下面为一些字符串创建内存索引：
```java
StandardAnalyzer analyzer = new StandardAnalyzer(Version.LUCENE_40);
Directory index = new RAMDirectory();
 
IndexWriterConfig config = new IndexWriterConfig(Version.LUCENE_40, analyzer);
 
IndexWriter w = new IndexWriter(index, config);
addDoc(w, "Lucene in Action", "193398817");
addDoc(w, "Lucene for Dummies", "55320055Z");
addDoc(w, "Managing Gigabytes", "55063554A");
addDoc(w, "The Art of Computer Science", "9900333X");
w.close();
```
addDoc()方法把文档（译者注：这里的文档是Lucene中的Document类的实例）添加到索引中。
```java
private static void addDoc(IndexWriter w, String title, String isbn) throws IOException {
  Document doc = new Document();
  doc.add(new TextField("title", title, Field.Store.YES));
  doc.add(new StringField("isbn", isbn, Field.Store.YES));
  w.addDocument(doc);
}
```
注意，对于需要分词的内容我们使用TextField，对于像id这样不需要分词的内容我们使用StringField。
### 2.搜索请求
我们从标准输入（stdin）中读入搜索请求，然后对它进行解析，最后创建一个Lucene中的Query对象。
```java
String querystr = args.length > 0 ? args[0] : "lucene";
Query q = new QueryParser(Version.LUCENE_40, "title", analyzer).parse(querystr);
```
### 3.搜索
我们创建一个Searcher对象并且使用上面创建的Query对象来进行搜索，匹配到的前10个结果封装在TopScoreDocCollector对象里返回。
```java
int hitsPerPage = 10;
IndexReader reader = IndexReader.open(index);
IndexSearcher searcher = new IndexSearcher(reader);
TopScoreDocCollector collector = TopScoreDocCollector.create(hitsPerPage, true);
searcher.search(q, collector);
ScoreDoc[] hits = collector.topDocs().scoreDocs;
```
### 4.展示
现在我们得到了搜索结果，我们需要想用户展示它。
```java
System.out.println("Found " + hits.length + " hits.");
for(int i=0;i<hits.length;++i) {
    int docId = hits[i].doc;
    Document d = searcher.doc(docId);
    System.out.println((i + 1) + ". " + d.get("isbn") + "\t" + d.get("title"));
}
```
这里是这个小应用的完整代码。[下载HelloLucene.java](http://www.lucenetutorial.com/code/HelloLucene.java)。
```java
import org.apache.lucene.analysis.standard.StandardAnalyzer;
import org.apache.lucene.document.Document;
import org.apache.lucene.document.Field;
import org.apache.lucene.document.StringField;
import org.apache.lucene.document.TextField;
import org.apache.lucene.index.DirectoryReader;
import org.apache.lucene.index.IndexReader;
import org.apache.lucene.index.IndexWriter;
import org.apache.lucene.index.IndexWriterConfig;
import org.apache.lucene.queryparser.classic.ParseException;
import org.apache.lucene.queryparser.classic.QueryParser;
import org.apache.lucene.search.IndexSearcher;
import org.apache.lucene.search.Query;
import org.apache.lucene.search.ScoreDoc;
import org.apache.lucene.search.TopScoreDocCollector;
import org.apache.lucene.store.Directory;
import org.apache.lucene.store.RAMDirectory;
import org.apache.lucene.util.Version;
 
import java.io.IOException;
 
public class HelloLucene {
  public static void main(String[] args) throws IOException, ParseException {
    // 0. Specify the analyzer for tokenizing text.
    //    The same analyzer should be used for indexing and searching
    StandardAnalyzer analyzer = new StandardAnalyzer(Version.LUCENE_40);
 
    // 1. create the index
    Directory index = new RAMDirectory();
 
    IndexWriterConfig config = new IndexWriterConfig(Version.LUCENE_40, analyzer);
 
    IndexWriter w = new IndexWriter(index, config);
    addDoc(w, "Lucene in Action", "193398817");
    addDoc(w, "Lucene for Dummies", "55320055Z");
    addDoc(w, "Managing Gigabytes", "55063554A");
    addDoc(w, "The Art of Computer Science", "9900333X");
    w.close();
 
    // 2. query
    String querystr = args.length > 0 ? args[0] : "lucene";
 
    // the "title" arg specifies the default field to use
    // when no field is explicitly specified in the query.
    Query q = new QueryParser(Version.LUCENE_40, "title", analyzer).parse(querystr);
 
    // 3. search
    int hitsPerPage = 10;
    IndexReader reader = DirectoryReader.open(index);
    IndexSearcher searcher = new IndexSearcher(reader);
    TopScoreDocCollector collector = TopScoreDocCollector.create(hitsPerPage, true);
    searcher.search(q, collector);
    ScoreDoc[] hits = collector.topDocs().scoreDocs;
     
    // 4. display results
    System.out.println("Found " + hits.length + " hits.");
    for(int i=0;i<hits.length;++i) {
      int docId = hits[i].doc;
      Document d = searcher.doc(docId);
      System.out.println((i + 1) + ". " + d.get("isbn") + "\t" + d.get("title"));
    }
 
    // reader can only be closed when there
    // is no need to access the documents any more.
    reader.close();
  }
 
  private static void addDoc(IndexWriter w, String title, String isbn) throws IOException {
    Document doc = new Document();
    doc.add(new TextField("title", title, Field.Store.YES));
 
    // use a string field for isbn because we don't want it tokenized
    doc.add(new StringField("isbn", isbn, Field.Store.YES));
    w.addDocument(doc);
  }
}
```
可以直接在命令行中使用这个小应用，键入**java HelloLucene **。
### 下面可以做什么？
- 阅读下面关于Lucene的书籍。
- [你需要应该使用Apache Solr代替Apache Lucene吗](http://www.lucenetutorial.com/lucene-vs-solr.html)？
- 更多关于[Lucene的基本概念](http://www.lucenetutorial.com/basic-concepts.html)
### 一些与Lucene和搜索相关的书籍
|![](http://incdn1.b0.upaiyun.com/2014/08/fc09de7d5ec732718c1637f5b829a1d0.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/231638e9cb0c25ad10ce0b12622dfdde.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/ace1f7166c079c3cec2640e2d83b0c4a.jpg)|
|----|----|----|
|![](http://incdn1.b0.upaiyun.com/2014/08/cca38cbb30195dcd37698af1a53cd111.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/12e608cf0a87775c6186e3469d0d1590.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/ec9743e4bf74d04836d040b43aef0415.jpg)|
|![](http://incdn1.b0.upaiyun.com/2014/08/9716268b1adce22306a2e7364900cd7f.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/2d301f3b5b5e2276662f0261f17a6b23.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/0f22400d0c765371e4d67b7907bff464.jpg)|
|![](http://incdn1.b0.upaiyun.com/2014/08/8dcea4b00fd11ebe3387810c30e9707d.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/eb103c04ef894ec1060008c830dd1700.jpg)|![](http://incdn1.b0.upaiyun.com/2014/08/c6875451044318b46a7ba1e25b120422.jpg)|
### Github上的基于maven的库
Mac Luq在Github上的基于maven的库：
[https://github.com/macluq/helloLucene]()
用下面这条命令下载它：
```
`git clone https:``//github``.com``/macluq/helloLucene``.git`
```
PS：如果你是Java新手的话，试试下面的命令：
```
`wget http:``//repo1``.maven.org``/maven2/org/apache/lucene/lucene-core/4``.0.0``/lucene-core-4``.0.0.jar`
`wget http:``//repo1``.maven.org``/maven2/org/apache/lucene/lucene-analyzers-common/4``.0.0``/lucene-analyzers-common-4``.0.0.jar`
`wget http:``//repo1``.maven.org``/maven2/org/apache/lucene/lucene-queryparser/4``.0.0``/lucene-queryparser-4``.0.0.jar`
`wget http:``//www``.lucenetutorial.com``/code/HelloLucene``.java`
`javac -classpath .:lucene-core-4.0.0.jar:lucene-analyzers-common-4.0.0.jar:lucene-queryparser-4.0.0.jar HelloLucene.java`
`java -classpath .:lucene-core-4.0.0.jar:lucene-analyzers-common-4.0.0.jar:lucene-queryparser-4.0.0.jar HelloLucene`
```
你会得到下面的结果：
```
`Found 2 hits.`
`1. Lucene ``in``Action`
`2. Lucene ``for``Dummies`
```
Erik，一个可能对你有所帮助的读者抱怨到：
编译过程还算顺利，但是我不能正常运行这段代码。在网上搜索并且自己尝试了以后发现Lucene的jar文件必须在classpath中，否则运行不起来。这可能对很多像我这样的java初学者很多帮助。
### 安装Lucene
PS：我发现一些初学者在安装Lucene时有些困难。
你应该先[下载Lucene](http://www.apache.org/dyn/closer.cgi/lucene/java/)，然后把它解压到一个你用于编程的目录。
如果你使用Netbeans，你也可以这么做：
- 遵循[这里的](http://www.jguru.com/forums/view.jsp?EID=1303995)教程。
- 按照下面的步骤：
- 通过以此点击Netbeans菜单栏上的“工具”，然后选择“库管理器”，把Lucene的jar文件作为外部类库加进来。
- 在Lucene项目上面右键，选择“属性”
- 在弹出来的对话框中，以此选择“类库”,”添加jar或文件夹”选项
- 定位到从lucene-[version].tar.gz解压出来的文件夹上，选择 lucene-core-[version].jar。
- 点击“确定”，现在jar文件就已经添加到你项目的classpath中去了。
