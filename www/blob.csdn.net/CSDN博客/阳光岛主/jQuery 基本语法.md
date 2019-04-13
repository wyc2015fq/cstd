
# jQuery 基本语法 - 阳光岛主 - CSDN博客

2013年08月02日 23:50:49[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：5397


**[jQuery](http://jquery.com/)**，一个 JavaScript 库，极大地简化了 JavaScript 编程，很容易学习。它是一款同[prototype](http://prototypejs.org/)、[Note.js](http://nodejs.org/)等一样优秀的js开发库类，特别是对css和XPath的支持，使我们写js变得更加方便！如果你不是个js高手又想写出优秀的js效果，jQuery可以帮你达到目的！
**下载地址：**[jQuery Downloads](http://jquery.com/download/)
**简单示例：**

```python
<html>
<head>
	<title>IT-Homer</title>
	<script src="http://code.jquery.com/jquery-1.10.1.min.js"></script>
	<script language="javascript" type="text/javascript">   
		$(document).ready(function(){
			$("a").click(function() {
				alert("Hello world!");
			});
		});
	</script>
</head>
<body>
	CSDN Blog : <a href="http://blog.csdn.net/ithomer" target="_blank" title="click me">IT-Homer</a>
</body>
</html>
```
**运行效果：**
![](https://img-blog.csdn.net/20130802234305437)
**示例解析：**
上边的效果是点击文档中所有a标签时将弹出对话框（alert），其中，$("a") 是一个jQuery选择器，$本身表示一个jQuery类，所有$()是构造一个jQuery对象，click()是这个对象的方法，同理$(document)也是一个jQuery对象，ready(fn)是$(document)的方法，表示当document全部下载完毕时执行函数。
在进行下面内容之前我还要说明一点$("p")和$("\#p")的区别,$("p")表示取所有p标签（例如：<p></p>）的元素，$("\#p")表示取id为"p"（例如：<span  id="p"></span>）的元素.

**一、核心部分**
$(expr)
说明：该函数可以通过css选择器，Xpath或html代码来匹配目标元素，所有的jQuery操作都以此为基础
参数：expr：字符串，一个查询表达式或一段html字符串
**例子：**
未执行jQuery前：
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>one</p>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<div>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>two</p>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</div>
<p>three</p>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
alert($("div > p").html());
}
运行：当点击id为test的元素时，弹出对话框文字为two，即div标签下p元素的内容

functionjq(){
$("<div><p>Hello</p></div>").appendTo("body");
}
运行：当点击id为test的元素时，向body中添加“<div><p>Hello</p></div>”，在网页中显示字符串“Hello”
$(elem)
说明：限制jQuery作用于一个特定的dom元素，这个函数也接受xml文档和windows对象
参数： elem：通过jQuery对象压缩的DOM元素
**例子：**
未执行jQuery前：<p>one</p>
<div>
<p>two</p>
</div><p>three</p>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
alert($(document).find("div > p").html());
}
运行：当点击id为test的元素时，弹出对话框文字为two，即div标签下p元素的内容

functionjq(){
$(document.body).css("background-color", "yellow");
}
运行：当点击id为test的元素时，背景色变成黄色（yellow）
$(elems)
说明：限制jQuery作用于一组特定的DOM元素
参数： elem：一组通过jQuery对象压缩的DOM元素
**例子：**
未执行jQuery前：<form id="form1">
<input type="text" name="text1" value="text1">
<input type="text" name="text2" value="text2">
<input type="submit" name="Submit" value="提交">
</form>
<a href="\#" id="test" onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
$(form1.elements).hide();
}
运行：当点击id为test的元素时，隐藏form1表单中的所有元素。

jQuery代码及功能：
functionjq(){
$(":text:first").hide();
}
运行：当点击id为test的元素时，隐藏form1表单中的type为text的第一个，即隐藏text1。
$(fn)
说明：$(document).ready()的一个速记方式，当文档全部载入时执行函数。可以有多个$(fn)当文档载入时，同时执行所有函数
参数：fn (Function):当文档载入时执行的函数
**例子：**$(function(){
$(document.body).css("background", "black");
})
运行：当文档载入时背景变成黑色，相当于onLoad。

$(obj)
说明：复制一个jQuery对象，
参数：obj (jQuery): 要复制的jQuery对象
**例子：**
未执行jQuery前：
参数：obj (jQuery): 要复制的jQuery对象
例子：
未执行jQuery前：
<p>one</p>
<div>
<p>two</p>
</div>
<p>three</p>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
var f = $("div");
alert($(f).find("p").html());
}
运行：当点击id为test的元素时，弹出对话框文字为two，即div标签下p元素的内容。

each(fn)
说明：将函数作用于所有匹配的对象上
参数：fn (Function): 需要执行的函数
**例子：**
未执行jQuery前：
<imgsrc="1.jpg"/>
<imgsrc="1.jpg"/>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
$("img").each(function(){
this.src="2.jpg"; });
}
运行：当点击id为test的元素时，img标签的src都变成了2.jpg。
eq(pos)
说明：减少匹配对象到一个单独得dom元素
参数：pos (Number): 期望限制的索引，从0 开始
**例子：**
未执行jQuery前：
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>This is just a test.</p>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>So is this</p>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
alert($("p").eq(1).html())
}
运行：当点击id为test的元素时，alert对话框显示：So is this，即第二个<p>标签的内容

get(num)
说明：获取匹配元素，get(num)返回匹配元素中的某一个元素
参数：get (Number): 期望限制的索引，从0 开始
**例子：**
未执行jQuery前：
<p>This is just a test.</p>
<p>So is this</p>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
alert($("p").get(1).innerHTML);
}
运行：当点击id为test的元素时，alert对话框显示：So is this，即第二个<p>标签的内容

**注意：**get和eq的区别，eq返回的是jQuery对象，get返回的是所匹配的dom对象，所有取$("p").eq(1)对象的内容用jQuery方法html()，而取$("p").get(1)的内容用innerHTML


index(obj)
说明：返回对象索引
参数：obj (Object): 要查找的对象
**例子：**
未执行jQuery前：
<divid="test1"></div>
<divid="test2"></div>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
alert($("div").index(document.getElementById('test1')));
alert($("div").index(document.getElementById('test2')));
}
运行：当点击id为test的元素时，两次弹出alert对话框分别显示0，1

size()或length
说明：当前匹配对象的数量，两者等价
**例子：**
未执行jQuery前：
<imgsrc="1.jpg"/>
<imgsrc="2.jpg"/>
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：functionjq(){
alert($("img").length);
}
运行：当点击id为test的元素时，弹出alert对话框显示2，表示找到两个匹配对象


**二、DOM操作**
**属性**
以<img id="a" src="5.jpg"/>为例，在原始的javascript里面可以用var o=document.getElementById('a')取的id为a的节点对象，在用o.src来取得或修改该节点的src属性。
在jQuery里$("\#a")将得到jQuery对象[ <img id="a" scr="5.jpg"/> ]，然后可以用jQuery提供的很多方法来进行操作，如$("\#a").src()将得到5.jpg，$("\#a").src("1.jpg")将该对象src属性改为1,jpg。下面我们来讲jQuery提供的众多jQuery方法，方便大家快速对DOM对象进行操作
herf()   herf(val)
说明：对jQuery对象属性herf的操作。
**例子：**
未执行jQuery前<ahref="http://blog.csdn.net/ithomer"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
alert($("\#test").attr("href"));
$("\#test").href("http://www.baidu.com");
}
完整HTML脚本：
```python
<html>
<head>
	<title>IT-Homer</title>
	<script src="http://code.jquery.com/jquery-1.10.1.min.js"></script>
	<script language="javascript" type="text/javascript">   
	function jq(){
		alert($("#test").attr("href"));
		$("#test").href("http://www.baidu.com");
	}
	</script>
</head>
<body>
<a href="http://blog.csdn.net/ithomer" id="test" onClick="jq()">jQuery</a>
</body>
</html>
```
运行：先弹出对话框显示id为test的连接url，即 http://blog.csdn.net/ithomer，点击“jQuery”超链接后，将其url改为http://www.baidu.com，当弹出对话框后会看到转向到http://www.baidu.com
同理，jQuery还提供类似的其他方法，大家可以分别试验一下：
herf()  herf(val)   html()  html(val)   id()  id (val)  name()  name (val)   rel()  rel (val)
src()    src (val)   title()  title (val)   val()  val(val)

**操作**
after(html)  在匹配元素后插入一段html
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
$("\#test").after("<b>Hello</b>");
}
执行后相当于：
<a href="\#" id="test" onClick="jq()">jQuery</a>**<b>Hello</b>**


after(elem)  after(elems)  将指定对象elem或对象组elems插入到在匹配元素后
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<pid="test">after</p><ahref="\#"onClick="jq()">jQuery</a>
jQuery代码及功能
functionjq(){
$("a").after($("\#test"));
}
执行后相当于
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<a href="\#" onClick="jq()">jQuery</a>**<p id="test">after</p>**


append(html)在匹配元素内部，且末尾插入指定html
<ahref="\#"id="test"onClick="jq()">jQuery</a>
jQuery代码及功能：
function jq(){
$("\#test").append("<b>Hello</b>");
}
执行后相当于
<ahref="\#"onClick="jq()">jQuery<b>Hello</b></a>
同理还有append(elem)  append(elems) before(html) before(elem) before(elems)请执行参照append和after的方来测试、理解！
appendTo(expr)  与append(elem)相反
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<ahref="\#"onClick="jq()">jQuery</a><pid="test">after</p>
jQuery代码及功能
functionjq(){
$("a"). appendTo ($("\#test"));
}
执行后相当于
<p id="test">after**<a href="\#" onClick="jq()">jQuery</a>**</p>


clone() 复制一个jQuery对象
<pid="test">after</p><ahref="\#"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
$("\#test").clone().appendTo($("a"));
}
复制$("\#test")然后插入到<a>后,执行后相当于
<p id="test">after</p><a href="\#" onClick="jq()">jQuery</a>**<p id="test">after</p>**


empty() 删除匹配对象的所有子节点
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<divid="test">
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<span>span</span>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>after</p>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)</div>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<ahref="\#"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
$("\#test").empty();
}
执行后相当于
<divid="test"></div><ahref="\#"onClick="jq()">jQuery</a>


**insertAfter(expr)  和 insertBefore(expr)**
按照官方的解释和几个简单测试，insertAfter(expr)相当于before(elem)，insertBefore(expr)相当于after (elem)
prepend (html)  prepend (elem)  prepend (elems)   在匹配元素的内部且开始出插入
通过下面例子区分append(elem)  appendTo(expr)  prepend (elem)
<pid="a">p</p>
<div>div</div>
执行$("\#a").append($("div")) 后相当于
<p id="a">
P
**<div>div</div>**
</p>

执行$("\#a").appendTo($("div")) 后 相当于
<div>
div
**<p id="a">p</p>**
</div>

执行$("\#a").prepend ($("div")) 后 相当于
<p id="a">
**<div>div</div>**
P
</p>

remove()  删除匹配对象
注意区分empty()，empty()移出匹配对象的子节点，remove()移出匹配对象

wrap(htm) 将匹配对象包含在给出的html代码内<p>Test Paragraph.</p><ahref="\#"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
$("p").wrap("<div class='wrap'></div>");
}
执行后相当于<div class='wrap'>**<p>Test Paragraph.</p>**</div>


wrap(elem) 将匹配对象包含在给出的对象内<p>Test Paragraph.</p><divid="content"></div>
<ahref="\#"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
$("p").wrap( document.getElementById('content') );
}
执行后相当于
<div id="content">**<p>Test Paragraph.</p>**</div>


**遍历、组合**
add(expr)  在原对象的基础上在附加符合指定表达式的jquery对象
<p>Hello</p><p><span>Hello Again</span></p>
<ahref="\#"onClick="jq()">jQuery</a>
jQuery代码及功能：
functionjq(){
varf=$("p").add("span");
for(vari=0;i < $(f).size();i++){
alert($(f).eq(i).html());
}
}

执行$("p")得到匹配<p>的对象，有两个，add("span")是在("p")的基础上加上匹配<span >的对象，所有一共有3个，从上面的函数运行结果可以看到$("p").add("span")是3个对象的集合，分别是[<p>Hello</p>]，[<p><span>Hello Again</span></p>]，[<span>Hello Again</span>]。
add(el)  在匹配对象的基础上再附加指定的dom元素。
$("p").add(document.getElementById("a"));
add(els)  在匹配对象的基础上在附加指定的一组对象，els是一个数组。
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>Hello</p><p><span>Hello Again</span></p>
jQuery代码及功能：
functionjq(){
varf=$("p").add([document.getElementById("a"), document.getElementById("b")])
for(vari=0;i<$(f).size();i++){
alert($(f).eq(i).html());}
}
注意els是一个数组，这里的[ ]不能漏掉。
ancestors ()  一依次以匹配结点的父节点的内容为对象，根节点除外（有点不好理解，看看下面例子就明白了）
<div>
<p>one</p>
<span>
<u>two</u>
</span>
</div>
jQuery代码及功能：
functionjq(){
varf=$("u").ancestors();
for(vari=0;i<$(f).size();i++){
alert($(f).eq(i).html());}
}
第一个对象是以<u>的父节点的内容为对象，[ <u>two</u> ]
第一个对象是以<u>的父节点的父节点（div）的内容为对象，[<p>one</p><span><u>two</u></span> ]
一般一个文档还有<body>和<html>，依次类推下去。
ancestors (expr)  在ancestors（）的基础上之取符合表达式的对象
如上各例子讲var f改为var f= $("u").ancestors(“div”),则只返回一个对象：
[ <p>one</p><span><u>two</u></span>  ]
children()  返回匹配对象的子介点<p>one</p>
<divid="ch">
<span>two</span>
</div>
jQuery代码及功能：functionjq(){
alert($("\#ch").children().html());
}
$("\#ch").children()得到对象[ <span>two</span> ].所以.html()的结果是”two”


children(expr)  返回匹配对象的子介点中符合表达式的节点<divid="ch">
<span>two</span>
<spanid="sp">three</span>
</div>
jQuery代码及功能
functionjq(){
alert($("\#ch").children(“\#sp”).html());
}
$("\#ch").children()得到对象[<span>two</span><span id="sp">three</span> ].
$("\#ch").children(“\#sp”)过滤得到[<span id="sp">three</span> ]
parent ()  parent (expr)取匹配对象父节点的。参照children帮助理解
contains(str)  返回匹配对象中包含字符串str的对象![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<p>This is just a test.</p><p>So is this</p>
jQuery代码及功能：functionjq(){
alert($("p").contains("test").html());
}
$("p")得到两个对象，而包含字符串”test”只有一个。所有$("p").contains("test")返回 [ <p>This is just a test.</p> ]
end() 结束操作,返回到匹配元素清单上操作前的状态.
**filter(expr)**
filter(exprs)   过滤现实匹配符合表达式的对象 exprs为数组，注意添加“[ ]”
<p>Hello</p><p>Hello Again</p><pclass="selected">And Again</p>
jQuery代码及功能：
functionjq(){
alert($("p").filter(".selected").html())
}
$("p")得到三个对象，$("p").contains("test")只返回class为selected的对象。
**find(expr)**
在匹配的对象中继续查找符合表达式的对象
<p>Hello</p><pid="a">Hello Again</p><pclass="selected">And Again</p>
Query代码及功能：
functionjq(){
alert($("p").find("\#a").html())
}
在$("p")对象中查找id为a的对象。
**is(expr)**
判断对象是否符合表达式,返回boolen值
<p>Hello</p><pid="a">Hello Again</p><pclass="selected">And Again</p>
Query代码及功能：
functionjq(){
alert($("\#a").is("p"));
}
在$("\#a ")是否符合jquery表达式。
大家可以用$("\#a").is("div");  ("\#a").is("\#a")多来测试一下
**next()  next(expr)**
返回匹配对象剩余的兄弟节点
<p>Hello</p><pid="a">Hello Again</p><pclass="selected">And Again</p>
jQuery代码及功能
functionjq(){
alert($("p").next().html());
alert($("p").next(".selected").html());
}
$("p").next()返回 [ <p id="a">Hello Again</p> , <p class="selected">And Again</p> ]两个对象
$("p").next(".selected)只返回 [<p class="selected">And Again</p> ]一个对象
prev ()  prev (expr)  参照next理解
**not(el)  not(expr)**
从jQuery对象中移出匹配的对象，el为dom元素，expr为jQuery表达式
<p>one</p><pid="a">two</p>
<ahref="\#"onclick="js()">jQuery</a>
jQuery代码及功能：functionjs(){
alert($("p").not(document.getElementById("a")).html());
alert($("p").not(“\#a”).html());
}
$("p")由两个对象，排除后的对象为[<p>one</p> ]
**siblings ()**
siblings (expr)  jquery匹配对象中其它兄弟级别的对象
<p>one</p>
<div>
<pid="a">two</p>
</div>
<ahref="\#"onclick="js()">jQuery</a>
jQuery代码及功能：
functionjs(){
alert($("div").siblings().eq(1).html());
}
$("div").siblings()的结果实返回两个对象[<p>one</p>，<a href="\#" onclick="js()">jQuery</a> ]
alert($("div").siblings(“a”)返回一个对象[<a href="\#" onclick="js()">jQuery</a> ]


**其他**
addClass(class)   为匹配对象添加一个class样式
removeClass (class)   将第一个匹配对象的某个class样式移出

**attr (name)**
获取第一个匹配对象的属性
<imgsrc="test.jpg"/><ahref="\#"onclick="js()">jQuery</a>
jQuery代码及功能：functionjs(){
alert($("img").attr("src"));
}
返回**test.jpg**

**attr (prop)**
为第一个匹配对象的设置属性，prop为hash对象，用于为某对象批量添加众多属性
<img/><ahref="\#"onclick="js()">jQuery</a>
jQuery代码及功能：
functionjs(){
$("img").attr({ src: "test.jpg", alt: "Test Image" });
}
运行结果相当于<img src="test.jpg" alt="Test Image"/>
**attr (key,value)**
为第一个匹配对象的设置属性，key为属性名，value为属性值
<img/><a href="\#" onclick="js()">jQuery</a>
jQuery代码及功能
functionjs(){
$("img").attr(“src”,”test.jpg”);
}
运行结果相当于<img src="test.jpg"/>
**removeAttr (name)**
将第一个匹配对象的某个属性移出
<imgalt="test"/><ahref="\#"onclick="js()">jQuery</a>
jQuery代码及功能：
functionjs(){
$("img"). removeAttr("alt");
}
运行结果相当于<img />

**toggleClass (class)**
将当前对象添加一个样式，不是当前对象则移出此样式，返回的是处理后的对象
```python
<html>
<head>
	<title>IT-Homer</title>

	<style type="text/css">
	.selected {
		color: red;
	}
	</style>
	
	<script src="http://code.jquery.com/jquery-1.10.1.min.js"></script>
	<script language="javascript" type="text/javascript">   
		function jq(){
			$("p").toggleClass ("selected") ; 
		}
	</script>

</head>

<body>
<p>Hello</p><p class="selected">IT-Homer</p><a href="#" onclick="jq()">jQuery</a>
</body>

</html>
```
<p> 在点击“Jquery”后，切换字体红色


**三、CSS操作**
Javascript对css的操作相当繁琐
比如<div id="a" style="background:blue">css</div>
取它的background语法是 document.getElementById("a").style.background，
而jQuery对css更方便的操作，$("\#a").background()，$("\#a").background(“red”)

$("\#a")得到jQuery对象[ <div id="a" … /div> ]
$("\#a").background()将取出该对象的background样式。
$("\#a").background(“red”)将该对象的background样式设为red

**jQuery提供了以下方法，来操作css**
background ()   background (val)     color()    color(val)     css(name)    css(prop)
css(key, value)      float()   float(val)   height()   height(val)  width()  width(val)
left()   left(val)       overflow()   overflow(val)   position()   position(val)  top()   top(val)
这里需要讲解一下css(name)  css(prop)  css(key, value)，其他的看名字都知道什么作用了<divid="a"style="background:blue; color:red">css</div><Pid="b">test</P>
css(name)获取样式名为name的样式
$("\#a").css("color") 将得到样式中color值red，("\#a").css("background ")将得到blue
css(prop)prop是一个hash对象，用于设置大量的css样式
$("\#b").css({ color: "red", background: "blue" });
最终效果是<p id="b" style="background:blue; color:red">test</p>,{ color: "red", background: "blue" }，hash对象，color为key，"red"为value，
css(key, value)用于设置一个单独得css样式
$("\#b").css("color","red");最终效果是<p id="b" style="color:red">test</p>


**四、JavaScript处理**
$.browser()  判断浏览器类型，返回boolen值
$(function(){
if($.browser.msie) {
alert("这是一个IE浏览器");}
elseif($.browser.opera) {
alert("这是一个opera浏览器");}
})
当页面载入式判断浏览器类型，可判断的类型有msie、mozilla、opera、safari
**$.each(obj, fn)**
obj为对象或数组，fn为在obj上依次执行的函数，注意区分$().each()
$.each( [0,1,2],function(i){ alert("Item \#"+i+":"+this); });
分别将0，1，2为参数，传入到function(i)中
$.each({ name:"John", lang:"JS"},function(i){ alert("Name:"+i+", Value:"+this);
{ name: "John", lang: "JS" }为一个hash对象，依次将hash中每组对象传入到函数中
**$.extend(obj, prop)**
用第二个对象扩展第一个对象
varsettings={ validate:false, limit:5, name:"foo"};
varoptions={ validate:true, name:"bar"};
$.extend(settings, options);
执行后settings对象为{ validate: true, limit: 5, name: "bar" }
可以用下面函数来测试
$(function(){
varsettings={ validate:false, limit:5, name:"foo"};
varoptions={ validate:true, name:"bar"};
$.extend(settings, options);
$.each(settings,function(i){ alert( i+"="+this); });
})
**$.grep(array,fn)**
通过函数fn来过滤array，将array中的元素依次传给fn，fn必须返回一个boolen，如fn返回true，将被过滤
$(function(){
vararr=$.grep( [0,1,2,3,4],function(i){returni>2; });
$.each(arr,function(i){ alert(i); });
})
我们可以看待执行$.grep后数组[0,1,2,3,4]变成[0，1]
**$.merge(first, second)**
两个参数都是数组，排出第二个数组中与第一个相同的，再将两个数组合并
$(function(){
vararr=$.merge( [0,1,2], [2,3,4] )
$.each(arr,function(i){ alert(i); });
})
可以看出arr的结果为[0,1,2,3,4]
**$.trim(str)**
移出字符串两端的空格
$.trim("   hello, how are you?   ")的结果是"hello, how are you?"


**五、动态效果**
**hide()**
隐藏匹配对象
<pid="a">Hello Again</p><ahref="\#"onClick=’("\#a").hide()’>jQuery</a>
当点击连接时,id为a的对象的display变为none。
show()显示匹配对象
hide(speed)以一定的速度隐藏匹配对象，其大小（长宽）和透明度都逐渐变化到0，speed有3级("slow", "normal",  "fast")，也可以是自定义的速度。
show(speed)以一定的速度显示匹配对象，其大小（长宽）和透明度都由0逐渐变化到正常
hide(speed, callback)  show(speed, callback)当显示和隐藏变化结束后执行函数callback
**toggle()    toggle(speed)**
如果当前匹配对象隐藏，则显示他们，如果当前是显示的，就隐藏，toggle(speed),其大小（长宽）和透明度都随之逐渐变化。
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<imgsrc="1.jpg"style="width:150px"/>
![](http://www.cnblogs.com/Images/OutliningIndicators/None.gif)<ahref="\#"onClick='$("img").toggle("slow")'>jQuery</a>
**fadeIn(speeds)   fadeOut(speeds)**
根据速度调整透明度来显示或隐藏匹配对象，注意有别于hide(speed)和show(speed)，fadeIn和fadeOut都只调整透明度，不调整大小
<imgsrc="1.jpg"style="display:none"/><ahref="\#"onClick='$("img").fadeIn("slow")'>jQuery</a>
点击连接后可以看到图片逐渐显示。
**fadeIn(speed, callback)  fadeOut(speed, callback)**
callback为函数，先通过调整透明度来显示或隐藏匹配对象，当调整结束后执行callback函数
<imgsrc="1.jpg"/>
<ahref="\#"onClick='$("img").fadeIn("slow",function(){ alert("Animation Done."); })'>jQuery</a>
点击连接后可以看到图片逐渐显示,显示完全后弹出对话框
**fadeTo(speed, opacity, callback)**
将匹配对象以speed速度调整倒透明度opacity，然后执行函数callback。Opacity为最终显示的透明度(0-1).
<imgsrc="1.jpg"/><br>
<ahref="\#"onClick='$("img").fadeTo("slow",0.55,function(){ alert("Animation Done."); })'>jQuery</a>
大家可以看一下自己看看效果，如果不用jQuery，编写原始javascript脚本可能很多代码！
**slideDown(speeds)**
将匹配对象的高度由0以指定速率平滑的变化到正常！
<imgsrc="1.jpg"style="display:none"/>
<ahref="\#"onClick='$("img").slideDown("slow")'>jQuery</a>
slideDown(speeds,callback)将匹配对象的高度由0变化到正常！变化结束后执行函数callback
slideUp("slow")  slideUp(speed, callback)匹配对象的高度由正常变化到0
slideToggle("slow")如果匹配对象的高度正常则逐渐变化到0，若为0，则逐渐变化到正常


**六、事件处理**
**hover(Function, Function)**
当鼠标move over时触发第一个function，当鼠标move out时触发第二个function
样式：<style>.red{color:\#FF0000}</style>
Html代码： <div id="a">sdf</div>
jQuery代码及效果
$(function(){
$("\#a").hover(function(){$(this).addClass("red");},
function(){ $(this).removeClass("red");
});
})
最终效果是当鼠标移到id为a的层上时图层增加一个red样式，离开层时移出red样式
**toggle(Function, Function)**
当匹配元素第一次被点击时触发第一个函数，当第二次被点击时触发第二个函数
样式：<style>.red{color:\#FF0000}</style>
Html代码： <div id="a">sdf</div>
jQuery代码及效果
$(function(){
$("\#a"). toggle (function(){$(this).addClass("red");},
function(){ $(this).removeClass("red");
});
})
最终效果是当鼠标点击id为a的层上时图层增加一个red样式，离开层时移出red样式
bind(type, fn)用户将一个事件和触发事件的方式绑定到匹配对象上。
trigger(type)用户触发type形式的事件。$("p").trigger("click")
还有：unbind()   unbind(type)    unbind(type, fn)
Dynamic event(Function)    绑定和取消绑定提供函数的简捷方式
例：
$("\#a").bind("click",function() {
$(this).addClass("red");
})
也可以这样写：
$("\#a").click(function() {
$(this).addClass("red");
});
最终效果是当鼠标点击id为a的层上时图层增加一个red样式，
jQuery提供的函数
用于browers事件
error(fn)    load(fn)     unload(fn)    resize(fn)    scroll(fn)
用于form事件
change(fn)    select(fn)    submit(fn)
用于keyboard事件
keydown(fn)    keypress(fn)    keyup(fn)
用于mouse事件
click(fn)    dblclick(fn)    mousedown(fn)   mousemove(fn)
mouseout(fn)  mouseover(fn)     mouseup(fn)
用于UI事件
blur(fn)    focus(fn)
**以上事件的扩展再扩展为5类**
举例，click(fn) 扩展 click()  unclick()  oneclick(fn)  unclick(fn)
click(fn)：增加一个点击时触发某函数的事件
click()：可以在其他事件中执行匹配对象的click事件。
unclick ()：不执行匹配对象的click事件。
oneclick(fn)：只增加可以执行一次的click事件。
unclick (fn)：增加一个点击时不触发某函数的事件。
上面列举的用于browers、form、keyboard、mouse、UI的事件都可以按以上方法扩展。

**七、Ajax支持**
通用方式：
$.ajax(prop)    通过一个ajax请求，回去远程数据，prop是一个hash表，它可以传递的key/value有以下几种。
(String)type：数据传递方式(get或post)。
((String)url：数据请求页面的url
((String)data：传递数据的参数字符串，只适合post方式
((String)dataType：期待数据返回的数据格式(例如 "xml", "html", "script",或 "json")
((Boolean)ifModified： 当最后一次请求的相应有变化是才成功返回，默认值是false
((Number)timeout:设置时间延迟请求的时间。可以参考$.ajaxTimeout
((Boolean)global：是否为当前请求触发ajax全局事件，默认为true
((Function)error：当请求失败时触发的函数。
((Function)success：当请求成功时触发函数
((Function)complete：当请求完成后出发函数
jQuery代码及说明
$.ajax({url:"ajax.htm",
success:function(msg){
$(div"\#a").html(msg);
}
});
将ajax.htm返回的内容作为id为a的div内容
$.ajax({ url:"ajax.aspx",
type:"get",
dataType:"html",
data:"name=John&location=Boston",
success:function(msg){
$("\#a").html(msg);
}
});
用get方式向ajax.aspx页面传参数，并将返回内容负给id为a的对象。

**$.ajaxTimeout(time)**
设置请求结束时间
$.ajaxTimeout( 5000 )

**$.get(url, params, callback)**
用get方式向远程页面传递参数，请求完成后处理函数，除了url外，其它参数任意选择
$.get("ajax.htm",function(data){ $("\#a").html(data)  })
$.get("ajax.asp",
{ name:"young", age:"25"},
function(data){ alert("Data Loaded:"+data); }
)
$.getIfModified(url, params, callback)用get方式向远程页面传递参数，从最后一次请求后如果数据有变化才作出响应，执行函数callback
$.getJSON(url, params, callback)用get方式向远程json对象传递参数，请求完成后处理函数callback。
$.getScript(url, callback)用get方式载入并运行一个远程javascript文件。请求完成后处理函数callback。
$.post(url, params, callback)用post方式向远程页面传递参数，请求完成后处理函数callback
load(url, params, callback)载入一个远程文件并载入页面DOM中，并执行函数callback
$("\#a").load("ajax.htm",function() { alert("load is done"); } );
向ajax.htm页面发出请求，将返回结果装入id为a的内容中，然后再执行函数callback。
loadIfModified(url, params, callback)用get方式向远程页面传递参数，从最后一次请求后如果数据有变化才作出响应，将返回结果载入页面DOM中，并执行函数callback
ajaxStart(callback)当ajax请求发生错误是时执行函数callback
ajaxComplete(callback)当ajax请求完成时执行函数callback
ajaxError(callback)当ajax请求发生错误时执行函数callback
ajaxStop(callback)当ajax请求停止时执行函数callback
ajaxSuccess(callback)当ajax请求成功时执行函数callback


**八、jQuery插件**
随着jQuery的广泛使用，已经出现了大量jQuery插件，如thickbox，iFX，jQuery-googleMap等，简单的引用这些源文件就可以方便的使用这些插件。
http://jquery.com/plugins（官方推荐）
http://interface.eyecon.ro/demos
[http://www.dyve.net/jquery/](http://www.dyve.net/jquery/)
http://bassistance.de/jquery-plugins



