# softmax，softmax-loss，BP的解释 - AI之路 - CSDN博客





2017年05月04日 20:32:50[AI之路](https://me.csdn.net/u014380165)阅读数：13747








本文转载自：http://freemind.pluskid.org/machine-learning/softmax-vs-softmax-loss-numerical-stability/ ，看完这个博客让我对softmax，softmax-loss以及BP算法有了更深的理解，以前BP只是知道链式求导，知道梯度回传，但是具体到哪些变量和哪些变量求导就有些模糊了，现在我特整理了下思路，以下大部分是链接的原文，附带自己的一些理解（用红色标出）

![](https://img-blog.csdn.net/20170504203817251?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


联想逻辑回归的重要公式就是![](https://img-blog.csdn.net/20170504203942767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)，得到预测结果，然后再经过sigmoid转换成0到1的概率值，而在softmax中则通过取exponential的方式并进行归一化得到某个样本属于某类的概率。非负的意义不用说，就是避免正负值抵消。

![](https://img-blog.csdn.net/20170504204409994?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


逻辑回归的推导可以用最大似然或最小损失函数，本质是一样的，可以简单理解成加了一个负号，这里的y指的是真实类别。注意下softmax-loss可以看做是softmax和multinomial logistic loss两步，正如上述所写公式，把变量展开即softmax-loss。

![](https://img-blog.csdn.net/20170504204847454?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


原博客的重点在于介绍softmax-loss是分成两步还是一步到位比较好，而我这则重点说下BP。上面这个神经网络的图应该不陌生，这个公式也是在逻辑回归的核心（通过迭代得到w，然后在测试时按照上面这个公式计算类别概率）

![](https://img-blog.csdn.net/20170504205237865?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这里第一个公式是损失函数对权重w求导，其实就是梯度，红色那部分可以看前面O是怎么算出来的，就知道其导数的形式非常简单，就是输入I。蓝色部分就是BP的核心，回传就是通过这个达到的，回传的东西就是损失函数对该层输出的导数，只有把这个往前回传，才能计算前面的梯度。所以回传的不是对权重的求导，对每层权重的求导的结果会保留在该层，等待权重更新时候使用。具体看上面最后一个公式。

![](https://img-blog.csdn.net/20170504210106340?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这部分的求导：l(y,z)函数是log函数，log（x）函数求导是取1/x，后面的那个数是zy对zk的导数，当k=y时，那就是1，k不等于y时就是两个不同的常数求导，就是0。

![](https://img-blog.csdn.net/20170504210625060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxNDM4MDE2NQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


这一部分就是把softmax-loss分成两步来做，第一个求导可以先找到最前面l(y,o)的公式，也是log函数，所以求导比较简单。第二个求导也是查看前面Oi的公式，分母取平方的那种求导。最后链式相乘的结果和原来合并算的结果一样。



