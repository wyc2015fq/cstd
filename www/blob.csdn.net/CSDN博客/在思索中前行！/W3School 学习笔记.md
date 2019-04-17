# W3School 学习笔记 - 在思索中前行！ - CSDN博客





2014年07月08日 22:13:30[_Tham](https://me.csdn.net/txl16211)阅读数：2301








# 网站构建初级教程

1、每个网站开发人员都有必要了解以下几方面的知识：
- 万维网如何工作 
- HTML 语言 
- 如何使用层叠样式表 (CSS)
- JavaScript 编程
- XML 标准 
- 服务器脚本技术 
- 使用 SQL 来管理数据 

2、www

       WWW 指万维网（World Wide Web），常被称为Web。Web 信息存储于被称为网页的文档中 ，网页是存储于被称为 Web 服务器的计算机上 ，读取网页的计算机称为 Web 客户端，Web 客户端通过称为浏览器的程序来查看网页。所有的网页都含有其如何被显示的结构，浏览器通过阅读这些结构来显示页面 。

3、HTML

      HTML 指的是超文本标记语言 (**H**yper**T**ext**M**arkup**L**anguage) ，HTML 不是一种编程语言，而是一种*标记语言* (markup language) 。

      HTML 文档 = 网页，HTML 文档*描述网页* ，HTML 文档*包含HTML 标签*和纯文本 ，HTML 文档也被称为*网页*

      Web 浏览器的作用是读取 HTML 文档，并以网页的形式显示出它们。浏览器不会显示 HTML 标签，而是使用标签来解释页面的内容。

4、CSS

      CSS 指层叠样式表 (*C*ascading*S*tyle*S*heets) ，样式定义*如何显示* HTML 元素 。把样式添加到 HTML 4.0 中，是为了*解决内容与表现分离的问题*，样式表可以极大提高工作效率
 ，外部样式表通常存储在*CSS 文件*中 。

      样式表的优先级
- 浏览器缺省设置 
- 外部样式表 
- 内部样式表（位于 <head> 标签内部）
- 内联样式（在 HTML 元素内部）

因此，内联样式（在 HTML 元素内部）拥有最高的优先权，这意味着它将优先于以下的样式声明：<head> 标签中的样式声明，外部样式表中的样式声明，或者浏览器中的样式声明（缺省值）。

5、Javascript

      JavaScript 被设计用来向 HTML 页面添加交互行为。JavaScript 是一种脚本语言（脚本语言是一种轻量级的编程语言），且是一种解释性语言（执行无需编译），通常被直接嵌入 HTML 页面。

6、XML

      XML 指*可扩展标记语言*（EXtensible Markup Language），XML 是一种*标记语言*，很类似 HTML 。XML 被设计用来*描述数据* 。XML 标签没有被预定义，您需要*自行定义标签*。XML 使用*文件类型声明*（DTD）或者*XML Schema*
 来描述数据，带有 DTD 或者 XML Schema 的 XML 被设计为具有*自我描述性*。

7、服务器脚本

      服务器端脚本是对服务器行为的编程。浏览器请求某个 HTML 文件时，服务器会返回此文件，但是假如此文件含有服务器端的脚本，那么在此 HTML 文件作为纯 HTML 被返回浏览器之前，首先会执行 HTML 文件中的脚本。

8、SQL

      SQL （*S*tructured*Q*uery*L*anguage）是一门 ANSI 的标准计算机语言，用来访问和操作数据库系统。SQL 语句用于取回和更新数据库中的数据。SQL 可与数据库程序协同工作，比如 MS Access、DB2、Informix、MS SQL Server、Oracle、Sybase 以及其他数据库系统。




# HTML 篇

1、Web 浏览器的作用是读取 HTML 文档，并以网页的形式显示出它们。浏览器不会显示 HTML 标签，而是使用标签来解释页面的内容。


       <html>
        <body>
        <h1>My First Heading</h1>
        <p>My first paragraph.</p>
       </body>
      </html>

### 例子解释
- <html> 与 </html> 之间的文本描述网页
- <body> 与 </body> 之间的文本是可见的页面内容
- <h1> 与 </h1> 之间的文本被显示为标题
- <p> 与 </p> 之间的文本被显示为段落 





## 2、什么是 HTTP（Get与Post）？

超文本传输协议（HTTP）的设计目的是保证客户机与服务器之间的通信。

HTTP 的工作方式是客户机与服务器之间的请求-应答协议。

举例：客户端（浏览器）向服务器提交 HTTP 请求；服务器向客户端返回响应。响应包含关于请求的状态信息以及可能被请求的内容。

在客户机和服务器之间进行请求-响应时，两种最常被用到的方法是：GET 和 POST。
- GET - 从指定的资源请求数据。
- POST - 向指定的资源提交要被处理的数据
   Get
查询字符串（名称/值对）是在 GET 请求的 URL 中发送的：/test/demo_form.asp?name1=value1&name2=value2
   有关 GET 请求的其他一些注释：
- GET 请求可被缓存
- GET 请求保留在浏览器历史记录中
- GET 请求可被收藏为书签
- GET 请求不应在处理敏感数据时使用
- GET 请求有长度限制
- GET 请求只应当用于取回数据


**   POST**

*查询字符串（名称/值对）是在 POST 请求的 HTTP 消息主体中发送的：*



POST /test/demo_form.asp HTTP/1.1
Host: w3schools.com
name1=value1&name2=value2
   有关 POST 请求的其他一些注释：
- POST 请求不会被缓存
- POST 请求不会保留在浏览器历史记录中
- POST 不能被收藏为书签
- POST 请求对数据长度没有要求





2、**标题很重要**

     请确保将 HTML heading 标签只用于标题。不要仅仅是为了产生粗体或大号的文本而使用标题。

     搜索引擎使用标题为您的网页的结构和内容编制索引。

     因为用户可以通过标题来快速浏览您的网页，所以用标题来呈现文档结构是很重要的。

     应该将 h1 用作主标题（最重要的），其后是 h2（次重要的），再其次是 h3，以此类推。




# **Xhtml 相关知识总结**



## XHTML 是什么？
- XHTML 指可扩展超文本标签语言（EXtensible HyperText Markup Language）。
- XHTML 的目标是取代 HTML。
- XHTML 与 HTML 4.01 几乎是相同的。
- XHTML 是更严格更纯净的 HTML 版本。
- XHTML 是作为一种 XML 应用被重新定义的 HTML。
- XHTML 是一个 W3C 标准。






XML 是一种标记化语言，其中所有的东西都要被正确的标记，以产生形式良好的文档。

XML 用来描述数据，而 HTML 则用来显示数据。

今天的市场中存在着不同的浏览器技术，某些浏览器运行在计算机中，某些浏览器则运行在移动电话和手持设备上。而后者没有能力和手段来解释糟糕的标记语言。

因此，通过把 HTML 和 XML 各自的长处加以结合，我们得到了在现在和未来都能派上用场的标记语言 - XHTML。

XHTML 可以被所有的支持 XML 的设备读取，同时在其余的浏览器升级至支持 XML 之前，XHTML 使我们有能力编写出拥有良好结构的文档，这些文档可以很好地工作于所有的浏览器，并且可以向后兼容。






## 与Html相比Xhtml最主要的不同：
- XHTML 元素必须被正确地嵌套。
- XHTML 元素必须被关闭。
- 标签名必须用小写字母。
- XHTML 文档必须拥有根元素。
- 属性名称必须小写
- 属性值必须加引号
- 属性不能简写
- 用 Id 属性代替 name 属性
- XHTML DTD 定义了强制使用的 HTML 元素





### 重要的兼容性提示：

你应该在 "/" 符号前添加一个额外的空格，以使你的 XHTML 与当今的浏览器相兼容。如下：



<img src="picture.gif" `id`="picture1" />






## 语言属性（lang）

lang 属性应用于几乎所有的 XHTML 元素。它定义元素内部的内容的所用语言的类型。

如果在某元素中使用 lang 属性，就必须添加额外的 xml:lang，像这样：
<div `lang="no" xml:lang="no"`>Heia Norge!</div>


## 强制使用的 XHTML 元素

所有 XHTML 文档必须进行文件类型声明（DOCTYPE declaration）。在 XHTML 文档中必须存在html、head、body元素，而 title 元素必须位于在 head 元素中。

下面是一个最小化的 XHTML 文件模板：
`<!DOCTYPE Doctype goes here>`
<html xmlns="http://www.w3.org/1999/xhtml">
<head>
<title>Title goes here</title>
</head>

</html>
**提示：**文件类型声明并非 XHTML 文档自身的组成部分。它并不是 XHTML 元素，也没有关闭标签。

**提示：**在 XHTML 中，<html> 标签内的 xmlns 属性是必需的。然而，即使当 XHTML 文档中没有这个属性时，w3.org 的验证工具也不会提示错误。这是因为，"xmlns=http://www.w3.org/1999/xhtml"
 是一个固定的值，即使你没有把它包含在代码中，这个值也会被添加到 <html> 标签中。







## 3种文档类型声明
- DTD 规定了使用通用标记语言(SGML)的网页的语法。
- 诸如 HTML 这样的通用标记语言应该使用 DTD 来规定应用于某种特定文档中的标签的规则，这些规则包括一系列的元素和实体的声明。
- 在通用标记语言(SGML)的文档类型声明或 DTD 中，XHTML 被详细地进行了描述。
- XHTML DTD 使用精确的可被计算机读取的语言来描述合法的 XHTML 标记的语法和句法。

## 存在三种XHTML文档类型：
- STRICT（严格类型）
- TRANSITIONAL（过渡类型）
- FRAMESET（框架类型）





### XHTML 1.0 规定了三种 XML 文档类型，以对应上述三种 DTD。

### XHTML 1.0 Strict
<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Strict//EN" 
"http://www.w3.org/TR/xhtml1/DTD/`xhtml1-strict.dtd`">
在此情况下使用：需要干净的标记，避免表现上的混乱。请与层叠样式表配合使用。

### XHTML 1.0 Transitional
<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/`xhtml1-transitional.dtd`">
在此情况下使用：当需要利用 HTML 在表现上的特性时，并且当需要为那些不支持层叠样式表的浏览器编写 XHTML 时。

### XHTML 1.0 Frameset
<!DOCTYPE html
PUBLIC "-//W3C//DTD XHTML 1.0 Frameset//EN"
"http://www.w3.org/TR/xhtml1/DTD/`xhtml1-frameset.dtd"`>
在此的情况下使用：需要使用HTML框架将浏览器窗口分割为两部分或更多框架时






### 关于文件类型声明的一点提示



如果你希望将页面验证为正确的 XHTML，那么页面中必须含有文件类型声明。

需要注意的是，根据不同的文件类型声明，新式的浏览器对文档的处理方式也是不同的。如果浏览器读到一个文件类型声明，那么它会按照“恰当”的方式来处理文档。如果没有 DOCTYPE，文档也许会以截然不同的方式显示出来。




## 验证站点（及XHTML页面转换）



做完所有这一切以后，使用下面的链接根据官方的 W3C DTD 对所有修改过的页面进行验证： [XHTML
 Validator](http://validator.w3.org/check/referer)。接下来，可能还会有少数的错误被发现，逐一对这些错误进行（手工地）修正。我们的经验是，最容易犯的错误是在列表中漏掉了 </li> 标签。

我们应该使用转换工具吗（比方说 TIDY）？对，我们本可以使用 TIDY。

[Dave Raggett 的 HTML TIDY](http://www.w3.org/People/Raggett/tidy/)是用来清理HTML代码的免费工具。在处理那些由专门的
 HTML 代码编辑器和转换工具生成的难以阅读的HTML代码方面，TIDY 还是做得很棒的。同时，它可以帮助你发现站点中哪些地方需要投入更多精力，使得对于残疾人士，网页具有更强的易用性。

XHTML 文档是根据文档类型声明（DTD）进行验证的。只有将正确的 DTD 添加到文件的首行，XHTML 文件才会被正确地验证。

严格 DTD 包含没有被反对使用的或不出现在框架结构中的元素和属性：






## Xhtml为表达语义而标记文档，而不是为了样式



记住：请最大限度地使用 CSS 来进行布局。在 web 标准的世界里，XHTML 标记与表现无关，它只与文档结构有关。

结构良好的文档可以向浏览器传达尽可能多的语义，不论是浏览器位于掌上电脑还是时髦的桌面图形浏览器。结构良好的文档都能向用户传达可视化的语义，即使是在老的浏览器，或是在被用户关闭了 CSS 的现代浏览器中。

不是每个站点都能立即抛弃 HTML 表格布局。CSS 的发明者，W3C，直到 2002 年 11 月才将官方网站转换为 CSS 布局。然而，

即使是顽固的唯标准主义者也不总是将表现从结构中完全分离处理，至少在 XHTML 1 中是做不到的。但是现在，我们可以向这个

理想迈出重大的一步，通过将**表现从结构中分离（或者说将数据从设计中）**，即使是混合的传统的布局也可从中受益。




### 根据XHtml它们的意义使用元素，而不是根据它们的外观



从今天开始，我们将要使用 CSS 来决定元素的外观。我们甚至可以根据元素在页面中或者在站点中所在的位置来改变它们的外观。CSS 可以将表现从结构中彻底抽离，并且允许你按照你喜欢的样式来格式化任何元素。


我们并不想在关于 XHTML 的章节讲述更多 CSS 方面的技术。我们只是**希望展示文档结构和可视表达是两个完全不同的事物，并且**

**结构化元素应被用来转换文本，而不是强加显示效果**。




## **XHTML 结构化之二：案例分析：W3school 的结构化标记**

如何撰写合乎逻辑的、紧凑的标记，使得你有能力将带宽流量降低50%左右，在减少服务器负担和压力的同时，减少网站的加载时间。通过去除那些表现元素，并改掉那些没有任何好处的坏习惯，我们就可以达到上述的目的。


详细阐述唯一标识符属性 (id) - ，并展示它如何使你可以编写极其紧凑的
 XHTML 代码，不论你创建的是混合布局还是纯粹的 CSS 布局。




## div、id 和其他帮手



如果被正确地使用，div 可以成为结构化标记的好帮手，而 id 则是一种令人惊讶的小工具，它使你有能力编写极其紧凑XHTML，以及巧妙地利用 CSS，并通过标准文档对象模型 (DOM) 向站点添加复杂精巧的行为。

W3C 在其最新的 XHTML2 草案的 XHTML 结构模型中这样定义 div：

div 元素，通过与 id、class 及 role 属性配合，提供向文档添加额外结构的通用机制。这个元素不会将表现的风格定义于内容。所以，创作者可以通过将这个元素与样式表、xml:lang、属性等配合使用，使 XHTML 适应他们自身的需求和口味。

div 是 division 的简写。division 意为分割、区域、分组。比方说，当你将一系列的链接组合在一起，就形成了文档的division。



### id Vs class区别



id 属性对于 XHTML 并不新鲜；class 属性或者 div 元素也一样。它们都可以回溯到 HTML 时代。id 属性为一个元素分配一个唯一的名字。每个名字只能在被赋予的页面使用一次。（例如，假如你的页面包含 id 为 content 的 div，那么另外一个 div 或者其他别的元素都不能使用这个名字。相反地，class 属性可以被一遍又一遍地使用在页面中（例如，页面中的五个段落都可以使用名为
 "small" 或者"footnote" 的 class 名称）



下面的标记将有助于阐明 id 和 class 的差异：
<div id="searchform">Search form components go here. This
section of the page is unique.</div>
<div class="blogentry">
   <h2>Today's blog post</h2>
   <p>Blog content goes here.</p>
   <p>Here is another paragraph of blog content.</p>
   <p>Just as there can be many paragraphs on a page, so too
   there may be many entries in a blog. A blog page could use
   multiple instances of the class "blogentry" (or any other
   class).</p>
</div>
<div class="blogentry">
   <h2>Yesterday's blog post</h2>
   <p>In fact, here we are inside another div of class
   "blogentry."</p>
   <p>They reproduce like rabbits.</p>
   <p>If there are ten blog posts on this page, there might
   be ten divs of class "blogentry" as well.</p>
</div>
然而不是所有的站点都需要 div。blog 站点可以仅仅使用 h1, H2, 和 h2 标题和 <p> 段落，新闻站点也一样。我们在这里展示 class为 blogentry 的
 div，并**不是鼓励你在站点中塞满 div，而仅仅是为了向你展示这个原则：在同一个 HTML 文档中，使用多次 class，但只能使用一次 id**。



### id 的力量与规则



id 属性不可思议地强有力。它具有以下的能力：
- 作为样式表选择器，使我们有能力创作紧凑的最小化的 XHTML。
- 作为超文本的目标锚，取代过时的 name 属性。
- 作为从基于 DOM 的脚本来定位特定元素的方法。
- 作为对象元素的名称。
- 作为一种综合用途处理 (general purpose processing) 的工具（在 W3C 的例子中，“当把数据从HTML页面中提取到数据库，或将 HTML 文档转换为其他格式等情况下，作为域识别工具来使用。”）。

id 值必须以字母或者下划线开始；不能以数字开始。虽然 W3C 验证不会捕获这个错误，但是 XML 解析器会的。同时，如果你将 id 与 JavaScript 在表单中配合使用，那么 id 名称和值必须是合法的 JavaScript 变量。空格和连字号，特别是连字号，是不被允许的。不仅如此，将下划线用于 class 或者 id 名都不是个好主意，这是由于在 CSS2.0（以及某些浏览器）中的限制。



## 现在你已经学会了 XHTML，下一步学习什么呢？

下一步你需要学习 CSS 和 JavaScript 。

### CSS

CSS 被用于在同时控制多重页面的样式和布局。通过使用 CSS，所有的格式化任务均可被移出 HTML 文档，并存储于一个独立的文件中。在不把文档内容搞乱的情况下，CSS
 允许你对布局进行完全地控制。



### JavaScript



JavaScript 可使网站的动态性更强。

当你仅仅想展示固定不变的内容时，静态站点是不错的。然而动态站点可以对事件作出响应，并允许用户进行交互。

JavaScript 是因特网上最流行的脚本语言，并且它可以运行于所有的主流浏览器。



## 什么是 XMLHttpRequest 对象？



XMLHttpRequest 对象用于在后台与服务器交换数据。

XMLHttpRequest 对象是开发者的梦想，因为您能够：
- 在不重新加载页面的情况下更新网页
- 在页面已加载后从服务器请求数据
- 在页面已加载后从服务器接收数据
- 在后台向服务器发送数据

所有现代的浏览器都支持 XMLHttpRequest 对象。

如需学习更多有关 XMLHttpRequest 对象的知识，请学习我们的 [XML
 DOM 教程](http://www.w3school.com.cn/xmldom/index.asp)。

## 创建 XMLHttpRequest 对象

所有现代浏览器 (IE7+、Firefox、Chrome、Safari 以及 Opera) 都内建了 XMLHttpRequest 对象。

通过一行简单的 JavaScript 代码，我们就可以创建 XMLHttpRequest 对象。

### 创建 XMLHttpRequest 对象的语法：
xmlhttp=new XMLHttpRequest();
### 老版本的 Internet Explorer （IE5 和 IE6）使用 ActiveX 对象：
xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");

<html>
<head>
<script type="text/javascript">
var xmlhttp;
function loadXMLDoc(url)
{
xmlhttp=null;
if (window.XMLHttpRequest)
  {// code for IE7, Firefox, Opera, etc.
   xmlhttp=new XMLHttpRequest();
  }
else if (window.ActiveXObject)
  {// code for IE6, IE5
  xmlhttp=new ActiveXObject("Microsoft.XMLHTTP");
  }
if (xmlhttp!=null)
  {
 xmlhttp.onreadystatechange=state_Change;
  xmlhttp.open("GET",url,true);
  xmlhttp.send(null);
  }
else
  {
  alert("Your browser does not support XMLHTTP.");
  }
}
function state_Change()
{
if (xmlhttp.readyState==4)
  {// 4 = "loaded"
  if (xmlhttp.status==200)
    {// 200 = "OK"
    document.getElementById('A1').innerHTML=xmlhttp.status;
    document.getElementById('A2').innerHTML=xmlhttp.statusText;
    document.getElementById('A3').innerHTML=xmlhttp.responseText;
    }
  else
    {
    alert("Problem retrieving XML data:" + xmlhttp.statusText);
    }
  }
}
</script>
</head>

<body>
<h2>Using the HttpRequest Object</h2>
<p><b>Status:</b>
<span id="A1"></span>
</p>
<p><b>Status text:</b>
<span id="A2"></span>
</p>
<p><b>Response:</b>
<br /><span id="A3"></span>
</p>
<button onclick="loadXMLDoc('/example/xdom/note.xml')">Get XML</button>
</body>
</html>


注释：onreadystatechange 是一个事件句柄。它的值 (state_Change) 是一个函数的名称，当
 XMLHttpRequest 对象的状态发生改变时，会触发此函数。状态从 0 (uninitialized) 到 4 (complete) 进行变化。仅在状态为 4 时，我们才执行代码。

### 为什么使用 Async=true ？

我们的实例在 open() 的第三个参数中使用了 "true"。

该参数规定请求是否异步处理。

True 表示脚本会在 send() 方法之后继续执行，而不等待来自服务器的响应。

onreadystatechange 事件使代码复杂化了。但是这是在没有得到服务器响应的情况下，防止代码停止的最安全的方法。

通过把该参数设置为 "false"，可以省去额外的 onreadystatechange 代码。如果在请求失败时是否执行其余的代码无关紧要，那么可以使用这个参数。


# Javascript学习


## JavaScript 标签

正如您在 switch 语句那一章中看到的，可以对 JavaScript 语句进行标记。

如需标记 JavaScript 语句，请在语句之前加上冒号：
label:
语句

break 和 continue 语句仅仅是能够跳出代码块的语句。

### 语法
break labelname;

continue labelname;

continue 语句（带有或不带标签引用）只能用在循环中。

break 语句（不带标签引用），只能用在循环或 switch 中。

通过标签引用，break 语句可用于跳出任何 JavaScript 代码块：如果一段js代码中使用了不带标签的break语句，将结束整个代码段，且无任何其他执行操作。

### 实例
cars=["BMW","Volvo","Saab","Ford"];
list:
{
document.write(cars[0] + "<br>");
document.write(cars[1] + "<br>");
document.write(cars[2] + "<br>");
break list;
document.write(cars[3] + "<br>");
document.write(cars[4] + "<br>");
document.write(cars[5] + "<br>");
}



## Javascript 错误 - Throw、Try 和 Catch




try 语句测试代码块的错误。

catch 语句处理错误。

throw 语句创建自定义错误。






## JavaScript 抛出错误



当错误发生时，当事情出问题时，JavaScript 引擎通常会停止，并生成一个错误消息。描述这种情况的技术术语是：JavaScript 将抛出一个错误。




### JavaScript 测试和捕捉



try 语句允许我们定义在执行时进行错误测试的代码块。

catch 语句允许我们定义当 try 代码块发生错误时，所执行的代码块。

JavaScript 语句 try 和 catch 是成对出现的。



### 实例



在下面的例子中，我们故意在 try 块的代码中写了一个错字。

catch 块会捕捉到 try 块中的错误，并执行代码来处理它。
<!DOCTYPE html>
<html>
<head>
<script>
var txt="";
function message()
{
try
  {
  adddlert("Welcome guest!");
  }
catch(err)
  {
  txt="There was an error on this page.\n\n";
  txt+="Error description: " + err.message + "\n\n";
  txt+="Click OK to continue.\n\n";
  alert(txt);
  }
}
</script>
</head>
<body>
<input type="button" value="View message" onclick="message()">
</body>
</html>



### Throw 语句



throw 语句允许我们创建自定义错误。正确的技术术语是：创建或抛出异常（exception）。

如果把 throw 与 try 和 catch 一起使用，那么您能够控制程序流，并生成自定义的错误消息。

### 语法
throw exception
异常可以是 JavaScript 字符串、数字、逻辑值或对象。

### 实例

本例检测输入变量的值。如果值是错误的，会抛出一个异常（错误）。catch 会捕捉到这个错误，并显示一段自定义的错误消息：
<script>
function myFunction()
{
try
  {
  var x=document.getElementById("demo").value;
  if(x=="")    `throw "empty"`;
  if(isNaN(x)) `throw "not a number"`;
  if(x>10)     `throw "too high"`;
  if(x<5)      `throw "too low"`;
  }
catch(err)
  {
  var y=document.getElementById("mess");
  y.innerHTML="Error: " + err + ".";
  }
}
</script>

<h1>My First JavaScript</h1>
<p>Please input a number between 5 and 10:</p>
<input id="demo" type="text">
<button type="button" onclick="myFunction()">Test Input</button>
<p id="mess"></p><html>
<body>
<script type="text/javascript">
var x=prompt("请输入 0 至 10 之间的数：","5")
try
{ 
if(x>10) 
  throw "Err1" 
else if(x<0)
  throw "Err2"
else if(isNaN(x))
  throw "Err3"
} 
catch(er)
{
if(er=="Err1") 
  alert("错误！该值太大！")
if(er == "Err2") 
  alert("错误！该值太小！") 
if(er == "Err3") 
  alert("错误！该值不是数字！") 
}
</script>
</body>
</html>

### **Javascript 处理错误的onerror事件**
<html>
<head>
<script type="text/javascript">
onerror=handleErr
var txt=""
function handleErr(msg,url,l)
{
txt="本页中存在错误。\n\n"
txt+="错误：" + msg + "\n"
txt+="URL: " + url + "\n"
txt+="行：" + l + "\n\n"
txt+="点击“确定”继续。\n\n"
alert(txt)
return true
}
function message()
{
adddlert("Welcome guest!")
}
</script>
</head>
<body>
<input type="button" value="查看消息" onclick="message()" />
</body>
</html>

请注意，如果 getElementById 函数出错，上面的例子也会抛出一个错误。



### JavaScript 表单验证



JavaScript 可用来在数据被送往服务器前对 HTML 表单中的这些输入数据进行验证。

被 JavaScript 验证的这些典型的表单数据有：
- 用户是否已填写表单中的必填项目？
- 用户输入的邮件地址是否合法？
- 用户是否已输入合法的日期？
- 用户是否在数据域 (numeric field) 中输入了文本？





### HTML DOM （文档对象模型）

当网页被加载时，浏览器会创建页面的文档对象模型（Document Object Model）。

HTML DOM 模型被构造为对象的树。

### HTML DOM 树
![DOM HTML 树](http://www.w3school.com.cn/i/ct_htmltree.gif)
通过可编程的对象模型，JavaScript 获得了足够的能力来创建动态的 HTML。
- JavaScript 能够改变页面中的所有 HTML 元素
- JavaScript 能够改变页面中的所有 HTML 属性
- JavaScript 能够改变页面中的所有 CSS 样式
- JavaScript 能够对页面中的所有事件做出反应


## 查找 HTML 元素

通常，通过 JavaScript，您需要操作 HTML 元素。

为了做到这件事情，您必须首先找到该元素。有三种方法来做这件事：
- 通过 id 找到 HTML 元素
- 通过标签名找到 HTML 元素
- 通过类名找到 HTML 元素




## 通过标签名查找 HTML 元素，实例

本例查找 id="main" 的元素，然后查找 "main" 中的所有 <p> 元素：
<!DOCTYPE html>
<html>
<body>
<p>Hello World!</p>
<div id="main">
<p>The DOM is very useful.</p>
<p>本例演示 <b>getElementsByTagName</b> 方法。</p>
</div>
<script>
var x=document.getElementById("main");
var y=x.getElementsByTagName("p");
document.write('id 为 "main" 的 div 中的第一段文本是：' + y[0].innerHTML);
</script>
</body>
</html>


## 改变 HTML 内容

修改 HTML 内容的最简单的方法时使用 innerHTML 属性。

如需改变 HTML 元素的内容，请使用这个语法：
document.getElementById(id).innerHTML=new HTML


## 改变 HTML 属性

如需改变 HTML 元素的属性，请使用这个语法：
document.getElementById(id).attribute=new value



## 改变 HTML 样式

如需改变 HTML 元素的样式，请使用这个语法：
document.getElementById(id).style.property=new style
### 例子 1

下面的例子会改变 <p> 元素的样式：
<p id="p2">Hello World!</p>

<script>
document.getElementById("p2").style.color="blue";
</script>


### ** JavaScript 对 HTML 事件做出反应**


我们可以在事件发生时执行 JavaScript，比如当用户在 HTML 元素上点击时。

如需在用户点击某个元素时执行代码，请向一个 HTML 事件属性添加 JavaScript 代码：
onclick=JavaScript
HTML 事件的例子：
- 当用户点击鼠标时
- 当网页已加载时
- 当图像已加载时
- 当鼠标移动到元素上时
- 当输入字段被改变时
- 当提交 HTML 表单时
- 当用户触发按键时

在本例中，当用户在 <h1> 元素上点击时，会改变其内容：
<h1 `onclick="this.innerHTML='谢谢!'"`>请点击该文本</h1>



本例从事件处理器调用一个函数：
<!DOCTYPE html>
<html>
<head>
<script>
function changetext(id)
{
id.innerHTML="谢谢!";
}
</script>
</head>
<body>
<h1 `onclick="changetext(this)"`>请点击该文本</h1>
</body>
</html>



## HTML 事件属性


如需向 HTML 元素分配 事件，您可以使用事件属性。


向 button 元素分配 onclick 事件：
<button onclick="displayDate()">点击这里</button><!DOCTYPE html>
<html>
<body>
<p>点击按钮就可以执行 <em>displayDate()</em> 函数。</p>
<button onclick="displayDate()">点击这里</button>
<script>
function displayDate()
{
document.getElementById("demo").innerHTML=Date();
}
</script>
<p id="demo"></p>
</body>
</html>

## 使用 HTML DOM 来分配事件


HTML DOM 允许通过使用 JavaScript 来向 HTML 元素分配事件：


向 button 元素分配 onclick 事件：
<!DOCTYPE html>
<html>
<head>
</head>
<body>
<p>点击按钮就可以执行 <em>displayDate()</em> 函数。</p>
<button id="myBtn">点击这里</button>
<script>
document.getElementById("myBtn").onclick=function(){displayDate()};
function displayDate()
{
document.getElementById("demo").innerHTML=Date();
}
</script>
<p id="demo"></p>
</body>
</html> 


## onload 和 onunload 事件，onchange 事件，onfocus事件，onmouseover 和 onmouseout 事件，onmousedown、onmouseup 以及 onclick 事件等这些都是Javascript常见的动作事件




### **Javascript创建Html Dom 节点**

添加和删除节点（HTML 元素）
如需向 HTML DOM 添加新元素，您必须首先创建该元素（元素节点），然后向一个已存在的元素追加该元素。
<!DOCTYPE html>
<html>
<body>

<div id="div1">
<p id="p1">这是一个段落。</p>
<p id="p2">这是另一个段落。</p>
</div>

<script>
var para=document.createElement("p");
var node=document.createTextNode("这是新段落。");
para.appendChild(node);

var element=document.getElementById("div1");
element.appendChild(para);
</script>
</body>
</html>


例子解释：

这段代码创建新的 <p> 元素：
var para=document.createElement("p");
如需向 <p> 元素添加文本，您必须首先创建文本节点。这段代码创建了一个文本节点：
var node=document.createTextNode("这是新段落。");
然后您必须向 <p> 元素追加这个文本节点：
para.appendChild(node);
最后您必须向一个已有的元素追加这个新元素。

这段代码找到一个已有的元素：
var element=document.getElementById("div1");
这段代码向这个已有的元素追加新元素：
element.appendChild(para);


## 删除已有的 HTML 元素

如需删除 HTML 元素，您必须首先获得该元素的父元素：
<!DOCTYPE html>
<html>
<body>

<div id="div1">
<p id="p1">这是一个段落。</p>
<p id="p2">这是另一个段落。</p>
</div>
<script>
var parent=document.getElementById("div1");
var child=document.getElementById("p1");
parent.removeChild(child);
</script>
</body>
</html>


例子解释：

这个 HTML 文档含有拥有两个子节点（两个 <p> 元素）的 <div> 元素：
<div id="div1">
<p id="p1">这是一个段落。</p>
<p id="p2">这是另一个段落。</p>
</div>

找到 id="div1" 的元素：
var parent=document.getElementById("div1");
找到 id="p1" 的 <p> 元素：
var child=document.getElementById("p1");
从父元素中删除子元素：
parent.removeChild(child);
提示：如果能够在不引用父元素的情况下删除某个元素，就太好了。

不过很遗憾。DOM 需要清楚您需要删除的元素，以及它的父元素。

这是常用的解决方案：找到您希望删除的子元素，然后使用其 parentNode 属性来找到父元素：
var child=document.getElementById("p1");
child.parentNode.removeChild(child);



## JavaScript 对象

JavaScript中的所有事物都是对象：字符串、数值、数组、函数...。此外，JavaScript
 允许自定义对象。
提供多个内建对象，比如 String、Date、Array
 等等。对象只是带有属性和方法的特殊数据类型。



## 访问对象的属性



属性是与对象相关的值。

访问对象属性的语法是：
objectName.propertyName
这个例子使用了 String 对象的 length 属性来获得字符串的长度：
var message="Hello World!";
var x=message.`length`;

## 访问对象的方法

方法是能够在对象上执行的动作。

您可以通过以下语法来调用方法：
objectName.methodName()
这个例子使用了 String 对象的 toUpperCase() 方法来将文本转换为大写：
var message="Hello world!";
var x=message.`toUpperCase()`;


## 创建 JavaScript 对象

通过 JavaScript，您能够定义并创建自己的对象。

创建新对象有两种不同的方法：
- 定义并创建对象的实例
- 使用函数来定义对象，然后创建新的对象实例







## 创建直接的实例

这个例子创建了对象的一个新实例，并向其添加了四个属性：
person=new Object();
person.firstname="Bill";
person.lastname="Gates";
person.age=56;
person.eyecolor="blue";



替代语法（使用对象 literals）：
person={firstname:"John",lastname:"Doe",age:50,eyecolor:"blue"};



### 使用对象构造器（构造函数）创建实例

本例使用函数来构造对象：
<!DOCTYPE html>
<html>
<body>
<script>
function person(firstname,lastname,age,eyecolor)
{
this.firstname=firstname;
this.lastname=lastname;
this.age=age;
this.eyecolor=eyecolor;
}                                                                                 //下面创建对象person
myFather=new person("Bill","Gates",56,"blue");      //创建person的两个实例
var myMother=new person("Steve","Jobs",48,"green");
document.write(myFather.firstname + " is " + myFather.age + " years old.");
</script>

</body>
</html>




## 把方法添加到 JavaScript 对象



方法只不过是附加在对象上的函数。

在构造器函数内部定义对象的方法：（构造函数内部定义函数）
function person(firstname,lastname,age,eyecolor)
{
   this.firstname=firstname;
   this.lastname=lastname;
   this.age=age;
   this.eyecolor=eyecolor;

   this.changeName=changeName;
   function changeName(name)
   {
     this.lastname=name;
   }
}

changeName() 函数 name 的值赋给 person 的 lastname 属性。

现在您可以试一下：
myMother.changeName("Ballmer");



### JavaScript 类及Javascript数字



JavaScript 是面向对象的语言，但 JavaScript 不使用类。

在 JavaScript 中，不会创建类，也不会通过类来创建对象（就像在其他面向对象的语言中那样）。

JavaScript 基于 prototype，而不是基于类的。




## 所有 JavaScript 数字均为 64 位



JavaScript 不是类型语言。与许多其他编程语言不同，JavaScript 不定义不同类型的数字，比如整数、短、长、浮点等等。

JavaScript 中的所有数字都存储为根为 '1' '0' 的 64 位（8 比特），浮点数。




## 精度



整数（不使用小数点或指数计数法）最多为 15 位。小数的最大位数是
 17，但是浮点运算并不总是 100% 准确：




## 数字属性和方法

### 属性：
- MAX VALUE
- MIN VALUE
- NEGATIVE INFINITIVE
- POSITIVE INFINITIVE
- NaN
- prototype
- constructor

### 方法：
- toExponential()
- toFixed()
- toPrecision()
- toString()
- valueOf()





如需可用于 Number 对象的所有属性和方法的完整参考，请访问我们的 [Number
 对象参考手册](http://www.w3school.com.cn/jsref/jsref_obj_number.asp)。



### JavaScript String（字符串）对象 实例
- [计算字符串的长度](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_string_length)
如何使用长度属性来计算字符串的长度。- [为字符串添加样式](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_string_style)
如何为字符串添加样式。- [indexOf()
 方法](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_string_indexof)
如何使用 indexOf() 来定位字符串中某一个指定的字符首次出现的位置。- [match()
 方法](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_string_match)
如何使用 match() 来查找字符串中特定的字符，并且如果找到的话，则返回这个字符。- [如何替换字符串中的字符
 - replace()](http://www.w3school.com.cn/tiy/t.asp?f=jseg_replace_1)
如何使用 replace() 方法在字符串中用某些字符替换另一些字符。



[JavaScript
 String 对象参考手册](http://www.w3school.com.cn/jsref/jsref_obj_string.asp)，其中提供了可以与字符串对象一同使用的所有的属性和方法。







### JavaScript Date（日期）对象 实例
- [返回当日的日期和时间](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_date)
如何使用 Date() 方法获得当日的日期。- [getTime()](http://www.w3school.com.cn/tiy/t.asp?f=jseg_date_gettime)
getTime() 返回从 1970 年 1 月 1 日至今的毫秒数。- [setFullYear()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_date_setfullyear2)
如何使用 setFullYear() 设置具体的日期。- [toUTCString()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_date_toutcstring)
如何使用 toUTCString() 将当日的日期（根据 UTC）转换为字符串。- [getDay()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_date_date_weekday)
如何使用 getDay() 和数组来显示星期，而不仅仅是数字。- [显示一个钟表](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_date_timing_clock)
如何在网页上显示一个钟表。

## [JavaScript Date 对象参考手册](http://www.w3school.com.cn/jsref/jsref_obj_date.asp)，其中包括所有可用于日期对象的属性和方法。







## 定义日期

Date 对象用于处理日期和时间。

可以通过 new 关键词来定义 Date 对象。以下代码定义了名为 myDate 的 Date 对象：
var myDate=`new` Date() 
注释：Date 对象自动使用当前的日期和时间作为其初始值。



## 操作日期

通过使用针对日期对象的方法，我们可以很容易地对日期进行操作。

在下面的例子中，我们为日期对象设置了一个特定的日期 (2008 年 8 月 9 日)：
var myDate=new Date()
myDate.`setFullYear`(2008,7,9)

注意：表示月份的参数介于 0 到 11 之间。也就是说，如果希望把月设置为 8 月，则参数应该是 7。

在下面的例子中，我们将日期对象设置为 5 天后的日期：
var myDate=new Date()
myDate.`setDate`(myDate.getDate()+5)

注意：如果增加天数会改变月份或者年份，那么日期对象会自动完成这种转换。



## 比较日期

日期对象也可用于比较两个日期。

下面的代码将当前日期与 2008 年 8 月 9 日做了比较：
var myDate=new Date();
myDate.setFullYear(2008,8,9);
var today = new Date();
if (myDate>today)
{
alert("Today is before 9th August 2008");
}
else
{
alert("Today is after 9th August 2008");
}



### **  Javascript Array 数组对象**

定义数组
数组对象用来在单独的变量名中存储一系列的值。

我们使用关键词 new 来创建数组对象。下面的代码定义了一个名为 myArray 的数组对象：
var myArray=new Array()
有两种向数组赋值的方法（你可以添加任意多的值，就像你可以定义你需要的任意多的变量一样）。



### 1:
var mycars=`new Array()`
mycars[0]="Saab"
mycars[1]="Volvo"
mycars[2]="BMW"
### 2:
var mycars=`new Array("Saab","Volvo","BMW")`
注意：如果你需要在数组内指定数值或者逻辑值，那么变量类型应该是数值变量或者布尔变量，而不是字符变量。




## 访问数组

通过指定数组名以及索引号码，你可以访问某个特定的元素。
<html>
<body>
<script type="text/javascript">
var mycars = new Array()
mycars[0] = "Saab"
mycars[1] = "Volvo"
mycars[2] = "BMW"
for (i=0;i<mycars.length;i++)
{
document.write(mycars[i] + "<br />")
}
</script>
</body>
</html>

### **Javascript  Math 算数**

实例：- [round()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_math_round)
如何使用 round()。- [random()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_math_random)
如何使用 random() 来返回 0 到 1 之间的随机数。- [max()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_math_max)
如何使用 max() 来返回两个给定的数中的较大的数。（在 ECMASCript v3 之前，该方法只有两个参数。）- [min()](http://www.w3school.com.cn/tiy/t.asp?f=jsrf_math_min)
如何使用 min() 来返回两个给定的数中的较小的数。（在 ECMASCript v3 之前，该方法只有两个参数。）




## Math 对象


Math（算数）对象的作用是：执行普通的算数任务。


Math 对象提供多种算数值类型和函数。无需在使用这个对象之前对它进行定义。

### **Javascript RegExp 正则表达式**



当您检索某个文本时，可以使用一种模式来描述要检索的内容。RegExp 就是这种模式。

简单的模式可以是一个单独的字符。

更复杂的模式包括了更多的字符，并可用于解析、格式检查、替换等等。

您可以规定字符串中的检索位置，以及要检索的字符类型，等等。

## 定义 RegExp

RegExp 对象用于存储检索模式。

通过 new 关键词来定义 RegExp 对象。以下代码定义了名为 patt1 的 RegExp 对象，其模式是 "e"：
var patt1=new RegExp("e");
当您使用该 RegExp 对象在一个字符串中检索时，将寻找的是字符 "e"。

## RegExp 对象的方法

RegExp 对象有 3 个方法：test()、exec() 以及 compile()。

test() 方法检索字符串中的指定值。返回值是 true 或 false。
var patt1=new RegExp("e");

document.write(patt1.test("The best things in life are free")); 

由于该字符串中存在字母 "e"，以上代码的输出将是：
true




exec() 方法检索字符串中的指定值。返回值是被找到的值。如果没有发现匹配，则返回 null。

### 例子 1：
var patt1=new RegExp("e");

document.write(patt1.exec("The best things in life are free")); 

由于该字符串中存在字母 "e"，以上代码的输出将是：
e
您可以向 RegExp 对象添加第二个参数，以设定检索。例如，如果需要找到所有某个字符的所有存在，则可以使用 "g" 参数 ("global")。




在使用 "g" 参数时，exec() 的工作原理如下：
- 找到第一个 "e"，并存储其位置
- 如果再次运行 exec()，则从存储的位置开始检索，并找到下一个 "e"，并存储其位置
var patt1=new RegExp("e","g");
do
{
result=patt1.exec("The best things in life are free");
document.write(result);
}
while (result!=null) 

由于这个字符串中 6 个 "e" 字母，代码的输出将是：
eeeeeenull


## compile() 方法用于改变 RegExp。




compile() 既可以改变检索模式，也可以添加或删除第二个参数。

### 例子：
var patt1=new RegExp("e");

document.write(patt1.test("The best things in life are free"));

patt1.compile("d");

document.write(patt1.test("The best things in life are free"));

由于字符串中存在 "e"，而没有 "d"，以上代码的输出是：
truefalse



如需可与 RegExp 对象一同使用所有属性和方法的完整信息，请访问我们的 [RegExp
 对象参考手册](http://www.w3school.com.cn/jsref/jsref_obj_regexp.asp)。

### **Javascript Window**


浏览器对象模型（Browser Object Model）尚无正式标准。

由于现代浏览器已经（几乎）实现了 JavaScript 交互性方面的相同方法和属性，因此常被认为是 BOM 的方法和属性。




所有浏览器都支持 window 对象。它表示浏览器窗口。


所有 JavaScript 全局对象、函数以及变量均自动成为 window 对象的成员。


全局变量是 window 对象的属性。全局函数是 window 对象的方法。


甚至 HTML DOM 的 document 也是 window 对象的属性之一：
window.document.getElementById("header");

与此相同：
document.getElementById("header");



## Window 尺寸

有三种方法能够确定浏览器窗口的尺寸（浏览器的视口，不包括工具栏和滚动条）。

对于Internet Explorer、Chrome、Firefox、Opera 以及 Safari：
- window.innerHeight - 浏览器窗口的内部高度
- window.innerWidth - 浏览器窗口的内部宽度

对于 Internet Explorer 8、7、6、5：
- document.documentElement.clientHeight
- document.documentElement.clientWidth

或者
- document.body.clientHeight
- document.body.clientWidth

实用的 JavaScript 方案（涵盖所有浏览器）：
<!DOCTYPE html>
<html>
<body>
<p id="demo"></p>
<script>
var w=window.innerWidth
|| document.documentElement.clientWidth
|| document.body.clientWidth;

var h=window.innerHeight
|| document.documentElement.clientHeight
|| document.body.clientHeight;

x=document.getElementById("demo");
x.innerHTML="浏览器的内部窗口宽度：" + w + "，高度：" + h + "。"
</script>
</body>
</html>



window.location 对象用于获得当前页面的地址 (URL)，并把浏览器重定向到新的页面。



window.location 对象在编写时可不使用 window 这个前缀。
- location.hostname 返回 web 主机的域名
- location.pathname 返回当前页面的路径和文件名
- location.port 返回 web 主机的端口 （80 或 443）
- location.protocol 返回所使用的 web 协议（http:// 或 https://）
- location.href
 属性返回当前页面的 URL
- location.assign() 方法加载新的文档。


## Window Location Assign


location.assign() 方法加载新的文档。


加载一个新的文档：
<html>
<head>
<script>
function newDoc()
  {
`window.location.assign("http://www.w3school.com.cn")`
  }
</script>
</head>
<body>
<input type="button" value="加载新文档" onclick="newDoc()">
</body>
</html>

window.history 对象在编写时可不使用 window 这个前缀。

为了保护用户隐私，对 JavaScript 访问该对象的方法做出了限制。

一些方法：
- history.back() - 与在浏览器点击后退按钮相同
- history.forward() - 与在浏览器中点击按钮向前相同

JavaScript 中创建三种消息框：警告框、确认框、提示框。



- [警告框](http://www.w3school.com.cn/tiy/t.asp?f=jseg_alert)      alter()
- [带有折行的警告框](http://www.w3school.com.cn/tiy/t.asp?f=jseg_alert2)   +'\n'+
- [确认框](http://www.w3school.com.cn/tiy/t.asp?f=jseg_confirm)      confirm()
- [提示框](http://www.w3school.com.cn/tiy/t.asp?f=jseg_prompt)      prompt()
- **Javascript 计时**使用
 JavaScript，我们有能力做到在一个设定的时间间隔之后来执行代码，而不是在函数被调用后立即执行。我们称之为计时事件。


在 JavaScritp 中使用计时事件是很容易的，两个关键方法是:


- setTimeout()
未来的某时执行代码- clearTimeout()
取消setTimeout()
## setTimeout()
var t=setTimeout("javascript语句",毫秒)


setTimeout() 方法会返回某个值。在上面的语句中，值被储存在名为 t 的变量中。假如你希望取消这个 setTimeout()，你可以使用这个变量名来指定它。

setTimeout() 的第一个参数是含有 JavaScript 语句的字符串。这个语句可能诸如 "alert('5 seconds!')"，或者对函数的调用，诸如 alertMsg()"。

第二个参数指示从当前起多少毫秒后执行第一个参数。 提示：1000 毫秒等于一秒。

当下面这个例子中的按钮被点击时，一个提示框会在5秒中后弹出。
<html>
<head>
<script type="text/javascript">
function timedMsg()
 {
`var t=setTimeout("alert('5 seconds!')",5000)`
 }
</script>
</head>
<body>
<form>
<input type="button" value="Display timed alertbox!" onClick="timedMsg()">
</form>
</body>
</html>
### 实例 - 无穷循环

要创建一个运行于无穷循环中的计时器，我们需要编写一个函数来调用其自身。在下面的例子中，当按钮被点击后，输入域便从 0 开始计数。
<html>
<head>
<script type="text/javascript">
var c=0
var t
function timedCount()
 {
 document.getElementById('txt').value=c
 c=c+1
`t=setTimeout("timedCount()",1000)`
 }
</script>
</head>

<body>
<form>
<input type="button" value="Start count!" onClick="timedCount()">
<input type="text" id="txt">
</form>
</body>
</html>



## clearTimeout()
clearTimeout(setTimeout_variable)


下面的例子和上面的无穷循环的例子相似。唯一的不同是，现在我们添加了一个 "Stop Count!" 按钮来停止这个计数器：
<html>
<head>
<script type="text/javascript">
var c=0
var t

function timedCount()
 {
 document.getElementById('txt').value=c
 c=c+1
 `t=setTimeout("timedCount()",1000)`
 }

function stopCount()
 {
 `clearTimeout(t)`
 }
</script>
</head>

<body>
<form>
<input type="button" value="Start count!" onClick="timedCount()">
<input type="text" id="txt">
<input type="button" value="Stop count!" onClick="stopCount()">
</form>
</body>
</html>

### **Javascript Cookies 使用**



## 什么是cookie?



cookie 是存储于访问者的计算机（客户机）中的变量。每当同一台计算机通过浏览器请求某个页面时，就会发送这个 cookie。你可以使用 JavaScript 来创建和取回 cookie 的值。

### 有关cookie的例子：
- 名字 cookie
当访问者首次访问页面时，他或她也许会填写他/她们的名字。名字会存储于 cookie 中。当访问者再次访问网站时，他们会收到类似 "Welcome John Doe!" 的欢迎词。而名字则是从 cookie 中取回的。- 密码 cookie
当访问者首次访问页面时，他或她也许会填写他/她们的密码。密码也可被存储于 cookie 中。当他们再次访问网站时，密码就会从 cookie 中取回。- 日期 cookie
当访问者首次访问你的网站时，当前的日期可存储于 cookie 中。当他们再次访问网站时，他们会收到类似这样的一条消息："Your last visit was on Tuesday August 11, 2005!"。日期也是从 cookie 中取回的。
## 创建和存储 cookie

在这个例子中我们要创建一个存储访问者名字的 cookie。当访问者首次访问网站时，他们会被要求填写姓名。名字会存储于 cookie 中。当访问者再次访问网站时，他们就会收到欢迎词。

首先，我们会创建一个可在 cookie 变量中存储访问者姓名的函数：
function setCookie(c_name,value,expiredays)
{
var exdate=new Date()
exdate.setDate(exdate.getDate()+expiredays)
document.cookie=c_name+ "=" +escape(value)+
((expiredays==null) ? "" : ";expires="+exdate.toGMTString())
}

上面这个函数中的参数存有 cookie 的名称、值以及过期天数。

在上面的函数中，我们首先将天数转换为有效的日期，然后，我们将 cookie 名称、值及其过期日期存入 document.cookie 对象。

之后，我们要创建另一个函数来检查是否已设置 cookie：
function getCookie(c_name)
{
if (document.cookie.length>0)
  {
  c_start=document.cookie.indexOf(c_name + "=")
  if (c_start!=-1)
    { 
    c_start=c_start + c_name.length+1 
    c_end=document.cookie.indexOf(";",c_start)
    if (c_end==-1) c_end=document.cookie.length
    return unescape(document.cookie.substring(c_start,c_end))
    } 
  }
return ""
}

上面的函数首先会检查 document.cookie 对象中是否存有 cookie。假如 document.cookie 对象存有某些 cookie，那么会继续检查我们指定的 cookie 是否已储存。如果找到了我们要的 cookie，就返回值，否则返回空字符串。

最后，我们要创建一个函数，这个函数的作用是：如果 cookie 已设置，则显示欢迎词，否则显示提示框来要求用户输入名字。
function checkCookie()
{
  username=getCookie('username')
  if (username!=null && username!="")
  {alert('Welcome again '+username+'!')}
  else 
  {
  username=prompt('Please enter your name:',"")
  if (username!=null && username!="")
    {
      setCookie('username',username,365)
    }
  }
}
这是所有的代码：
<html>
<head>
<script type="text/javascript">
function getCookie(c_name)
{
   if (document.cookie.length>0)
   {
   c_start=document.cookie.indexOf(c_name + "=")
   if (c_start!=-1)
    { 
      c_start=c_start + c_name.length+1 
      c_end=document.cookie.indexOf(";",c_start)
      if (c_end==-1) c_end=document.cookie.length
      return unescape(document.cookie.substring(c_start,c_end))
    } 
  }
  return ""
}
function setCookie(c_name,value,expiredays)
{
  var exdate=new Date()
  exdate.setDate(exdate.getDate()+expiredays)
  document.cookie=c_name+ "=" +escape(value)+
  ((expiredays==null) ? "" : ";expires="+exdate.toGMTString())
}
function checkCookie()
{
  username=getCookie('username')
  if (username!=null && username!="")
  {alert('Welcome again '+username+'!')}
  else 
  {
    username=prompt('Please enter your name:',"")
      if (username!=null && username!="")
    {
      setCookie('username',username,365)
    }
  }
}
</script>
</head>
<body onLoad="checkCookie()">
</body>
</html>














