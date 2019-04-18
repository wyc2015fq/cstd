# display:inline、block、inline-block的区别 - z69183787的专栏 - CSDN博客
2013年11月17日 08:42:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1126
display:block就是将元素显示为块级元素.
　　block元素的特点是：
　　总是在新行上开始；
　　高度，行高以及顶和底边距都可控制；
　　宽度缺省是它的容器的100%，除非设定一个宽度
　　<div>, <p>, <h1>, <form>, <ul> 和 <li>是块元素的例子。
　　display:inline就是将元素显示为行内元素.
　　inline元素的特点是： 
　　和其他元素都在一行上；
　　高，行高及顶和底边距不可改变；
　　宽度就是它的文字或图片的宽度，不可改变。
　　<span>, <a>, <label>, <input>, <img>, <strong> 和<em>是inline元素的例子。
　　inline和block可以控制一个元素的行宽高等特性，需要切换的情况如下：
　　让一个inline元素从新行开始；
　　让块元素和其他元素保持在一行上；
　　控制inline元素的宽度（对导航条特别有用）；
　　控制inline元素的高度；
　　无须设定宽度即可为一个块元素设定与文字同宽的背景色。
　　display:inline-block将对象呈递为内联对象，但是对象的内容作为块对象呈递。旁边的内联对象会被呈递在同一行内，允许空格。
　　inline-block的元素特点：
　　将对象呈递为内联对象，但是对象的内容作为块对象呈递。旁边的内联对象会被呈递在同一行内，允许空格。(准确地说，应用此特性的元素呈现为内联对象，周围元素保持在同一行，但可以设置宽度和高度地块元素的属性)
　　并不是所有浏览器都支持此属性，目前支持的浏览器有：Opera、Safari在IE中对内联元素使用display:inline-block，IE是不识别的，但使用display:inline-block在IE下会触发layout，从而使内联元素拥有了display:inline-block属性的表症。从上面的这个分析，也不难理解为什么IE下，对块元素设置display:inline-block属性无法实现inline-block的效果。这时块元素仅仅是被display:inline-block触发了layout，而它本就是行布局，所以触发后，块元素依然还是行布局，而不会如Opera中块元素呈递为内联对象。
　　IE下块元素如何实现display:inline-block的效果？
　　有两种方法：
　　1、先使用display:inline-block属性触发块元素，然后再定义display:inline，让块元素呈递为内联对象（两个display要先后放在两个CSS声明中才有效果，这是IE的一个经典bug，如果先定义了display:inline-block，然后再将display设回inline或block，layout不会消失）。代码如下（...为省略的其他属性内容）：
　　div {display:inline-block;...} 
　　div {display:inline;}
　　2、直接让块元素设置为内联对象呈递（设置属性display:inline），然后触发块元素的layout（如：zoom:1等）。代码如下：
　　div {display:inline; zoom:1;...}
　　以下用个例子来说明三者的区别和用
<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "[http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd](http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd)">
<html xmlns="[http://www.w3.org/1999/xhtml](http://www.w3.org/1999/xhtml)">
<head>
<meta http-equiv="Content-Type" content="text/html; charset=gb2312" />
<title>display:inline、block、inline-block的区别</title>
</head>
<style>
div,span{background-color:green;margin:5px;border:1px solid #333;padding:5px;height:52px;color:#fff;}
.b{display:block;}
.i{display:inline;}
div.ib{display:inline-block;}
div.ib{display:inline;}
a.ib{display:inline-block;}
a.ib{display:block;}
span.v{padding:0;margin:0;border:0;vertical-align:middle;height:100%}
</style>
<body>
<div>div display:block</div>
<div class="i">div display:inline</div>
<div class="ib">div display:inline-block</div>
<span>span display:inline</span>
<span class="b">span display:block</span>
<span><a class="ib">a display:block</a></span><br />
<div><span class="v"></span>vertical-align:middle</div>
</body>
</html>
