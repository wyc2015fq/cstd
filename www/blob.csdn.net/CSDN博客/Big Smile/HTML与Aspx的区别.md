# HTML与Aspx的区别 - Big Smile - CSDN博客
2016年04月02日 10:41:57[王啸tr1912](https://me.csdn.net/tr1912)阅读数：1044
        敲牛腩的时候，发现了这样的一个问题，就是牛腩老师在视频中敲报错的窗体的时候，用的是HTML的页面，而在敲别的页面的时候，用的都是Aspx的web窗体页面，最开始的时候没有注意到这一点，导致了一些小的错误，但是效果还是可以出来的，但是这是为什么呢？于是有了下面的文章。
# 一、静态页面和动态页面
        说到HTML和Aspx的区别，也就谈到了静态页面和动态页面的区别，静态和动态在这个层面上指的就是数据的动态与静态。简单的来说就是页面上显示的东西是固定不变的（不需要从数据库中获得的）就是静态的页面，需要从别的程序或数据库中获得数据并处理的就是动态的界面。但是在Aspx没有写后面代码的时候也可以称一个Aspx的页面为静态页面。
        回过头来我们看定义：
        HTML(HyperText Markup Language)：超文本标记语言，标准通用标记语言下的一个应用。超文本标记语言的结构包括“头”部分（英语：Head）、和“主体”部分（英语：Body），其中“头”部提供关于网页的信息，“主体”部分提供网页的具体内容。
        Aspx：是微软的在服务器端运行的动态网页文件，通过IIS解析执行后可以得到动态页面，是微软推出的一种新的网络编程方法，而不是asp的简单升级，因为它的编程方法和asp有很大的不同，他是在服务器端靠服务器编译执行的程序代码，
        在某种程度上也可以说Aspx包含了HTML的东西，HTML是Aspx的一部分，只不过细节上有些许的不同。
# 二、vs中界面的不同
**1、默认代码的不同：**
        HTML网页新建立出来的代码是：
```
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
</body>
</htm
```
        而Aspx网页建立出来的代码是：
```
<%@ Page Language="C#" AutoEventWireup="true" CodeFile="Default2.aspx.cs" Inherits="Default2" %>
<!DOCTYPE html>
<html xmlns="http://www.w3.org/1999/xhtml">
<head runat="server">
<meta http-equiv="Content-Type" content="text/html; charset=utf-8"/>
    <title></title>
</head>
<body>
    <form id="form1" runat="server">
    <div>
    
    </div>
    </form>
</body>
</html>
```
从代码上可以清晰的看出来Aspx包含了HTML的东西，但是还有不同的地方。
** 2、可编程界面的不同**
        HTML的编程界面只有现有的HTML语言的编程界面
![](https://img-blog.csdn.net/20160402102028779?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)比较单一。
        而Aspx的页面有很多可编辑的地方也有很多可以查看的地方：
![](https://img-blog.csdn.net/20160402102037623?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
** 3、使用JavaScript的时候也不同**
     在HTML中使用JavaScript的时候是可以用两种方法来声明的（language 和 type都可以）：
`<script language="javascript" type="text/javascript"></script>`     而在Aspx中就会出错：
![](https://img-blog.csdn.net/20160402102442093?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
     所以在Aspx中只能够使用type这一种声明JavaScript的方法了。
# 三、总结
        经历了牛腩的学习，才发现这些东西的最根本的区别，静态和动态，网页类似于窗体，有了表象的控件和界面还得有后台代码的支持才能够运行，不过B/S的学习不止于此，看到了HTML语言的强大才知道了B/S和C/S的区别也不只是网页和应用程序的区别了。这真的是很重要。
