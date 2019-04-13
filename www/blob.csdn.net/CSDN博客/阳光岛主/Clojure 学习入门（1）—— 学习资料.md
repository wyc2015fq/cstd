
# Clojure 学习入门（1）—— 学习资料 - 阳光岛主 - CSDN博客

2013年12月22日 08:38:20[阳光岛主](https://me.csdn.net/sunboy_2050)阅读数：12350所属专栏：[Clojure 学习总结](https://blog.csdn.net/column/details/learn-clojure.html)



**Clojure**（发音类似"closure"，['kləʊʒə(r)]）是一套现代的Lisp语言的动态语言版，它是一个函数式多用途的语言，其语法和其他的[Lisp](http://www.lispchina.com/)一样，都是建立在[S-expression](http://en.wikipedia.org/wiki/S-expression)之上，即”全是括号，前缀表达式”的语言。
Clojure可以执行于Java虚拟机（JVM）、通用语言运行时（CLR），以及JavaScript引擎之上。与其他Lisp一样，Clojure认为代码即数据，同时有复杂的宏系统。


### Clojure 的理念和特性
Clojure是什么，为什么有人要学这门语言? 总的来说，那些认为 Clojure会在现有的大多数编程语言中取得成功的人至少基于这样一个理由:它是一门新的，支持并发的语言。而那些对lisp语言缺乏了解的人却往往会认为：这是一门相当奇怪的，充斥着令人困惑的圆括号和方括号的语言.
但是，不争的事实是 Clojure 在市场上赢得比其他新诞生的编程语言更多的追捧和表现，更强的发展势头。它第一个对外的发行版是在2007年秋季的时候发布的，在2009年5月发行了第一个稳定版，到这个时候，它已经培育了一个活跃的、充满热情的社区，一个欣欣向荣的开发工具和类库的生态系统，它们被用在迅速增长的一系列专业的应用程序中。

### 下一代的语言
每一年，在世界范围内的众多机构中的都有很多有才华的计算机科学家通过发表成千上万的论文来阐发它们新的和有趣的想法。这些新的概念在经受了自然选择和沉淀后，其中那些优秀和有用的部分会被准许进入实际的应用环境中。
Clojure 包括了众多尚未在其他(任何)编程语言的实现中出现的最新的想法和伟大的理念. 这其中最引人注目的就是涉及并发程序的部分：软件内存交互和基于代理的进程调度 (Software Transactional Memory and agent-based processing ) 被 "烧录进" (嵌入)到 Clojure编程语言的底层. 除此之外(例如持续不可更改 persistent immutability) 还拥有很多精妙的设计理念，这些设计理念是现代学术研究的成果，也是数十载实践的经验。
抛开Clojure的学术背景，Clojure 是主要设计原则是坚持: 有用性(useful 有益处的) 高于可用性. 它的高级属性是被精心选择的,目的是向程序员提供健壮的、干净的代码，以便程序员容易理解和高效编程. Clojure虽然有很强的学术背景支持，但是绝不是一门只在象牙塔中的语言，每天都有众多开发者在各自的开发领域内使用它。

### 动态而强有力 (没错,它是lisp!)
对下面语句的意思的理解会因为读者编程经验的不同而有很大差异，它可能会让你非常兴奋，也可能让你产生一些轻微的不适.Clojure 是完全的，真正意义上的神圣的lisp语言的一个方言。
lisp语言因为其无以伦比强大能力和几乎无穷的表达力而获得了盛誉，Clojure自然也不例外。它的功能和元编程的能力是建立在这样的基础之上的：异常驯服的C语言的"石头" 或 具有延展性的java语言的"木头" . 你可以用几百行甚至几十行Clojure代码取替代几千行静态语言的代码，伴随着这而来的是bug数量的减少和开发时间的缩短。
样板代码(Boilerplate code )被完全删去. 域指定语言(Domain Specific Languages ) 不仅简单，而且更一般化--lisp程序往往是按照 "自下而上" 的开发方式写成的。展开式(演进式)的 结构和语法更适合特定的问题领域。你在程序运行的时候取修改程序，而不需要重新编译或重启程序。
但是，历史上也有对lisp进行诋毁的人，或许称为抱怨更为合适. lisp发展过程中，没有完整的规范, 各种不兼容的实现,陈旧落伍的限制.cruft accumulate 在其存在的四五十年里一直存在. 对于 大多数人来说，它的语法过于诡异了。
Clojure 修正上面的大多数这些问题. 它保留了 lisp 的思想和哲学,并同时清除了过去的很多限制. Clojure 高速、干净、具有优先能力和优雅的特征. 但是没有改变lisp中 "代码也是数据" 的哲学. Clojure 语言在直觉和观感上比历史上的lisp更易于阅读. 在后面开始学习的初级阶段,你就发现虽然 仍有各种括号,但是代码是难以置信的容易读和写。
对于那些熟悉lisp语言的人来说,他们很快就会发现他们非常适应Clojure. 对于那些原先没有接触 过lisp的人来说，也没有必要担心， Clojure是简洁和无痛学习。

**官方文档：**
[http://clojure.org/documentation](http://clojure.org/documentation)
**中文资料**（强烈推荐）：
1. clojure入门教程
[http://xumingming.sinaapp.com/302/clojure-functional-programming-for-the-jvm-clojure-tutorial/](http://xumingming.sinaapp.com/302/clojure-functional-programming-for-the-jvm-clojure-tutorial/)
2. clojure文档翻译
[http://code.google.com/p/clojure-doc-en2ch/w/list](http://code.google.com/p/clojure-doc-en2ch/w/list)
3. clojure手册（低版本）
[http://qiujj.com/static/clojure-handbook.html](http://qiujj.com/static/clojure-handbook.html)
4. clojure中文社区：
[http://blog.clojure.cn/](http://blog.clojure.cn/)
5. Lisp中文社区：
[http://www.lispchina.com/](http://www.lispchina.com/)
**英文资料**（强烈推荐）：
[http://clojure-doc.org/articles/content.html](http://clojure-doc.org/articles/content.html)
**clojure 开源工具：**
clojure工具箱汇总：
[http://www.clojure-toolbox.com/](http://www.clojure-toolbox.com/)
ring server：
[https://github.com/ring-clojure/ring/wiki](https://github.com/ring-clojure/ring/wiki)
compojure：
[https://github.com/weavejester/compojure/wiki](https://github.com/weavejester/compojure/wiki)
noir web框架：
[http://www.webnoir.org](http://www.webnoir.org/)
sandbar session框架：
[https://github.com/brentonashworth/sandbar/wiki](https://github.com/brentonashworth/sandbar/wiki)
clojureQL连接数据库：
[http://www.clojureql.org/](http://www.clojureql.org/)
datomic数据库：
[http://www.datomic.com/](http://www.datomic.com/)
fleetdb数据库：
[http://fleetdb.org/](http://fleetdb.org/)
[https://github.com/mmcgrana/fleetdb-client](https://github.com/mmcgrana/fleetdb-client)
pdf处理：
[https://github.com/yogthos/clj-pdf](https://github.com/yogthos/clj-pdf)
csv处理：
[https://github.com/clojure/data.csv](https://github.com/clojure/data.csv)
json库：
[https://github.com/dakrone/cheshire](https://github.com/dakrone/cheshire)
[https://github.com/mmcgrana/clj-json](https://github.com/mmcgrana/clj-json)
mail库：
[https://github.com/clojurewerkz/mailer](https://github.com/clojurewerkz/mailer)
[https://github.com/MayDaniel/clj-mail](https://github.com/MayDaniel/clj-mail)
图形处理库：
[https://github.com/mikera/clisk](https://github.com/mikera/clisk)
[https://github.com/ztellman/penumbra/wiki/Applications](https://github.com/ztellman/penumbra/wiki/Applications)
[https://github.com/quil/quil](https://github.com/quil/quil)
音频处理库：
[https://github.com/overtone/overtone](https://github.com/overtone/overtone)
报表工具库：
[http://incanter.org/](http://incanter.org/)
定时任务框架：
[http://clojurequartz.info](http://clojurequartz.info/)
webservices
[http://clojure-liberator.github.com](http://clojure-liberator.github.com/)



### 附：
### 编程语言发展的编年史

编程语言的历史早于真正意义的计算机的出现。19世纪就有“可编程的”织布机和钢琴弹奏装置出现，它们都是领域特定语言(DSL)的样例。
据统计已有256种编程语言，这么多的语言中只有一些是常用的或实用的。
1951 – Regional Assembly Language
1952 – Autocode
1954 – IPL (LISP语言的祖先)
1955 – FLOW-MATIC (COBOL语言的祖先)
1957 – FORTRAN(第一个编译型语言)
1957 – COMTRAN (COBOL语言的祖先)
1958 – LISP
1958 – ALGOL 58
1959 – FACT (COBOL语言的祖先)
1959 – COBOL
1959 – RPG
1962 – APL
1962 – Simula
1962 – SNOBOL
1963 – CPL (C语言的祖先)
1964 – BASIC
1964 – PL/I
1966 – JOSS
1967 – BCPL (C语言的祖先)
1968 – Logo
1969 – B (C语言的祖先)
1970 – Pascal
1970 – Forth
1972 – C
1972 – Smalltalk
1972 – Prolog
1973 – ML
1975 – Scheme
1978 – SQL
1980 – C++(既有类的C语言，更名于1983年7月)
1983 – Ada
1984 – Common Lisp
1984 – MATLAB
1985 – Eiffel
1986 – Objective-C
1986 – Erlang
1987 – Perl
1988 – Tcl
1988 – Mathematica
1989 – FL
1990 – Haskell
1991 – Python
1991 – Visual Basic
1993 – Ruby
1993 – Lua
1994 – CLOS (ANSI Common Lisp的一部分)
1995 – Java
1995 – Delphi (Object Pascal)
1995 – JavaScript
1995 – PHP
1996 – WebDNA
1997 – Rebol
1999 – D
2000 – ActionScript
2001 – C\#
2001 – Visual Basic .NET
2002 – F\#
2003 – Groovy
2003 – Scala
2007 – Clojure
2009 – Go
2011 – Dart
从1951年至今，人类一共发明了256种编程语言，每一种语言的出现都带有某些新特征。编程语言不断的在革新，很快就会有超出这个清单的新编程语言出现。更多参考[程序员必须知道的编程语言编年史](http://www.oschina.net/news/46882/history-of-programming-languages-must-know)


