# 信息检索及DM必备知识总结：luncene - wishchinYang的专栏 - CSDN博客
2014年01月23日 14:10:55[wishchin](https://me.csdn.net/wishchin)阅读数：669
原文链接：[http://blog.csdn.net/htw2012/article/details/17734529](http://blog.csdn.net/htw2012/article/details/17734529)
有少量修改！如有疑问，请访问原作者.
**一：信息检索领域：**
信息检索和网络数据领域（WWW, SIGIR, CIKM, WSDM, ACL, EMNLP等）的论文中常用的模型和技术总结(为什么概率是可靠的，概率隐藏了大部分事实，而给予我们可以看得见的部分.)
引子：对于这个领域的博士生来说，看懂论文是入行了解大家在做什么的研究基础，通常我们会去看一本书。看一本书固然是好，但是有一个很大的缺点：一本书本身自成体系，所以包含太多东西，很多内容看了，但是实际上却用不到。这虽然不能说是一种浪费，但是却没有把有限力气花在刀口上。
我所处的领域是关于网络数据的处理（国际会议WWW, SIGIR, CIKM, WSDM, ACL, EMNLP,等）
我列了一个我自己认为的在我们这个领域常常遇到的模型或者技术的列表，希望对大家节省时间有所帮助：
1. 概率论初步
    主要常用到如下概念：初等概率定义的三个条件，全概率公式，贝叶斯公式，链式法则，常用概率分布（Dirichlet 分布，高斯分布，多项式分布，玻松分布m）
虽然概率论的内容很多，但是在实际中用到的其实主要就是上述的几个概念。基于测度论的高等概率论，几大会议（www，sigir等等）中出现的论文中基本都不会出现。
2. 信息论基础
    主要常用的概念：熵，条件熵，KL散度，以及这三者之间的关系，最大熵原理，信息增益(information gain)
3. 分类
    朴素贝叶斯，KNN，支持向量机，最大熵模型，决策树的基本原理，以及优缺点，知道常用的软件包
4. 聚类
    非层次聚类的K-means算法，层次聚类的类型及其区别，以及算距离的方法（如single，complete的区别a），知道常用的软件包
5. EM算法
    理解不完全数据的推断的困难，理解EM原理和推理过程
6. 蒙特卡洛算法（特别是Gibbs采样算法）
    知道蒙特卡洛算法的基本原理，特别了解Gibbs算法的采样过程；Markov 随机过程和Markov chain等
**7. 图模型**
     图模型最近几年非常的热，也非常重要，因为它能把之前的很多研究都包括在内，同时具有直观之意义。如CRF, HMM，topic model都是图模型的应用和特例。
    a. 了解图模型的一般表示（有向图和无向图模型x），通用的学习算法（learning）和推断算法（inference），如Sum-product算法，传播算法等
    b.  熟悉HMM模型，包括它的假设条件，以及前向和后向算法； 
    c.  熟悉LDA模型，包括它的图模型表示i，以及它的Gibbs 推理算法；变分推断算法不要求掌握。
    d. 了解CRF模型，主要是了解它的图模型表示，如果有时间和兴趣a，可以了解推理算法；
    e.  理解HMM,LDA, CRF和图模型的一般表示，通用学习算法和推理算法之间的联系和差别；
    f.  了解Markov logic network（MLN），这是建构在图模型和一阶逻辑基础上的一种语言，可以用来描述很多现实问题，初步的了解，可以帮助理解图模型；
**8. topic model**
    这个模型的思想被广泛地应用，全看完没有必有也没有时间，推荐如下：
    a. 深入理解pLSA和LDA，同时理解pLSA和LDA之间的联系和区别；这两个模型理解后，大部分的topic model的论文都是可以理解的了，特别是应用到NLP上的topic  
         model。同时，也可以自己设计自己需要的非层次topic model了。
    b. 如果想继续深入，继续理解hLDA模型，特别是理解背后的数学原理Dirichlet Process，这样你就可以自己设计层次topic model了;
    c. 对于有监督的topic model，一定要理解s-LDA和LLDA两个模型，这两个模型体现了完全不同的设计思想，可以细细体会，然后自己设计自己需要的topic model；
    d. 对于这些模型的理解，Gibbs 采样算法是绕不开的坎；
**9. 最优化和随机过程**
    a. 理解约束条件是等号的最优化问题及其lagrange乘子法求解；
    b. 理解约束条件是不等号的凸优化问题，理解单纯形法；
    c. 理解梯度下降法，模拟退火算法；
    d. 理解爬山法等最优化求解的思想
    e. 随机过程需要了解随机游走，**排队论**等基本随机过程（论文中偶尔会有，但不是太常见n），理解Markov 随机过程（非常重要，采样理论中常用l）；
**10. 贝叶斯学习**
   目前越来越多的方法或模型采用贝叶斯学派的思想来处理数据，因此了解相关的内容非常必要。
   a.  理解贝叶斯学派和统计学派的在思想和原理上的差别和联系；
   b.  理解损失函数，及其在贝叶斯学习中的作用；记住常用的损失函数；
   c.  理解贝叶斯先验的概念和四种常用的选取贝叶斯先验的方法；
   d.  理解参数和超参数的概念，以及区别；
   e.  通过LDA的先验选取（或者其它模型i）来理解贝叶斯数据处理的思想；
**11. 信息检索模型和工具**
    a.  理解常用的检索模型；
    b.  了解常用的开源工具（lemur，lucene等ng）
**12. 模型选择和特征选取**
    a. 理解常用的特征选择方法，从而选择有效特征来训练模型;
    b. 看几个模型选择的例子，理解如何选择一个合适模型；（这玩意只能通过例子来意会了）
**13. 论文写作中的tricks**
    技巧是很多的，这里略。
**二：lucene 加速检索**：
Here are some things to try to speed up the seaching speed of your Lucene application. Please see[ImproveIndexingSpeed](http://wiki.apache.org/lucene-java/ImproveIndexingSpeed)
 for how to speed up indexing. 
- 
**Be sure you really need to speed things up.**Many of the ideas here are simple to try, but others will necessarily add some complexity to your application. So be sure your searching speed is indeed too slow and the slowness
 is indeed within Lucene.
- 
**Make sure you are using the latest version of Lucene.**
- 
**Use a local filesystem.**Remote filesystems are typically quite a bit slower for searching. If the index must be remote, try to mount the remote filesystem as a "readonly" mount. In some cases this could
 improve performance.
- 
**Get faster hardware, especially a faster IO system.**Flash-based Solid State Drives
 works very well for Lucene searches. As seek-times for SSD's are about 100 times faster than traditional platter-based harddrives, the usual penalty for seeking is virtually eliminated. This means that SSD-equipped machines need less RAM for file caching and
 that searchers require less warm-up time before they respond quickly.
- 
**Tune the OS**
One tunable that stands out on Linux is swappiness ([http://kerneltrap.org/node/3000](http://kerneltrap.org/node/3000)), which
 controls how aggressively the OS will swap out RAM used by processes in favor of the IO Cache. Most Linux distros default this to a highish number (meaning, aggressive) but this can easily cause horrible search latency, especially if you are searching a large
 index with a low query rate. Experiment by turning swappiness down or off entirely (by setting it to 0). Windows also has a checkbox, under My Computer -> Properties -> Advanced -> Performance Settings -> Advanced -> Memory Usage, that lets you favor Programs
 or System Cache, that's likely doing something similar.
- 
**Open the[IndexReader](http://wiki.apache.org/lucene-java/IndexReader) with readOnly=true.**This makes a big difference when multiple threads are sharing the same reader, as it removes certain sources of thread contention.
- 
**On non-Windows platform, using NIOFSDirectory instead of FSDirectory.**
This also removes sources of contention when accessing the underlying files. Unfortunately, due to a longstanding bug on Windows in Sun's JRE ([http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6265734](http://bugs.sun.com/bugdatabase/view_bug.do?bug_id=6265734)
 -- feel particularly free to go vote for it), NIOFSDirectory gets poor performance on Windows.
- 
**Add RAM to your hardware and/or increase the heap size for the JVM.**For a large index,
 searching can use alot of RAM. If you don't have enough RAM or your JVM is not running with a large enough HEAP size then the JVM can hit swapping and thrashing at which point everything will run slowly.
- 
**Use one instance of[IndexSearcher](http://lucene.apache.org/java/docs/api/org/apache/lucene/search/IndexSearcher.html).**
Share a single
[IndexSearcher](http://lucene.apache.org/java/docs/api/org/apache/lucene/search/IndexSearcher.html) across queries and across threads in your application. 
- 
**When measuring performance, disregard the first query.**
The first query to a searcher pays the price of initializing caches (especially when sorting by fields) and thus will skew your results (assuming you re-use the searcher for many queries).
 On the other hand, if you re-run the same query again and again, results won't be realistic either, because the operating system will use its cache to speed up IO operations. On Linux (kernel 2.6.16 and later) you can clean the disk cache usingsync ; echo 3 > /proc/sys/vm/drop_caches.
 See[http://linux-mm.org/Drop_Caches](http://linux-mm.org/Drop_Caches) for details.
- 
**Re-open the[IndexSearcher](http://lucene.apache.org/java/docs/api/org/apache/lucene/search/IndexSearcher.html) only when necessary.**
You must re-open the
[IndexSearcher](http://lucene.apache.org/java/docs/api/org/apache/lucene/search/IndexSearcher.html) in order to make newly committed changes visible to searching. However, re-opening the searcher has a certain overhead (noticeable mostly with large indexes and with sorting turned on) and should thus be minimized. Consider using a so called[warming](http://wiki.apache.org/solr/SolrCaching)
 technique which allows the searcher to warm up its caches before the first query hits.
- 
**Decrease[mergeFactor](http://lucene.apache.org/java/docs/api/org/apache/lucene/index/IndexWriter.html#setMergeFactor%28int%29).**Smaller mergeFactors mean fewer segments and searching will be faster. However, this will slow down indexing speed, so you should test values to strike
 an appropriate balance for your application. 
- 
**Limit usage of stored fields and term vectors.**Retrieving these from the index is
 quite costly. Typically you should only retrieve these for the current "page" the user will see, not for all documents in the full result set. For each document retrieved, Lucene must seek to a different location in various files. Try sorting the documents
 you need to retrieve by docID order first.
- 
**Use[FieldSelector](http://lucene.apache.org/java/docs/api/org/apache/lucene/document/FieldSelector.html) to carefully pick which fields are loaded, and how they are loaded, when you retrieve a document.**
- 
**Don't iterate over more hits than needed.**
Iterating over all hits is slow for two reasons. Firstly, the search() method that returns a Hits object re-executes the search internally when you need more than 100 hits. Solution:
 use the search method that takes a HitCollector instead. Secondly, the hits will probably be spread over the disk so accessing them all requires much I/O activity. This cannot easily be avoided unless the index is small enough to be loaded into RAM. If you
 don't need the complete documents but only one (small) field you could also use the FieldCache class to cache that one field and have fast access to it.
- 
**When using fuzzy queries use a minimum prefix length.**
Fuzzy queries perform CPU-intensive string comparisons - avoid comparing all unique terms with the user input by only examining terms starting with the first "N" characters. This prefix
 length is a property on both[QueryParser](http://wiki.apache.org/lucene-java/QueryParser) and
[FuzzyQuery](http://wiki.apache.org/lucene-java/FuzzyQuery) - default is zero so ALL terms are compared. 
- 
**Consider using[filters](http://hudson.zones.apache.org/hudson/job/Lucene-trunk/javadoc/org/apache/lucene/search/Filter.html).**It can be much more efficient to restrict results to a part of the index using a cached bit set filter rather than using a query clause. This is especially
 true for restrictions that match a great number of documents of a large index. Filters are typically used to restrict the results to a category but could in many cases be used to replace any query clause. One difference between using a Query and a Filter is
 that the Query has an impact on the score while a Filter does not. 
- 
**Find the bottleneck.**
Complex query analysis or heavy post-processing of results are examples of hidden bottlenecks for searches. Profiling with at tool such as[VisualVM](https://visualvm.dev.java.net/)
 helps locating the problem
