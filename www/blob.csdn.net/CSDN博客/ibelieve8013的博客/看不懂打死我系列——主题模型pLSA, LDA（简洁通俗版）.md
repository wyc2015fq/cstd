# 看不懂打死我系列——主题模型pLSA, LDA（简洁通俗版） - ibelieve8013的博客 - CSDN博客





2019年03月22日 10:27:07[ibelieve8013](https://me.csdn.net/ibelieve8013)阅读数：64










讲到主题模型，大家普遍觉得是LDA，但是其实前面还有一个模型pLSA，即概率隐语义分析。

我们知道，当我们要写作的时候，是怎么样写成一篇文章的？我们肯定要先确定我们要写作的主题，然后我们写的词，则都是和这个主题相关的（可以看成是从主题中以一定的概率挑选词出来）。

**1.pLSA**

【这里规定一下符号：w是词，K是主题数量，k是某个主题，d是某篇文档，M是文档总数，对某篇文档，N是这个文档的词数。以上这些符号一定要牢记，不然后面公式看不懂】

有个看起来不像贝叶斯网络的贝叶斯网络图应运而生：（也就是我们刚才说的一个词是如何产生的）

![](https://img-blog.csdnimg.cn/20190322102250830.png)

公式来了： p(w|d) =Σz p(z|d)*p(w|z)![](https://img-blog.csdnimg.cn/20190322102250821.png)

这是某篇文档的某个词的概率，我们得把所有词都考虑到，因此整个语料库的生成概率可以用以下似然函数表示：

![](https://img-blog.csdnimg.cn/20190322102250845.png)

其中，p(dm, wn)是在第dm篇文档中，第wn个单词出现的概率，和上文的p(w|d)一致，c(dm,wn)是在第dm篇文档中，wn出现的次数。然后呢，我们看着连乘的形式就很不爽，要加以对数，于是得到了：

![](https://img-blog.csdnimg.cn/20190322102251532.png)

现在这个式子，我们来看看，c(dm,wn)是可以数出来的，我们要知道的是dm这篇文章，各个主题的分布是怎么样的即：p(zk|dm),以及第k个主题上，词的分布是怎样的：p(wn|zk)。这样一个似然函数，很容易想到极大似然法，但是遗憾的是，zk是隐变量，那么这里可以用EM算法迭代得到结果。

好了，pLSA就说到这里，其实呢，只要你能写出上面三个公式，基本上已经证明了你的实力。

**2.LDA**

下面开始讲LDA，LDA不一样在哪里呢，就是假设了主题分布和某主题的词分布服从了先验狄利克雷分布，这是什么意思呢：在pLSA中，我们认为主题分布p(zk|dm)和词分布 p(wn|zk)就是一个确定的值，我们的目的就是估计出来这个确定的值；而在LDA中，我们假设这两个参数，是不确定的，是服从一定分布的一个变化的东西，这个先验的分布，就是狄利克雷分布，【这里为什么是狄利克雷分布，是由于狄利克雷分布是多项分布的共轭先验分布，后验分布依然是服从狄利克雷分布，好处就是在后续要对这个分布更新的时候，计算起来很方便】

LDA也有一个不像贝叶斯网络的贝叶斯网络：

![](https://img-blog.csdnimg.cn/20190322102252367.png)

【注意，这里的方框的意思是一对多，上文同】

【符号说明：theta是主题分布，z是某主题，alpha是主题分布的先验参数，ϕ![](https://img-blog.csdnimg.cn/20190322102251517.png)是词分布，beta是词分布的先验参数，M是文档数，N是每个文档的词数，K是主题数】

这里，解释一下这个贝叶斯网络：

根据先验参数alpha，确定一个主题分布theta，再从这个多项式分布中采样一个主题Zij；

根据参数beta的狄利克雷分布中采样一个词分布【这个词分布是主题Zij对应的词分布

Phi_ij】

然后再由词分布phi_ij抽样生成词wij.

(这里普遍的疑惑可能是：为什么主题分布Z没有指向词分布phi，不是说词分布是主题分布Zij得来的吗？我的理解是：因为对于主题Zij来说，Zij里面的词汇就那么多，phi和Zij有关系，仅仅是因为从超参数beta采样出来的某个多项分布，把他用在主题Zij上，这个意思，才是“![](https://img-blog.csdnimg.cn/20190322102253544.png)”这句话要表达的，而不是说，主题Zij和词分布phi_ij有依赖关系，这个是没有关系的，词分布仅仅受超参数beta的影响！)

然后，我们在LDA中，最最最重要的就是求到词和主题的联合分布：

![](https://img-blog.csdnimg.cn/20190322102308147.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

第一项因子：

![](https://img-blog.csdnimg.cn/20190322102312202.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

同样的道理第二项因子：

![](https://img-blog.csdnimg.cn/20190322102311325.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

注意，这里推出来这两项是很有用的。

别忘了我们的最终的目的：给定当前词，计算它属于某主题的概率是多少：p(zi=k|w)

这里有个重要的思想，就是当我要看当前词的主题时，我看看除当前词外周围所有词的主题。

![](https://img-blog.csdnimg.cn/20190322102255248.png)的意思是，给定其他所有词的主题，当前词主题为k的概率是多少，这是很有意义的事情，因为如果给所有词初始化了一个主题，这个概率是可以算出来的！当然怎么算，这就很考手艺了。

大招来了。

pzi=kz¬i,w*= **p**z**i**,**z**¬i**,w**p*z¬i,w*=**p**z,w**p**z**¬i**,w**=**p(w|z)p(z)**p(w|**z**¬i**)p(**z**¬i**)**=**p(w|z)**p(**w**¬i**|**z**¬i**)p(**w**i**)**p(z)**p(**z**¬i**)*
![](https://img-blog.csdnimg.cn/20190322102252871.png)干蒙了吧，脑袋嗡嗡的吧？

![](https://img-blog.csdnimg.cn/20190322102312388.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

其中：

![](https://img-blog.csdnimg.cn/20190322102312431.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

上面这几个都是可以统计出来的，把所有的主题都可以计算一遍！归一化否已经不重要了。

现在推出了结论，而这个结论，只是某个词的主题！我们要求的是，这篇文档的主题分布，以及主题对应的词分布。

这里由于我们的pzi=kz¬i,w![](https://img-blog.csdnimg.cn/20190322102252764.png)是可以通过迭代计算出来的了，那么我们设给定第m个文档的主题，计算如下式子：

![](https://img-blog.csdnimg.cn/20190322102302788.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

解释一下，这个式子成立是由于在这个贝叶斯网络中的马尔科夫毯：

![](https://img-blog.csdnimg.cn/2019032210225823.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

给定了alpha和z，第m篇文档的主题theta_m怎么求？可以通过采样的方法，给定alpha对第m篇文档主题分布采样得到theta，再从theta对某个词采样一个主题z，对所有的词连乘。

然后式中是一个多点分布和狄利克雷分布相乘，依然是狄利克雷分布。

狄利克雷分布的期望很好求，直接用期望估计参数本身：

![](https://img-blog.csdnimg.cn/20190322102302422.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2liZWxpZXZlODAxMw==,size_16,color_FFFFFF,t_70)

这个式子：对词分布而言，第k个主题第t的词的概率如何计算呢？分子：超参数beta+第t个词在主题k中出现的次数，分母：beta+ 所有的词表中的词在主题k中出现的次数之和。

类似的，对主题分布而言，第m个文档的主题分布，（是文档m中出现主题k的次数+alpha）/(文档m中出现所有主题的次数之和+alpha)。

好了，结论怎么样？结论是不是很简单？结论是不是难以置信。

打完收工。

可能本文比较简洁，因为这里并没有补充关于多项分布，beta分布，狄利克雷分布，共轭先验，贝叶斯网络，马尔科夫毯等等等等的知识。如果要补充的话，文章显得繁琐且冗长，让人没有了阅读的欲望。这里假设各位看官已经懂了那些知识，然后本文的梳理，应该十分清楚了。

我常常在想，当有人问你主题模型是什么的时候，你如何把这个概念给说清楚？大家可能常常都是这样的情况，你看懂了大板大板的公式，遗憾的是仅仅是看懂了，但让你推，你又推不出来，现在让你讲，如果你也讲不出来，那假设对面坐的是面试官，你让人家怎么相信你是搞懂了，你如何从菜鸡里面区分出来。所以我觉得，对自己要求还是要更高一点，你看起来很用功，很努力，但是你浅尝辄止，不求甚解，搞不透彻，这是致命的，假设你装逼的能力还不强，求职的时候比你菜几个数量级的比你先拿到offer也是很可能的事情。

这是最近的一点感悟，看了一些兄弟的面经，还是挺有感触的，面试的时候现场推导公式，手撸经典的算法，也不是不可能。就算面试官不让你推，你抢过来纸笔也得给他写出来，这就是实力，这一看就是大佬，所以近期的工作，一定是把经典的算法吃透，每个算法的几个核心公式要写得出来，写出来，这比你瞎比比几句，有说服力得多！！

所以：本文出现的公式，各位都应该能够手撸出来，你才算真的基本搞懂了LDA主题模型。

参考文献：

百面机器学习第五章149-153.

邹博的七月在线视频及课件





