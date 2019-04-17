# JSP 国际化 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2016年08月09日 16:48:25[boonya](https://me.csdn.net/boonya)阅读数：350








文章来自：[http://www.runoob.com/jsp/jsp-internationalization.html](http://www.runoob.com/jsp/jsp-internationalization.html)



在开始前，需要解释几个重要的概念：
- 国际化（i18n）：表明一个页面根据访问者的语言或国家来呈现不同的翻译版本。
- 本地化（l10n）：向网站添加资源，以使它适应不同的地区和文化。比如网站的印度语版本。
- 区域：这是一个特定的区域或文化，通常认为是一个语言标志和国家标志通过下划线连接起来。比如"en_US"代表美国英语地区。

如果想要建立一个全球化的网站，就需要关心一系列项目。本章将会详细告诉您如何处理国际化问题，并给出了一些例子来加深理解。

JSP容器能够根据request的locale属性来提供正确地页面版本。接下来给出了如何通过request对象来获得Locale对象的语法：
java.util.Locale request.getLocale()
## 检测Locale

下表列举出了Locale对象中比较重要的方法，用于检测request对象的地区，语言，和区域。所有这些方法都会在浏览器中显示国家名称和语言名称：
|**序号**|**方法 & 描述**|
|----|----|
|1|**String getCountry()**返回国家/地区码的英文大写，或 ISO 3166 2-letter 格式的区域|
|2|**String getDisplayCountry()**返回要显示给用户的国家名称|
|3|**String getLanguage()**返回语言码的英文小写，或ISO 639 格式的区域|
|4|**String getDisplayLanguage()**返回要给用户看的语言名称|
|5|**String getISO3Country()**返回国家名称的3字母缩写|
|6|**String getISO3Language()**返回语言名称的3字母缩写|

## 实例演示

这个例子告诉我们如何在JSP中显示语言和国家：
<%@ page import="java.io.*,java.util.Locale" %>
<%@ page import="javax.servlet.*,javax.servlet.http.* "%>
<%//获取客户端本地化信息Locale locale = request.getLocale();String language = locale.getLanguage();String country = locale.getCountry();
%>
<html><head><title>Detecting Locale</title></head><body><center><h1>Detecting Locale</h1></center><palign="center"><%out.println("Language : "+ language  +"<br />");out.println("Country  : "+ country   +"<br />");
%>
</p></body></html>
## 语言设置

JSP可以使用西欧语言来输出一个页面，比如英语，西班牙语，德语，法语，意大利语等等。由此可见，设置Content-Language信息头来正确显示所有字符是很重要的。

第二点就是，需要使用HTML字符实体来显示特殊字符，比如"ñ" 代表的是"?"，"¡"代表的是 "?" ：
<%@ page import="java.io.*,java.util.Locale" %>
<%@ page import="javax.servlet.*,javax.servlet.http.* "%>
<%// Set response content type
    response.setContentType("text/html");// Set spanish language code.
    response.setHeader("Content-Language","es");String title ="En Espa?ol";

%>
<html><head><title><%out.print(title); %></title></head><body><center><h1><%out.print(title); %></h1></center><divalign="center"><p>En Espa?ol</p><p>?Hola Mundo!</p></div></body></html>
## 区域特定日期

可以使用java.text.DateFormat类和它的静态方法getDateTimeInstance()来格式化日期和时间。接下来的这个例子显示了如何根据指定的区域来格式化日期和时间：
<%@ page import="java.io.*,java.util.Locale" %>
<%@ page import="javax.servlet.*,javax.servlet.http.* "%>
<%@ page import="java.text.DateFormat,java.util.Date" %>

<%String title ="Locale Specific Dates";//Get the client's LocaleLocale locale = request.getLocale();String date =DateFormat.getDateTimeInstance(DateFormat.FULL,DateFormat.SHORT, 
                                  locale).format(newDate());
%>
<html><head><title><%out.print(title); %></title></head><body><center><h1><%out.print(title); %></h1></center><divalign="center"><p>Local Date: <%out.print(date); %></p></div></body></html>
## 区域特定货币

可以使用java.text.NumberFormat类和它的静态方法getCurrencyInstance()来格式化数字。比如在区域特定货币中的long型和double型。接下来的例子显示了如何根据指定的区域来格式化货币：
<%@ page import="java.io.*,java.util.Locale" %>
<%@ page import="javax.servlet.*,javax.servlet.http.* "%>
<%@ page import="java.text.NumberFormat,java.util.Date" %>

<%String title ="Locale Specific Currency";//Get the client's LocaleLocale locale = request.getLocale();NumberFormat nft =NumberFormat.getCurrencyInstance(locale);String formattedCurr = nft.format(1000000);
%>
<html><head><title><%out.print(title); %></title></head><body><center><h1><%out.print(title); %></h1></center><divalign="center"><p>Formatted Currency: <%out.print(formattedCurr); %></p></div></body></html>
## 区域特定百分比

可以使用java.text.NumberFormat类和它的静态方法getPercentInstance()来格式化百分比。接下来的例子告诉我们如何根据指定的区域来格式化百分比：
<%@ page import="java.io.*,java.util.Locale" %>
<%@ page import="javax.servlet.*,javax.servlet.http.* "%>
<%@ page import="java.text.NumberFormat,java.util.Date" %>

<%String title ="Locale Specific Percentage";//Get the client's LocaleLocale locale = request.getLocale();NumberFormat nft =NumberFormat.getPercentInstance(locale);String formattedPerc = nft.format(0.51);
%>
<html><head><title><%out.print(title); %></title></head><body><center><h1><%out.print(title); %></h1></center><divalign="center"><p>Formatted Percentage: <%out.print(formattedPerc); %></p></div></body></html>



