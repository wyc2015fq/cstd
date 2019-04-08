# NLP学习记录：语言模型

2018年02月17日 21:02:02 [浪漫主义AI](https://me.csdn.net/qq_37171771) 阅读数：678



学习了cs224n之后，深感这门课更偏深度学习，因此仅学习这门课后NLP基础不足，NLP领域的知识学习并不系统，基础概念不清，感觉对于NLP领域的问题直觉不足，因此开始学习Michael Collins的NLP课程，结合此前学习的体悟写一些综合性的感想。

### 语言模型

对于一个含有有限个单词的单词库V： 
![单词库](https://img-blog.csdn.net/20180217184421128?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
从中选取若干个单词，以某种顺序排列，最后加上STOP符结束，就可以生成一个语句，将所有这些语句的集合称为V+： 
![这里写图片描述](https://img-blog.csdn.net/20180217184850071?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
由于在NLP任务中，V本身已经很大，所以可以认为V+是无限大的。 
语言模型就是要计算V+的概率分布p的模型： 
![这里写图片描述](https://img-blog.csdn.net/20180217185130156?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
从直觉上来说，符合语言规范和人类思维的语句的概率应该较高，而人类看着很荒唐的语句的概率应该很低。 
那么这个概率分布如何计算呢？ 
先从最简单的方式开始： 
![这里写图片描述](https://img-blog.csdn.net/20180217190048874?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
其中N是训练集中语句的个数，c指x1…xn词序列的语句出现的次数。 
显然，这种计算方式对于训练集的要求太高了，需要预测的语句均在训练集中出现，且要有一定的频率来计算合理的概率，这显然是很没有效率、性能也很差的方法。

### 马尔科夫过程

对于一个长度为n的序列：X1,X2,X3…Xn，要计算： 
![这里写图片描述](https://img-blog.csdn.net/20180217190725601?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
考虑概率的链式分解： 
P(A,B)=P(A)P(B|A) 
P(A,B,C)=P(A)P(B|A)P(C|A,B) 
……. 
有： 
![这里写图片描述](https://img-blog.csdn.net/20180217191332086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
假设： 
![这里写图片描述](https://img-blog.csdn.net/20180217191556958?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
则： 
![这里写图片描述](https://img-blog.csdn.net/20180217191647391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这个假设的意思就是某个单词出现的概率只与前一个单词有关，似乎是一个过强的假设，实际应用中用处不大，所以我们可以使用弱一些的假设：某单词出现的概率与之前两个单词有关。那么会得到： 
![这里写图片描述](https://img-blog.csdn.net/20180217192024397?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中x0=x-1=*。 
这样，我们就得到了常用的trigram模型： 
对于包含有限个单词的集合V(包含STOP和*)： 
![这里写图片描述](https://img-blog.csdn.net/20180217192634669?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
其中xi属于V，xn=STOP，x0=x-1=*。 
例： 
![这里写图片描述](https://img-blog.csdn.net/20180217192742050?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
那么如何计算p呢？ 
一种最简单的方式： 
![这里写图片描述](https://img-blog.csdn.net/20180217192958030?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
由于在实践中可能会遇到count=0，为了避免这种情况，可以使用linear interpolation 方法： 
![这里写图片描述](https://img-blog.csdn.net/20180217194330446?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

### RNN

（个人脑洞部分） 
现在，让我们看一下RNN的公式： 
![这里写图片描述](https://img-blog.csdn.net/20180217194911094?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
其中，e是词向量，x是one-hot向量。 
实际上，RNN可以看作是利用一个单词序列来预测下一个单词的模型，在预测过程中序列中的每一个单词都有不同的权重——前一个单词乘了一个I，再前一个乘了IH，再前一个乘了IHH…….与上面的linear interpolation思想如出一辙。 
那么，RNN是如何计算概率p的呢？ 
RNN输出中的概率源于词向量。 
回想词向量的计算过程，以CBOW为例，我们使用窗口中的上下文单词预测中心词，训练过程中利用概率最大化来更新词向量，这个过程中赋予了词向量预测其他单词出现的概率的能力，而RNN本质上只是将这些概率信息组合起来而已。 
要注意，这个概率组合的基本单位不是词向量，而是词向量的每一个维度。 
不同的RNN架构给予了模型不同的提取和组合概率的能力。 
![这里写图片描述](https://img-blog.csdn.net/20180217201748292?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
普通RNN只能在词向量上乘IHH……，显然，越远的单词乘的矩阵越多，必然越难准确地发挥作用，所以，GRU和LSTM通过设置一些gate使得模型获得更大的灵活度，稍远的单词可以比更近的单词发挥更为主动的作用，这就增加了有效提取词向量中信息的能力。 
在CNN上，“组合”的意味更浓，设想一个句法树，第一层卷积层计算了基于filter的所有可能的组合，然后在第一层pooling层上筛选出最有价值的组合方式，此时就相当于是经过了句法树中最底层的一层组合，之后还可以有更深更多的组合，直到完成一颗句法树，获得一个语句向量。 
目前，出现了非常多基于RNN、CNN和注意力机制的模型，其本质都是对词向量信息的提取组合。

### 困惑度

困惑度是用来衡量语言模型性能的一种方式。 
对于测试集中的m个语句： 
s1,s2…..sm 
![这里写图片描述](https://img-blog.csdn.net/20180217193342978?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast) 
其中M是测试集中的单词个数。 
对于|V|=50000的V,有实践经验估计的困惑度 
![这里写图片描述](https://img-blog.csdn.net/20180217193607735?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzcxNzE3NzE=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)