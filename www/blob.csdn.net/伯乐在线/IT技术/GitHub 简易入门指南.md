# GitHub 简易入门指南 - 文章 - 伯乐在线
原文出处： [Linuxandubuntu](http://www.linuxandubuntu.com/home/getting-started-with-github)   译文出处：[Linux中国/firmianay](https://linux.cn/article-8814-1.html)
![Getting Started With GitHub](http://jbcdn2.b0.upaiyun.com/2017/08/e1c1cc8db1f52eec31747b94474b595a.jpg)
[GitHub](https://github.com/) 是一个在线平台，旨在促进在一个共同项目上工作的个人之间的代码托管、版本控制和协作。通过该平台，无论何时何地，都可以对项目进行操作（托管和审查代码，管理项目和与世界各地的其他开发者共同开发软件）。**GitHub 平台**为开源项目和私人项目都提供了项目处理功能。
关于团队项目处理的功能包括：GitHub 流Flow>和 GitHub 页Pages。这些功能可以让需要定期部署的团队轻松处理工作流程。另一方面，GitHub 页提供了页面用于展示开源项目、展示简历、托管博客等。
GitHub 也为个人项目提供了必要的工具，使得个人项目可以轻松地处理。它也使得个人可以更轻松地与世界分享他们的项目。
### 注册 GitHub 并启动一个项目
在 GitHub 上启动新项目时，您必须先使用您的电子邮件地址创建一个帐户。
![github homepage](http://jbcdn2.b0.upaiyun.com/2017/08/01c7882ef18db3d3f3887534261dc45c.jpg)
然后，在验证邮箱的时候，用户将自动登录到他们的 GitHub 帐户。
#### 1、 创建仓库
之后，我们会被带到一个用于创建仓库repository的页面。仓库存储着包括修订历史记录在内的所有项目文件。仓库可以是公开的或者是私有的。公开的仓库可以被任何人查看，但是，只有项目所有者授予权限的人才可以提交修改到这个仓库。另一方面，私有仓库提供了额外的控制，可以将项目设置为对谁可见。因此，公开仓库适用于开源软件项目，而私有仓库主要适用于私有或闭源项目。
- 填写 “仓库名称Repository Name” 和 “简短描述Short Description”。
- 选中 “以一个 README 文件初始化Initialize this repository with a README”。
- 最后，点击底部的 “创建仓库Create Repository” 按钮。
![create a github repository](http://jbcdn2.b0.upaiyun.com/2017/08/14fcf6fd8154adb9a3c89cf0fa0a93fb.jpg)
#### 2、 添加分支
在 GitHub 中，分支branch是一种同时操作单个仓库的各种版本的方式。默认情况下，任何创建的单个仓库都会被分配一个名为 “MASTER” 的分支，它被认为是最后一个分支。在 GitHub 中，分支在被合并到主干master（最后的分支）之前，可以在对仓库进行实验和编辑中发挥作用。
为了使项目适合每一个人的需求，通常情况下，总是需要添加几个格外的分支来匹配不同的项目。在主分支上创建一个分支和复制主分支时的当前状态是一样的。
![add a branch to github repository](http://jbcdn2.b0.upaiyun.com/2017/08/09f5d75c22e6dee20daa51b241e8a809.jpg)
创建分支与在不同版本中保存单个文件是类似的。它通过在特定仓库上执行的任务重命名来实现。
分支在保持错误修复和功能添加工作中同样被证明是有效。在进行必要的修改后，这些分支会被合并到主分支中。
在创建仓库后创建一个分支：
- 在这个例子中，点击仓库名称 “Hello-World” 跳转到你的新仓库。
- 点击顶部的 “Branch:Master” 按钮，会看到一个下拉菜单，菜单里有填写分支名称的空白字段。
- 输入分支名称，在这个例子中我们输入 “readme-edits“。
- 按下回车键或者点击蓝色的 “创建分支create branch” 框。
这样就成功创建了两个分支：master 和 readme-edits。
#### 3、 修改项目文件并提交
此步骤提供了关于如何更改仓库并保存修改的指导。在 GitHub 上，提交commit被定义为保存的修改的意思。每一次提交都与一个提交信息commit message相关联，该提交信息包含了保存的修改的历史记录，以及为何进行这些更改。这使得其他贡献者可以很轻松地知道你做出的更改以及更改的原因。
要对仓库进行更改和提交更改，请执行以下步骤：
- 点击仓库名称 “Hello-World”。
- 点击右上角的铅笔图标查看和编辑文件。 ![commit changes to github repository](http://jbcdn2.b0.upaiyun.com/2017/08/05ce163a30316ed081d9d40cf125afa5.jpg)
- 在编辑器中，写一些东西来确定你可以进行更改。
- 在提交消息commit message字段中做简要的总结，以解释为什么以及如何进行更改。
- 点击提交更改 commit changes按钮保存更改。
请注意，这些更改仅仅影响到 readme-edits 分支，而不影响主分支。
![commit branch to master](http://jbcdn2.b0.upaiyun.com/2017/08/5424b71e6ca81e83e4abdaa2b6f369ee.jpg)
#### 4、 开启一个拉取请求
拉取请求pull request是一个允许贡献者提出并请求某人审查和合并某些更改到他们的分支的功能。拉取请求还显示了几个分支的差异（diffs）。更改、添加和删减通常以红色和绿色来表示。一旦提交完成就可以开启拉取请求，即使代码还未完成。
开启一个拉取请求：
- 点击拉取请求pull requests选项卡。 ![github pull request](http://jbcdn2.b0.upaiyun.com/2017/08/ff952c1eb62fd8adbd387eeb38e9673e.jpg)
- 点击新建拉取请求new pull requests按钮。
- 选择 readme-edits 分支与 master 分支进行比较。 ![compare commit changes github](http://jbcdn2.b0.upaiyun.com/2017/08/c5eec8f40ee29eb37ecbf19b41584b56.jpg)
- 确定请求，并确定这是您要提交的内容。
- 点击创建拉取请求绿色按钮并输入一个标题。 ![open a pull request in github repository](http://jbcdn2.b0.upaiyun.com/2017/08/c581f47872c3af18d32d2dff1d05a4ad.jpg)
- 按下回车键。
用户可以通过尝试创建并保存拉取请求来证实这些操作。
#### 5、 合并拉取请求
最后一步是将 readme-edits 分支和 master 分支合并到一起。如果 readme-edits 分支和 master 分支不会产生冲突，则会显示merge pull request合并拉取请求的按钮。
![merge the pull request github](http://jbcdn2.b0.upaiyun.com/2017/08/c45ba98c142ca89fa2544a69baab31d6.jpg)
当合并拉取时，有必要确保评论comment和其他字段被正确填写。合并拉取：
- 点击merge pull request合并拉取请求的按钮。
- 确认合并。
- 按下紫色的删除分支按钮，删除 readme-edits 分支，因为它已经被包含在 master 分支中。（LCTT 译注：如果是合并他人提交的拉取请求，则无需也无法删除合并过来的他人的分支。）
本文提供了 GitHub 平台从注册到使用的基本操作，接下来由大家尽情探索吧。
