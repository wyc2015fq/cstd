# 聊聊 StackOverflow 的标签引擎（下） - 文章 - 伯乐在线
本文由 [伯乐在线](http://blog.jobbole.com) - [赵荣](http://www.jobbole.com/members/q3612171732) 翻译，[sunshinebuel](http://www.jobbole.com/members/sunshinebuel) 校稿。未经许可，禁止转载！
英文出处：[Matt Warren](http://mattwarren.github.io/2015/10/29/the-stack-overflow-tag-engine-part-3/)。欢迎加入[翻译组](https://github.com/jobbole/translation-project)。
这是讨论如何实现 Stack Overflow 标签引擎系列的第三部分，如果之前没看过[第一篇](http://blog.jobbole.com/99910/)和[第二篇](http://blog.jobbole.com/100623/)的话，建议先阅读一下前两篇。
## 复杂的布尔查询
Stack Overflow的标签引擎有个强大的功能，它允许用户使用多个标签组合出非常复杂的查询，比如：
- [.net OR (NOT jquery)](http://stackoverflow.com/questions/tagged/.net+or+jquery-)
- [.net OR (NOT jquery) AND javascript](http://stackoverflow.com/questions/tagged/.net+or+jquery-+javascript)
一个简单的实现方法就像下面代码中那样，使用一个[HashSet](https://msdn.microsoft.com/en-us/library/bb359438(v=vs.110).aspx)来高效的判断某个问题应该被包含在搜索结果内，还是被排除在搜索结果之外。


```
var result = new List<Question>(pageSize);
var andHashSet = new HastSet<int>(queryInfo[tag2]);
foreach (var id in queryInfo[tag1]){
    if (result.Count >= pageSize)
        break;
    baseQueryCounter++;
    if (questions[id].Tags.Any(t => tagsToExclude.Contains(t)))
    {
        excludedCounter++;
    }
    else if (andHashSet.Remove(item))
    {
        if (itemsSkipped >= skip)
             result.Add(questions[item]);
        else
            itemsSkipped++;
    }
}
```
我们面临的最主要问题是需要对所有带有tag1标签的问题遍历一次，直到我们找到足够一页的匹配结果，就是foreach (var id in queryInfo[tag1])这段循环里的代码；另外我们还要将所有带有tag2标签的问题初始化到HashSet中，这样我们才能检查是否匹配。这种方法的缺点是，当skip变量很大，或者需要排除的标签列表tagsToExclude很长的时候（比如“忽略标签”），这个方法就需要更长的时间。请查看本系列[第二篇](http://blog.jobbole.com/100623/)了解更多“忽略标签”的情况。
## 位图索引(Bitmaps)
那么我们能做的更好吗？答案当然是肯定的，正好有种机制非常适合做这种查询，它的名字就是：**[位图索引](http://lemire.me/blog/archives/2008/08/20/the-mythical-bitmap-index/)**。想要用这种技术，需要做一些预计算，将匹配和不匹配的信息存储在bit位中，匹配设置为1，不匹配设置为0。在我们的使用场景中，想要的效果大概是下图中这样的【译者注：下图是对第一篇的图增加了扩展，建议看一下[第一篇](http://blog.jobbole.com/99910/)】：
![](http://jbcdn2.b0.upaiyun.com/2016/01/28c8edde3d61a0411511d3b1866f06365.png)
接下来，要做的就是对这些bit位做操作了（真实代码只能以字节为单位来操作），比如你想要找到带C#和Java标签的问题，代码大概是这样的：


```
for (int i = 0; i < numBits / 8; i++){ 
     result[i] = bitSetCSharp[i] & bitSetJava[i];
}
```
这个方法最大的缺点是我们必须要针对每种排序规则（比如按评论时间排序，按创建时间排序，按分数排序，按查看次数排序，按回答数量排序）给每个标签创建位图索引（C#, .NET, Java等等），因此需要消耗很大的内存空间。拿Stack Overflow 2014年9月份的数据来看，当时大概有800万个问题，每8个问题用一个字节，一个位图大概占976KB或记为0.95MB。总共加起来需要的内存达到惊人的**149GB**（0.95MB*32000个标签*5种排序）。
## 压缩位图
幸运的是有种叫做“[游程编码](http://en.wikipedia.org/wiki/Run-length_encoding)”的技术可以大幅压缩位图【译者注：游程编码是一种无损压缩算法，基本原理是用某种符号或者串长来替代具有相同值的连续符号】，我在这里使用了出色的[EWAH库](https://github.com/lemire/javaewah)的[C#版本](https://github.com/lemire/csharpewah)，这个开发库是基于[Daniel Lemire](https://twitter.com/lemire)等人发表的论文《 [Sorting improves word-aligned bitmap indexes](http://arxiv.org/abs/0901.3751) 》。使用EWAH的最大好处是你在做bit位操作的时候不用自己去解压缩位图，它会在适当的时候自己完成（想要知道如何做到的，可以看一下我在github上对[AndNot方法做的这次提交](https://github.com/mattwarren/StackOverflowTagServer/commit/20561e60e1b7d90ff0bb023ec8cf89494d0705f5)）。
如果你不想读这篇[学术论文](http://arxiv.org/abs/0901.3751)，下图展示了如何将位图压缩成64bit位长的word的，这些位图的特点是有1个或者多个bit被设置为1，接着又是多个连续的0. 因此下图中31 0x00的意思是将31个64bit的word（所有bit位都被设置为0）表示成单个值，而不是31个word。
![](http://jbcdn2.b0.upaiyun.com/2016/01/4fdbc517eccdc8c1448cbb76b074a9a1.png)
【译者注：上图中是一种典型的位图索引方法，即使用串长替代真实串的方法，具体的意思是这个位图首先是0个64bit的word，接下来是1个不全是0的64bit的word，这个word的值是17，2 bits set意思是这64位的信息里有2个1；接下来是31_64个的连续0，然后是1个不全是0的64bit的word，具体值是2的41次方。依次类推，凡是出现n 0x00的地方，都表示连续的n_64个0，这样只需要用一个数字加上0x00即可表示n*64bit长度的数据】
为了能直观的说明能节省多少空间，下表列出了有不同位数置为1时，使用压缩位图算法所占用的大小，单位是字节。（为了对比，一个未压缩的位图有100万字节，或者0.95MB）
|# Bits Set|Size in Bytes|
|----|----|
|1|24|
|10|168|
|25|408|
|50|808|
|100|1,608|
|200|3,208|
|400|6,408|
|800|12,808|
|1,600|25,608|
|3,200|512,008|
|6,400|1,000,008|
|128,000|1,000,008|
正如你所看到的这样，我们只需要64,000个bit位即可表达出一个正常位图（准确的说，只需要62,016个bit位）。**注意：**我做这些测试的时候，置为1的bit位的位置在800万个数据样本中是均匀分布的。压缩效果也是要看具体压缩数据的情况的，像这种均匀分布的情况是不利于压缩的。如果连续相同的bit位越集中，压缩效果越好。
整个Stack Overflow 3万2千个标签的数据，使用压缩位图的方法，将内存使用量降低到了**1.17GB**， 而未压缩前需要用149GB，效果是相当令人印象深刻的。
## 结论
使用压缩位图实现的查询比朴素的使用HashSet的常规查询方法（见上面代码）要更快，事实上这两种方法其实都不赖，但是在某些情况下，效果区别还是很大的。
如下图所示，在做AND NOT查询时，使用压缩位图的方法快出许多，即使是最不利于压缩的情况下，常规方法耗时超过150毫秒，而压缩位图方法只需要大约5毫秒（图中x轴是需要排除忽略的问题数量，y轴是消耗时间，时间单位是毫秒）
![](http://jbcdn2.b0.upaiyun.com/2016/01/665f644e43731ff9db3d341da5c827e14.png)
测试样本中有194,384个问题带有.net标签，528,490个问题带有jquery标签。
为了体现本人严明公正的高贵品格，我必须要申明，使用压缩位图方法在做OR查询时要比常规方法慢，具体数据如下图所示。但是需要注意数据范围，相比常规查询方法也就是5毫秒和1~2毫秒的差距，因此压缩位图算法依然是非常快的！但是压缩位图算法还有个优势就是不管要排除的问题数量有多少，它用的时间都差不多，而常规方法下，随着需要排除忽略的问题数量增加，速度变得更慢。
![](http://jbcdn2.b0.upaiyun.com/2016/01/38026ed22fc1a91d92b5d2ef93540f203.png)
如果你对所有查询类型的结果感兴趣，可以点击下面的链接查看：
- [AND查询](http://mattwarren.github.io/images/2015/10/and-queries-with-exclusions.png)
- [AND NOT查询](http://mattwarren.github.io/images/2015/10/and-not-queries-with-exclusions.png)
- [OR查询](http://mattwarren.github.io/images/2015/10/or-queries-with-exclusions.png)
- [OR NOT查询](http://mattwarren.github.io/images/2015/10/or-not-queries-with-exclusions.png)
## 深入阅读
- 位图索引
- [位图索引的神话](http://lemire.me/blog/archives/2008/08/20/the-mythical-bitmap-index/)
- [Roaring Bitmaps](http://roaringbitmap.org/) (一种更新的、更快速的压缩位图方法)
- [什么情况下位图索引会比整数列表快](http://lemire.me/blog/archives/2012/10/23/when-is-a-bitmap-faster-than-an-integer-list/)
- [位图索引在数据库中的应用](http://kellabyte.com/2013/03/05/using-bitmap-indexes-in-databases/)
- [一些黑客们对Roaring Bitmaps的讨论](https://news.ycombinator.com/item?id=8796997)
- [不同位图索引的实现研究](http://ascr-discovery.science.doe.gov/2008/12/more-than-a-bit-faster/)
- 实战
- [Github是如何通过位图索引快速复制仓库的](http://githubengineering.com/counting-objects/)
- [Roaring Bitmap在Elastic Search中的使用](https://www.elastic.co/blog/frame-of-reference-and-roaring-bitmaps)【译者注：Elastic Search是一个搜索引擎，我没研究过，看来这个搜索引擎背后的技术也是位图索引】
- [位图索引在Lucene中的应用](https://issues.apache.org/jira/browse/LUCENE-5983)【译者注：Lucene是注明的Java全文检索引擎工具包】
- [压缩位图在Druid中的应用](https://groups.google.com/forum/m/#!topic/druid-development/_kw2jncIlp0)【译者注：Druid是一种分布式大数据实时处理系统】
## 内容预告
【译者注：尽管原作者有预告，但是他真的没接着写了，不是我懒没翻译。。。】
还有很多东西要去做，接下来我讨论下面的话题：
- 到目前为止，我的实现方法还没有对垃圾收集器做一些调优，而且我的方法真的做了很多很多内存分配的事情。Stack Overflow在[如何与.NET垃圾收集做斗争](http://blog.marcgravell.com/2011/10/assault-by-gc.html)中提到“no-allocation”原则，我打算也尝试一下。
![](http://jbcdn2.b0.upaiyun.com/2016/01/011ecee7d295c066ae68d4396215c3d01.png)
【译者注：上图中的Tweeter中说Stack Overflow的用户非常多，导致很多次垃圾回收（频繁的垃圾回收会降低系统性能）】
- [针对标签服务的DDOS攻击](http://stackstatus.net/post/107352821074/outage-postmortem-january-6th-2015)
在十月份，我们收到大量的精心构造的请求，导致我们的标签引擎服务器资源使用率飙升，不过这正好是我们将高效的标签引擎发挥用途的地方。
> 
**打赏支持我翻译更多好文章，谢谢！**
[打赏译者](#rewardbox)
#### 打赏支持我翻译更多好文章，谢谢！
任选一种支付方式
![](http://www.jobbole.com/wp-content/uploads/2016/04/ecc1aaae4d8261fe267c08e14ef6e064.jpg)![](http://www.jobbole.com/wp-content/uploads/2016/04/0dd5adad8022506f9d221188fe54f5bc.jpg)
