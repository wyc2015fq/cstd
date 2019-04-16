# 用Doctype激活浏览器模式（Activating Browser Modes with Doctype） （一） - 沧海一粟 —— 技术随手记 - CSDN博客





2011年01月28日 12:53:00[慢游](https://me.csdn.net/eengel)阅读数：849








英文链接为：[http://hsivonen.iki.fi/doctype/](http://hsivonen.iki.fi/doctype/)



在开始正式的译文前，先介绍一下背景，以及我为什么要翻译这篇文章。

**Doctype **

即HTML与XHTML中的文档声明，作用是告知浏览器当前文档所使用的是哪种HTML或XHTML规范。[1]

比如当用Adobe Dreamwaver CS3新建一个HTML文档时，它会自动在文档头部加入一句Doctype声明：

<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN" "http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">

当浏览器请求并打开HTML文件时，里面的Doctype声明会指导浏览器如何解析、呈现这个文件的内容。而对应不同的Doctype, 浏览器采取的不同解析呈现方式就叫做 **浏览器模式**

（**Browser Mode**

）。

读这篇文章，可以了解不同的浏览器模式，知道它们都是怎么被激活的，当然也可以指导我们平时做web文档时如何声明Doctype。

初次翻译，有些地方翻译得不恰当的请多包涵了！



**目录**



本文范围

模式（The Modes）

针对text/html内容的模式（Modes for text/html Content）

怪异模式（Quirks Mode）

标准模式（Standards Mode）

准标准模式（Almost Standards Mode）

IE7模式（IE7 Mode）

针对application/xhtml+xml内容的模式（XML模式）

非Web模式

效果（The Effects）

布局（Layout）

解析（Parsing）

脚本（Scripting）

Doctype嗅探/Doctype转换（Doctype Sniffing aka. Doctype Switching）

选择Doctype（Choosing a Doctype）

IE8的复杂状况（IE8 Complications）

相关页面链接

附录：一个对XML的实现者和规范制订者的请求

附录：一些text/html Doctype的处理

附录：IE8中的模式选择





[1]DOCTYPE，百度百科，http://baike.baidu.com/view/73768.htm



[](#_Toc283984170)



