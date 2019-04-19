# 如何 DIY 自己的 Git 托管项目？别怕，我有 Pagure！ - 文章 - 伯乐在线
原文出处： [fedoramagazine](https://fedoramagazine.org/pagure-diy-git-project-hosting/)   译文出处：[LinuxStory](http://www.linuxstory.org/pagure-diy-git-project-hosting/)
Pagure是一个用Python编写的新的、功能齐全的、提供 Web 服务的 Git仓库。它类似于Github 和 Gitlab ，同时允许开源贡献者分享彼此的资源，实现代码和内容上的合作。顺便说一句， Pagure 法语译为“寄居蟹”，在项目文本的 LOGO 上容易发现这一点哦。
Pagure 是 Fedora Engineering 团队成员—— Pierre-Yves Chibon 的心血结晶。 FedoraEngineering 团队专注于基于 Python 的解决方案，因为 Python 简单易学，从而降低了贡献者的入门门槛。因此 Pagure 不仅提供了项目托管服务，同时也鼓励新人维护并改进这个服务。
Facebook 、 Google 等公司已经表明： Python 是可以做到高性能的。 Pagure 也不例外，它给 Web 用户提供了不错的体验。你可以看到数十个高度活跃的项目已经被托管到 Pagure 上了，当然 Pagure 本身的代码也托管到上面。
**Pagure 的功能**
Pagure 提供了类似 Git 程序提供的很多功能：
- 用户，群组，项目库和克隆服务的自助创建。
- 支持 HTTPS 和 SSH 访问代码仓库。
- 支持多认证登录，可以通过 Pagure 像 Ipsilon 这样提供 OpenID 的外部认证登录。所以你可以链接 Pagure  到 LDAP 、系统账号或者其他的账户。
- pull 请求基于工作流，有完整的讨论和评价。
- Web 接口提供文本编辑器。
- 能够执行 signoffs 和对 pull 请求进行评价打分。
- 问题跟踪，文档和版本管理。
- 对仓库活动提供邮件提醒服务。
- 支持不同 Pagure 实例的同步。
- 完全的自由、免费。
- 还有……很多很多…….
不仅如此，Pagure 在内部还提供以下额外的功能：
- 所以的问题和 pull 请求的追踪元数据是作为侧仓库来提供可追踪性和可移植性的。
- Web 文件夹和项目在线管理中将文档作为侧仓库进行管理。
- 每个项目可以选择完全可扩展的 web-hooks 系统。
**托管自己的 Git 项目**
Pagure 拥有允许用户建立自己的托管服务项目的能力。这项服务是低开销的并依赖于其他自由软件的，这些自由软件你可以在 Fedora 和其他发行版中找到。该 项目文档可以帮助你开始部署 Pagure ，你可以使用普通工具从基于 RPM 的发行版中安装或者使用源码部署 Pagure ，两种方法文档中都提供了。
Pagure 已经在 Fedora 中用于生产几个月了。然而，一个重要的里程碑才刚刚出现：1.0版本的发布。第一个主要版本提供了全新的用户界面。感谢 Fedora Engineering 团队的设计师（ Ryan Lerch ）进行的全新设计。
Pagure 的使用方式同 Github 等同类系统没有太大的区别。Pagure 假设项目是协作的，所以它们没有命名空间。拷贝，却在另一方面是存在命名空间的，这使得上游的仓库清楚明了。虽然拷贝一个上游仓库的方法有些不同。但是 pull 请求的方法是相同的。因此其它 Git 系统的用户立刻上手 Pagure是没有问题的。
![171416002862539](http://jbcdn2.b0.upaiyun.com/2016/02/3fc709b59b6ed7cf667102a77c912084.jpg)
**Pagure 的未来**
Pagure 的发展仍在继续，很多人都在进行修复和完善。未来可能会添加以下的一些功能：
- 自定义主题。
- 同步如 Github 等其它服务商仓库。
- 支持个人项目的命名空间。
- 支持如 Mailman 3 之类的邮件列表。
对有一点 Python 基础的人来说，Pagure 的代码是十分容易理解的。贡献者欢迎大家查看问题清单并参与到项目中。为了修复一个 bug ，你可以拷贝 Pagure 的代码，克隆到自己的仓库中，提交和推送修复补丁，并提交一个 Pagure 的 pull 请求。
