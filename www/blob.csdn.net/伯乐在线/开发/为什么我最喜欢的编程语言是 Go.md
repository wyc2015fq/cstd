# 为什么我最喜欢的编程语言是 Go - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [tsteho](http://www.jobbole.com/members/q3198108035) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Michael Stapelberg](https://michael.stapelberg.de/Artikel/golang_favorite)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
我力图尊重每个人的喜好，因此我通常会避开相关争论：哪种编程语言、文本编辑器或者操作系统才是最好的。然而，我最近很多次被问到为什么我喜欢并且大量使用 [Go](https://golang.org/)，所以写下这篇文章 :-)
![](http://wx2.sinaimg.cn/large/7cc829d3gy1fknpksz2oej20h30aymzi.jpg)
### 我的经历/背景
我已经使用 C 和 Perl 进行了很多规模宏大的项目。我也用 Python、Ruby、 C++、CHICKEN Scheme、Emacs Lisp、Rust和Java (仅仅针对Android )编写程序。我了解一些 Lua、PHP、Erlang 和 Haskell。我以前使用 [Delphi](https://en.wikipedia.org/wiki/Delphi_(programming_language)) 开发了很多程序。
在 2009 年，当它首次发布时，我简要地了解了一下 Go。当 Go1.0 在 2012 年发布时，我认真地开始使用该语言。Go1.0 的一个特色是 [Go 1 兼容性保证](https://golang.org/doc/go1compat)的发布。我仍然在生产环境中运行着这些[代码](https://github.com/stapelberg/greetbot) ，它们是我在2012年编写的，基本上没有修改过。
# 一、清晰性
### 格式化
按照惯例，Go使用 [gofmt](https://golang.org/cmd/gofmt/) 来格式化代码。以编程方式来格式化代码并不是新的想法，但与它的先辈们相比，[gofmt](https://golang.org/cmd/gofmt/) 严格地支持一种规范风格。
用相同的方式格式化所有代码，使得阅读代码更容易，因为代码令人感觉似曾相识。这不仅有助于阅读标准库或 Go 编译器，也有助于和许多代码库打交道—想想看开源或者大公司。
此外，在代码审查（Code Review）期间，自动格式化能大大节省时间，因为在代码能够被审计前，代码的规范风格不再需要人为处理：现在，你能让持续集成系统验证 [gofmt](https://golang.org/cmd/gofmt/) 并没有产生差异。
有趣的是，有了编辑器在保存文件时应用 gofmt，我写代码的方式也已经改变。我曾经试图匹配格式化程序执行后的内容，之后再让gofmt 更正我的错误。如今，我尽可能快地表达我的想法，并且相信 gofmt 能够使得格式化的更好（这个[例子](https://play.golang.org/p/I6GJwiT77v)是我可能会写的内容，点击Format）。
### 高质量的代码
我使用标准库（[文档](https://golang.org/pkg/)，[源码](https://github.com/golang/go/tree/master/src)）相当多，具体参见下文。
所有我已经阅读的标准库的质量都是极其高的。其中一个例子就是 [image/jpeg](https://golang.org/pkg/image/jpeg/) 包：我当时并不知道 JPEG 是如何工作的，但通过在阅读维基百科中有关介绍 JPEG 的文章和阅读 [image/jpeg](https://golang.org/pkg/image/jpeg/) 源码之间切换，我轻易地明白了JPEG是如何工作的。如果包中还有一些注释，我把它看作教学实施。
### 观点
我已经慢慢地认同 Go 社区秉承的很多观点，比如：
- [变量](https://github.com/golang/go/wiki/CodeReviewComments#variable-names)默认情况下应该简短，并且能够从它的声明中进一步变得更具描述性。
- 保持依赖关系树短小精悍（以一种合理的程度）：[有点重复代码比一点依赖更好](https://www.youtube.com/watch?v=PAAkCSZUG1c&t=9m28s)。
- 引入抽象层是有代价的。Go代码通常是相当清晰的，但这是有代价的，因为有时候代码有点重复。
- 参见[代码审计说明](https://github.com/golang/go/wiki/CodeReviewComments)和[Go格言](https://go-proverbs.github.io/)获取更多信息。
### 很少的关键字和抽象层
Go 规范仅仅列出了 [25 个关键字](https://golang.org/ref/spec#Keywords)，而我能很容易地记在脑中。
[内建函数](https://golang.org/pkg/builtin/)和[类型](https://golang.org/ref/spec#Types)也是同样的情况。
根据我的经验，少量的抽象层次和概念使得该语言容易学习，并且很快感到适应。
当我们在谈论它时：我对 [Go 规范](https://golang.org/ref/spec)的可读性感到惊讶。它真的看起来是针对程序员（而不是标准委员会？）
# 二、速度
### 反馈快/延迟低
我喜欢快速的反馈：我喜欢快速加载的网站，我更喜欢流畅、不会滞后的用户界面，并且任何时候我都将选择一个快速的工具，而不是一个功能更强大的工具。大型网站性能的[调查结果](https://blog.gigaspaces.com/amazon-found-every-100ms-of-latency-cost-them-1-in-sales/)证明这种行为被大量用户认可。
Go 编译器的作者满足了我对低延迟的期望：编译速度对他们来讲很重要，并且新的优化需要仔细地衡量是否会降低编译速度。
我有一个之前没有用过 Go 的朋友。在使用 go get 命令安装了 [RobustIRC](https://robustirc.net/) 桥之后，他得出结论，Go 一定是一种解释性语言，我不得不纠正他：不，Go 编译器本来就这么快。
大部分 Go 工具也不例外，例如 gofmt 或者 goimports 是惊人的快。
### 最大化利用资源
对于批量应用程序（相对于交互式应用程序），充分利用可用资源通常比低延迟更重要。
配置和更改 Go 程序利用所有可用的 IOPS、网络带宽和计算能力是很容易的。例如，我之前写过[填充 1Gbps 的链路信息](https://people.debian.org/~stapelberg/2014/01/17/debmirror-rackspace.html)，并且优化 [debiman](https://github.com/Debian/debiman/) 利用所有可用的资源，这减少了几小时运行时间。
# 三、丰富的标准库
[Go 标准库](https://golang.org/pkg)提供了有效地使用通用通讯协议和数据存储格式/机制的方法，如TCP/IP、HTTP、JPEG、SQL等。
Go 的标准库是我迄今看到的最好的标准库。我认为它组织良好、清晰、精细又全面：我经常发现只需使用标准库和一两个外部包就可以编写出大小合理的程序。
领域特定的数据类型和算法（通常）不包括在标准库内，而是包括在第三方库中，例如 [golang.org/x/net/html](https://godoc.org/golang.org/x/net/html)。在新代码合入标准库之前，golang.org/x命名空间也充当了新代码的暂存区：[Go 1 兼容性保证](https://golang.org/doc/go1compat)排除任何破坏性的改变，即使这些改变看起来很明显是有价值的。golang.org/x/crypto/ssh就是一个典型的例子，为了[建立更安全的默认配置](https://github.com/golang/crypto/commit/e4e2799dd7aab89f583e1d898300d96367750991)，它不得不打破现有的代码。
# 四、工具
我使用 go get 工具来下载、编译、安装和更新 Go 包。
所有我用到的 Go 代码库都用内建的 [testing](https://golang.org/pkg/testing/) 工具。这不仅令测试容易和快速，而且使得[覆盖率报告](https://blog.golang.org/cover)很容易生成。
每当一个程序使用比预期更多的资源时，我就使用 pprof。参见 golang.org 发布的有关 pprof 的[博文](https://blog.golang.org/profiling-go-programs)，或者我发布的有关优化 Debian 代码搜索的[博文](https://people.debian.org/~stapelberg/2014/12/23/code-search-taming-the-latency-tail.html)。在导入 net/http/pprof [包](https://golang.org/pkg/net/http/pprof/)之后，你可以在服务器运行时对其进行配置，而无需重新编译或重新启动。
交叉编译就像设置 GOARCH 环境变量一样简单，例如树莓派3 只需设置 GOARCH=arm64。值得注意的是，工具也能够跨平台运行！比如，我能在我的 AMD64 电脑上配置 [gokrazy](https://gokrazy.org)：go tool pprof ~/go/bin/linux_arm64/dhcp [http://gokrazy:3112/debug/pprof/heap](http://gokrazy:3112/debug/pprof/heap)。
[godoc](https://godoc.org/golang.org/x/tools/cmd/godoc) 以纯文本形式显示文档，或者通过 HTTP 提供服务。[godoc.org](https://godoc.org/) 是一个公共实例，但是我在本地运行了一个实例，这样我就可以在离线时或者包还未发布时使用了。
需要注意的是，这些标准工具和 Go 语言是互相伴随的。如果这些标准工具来自于 C 语言，上述每一项将是一个重大的成就。在 Go 中，我们认为这是理所应当的。
### 开始学习
希望我已经说清楚了，为什么我喜欢使用Go。
如果你有兴趣开始使用 Go，请核查[初学者的资源](https://github.com/gopheracademy/gopher/blob/1cdbcd9fc3ba58efd628d4a6a552befc8e3912be/bot/bot.go#L516)获取相关信息，这也是我们引导刚加入Gophers [Slack](https://slack.com/)频道的人需要核查的。参见[https://golang.org/help/](https://golang.org/help/)。
### 注意事项
当然，没有编程工具是完全没有问题的。鉴于这篇文章解释为什么 Go 是我最喜欢的编程语言，所以该文只讨论了好的一面。不过，我将顺便地提及一些问题：
- 如果你使用的 Go 包还没有提供稳定的 API，你可能想要使用特定的、已知的能够正常运行的版本。最好的办法是用 [dep](https://github.com/golang/dep) 工具，在编写 Go 语言时，它还不是 Go 的一部分。
- 地道的 Go 代码不一定能转化为最高性能的机器代码，而且运行时间需要付出一些（小的）代价。在极少的情况下，我发现有性能缺乏的情况，我成功地诉诸于 [cgo](https://golang.org/cmd/cgo/) 或者汇编器。如果你的范围是硬实时应用程序或其他性能极其关键的代码，你的情况就可能不一样了。
- 我之前写到 Go 标准库是我见过的最好的标准库，但是那不意味着它一点问题都没有。其中一个例子是 go/ast—标准库中最古老的包之一，当以编程的方式修改 Go 代码时，用它[处理注释时会很复杂](https://golang.org/issues/20744)。
