
# 神经网络之激活函数(activation function) - bitcarmanlee的博客 - CSDN博客


2017年12月18日 16:06:00[bitcarmanlee](https://me.csdn.net/bitcarmanlee)阅读数：1285


## 1.为什么要引入非线性激活函数(activation function)
如果不使用非线性激活函数，此时激活函数本质上相当于f(x)=ax+b。这种情况先，神经网络的每一层输出都是上层输入的线性函数。不难看出，不论神经网络有多少层，输出与输入都是线性关系，与没有隐层的效果是一样的，这个就是相当于是最原始的感知机(Perceptron)。至于感知机，大家知道其连最基本的异或问题都无法解决，更别提更复杂的非线性问题。
## 2.常见的非线性激活函数
因为上面的原因，所以我们引入了非线性激活函数，这样神经网络才具有实用意义，其不再是输入的线性组合关系，而可以非线性组合成各种任意函数的输出。常用的激活函数包括以下形式：
### 2.1 Sigmoid函数
Sigmoid函数我们很熟悉，将一个实数输入映射到[0,1]范围内，并且是平滑的，导数为f(x)(1-f(x))。![这里写图片描述](https://img-blog.csdn.net/20171218153738493?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
作为激活函数，其缺点如下：
1.容易出现梯度消失(gradient vanishing)
2.函数的输出不是以0为中心对称(zero-centered)
3.幂运算相对耗时
梯度消失
神经网络的优化算法是反向传播算法(Back Propagation)，即导数的反向传播：先计算输出层对用的损失，然后将损失以导数的形式向上一层网络传递，调整对应的参数，最终达到降低损失函数的目的。Sigmoid函数在深度网络中常常会导致导数逐渐变为0，使得参数无法被更新，神经网络无法被优化。
原因主要有如下两点：1.从导数图像中不难看出，当x较大或较小时，导数接近0。反向传播使用的是链式法则，当前层的导数需要之前隔层导数的乘积，几个很小的数相称，结果会很接近0。2.Sigmoid导数的最大值为x=0那一点，为0.25，这意味着导数在每一层至少会被压缩为原来的1/4，两层后为1/16，三层为1/64… 而且这还是上限，每次导数达到最大值的情况很少见。
输出不是以0为中心对称
Sigmoid函数的输出值恒大于0，这会导致模型训练的收敛速度变慢。举例来讲，对，如果所有均为正数或负数，那么其对的导数总是正数或负数，这会导致如下图红色箭头所示的阶梯式更新，这显然并非一个好的优化路径。深度学习往往需要大量时间来处理大量数据，模型的收敛速度是尤为重要的。所以，总体上来讲，训练深度学习网络尽量使用zero-centered数据 (可以经过数据预处理实现) 和zero-centered输出。
![这里写图片描述](https://img-blog.csdn.net/20171218155221635?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
幂运算相对耗时
相对于前两项，这其实并不是一个大问题，我们目前是具备相应计算能力的，但面对深度学习中庞大的计算量，最好是能省则省 :-)。之后我们会看到，在ReLU函数中，需要做的仅仅是一个thresholding，相对幂运算会快很多。
### 2.tanh函数

$$
\text{tanh}\ x = \frac{e^x - e^{-x}}{e^x + e^{-x}}
$$
![这里写图片描述](https://img-blog.csdn.net/20171218155449996?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
tanh读作Hyperbolic Tangent，其导数为$1-(f(x))^2$
如上图所示，它解决了zero-centered的输出问题，然而，gradient vanishing的问题和幂运算的问题仍然存在。
### 3.修正线性单元函数(Rectified linear unit，ReLU)
ReLu激活函数(The Rectified Linear Unit)数学表达式为：$f(x) = max(0, x)$
![这里写图片描述](https://img-blog.csdn.net/20171218160221767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYml0Y2FybWFubGVl/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
ReLU激活函数的优点：
1.梯度不饱和。梯度计算公式为：1{x>0}。因此在反向传播过程中，减轻了梯度弥散的问题，神经网络前几层的参数也可以很快的更新。
2.计算速度快。正向传播过程中，sigmoid和tanh函数计算激活值时需要计算指数，而Relu函数仅需要设置阈值。如果x<0,f(x)=0，如果x>0,f(x)=x。加快了正向传播的计算速度。
不足的地方在于：
1.ReLU的输出不是zero-centered
2.Dead ReLU Problem，指的是某些神经元可能永远不会被激活，导致相应的参数永远不能被更新。有两个主要原因可能导致这种情况产生: (1) 非常不幸的参数初始化，这种情况比较少见  (2) learning rate太高导致在训练过程中参数更新太大，不幸使网络进入这种状态。解决方法是可以采用Xavier初始化方法，以及避免将learning rate设置太大或使用adagrad等自动调节learning rate的算法。
参考地址：
[https://www.zhihu.com/question/29021768](https://www.zhihu.com/question/29021768)

