
# Java文件操作①——XML文件的读取 - anlcy - 博客园






# [Java文件操作①——XML文件的读取](https://www.cnblogs.com/camilla/p/6845889.html)

## 一、邂逅XML
文件种类是丰富多彩的，XML作为众多文件类型的一种，经常被用于数据存储和传输。所以XML在现今应用程序中是非常流行的。本文主要讲Java解析和生成XML。用于不同平台、不同设备间的数据共享通信。
XML文件的表现：以“.xml”为文件扩展名的文件；
存储结构：树形结构；
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711112324764-685150665.png)
节点名称区分大小写。
1、<book id="1"></book> id为属性， <book><id>1</id></book> id为节点
2、xml文件开头要加上版本信息和编码方式<?xml version="1.0" encoding="UTF-8"?>
比如：
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711112844389-877076739.png)
### ❤ 为什么要使用XML？
思考1：不同应用程序之间的通信？
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711113151873-462684862.png)
思考2：不同平台间的通信？
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711113223326-135014397.png)
思考3：不同平台间的数据共享？
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711113315279-1515996557.png)
答案就是我们要学习的XML文件。我们可以使用相同的xml把不同的文件联系起来
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711113519826-78660407.png)
[回到顶部](http://www.cnblogs.com/Qian123/p/5231303.html#_labelTop)
## 二、应用 DOM 方式解析 XML
❤ 在Java程序中如何获取XML文件的内容
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711115535795-1239960668.png)
解析的目的：获取节点名、节点值、属性名、属性值；
四种解析方式：**DOM、****SAX、****DOM4J、****JDOM**
DOM、SAX ：java 官方方式，不需要下载jar包
DOM4J、JDOM ：第三方，需要网上下载jar包
示例：解析XML文件，目标是解析XML文件后，Java程序能够得到xml文件的所有数据
思考：如何在Java程序中保留xml数据的结构？
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711131447764-654390440.png)
如何保留节点之间的层级关系？
注意常用的节点类型：
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711155901295-463945806.png)
下面介绍DOM方式解析XML：
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711132023998-1829651630.png)
功能说明：
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711152223842-1292427297.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160711161513686-1189786216.png)
代码示例：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1packagecom.study.domtest;23importjava.io.IOException;45importjavax.xml.parsers.DocumentBuilder;6importjavax.xml.parsers.DocumentBuilderFactory;7importjavax.xml.parsers.ParserConfigurationException;89importorg.w3c.dom.Document;10importorg.w3c.dom.NamedNodeMap;11importorg.w3c.dom.Node;12importorg.w3c.dom.NodeList;13importorg.xml.sax.SAXException;1415/**16* DOM方式解析xml17*/18publicclassDOMTest {1920publicstaticvoidmain(String[] args) {21//1、创建一个DocumentBuilderFactory的对象22         DocumentBuilderFactory dbf =DocumentBuilderFactory.newInstance();23//2、创建一个DocumentBuilder的对象24try{25//创建DocumentBuilder对象26             DocumentBuilder db =dbf.newDocumentBuilder();27//3、通过DocumentBuilder对象的parser方法加载books.xml文件到当前项目下28/*注意导入Document对象时，要导入org.w3c.dom.Document包下的*/29             Document document = db.parse("books.xml");//传入文件名可以是相对路径也可以是绝对路径30//获取所有book节点的集合31             NodeList bookList = document.getElementsByTagName("book");32//通过nodelist的getLength()方法可以获取bookList的长度33             System.out.println("一共有" + bookList.getLength() + "本书");34//遍历每一个book节点35for (int i = 0; i < bookList.getLength(); i++) {36                 System.out.println("=================下面开始遍历第" + (i + 1) + "本书的内容=================");37//❤未知节点属性的个数和属性名时:38//通过 item(i)方法 获取一个book节点，nodelist的索引值从0开始39                 Node book =bookList.item(i);40//获取book节点的所有属性集合41                 NamedNodeMap attrs =book.getAttributes();42                 System.out.println("第 " + (i + 1) + "本书共有" + attrs.getLength() + "个属性");43//遍历book的属性44for (int j = 0; j < attrs.getLength(); j++) {45//通过item(index)方法获取book节点的某一个属性46                     Node attr =attrs.item(j);47//获取属性名48                     System.out.print("属性名：" +attr.getNodeName());49//获取属性值50                     System.out.println("--属性值" +attr.getNodeValue());51}52//❤已知book节点有且只有1个id属性:53/*54//前提：已经知道book节点有且只能有1个id属性55//将book节点进行强制类型转换，转换成Element类型56Element book1 = (Element) bookList.item(i);57//通过getAttribute("id")方法获取属性值58String attrValue = book1.getAttribute("id");59System.out.println("id属性的属性值为" + attrValue);60*/6162//解析book节点的子节点63                 NodeList childNodes =book.getChildNodes();64//遍历childNodes获取每个节点的节点名和节点值65                 System.out.println("第" + (i+1) + "本书共有" + childNodes.getLength() + "个子节点");66for (int k = 0; k < childNodes.getLength(); k++) {67//区分出text类型的node以及element类型的node68if(childNodes.item(k).getNodeType() ==Node.ELEMENT_NODE){69//获取了element类型节点的节点名70                         System.out.print("第" + (k + 1) + "个节点的节点名：" +childNodes.item(k).getNodeName());71//获取了element类型节点的节点值72                         System.out.println("--节点值是：" +childNodes.item(k).getFirstChild().getNodeValue());73//System.out.println("--节点值是：" + childNodes.item(k).getTextContent());74}75}76                 System.out.println("======================结束遍历第" + (i + 1) + "本书的内容=================");77}7879         }catch (ParserConfigurationException e) {
80             e.printStackTrace();
81         } catch (SAXException e) {
82             e.printStackTrace();
83         } catch (IOException e) {
84             e.printStackTrace();
85         }
86     }
87 
88 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
[回到顶部](http://www.cnblogs.com/Qian123/p/5231303.html#_labelTop)
## 三、应用 SAX 方式解析 XML
SAX是SIMPLE API FOR XML的缩写，与DOM比较而言，SAX是一种轻量型的方法。
**Dom解析会将整个xml文件加载到内存中，然后再逐个解析**
**Sax解析是通过Handler处理类逐个依次解析每个节点**
在处理DOM的时候，我们需要读入整个的XML文档，然后在内存中创建DOM树，生成DOM树上的每个NODE对象。当文档比较小的时候，这不会造成什么问题，但是一旦文档大起来，处理DOM就会变得相当费时费力。特别是其对于内存的需求，也将是成倍的增长，以至于在某些应用中使用DOM是一件很不划算的事。这时候，一个较好的替代解决方法就是SAX。
    
SAX在概念上与DOM完全不同。首先，不同于DOM的文档驱动，它是事件驱动的，也就是说，它并不需要读入整个文档，而文档的读入过程也就是SAX的解析过程。所谓事件驱动，是指一种基于回调（callback）机制的程序运行方法。在XMLReader接受XML文档，在读入XML文档的过程中就进行解析，也就是说读入文档的过程和解析的过程是同时进行的，这和DOM区别很大。
❤
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712095846951-278986128.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712100009654-1623042976.png)
代码示例：**Book实体类**
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1packagecom.study.saxtest.entity;23/**4* 用Book实体类代表XML文件中的"<book>...</book>"中整个元素5* 在遇到<book>标签，证明我们要存储新的book时需要创建Book对象6*/7publicclassBook {8privateString id;9privateString name;10privateString author;11privateString year;12privateString price;13privateString language;14publicString getId() {15returnid;16}17publicvoidsetId(String id) {18this.id =id;19}20publicString getName() {21returnname;22}23publicvoidsetName(String name) {24this.name =name;25}26publicString getAuthor() {27returnauthor;28}29publicvoidsetAuthor(String author) {30this.author =author;31}32publicString getYear() {33returnyear;34}35publicvoidsetYear(String year) {36this.year =year;37}38publicString getPrice() {39returnprice;40}41publicvoidsetPrice(String price) {42this.price =price;43}44publicString getLanguage() {45returnlanguage;46}47publicvoidsetLanguage(String language) {48this.language =language;49}50 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
SAXParserHandler类：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1packagecom.study.saxtest.handler;23importjava.util.ArrayList;45importorg.xml.sax.Attributes;6importorg.xml.sax.SAXException;7importorg.xml.sax.helpers.DefaultHandler;89importcom.study.saxtest.entity.Book;1011publicclass SAXParserHandlerextendsDefaultHandler{12/*注意DefaultHandler是org.xml.sax.helpers包下的*/1314int bookIndex = 0;//设置全局变量，用来记录是第几本书1516     String value =null;17     Book book =null;18private ArrayList<Book> bookList =new ArrayList<Book>();//保存book对象1920public ArrayList<Book>getBookList() {21returnbookList;22}2324/**25* 用来标识解析开始26*/27@Override28publicvoid startDocument()throwsSAXException {29super.startDocument();30         System.out.println("SAX解析开始");3132}3334/**35* 用来标识解析结束36*/37@Override38publicvoid endDocument()throwsSAXException {39super.endDocument();40         System.out.println("SAX解析结束");41}4243/**44* 用来遍历xml文件的开始标签45* 解析xml元素46*/47@Override48publicvoid startElement(String uri, String localName, String qName,Attributes attributes)throwsSAXException {49//调用DefaultHandler类的startElement方法50super.startElement(uri, localName, qName, attributes);51if (qName.equals("book")) {52             bookIndex++;53//创建一个book对象54/*Book*/ book =newBook();55//开始解析book元素的属性56             System.out.println("======================开始遍历第"+bookIndex+"本书的内容=================");57/*//❤已知节点的属性名时：比如已知id属性，根据属性名称获取属性值58String value = attributes.getValue("id");59System.out.print("book的属性值是："+value);*/60//❤未知节点的属性名时，获取属性名和属性值61int num=attributes.getLength();62for(int i=0;i<num;i++){63                 System.out.print("book元素的第"+(i+1)+"个属性名是："+attributes.getQName(i));64                 System.out.println("---属性值是："+attributes.getValue(i));65if (attributes.getQName(i).equals("id")) {//往book对象中塞值66book.setId(attributes.getValue(i));67}68}69         }elseif (!qName.equals("book") && !qName.equals("bookstore")) {70             System.out.print("节点名是：" + qName + "---");//此时qName获取的是节点名（标签）71}72}7374/**75* 用来遍历xml文件的结束标签76*/77@Override78publicvoid endElement(String uri, String localName, String qName)throwsSAXException {
 79         //调用DefaultHandler类的endElement方法
 80         super.endElement(uri, localName, qName);
 81         //判断是否针对一本书已经遍历结束
 82         if (qName.equals("book")) {
 83             bookList.add(book);//在清空book对象之前先保存
 84             book = null;//把book清空，方便解析下一个book节点
 85             System.out.println("======================结束遍历第"+bookIndex+"本书的内容=================");
 86         }else if (qName.equals("name")) {
 87             book.setName(value);
 88         }
 89         else if (qName.equals("author")) {
 90          book.setAuthor(value);
 91         }
 92         else if (qName.equals("year")) {
 93          book.setYear(value);
 94         }
 95         else if (qName.equals("price")) {
 96          book.setPrice(value);
 97         }
 98         else if (qName.equals("language")) {
 99          book.setLanguage(value);
100         }
101     }
102     
103     /**
104      * 获取文本
105      * 重写charaters()方法时，
106      * String(byte[] bytes,int offset,int length)的构造方法进行数组的传递
107      * 去除解析时多余空格
108      */
109     @Override
110     public void characters(char[] ch, int start, int length)throws SAXException {
111         /**
112          * ch 代表节点中的所有内容，即每次遇到一个标签调用characters方法时，数组ch实际都是整个XML文档的内容
113          * 如何每次去调用characters方法时我们都可以获取不同的节点属性？这时就必须结合start（开始节点）和length（长度）
114          */
115         super.characters(ch, start, length);
116         /*String */value = new String(ch, start, length);//value获取的是文本（开始和结束标签之间的文本）
117 //        System.out.println(value);//输出时会多出两个空格，是因为xml文件中空格与换行字符被看成为一个文本节点
118         if(!value.trim().equals("")){//如果value去掉空格后不是空字符串
119             System.out.println("节点值是：" + value);
120         }
121     }
122     
123     /**
124      * qName获取的是节点名（标签）
125      * value获取的是文本（开始和结束标签之间的文本）
126      * 思考：qName和value分别在两个方法中，如何将这两个方法中的参数整合到一起？
127      * 分析：要在两个方法中用同一个变量，就设置成全局变量，可以赋初值为null。
128      *     可以把characters()方法中的value作成一个全局变量
129      * 
130      * 然后在endElement()方法中对book对象进行塞值。记得要把Book对象设置为全局变量，变量共享
131      */
132 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
测试类：SAXTest
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1packagecom.study.saxtest.test;23importjava.io.IOException;45importjavax.xml.parsers.ParserConfigurationException;6importjavax.xml.parsers.SAXParser;7importjavax.xml.parsers.SAXParserFactory;89importorg.xml.sax.SAXException;1011importcom.study.saxtest.entity.Book;12importcom.study.saxtest.handler.SAXParserHandler;1314/**15* sax方式解析XML16*/17publicclassSAXTest {1819publicstaticvoidmain(String[] args) {20//1.获取一个SAXParserFactory的实例对象21         SAXParserFactory factory =SAXParserFactory.newInstance();22//2.通过factory的newSAXParser()方法获取一个SAXParser类的对象。23try{24             SAXParser parser =factory.newSAXParser();25//创建SAXParserHandler对象26             SAXParserHandler handler =newSAXParserHandler();27             parser.parse("books.xml", handler);28             System.out.println("~~~~~共有"+handler.getBookList().size()+"本书");29for(Book book : handler.getBookList()) {30System.out.println(book.getId());31System.out.println(book.getName());32System.out.println(book.getAuthor());33System.out.println(book.getYear());34System.out.println(book.getPrice());35System.out.println(book.getLanguage());36                 System.out.println("----finish----");37}38         }catch(ParserConfigurationException e) {39e.printStackTrace();40         }catch(SAXException e) {41e.printStackTrace();42         }catch(IOException e) {43e.printStackTrace();44}45}4647 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
运行结果：
![](https://images.cnblogs.com/OutliningIndicators/ContractedBlock.gif)View Code
[回到顶部](http://www.cnblogs.com/Qian123/p/5231303.html#_labelTop)
## 四、
## 应用 DOM4J 及 JDOM 方式解析 XML
**\# JDOM 方式解析 XML**
JDOM 开始解析前的准备工作：
JDOM是第三方提供的解析XML方法，需要jdom-2.0.5.jar包
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712113548498-676015460.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712122448311-1836869632.png)
示例代码：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1packagecom.study.jdomtest1.test;23importjava.io.FileInputStream;4importjava.io.FileNotFoundException;5importjava.io.IOException;6importjava.io.InputStream;7importjava.io.InputStreamReader;8importjava.util.ArrayList;9importjava.util.List;1011importorg.jdom2.Attribute;12importorg.jdom2.Document;13importorg.jdom2.Element;14importorg.jdom2.JDOMException;15importorg.jdom2.input.SAXBuilder;1617importcom.study.jdomtest1.entity.Book;1819/**20*  JDOM 解析XML21*/22publicclassJDOMTest {23privatestatic ArrayList<Book> booksList =new ArrayList<Book>();2425publicstaticvoidmain(String[] args) {26//进行对books.xml文件的JDOM解析27//❤准备工作28//1.创建一个SAXBuilder的对象29         SAXBuilder saxBuilder =new SAXBuilder();//注意SAXBuilder是org.jdom2.input包下的30InputStream in;31try{32//2.创建一个输入流，将xml文件加载到输入流中33             in=new FileInputStream("books.xml");//如果将xml文件放在src/res包下，此时应该输入“src/res/books.xml”34             InputStreamReader isr =new InputStreamReader(in, "UTF-8");//使用包装流InputStreamReader进行读取编码的指定，防止乱码35//3.通过saxBuilder的build方法，将输入流加载到saxBuilder中36             Document document =saxBuilder.build(isr);37//4.通过document对象获取xml文件的根节点38             Element rootElement =document.getRootElement();39//5.获取根节点下的子节点的List集合40             List<Element> bookList =rootElement.getChildren();41//❤ 继续解析，采用for循环对bookList进行遍历42for(Element book : bookList) {43                 Book bookEntity =newBook();44                 System.out.println("======开始解析第" + (bookList.indexOf(book) + 1) + "书======");//indexOf()返回的是index的位置，是从0开始45//解析book的属性集合46                 List<Attribute> attrList = book.getAttributes();//适用于未知属性情况下47/*//知道节点下属性名称时，获取节点值48book.getAttributeValue("id");*/49//遍历attrList(针对不清楚book节点下属性的名字及数量)50for(Attribute attr : attrList) {51/**注：JDom中，Attribute的getName和getValue方法获取到的都是实际的名称和值，52* 不会出现SAX和DOM中的空格和换行的情况*/53//获取属性名54                     String attrName =attr.getName();55//获取属性值56                     String attrValue =attr.getValue();57                     System.out.println("属性名：" + attrName + "----属性值：" +attrValue);58if (attrName.equals("id")) {59bookEntity.setId(attrValue);60}61}6263//❤对book节点的子节点的节点名以及节点值的遍历64                 List<Element> bookChilds =book.getChildren();65for(Element child : bookChilds) {66                     System.out.println("节点名：" + child.getName() + "----节点值：" +child.getValue());67if (child.getName().equals("name")) {68bookEntity.setName(child.getValue());69}70elseif (child.getName().equals("author")) {71bookEntity.setAuthor(child.getValue());72}73elseif (child.getName().equals("year")) {
 74                         bookEntity.setYear(child.getValue());
 75                        }
 76                        else if (child.getName().equals("price")) {
 77                         bookEntity.setPrice(child.getValue());
 78                        }
 79                        else if (child.getName().equals("language")) {
 80                         bookEntity.setLanguage(child.getValue());
 81                        }
 82 
 83                 }
 84                 System.out.println("======结束解析第" + (bookList.indexOf(book) + 1) + "书======");
 85                 booksList.add(bookEntity);
 86                 bookEntity = null;
 87                 //测试数据
 88                 System.out.println(booksList.size());
 89                 System.out.println(booksList.get(0).getId());
 90                 System.out.println(booksList.get(0).getName());
 91 
 92             }
 93         } catch (FileNotFoundException e) {
 94             e.printStackTrace();
 95         } catch (JDOMException e) {
 96             e.printStackTrace();
 97         } catch (IOException e) {
 98             e.printStackTrace();
 99         }
100     }
101 
102 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
注意：
❤关于 JDOM 使用过程中 JAR 包的引用 ：
方式1：通过右击项目-->build path-->add external archives...-->然后选择本地文件的jar包
这种方式并不能将jar包真正导入到项目源码中，当把项目导出放在另外的机器上，这个jar包并不会随着project一同被导出。
如图：
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712133811623-992374578.png)
方式2：项目根目录下新建lib文件夹——复制、粘帖jar包——右击jar包选择“build path构建路径”——“add to build path添加至构建路径”即可
如图：
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712133837217-1349992458.png)
**\# DOM4J 方式解析 XML**
**DOM4J**是第三方提供的解析XML方法，需要dom4j-1.6.1.jar包
示例：
![复制代码](https://common.cnblogs.com/images/copycode.gif)
1packagecom.study.dom4jtest;23importjava.io.File;4importjava.util.Iterator;5importjava.util.List;67importorg.dom4j.Attribute;8importorg.dom4j.Document;9importorg.dom4j.DocumentException;10importorg.dom4j.Element;11importorg.dom4j.io.SAXReader;1213/**14* DOM4J 方式解析XML15*/16publicclassDOM4JTest {1718publicstaticvoidmain(String[] args) {19//解析books.xml文件20//创建SAXReader的对象reader21         SAXReader reader =newSAXReader();22try{23//通过reader对象的read方法加载books.xml文件,获取docuemnt对象。24             Document document = reader.read(new File("books.xml"));25//通过document对象获取根节点bookstore26             Element bookStore =document.getRootElement();27//通过element对象的elementIterator方法获取迭代器28             Iterator it =bookStore.elementIterator();29//遍历迭代器，获取根节点中的信息（书籍）30while(it.hasNext()) {31              System.out.println("=====开始遍历某一本书=====");32              Element book =(Element) it.next();33//获取book的属性名以及 属性值34              List<Attribute> bookAttrs =book.attributes();35for(Attribute attr : bookAttrs) {36               System.out.println("属性名：" + attr.getName() + "--属性值：" +attr.getValue());37}38//解析子节点的信息39              Iterator itt =book.elementIterator();40while(itt.hasNext()) {41               Element bookChild =(Element) itt.next();42               System.out.println("节点名：" + bookChild.getName() + "--节点值：" +bookChild.getStringValue());43}44              System.out.println("=====结束遍历某一本书=====");45}4647         }catch(DocumentException e) {48e.printStackTrace();49}50}5152 }![复制代码](https://common.cnblogs.com/images/copycode.gif)
[回到顶部](http://www.cnblogs.com/Qian123/p/5231303.html#_labelTop)
## 五、四种解析方式比较分析
基础方法：DOM（平台无关的官方解析方式）、SAX（基于事件驱动的解析方式）
扩展方法：JDOM、DOM4J（在基础的方法上扩展出的，只有在java中能够使用的解析方法）
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712141035967-1697119233.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712141451076-471607087.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712141718654-1816134668.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712142205451-160856094.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712142519451-102082048.png)
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712142858889-1475226566.png)
**\#\#解析速度的分析**
![](https://images2015.cnblogs.com/blog/690102/201607/690102-20160712145709982-150819739.png)
XML四种解析方式性能测试：
SAX>DOM>DOM4J>JDOM
JUnit是Java提供的一种进行单元测试的自动化工具。测试方法可以写在任意类中的任意位置。使用JUnit可以没有main()入口进行测试。
DOM4J在灵活性和对复杂xml的支持上都要强于DOM
DOM4J的应用范围非常的广，例如在三大框架的Hibernate中是使用DOM4J的方式解析文件的。
DOM是w3c组织提供的一个官方解析方式，在一定程度上是有所应用的。
当XML文件比较大的时候，会发现DOM4J比较好用
1.JUnit：Java提供的单元测试；@Test注解；采用JUnit不需要程序入口main方法
2.性能测试结果：几kB的xml文件；建议使用DOM4J解析
DOM-33ms
SAX-6ms
JDOM-69ms
DOM4J-45ms
工程右键build path --Add library--JUnit单元测试 --version:JUnit4
DOM:33,SAX:6
JDOM:69;DOM4J:45
DOM 有可能溢出
多使用DOM4J





