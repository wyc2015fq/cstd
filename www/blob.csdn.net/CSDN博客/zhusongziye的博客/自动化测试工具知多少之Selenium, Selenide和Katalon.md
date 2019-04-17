# 自动化测试工具知多少之Selenium, Selenide和Katalon - zhusongziye的博客 - CSDN博客





2018年10月28日 19:29:13[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：97








引子

之前就有个想法，整理一下自动化测试的相关工具，开始的时候以为不复杂，但是只是简单的列表似乎又单调了些，没什么意义。如果加上介绍和相关说明，放在一篇文章里，又会太过杂乱。

干脆还是先从细节开始，写个系列文章来介绍工具，后面再做整理，可能会清楚一点。

从哪儿开始呢，要不咱先唠唠Selenium？



## Selenium

这是鼎鼎大名家喻户晓的工具了，大家应该都知道，它是做Web UI自动化的。

但是初次接触的朋友可能还是会有点懵逼，什么Selenium1.0, 2.0, 3.0, 什么seleniumRC、Selenium IDE，Selenium Grid，还有WebDriver又是什么鬼？所以咱也来先简单介绍两句。

## 1.0

Selenium 1.0 = Selenium IDE + Selenium Grid + Selenium RC


Selenium IDE是嵌入到Firefox浏览器中的一个插件，实现简单的浏览器操作的录制与回放功能。


Selenium Grid是一种自动化的测试辅助工具，Grid通过利用现有的计算机基础设施，能加快Web-App的功能测试。利用Grid可以很方便地实现在多台机器上和异构环境中运行测试用例。


Selenium RC（Remote Control）是Selenium1.0的核心部分。Selenium RC 支持多种不同语言编写的自动化测试脚本，通过Selenium RC的服务器作为代理服务器去访问应用，从而达到测试的目的。


Selenium RC分为Client Libraries和Selenium Server。Client Libraries库主要用于编写测试脚本，用来控制Selenium Server的库。Selenium Server负责控制浏览器行为。

关于1.0的介绍，简单看看就好了，因为在2.0中，核心已经从RC变成了WebDriver。

## WebDriver

在2006年的时候，Google的工程师Simon Stewart发起了WebDriver的项目。

Selenium RC 是在浏览器中运行JavaScript应用，使用浏览器内置的JavaScript翻译器来翻译和执行selenese命令（selenese是Selenium命令集合）。

WebDriver是通过原生浏览器支持或者浏览器扩展来直接控制浏览器。WebDriver针对各个浏览器而开发，取代了嵌入到被测Web应用中的JavaScript，与浏览器紧密集成，因此支持创建更高级的测试，避免了JavaScript安全模型导致的限制。除了来自浏览器厂商的支持之外，WebDriver还利用操作系统级的调用，模拟用户输入。

Selenium与WebDriver原是属于两个不同的项目，WebDriver的创建者Simon Stewart早在2009年8月的一份邮件中解释了项目合并的原因。

> Selenium与WebDriver合并原因：为何把两个项目合并？部分原因是WebDriver解决了Selenium存在的缺点（例如能够绕过JavaScript沙箱，我们有出色的API），部分原因是Selenium解决了WebDriver存在的问题（例如支持广泛的浏览器），部分原因是因为Selenium的主要贡献者和我都觉得合并项目是为用户提供最优秀框架的最佳途径。

## 2.0

Selenium和Webdriver合并之后，就有了Selenium 2.0。


Selenium 2.0 = Selenium 1.0 + WebDriver

在Selenium 2.0中主推的是WebDriver，可以将其看作Selenium RC的替代品。因为Selenium为了保持向下的兼容性，所以在Selenium 2.0中并没有彻底地抛弃Selenium RC。

现在网上关于Selenium的教程，大多也是关于Selenium 2.0的WebDriver的。

## 3.0

2016年7月，Selenium3.0悄悄发布第一个beta版。2016年10月13日，官方宣布发布Selenium3.0版本。


在Selenium 3.0中，彻底移除了旧的RC APIs，替换成了WebDriver APIs。

Selenium3.0只支持Java8版本以上。


Selenium3.0中的Firefox浏览器驱动独立了，以前装完selenium2就可以驱动Firefox浏览器了，现在和Chrome一样，必须下载和设置浏览器驱动。


MAC OS 集成Safari的浏览器驱动。默认在/usr/bin/safaridriver 目录下。

只支持IE 9.0版本以上。

## Selenide

初看到这个词的时候，是不是以为把Selenium拼错了？我一开始也这样认为，但是一搜才发现，并不是！

> selenide is just a wrapper.

Selenide是webdriver的一个封装，理论上一切WebDriver能做的Selenide都能做，并且现在只有java版本。

使用Selenide，你可以把注意力集中在实现业务逻辑上，而不用过多的考虑浏览器/ajax/超时等问题。

想要了解的更具体一点？

请参考官方文章：

http://selenide.org/2013/04/23/what-is-selenide/



## Katalon

这是在Selemium相同的内核上构建起来的一个自动化测试工具。它是一个完全免费且易用的图形化工具，用户不需要任何编程基础即可使用。

至于它和Selenium的具体区别？我懒得翻译了，大家去看官方文章来具体了解吧。

官方文章：

https://www.katalon.com/resources-center/blog/katalon-studio-vs-selenium-based-open-source-frameworks/



