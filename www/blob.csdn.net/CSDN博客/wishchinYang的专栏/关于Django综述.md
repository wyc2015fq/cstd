# 关于Django综述 - wishchinYang的专栏 - CSDN博客
2014年07月06日 17:38:46[wishchin](https://me.csdn.net/wishchin)阅读数：890

主页：[https://www.djangoproject.com/](https://www.djangoproject.com/)
使用 Django 和 Python 开发 Web 站点: [http://www.ibm.com/developerworks/cn/linux/l-django/](http://www.ibm.com/developerworks/cn/linux/l-django/)
**0.  简介**：Django 项目是一个python定制框架，它源自一个在线新闻 Web 站点，于 2005 年以开源的形式被释放出来。Django 框架的核心组件有：
- 
用于创建模型的[对象关系映射](http://baike.baidu.com/view/6236303.htm)
- 
为最终用户设计的完美管理界面
- 
一流的 URL 设计
- 
设计者友好的[模板](http://baike.baidu.com/view/160044.htm)语言
- 
缓存系统。
Django(发音：[`dʒæŋɡəʊ]) 是用[python](http://baike.baidu.com/view/21087.htm)语言写的开源web开发框架(open source web framework)，它鼓励快速开发,并遵循[MVC](http://baike.baidu.com/view/31.htm)设计。Django遵守[BSD](http://baike.baidu.com/view/209692.htm)版权，初次发布于2
![django logo](http://f.hiphotos.baidu.com/baike/s%3D220/sign=bea1cf7bddc451daf2f60be986fd52a5/4b90f603738da9779c66d10eb051f8198618e34e.jpg)
django logo
005年7月, 并于2008年9月发布了第一个正式版本1.0 。
Django 根据比利时的爵士音乐家[Django Reinhardt](http://baike.baidu.com/view/1275070.htm)命名，他是一个吉普赛人，主要以演奏吉它为主，还演奏过小提琴等。
**1.  地位**：（百度百科）由于Django在近年来的迅速发展，应用越来越广泛，被著名IT开发杂志SD Times评选为[2013 SD Times 100](http://www.evget.com/article/2013/6/21/19095.html)，位列“API、库和框架”分类第6位，被认为是该领域的佼佼者[1]。
## 2.  架构设计
Django是一个基于[MVC](http://baike.baidu.com/view/31.htm)构造的框架。但是在Django中，控制器接受用户输入的部分由框架自行处理，所以 Django 里更关注的是模型（Model）、模板(Template)和视图（Views），称为 MTV模式。它们各自的职责如下：
|层次|职责|
|----|----|
|模型（Model），即数据存取层|处理与数据相关的所有事务： 如何存取、如何验证有效性、包含哪些行为以及数据之间的关系等。|
|模板(Template)，即表现层|处理与表现相关的决定： 如何在页面或其他类型文档中进行显示。|
|视图（View），即业务逻辑层|存取模型及调取恰当模板的相关逻辑。模型与模板之间的桥梁。|
     从以上表述可以看出Django 视图不处理用户输入，而仅仅决定要展现哪些数据给用户，而Django 模板 仅仅决定如何展现Django视图指定的数据。或者说, Django将MVC中的视图进一步分解为 Django视图 和 Django模板两个部分，分别决定 “展现哪些数据” 和 “如何展现”，使得Django的模板可以根据需要随时替换，而不仅仅限制于内置的模板。
至于MVC控制器部分，由Django框架的URLconf来实现。URLconf机制是使用正则表达式匹配URL，然后调用合适的Python函数。URLconf对于URL的规则没有任何限制，你完全可以设计成任意的URL风格，不管是传统的，RESTful的，或者是另类的。框架把控制层给封装了，无非与数据交互这层都是数据库表的读,写,删除,更新的操作.在写程序的时候，只要调用相应的方法就行了，感觉很方便。程序员把控制层东西交给Django自动完成了。 只需要编写非常少的代码完成很多的事情.所以。它比MVC框架考虑的问题要深一步.因为我们程序员大都写程序在控制层。现在这个工作交给了框架，仅需写很少的调用代码，大大提高了工作效率。
## 3设计哲学
Django的主要目的是简便、快速的开发[数据库](http://baike.baidu.com/view/1088.htm)驱动的网站。它强调代码复用,多个组件可以很方便的以“插件”形式服务于整个框架，Django有许多功能强大的第三方[插件](http://baike.baidu.com/view/18979.htm)，你甚至可以很方便的开发出自己的工具包。这使得Django具有很强的可扩展性。它还强调快速开发和[DRY](http://baike.baidu.com/view/350348.htm)(Do
 Not Repeat Yourself)原则。
Django基于[MVC](http://baike.baidu.com/view/31.htm)的设计十分优美：
- 
对象关系映射 ([ORM](http://baike.baidu.com/view/197951.htm),object-relational mapping)：以Python类形式定义你的数据模型，ORM将模型与关系数据库连接起来，你将得到一个非常容易使用的数据库API，同时你也可以在Django中使用原始的SQL语句。
- 
URL 分派：使用[正则表达式](http://baike.baidu.com/view/94238.htm)匹配URL，你可以设计任意的URL，没有框架的特定限定。像你喜欢的一样灵活
- 
模版系统：使用Django强大而可扩展的模板语言，可以分隔设计、内容和Python代码。并且具有可继承性。
- 
表单处理：你可以方便的生成各种表单模型，实现表单的有效性检验。可以方便的从你定义的模型实例生成相应的表单。
- 
Cache系统：可以挂在内存缓冲或其它的框架实现超级缓冲 －－ 实现你所需要的粒度。
- 
会话([session](http://baike.baidu.com/view/25258.htm))，用户登录与权限检查，快速开发用户会话功能。
- 
国际化：内置[国际化](http://baike.baidu.com/view/2062916.htm)系统，方便开发出多种语言的网站。
- 
自动化的管理界面：不需要你花大量的工作来创建人员管理和更新内容。Django自带一个ADMIN site,类似于[内容管理系统](http://baike.baidu.com/view/857578.htm)。

## 4.工作机制
1.用manage .py runserver 启动Django服务器时就载入了在同一目录下的settings .py。该文件包含了项目中的配置信息，如前面讲的URLConf等，其中最重要的配置就是ROOT_URLCONF，它告诉Django哪个Python模块应该用作本站的URLConf，默认的是urls .py
![工作机制](http://f.hiphotos.baidu.com/baike/s%3D220/sign=10396b5b9825bc312f5d069a6ede8de7/738b4710b912c8fcfe024858fd039245d78821b5.jpg)
工作机制
2.当访问url的时候，Django会根据ROOT_URLCONF的设置来装载URLConf。
3.然后按顺序逐个匹配URLConf里的URLpatterns。如果找到则会调用相关联的视图函数，并把HttpRequest对象作为第一个参数(通常是request)
4.最后该view函数负责返回一个HttpResponse对象。
## 5.部署
     Django可以运行在[Apache](http://baike.baidu.com/view/28283.htm)上，也可以运行在支持[WSGI](http://baike.baidu.com/view/1660037.htm)，[FastCGI](http://baike.baidu.com/view/641394.htm)的服务器上。支持多种数据库，已经支持[Postgresql](http://baike.baidu.com/view/28196.htm),[MySql](http://baike.baidu.com/view/24816.htm),
 Sqlite3,[Oracle](http://baike.baidu.com/view/15020.htm)。[Google App Engine](http://baike.baidu.com/view/1524918.htm)也支持 Django的某些部分，国内支持的平台有（SAE）Sina App Engine，（BAE）百度应用引擎。
## 6.文档
           Django1.0 建立了强大完整的文档体系，,涵盖了Django的方方面面,并且适合各种水平的读者和开发者. 其中还包含若干个简单的示例,你可以跟随他们一步步体验Django的优美。
**7.翻译一下主页**：
## The Django framework --- Django框架
### Object-relational mapper---  对象关系映射
     Define your [data models](https://docs.djangoproject.com/en/1.6/topics/db/models/) entirely in Python. You get a rich, [dynamic database-access API](https://docs.djangoproject.com/en/1.6/topics/db/queries/) for free — but you can still write SQL if needed.
      在Python中确定数据模型，你可以获得丰富易用的动态数据库接口，也可以自己写SQL接口。
### Automatic admin interface---自动管理界面
     Save yourself the tedious work of creating interfaces for people to add and update content.[Django does that automatically](https://docs.djangoproject.com/en/1.6/intro/tutorial02/), and it's production-ready.
     把自己从繁琐的界面设计中解放出来，Django为你自动完成。
### Elegant URL design---优雅的URL设计
   Design pretty, [cruft-free URLs](https://docs.djangoproject.com/en/1.6/topics/http/urls/) with no framework-specific limitations. Be as flexible as you like.
   优美的设计，不令人讨厌的弹性设计。
### Template system---模板系统
    Use Django's powerful, extensible and designer-friendly [template language](https://docs.djangoproject.com/en/1.6/topics/templates/) to separate design, content and Python code.
    使用Django有力的、可扩增的、设计友好的模板语言 完成设计、内容和编码。
### Cache system---缓存系统
    Hook into memcached or other cache frameworks for [super performance](https://docs.djangoproject.com/en/1.6/topics/cache/) — caching is as granular as you need.
    更好表现的颗粒状的缓存系统
### Internationalization---国际化
     Django has full support for [multi-language applications](https://docs.djangoproject.com/en/1.6/topics/i18n/), letting you specify translation strings and providing hooks for language-specific functionality.
     更有力的多语言支持，让你自由地翻译....
....................每次进步都得靠翻译..................
