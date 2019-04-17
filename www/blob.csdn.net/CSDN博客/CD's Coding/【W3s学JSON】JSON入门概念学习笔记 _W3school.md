# 【W3s学JSON】JSON入门概念学习笔记 _W3school - CD's Coding - CSDN博客





2015年07月20日 15:22:23[糖果天王](https://me.csdn.net/okcd00)阅读数：1441








# 概念




JSON：JavaScript 对象表示法（JavaScript Object Notation）。

JSON 是存储和交换文本信息的语法。类似 XML。

JSON 比 XML 更小、更快，更易解析。



## 每一章中用到的实例
{
"employees": [
{ "firstName":"Bill" , "lastName":"Gates" },
{ "firstName":"George" , "lastName":"Bush" },
{ "firstName":"Thomas" , "lastName":"Carter" }
]
}


这个 employee 对象是包含 3 个员工记录（对象）的数组。



## 什么是 JSON ？
- JSON 指的是 JavaScript 对象表示法（JavaScript Object Notation）
- JSON 是轻量级的文本数据交换格式
- JSON 独立于语言 *
- JSON 具有自我描述性，更易理解

* JSON 使用 JavaScript 语法来描述数据对象，但是 JSON 仍然独立于语言和平台。JSON 解析器和 JSON 库支持许多不同的编程语言。



## JSON - 转换为 JavaScript 对象


JSON 文本格式在语法上与创建 JavaScript 对象的代码相同。


由于这种相似性，无需解析器，JavaScript 程序能够使用内建的 [eval() 函数](http://www.w3school.com.cn/jsref/jsref_eval.asp)，用
 JSON 数据来生成原生的 JavaScript 对象。





# 简介







## 亲自试一试 - 实例


通过我们的编辑器，您可以在线编辑 JavaScript 代码，然后通过点击一个按钮来查看结果：
<html>
<body>
<h2>在 JavaScript 中创建 JSON 对象</h2>

<p>
Name: <span id="jname"></span><br />
Age: <span id="jage"></span><br />
Address: <span id="jstreet"></span><br />
Phone: <span id="jphone"></span><br />
</p>

<script type="text/javascript">

```
var JSONObject= {
"name":"Bill Gates",
"street":"Fifth Avenue New York 666",
"age":56,
"phone":"555 1234567"};
```

document.getElementById("jname").innerHTML=JSONObject.name
document.getElementById("jage").innerHTML=JSONObject.age
document.getElementById("jstreet").innerHTML=JSONObject.street
document.getElementById("jphone").innerHTML=JSONObject.phone
</script>

</body>
</html>

[亲自试一试](http://www.w3school.com.cn/tiy/t.asp?f=json_create)

![](https://img-blog.csdn.net/20150720150618817?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




## 类似 XML
- JSON 是纯文本
- JSON 具有“自我描述性”（人类可读）
- JSON 具有层级结构（值中存在值）
- JSON 可通过 JavaScript 进行解析
- JSON 数据可使用 AJAX 进行传输



## 相比 XML 的不同之处
- 没有结束标签
- 更短
- 读写的速度更快
- 能够使用内建的 JavaScript eval() 方法进行解析
- 使用数组
- 不使用保留字



## 为什么使用 JSON？


对于 AJAX 应用程序来说，JSON 比 XML 更快更易使用：

#### 使用 XML
- 读取 XML 文档
- 使用 XML DOM 来循环遍历文档
- 读取值并存储在变量中

#### 使用 JSON
- 读取 JSON 字符串
- 用 eval() 处理 JSON 字符串





# 语法




JSON 语法是 JavaScript 语法的子集。



## JSON 语法规则


JSON 语法是 JavaScript 对象表示法语法的子集。
- 数据在名称/值对中
- 数据由逗号分隔
- 花括号保存对象
- 方括号保存数组



## JSON 名称/值对


JSON 数据的书写格式是：名称/值对。


名称/值对包括字段名称（在双引号中），后面写一个冒号，然后是值：
"firstName" : "John"

这很容易理解，等价于这条 JavaScript 语句：
firstName = "John"


## JSON 值


JSON 值可以是：
- 数字（整数或浮点数）
- 字符串（在双引号中）
- 逻辑值（true 或 false）
- 数组（在方括号中）
- 对象（在花括号中）
- null



## JSON 对象


JSON 对象在花括号中书写：


对象可以包含多个名称/值对：
{ "firstName":"John" , "lastName":"Doe" }

这一点也容易理解，与这条 JavaScript 语句等价：
firstName = "John"
lastName = "Doe"



## JSON 数组


JSON 数组在方括号中书写：


数组可包含多个对象：
{
"employees": [
{ "firstName":"John" , "lastName":"Doe" },
{ "firstName":"Anna" , "lastName":"Smith" },
{ "firstName":"Peter" , "lastName":"Jones" }
]
}


在上面的例子中，对象 "employees" 是包含三个对象的数组。每个对象代表一条关于某人（有姓和名）的记录。



## JSON 使用 JavaScript 语法


因为 JSON 使用 JavaScript 语法，所以无需额外的软件就能处理 JavaScript 中的 JSON。


通过 JavaScript，您可以创建一个对象数组，并像这样进行赋值：

### 例子
var employees = [
{ "firstName":"Bill" , "lastName":"Gates" },
{ "firstName":"George" , "lastName":"Bush" },
{ "firstName":"Thomas" , "lastName": "Carter" }
];


可以像这样访问 JavaScript 对象数组中的第一项：
employees[0].lastName;

返回的内容是：
Gates

可以像这样修改数据：
employees[0].lastName = "Jobs";
[亲自试一试](http://www.w3school.com.cn/tiy/t.asp?f=json_objectarray)



```java
<html>
<body>
<h2>在 JavaScript 中创建 JSON 对象</h2>

<p>
Name: <span id="jname"></span><br />
Age: <span id="jage"></span><br />
Address: <span id="jstreet"></span><br />
Phone: <span id="jphone"></span><br />
try: <span id="jtry"></span><br />
</p>

<script type="text/javascript">
var JSONObject= {
"name":"Bill Gates",
"street":"Fifth Avenue New York 666",
"age":56,
"phone":"555 1234567"};
document.getElementById("jname").innerHTML=JSONObject.name
document.getElementById("jage").innerHTML=JSONObject.age
document.getElementById("jstreet").innerHTML=JSONObject.street
document.getElementById("jphone").innerHTML=JSONObject.phone

var employees = [
{ "firstName":"Bill" , "lastName":"Gates" },
{ "firstName":"George" , "lastName":"Bush" },
{ "firstName":"Thomas" , "lastName": "Carter" }
];
document.getElementById("jtry").innerHTML=employees[0].lastName;

</script>

</body>
</html>
```


这里需要注意的是<xxx></xxx>相当于花括号，需要在那中间来申明变量之类的操作，否则就直接作为文本显示出来了。

![](https://img-blog.csdn.net/20150720151458441?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




在下面的章节，您将学到如何把 JSON 文本转换为 JavaScript 对象。



## JSON 文件
- JSON 文件的文件类型是 ".json"
- JSON 文本的 MIME 类型是 "application/json"





# 使用




## 把 JSON 文本转换为 JavaScript 对象


JSON 最常见的用法之一，是从 web 服务器上读取 JSON 数据（作为文件或作为 HttpRequest），将 JSON 数据转换为 JavaScript 对象，然后在网页中使用该数据。


为了更简单地为您讲解，我们使用字符串作为输入进行演示（而不是文件）。



## JSON 实例 - 来自字符串的对象


创建包含 JSON 语法的 JavaScript 字符串：
var txt = '{ "employees" : [' +
'{ "firstName":"Bill" , "lastName":"Gates" },' +
'{ "firstName":"George" , "lastName":"Bush" },' +
'{ "firstName":"Thomas" , "lastName":"Carter" } ]}';


由于 JSON 语法是 JavaScript 语法的子集，JavaScript 函数 eval() 可用于将 JSON 文本转换为 JavaScript 对象。


eval() 函数使用的是 JavaScript 编译器，可解析 JSON 文本，然后生成 JavaScript 对象。必须把文本包围在括号中，这样才能避免语法错误：
var obj = eval ("(" + txt + ")");

在网页中使用 JavaScript 对象：

### 例子
<p>
First Name: <span id="fname"></span><br />
Last Name: <span id="lname"></span><br />
</p>

<script type="text/javascript">
document.getElementById("fname").innerHTML = obj.employees[1].firstName
document.getElementById("lname").innerHTML = obj.employees[1].lastName
</script>

[亲自试一试](http://www.w3school.com.cn/tiy/t.asp?f=json_eval)



```java
<html>
<body>

<p>
First Name: <span id="fname"></span><br />
Last Name: <span id="lname"></span><br />
</p>

<script type="text/javascript">
var txt = '{ "employees" : [' +
'{ "firstName":"Bill" , "lastName":"Gates" },' +
'{ "firstName":"George" , "lastName":"Bush" },' +
'{ "firstName":"Thomas" , "lastName":"Carter" } ]}';

var obj = eval ("(" + txt + ")");

document.getElementById("fname").innerHTML = obj.employees[1].firstName
document.getElementById("lname").innerHTML = obj.employees[1].lastName
</script>

</body>
</html>
```

![](https://img-blog.csdn.net/20150720152124305?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)





## JSON 解析器

**提示：**eval() 函数可编译并执行任何 JavaScript 代码。这隐藏了一个潜在的安全问题。


使用 JSON 解析器将 JSON 转换为 JavaScript 对象是更安全的做法。JSON 解析器只能识别 JSON 文本，而不会编译脚本。


在浏览器中，这提供了原生的 JSON 支持，而且 JSON 解析器的速度更快。


较新的浏览器和最新的 ECMAScript (JavaScript) 标准中均包含了原生的对 JSON 的支持。
|Web 浏览器支持|Web 软件支持|
|----|----|
|- Firefox (Mozilla) 3.5- Internet Explorer 8- Chrome- Opera 10- Safari 4|- jQuery- Yahoo UI- Prototype- Dojo- ECMAScript 1.5|

[亲自试一试](http://www.w3school.com.cn/tiy/t.asp?f=json_parse)


对于较老的浏览器，可使用 JavaScript 库： [https://github.com/douglascrockford/JSON-js](https://github.com/douglascrockford/JSON-js)


JSON 格式最初是[由 Douglas Crockford 制定的](http://developer.yahoo.com/yui/theater/video.php?v=crockford-json)。





# Reference

Github:  [douglascrockford/JSON-js](https://github.com/douglascrockford/JSON-js)

# [](https://github.com/douglascrockford/JSON-js)





JSON in JavaScript


Douglas Crockford
douglas@crockford.com

2015-05-03


JSON is a light-weight, language independent, data interchange format.
See [http://www.JSON.org/](http://www.json.org/)

The files in this collection implement JSON encoders/decoders in JavaScript.

JSON became a built-in feature of JavaScript when the ECMAScript Programming
Language Standard - Fifth Edition was adopted by the ECMA General Assembly
in December 2009. Most of the files in this collection are for applications
that are expected to run in obsolete web browsers. For most purposes, json2.js
is the best choice.


json2.js: This file creates a JSON property in the global object, if there
isn't already one, setting its value to an object containing a stringify
method and a parse method. The parse method uses the eval method to do the
parsing, guarding it with several regular expressions to defend against
accidental code execution hazards. On current browsers, this file does nothing,
preferring the built-in JSON object. There is no reason to use this file unless
fate compels you to support IE8, which is something that no one should ever 
have to do again.

json_parse.js: This file contains an alternative JSON parse function that
uses recursive descent instead of eval.

json_parse_state.js: This files contains an alternative JSON parse function that
uses a state machine instead of eval.

cycle.js: This file contains two functions, JSON.decycle and JSON.retrocycle,
which make it possible to encode cyclical structures and dags in JSON, and to
then recover them. This is a capability that is not provided by ES5. JSONPath 
is used to represent the links. [[http://GOESSNER.net/articles/JsonPath/](http://goessner.net/articles/JsonPath/)]











