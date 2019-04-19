# GetStream.io：我们用 Go 替换 Python 的原因 - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [tsteho](http://www.jobbole.com/members/q3198108035) 翻译，[艾凌风](http://www.jobbole.com/members/hanxiaomax) 校稿。未经许可，禁止转载！
英文出处：[Nick Parsons](https://codeburst.io/why-we-switched-from-python-to-go-60c8fd2cb9a9)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
![](https://cdn-images-1.medium.com/max/1600/1*NRRnK49Q9HuOh9r9G4L-pg.png)
切换到新的编程语言始终是一件大事，尤其是在这种严峻的情况下：团队成员中仅有一人具备该种语言的使用经验。今年年初，我们将 [Stream’s](https://getstream.io/) 的主要编程语言从 Python 切换到了 Go。 这篇文章将给出一些理由以说明两个问题：为什么我们决定舍弃 Python？又是为什么选择了 Go？
## Reasons to Use Go
## 为什么使用 Go？
### Reason 1 — Performance
### 原因 1 – 性能
Go 的运行速度非常快。性能类似于 Java 或者 C++。对于我们的使用情况来说，Go 一般比 Python 快 30 倍。这里有个小型的测试游戏 [Go vs Java](https://benchmarksgame.alioth.debian.org/u64q/go.html)。
### 原因 2 – 语言的性能很重要
对于很多应用来说，编程语言只是应用程序和数据库之间的粘合剂。语言本身的性能通常无关紧要。
然而，Stream 是一家 API 提供商，其为 500 家公司和 2 亿多终端用户提供信息流基础设施。我们一直在优化 Cassandra、PostgreSQL 和 Redis 等工具。这持续了好几年，但是最终，我们还是到达了所用语言的瓶颈。
Python 是一门很棒的语言，但是在诸如序列化/反序列化、排序以及聚合之类的场景上，它的性能相当差。我们常常遇到性能上的问题：花 1ms 的时间拿到 Cassandra 中的数据，Python 接下来还需要花 10ms 的时间将拿到的数据转换成对象。
### 原因 3 – 开发人员的开发效率 & 拒绝太富有创造性
看看这些来自于 [How I Start Go tutorial](http://howistart.org/posts/go/1/) 的 Go 的代码片段。（这是一个很棒的教程，也是一个学习 Go 语言的一个很好的起点。）
如果你是 Go 语言新手，在阅读那些代码片段时，没有什么会让你大吃一惊。这些代码段仅仅演示了go语言的一些特性，例如赋值、数据结构、指针、格式化和内置的 HTTP 库。
当我第一次开始编程时，我总是喜欢用 Python 比较高级的特性。Python 允许你更有“创意”的写代码。比如，你能够做如下的事：
- 在代码初始化时使用元类自行注册类
- 关键字 True 和 False 的值可以互换
- 编写自己的函数，并且使其成为内建函数。
- 通过魔法方法重载运算符
这些特性很有趣，但是，正如大多数程序员都同意的那样，在阅读别人的代码时，它们的存在使得代码更难理解。
Go 迫使你回归基础。这决定了 Go 代码是容易阅读和理解的。
说明：当然，“容易”的程度需要视情况而定。如果你想要创建一个基本的增删改查接口，我仍然推荐你使用 Django + [DRF](http://www.django-rest-framework.org/)，或者用 Rails。
### 原因 4 – 并发 & 管道
作为一门编程语言，Go 试图让事情变得简单。它没有引入很多的新概念。重点是创造的这门编程语言的性能要难以置信的快，并且容易上手。goroutines 和管道是 Go 仅有的创新点。（准确的讲，[CSP](https://en.wikipedia.org/wiki/Communicating_sequential_processes) 这个概念1977年就被提出了，因此这个创新更准确的说法是——旧点子的新实现）Goroutines 是 Go 对线程的轻量级实现，而管道是让 goroutines 之间相互通信的绝佳的方式。
Goroutines 占用的资源非常少，只需要几 KBs 的额外内存。因为 Goroutines 非常轻量，所以同时运行数百甚至数千个也不在话下。
你可以使用管道在 goroutines 之间通信。Go 运行时会处理所有的复杂事物。goroutines 的存在以及基于管道的并发方法，使得程序可以充分利用 CPU 资源、处理并发 IO — 所有这些都没有增加开发的复杂性。与 Python/Java 相比，在 goroutine 上运行一个函数只需要非常少的样板代码。您只需在函数调用前加上关键字“go”：
[https://tour.golang.org/concurrency/1](https://tour.golang.org/concurrency/1)
和 Node 相比，Go 的并发处理更加容易。如果使用 Node 写并发方法，开发者必须密切关注异步代码的处理方式。
Go 自带[竞争检测器](https://blog.golang.org/race-detector)，这是用 Go 写并发程序另一个好的方面。如果异步代码中出现条件竞争的情况，检测器能帮你轻松地找到问题。
如果要学习 Go 和管道的话，下面是一些不错的资料：
- [https://blog.golang.org/race-detector](https://blog.golang.org/race-detector)
- [https://tour.golang.org/concurrency/2](https://tour.golang.org/concurrency/2)
- [http://guzalexander.com/2013/12/06/golang-channels-tutorial.html](http://guzalexander.com/2013/12/06/golang-channels-tutorial.html)
- [https://www.golang-book.com/books/intro/10](https://www.golang-book.com/books/intro/10)
- [https://www.goinggo.net/2014/02/the-nature-of-channels-in-go.html](https://www.goinggo.net/2014/02/the-nature-of-channels-in-go.html)
- [Goroutines vs Green threads](https://softwareengineering.stackexchange.com/questions/222642/are-go-langs-goroutine-pools-just-green-threads)
### 原因 5 – 编译时间短
目前，我们使用 Go 编写的最大微服务只需 6 秒钟就能完成编译。与 Java 和 C++ 这样以低速编译速度著称的语言相比，Go 的快速编译能力是一场生产力上的大胜。我也喜欢趁着代码编译的时间去放松一下，但是，如果能在我还记得代码是做什么事情的时候就完成编译，岂不更好？况且本来就应该是这样的才对。
![](https://cdn-images-1.medium.com/max/1600/1*N5NJvvxy-D9TrSwXGaMsCQ.png)
### 原因 6 – 创建一个团队的能力
首先，让我们认清一个现实：与 C++ 和 Java 这样的老牌编程语言相比，Go 开发人员的数量是不占上风的。根据 [StackOverflow](https://insights.stackoverflow.com/survey/2017) 的数据，38% 的开发人员熟悉 Java，19.3% 的开发人员熟悉 C++，仅仅 4.6% 的开发人员熟悉 Go。[GitHub 上的数据](https://madnight.github.io/githut/)显示一个[相似的趋势](https://madnight.github.io/githut/)： Go 用得比 Erlang、Scala 以及 Elixir 广泛，但是不及 Java 和 C++。
幸运的是，Go 很简单，而且易于学习。它提供了你所需要的基本的特性，一点不多，一点不少。它引入了 2 个新的概念：“[defer](https://blog.golang.org/defer-panic-and-recover)”声明、“go routines” 和管道内建的并发管理。（对于纯粹主义者来说：Go 并不是第一种实现这些概念的语言，而是第一种使它们受欢迎的语言。）团队中任何地 Python、Elixir、C++、Scala 或 Java 开发人员都可以在一个月内有效地掌握 Go，因为它非常简单。
我们发现，和很多其他的编程语言相比，创建一个 Go 开发团队更容易。如果你在竞争激烈的环境（如 [Boulder、Amsterdam](http://angel.co/stream/jobs)）雇佣人员，这是一大优点。
### 原因 7 – 强大的生态系统
对于我们一个大约 20 个人的团队来说，生态系统很重要。如果你不得不重新发明每一部分的功能，你根本不可能为你的客户创造价值。Go 对我们使用的工具提供了很大的支持。比如这些可靠的库：Redis、RabbitMQ、PostgreSQL、模板解析、任务调度、表达式解析和 RocksDB。
与 Rust 或 Elixir 等其他新语言相比，Go 的生态系统是一项重大胜利。当然，Go 并不像 Java、Python 或者 Node 那样出色。但是它非常的可靠，并且对于一些基本的需求，你都可以找到高质量的包。
### 原因 8 – Gofmt：强制代码格式化
那么什么是 Gofmt 呢？注意，它并不是脏话。Gofmt 是一个极棒的命令行工具集，已集成到了 Go 编译器，用于格式化代码。从功能上来讲，它有点像 Python 中的 autopep8。除非是在《硅谷》电视剧中，不然大多数人并不真的喜欢争论该用 tabs 还是 spaces。格式的一致性是非常重要的，但是实际的格式标准并不是那么重要。Gofmt 提供官方的标准来格式化你的代码，从而避免了不必要的争论。
### 原因 9 – gRPC 与 Protocol Buffers
Go 对 protocol buffers 和 gRPC 有着一流的支持。在构建需要通过 RPC 进行通信的微服务时，这两个工具可以很好地协同工作。你只需编写一个说明文件，里面只需定义可以进行的 RPC 调用以及它们采用的参数。根据这份说明文件，服务器和客户端代码就会自动生成。由此产生的代码运行快速，网络占用空间小，易于使用。
根据相同的说明文件，甚至可以生成很多不同编程语言的客户端代码，比如 C++、Java、Python 和 Ruby。因此，内部流量不再有模糊的 REST 终端，因为你不必每次都写一遍几乎相同的客户端和服务器端代码。
## 使用 Golang 的缺点
### 缺点 1 – 缺少框架
Go 没有一个具有代表性的框架，像 Ruby 有 Rails、Python 有 Django 或者 PHP 有 Laravel。在 Go 社区中，这是一个争论激烈的话题，很多人提倡不应该一开始就使用框架。某些使用案例，我完全同意这样的观点。然而，如果只是想要创建一个增删改查的接口，使用 Django/DJRF、Rails Laravel 或者 [Phoenix](http://phoenixframework.org/) 是一个更好的选择。
### 缺点 2 – 错误处理机制
Go 处理错误的过程如下：简单地从函数中返回错误，并且期望你调用代码来处理该错误（或者将它返回到调用堆栈之上）。虽然这种方法有效，但很容易丢失出错的范围，导致无法为用户提供有意义的错误。 [errors](https://github.com/pkg/errors)包通过允许你为错误添加上下文和堆栈来跟踪问题。
另一个问题是很容易忘记处理错误。像 errcheck 和 megacheck 这样的静态分析工具可以方便地规避这些错误。
虽然这些解决方法很有效，但总感觉哪里不太对劲。 你肯定希望语言本身就支持一定的错误处理的功能。
### 缺点 3 – 包管理
Go 的包管理肯定不是完美的。默认情况下，它没有办法指定依赖项的特定版本，也没有办法创建[可重现的构建](https://en.wikipedia.org/wiki/Reproducible_builds)。 Python、Node 和 Ruby 都有更好的包管理系统。然而，通过合适的工具，Go 的包管理表现的很好。
你可以使用 [Dep](https://github.com/golang/dep) 来管理依赖项以允许指定和固定版本。 除此之外，我们还提供了一个叫做 [VirtualGo](https://github.com/getstream/vg) 的开源工具，它可以更轻松地处理用 Go 编写的多个项目。
![](https://cdn-images-1.medium.com/max/1600/1*OVIzfruxudBtFpzISIEriA.png)
## Python vs Go
我们之前做过一个有趣的实验：选择我们的 [ranked feed](https://getstream.io/docs/#custom_ranking) 功能，用 Go 语言将它重写。简单看下这个排名方法的例子：
为了使这个排名方法成立，Python 和 Go 都需要遵循下面的事：
- 解析表达式以便打分。在这种情况下，我们希望将“simple_gauss（time）* popular”这个字符串转换成一个函数：函数以一个活动作为输入，然后返回一个分数作为输出。
- 基于 JSON 配置创建偏函数。比如：我们想要“simple_gauss”调用“decay_gauss”，并传递规模为 5 天，偏差为 1 天，衰减系数为 0.3 这些参数。
- 解析“默认值”配置，以便在活动中出现未定义字段时可以进行回退。
- 使用步骤 1 中的函数给流中的所有活动打分。
开发 Python 版本的排名代码大约需要 3 天。这包括编写代码、单元测试和文档书写。接下来，我们花了大约 2 周时间来优化代码。其中一个优化是将评分表达式（simple_gauss(time)*popularity）转换为抽象[语法树](https://docs.python.org/3/library/ast.html)。我们还实现了缓存逻辑，该逻辑在将来的某些时间预先计算得分。
相比之下，开发该代码的 Go 版本大约需要 4 天时间。性能不需要任何进一步的优化。因此，虽然 Python 初始的开发速度更快些，但如果基于 Go 的版本，最终，我们团队的工作量大大减少。作为额外的优点，Go 代码的执行速度比我们高度优化的 Python 代码快大约 40 倍。
这只是一个简单的说明性能提升的例子：仅仅用 Go 替换 Python。 当然，它们没有可比性：
- 排名代码是我第一个用 Go 写的项目
- Go 代码是在 Python 代码之后构建的，因此我可以更好地理解用例
- 用于表达式解析的 Go 库是非常高质量的
具体细节需要视情况而定。和 Python 相比，用 Go 构建一些我们系统中其他的组件，需要花费更多的时间。一般情况下，我们发现用 Go 开发代码更费些劲。然而，在性能方面，我们花费更少的时间来优化代码。
## Elixir vs Go
我们评估了另一种语言：[Elixir](https://elixir-lang.org/)。Elixir 构建于 Erlang 虚拟机之上。这是一种引人入胜的语言。我们考虑过它，因为我们团队成员中有一个人拥有大量的 Erlang 经验。
对于我们的用例，我们注意到 Go 的原始性能要好得多。Go 和 Elixir 都可以很好地为数千个并发请求提供服务。但是，如果你查看单个请求的性能，Go 对我们的用例来说要快得多。生态系统是另一个我们选择 Go 而不选择 Elixir 的原因。对于我们需要的组件，Go 有更多成熟的库，而在许多情况下，Elixir 库还没有为生产使用做好准备。培训/招聘用 Elixir 的开发人员也更难。
这些原因让我们选择了 Go。虽然 Elixir 的 Phoenix 框架看起来非常棒，并且也绝对值得一看。
## 总结
Go 是一种非常高效的语言，且对并发性有很大的支持。它的性能几乎与 C++ 和 Java 等语言一样快。虽然和 Python 或 Ruby 相比，使用 Go 构建内容需要花费更多时间，但你将节省大量时间来优化代码。
我们在 [Stream](https://getstream.io/team/) 有一个小型的开发团队，为超过2亿的终端用户提供信息流。拥有一个伟大的生态系统、新开发人员容易上手、快速的性能、对并发性的可靠支持以及高效的编程环境，使 Go 成为一个很好的选择。
Stream 仍然利用 Python 为我们的控制面板、站点和机器学习提供[个性化的流](https://getstream.io/personalization)。 我们不会很快告别 Python，但是所有性能密集型代码都将用 Go 编写。
如果你想要了解更多有关 Go，查看下面列出的博客文章。如果你想要了解 Stream，[这个互动教程](https://getstream.io/get_started/)是一个好的起点。
## More Reading about Switching to Golang
## 关于切换到 Golang 的更多阅读
- [https://movio.co/en/blog/migrate-Scala-to-Go/](https://movio.co/en/blog/migrate-Scala-to-Go/)
- [https://hackernoon.com/why-i-love-golang-90085898b4f7](https://hackernoon.com/why-i-love-golang-90085898b4f7)
- [https://sendgrid.com/blog/convince-company-go-golang/](https://sendgrid.com/blog/convince-company-go-golang/)
- [https://dave.cheney.net/2017/03/20/why-go](https://dave.cheney.net/2017/03/20/why-go)
## Learning Go
## 学习 Go
- [https://learnxinyminutes.com/docs/go/](https://learnxinyminutes.com/docs/go/)
- [https://tour.golang.org/](https://tour.golang.org/)
- [http://howistart.org/posts/go/1/](http://howistart.org/posts/go/1/)
- [https://getstream.io/blog/building-a-performant-api-using-go-and-cassandra/](https://getstream.io/blog/building-a-performant-api-using-go-and-cassandra/)
- [https://www.amazon.com/gp/product/0134190440](https://www.amazon.com/gp/product/0134190440)
> 
这篇文章最初由 Thierry Schellenbach 撰写，[GetStream.io](https://getstream.io/) 的 CEO。原始博文的地址是[https://getstream.io/blog/switched-python-go/](https://getstream.io/blog/switched-python-go/)。
