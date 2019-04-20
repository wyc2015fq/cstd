# 基于java的中文分词工具ANSJ - Spark高级玩法 - CSDN博客
2017年12月05日 00:00:00[Spark高级玩法](https://me.csdn.net/rlnLo2pNEfx9c)阅读数：2037
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFU5A2z4WnTGWTFfJacdqkq4XNBEBSoCo1ExrPkUaOCks6vXVLHI17Vczc0YibNK0tpNSM1wOpJHJMQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
**ANSJ**
这是一个基于n-Gram+CRF+HMM的中文分词的java实现.
分词速度达到每秒钟大约200万字左右（mac air下测试），准确率能达到96%以上
目前实现了.中文分词. 中文姓名识别 . 用户自定义词典,关键字提取，自动摘要，关键字标记等功能
可以应用到自然语言处理等方面,适用于对分词效果要求高的各种项目.
分词的目的是创建一个高稳定可用的中文分词工具,可以利用到各种需要文字处理的场景中下面简单介绍一下Ansj中文分词的主要算法及特点.
**数据结构**
**高度优化Trie树**
在用户自定义词典以及各种类似于Map的场景中,大量使用的一个工具,众所周知,Trie具有高速的文本扫描能力,和较低的内存占用率,是最好的AC机之一,弦外之音,在我的认知范围内,貌似没有之一.相比其它结构在性能和构造上做到了很好的平衡,但是在java中,大量构建map尤其是hashmap,是一个非常昂贵的操作,通过对于一个map放入大量的key也注定其在自动拆箱装箱,以及解决冲突,大量hash匹配上做了过多的小消耗,虽然多数人认为,这种消耗属于纳秒级别的,但是对于动不动就上GB的文本来说,这个消耗是不可忽略的,所以作者在这里使用了首字母hash次字二分的方式来避免过多的消耗内存,也正应为有了这个机制.可以保证Ansj加载更多的用户自定义词典,有人问我具体的数字.大约500万词,1Gde
 内存.在这里作者强烈推荐这个小家伙,你可以通过nlp-lang包来获取这个小工具,居家神器.
**三数组trie树**
三数组trie树,好吧我知道你们会吐槽我明明用的DAT(Double Array Tree),为什么在这里改成了TAT(Tree Array Tree)。我也不想如此，但是为了严谨一些真实的还原算法，的确是用了三个数组来实现的DAT的，主要是为了在判断词语之后避免一次无谓的倒退，算是空间换取时间的一个策略吧，具体感兴趣的可以参见nlp-lang中DAT的创建。对于DAT算法，个人觉得。如非必要，勿用，其在构造和修改上具有很多不确定性，不符合简单可依赖的理念，有兴趣的人可以了解下。在我小时候的博客中写了几篇关于DAT的文章，虽然很水。据说也有几个人看明白了。
**机器学习**
隐马尔科夫 语言模型 最短路径
ansj中和ngram一起使用.通过两个词语之间的关联来确定,用来做语义消歧.
TF/IDF 词袋模型
关键词抽取中用到.用来确定一个词的重要程度.同时利用关键词对文章进行自动摘要
CRF 类似CRF的上下文基于词的标注 用来实现新词发现功能,同时新词发现也服务于关键词抽取
**ToAnalysis 精准分词**
精准分词是Ansj分词的***店长推荐款***
它在易用性,稳定性.准确性.以及分词效率上.都取得了一个不错的平衡.如果你初次尝试Ansj如果你想开箱即用.那么就用这个分词方式是不会错的.
**DicAnalysis 用户自定义词典优先策略的分词**
用户自定义词典优先策略的分词,如果你的用户自定义词典足够好,或者你的需求对用户自定义词典的要求比较高,那么强烈建议你使用DicAnalysis的分词方式.
可以说在很多方面Dic优于ToAnalysis的结果
**NlpAnalysis 带有新词发现功能的分词**
nlp分词是总能给你惊喜的一种分词方式.
它可以识别出未登录词.但是它也有它的缺点.速度比较慢.稳定性差.ps:我这里说的慢仅仅是和自己的其他方式比较.应该是40w字每秒的速度吧.
个人觉得nlp的适用方式.1.语法实体名抽取.未登录词整理.主要是对文本进行发现分析等工作
**IndexAnalysis 面向索引的分词**
面向索引的分词。顾名思义就是适合在lucene等文本检索中用到的分词。 主要考虑以下两点
召回率 * 召回率是对分词结果尽可能的涵盖。比如对“上海虹桥机场南路” 召回结果是[上海/ns, 上海虹桥机场/nt, 虹桥/ns, 虹桥机场/nz, 机场/n, 南路/nr]
准确率 * 其实这和召回本身是具有一定矛盾性的Ansj的强大之处是很巧妙的避开了这两个的冲突 。比如我们常见的歧义句“旅游和服务”->对于一般保证召回 。大家会给出的结果是“旅游 和服 服务” 对于ansj不存在跨term的分词。意思就是。召回的词只是针对精准分词之后的结果的一个细分。比较好的解决了这个问题
**BaseAnalysis 最小颗粒度的分词**
基本就是保证了最基本的分词.词语***颗粒度最非常小***的..所涉及到的词大约是***10万***左右.
基本分词速度非常快.在macAir上.能到每秒300w字每秒.同时准确率也很高.但是对于新词他的功能十分有限.
**功能统计**
![0?wx_fmt=png](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/adI0ApTVBFXwjj72TR3bpvmThqcOzmrCC8QeGlGnO4cJ21oaz2NpaN1lpm7agG8xZG2iaPm60iaEGBiapLCyoCt7Q/0?wx_fmt=png)
String str = "洁面仪配合洁面深层清洁毛孔 清洁鼻孔面膜碎觉使劲挤才能出一点点皱纹 脸颊毛孔修复的看不见啦 草莓鼻历史遗留问题没辙 脸和脖子差不多颜色的皮肤才是健康的 长期使用安全健康的比同龄人显小五到十岁 28岁的妹子看看你们的鱼尾纹" ;
System.out.println(BaseAnalysis.parse(str));
洁/ag,面/q,仪/k,配合/v,洁/ag,面/q,深层/b,清洁/a,毛孔/n, ,清洁/a,鼻孔/n,面膜/n,碎/a,觉/v,使劲/v,挤/v,才能/v,出/v,一/m,点点/v,皱纹/n, ,脸颊/n,毛孔/n,修复/v,的/u,看/v,不见/v,啦/y, ,草莓/n,鼻/ng,历史/n,遗留/vn,问题/n,没辙/v, ,脸/n,和/c,脖子/n,差不多/l,颜色/n,的/u,皮肤/n,才/d,是/v,健康/a,的/u,
 ,长期/d,使用/v,安全/an,健康/a,的/u,比/p,同龄人/n,显/v,小/a,五/m,到/v,十/m,岁/q, ,28/m,岁/q,的/u,妹子/n,看看/v,你们/r,的/u,鱼尾纹/n
System.out.println(ToAnalysis.parse(str));
洁/ag,面/q,仪/k,配合/v,洁/ag,面/q,深层/b,清洁/a,毛孔/n, ,清洁/a,鼻孔/n,面膜/n,碎/a,觉/v,使劲/v,挤/v,才能/v,出/v,一/m,点点/v,皱纹/n, ,脸颊/n,毛孔/n,修复/v,的/u,看/v,不见/v,啦/y, ,草莓/n,鼻/ng,历史/n,遗留/vn,问题/n,没辙/v, ,脸/n,和/c,脖子/n,差不多/l,颜色/n,的/u,皮肤/n,才/d,是/v,健康/a,的/u,
 ,长期/d,使用/v,安全/an,健康/a,的/u,比/p,同龄人/n,显/v,小/a,五/m,到/v,十岁/mq, ,28岁/mq,的/u,妹子/n,看看/v,你们/r,的/u,鱼尾纹/n
System.out.println(DicAnalysis.parse(str));
洁/ag,面/q,仪/k,配合/v,洁/ag,面/q,深层/b,清洁/a,毛孔/n, ,清洁/a,鼻孔/n,面膜/n,碎/a,觉/v,使劲/v,挤/v,才能/v,出/v,一/m,点点/v,皱纹/n, ,脸颊/n,毛孔/n,修复/v,的/u,看/v,不见/v,啦/y, ,草莓/n,鼻/ng,历史/n,遗留/vn,问题/n,没辙/v, ,脸/n,和/c,脖子/n,差不多/l,颜色/n,的/u,皮肤/n,才/d,是/v,健康/a,的/u,
 ,长期/d,使用/v,安全/an,健康/a,的/u,比/p,同龄人/n,显/v,小/a,五/m,到/v,十岁/mq, ,28岁/mq,的/u,妹子/n,看看/v,你们/r,的/u,鱼尾纹/n
System.out.println(IndexAnalysis.parse(str));
洁/ag,面/q,仪/k,配合/v,洁/ag,面/q,深层/b,清洁/a,毛孔/n, ,清洁/a,鼻孔/n,面膜/n,碎/a,觉/v,使劲/v,挤/v,才能/v,出/v,一/m,点点/v,皱纹/n, ,脸颊/n,毛孔/n,修复/v,的/u,看/v,不见/v,啦/y, ,草莓/n,鼻/ng,历史/n,遗留/vn,问题/n,没辙/v, ,脸/n,和/c,脖子/n,差不多/l,颜色/n,的/u,皮肤/n,才/d,是/v,健康/a,的/u,
 ,长期/d,使用/v,安全/an,健康/a,的/u,比/p,同龄人/n,显/v,小/a,五/m,到/v,十岁/mq, ,28岁/mq,的/u,妹子/n,看看/v,你们/r,的/u,鱼尾纹/n
System.out.println(NlpAnalysis.parse(str));
洁/ag,面仪/nw,配合/v,洁面/nw,深层/b,清洁/a,毛孔/n, ,清洁/a,鼻孔/n,面膜/n,碎觉/nw,使劲/v,挤/v,才/d,能/v,出/v,一点点/nw,皱纹/n, ,脸颊/n,毛孔/n,修复/v,的/u,看/v,不/d,见/v,啦/y, ,草莓/n,鼻历史/nw,遗留/vn,问题/n,没辙/v, ,脸/n,和/c,脖子/n,差不多/l,颜色/n,的/u,皮肤/n,才/d,是/v,健康/a,的/u, ,长期/d,使用/v,安全/an,健康/a,的/u,比/p,同龄人/n,显/v,小/a,五/m,到/v,十岁/mq,
 ,28岁/mq,的/u,妹子/n,看看/v,你们/r,的/u,鱼尾纹/n
**停用词过滤**
停用词需求是一种及其常见的需求,好处很多很多,令人惊讶的是坏处比好处还多,所以一般情况下不要用这个破玩意.我觉得主要是古时候,计算能力有限,大家不想对于无畏的字符串,计算太多,放给处女座程序员,一般觉得,tmd有个家伙没用一直在耗费我的cpu,删了丫的,好吧,我真不是处女座黑.反正这个功能也得有,并且已经将这个功能移位了.调用如下.
1.实例化一个过滤器
StopRecognition filter = new StopRecognition();
filter.insertStopNatures("uj"); //过滤词性
filter.insertStopNatures("ul");
filter.insertStopNatures("null");
filter.insertStopWords("我"); //过滤单词
filter.insertStopRegexes("小.*?"); //支持正则表达式
调用过滤
Result modifResult = ToAnalysis.parse(str).recognition(fitler); //过滤分词结果
**测试例子**
String str = "欢迎使用ansj_seg,(ansj中文分词)在这里如果你遇到什
么问题都可以联系我.我一定尽我所能.帮助大家.ansj_seg更快,更准,更自由!" ;
StopRecognition filter = new StopRecognition();
filter.insertStopWords("我"); //过滤单词
filter.insertStopWords("你");
filter.insertStopWords("的");
filter.insertStopWords(")");
filter.insertStopWords("(");
System.out.println(ToAnalysis.parse(str).recognition(filter));
System.out.println(ToAnalysis.parse(str).recognition(filter));
欢迎/v,使用/v,ansj/en,_,seg/en,,/w,ansj/en,中文/nz,分词/v,在/p,这里/r,如果/c,遇到/v,什么/r,问题/n,都/d,可以/v,联系/v,./w,一定/d,尽/v,所能/v,./w,帮助/v,大家/r,./w,ansj/en,_,seg/en,更快/d,,/w,更/d,准/a,,/w,更/d,自由/a,!/w
欢迎/v,使用/v,ansj/en,_,seg/en,,/w,ansj/en,中文/nz,分词/v,在/p,这里/r,如果/c,遇到/v,什么/r,问题/n,都/d,可以/v,联系/v,./w,一定/d,尽/v,所能/v,./w,帮助/v,大家/r,./w,ansj/en,_,seg/en,更快/d,,/w,更/d,准/a,,/w,更/d,自由/a,!/w
**推荐阅读：**
1，[基于DF的Tokenizer分词](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484300&idx=1&sn=d09e8213b4572b3310fcf5c0f260306a&chksm=9f38e0a4a84f69b281c16a454ab71d7b628737d432d42c309cb92f6e05810ce253a42cd8ce74&scene=21#wechat_redirect)
2，[基于DataFrame的StopWordsRemover处理](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247484300&idx=2&sn=92e6130aadc992be9d6fd35348b037f6&chksm=9f38e0a4a84f69b2d56f92bf7bfcc1cf24df980741274c16c5f12adcea960ed0963a19b81748&scene=21#wechat_redirect)
3，[Spark Structured
 Streaming高级特性](http://mp.weixin.qq.com/s?__biz=MzA3MDY0NTMxOQ==&mid=2247483938&idx=1&sn=0350e49c3f86f0ec2e196dfdf9506e22&chksm=9f38e10aa84f681c06d069690b8ddd949432d8e46817e5a6ff366ac128139b935b800238a9ad&scene=21#wechat_redirect)
**ANSJ的jar包，工程，demo，及词性标注表格。请关注公众号，输入：ansj，获取。**
![640.png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/yqVAqoZvDibGvvZsCSFg4MtnXJvpwMWqssPsNF7n9JsSib9z44Naja7DrEQls7XabzQXaAfiaceo4mmovPjEzoIjQ/640.png)
**关于Spark高级玩法**
kafka，hbase，spark，Flink等入门到深入源码，spark机器学习，大数据安全，大数据运维，请关注浪尖公众号，看高质量文章。
![640.jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/adI0ApTVBFWicEOUZkFLHTzutxwiadmt0ickOuxL2Ml31qdB6W0n5AgPMfpzSib0HVDDNhtm3MwW2yC8qo09ibpeoYA/640.jpeg)
**更多文章，敬请期待**

