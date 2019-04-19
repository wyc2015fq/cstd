# HIT CS科班对计算机专业knowledge的compilation - SHINING的博客 - CSDN博客
2014年04月18日 15:57:30[snsn1984](https://me.csdn.net/snsn1984)阅读数：3804
【题记】Great minds discuss ideas. Average minds discuss events. Small minds discuss people.
【前面的话】本文首发于[某留学论坛](http://www.1point3acres.com/bbs/thread-26964-1-1.html)，并可以随意转载。有任何建议、意见，都可以给我发邮件，hitzzy at gmail dot com。
作为计算机科班、系统方向，谈谈我对计算机专业素养的理解，希望对大家有所帮助。
所谓，知之者不如好之者，好之者不如乐之者，计算机专业也不例外。虽然本科是被选到计算机专业，但大一因为[C语言老师](http://sunner.cn/)的激励而喜欢这个专业，并在大三偶然参加[国防科大“微处理器设计”研究生暑期学校](http://www.nudt.edu.cn/summerschool2009/)时，知道了超级计算机之父Cray及其借用1024只鸡与牛来比喻性能差异的名言，见识了龙芯实现的经验，而决定做系统领域研究。兴趣入门，比如《计算机文化》（第10版？）《编码的奥秘》（我觉得旧版比新版的小开本更好）《大话处理器》，[the
 Information Science Hall of Fame](http://www.sis.pitt.edu/~mbsclass/hall_of_fame/list.html)，以及传记类的《ACM图灵奖演讲集:前20年 (1966-1985)》《ACM图灵奖 (1966-2006):计算机发展史的缩影》（[ACM网站的演讲原稿](http://amturing.acm.org/lectures.cfm)）《IEEE计算机先驱奖:1980-2006》。更专业一些，推荐UT
 Austin [Prof. Patt](http://users.ece.utexas.edu/~patt/)的《计算机系统概论》(Introduction to Computing Systems: From Bits and Gates to C and Beyond)，CMU
[Prof. Bryant](http://www.cs.cmu.edu/~bryant/)合著的《深入理解计算机系统》（[Computer
 Systems: A Programmer's Perspective](http://csapp.cs.cmu.edu/)）。CMU流传着这个段子：15-213，给CMU门牌的课程。这里的15-213，既是以CSAPP为课本的Introduction to Computer System课程编号，也是匹村的邮编。信不信由你，我也是因为这本书，了解[系统各部分延迟](http://www.eecs.berkeley.edu/~rcs/research/interactive_latency.html)并喜欢上系统这个方向的！
在谈具体课程前，插播三类文章，以培养专业（职业？）精神。
a) 专业科普
Utah AP的[What every computer science major should know](http://matt.might.net/articles/what-cs-majors-should-know/)、[我上届创业的师兄](http://www.zhiyan.info/)编辑的《[计算机新人入门任务](http://wiki.pureweber.com/index.php/Develop/%E6%96%B0%E4%BA%BA%E5%85%A5%E9%97%A8%E4%BB%BB%E5%8A%A1%E5%B7%A5%E5%85%B7%E4%B8%8E%E5%B9%B3%E5%8F%B0)》，Eric
 Raymond的[How To Become A Hacker](http://www.catb.org/esr/faqs/hacker-howto.html)、William Long的《[写给新手程序员的一封信](http://www.williamlong.info/archives/2700.html)》，《[给未来的你——李开复在2011级大学新生学习规划讲座上的演讲](http://blog.sina.com.cn/s/blog_475b3d560102dt81.html)》。[Linux:
 What are some time-saving tips that every Linux user should know?](https://www.quora.com/Linux/What-are-some-time-saving-tips-that-every-Linux-user-should-know)。创业师兄写的《[程序员的战斗力](http://www.fancycedar.info/2013/06/programmer/)》。Peter Norvig's
[Teach Yourself Programming in Ten Years](http://norvig.com/21-days.html)。[杂类汇编](http://doc.cat-v.org/sitemap)。现在M$当QA的本科校友的[转CS建议](http://www.1point3acres.com/bbs/thread-24540-1-1.html)和[面经](http://www.1point3acres.com/bbs/thread-52028-1-1.html)。《[资深软件工程师的特点](http://dandreamsofcoding.com/2013/04/08/how-senior-software-engineers-are-different/)》。工欲善其事，必先利其器——[程序员的文本编辑器简介](http://heather.cs.ucdavis.edu/~matloff/ProgEdit/ProgEdit.html)；基于HTML5的VIM寓教于乐在线游戏[Vim
 Adventures](http://vim-adventures.com/)。[IEEE Computer Society](http://www.computer.org/portal/web/tandc/tclist)。NSF旗下课程网站TRUST Academy Online ([TAO](https://tao.truststc.org/))。
b) 科研素养
前哈佛教授、现Google SE的[Getting started as a PhD student](http://matt-welsh.blogspot.com/2010/09/getting-started-as-phd-student.html)，基于指导MIT和Berkerly学生总结而成的[Advice
 for students starting into research work](http://www.seas.upenn.edu/~andre/general/student_research_advice.html)，CMU教授的[On the Ph.D.](http://da-data.blogspot.com/2013/01/on-phd.html)，Wisc教授的[Advice](http://pages.cs.wisc.edu/~gleicher/Web/Advice/Advice)，Utah
 AP的[10 easy ways to fail a Ph.D.](http://matt.might.net/articles/ways-to-fail-a-phd/)和[Reading
 for graduate students](http://matt.might.net/articles/books-papers-materials-for-graduate-students/)，HIT校友、Wisc AP的[外链](http://xyzhang.ece.wisc.edu/links.html)。Princeton[李凯教授](http://blog.sciencenet.cn/blog-414166-542685.html)关于[职业生涯规划的报告](http://blog.sciencenet.cn/blog-414166-321619.html)。UT-Dallas
 AP关于科研方法和安全领域的[资料汇总](http://www.utdallas.edu/~zxl111930/resource.html)。UW教授对Paper Award的[观点](http://programanalysis.blogspot.com/2013/02/acm-sigsoft-impact-paper-award.html)。[The
 Ph.D. grind](http://blog.sciencenet.cn/blog-493885-589542.html)也非常[有参考价值](http://blog.regehr.org/archives/743)。工大博士师兄的《[读书的习惯需要好好培养](http://billlangjun.blogspot.com/2012/09/blog-post.html)》。[Active
 Learning](http://cgi.stanford.edu/~dept-ctl/cgi-bin/tomprof/posting.php?ID=1232)。[Intensive Study Abroad for First-Generation College Students](http://cgi.stanford.edu/~dept-ctl/cgi-bin/tomprof/posting.php?ID=1227)。来自NUS的CS[会议](http://www.comp.nus.edu.sg/~wang06/SoC%20Conference%20Ranking.htm)和[期刊](http://www.comp.nus.edu.sg/~wang06/SoC%20Journal%20Ranking.htm)排行。[Google's
 Hybrid Approach to Research](http://norvig.com/cacm-hybrid.html).《[梁漱溟：做学问的八个境界](http://www.ruanyifeng.com/blog/2013/09/liang_shuming.html)》。[Best
 CS papers](http://jeffhuang.com/best_paper_awards.html)。
c) 时间规划
[李笑来老师](http://lixiaolai.com/)的《[把时间当作朋友](http://code.google.com/p/befriend-with-time/)》，[MSRA员工](http://www.weibo.com/pongba)的《[暗时间](http://mindhacks.cn/2009/12/20/dark-time/)》，Utah
 AP的[Tips for work-lif balance](http://matt.might.net/articles/work-life-balance/)，Time Planning by
[Establishing your Absence](http://chronicle.com/article/Establishing-Your-Absence/45649/) and
[Multitasking](http://cgi.stanford.edu/~dept-ctl/cgi-bin/tomprof/posting.php?ID=1234)，Prof. Tao Xie's
[checklist](http://asegrp.blogspot.com/2010/11/time-management-self-checklist.html)，工大博士师兄的[页面](http://www1.i2r.a-star.edu.sg/~jlang/timemanagement.html)和[博文](http://billlangjun.blogspot.com/2012/10/blog-post_2.html)，[GTD](http://blog.samaltman.com/how-things-get-done)
 from a startup's perspective，我上届百度师兄的《[我是如何“管理时间”的](http://cnberg.com/archive/how-i-manage-my-time/)》，Fenng在微信“[小道消息](http://hutu.me/)”发布的《碎片化时间与我的阅读习惯》《我阅读技术图书的一些习惯》。《[如何提高开会的效益](http://blog.sciencenet.cn/blog-393255-641222.html)》。Habits
 for [college students](http://matt.might.net/articles/college-tips/) and
[grads](http://matt.might.net/articles/grad-student-resolutions/), by Matt.
1）Programming Language
入门当然是[C](http://www.hrbxinzhi.com/pro-knowledge.html)，否则都不好意思说自己是学cs。我当年用的是C Prime Plus，很基础。C之父写的C bible——The C Programming Language绝对是好书，是进阶和reference必备。C进阶还有各种C专家编程，C与指针，C陷阱了，可以提高。之后是C++，java，C#就看科研/项目需要了。我认识的一些MSRA实习生从C++转到C#（C++++），说什么编码效率高云云。反正我因为做llvm，就是C++一条道走到黑。（LLVM
 is to me as Allegro is to [Cloud Wu](http://codingnow.com/)）人称“大牛”的师兄跟我说，按照某语言排名榜的说法 c++快挂了；anyway，大牛没给出出处。而在TAMU任教的C++之父还在弄STAPL，并行版的[STL](http://blog.sina.com.cn/s/blog_687d9aa90100wjsq.html)，我觉得C++肯定还不至于是五丈原的诸葛半仙。C++入门，我用的是C++
 Prime（注意，不是C++ Prime Plus）。C++之父写的C++程序设计语言，其实比较难懂，入门之后可以翻一翻。C++的进阶书更多，各种Efficient C++，侯捷老师的STL源码剖析啊。boost是C++广泛应用的开源库。[C++11](http://www.stroustrup.com/C++11FAQ.html)也很[有趣](http://mindhacks.cn/2012/08/27/modern-cpp-practices/)。（不好意思，我也只是知道，没怎么看。）而Java是不适合作为第一门语言的；CMU,
 "the home of object-oriented modular programming" by David Parnas，[2010年3月末取消了大一的OOP，转而开设OOD](http://www.i-programmer.info/news/150-training-a-education/2188-cmu-drops-oop.html)，因为前者反模块化反并行化。当然，这点还在广泛争论中。
脚本语言，顾名思义，用于完成冗余繁琐的事情。把这些事情交给计算机，定时跑就行，什么[python](http://www.pgbovine.net/python-teaching.htm), shell, perl, awk, sed, ruby, ror等等。选一个顺手即可。记得豆瓣还是哪里的程序员自述，计算机科班教育不会开脚本语言课，但谁真正掌握了一门脚本语言，说明他是爱计算机的，所以我们会招聘他。（说到job-hiring，相信很多人看过《[The
 Social Network](http://movie.douban.com/subject/3205624/)》。Zack招来的人，和那个硅谷小子的，风格完全不一样；而前者才适合一起创业。）书籍都不一一举例了，似乎是个脚本语言，O'Reilly都有从入门到精通的书。最后，要精通正则表达式哦。
[浪漫](http://matt.might.net/articles/i-love-you-in-racket/)的[函数式语言](http://www.infoq.com/interviews/john-hughes-fp)，比如[Haskell](http://www.haskell.org/haskellwiki/Haskell)，lisp，[ML](http://www.3nfconsulting.com/students-ML.aspx)等等，是另一种思维方式。有一个笑话，前苏联克克勃苦心偷到了美国NASA的核心代码，交给上峰后被训了一顿；他很莫名其妙，瞅了一眼代码，全是（）的组合，当场崩溃。后面介绍Compiler时也会涉及FP。事实上，我觉得，这才是计算机学生应该掌握的语言，MIT大二就开了Scheme的[课程](http://mitpress.mit.edu/sicp/)（但现在似乎[停了](http://blog.willbenton.com/2009/05/computer-science-and-plumbing/)）。法国“中科院”INRIA设计了[Coq](http://coq.inria.fr/)，用于各种形式化证明（比如，编译器生成代码的正确性验证），以及[教授程序原理](http://blog.willbenton.com/2008/05/shortening-the-feedback-loop/)。首届国家科学进步奖的两位获得者有吴文俊，主要贡献就是机器证明，而另一位是袁隆平，解决咱们的口粮。可见这一领域有多么重要！
最后，我很纳闷，居然有人推荐HTML/CSS/JS作为语言的入门？亲，这说的是计算机科学学生的第一门专业课。我讨论的是计算机科学，是科学，不是工程；是本科，不是大专技校。学C，是为了后面的OS——linux。学web，你跟得上市场潮流吗？HTML/CSS/JS，你咋不HTML5、node.js、Go、以及Scala呢。看到这个[benchmarking](http://www.techempower.com/benchmarks/)，我都眼花缭乱啦。不好意思，本人[很惭愧](http://wordpress.lixiaolai.com/archives/1455.html)，不太懂[前端编程](http://www.lxkid.com/?p=154)，也感叹前端的[技术细节](http://bjk5.com/post/44698559168/breaking-down-amazons-mega-dropdown)；但推荐各位想做web的看看HTML5的发布会视频，演讲者一上来给出各种设计哲学，包括了The
 Declaration Of Independence、Karl Marx的按需分配、George Orwell的Animal Farm，最后才给出了HTML5的。这才是精髓，或是说web设计的精髓。看到自己做的webpage很有成就感，修改linux内核代码的成就感怎样？先抑后扬的感觉不是更爽！本科学习的原则，在我看来，是不要被外界所打动，夯实基础，做自己有兴趣的事情。举个俗透了的例子，如乔布斯学书法，去印度礼佛！
[编程语言语法大全](http://rigaux.org/language-study/syntax-across-languages-per-language/)
2）Data Structure & Algorithm
很多人推荐CLRS，我也不能免俗，但毕竟不是入门级别的。其实，学DS掌握基本概念就行，推荐清华大学严蔚敏老师的《数据结构》，网上也有视频。同时推荐MIT的[Advanced Data Structures](http://courses.csail.mit.edu/6.851/)和Wisc的Introduction
 to Algorithms（[普通版](http://pages.cs.wisc.edu/~dieter/Courses/2012f-CS577/)和[Honor版](http://pages.cs.wisc.edu/~dieter/Courses/2013f-CS577/)）。之后也正如大家所言，ACM吧，不是为了WF，只为了不惧怕编程，[体验成就感](http://blog.renren.com/blog/60525895/852730269)（再回到PL，web语言可以acm?）。一定题量后，需要[加强算法](http://blog.csdn.net/v_july_v/article/details/6543438)，推荐Robert
 Sedgewick的算法书，[the Algorithm Design Manual](http://www8.cs.umu.se/kurser/TDBAfl/VT06/algorithms/INDEX.HTM)，[Algorithm
 Design](http://www.cs.princeton.edu/~wayne/kleinberg-tardos/)，以及《算法学：计算精髓》，并辅以《编程珠玑》。[algorithmist website](http://www.algorithmist.com/index.php/Main_Page)。最后，拜一下[高德纳](http://book.douban.com/review/5384627/)的[TAOCP](http://book.douban.com/review/1319514/)，所谓家居编码旅行侃山必备利器。
据研究TCS的同学说，the power of randomness in computation是现代的计算复杂性理论最活跃也是最核心的一个部分，其目的在于fool any computationally bounded observer，而高德纳的伪随机卷似乎只是讨论ad hoc伪随机方法。他认为，伪随机是整个计算理论最核心也是最有影响力的理论，没有之一；现代的密码学正是植根于这套理论。他同时推荐了[伪随机的教材](http://people.seas.harvard.edu/~salil/pseudorandomness/)，以及（伪）随机课程（[Harvard版](http://people.seas.harvard.edu/~salil/cs225/)和[Wisc版](http://pages.cs.wisc.edu/~dieter/Courses/2013s-CS880/general.html)）。他认为，Prof.
 Yao是现代密码学的founding father，并对其论文theoy and applications of trapdoor functions的评价是“完全的神作，一语洞穿宇宙的奥秘”。
[Prof. Moni Naor](http://www.wisdom.weizmann.ac.il/~naor/), who taught
[Randomized Algorithms](http://www.wisdom.weizmann.ac.il/~robi/teaching/2013a-RandomizedAlgorithms/), conducts research in cryptography.
TCS也是有qual的，这是[UIUC版](http://valis.cs.uiuc.edu/~sariel/research/algorithms/quals/)和[Wisc版](http://research.cs.wisc.edu/areas/theory/qual.html)。
[Dictionary](http://xlinux.nist.gov/dads/) of Algorithms and Data Structures;
[SUNY Algo](http://www.cs.sunysb.edu/~algorith/)
[Interesting Pi](http://www.cafepress.com/notsohumblepi).
[Big O](http://bigocheatsheet.com/).
Off-topic:
The [Stacks project](http://stacks.math.columbia.edu/browse) on algebraic.
An interesting [graphic guy](http://www.connellybarnes.com/work/) who products a really, if not the most, popular feature in Adobe Photoshop.
[Prime Number Patterns](http://www.jasondavies.com/primos/), also a fantasy expamle for interactive web design.
3）Operating System
与DS & Algo需要考虑实际的应用场合一样，[操作系统](http://www.hrbxinzhi.com/os/resource.asp)也是门实践的学科。记得老师在课上说，当年他读书时，觉得银行家算法是这么优雅，为什么没有任何OS使用？原因就在于时间复杂度是O(n^3)。话不多说，熟悉[类UNIX环境](http://heather.cs.ucdavis.edu/~matloff/unix.html)后，参考[Linux
 Kernel Map](http://www.makelinux.net/kernel_map/)和《[Linux Kernel Development](http://www.makelinux.net/books/lkd2/)》，直接读[linux
 kernel](http://kernelnewbies.org/)的源码，比如0.11，一万行左右，市面上有代码剖析的书。我的建议是，以访客身份自学[工大cms](https://cms.hit.edu.cn/)的[OS课程](https://cms.hit.edu.cn/course/view.php?id=44)，自己动手实验+[智慧](http://bbs.csdn.net/IndexPage/SmartQuestion.aspx)的提问。有了[Bochs模拟器](http://people.cs.nctu.edu.tw/~chenwj/dokuwiki/doku.php?id=bochs)和0.11的基础，恐怕将来对任何OS部分都不会感到陌生（进一步可以读LDD;[阅读代码](http://blog.regehr.org/archives/940)的方法论可以参考《[Code
 Reading: The Open Source Perspective](http://book.douban.com/subject/1156148/)》），此时读者可就不只是“牛人”了，而是“大牛”甚至“专家”了（参见《[程序员的十层楼](http://software.intel.com/zh-cn/blogs/2009/02/04/400001071/)》）。[这篇文章](http://www.lingcc.com/2013/02/23/12261/)介绍了linux
 FS的进化史。MIT用于教学的类UNIX OS [XV6](http://pdos.csail.mit.edu/6.828/2012/xv6.html)是另一种选择。果然还是有推荐[OS
 Concepts](http://os-book.com/)，APUE，Unix编程艺术和UNP的。感兴趣的同学可以一起学习Wisc系统课程[Introduction to OS](http://pages.cs.wisc.edu/~remzi/Classes/537/Fall2013/)、[Advanced
 OS](http://pages.cs.wisc.edu/~swift/classes/cs736-fa12/wiki/pmwiki.php)和[Distributed Systems](http://pages.cs.wisc.edu/~swift/classes/cs739-sp12/wiki/pmwiki.php/Main/HomePage)。前者可以参考[Operating
 Systems: Three Easy Pieces](http://pages.cs.wisc.edu/~remzi/OSTEP/)，后两者直接读[paper](http://research.cs.wisc.edu/areas/os/Qual/readinglist-print.html)。而哈佛教授也在钻研[OS课程教学创新](http://mis-misinformation.blogspot.com/2013/02/cs161-one-womans-adventure-with-flipped.html)。[screen
 login tool](http://www.rackaid.com/resources/linux-screen-tutorial-and-how-to/)。
当然，也可以拿实时操作系统练练手，比如uC/OS，代码短小精悍，和linux kernel一样，pure C。[QNX](http://www.qnx.com/developers/docs/6.4.1/neutrino/getting_started/about.html)。除了平时就使用linux发行版外，百尺竿头，还可以学习如何自定制Linux系统[Linux
 From Scratch](http://www.linuxfromscratch.org/lfs/)。DWARF debug info in [Debian](http://lkml.indiana.edu/hypermail/linux/kernel/0912.2/00161.html)and thus Ubuntu as a static library is a root of evil.
国内还有本“[自己动手写操作系统](http://book.douban.com/subject/3735649/)”？
Win方面，好像有Window internals?虽然是牛书，但是用膝盖想想也知道，写出来的肯定还没深入到骨髓（那可是商业机密啊），不如读linux源码学得扎实。当然，我不太用win，也只是偶尔对win编程。Win API似乎设计得太诡异啦，一点也不正交。以操作内存页面权限为例，为啥设置页面权限的同时还返回页面的初始权限；而获取页面权限时却是返回给定页面的所有信息，再从中找出权限信息。当然，想进入微软，还是要积累些Win[系统编程](http://mindhacks.cn/2011/11/04/how-to-interview-a-person-for-two-years/)和[调试](http://blog.csdn.net/eparg/article/details/1792015)的阅读量的。[Win32
 cheat sheet](http://research.microsoft.com/en-us/um/people/simonpj/win32-cheat.html)也有帮助。这是Windows进程管理的[脚本](http://blog.codingnow.com/cloud/CmdTips)。
至此，还是要鄙视一下win——没有好用的终端（git的Win版都配有bash），只是[以图形界面赢得普通市场，取得商业成功](http://www.dbanotes.net/opensource/OS_and_Matrix.html)而已；当然win的内核技术事实上是比linux强大。君不见，哪个知名IT公司没有自己的集群？集群需要图形界面吗？在我看来，不用终端，似乎失去对整个计算机的控制；不爽，计算机是服务于人的，是被CSer控制的。顺便8g一下，“盖茨对'计算'这一概念的理解，是初中生水平；Google的Page，是本科生。”（引自《[程序员的十层楼](http://software.intel.com/zh-cn/blogs/2009/02/04/400001073/)》）现在，我还不确定实现[facemash](http://blog.andpue.com/michelangelo/7.html)的Zack是什么水平，但至少和Page是平手。再拜一下Amazon，这是因其云平台，才有Dropbox等一路风生水起；可叹中国，国家垄断。
至于选择日常使用的OS，王垠04年写过《完全用 GNU/Linux 工作》和《写给支持和反对<完全用Linux工作>的人们》([两文合集下载](http://ishare.iask.sina.com.cn/f/19062056.html))。正如之后有工大师弟向我介绍Windows的优势一样，事后也有人[批驳王垠](http://ishare.iask.sina.com.cn/f/11228358.html)。现在，王倒戈啦，但其观点[没法立足](http://blog.youxu.info/2013/03/03/unix-haters-2/)啊；等我准备完[OS
 qual](http://research.cs.wisc.edu/areas/os/Qual/)，新开一文试论类UNIX系统发展历程。在我看来，萝卜青菜，各有所爱嘛；凡事都有各自的因缘，保持和而不同的姿态，不是更体现多样化之美嘛。
还有，移动设备的OS有[自己的研究特点](http://matt-welsh.blogspot.com/2013/01/my-mobile-systems-research-wish-list.html)。固态硬盘也有其市场化的[局限](http://cseweb.ucsd.edu/users/swanson/papers/FAST2012BleakFlash.pdf)。
既然来地里的大都是出国的，那么清华学生写的《[OSDI, SOSP与美国著名计算机系的调查](http://blog.csdn.net/celestialwy/article/details/2766402)》自然不能错过；虽然有些信息已经过时，比如Prof Zhou已转到UCSD了；OSDI、SOSP已经“沦陷”了。文中以这两大OS领域顶级会议为基础，介绍了MIT、Princeton、Washington、Stanford、Berkeley和CMU的OS牛教授。我相信，包括我在内的大多非牛牛们，对这些学校大都只是dream而已（给写推荐信的教授直接灭了我Stanford,
 berkeley的念想，各种they are top云云），那么大家可以关注下这些牛教授的学生们，都去哪些地方任教了，比如Utah，OSU都是很不错的，可以尝试。另一方面，从[ASPLOS](http://www.sigplan.org/asplos.htm)、HPCA、[MICRO](http://www.microarch.org/)、[PACT](http://www.pactconf.org/)、[PLDI](http://www.sigplan.org/pldi.htm)、FAST、[SC](http://www.supercomp.org/)、[ICS](http://ics-conference.org/)和[CGO](http://www.cgo.org/)等会议[发表的论文](http://da-data.blogspot.com/2013/01/two-examples-from-computer-science.html)，可以大致了解系统方向的牛校。
4）Database
不好意思，我本科db没学好。但其实[db](http://duanple.blog.163.com/blog/static/70971767201311810939564/)是相当成熟也很有意思的领域。入门我就不推荐了，直接进阶吧——《数据库系统实现》。Prof. Anhai Doan recommends
[the Cow Book](http://www.cs.wisc.edu/~dbbook) and the
[slides](http://pages.cs.wisc.edu/~dbbook/openAccess/thirdEdition/slides/slides3ed.html), or
[the Complete Book](http://infolab.stanford.edu/~ullman/dscb.html). And this is a collection of
[resource](http://pages.cs.wisc.edu/~dbbook/openAccess/www_resources.html) about languages, technologies, and DB systems. Another
[course](http://www.labouseur.com/courses/db/) on DB systems.
[Guru](http://www.sigmod.org/publications/interview) in DB.
Wisc的DB据说比较强势：
i）[CS 564](http://pages.cs.wisc.edu/~cs564-1/) (Database Management Systems: Design And Implementation) is “everything you should know so that you can get
 an industrial job working with relational databases”.
ii）[CS 764](http://pages.cs.wisc.edu/~cs764-1/) (Topics In Database Management Systems) is “all the gory details you may (or may not) want to know about
 relational data management systems”.
iii）[CS 784](http://pages.cs.wisc.edu/~anhai/courses/784-spring13/) (Data Models And Languages) is “all the stuff beyond relational data (e.g., Web, text,
 data mining, data integration, data extraction) that you should know to broaden your data management knowledge or to work in the field as an advanced developer/researcher”. 而[其课程项目列表](http://pages.cs.wisc.edu/~anhai/courses/784-spring13/)足以成立若干创业公司。同时推荐[Principles
 of Data Integration](http://research.cs.wisc.edu/dibook/)。CMU就有将DI技术应用于建筑设计领域的startup [Schema Fusion](http://v.youku.com/v_show/id_XNTEyNzMwMTYw.html)。
Parallel db by [Dr. David DeWitt](http://gsl.azurewebsites.net/People/dewitt.aspx).
In the midst of Big Data era, we may need to have a profound understanding of
[NoSQL](http://www.aosabook.org/en/nosql.html) ([模式](http://horicky.blogspot.com/2009/11/nosql-patterns.html)，[漫谈](http://www.yankay.com/nosql%E6%95%B0%E6%8D%AE%E5%BA%93%E7%AC%94%E8%B0%88v0-2/),
[举例](http://nosql-database.org/)).
[In-memory computing infrastructure](http://natishalom.typepad.com/nati_shaloms_blog/2013/01/in-memory-computing-data-grid-for-big-data.html) for Big Data. CACM published
 a [blog](http://cacm.acm.org/blogs/blog-cacm/50678) on SQL databases v. NoSQL databases. However, Google developed
[F1](http://research.google.com/pubs/pub41344.html), a distributed rational database with the scalability of NoSQL key/value system, to support its core business, AdWords.
A review article on [Crowdsourcing systems](http://cacm.acm.org/magazines/2011/4/106563-crowdsourcing-systems-on-the-world-wide-web/fulltext).
5）Software Engineering
这简直是个人文学科，集计算机科学、管理学、心理学、社会工程学于一身。在此我不展开，可以参考UW教授的讲座[Software evolution then and now:
 the research of David Notkin](http://programanalysis.blogspot.com/2013/02/talk-video-software-evolution-then-and.html)。经典书籍也有很多分类，比如编码类（Write solid code, refactor, 各类[design pattern](http://en.wikipedia.org/wiki/Software_design_pattern)，《代码大全》第二版，《程序员修炼之道》，《编程卓越之道》，O‘Reilly“xx之美”系列，各种“UML，[XP编程](http://www.ruanyifeng.com/blog/2013/01/abstraction_principles.html)、敏捷、需求，测试”书籍），故事类（dreaming
 code），管理类（各种Joel on software，美国“创新工场”YC创始人Graham的《黑客与画家》，peopleware，37single的好书，甚至时间管理GTD的书籍）。同时敬礼图灵奖获得者、北卡教堂山教授、《人月神话》作者Brooks，但我推荐的是他另一本书《设计原本》。
The co-author (and UW alumnus) of [lmbench](http://www.bitmover.com/lmbench/lmbench.html), a system performance benchmark, is proud of his
[Sourceware Operating System proposal](http://www.bitmover.com/lm/papers/srcos.html) considered as the peer to Raymond's
[Cathedral and Bazaar paper](http://www.catb.org/esr/writings/homesteading/). He also recommends
[this](http://www.bitmover.com/lm/bugs.html) to everyone who was annoyed at countless bugs.
搭车说一下，我不是鄙视web。如果想学web，也应该学习CSS，Ruby的framework，再不济也得HTML5。在我看来，后端架构更为重要，包括apache，linux sever管理（[鸟哥的linux私房菜馆](http://linux.vbird.org/)）之类的。但如果真想学web，推荐[UIUC课程](https://wiki.engr.illinois.edu/display/cs498stk/Home)，我上届创业[师兄](http://zhiyan.de/)建立的[pureweber](http://wiki.pureweber.com/)及其搭档的关于前后端开发的[博客](http://www.fancycedar.info/)，至少[坚持180天](http://jenniferdewalt.com/index.html)。
SE [paper bibliography/categorization/classification](https://sites.google.com/site/asergrp/bibli) by Prof. Tao Xie's Automated Software Engineering Research
 Group
Stanford的[创业课程](http://startup.stanford.edu/)
6) Compiler及PL设计哲学
出来混，早晚都要还的。我本科学的最差的就是编译；一个字，混。结果研究生做的都是编译器，即[LLVM](http://llvm.org/)。那些用iDevice的同学为什么觉得系统很流畅，[闭源版本的llvm功不可没](http://www.programmer.com.cn/9436/)；当然还有基于freeBSD（B是伯克利的哦，他们有做工具、开源的传统。8g一下，现在Berkeley的[工程系主任Sastry教授](http://robotics.eecs.berkeley.edu/~sastry/)，就是受不了Harvard的理论派作风，于1994年跳过来白手起家做系统的）的Mac
 OS X及其简化版iOS的功劳。北京intel研究院实习的师兄跟我说，intel之所以牛，是在于其编译器优化。所以说，学好OS和compiler，走遍天下都不怕。
可惜学到这门课，大家应该大三了。很多人[如梦方醒](http://duanple.blog.163.com/blog/static/709717672012535958380/)，要面对出国[保研考研工作](http://www.cloudfly.info/archives/181)公务员等等了，无心恋战。但从[这门课](http://duanple.blog.163.com/blog/static/7097176720111141085197/)（包括以前的汇编语言，工大是大一下在C之后开设的）起，才能真正体现CSer与其它非计算机的区别。CSer应有的境界就是，写[高级语言](http://erlang.org/pipermail/erlang-questions/2013-January/071949.html)的时候能想到对应的汇编；这应该要花[10年功夫](http://blog.youxu.info/21-days/)，或至少[10,000小时](http://www.ruanyifeng.com/blog/2011/04/the_dan_plan.html)吧？
[徒手实现C++11](http://cppgm.org/)？这是OSDev论坛讨论此项目的[帖子](http://forum.osdev.org/viewtopic.php?f=13&p=219252)。
编译强调的更是优化。这里介绍了[基于Intel处理器的优化](http://www.agner.org/optimize/)。[编译器构造相关的主题](http://www.bearcave.com/software/comp_index.html)。
《程序设计方法》《计算机程序的构造和解释》《程序设计语言概念》《程序设计语言原理》，甚至《自动机理论、语言和计算导论》《计算理论导引》才应是计算机专业学生应该具有的理论素养。优秀的程序员应是[理解程序设计语言原理](http://blog.sina.com.cn/s/blog_5d90e82f010165wf.html)的，包括[Relational
 operator](http://en.wikipedia.org/wiki/Relational_operator)，[lazy evaluation](http://blog.enfranchisedmind.com/2007/01/ocaml-lazy-lists-an-introduction/)，[Abstract
 Interpretation](http://www.di.ens.fr/~cousot/AI/IntroAbsInt.html)等概念；即使不理解语言原理，也能围观[各种编程语言的特色](http://www.aqee.net/if-programming-languages-were-cars/)。Wisc有[介绍程序语言原理的课程](http://pages.cs.wisc.edu/~cs704-1/cs704.html)，使用[OCaml](http://ocaml.org/)（这是[99难题](http://ocaml.org/tutorials/99problems.html)和[其标准库实现](http://caml.inria.fr/ocaml/anonsvn.en.html)；阅读STL是学习语言的最好方式）。编译点滴blog也给出了[Compiler书目的点评](http://www.lingcc.com/2012/05/16/12048/)。Utah
 Prof. John Regehr科研之余写[blog](http://blog.regehr.org/)。Utah另一位研究PL、编译的AP
[Matt Might](http://matt.might.net/)也有关于[PL](http://matt.might.net/articles/books-papers-materials-for-graduate-students/#pl)、[Compilers](http://matt.might.net/articles/books-papers-materials-for-graduate-students/#compilers)、[static
 analysis](http://matt.might.net/articles/books-papers-materials-for-graduate-students/#analysis)的推荐；更重要的是，其[blog](http://matt.might.net/articles/)营养价值之高，让我惊叹！
推荐研究PL的[王垠](http://www.cs.indiana.edu/~yw21/)（[11年及以前的状况](http://kurttlin.diandian.com/post/2011-10-03/5502426)）的《初学者程序语言的选择》。Indiana
 U in Bloomington教授、PL领域先驱[Daniel Friedman](https://www.cs.indiana.edu/~dfried/)（[wikipedia](http://en.wikipedia.org/wiki/Daniel_P._Friedman)；从其学生王垠的叙述，怎么看怎么像CS领域的“周伯通”[费曼](http://book.douban.com/review/5258811/)）01年做了主题为The
 Role of the Study of Programming Languages in the Education of a Programmer的演讲（[pdf原文](http://www.cs.indiana.edu/~dfried/mex.pdf)，[PPT
 slides](http://www.cs.indiana.edu/hyplan/dfried/SLIDES~1.PPT)）。
当然，不能少了龙书虎书和鲸书。Wisc的程序语言和编译方向的[qual](http://www.cs.wisc.edu/sites/default/files/pl.reading.pdf)自然不可错过。对于[the
 life of Binaries](http://www.youtube.com/watch?v=D8gFWWyWr0k)，程序员又知多少。编译也是注重实践的一门学科；基于[SSA形式](http://ssabook.gforge.inria.fr/latest/book.pdf)中间表示的LLVM从UIUC发芽，经过10多年的发展，成为apple主力支持的用于iOS的商业编译器，且大有替代gcc之势（FreeBSD已经完成了这一步），不可谓不是个奇迹。但有时，有人[质疑](http://blog.willbenton.com/2010/04/research-and-development/)PL研究，比如program
 slicing，的价值。
关于编译的牛校，Rice肯定有一席之地，历史上就完成了多个高性能编译器设计的研究，现存的有[Habanero](http://habanero.rice.edu/Habanero_Home.html)。
Facebook从10年起开发、[华人](http://developers.facebook.com/blog/post/2010/02/02/hiphop-for-php--move-fast/)始创的[HHVM](http://www.facebook.com/note.php?note_id=10150415177928920)([paper](https://www.facebook.com/publications/370247373089711/))旨在优化[PHP](http://www.digital-web.com/articles/php_for_designers/)，[技术细节](https://github.com/facebook/hiphop-php)是[PHP2C++](http://www.youtube.com/watch?v=p5S1K60mhQU)以及[Dynamic
 Binary Translation](http://blog.kowalczyk.info/article/67j/Summary-of-David-Ditzel-talk-on-binary-translati.html) & [Optimization](http://www.ccf.org.cn/resources/1190201776262/2011/09/20/2.pdf)。
7) Computer Architecture
海纳百川，系统结构就是这样一门学科——把低层的指令集/汇编、中间的OS、高层的PL熔于一炉。君不见，因为银河机，国防科大永远是中国计算机系的翘楚，虽然现在也开始浮躁了，但清华华科上交（以及南大某并行实验室）还是难以望其项背。
系统结构可大可小。小的指指令集，大的可以包括计算机组成原理、编译（每个指令集都需要自己的编译器）。所以，我把帕特森和轩尼诗的两本扛鼎之作和盘托出——《计算机组成和设计：软硬件接口》和《计算机系统结构：量化的研究方法》（caaqa）。读不懂没关系，[书读百遍，其意自现](http://pages.cs.wisc.edu/~ragh/Qualfiles/base.html)。还有《Readings
 in Computer Architecture》《[Parallel Computer Organization and Design](http://www.cambridge.org/us/knowledge/isbn/item6633770/?site_locale=en_US)》，[ASOA](http://www.aosabook.org/en/index.html)以及《[21世纪计算机体系结构——计算机体系结构共同体白皮书](http://www.sigarch.org/2013/01/06/chinese-translation-21st-century-computer-architecture/)》。同时，推荐陈首席及其[弯曲评论](http://www.tektalk.org/)，保罗万象，从系统结构到IT史话，从千人计划到复兴中国，特别推荐“弯曲推荐”。至于国内做编译/系统结构的blog
 roll，有IBM员工的[并行实验室](http://www.parallellabs.com/)，中科院博士的[编译点滴](http://www.lingcc.com/)，[HelloGCC](http://www.hellogcc.org/)，台湾研究员[陳韋任](http://people.cs.nctu.edu.tw/~chenwj/)、特别是他的[wiki](http://people.cs.nctu.edu.tw/~chenwj/dokuwiki/doku.php)。
还是那句话，[计算机专业是很practical的](http://www.tektalk.org/2010/11/21/%E7%B3%BB%E7%BB%9F%E8%BD%AF%E4%BB%B6%E7%A0%94%E7%A9%B6%E7%9A%84%E4%B8%80%E4%BA%9B%E8%B0%83%E6%9F%A5%E6%95%B0%E6%8D%AE/)，Dr.
 Brooks是因为领导IBM 360而成就的图灵奖；系统结构领域也是很活跃的——传统的处理器设计（包括cache一致性、互联网络等）、软件层级的并行编程模型（如TAMU的STAPL、UCLA的CDSC、Intel的TBB等等）、多核众核，以及利用GPU/FPGA加速的可重构计算等等，甚至还包括系统平台相关的编译支持。[Mr.
 Esmaeilzadeh](http://homes.cs.washington.edu/~hadianeh/)关于系统结构的进化，特别是多核芯片功耗的研究，非常具有前瞻性；[Prof.
 Dave Patterson](http://cacm.acm.org/magazines/2012/7/151242-technical-perspective-for-better-or-worse-benchmarks-shape-a-field/fulltext)和[Dr. Pradip Bose](http://dl.acm.org/citation.cfm?id=2408796)分别阐述了自己的观点。务实的方案是利用现有技术扩展计算能力，比如CMU的FAWN。
Gene M. Amdahl Prof. Mark Hill shared [PBS American Experience: Silicon Valley](http://www.pbs.org/wgbh/americanexperience/films/silicon/) and pointed out
 that “This excellent documentary is mostly about how discrete transistors and then integrated circuits became commercially successful at Fairchild Semiconductor, which then in turn spawned many companies. At one time NASA bought over half the transistors sold,
 accelerating computer development (and IMHO allowing you to have a smartphone now and not in decade)”.
Wisc维护的[World-Wide Computer Architecture Information](http://pages.cs.wisc.edu/~arch/www/)以及[Computer
 Architecture Research at Wisconsin](http://pages.cs.wisc.edu/~arch/uwarch/)提供了很多信息。ISCA是系统结构的顶级会议；有些国人分享了他们在ISCA的经历：计算所研究生的《[ISCA和中国](http://hi.baidu.com/microarch/blog/item/458c308f0a3b4119b31bba8e.html)》，[计算所系统组副教授](http://asl.ncic.ac.cn/baoyg/)的《[ISCA08见闻](http://blog.sciencenet.cn/blog-414166-302386.html)》，[Prof.
 Kevin Skadron](http://www.cs.virginia.edu/~skadron/)学生记录ISCA 2011的《[ISCA归](http://www.douban.com/note/155598567/)》。
原复旦大学副教授、现上交软件学院教授陈海波，于11年在中国计算机学会通讯专栏发表的《一名系统研究者的攀登之路》([pdf](http://www.ccf.org.cn/resources/1190201776262/2011/09/21/7.pdf))，很适合作为Statement of Purpose的第一部分，即Why
 computer architecture。按我导师的话说，与其说是此人的攀登之路，不如说是本来很牛、思维很活跃的人（从04年大四到11年投中若干重要甚至顶级会议SOSP，只花了7年时间），通过明师（注意，是明白的明，不是名气的名）指点，在转换若干方向（包括VLIW的GCC后端移植和可重构计算）后，选定系统虚拟化这个三年后国内很火的领域后，一路顺风顺水，并从此处在系统虚拟化研究的浪潮之巅，无论是从科研经历（在MSRA和MIT鼎鼎大名的PDOS组和大牛人[Frans
 Kaashoek](http://pdos.csail.mit.edu/~kaashoek/)共事），还是发表的论文（包括ISCA'08、MICRO 2009、PACT 2010、SOSP 2011、PPoPP 2011等）的角度。《ISCA08见闻》谈及的Harford博士毕业、现在是上交教授的Xiaoyao Liang，也很幸运——“他的老板是David Brooks，给他指导了一个很好的方向，避开了功耗这个热点，选择Process Variation这个突破口，在Top Conference上发表了一系列文章。”事实上，每个博士都有自己进入某一领域的独特因缘，否则真应该思考一下[Why
 Ph.D.](http://www.pgbovine.net/undergrad-research-guest-article.htm)——6年的commitment（特指美国）。来自台湾的MIT CS博士对为什么读博士也有[他的观点](http://blog.vgod.tw/2012/03/20/why-i-do-phd/)（需翻墙）。以上故事能有所借鉴最好，实在不行，当成茶余饭后的一个段子，也能博得一笑，十年少！
而从我的偶像，[LLVM](http://www.drdobbs.com/architecture-and-design/the-design-of-llvm/240001128)首席architect Chris Lattner自述的[简短生平](http://www.nondot.org/sabre/bio.html)中，能从中了解他的成长轨迹；更重要的是，他和咱的差距也就是十年！不过，系统结构方向更务实的马拉松目标，可以定为Wisc的09届本科、11届硕士，UIUC
 11年入学的博士[Matt Sinclair](http://pages.cs.wisc.edu/~sinclair/)。
最后，推荐威斯康馨系统结构课程[Advanced Arch I](http://pages.cs.wisc.edu/~markhill/cs752/Fall2011/)和II（[Prof.
 Mark Hill](http://pages.cs.wisc.edu/~markhill/cs757/Spring2012/)或[Prof. Mikko Lipasti](http://ece757.ece.wisc.edu/)版），[多核处理器编程](http://pages.cs.wisc.edu/~david/courses/cs758/Fall2013/wiki/)，以及Qual的[备考资料](http://pages.cs.wisc.edu/~karu/qualwiki/)和[历年真题](http://www.cs.wisc.edu/academic-programs/graduate-program/phd-qualifying-exams-archive)，让想做CA研究的感受一下！话说，我当年本科时苦于没人指导如何研究系统结构，就下载《微处理器设计》、caaqa等经典书籍每章后面感兴趣的论文，陷于浩瀚paper海而没有灯塔指引。希望后来者不要重蹈我的覆辙！
上面推荐了一些书，但是对于《[最常被程序员们谎称读过的计算机书籍](http://www.aqee.net/books-programmers-dont-really-read/)》，不知是不是有人躺枪了；反正，我是中了。
8) Systems
CS科研，尤其是系统领域的研究，不是一门课的知识就能涵盖的。比如，[此文](http://highscalability.com/blog/2013/1/28/duckduckgo-architecture-1-million-deep-searches-a-day-and-gr.html)介绍了垂直搜索引擎[Duckduckgo](https://github.com/duckduckgo)的架构。
[虚拟化](http://pages.cs.wisc.edu/~remzi/Classes/838/Spring2013/)也是系统领域研究的热点之一，比如Virtual Machine Moniter应用于smartphone平台方面，[哥大](http://systems.cs.columbia.edu/lab/news/)比较前沿。
网络方面本人水平有限，暂归此类。这是[网络方向会议统计](http://www.cs.ucsb.edu/~almeroth/conf/stats/)。[Chrome
 Browser architecture](http://lizstinson.blogspot.com/2009/06/browser-architecture-chrome.html). An interesting [trick, powered by Google,](http://lizstinson.blogspot.com/2009/09/so-i-recently-added-google-analytics.html)to augment the visiting counter for ur website; I am sure I am joking.
[MultiPath TCP](http://multipath-tcp.org/) seems to be
[promising](http://blog.codingnow.com/2014/02/connection_reuse.html) in the mobile networking settings.
纷繁复杂的分布式系统永远是系统领域研究的皇冠之一。Google Fellow[Jeff Dean](http://research.google.com/people/jeff/)必然是其中翘楚。Google分布式计算的[三驾马车](http://blog.sina.com.cn/s/blog_4ed630e801000bi3.html)。[MapReduce](http://lizstinson.blogspot.com/2009/09/some-google-technology-briefly-pagerank.html)其实是FP的两个higher-order
 functions map和reduce；当然，这也不是大数据处理的救世主，详见[A Comparison of Approaches to Large-Scale Data Analysis](http://dl.acm.org/citation.cfm?id=1559865)。关于[分布式系统](http://pages.cs.wisc.edu/~swift/classes/cs739-sp12/wiki/pmwiki.php/Main/DistributedSystemArticles)和[scaling](http://pages.cs.wisc.edu/~swift/classes/cs739-sp12/wiki/pmwiki.php/Main/ScalingPresentations)的一些文章。[Distributed
 Key-Value Stores](http://www.metabrew.com/article/anti-rdbms-a-list-of-distributed-key-value-stores)。云计算。[此博客](http://the-paper-trail.org/blog/)主要讨论分布式领域技术。
[QoS of SE](http://jeffhuang.com/search_query_logs.html)
有一类分布式系统是基于[事件驱动模型](http://www.elementallinks.com/2011/02/06/5th-anniversary-edition-event-driven-architecture-overview/)。微软也有一篇[综述](http://msdn.microsoft.com/en-us/library/dd129913.aspx)。
这是[高级CS课程合辑](http://the-paper-trail.org/blog/advanced-computer-science-courses/)。
[德国德累斯顿工业大学的系统方向论文阅读列表](http://www.inf.tu-dresden.de/index.php?node_id=1738)。
9) Security & Privacy
有人的地方就有江湖，也有安全和隐私。计算机安全领域的会议统计有很多人维护：[麦屯版](http://pages.cs.wisc.edu/~mihai/conference%20acceptance%20rates.html)、[TAMU版](http://faculty.cs.tamu.edu/guofei/sec_conf_stat.htm)、[新加坡版](http://icsd.i2r.a-star.edu.sg/staff/jianying/conference-ranking.html)、[英国版](https://personal.cis.strath.ac.uk/changyu.dong/ranking.html)、[沙特版](http://faculty.kfupm.edu.sa/ics/zhioua/Research/ResearchResources.html)。IEEE
 S&P展示了数十年该领域的[研究趋势](http://www.ieee-security.org/TC/SP-Index.html)以及近些年的[会议总结](http://www.ieee-security.org/TC/Reports.html)。加拿大Carleton大学汇编了[一些资料](http://www.ccsl.carleton.ca/conferences/)。而TAMU的华人AP也收集了[一些信息](http://faculty.cs.tamu.edu/guofei/links.htm)。[Security
 and Privacy related Events and Slang](http://www.ieee-security.org/Calendar/cipher-hypercalendar.html). [Computer Security Foundations Symposium workshop](http://www.ieee-security.org/CSFWweb/)。[DIMACS
 workshop](http://dimacs.rutgers.edu/Workshops/)。
UT-Dallas的[安全课程](http://www.utdallas.edu/~zxl111930/#toc3)；雪城大学以instructional laboratories for security education闻名的[计算机安全](http://www.cis.syr.edu/~wedu/Teaching/cis643/)和[网络安全](http://www.cis.syr.edu/~wedu/Teaching/cis644/)课程；麦屯的[安全课程](http://pages.cs.wisc.edu/~rist/642-fall-2012/)和[密码学课程](http://pages.cs.wisc.edu/~rist/838-spring-2012/)。
[Handbook of Applied Cryptography](http://cacr.uwaterloo.ca/hac/)
病毒（Malware）早已不是单纯的压缩和加密，还包括利用packer工具，甚至是protection packer工具，对可执行程序进行混淆（obfuscation）处理。Cisco安全博客回顾了[病毒的obfuscation技术历史](http://blogs.cisco.com/security/a_brief_history_of_malware_obfuscation_part_1_of_2/)。Panda研究所介绍了[packer工具的进化史](http://research.pandasecurity.com/packer-revolution/)。
[av-test](http://www.av-test.org/)
A [website](http://www.securityfocus.com/) focus on security & Vulnerabilities
Security tools to [download](http://www.spychecker.com/)
10）课程之外
回首这些年，很高兴学计算机，不仅是因为与人相比，计算机其实很“简单”，照着程序执行，出现了意外也是可评估可能恢复并规避的——每个程序员都是折翼的天使，在计算机世界里构建属于自己的理想国，没有尔虞我诈；更是因为通过计算机专业的学习（推荐MIT CS五年本硕、海南中学07届师弟给08级高中生[介绍学习经验的视频](http://www.56.com/u11/v_NTc3NDMyMjQ.html)），更懂得如何获得更多的信息，以更开放的心态接受新鲜事物，而不是成为完全执着保守的左派。这是另一个[励志的故事](http://blog.csdn.net/v_july_v/article/details/6628086)，在平庸的大学做不平凡的计算机人。事实上，很多学计算机的人喜欢科幻，喜欢哲学，比如冯大师的《计算机和人脑》、《万里任禅游》，喜欢讨论技术背后体现的价值观，如《我们的防火墙:网络时代的表达与监管》。
计算机的[博士研究](http://www.pgbovine.net/PhD-memoir.htm)，需要扎实的[数学基础](http://cstheory.stackexchange.com/questions/3253/what-books-should-everyone-read)，更需要人文素养。在介绍SE时，其实更多的书目是偏人文的。是的，我认为中国缺少安贫乐道、少欲知足的精神，这与缺乏真正[Ph.D.](http://www.phdcomics.com/comics/aboutcomics.html)教育——五到六年时间在一个方向钻研（[Prof.
 Mike Gleicher](http://pages.cs.wisc.edu/~gleicher/Web/)给出了很多建议）——有关。以计算机专业而言，本科学位是B.E.，工科学士；研究生或是M.S.，科学大拿；博士是[Ph.D.](http://graphics.stanford.edu/~levoy/unc-commencement.html)，哲学博士。[Ph.D.](http://da-data.blogspot.com/2013/01/on-phd.html)毕业后，追求的不应该是过分的物质生活，反而应是研究领域精神上的快乐！
这也与传统文化的缺失有关，包括我在内，没几人读过千字文、三字经、弟子规、四书五经、春秋、左传。另外，学计算机技术也是要读[计算机发展史](http://www.hrbxinzhi.com/history/resource.asp)、参观[硅谷](http://www.ruanyifeng.com/blog/2010/10/where_to_see_silicon_valley.html)的[计算机历史博物馆](http://blog.sciencenet.cn/blog-414166-568032.html)的，什么《数学之美和浪潮之巅》《[激荡三十年](http://book.douban.com/subject/1970428/)》《[沸腾十五年](http://book.douban.com/subject/3816827/)》《[IT史记](http://book.douban.com/subject/1085729/)》《[IT通史](http://book.douban.com/subject/1434831/)》《[IT大败局](http://book.douban.com/subject/1230055/)》，以及[《信息技术文化史》豆列](http://book.douban.com/doulist/154109/)，了解什么技术方兴未艾，什么如日中天，什么日薄西山。同时推荐这个ted视频，[how
 inspired leader inspire others](http://www.ted.com/talks/simon_sinek_how_great_leaders_inspire_action.html)，里面有描述创新的源泉和从新兴到衰亡的市场趋势分析。桑德尔的《公正》，讨论的就是很贴近生活实际的道德标准评判。而《细读美国大学》则是让国人了解米国大学的细节，以及如何在米国读大学，特别是本科Liberal Art教育（通识教育、素质教育都不是恰当的翻译）。子曰从心所欲而不逾矩，庄子说逍遥游，佛家的观自在（即大悲观世音菩萨的别称），让我向往，让我思考，让我自由！
跑远了，期待一下[量子计算机](http://www.lingcc.com/2010/12/23/11431/)吧。那时的计算机基础课，可能完全不是上面这一套了。1）应该是量子力学了吧？
总之，祝福所有CSer，这是门最好的学科。祝福所有飞友，并附上[我的飞跃总结](http://www.douban.com/note/269668597/)以及[Remzi的CS牛校统计图](http://2.bp.blogspot.com/-adt1AR56vHg/UZuO_CmzAJI/AAAAAAAAAws/4NXz0sbgS18/s1600/zips-2013.jpg)，心安处，即是家。南无阿弥陀佛！
不好意思，知者不言，言者不知。喷了一地口水，与我头像不符，[常惭愧](http://www.lianhua33.com/gs/gs131.htm)。
