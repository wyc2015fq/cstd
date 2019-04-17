# JSP XML 数据处理 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月09日 16:44:55[boonya](https://me.csdn.net/boonya)阅读数：353标签：[jsp																[xml																[数据](https://so.csdn.net/so/search/s.do?q=数据&t=blog)
个人分类：[Servlet/JSP](https://blog.csdn.net/boonya/article/category/2864195)





文章来自：[http://www.runoob.com/jsp/jsp-xml-data.html](http://www.runoob.com/jsp/jsp-xml-data.html)



当通过HTTP发送XML数据时，就有必要使用JSP来处理传入和流出的XML文档了，比如RSS文档。作为一个XML文档，它仅仅只是一堆文本而已，使用JSP创建XML文档并不比创建一个HTML文档难。

## 使用JSP发送XML

使用JSP发送XML内容就和发送HTML内容一样。唯一的不同就是您需要把页面的context属性设置为text/xml。要设置context属性，使用<%@page % >命令，就像这样：
<%@ page contentType="text/xml" %>
接下来这个例子向浏览器发送XML内容：
<%@ page contentType="text/xml" %>

<books><book><name>Padam History</name><author>ZARA</author><price>100</price></book></books>
使用不同的浏览器来访问这个例子，看看这个例子所呈现的文档树。

## 在JSP中处理XML

在使用JSP处理XML之前，您需要将与XML 和XPath相关的两个库文件放在<Tomcat Installation Directory>\lib目录下：
- XercesImpl.jar：在这下载[http://www.apache.org/dist/xerces/j/](http://www.apache.org/dist/xerces/j/)
- xalan.jar：在这下载[http://xml.apache.org/xalan-j/index.html](http://xml.apache.org/xalan-j/index.html)

books.xml文件:
<books><book><name>Padam History</name><author>ZARA</author><price>100</price></book><book><name>Great Mistry</name><author>NUHA</author><price>2000</price></book></books>
main.jsp文件：
<%@ page language="java" contentType="text/html; charset=UTF-8"
    pageEncoding="UTF-8"%>
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="x" uri="http://java.sun.com/jsp/jstl/xml" %>
 
<html><head><title>JSTL x:parse Tags</title></head><body><h3>Books Info:</h3><c:importvar="bookInfo"url="http://localhost:8080/books.xml"/><x:parsexml="${bookInfo}"var="output"/><b>The title of the first book is</b>: 
<x:outselect="$output/books/book[1]/name"/><br><b>The price of the second book</b>: 
<x:outselect="$output/books/book[2]/price"/></body></html>
访问http://localhost:8080/main.jsp，运行结果如下：
BOOKS INFO:The title of the first book is:PadamHistoryThe price of the second book:2000
## 使用JSP格式化XML

这个是XSLT样式表style.xsl文件：
<?xml version="1.0"?><xsl:stylesheetxmlns:xsl="http://www.w3.org/1999/XSL/Transform"version="1.0"><xsl:outputmethod="html"indent="yes"/><xsl:templatematch="/"><html><body><xsl:apply-templates/></body></html></xsl:template><xsl:templatematch="books"><tableborder="1"width="100%"><xsl:for-eachselect="book"><tr><td><i><xsl:value-ofselect="name"/></i></td><td><xsl:value-ofselect="author"/></td><td><xsl:value-ofselect="price"/></td></tr></xsl:for-each></table></xsl:template></xsl:stylesheet>
这个是main.jsp文件：
<%@ taglib prefix="c" uri="http://java.sun.com/jsp/jstl/core" %>
<%@ taglib prefix="x" uri="http://java.sun.com/jsp/jstl/xml" %>
 
<html><head><title>JSTL x:transform Tags</title></head><body><h3>Books Info:</h3><c:setvar="xmltext"><books><book><name>Padam History</name><author>ZARA</author><price>100</price></book><book><name>Great Mistry</name><author>NUHA</author><price>2000</price></book></books></c:set><c:importurl="http://localhost:8080/style.xsl"var="xslt"/><x:transformxml="${xmltext}"xslt="${xslt}"/></body></html>
运行结果如下：

![](http://www.runoob.com/wp-content/uploads/2014/01/xml-1.jpg)](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=jsp&t=blog)




