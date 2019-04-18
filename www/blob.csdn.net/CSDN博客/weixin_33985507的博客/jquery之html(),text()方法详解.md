# jquery之html(),text()方法详解 - weixin_33985507的博客 - CSDN博客
2016年10月17日 22:09:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：2
**一：html()**
`html()`函数用于**设置或返回当前jQuery对象所匹配的DOM元素内的html内容**。
该函数的用途相当于设置或获取DOM元素的`innerHTML`属性值。
该函数属于`jQuery`对象(实例)。
### 语法
jQueryObject.html([ htmlString ])
### 参数
|参数|描述|
|----|----|
|htmlString|可选/String/Function类型用于设置的html字符串。|
如果没有指定`htmlString`参数，则表示获取**第一个匹配元素**的html内容；如果指定了`htmlString`参数，则表示设置**所有匹配元素**的html内容。
jQuery 1.4 新增支持：参数`htmlString`可以为函数。`html()`将根据匹配的所有元素遍历执行该函数，函数中的`this`将指向对应的DOM元素。
`html()`还会为函数传入两个参数：第一个参数就是该元素在匹配元素中的索引，第二个参数就是该元素当前的html内容。函数的返回值就是需要为该元素设置的html内容。
如果参数`htmlString`不是字符串或函数类型，则会被转换为字符串类型( [toString()](http://www.365mini.com/page/javascript-tostring.htm) )。如果参数为`null`或`undefined`，则将其视作空字符串("")。
### 返回值
`html()`函数的返回值是String/jQuery类型，返回值的实际类型取决于`html()`函数所执行的操作。
如果`html()`函数执行的是设置操作，将返回当前jQuery对象本身。如果执行的是获取操作，将返回第一个匹配元素的html内容，该值为字符串类型。
### 示例&说明
以下面这段HTML代码为例：
```
<div id="n1">
    <p id="n2">Hello</p>
    <p id="n3">
        CodePlayer
        <span id="n4">专注于编程开发技术分享</span>
        <span id="n5"></span>
    </p>
</div>
```
我们编写如下jQuery代码：
var $n2 = $("#n2");
alert( $n2.html());// Hello
// 设置n2的html内容
$n2.html('<strong>Hello World</strong>');
var $n3 = $("#n3");
alert( $n3.html());
/*
CodePlayer
<span id="n4">专注于编程开发技术分享</span>
<span id="n5"></span>
*/
// 清空所有span元素的html内容(将html内容设为"")
$("span").html("");
// 将所有p元素的html内容改为"第<em>N</em>个p元素，id为this.id"
// 这里的N表示该元素在所有匹配元素中的序号(1、2、3……)
// this.id是该元素的id属性值
$("p").html(function(index, currentHtml){
// 函数内的this指向当前迭代的p元素
return"第<em>"+(index +1)+"</em>个p元素，id为"+this.id;
});
**二：text()**
`text()`函数用于**设置或返回当前jQuery对象所匹配的DOM元素内的text内容**。
所谓text内容，就是在该元素的html内容(即`innerHTML`属性值)的基础上过滤掉所有HTML标记后的文本内容(即IE浏览器的DOM元素特有的`innerText`属性值)。
如果jQuery对象匹配的元素不止一个，则`text()`返回合并了每个匹配元素(包含其后代元素)中的文本内容后的字符串。
该函数属于`jQuery`对象(实例)。
### 语法
jQueryObject.text([ textString ])
### 参数
|参数|描述|
|----|----|
|textString|可选/String/Function类型用于设置的text字符串。|
如果没有指定`textString`参数，则表示获取合并了**每一个匹配元素**中的内容后的text内容；如果指定了`textString`参数，则表示设置**所有匹配元素**的text内容。
jQuery 1.4 新增支持：参数`textString`可以为函数。`text()`将根据匹配的所有元素遍历执行该函数，函数中的`this`将指向对应的DOM元素。
`text()`还会为函数传入两个参数：第一个参数就是该元素在匹配元素中的索引，第二个参数就是该元素当前的text内容。函数的返回值就是需要为该元素设置的text内容。
如果参数`textString`不是字符串或函数类型，则会被转换为字符串类型( [toString()](http://www.365mini.com/page/javascript-tostring.htm) )。如果参数为`null`或`undefined`，则将其视作空字符串("")。
### 返回值
`text()`函数的返回值是String/jQuery类型，返回值的实际类型取决于`text()`函数所执行的操作。
如果`text()`函数执行的是设置操作，将返回当前jQuery对象本身。如果执行的是获取操作，将返回合并了每一个匹配元素中的内容后的text内容，该值为字符串类型。
### 示例&说明
以下面这段text代码为例：
<divid="n1">
<pid="n2">Hello</p>
<pid="n3">
        CodePlayer
<spanid="n4">专注于编程开发技术分享</span>
<spanid="n5"></span>
</p>
<ul>
<li>item1</li>
<li>item1</li>
<li>item1</li>
</ul>
</div>
我们编写如下jQuery代码：
var $n2 = $("#n2");
alert( $n2.text());// Hello
// 设置n2的text内容
$n2.text("Hello World");
alert($("p").text());// 返回"Hello World\nCodePlayer\n专注于编程开发技术分享\n"
// 由于不同浏览器的HTML解析器差异，返回的文本在换行符或其他空白字符方面也可能不同。
var $n3 = $("#n3");
// 返回的是过滤掉HTML标记的文本内容
alert( $n3.text());
/*
CodePlayer
专注于编程开发技术分享
*/
var $n5 = $("#n5");
// 虽然设置的内容中包含符合html标签的字符串，但是这些都会被当作文本内容看待，而不会被当作html内容来看待
// 因此页面上将显示字符串"<strong>Hello World</strong>"，而不是粗体的"Hello World"。
// 这相当于$5.html( '<strong>Hello World</strong>' );
$n5.text('<strong>Hello World</strong>');
alert( $n5.text());// <strong>Hello World</strong>
// 清空所有span元素的text内容(将text内容设为"")
$("span").text("");
// 将所有li元素的text内容改为"第N个<li>元素"
// 这里的N表示该元素在所有匹配元素中的序号(1、2、3……)
$("li").text(function(index, currentText){
// 函数内的this指向当前迭代的li元素
return"第"+(index +1)+"个<li>元素";
