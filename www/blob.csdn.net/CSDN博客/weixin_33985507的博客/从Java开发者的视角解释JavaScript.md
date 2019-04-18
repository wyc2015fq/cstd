# 从Java开发者的视角解释JavaScript - weixin_33985507的博客 - CSDN博客
2014年12月03日 08:55:00[weixin_33985507](https://me.csdn.net/weixin_33985507)阅读数：6
　我们无法在一篇博文里解释JavaScript的所有细节。如果你正或多或少地涉及了web应用程序开发，那么，我们的[Java工具和技术范围报告](http://zeroturnaround.com/rebellabs/java-tools-and-technologies-landscape-for-2014/)揭示了，大多数（71%）Java开发者被归到了这一类，只是你对JavaScript遇到了阻碍。
　　毫无疑问，你已经知道了Java和JavaScript，不管它们有着多么类似的命名，彼此没有共享太多共通之处。Java的静态类型、符合直接规律的简单语法和冗长，与JavaScript的动态、缺乏一致性原则和[怪异](http://www.admin10000.com/document/5524.html)，有着巨大的不同。
　　然而，JavaScript是web的编程语言，最近由于Node.js和JVM自己的Nashorn JavaScript引擎的发展，在服务器端获得了相当的注意。
　　本文，我不想只是漫谈JavaScript的好与不好，或重复任何人都能免费找到的、不计其数的JavaScript教程。我想列出一些有助于理解JavaScript做为一种语言的技术点，并从接近[horse](https://twitter.com/horse_js)的角度来理解。
　　我们将在本文包含下列语言级别的技术点：
- JavaScript的通用性
- JavaScript的函数编程问题
- 不同于Java的继承
　　另外，你会找到一些工具方面的推荐，没有这些工具，你是不想着手JavaScript项目的，包含了构建系统的代码质量分析和测试框架方面的工具。
### 　　优点
### 　　编写一次，差不多处处运行！
　　毋庸置疑JavaScript是web编程语言，是很多其它语言的编译目标，也是用来证明有时候人们只是想拥有更多自由时间的终极方式。尽管如此，这不是一件坏事。每一台能够浏览现代网站的电脑都装备了具有性能和可用的JavaScript引擎。最重要的是，JavaScript代码可以在后端运行。
　　内置到我们喜爱的JVM的、轻量级高性能JavaScript运行时Nashorn，完全能够[解释JavaScript脚本](https://blog.codecentric.de/en/2014/06/project-nashorn-javascript-jvm-polyglott/)，还能够解释项目中带有Java代码的JavaScript脚本。
　　鉴于每台电脑运行时都可获得的自由，JavaScript成为Java体验的完美延续。
### 　　函数式编程：一等公民是函数，而不是递归
　　JavaScript中的函数是第一类公民，它们是值，可被存储在变量里、传递给其它函数、在适当的时候再执行。
　　这打开了函数式编程世界的大门，这是结构化JavaScript编程的完美方式。
　　注意，JavaScript里的对象是任何东西的映射，对象的每个特性（attribute）都在同一个映射里：函数、属性（property）、构造器；易变性带来了更大的隐患，而对于Java，你至少能够确保方法和字段结构在某种程度上是稳定的。
　　反过来，这使得函数式编程更加有利：涉及到小的、可理解函数和不变的数据结构是在JavaScript里运行的方式。
　　这不是没有依据的，下面是在JavaScript里定义一个reduce函数的例子，来自于《[Eloquent JavaScript](http://eloquentjavascript.net/)》一书。
```
`function``forEach(array, action) {`
`for``(``var``i = 0; i < array.length; i++) {`
`action(array[i]); ``//apply action to every element of the arra.`
`}`
`}`
` `
`function``reduce(combine, base, array) {`
`forEach(array, ``function``(element) {`
`base = combine(base, element); ``// and here we apply function passed as ‘combine’ parameter to ‘base’ and ‘element’`
`});`
`return``base;`
`}`
` `
`function``add(a, b) { ``// btw, this is how you define a function in JavaScript`
`return``a + b;`
`}`
` `
`function``sum(numbers) {`
`return``reduce(add, 0, numbers);`
`}`
```
　　注意：我们没有在这里使用reduce的递归版本。JavaScript没有以尾调用【注1】为特色，这意味着每个函数的递归版本都将用到栈的深度，和Java一样，如果你递归太深，程序就崩溃。
### 　　继承：就像真实的世界
　　JavaScript的继承是基于原型的。即，你没有扩展了其它类型的类型，而实际上，你拥有的实例从其它实例继承了功能。
　　想象一下，对象A就像一个映射，我们刚才稍微提到了一些、但是用了不同的视角，然后另一个类似映射的对象B从A继承了一切。
　这说明B可以访问A所有部分：A的方法、字段等等。
> 
在实践中，我从来没有看到有人实际使用简单的基于原型的继承。通常当某人需要继承时，他只是构造类，因此你可以用到所有广泛的技能，和基于类的继承的工作模式。
——Rene Saarsoo，XRebel前端工程师
　　我不太确定Java开发者应该从中吸取什么，但是要当心继承方式的不同，对于父级对象要格外留意、而不要意外地改变整个程序的行为。
### 　　任何时候要避免的
　　列出不可靠的JavaScript设计上的决定比想象中要容易。在JavaScript程序中要避免的最明显的地方就是全局变量的声明。
　　注意，在JavaScript里，无论什么时候，不使用var关键词定义变量，那么定义的变量被推到了它们被定义的作用域顶端。这意味着，每个用这种方式定义的变量将跑到全局范围顶部，这会引发冲突以及你和同事不可预期的头痛。
　　可以开启[strict模式](https://developer.mozilla.org/en-US/docs/Web/JavaScript/Reference/Strict_mode)。只需在脚本文件顶部写上“use strict”，那么不经意编写的全局变量声明将显示错误。
　　JavaScript与Java另一个重要的不同点在于，前者是动态类型语言，其真谛是所有东西都可以是任何类型。这很明显了，实在不能再强调了：不要针对不同类型的值，去复用相同的变量。
　　跟踪刚开始是个string类型的变量，但是现在它成了浮点数、或者函数了，相信我！
　　还有，我不想太深入[类型和布尔值的讨论](https://www.inkling.com/read/javascript-definitive-guide-david-flanagan-6th/chapter-3/type-conversions)，但是要警惕JavaScript引擎扔给你的隐式类型转换。
### 　　搞定工作的小提示
　　正如我上面提到的，在编程上要更加注意这种语言的语法和怪癖，而不仅仅是知道。项目很少由于语言的不足而失败，更多的失败是与总体项目框架不足有关。下面是有助于你交付项目的一些工具。
### 　　静态代码分析
　　大部分项目是不同的，其复杂度和需求导致了大量的细节，你该如何着手代码库呢。尽管如此，在所有地方都有一致性的目标，那就是代码质量。
　　是的，代码质量，对于任何开发者来说，最重要的工作就是交付。但是不要在质量上妥协，不要对你提交的代码感到不自信就不情愿与同事分享。
　　幸运的是，JavaScript有一套得体的解决方案——[JSHint](http://www.jshint.com/)。JSHint是为JavaScript量身打造的静态分析工具，与应用于Java代码的FindBug类似。JSHint可以在你的代码库运行，并高亮出可疑的或有问题的地方，即使你不会马上产生bug，但这些地方将来变得难以维护。在项目中支持它[相当简单](http://www.jshint.com/install/)。帮自己一个忙——如果你在写JavaScript代码，就用JSHint让它更安全、少一些尴尬。
### 　　REPL
　　REPL代表“读取-求值-输出”循环（Read-Eval-Print Loop）【注2】，是很多动态语言的强大工具。如果你看过Scala或Groovy，你一定能够理解这个概念。
　　激活JavaScript REPL的一种途径是打开浏览器的控制台，它产生了对JavaScript代码求值的界面。
![浏览器控制台调试JavaScript](http://www.admin10000.com/UploadFiles/Document/201411/27/20141127081853905854.PNG)
　　另一个比较方便的工具是jjs，它捆绑在JDK1.8。
![jjs的演示](http://www.admin10000.com/UploadFiles/Document/201411/27/20141127081853741240.PNG)
　　它是命令行工具，允许你访问JDK中的Nashorn JavaScript 引擎，完全有能力执行那些甚至最为严格的JavaScript脚本。
### 　　测试
　　对于任何一个项目，你都想运行一些测试。测试对于动态类型的语言尤为重要，最好选择一种测试框架。我推荐[Jasmine](http://jasmine.github.io/)，它是用于测试JavaScript的行为驱动开发框架。
![Jasmine示例代码](http://www.admin10000.com/UploadFiles/Document/201411/27/20141127081853194481.PNG)
　　在Jasmine，你用describe描述测试套件，它阻止了你想测试的代码访问。在测试中的代码完成后，你expect一些结果。
　　很明显这里不是要给出教程，但是我想让你一瞥JavaScript代码看起来是多么地优雅。Jasmine是JavaScript项目最好的实践之一，我们私下在产品开发中应用到了ZeroTurnaround项目，尤其是对于富含JavaScript的不间断运行的交互分析器[XRebel](http://zeroturnaround.com/software/xrebel/)。
### 　　构建工具
　　最后，你的项目将需要的、比较重要的是构建工具。如果你在Java项目中使用JavaScript，请确保你可以避开Java构建工具，这就差不多足够了。但是，对于独立的JavaScript项目，没有必要引入庞然大物—Maven【注3】。
　可以考虑的JavaScript项目用到的构建工具是[GulpJS](http://gulpjs.com/)【注4】。它是基于插件的构建系统，你可以为其指定任务。任务可以是“拷贝src目录下的.js文件到dest”、或“压缩我的JavaScript代码用于生产环境”。让人受到震动的是，GulpJS把任务相关的文件流加入过滤器，因此你可以把上面的两个任务加入一次有效的清扫中。
　　还有大量的可用插件，借助适当的构建系统，你将发现项目中的协作会轻松很多。
### 　　结论
　　我们只是看到了JavaScript的冰山一角，并尽量介绍一些Java开发者在解决JavaScript时应该知道的概念和工具。自然地，这里没有提供要学习的完整的技术清单，但是如果你正准备义无反顾地深入JavaScript项目，这会帮助你起步，拥抱JavaScript的怪癖将有助于你不会频繁地沮丧。
　　你了解让JS开发者走向快乐的秘密或最佳实践吗？毫无疑问应该去分享！在下面评论或在Twitter：[@shelajev](https://twitter.com/shelajev)上与我交谈。我乐于听到你的想法！
- 注1：在计算机科学里，尾调用是指一个函数里的最后一个动作是一个函数调用的情形：即这个调用的返回值直接被当前函数返回的情形。这种情形下称该调用位置为尾位置。若这个函数在尾位置调用本身（或是一个尾调用本身的其他函数等等），则称这种情况为尾递归，是递归的一种特殊情形。尾调用不一定是递归调用，但是尾递归特别有用，也比较容易实现。[http://zh.wikipedia.org/wiki/尾调用](http://zh.wikipedia.org/wiki/%E5%B0%BE%E8%B0%83%E7%94%A8)
- 注2：REPL是一个简单的，交互式的编程环境。这个词常常用于指代一个Lisp的交互式开发环境，但也能指代命令行的模式和例如 APL, BASIC, Clojure, F#, Haskell, J, Julia, Perl, PHP, Prolog, Python, R, Ruby, Scala, Smalltalk, Standard ML, Tcl, Javascript 这样的编程语言所拥有的类似的编程环境。这也被称做交互式顶层构件（interactive toplevel）。[http://zh.wikipedia.org/wiki/%E8%AF%BB%E5%8F%96%EF%B9%A3%E6%B1%82%E5%80%BC%EF%B9%A3%E8%BE%93%E5%87%BA%E5%BE%AA%E7%8E%AF](http://zh.wikipedia.org/wiki/%E8%AF%BB%E5%8F%96%EF%B9%A3%E6%B1%82%E5%80%BC%EF%B9%A3%E8%BE%93%E5%87%BA%E5%BE%AA%E7%8E%AF)
- 注3：Maven 除了以程序构建能力为特色之外，还提供 Ant 所缺少的高级项目管理工具。由于 Maven 的缺省构建规则有较高的可重用性，所以常常用两三行 Maven 构建脚本就可以构建简单的项目，而使用 Ant 则需要十几行。事实上，由于 Maven 的面向项目的方法，许多 Apache Jakarta 项目现在使用 Maven，而且公司项目采用 Maven 的比例在持续增长。[http://www.oschina.net/p/maven](http://www.oschina.net/p/maven)
- 注4：从头编写HTML\CSS\Javascript是上个世纪的事情了，如今的JavaScript都是通过CoffeeScript这样的支持句法缩写的编辑器写成的。如果你希望写完JavaScript能够一个工具完成代码清理优化工作，Gulp 就是你的不二之选，GulpJS类似Ant或Maven之于Java。[http://www.oschina.net/p/gulp](http://www.oschina.net/p/gulp)
　　原文：[javascript-explain-it-like-im-a-java-developer](https://www.voxxed.com/blog/2014/10/javascript-explain-it-like-im-a-java-developer/) 翻译：[labazhou](http://www.labazhou.net/2014/11/javascript-explain-it-like-im-a-java-developer/)
