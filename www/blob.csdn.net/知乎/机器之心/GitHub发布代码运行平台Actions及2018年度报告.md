# GitHub发布代码运行平台Actions及2018年度报告 - 知乎
# 



机器之心报道，参与：机器之心编辑部。

> 今年 6 月份，[微软官方宣布以 75 亿美元高价收购 Github](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650743216%26idx%3D3%26sn%3Da4cc5c55f7bb96dc5d0825450e6fa887%26scene%3D21%23wechat_redirect)。今天，据外媒 The Verge 报道，欧盟已经批准了这项交易。在此之前，GitHub 有两项新动作：一是发布 GitHub Actions 平台，开发者不仅能够在该平台上托管代码，还可以运行代码；二是发布了 2018 年度报告，从存储库、语言、项目、热门话题等多个维度对用户数据和平台发展趋势进行了分析。

欧洲反垄断局表示，他们确定「二者合并后将继续面临来自其他同行的持续、激烈竞争。」当局也不担心微软会「破坏 GitHub 的开放性」，他们指出，如果微软对该平台的管理不合开发者的口味，开发者还有其他选择。

微软曾经敌视开源软件，前首席执行官 Steve Ballmer 更是将其称之为「癌症」。「这一历史让依赖 GitHub 的开发者担忧。但是，即将上任的 GitHub 首席执行官 Nat Friedman 表示，微软已经接受了开源软件，开发者不必担心。他表示，「微软就像一个正在康复的酒鬼：它反对开源，但是现在正在采取措施来改变自己的行为，而且现在真的已经改变了」。

[传出收购至今](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650743162%26idx%3D2%26sn%3D33fd3e5301a67066fbe4f54042390b06%26scene%3D21%23wechat_redirect)已经四个多月，GitHub 与微软的联姻也已尘埃落定。GitHub 的未来走向尚不确定，但在欧盟批准这项交易之前，GitHub 发布了一个对开发者颇为友好的平台 Actions 及 2018 年度报告，从中我们可以看出 GitHub 过去一年的发展状况。
![](https://pic2.zhimg.com/v2-a5bf57416f086a3405696f42eed09465_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
**GitHub Actions 平台**

Actions 是 GitHub 的重大更新。GitHub 的平台主管 Sam Lambert 将其描述为「GitHub 历史上的最大转变」。他将 Actions 与 iOS 中的快捷键（shortcut）联系起来，但是比后者更加灵活。Sam Lambert 说到，「想象一个更加灵活的快捷键，托管在 GitHub 上，且允许所有人在容器内创建 Actions 来增强和连接工作流。」

GitHub 用户可以使用 Actions 构建自己的连续输送通道。该公司在发布的公告中写道：「易创建、易发布、易更新，在 GitHub 或其他任何外部系统上使用任何语言轻松部署你的项目——无需自己运行代码。」

Lambert 强调说，「我认为 CI/CD 只是 Actions 一个小小的用例。它还有很多很多用途。它将改变 DevOps。」
![](https://pic4.zhimg.com/v2-73eba2e34cc5ec4b3213958ae64f605f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='606'></svg>)
这意味着，每当有人在你的存储库中使用「紧急问题」标签时，你可以用它通过 Twilio 来发送信息。或者你可以写一个单行命令，用一个基本的 grep 命令搜索你的存储库。亦或是真的运行任何其它你想运行的代码，因为当你要把存储库中的任何代码转换成一个 Actions 时，只需为它编写一个 Docker 文件，GitHub 就可以运行它。Lambert 解释说，「只要有一个 Docker 文件，我们就可以构建它并运行，将其连接到你的工作线程中。」但是，如果你不想写 Docker 文件，还有一个可视化编辑器可以用来构建你的工作线程。

正如 GitHub 的产品工程主管 Corey Wilkerson 所说，这些 Actions 中有很多已经存在于 GitHub 的存储库中。GitHub 上已经有超过 9600 万个了，这使得很多潜在的 Actions 从一开始就可以利用。

现在，Actions 在有限的公共测试版中，开发人员可以设置工作线程来构建、打包、发布、更新和部署他们的代码，而不必自己运行代码。

现在开发者可以自己托管这些 Actions，它们仅仅只是 Docker 容器，只不过 GitHub 会托管和运行代码。虽然 GitHub 为开发者提供了 Actions，但是这是一个开放平台，GitHub 社区中的其他人也可以贡献他们的 Actions。

beta 测试版注册入口：[https://github.com/features/actions](https://link.zhihu.com/?target=https%3A//github.com/features/actions)




**GitHub 2018 年年度报告**

除 Actions 外，GitHub 还发布了 2018 年年度报告。以下是报告的主要内容：

报告显示，GitHub 上的开发者已超过 3100 万，其中，2018 年增加的新用户比以往 6 年的总数都要多。超过 210 万个组织使用 GitHub，比去年增加 40%。管理代码仓库超过 9600 万个，比去年增加 40%。过去一年增加的代码数量几乎相当于平台上代码总数的 1/3。

目前，GitHub 平台共有超过 2 亿的 pull request，超过三分之一是过去 12 个月创建的。

截至 2018 年 9 月 30 日，GitHub 上的开发者超过 3100 万，其中新开发者超过 800 万，7 月份注册人数达到 3000 万。




**贡献者最多的开源项目**

VS Code 再次占据开源项目列表榜首。Facebook、TensorFlow 分别占据榜单第二、三位，贡献者数量分别为 10000 和 9300。新增的项目有管理集装箱化应用程序、共享 Azure 文档和整合 TypeScript 类型定义的 Kubernetes、Azure Docs 和 DefinitelyTyped。
![](https://pic3.zhimg.com/v2-1fa7b5cfdfadc496a1fe81be72f51ece_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='858' height='659'></svg>)
**增长最快的开源项目**

总体来说，与机器学习、游戏、3D 打印、家庭自动化、科学规划、数据分析、全栈 JavaScript 开发有关的项目增势明显。其中，Pytorch 是增长最快的开源项目之一，增加了 1.8 倍。
![](https://pic1.zhimg.com/v2-87bad1372d0f00cdeb9d1a8220158d7c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='882' height='627'></svg>)



**好玩的新开源项目**

今年开源了很多有趣的项目，从机器学习框架到游戏，包括谷歌的快速原型强化学习算法框架 dopamine 以及 Facebook 的最新目标检测算法实现 Detection 等。这些项目不是增长最快或总收入最高的项目，但是我们认为它们是最具明星价值的——社区也这样认为。
![](https://pic1.zhimg.com/v2-4a535f3f5165bc2650822efb90b2e650_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='818' height='552'></svg>)
**标记的热门话题**

今年的数百万个项目跨越了数千个主题。JavaScript（前端和后端）、机器学习、移动应用开发和集装箱化是贡献次数最多的主题。
![](https://pic4.zhimg.com/v2-817c6246c33625ec04d7785b9e0b04db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='457' height='835'></svg>)
**增长最快的话题**

机器学习和 React 是 GitHub 社区的热门话题：机器学习库 PyTorch 及 Gatsby 等基于 React 的网页开发工具都是今年热度增长最快的话题。有关区块链发展不同领域的主题也呈趋势。当然，Hacktoberfest 名列榜首。
![](https://pic2.zhimg.com/v2-f213dc6633bcf591f8a253284cb318e5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='963' height='835'></svg>)
**近年流行语言排行榜**

GitHub 上的编程语言五花八门，但在各种公、私库，各种规模的组织及全球各个地区中，JavaScript 的使用人数始终占据最高位。

TypeScript 在去年首次挤进前十，今年更是跃升到第七位。在贡献者所在的各个地区以及各种公共、私人、开源库中，TypeScript 现在是十大最常用的语言之一。
![](https://pic1.zhimg.com/v2-2515667cfb20d931bd65b597aab4c1d8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='748' height='640'></svg>)
**增长最快的语言**

从报告中可以看到专注于线程安全性和互用性的静态类型的语言越来越受欢迎：Kotlin、TypeScript 和 Rust 今年发展迅速。此外，写 HCL（一种用于 DevOps 的人类可读性语言）的贡献者的数量在过去一年翻了一番还要多。机器学习热门语言 Python 位居第八。今年使用 Go 语言的人数是去年的 1.5 倍。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*![](https://pic3.zhimg.com/v2-820c184580498a51c2f56503b52a47d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='770' height='571'></svg>)
参考链接：

[https://www.theinformation.com/articles/microsoft-wont-mess-up-github-executive-says](https://link.zhihu.com/?target=https%3A//www.theinformation.com/articles/microsoft-wont-mess-up-github-executive-says)

[https://octoverse.github.com/](https://link.zhihu.com/?target=https%3A//octoverse.github.com/)

[https://www.theverge.com/2018/10/19/17999562/microsoft-github-acquisition-approved-eu-antitrust](https://link.zhihu.com/?target=https%3A//www.theverge.com/2018/10/19/17999562/microsoft-github-acquisition-approved-eu-antitrust)

[https://techcrunch.com/2018/10/16/github-launches-actions-its-workflow-automation-tool/](https://link.zhihu.com/?target=https%3A//techcrunch.com/2018/10/16/github-launches-actions-its-workflow-automation-tool/)




