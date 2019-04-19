# JAVA 程序员需要用到 10 个测试框架和库 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [搬砖大师傅](http://www.jobbole.com/members/wx1615593136) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Javin Paul](https://dzone.com/articles/10-essential-testing-tools-for-java-developers)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
> 
想要提高你的自动化测试技术？以下是 10 个优秀的测试框架和库，以及它们常见用法的概述。
最近我写了一些文章，关于 Java 程序员今年应该学习什么，例如编程语言，库和框架等，如果只能学习或提高其中一项，那必然是自动化测试技能。
测试是专业程序员区别于业余程序员的一项指标，作为专业程序员，并非必须采用 TDD，BDD 或其它测试方法论，但最低标准是通过编写代码的方式，来自动化测试自己的代码。
许多 Java 程序员编写单元测试和集成测试，使用 Jenkins、TeamCity 等持续集成工具，在构建阶段自动运行。
如果还有人对程序员是否应该关注自动化测试存有疑问，那么让我来回答，随着 DevOps 理念的增强和角色的涌现，自动化测试的重要性正在呈指数型增长。
企业通常青睐那种擅长编写单元测试的程序员，这些程序员对各种单元测试框架、库和工具有着丰富的知识，比如 JUnit，Selenium，REST-Assured，Spock 框架等。
作为 Java 程序员，我们在截然不同的领域工作，从编写 Java 核心代码到 JSP 页面，REST API，甚至有时为了构建自动化而去编写 Groovy 脚本，这就要求我们必需了解不同的自动化测试工具。
举一个例子，很长一段时间内，我只了解 JUnit，但当不得不测试 JSP 页面时，我却束手无策，直到我找到了 Selenium。REST Assured 是另一个类似的例子，我通常使用 curl 命令测试 REST API，但 REST Assured 将 REST API 的单元测试水平提升到了另一个层次。
## Java 程序员需要用到十大单元测试和自动化集成测试工具
我认为一个优秀的程序员，必然能够很好地利用手头上的工具，因此我总在业余时间学习和探索新的工具和库，以下列表是我部分研究成果。
在这篇文章中，我将分享 10 个最为优秀且必不可少的工具，框架和库，这些可以帮助 java 程序员在各类 java 项目中编写单元测试和集成测试。
### JUnit
JUnit 无须赘述，即便是初级Java程序员，可能也已经听说过它，你可以使用它编写 Java 代码的单元测试。
几乎所有主流 IDE，例如 Eclipse，NetBeans 和 IntelliJ，都集成了 JUnit，可以直接在这些IDE中编写和运行单元测试。
大多数人仍在使用 JUnit 4，即使 JUnit 5 已经发布，它很可能是今年下一个热点。通过 JUnit 5，可以将 JUnit 同时应用于单元测试和集成测试，并且它还支持 Java 8 的特性。
![junit5 architecture testing](http://jbcdn2.b0.upaiyun.com/2018/04/8e5c05d6729665c7b55a0be0128a4e3b.png)
### REST Assured
用 Java 语言测试和验证 REST 服务，要难于 Groovy 这类动态语言。
REST Assured 将这类语言的易用性带入了 Java 领域，是一个优秀的 REST API 的集成测试工具。
![REST assured testing Restful web application](http://jbcdn2.b0.upaiyun.com/2018/04/83fb827ae23afa4c0fcae32470debac2.jpg)
### Selenium
Selenium 很可能是最流行的 Java UI 测试工具了，它可以让你在不必启动浏览器的情况下测试 JSP 页面。
你可以使用 JUnit 和 Selenium 来测试 Web 程序的界面，它甚至允许你编写 Web 应用程序的验收测试。
![selenium-webdriver-with-java-basics-advance-archit](http://jbcdn2.b0.upaiyun.com/2018/04/01641d464ff195a39f2e613020446df9.jpg)
### TestNG
TestNG 是一个测试框架，其灵感来自 JUnit 和 NUnit，但同时引入了一些新的功能，使其功能更强大，使用更方便。例如可以使用注解，在任意大的线程池中，配置各种可用策略进行测试（例如所有方法都在自己的线程中，每一个测试类使用一个线程等）。
因为 TestNG 使用 JUnit 4 的注解，同时又集成了 HAMCSTREST 匹配器，它与 JUnit 的差异已经减小了，但两者如何选择，这取决于你。
![TestNG framework](http://jbcdn2.b0.upaiyun.com/2018/04/58bb4c4068e8326c80735dfd0d1f430a.png)
### Mockito
Java 类有许多 Mock 框架，例如 PowerMock 和 JMock，但我个人偏向于 [Mockito](http://site.mockito.org/)，因为它有简单的 API，优秀的文档以及大量的示例。
Mocking 是现代单元测试的一项关键技术，因为它允许你在没有任何依赖的情况下独立测试代码，这就是为什么我鼓励每个 Java 程序员在学习 JUnit 的同时，一起学习 Mocking 框架的原因。
我最喜欢的 mocking 框架是 Mockito，但如果你愿意，也可以研究下 PowerMock 或 JMock。
### Spock框架
Spock 是另一个测试和规范框架，用于 Java 和 Groovy 应用程序。由于使用 Groovy 编写，Spock 成为一种兼具丰富表现力且简明扼要的规范语言。
当你使用 Spock 时，你的测试将变得更容易阅读和维护，这得益于它采用的 JUnit 运行器，Spock 兼容大部分 IDE，构建工具和持续集成服务器。
可惜我没有找到有助于学习 Spock 框架的课程，但阅读《[Java Testing with Spock](https://www.amazon.com/Java-Testing-Spock-Konstantinos-Kapelonis/dp/1617292532)》这本书是很好的开始。
![Java Testing with Spock](http://jbcdn2.b0.upaiyun.com/2018/04/7d9d4a5f78beba6c942d1ee0a60065ec.jpg)
### Cucumber
Cucumber 是另一个重要的自动化集成测试工具，但与其它同类别的工具不同的是它能够针对规格文档进行自动化测试。
Cucumber 将规格文档和测试文档合成整个动态文档，同时 Cucumber 自动测试这个文档，使测试规范始终保持在最新版本。
![Cucumber testing framework 2](http://jbcdn2.b0.upaiyun.com/2018/04/2253469cb78a9de143e0ff56c5bee12f.png)
### Spring Test
Spring MVC 自带一个很有用的测试框架，它可以在不引入 Web 容器的情况下进行深入测试。
Spring Test 是为 Spring 程序编写自动化测试的最有用的库之一。为了给 Spring 驱动的应用程序（包括 MVC 控制器在内），编写单元测试和集成测试，Spring Test 提供了一流的支持。
另外，Spring Test DbUnit 集成了 Spring Test 框架与 DbUnit；Spring Test MVC HtmlUnit 集成了Spring Test MVC 框架和 HtmlUnit。
通过使用这些工具，你可以轻松地自动测试 Spring MVC 应用程序。
### DBUnit
数据库是许多 Java 应用程序，包括核心 Java 和 Web 应用程序中不可或缺的部分，也有可能是单元测试的最大障碍。
在进行集成测试时，连接开发环境或用户验收测试的数据库并不可靠，因为任何人都可以更改数据模式和数据本身，例如表和存储过程等，这会导致自动化集成测试失败。
DbUnit 是一个 JUnit 扩展，每次集成测试前，将数据库初始化成已知状态，确保数据库存储正确的数据。
DbUnit 自身还存在着一些问题，但它是一个非常有用的工具，因为它可以帮助我们分离测试数据与测试代码。
![Unit testing of database using DBUnit](http://jbcdn2.b0.upaiyun.com/2018/04/e86c20da61db52957e6e4f87c7a25b65.png)
### Robot 框架
Robot 框架是一个基于 Python 的通用测试自动化框架，用于验收测试和验收测试驱动开发。
它是一个由关键字驱动的，使用表格测试数据语法的测试框架，可以用来测试那些涉及多种技术和接口的分布式异构应用。
如果你打算学习这个优秀的集成测试框架，那么你可以从 Udemy 上的《[Robot 框架测试自动化](https://www.udemy.com/robot-framework-level-1/?siteID=JVFxdTr9V80-fviC4FKz0NBU7fs4fjXA6A&LSNPUBID=JVFxdTr9V80)》的课程开始，这是一个很好的学习资源。
该课程涵盖了两部分内容，Robot 框架基础和高级特性。
![accept test report using robot framework](http://jbcdn2.b0.upaiyun.com/2018/04/6b6e14053b953291f679a7bd7e456d51.png)
### 结论
以上列举了Java 程序员需要用到的单元测试和集成测试工具，框架和库。
还有很多库没有包括在这个列表中，例如 AssertJ 和 Hamcrest，它们可以帮助你写出漂亮且流畅的测试，但学习需要一步步来。
首先，学习一个可以应用于日常工作的工具或库。 例如，如果你正在使用 Java UI，那么首先应该学习 Selenium，这样你可以有更多时间专注在这个工具上。
同样的，如果你的工作内容是 REST API，请学习 REST Assured（参阅 [REST with Spring](http://www.baeldung.com/rest-with-spring-course?utm_source=javarevisited&utm_medium=web&utm_campaign=rws&affcode=22136_bkwjs9xa)）；如果你正在做很多核心 Java 的工作，那么 JUnit 5 可能是你首先需要关注的库。
