# OSGi 和 C++ - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [周昌鸿](http://www.jobbole.com/members/zzzworm) 翻译。未经许可，禁止转载！
英文出处：[cpp microservices](http://blog.cppmicroservices.org/2012/03/29/osgi-and-c++/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
2011年 9月我参加了OSGi社区在达姆施塔特的[会议](http://www.osgi.org/CommunityEvent2011)，并且有机会与其他与会者探讨本机c++实现的[OSGi](http://www.osgi.org/)规范的现状。在这一事件之前我也一直想写一篇博客，来描述关于当前实现OSGi规范的现状和努力——类似于C / c++实现的OSGI框架。最后,这篇文章会给出OSGi本机实现的概述。
## 简介
我第一次了解OSGI组件模型是在7年前开发一个Eclipse RCP应用程序。我现在更多从事C++的开发工作，但是仍然关注JAVA和OSGI的发展趋势。尽管C++有许多高质量的库可以创建复杂的应用程序，但是针对面向组件（或服务）的设计和实现，C++相关的类库和框架就显得有些匮乏了。我试着总结一下现在正在开发的用C/C++实现OSGI应用程序接口的项目。一些复杂的[中间件](http://en.wikipedia.org/wiki/Middleware)，比如：[CORBA](http://en.wikipedia.org/wiki/Corba)和[SCA](http://en.wikipedia.org/wiki/Service_component_architecture)实现了一些面向服务的设计，不过都不是使用C++实现或者缺乏开发活跃度（[Apache Tuscany C++](http://tuscany.apache.org/sca-native.html)项目最新的版本是2007年发布的），这些项目就不在讨论之列。
## 本地开发和OSGI的通用性
2007年发布了RFP-89提案，该提案规范了一个通用OSGI的实现。在通用OSGI的邮件列表中有一些简短的讨论，Peter Kriens也发布了一篇博文讲述这个观点。简短地说，这个观点是使得其他语言开发的（非JAVA）服务对象可以通过OSGI注册项被访问（后台实现可能类似于IPC机制）。其核心管理服务单元仍然使用JAVA实现OSGI，但是允许管理本机开发的组件（bundles）.
原生OSGi似乎更经常被提及，将OSGi原则使用本机OSGi框架实现(例如C或c++)。本文将重点关注这些原生框架。当然，如果原生OSGI框架在实现通用OSGI的规范时能提供和JAVA的互操作性，那么就会有更大的影响力（因为提供了一个更大的功能超集）。在这篇文章中，你可以找到一些关于[《自2008年9月以来的通用OSGI状态》](https://mail.osgi.org/pipermail/osgi-dev/2008-September/001450.html)。这里提到的论文[《物联网软件架构》](http://systems.ethz.pubzone.org/servlet/Attachment?attachmentId=43&versionId=716259)是由Jan Rellermeyer写的，描述了一个轻型JNI实现的本机C++和Java服务之间的互操作。不过，我没有发现相关的本机代码（倒是找到了Java实现的远程OSGI服务，见[这里](http://r-osgi.sourceforge.net/)）。
大家对本机实现OSGI的兴趣并没有消退(见 [1](http://stackoverflow.com/questions/2627114/c-modularization-framework-like-osgi), [2](http://stackoverflow.com/questions/7069144/how-to-compose-osgi-based-project-with-c-based-project), [3](http://stackoverflow.com/questions/9249708/is-there-a-non-java-alternative-to-osgi), 和 [4](http://stackoverflow.com/questions/5127981/portable-c-component-design))，不过这样做还不是主流，另一篇由Peter Kriens 在2010年10月发布的博客文章（[链接](http://www.osgi.org/blog/2010/10/minimal-osgi-systems.html)）指出了通用OSGI的观点和一个Apache基金会孵化的项目提供了C实现OSGI（[celix](http://wiki.apache.org/incubator/CelixProposal)）。我会在下一个章节详细介绍当前受关注的原生OSGI实现项目。
## C/C++项目
尽管原生OSGI并不指定特定的平台或者本机开发语言，不过人们更关注C和C++的实现版本（前面的连接也提到了）。最早的一个项目是[OSGI for C++](http://sourceforge.net/projects/osgi4cpp/),最初是在2007年7月在SourceForge上注册的。不过这个项目没有发布任何源码和二进制产品，看起来像是被遗弃了。我会汇总一下我所知道的当前正在开发的C或者C++实现的相关项目，尽力以这些项目发布的时间顺序排列（以我所知的）。
声明:我是CTK插件框架的主要开发者。我对于其他项目的了解是通过不同互联网资源汇总而成的（我会提供相应的链接），不过可能存在不完善。不幸的是，我没有足够的时间和精力来测试和使用所有的项目。所以，我对于这些项目的了解大部分来自阅读相关的文档和源码，如果你是下面提到的其中某个框架的开发者，请联系我详细或修正相关信息。
## POCO OSP
2007年7月，[POCO开放服务平台](http://www.appinf.com/en/products/osp.html)作为第一个用C++开发的类OSGI项目发布了。项目白皮书中的[版权声明](http://www.appinf.com/download/OpenServicePlatform.pdf)似乎显示项目是在2007年某个时间创建的。这个项目和其他C/C++项目有两个地方不同，第一，这是一个商业项目，第二，它使用了和OSGI规范相似的API。*Bundles**和**service registry**的概念是从**OSGI**规范中提取的。*
在[POCO OSP API](http://www.appinf.com/docs/poco/index.html)的文档中列举了一系列服务（如：首选项和用户管理），这些都和OSGI服务规范相似。它实现了OSGI框架的一个子集，并且提供了一个OSGI控制台，一个基于Eclipse RCP的管理界面，支持远程服务调用，允许通过命令行对bundles进行签名并与框架进行交互。
## SOF
面向服务框架（[SOF](http://sof.tiddlyspot.com/)）是在2008年早些时候在SourceForge上注册的，这个项目是最早的可以使用的C++实现OSGI的开源项目。该项目实现了OSGI框架的一个子集，提供了一个OSGI控制台，一个基于[Eclipse RCP](http://wiki.eclipse.org/index.php/Rich_Client_Platform)的管理界面，并且支持远程服务。
SOF的远程服务能力是基于[MICO](http://www.mico.org/)（一个C++ CORBA实现）实现的。
## CTK 插件框架
[CTK插件框架](http://www.commontk.org/index.php/Documentation/Plugin_Framework)是第三个使用C++重写类OSGI的组件框架，由德国癌症研究中心[医药和生物信息技术部门](http://www.dkfz.de/en/mbi/)开发的。第一个版本是在2007/2008年期间开发的一个更大框架OpenCherry的一部分，主要是提供了基于[Eclipse RCP](http://wiki.eclipse.org/index.php/Rich_Client_Platform)的C++组件模型（类似于[Equinox](http://www.eclipse.org/equinox/)）。项目后来被命名为[BlueBerry](http://www.mitk.org/wiki/BlueBerry)，成为[MITK应用程序框架](http://www.dkfz.de/en/mbi/projects/mitk.html)的基础（Blueberry自身是一个真正的应用程序平台，并不是针对特定应用的），Blueberry中OSGI相关的C++代码在2009年被重写，并形成了现在的[CTK插件框架](http://www.commontk.org/index.php/Documentation/Plugin_Framework)。
CTK插件框架是基于[QT core](http://qt-project.org/doc/qt-4.8/qtcore.html)库的，实现了几乎完整的OSGI框架API。它使用了QT的插件系统，资源系统和信号、槽机制来支持所有的OSGI框架功能实现，此外，CTK也提供了一些OSGI可选服务的实现。
## nOSGI
[nOSGI](http://www-vs.informatik.uni-ulm.de/proj/nosgi/)是另一个C++实现的针对Posix系统的OSGI项目。根据这个博客的[评论](http://www.osgi.org/blog/2010/10/minimal-osgi-systems.html)显示，该项目最早在2009年开发的。该项目的作者也写了一份非常棒的[论文](http://dl.acm.org/authorize?6553100)，阐述了将OSGI转换为一个原生系统（POSIX）的可行性。
nOSGI尤其专注于建模互相捆绑软件包的依赖关系（见前面论文中C++软件包的定义），使用了在运行时通过修补的DSO的ELF头。同时，该项目也提供了一个OSGI的控制台来和运行环境交互。
## Celix
2010年，[Celix](http://incubator.apache.org/celix/)作为Apache孵化器的一个项目被创建（[提案](http://wiki.apache.org/incubator/CelixProposal)）。最初是由[Luminis](http://www.luminis.eu/en)用C开发的一个OSGI实现。Celix主要关注尽可能参照OSGI的API实现，并且实现JAVA OSGI 组件和使用Celix原生C组件的互操作性问题。
Celix同样也提供了一个OSGI控制台和一个日志服务实现，另外，Celix项目团队还试图形成一个C\C++ OSGI开发社区，并号召开源社区来响应这些项目（[见邮件列表](http://incubator.markmail.org/search/+list:org.apache.incubator.celix-dev#query:%20list%3Aorg.apache.incubator.celix-dev+page:2+mid:a3qltqhsocmrnerd+state:results)）。
## 对比
我会提供上述项目的一个高层的比较。请注意，尽管我尽力收集准确的数据，不过仍然可能存在不准确和错误之处。这些数据来源于以下地方：
- Poco OSP:官方 API 文档 (自 29/03/2012) 和评估软件包 2011_2.
- SOF: 源码 版本1.3 (revision 11090)和站点在线文档.
- CTK: 源码(Git hash 233893) 和站点在线文档(from 29/03/2012).
- nOSGi: 源码(SVN revision 8).
下面的表格列举了各项目支持的平台，许可证信息，实现语言和创建日期（根据目前能确定的信息）。
- ||Supported Platforms|License|Language|Created|
|----|----|----|----|----|
|**[Poco OSP](http://www.appinf.com/en/products/osp.html)**|Linux, Windows, Mac OS, QNX|Commercial|C++|2007 (?)1|
|**[SOF](http://sof.tiddlyspot.com/)**|Linux, Windows|BSD (?)2|C++|2008|
|**[CTK](http://www.commontk.org/index.php/Documentation/Plugin_Framework)**|Linux, Windows, Mac OS|Apache License 2.0|C++|2009|
|**[nOSGi](http://www-vs.informatik.uni-ulm.de/proj/nosgi/)**|POSIX|GPLv3|C++|2009|
|**[Celix](http://incubator.apache.org/celix/)**|Linux3|Apache License 2.0|C|2010|
- 1.      来自项目白皮书的最早版权声明时间。
- 2.      来自SourceForge项目的描述信息，不过代码中没有相应的许可证信息。
- 3.      该项目支持跨平台，不过看起来主要面向linux.
这五个OSGI框架实现在很多方面存在差异，下面的表格会总结这些项目的下述方面特性：
- 服务查询语言，查询服务中组件上下文，并允许使用过滤表达式添加服务监听（根据服务属性）
- 服务/组件记录器。提供使用类来跟踪服务和组件的变更（基础上说，提供了Tracker规范的实现）。
- 组件在线升级。允许在运行中升级组件
- 类型安全的服务。提供一种机制，允许将一个服务实例安全地转换为一个实现的接口。
- 线程安全性。线程安全的OSGI框架API
||Service Query Language|Service/Bundle Tracker|Bundle Updates|Type-safe Services|Thread-safe|
|----|----|----|----|----|----|
|**[Poco OSP](http://www.appinf.com/en/products/osp.html)**|Yes|No/No|Yes|Yes|Yes|
|**[SOF](http://sof.tiddlyspot.com/)**|No|Yes/No|No|Yes|(Yes)1|
|**[CTK](http://www.commontk.org/index.php/Documentation/Plugin_Framework)**|Yes (RFC1960)|Yes/Yes|Yes|Yes|Yes|
|**[nOSGi](http://www-vs.informatik.uni-ulm.de/proj/nosgi/)**|(Yes)2 (RFC1960)|No/No|Yes|No|No|
|**[Celix](http://incubator.apache.org/celix/)**|Yes (RFC1960)|Yes/No|Yes|No|Yes|
1.线程安全性需要用户提供一个定制的线程策略类作为启动的一个模版参数传递进去。
2.仅仅支持注册的服务监听器
下面的是对OSGI规范的实现情况（可能是不完整的）。相同级别API实现和原始OSGI规范在不同的项目中差距会很大。
||Implemented OSGi Specifications|Planned|
|----|----|----|
|**[Poco OSP](http://www.appinf.com/en/products/osp.html)**|(Preferences, User Admin, Http, Remote Services)1|?|
|**[SOF](http://sof.tiddlyspot.com/)**|Remote Services|?|
|**[CTK](http://www.commontk.org/index.php/Documentation/Plugin_Framework)**|Event Admin, Configuration Admin, Metatype Service, Log Service|Remote Services|
|**[nOSGi](http://www-vs.informatik.uni-ulm.de/proj/nosgi/)**|–|?|
|**[Celix](http://incubator.apache.org/celix/)**|Log Service, (Deployment Admin, Remote Services)2|?|
1 当Poco OSP 以服务*services**形式提供这些功能时*, 似乎与OSGi规范不兼容.
2 SVN上有一些代码和示例，但是站点上没有相关文档，状态未知
最后，最后一个表格总计了一些开源项目的代码规格，使用的代码是在章节前声明的版本。开发成本是用*sloccount**基于基本*COCOMO模型进行统计的。代码行统计使用了cloc工具。所有统计都是针对源码的（不包含示例，测试代码，服务实现生成代码等等），
||Lines of Code|Lines of Comments|Costs|
|----|----|----|----|
|**[SOF](http://sof.tiddlyspot.com/)**|3559|2801|$ 102k|
|**[CTK](http://www.commontk.org/index.php/Documentation/Plugin_Framework)**|8770|10024|$ 264k|
|**[nOSGi](http://www-vs.informatik.uni-ulm.de/proj/nosgi/)**|2208|2284|$ 62k|
|**[Celix](http://incubator.apache.org/celix/)**|8923|2450|$ 269k|
## 总结
好消息是现在已经有了原生OSGI解决方案，并且相应的开发正在进行。坏消息是原生OSGI开发正在碎片化，工业界现在还没有一个大的开发社区和有效的兴趣点(我猜作者的意思是大家都关注的项目和社区)。我认为,当前针对嵌入式系统和大规模组件应用系统的[C++复兴](http://channel9.msdn.com/Shows/Going+Deep/Craig-Symonds-and-Mohsen-Agsen-C-Renaissance)趋势中，一个原生的OSGI解决方案是非常有益的（想想[每美元性能提高比例](http://channel9.msdn.com/posts/C-and-Beyond-2011-Herb-Sutter-Why-C)）。
从技术方面来说，我没有讨论关于OSGI实现上的许多细节以及当前项目的实现情况。我也许应该针对这些项目如何处理组件的元数据、依赖性、版本控制、资源、动态加载和RTTI(资源初始化即获取)问题进行更深一步的比较。
