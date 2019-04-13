
# 网站开发之HTML基础知识及超链接（二） - 杨秀璋的专栏 - CSDN博客

2016年10月02日 19:56:59[Eastmount](https://me.csdn.net/Eastmount)阅读数：4086所属专栏：[HTML网站前端设计](https://blog.csdn.net/column/details/13444.html)



前面的文章我讲述了HTML的基础知识。包括：1.JSP前段设计及HTML基础介绍；2.HTML常用标记符：HEAD、HTML属性、BODY设置背景、注释、FONT、B\I\U；3.HTML设置段落格式：分段P、换行BR、标题样式Hn、HR、align对齐、列表等。
链接：[JSP网站开发之HTML入门知识及常用标记符 （一）](http://blog.csdn.net/eastmount/article/details/51971638)
这篇文章同样是对基础知识的介绍，主要包括几个内容：
1.基础知识回顾
2.超链接
前端基础性文章，希望对你有所帮助，尤其是刚学习网站的新手。

## 一. 基础知识回顾
先给大家简单回顾下前两节课关于HTML的基本布局，现存在如下布局。

```python
<html>
    <head>
         <title>第一个HTML代码</title>
    </head>
    <body>
         <div align="center">
            <img src="t0158cec82cc27c8eb1.jpg" height="300" width="600" /><br/>
         </div>
         <hr color="red" size="4" width="600">
         <marquee><h1>欢迎访问李福的主页！</h1></marquee><br/>
         <div align="center">
               <h1>诗歌赏析</h1>
               <hr color="red" size="4" width="600">
         </div>
         <div align="center">
         <p>
         <pre>
             <h1><font color="green" face="楷体" size="20">
                 李商隐无题</font></h1><br/>
             <h3> 
                 相见时难别亦难，东风无力百花残。<br/> 
                 春蚕到死丝方尽，蜡炬成灰泪始干。<br/> 
                 晓镜但愁云鬓改，夜吟应觉月光寒。<br/> 
                 蓬莱此去无多路，青鸟殷勤为探看。<br/>
             </h3>
             <hr color="red" size="4" width="600">
             版权所有:贵州省贵阳市花溪区花溪大学城贵州财经大学计算机信息学院信息管理与信息系统--李福
         </pre>
         </p>
         </div>
</body>
</html>
```
它显示的效果如下所示，这是一个学生的作品。虽然代码中还是存在部分问题，如Hn用于定于标题，不要用来设置字体大小，<pre>格式化输出不需要<br>等。

![](https://img-blog.csdn.net/20161001152136764)
涉及的知识包括HTML-HEAD-BODY的布局，img插入图片，Hn设置标题等。
![](https://img-blog.csdn.net/20161002154000040)
**1.BODY**
正文标记符<BODY>和</BODY>包含Web 页的内容。文字、图形、链接以及其他HTML元素都位于该标记符内。注意：空格、回车这些格式控制在显示时都不起作用，如要使它们起作用，应使用预格式化元素<PRE>和</PRE>。
插入背景图片：<BODY BACKGROUND="image.gif">
插入音乐head：<BGSOUND src="soundfileURL">
**2.字符格式 FONT**
所谓字符格式是指针对段落中的部分文字（一个或一组文字，也可以是整个段落的文字）所设置的格式，通常包括字体、字号、文字颜色。
FONT标记符具有3个常用的属性： SIZE 设置字体大小、COLOR 设置字体颜色、FACE设置字体样式。
**3.标题样式 Hn**
在HTML中，用户可以通过Hn标记符来标识文档中的标题和副标题，其中n是1至6的数字；H1表示最大的标题，H6表示最小的标题。使用标题样式必须使用结束标记符。
**4.添加水平线 HR**
添加水平线的标记符为HR，它包括以下属性： SIZE 属性用来改变水平线的粗细， WIDTH 属性用来更改水平线的长度，NOSHADE 属性使水平线以实线显示， COLOR属性可以控制水平线的颜色。
**5.ALIGN属性**
ALIGN 属性用于设置段落的对齐格式，其值包括：RIGHT（右对齐）、LEFT（左对齐）、CENTER（居中对齐）和 JUSTIFY（两端对齐）。ALIGN 属性可应用于多种标记符，最典型的是应用于 P、DIV、Hn（标题标记符）、HR 等。
**6.有序列表 OL LI**
有序列表（Ordered list）也称数字式列表，它是一种在表的各项前显示有数字或字母的缩排列表。 定义有序列表需要使用有序列表标记符<OL></OL>和列表项（List item）标记符<LI></<LI>（结束标记符可省略）。OL 标记符最常用的属性type，用来设置数字序列样式。取值为：1、A、a、I、i。
语法如下：
![](https://img-blog.csdn.net/20161002161942042)

**7****.无序列表 UL LI**
无序列表（Unordered list）也称强调式列表，它是一种在表的各项前显示有特殊项目符号的缩排列表。 定义无序列表需要使用无序列表标记符<UL></UL>和列表项（List item）标记符<LI></LI>（结束标记符可省略）。
语法如下：
![](https://img-blog.csdn.net/20161002162142340)
**8****.IMG 标记符**
- src 属性用于指定要插入的图像的文件名（包括路径）
- alt 属性表示图像的简单文本说明
- width 和 height 属性指定图像的显示大小
- border 属性指定图像的边框
- align属性设置对齐方式
- hspace属性设置水平方向的空白
- vspace属性设置垂直方向的空白

## 二. 超链接
HTML（Hypertext Markup Language）超文本标记语言，它是一种规范，一种标准，它通过标记符（tag）来标记要显示的网页的各个部分。前面讲述了ML标记语言部分，通常是一个网页，这部分会讲解超链接Hyperlink，通过超链接才能够实现跳转，构成完整的网页。
**1.URL**
URL（Universal Resources Locator）用于定位Web上的文档信息。 一个 URL 包括 3 部分：协议、计算机地址、文件路径。 协议：//计算机/文件路径
![](https://img-blog.csdn.net/20161002165756524)
使用相对URL时，经常使用两个与DOS类似的符号：句点（.）表示当前目录双重句点（..）表示当前目录的上一级目录。
![](https://img-blog.csdn.net/20161002170026106)
**2.超链接类型**
根据超链接的目标文件不同，分为：
- 网页之间的超链接
- 页面内的超链接
- 文件下载超链接
- Email超链接
- 空超链接
根据超链接源对象的不同，分为：
- 文本超链接
- 图像超链接（包括图像映射）
- 对象超链接（例如Flash、Java小程序）
根据超链接实现方式的不同，分为：
- HTML超链接
- JavaScript超链接
- 对象超链接
**3.创建超链接**
A标记符用于创建超链接（结束标记不能省略），href 属性指定超链接的目标文件。
![](https://img-blog.csdn.net/20161002172116634)

内部网页超链接：<a href=test.htm>link</a>
外部网页超链接：<a href=http://www.microsoft.com>link</a>
注意：<a></a>之间是具体需要跳转的内容（切勿丢失），设置超链接后会增加下划线变蓝。
![](https://img-blog.csdn.net/20161002172619062)

注意：test.htm文件需要和源文件放在同一目录下，同时如果想弹出新的网页，则添加target属性，如下：
<a href="http://www.csdn.net/" target="_blank">Visit CSDN</a>
详见：[http://www.w3school.com.cn/tags/tag_a.asp](http://www.w3school.com.cn/tags/tag_a.asp)
如果希望图片作为跳转目录，参考W3School例子：![](https://img-blog.csdn.net/20161002172904534)
**4.锚点链接**
使用页面内的超链接，首先需要定义锚点，然后在超链接中指向该锚点。
定义锚点应使用<a name=xxx></a>
指向锚点的超链接为：<a href=\#锚点名称>link</a>
指向其他页面内锚点的超链接：<a href = 页面的URL\#该文件中的锚点>link</a>
<a href= \# >blank link</a>

```python
<html>
<body>
<p>
<a href="#C6">查看 Chapter 6。</a>
</p>
<h2>Chapter 1</h2>
<p>This chapter explains ba bla bla</p>
<h2>Chapter 2</h2>
<p>This chapter explains ba bla bla</p>
<h2>Chapter 3</h2>
<p>This chapter explains ba bla bla</p>
<h2>Chapter 4</h2>
<p>This chapter explains ba bla bla</p>
<h2>Chapter 5</h2>
<p>This chapter explains ba bla bla</p>
<h2><a name="C6">Chapter 6</a></h2>
<p>This chapter explains ba bla bla</p>
<h2>Chapter 7</h2>
<p>This chapter explains ba bla bla</p>
<h2>Chapter 8</h2>
<p>This chapter explains ba bla bla</p>
</body>
</html>
```
运行结果如下图所示，点击超链接跳转到第6章节。
![](https://img-blog.csdn.net/20161002174946084)
百度百科的目录链接或CSDN的标题通常都是使用该方法实现。
![](https://img-blog.csdn.net/20161002195459999)

**5.超链接的显示效果**
在伪类和伪对象中，与超链接相关的四个伪类选择器应用比较广泛。
- a:link 定义超链接的默认样式
- a:visited 定义超链接被访问后的样式
- a:hover 定义鼠标经过超链接的样式
- a:active 定义超链接被激活时的样式，如鼠标单击之后，到鼠标被松开之间的这段时间的样式
超链接的四种状态样式的排列顺序是有要求的，一般不能随意调换。先后顺序应该是：link、visited、hover、active 超链接的四种状态并非都必须要定义，可以定义其中的两个或三个。
下划线是超链接的基本样式，但是很多网站并不喜欢使用，所以在建站之初，就彻底清除了所有超链接的下划线: a{text-decoration:none;}。
下面举个简单的CSS例子，后面会详细介绍CSS，也是难点。
```python
<html>
    <head>
         <style type="text/css"> 
         A { 
             FONT-SIZE: 16px; FONT-FAMILY: 宋体
         } 
         A:link { 
             COLOR: #0055bb; TEXT-DECORATION: none
         }
         A:visited { 
             COLOR: #0077bb; TEXT-DECORATION: none 
         } 
         A:hover { 
             COLOR: green; FONT-FAMILY: 楷体; FONT-SIZE: 20
         } 
         P {COLOR: red}  
         </style>
         <title>第一个HTML代码</title>
    </head>
   
    <body>
    <P>CSS设置超链接样式<BR /></P>
    <A href="http://blog.csdn.net/eastmount/" target="_blank"> Eastmount CSDN </A>
    </body>
</html>
```
运行结果如下图所示，没有下划线，同时悬停变化。
![](https://img-blog.csdn.net/20161002191604310)
**6.Email链接**
<a href=mailto:i@email.com>Email</a>
控制命令：
?subject= 设置信件主题
?cc= 设置抄送人
?bcc= 设置密件抄送人
& 组合多个控制命令
![](https://img-blog.csdn.net/20161002194414682)
下面是学生做的一些练习，还是挺不错的，毕竟才两节课内容，可惜无证驾驶！
![](https://img-blog.csdn.net/20161002195123131)
![](https://img-blog.csdn.net/20161002195154857)

最后希望文章对你有所帮助，主要讲述了HTML相关的基础知识及超链接的用法，如果文章中存在错误或不足之处，还请海涵~文章中的部分例子是我讲课的内容，部分是我读书时的HTML网页内容，下一篇将讲解Table和表单控件，今天加班，明天回家。
（By:Eastmount 2016-10-02 晚上8点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/)）


