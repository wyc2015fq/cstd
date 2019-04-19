# 自动化测试工具- selenium介绍 - 零度的博客专栏 - CSDN博客
2017年05月25日 10:51:52[零度anngle](https://me.csdn.net/zmx729618)阅读数：1103
转自：[http://www.cnblogs.com/TankXiao/p/5252754.html](http://www.cnblogs.com/TankXiao/p/5252754.html)
Selenium 是目前用的最广泛的Web UI 自动化测试框架。 本系列文章，将深入简出来讲解selenium 的用法文章的末尾处， 有整个系列的链接
# selenium 的命名
selenium 的意思是 硒   (有点QTP 杀手的意思)
QTP mercury 是水银     硒可以对抗水银
QTP 目前的价格是  三个单机版 十万人民币左右， 越来越少的人用QTP了
# 什么是selenium
一套软件工具，用来支持不同的自动化测试方法
开源软件：可以根据需要来增加重构工具的某些功能
跨平台：linux ， windows mac
核心功能就是可以在多个浏览器上进行自动化测试
支持多种编程语言
目前已经被google , 百度， 腾讯等公司广泛使用
能够实现类似商业工具的大部分功能，并且还实现了商业工具不能支持的功能
建议大家整体学习整套工具， 将来好选择适合自己测试目的的测试工具。 
# Selenium 的发展历史
2004年在ThoughtWorks 公司， 一个叫做Jason Huggins为了减少手工测试的工作， 自己写了一套Javascript的库， 这套库可以进行页面交互， 并且可以重复的在不同浏览器上进行重复的测试操作。
这套库后来变为了Selenium Core. 为Selenium Remote Control (RC) 和Selenium IDE 提供了坚实的核心基础能力
Selenium 的作用是划时代的，因为他允许你使用多种语言来控制浏览器。
浏览器的对JS的安全限制也对Selenium的发展带来了困扰，并且Web程序也越来越大，特性也越来越多，都对selenium的发展来说带来了不少困难
2006年Google 的工程师Simon Stewart开启了一个叫做WebDriver的项目， 此项目可以直接让测试工具使用浏览器和操作系统本身提供的方法， 借此来绕过JS环境的沙盒效应， WebDriver项目目标就是为了解决Selenium的痛处
2008年北京奥运年会， Selenium 和 WebDriver 这两个项目进行了合并， Selenium 2.0 出现了，也就是大家说的WebDriver
# Selenium 的工具套件
selenium 2 (aka. Selenium Webdriver)提供了极佳的测试工具特性， 例如:关联的面向对象API
Selenium 1 (aka. Selenium RC or Remote Control) 支持更多的浏览器，支持更多的编程语言(Java, JavaScript, Ruby PHP)
Selenium IDE (集成开发环境) 是Firefox 的插件， 有图形界面来录制和回放脚本。 此插件只是用来做原型工具，不用希望你使用这个工具来运行所有的测试脚本
Selenium-grid 可以并行的在多个测试环境之下测试脚本，实现脚本的并发测试执行。 缩短大量测试脚本集合的执行时间
# 选择你的Selenium 工具
如果没有编程经验，建议选择Selenium IDE 来熟悉Selenium 的命令。 使用IDE以快速来创建简单的测试脚本
我们不建议你选择Selenium IDE 来执行自动化测试，
建议先熟悉:　Selenium IDE
再进一步熟练使用Selenium-WebDriver API 
# WebDriver 支持的浏览器
IE6-10
FireFox大部分版本
Chrome
Safari
Opera
Andrioid 系统上的自带浏览器
IOS系统上自带浏览器
HtmlUnit的无界面实现
# Selenium 1和 WebDriver 的区别
WebDriver 是Selenium 1 的升级版本， 也可以理解为两个不同的产品
WebDriver 相比Selenium 1 来说可以更好地绕过JS的限制，API也更易于使用.
Selenium 1 比WebDriver 支持更多的浏览器
WebDriver 可以实现向下兼容Selenium 1 的脚本
附： selenium java教程 (连载中, 敬请期待）
[java selenium (一) selenium 介绍](http://www.cnblogs.com/TankXiao/p/5252754.html)
[java selenium (二) 环境搭建方法一](http://www.cnblogs.com/TankXiao/p/4110494.html)
[java selenium (三) 环境搭建 基于Maven](http://www.cnblogs.com/TankXiao/p/4142070.html)
[java selenium (四) 使用浏览器调试工具](http://www.cnblogs.com/TankXiao/p/5211759.html)
[java selenium (五) 元素定位大全](http://www.cnblogs.com/TankXiao/p/5222238.html)
[java selenium (六) xpath 定位](http://www.cnblogs.com/TankXiao/p/5253072.html)
[java selenium (七) CSS 定位](http://www.cnblogs.com/TankXiao/p/5260683.html)
[java selenium (八) Selenium IDE 用法](http://www.cnblogs.com/TankXiao/p/4140543.html)
[java selenium (九) 常见web UI 元素操作 及API使用](http://www.cnblogs.com/TankXiao/p/5258104.html)
[java selenium (十) 操作浏览器](http://www.cnblogs.com/TankXiao/p/5260557.html)
[java selenium (十一) 操作弹出对话框](http://www.cnblogs.com/TankXiao/p/5260445.html)
[java selenium (十二) 操作弹出窗口](http://www.cnblogs.com/TankXiao/p/5260707.html)
[java selenium (十三) 智能等待页面加载完成](http://www.cnblogs.com/TankXiao/p/5246557.html)
[java selenium (十四) 处理Iframe 中的元素](http://www.cnblogs.com/TankXiao/p/5237189.html)
