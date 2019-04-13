
# 为什么交叉熵损失可以提高具有sigmoid和softmax输出的模型的性能，而使用均方误差损失则会存在很多问题 - 郭云飞的专栏 - CSDN博客


2017年10月16日 10:42:08[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：3062


**一、均方误差的权值更新过程（举例说明）**
代价函数经常用方差代价函数（即采用均方误差*MSE*），比如对于一个神经元（单输入单输出，sigmoid函数）,定义其代价函数为：
![](http://i.imgur.com/D4n2Dsz.jpg)
其中*y*是我们期望的输出，*a*为神经元的实际输出【 a=σ(z), where z=wx+b 】。在训练神经网络过程中，我们通过梯度下降算法来更新w和b，因此需要计算代价函数对w和b的导数：
![](https://img-blog.csdn.net/20171016104410572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
然后更新w、b：
![](https://img-blog.csdn.net/20171016105136642?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
![](https://img-blog.csdn.net/20171016105415069?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
因为sigmoid函数的性质，导致σ′(z)在z取大部分值时会很小（如下图标出来的两端，几近于平坦），这样会使得w和b*更新非常慢*（因为η * a * σ′(z)这一项接近于0）。
![](https://img-blog.csdn.net/20171016105508693?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
**二、交叉熵代价函数（cross-entropy cost function）**
为了克服MSE的这个缺点，引入了交叉熵代价函数：
![](https://img-blog.csdn.net/20171016105650113?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZ3VveXVuZmVpMjA=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
其中：y为期望的输出，*a*为神经元实际输出【a=σ(z), where z=∑Wj*Xj+b】。我们同样看看它的导数：
![](http://i.imgur.com/LCXi3QA.jpg)
![](http://i.imgur.com/LCXi3QA.jpg)
可以看到，导数中没有σ′(z)这一项，权重的更新是受σ(z)−y这一项影响，即受误差的影响。所以*当误差大的时候，权重更新就快，当误差小的时候，权重的更新就慢*。这是一个很好的性质。
**三、总结**
当用sigmoid函数作为神经元的激活函数时，最好使用交叉熵代价函数来替代方差代价函数，以避免训练过程太慢。
不过，为什么是交叉熵函数？导数中不带σ′(z)项的函数有无数种，怎么就想到用交叉熵函数？这自然是有来头的，更深入的讨论就不写了。
另外，交叉熵函数的形式是−[ylna+(1−y)ln(1−a)]，而不是 −[alny+(1−a)ln(1−y)]，为什么？因为当期望输出的y=0时，lny没有意义；当期望y=1时，ln(1-y)没有意义。而因为a是sigmoid函数的实际输出，永远不会等于0或1，只会无限接近于0或者1，因此不存在这个问题。






