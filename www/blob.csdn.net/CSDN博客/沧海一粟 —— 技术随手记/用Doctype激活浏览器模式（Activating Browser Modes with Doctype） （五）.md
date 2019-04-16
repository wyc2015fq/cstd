# 用Doctype激活浏览器模式（Activating Browser Modes with Doctype） （五） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月12日 10:27:00[慢游](https://me.csdn.net/eengel)阅读数：1289标签：[浏览器																[browser																[microsoft																[ie																[standards																[scripting](https://so.csdn.net/so/search/s.do?q=scripting&t=blog)
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



**IE8的复杂状况**

在[另外一个单独的列表](http://www.alistapart.com/articles/beyonddoctype)中声明了IE8在doctype之外，会另外采用一个meta元素作为模式转换的控制。（请参考[Ian Hickson](http://ln.hixie.ch/?start=1201080691&count=1)， [David Baron](http://lists.w3.org/Archives/Public/public-html/2007Apr/0279.html)， [David Baron](http://dbaron.org/log/2008-01#e20080124a)（又）， [Robert O’Callahan](http://weblogs.mozillazine.org/roc/archives/2008/01/post_2.html)及[Maciej Stachowiak](http://webkit.org/blog/155/versioning-compatibility-and-standards/)的解释。）
IE 8有四种模式：IE 5.5怪异模式，IE 7标准模式，IE 8准标准模式和IE 8标准模式。对模式的选择依赖于多种手段：doctype，一个meta元素，一个HTTP头，从Microsoft定期下载的数据，局域网域，用户的设置，局域网网管的设置，frame父节点（如果有的话）的模式，以及用户可控的UI按钮。（对于那些嵌入了浏览器引擎的应用，模式的选择也取决于那个应用。）
幸运的是，IE 8基本上会和其他浏览器一样使用doctype嗅探，如果下面的条件满足的话：


 ●网页制作者没有将HTTP头设置成X-UA-Compatible。
 ●网页制作者没有添加X-UA-Compatible meta元素。
 ●Microsoft没有将此站点的域名放到[黑名单](http://go.microsoft.com/fwlink/?LinkId=145413)中。
 ●局域网网管没有没有将此站点加入黑名单。
 ●用户没有按下兼容性视图（Compatibility View）按钮（或者相反将此站点加入了用户自定义的黑名单中）。
 ●此站点不在局域网域中。
 ●用户没有选择将所有站点都按IE 7的方式显示。
 ●此页面没有被封装在兼容性模式（Compatibility Mode）页面中。

除了一开始的两个关于X-UA-Compatible的条件，其他条件IE 8都会和IE 7里一样做doctype嗅探。这种模拟IE 7的视图就叫兼容性视图（Compatibility View）。
若网页满足那两个X-UA-Compatible的条件，IE 8的处理将会和其他浏览器完全不同。请参考[本文附录](http://hsivonen.iki.fi/doctype/#ie8modes)或[PDF](http://hsivonen.iki.fi/doctype/ie8-mode.pdf)和[PNG](http://hsivonen.iki.fi/doctype/ie8-mode.png)格式的流程图。
不幸的是，若没有X-UA-Compatible的HTTP头或meta元素，IE 8会随机地从IE 8标准模式转换为模拟IE 7标准模式的IE 7模式，哪怕你设置了正确的doctype。更糟的是，一个局域网网管就可以做这件事。另外，Microsoft也有可能将你访问的整个站点都归入黑名单中（比如mit.edu!）。
为了防止这些情况，只设置doctype是不够的，还需要设置X-UA-Compatible的HTTP头或meta元素。


这儿有一些简单的规则用于为新建text/html文档选择X-UA-Compatible的HTTP头或meta元素。假设这样的文档已经有一个doctype，并且在其他浏览器中能够触发标准模式或准标准模式。
**你的域名不在Microsoft的黑名单中，并且相比较保证用户不会退回到用IE 7模式进行渲染，你更关心不必有针对不同浏览器的差异化处理。**        你不需要包含X-UA-Compatible的HTTP头或meta元素。
**你的站点在Microsoft的黑名单中，或者你的站点（比如iki.fi！）有别的创作人，而他们的坏站点（broken sites）会诱导访问者对整个站点开启兼容性视图，或者你比较关心Google或Digg能够构架你的站点，或者你想确保用户不能开启兼容性视图**        或者在你的页面中加入如下meta元素（HTML5中是非法的）<meta http-equiv="X-UA-Compatible" content="IE=Edge">  （注意，一定要在所有script元素之前！），或者设置你的页面的HTTP头为：X-UA-Compatible: IE=Edge。
**你的站点在IE 7中正常工作但在IE 8中不行**        首先，或者在你的页面中加入如下meta元素（HTML5中是非法的）<meta http-equiv="X-UA-Compatible" content="IE=EmulateIE7">（注意，一定要在所有script元素之前！），或者设置你的页面的HTTP头为：X-UA-Compatible: IE=EmulateIE7。
        然后修正你的站点为不回复任何不标准IE 7行为，并迁移到IE=Edge。




**相关页面链接**

 ●Eric Meyer 在[Use the Right Doctype](http://www.oreillynet.com/pub/a/network/2000/04/14/doctype/index.html)中撰写了有关Mac IE 5中的模式 
 ●[Mozilla’s DOCTYPE sniffing](http://www.mozilla.org/docs/web-developer/quirks/doctypes.html)，David Baron 
 ●[CSS Enhancements in Internet Explorer 6](http://msdn.microsoft.com/library/default.asp?url=/library/en-us/dnie60/html/cssenhancements.asp)，Lance Silver，讨论了Windows IE 6中的模式及doctype嗅探  
 ●[The Opera 9 DOCTYPE Switches](http://www.opera.com/docs/specs/doctype/) ●Faruk Ateş在[IE8 and the X-UA-Compatible situation](http://farukat.es/journal/2009/05/245-ie8-and-the-x-ua-compatible-situation)中讨论了IE8](https://so.csdn.net/so/search/s.do?q=standards&t=blog)](https://so.csdn.net/so/search/s.do?q=ie&t=blog)](https://so.csdn.net/so/search/s.do?q=microsoft&t=blog)](https://so.csdn.net/so/search/s.do?q=browser&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)




