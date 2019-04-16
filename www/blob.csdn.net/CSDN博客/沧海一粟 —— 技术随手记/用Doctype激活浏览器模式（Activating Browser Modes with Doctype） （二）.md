# 用Doctype激活浏览器模式（Activating Browser Modes with Doctype） （二） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月28日 13:35:00[慢游](https://me.csdn.net/eengel)阅读数：660标签：[浏览器																[browser																[standards																[ie																[xml																[opera](https://so.csdn.net/so/search/s.do?q=opera&t=blog)
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



由于历史原因，目前写web文档有两种方式，一种是按照web标准写，另一种是按照90年代后期流行的方式写。而当代的浏览器为了同时兼容这两种web文档，实现了多种引擎模式（engine modes）。本文接下来就介绍这些模式都是什么，以及它们都是怎么被触发的。



**本文范围**

本文会涵盖各种模式转换（mode switching），适用的浏览器有：Firefox和其他基于Gecko的浏览器，Safari、Chrome和其他基于WebKit的浏览器，Opera，Konqueror，用于Mac和Windows的IE，以及其它内嵌IE的浏览器。

本文侧重描述模式选择机制（mode selection mechanism），而不关心每种模式的具体行为。



**模式（The Modes）**

以下列举各种不同的模式。



**针对text/html内容的模式（Modes for text/html Content）**

对text/html内容进行模式选择依赖于doctype嗅探（doctype sniffing）（本文后面讨论）。IE8中的模式选取还取决于其他因素。不过，只要不在微软的非法网站黑名单中，默认即使在IE8中，模式也依赖于doctype。

**需要一再强调的是，模式的确切行为因浏览器而异，即使本文已做统一讨论。**


**怪异模式（Quirks Mode）**

在怪异模式中，浏览器没有遵循当下的web格式规范，以避免破坏根据90年代末流行的方式所创作的web文档。而不同的浏览器实现的怪异模式也不一样。在IE6，7和8中，Quirks模式是有效冻结的IE5.5。（原文：In Internet Explorer 6, 7 and 8, the Quirks mode is effectively frozen IE 5.5.）而在其他浏览器中，怪异模式与准标准模式的差别不大。

如果你是现在开始制作新页面，我的建议是最好遵守相关规范（特别是CSS2.1）并使用标准模式。


**标准模式（Standards Mode）**

在标准模式下，一个特定的浏览器会在某种程度上尽量正确地对待遵守规范的web文档。

而由于不同的浏览器兼容的阶段不同，标准模式也不只是一个单一的目标。

HTML5中，标准模式被称为“非怪异模式（no quirks mode）”。


**准标准模式（Almost Standards Mode）**

Firefox，Safari，Chrome，Opera（从7.5版本开始）和IE8还有一个称为“准标准模式”的模式，它用传统方法实现单元格的垂直尺寸计算，而没有严格遵循CSS2规范。 Mac上的IE5，Windows中的IE 6和7，7.5版本前的Opera和Konqueror不需要准标准模式，因为他们在各自的标准模式中也没有严格按照CSS2规范实现单元格的垂直尺寸计算。其实，比起Mozilla的标准模式，它们所谓的标准模式更接近Mozilla的准标准模式。

HTML5称此模式为“有限怪异模式（limited quirks mode）”。


**IE7模式（IE7 Mode）**

IE8还有一个模式，差不多是IE7标准模式的冻结副本（frozen copy）。其他浏览器没有这样的模式，这种模式也不符合HTML5规范。

**针对application/xhtml+xml内容的模式（XML Mode）**

在Firefox，Safari，Chrome和Opera中，将HTTP Content-type设置成application/xhtml+xml会触发XML模式。这些浏览器会在某种程度上尽量正确地对待遵守规范的XML文档。

IE 6, 7和8及Mac IE 5不支持application/xhtml+xml。

在基于WebKit的Nokia S60浏览器中，HTTP Content-type为application/xhtml+xml也不会触发XML模式，主要是为了兼容“walled garden”[1]
里面的不规范内容。（以往的“移动浏览器”没有采用真正的XML parser，因此不规范的内容也会被标记为XML。）

我还没有充分测试过Konqueror，因此没法确切说明这个浏览器是如何处理的。

**非Web模式（Non-Web Modes）**

有些引擎有针对非web内容的模式。这儿为了叙述的完备性，略微涉及一点。Opera有一个WML 2.0模式。Leopard上的WebKit有一个特殊模式是专门针对原先的Dashboard widgets。



[1] http://www.cnpaf.net/Class/w/200510/9725.html](https://so.csdn.net/so/search/s.do?q=xml&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=standards&t=blog)](https://so.csdn.net/so/search/s.do?q=browser&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)




