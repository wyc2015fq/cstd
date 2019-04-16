# [Python人工智能] 七.加速神经网络、激励函数和过拟合 - 杨秀璋的专栏 - CSDN博客





2018年06月25日 23:56:12[Eastmount](https://me.csdn.net/Eastmount)阅读数：1404
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









从本系列文章开始，作者正式开始研究Python深度学习、神经网络及人工智能相关知识。前六篇文章讲解了神经网络基础概念、Theano库的安装过程及基础用法、theano实现回归神经网络、theano实现分类神经网络、theano正规化处理，这篇文章讲解加速神经网络训练的方法，为什么有激励函数以及过拟合，均是基础性知识。主要是学习"[莫烦大神](http://study.163.com/provider/1111519/course.htm)" 网易云视频的在线笔记，后面随着深入会讲解具体的项目及应用。基础性文章和在线笔记，希望对您有所帮助，本系列作者采用一篇基础一篇代码的形式讲解，也建议大家一步步跟着学习，同时文章中存在错误或不足之处，还请海涵~

["](http://study.163.com/provider/1111519/course.html)[莫烦大神](http://study.163.com/provider/1111519/course.htm)" 网易云视频地址：http://study.163.com/provider/1111519/course.html

同时推荐前面作者另外三个Python系列文章。

从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
- Python基础知识系列：[Pythonj基础知识学习与提升](http://blog.csdn.net/column/details/eastmount-python.html)
- Python网络爬虫系列：[Python爬虫之Selenium+Phantomjs+CasperJS](http://blog.csdn.net/column/details/eastmount-spider.html)
- Python数据分析系列：[知识图谱、web数据挖掘及NLP](http://blog.csdn.net/column/details/eastmount-kgdmnlp.html)

![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

前文参考：
[[Python人工智能] 一.神经网络入门及theano基础代码讲解](https://blog.csdn.net/eastmount/article/details/80363106)
[[Python人工智能] 二.theano实现回归神经网络分析](https://blog.csdn.net/eastmount/article/details/80390462)
[[Python人工智能] 三.theano实现分类神经网络及机器学习基础](https://blog.csdn.net/eastmount/article/details/80432844)
[[Python人工智能] 四.神经网络和深度学习入门知识](https://blog.csdn.net/eastmount/article/details/80499259)
[[Python人工智能] 五.theano实现神经网络正规化Regularization处理](https://blog.csdn.net/Eastmount/article/details/80536725)
[[Python人工智能] 六.神经网络的评价指标、特征标准化和特征选择](https://blog.csdn.net/eastmount/article/details/80650980)



PSS：最近参加CSDN2018年博客评选，希望您能投出宝贵的一票。我是59号，Eastmount，杨秀璋。投票地址：[https://bss.csdn.net/m/topic/blog_star2018/index](https://bss.csdn.net/m/topic/blog_star2018/index)

![](https://img-blog.csdnimg.cn/20190104155648543.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==,size_16,color_FFFFFF,t_70)



五年来写了314篇博客，12个专栏，是真的热爱分享，热爱CSDN这个平台，也想帮助更多的人，专栏包括Python、数据挖掘、网络爬虫、图像处理、C#、Android等。现在也当了两年老师，更是觉得有义务教好每一个学生，让贵州学子好好写点代码，学点技术，"师者，传到授业解惑也"，提前祝大家新年快乐。2019我们携手共进，为爱而生。



## 一. 为什么需要激励函数

首先，为什么需要激励函数（Activation Function）呢？

因为现实并没有我们想象的美好，激励函数是为了解决我们日常生活中不能用线性概括的问题而提出，如二分类问题。假设女生越漂亮，喜欢的男生人数越多，这是一个线性方程（Linear）问题，但假设场景发生在校园里，校园里男生人数有限，这就变成了一个非线性问题，并且女生不可能是无线漂亮的。


![](https://img-blog.csdn.net/20180621130114530?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



神经网络中的每个节点接受输入值，并将输入值传递给下一层，输入节点会将输入属性值直接传递给下一层（隐层或输出层）。在神经网络中，隐层和输出层节点的输入和输出之间具有函数关系，这个函数称为激励函数。



![](https://img-blog.csdn.net/20180621133250899?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



首先把整个神经网络简化为一个式子：**y = W · x**

其中，y称为预测值、W是参数、x称为输入值。那么，非线性方程如何描述这个问题呢？我们增加一个AF()函数，如下所示。


![](https://img-blog.csdn.net/20180621130418559?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



非线性方程：**y = AF( W · x )**

其中，AF就是所谓的激励函数。如下所示，常见的激励函数包括relu（x为-1时值为0，x为1时值为1）、sigmoid、tanh等。这些激励函数能够强行将原有线性结构弄弯，使输出结果具有非线性特征。

Sigmoid函数：是连续、可导、有界，关于原点对称的增函数，呈S形，具体可用反正切函数arctan或指数函数exp来实现，如f(x)=arctan(x)/(pi/2), f(x)=1/(1+e-x)。


![](https://img-blog.csdn.net/20180621130549790?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



我们甚至可以创造自己的激励函数，但需要保证它们是可以微分的，因为误差反向传递时，只有这些可微分的激励函数才能把误差传递回去（不太理解）。


![](https://img-blog.csdn.net/20180621131655865?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



当使用AF激励函数，如果神经层只有2-3层时，对于隐藏层使用任意激励函数都能掰弯直线，而当存在多层神经层时，随意选择AF会造成梯度爆炸、梯度消失等问题。

激励函数选择：

  1.少量神经层，多种选择；

  2.卷积神经网络，使用Relu；

  3.循环神经网络，使用Relu、Tanh。


![](https://img-blog.csdn.net/20180621132941338?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180621133005361?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







## 二. 什么是过拟合

实际生活中，机器学习过于自信，甚至自负，在自己的小圈子里非凡，但在大圈子里却处处碰壁。机器学习中的自负表现在哪些方面呢？


![](https://img-blog.csdn.net/20180621134724110?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180621134738282?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)







这条直线是希望机器学习学到的直线。假设蓝色直线与所有黄色点的总误差为10，有时机器过于追求误差小，它学到的可能是红色这条虚曲线，它经过了所有的数据点，误差为1。


![](https://img-blog.csdn.net/20180621134835877?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



可是，误差小真的好吗？当我们拿这个模型预测实际值时，如下图所示“+”号；这时，蓝色误差几乎不变，而红色误差突然升高，而红线不能表达除训练数据以外的数据，这就叫做过拟合。


![](https://img-blog.csdn.net/20180625225152242?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



同样，分类中的过拟合如下图所示，有黄色两个“+”号没有很好的被分隔，这就是过拟合在作怪。



![](https://img-blog.csdn.net/20180625225529269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



那么，怎么解决过拟合呢？
**方法一：增加数据量**

大多数过拟合的原因是数据量太小，如果有成千上万数据，红线也会被拉直，没有这么扭曲，所以增加数据量能在一定程度上解决过拟合问题。

**方法二：L1, L2 Regularization**

正规化是处理过拟合的常见方法，该方法适合大多数机器学习。

机器学习：y = W · x

其中，W是参数。过拟合中W往往变化太大，为了让变化不会太大，我们在计算误差时需要做些手脚。
**L1：cost = (Wx - real y)^2 + abs(W)**

=>L1正规化是预测值与真实值平方，加上W的绝对值

**L2：cost = (Wx - real y)^2 + (W)^2**

=>L2正规化是预测值与真实值平方，加上W的平方
**L3：加立方，L4：加四次方**


由于过度依赖的W会很大，我们在上述L1和L2公式中惩罚了这些大的参数。如果W变化太大，我们让cost也跟着变大，变成一种惩罚机制，把W自己也考虑进来，从而解决过拟合。



![](https://img-blog.csdn.net/20180625230006807?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**方法三：Droput Regularization**

该方法是训练时，随机忽略一些神经元和连接，神经网络会变得不完整，用不完整的神经网络训练一次，紧接着第二次再随机训练，忽略另一部分的神经元和连接，让每次结果不依赖特定的神经元，Droput从根本上解决过拟合。



![](https://img-blog.csdn.net/20180625231159710?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180625231215335?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)











## 三. 加速神经网络

Speed Up neural network training process

如何加速神经网络训练呢？

越复杂的神经网络，越多的数据，我们需要花费在神经网络上的时间就越多，其原因是计算量太大了，可是往往为了解决复杂的问题、复杂的结构和大数据，又是不可避免的。所以我们需要找一些方法，让神经网络变得聪明起来、快起来。



![](https://img-blog.csdn.net/20180625232407836?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180625232427521?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


**最简单方法：SGD（Stochastic Gradient Descent）**



假设红色方块是我们要训练的数据Data，如果使用普通的训练方法，需要重复的将整套数据放入神经网络NN中训练，这样消耗的计算资源会很大。我们需要换一种思路，将数据拆分成小批小批的，然后再分批不断放入NN中计算，这就是SGD的正确打开方式。



SGD每次使用批量数据，虽然不能反映整体数据的情况，但在很大程度上加速神经网络的训练过程，而且不会丢失太多准确率。



![](https://img-blog.csdn.net/20180625233015742?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



![](https://img-blog.csdn.net/20180625233027388?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



如果还是嫌SGD训练速度太慢，怎么办呢？

事实证明，SGD并不是训练最快的方法，图中红线是SGD的训练过程，时间相对较长，其他途径可以加速训练。




![](https://img-blog.csdn.net/20180625233952157?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




**方法二：Momentum**

大多数其他方法是在更新神经网络参数时动手脚，如下所示：





W += - Learning rate * dx

参数W的更新，是将原始W累加上一个负的学习效率（Learning rate）乘以校正值（dx），这种方法可能会让学习过程曲折无比，看起来就像一个喝醉酒的人回家时摇摇晃晃走了很多弯路，所以我们把这个人从平地上放到斜坡上，只要他往下坡的方向走一点点，由于向下的惯性，他走的弯路也会不自觉的变小 ，称为Momentum的方法，它的数学形式如下所示。



m = b1*m - Learning rate * dx

W += m



![](https://img-blog.csdn.net/20180625234226526?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



**方法三：AdaGrad**

该方法是在学习率上动手脚，使得每一个参数的更新都有与众不同的学习效率，它的作用和Momentum类似，不过不是给喝醉的人安排一个斜坡，而是给喝醉酒的人一双不好走的鞋子，使得他摇晃走路时，发现脚疼，鞋子变成了走弯路的阻力，逼着他往前直着走，它的数学形式如下。



v += dx^2

W += -Learning rate * dx / √v


![](https://img-blog.csdn.net/20180625234818286?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




**方法四：RMSProp**

如果把下坡和不好走的鞋子结合起来，是不是就更好呢？我们这就有了RMSProp方法，通过Momentum的惯性原则，加上AdaGrad对错误发生了阻力，就合并成如下所示的RMSProp，同时具有两则优点。





![](https://img-blog.csdn.net/20180625234847856?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




不过细心的同学可能发现了RMSProp中少了些什么？原来我们是没有完全合并Momentum，少了Momentum中的“-Learning rate * dx”部分，所以我们会在Adam中补上这种想法。

计算m时有Momentum的下坡属性，计算v时有AdaGrad的阻力属性 ，然后再更新参数时，把m和v都考虑进去。实验证明，大多数使用Adam都能又快又好的达到目标，迅速收敛，所以在加速神经网络训练时，一个下坡，一双破鞋，就能实现。



![](https://img-blog.csdn.net/20180625235334303?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L0Vhc3Rtb3VudA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





基础性文章，希望对您有所帮助，推荐大家阅读莫烦大神的学习视频，也建议大家一步步跟着学习，同时文章中存在错误或不足之处，还请海涵~真是太忙了，有时候挤点时间出来写文章，才能让自己放平心态，谁又能写一辈子博客呢？又有几个人能做自己喜欢的事情，哎。看到学生也开始写文章，还是高兴的。

(By:Eastmount 2018-06-25 晚上12点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))





