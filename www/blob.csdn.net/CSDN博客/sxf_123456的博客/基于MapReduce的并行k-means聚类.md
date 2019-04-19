# 基于MapReduce的并行k-means聚类 - sxf_123456的博客 - CSDN博客
2018年01月07日 13:30:06[sxf_0123](https://me.csdn.net/sxf_123456)阅读数：365
http://blog.csdn.net/baidu_35570545/article/details/72840734
**摘要**：在许多应用上，数据聚类已经受到了广泛的关注，比如数据挖掘、文本检索、图像分割和模式识别等。随着科技进步而逐渐扩大的信息量使大数据的聚类变成了一个具有挑战性的任务。为了解决这个问题，许多调查研究者尝试去设计一种高效的并行聚类算法。在这篇文章中，我们提出一种基于MapReduce的并行k-means聚类算法，这是一种简单又强大的并行编程技术。实验结果表明所提出的算法可以大规模而且高效地在廉价的硬件上处理大型数据集。 
**关键字**：数据挖掘；并行聚类；k-means；Hadoop；MapReduce
# 1 介绍
  随着信息技术的发展，许多应用处理的数据规模会达到千兆级别，而这会自然而然地对计算提出更高的要求。高效的并行聚类算法和运行技术是满足科学数据分析的可扩展性以及性能要求的关键。到目前为止，一些研究者已经提出了一些并行聚类算法。所有的这些并行聚类算法都有下述的缺点：a）他们假设所有对象都能在主存中同时存放；b）这些并行系统提供了有限的编程模型，并使用这种限制去自动并行计算。以上两者在面对拥有成千上万对象的大规模数据集时会望而却步。因此，以并行聚类算法为方向的数据集需要得到发展。 
  MapReduce是一种编程模型，用于处理和生成适用于各种现实世界任务的大型数据集的关联实施。用户指定map和reduce函数的计算过程，底层的运行系统便可以自动地在大规模集群上并行计算，可以自动地处理机器的错误，可以协调好中间机器的交互以至于高效地利用网络和磁盘资源。Google和Hadoop都提供了MapReduce运行时容错和动态灵活性的支持。 
  在这篇文章中，我们把k-means算法改编到MapReduce框架下，该框架是在Hadoop下执行的，目的是为了使聚类方法变得可行。通过采用合适的
# 2 基于MapReduce的并行K-Means算法
  在这节中，我们展示了基于MapReduce的并行K-Means（PKMeans）的主要设计。首先，我们简短的回顾K-Means算法，并分析了可并行化和可串行化的部分。然后，我们在细节上解释形式化map和reduce操作所必要的计算。
## 2.1 K-Means 算法
  K-Means算法是最著名、最广泛使用的聚类算法。它用输入参数k把n个对象集合分成了k份，目的是让簇内的相似度高而簇间的相似度低。簇的相似度可以根据簇内对象到簇中心距离的平均值来衡量。 
  算法运行如下：首先随机从所有对象中选择k个对象，这k个对象代表初始聚类的中心。每个剩下的对象会基于对象到与簇中心的距离被分配到最相似的簇。然后计算得到每个簇新的均值。这个过程需要不断迭代直到标准函数收敛。 
在k-means算法中，最密集的计算发生在距离的计算上。在每个迭代的过程中，需要n*k个距离的计算，其中n为对象总数，k为簇的总数。很明显，一个对象与中心距离计算跟其它对象与中心距离计算是无关的。因此，不同对象到中心距离的计算可以并行执行。每个迭代中，用来进行下一轮迭代计算的新中心需要更新，所以迭代过程必须串行执行。
## 2.2 基于MapReduce的PKMeans
  如上文分析，PKMeans算法需要一种MapReduce job。map函数分配每个样本到最近的中心，reduce函数负责聚类中心的更新。为了减少网络负责，需要combiner函数来处理同一个map同一个key的中间结果的部分合并。 
**Map函数.** 输入数据集存储在分布式文件系统HDFS中，作为`<key,value>`的序列文件，每个`<key,value>`代表数据集的一条记录。Key为该记录对应于数据文件起始位置的偏移量，value为该条记录的内容。数据集被分割并传给所有的map，因此距离的计算就会并行执行。对于每个map任务，PKMeans构造了一个全局变量centers，centers是一个包含所有聚类中心信息的数组。如果给定该信息，map函数就可以计算判断某个样本到哪个中心最近。然后，中间结果由两部分组成：最近中心的索引和样本信息。Map函数的伪代码见算法1。
**算法1.**  map(*key*,*value*)
输入：全局变量*centers*，偏移量*key*，样本*value*
输出：<*key’*,*value*>对，其中*key’*是最近中心的索引，*value’*是样本信息的字符串
- 从*value*构造样本的*instance*；
- *minDis*=*Double*.*MAX_VALUE*；
- *Index*=-1；
- For i=0 to *centers*.length do 
*dis*=*ComputeDist*(*instance*,*centers*[i]); 
    If *dis*`<`*minDis*{ 
*minDis*=*dis*; 
*index*=*i*; 
    }
- End For
- 把*index*作为*key’*；
- 把不同维度的values构造成*value’*；
- 输出<*key’*,*value’*>对；
- End
注意这里的Step 2和Step 3初始化了辅助变量*minDis*和*index*；Step 4通过计算找出了与样本最近的中心点，函数*ComputeDist*(*instance*,*centers*[i])返回样本和中心点centers[i]的距离；Step 8输出了用来进行下一个过程（combiner）的中间数据。 
**Combine函数.** 每个map任务完成之后，我们用combiner去合并同一个map任务的中间结果。因为中间结果是存储在结点的本地磁盘上，所以这个过程不会耗费网络传输的代价。在combine函数中，我们把属于相同簇的values求和。为了计算每个簇的对象的平均值，我们需要记录每个map的每个簇中样本的总数。Combine函数的伪代码见算法2.
**算法2.** combine(*key*,*V*)
输入：*key*为簇的索引，*V*为属于该簇的样本列表 
输出：<*key’*,*value’*>对，*key’*为簇的索引，*value’*是由属于同一类的所有样本总和以及样本数所组成的字符串。
- 初始化一个数组，用来记录同一类的所有样本的每个维度的总和，样本是*V*中的元素；
- 初始化一个计数器*num*为0来记录属于同一类的样本总数；
- While(*V*.hasNext()){ 
  从*V*.next()构造样本实例*instance*； 
  把*instance*的不同维度值相加到数组 
*num*++;
- }
- 把*key*作为*key’*；
- 构造*value’*：不同维度的求和结果+*num*；
- 输出<*key’*,*value’*>对；
- End
**Reduce函数.** Reduce函数的输入数据由每个结点的combine函数获得。如combine函数所描述，输入数据包括部分样本（同一类）的求和以及对应样本数。在reduce函数中，我们可以把同一类的所有样本求和并且计算出对应的样本数。因此，我们可以得到用于下一轮迭代的新中心。Reduce函数的伪代码见算法3。
**算法3.** Reduce(key,V)
输入：key为簇的索引，V为来自不同结点的部分总和的样本列表 
输出：<*key’*,*value’*>对，*key’*为簇的索引，*value’*是代表新的聚类中心的字符串
- 初始化一个数组，用来记录同一类的所有样本的每个维度的总和，样本是*V*中的元素；
- 初始化一个计数器*NUM*为0来记录属于同一类的样本总数；
- While(*V*.hasNext()){ 
  从*V*.next()构造样本实例*instance*； 
  把*instance*的不同维度值相加到数组 
*NUM*+=*num*;
- }
- 数组的每个元素除以NUM来获得新的中心坐标；
- 把*key*作为*key’*；
- 构造*value’*为所有中心坐标的字符串；
- 输出<*key’*,*value’*>对；
- End
# 3 实验结果
  在这节中，我们针对speedup、scaleup、sizeup来评估所提出的算法性能。性能的实验是在计算机集群上运行的，每个结点有2.8GHz核心和4GB内存。所有的实验中，MapReduce系统上的Hadoop版本为0.17.0，Java 版本为1.5.0_14。 
![Speedup](https://img-blog.csdn.net/20170602073414059?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzU1NzA1NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（a）Speedup 
![Scaleup](https://img-blog.csdn.net/20170602073453543?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzU1NzA1NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（b）Scaleup 
![Sizeup](https://img-blog.csdn.net/20170602073519637?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYmFpZHVfMzU1NzA1NDU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
（c）Sizeup 
图1 评估结果 
  为了衡量speedup，我们保持数据集不变，增加系统的结点数。完美的并行算法的speedup是线性的：具有m倍结点数的系统的speedup为m。然而，线性的speedup是很难实现的，因为传输代价会随着结点数的增加而增加。   我们用了不同的数据集大小和系统评估了speedup。结点数量从1到4。数据集的大小从1GB增加到8GB。图1（a）展示了不同数据集的speedup。如结果所示，PKMeans具有非常好的speedup性能。特别是当数据集大小增加时，speedup表现得更加好。因此，PKMeans算法可以有效对付大型数据集。
   Scaleup评估了算法在同时增加系统结点和数据集大小的运行性能。Scaleup被定义为：在与原始系统相同的运行时间内，m倍系统执行m倍工作的能力。   为了描述PKMeans算法如何能够在更多结点可用的情况下处理得好更大的数据集，我们做了scaleup评估实验，该实验直接按照增加结点数的比例增加数据集的大小。数据集的大小1GB、2GB、3GB和4GB分别执行在1、2、3和4个结点上。图1（b）为性能展示。可以清楚地看到，PKMeans算法scaleup性能非常好。   Sizeup的分析需要保持系统结点数不变，以倍数m增加数据集的大小。Sizeup用来衡量当给定m倍大的原始数据集时需要运行多长时间。
   为了衡量sizeup的性能，我们分别固定结点数为1、2、3和4。图1（c）展示了不同结点数下的sizeup结果。图中表明PKMeans具有非常好的sizeup性能。
# 4 结论
  由于数据聚类吸引了一大批研究者目光的焦点，许多聚类算法在过去的十年都被纷纷提出。然而，逐渐扩大的应用数据使得聚类变成了一个极具挑战性的任务。在这篇文章中，我们提出了一种基于MapReduce的快速并行的k-means聚类算法，该算法已经在学术界和工业界得到广泛认可。我们用speedup、scaleup和sizeup来评估所提出算法的性能。结果表明，该算法可以在廉价机器上有效处理大型数据集。
翻译原文：
Zhao W, Ma H, He Q. Parallel K-Means Clustering Based on MapReduce [C] //Proc of International Conference on Cloud Computing.2009:674-679.
