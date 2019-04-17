# 实践selenium自动化时先要弄清的几件事 - zhusongziye的博客 - CSDN博客





2017年10月31日 22:59:29[zhusongziye](https://me.csdn.net/zhusongziye)阅读数：191








最近在招聘，很多同学的简历都有自动化测试，基本都有selenium。但很少有人真正弄清selenium。一个是selenium的提供了很多组件，让人容易上手，然后很多人就没有然后了。二是selenium的更新伴随着工具的合并，很多人压根没高清自己用的是selenium
 RC 还是 webdriver。根据最近面试的情况，把很多同学在实践selenium的时候没弄清的问题总结了一下。详述如下：




1、selenium 工具组件




　　1.1 selenium2，也称为selenium webdriver。webdriver原来是另一个自动化测试工具，后与selenium
 合并了。webdriver直接调用浏览器本身对自动化支持的接口来驱动浏览器的操作。故名思义，它将有不同的driver来驱动不同系统上不同的浏览器。




　　1.2 selenium1，也称为seleniumRC或者Remote Control。就是selenium的原本项目。RC是在浏览器启动后，使用js来驱动浏览器的操作。为什么叫RC呢？因为RC的设计是可以支持控制远程服务器上的selenium server来操作远程端的浏览器。




　　1.3 seleniumIDE，这个是大家都明确的，以插件的形式安装在firefox，用于录制。




　　1.4 selenium-grid，这个是对使用RC进行测试时，需要执行大量用例或分布式执行的时候，提供的框架，就是建立多个服务器的hub与node的关系而已。




　　简单说，selenium项目原本的工具是IDE、RC、grid。而webdriver确实功能强大，“他们”讨论讨论，就把它被并入selenium了。




　2、工具包




　　目前selenium的包已升级到3.x，大概是16年下半年。常用的包如下：




　　2.1 Selenium Standalone Server 作为远程的服务，运行RC和webdriver。从3.x版本开始，RC不能直接运行，需要通过WebDriverBackedSelenium的接口。




　　2.2 selenium client & webdriver ：编写RC、webdriver代码需要的包。主要支持：java、c#、ruby、python、js。




　　2.3 google chrome driver：支持google chrome的driver。有win、mac、linux版本。




　　2.4 htmlUnitDriver：不使用具体的浏览器，而是在内存中运行。




　　2.5 Mozilla GeckoDriver：支持火狐浏览器的driver。有win、mac、linux版本

　　更多的包参考：http://www.seleniumhq.org/download/




　3、选用工具




　　了解阶段：IDE＋firefox

　　入门阶段：RC、webDriver＋firefox

　　深入阶段： webdriver＋testNG＋google chrome driver＋一门编程语言

　　浏览器选用google chrome 主要是经验主观觉得selenium对firefox新版本的支持一直比较慢。而google chrome的较稳定。




　4、mac系统下google chrome浏览器的路径配置




　　win系统的资料比较多，主要说下mac系统的问题。




　　在创建driver时，找不到浏览器路径，通常使用设定系统属性来实现。




　　而在mac系统下，使用google chrome浏览器或firefox时，先下载对应的 driver。在代码中配置driver的地址，再创建driver，例如 ：

　　System.setProperty("webdriver.chrome.driver","/Users/yp-tc-m-2933/Desktop/selejar/chromedriver");

　　WebDriverdriver=newChromeDriver();

　　亲测：firefox 51.0.1 (64 位)，mac 10.10.5。创建firefox driver后直接崩溃。




5、元素定位与可测性




　　selenium提供了很丰富的元素定位工具。但总是会有找不到或者变更频繁的，导致元素定位失败。实际上，作为测试方，在系统设计时，就应该提出对页面测试的需求，在页面设计时，在页面元素中埋入测试属性值。这就降低了页面自动化的难度，不用把时间花在复杂的定位元素的过程中，并且最重要的是避免一旦页面修改，带来的脚本维护工作。这就提高了系统可测性。




　6、后续




　　随着对测试开发的逐渐流行，测试人员的代码能力也在提升。在selenium的学习中，更需要掌握webdriver的api，探索selenium的更多特性。






(本文转载自网络，感谢原文作者,如有版权问题请及时留言,我们会第一时间处理)



