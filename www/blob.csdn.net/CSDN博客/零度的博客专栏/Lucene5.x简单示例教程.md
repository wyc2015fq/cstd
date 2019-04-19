# Lucene5.x简单示例教程 - 零度的博客专栏 - CSDN博客
2016年10月26日 16:40:26[零度anngle](https://me.csdn.net/zmx729618)阅读数：2534
**简单实例：**
```
package zmx.lucene.test;
import org.apache.lucene.analysis.Analyzer;
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
 
public class LuceneTest {
  public static void main(String[] args) throws IOException, ParseException {
	  
	   Analyzer analyzer = new StandardAnalyzer();
	    //将索引存储到内存中
	    Directory directory = new RAMDirectory();
	    //如下想把索引存储到硬盘上，使用下面的代码代替
	    //Directory directory = FSDirectory.open(Paths.get("/tmp/testindex"));
	    
	    //写入索引库
	    IndexWriterConfig config = new IndexWriterConfig(analyzer);
	    IndexWriter iwriter = new IndexWriter(directory, config);
	    String[] texts = new String[]{
	        "Mybatis分页插件 - 示例",
	        "Mybatis 贴吧问答 第一期",
	        "Mybatis 示例之 复杂(complex)属性(property)",
	        "Mybatis极其(最)简(好)单(用)的一个分页插件",
	        "Mybatis 的Log4j日志输出问题 - 以及有关日志的所有问题",
	        "Mybatis 示例之 foreach （下）",
	        "Mybatis 示例之 foreach （上）",
	        "Mybatis 示例之 SelectKey",
	        "Mybatis 示例之 Association (2)",
	        "Mybatis 示例之 Association"
	    };
	    for (String text : texts) {
	        Document doc = new Document();
	       // doc.add(new Field("fieldname", text, TextField.TYPE_STORED));
	        doc.add(new TextField("title", text, Field.Store.YES));  
	        doc.add(new StringField("isbn", ""+String.valueOf(Math.random()), Field.Store.YES));
	        iwriter.addDocument(doc);
	    }
	    iwriter.close();
	    //读取索引并查询
	    DirectoryReader reader = DirectoryReader.open(directory);
	    IndexSearcher isearcher = new IndexSearcher(reader);
	    //解析一个简单的查询
	    QueryParser parser = new QueryParser("title", analyzer);
	    Query query = parser.parse("foreach");
	    ScoreDoc[] hits = isearcher.search(query, 1000).scoreDocs;
	    //迭代输出结果
	    for (int i = 0; i < hits.length; i++) {
	        Document hitDoc = isearcher.doc(hits[i].doc);
	        System.out.println(hitDoc.get("title"));
	        System.out.println(hitDoc.get("isbn"));
	    }
	    reader.close();
	    directory.close();
  }
}
```
运行结果：
Mybatis 示例之 foreach （下）
0.8654220005188332
Mybatis 示例之 foreach （上）
0.4180114705503467
## **Lucene API ：**
### **`org.apache.lucene.analysis`**
定义了从`Reader`转换为`TokenStream`的抽象`Analyzer`API，主要就是分词器。提供了一些默认的实现，包含`StopAnalyzer`和基于文法的`StandardAnalyzer`。中文分词可以参考[中文分词库 IKAnalyzer](http://www.oschina.net/p/ikanalyzer)。
### **`org.apache.lucene.codecs`**
提供了一个抽象的编码和解码的倒排索引结构，还提供了一些不同的实现可以应用于不同的程序需求。
### **`org.apache.lucene.document`**
提供了一个简单的`Document`类。一个文档只是一组命名的字段，它的值可以是字符串或者`Reader`的实例。
### **`org.apache.lucene.index`**
提供了两个主要的类：`IndexWriter`用于创建和给文档添加索引，`IndexReader`用于访问索引数据。
### **`org.apache.lucene.search`**
提供代表查询的数据结构（例如`TermQuery`用于单独的关键字查询，`PhraseQuery`用于短句，`BooleanQuery`用于布尔联合查询）。
`IndexSearcher`将查询转换为`TopDocs`。一些`QueryParsers`提供了从字符串或者xml生成查询结构的功能。
### **`org.apache.lucene.store`**
定义了一个抽象类来存储持久化数据，`Directory`这是一个由`IndexOutput`和`IndexInput`分别写和读取的指定文件的集合。提供了多个实现，包括`FSDirectory`，这个实现使用文件系统目录来存储文件。还有`RAMDirectory`类实现了文件驻留在内存中的数据结构。
### **`org.apache.lucene.util`**
包含了一些有用的数据结构和工具类，例如`FixedBitSet`和`PriorityQueue`。
## **Luncene使用步骤：**
- 
通过添加字段(`Field`)创建文档(`Document`)；
- 
创建`IndexWriter`，通过`addDocument()`方法添加文档(`Document`)；
- 
调用`QueryParser.parser()`方法从字符串生成查询对象；
- 
创建`IndexSearcher`并通过`search()`方法进行查询。
