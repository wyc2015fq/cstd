# 用Lucene索引数据库 - z69183787的专栏 - CSDN博客
2015年08月23日 22:20:20[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1218
1.写一段传统的JDBC程序，将每条的用户信息从数据库读取出来
2.针对每条用户记录，建立一个lucene document 
Document doc = new Document();
并根据你的需要，将用户信息的各个字段对应luncene document中的field 进行添加，如：
doc.add(new Field("NAME","USERNAME",Field.Store.YES,Field.Index.UN_TOKENIZED));
然后将该条doc加入到索引中，如： luceneWriter.addDocument(doc);
这样就建立了lucene的索引库
3.编写对索引库的搜索程序（看lucene文档），通过对lucene的索引库的查找，你可以快速找到对应记录的ID
4.通过ID到数据库中查找相关记录
**用Lucene索引数据库**
Lucene，作为一种全文搜索的辅助工具，为我们进行条件搜索，无论是像Google,Baidu之类的搜索引擎，还是论坛中的搜索功能，还是其它C/S架构的搜索，都带来了极大的便利和比较高的效率。本文主要是利用Lucene对MS
 Sql Server 2000进行建立索引，然后进行全文索引。至于数据库的内容，可以是网页的内容，还是其它的。本文中数据库的内容是图书馆管理系统中的某个作者表－ Authors表。
　　因为考虑到篇幅的问题，所以该文不会讲的很详细，也不可能讲的很深。
　　本文以这样的结构进行：
1.介绍数据库中Authors表的结构
2.为数据库建立索引
3.为数据库建立查询功能
4.在web界面下进行查询并显示结果
1．介绍数据库中Authors表的结构
字段名称字段类型字段含义
Au_id                Varchar(11)    作者号
Au_name        Varchar(60)     作者名
Phone             Char(12)           电话号码
Address          Varchar(40)      地址
City                   Varchar(20)     城市
State                Char(2)             省份
Zip                    Char(5)             邮编
contract            Bit(1)                外键（关系不大）
表中的部分内容：
2．为数据库建立索引
　　首先建立一个类TestLucene.java。这个类就是对数据库进行建立索引，编写查询条件等。
　　当然，最开始就是建立数据库连接。连接代码这里就省略了。^_^
　　接着，新建一个方法getResutl(String),它返回的是数据库表Authors的内容。具体代码如下：
    public ResultSet getResult(String sql){
      try{
        Statement stmt = conn.createStatement();
        ResultSet rs = stmt.executeQuery(sql);
        return rs;
      }
      catch(SQLException e){
        System.out.println(e);
      }
      return null;
    }
　　然后，为数据库建立索引。
　首先要定义一个IndexWriter（），它是将索引写进Lucene自己的数据库中，它存放的位置是有你自己定义的。在定义IndexWriter 是需要指定它的分析器。Lucene自己自带有几个分析器，例如：StandarAnalyzer(),SimpleAnalyzer(),
 StopAnalyzer()等。它作用是对文本进行分析，判断如何进行切词。
接着，要定义一个Document。Document相当于二维表中一行数据一样。Document里包含的是Field字段，Field相当于数据库中一列，也就是一个属性，一个字段。
最后应该对IndexWriter进行优化，方法很简单，就是writer.optimize().
具体代码如下：
  public void Index(ResultSet rs){
      try{
        IndexWriter writer = new IndexWriter("d:/index/", getAnalyzer(), true);
        while(rs.next()){
            Document doc=new Document();
            doc.add(Field.Keyword("id",rs.getString("au_id")));
            doc.add(Field.Text("name",rs.getString("au_name")));
            doc.add(Field.UnIndexed("address",rs.getString("address")));
            doc.add(Field.UnIndexed("phone",rs.getString("phone")));
            doc.add(Field.Text("City",rs.getString("city")));
            writer.addDocument(doc);
          }
        writer.optimize();
        writer.close();
      }
      catch(IOException e){
        System.out.println(e);
      }
      catch(SQLException e){
        System.out.println(e);
      }
    }
    public Analyzer getAnalyzer(){
      return new StandardAnalyzer();
    }
3．为数据库建立查询功能
　　在类TestLucene中建立一个新的方法searcher(String)，它返回的是一个搜索的结构集，相当于数据库中的ResultSet一样。它代的参数是你要查询的内容。这里，我把要查询的字段写死了。你可以在添加一个参数表示要查询的字段。
这里主要有两个对象IndexSearcher和Query。IndexSearcher是找到索引数据库，Query是处理搜索，它包含了三个参数：查询内容，查询字段，分析器。
具体代码如下：
  public Hits seacher(String queryString){
      Hits hits=null;;
      try{
        IndexSearcher is = new IndexSearcher("D:/index/");
        Query query=QueryParser.parse(queryString,"City",getAnalyzer());
        hits=is.search(query);
      }catch(Exception e){
        System.out.print(e);
      }
      return hits;
    }
4．在web界面下进行查询并显示结果
　　这里建立一个Jsp页面TestLucene.jsp进行搜索。
　　在TestLucene.jsp页面中首先引入类
<%@ page import="lucenetest.LucentTest"%>
<%@ page import="org.apache.lucene.search.*,org.apache.lucene.document.*" %>
　　然后定义一个LuceneTest对象，获取查询结果集：
  LucentTest lucent=new LucentTest();
  Hits hits=lucent.seacher(request.getParameter("queryString"));
　　定义一个Form，建立一个查询环境：
<form action="TestLucene.jsp">
  <input  type="text" name="queryString"/>
  <input type="submit" value="搜索"/>
</form>
　　显示查询结果：
<table>
  <%if(hits!=null){%>
  <tr>
    <td>作者号</td>
    <td>作者名</td>
    <td>地址</td>
    <td>电话号码</td>
  </tr>
 <% for(int i=0;i<hits.length();i++){
    Document doc=hits.doc(i);
   %>
    <tr>
    <td><%=doc.get("id") %></td>
    <td><%=doc.get("name") %></td>
    <td><%=doc.get("address") %></td>
    <td><%=doc.get("phone") %></td>
  </tr>
 <% }}%>
</table>
**用Lucene-1.3-final为网站数据库建立索引**
下是看了lnboy写的《用lucene建立大富翁论坛的全文检索》后写的测试代码。
为数据库cwb.mdb建立全文索引的indexdb.jsp 
<%@ page import ="org.apache.lucene.analysis.standard.*" %>   
<%@ page import="org.apache.lucene.index.*" %>  
<%@ page import="org.apache.lucene.document.*" %>  
<%@ page import="lucene.*" %>  
<%@ page contentType="text/html; charset=GBK" %>  
<%  
      long start = System.currentTimeMillis();  
      String aa=getServletContext().getRealPath("/")+"index";     
      IndexWriter writer = new IndexWriter(aa, new StandardAnalyzer(), true);  
    try {  
      Class.forName("sun.jdbc.odbc.JdbcOdbcDriver").newInstance(); 
 String url = "jdbc:odbc:driver={Microsoft Access Driver (*.mdb)}
       ;DBQ=d://Tomcat 5.0//webapps//zz3zcwbwebhome//WEB-INF//cwb.mdb";  
      Connection conn = DriverManager.getConnection(url);  
      Statement stmt = conn.createStatement();  
      ResultSet rs = stmt.executeQuery(  
          "select Article_id,Article_name,Article_intro from Article");  
      while (rs.next()) {  
             writer.addDocument(mydocument.Document(rs.getString("Article_id"),
                rs.getString("Article_name"),rs.getString("Article_intro")));  
      }  
      rs.close();  
      stmt.close();  
      conn.close();  
      out.println("索引创建完毕");     
      writer.optimize();  
      writer.close();  
      out.print(System.currentTimeMillis() - start);  
      out.println(" total milliseconds");  
    }  
    catch (Exception e) {  
      out.println(" 出错了 " + e.getClass() +  
                         "/n 错误信息为: " + e.getMessage());  
    }  
 %>  
用于显示查询结果的aftsearch.jsp  
<%@ page import="org.apache.lucene.search.*" %>  
<%@ page import="org.apache.lucene.document.*" %>  
<%@ page import="lucene.*" %>  
<%@ page import = "org.apache.lucene.analysis.standard.*" %>   
<%@ page import="org.apache.lucene.queryParser.QueryParser" %>  
<%@ page contentType="text/html; charset=GBK" %>  
<%  
    String keyword=request.getParameter("keyword");  
     keyword=new String(keyword.getBytes("ISO8859_1"));   
      out.println(keyword);  
   try {  
       String aa=getServletContext().getRealPath("/")+"index";     
      Searcher searcher = new IndexSearcher(aa);  
      Query query = QueryParser.parse(keyword, "Article_name", new StandardAnalyzer());  
      out.println("正在查找: " + query.toString("Article_name")+"<br>");  
      Hits hits = searcher.search(query);  
      System.out.println(hits.length() + " total matching documents");  
      java.text.NumberFormat format = java.text.NumberFormat.getNumberInstance();  
      for (int i = 0; i < hits.length(); i++) {  
        //开始输出查询结果  
        Document doc = hits.doc(i);  
        out.println(doc.get("Article_id"));  
        out.println("准确度为：" + format.format(hits.score(i) * 100.0) + "%");  
        out.println(doc.get("Article_name")+"<br>");  
       // out.println(doc.get("Article_intro"));  
      }  
    }catch (Exception e) {  
      out.println(" 出错了 " + e.getClass() +"/n 错误信息为: " + e.getMessage());  
    }  
%>  
辅助类：  
package lucene;  
import org.apache.lucene.document.Document;  
import org.apache.lucene.document.Field;  
import org.apache.lucene.document.DateField;  
public class mydocument {  
public static Document Document(String Article_id,String Article_name,String Article_intro){  
     Document doc = new Document();  
      doc.add(Field.Keyword("Article_id", Article_id));  
      doc.add(Field.Text("Article_name", Article_name));  
      doc.add(Field.Text("Article_intro", Article_intro));  
      return doc;  
  }  
  public mydocument() {  
  }  
} 
**用lucene为数据库搜索建立增量索引**
用 lucene 建立索引不可能每次都重新开始建立,而是按照新增加的记录,一次次的递增
建立索引的IndexWriter类,有三个参数
IndexWriter writer = new IndexWriter(path, new StandardAnalyzer(),isEmpty);
其中第三个参数是bool型的,指定它可以确定是增量索引,还是重建索引.
对于从数据库中读取的记录,譬如要为文章建立索引,我们可以记录文章的id号,然后下次再次建立索引的时候读取存下的id号,从此id后往下继续增加索引,逻辑如下.
建立增量索引,主要代码如下
publicvoid createIndex(String path)
{
     Statement myStatement = null;
     String articleId="0";
//读取文件,获得文章id号码,这里只存最后一篇索引的文章id
try { 
        FileReader fr = new FileReader("**.txt");
        BufferedReader br = new BufferedReader(fr);                 
        articleId=br.readLine();
if(articleId==null||articleId=="")
        articleId="0";
        br.close();
        fr.close(); 
      } catch (IOException e) { 
        System.out.println("error343!");
        e.printStackTrace();
      }
try {
//sql语句,根据id读取下面的内容
        String sqlText = "*****"+articleId;
        myStatement = conn.createStatement();
        ResultSet rs = myStatement.executeQuery(sqlText);
//写索引
while (rs.next()) {
         Document doc = new Document();
         doc.add(Field.Keyword("**", DateAdded));
         doc.add(Field.Keyword("**", articleid));
         doc.add(Field.Text("**", URL));    
         doc.add(Field.Text("**", Content));
         doc.add(Field.Text("**", Title));    
try{
            writer.addDocument(doc);
          }
catch(IOException e){
            e.printStackTrace();
         }
//将我索引的最后一篇文章的id写入文件
try { 
           FileWriter fw = new FileWriter("**.txt");
           PrintWriter out = new PrintWriter(fw);    
           out.close();
           fw.close();
           } catch (IOException e) { 
             e.printStackTrace();
           }
         }
            ind.Close();
            System.out.println("ok.end");
         }
catch (SQLException e){
            e.printStackTrace();
        }
finally {
//数据库关闭操作
        }        
    }
然后控制是都建立增量索引的时候根据能否都到id值来设置IndexWriter的第三个参数为true 或者是false
boolean isEmpty = true;
try { 
    FileReader fr = new FileReader("**.txt");
    BufferedReader br = new BufferedReader(fr);                 
if(br.readLine()!= null) {
        isEmpty = false;
     }
     br.close();
     fr.close(); 
    } catch (IOException e) { 
       e.printStackTrace();
  }
  writer = new IndexWriter(Directory, new StandardAnalyzer(),isEmpty);
