# XSL：转换从哪里开始? - ljx0305的专栏 - CSDN博客
2008年05月31日 22:28:00[ljx0305](https://me.csdn.net/ljx0305)阅读数：730标签：[xsl																[stylesheet																[xml																[语言																[xslt																[浏览器](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)](https://so.csdn.net/so/search/s.do?q=xslt&t=blog)](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=stylesheet&t=blog)](https://so.csdn.net/so/search/s.do?q=xsl&t=blog)
个人分类：[XML](https://blog.csdn.net/ljx0305/article/category/401859)
**XSL：转换从哪里开始?**
**前言**
爱好XML的人最终会试着将XML转换为HTML，或者转换为其他类型的文档，DOM/SAX显然不是专门为转换设计的，CSS对于转换也是力有不逮，所以XML的爱好者们几乎无一例外的要遭遇XSL，但是XSL似乎有非常多的用法，对于XML仅仅只是表示格式化的数据而言，XSL显得复杂且毫无头绪。
例如《跟我学XSL》和《XSL基础入门》这样的教程会带给你XSL的一些概念和例子，但是对于XSL的运行环境、平台特性和本质，似乎都语焉不详，你最终学会的仅仅是在XMLSPY或者IE中打开你的XML看看它转换后的效果罢了。一有人提到脚本语言或者JAVA中调用XSL你就头大了，甚至你不清楚XSL和XSLT究竟有什么区别。迷失在网络中的人们喜欢不停的用google搜索你想要的中文资料，但是其实有那个时间，干脆去那种技术的官方网站上好好看看吧。[http://www.w3.org/Style/XSL/](http://www.w3.org/Style/XSL/)是XSL技术的W3C的官方网站，在网页正文的第一行它就解释和XSL和XSLT的区别。原文如下：
**XSL** is a family of recommendations for defining XML document transformation and presentation. It consists of three parts:
[XSL Transformations](http://www.w3.org/TR/xslt) (XSLT) 
a language for transforming XML 
the [XML Path Language](http://www.w3.org/TR/xpath) (XPath) 
an expression language used by XSLT to access or refer to parts of an XML document. (XPath is also used by the [XML Linking](http://www.w3.org/TR/xlink/) specification) 
[XSL Formatting Objects](http://www.w3.org/TR/xsl) (XSL-FO) 
an XML vocabulary for specifying formatting semantics 
XSL是一组定义XML文档的转换和显示特征的推荐标准，它包括三个部分：XSL转换（XSLT）是一种为了转换XML而定义的语言；XML路径语言（XPath）是一种表达式语言，它被XSLT用来访问或者提交一个XML文档的某些部分（XPath也同时被XML Linking标准使用）；XSL格式化对象（XSL-FO）是一个XML词汇表用来定义XML的格式化语义。
**从何开始**
一般人学习XSL都是从XMLSPY等工具开始运行他的一个XSL例子，当然用文本编辑器编辑XML何XSL文件，用IE去打开XML也是一个好主意。因为XMLSPY和IE都有嵌入式的XSL解析器，例如IE的XSL解析器是MSXML，这样不用显式的调用XSL进行转换过程，只需要在XML文档的头部加上一句<?xml:stylesheet type="text/xsl" href="xxx.xsl"?>就可以让嵌入的XSL解析器自动的进行转换了。例如下面这个著名的例子，它包括cd_catalog.xml和cd_catalog.xsl文件，内容如下：
xml文件：
<?xml version="1.0" encoding="GB2312"?>
<?xml:stylesheet type="text/xsl" href="cd_catalog.xsl"?>
<CATALOG>
<CD>
<TITLE>Empire Burlesque</TITLE>
<ARTIST>Bob Dylan</ARTIST>
<COUNTRY>USA</COUNTRY>
<COMPANY>Columbia</COMPANY>
<PRICE>10.90</PRICE>
<YEAR>1985</YEAR>
</CD>
<CD>
<TITLE>喀什噶尔胡杨</TITLE>
<ARTIST>刀郎</ARTIST>
<COUNTRY>China</COUNTRY>
<COMPANY>先之唱片</COMPANY>
<PRICE>20.60</PRICE>
<YEAR>2004</YEAR>
</CD>
<CD>
<TITLE>敦煌(特别版)</TITLE>
<ARTIST>女子十二乐坊</ARTIST>
<COUNTRY>China</COUNTRY>
<COMPANY>百代唱片</COMPANY>
<PRICE>25.60</PRICE>
<YEAR>2005</YEAR>
</CD>
</CATALOG>
xsl文件：
<?xml version="1.0" encoding="GB2312"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns:fo="http://www.w3.org/1999/XSL/Format">
<xsl:template match="/">
<html>
<body>
<table border="2" bgcolor="yellow">
<tr>
<th>Title</th>
<th>Artist</th>
</tr>
<xsl:for-each select="CATALOG/CD">
<tr>
<td>
<xsl:value-of select="TITLE"/>
</td>
<td>
<xsl:value-of select="ARTIST"/>
</td>
</tr>
</xsl:for-each>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
将它们保存在同一目录下然后用IE5以上版本的IE直接打开xml文件，则会看到转换后的效果。当然用XMLSPY中自带的浏览器也可。
**用JScript显式调用XSL解析器**
上面的运行方法显然是“贪天之功”，利用了IE和XMLSPY自带的XSL解析器，是让一只看不见的手运行了转换过程。那么，也可以用Jscript语言显式的调用XSL解析器，让没有嵌入解析器的浏览器也可以运行XSL，当然，此浏览器必须支持Jscript脚本语言。我们还是使用上面的例子，不过将cd_catalog.xml中的<?xml:stylesheet type="text/xsl" href="cd_catalog.xsl"?>这一行去掉，同时新建一个cd_catalog.html文档，内容如下：
<html>
<body>
<script language="javascript">
    // Load XML
    var xml = new ActiveXObject("Microsoft.XMLDOM")
    xml.async = false
    xml.load("cd_catalog.xml")
    // Load the XSL
    var xsl = new ActiveXObject("Microsoft.XMLDOM")
    xsl.async = false
    xsl.load("cd_catalog.xsl")
    // Transform
    document.write(xml.transformNode(xsl))
</script>
</body>
</html>
将此html文档在支持Jscript的浏览器中打开，即可看到如前一段执行的结果。当然不仅仅是Jscript，其他的脚本语言如VBScript等等也可以，不过Jscript是XSL默认的脚本语言。
**脚本扩充的XSL，令人疑惑的xsl:eval标记**
    xsl:eval标记并不是一个标准的xsl标记，它属于[http://www.w3.org/TR/WD-xsl这个名字空间，这个名字空间最终被微软采用，于是xsl:eval也被微软用来调用Jscript脚本，以此来扩充XSL的功能。而标准的XSL1.0版本的名字空间是](http://www.w3.org/TR/WD-xsl)[http://www.w3.org/1999/XSL/Transform，它并不包含xsl:eval标记，这是很容易理解的，XSL应该属于一个平台无关的技术，如果它的某个标记要依赖微软公司的产品，那显然是自掘坟墓。关于平台无关的讨论，将在本文的最后展开。](http://www.w3.org/1999/XSL/Transform)
    xsl:eval标记的含义是计算其中脚本语言的表达式，并作为文本输出。下面的例子中计算了cd_catalog.xml中各种CD的总价格，修改上面的cd_catalog.xsl并另存为cd_catalog2.xsl文件如下：
<?xml version="1.0" encoding="GB2312"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/TR/WD-xsl">
<xsl:template match="/">
<html>
<body>
<table border="2" bgcolor="yellow">
<tr>
<th>Title</th>
<th>Artist</th>
</tr>
<xsl:for-each select="CATALOG/CD">
<tr>
<td>
<xsl:value-of select="TITLE"/>
</td>
<td>
<xsl:value-of select="ARTIST"/>
</td>
</tr>
</xsl:for-each>
<tr>
<td>合计</td>
<td>
<xsl:eval>total("PRICE")</xsl:eval>
</td>
<xsl:script>
                       function total(q){ 
                            temp=0; 
                            mark='/CATALOG/CD/'+q; 
                            v=selectNodes(mark); 
                            for(t=v.nextNode();t;t=v.nextNode()){ 
                                 temp+=Number(t.text);
                            } 
                            return temp;
                      } 
</xsl:script>
</tr>
</table>
</body>
</html>
</xsl:template>
</xsl:stylesheet>
在IE中打开cd_catalog.xml文件（注意修改xsl为cd_catalog2.xsl）即可看到结果，注意这个xsl文件的这一行<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/TR/WD-xsl">，写错了名字空间xsl:eval标记就会报错。
**浏览器无关的XSL解决方案，服务端的XSL**
不管如何折腾，要将XML通过XSL转换为HTML必须要求本地主机上有一个XSL解析器，不管是浏览器内嵌的，还是可以通过脚本语言调用。那么，更好的解决方案当然是从服务器端直接发送HTML回来，这样无论什么浏览器都可以看到转换的结果了。ASP提供了这个功能，这是可想而知的，不过我对ASP不熟，这段略过，有兴趣的可以找本ASP的XML教材看看。
**应用程序中的XSL，语言相关的XSL**
众所周知，Java是对XML技术支持得最好的语言，Java上面的xml包非常多，其中支持XSL转换的包最著名的有Saxon和xalan。Saxon包可以在[http://saxon.sourceforge.net/](http://saxon.sourceforge.net/)上面下载。将Saxon包解压缩到C:/saxon6_5_3，6.5.3版本提供了对XSL1.0最稳定的支持。然后在Classpath中加入C:/saxon6_5_3/saxon.jar;C:/saxon6_5_3/saxon-jdom.jar。
Saxon提供命令行式的XSL转换和API。其中命令行式的转换如下，将目录移动到存放xml（去掉xml的指定xsl的那一行）和xsl的目录，然后输入下面的命令：
java com.icl.saxon.StyleSheet cd_catalog.xml cd_catalog.xsl
就可以看到输出在屏幕上的结果，但是这样看起来不方便，所以输入如下命令：
java com.icl.saxon.StyleSheet cd_catalog.xml cd_catalog.xsl>a.html
然后将生成的a.html在浏览器中打开，可以清晰的看到结果。
下面是在Java程序中调用Saxon包，进行XSL转换的例子，文件名为XslExam.java：
import java.io.File;
import javax.xml.transform.Result;
import javax.xml.transform.Source;
import javax.xml.transform.Templates;
import javax.xml.transform.Transformer;
import javax.xml.transform.TransformerConfigurationException;
import javax.xml.transform.TransformerException;
import javax.xml.transform.sax.SAXSource;
import javax.xml.transform.stream.StreamResult;
import com.icl.saxon.ExtendedInputSource;
import com.icl.saxon.TransformerFactoryImpl;
public class XSLExam {
     public static void main(String[] args) {
        String sourceFileName = "cd_catalog.xml";
        String styleFileName = "cd_catalog.xsl";
        String outputFileName = "result.html";
        File sourceFile = null;
        File styleFile = null;
        File outputFile = null;
        TransformerFactoryImpl factory = new TransformerFactoryImpl();
        Source sourceInput = null;
        sourceFile = new File(sourceFileName);
        ExtendedInputSource eis = new ExtendedInputSource(sourceFile);
        sourceInput = new SAXSource(factory.getSourceParser(), eis);
        eis.setEstimatedLength((int)sourceFile.length());
        Source styleSource ;
        File sheetFile = new File(styleFileName);
        eis = new ExtendedInputSource(sheetFile);
        styleSource = new SAXSource(factory.getStyleParser(), eis);
        outputFile=new File(outputFileName);
            try {
                     Templates sheet = factory.newTemplates(styleSource);
                     Transformer instance = sheet.newTransformer();
               Result result = new StreamResult(outputFile);
               instance.transform(sourceInput, result);
              } catch (TransformerConfigurationException e) {
                     e.printStackTrace();
              }catch (TransformerException err) {
                     err.printStackTrace();
        }
       }
}
这个例子程序将cd_catalog.xml文件使用cd_catalog.xsl转换为result.html。在Eclipse3.01中调试通过（Saxon没有简单的xsl示例程序，我也是将com.icl.saxon.StyleSheet类拔光了才得到这个稍微简单的例子，如果需要更详细的用法，参考com.icl.saxon.StyleSheet类）。
**数据是独立的，处理是平台相关的**
总结前面的内容，可以看出XSL转换可以从这几个地方开始：
ØIE，XMLSPY:嵌入的解析器，例如MSXML3；
ØJScript，显式调用XSL解析器；
Ø用JScript扩充XSL功能，半吊子的XSL；
Ø浏览器无关的XSL解决方案，服务器端的XSL，ASP显式调用XSL；
Ø语言相关的XSL，Java的XSL包Saxon，xalan。
可以看出来，XSL无论如何，都是要平台相关的，第一种方法依赖嵌入浏览器的XSL解析器；第二、三种方法依赖操作系统安装的XSL解析器；第四种方法依赖服务器端安装的XSL解析器；最后的方法依赖JAVA语言提供的XSL API。其中微软还不顾W3C的反对，自定义了XSL的脚本扩充功能，功能倒是强大了，可惜脱离了Windows就玩不转了。JAVA号称平台无关，可是JAVA本身就是一个平台，要是有人的机器没有JRE又怎么办呢？丢弃XSL？
不过事物总是有因果的，其实XML作为数据的存储载体，可以做到完全的平台无关，但是XSL作为一个可执行的语言，一定要依赖某种已存在的运行环境的，就如同数据库中的表格和SQL语言一样。SQL号称适用于任何关系数据库，但是实际上还是需要一个环境来run的。那么XSL是否破坏了XML的平台无关性呢？我认为没有，因为XSL本身是一个XML文档，XML文档可以平台无关的保存和传输，至于使用何种方法来调用它则是另外考虑的问题。再者，XSL的源和目标都是平台无关的文档（例如XML和HTML），而它自己的调用方式则是可替换的，这点也减轻了XSL的负罪感吧。
以上的讨论都是基于XSL1.0标准的，目前XSL2.0标准尚在讨论中，不过初稿已经发布了，而Saxon8.0以上的版本号称已经支持了XSL2.0。让我们拭目以待XSL2.0带给我们的惊喜。
**参考文献**
*W3C**站点：[http://www.w3.org/Style/XSL/](http://www.w3.org/Style/XSL/)*
*XSL**主题：[http://www-900.ibm.com/developerWorks/cn/xml/theme/x-xsl.shtml](http://www-900.ibm.com/developerWorks/cn/xml/theme/x-xsl.shtml)*
*中文译文站点：[http://www.opendl.com/](http://www.opendl.com/)*
*XSLT**是什么类型的语言，SAXON**的作者谈XSL**：[http://www-900.ibm.com/developerWorks/cn/xml/x-xslt/index.shtml](http://www-900.ibm.com/developerWorks/cn/xml/x-xslt/index.shtml)*
转自:http://www.blogjava.net/wxb_nudt/archive/2005/03/08/1819.html
