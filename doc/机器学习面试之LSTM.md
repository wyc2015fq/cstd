# 机器学习面试之LSTM

> 在深度学习中，RNN已经成为标准组件，很多领域都要用到，LSTM则是最经典的RNN结构。所以面试时是必问的，最基本的面试题就是要说清楚LSTM的结构。本文试图对其结构进行浅显易懂的介绍，主要参考的是这篇文章：<http://colah.github.io/posts/2015-08-Understanding-LSTMs/>如果你已经看过，就没必要看本文了。如果你觉得看完后还是有些疑惑，那么本文就是你需要的。

# 1从Cell State开始

在下面的过程中，我们总是用语言模型作为例子来说明，心中始终有个具体例子对照，可以大大减轻理解上的困难。

来看这句话：“我是中国人，我会说*中文*”，我们的任务是根据这句话前面的内容，预测最后的两个字“中文”。要做到这一点，模型必须能够记住前面的信息，尤其是“中国人”。在LSTM中，记住前面的信息是通过Cell State来实现的。
所以，在理解LSTM的结构时，应始终以 Cell State为中心，这样就抓住了理解其结构的关键。

先来看一张LSTM的完整结构图，然后我们再来一步步从Cell State开始分析里面的内容。

完整图如下：



![img](https://upload-images.jianshu.io/upload_images/1371984-a18dc460cb62025c.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/790/format/webp)

屏幕快照 2018-08-24 上午7.40.41.png

# 2Cell State的传递主线

Cell State好比一个记忆器，当你不断往cell里面输入数据时，它会不断变化，来记住之前输入的信息，这种记忆并不是机械式的，而是有选择地记忆的。下面我们就来看看它是如何选择性记忆的。

Cell State 的传递过程如下所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-dee63c070477939b.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/500/format/webp)

屏幕快照 2018-08-24 上午7.45.22.png

我们看到，当xt输入到Cell中后，Cell State 从Ct-1变到了Ct。

C是一个向量，向量的维度是我们自己来选择的，如果我们选256，就表示我们想要用256位来保存记忆信息，这个维度越大，相当于记忆的容量越大，可保存的信息自然越多，但是需要训练的参数也会越多。

我们看到，Ct-1到Ct共经过了两步操作，第一步是一个point wise 的乘法操作，第二步是一个point wise的加操作。第一步是忘记一些不再有用的记忆，第二步是把xt中有用的信息加到记忆中。

来看这句话：“小丽是一个女孩，*她*唱歌很好，小明是一个男孩，*他*篮球很好。”我们的模型要预测*她*，*他*两个代词。当预测第一个时，我们希望Cell State能记住之前的小丽是女孩的信息，当预测第二个时，我们希望Cell State清除掉之前记住的女孩的信息，将小明是男孩的信息记住，这样在预测时才会正确。

做到这一点，正是靠着上面提到的两步，我们分别详细看看。

## 1第一步详解

第一步的具体过程如图所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-d0466ffe52417303.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/775/format/webp)

屏幕快照 2018-08-24 上午8.04.31.png

这里出现了ht-1，ht-1是输出，它也是来自于Ct-1，只不过多了一层计算。可以将它理解为基于Ct-1给出的预测。以“小丽是一个女孩，*她*唱歌很好”为例，当Cell接收到输入“小丽是一个女孩”时，Cell State中存储了小丽是女孩这一信息，所以下面预测的代词就会是“她”。在这个例子中，“小丽是女孩”就是Ct-1，“她”就是ht-1。ht-1和Ct-1的向量位数是相同的。

ft就是要和Ct-1进行point wise乘的向量，如果Ct-1是s位，那么，ft自然也要是s位。看上图右边的公式，我们自然可以算出，Wf的行数必须s，它的列数=s+ xt的位数。总的参数个数是：s*（s + xt的位数），可见，Wf的规模是和s的大小直接相关的，这就是刚才说过的，s越大，虽然保存的信息也会越多，但是模型的参数规模也会按照平方级别上升。bf的位数也是s。

最外面的sigma表示对s位的向量的每一位都做sigmoid运算，以便将共s位的ft中的每一位都约束到0和1之间。

这样ft与Ct-1相乘时，ft中接近0的位表示将Ct-1相应的位的值进行清除，ft中接近1的位表示将Ct-1相应的位的值进行保留。

这就是第一步的操作，对原有的记忆信息进行选择性保留和遗忘。

## 2第二步详解

第二步的目的就是将新的信息加入到Cell State中，如下图所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-1bfa49fedcd77791.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/760/format/webp)

屏幕快照 2018-08-24 上午8.27.34.png

这个过程又分为两步：第一步是将xt中的信息转换为和Ct-1一样长度的向量，这个过程就是上图中下面的带波浪线的Ct所做的事情，第二步就是将带波浪线的Ct也进行一个信息筛选，筛选后的信息才最终会加到Ct中。这个信息筛选功能就是通过和it进行point wise 的相乘来实现的，这和上面的ft非常相似。ft就表示forget，it就表示input，一个对原有信息进行筛选，一个对新的信息进行筛选。

好了，下面就是要把两部分筛选的信息合并到一起：



![img](https://upload-images.jianshu.io/upload_images/1371984-2c418cb644070af8.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/722/format/webp)

屏幕快照 2018-08-24 上午8.34.53.png

至此，我们的Ct-1成功地进化到了Ct，还有一件事没有做，就是要根据Ct产生本步的预测ht。下面就来看看。

# 3根据Ct产生ht

这个过程如下所示：



![img](https://upload-images.jianshu.io/upload_images/1371984-e49f2f6d64c8175e.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/742/format/webp)

屏幕快照 2018-08-24 上午8.37.39.png

同样的，根据Ct产生ht时，我们也要经过一个筛选，这个筛选就由ot来承担，ot就是代表output的意思。同时，对Ct做了一个tanh的变换，目的是让其每一位都在-1和1之间。

那么我们来计算一下，如果Ct是s位，那么整个Cell的参数是多少呢？

ft、ot、it以及产生带波浪线的Ct时，都用到了矩阵，一共4个，对每一个矩阵，伴随的还有一个b的bias，它的位数也是s，所以，总的参数就是：

**4 \*s\*（s + xt的位数 + 1）**

由此可见，选择Ct的位数是一个非常关键的参数，它决定了模型的规模。

# 4无总结不进步

以上，通过对LSTM的分析，我们清楚了LSTM背后的idea，理解了这些，就很容易记住其结构，并且不容易遗忘。

Note：下一篇文章，将会写写我对注意力机制的一些理解。

