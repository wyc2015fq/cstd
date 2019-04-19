# 微软拥抱 Linux，着实太晚了 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [alvendarthy](http://www.jobbole.com/members/alvendarthy) 翻译，[Panblack](http://www.jobbole.com/members/upanblack) 校稿。未经许可，禁止转载！
英文出处：[Paul Venezia](http://www.infoworld.com/article/3050845/microsoft-windows/microsoft-embraces-linux-way-too-late.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
如果你错过了，简单回顾一下最近微软发布的一堆关于 Linux 的声明。首先，微软发布了 [SQL Server for Linux](http://www.infoworld.com/article/3041450/sql/8-no-bull-reasons-why-sql-server-on-linux-is-huge-for-microsoft.html)。紧接着在3月31日的微软 Build 2016 开发者大会中，微软推出了“[Windows 运行原生 Ubuntu 程序](http://www.infoworld.com/article/3049715/microsoft-windows/bash-on-windows-is-just-the-beginning-for-microsofts-linux-experiments.html)”功能，并且在 Windows10 中提供了 Bash 的预览版本。
![](http://ww1.sinaimg.cn/large/7cc829d3jw1f311263yo5j20h70bhwgz.jpg)
微软现在开始支持 Linux 版本 SQL Server，其本质并非技术层面的升级——只是一种商业行为而已。值得微软庆幸的是，鲍尔默的短视已成为历史，现在这家公司清楚看到 Linux 系统才是未来云计算的必然选择，而非 Windows。微软从很久之前就输掉了这场对抗。众所周知，Azure 云的成员们最早推动了 Windows 支持 Linux（还有其他基于云计算的技术，包括 Docker\Kubernetes，以及大量的 NoSQL 数据库）。
提供 Linux 版本的微软 SQL Server 应该会增加营收，而非降低营收。微软 SQL Server 的所有竞争对手都可以运行在 Linux 上，但微软 SQL Server 却不能，所以微软正在错失销售和相关的支持合同。实际上微软 SQL Server 是一款可靠的数据库，并且早应该在很久之前开始跨平台工作。Linux 版本 SQL Server 的推出，将必然使得运行 SQL Server 的老版本 Windows 系统改为 Linux 机器，但对微软来说，这比改为运行 Oracle 或者 DB2 的 Linux 机器要好的多。
Windows 支持 Linux 的消息很令人振奋，但是还是需要澄清一些事实。这项技术并不是运行 Linux 虚拟机——系统中根本没有实现 Linux 内核，也不基于虚拟仿真硬件。它不是 [Cygwin](https://www.cygwin.com/)（一种专门用于在 Windows 平台运行 Linux 环境的程序）。它也不是进程容器。Windows 10 上运行的 Ubuntu 环境与 Ubuntu 平台二进制无差别——都是 ELF 可执行文件。
微软所做的是，构建了一套系统调用转换层。当某个 Linux 程序进行系统调用时，微软的 [Windows 下 Linux 子系统](https://technet.microsoft.com/en-us/library/cc771672.aspx)就将其转换为 Windows 系统调用，并且输出程序期望的结果。这与 WINE 类似，WINE 为了使 Windows 程序运行在 Linux 上做了类似的工作。也如同 WINE 一样，它也不是十全十美的——很多程序不能“直接运行”。这只是微软漫长征途的第一步。
最根本的问题：这一切都是如何发生的？为什么经过这么长的时间后，微软突然决定跟 Canonical 合作，并且开发了这一套转换层？首先是与发布 SQL Server for Linux 一样的原因——这家公司在开发者、系统管理员、产品运营工程师等等群体中，不断丧失桌面市场。如今这些工程师相较于 Windows 平台，他们更多工作在 Linux 平台。而且，在笔记本上用 Windows 开发 Linux 应用，往好了说是笨重，往差了说是无用。
开发者们不能很方便的在 Windows 笔记本上配置基于 Linux 的应用开发环境，他们需要在别的某处部署这样的环境，要么是一台远程主机，要么就是 Windows 上的虚拟机。很快，这一系列配置带来的麻烦显而易见，开发者们就趋向于使用 Linux 笔记本，或者（更常见的）购买Mac，因为基于 BSD 的 OS X 很容易满足他们的需要，还是原生的。
这也是在研发部门办公室、会议室，以及其他开发人员集中的场所，你都会看到一片 Apple 商标。究其根本，当你在 *nix 系统上工作，无论如何也是用 *nix 电脑更便捷。更何况这个世界正在向主要由 Linux 系统构建的云端迁移，这种趋势更是无法阻挡。
这不是微软拥抱 Linux，甚至不是微软一贯“拥抱，扩展，消灭”的传统作风，而是微软最终踏步投入 Linux 怀抱。由于微软当初错失良机，微软将会在此进程中面临诸多严重的问题，但是至少现在它有所动作了。从前，这一切都是不可能的，微软的霸主地位让它拒绝接受众人皆见事实： Linux 正在夺取云计算服务器的市场空间，无论如何，云计算是世界趋势。
微软错失潮流的历史悠久，富有戏剧性，最早可追述到“[640K 内存对人人应该都够用](http://www.computerworld.com/article/2534312/operating-systems/the--640k--quote-won-t-go-away----but-did-gates-really-say-it-.html)”的言论，完全忽视了互联网催生的虚拟化产业将会是多重要的事。微软并不是真正的变革者。一旦它进入某一行业，它便利用其巨大的体量，投入资源，最终获得可行的、有竞争力的产品或服务。
但是这一次，微软确实是太太太晚进入这场角逐，而且它不是在提供一种有竞争力的产品——它正努力适应竞争，只为求自保。这种状况前所未有，观其如何收场将是很有意思的。
与此同时，我确信不会出现大量开发者、管理员涌向 Windows 10 上高度受限的 Ubuntu 应用系统的潮流。他们会继续使用他们的 Mac 和 Mint 电脑。
