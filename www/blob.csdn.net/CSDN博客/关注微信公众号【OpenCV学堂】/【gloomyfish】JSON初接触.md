# 【gloomyfish】JSON初接触 - 关注微信公众号【OpenCV学堂】 - CSDN博客





2011年06月26日 20:32:00[gloomyfish](https://me.csdn.net/jia20003)阅读数：2698标签：[json																[javascript																[object																[java																[语言																[string](https://so.csdn.net/so/search/s.do?q=string&t=blog)
个人分类：[J2EE](https://blog.csdn.net/jia20003/article/category/871633)





Get to know JSON



一：什么是JSON

术语JSON源于三个英文单词（JavaScript
 Object Notation）的缩写。JSON是一种轻量级的数据交换格式，非常易于人们读写和计算机解析和生成。它是基于JavaScript编程语言子集而衍伸出来的文本数据格式（text-format）。是独立于语言实现的标准
 – RFC4627是JSON的官方标准文档。



二：JSON语法格式

一个标准的JSON文本是被串行化的对象或者数组JSON-text
 = object / array

数组开始标记= [ 左方括号

对象开始标记= { 左卷曲括号

数组结束标记 = ]
右方括号

对象结束标记 = }
右卷曲括号

名称分隔符 = :
冒号表示

值与值分隔符 = ,
逗号表示



值类型必须是支持的boolean, number, object, array其中的任意一种，而且值必须是小写字符，value
 = false / null / true / object / array / number / string



对于number类型的值，不支持八进制、十六进制和任何以零开头的值。



三：Java语言对JSON的支持

当前C, C++, C#, Java, JavaScript, Perl, Python已经引入了对JSON的支持，此外很多项目也已经完成对JSON的支持。Google浏览器的extension
 point技术就是JSON应用之一。以下Java语言中对JSON的操作是引用[www.json.org/java](http://www.json.org/java)的最小化完成。

在java中封装一个简单JSON对象代码如下：

//import org.json.simple.JSONObject;
JSONObject obj=newJSONObject();

  obj.put("name","foo");

  obj.put("num",newInteger(100));

  obj.put("balance",newDouble(1000.21));

  obj.put("is_vip",newBoolean(true));

StringWriterout=newStringWriter();

  obj.writeJSONString(out);
String jsonText
=out.toString();
System.out.print(jsonText);

输出结果为:{"balance":1000.21,"num":100,"is_vip":true,"name":"foo"}

如何将一个JSON对象的文本字符串转换为JSONObject：

String jsonText = "{test:001,author:fish,language:java}";

JSONObject testObj=**new** JSONObject(jsonText);

四：JSON在J2EE程序中的应用
- 如何在Sever端接收和发送JSON数据

在封装好JSON数据之后，把数据写入HTTP
 response body部分即可

response.reset(); 

response.setContentType("text/plain");

response.getWriter().print(jsonObject);

接收JSON数据，可以直接读取Request的content
 type 和 body部分信息。
- 如何在客户端发送和接收JSON数据

利用XMLHTTP对象实现JSON数据读写
- 在JavaScript中如何把一个JSON
字符串转化成一个JSON对象

接收到的JSON对象作为一个字符串，调用JavaScript的eval()方法可以把它转换成一个JSON对象，原因在于JSON是JavaScript语言的一部分eval()会调用JavaScript编译器来完成解析。

var jsonData = eval(’(’req.responseText’)’);

一旦对象被创建，你就可以通过.来访问它的属性

例如：jsonData.name[0];
- 如何把JSON对象变成一个JavaScript字符串

Var jsonText = JSON.stringify(jsonData);



五：一个很cool的例子

下面是基于Dojo的chart包利用来自Sever段的JSON数据刷新一个演示程序：

![json demo](http://hi.csdn.net/attachment/201106/26/0_130907551638O3.gif)



六：讨论

与传统的基于XML的数据交换相比，JSON显然更加轻便，省去了xml传输中一些不必要的冗余，使得网络带宽得到更有效的利用，对数据传输类型的支持更是xml数据方式不具备的。



七：参考引用

1.[http://code.google.com/p/json-simple/wiki/EncodingExamples](http://code.google.com/p/json-simple/wiki/EncodingExamples)

2.[www.json.org](http://www.json.org/)

3.RFC4627

4.http://dojotoolkit.org](https://so.csdn.net/so/search/s.do?q=语言&t=blog)](https://so.csdn.net/so/search/s.do?q=java&t=blog)](https://so.csdn.net/so/search/s.do?q=object&t=blog)](https://so.csdn.net/so/search/s.do?q=javascript&t=blog)](https://so.csdn.net/so/search/s.do?q=json&t=blog)




