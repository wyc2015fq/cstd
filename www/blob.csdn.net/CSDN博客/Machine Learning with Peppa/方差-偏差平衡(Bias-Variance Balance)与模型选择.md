# 方差-偏差平衡(Bias-Variance Balance)与模型选择 - Machine Learning with Peppa - CSDN博客





2017年12月22日 23:06:10[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：517









本文的主要思想来源于L.沃塞曼的《All of Statistics》一书，以及林轩田的机器学习笔记

![](http://118.126.108.52/wp-content/uploads/2017/12/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20171217194847-300x292.png)![](http://118.126.108.52/wp-content/uploads/2017/12/%E5%BE%AE%E4%BF%A1%E6%88%AA%E5%9B%BE_20171217194856-300x195.png)
- 准：bias描述的是根据样本拟合出的模型的输出预测结果的期望与样本真实结果的差距，简单讲，就是在样本上拟合的好不好。要想在bias上表现好，low bias，就得复杂化模型，增加模型的参数，但这样容易过拟合 (overfitting)，过拟合对应上图是high variance，点很分散。low
 bias对应就是点都打在靶心附近，所以瞄的是准的，但手不一定稳。
- 确：varience描述的是样本上训练出来的模型在测试集上的表现，要想在variance上表现好，low varience，就要简化模型，减少模型的参数，但这样容易欠拟合(unfitting)，欠拟合对应上图是high bias，点偏离中心。low variance对应就是点都打的很集中，但不一定是靶心附近，手很稳，但是瞄的不准。


这个靶子上的点(hits)可以理解成一个一个的拟合模型，如果许多个拟合模型都聚集在一堆，位置比较偏，如图中 high bias low variance 这种情景，意味着无论什么样子的数据灌进来，拟合的模型都差不多，这个模型过于简陋了，参数太少了，复杂度太低了，这就是欠拟合；但如果是图中 low bias high variance 这种情景，你看，所有拟合模型都围绕中间那个 correct target 均匀分布，但又不够集中，很散，这就意味着，灌进来的数据一有风吹草动，拟合模型就跟着剧烈变化，这说明这个拟合模型过于复杂了，不具有普适性，就是过拟合。


所以bias和variance的选择是一个tradeoff，过高的variance对应的概念，有点『剑走偏锋』『矫枉过正』的意思，如果说一个人variance比较高，可以理解为，这个人性格比较极端偏执，眼光比较狭窄，没有大局观。而过高的bias对应的概念，有点像『面面俱到』『大巧若拙』的意思，如果说一个人bias比较高，可以理解为，这个人是个好好先生，谁都不得罪，圆滑世故，说话的时候，什么都说了，但又好像什么都没说，眼光比较长远，有大局观。（感觉好分裂 ）

注：关于这个偏执和好好先生的表述，不是非常严谨，对这两个词的不同理解会导致截然相反的推理，如果你看完这段觉得有点困惑，可以去看评论区的讨论，不得不感叹一下，在准确描述世界运行的规律这件事上，数学比文学要准确且无歧义的多。

在林轩田的课中，对bias和variance还有这样一种解释，我试着不用数学公式抽象的简单概括一下：


我们训练一个模型的最终目的，是为了让这个模型在测试数据上拟合效果好，也就是Error(test)比较小，但在实际问题中，test data我们是拿不到的，也根本不知道test data的内在规律（如果知道了，还machine learning个啥 ），所以我们通过什么策略来减小Error(test)呢？


分两步：
- 让Error(train)尽可能小
- 让Error(train)尽可能等于Error(test)


三段论，因为A小，而且A=B，这样B就小。


那么怎么让Error(train)尽可能小呢？——》把模型复杂化，把参数搞得多多的，这个好理解，十元线性回归，肯定error要比二元线性回归低啊。——》low bias


然后怎么让Error(train)尽可能等于Error(test)呢？——》把模型简单化，把参数搞得少少的。什么叫Error(train)=Error(test)？就是模型没有偏见，对train test一视同仁。那么怎样的模型更容易有这这种一视同仁的特性，换句话说，更有『通用性』，对局部数据不敏感？那就是简单的模型。——》low variance




Reference:


[1][Understanding
 the Bias-Variance Tradeoff](https://link.zhihu.com/?target=http%3A//scott.fortmann-roe.com/docs/BiasVariance.html)



