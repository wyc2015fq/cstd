# [转]高压缩空间占用的 Hyper LogLog 算法 - z69183787的专栏 - CSDN博客
2017年01月22日 11:29:08[OkidoGreen](https://me.csdn.net/z69183787)阅读数：314
个人分类：[Redis-基础](https://blog.csdn.net/z69183787/article/category/2923783)
**[大数据](http://lib.csdn.net/base/hadoop)计算：如何仅用1.5KB内存为十亿对象计数 - Hyper LogLog [算法](http://lib.csdn.net/base/datastructure)**
**Big Data Counting: How To Count A Billion Distinct Objects Using Only 1.5K**
This is a guest post by Matt Abrams (@abramsm), from Clearspring, discussing how they are able to accurately
 estimate the cardinality of sets with billions of distinct elements using surprisingly small data structures. Their servers receive well over 100 billion events per month.
在[Clearspring](http://www.google.com/url?q=http%3A%2F%2Fclearspring.com%2F&sa=D&sntz=1&usg=AFQjCNGLySXAkyxC5vAYZgsEnf9t29xzTw)，我们从事统计数据。统计一组不同元素且数量很大的数据集时，是一个挑战。
为了更好地理解已经明确基数的大数据集的挑战，我们假设你的日志文件包含16个字符的ID,并且你想统计不同ID的数量.例如:
4f67bfc603106cb2
这16个字符需要用128位来表示。6万5千个ID将需要1MB的空间。我们每天收到30多亿条事件记录，每条记录都有一个ID。这些ID需要3840亿位或45GB的存储。而这仅仅是ID字段需要的空间。我们采取一种简单的方法获取日常事件记录中以ID为基数的数据。最简单的办法就是使用哈希集合且存放到内存中，其中哈希集包含唯一ID的列表（即输入文件中可能会有多条记录的id是相同，但在哈希集中只存放一条）。即使我们假设只有1/3的条记录ID是唯一的（即2/3的记录ID是重复的），哈希集仍需要119GB的RAM，这其中不包括[Java](http://lib.csdn.net/base/javase)需要在内存中存储对象的开销。你需要一台配备几百GB内存的机器来计算不同的元素，并且这只是计算一天内日志事件记录的唯一ID的内存消耗。如果我们想要统计数周或数月的数据，这问题只会变得更加困难。我们身边当然不会有一台配备几百GB内存的空闲机器，所以我们需要一个更好的解决方案。
解决这一问题的常见办法是使用[位图](http://blog.csdn.net/hguisu/article/details/7880288)（博客：[海量数据处理算法—Bit-Map](http://blog.csdn.net/hguisu/article/details/7880288)）。位图可以快速、准确地获取一个给定输入的基数。位图的基本思想是使用哈希函数把数据集映射到一个bit位，每个输入元素与bit位是一一对应。这样Hash将没有产生碰撞冲突，并减少需要计算每个元素映射到1个bit的空间。虽然Bit-map大大节省了存储空间，但当统计很高的基数或非常大的不同的数据集，它们仍然有问题。例如，如果我们想要使用Bit-map计数十亿，你将需要Bit-map位，或需要每个约120
 MB的计数器。稀疏的位图可以被压缩，以获得更多的空间效率，但也并不总是有帮助的。
幸运的是，[基数估计](http://www.google.com/url?q=http%3A%2F%2Falgo.inria.fr%2Fflajolet%2FPublications%2FDuFl03.pdf&sa=D&sntz=1&usg=AFQjCNHij4lkOvtFkqLQ7BwDCOX3DHx2IQ)是一个热门的[研究](http://www.google.com/url?q=http%3A%2F%2Falgo.inria.fr%2Fflajolet%2FPublications%2FDuFl03-LNCS.pdf&sa=D&sntz=1&usg=AFQjCNHRJ_TiQ8JpEXwjPibzmMkzWmWXsg)领域。我们已经利用这项研究提供了一个开源实现的基数估计、集合元素检测和top-k算法。
基数估计算法就是使用准确性换取空间。为了说明这一点，我们用三种不同的计算方法统计所有莎士比亚作品中不同单词的数量。请注意，我们的输入数据集增加了额外的数据以致比问题的参考基数更高。这三种技术是：[Java](http://lib.csdn.net/base/java)HashSet、Linear
 Probabilistic Counter以及一个Hyper LogLog Counter。结果如下：
![](https://img-my.csdn.net/uploads/201212/25/1356428056_9055.png)
该表显示，我们统计这些单词只用了512 bytes，而误差在3%以内。相比之下，HashMap的计数准确度最高，但需要近10MB的空间，你可以很容易地看到为什么基数估计是有用的。在实际应用中准确性并不是很重要的，这是事实，在大多数网络规模和网络计算的情况下，用概率计数器会节省巨大的空间。
**线性概率计数器**
线性概率计数器是高效的使用空间，并且允许实现者指定所需的精度水平。该算法在注重空间效率时是很有用的，但你需要能够控制结果的误差。该算法分两步运行：**第一步，**首先在内存中分配一个初始化为都为0的Bit-map，然后使用哈希函数对输入数据中的每个条目进行hash计算，哈希函数运算的结果是将每条记录（或者是元素）映射到Bit-map的一个Bit位上，该Bit位被置为1；**第二步，**算法计算空的bit位数量，并使用这个数输入到下面的公式来进行估算：
n=-m ln Vn
**注意:ln Vn=Loge(Vn) 自然对数**
在公式中，**m**是 Bit-map的大小，Vn是空bit位和map的大小的比率。需要重点注意的是原始Bit-map的大小，可以远小于预期的最大基数。到底小多少取决于你可以承受误差的大小。因为Bit-map的大小**m**小于不同元素的总数将会产生碰撞。虽然碰撞可以节省空间，但同时也造成了估算结果出现误差。所以通过控制原始map的大小，我们可以估算碰撞的次数，以致我们将在最终结果中看到误差有多大。
**Hyper LogLog**
顾名思义，Hyper LogLog计数器就是估算Nmax为基数的数据集仅需使用loglog(Nmax)+O(1) bits就可以。如线性计数器的Hyper LogLog计数器允许设计人员指定所需的精度值，在Hyper
 LogLog的情况下，这是通过定义所需的相对标准差和预期要计数的最大基数。大部分计数器通过一个输入数据流**M**，并应用一个哈希函数设置**h(M)**来工作。这将产生一个S = h(M) of {0,1}^∞字符串的可观测结果。通过分割哈希输入流成m个子字符串，并对每个子输入流保持m的值可观测 ，这就是相当一个新Hyper LogLog（一个子m就是一个新的Hyper
 LogLog）。利用额外的观测值的平均值，产生一个计数器，其精度随着m的增长而提高，这只需要对输入集合中的每个元素执行几步操作就可以完成。其结果是，这个计数器可以仅使用1.5 kb的空间计算精度为2%的十亿个不同的数据元素。与执行 HashSet所需的120 兆字节进行比较，这种算法的效率很明显。
**合并分布式计数器**
我们已经证明了使用上面描述的计数器我们可以估算大集合的基数。但是，如果你的原始输入数据集不适合于单台机器，将怎么做呢？这正是我们在[Clearspring](http://www.google.com/url?q=http%3A%2F%2Fclearspring.com%2F&sa=D&sntz=1&usg=AFQjCNGLySXAkyxC5vAYZgsEnf9t29xzTw)所面临的问题。我们的数据分散在数百台服务器上，并且每个服务器只包含整个数据集子集的一部分。这事实上我们能合并一组分布式计数器的内容是至关重要的。这个想法有点令人费解，但如果你花费一些时间去思考这个问题，就会发现其与基本的基数估计值相比并没有太大的不同。因为这个计数器表示映射中的位作为基数，我们可以采取两个兼容计数器并将他们bit位合并到单一的map上。这个算法已经处理碰撞，所以我们可以得到一个基数估计所需的精密，即使我们从来没有把所有的输入数据到一台机器。这是非常有用的，节省了我们在网络中移动数据的大量时间和精力。
**Next Steps**
希望这篇文章能帮助你更好地理解这个概念和概率计数器的应用。如果估算大集合的基数是一个问题，而你又碰巧使用一个基于JVM的语言，那么你应该使用stream-lib项目——它提供了其他几个流处理工具以及上文所述的算法的实现。
本文来自：[High Scalability](http://highscalability.com/blog/2012/4/5/big-data-counting-how-to-count-a-billion-distinct-objects-us.html)
若深入了解**Hyper LogLog：**
[http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf](http://algo.inria.fr/flajolet/Publications/FlFuGaMe07.pdf)
[http://www.ic.unicamp.br/~celio/peer2peer/math/bitmap-algorithms/durand03loglog.pdf](http://www.ic.unicamp.br/~celio/peer2peer/math/bitmap-algorithms/durand03loglog.pdf)
文章来源：[http://blog.csdn.net/hguisu/article/details/8433731](http://blog.csdn.net/hguisu/article/details/8433731)
=================================================================================================
## Hyperloglog算法浅说
这个[算法](http://www.java123.net/v/list-192-1.html)的目的：
比如给你一个数组，int a[]={1,1,2,6,9,8,5,4,1,2}
这个数组里一共有十个元素，其中distinct的数一共有7个，它们是1,2,4,5,6,8,9
这个算法就是判断输入流中互不相同的元素一共有多少个。
这个算法是概率算法，但是它的精确度很高，以下是它的描述和实现细节
我们首先需要以下几个辅助函数或者数据
1，int hash(type input);//将输入的元素hash成一个32bit的整数，输入可能是整数，也可能是字符串，甚至是结构体，etc
2，unsigned int position(int input);//返回input的二进制表示中，从左往右数，第一个1出现的位置
比如
position(1000000100000111110)=1
position(0001111000011100000)=4
position(000000)=7
3，m=2^b,其中b在[4,16]之间
4，几个常数
const double a16=0.673
const double a32=0.697
const double a64=0.709
const double am=0.7213/(1+1.079/m)  (m>=128)
有了这四个准备之后，我们就可以开始用hyperloglog来实现计数了
m=2^b个计数器，M[1]到M[m]都初始化为0
for(v=input)
{
           x=hash(v);
           j=1+<x1x2...xb>(binary)
           w=x(b+1)x(b+2)....x32
            M[j]=max(M[j],position(w));
 }
res=am*m^2*S(1,m,2^(-M[j]))
来源：[http://www.java123.net/v/356202.html](http://www.java123.net/v/356202.html)
=================================================================================================
**[Redis](http://lib.csdn.net/base/redis)[数据结构](http://lib.csdn.net/base/datastructure)HyperLogLog**
如果我们要实现记录网站每天访问的独立IP数量这样的一个功能
**集合实现：**
使用集合来储存每个访客的 IP ，通过集合性质（集合中的每个元素都各不相同）来得到多个独立 IP ，
然后通过调用 SCARD 命令来得出独立 IP 的数量。
举个例子，程序可以使用以下代码来记录 2014 年 8 月 15 日，每个网站访客的 IP ：
ip = get_vistor_ip()
SADD '2014.8.15::unique::ip' ip
然后使用以下代码来获得当天的唯一 IP 数量：
SCARD '2014.8.15::unique::ip'
**集合实现的问题**
使用字符串来储存每个 IPv4 地址最多需要耗费 15 字节（格式为 'XXX.XXX.XXX.XXX' ，比如
'202.189.128.186'）。
下表给出了使用集合记录不同数量的独立 IP 时，需要耗费的内存数量：
独立 IP 数量一天一个月一年
一百万15 MB 450 MB 5.4 GB
一千万150 MB 4.5 GB 54 GB
一亿1.5 GB 45 GB 540 GB
随着集合记录的 IP 越来越多，消耗的内存也会越来越多。
另外如果要储存 IPv6 地址的话，需要的内存还会更多一些
*为了更好地解决像独立 IP 地址计算这种问题，Redis 在 2.8.9 版本添加了 HyperLogLog 结构。*
**HyperLogLog介绍**
HyperLogLog 可以接受多个元素作为输入，并给出输入元素的基数估算值：
• 基数：集合中不同元素的数量。比如 {'apple', 'banana', 'cherry', 'banana', 'apple'} 的基数就是 3 。
• 估算值：算法给出的基数并不是精确的，可能会比实际稍微多一些或者稍微少一些，但会控制在合
理的范围之内。
HyperLogLog 的优点是，即使输入元素的数量或者体积非常非常大，计算基数所需的空间总是固定
的、并且是很小的。
在 Redis 里面，每个 HyperLogLog 键只需要花费 12 KB 内存，就可以计算接近 2^64 个不同元素的基
数。这和计算基数时，元素越多耗费内存就越多的集合形成鲜明对比。
但是，因为 HyperLogLog 只会根据输入元素来计算基数，而不会储存输入元素本身，所以
HyperLogLog 不能像集合那样，返回输入的各个元素。
**将元素添加至 HyperLogLog**
PFADD key element [element ...]
将任意数量的元素添加到指定的 HyperLogLog 里面。
这个命令可能会对 HyperLogLog 进行修改，以便反映新的基数估算值，如果 HyperLogLog 的基数估算
值在命令执行之后出现了变化， 那么命令返回 1 ， 否则返回 0 。
命令的复杂度为 O(N) ，N 为被添加元素的数量。
返回给定 HyperLogLog 的基数估算值
PFCOUNT key [key ...]
当只给定一个 HyperLogLog 时，命令返回给定 HyperLogLog 的基数估算值。
当给定多个 HyperLogLog 时，命令会先对给定的 HyperLogLog 进行并集计算，得出一个合并后的
HyperLogLog ，然后返回这个合并 HyperLogLog 的基数估算值作为命令的结果（合并得出的
HyperLogLog 不会被储存，使用之后就会被删掉）。
当命令作用于单个 HyperLogLog 时， 复杂度为 O(1) ， 并且具有非常低的平均常数时间。
当命令作用于多个 HyperLogLog 时， 复杂度为 O(N) ，并且常数时间也比处理单个 HyperLogLog 时要
大得多。
**PFADD 和 PFCOUNT 的使用示例**
redis> PFADD unique::ip::counter '192.168.0.1'
(integer) 1
redis> PFADD unique::ip::counter '127.0.0.1'
(integer) 1
redis> PFADD unique::ip::counter '255.255.255.255'
(integer) 1
redis> PFCOUNT unique::ip::counter
(integer) 3
**合并多个 HyperLogLog**
PFMERGE destkey sourcekey [sourcekey ...]
将多个 HyperLogLog 合并为一个 HyperLogLog ，合并后的 HyperLogLog 的基数估算值是通过对所有
给定 HyperLogLog 进行并集计算得出的。
命令的复杂度为 O(N) ， 其中 N 为被合并的 HyperLogLog 数量， 不过这个命令的常数复杂度比较高。
PFMERGE 的使用示例
redis> PFADD str1 "apple" "banana" "cherry"
(integer) 1
redis> PFCOUNT str1
(integer) 3
redis> PFADD str2 "apple" "cherry" "durian" "mongo"
(integer) 1
redis> PFCOUNT str2
(integer) 4
redis> PFMERGE str1&2 str1 str2
OK
redis> PFCOUNT str1&2
(integer) 5
**HyperLogLog 实现独立 IP 计算功能**
独立 IP 数量一天一个月一年一年（使用集合）
一百万12 KB 360 KB 4.32 MB 5.4 GB
一千万12 KB 360 KB 4.32 MB 54 GB
一亿12 KB 360 KB 4.32 MB 540 GB
下表列出了使用 HyperLogLog 记录不同数量的独立 IP 时，需要耗费的内存数量：
可以看到，要统计相同数量的独立 IP ，HyperLogLog 所需的内存要比集合少得多。
来源：[http://www.cnblogs.com/ysuzhaixuefei/p/4052110.html](http://www.cnblogs.com/ysuzhaixuefei/p/4052110.html)
