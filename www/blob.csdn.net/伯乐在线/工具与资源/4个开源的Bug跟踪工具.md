# 4个开源的Bug跟踪工具 - 文章 - 伯乐在线
原文出处： [opensource](https://opensource.com/business/16/2/top-issue-support-and-bug-tracking-tools)   译文出处：[51cto](http://os.51cto.com/art/201602/506102.htm)
![1](http://jbcdn2.b0.upaiyun.com/2016/02/28c8edde3d61a0411511d3b1866f0636.jpg)
生活中充斥着大大小小的bug。
无论如何精心加以策划，无论我们在设计工作中投入多少时间，任何项目都会在步入实际运行的过程当中遭遇种种不可预见的问题。这很正常，也许衡量一家企业是否拥有良好弹性的标准并不在于其处理保障一切以规划方式正确起效，而是如何在问题发生时将其识别出来并加以处理。
对于任何项目管理工作流，特别是在软件开发工作当中，一大关键性工具就是问题追踪方案。其基本原理非常简单：允许以协作模式对bug进行开启、追踪及解决，同时简化整个进程推进流程。除了基础功能之外，此类工具还提供大量其他选项，专注于满足特定需求、功能以及用例，具体包括软件开发及其他事务。大家对于此类工具的托管版本可能较为熟悉，例如[GitHub Issues](https://guides.github.com/features/issues/)或者[Launchpad](https://launchpad.net/)，其中一部分本身即属于开源项目。
在今天的文章中，我们将审视四款卓越的bug与问题管理选项，它们全部属于开源项目且能够轻松实现下载与托管。为了简洁起见，我们不会列出当前可用的每款追踪工具；相反，这里选出了四款我们最偏爱的方案，其皆拥有丰富的功能集以及强大的背后技术社区。当然，其他优秀选项亦所在多有，大家可以在评论栏中分享您认为更出色或者自己所喜爱的其他解决方案。
**1.Redmine**
[Redmine](http://www.redmine.org/)是一款高人气问题追踪工具，由Ruby on Rails构建而成且诞生于2006年。作为一款在诸多方面与我们的另一大推荐选项Trac类似的方案，Redmine能够管理多个项目并集成多种版本控制系统。除了基本问题追踪能力，Redmine还提供论坛、维基、时间追踪工具，且有能力生成Gantt图表与日程表以实现进度追踪。
Redmine亦具有非常灵活的设置机制，支持多种数据库后端及数十种语言，同时具备可定制性并允许面向问题、用户以及项目等等添加定制化字段。另外，它还能够进一步利用由社区创建的插件与主题实现其他多种定制化能力。
感兴趣的朋友可以[点击此处](http://demo.redmine.org/)尝试其在线演示。Redmine遵循[GPL version 2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)开源版本；其开源代码可在项目中的[子版本库](https://svn.redmine.org/redmine)或者[GitHub](https://github.com/redmine/redmine)中的镜像处获取。
![2](http://jbcdn2.b0.upaiyun.com/2016/02/665f644e43731ff9db3d341da5c827e1.jpg)
**2.Bugzilla**
[Bugzilla](https://www.bugzilla.org/)是另一款极具人气的开发工具，且具备问题追踪功能。顾名思义，Bugzilla最初由[Mozilla基金会](https://www.mozilla.org/en-US/)开发而成，用于在Netscape Communicator套件开发过程中追踪各类bug。其随后被移植至Perl版本以利用tcl路由实现更出色的可访问能力。Bugzilla是目前发展时间最长且应用最为广泛的问题追踪系统之一，当下有大量知名开源项目将其纳入自身，具体包括GNOME、KDE以及Linux内核本身。
Bugzilla当中囊括有大量先进工具，从通知到复制错误检测再到共享式搜索，Bugzilla已经成为一套功能丰富的解决方案选项。Bugzilla还具备一套综合性报告工具，能够生成图表及自动定时报告，从而建立起一套先进的搜索系统。与Redmine一样，Bugzilla还具备面向字段本身的可扩展性与可定制性，且能够为bug创建定制化工作流。其也能够与多种数据库后端对接，并直接支持大量不同类型的编程语言。
Bugzilla基于Mozilla Public许可，大家可以[点击此处](https://www.bugzilla.org/status/roadmap.html)查阅其未来发展路线图，或者[点击此处](https://landfill.bugzilla.org/)通过其官方网站体验demo server。
![3](http://jbcdn2.b0.upaiyun.com/2016/02/38026ed22fc1a91d92b5d2ef93540f20.jpg)
**3.Trac**
[Trac](http://trac.edgewall.org/browser)将自身描述为一套简约型方案，负责实现基于Web的软件项目管理——但这种极简设计并不会导致基础性功能的缺失。
作为由Python语言编写而成的项目，Trac将自身bug追踪功能与其维基系统紧密加以结合，同时匹配一套由用户选定的版本控制系统。其提供多种项目管理功能，包括生成里程碑与路线图、时限、对多套库提供支持、内置垃圾邮件过滤机制并可用于多种常见语言。与本文提到的其它bug追踪工具一样，其拥有一系列能够对现有功能基础加以进一步扩展的插件选项。
Trac遵循一套经过修改的开源BSD许可，不过其前期版本则遵循 GPL许可。大家可以利用一套[自托管库](http://trac.edgewall.org/browser)查看Trac源代码，或者[点击此处](http://trac.edgewall.org/wiki/TracRoadmap)查看其未来发展规划。
![4](http://jbcdn2.b0.upaiyun.com/2016/02/011ecee7d295c066ae68d4396215c3d0.jpg)
**4.Mantis**
[Mantis](https://www.mantisbt.org/)是我们本篇文章介绍的最后一款工具，这套基于PHP语言的bug追踪工具已经拥有十六年发展历史。这款bug追踪工具支持多种不同版本控制系统以及一套事件驱动型通知系统。Mantis的功能集与本文的其他工具相似。尽管其本身并不提供维基机制，但它亦可与多款高人气维基平台相集成，且已被本地化为多种语言。
Mantis基于[GPL version 2](http://www.gnu.org/licenses/old-licenses/gpl-2.0.en.html)开源许可；大家可以通过[GitHub](https://github.com/mantisbt/mantisbt)查看其源代码，或者检查其自托管[发展路线图](https://www.mantisbt.org/bugs/roadmap_page.php?project=mantisbt&version=1.3.x)以了解未来发展规划。另外，大家也可以[点击此处](https://www.mantisbt.org/bugs/my_view_page.php)检查其内部bug追踪演示工具。
![5](http://jbcdn2.b0.upaiyun.com/2016/02/4e44f1ac85cd60e3caa56bfd4afb675e.jpg)
正如我们所指出，这四款方案绝非唯一选项。除了这四款工具，大家还可以选择Apache Bloodhound、Fossil、The Bug Genie以及其他备选产品，它们也各自拥有独特的优势与短板。那么，大家更偏爱哪些bug追踪与处理工具?请在评论栏中分享您的心得。
