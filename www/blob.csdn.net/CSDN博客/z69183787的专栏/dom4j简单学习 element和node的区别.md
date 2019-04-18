# dom4j简单学习 element和node的区别 - z69183787的专栏 - CSDN博客
2014年02月10日 15:43:21[OkidoGreen](https://me.csdn.net/z69183787)阅读数：38811
使用dom4j解析处理html时，遇到一些麻烦的问题：
基中必须要使用XPath取得节点，有个命名空间的问题，需要如下处理：
HashMap<String, String> gm = new HashMap<String, String>();
gm.put("xmlns", "http://www.w3.org/1999/xhtml");
Document rootDoc = DocumentHelper.parseText(s);
XPath xpathTable = DocumentHelper
.createXPath("//xmlns:table[@class=\"tableList\"]");
xpathTable.setNamespaceURIs(gm);
List<Node> tables = xpathTable.selectNodes(rootDoc);
现在取出来是Node对象（比如一个table），再往下取时，再用node1.selectNodes会取不到东西，可能还是因为命名空间的麻烦问题，所以最好转为Element，就很容易了，element1.elements("tr")，就可以方便地循环了，比较奇怪的是转换为Element的方法居然是强转-_-!
Element table1 = (Element) tables.get(0); // 第一个表格
由此可以这样理解Node和Element，Node是节点，一个属性、一段文字、一个注释等都是节点，而Element是元素，是比较完整的一个xml的元素，即我们口头上说的xml“结点”（此处故意使用“结”字，以示与“节点”Node区别），呵呵……
dom4j帮助文档在dom4j-1.6.1/docs/guide.html文件里面，全部是英文，但是基本上应该能看懂。
1、  四种操作xml的方式:SAX,DOM,JDOM,DOM4J.
2、  DOM(Document Object Model)：意思是把整个xml做为一个整体对象，直接全部放到内存里面，不管xml文件有多大；
3、  SAX=Sample API for XML：假如xml文件有2G，用第一种方式，是很困难的，SAX用的是一种类似流媒体方式进行的。
DOM和SAX是java的API，处理xml文件的时候，相对来说是比较困难的，于是出现了下面两种框架，包装了上面两个框架。
4、  JDOM：
5、  DOM4J：
6、  利用DOM4J解析xml文件（读取所有的属性节点）。
Document可以理解为整个文档对象
取root节点。Document.getRootElement()返回的是一个Element（元素）
7、  理解运用xpath，path就是路径，xpath在xml文件里面找到特定的节点。
Document.selectNodes(“//hibernate-mapping/class/property”)  //其中hibenate-mapping是根节点。做变org.dom4j.Node对象返回的。
Document.selectSingleNodes(“//hibernate-mapping/class/property”)//只拿第一个。
基中@name是property的属性名valueOf是拿出name的值；
需要加入DOM4J里面的jaxen包
8、  DOM4J的生成xml文件
9、  用FileWriter生成xml文件的时候，可能没有格式，可以用XMLWriter类，这样自动进行美化。
10、  DOM4J修改xml文件；很少用。
11、  自动生成代码往往用的是：freemarker或velocity，这是常用的。用dom4j生成也很少用。
12、  怎么用表里面的结构生成xml文件。
13、 JavaDB
a)       Properties对象：可以理解为一个表格对应key,value
14、  Databasemetadata，可以能过这个JDK类，读取数据库的表结构，通过这个表结构做一些自己的事情。（熟悉用法）通过JDK文档和google学习
15、   元数据就是数据的数据，用来描述数据的数据。
16、   DWR，对Ajax框架封住得比较好，是类和java的结合。
dom4j学习实例：
**[java]**[view
 plain](http://blog.csdn.net/huangbiao86/article/details/6591667#)[copy](http://blog.csdn.net/huangbiao86/article/details/6591667#)
- package hb.dom4j;  
- 
- import java.io.FileWriter;  
- import java.io.IOException;  
- import java.util.Iterator;  
- import java.util.List;  
- 
- import org.dom4j.Attribute;  
- import org.dom4j.Document;  
- import org.dom4j.DocumentException;  
- import org.dom4j.DocumentHelper;  
- import org.dom4j.Element;  
- import org.dom4j.Node;  
- import org.dom4j.io.OutputFormat;  
- import org.dom4j.io.SAXReader;  
- import org.dom4j.io.XMLWriter;  
- 
- publicclass MyJdomReaderXML {  
- public Document parse(String string) throws DocumentException {  
-         SAXReader reader = new SAXReader();  
-         Document document = reader.read(string);  
- return document;  
-     }  
- 
- publicvoid bar(Document document) throws DocumentException {  
- 
-         Element root = document.getRootElement();//获得根节点；
- //进行迭代；读取根节点下的所有节点和子节点下的所有节点
- for (Iterator i = root.elementIterator(); i.hasNext();) {  
-             Element element = (Element) i.next();  
-             System.out.println(element.getName());  
- for (Iterator j = element.elementIterator(); j.hasNext();) {  
-                 System.out.println(((Element) j.next()).getName());  
-             }  
-         }  
- 
- //读取节点名为foo的所有子节点
- for (Iterator i = root.elementIterator("foo"); i.hasNext();) {  
-             Element foo = (Element) i.next();  
-             System.out.println(foo.getName());  
-         }  
- 
- //读取根节点的所有属性
- for (Iterator i = root.attributeIterator(); i.hasNext();) {  
-             Attribute attribute = (Attribute) i.next();  
-             System.out.println(attribute.getName());  
-         }  
-     }  
- 
- /*
-      * 可以根据节点名字读取节点，也可以读取节点里的key和value
-      */
- publicvoid readNodes(Document document) {  
-         List list = document.selectNodes( "//foo/bar" );   //foo为根节点，获得根节点下的bar节点  
-         Node node = document.selectSingleNode( "//foo/bar/author" );  //获得名为author的第一 节点  
-         String name = node.valueOf( "@name" ); //获得节点名属性名为name的value
-     }  
- 
- /*
-      * 如果xml文件很大的情况下，用上面的方法很费时，这样 可以用递归遍历整个xml文件
-      */
- publicvoid treeWalk(Document document) {  
-         treeWalk(document.getRootElement());  
-     }  
- 
- /*
-      * 递归调用，传递每一个父节点做为参数
-      */
- publicvoid treeWalk(Element element) {  
- for (int i = 0, size = element.nodeCount(); i < size; i++) {  
-             Node node = element.node(i);  
- if (node instanceof Element) {//如果node实现了Element接口，那么就表示node是一个节点。再递归
-                 treeWalk((Element) node);  
-                 System.out.println(((Element) node).getName()+":"+node.valueOf("@name"));  
-             } else {//如果没有实现Element接口，那么就表示这个node不是节点了，输出节点等操作；
-             }  
-         }  
-     }  
- 
- //获得节点属性名key为name的value
- publicvoid findLinks(Document document) throws DocumentException {  
-         List list = document.selectNodes( "//ehcache/cache/@name" );  
- for (Iterator iter = list.iterator(); iter.hasNext(); ) {  
-             Attribute attribute = (Attribute) iter.next();  
-             String url = attribute.getValue();  
-             System.out.println(url);  
-         }  
-     }  
- 
- /*
-      * 创建一个documnet文档
-      */
- public Document createDocument() {  
-             Document document = DocumentHelper.createDocument();  
-             Element root = document.addElement( "root" );  
- 
-             Element author1 = root.addElement( "author" )  
-                 .addAttribute( "name", "James" )  
-                 .addAttribute( "location", "UK" )  
-                 .addText( "James Strachan" );  
- 
-             Element author2 = root.addElement( "author" )  
-                 .addAttribute( "name", "Bob" )  
-                 .addAttribute( "location", "US" )  
-                 .addText( "Bob McWhirter" );  
- return document;  
-         }  
- 
- //写入xml文件
- publicvoid write(Document document) throws IOException {  
- 
- // lets write to a file
-             XMLWriter writer = new XMLWriter(  
- new FileWriter( "output.xml" )  
-             );  
-             writer.write( document );  
-             writer.close();  
- 
- 
- // Pretty print the document to System.out
-             OutputFormat format = OutputFormat.createPrettyPrint();  
-             writer = new XMLWriter( System.out, format );  
-             writer.write( document );  
- 
- // Compact format to System.out
-             format = OutputFormat.createCompactFormat();  
-             writer = new XMLWriter( System.out, format );  
-             writer.write( document );  
-         }  
- 
- /*
-      * 主函数，用来测试
-      */
- publicstaticvoid main(String[] args) throws DocumentException,  
-             IOException {  
-         MyJdomReaderXML mjrx = new MyJdomReaderXML();  
-         Document d = mjrx.parse("test.xml");  
-         mjrx.bar(d);  
-         System.out.println("------one----------");  
-         mjrx.treeWalk(d);  
-         System.out.println("------two----------");  
-         mjrx.findLinks(d);  
-     }  
- }  
** 元素(Element)和结点(Node)有区别，节点包含了元素，元素一定是节点，而必须是含有完整信息的结点才是一个元素。**
