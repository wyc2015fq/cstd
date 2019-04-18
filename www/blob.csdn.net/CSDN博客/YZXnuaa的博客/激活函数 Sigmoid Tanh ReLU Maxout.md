# 激活函数 Sigmoid/Tanh/ReLU/Maxout - YZXnuaa的博客 - CSDN博客
2018年03月28日 20:36:42[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：1099
日常 coding 中，我们会很自然的使用一些激活函数，比如：sigmoid、ReLU等等。不过好像忘了问自己一()件事：
- 为什么需要激活函数？
- 激活函数都有哪些？都长什么样？有哪些优缺点？
- 怎么选用激活函数？
本文正是基于这些问题展开的，欢迎批评指正！
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-nn.png)
(此图并没有什么卵用，纯属为了装x …)
# Why use activation functions?
激活函数通常有如下一些性质：
- 非线性： 当激活函数是线性的时候，一个两层的神经网络就可以逼近基本上所有的函数了。但是，如果激活函数是恒等激活函数的时候（即），就不满足这个性质了，而且如果MLP使用的是恒等激活函数，那么其实整个网络跟单层神经网络是等价的。
- 可微性： 当优化方法是基于梯度的时候，这个性质是必须的。
- 单调性： 当激活函数是单调的时候，单层网络能够保证是凸函数。
- ： 当激活函数满足这个性质的时候，如果参数的初始化是random的很小的值，那么神经网络的训练将会很高效；如果不满足这个性质，那么就需要很用心的去设置初始值。
- 输出值的范围： 当激活函数输出值是 有限 的时候，基于梯度的优化方法会更加 稳定，因为特征的表示受有限权值的影响更显著；当激活函数的输出是 无限 的时候，模型的训练会更加高效，不过在这种情况小，一般需要更小的learning rate.
> 
这些性质，也正是我们使用激活函数的原因！
# Activation Functions.
## Sigmoid
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-sigmoid_relu.png)
Sigmoid 是常用的非线性的激活函数，它的数学形式如下： 
正如前一节提到的，它能够把输入的连续实值“压缩”到0和1之间。 
特别的，如果是非常大的负数，那么输出就是0；如果是非常大的正数，输出就是1. 
sigmoid 函数曾经被使用的很多，不过近年来，用它的人越来越少了。主要是因为它的一些 缺点：
- Sigmoids saturate and kill gradients. （saturate 这个词怎么翻译？饱和？）sigmoid 有一个非常致命的缺点，当输入非常大或者非常小的时候（saturation），这些神经元的梯度是接近于0的，从图中可以看出梯度的趋势。所以，你需要尤其注意参数的初始值来尽量避免saturation的情况。如果你的初始值很大的话，大部分神经元可能都会处在saturation的状态而把gradient kill掉，这会导致网络变的很难学习。
- Sigmoid 的 output 不是0均值. 这是不可取的，因为这会导致后一层的神经元将得到上一层输出的非0均值的信号作为输入。 
产生的一个结果就是：如果数据进入神经元的时候是正的(e.g.  elementwise in )，那么  计算出的梯度也会始终都是正的。 
当然了，如果你是按batch去训练，那么那个batch可能得到不同的信号，所以这个问题还是可以缓解一下的。因此，非0均值这个问题虽然会产生一些不好的影响，不过跟上面提到的 kill gradients 问题相比还是要好很多的。
## tanh
tanh 是上图中的右图，可以看出，tanh 跟sigmoid还是很像的，实际上，tanh 是sigmoid的变形： 
与 sigmoid 不同的是，tanh 是0均值的。因此，实际应用中，tanh 会比 sigmoid 更好（毕竟去粗取精了嘛）。
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-relu.png)
## ReLU
近年来，ReLU 变的越来越受欢迎。它的数学表达式如下： 
很显然，从图左可以看出，输入信号时，输出都是0， 的情况下，输出等于输入。 是二维的情况下，使用ReLU之后的效果如下：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-relu-perf.png)
ReLU 的优点：
- [Krizhevsky et al.](http://www.cs.toronto.edu/~fritz/absps/imagenet.pdf) 发现使用 ReLU 得到的SGD的收敛速度会比 sigmoid/tanh 快很多(看右图)。有人说这是因为它是linear，而且 non-saturating
- 相比于 sigmoid/tanh，ReLU 只需要一个阈值就可以得到激活值，而不用去算一大堆复杂的运算。
ReLU 的缺点： 当然 ReLU 也有缺点，就是训练的时候很”脆弱”，很容易就”die”了. 什么意思呢？
> 
举个例子：一个非常大的梯度流过一个 ReLU 神经元，更新过参数之后，这个神经元再也不会对任何数据有激活现象了。
如果这个情况发生了，那么这个神经元的梯度就永远都会是0.
实际操作中，如果你的learning rate 很大，那么很有可能你网络中的40%的神经元都”dead”了。 
当然，如果你设置了一个合适的较小的learning rate，这个问题发生的情况其实也不会太频繁。
## Leaky-ReLU、P-ReLU、R-ReLU
Leaky ReLUs： 就是用来解决这个 “dying ReLU” 的问题的。与 ReLU 不同的是： 

这里的  是一个很小的常数。这样，即修正了数据分布，又保留了一些负轴的值，使得负轴信息不会全部丢失。
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-leaky.png)
关于Leaky ReLU 的效果，众说纷纭，没有清晰的定论。有些人做了实验发现 Leaky ReLU 表现的很好；有些实验则证明并不是这样。
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-prelu.png)
Parametric ReLU： 对于 Leaky ReLU 中的，通常都是通过先验知识人工赋值的。 
然而可以观察到，损失函数对的导数我们是可以求得的，可不可以将它作为一个参数进行训练呢？ 
Kaiming He的论文《Delving Deep into Rectifiers: Surpassing Human-Level Performance on ImageNet Classification》指出，不仅可以训练，而且效果更好。
公式非常简单，反向传播至未激活前的神经元的公式就不写了，很容易就能得到。对的导数如下：
原文说使用了Parametric ReLU后，最终效果比不用提高了1.03%.
Randomized ReLU： 
Randomized Leaky ReLU 是 leaky ReLU 的random 版本 （ 是random的）. 
它首次试在 kaggle 的NDSB 比赛中被提出的。
> 
核心思想就是，在训练过程中， 是从一个高斯分布  中 随机出来的，然后再测试过程中进行修正（有点像dropout的用法）。
数学表示如下：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-rrelu.png)
在测试阶段，把训练过程中所有的  取个平均值。NDSB 冠军的  是从  中随机出来的。那么，在测试阶段，激活函数就是就是： 
看看 cifar-100 中的实验结果：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-per.png)
## Maxout
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-mm.png)
Maxout出现在ICML2013上，作者Goodfellow将maxout和dropout结合后，号称在MNIST, CIFAR-10, CIFAR-100, SVHN这4个数据上都取得了start-of-art的识别率。 
Maxout 公式如下： 
假设  是2维，那么有： 
可以注意到，ReLU 和 Leaky ReLU 都是它的一个变形（比如， 的时候，就是 ReLU）.
Maxout的拟合能力是非常强的，它可以拟合任意的的凸函数。作者从数学的角度上也证明了这个结论，即只需2个maxout节点就可以拟合任意的凸函数了（相减），前提是”隐隐含层”节点的个数可以任意多.
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-maxout2.png)
所以，Maxout 具有 ReLU 的优点（如：计算简单，不会 saturation），同时又没有 ReLU 的一些缺点 （如：容易 go die）。不过呢，还是有一些缺点的嘛：就是把参数double了。
> 
还有其他一些激活函数，请看下表：
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-ac1.png)
![此处输入图片的描述](http://7pn4yt.com1.z0.glb.clouddn.com/blog-ac2.png)
# How to choose a activation function?
> 
怎么选择激活函数呢？
我觉得这种问题不可能有定论的吧，只能说是个人建议。
如果你使用 ReLU，那么一定要小心设置 learning rate，而且要注意不要让你的网络出现很多 “dead” 神经元，如果这个问题不好解决，那么可以试试 Leaky ReLU、PReLU 或者 Maxout.
友情提醒：最好不要用 sigmoid，你可以试试 tanh，不过可以预期它的效果会比不上 ReLU 和 Maxout.
还有，通常来说，很少会把各种激活函数串起来在一个网络中使用的。
# Reference
[1]. [http://www.faqs.org/faqs/ai-faq/neural-nets/part2/section-10.html](http://www.faqs.org/faqs/ai-faq/neural-nets/part2/section-10.html)
[2]. [http://papers.nips.cc/paper/874-how-to-choose-an-activation-function.pdf](http://papers.nips.cc/paper/874-how-to-choose-an-activation-function.pdf)
[3]. [https://en.wikipedia.org/wiki/Activation_function](https://en.wikipedia.org/wiki/Activation_function)
[4]. [http://cs231n.github.io/neural-networks-1/](http://cs231n.github.io/neural-networks-1/)
