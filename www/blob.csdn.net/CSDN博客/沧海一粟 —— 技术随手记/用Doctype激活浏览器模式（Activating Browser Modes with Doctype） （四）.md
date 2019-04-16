# 用Doctype激活浏览器模式（Activating Browser Modes with Doctype） （四） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年02月11日 16:05:00[慢游](https://me.csdn.net/eengel)阅读数：635标签：[浏览器																[browser																[文档																[html																[xhtml																[standards](https://so.csdn.net/so/search/s.do?q=standards&t=blog)
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



# Doctype嗅探/Doctype转换（Doctype Sniffing aka. Doctype Switching）

现在的浏览器用doctype嗅探（doctype sniffing）方式来为text/html文档确定浏览器引擎模式。这意味着模式将会根据HTML文档开头的文档类型声明（document type declaration）来选定。（不适用于XML文档。）

文档类型声明doctype是根据SGML语法实现的。SGML是一种标记（markup）语言的架构，HTML5之前的HTML定义都来源于SGML。在HTML 4.01规范中，文档类型声明被看作是用于和HTML 版本信息（version information）交流的。抛开它的名字“文档类型声明”，抛开HTML 4.01规范所说的“版本信息”，文档类型声明并不是用来将SGML或XML当作一种特殊文档类型加以区分的合适手段，虽然看起来它是这样被期望的（从它的名字就能看出来）。（更多信息请查阅附录。）

HTML 4.01规范和ISO 8879（SGML）中都没有任何关于如何根据文档类型声明转换引擎模式的说明。Doctype嗅探的产生是基于以下实际情况：当doctype嗅探出来的那会，绝大多数“怪异的”文档既没有文档类型声明，也没有引用老的DTD。HTML5认识到了这个事实，而将text/html中的doctype定义为专门为了模式转换。

一个典型的前HTML5文档类型声明包含字符串“<!DOCTYPE”，根元素的通用标识符（“html”），字符串“PUBLIC”，一个引号括起来的DTD公共标识符，也可能包含一个此DTD的系统标识符（一个URL），以及字符“>”。这些东西都用空格隔开。此文档类型声明放在文档中的根元素之前。

# 选择Doctype（Choosing a Doctype）

**text/html**

下面是如何为一个新建text/html文档选择doctype的简单指导。

**标准模式，最新的有效性验证方式**

*< !DOCTYPE html>*

如果你想使用新特性如<video>，<canvas>和ARIA，你最好用这个模式。记住HTML5的有效性定义一直处于变动之中。请确保你在Firefox，Safari，Chrome，Opera 9或10中测试过你的图片的对齐了。在IE中测试图片对齐没什么用，不过还是要确保在IE 8中测试一下。

**标准模式，更稳定的有效性验证目标**

*<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01//EN" "http://www.w3.org/TR/html4/strict.dtd">*

这种doctype也会触发标准模式，并且十年前的老HTML 4.01有效性定义也比较稳定。请确保你在Firefox，Safari，Chrome，Opera 9或10中测试过你的图片的对齐。和上面一样，在IE中测试图片对齐没什么用，不过还是要确保在IE 8中测试一下。

**你会喜欢上用标准模式，不过你也许仍想用一些非标准的标记，或在表格布局中采用切片图片（sliced images****）而不想修改它们**

*<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">*

这样可以触发准标准模式（在非常老的Mozilla浏览器中就是完全的标准模式了）。请注意如果你在表格中嵌入切片图片，而后来又要升级到HTML5（也就是完全的标准模式），那你的布局有可能会被破坏。

**如果你执意要使用怪异模式**

没有doctype。

请不要做这种事。执意用怪异模式会一直纠缠你、你的合作者甚至将来的接手人——那个时候已经没人关心Windows IE 6了（就如现在已经没人关心Netscape 4.x 和IE 5一样）。相信我。用怪异模式不是一个好主意。

如果你还想支持IE 6，最好在需要的地方用条件注释（conditional comments）做特殊处理，而不是让别的浏览器也倒退回怪异模式。



我不推荐使用任何XHTML doctype，因为用XHTML来标记text/html被认为是有害的。如果你还是要使用XHTML doctype，请注意XML声明会在IE 6（IE 7不会！）中触发怪异模式。



**application/xhtml+xml**

application/xhtml+xml文档最简单的做法就是完全不使用doctype。这样网页就不会“严格遵照（strictly conforming）”XHTML 1.0，不过这样做也没关系。（请查阅下面的附录。）](https://so.csdn.net/so/search/s.do?q=xhtml&t=blog)](https://so.csdn.net/so/search/s.do?q=html&t=blog)](https://so.csdn.net/so/search/s.do?q=文档&t=blog)](https://so.csdn.net/so/search/s.do?q=browser&t=blog)](https://so.csdn.net/so/search/s.do?q=浏览器&t=blog)




