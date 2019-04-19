# Scala 之父 Martin Odersky 访谈录 - 文章 - 伯乐在线
本文作者： [伯乐在线](http://blog.jobbole.com) - [ThoughtWorks](http://www.jobbole.com/members/thoughtworkschina) 。未经作者许可，禁止转载！
欢迎加入伯乐在线 [专栏作者](http://blog.jobbole.com/99322)。
2016年3月，笔者有幸和诸多对Scala感兴趣的人一起，跟Scala的创始人Martin Odersky做了一次面对面的交流。下面是这次交流中的一些问题的整理，采用问答的形式，根据问答内容做了简单的摘要。
![Mark_Odersky_photo_by_Linda_Poeng](http://ww1.sinaimg.cn/mw690/7cc829d3jw1f47ewhemzzj20bj0hs0vo.jpg)
**提问者**：我看到[Spark Core](https://github.com/apache/spark/tree/master/core)里面有很多OOP风格的代码，这是为什么？
**Martin Odersky**：Spark的API设计是和Scala 集合类设计是一致的函数式风格，里面具体的实现为了追求性能用了命令式，你可以看到Scala集合里面的实现函数为了性能也用了很多var。
**提问者**：很高兴看到你将在Coursera上再发布两门[Scala的课程](https://www.coursera.org/courses?languages=en&query=scala)。但我想问一下，怎么培训那些大学没有毕业的人用Scala?
**Martin Odersky**：这是个很有意思的问题，我们在Coursera上的学生绝大多数是大学毕业的。我现在还不知道针对大学没毕业学生的培训材料。但是有用Scala教10几岁孩子的教材，他们学得很好。
**提问者**：go语言有很严格的规范，但Scala没有，每个项目的代码风格都不一样怎么办？
**Martin Odersky**：对，go严格限制了程序员的选择，而Scala相信程序员自己会做出正确的选择。每个项目可以根据自己的情况制定合适的规则。
**提问者**：有没有推荐的Scala编码指导？
**Martin Odersky**：你可以用[Spark的Scala编程规范](https://github.com/databricks/scala-style-guide)，虽然有点保守，但比较适合有大量新人的项目。[lihaoyi](http://www.lihaoyi.com/)也写了博客来说明怎么样进行选择。
**提问者**：很不幸的是我们的项目上往往会有很多新人，我带了很多Scala项目，遇到的最大的挑战是如何保持代码简洁，能给些意见吗？
**Martin Odersky**：有两个原则：1. 尽量用能力弱的功能；2. 给中间步骤命名。
**提问者**：现在Scala除了大数据领域，还有其他哪些主要的应用领域？
**Martin Odersky**：Scala还在金融领域很流行，比如高盛，摩根斯坦利等。
**提问者**：现在很多创业公司喜欢用[node.js](https://nodejs.org/), 比如Uber他们一开始用nodejs, 现在开始转向go，你怎么看？
**Martin Odersky**：用nodejs可以使得前后台一致，但长期维护成本会很高。沃尔玛加拿大用Scala，沃尔玛美国选择用nodejs，但后来沃尔玛美国也改成用Scala了。
**提问者**：但是很多初创公司可能活不到明天，他们不太需要考虑长期可维护性啊。
**Martin Odersky**：这也有道理，不过硅谷很多初创公司选择使用Scala。
**提问者**：Scala有没有不合适的应用领域？
**Martin Odersky**：非JVM的目前不行。还有一些非常靠近硬件而且对实时性可预测性能要求很高的应用也不是很适合。
**提问者**：2015年JavaOne Keynote讲到Java8使生产效率提升了15%， 那么Scala那些特性能使生产效率提升？
**Martin Odersky**：Java8这个统计数据很有意思，Scala相比Java生产率能提升2~3倍。
**提问者**：我们的项目代码看起来像Java，怎么变得函数式？
**Martin Odersky**：你可以多用map，filter，pattern match等，而少用循环和if判断。
**提问者**：有个谣言说一个twitter的VP说如果能重新选择他会重新考虑是否用Scala，还有LinkedIn好像也要退出Scala？
**Martin Odersky**：你说了这是个谣言，不是真的。这个话是一个从twitter跳槽的uber的工程师说的，而不是VP说的，他说如果你能忍受员工两个月的学习成本才能用Scala。LinkedIn确实是制定了从基础设施中退出Scala的决定，尤其是Kafka现在独立运作了。主要是因为Scala二进制不兼容造成的。而LinkedIn的应用程序将继续使用Scala。
**提问者**：Tasty是为了解决Scala二进制不兼容的问题，那会在2.12发布吗？
**Martin Odersky**：我们还需要时间，可能在2.12发布不了。
**提问者**：[Dotty](https://github.com/lampepfl/dotty)什么时候能成为Scala默认的编译器？
**Martin Odersky**：我从三年前开始设计Dotty,现在刚刚有个可运行的初始版本，还需要几年才会成为Scala默认的编译器。
**提问者**：Dotty会带来一些新的功能，比如trait可以有参数，会是Scala 3.0吗？同时也会删除一些功能吗，比如抽象类？
**Martin Odersky**：Dotty会重新定义Scala，会是Scala 3.0，但这还需要几年的时间。是否删除抽象类是个艰难的决定，抽象类有它自己的作用，比如与Java兼容。并且抽象类和trait在语义上也有不同。
**提问者**：你在演讲中提到限制使用隐式转换，同时又提了typeclass，那隐式转换要怎么用呢？
**Martin Odersky**：限制使用implicit def。 隐式参数和隐式类是好的，可以使用implicit class。
**提问者**：大系统比较难维护怎么办？
**Martin Odersky**：有副作用的组件和隐含的全局状态会导致组件之间耦合，使得系统维护困难。
**提问者**：我问最后一个问题，最近MIT发表了用机器学习来编程的论文，你怎么看？
**Martin Odersky**：这个话题很有意思值得观察，不过不管怎么样机器还是得知道人要干什么。我们现在可以用机器学习来辅助编程，例如集成在IDE里面帮助程序员更快更好的编程。
