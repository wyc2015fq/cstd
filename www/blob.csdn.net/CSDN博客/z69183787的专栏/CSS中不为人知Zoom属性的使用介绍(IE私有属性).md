# CSS中不为人知Zoom属性的使用介绍(IE私有属性) - z69183787的专栏 - CSDN博客
2014年02月08日 09:55:30[OkidoGreen](https://me.csdn.net/z69183787)阅读数：1445
其实Zoom属性是IE浏览器的专有属性，Firefox等浏览器不支持。它可以设置或检索对象的缩放比例。除此之外，它还有其他一些小作用，比如触发ie的hasLayout属性，清除浮动、清除margin的重叠等。 
**Zoom的使用方法： **zoom :﻿ normal | number 
normal :　 默认值。使用对象的实际尺寸 
number :　 百分数 | 无符号浮点实数。浮点实数值为1.0或百分数为100%时相当于此属性的 normal 值用白话讲解就是zoom:后面的数字即放大的倍数，可以是数值，也可以是百分比。如：zoom:1，zoom:120%。而这个属性只要在IE中才起作用，所以很少用到它的实际用途，而最经常用到作用是清除浮动等，如： 
复制代码
代码如下:
.border{ 
border:1px solid #CCC; 
padding:2px; 
overflow:hidden; 
_zoom:1; 
} 
_zoom是CSS hack中专对IE6起作用的部分。IE6浏览器会执行zoom:1表示对象的缩放比例，但这里 
overflow:hidden;和_zoom:1;是连起来用的，作用是清除border内部浮动。 
同理，还可以使用同样方法清除margin属性在IE浏览器中的重叠问题。
**CSS中zoom:1的作用 **
兼容IE6、IE7、IE8浏览器，经常会遇到一些问题，可以使用zoom:1来解决，有如下作用： 
触发IE浏览器的haslayout 
解决ie下的浮动，margin重叠等一些问题。 
比如，本站使用DIV做一行两列显示，HTML代码： 
复制代码
代码如下:
<div class="h_mainbox"> 
<h2>推荐文章</h2> 
<ul class="mainlist"> 
<li><a href="#" style="color:#0000FF" target="_blank">CSS库吧</a></li> 
<li><a href="#" style="color:#0000FF" target="_blank">原创< /a></li> 
</ul> 
</div> 
CSS代码： 
复制代码
代码如下:
.h_mainbox { border:1px solid #dadada; padding:4px 15px; background:url(../mainbox_bg.gif) 0 1px repeat-x; margin-bottom:6px; overflow:hidden} 
.h_mainbox h2 { font-size:12px; height:30px; line-height:30px; border-bottom:1px solid #ccc; color:#555;} 
.h_mainbox h2 span { float:right; font-weight:normal;} 
.h_mainbox ul { padding:6px 0px; background:#fff;} 
.mainlist { overflow:auto; zoom:1;} 
.h_mainbox li { width:268px; float:left; height:24px; overflow:hidden; background:url(../icon3.gif) 0 6px no-repeat; padding:0px 5px 0px 18px; line-height:200%;} 
.mainlist里面的zoom:1的那里就可以在IE6、IE7、IE8正常显示效果了。 
**css中的zoom的作用 **
1、检查页面的标签是否闭合 
不要小看这条，也许折腾了你两天都没有解决的 CSS BUG 问题，却仅仅源于这里。毕竟页面的模板一般都是由开发来嵌套的，而他们很容易犯此类问题。 
快捷提示：可以用 Dreamweaver 打开文件检查，一般没有闭合的标签，会黄色背景高亮。 
　　2、样式排除法 
有些复杂的页面也许加载了 N 个外链 CSS 文件，那么逐个删除 CSS 文件，找到 BUG 触发的具体 CSS 文件，缩小锁定的范围。 
　　对于刚才锁定的问题 CSS 样式文件，逐行删除具体的样式定义，定位到具体的触发样式定义，甚至是具体的触发样式属性。 
　　3、模块确认法 
有时候我们也可以从页面的 HTML 元素出发。删除页面中不同的 HTML 模块，寻找到触发问题的 HTML 模块。 
　　4、检查是否清除浮动 
其实有不少的 CSS BUG 问题是因为没有清除浮动造成的。养成良好的清除浮动的习惯是必要的，推荐使用 无额外 HTML 标签的清除浮动的方法（尽量避免使用 overflow:hidden;zoom:1 的类似方法来清除浮动，会有太多的限制性）。 
　　5、检查 IE 下是否触发 haslayout 
很多的 IE 下复杂 CSS BUG 都与 IE 特有的 haslayout 息息相关。熟悉和理解 haslayout 对于处理复杂的 CSS BUG 会事半功倍。推荐阅读 old9 翻译的 《On having layout》（如果无法翻越穿越伟大的 GFW，可阅读 蓝色上的转帖 ） 
快捷提示：如果触发了 haslayout，IE 的调试工具 IE Developer Toolbar 中的属性中将会显示 haslayout 值为 -1。 
　　6、边框背景调试法 
故名思议就是给元素设置显眼的边框或者背景（一般黑色或红色），进行调试。此方法是最常用的调试 CSS BUG 的方法之一，对于复杂 BUG 依旧适用。经济实惠还环保^^ 
最后想强调一点的是，养成良好的书写习惯，减少额外标签，尽量语义，符合标准，其实可以为我们减少很多额外的复杂 CSS BUG，更多的时候其实是我们自己给自己制造了麻烦。希望你远离 BUG ，生活越来越美好。
