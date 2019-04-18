# 软件开发的MVC构架 - wishchinYang的专栏 - CSDN博客
2014年02月10日 14:43:52[wishchin](https://me.csdn.net/wishchin)阅读数：771
MVC：IDE开发环境开发时，无意中使用的软件结构.
转自于wikipedia：[http://zh.wikipedia.org/wiki/MVC](http://zh.wikipedia.org/wiki/MVC)
软件的层次划分：框架——组件（设计模式）——算法与数据结构.
## 层次
**模型（Model）** “数据模型”（Model）用于封装与应用程序的业务逻辑相关的数据以及对数据的处理方法。“模型”有对数据直接访问的权力，例如对数据库的访问。“模型”不依赖“视图”和“控制器”，也就是说，模型不关心它会被如何显示或是如何被操作。但是模型中数据的变化一般会通过一种刷新机制被公布。为了实现这种机制，那些用于监视此模型的视图必须事先在此模型上注册，从而，视图可以了解在数据模型上发生的改变。（比较：[观察者模式](http://zh.wikipedia.org/wiki/%E8%A7%82%E5%AF%9F%E8%80%85%E6%A8%A1%E5%BC%8F)（[软件设计模式](http://zh.wikipedia.org/wiki/%E8%BD%AF%E4%BB%B6%E8%AE%BE%E8%AE%A1%E6%A8%A1%E5%BC%8F)））
**视图（View）** 视图层能够实现数据有目的的显示（理论上，这不是必需的）。在视图中一般没有程序上的逻辑。为了实现视图上的刷新功能，视图需要访问它监视的数据模型（Model），因此应该事先在被它监视的数据那里注册。
**控制器（Controller）** 控制器起到不同层面间的组织作用，用于控制应用程序的流程。它处理事件并作出响应。“事件”包括用户的行为和数据模型上的改变。
## 优点
在最初的[JSP](http://zh.wikipedia.org/wiki/JSP)网页中，像[数据库](http://zh.wikipedia.org/wiki/%E6%95%B0%E6%8D%AE%E5%BA%93)查询语句这样的数据层代码和像[HTML](http://zh.wikipedia.org/wiki/HTML)这样的表示层代码混在一起。经验比较丰富的开发者会将数据从表示层分离开来，但这通常不是很容易做到的，它需要精心地计划和不断的尝试。MVC从根本上强制性地将它们分开。尽管构造MVC应用程序需要一些额外的工作，但是它带给我们的好处是毋庸置疑的。
首先，多个视图能共享一个模型。如今，同一个Web应用程序会提供多种用户界面，例如用户希望既能够通过浏览器来收发[电子邮件](http://zh.wikipedia.org/wiki/%E7%94%B5%E5%AD%90%E9%82%AE%E4%BB%B6)，还希望通过手机来访问[电子邮箱](http://zh.wikipedia.org/wiki/%E7%94%B5%E5%AD%90%E9%82%AE%E7%AE%B1)，这就要求Web网站同时能提供[Internet](http://zh.wikipedia.org/wiki/Internet)界面和[WAP](http://zh.wikipedia.org/wiki/WAP)界面。在MVC设计模式中，模型响应用户请求并返回响应数据，视图负责格式化数据并把它们呈现给用户，业务逻辑和表示层分离，同一个模型可以被不同的视图重用，所以大大提高了代码的可重用性。
其次，控制器是自包含（self-contained）指高独立内聚的对象，与模型和视图保持相对独立，所以可以方便的改变应用程序的数据层和业务规则。例如，把数据库从[MySQL](http://zh.wikipedia.org/wiki/MySQL)移植到[Oracle](http://zh.wikipedia.org/wiki/Oracle)，或者把[RDBMS](http://zh.wikipedia.org/wiki/RDBMS)数据源改变成[LDAP](http://zh.wikipedia.org/wiki/LDAP)数据源，只需改变模型即可。一旦正确地实现了控制器，不管数据来自数据库还是[LDAP](http://zh.wikipedia.org/wiki/LDAP)服务器，视图都会正确地显示它们。由于MVC模式的三个模块相互独立，改变其中一个不会影响其他两个，所以依据这种设计思想能构造良好的少互扰性的构件。
此外，控制器提高了应用程序的灵活性和可配置性。控制器可以用来连接不同的模型和视图去完成用户的需求，也可以构造应用程序提供强有力的手段。给定一些可重用的模型和视图，控制器可以根据用户的需求选择适当的模型进行处理，然后选择适当的的视图将处理结果显示给用户。
## 评价及适用范围
MVC模式的缺点是由于它没有明确的定义，所以完全理解MVC模式并不是很容易。使用MVC模式需要精心的计划，由于它的内部原理比较复杂，所以需要花费一些时间去思考。开发一个MVC模式架构的工程，将不得不花费相当可观的时间去考虑如何将MVC模式运用到应用程序中，同时由于模型和视图要严格的分离，这样也给调试应用程序带来了一定的困难。每个构件在使用之前都需要经过彻底的测试。另外由于MVC模式将一个应用程序分成了三个部件，所以这意味着同一个工程将包含比以前更多的文件。
过去MVC模式并不适合小型甚至中等规模的应用程序，这样会带来额外的工作量，增加应用的复杂性。但现在多数软件设计框架，能直接快速提供MVC骨架，供中小型应用程序开发，此问题不再存在。对于开发存在大量用户界面，并且逻辑复杂的大型应用程序，MVC将会使软件在健壮性、代码重用和结构方面上一个新的台阶。尽管在最初构建MVC模式框架时会花费一定的工作量，但从长远的角度来看，它会大大提高后期软件开发的效率。
## 实现
### [MFC](http://zh.wikipedia.org/wiki/MFC)
[微软](http://zh.wikipedia.org/wiki/%E5%BE%AE%E8%BD%AF)所推出的[MFC](http://zh.wikipedia.org/wiki/MFC) Document/View架构是早期对于MVC模式的实现，[MFC](http://zh.wikipedia.org/wiki/MFC)将程序分成CView以及CDocument两大类型，其中的Document对应MVC中的Model，View相当于MVC中的View＋Controller，再加上CWinApp类型，合成三大项。但是基本上[MFC](http://zh.wikipedia.org/wiki/MFC)是一个失败的MVC模式作品。
由于[MFC](http://zh.wikipedia.org/wiki/MFC)之下的Document/View定义过于模糊，未将Controller（MessageMap）部份取出，因此Controller可以置入View或Document，但不管置入哪一方面，都会与View或Document绑死，没有弹性。
Controller（MessageMap）对于Qt未必能自由控制，一般由语法规则控制，在这个节点上说其是不太人性化。
### Python
[Python](http://zh.wikipedia.org/wiki/Python) 有许多的 MVC 架构。最常用的有[Django](http://zh.wikipedia.org/wiki/Django) 和[TurboGears](http://zh.wikipedia.org/wiki/TurboGears)。
