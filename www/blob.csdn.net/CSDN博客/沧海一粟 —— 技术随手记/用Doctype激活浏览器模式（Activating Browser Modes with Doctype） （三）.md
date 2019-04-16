# 用Doctype激活浏览器模式（Activating Browser Modes with Doctype） （三） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月29日 16:46:00[慢游](https://me.csdn.net/eengel)阅读数：619标签：[浏览器																[browser																[standards																[scripting																[parsing																[xhtml](https://so.csdn.net/so/search/s.do?q=xhtml&t=blog)
个人分类：[Browser](https://blog.csdn.net/eengel/article/category/776807)





**目录**

*本文范围*

*模式（The Modes）*

*针对text/html内容的模式（Modes for text/html Content）*

*怪异**模式（Quirks Mode）*

*标准模式（Standards Mode）*

*准标准模式（Almost Standards Mode）*

*IE7模式（IE7 Mode）*

*针对application/xhtml+xml内容的模式（XML模式）*

*非Web模式*

*效果（The Effects）*

*布局（Layout）*

*解析（Parsing）*

*脚本（Scripting）*

*Doctype嗅探/Doctype转换（Doctype Sniffing aka. Doctype Switching）*

*选择Doctype（Choosing a Doctype）*

*IE8的复杂状况（IE8 Complications）*

*相关页面链接*

*附录：一个对XML的实现者和规范制订者的请求*

*附录：一些text/html Doctype的处理*

*附录：IE8中的模式选择*



**效果（The Effects）**下面是浏览器模式造成的主要效果：


**布局（Layout）**text/html中的模式主要会影响CSS布局（layout）。例如，不能让样式被继承至表格内部是一种怪异的地方（quirk）。对某些浏览器的怪异模式，盒模型（box model）会变更到IE5.5盒模型。这儿就不一一列举所有布局的怪异了。
在准标准模式中（对于那些包含此模式的浏览器来说），和标准模式不同的地方是对于只包含图像的单元格高度的计算。
在XML模式中，选择器（selectors）有不同的区分大小写的行为。此外，针对HTML body的特殊规则并不适用于未执行最新版CSS 2.1的旧版本浏览器。


**解析（Parsing）**也有一些怪异的地方会影响HTML和CSS的解析（parsing），并使得解析规范的页面失败了。这些怪异的地方主要是由怪异的布局（layout）引起。但值得注意的是，对比标准模式，怪异模式主要体现在CSS的布局和解析中，而不是HTML解析中。
有些人误将标准模式当作了“严格解析模式”，以为浏览器会强制执行HTML语法规则，且这样的浏览器可以被用来评估HTML标记是否被正确使用了。但其实不是这样的。浏览器甚至在标准模式起效的时候也会修正“tag soup”[1] （原文：The browsers do tag soup fix-ups even when the Standards mode layout is in effect.）（在2000年Netscape 6发布之前，Mozilla确实有解析模式（parser modes）可以强制执行HTML语法规则。但这些模式由于和目前的web内容不兼容而逐渐被淘汰了。）
另外一个误解是关于XHTML 解析的。一般会认为使用XHTML doctype会导致不同的解析方式。其实不是。text/html的XHTML和HTML采用相同的解析器（parser）。对于浏览器来说，text/html的XHTML只不过是“tag soup with croutons”（这儿或那儿多了个斜杠/）。
只有使用了XML类型（application/xhtml+xml或application/xml）的文档才会触发XML模式，该模式的解析器则和HTML解析器完全不同。


**脚本（Scripting）**虽然怪异模式主要和CSS相关，但还是有一些和脚本（scripting）有关的。比如，Firefox的怪异模式中，HTML中的属性id可以从脚本的全局作用域中建立对象引用，就和IE一样。比起其他浏览器，怪异模式对脚本的影响在IE8中尤其明显。
在XML模式中，有些DOM API的表现简直太不一样了，主要是由于对XML的DOM API行为没有定义成与HTML的行为兼容。



[1] [http://en.wikipedia.org/wiki/Tag_soup](http://en.wikipedia.org/wiki/Tag_soup)](https://so.csdn.net/so/search/s.do?q=parsing&t=blog)](https://so.csdn.net/so/search/s.do?q=scripting&t=blog)](https://so.csdn.net/so/search/s.do?q=standards&t=blog)](https://so.csdn.net/so/search/s.do?q=browser&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)




