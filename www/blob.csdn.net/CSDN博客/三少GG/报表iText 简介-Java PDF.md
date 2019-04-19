# 报表iText 简介-Java PDF - 三少GG - CSDN博客
2010年06月25日 03:41:00[三少GG](https://me.csdn.net/scut1135)阅读数：1383
[](http://blog.csdn.net/xiaojunjava/archive/2007/12/17/1943768.aspx)
一、前言 
　　在企业的信息系统中，报表处理一直占比较重要的作用，**本文将介绍一种生成PDF报表的Java组件--iText**。通过在服务器端使用Jsp或JavaBean生成PDF报表，客户端采用超级连接显示或下载得到生成的报表，这样就很好的解决了B/S系统的报表处理问题。 
　　二、iText简介 
　　iText是著名的开放源码的站点sourceforge一个项目，是用于生成PDF文档的一个java类库。通过iText不仅可以生成PDF或rtf的文档，而且可以将XML、Html文件转化为PDF文件。 
　　iText的安装非常方便，在[http://www.lowagie.com/iText/download.html](http://www.lowagie.com/iText/download.html) - download 网站上下载iText.jar文件后，只需要在系统的CLASSPATH中加入iText.jar的路径，在程序中就可以使用iText类库了。 
　　三、建立第一个PDF文档 
　　用iText生成PDF文档需要5个步骤： 
　　①建立com.lowagie.text.Document对象的实例。 
Document document = new Document();  
　　②建立一个书写器(Writer)与document对象关联，通过书写器(Writer)可以将文档写入到磁盘中。 
PDFWriter.getInstance(document, new FileOutputStream("Helloworld.PDF"));  
　　③打开文档。 
document.open();  
　　④向文档中添加内容。 
document.add(new Paragraph("Hello World"));  
　　⑤关闭文档。 
document.close();  
　　通过上面的5个步骤，就能产生一个Helloworld.PDF的文件，文件内容为"Hello World"。 
　　建立com.lowagie.text.Document对象的实例 
　　com.lowagie.text.Document对象的构建函数有三个，分别是： 
public Document(); 
public Document(Rectangle pageSize); 
public Document(Rectangle pageSize, 
int marginLeft, 
int marginRight, 
int marginTop, 
int marginBottom);  
　　构建函数的参数pageSize是文档页面的大小，对于第一个构建函数，页面的大小为A4，同Document(PageSize.A4)的效果一样；对于第三个构建函数，参数marginLeft、marginRight、marginTop、marginBottom分别为左、右、上、下的页边距。 
　　通过参数pageSize可以设定页面大小、面背景色、以及页面横向/纵向等属性。iText定义了A0-A10、AL、LETTER、HALFLETTER、_11x17、LEDGER、NOTE、B0-B5、ARCH_A-ARCH_E、FLSA 和FLSE等纸张类型，也可以通过Rectangle pageSize = new Rectangle(144, 720);自定义纸张。通过Rectangle方法rotate()可以将页面设置成横向。 
　　书写器（Writer）对象 
　　一旦文档(document)对象建立好之后，需要建立一个或多个书写器(Writer)对象与之关联。通过书写器(Writer)对象可以将具体文档存盘成需要的格式，如com.lowagie.text.PDF.PDFWriter可以将文档存成PDF文件，com.lowagie.text.html.HtmlWriter可以将文档存成html文件。 
　　设定文档属性 
　　在文档打开之前，可以设定文档的标题、主题、作者、关键字、装订方式、创建者、生产者、创建日期等属性，调用的方法分别是： 
public boolean addTitle(String title) 
public boolean addSubject(String subject) 
public boolean addKeywords(String keywords) 
public boolean addAuthor(String author) 
public boolean addCreator(String creator) 
public boolean addProducer() 
public boolean addCreationDate() 
public boolean addHeader(String name, String content)  
　　其中方法addHeader对于PDF文档无效，addHeader仅对html文档有效，用于添加文档的头信息。 
当新的页面产生之前，可以设定页面的大小、书签、脚注（HeaderFooter）等信息，调用的方法是： 
public boolean setPageSize(Rectangle pageSize) 
public boolean add(Watermark watermark) 
public void removeWatermark() 
public void setHeader(HeaderFooter header) 
public void resetHeader() 
public void setFooter(HeaderFooter footer) 
public void resetFooter() 
public void resetPageCount() 
public void setPageCount(int pageN)   
　　如果要设定第一页的页面属性，这些方法必须在文档打开之前调用。 
　　对于PDF文档，iText还提供了文档的显示属性，通过调用书写器的setViewerPreferences方法可以控制文档打开时Acrobat Reader的显示属性，如是否单页显示、是否全屏显示、是否隐藏状态条等属性。 
　　另外，iText也提供了对PDF文件的安全保护，通过书写器（Writer）的setEncryption方法，可以设定文档的用户口令、只读、可打印等属性。 
　　添加文档内容 
　　所有向文档添加的内容都是以对象为单位的，如Phrase、Paragraph、Table、Graphic对象等。比较常用的是段落(Paragraph)对象，用于向文档中添加一段文字。 
四、文本处理 
　　iText中用文本块(Chunk)、短语(Phrase)和段落(paragraph)处理文本。 
文本块(Chunk)是处理文本的最小单位，有一串带格式（包括字体、颜色、大小）的字符串组成。如以下代码就是产生一个字体为HELVETICA、大小为10、带下划线的字符串： 
Chunk chunk1 = new Chunk("This text is underlined", FontFactory.getFont(FontFactory.HELVETICA, 12, Font.UNDERLINE));   
　　短语(Phrase)由一个或多个文本块(Chunk)组成，短语(Phrase)也可以设定字体，但对于其中以设定过字体的文本块(Chunk)无效。通过短语(Phrase)成员函数add可以将一个文本块(Chunk)加到短语(Phrase)中，如：phrase6.add(chunk); 
　　段落(paragraph)由一个或多个文本块(Chunk)或短语(Phrase)组成，相当于WORD文档中的段落概念，同样可以设定段落的字体大小、颜色等属性。另外也可以设定段落的首行缩进、对齐方式（左对齐、右对齐、居中对齐）。通过函数setAlignment可以设定段落的对齐方式，setAlignment的参数1为居中对齐、2为右对齐、3为左对齐，默认为左对齐。 
　　五、表格处理 
　　iText中处理表格的类为：com.lowagie.text.Table和com.lowagie.text.PDF.PDFPTable，对于比较简单的表格处理可以用com.lowagie.text.Table，但是如果要处理复杂的表格，这就需要com.lowagie.text.PDF.PDFPTable进行处理。这里就类com.lowagie.text.Table进行说明。 
　　类com.lowagie.text.Table的构造函数有三个： 
　　　①Table (int columns) 
　　　②Table(int columns, int rows) 
　　　③Table(Properties attributes) 
　　参数columns、rows、attributes分别为表格的列数、行数、表格属性。创建表格时必须指定表格的列数，而对于行数可以不用指定。 
　　建立表格之后，可以设定表格的属性，如：边框宽度、边框颜色、衬距（padding space 即单元格之间的间距）大小等属性。下面通过一个简单的例子说明如何使用表格，代码如下： 
1:Table table = new Table(3); 
2:table.setBorderWidth(1); 
3:table.setBorderColor(new Color(0, 0, 255)); 
4:table.setPadding(5); 
5:table.setSpacing(5); 
6:Cell cell = new Cell("header"); 
7:cell.setHeader(true); 
8:cell.setColspan(3); 
9:table.addCell(cell); 
10:table.endHeaders(); 
11:cell = new Cell("example cell with colspan 1 and rowspan 2"); 
12:cell.setRowspan(2); 
13:cell.setBorderColor(new Color(255, 0, 0)); 
14:table.addCell(cell); 
15:table.addCell("1.1"); 
16:table.addCell("2.1"); 
17:table.addCell("1.2"); 
18:table.addCell("2.2"); 
19:table.addCell("cell test1"); 
20:cell = new Cell("big cell"); 
21:cell.setRowspan(2); 
22:cell.setColspan(2); 
23:table.addCell(cell); 
24:table.addCell("cell test2");  
　　运行结果如下： 
header   
example cell with colspan 1 and rowspan 2  1.1 2.1   
1.2 2.2   
cell test1 big cell   
cell test2   
　　代码1-5行用于新建一个表格，如代码所示，建立了一个列数为3的表格，并将边框宽度设为1，颜色为蓝色，衬距为5。 
　　代码6-10行用于设定表格的表头，第7行cell.setHeader(true);是将该单元格作为表头信息显示；第8行cell.setColspan(3);指定了该单元格占3列；为表格添加表头信息时，要注意的是一旦表头信息添加完了之后，必须调用endHeaders()方法，如第10行，否则当表格跨页后，表头信息不会再显示。 
　　代码11-14行是向表格中添加一个宽度占一列，长度占二行的单元格。 
　　往表格中添加单元格(cell)时，按自左向右、从上而下的次序添加。如执行完11行代码后，表格的右下方出现2行2列的空白，这是再往表格添加单元格时，先填满这个空白，然后再另起一行，15-24行代码说明了这种添加顺序。 
　　六、图像处理 
　　iText中处理表格的类为com.lowagie.text.Image，目前iText支持的图像格式有：GIF, Jpeg, PNG, wmf等格式，对于不同的图像格式，iText用同样的构造函数自动识别图像格式。通过下面的代码分别获得gif、jpg、png图像的实例。 
Image gif = Image.getInstance("vonnegut.gif"); 
Image jpeg = Image.getInstance("myKids.jpg"); 
Image png = Image.getInstance("hitchcock.png");  
　　图像的位置 
　　图像的位置主要是指图像在文档中的对齐方式、图像和文本的位置关系。IText中通过函数public void setAlignment(int alignment)进行处理，参数alignment为Image.RIGHT、Image.MIDDLE、Image.LEFT分别指右对齐、居中、左对齐；当参数alignment为Image.TEXTWRAP、Image.UNDERLYING分别指文字绕图形显示、图形作为文字的背景显示。这两种参数可以结合以达到预期的效果，如setAlignment(Image.RIGHT|Image.TEXTWRAP)显示的效果为图像右对齐，文字围绕图像显示。 
　　图像的尺寸和旋转 
　　如果图像在文档中不按原尺寸显示，可以通过下面的函数进行设定： 
public void scaleAbsolute(int newWidth, int newHeight) 
public void scalePercent(int percent) 
public void scalePercent(int percentX, int percentY)  
　　函数public void scaleAbsolute(int newWidth, int newHeight)直接设定显示尺寸；函数public void scalePercent(int percent)设定显示比例，如scalePercent(50)表示显示的大小为原尺寸的50%；而函数scalePercent(int percentX, int percentY)则图像高宽的显示比例。 
　　如果图像需要旋转一定角度之后在文档中显示，可以通过函数public void setRotation(double r)设定，参数r为弧度，如果旋转角度为30度，则参数r= Math.PI / 6。 
　　七、中文处理 
　　默认的iText字体设置不支持中文字体，需要下载远东字体包iTextAsian.jar，否则不能往PDF文档中输出中文字体。通过下面的代码就可以在文档中使用中文了： 
BaseFont bfChinese = BaseFont.createFont("STSong-Light", "UniGB-UCS2-H", BaseFont.NOT_EMBEDDED); 
com.lowagie.text.Font FontChinese = new com.lowagie.text.Font(bfChinese, 12, com.lowagie.text.Font.NORMAL); 
Paragraph pragraph=new Paragraph("你好", FontChinese);   
　　八、后计 
　　iText还有很多高级的功能，这里就不一一介绍了，具体开发时可参考发布的文档。总的来说，iText是一套java环境下不错的制作PDF的组件。因为iText支持jsp/javabean下的开发，这使得B/S应用中的报表问题能得到很好的解决。由于iText毕竟不是专门为制作报表设计，所有报表中的内容、格式都需要通过写代码实现，相对于那些专业的支持可视化设计的报表软件来说，编程的工作量就有一定程度的增加。 
**学习资料：**
1.[http://blog.csdn.net/xiaojunjava/archive/2007/12/17/1943768.aspx](http://blog.csdn.net/xiaojunjava/archive/2007/12/17/1943768.aspx)
2. [http://wenku.baidu.com/view/9d9f834bcf84b9d528ea7a82.html](http://wenku.baidu.com/view/9d9f834bcf84b9d528ea7a82.html)
3. [http://www.360doc.com/content/06/0725/15/10365_164181.shtml](http://www.360doc.com/content/06/0725/15/10365_164181.shtml)
4. [http://moon-vv.javaeye.com/blog/224720](http://moon-vv.javaeye.com/blog/224720)
### [Itext笔记1](http://writeblog.csdn.net/blog/224720)
关键字: itext pdf 
### 在Web应用中动态创建PDF文件
原文：Sean C. Sullivan    翻译：gagaghost
  在一个最近的后勤项目中，客户要求我们建一个能让用户能从一个遗留系统查询出货信息的Web站点，有三个主要的需求：
  1.出货信息必须以PDF文档的格式返回；
  2.PDF文件必须能通过浏览器下载；
  3.PDF文件必须能用Adobe Acrobat Reader阅读；
尽管我们的团队有很多J2EE Web应用的开发经验，但在PDF文档处理上却没有多少经验。我们需要找一个能在服务器端Web应用里产生复杂的PDF文档的纯Java类库。最后，我们发现iText([http://www.lowagie.com/iText/](http://www.lowagie.com/iText/) )能完全满足我们的需要。
#### 1.iText类库
iText是一个创建和处理PDF文档的开源纯Java类库。Bruno Lowagie和Paulo Soares领导着这个项目。IText API能让Java开发人员以编程的方式创建PDF文档。iText提供了很多的特性：
支持PDF和FDF文档
各种页面尺寸
横向和竖向布局
页边距
表格
断字
页头
页脚
页码
条形码
字体
颜色
文档加密
JPEG,GIF,PNG和WMF图片
有序和无序列表
阴影
水印
文档模板
  iText是一个开源库。在写本文的时候，iText可以在两个许可协议下使用：Mozilla Public License和LGPL。如果想了解详细信息，请参考iText站点。在本文中，你将会看到iText API的应用。我们将阐述如何在服务器端应用中使用iText和servlet动态生成PDF文档。
#### 2.开始(Getting Started)
首先，你需要一个iText Jar文件。访问iText站点并下载最新的版本。在写本文时，最新的版本是使0.99。iText站点提供了API文档和一个全面的指南。
除了iText，我们还要用servlet.如果你不熟悉servlet，你可以通过Jason Hunter的书《Java Servlet Programming》来学习它。你需要一个J2EE应用服务器或可以独立运行的servlet引擎。开源软件 Tomcat，Jetty和Jboss是不错的选择。下文假设你使用的是Jakarta Tomcat 4.1。
##### 1.iText API
iText API简单易用。通过使用iText，你能创建自定义的PDF文档。iText库由下边的一些包组成：
com.lowagie.servlets
com.lowagie.text
com.lowagie.text.html
com.lowagie.text.markup
com.lowagie.text.pdf
com.lowagie.text.pdf.codec
com.lowagie.text.pdf.hyphenation
com.lowagie.text.pdf.wmf
com.lowagie.text.rtf
com.lowagie.text.xml
com.lowagie.tools
为了生成PDF文件，你只需要com.lowagie.text和com.lowagie.text.pdf两个包。
我们的例子使用了这些iText类：
com.lowagie.text.pdf.PdfWriter
com.lowagie.text.Document
com.lowagie.text.HeaderFooter
com.lowagie.text.Paragraph
com.lowagie.text.Phrase
com.lowagie.text.Table
com.lowagie.text.Cell
关键的类是Document和PdfWriter。在创建PDF文档时，你将经常使用这两个类。Document是PDF文档基于对象的描述。你可以通过调用Document类提供的方法往文档中加入内容。PdfWriter对象通过java.io.OutputStream对象与Document关联在一起。
#### 3.在Web应用中使用iText
在设计阶段，你必须决定如何使用iText。我们使用了下边的技术开发了我们的Web应用。
#### 1.A技术
在服务器文件系统上创建PDF文件。应用使用java.io.FileOutputStream把文件写到服务器文件系统上。用户通过HTTP GET方法下载该文件。
#### 2.B技术
使用java.io.ByteArrayOutputStream在内存中创建PDF文件。应用通过servlet的输出流将该PDF文件字节发送到客户端。
由于应用不需要把文件写到文件系统上，这样能保证在集群服务环境中能正常工作，所以我更倾向于使用B技术。如果你的应用运行在集群环境中且服务器集群不提供会话亲和的功能，A技术可能会导致失败。
#### 3.例子：PDFServlet
我们的例子应用由一个类组成：PDFServlet。这个servlet采用B技术。输出流 OutputStream是java.io.ByteArryOutputStream。用ByteArrayOutputStream，PDF文档字节将存储在内存中。当PDFServlet接收到一个HTTP请求时，它将动态地生成一个PDF文档并将该文档发送到客户端。
PDFServlet类扩展了javax.servlet.http.HttpServlet类并导入了两个iText包：com.lowagie.text和com.lowagie.text.pdf。
**doGet方法**
大多数servlet覆盖了doPost和doGet方法中的一个方法。我们的servlet没有什么不同。PDFServlet类覆盖了doGet方法。该servlet将在接收到HTTP GET请求后生成一个PDF文件。
在核心部分，servlet的doGet方法做了如下的工作：
1.创建一个包含PDF文档字节的ByteArrayOutputStream对象；
2.在reponse对象上设置HTTP响应头内容；
3.得到servlet输出流；
4.把文档字节写到servlet的输出流中；
5.刷新servlet输出流；
**generatePDFDocumentBytes方法**
generatePDFDocumentBytes 方法负责创建PDF文档。在这个方法中三个最重要的对象是Document对象，ByteArrayOutputStream对象和PdfWriter对象。PdfWriter使用ByteArrayOutputStream关联Document。
Document doc = new Document();
ByteArrayOutputStream baosPDF = new ByteArrayOutputStream();
PdfWriter docWriter = null;
docWriter = PdfWriter.getInstance(doc, baosPDF);
// ...
用add方法把内容添加到Document中。
doc.add(new Paragraph(
    "This document was created by a class named: "
    + this.getClass().getName()));
doc.add(new Paragraph(
    "This document was created on "
+ new java.util.Date()));
当你添加完内容后，要关闭Document和PdfWriter对象。
doc.close();
docWriter.close();
当关闭文档后，ByteArrayOutputStream对象返回到调用者。
return baosPDF;
ByteArrayOutputStream包含了PDF文档的所有字节。
**HTTP响应头**
在这个应用中，我们仅仅关注四个HTTP 响应头：Content-type,Content-disposition,Content-length,和Cache-control。如果你从没有使用过HTTP头，请参考HTTP 1.1规范。
研究在PDFServlet中的doGet方法，你会注意到要在任何数据写到servlet输出流之前设置HTTP响应头内容，这是很重要的，也是细微的一点。
让我们更详细地说明一下每个响应头的含义。
**Content-type**
在servlet中，HttpServletResponse有一个表明响应所包含内容类型的参数。对PDF文件而言，内容类型是application/pdf。如果servlet没有设置类型，web浏览器很难决定如何处理这个文件。
PDFServlet用下边的代码设置内容类型：
resp.setContentType("application/pdf");
Content-disposition
Content-disposition头提供给浏览器确定HTTP响应内容的信息。当浏览器读到这些头信息后，它能确定：
    
HTTP响应包含一个文件；
    
包含在响应中的文件名；
    
该文件是显示在浏览器主窗口中还是要用外部的应用查看；
RFC 2183中有对Content-disposition头完整的解释。
通过合适地设置Content-disposition的值，servlet能指示浏览器是“内嵌”显示文件还是把它当作附件处理。
例1.内嵌显示一个文件
Content-disposition: inline; filename=foobar.pdf
例2.往response里附加一个文件
Content-disposition: attachment; filename=foobar.pdf
下边的伪码说明了如何设置头信息：
public void doGet(HttpServletRequest req, HttpServletResponse resp)
{
    // ... 
    resp.setHeader(
        "Content-disposition",
        "inline; filename=foobar.pdf" );
    // ... 
}
Cache-Control 
根据你应用的特性不同，你可以让浏览器缓存或者不缓存你正在生成的PDF文件。服务器端应用可以有很多种HTTP 头来控制内容缓存。下边是一些例子：
  
Cache-Control: no-cache 
  
Cache-Control: no-store 
  
Cache-Control: must-revalidate 
  
Cache-Control: max-age=30 
  
Pragma: no-cache 
  
Expires: 0
关于Cache-Control头的全面解释见HTTP 1.1规范。
PDFServlet把Cache-Control设置为max-age=30。这个头信息告诉浏览器缓存这个文件的最长时间为30秒。
**Content-length**
Content-length头必须设置成PDF文件中字节的数值。如果Content-length没有设置正确，浏览器可能不能正确地显示该文件。下边是例子代码：
  ByteArrayOutputStream baos = getByteArrayOutputStream();
resp.setContentLength(baos.size());
**把PDF文档送到Web浏览器**
PDFServlet 通过把字节流写到servlet的输出流的方式把PDF文档送到客户端。它通过调用HttpServletResponse对象的 getOutputStream方法来获得输出流。getOutputStream方法返回一个 javax.servlet.ServletOutputStream类型的对象。
ServletOutputStream sos;
sos = resp.getOutputStream();
baos.writeTo(sos);
sos.flush();
在把所有的数据写到流之后，调用flush()方法把所有的字节发送到客户端。
**打包和部署**
为了在Tomcat中运行PDFServlet，你需要把应用打包在WAR文件中。iText JAR文件(itext-0.99.jar)必须放在WAR 文件的lib目录下边。如果你忘了把iText JAR文件打包进去，servlet会报一个 java.lang.NoClassDefFoundError的错误并停止运行。
**运行应用**
在WAR文件部署之后，你已经准备好了测试servlet了。Jakarta Tomcat在8080端口上监听请求。
在浏览器中请求[http://hostname:8080/pdfservlet/createpdf](http://hostname:8080/pdfservlet/createpdf) 。servlet将会执行并返回浏览器一个PDF文档。
#### 4.iText之外的方案
iText提供了许多产生PDF文档的底层API。然而，它不是对任何应用都有效。
在我的日常工作中，我结合Microsoft Word和Adobe Acrobat使用iText。首先，我们的团队使用Microsoft Word设计了一个出货表单。之后，我们用Acrobat把Word文档转换成PDF文档。然后，我们使用iText的模板的功能，我们把PDF文件装入到我们的应用中。从这里，把数据填入表格和输出最终的PDF文档是相当容易的。
对基于报表的Web应用，像JasperReports这样的工具,它提供了比iText更高层次的抽象。
#### 5.总结
当你的应用需要动态地创建PDF文档的时，iText类库是一个不错的方案。你可以通过增强和扩展本文中的代码来体验iText的能力。很快，由于提供了完善的PDF文档，你将会给你的同事和客户留下深刻的印象。
#### 6.其他资源
[http://www.lowagie.com/iText/](http://www.lowagie.com/iText/)
www.jpedal.org 
www.pdfbox.org 
xml.apache.org/fop 
HTTP 1.1 protocol specification 
RFC 2183 
dmoz.org/Computers/Data_Formats/Document/Publishing/PDF 
www.planetpdf.com 
[](http://blog.csdn.net/xiaojunjava/archive/2007/12/17/1943768.aspx)
