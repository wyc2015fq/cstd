# 分析了 7500w+ GitHub 代码仓库 哪门语言热度最高？ - 文章 - 伯乐在线
原文出处： [开源中国社区](http://www.oschina.net/news/92822/ranking-programming-languages-by-github-users)
近日，有开发者在 reddit [发布](https://www.reddit.com/r/programming/comments/7sxser/ranking_programming_languages_by_github_users/)了一个帖子，内容是通过对 GitHub 用户的数据进行分析得到的编程语言排序榜。作者认为，通过计算 GitHub 上有多少人使用每种语言来对所有的编程语言进行排序会很有趣。
下面我们不妨看看这份数据源“独特”的排行榜有哪些值得关注的东西。
首先，作者表示这次分析的数据主要来源于 [GitHub Archive](https://www.githubarchive.org/) 和 [GHTorrent](http://ghtorrent.org/)。他通过分析 GitHub Archive 上 7500w+ 个不同的仓库中超过 12.5 亿次的事件（包括推送新代码、fork、star、issue 处理等），每月汇总一次，计算了每种语言的 MAU(月活跃用户数量)，详细的分析说明可[点此查看](https://github.com/benfred/github-analysis/#inferring-languages)。
先看看总体数据，截至 2018 年 1 月 24 日，通过统计 GitHub 上活跃用户的活动情况，对每种语言的排名结果如下(TOP 25)：
![](http://jbcdn2.b0.upaiyun.com/2018/01/fc887734f13170721d21150266c78b0f.png)
整体排名十分有趣，但我们应该深入了解这些语言随着时间的推移有着怎么的演变。（数据可以追溯到 7 年前）
**主流编程语言**
![](http://jbcdn2.b0.upaiyun.com/2018/01/9f50396b32bcc8cd904aa87e9e97086e.png)
可以看到，主流编程语言都有着相对稳定的用户群，而且大多数也都是你意料之中的。
从追踪的数据来看，JavaScript, Python, Java, C++ 和 C 保持这种热度的势头已经超过 7 年，所以近段时间依然不会有所改变。
JavaScript 保持它的“龙头”地位是可以解释的，因为它基本上是所有程序员都需要使用的一门语言。
不过随着时间的推移，Python 的使用者越来越多，最近已超过 Java 成为 GitHub 上第二大流行的编程语言。Python 的增长大部分似乎都来自开发者对机器学习的兴趣。事实上，Python 的整体流行度在这里可能被低估了（以后可能会更多）。
C++ 似乎也正在超越 C 的地位，而这也是有根据的，像 GCC 这种项目都已经从 C 转换到 C++，以便使用 C++ 的一些特性。由于 C++ 基本上是 C 的超集，GCC 团队发现[使用 C++ 的有限子集可让他们编写更干净的代码](https://lwn.net/Articles/542457/)。
**2018 年要学习的编程语言**
观察这种编程语言趋势的最好方法就是，确定具有快速增长的用户群的新兴编程语言：
![](http://jbcdn2.b0.upaiyun.com/2018/01/d8411b438b8276b946a43f1a946b143f.png)
明显能看到，用户群增长最快的语言分别有：Go，TypeScript，Kotlin 和 Rust。
作者表示，在过去的几周里他也一直在学习 Go 和 Typescript。他发现，对于编写并发的服务器端程序，Go 确实非常有用，而在使用 TypeScript 几个星期后，自己可能再也不会乐意使用 JavaScript 了。
Kotlin 的崛起是意料之中的，Kotlin 主要用于 Android 应用程序开发，当被宣布成为 Android 开发的官方支持语言后，看看图中的斜率变化情况。
对于 Rust，在这里的增长虽然比其他语言慢，但其中依然有许多令人惊叹的项目。作者推荐了两个用 Rust 编写的两个项目 —— [用于 Ruby 的采样分析器](https://github.com/rbspy/rbspy)和 [autodifferentiation 框架](https://github.com/maciejkula/wyrm)。
事实上，所有这些语言都有一个共同点，它们背后都有大公司在支持着。正如谷歌之于 Go，微软之于 TypeScript，JetBrains 之于 Kotlin，Mozilla 之于 Rust。成功推广一门新的语言需要不少的付出 —— 至少仅仅开发出一门优雅的语言是不够的，还必须扩大语言背后的社区和生态系统。诸如 IDE 支持，用于常见任务的库和软件包、工具和文档，这些对于让用户使用某种语言都非常重要，而且都需要付出巨大的努力。
**出现下滑趋势的语言**
![](http://jbcdn2.b0.upaiyun.com/2018/01/52a6027178298cf4ac5fcb4fd2cdfca6.png)
在过去的 7 年里，Ruby，PHP，Objective-C，CoffeeScript 和 Perl 在 GitHub 上的用户比例都显着下降。
不过值得注意的是，这些数据是以 GitHub 用户群的百分比得出的，而 GitHub 的用户数量正在快速增长。在过去的 7 年中，GitHub 的用户增长已经超过了 20 倍。这意味着即使是市场份额下降的语言也可能拥有不断增长的用户群。
![](http://jbcdn2.b0.upaiyun.com/2018/01/0cb76bf1f6742f4bffa034266b101d3a.png)
这样来看，相比 2011 年，使用 Ruby 语言的活跃用户数量已经超过了3倍。它的增长没有其他语言那么快，导致它在这个分析上的表现相对较差。
此外要注意的是，某些较新的语言似乎在蚕食旧版语言的用户群。例如，Objective-C 用户的下降与 Swift 的上升相对应。另外，CoffeeScript 似乎已经被 TypeScript 取代：
![](http://jbcdn2.b0.upaiyun.com/2018/01/1bbc04d35c7761d2c22bbab0a085feaf.png)
虽然 Objective-C 正在下滑，但整体上 iOS 开发相对稳定。同样的，CoffeeScript 似乎已经为 TypeScript 铺平了道路，因为它让程序员习惯于将代码编译成 JavaScript。
**函数式编程语言**
![](http://jbcdn2.b0.upaiyun.com/2018/01/c44dae18e5d68f2769917e1d42df0ee9.png)
作者表示，这里的数据相对较少，排名中的干扰因素较多，他不相信随着时间的推移，Clojure 和 Haskell 会出现这样明显的下滑。而 Elixir 这门语言值得大家留意，虽然未能排在前 25 名。
文中作者还对用于科学领域的语言进行了分析，详情可查看[原文](http://www.benfrederickson.com/ranking-programming-languages-by-github-users/)。作者还表示未来对编程语言流行度的跟踪将会参考其他一些数据，如 TIOBE 指数、Stack Overflow 上的标签趋势图、GitHub Octoverse 等。
源码和整体排名可在 [GitHub](https://github.com/benfred/github-analysis) 上查看。
最后，reddit 帖子的回帖中，有用户给出了自己统计的数据以进行对比参考，他统计了 reddit 上十大最受欢迎的编程语言的 subreddits 数量（ subreddits 是 reddit 中的子版块，用户可根据自己的兴趣和需要进行订阅）
> 
Python – 213594
JavaScript – 199592
Java – 81241
PHP – 58794
C++ – 58788
C# – 52103
Go – 39529
Ruby – 38405
Rust – 33124
C – 32351
