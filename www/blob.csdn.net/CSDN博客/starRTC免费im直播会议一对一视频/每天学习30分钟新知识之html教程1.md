# 每天学习30分钟新知识之html教程1 - starRTC免费im直播会议一对一视频 - CSDN博客
2017年08月11日 15:33:39[starRTC免费IM直播会议一对一视频](https://me.csdn.net/elesos)阅读数：269
|版本|年份|
|----|----|
|HTML|1991|
|HTML+|1993|
|HTML 2.0|1995|
|HTML 3.2|1997|
|HTML 4.01|1999|
|XHTML 1.0|2000|
|HTML5|2012|
|XHTML5|2013|
XHTML
 是更严谨更纯净的 HTML 版本。
HTML
 5 是下一代的 HTML。
CSS
 指层叠样式表（Cascading Style Sheets）。
CSS3
 是最新的 CSS 标准。
HTML 不是一种编程语言，而是一种**标记语言** (markup
 language)
<body> 与 </body> 之间的文本是可见的页面内容

## 图像<img src="w3school.jpg" width="104" height="142"/>
没有内容的 HTML 元素被称为空元素。在开始标签中添加斜杠，比如空行 <br />，是关闭空元素的正确方法
# 属性
属性总是以名称/值对的形式出现，比如：**name="value"**。
<h1
 align="center"> 
<body
 bgcolor="yellow"> 
<table
 border="1"> 关于表格边框的附加信息。

属性值应该始终被包括在引号内。双引号是最常用的。
在某些个别的情况下，比如属性值本身就含有双引号，那么您必须使用单引号，例如：
name='Bill "HelloWorld" Gates'
适用于大多数 HTML 元素的属性：
|class|classname|规定元素的类名|
|----|----|----|
|id|id|规定元素的唯一id|
|style|style_definition|规定元素的行内样式（inline style）|
|title|text|规定元素的额外信息（可在工具提示中显示）|
浏览器会自动地在标题的前后添加空行。
默认情况下，HTML 会自动地在块级元素前后添加一个额外的空行，比如段落、标题元素前后。
## 创建水平线<hr /> 标签
当显示页面时，浏览器会移除源代码中多余的空格和空行。可以用
样式**style 属性**
**改变所有 HTML 元素**
也可以在css中定义
### 应该避免使用下面这些标签和属性：
|标签|描述|
|----|----|
|<center>|定义居中的内容。|
|<font> 和 <basefont>|定义 HTML 字体。|
|<s> 和 <strike>|定义删除线文本|
|<u>|定义下划线文本|
| | |
|~~align~~|定义文本的对齐方式|
|~~bgcolor~~|定义背景颜色|
|~~color~~|定义文本颜色|
对于以上这些标签和属性：请使用样式代替！
<p style="font-family:verdana;color:red">
<p style="font-size:30px">This text is 30 pixels high</p>
<body style="background-color:yellow"><h1 style="text-align:center">This is a heading</h1>
sup，可实现上标字
|[<pre>](http://www.w3school.com.cn/tags/tag_pre.asp)|保留空格和换行，常用于显示代码|
所有的格式化代码均可移出 HTML 文档，然后移入一个独立的样式表。
三种方式来插入样式表
### 1.外部样式表：
<head>
`<link rel="stylesheet" type="text/css" href="mystyle.css">`
</head>2，<style> 标签定义内部样式表。
关于链接
`target="_blank"新窗口打开`请始终将正斜杠添加到子文件夹。假如这样书写链接：href="http://www.w3school.com.cn/html"，就会向服务器产生两次
 HTTP 请求。这是因为服务器会添加正斜杠到这个地址，然后创建一个新的请求，就像这样：href="http://www.w3school.com.cn/html/"。
命名的a标签，经常用于在大型文档开始位置上创建目录
表格
行由 <tr> 标签， 每行被分割为若干单元格（由 <td> 标签定义）
## 表头<th> 标签
<tr>
```
<th>Heading</th>
<th>Another Heading</th>
```
</tr>
# 块div将 HTML 元素组合起来。
通常会以新行来开始（和结束）。是可用于组合其他 HTML 元素的容器。没有特定的含义
另一个常见的用途是文档布局
对 HTML 进行分类，使我们能够为元素的类定义 CSS 样式。
<style>
.cities {
    background-color:black;   
}    
</style>
<div class="cities">
</div> 
或
<style>
span.red {
    color:red;
}
</style>
<span class="red">重要</span>
布局div
<style>
#header {
    background-color:black;
    color:white;
    text-align:center;
    padding:5px;
}
#nav {
    line-height:30px;
    background-color:#eeeeee;
    height:300px;
    width:100px;
    float:left;
    padding:5px; 
}
#section {
    width:350px;
    float:left;
    padding:10px; 
}
#footer {
    background-color:black;
    color:white;
    clear:both;
    text-align:center;
    padding:5px; 
}
</style>
或使用html5的元素，比如header，nav，footer
# 响应式 Web 设计
可变尺寸,平板和移动设备是必需的
使用现成的 CSS 框架Bootstrap。
框架子窗口
可以在同一个浏览器窗口中显示不止一个页面。

<frameset cols="25%,50%,25%">
  <frame src="/example/html/frame_a.html">
  <frame src="/example/html/frame_b.html">
  <frame src="/example/html/frame_c.html">
<noframes>
<body>您的浏览器无法处理框架！</body>
</noframes>
</frameset>
iframe 用于在网页内显示网页。
<iframe src="URL"></iframe>
## 使用 iframe 作为链接的目标，链接的 target 属性必须引用 iframe 的 name 属性：
<iframe src="demo_iframe.htm" `name``="iframe_a"`></iframe>
<p><a href="http://www.w3school.com.cn" target="iframe_a">W3School.com.cn</a></p>
脚本
script 元素既可包含脚本语句，也可通过 src 属性指向外部脚本文件。
JavaScript 最常用于图片操作、表单验证以及内容动态更新。
头部
<head> 元素是所有头部元素的容器
以下标签都可以添加到 head 部分：<title>、<base>、<link>、<meta>、<script> 以及 <style>。
<base> 标签为页面上的所有链接规定默认地址或默认目标（target）：
<head>
<base href="http://www.w3school.com.cn/images/" />
<base target="_blank" />
</head>
meta 元素被用于规定页面的描述、关键词、文档的作者、最后修改时间
一些搜索引擎会利用 meta 元素的 name 和 content 属性来索引您的页面。
下面的 meta 元素定义页面的描述：
<meta name="description" content="Free Web tutorials on HTML, CSS, XML" />
下面的 meta 元素定义页面的关键词：
<meta name="keywords" content="HTML, CSS, XML" />
有些字符是不能用的，不能使用小于号（<）和大于号（>），这是因为浏览器会误认为它们是标签。
如果希望正确地显示，必须用<

<!DOCTYPE> 不是 HTML 标签。它为浏览器提供一项信息（声明），即 HTML 是用什么版本编写的。
如
`<!DOCTYPE html>是html5声明`
表单
**<input>**元素是最重要的**表单元素**。
<input type="text">定义用于文本输入
<input type="radio">定义单选按钮。
<input type="radio" name="sex" value="male" checked>Male
<br>
<input type="radio" name="sex" value="female">Female<input type="checkbox"> 定义复选框。
**<input type="password">**定义**密码字段**：
value规定默认值。
## 提交按钮
**<input type="submit">**
**每个输入字段必须设置一个 name 属性。才会提交**
## <select> 元素（下拉列表）
按钮：<button type="button" onclick="alert('Hello World!')">Click Me!</button>
<input type="button" onclick="alert('Hello World!')" value="Click Me!">
<input type="range" name="points" min="0" max="10">显示为滑块控件

HTML5 增加了如下表单元素：
- <datalist>
- <keygen>
- <output>
HTML5 增加了多个新的输入类型：
- color
- date
- datetime
- datetime-local
- email
- month
- number
- range
- search
- tel
- time
- url
- week
 <input type="email" name="email"> <input type="url" name="homepage">
## 输入限制
maxlength最大字符数，但不会提供任何反馈
pattern通过其检查输入值的正则表达式
required必需填写

HTML5 为 <input> 增加了如下属性：
- autocomplete
- autofocus
- form
- formaction
- formenctype
- formmethod
- formnovalidate
- formtarget
- height 和 width
- list
- min 和 max
- multiple
- pattern (regexp)
- placeholder
- required
- step
并为 <form> 增加如需属性：
- autocomplete
- novalidate
- 
