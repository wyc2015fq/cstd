# Python 10大谬论 - 算法与数学之美 - CSDN博客
2019年03月24日 20:30:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：2
## **谬误 #1: Python 是一门新语言**
伴随着所有的初创公司正在使用它以及孩子们最近也在学习它的事实，这个谬误为何仍然存在是可以理解的。实际上 Python 已经 超过23岁了, 它最初发布于1991年, 早于 HTTP 1.0协议 5年且早于 Java 4年. 目前比较有著名的很早就使用 Python 的例子是在1996年: Google 的第一个成功的网络爬虫.
如果你对于长长的 Python 历史比较好奇，Python 的作者 Guido van Rossum 已经为你准备好整个故事了.
## **谬误 #2: Python 没有被编译**
不像 C++ 一样需要一个独立的编译器工具链，Python 实际上被编译成了字节码，和 Java 或者许多其他的编译型语言十分相似。更进一步的编译过程，如果有的话, 取决于运行时环境, 不管是 CPython，PyPy，Jython/JVM，IronPython/CLR，或是其它的进程式虚拟机（process virtual machine）。参考 谬误 #6 来了解更多。
一条在 PayPal 以及其它地方的通用原则就是，（应用的）安全性不能依赖于代码的已编译状态。更为重要的是加强运行时环境的安全，因为实质上每种语言都有一个解码器，或者能被拦截并导出受保护的状态。参考下一条谬误来了解更多的 Python 安全性问题。
## **谬误 #3: Python 不安全**
轻量级 Python 的亲和力可能使他看起来不怎么可怕，但是这里直觉很大程度上是受到了误导的. 安全的一个核心原则就是尽可能让呈现的目标更小. 大系统是违背安全原则的，因为他们趋向于 使行为过渡集中化, 并且也 让开发者难于理解. Python 通过倡导简洁化来边缘化这些恶心的问题. 更有甚者, CPython 通过让自己成为一个简单、稳定并且易于审核的虚拟机来使这些问题得到解决. 事实上，近期 Coverity Software 的一个分析结果显示 CPython 得到了他们的最高质量评级.
Python 还拥有一系列可扩展的开源、产业标准化的安全库序列. 在PayPal, 我们把安全和授信看做是重中之重, 我们发现 hashlib, PyCrypto, 以及 OpenSSL, 通过 PyOpenSSL 和我们自己的定制构建的结合，涵盖了 PayPal 多样化的安全和性能需求.
这些诸多的原因，使得 Python 成为PayPal（和eBay）的应用程序安全团队在某些业务中最快的选择. 这里有把Python用在PayPal的安全第一环境中的几个以安全为基础应用程序:
- 
创建安全代理，以促进密钥的轮换以并巩固加密实现
- 
同业界领先的 HSM 技术集成
- 
为缺乏兼容性的技术栈构建受TLS保护的封装代理
- 
为我们内部的互相认证计划生成键和证书
- 
开发主动的漏洞扫描器
另外，还有无数存在安全隐患的用Python构建，面向操作的系统, 诸如防火墙和连接管理. 未来，我们一定回去深入的整合PayPal Python的安全事项.
## **谬误 #4: Python 是一门脚本语言**
Python 确实可以用来编写脚本，并且因其简单的语法、跨平台并且无所不在于 Linux, Macs, 和其它Unix 机器而成为这个领域的领跑者之一.
事实上, Python 可能是常规用途编程语言中最灵活的技术. 以下是一些实例:
- 
电信基础设施 (Twilio)
- 
支付系统 (PayPal, Balanced Payments)
- 
神经科学和心理学 (许多, 许多, 例子)
- 
数值分析和工程 (numpy, numba, 以及 更多其它)
- 
动画(LucasArts, Disney, Dreamworks)
- 
游戏后台 (Eve Online, Second Life, Battlefield, 以及 其它很多)
- 
Email 基础设施 (Mailman, Mailgun)
- 
媒体存储和处理 (YouTube, Instagram, Dropbox)
- 
操作和系统管理 (Rackspace, OpenStack)
- 
自然语言处理(NLTK)
- 
机器学习和计算机版本 (scikit-learn, Orange, SimpleCV)
- 
安全性和渗透性测试 (很多很多 以及 eBay/PayPal
- 
大数据 (Disco, Hadoop support)
- 
如理 (Calendar Server, 它 驱动了 Apple iCal)
- 
搜索系统 (ITA, Ultraseek, 还有 Google)
- 
Internet 基础设施 (DNS) (BIND 10)
更别提网站和web服务了，那些都不在少数. 事实上，PayPal工程师看起来像是有兴趣致力于基于Python的web特性,比如 YouTube 和 Yelp. 如果对Python成功案例的更大清单感兴趣，那就看看官方的清单吧.
## **谬误 #5: Python 是弱类型的**
Python 类型系统的特点是拥有强大、灵活的类型操作. 维基百科上对此作出的阐述.
而存在一个不争而有趣的事实是， Python 是比Java更加强类型的. Java 对于原生类型和对象区分了类型系统，它让null存在于一个灰色地带. 另一方面，现代的 Python 拥有一个统一的强类型系统, 其中什么都没有（None） 的类型是明确指定的. 更进一步的，JVM自身也是动态类型的，因为可以把它的 根源 追溯到由Sun所收购的Smalltalk VM的一个实现.
Python的类型系统 很棒，但要提供给企业级使用，目前仍然还有许多更重大的事项需要关注.
## **谬误 #6: Python 速度慢**
首先是有一个重要区别: Python 是一门编程语言，而不是运行时环境. Python 拥有几个实现:
- 
CPython是参考实现, 且也是广泛发布和使用的实现.
- 
Jython是Python用于JVM的是一个成熟的实现.
- 
IronPython是 Microsoft 针对其自家的通用语言运行时——又名 .NET，实现的Python .
- 
PyPy是一个正在日趋成熟的Python实现，拥有JIT编译，增量垃圾收集诸多先进的特性.
每一个运行时都有其自己的性能特点, 而且他们本身也不慢. 这里更重要的地方在于不能错误地把一个性能指标分派到一门编程语言智商. 应该总是把该评估用在一个应用程序运行时上面，最好是针对一个特定的使用场景.
清楚了那些事项之后，下面就是一些有Python提供的小项，体现其重要的性能优势:
- 
把 NumPy 用作 Intel 的 MKL SIMD接口
- 
PyPy的 JIT 编译能 达到比C还快的性能
- 
Disqus 能在同样的100个盒子上容纳两亿五千万到5亿用户
诚然，这些都不是最新的列子，只是我个人的最爱罢了. 这将很容易扯到高性能Python以及独立提供的运行时这些广阔的领域. 我们不应只是专注于解决单个特殊的案例, 而是应该把注意力放在对开发人员在 最终产品性能 方面的生产力的普遍影响上面, 特别是在一种企业级环境之下.
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bLmy0N4HIafliafibZCeicBD9hR7pAib6ib4icHSiakHPp610TswWjA4Csdq6H4iacdxhSKOCsjKUd2r9RibySQIJRmwhcA/640?wx_fmt=jpeg)
C++ vs Python,. 两种语言在同一个输出下的对比.
给定足够的时间，一个循规蹈矩的开发者只会按照下面这种经过论证的方式来编写精确高效的软件：
- 
设计实现一个可以正确完成任务的软件，包括开发单独的测试
- 
测试性能，明确瓶颈
- 
优化，根据测试和Amdahl法则，并且利用Python与C的渊源
虽然这听起来很简单，但是即使是老道的工程师，这依旧是一个非常耗时的过程。Python设计之初就考虑到了这一套开发流程。根据我们的经验，通常C++和Java项目完成一次迭代流程的时间，够Python项目完成三次迭代流程。今天，PayPal和eBay中不乏有Python项目使用更少的代码战胜了同类C++和Java项目，这多亏了快速的开发使得仔细的裁剪和优化变得可能。
## **Myth #7: Python无法做到大规模**
大规模有许多定义，但无论怎样，YouTube是个大规模网站。每月UV超过十亿，每分钟上传的视频时长超过100小时，占用互联网带宽的20%，所有这一切都以Python作为核心技术。Dropbox,Disqus, Eventbrite, Reddit, Twilio, Instagram, Yelp, EVE Online, Second Life,，以及，是的，以及eBay和PayPal中都有Python大规模的例子，这些证明大规模不仅仅是可能：它是一种模式。
成功的关是键简单性且一致性。CPython，Python的主要虚拟机，其最大限度地放大了这些特性，从而演变出了一个精确可测的运行时。人们很难发现 Python程序员关心垃圾的收集暂停或应用地启动时间。拥有强大的平台和网络支持，Python其本身自然而然的智能水平可扩展，BitTorrent就是其充分的体现。
此外，规模化主要涵盖测量和迭代。Python是以分析和优化为要义建立的。看Myth #6了解更多Python如何垂直拓展的细节。
## **Myth #8: Python缺少好的并发支持**
除了偶尔叫嚣性能和规模化的问题，有人想提的技术些，”Python缺乏并发，”或者，”GIL怎么样？”如果几十个反例仍不足以支持Python水平及垂直拓展规模的能力，那么再更深地解释CPython实现细节也不会有帮助，所以我会简短些。
Python拥有强大的并发原语，包括generators, greenlets, Deferreds, 和futures.。Python有优秀的并发框架，包括eventlet, gevent,和Twisted。Python在定制运行时尚投入了惊人的工作量，包括Stackless和 PyPy。所有烦人这些和更多表明，根本不存工程师们在Python并发编程方面的缺憾。同时，所有这些都正在被正式的在企业生产环境中支持或使用。例如，请参考Myth #7。
全局解释器锁，或称GIL，是Python在大多数应用场景下的性能优化，也是几乎所有CPython实现代码的开发上的基础优化。GIL使得Python可以很便利地使用操作系统的线程或轻线程（通常指greenlets），且不影响使用多进程。更多相关信息，请看该主题的Q&A列表，以及Python文档中的介绍。
在PayPal中，一个典型服务的部署需要多台机器，多个进程，多个线程，以及一个数字非常庞大的greenlets，相当于一个非常强大可扩展的并行环境（见下图）。在大多数的企业环境中，团队更倾向于往更高层次过度，谨慎并注重灾难恢复。然而，在某些情况下，每台机器每天Python服务仍然处理数以百万计的请求，而且轻松处理。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/bLmy0N4HIafliafibZCeicBD9hR7pAib6ib4icFGDXAZtKXvqs8llgjWjicz3zaSV5NNsbO3bDwCCeyuAthd5ANaKhkqA/640?wx_fmt=jpeg)
一个基于单一worker的协同异步架构草图。最外层的盒子是进程，下一个层次为线程，这里这些线程都是轻线程。操作系统处理线程间的抢占，而I/O异步协同合作。
## **谬误 #9: Python 程序员很稀缺**
事实上，现在使用 Python 的 web 开发者的确没有使用 PHP 或者 Java 的 web 开发者多。这可能主要是由于企业需求和教育之间的相互作用导致的，不过 教育领域（教学所使用的编程语言）的趋势使得情况可能产生变化 。
也就是说，使用 Python 的开发者并不稀缺。现在全世界有数百万使用 Python 的开发者。已有几十个Python 技术大会、 StackOverflow 上成千上万的 Python 内容问答、雇佣大量使用 Python 的开发者的大企业比如 YouTube 、 美国银行（ Bank of American ）和 LucasArts/Dreamworks 等等，这些都显而易见地证实了这一点。在 eBay 和 PayPal 我们一直保持拥有几百位使用 Python 的正式开发者，这是怎么做到的呢？
那么，当一个项目被创建时为什么它会被首推？对于孩子来，大学生和教授们来说，Python作为第一门程序设计语言是非常易于学习的。在eBay，仅仅需要一个星期，一个新的Python程序员就能展示一个真正的成果，并且他们开始散发光芒常常只要2-3个月，通过Internet的宝藏（互动式教程，书，文档和开源代码库）一切皆有可能。
另外一个重要的考虑因素是，项目使用Python会更简单，它不会像其他项目那样需要那么多的开发者。在谬误6和谬误9中提到的那样，在Python项目中，学习像Instagram那样的高效团队是一个常见的比喻，并且这确实是我们在eBay和PayPal的经验。
## **Myth #10: Python不适应于大项目**
Myth #7 讨论了大规模运行Python的项目，但开发Python大规模项目是什么情况呢？正如在Myth #9中提到的，大多数Python不被人看好。 然而Instagram在其被亿元美金收购当天达到千万的点击量，而整个公司只有十几个人。Dropbox在2011年只有70个工程师，其他团队更少。所以，Python适合大规模团队吗？
美国银行实际上有超过5000的Python开发者，一个单独的项目超过一千万行Python代码。JP摩根也经历了类似的转变。YouTube也有数千的开发者和数百万行的代码。大规模产品和团队每天都在使用Python，因为它具有良好的模块化和封装特性，在特定方面许多的大规模开发建议是一致的。工具，强大的惯例以及代码审查促使了项目规模化管理的现实。
幸运的是，Python发展于上面所提到的好的奠基。我们在检查执行使用pyflakes以及其他工具进行Python代码的静态分析，正如坚持PEP8——Python语言的基础风格指南。
最后，应该指出的是，除了调度加速Myth #6以及#7中所提到的，使用Python的项目通常需要更少的开发者。我们常见的成功案例中，使用Java或C++的项目通常有3-5开发者耗时2-6个月，最终由单一的开发者在2-6周（或小时，因为这些原因）完成项目。
有点像奇迹，但却是现代发展的事实，但其往往出自一个竞争激烈的行业。
## **一种干净的状态**
这些谬误可能只是消遣。讨论这些谬误仍然很活跃的和受启发的，包括内部和外部， 因为隐含在每一个谬误里的都是一个Python的优势的认识。 而且， 记住这些看似乏味的表现和麻烦的问题是稳步成长的表露， 并稳定地增长兴趣促进教育和持续地工作。 在这里，希望能扑灭一场充满火焰的战争，并且使得能真正去谈论工作与Python的实现。
**————**
编辑 ∑ Pluto
来源：数据与算法之美
微信公众号“算法数学之美”，由算法与数学之美团队打造的另一个公众号，欢迎大家扫码关注！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyjQVC3BFiaFKQ4DJqz2xhrwkzeCPbjQdnnG8678fRf1sxc2ZQtvtVib2dqWUkeopYtmgckINoOoGoQ/640?wx_fmt=jpeg)
**更多精彩：**
☞[如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=1&sn=e5e008fb68a7d837546d0ac5b5438042&chksm=ebe9cbf3dc9e42e5d625b2da6b9b3866dff9f08d442d8106f4cbf035d8602e1fdda86eec6476&scene=21#wechat_redirect)
☞[自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483726&idx=2&sn=18272b7eaa172794b51c30d0a2dd9c48&chksm=ebe9cbf3dc9e42e5ddf9a189822a2fa099543a631ad63a1d6ed0158b51c76212eb65ebbfe71b&scene=21#wechat_redirect)
[☞费马大定理，集惊险与武侠于一体](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=1&sn=9069dadf4bbce2aa34bd64b85a69dcee&chksm=ebe9cbfedc9e42e81c27d72da15c0dbf848e505946f231051b8b4033d0941bc6f51cef32790e&scene=21#wechat_redirect)
☞[简单的解释，让你秒懂“最优化” 问题](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483715&idx=2&sn=55a16f37c89b27994b263e0dc9837561&chksm=ebe9cbfedc9e42e842deb581ea62b750cedd839abd58c2db3261bf9fbcd172a2cf18512e4d2d&scene=21#wechat_redirect)
[☞一分钟看懂一维空间到十维空间](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483676&idx=1&sn=2366a39bca3ef42a6e868e91ea718813&chksm=ebe9cba1dc9e42b70c7e147b9e43828c1a7c68401f442890471a06e5cf0704437f9813ca0e0f&scene=21#wechat_redirect)
☞ [本科、硕士和博士到底有什么区别？](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483661&idx=1&sn=d822666a054ba70b37dfb06d14c60f3a&chksm=ebe9cbb0dc9e42a6c476f7f81095b772aa45d960bf516f60c5b2e1155c9093696222cea0a83d&scene=21#wechat_redirect)
☞[小波变换通俗解释](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=1&sn=7d0d05c78cd01df91495f1d14609cbce&chksm=ebe9cbbbdc9e42add13cfe99f3383745fa5c059df705a3a9e28644d073dff804569af94970e3&scene=21#wechat_redirect)
☞[微积分必背公式](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=4&sn=ce88086b650c601bdbf57ecfe5a490a1&chksm=ebe9cbbbdc9e42adfaf0e4ee644d254835c830ef47663315b70a39a2b47e6a7cf10d0826b88d&scene=21#wechat_redirect)
☞[影响计算机算法世界的十位大师](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=3&sn=ceaec6043bb0e8a851033482f8f572bf&chksm=ebe9cbbbdc9e42ad30fd38383cf1caa609ac6e81964da17277f8e2a7f17a933cd11e0f3840c8&scene=21#wechat_redirect)
☞[数据挖掘之七种常用的方法](http://mp.weixin.qq.com/s?__biz=MzI4NTY3OTU3MA==&mid=2247483654&idx=2&sn=bf439d56bc7d42083708fa76434a6025&chksm=ebe9cbbbdc9e42ad9e5f2b3c1952e620e0e3d4452aae25b611e7e54be8678b0d80e002e7be6d&scene=21#wechat_redirect)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
