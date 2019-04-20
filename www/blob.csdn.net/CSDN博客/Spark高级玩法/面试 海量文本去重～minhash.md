# 面试|海量文本去重～minhash - Spark高级玩法 - CSDN博客
2018年12月23日 00:00:32[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：87
在实际应用的过程中。相似性度量和计算是很经常使用的一个方法。比如网页去重、推断帖子是否相似、推荐系统衡量物品或者用户的相似度等等。当数据量大的时候，计算的时间和空间复杂度就会是一个很重要的问题，比如在推断相似发帖的时候。我们能够用kmeans来进行聚类。可是资源的消耗是巨大的。所以本文推荐一种方法，minhash+lsh（局部敏感hash），用minhash来降维。用lsh来做近似查询，本文主要介绍一下minhash。
在介绍minhash之前，先给出相似性的度量方法。
**1. 相似性的度量**
相似性度量有非常多方法，欧氏距离是比較经常使用的。这里我们用一下Jaccard相似性系数，公式例如以下
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFV3kuEUmbx8vWBjv4oIJogia6JZNUflZTPicjZQ9yTxGH4PIczAIvwUukOWJGIsSKay8B4249Hv4q7g/640)
计算方法非常easy。文档A和文档B共同拥有的单词数除以A和B单词的集合。比如A={a,b,c,d}，B={c,d,e,f}，那么相似性系数就是2/6=0.33。
**2. minhash**
刚才我们知道在求相似度的时候我们用到了文档和单词。通常情况下，我们都会将文档和单词表示成doc-term矩阵的形式，能够看到term详细的是什么对最后的结果没有不论什么影响。所以我索性用行号来代表term，行号跟term是一一相应的。比如
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFV3kuEUmbx8vWBjv4oIJogiaiay39kxa0LojpF4fzDV7A3ia9w6AnfSvzyj89ex8HE6Ria0pibY7tmbzibg/640)
第一行中的S1,、S2、S3表示文档，第一列的01234表示行号。也即单词。其它部分1表示文档S中有这个单词，0表示没有这个单词，有了这个集合，我们看一下minhash是怎么做的
随机确定一个顺序。比如上面的顺序是01234。随机确定一个顺序，比如12340。注意这里是随机。目的就是不让最后的结果受人为的干扰。结果例如以下
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFV3kuEUmbx8vWBjv4oIJogiapeERYN3iafZfUiaLCFGibXGuq1zsw0H4ZScfr2k9dj6eGIrrUwL18iasfA/640)
我们看到，行号是不变的，行号还是那个行号，变化的是矩阵的内容。找到排在最前面的1代表这个文档，比如S1排在最前面的1行号为2，那么就用2代表文档S1，同理，用1代表S2，那么能够计算S1和S2的相似性系数了，1交2除以1并2等于0。
后面会给出为什么用这样的方法是合理的证明。我们临时先跳过。能够想象一下，用一个单词来代表一个文档偶然性会比較大，那么这个时候我们的想法可能是，能够随机的产生多次变换，取出多个单词来进行比較。这个时候问题就来了，在实际应用的过程中，文档可能有几百万，单词也会有几万，对如此庞大的矩阵做变换时间和空间的代价都会比較大。是不是有别的方法呢，答案是肯定的，我们知道运动是相对的。之前是变换矩阵内容不变行号。我们如今不变矩阵，仅仅变换行号，是不是计算量少了许多。
所以问题转换为怎样产生随机的行号，我们能够用hash函数来产生行号的顺序，两个函数能够自定义。最好保证hash后的值均匀。比如x+1mod5，3x+1mod5。我们选用这两个hash函数来产生行号的顺序。看一下我们如今的情况
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFV3kuEUmbx8vWBjv4oIJogiaYSeK0EEX8d9LdeibHeBhdtWOKQhQb6piawKiaLPJt7W3nw06PuWujMtzQ/640)
我们用h1、h2两个hash函数产生了两个行号顺序，那么接下来就是关键步骤了
比如求文档s1的值。遍历s1相应的单词
从第0行到第四行
1. 第0行为1，看一下h1计算出来的行号为1。赋值h1为1（就是行号）。继续遍历
2. 第1行为0，不关心，跳过
3. 第2行为0，不关心。跳过
4. 第3行为1， 看一下h1计算出来的行号为4。4大于此时h1的值，h1的值不变。假设小于h1此时的值，将值付给h1
5. 第4行为0。不关心，跳过
遍历完了之后此时h1的值就是1，能够看到。我们事实上在做的就是遍历矩阵中的值，对0的不关心。跳过。对1的。看一下hash函数产生的行号，找到行号最小的值作为h1输出的值。同理，h2也一样，最后得到例如以下的矩阵
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFV3kuEUmbx8vWBjv4oIJogiaiaWnXiabI7pEkI3cGzWxRyCgtYSP0QXfbfFgmO3kQ0eDpLd9Hibz4AEnw/640)
这个时候就能够计算s1、s2的相似度了，J=0/3=0
**3. 为什么minhash的方法是合理的**
问题：两个集合的随机的一个行排列的minhash值相等的概率和两个集合的Jaccard相似度相等
证明例如以下：
两个集合。A、B。对一行来说。他们的状态有三种
X：A、B都为1，即表示A、B集合中都有这个单词
Y：A、B当中一个为1，当中一个不为1，即一个有这个单词，一个没有
Z：A、B都为0，即表示A、B中都没有这个单词。
如果有x行为X，y行为Y，z行为z。这是jaccard系数为x/(x+y)。再看minhash，由于排列是随机的，在遇到Y之前遇到X的概率是x/(x+y)。是不是正好等于jaccard系数的值。
**4. 怎样进行相似查询比較**
通过前面的方法。我们将文档进行了压缩，比如使用了30个hash函数。那么就将一篇文档压缩成了30位表示。接下来的问题是怎样进行查询。
一种思路是：建立倒排，term是一个单词，doclist就是拥有这个单词的其它文档。
还有一种思路是：不是建立单个单词的倒排，而是建立多个单词的联合倒排，比如
一篇文档：通过前面的方式用30位进行了表示，将这30为进行分成m个桶，每桶r个单词，即m*r=30，这个倒排建立的是：term是r个单词（或者将这r个单词求hashcode），doclist就是拥有这r个单词的文档。
那么这里的问题就是。m、r怎样求解，m等于几好。r等于几好。
假设两个文档相似度为p，那么相应位数相似的概率也是p，那么一个桶里全然同样的概率是p^r，不同样的概率是1-p^r，那么m个桶都不同样的概率是(1-p^r)^m。所以至少有一个桶同样的概率是1-(1-p^r)^m，我们能够依据我们想要的概率p去分配m和r。
最后建立倒排是这种。
桶1——>doc1，doc2，doc3，doc4
桶2——>doc2，doc5，doc9，doc10
索引建立完毕了之后，下一步就是检索，一篇新的文档。也要经过全面的步骤，得到对应的桶。比如桶1，桶3，那么接下来就是用桶1查询，得到跟这篇文档相似的文档。为了保证确实相似。还能够对候选文档计算一下跟本片文档的相似度
推荐阅读：
[推荐两个不错的flink项目](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247485652&idx=1&sn=170ece64334979fc5ea5fc5628a33759&chksm=9f38ebfca84f62ea0c1aaaeb8410ec15aec31b1122558662175aa70a2d953f9a3f0e6e904c8f&scene=21#wechat_redirect)
[推荐系统系列之隐语义模型](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484733&idx=1&sn=44e69d3e5cb7c3d818689c9c0621b470&chksm=9f38e615a84f6f03e175052eabfc2cfea7baca091b80cce2c45b49eebd27ae99d754d52648ea&scene=21#wechat_redirect)
[面试|海量文本去重~simhash](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486127&idx=1&sn=08596f039575bc5f421a58c638ac0525&chksm=9f38e987a84f6091c61fb46e38b6e2f5345d37ceafb37327eb13102c32a6eba60b26548eb0fb&scene=21#wechat_redirect)
[leetcode第一题判断链表是否有环](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247486137&idx=1&sn=7b4dea0da220838facc51e376bdd4940&chksm=9f38e991a84f6087ccd927139a7f2036e93b9a4853cdba9301cb13b349899d71b7cba2c4437d&scene=21#wechat_redirect)
![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFUibDu2gaX89M600Ip3MDrVY0ibqsWg1gHuTdGCX1QfbyyCPlf31gLQna9OvNyRsBaByXsZrZeqibtVQ/640)
