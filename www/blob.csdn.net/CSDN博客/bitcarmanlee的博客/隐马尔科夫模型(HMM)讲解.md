
# 隐马尔科夫模型(HMM)讲解 - bitcarmanlee的博客 - CSDN博客


2017年08月22日 22:19:11[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1110


隐形马尔可夫模型，英文是 Hidden Markov Models，所以以下就简称 HMM。
既是马尔可夫模型，就一定存在马尔可夫链，该马尔可夫链服从马尔可夫性质：即无记忆性。也就是说，这一时刻的状态，受且只受前一时刻的影响，而不受更往前时刻的状态的影响。
在这里我们仍然使用非常简单的天气模型来做说明。
在这个马尔可夫模型中，存在三个状态，Sunny， Rainy， Cloudy，同时图片上标的是各个状态间的转移概率（如果不明白什么是转移概率，那建议先去学习什么是马尔可夫再来看HMM）。
![这里写图片描述](https://img-blog.csdn.net/20170822221253993?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在我们要说明什么是 HMM。既是隐形，说明这些状态是观测不到的，相应的，我们可以通过其他方式来『猜测』或是『推断』这些状态，这也是 HMM 需要解决的问题之一。
举个例子，我女朋友现在在北京工作，而我还在法国读书。每天下班之后，她会根据天气情况有相应的活动：或是去商场购物，或是去公园散步，或是回家收拾房间。我们有时候会通电话，她会告诉我她这几天做了什么，而闲着没事的我呢，则要通过她的行为猜测这几天对应的天气最有可能是什么样子的。
以上就是一个简单的 HMM，天气状况属于状态序列，而她的行为则属于观测序列。天气状况的转换是一个马尔可夫序列。而根据天气的不同，有相对应的概率产生不同的行为。在这里，为了简化，把天气情况简单归结为晴天和雨天两种情况。雨天，她选择去散步，购物，收拾的概率分别是0.1，0.4，0.5， 而如果是晴天，她选择去散步，购物，收拾的概率分别是0.6，0.3，0.1。而天气的转换情况如下：这一天下雨，则下一天依然下雨的概率是0.7，而转换成晴天的概率是0.3；这一天是晴天，则下一天依然是晴天的概率是0.6，而转换成雨天的概率是0.4. 同时还存在一个初始概率，也就是第一天下雨的概率是0.6， 晴天的概率是0.4.
![这里写图片描述](https://img-blog.csdn.net/20170822221337812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170822221337812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
根据以上的信息，我们得到了 HMM的一些基本要素：初始概率分布 π，隐含状态转移矩阵 A，隐含状态到可观测状态的转移概率分布矩阵 B，同时有两个状态，三种可能的观测值。或者用更一般的语言来描述，HMM模型由以下五个部分组成：
1.隐含状态H：晴天，雨天
2.可观察状态O：散步，购物，收拾
3.隐含状态间的转移概率矩阵A
4.隐含状态到可观测状态的概率转移矩阵B
5.开始的隐含状态PI：第一天的天气情况的概率分布
现在，重点是要了解并解决HMM 的三个问题。
问题1，已知整个模型，我女朋友告诉我，连续三天，她下班后做的事情分别是：散步，购物，收拾。那么，根据模型，计算产生这些行为的概率是多少。(predict 预测问题)
如果用数学语言描述问题1，就是H,A,B,PI全部已经，如何计算某一个可观察状态序列出现的概率。
问题2，同样知晓这个模型，同样是这三件事，我女朋友要我猜，这三天她下班后北京的天气是怎么样的。这三天怎么样的天气才最有可能让她做这样的事情。
如果用数学语言描述问题2，就是H,A,B,PI全部已经，已知了一个可观察状态序列{O}，我们想了解可观察状态序列{O}背后最可能对应的隐含状态序列{H}。（encode解码问题）
问题3，最复杂的，我女朋友只告诉我这三天她分别做了这三件事，而其他什么信息我都没有。她要我建立一个模型，晴雨转换概率，第一天天气情况的概率分布，根据天气情况她选择做某事的概率分布。（惨绝人寰）
如果用数学语言描述问题3，就是现在我们只知道O、H（有几种天气情况)，pi、A和B都不知道，如何通过可观察状态序列{O}去估计概率转移矩阵A和B。（learning学习问题）
而要解决这些问题，伟大的大师们分别找出了对应的算法。问题一，Forward Algorithm，向前算法，或者 Backward Algo，向后算法。 问题二，Viterbi Algo，维特比算法。问题三，Baum-Welch Algo，鲍姆-韦尔奇算法（中文好绕口）。
问题1的解决1：遍历算法。要计算产生这一系列行为的概率，那我们把每一种天气情况下产生这些行为都罗列出来，那每种情况的和就是这个概率。有三天，每天有两种可能的天气情况，则总共有种情况.举例其中一种情况 ： P（下雨，下雨，下雨，散步，购物，收拾）=P（第一天下雨）P（第一天下雨去散步）P（第二天接着下雨）P（下雨去购物）P（第三天还下雨）P（下雨回家收拾）=0.6X0.1X0.7X0.4X0.7X0.5=0.00588
当然，这里面的 P（第二天接着下雨）当然是已知第一天下雨的情况下，第二天下雨的概率，为0.7.将八种情况相加可得，三天的行为为{散步，购物，收拾}的可能性为0.033612. 看似简单易计算，但是一旦观察序列变长，计算量就会非常庞大（的复杂度，T 为观测序列的长度）。
问题1 的解决2：向前算法。先计算 t=1时刻，发生『散步』一行为的概率，如果下雨，则为 P(散步，下雨)=P（第一天下雨）X P（散步 | 下雨）=0.6X0.1=0.06；晴天，P（散步，晴天）=0.4X0.6=0.24
t=2 时刻，发生『购物』的概率，当然，这个概率可以从 t=1 时刻计算而来。
如果t=2下雨，则 P（第一天散步，第二天购物， 第二天下雨）= 【P（第一天散步，第一天下雨）X P（第二天下雨 | 第一天下雨）+P（第一天散步，第一天晴天）X P(第二天下雨 | 第一天晴天)】X P（第二天购物 | 第二天下雨）=【0.06X0.7+0.24X0.3】X0.4=0.0552
如果 t=2晴天，则 P（第一天散步，第二天购物，第二天晴天）=0.0486 （同理可得，请自行推理）
如果 t=3，下雨，则 P（第一天散步，第二天购物，第三天收拾，第三天下雨）=【P（第一天散步，第二天购物，第二天下雨）X P（第三天下雨 | 第二天下雨）+ P（第一天散步，第二天购物，第二天天晴）X P（第三天下雨 | 第二天天晴）】X P（第三天收拾 | 第三天下雨）=【0.0552X0.7+0.0486X0.4】X0.5= 0.02904
如果t=3，晴天，则 P（第一天散步，第二天购物，第三天收拾，第三天晴天）= 0.004572
那么 P（第一天散步，第二天购物，第三天收拾），这一概率则是第三天，下雨和晴天两种情况的概率和。0.02904+0.004572=0.033612.
以上例子可以看出，向前算法计算了每个时间点时，每个状态的发生观测序列的概率，看似繁杂，但在 T 变大时，复杂度会大大降低。
![这里写图片描述](https://img-blog.csdn.net/20170822221439936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170822221439936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
问题1的解决3：向后算法
[
](https://img-blog.csdn.net/20170822221439936?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)顾名思义，向前算法是在时间 t=1的时候，一步一步往前计算。而相反的，向后算法则是倒退着，从最后一个状态开始，慢慢往后推。
![这里写图片描述](https://img-blog.csdn.net/20170822221524455?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170822221550159?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
问题2的解决：维特比算法
维特比算法致力于寻找一条最佳路径，以便能最好地解释观测到的序列。
![这里写图片描述](https://img-blog.csdn.net/20170822221726436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170822221726436?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
![这里写图片描述](https://img-blog.csdn.net/20170822221742688?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
问题3的解决：Baum-Welch 算法。
此问题的复杂度要远远高于前两种算法，不是简单解释就能说的清楚的了。若有兴趣，可以私信我。
我非常赞同霍金老头的『多一个公式，少十个读者』的说法，但是自己写起来，却发现用英文的这些公式好像比中文更简洁易懂，中文好像更罗里吧嗦一些。
依然怀着非常感恩的心，再次感谢这个问题以及回答问题的这些热心的人们给我带来的帮助。
转载自：[https://www.zhihu.com/question/20962240](https://www.zhihu.com/question/20962240)
参考文献：[https://zhuanlan.zhihu.com/p/24732345](https://zhuanlan.zhihu.com/p/24732345)

