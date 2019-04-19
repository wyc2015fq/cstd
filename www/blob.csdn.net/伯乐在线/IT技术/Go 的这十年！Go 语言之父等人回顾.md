# Go 的这十年！Go 语言之父等人回顾 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [听风](http://www.jobbole.com/members/q4000052677) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[commandcenter](https://commandcenter.blogspot.com/2017/09/go-ten-years-and-climbing.html)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
【导读】：Go 语言的主要缔造者 Rob Pike、Robert Griesemer 与 Ken Thompson，他们三人撰文回顾 Go 的发展史与一些重要人物，并曝光促成 Go 诞生的相关邮件的片段。
![](http://wx4.sinaimg.cn/mw690/7cc829d3gy1fm29qjec1aj20sg0hs0to.jpg)
2017 年 9 月，Go 语言迎来其诞生十周年纪念日。
关于 Go 语言设计的初次讨论是在 2007 年 9 月 20 日，一个周四的下午。这次讨论促使 Robert Griesemer、Rob Pike、 以及 Ken Thompson 于第二天下午 2 点，在 Google’s Mountain View 的 43 号大楼的 Yaounde 会议室又组织了一次关于该语言设计的会议。语言的名字于 9 月 25 号确定了，在有关该语言设计的第一封邮件里面有关于命名的考量：
![](http://wx4.sinaimg.cn/mw690/7cc829d3gy1fm29qkruczj20c207lq3s.jpg)
（译注：这是一封由 Rob Pike 在 2007 年 9 月 25 号，星期二，下午 3：12 回复给 Robert Griesemer、Ken Thompson 的有关编程语言讨论主题的邮件，邮件正文大意为：在开车回家的路上我得到了些灵感。1.给这门编程语言取名为“go”。你可以为这个命名假借各种理由，但它确实有其可取之处。它很简短，易书写。工具类可以命名为：goc、 gol、goa。交互式的调试工具也可以直接命名为“go”。语言文件后缀名为 .go ……）
（值得一提的是，本语言的名字是 Go ；“golang”来自于其 web 站点网址（go.com 在当时已是一个迪士尼 web 站点地址。），并不是该语言的正确名字。）
Go 项目团队将 2009 年 11 月 10 日，即该语言正式对外开源的日字作为其官方生日。源代码最初托管在 code.google.com 上，之后几年才迁移到 GitHub 上。但是，现在我们要回到语言设计的最初概念构建阶段，即更早的两年，这可以让我们做更进一步地回顾，以更长远的视角，见证语言发展历史上的一系列早期事件。
在开发设计 Go 语言过程中的第一个重大惊喜是收到了以下这封邮件：
![](http://wx4.sinaimg.cn/mw690/7cc829d3gy1fm29qldqvxj20bi05qaar.jpg)
(译注：这是一封由 Ian Lance Taylor 在 2008 年 6月 7 日（星期六）的晚上 7：06 写给 Robert Griesemer、Rob Pike、 Ken Thompson 的关于 Go gcc 编译器前端的邮件。邮件正文大意如下：我的同事向我推荐了这个网站 http://…/go_lang.html 。这似乎是一门很有趣的编程语言。我为它写了一个 gcc 编译器前端。虽然这个工具仍缺少很多的功能，但它确实可以编译网站上展示的那个素数筛选程序了。）
![](http://wx3.sinaimg.cn/mw690/7cc829d3gy1fm29qk08vvj20hw0l2tdz.jpg)
（Drawing Copyright ©2017 [Renee French](http://reneefrench.io/)）
Ian Lance Taylor 的加入以及第二个编译器 (gcc go) 的实现 在带来震惊的同时，也伴随着喜悦。这对 Go 项目来说不仅仅是鼓励，更是一种对可行性的证明。语言的第二次实现对制定语言规范和确定标准库的过程至关重要，同时也有助于保证其高可移植性，这也是 Go 语言承诺的一部分。
虽然 Ian 的办公室离我们不远，但在收到此邮件以前，我们三人与他素未谋面。不过，自那以后，Ian Lance Taylor 便成为了设计和实现 Go 语言及其工具的核心人物。
 Russ Cox 在2008 年带着他的语言设计天赋和编程技巧也加入了刚成立不久的 Go 团队。Russ 发现 Go 方法的通用性意味着函数也能拥有自己的方法,这直接促成了 ***[http.HandlerFunc](https://golang.org/pkg/net/http/#HandlerFunc)***的实现，这是一个出乎我们所有人意料的结果。Russ 还提出了更多的泛化性的想法，比如 io.Reader 和 io.Writer 接口，奠定了所有 I/O 库的整体结构。
Jini Kim 是我们最初的产品经理，聘请了安全专家 Adam Langley 帮助 Go 走向 Google 外面的世界。Adam 为我们做了许多不为外人知晓的工作，包括创建最初的 [***golang.org***](https://golang.org/)  网站以及 [***build dashboard***](https://build.golang.org/)。不过他最大的贡献当属创建了 cryptographic 库。起先，在我们中的部分人看来，这个库无论在规模还是复杂度上都不成气候。但是就是这个库在后期成为了很多重要的网络和安全软件的基础，并且成为了 Go 语言开发历史的关键组成部分。许多网络基础设施公司，比如 [***Cloudflare***](https://www.cloudflare.com/)，均重度依赖 Adam 在 Go 项目上的工作，互联网也因它变得更好。因此，就 Go 而言，我们由衷地感谢 Adam。
事实上，有许多公司在项目早期就开始使用 Go 语言进行开发，尤其是一些初创公司。在这些公司当中有些现已成为云计算领域中的巨头。有一家创业公司，现在叫 Docker，就是使用 Go 进行项目开发，并促进了计算机领域的容器行业，进而出现了像 Kubernetes 这样的项目。现在，我们完全可以说 Go 是容器语言，这是另一个完全出乎意料的结果。
不过，Go 在云计算领域中所起的作用更大。2014 年 3 月， Donnie Berkholz 为 RedMonk 撰写文章，声称 Go 是“云计算基础设施的新兴语言”。同时，Apcera 的 Derek Collison 表示，Go 已是一门云计算语言。彼时，这样的表述可能言过其实，但 Berkholz 所使用的“新兴”一词恰如其分地表达了 Go 语言在当时的地位。
如今 Go 已经是云计算编程语言。想象一下：一个只有 10 年发展经历的编程语言已经成为了如此巨大而且正在不断发展的行业的主导者，这种成功是每个人都无法想象的。如果你认为“主导”一词太过强势，请看看中国的互联网。一段时间以来，Go 在中国大量使用的数据一度让我们误认为 Google 趋势图出现了某些错误，但是凡是去过中国，参加过中国区 Go 语言大会的人都可以证实：Google 趋势图的数据是真实的。Go 在中国的使用非常火爆！
简而言之，Go 语言的十年发展旅程中经历了众多里程碑。最令人震惊的是我们现在的位置：保守估计至少有 50 万 Go 程序员。当发送那封为 Go 命名的邮件时，憧景将会有五十万 Go 追随者，这听起来一定很荒唐。但就在此时此刻，我们不仅有 50 万 gopher，并且这个数量还在持续增长。
说到 gophers，很高兴看到来自 Renee French 想法的吉祥物 Go Gopher(地鼠)，不仅成为了一个非常受人喜爱的作品，而且也是世界各地Go 程序员的象征。许多各地区顶级的 Go 大会都被称为 GopherCons，因为他们聚集了来自世界各地的 gophers。
Gopher 会议正在飞速发展。第一次大会的举办只不过是三年前的事情，但今天在全世界各地有很多这样的 Gopher 大会,并且还有无数小的本地“聚会”。无论哪一天，都会有不止一个 gopher 群体相聚在世界上的某个地方分享有关 Go 语言设计的想法。
回顾过去十年的 Go 设计和开发，Go 社区的发展尤其惊人的。技术会议和聚会的数量、长长的且不断增加的 Go 项目贡献者名单、大量使用Go 实现的开源代码库、使用 Go 开发公司数量（有些甚至只使用 Go 开发）：任何一项数据考量后都足以让人震惊！
就我们三人而言，Robert、 Rob 和 Ken，当初只是想让我们的编程生活更轻松一些，而如今，我们难以置信地、欣慰地看到我们的工作已经开始起作用了。
未来十年它将会带来什么呢？让我们拭目以待。
– Rob Pike、Robert Griesemer 和 Ken Thompson
