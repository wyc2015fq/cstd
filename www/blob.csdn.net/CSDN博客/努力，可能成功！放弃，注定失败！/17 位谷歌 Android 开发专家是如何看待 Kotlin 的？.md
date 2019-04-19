# 17 位谷歌 Android 开发专家是如何看待 Kotlin 的？ - 努力，可能成功！放弃，注定失败！ - CSDN博客
置顶2017年06月07日 10:41:14[上天眷顾我](https://me.csdn.net/qq_30513483)阅读数：374标签：[android																[Kotlin](https://so.csdn.net/so/search/s.do?q=Kotlin&t=blog)](https://so.csdn.net/so/search/s.do?q=android&t=blog)
个人分类：[Android](https://blog.csdn.net/qq_30513483/article/category/6189211)
> 
译者简介：ASCE1885， [《Android 高级进阶》](https://item.jd.com/11999029.html)作者。
- 微信公众号：asce1885
- 小密圈：Android高级进阶，详情见[这篇文章](https://mp.weixin.qq.com/s?__biz=MzAwMTYwNzE2Mg==&mid=2651036684&idx=1&sn=71adbe8bfc795e0940bf13530d91f805&chksm=81202899b657a18f37db2169c7f81fec561b9513cba263436cf6919c5559cc1a27005be3fce6#rd)。
- [Github](https://github.com/ASCE1885)，[简书](http://www.jianshu.com/users/4ef984470da8/latest_articles)，[微博](http://weibo.com/asce885/profile?rightmod=1&wvr=6&mod=personinfo)，[CSDN](http://blog.csdn.net/asce1885)[知乎](https://www.zhihu.com/people/asce1885)
原文链接：[https://antonioleiva.com/google-kotlin/，](https://antonioleiva.com/google-kotlin/%EF%BC%8C) 由于 Markdown 对图片的排版存在限制，因此为了保证排版的美观，本文对作者介绍部分进行截图，不做另外的翻译。
![](http://upload-images.jianshu.io/upload_images/191937-0ded8b0ccb4225cf.jpg?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
Google 已经多次表达过他们不反对 Kotlin 的使用，只要 Kotlin 编译器依然生成有效的字节码，那么 Google 就不会阻止任何人使用它。
但很多人依然在等待官方的正式支持，一件可能永远不会发生的事情。
虽然我们等到了这一刻，但我想如果能够知道 Google 的 Android 开发专家是如何看待 Kotlin 的，那就更好了。
如果你没有听说过，那你应该了解下，Google 开发专家（GDEs）[[1]](http://www.jianshu.com/p/16601cf9fce8#fn_lemma_1)是一个旨在表彰优秀开发者对所在领域所作出的贡献的项目。
我联系了一些这样的 Android 专家（人员列表很长！），我得到了来自 17 位的答复。非常感谢你们这么好能够花时间来回答。
我只是要求他们告诉我们关于 Kotlin[[2]](http://www.jianshu.com/p/16601cf9fce8#fn_lemma_2) 的一点想法，答案显然是未经过修改的，所以你可以发现支持和反对（或者不那么支持）的声音。
我希望这有助于你知道 Kotlin 是如何成为一个游戏规则改变者的，同时如果你是一名 Android 开发者，我希望 Kotlin 会成为你考虑使用的一门语言。
其他的就不多说的，让我们来看看 GDEs 们的看法吧，排名不分先后：
### 1. Annyce Davis
![](http://upload-images.jianshu.io/upload_images/191937-d0dbe5f67c117f40.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MacHi 2017-05-29 21-18-37.png
我以前使用 Groovy 编程语言开发过 Grails 应用，对于我而言，Groovy 是对 Java 一个极大的改进。它很精简，并有助于避免当时使用 Java 6 语言编码时不得不遇到的很多样板代码。当 Kotlin 出现时，我立刻被它吸引了，因为它的很多语法特性让我想起了 Groovy。它既整洁又精炼。
我是一个狂热的学习者，所以我渴望学习 Kotlin 并基于 Kotlin Koans 开始我的学习之路。我也订购了《Kotlin in Action》的早期访问版本。当听了 Hadi 的 Kotlin 播客后，我被推到了正视 Kotlin 的边缘。当 Kotlin 1.1 发布后，我最终决定正视 Kotlin，并在我的线上应用中引入 Kotlin。
对于 Android 开发者而言，Kotlin 有很多优点。最明显的是它的类型系统和对空类型的处理，这迫使你在编码时指明哪些变量可为空，并在使用的时候遵循这个约定，之后编译器就会介入并确保对变量的赋值都是有效的。空指针异常[[3]](http://www.jianshu.com/p/16601cf9fce8#fn_lemma_3)是我在
 Android 应用程序中处理的最常见的异常类型。Kotlin 有助于公平的竞争环境。
Kotlin 另外一个显著的优点是具备扩展函数[[4]](http://www.jianshu.com/p/16601cf9fce8#fn_lemma_4)的能力，通过给 `Context`，`Activity` 和 `Date` 类添加扩展函数，使得我的代码简洁了很多，同时变得更加易于阅读。
通过使用 Kotlin，我的代码变得更好了。
### 2. Sebastiano Poggi
![](http://upload-images.jianshu.io/upload_images/191937-c558b29f07944e09.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/1240)
MacHi 2017-05-30 20-34-13.png
我真的真的很喜欢这门语言。我个人认为它在很多情况下使得编码变成一件愉快的事情，相比之前，同样的情况下用 Java 语言编码将会感到痛苦和繁琐。我绝不是一个 Kotlin 专家，因为我还没有在日常工作中使用它，但每次做小项目时我都会使用它。你知道传说中的 10 倍效率的开发者吗？Kotlin 有时让我觉得自己就像一个
 10 倍效率开发者（我一直不认为有人能够称为 10 倍效率的开发者，所以这对我来说是一个好的开始）。
简洁的语法是这门语言非常明显的好处，除此之外，我真的很感激它提供的所有附加的 API 和围绕它构建的生态系统。例如，不可变集合是我经常需要的东西，但如果不在应用中（部分）引入 Guava 函数库的话，在 Java 语言中是用不了的，但 Kotlin 对不可变集合提供了一级实现，此外还有一堆的其他特性。哦，我说过当看到 Kotlin
的发布版本将对 LLVM 的支持作为一个目标时，我有多兴奋吗？
然而正如我刚才提到的，我的日常工作并没有使用 Kotlin。虽然有理由相信 Kotlin 的引入将会使项目大大受益，但这个项目却很难引入 Kotlin，因为这个项目的团队规模很大，我苦恼的发现 Kotlin 存在的主要问题：静态代码分析工具的缺乏和成熟设计模式的缺乏。
如果你的项目不大且不复杂，那么后者是一个相对次要的问题，但如果你遇到的是有十个开发者在一个有将近十年历史的巨大的代码仓库中协同工作时，具有良好的设计模式将会使你更加的安心。当然，你可以使用 Java 设计模式，但并不是所有的 Java 设计模式在 Kotlin 中都是有意义的，因为有些设计模式是专门围绕 Java 语言设计的，存在一定的局限性。这是一个漫长的讨论，我将在这里中止它，否则整篇文章就变成这个讨论了
