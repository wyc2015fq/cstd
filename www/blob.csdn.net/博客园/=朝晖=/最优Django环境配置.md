# 最优Django环境配置 - =朝晖= - 博客园
# [最优Django环境配置](https://www.cnblogs.com/dhcn/p/7124694.html)
# 2 最优Django环境配置
本章描述了我们认为对于中等和高级Django使用者来说最优的本地环境配置
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#21-统一使用相同的数据库引擎)2.1 统一使用相同的数据库引擎
一个常见的开发者错误是在本地开发环境中使用SQLite3，而在生产环境中使用PostgreSQL(或[MySQL](http://lib.csdn.net/base/mysql))。本节的提示不仅仅适用于使用SQLite3和PostgreSQL这种种的情景，而是适用于任何你使用两种[数据库](http://lib.csdn.net/base/mysql)却希望它们行为特性一致的情景。
下面是一些我们在开发和生产使用不同数据库时碰到的问题：
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#211-你不能在本地对线上数据的完整copy做数据检查)2.1.1 你不能在本地对线上数据的完整Copy做数据检查
当你的线上生产数据库和你的本地开发[测试](http://lib.csdn.net/base/softwaretest)数据库不一样的时候，你就不能在本地数据库中对线上生产数据库的数据做完全复制，也就无法在本地做完整的数据检查。
当然你可以对线上数据库做一个SQL Dump，然后运行这些SQL把数据导入你的本地数据库，但是这不意味者你通过这样的导入导出得到的是线上数据的完整复制。
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#212-不同的数据库有不同的字段类型和数据约束)2.1.2 不同的数据库有不同的字段类型和数据约束
请牢记:不同的数据库可处理的数据字段的类型是不同的。Django的ORM努力去适配兼容这些不同的数据类型，但是其所能做的，也只有这么多。
比如：有些人在本地使用SQLite数据库，在线上生产环境用PostgreSQL数据库。他们认为Django ORM可以让他们不用考虑不同数据库的特性差异，最终他们都会陷入麻烦：因为SQLite是动态的弱类型，而不是PostgreSQL的强类型。
当然，Django ORM可以帮助你的代码以强类型的方式操作SQLite，但是表单和数据验证的错误在开发阶段不会被捕捉到，直到代码上线才会被发现。比如你在本地可以毫无障碍的存储长字符串，因为SQLite3不在乎这个，但是在生产环境里，你的PostgreSQL或[mysql](http://lib.csdn.net/base/mysql)会报出约束错误，这些错误你在本地测试环境中不会发现，你将很难重现这些问题，直到你建立一致的本地测试环境为止。
大多数问题都不会被发现，直到项目跑在一个强类型的数据库上(比如PostgreSQL或MySQL)，当这些类型的bug发生时，你应该结束自责，努力在本地开发环境中搭建正确的数据库。
提示：Django+PostgreSQL组合
据我们所知，大多数Django开发者在所有的环境优先使用PostgreSQL，这些环境包括开发环境、演示环境、QA环境、生产环境。
根据你的[操作系统](http://lib.csdn.net/base/operatingsystem)，可以使用以下操作
- Mac:下载一键Mac安装程序：[http://postgresapp.com](http://postgresapp.com/)
- Windows:下载一键Windows安装程序：[http://postgresql.org/download/windows/](http://postgresql.org/download/windows/)
- Linux:通过系统上的包管理器，或者使用以下指令：[http://postgresql.org/download/linux/](http://postgresql.org/download/linux/)
你也许得花些功夫才能让PostgreSQL本地运行在某些操作系统上，但是你会发现花这些功夫是值得的。
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#213-固定数据fixtures不是魔法解决方案)2.1.3 固定数据(Fixtures)不是魔法解决方案
你也许想知道为什么不采用固定数据来使本地数据库和生产数据库的区别透明化。
对，固定数据在提供简单硬代码测试数据集方面是非常优秀的。有时你需要在开发中用假的测试数据填充你的数据库，特别是在早期项目的演示阶段
固定数据不是一个在数据库之间移植大量数据集的可信工具，它是一个具体数据库不明确的方式，它们根本就不应该那么用。不要错以为固定可以用来做在生产数据库和其他数据库之间移植数据的基础数据工具(dumpdata/loaddta)
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#22-使用pip和virtualenv)2.2 使用Pip和Virtualenv
如果你目前还没用这它们，我们强烈建议你熟悉这两个工具。对于Django项目来说，它们是事实上的标准配置，大多数使用Django的公司都依赖于者两个工具
Pip是一个从[Python](http://lib.csdn.net/base/python)包目录(Pypi)及其镜像获取[python](http://lib.csdn.net/base/python)包的工具.它用来管理和安装Python包，它和easy_install很像,不过有更多特性，关键的一个特性是支持Virtualenv。
Virturlenv是一个可以用来创建相互独立的Python环境的工具，这些独立的环境可以有不同的包依赖体系。当你同时开发多个项目且不同的项目依赖不同的包版本库时，你应该考虑使用Vitualenv。
举个例子：假设你开发两个项目，其中一个依赖Django1.7，一个依赖Django1.8.
- 如果没有使用Vituraenv(或者其他管理包依赖的替代工具),你将不得不在每次切换项目时重装Django。
- 如果你觉得这繁琐无用,请注意：大多数实际Django生成项目至少需要依赖12个以上的包。
Pip已经包含在Python3.4以及更新版本,深度阅读以及安装指令可以在以下页面找到:
- pip: [http://pip-installer.org](http://pip-installer.org/)
- virtualenv: [http://virtualenv.org](http://virtualenv.org/)
提示：virtualenvwrapper
我们为Mac OS X和[Linux](http://lib.csdn.net/base/linux)系统用户高度推荐virtualenvwrapper,为Windows系统用户推荐virtualenvwrapper-win,这些项目都是由Doug Hellman 发起。
就个人意见，我们一般认为不带virtualenvwrapper使用virtualenv真是一种痛苦，因为当你每次想激活一个virtualenv环境的时候，你将不得不输入以下这个长命令？
```
Example 2.1
$ source  ̃/.virtualenvs/twoscoops/bin/activate
```
使用virtualenvwrapper，你只需要输入：
```
Example 2.2
$ workon twoscoops
```
Virtualenvwrapper是一个pip和vituralenv的流行搭配工具，它使我们的输入更简单，但它不是绝对必须的。
图2.1：Pip、virtualenv和virtualenvwraper之间关系以雪糕的形式的说明图---图找原版看吧。
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#23-通过pip安装django及其他依赖)2.3 通过Pip安装Django及其他依赖
Django官方文档描述了几种安装Django的方式。我们推荐的方法是通过pip和必需清单文件的方法。
概要描述一下工作过程：一个必需清单文件就像一个Python包的清单，包含了你想要装的所有Python包，它包含每个包的名字和版本,你可以使用pip从这个清单文件安装所需包到你的virturlenv环境。
这个清单文件设置安装的内容我们将在第五章的配置与所需清单文件中谈及。
提示：配置PYTHONPATH
如果你对命令行以及环境变量的知识掌握比较牢靠的话,你可以设置你的virtualenv环境的PYTHONPATH变量以便django-admin.py可以正常启动站点及执行其他任务。
你也可以通过pip的最新版本设置你的PYTHONPATH变量包含当前目录。在项目的根目录运行“pip install -e .”命令是把当前目录作为适时编辑的包的一个技巧。
如果你不知道如果设置或者你觉得上面内容有点复杂，别担心，坚持使用manage.py就可以了。
推荐阅读：
- [http://cs.simons-rock.edu/python/pythonpath.html](http://cs.simons-rock.edu/python/pythonpath.html)
- [https://docs.djangoproject.com/en/1.8/ref/django-admin/](https://docs.djangoproject.com/en/1.8/ref/django-admin/)
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#24-使用版本控制系统)2.4 使用版本控制系统
[版本控制](http://lib.csdn.net/base/git)系统一般用来做版本控制以及代码管理。任何时候，当你开发Django项目时，你都应该使用一个版本控制系统跟踪你的代码变更。
维基百科有不同版本控制系统的详细对比：
- [http://en.wikipedia.org/wiki/Comparison_of_revision_control_software](http://en.wikipedia.org/wiki/Comparison_of_revision_control_software)
所有可选方案中，[Git](http://lib.csdn.net/base/git)和和Mercurial似乎在Django开发者当中最流行。这两个都可以很容易地创建分支和合并变更。
当使用版本控制系统时,最重要的是不近要有你代码库的本地版本,而且要有一个版本控制主机服务作为备份。为此我们建议你使用GitHub([https://github.com/)或者Bitbucket(https://bitbucket.org/](https://github.com/)%E6%88%96%E8%80%85Bitbucket(https://bitbucket.org/)).
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#25-完全一致的环境)2.5 完全一致的环境
在一个程序员的笔记本上可运行的也许在生产环境不能正常工作。但是如果你的本地开发环境和项目的演示环境、测试环境、开发环境完全一致，那会怎样哪？
当然，如果你的生产环境体系[架构](http://lib.csdn.net/base/architecture)包含10000个服务器,另外拿10000个本地服务器用作开发环境用途完全不可能。所以当我们说一致的时候偶，是指尽实际可能保持一致。
有几个环境特性差异我们可以评估一下：
操作系统不同. 如果我们开发是在Mac或者Windows上，站点是部署在Ubuntu [linux](http://lib.csdn.net/base/linux)上,那么Django项目在这两种环境下的工作机制差别是很大的。
Python安装配置不同.让我们正面这个问题,很多开发者和系统管理员甚至都不确定他们的机器上安装的Python的版本，尽管没人承认这个问题。为什么？因为正确的配置Python环境和完全理解你的配置是有点难的。
开发者之间的差异. 在一个大型开发团队里,大量的时间都浪费在调试开发者之间的环境配置差异。
配置一直的开发环境的同行方法是使用Vagrant和ViturlBox.
### [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#251-vagrant和vitualbox)2.5.1 Vagrant和VitualBox
Vagrant是一个用来创建、配置、管理可复制的开发环境的流行工具.Vagrant的最大优势是它使得使用VitualBox(和其他VM工具)变得非常容易.
比如，如果你的开发笔记本是OS X系统，但是你的项目配置是面向Ubuntu的，我们可以使用Vagrant和项目的Vagrant文件在本地快速搭建一个虚拟的Ubuntu开发环境配置，环境中会自动带有所有的依赖包和所需的安装配置。
- 为你项目组的所欲开发人员搭建一致的本地开发环境
- 把这些本地环境环境的配置做成与你的演示、测试、生产环境的配置做成高度相似或一致的。
- 外部的复杂性大多数时候是无需考虑的，简单的项目大多不用考虑操作系统级的差异,一般可以忽略不计。
- 在旧的开发机器上跑虚拟机实例会使系统性能慢如蜗牛爬行一般，甚至在新一点的机器上都会感到微小但明显可感知的系统开销
试验：使用隔离的[Docker](http://lib.csdn.net/base/docker)容器环境
使用[docker](http://lib.csdn.net/base/docker)与在虚拟机下开发有点像，当然Docker更轻量级一点。Docker容器环境共享宿主OS环境，但是有它自己隔离的进程和内存空间。更进一步讲：因为Docker使用AUFS(译注:一种高级多层的统一文件系统),容器可以快速构建带有附加差量的快照,而不是从头构建。
在Eventbrite,架构团队使用于Vagrant连接的Docker来加速开发虚拟机的供给.然而，Docker仍旧在快速发展所以只是在研究试验阶段.
## [](https://github.com/dhcn/cn-docs/blob/master/Django/C2%20%E6%9C%80%E4%BC%98Django%E7%8E%AF%E5%A2%83%E9%85%8D%E7%BD%AE.md#26-总结)2.6 总结
这一章的内容包块在开发和生产环境中使用系统的数据库，以及使用pip、vitualenv和版本控制系统。这些都是好东西，应该放进你的工具箱，因为它们不止在Django中使用，而且在大多数Python软件开发中使用。

