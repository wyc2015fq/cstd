# DevOps部署发布前验证-Selenium自动化测试 - 纸上得来终觉浅，绝知此事要躬行 - CSDN博客





2019年01月13日 11:27:16[boonya](https://me.csdn.net/boonya)阅读数：384








当我们将代码搬到Github\Gitlab之上的时候，我们就可以通过类似于Jenkins这样的DevOps解决方案来实现对代码的自动化编译、打包、测试和发布等操作了。对于自动化测试这部分，相信不是专门做测试的人接触的还是比较少的，那么我们怎么实现自动化测试代码编写就是我们首要解决的问题。

## 自动化测试框架

自动化测试框架，即是应用于自动化测试所用的框架。按照框架的定义，自动化测试框架要么是提供可重用的基础自动化测试模块，如：selenium [1] 、watir等，它们主要提供最基础的自动化测试功能，比如打开一个程序，模拟鼠标和键盘来点击或操作被测试对象，最后验证被测对象的属性以判断程序的正确性；要么是可以提供自动化测试执行和管理功能的架构模块，如：[Phoenix Framework](https://baike.baidu.com/item/Phoenix%20Framework)，robot [2] ，[STAF](https://baike.baidu.com/item/STAF) [3] 等，它们本身不提供基础的自动化测试支持，只是用于组织、管理和执行那些独立的自动化测试用例，测试完成后统计测试结果，通常这类框架一般都会集成一个基础自动化测试模块，如：robot框架就可以集成selenium [4] 框架，Phoenix Framework集成的也是selenium框架。

## selenium自动化测试框架

Selenium是一个用于Web应用程序测试的工具。Selenium测试直接运行在浏览器中，就像真正的用户在操作一样。支持的浏览器包括IE、Mozilla Firefox、Mozilla Suite等。这个工具的主要功能包括：测试与浏览器的兼容性——测试你的应用程序看是否能够很好得工作在不同浏览器和操作系统之上。测试系统功能——创建衰退测试检验软件功能和用户需求。支持自动录制动作和自动生成。Net、Java、Perl等不同语言的测试脚本。Selenium 是ThoughtWorks专门为Web应用程序编写的一个验收测试工具。

[IBM-使用 Selenium 实现基于 Web 的自动化测试](https://www.ibm.com/developerworks/cn/web/1209_caimin_seleniumweb/index.html)

[Selenium快速入门](http://www.51testing.com/html/98/n-220298.html)

[自动化测试工具Selenium入门](https://blog.csdn.net/liuzhixiong_521/article/details/78607236)

[selenium用法详解](https://www.cnblogs.com/themost/p/6900852.html)

更多相关知识参考：

[http://www.ltesting.net/ceshi/open/kygncsgj/selenium/](http://www.ltesting.net/ceshi/open/kygncsgj/selenium/)



