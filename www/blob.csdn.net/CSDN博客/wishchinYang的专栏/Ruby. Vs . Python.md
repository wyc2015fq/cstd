# Ruby. Vs . Python - wishchinYang的专栏 - CSDN博客
2014年02月13日 20:11:30[wishchin](https://me.csdn.net/wishchin)阅读数：965
前言：从语言的本质上来分析，我对Ruby持反对态度，毕竟语言是为了交流，在表达的效率层面为了正确性必须适当放弃复杂性。且有句老话说的好，Ruby In Rails 才是语言，而Ruby只是这个语言的工具！所以除了Ruby In Rails，还是选择Python。
原文链接：[http://www.zhihu.com/question/19552194](%E7%8E%8B%E9%AA%8D%EF%BC%8CA)
**讨论一：**
Ruby 和 Python 太相似了，取舍大部分都是个人喜好上的原因。比如我就觉得 Python 的 “There is only one way to do it.” 比 Ruby 的 “There are many ways to do it.” 要好，这不光是考虑团队协作的问题，更重要的是自己能很快明白自己三个月前写的没有任何注释的代码是在干什么。当然也有很多人觉得自由和灵活要比可读性来的重要，所以我说这个是个人喜好的原因。
客观上的 Ruby 比 Python 的优势我想到的有这么几个：
Block 应该是语言层面上 Ruby 比 Python 更酷的地方，Python 的匿名函数（lambda function）制约太多，属于鸡肋（BFDL GvR 不认可函数式编程的许多方面）。
OS X 上的话，Ruby 比 Python 最重要的优势可能是 MacRuby ([http://www.macruby.org/](http://www.macruby.org/))
 的存在。我大胆猜测以后 MacRuby 将成为 Objective-C 之外（如果不是完全替代的话）的苹果官方语言。MacRuby 除了能更加方便快捷的创建 Cocoa 应用外，ruby block 使得利用 OS X 的 Grand Central Dispatch 变得容易，在以后的多核、混合核（CPU+GPU）应用上非常重要。
劣势：
Ruby 的硬伤之一在于性能。官方实现 Ruby 1.9 比 CPython 逊色不少。另外 Python 拥有诸如 Numpy, Psyco, PyPy, Cython 等多种性能倍增的途径，相比之下 Ruby 想要提高性能的途径则非常有限。
Ruby 的硬伤之二在于第三方包。Ruby 传入英语世界并大规模流行起来的时间不长，大部分的第三方包都和 Web 相关，Web 领域外的包数量远远不及 Python。特别是在科研等方面的区别非常明显，很多学科领域都有基于 Python 的成熟应用，而几乎找不到 Ruby 的相关包。这除了历史渊源，也和 Python 更加简洁的语法（很多用 Python 做研究的人不是计算机专业的）以及前述的性能倍增器（主要是 Numpy 和 Scipy）有密切的关系。
**讨论二：**
### [Michael wang](http://www.zhihu.com/people/michael-wang-92)，**爱好广泛的码农**
[徐普](http://www.zhihu.com/people/sheep9275)、[Xana
 Hopper](http://www.zhihu.com/people/xanahopper)、[刘缙](http://www.zhihu.com/people/liu-jin-70) 等人赞同
我发现一个有趣的现象，多年经验的c++ 程序员多数都比较喜欢python ;而有多年经验的java程序员多数都比较喜欢ruby,
[2012-11-04](http://www.zhihu.com/question/19552194/answer/15521750)[4
 条评论](http://www.zhihu.com/question/19552194#)[](http://www.zhihu.com/question/19552194#)[](http://www.zhihu.com/question/19552194#)[](http://www.zhihu.com/question/19552194#)[](http://www.zhihu.com/question/19552194#)[](http://www.zhihu.com/question/19552194#)
### [海鸥](http://www.zhihu.com/people/hai-ou)，**业余编程爱好者。**
[秋林](http://www.zhihu.com/people/xcodeman)、[张西家](http://www.zhihu.com/people/zhang-xi-jia)、[陈泓旭](http://www.zhihu.com/people/nonoob)赞同
这两个我都学了，谈谈自己的想法：
Ruby拿过来，你就写吧，每个人每个写法，同一个问题，可以有n多写法，无所谓孰优孰劣，达到目的就可以了。
Ruby最大的优势就是纯粹面向对象，例如对于一个string a，获取长度，在ruby中很干脆，a.length 或者 a.size；在python中，需要len(a)，对于有顽固面向对象思想人来说，python的写法让人受不了。
我个人也是业余爱好，使用的是windows系统，python相对来说对windows更友好，这是我自己实践感觉出来的，django在windows下apache上的配置很简单，ror想在windows上配置能把你折腾到死。
python的强制缩进可以让你养成良好的编程习惯，也便于别人阅读。同时python对于每一个问题，都有一个推荐做法，相对来说，一个人写的程序，另外一个人可以轻易读懂。
python有自己的哲学，python中文用户组的“大道至简”也是我个人比较喜欢的哲学。不要把简单的问题复杂化，“如无必要，勿增实体”。
最后，两者其实都是脚本语言，差不多，学会了一种，另外一种也很容易就学会了。 
[2011-05-23](http://www.zhihu.com/question/19552194/answer/12572066)[2
 条评论](http://www.zhihu.com/question/19552194#)[感谢](http://www.zhihu.com/question/19552194#)[分享](http://www.zhihu.com/question/19552194#)[收藏](http://www.zhihu.com/question/19552194#)•[没有帮助](http://www.zhihu.com/question/19552194#)•[举报](http://www.zhihu.com/question/19552194#)
### [李路](http://www.zhihu.com/people/li-lu-69)，**[http://knewone.com](http://knewone.com)**
[王超](http://www.zhihu.com/people/hit9)、[杨三金](http://www.zhihu.com/people/yang-san-jin)、[王赟](http://www.zhihu.com/people/wang-yun-38)
 等人赞同
我个人的选择是是Ruby和Lisp
不要小看编程语言的能力和差异，ruby和python是完全不一样的语言，就像python和perl，perl和php，只有实际写过大量代码做过设计构架的人，才知道一个更优美更适合**建筑师**（而非工程师）的编程语言能带给自己什么。
**讨论三：**
王骍，A Programmer 山海雍和、小天狼星、刘永辉 等人赞同
刚在G+上写了篇帖子，贴一下。有些观点和前面的观点有所重复，请见谅。
正文如下：
本周的Pycoder's Weekly不错，第一个Topic是：
“What are the advantages of Python over Ruby?”
这是Quora上的一个问答，网址在：
[http://www.quora.com/What-are-the-advantages-of-Python-over-Ruby](http://www.quora.com/What-are-the-advantages-of-Python-over-Ruby)
Python和Ruby阵营之间似乎一直在争论谁更好（貌似Ruby阵营的人们更狂热些），不信可以试试google一下“Python Ruby”。上面这篇文章我目前还没看，在看之前先说说我的个人看法：
Python优于Ruby的地方：
a) Python开发人员的背景丰富多彩，阵营规模很大，这直接导致了Python的各种lib比Ruby多很多很多很多
b) 和Ruby相反，Python的哲学是“做一件事情有且只有一种方法”（There should be one-- and preferably only one --obvious way to do it.）。Ruby的信徒会觉得这样非常boring，但是我觉得这一点减轻了人们在开发时的认知负担和选择成本，对于提高开发效率是很有帮助的。同时，这个特点使得不同的人用Python写出来的代码不至于相差很大，这对于团队合作也是很有用的。（这点也导致了Python中是有公认的Best Practice的，但是Ruby中就很难找到一个公认的Best
 Practice）
c) Python在面向对象上做的并不好，其实是把面向过程和面向对象杂糅起来了。（随着Python 3的发展相信这点会有所改观）我个人虽然感觉这样不太好，但是实际用的时候就会发现，不强调面向对象其实能够减轻程序员的认知负担，写起代码来会比较随心所欲，对于c程序员来说也更容易接受。（好吧，如果你觉得这点是Python的缺点，那么其实你也是对的~）
d) Python开始慢慢走进大学一年纪的教科书，可以预见，未来的Python开发者阵营会继续不断壮大。（当然Ruby的阵营也会越来越大）
e) Python解释器，特别是CPython，目前比现有的Ruby解释器快很多。这点Ruby的发明人Matz也是很想再重新写个Ruby的虚拟机，但是苦于要做的事情很多，不知道什么时候能完成。Ruby的运行效率目前来看实在太慢了些。（当然程序运行效率和代码是如何写的也有很大关系，想让Ruby程序尽可能快的话，除了一般的程序优化方法外，还要注意要多用built-in的东西，因为它们是用c写的，运行快）
f) Python的语法比较简单，容易上手。
说了这么多Python优于Ruby的地方，现在再来说说我觉得Ruby优于Python的地方：
a) 第一个印入我的脑海的优点是：有趣！当你开始慢慢学会使用Ruby后，你就会觉得Python的代码真TM是死板的不能再死板了。这是因为Ruby从Perl中继承的一条哲学是和Python完全相反的：“每一件事情总是有多种做法”（Having more than one way to do the same thing）。所以程序员的创造力就完全被激发出来了。当你看到某个问题的一种前所未见又十分优雅的解决方案时，你会有一种混杂着强烈喜悦的惊讶的——写（优美的）Ruby能够激发人的灵感。
b) Matz在设计Ruby的时候就想方设法让Ruby成为一个“人性化”的语言，力求使Ruby程序员感觉到使用Ruby是一件愉悦的事情。你能从Mixin、程序块、元编程、极具包容性的语法等等当中获得一种喜悦。
c) 纯粹的面向对象，极其强大的元编程功能。Ruby是纯粹的面向对象。CSDN的范凯曾经说过：“你学习了Ruby以后，你就会发现，原来Java/C++所谓的面向对象就是TMD的山寨版本的面向对象，原来面向对象还可以这样玩啊”（[http://robbin.iteye.com/blog/969154](http://robbin.iteye.com/blog/969154)）。Ruby的元编程能力还有宽松的语法使得使用Ruby可以很轻松的开发DSL（Domain
 Specific Language），这一点Python是搞不定的。
d) Ruby on Rails！要不是DHH开发出了RoR不知道现在还会有多少人在用Ruby。毋庸置疑，RoR是快速Web开发的神器。为什么RoR那么牛？有一个很重要的原因就是RoR将Ruby的元编程能力发挥到了极致，“甚至到了一种变态的程度”（Matz语，见《松本行弘的程序世界》[http://book.douban.com/subject/6756090/](http://book.douban.com/subject/6756090/)
 P.S.想要了解Ruby设计思想的话此书一定要读一读，起码是部分章节）
e) Ruby吸收了很多语言（Lisp、Smalltalk等等）的特性，对于程序员来说了解一下Ruby对于开阔思路是很有帮助的，它能帮助你更好的看清你之前一直使用的语言的限制之处在哪。
暂时就想到这么多，应该还有很多补充的空间。我使用Python和Ruby的时间也很短，所以还是多多参考些Python和Ruby双料大牛们的意见吧~
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~待续！！！
