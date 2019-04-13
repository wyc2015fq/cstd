
# 在线学习（Online Learning） - forever1dreamsxx--NLP - CSDN博客


2012年12月17日 15:57:35[forever1dreamsxx](https://me.csdn.net/forever1dreamsxx)阅读数：592


转载地址：[http://www.cnblogs.com/jerrylead/archive/2011/04/18/2020173.html](http://www.cnblogs.com/jerrylead/archive/2011/04/18/2020173.html)
原题目叫做The perception and large margin classifiers，其实探讨的是在线学习。这里将题目换了换。以前讨论的都是批量学习（batch learning），就是给了一堆样例后，在样例上学习出假设函数h。而在线学习就是要根据新来的样例，边学习，边给出结果。
假设样例按照到来的先后顺序依次定义为![clip_image002](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019429858.png)。X为样本特征，y为类别标签。我们的任务是到来一个样例x，给出其类别结果y的预测值，之后我们会看到y的真实值，然后根据真实值来重新调整模型参数，整个过程是重复迭代的过程，直到所有的样例完成。这么看来，我们也可以将原来用于批量学习的样例拿来作为在线学习的样例。在在线学习中我们主要关注在整个预测过程中预测错误的样例数。
拿二值分类来讲，我们用y=1表示正例，y=-1表示负例。回想在讨论支持向量机中提到的感知算法（perception algorithm）。我们的假设函数为
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019421254.png)![clip_image003](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019427873.png)
其中x是n维特征向量，![clip_image005](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019437807.png)是n+1维参数权重。函数g用来将![clip_image007](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019436105.png)计算结果映射到-1和1上。具体公式如下：
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019432725.png)![clip_image008](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019448580.png)
这个也是logistic回归中g的简化形式。
现在我们提出一个在线学习算法如下：
新来一个样例![clip_image010](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019442102.png)，我们先用从之前样例学习到的![clip_image012](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019457925.png)来得到样例的预测值y，如果![clip_image014](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/2011041820194584.png)（即预测正确），那么不改变![clip_image005[1]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019453922.png)，反之
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019461414.png)![clip_image016](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019468905.png)
也就是说，如果对于预测错误的样例，![clip_image005[2]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019464379.png)进行调整时只需加上（实际上为正例）或者减去（实际负例）样本特征x值即可。![clip_image005[3]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019472677.png)初始值为向量0。这里我们关心的是![clip_image018](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019484247.png)的符号，而不是它的具体值。调整方法非常简单。然而这个简单的调整方法还是很有效的，它的错误率不仅是有上界的，而且这个上界不依赖于样例数和特征维度。
下面定理阐述了错误率上界：
**定理（Block and Novikoff）**：
给定按照顺序到来的![clip_image002[1]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019487595.png)样例。假设对于所有的样例![clip_image020](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019482578.png)，也就是说特征向量长度有界为D。更进一步，假设存在一个单位长度向量![clip_image022](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019481150.png)且![clip_image024](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019486133.png)。也就是说对于y=1的正例，![clip_image026](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019484705.png)，反例![clip_image028](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019499688.png)，u能够有![clip_image030](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019497986.png)的间隔将正例和反例分开。那么感知算法的预测的错误样例数不超过![clip_image032](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019492969.png)。
根据前面对SVM的理解，这个定理就可以阐述为：如果训练样本线性可分，并且几何间距至少是![clip_image030[1]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019506491.png)，样例样本特征向量最长为D，那么感知算法错误数不会超过![clip_image032[1]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019501475.png)。这个定理是62年提出的，63年Vapnik提出SVM，可见提出也不是偶然的，感知算法也许是当时的热门。
下面主要讨论这个定理的证明：
感知算法只在样例预测错误时进行更新，定义![clip_image034](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019519457.png)是第k次预测错误时使用的样本特征权重，![clip_image036](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019516916.png)初始化为0向量。假设第k次预测错误发生在样例![clip_image038](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019511899.png)上，利用![clip_image034[1]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019525737.png)计算![clip_image040](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019535671.png)值时得到的结果不正确（也就是说![clip_image042](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019537066.png)，调换x和![clip_image005[4]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019537000.png)顺序主要是为了书写方便）。也就是说下面的公式成立：
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019531668.png)![clip_image043](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019541112.png)
根据感知算法的更新方法，我们有![clip_image045](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019548047.png)。这时候，两边都乘以u得到
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019541079.png)![clip_image046](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019546062.png)
两个向量做内积的时候，放在左边还是右边无所谓，转置符号标注正确即可。
这个式子是个递推公式，就像等差数列一样f(n+1)=f(n)+d。由此我们可得
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019541918.png)![clip_image047](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019546585.png)
因为初始![clip_image005[5]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019552931.png)为0。
下面我们利用前面推导出的![clip_image049](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019551503.png)和![clip_image020[1]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019554850.png)得到
![clip_image050](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019556453.png)
也就是说![clip_image052](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019564751.png)的长度平方不会超过![clip_image034[2]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019578273.png)与D的平方和。
又是一个等差不等式，得到：
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019577717.png)![clip_image053](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019577160.png)
两边开根号得：
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019573780.png)![clip_image054](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019578764.png)
其中第二步可能有点迷惑，我们细想u是单位向量的话，
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/20110418201957159.png)![clip_image055](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019583191.png)
因此上面的不等式成立，最后得到：
[
](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019586222.png)![clip_image056](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019589254.png)
也就是预测错误的数目不会超过样本特征向量x的最长长度与几何间隔的平方。实际上整个调整过程中![clip_image005[6]](http://images.cnblogs.com/cnblogs_com/jerrylead/201104/201104182019596995.png)就是x的线性组合。
整个感知算法应该是在线学习中最简单的一种了，目前发现online learning挺有用的，以后多多学习。


