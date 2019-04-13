
# JSP网站开发之HTML入门知识及常用标记符 （一） - 杨秀璋的专栏 - CSDN博客

2016年07月28日 05:51:38[Eastmount](https://me.csdn.net/Eastmount)阅读数：7004所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)



最近发生的事情很多，其中一件很重要的事情就是：学生生涯的结束，教学生涯的开始。我准备下个月写一篇总结研究生生涯的文章，包括自己放弃互联网选择回家教书、找工作经历、项目和毕业设计的各种感想。很荣幸XB七月初就给了我第一次大学教学的经历，很享受很珍惜也很感触。尤其是看到学生“秀璋，你好！”的跑马灯欢迎界面、发表“终于会编程了，感觉自己好牛逼”的说说、递给我餐巾纸、和你们打球等等。
![](https://img-blog.csdn.net/20160728013832050)

言归正传，因为学生是大二升大三的，但是却没有学过网页相关知识，所以准备给《JSP网站开发》这门小学期时间课程总结些HTML入门知识，这对网站设计很有用的，希望对你们也有所帮助吧！文章中如果有不足之处，还请海涵~
目录：
一. JSP前端设计及HTML基础介绍
1.JSP基础知识
2.HTML基础介绍
3.查看网页源代码及审查元素
二. HTML常用标记符
1.HEAD标记符
2.HTML属性
3.BODY标记符及设置BODY背景图片颜色
4.注释
5.FONT标记符
6.B\I\U字体样式及物理元素与逻辑元素
7.特色字符<>引号等
三.HTML设置段落格式
1.分段标记符P
2.换行标记符BR
3.标题样式Hn
4.添加水平线HR
5.align属性设置对齐方式
6.列表知识
推荐大家通过W3C学习HTML知识，文章主要参考我上课内容及北理HTML课件。

## 一. JSP前端设计及HTML基础介绍
JSP网站开发推荐这六篇文章：[Java+MyEclipse+Tomcat系列](http://blog.csdn.net/Eastmount/article/category/3173529)
参考前文：
[Java+MyEclipse+Tomcat
 (一)配置过程及jsp网站开发入门](http://blog.csdn.net/eastmount/article/details/45492065)
[Java+MyEclipse+Tomcat
 (二)配置Servlet及简单实现表单提交](http://blog.csdn.net/eastmount/article/details/45536369)
[Java+MyEclipse+Tomcat
 (三)配置MySQL及查询数据显示在JSP网页中](http://blog.csdn.net/eastmount/article/details/45653615)
[Java+MyEclipse+Tomcat
 (四)Servlet提交表单和数据库操作](http://blog.csdn.net/eastmount/article/details/45725077)
[Java+MyEclipse+Tomcat
 (五)DAO和Java Bean实现数据库和界面分开操作](http://blog.csdn.net/eastmount/article/details/45833663)
[Java+MyEclipse+Tomcat (六)详解Servlet和DAO数据库增删改查操作](http://blog.csdn.net/eastmount/article/details/45936121)
免费资源下载地址：
[http://download.csdn.net/detail/eastmount/8733385](http://download.csdn.net/detail/eastmount/8733385)

### 1.JSP基础知识
静态网页：指网页一旦制作完成，不能随意更改，缺点是不能实现用户与服务器之间的交互，网页制作成本高，更改困难，如“hao123”。
动态网页：能根据用户的要求而动态改变的网页内容，包括JSP、ASP、PHP，如“12306”。
JSP：英文是Java Server Page，Java服务器页面。JSP技术有点类似ASP技术，它是在传统网页HTML文件（*.html, *.htm）中插入Java程序段（Scriptlet）和JSP标记（Tag），从而形成JSP文件（*.jsp）。
在配置MyEclipse和Tomcat后运行如下图所示，其中WebRoot文件夹下的index.jsp即为网站的前端页面设计。

![](https://img-blog.csdn.net/20160728020839296)
其中index.jsp的代码为：
```python
<%@ page language="java" import="java.util.*" pageEncoding="ISO-8859-1"%>
<%
String path = request.getContextPath();
String basePath = request.getScheme()+"://"+request.getServerName()+":"+request.getServerPort()+path+"/";
%>
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
  <head>
    <base href="<%=basePath%>">
    
    <title>My JSP 'index.jsp' starting page</title>
	<meta http-equiv="pragma" content="no-cache">
	<meta http-equiv="cache-control" content="no-cache">
	<meta http-equiv="expires" content="0">    
	<meta http-equiv="keywords" content="keyword1,keyword2,keyword3">
	<meta http-equiv="description" content="This is my page">
	<!--
	<link rel="stylesheet" type="text/css" href="styles.css">
	-->
  </head>
  
  <body>
    This is my JSP page. <br>
  </body>
</html>
```
这篇文章的重点部分是普及HTML的基础知识，并结合JSP开发进行普及介绍。

### 2.HTML基础知识
第一个问题你肯定会问“什么是HTML”？
HTML是描述网页的一种语言，一种规范，超文本标记语言（HyperText Markup Language），并不是一种编程语言。主要通过HTML标记标签（markup tag）来标记要显示的各个部分。
HTML标签主要包括两个特性：
（1）HTML是所有标记符都由尖括号包围起来，如<html>；
（2）标签成对出现，开始标签或开放标签（Opening tag）是由“尖括号+关键字”组成，如<table>；而结束标签或闭合标签（Closing tag）是由“<+斜杠/+关键字+>”组成，如</table>。
HTML文档或网页：由HTML标签和纯文本组成。Web浏览器读取HTML网页，通过网页中添加的标记符，可以告诉浏览器如何显示网页，即确定内容的格式。然后根据内容周围的HTML标记符解析并显示各种内容。
如何编写HTML网页呢？
可以通过Dreamweaver编辑HTML，但个人建议使用Notepad记事本、Nptepad++、TextEdit、UltraEdit、Sublime Text等工具来书写网站。
运行直接将网站命名为.html或.htm后缀的文件，通过浏览器打开即可。例如：
```python
<html>
  <head>   
    <title>HTML开发</title>
  </head>
  
  <body>
    欢迎学习Eastmount的HTML网站开发知识! <br>
  </body>
</html>
```
使用Sublime Text编辑及运行结果如下图所示，其所有内容放在<html></html>两个标记符之间。图中包含一个首部标记<head></head>，其中首部在JSP中通常可设置标题<title>和JavaScript；同时包含正文标记<body></body>可见网页内容，里面书写具体的网页内容。
![](https://img-blog.csdn.net/20160728024053405)

### 3.查看网页源代码及审查元素
通过查看网页源代码学习别人的代码也是非常重要的一个内容。右键浏览器，查看源代码即可。

![](https://img-blog.csdn.net/20160728040944053)

通过审查元素可以定位到具体的网页内容，在爬虫过程中很常见。定位具体的控件，然后右键审查元素。
![](https://img-blog.csdn.net/20160728041109431)
显示百度Logo图片结果如下图所示：

![](https://img-blog.csdn.net/20160728041211273)
注意：同样右键可以设置浏览器的编码方式，只要.html或.jsp与浏览器编码方式一致，就不会显示中文乱码，例如都为UTF-8。

## 二. HTML常用标记符
注意：HTML标记符是不区分大小写的，<html>和<HTML>没有区别，但是XHTML要求标记符区分大小写。希望大家养成使用小写的习惯，兼容性更好。

### 1.HEAD标记符
首部标记<head>和</head>位于Web页的开头，其中不包括Web页面的任何实际内容，而是提供一些与Web页有关的特定信息。首部标记内容通常包括：
（1）样式表CSS定义位于<style>和</style>之间；
（2）脚本定义位于<script>和</script>之间；
（3）标题标记<title>和</tilte>之间，用于定义网页标题。如：
![](https://img-blog.csdn.net/20160728030213679)
### 2.HTML属性
HTML中属性用于描述对象特征的特性。所有属性斗放置在开始标记符的尖括号内，多个属性使用空格分开，通常不区分大小写，以“名称-值”的形式出现。
例如：如<table name="t1" id="t2">。
属性值应该始终包括在引号内，常用双引号，某些情况下属性本省包含有双引号，必须使用单引号。例如在java中显示超链接。
常见属性：
--class: 规定元素类名
--id: 规定元素唯一id
--name: 规定元素名称
--style: 规定元素行内样式（inline style）
--title: 规定元素额外信息（工具中提示）
PS：会面讲到具体的标记符会详细介绍。


### 3.BODY标记符及设置BODY背景图片颜色
BODY正文标记符中的文字，如果没有其他标记符修饰，则为无格式形式。
注意：空格、回车这些格式控制在显示时都不起作用，如果要使用它们应使用预格式化元素<pre>和</pre>。
通常网页格式如下图所示，建议开始结束标签补齐再写具体内容。
![](https://img-blog.csdn.net/20160728030933901)

设置<body>标记符的背景颜色使用bgcolor属性可以设置，text用于设置正文的颜色，颜色出来使用名称进行赋值外，可以使用\#RRGGBB格式进行赋值。
--bgcolor: 设置背景颜色；
--text: 设置背景正文字体颜色；
--background: 设置背景图片；
eg: <body bgcolor="yellow" text="red">
![](https://img-blog.csdn.net/20160728032531884)

使用background设置背景图案。
eg: <body background="image.jpg">
![](https://img-blog.csdn.net/20160728033227974)
注意1：背景图片会重复显示，默认是如果图片不能沾满整个屏幕，就在x方向和y方向重复显示。可以通过background-repeat设置重复属性，包括四个值：repeat默认，x和y方向重复；repeat-x水平方向重复；repeat-y垂直方向重复；no-repeat背景图像仅显示一次。
设置代码：<body style="background-image:url('图片地址'); background-repeat:no-repeat;">
![](https://img-blog.csdn.net/20160728034924431)
注意2：在JSP中建议将图片拖动到WebRoot目录下，或建立一个images文件夹，拖动到该项目中，然后通过相对路径进行调用，而不是通过设置"C:\\xxx\\xxx.jpg"的方式（容易丢失路径）。
![](https://img-blog.csdn.net/20160728034431588)

### 4.注释
HTML中的注释开始标签是<!--，结束标签是-->。两个标记之间的内容为注释，不在浏览器中显示。
主要用于提高代码的可读性、易于理解。
例如：<!-- This is a comment -->

### 5.FONT标记符
设置字符格式，包括字体、字号、文字颜色等。
--size: 设置字体大小，值从1到7，默认是3，可使用+或-作为相对值；
--color: 设置字体颜色；
--face: 设置字体样式；
例如：设置字体大小和颜色。

```python
<HTML>
<HEAD><TITLE>FONT 标记符的 size color 属性示例</TITLE></HEAD>
<BODY>
<P>正常文本
<P><FONT size="7"> 这些是大字体的文本 </FONT>
<P><FONT size="1"> 这些是小字体的文本 </FONT>
<P><FONT size="+2">这些文字的字体比正常文本大 2 号</FONT>
<P><FONT size="-2">这些文字的字体比正常文本小 2 号</FONT>
<P><FONT color="red">这些文字是红色字体</FONT>
<P><FONT color="green">这些文字是绿色字体</FONT>
<P><FONT color="#334455">这些文字是#334455色字体</FONT>
</BODY>
</HTML>
```
显示效果如下图所示：

![](https://img-blog.csdn.net/20160728042927796)
例如：设置字体样式。
```python
<HTML>
<HEAD><TITLE>字体示例</TITLE></HEAD>
<BODY>
<DIV align="center">
  <P>以下是常用中文字体：</P>
  <FONT face="宋体">宋体</FONT><BR>
  <FONT face="楷体_GB2312">楷体</FONT><BR>
  <FONT face="黑体">黑体</FONT><BR>
  <FONT face="隶书">隶书</FONT><BR>
  <FONT face="幼圆">幼圆</FONT>
  <P>以下是常用英文字体：</P>
  <FONT face="Times New Roman"> Times New Roman </FONT><BR>
  <FONT face="Arial">Arial</FONT><BR>
  <FONT face="Arial Black">Arial Black</FONT><BR>
  <FONT face="Courtier New">Courtier New</FONT><BR>
  <FONT face="Comic Sans MS">Comic Sans MS</FONT><BR>
  <FONT face="Verdana">Verdana</FONT><BR>
</DIV>
</BODY>
</HTML>
```
显示效果如下图所示：
![](https://img-blog.csdn.net/20160728043105640)
注意：字体样式拼写需书写正确。

### 6.B\I\U字体样式及物理元素与逻辑元素
主要包括：
--<b> ... </b>文字加粗
--<strong> ... </strong> 文本加重语气
--<big>  ...   </big>  字体大一号
--<small>  ...  </small>  字体小号效果
--<em>   ...   </em>着重显示（emphasized）
--<i>   ...    </i>   字体斜体（italic）
--<sub>  ...  </sub>   定义下标，如H20 水
--<sup>   ...  </sup>  定义上标，如O2 氧气
--<u>   ...   </u>   下划线
--<s>   ...    </s>  删除文本
例如：

```python
<HTML>
<HEAD><TITLE>物理字符样式效果示例</TITLE></HEAD>
<BODY>
  <P><B>此处为粗体b显示文本</B></P>
  <P><STRONG>此为粗体strong显示</STRONG>
  <P><BIG>此处为大字big体文本</BIG></P>
  <P><SMALL>此处为小字small体文本</SMALL></P>
  <P><I>此处为斜体i文本</I></P>
  <P><EM>此为加重语气em斜体</EM></P>
  <P><TT>此处为等宽tt字体文本</TT></P>
  <P><U>此处为下划线u文本</U></P>
  <P><S>此处为使用<S>标记设置的删除线文本</S></P>
  <P><STRIKE>此处为使用<STRIKE>标记设置的删除线文本</STRIKE></P>
  <P>此处为上标示例：x<SUP>2</SUP> + y<SUP>2</SUP> = R<SUP>2</SUP></P>
  <P>此处为下标示例：H<SUB>2</SUB>SO<SUB>4</SUB></P> 
</BODY>
</HTML>
```
显示效果如下图所示：
![](https://img-blog.csdn.net/20160728045248490)
物理元素：加粗Bold，表示标记符本身说明了所修饰的效果。
逻辑元素：strong强调某段文字的消息，表示标记符说明所修饰效果的逻辑含义。

### 7.特色字符<>引号等
如果用户需要在网页中显示某些特色字符，例如<（小括号）、>（大括号）、”（引号）等，需要使用特殊字符来表示。
参考字符以&开始，以；结束。既可以使用数字代码，也可以使用名称代码。
例如：显示1<2    <p>1&lt;2</p>
HTML特殊字符编码对照表
参考：[http://www.jb51.net/onlineread/htmlchar.htm](http://www.jb51.net/onlineread/htmlchar.htm)


## 三. HTML设置段落格式

### 1.分段标记符P
分段标记符用于将文档划分为段落，标记为<p></p>。
其中结束标记符通常可以省略。

### 2.换行标记符BR
在HTML中换行通常使用<br />或<br>。它是在不产生一个新段落情况下的换行。单标签元素 />关闭。
注意：在HTML源码中设置多个空格或换行，所有的连续空格或换行被显示为一个空格，浏览器会忽略源码排版信息，移除多余空格与换行；而如果使用<br />标签可以实现多个换行，&nbsp;实现多个空格。

### 3.标题样式Hn
在HTML中，用户可以通过Hn标记符来识别文档中的标题和副标题，其中n是数字1到6；H1表示最大的标题，H6为最小标题，使用标题样式时，必须使用结束标记符。
注意：使用标题时应按照其逻辑含义，而不是按照显示效果。换句话就是你不能为了产生大点的字体就使用它们，设置字体请使用其他标签或CSS替代。

![](https://img-blog.csdn.net/20160728051952428)


### 4.添加水平线HR
它包括以下属性：
--size：设置水平线的粗细
--width：设置水平线长度
--noshade：设置水平线以实线显示
--color：设置水平线颜色
例如：

```python
<HTML>
  <HEAD>
    <TITLE>水平线效果</TITLE>
  </HEAD>
  <BODY>
  以下是默认水平线：<HR>
  以下是粗为 5 像素的水平线：<HR size="5">
  以下是长度为100像素的水平线：<HR width="100">
  以下是长度为屏幕宽度 50% 的水平线：<HR width="50%">
  以下是粗为 5 像素的实心水平线：<HR size="5" noshade="noshade">
  以下是红色的水平线：<HR color="red">
  </BODY>
</HTML>
```
显示效果如下图所示：
![](https://img-blog.csdn.net/20160728052406117)

### 5.align属性设置对齐方式
设置段落对齐方式，其值包括：
--right：右对齐
--left：左对齐
--center：居中对齐
--justify：两端对齐
注意：align属性可以用于多种标记符，最典型为P、Hn、HR、table、div等。在讲述table、div布局时会详细介绍。

### 6.HTML列表
（1）有序列表（Ordered List）
在表的各项前显示数字或字母的缩排列表。
有序列表标记符<ol></ol>，列表项标记符<li></li>（结束可省略</li>）。
<ol>
<li> List item 1
<li> List item 2
</ol>
注意：type属性设置样式，取值为1、A、a、I、i。

```python
<HTML>
<HEAD>
  <TITLE>有序列表示例</TITLE>
</HEAD>
<BODY>
一般的有序列表
<OL>
 <LI>列表项1 <LI>列表项2 <LI>列表项3
</OL>
用大写罗马字母表示的有序列表：
<OL type="I">
  <LI>列表项1 <LI>列表项2 <LI>列表项3
</OL>
</BODY>
</HTML>
```
显示效果如下图所示：
![](https://img-blog.csdn.net/20160728053608169)
（2）无序列表（Unordered List）
它是一种在表各项前显示特殊项目符号的缩排列表。
无序列表标记符<ul></ul>，列表项标记符<li></li>（结束可省略</li>）。
<ul>
<li> List item 1
<li> List item 2
</ul>
注意：type属性设置样式，取值为disc、circle、square。在IE中，type属性的取值是区分大小写的，通常使用小写。

```python
<HTML>
<HEAD><TITLE>无序列表示例</TITLE></HEAD>
<BODY>
默认无序列表：
<UL><LI>列表项1<LI>列表项2<LI>列表项3</UL>
使用方块作为列表项标记的无序列表：
<UL type="square">
<LI>列表项1<LI>列表项2<LI>列表项3
</UL>
</BODY>
</HTML>
```
显示效果如下图所示：
![](https://img-blog.csdn.net/20160728053941045)
（3）自定义列表
格式如下：
<dl>
<dt>   </dt>
<dd>  </dd>
<dt>   </dt>
<dd>  </dd>
</dl>

![](https://img-blog.csdn.net/20160728054610266)

最后希望文章对你有所帮助，主要讲述了HTML相关的基础知识，如果文章中存在错误或不足之处，还请海涵~文章中的部分例子是我讲课的内容，部分是我读书时的HTML网页内容。
（By:Eastmount 2016-7-28 清晨5点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）



