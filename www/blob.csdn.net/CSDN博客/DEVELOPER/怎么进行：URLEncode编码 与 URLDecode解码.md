# 怎么进行：URLEncode编码 与 URLDecode解码 - DEVELOPER - CSDN博客





2014年04月07日 12:50:35[学术袁](https://me.csdn.net/u012827205)阅读数：52427标签：[URLEncode																[URLDecode																[编码																[解码](https://so.csdn.net/so/search/s.do?q=解码&t=blog)
个人分类：[URLEncode编码 与 URLDecode解码](https://blog.csdn.net/u012827205/article/category/2172843)





                
该方法要求你自己指定编码形式。这两个类都不用初始化：


　　public class URLDecoder extends Object


　　public class URLEncoder extends Object


String   text1  =   java.net.URLEncoder.encode("中国",   "utf-8");   
        String   text2   =   java.net.URLDecoder.decode(text1,   "utf-8");   

  这两条语句在同一个页面中的话,得到的结果是:   
  text1:   %E4%B8%AD%E5%9B%BD     
  tex2:   中国  

  String  China=new      String(request.getParameter("China").getBytes("iso8859_1"));   
  China=java.net.URLDecoder.decode(zhongguo,"utf-8");   

***                                                    一、URLEncoder***

***java***提供了一个***类URLEncoder***把string编码成这种形式。Java1.2增加了一个***类URLDecoder***它能以这种形式解码string。这个方法之前总是用它所在平台的默认编码形式，所以在不同系统上，它就会产生不同的结果。但是在java1.4中，这个方法被另一种方法取代了。
***特别需要注意***的是这个方法编码了符号，“\” ,“&”,“=”,和“：”，并把空格（“ ”）转换成了（+）。它不会尝试着去规定在一个URL中这些字符怎样被使用。由此，你不得不分块编码你的URL,而不是把整个URL一次传给这个方法。这是很重要的，因为对类URLEncoder最通常的用法就是查询string，为了和服务器端使用GET方式的程序进行交互。


例如，假设你想编码这个string：


　　pg=q&kl=XX&stype=stext&q=+"Java+I/O"&search.x=38&search.y=3


　　这段代码对其进行编码：


　　String ***query ***= 
java.net.URLEncoder.encode( "pg=q&kl=XX&stype=stext&q=+"Java+I/O"&search.x=38&search.y=3");


       System.out.println(query);


　　不幸的是，得到的输出是:


　　pg%3Dq%26kl%3DXX%26stype%3Dstext%26q%3D%2B%22Java%2BI%2FO%22%26search.x%3D38%26search.y%3D3


　　出现这个问题就是方法URLEncoder.encode( ) 在进行盲目地编码。它不能区分在URL或者查询string中被用到的特殊字符(像前面string中的“=”，和“&”)和确实需要被编码的字符。***所以URL需要一次只编码一块。***
***                                                         二、URLDecoder***

与URLEncoder 类相对应的URLDecoder 类有两种静态方法。它们解码以[***x-www-form-url-encoded***](http://blog.csdn.net/junhuahouse/article/details/23037541)这种形式编码的string。也就是说，它们把所有的加号(+)转换成空格符，把所有的%xx分别转换成与之相对应的字符：



```java
public static String decode(String s) throws Exception　　
    public static String decode(String s, String encoding) // Java 1.4 throws UnsupportedEncodingException
```
如果string包含了一个“%”，但紧跟其后的不是两位16进制的数或者被解码成非法序列，该方法就会抛出IllegalArgumentException 异常。当下次再出现这种情况时，它可能就不会被抛出了。这是与运行环境相关的，当检查到有非法序列时，抛不抛出IllegalArgumentException 异常，这时到底会发生什么是不确定的。在Sun's JDK 1.4中，不会抛出什么异常，它会把一些莫名其妙的字节加进不能被顺利编码的string中。这的确令人头疼，可能就是一个安全漏洞。




　　由于这个方法没有触及到非转义字符，所以你***可以把整个URL作为参数传给该方法<如下面的qerry>***。不用像之前那样分块进行，依然可以得到你想要的正确的解码结果。例如：

```java
String input = "http://www.altavista.com/cgi-bin/"+"qerry?pg=q&kl=XX&stype=stext&q=%2B%22Java+I%2FO%22&search.x=38&search.y=3";　　
try {　　
    String output = java.net.URLDecoder.decode(input, "UTF-8");　　
    System.out.println(output);　
　}
```](https://so.csdn.net/so/search/s.do?q=编码&t=blog)](https://so.csdn.net/so/search/s.do?q=URLDecode&t=blog)](https://so.csdn.net/so/search/s.do?q=URLEncode&t=blog)




