# [Python人工智能] 一.神经网络入门及theano基础代码讲解 - 杨秀璋的专栏 - CSDN博客





2018年05月18日 15:17:11[Eastmount](https://me.csdn.net/Eastmount)阅读数：4946
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









从本篇文章开始，作者正式开始研究Python深度学习、神经网络及人工智能相关知识。第一篇文章主要讲解神经网络基础概念，同时讲解Theano库的安装过程及基础用法，主要结合 "[莫烦大神](http://study.163.com/provider/1111519/course.htm)" 的视频介绍，后面随着深入会讲解具体的项目及应用。基础性文章，希望对您有所帮助，也建议大家一步步跟着学习，同时文章中存在错误或不足之处，还请海涵~
同时推荐前面作者另外三个Python系列文章。
从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
- Python基础知识系列：[Pythonj基础知识学习与提升](http://blog.csdn.net/column/details/eastmount-python.html)
- Python网络爬虫系列：[Python爬虫之Selenium+Phantomjs+CasperJS](http://blog.csdn.net/column/details/eastmount-spider.html)
- Python数据分析系列：[知识图谱、web数据挖掘及NLP](http://blog.csdn.net/column/details/eastmount-kgdmnlp.html)

![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




## 一. 神经网络入门

该部分主要通过白话文的方式讲述神经网络，其中主要转载吴老的文章。链接：
[吴祖增前辈：神经网络入门(连载之一)](http://blog.csdn.net/zzwu/article/details/574931)
[吴祖增前辈：神经网络入门(连载之二)](http://blog.csdn.net/zzwu/article/details/575050)
    斯坦福机器学习视频NG教授 [https://class.coursera.org/ml/class/index](https://class.coursera.org/ml/class/index)
    书籍《游戏开发中的人工智能》、《游戏编程中的人工智能技术》
[神经网络和机器学习基础入门分享 - eastmount](https://blog.csdn.net/eastmount/article/details/49591349)


神经网络(也称人工神经网络，ANN)算法是80年代机器学习界非常流行的算法，不过在90年代中途衰落。现在，携着“深度学习”之势，神经网络重装归来，重新成为最强大的机器学习算法之一。


![](https://img-blog.csdn.net/20151103023058400)

人工神经网络（Artificial Neural Network，缩写ANN），是一种模仿生物神经网络的结构和功能的数学模型或计算模型。神经网络由大量的人工神经元联结进行计算。其来源于生物，故吴老先先讲述了生物神经网络的基础知识，从而进行引入。


![](https://img-blog.csdn.net/20151103023724578)


神经细胞通过轴突将信号传递给其他的神经细胞，通过树突向各个方向接受信号。
神经细胞利用电-化学过程交换信号。输入信号来自另一些神经细胞。这些神经细胞的轴突末梢（也就是终端）和本神经细胞的树突相遇形成突触（synapse），信号就从树突上的突触进入本细胞。
信号在大脑中实际怎样传输是一个相当复杂的过程，但就我们而言，重要的是把它看成和现代的计算机一样，利用一系列的0和1来进行操作。就是说，大脑的神经细胞也只有两种状态：兴奋（fire）和不兴奋（即抑制）。


![](https://img-blog.csdn.net/20151103024805255)

神经细胞利用一种我们还不知道的方法，把所有从树突突触上进来的信号进行相加，如果全部信号的总和超过某个阀值，就会激发神经细胞进入兴奋（fire）状态，这时就会有一个电信号通过轴突发送出去给其他神经细胞。如果信号总和没有达到阀值，神经细胞就不会兴奋起来。这样的解释有点过分简单化，但已能满足我们的目的。


![](https://img-blog.csdn.net/20151103025739605)

由于人脑具有一下几个特点：
1.能实现无监督的学习
    大脑能够自己进行学习，而不需要导师的监督教导。如果一个神经细胞在一段时间内受到高频率的刺激，则它和输入信号的神经细胞之间的连接强度就会按某种过程改变，使得该神经细胞下一次受到激励时更容易兴奋。
2.对损伤有冗余性(tolerance)
    大脑即使有很大一部分受到了损伤, 它仍然能够执行复杂的工作。
3.处理信息的效率极高
    神经细胞之间电-化学信号的传递，与一台数字计算机中CPU的数据传输相比，速度是非常慢的，但因神经细胞采用了并行的工作方式，使得大脑能够同时处理大量的数据。例如，大脑视觉皮层在处理通过我们的视网膜输入的一幅图象信号时，大约只要100ms的时间就能完成，眼睛并发执行。
4.善于归纳推广
    大脑和数字计算机不同，它极擅长的事情之一就是模式识别，并能根据已熟悉信息进行归纳推广(generlize)。例如，我们能够阅读他人所写的手稿上的文字，即使我们以前从来没见过他所写的东西。 
**5.它是有意识的**


![](https://img-blog.csdn.net/20151103032002251)


如下图所示，它表示的是一个人工神经细胞。其中：
输入(Input)；权重(Weight)：左边五个灰色圆底字母w代表浮点数；激励函数(Activation Function)：大圆，所有经过权重调整后的输入加起来，形成单个的激励值；输出(Output)：神经细胞的输出。

![](https://img-blog.csdn.net/20151103033007229)


进入人工神经细胞的每一个input(输入)都与一个权重w相联系，正是这些权重将决定神经网络的整体活跃性。假设权重为-1和1之间的一个随机数，权重可正可负（激发和抑制作用）。当输入信号进入神经细胞时，它们的值将与它们对应的权重相乘，作为图中大圆的输入。如果激励值超过某个阀值（假设阀值为1.0），就会产生一个值为1的信号输出；如果激励值小于阀值1.0，则输出一个0。这是人工神经细胞激励函数的一种最简单的类型。涉及的数学知识如下图所示：

![](https://img-blog.csdn.net/20151103033932054)


如果最后计算的结果激励值大于阈值1.0，则神经细胞就输出1；如果激励值小于阈值则输出0。这和一个生物神经细胞的兴奋状态或抑制状态是等价的。下面图是通过神经网络实现逻辑表达式与运算：（参考NG斯坦福机器学习讲义）

![](https://img-blog.csdn.net/20151103034617548)

可以看到x1和x2变量作为神经网络的输入，当它们取不同的0或1值时，其结果通过sigmod函数计算的值是不同的。它模拟了整个AND运算。


![](https://img-blog.csdn.net/20151103035354873)

该图中神经网络共有三层 ( 注输入层不是神经细胞，神经细胞只有两层 )：
输入层中的每个输入都馈送到了隐藏层，作为该层每一个神经细胞的输入；然后，从隐藏层的每个神经细胞的输出都连到了它下一层（即输出层）的每一个神经细胞。
注意：
1.图中仅仅画了一个隐藏层，作为前馈网络，一般地可以有任意多个隐藏层。但在对付你将处理的大多数问题时一层通常是足够的。
2.事实上，有一些问题甚至根本不需要任何隐藏单元，你只要把那些输入直接连结到输出神经细胞就行了。
3.每一层实际都可以有任何数目的神经细胞，这完全取决于要解决的问题的复杂性。但神经细胞数目愈多，网络的工作速度也就愈低，网络的规模总是要求保持尽可能的小。


![](https://img-blog.csdn.net/20151103040150028)

神经网络体系创建成功后，它必须接受训练来认出数字4，方法：
1.先把神经网络的所有权重初始化为任意值；
2.再给他一系列输入代表面板不同配置的输入，对每种输入配置，检查它的输出是什么，并调整相应权重；
3.如果我们送给网络的输入模式不是4，则我们知道网络应该输出一个0。因此每个非4字符时，网络权重应进行调整，使得它的输出趋向于0；当代表4的模式输送给网络时，则应把权重调整到使其输出趋向于1；
4.我们可以进一步识别0到9的所有数字或字母，其本质是手写识别的工作原理。
5.最后，网络不单能认识已经训练的笔迹，还显示了它有显著的归纳和推广能力。

正是这种归纳推广能力，使得神经网络已经成为能够用于无数应用的一种无价的工具，从人脸识别、医学诊断，直到跑马赛的预测，另外还有电脑游戏中的bot（作为游戏角色的机器人）的导航，或者硬件的robot（真正的机器人）的导航。


![](https://img-blog.csdn.net/20151103041044137)

![](https://img-blog.csdn.net/20151103041200953)


上图会演示神经网络在图像识别领域的一个著名应用，这个程序叫做LeNet，是一个基于多个隐层构建的神经网络。通过LeNet可以识别多种手写数字，并且达到很高的识别精度与拥有较好的鲁棒性。LeNet的发明人是机器学习的大牛Yann LeCun（目前google）。
右下方的方形中显示的是输入计算机的图像，方形上方的红色字样“answer”后面显示的是计算机的输出。左边的三条竖直的图像列显示的是神经网络中三个隐藏层的输出，可以看出，随着层次的不断深入，越深的层次处理的细节越低，例如层3基本处理的都已经是线的细节了。

这种类型的训练称作有监督的学习（supervised learnig），用来训练的数据称为训练集（training set）。调整权重可以采用许多不同的方法。对本类问题最常用的方法就是反向传播（backpropagation，简称backprop或BP）方法，即BP神经网络。
你自己可以去学习另外的一种训练方式，即根本不需要任何导师来监督的训练，或称无监督学习（unsupervised learnig）。下图是神经网络的简单回顾与总结：



![](https://img-blog.csdn.net/20151103042300580)


最后给大家看一个利用神经网络对图片进行分类的例子：过程就不详细论述了，图片很清晰，对人、汽车、摩托车、卡车进行图片识别，而具体的隐藏层函数需要大家去深入研究，我自己研究得也很浅显，抱歉~


![](https://img-blog.csdn.net/20151103042724702)


下面简单讲解"[莫烦大神](http://study.163.com/provider/1111519/course.htm)"网易云课程的一个示例。假设存在千万张图片，现在需要通过神经网络识别出某一张图片是狗还是猫，如下图所示共包括输入层、隐藏层（3层）和输出层。


![](https://img-blog.csdn.net/20180518140713902)



![](https://img-blog.csdn.net/20180518141122300)

计算机通过训练或强化学习判断猫，将获取的特征转换为数学的形式。首先得到一堆数字，通过判断处理得到另一堆数据，最终判断其是狗还是猫。比如第一次正确识别的只有10%，下面那只猫被识别成了狗，它会将识别错误的信息（与真实答案的差别）反向传递回神经网络，并修改神经元权重，为下次更好地识别。

![](https://img-blog.csdn.net/2018051814115340)


每一个神经元都有一个激励函数，被激励的神经元传递的信息最有价值，它也决定最后的输出结果，经过海量数据的训练，最终神经网络将可以用于识别猫或狗。





![](https://img-blog.csdn.net/20180518141209994)




参考资料包括NG教授的课程讲义和CSDN一位很厉害的女博主Rachel-Zhang：
[Stanford机器学习---第五讲. 神经网络的学习 Neural Networks learning](http://blog.csdn.net/abcjennifer/article/details/7758797)


![](https://img-blog.csdn.net/20151103042908475)







## 二. theano安装及入门

**1.基础知识**


Theano在深度学习框架中是祖师级的存在。它的开发始于2007，早期开发者包括传奇人物Yoshua Bengio和Ian Goodfellow。Theano基于Python，是一个擅长处理多维数组的库（这方面它类似于 NumPy）。当与其他深度学习库结合起来，它十分适合数据探索。它为执行深度学习中大规模神经网络算法的运算所设计。其实，它可以被更好地理解为一个数学表达式的编译器：用符号式语言定义你想要的结果，该框架会对你的程序进行编译，来高效运行于GPU或CPU。 

它与后来出现的Tensorflow功能十分相似（或者应该说，Tensorflow 类似 Theano ），因而两者常常被放在一起比较。它们本身都偏底层，同样的，Theano 像是一个研究平台多过是一个深度学习库。你需要从底层开始做许多工作，来创建你需要的模型。比方说，Theano 没有神经网络的分级。

下面开始讲解Theano的安装过程，主要调用 "pip install theano" 安装，如下图所示：


![](https://img-blog.csdn.net/2018051814383130)


安装成功之后，下面给出基础性的代码。这也是学习莫烦大神的云课程笔记，希望对您有所帮助。
莫烦大神地址：[http://study.163.com/course/courseLearn.htm?courseId=1003215006](http://study.163.com/course/courseLearn.htm?courseId=1003215006)



**2.存量变量使用**

首先导入相关包，然后定义存量x和y，再进行相加计算。由于theano需要用到cpu或gpu，所以其function定义比较特殊，核心代码为f = function([x,y],z)。


```python
#coding:utf-8
import numpy as np
import theano.tensor as T
from theano import function

#存量相加
x = T.dscalar('x')
y = T.dscalar('y')
z = x + y

#输入[x,y]列表 输出结果z
f = function([x,y],z) 

#调用函数 
print(f(2,3))

#to pretty-print the function
##查看z函数原型,输出(x+y)
from theano import pp
print(pp(z))
```

输出结果如下所示：

```python
5.0
(x + y)
```




**3.矩阵变量使用**

定义两个矩阵x和y，均为3行4列，其中x通过np.arange(12).reshape((3,4))代码定义，为0-11数字；y通过10*np.ones((3,4))代码定义，全部为10。


```python
#coding:utf-8
import numpy as np
import theano.tensor as T
from theano import function

#定义矩阵 matrix
x = T.dmatrix('x')
y = T.dmatrix('y')
z = x + y

#矩阵的加法
f = function([x,y],z)

#3行4列矩阵 y全部为10 x从0-11
print(u"输入矩阵")
print(np.arange(12).reshape((3,4)))
print(10*np.ones((3,4)))
print(u"输出结果")
print(f(np.arange(12).reshape((3,4)),
        10*np.ones((3,4))
        ))
```

计算结果如下图所示：

![](https://img-blog.csdn.net/20180518145338471)





如果需要实现乘法，则使用z = T.dot(x,y) 代码实现。




## 三. theano函数

theano运用CPU、GPU加速和并行计算，会自己定义function，下面详细讲解其函数功能。


**1.激励函数**
神经网络中的神经元中都有激励函数（activation function），这里我们定义逻辑回归的激励函数，常见的激励函数参考维基百科：[https://en.wikipedia.org/wiki/Activation_function](https://en.wikipedia.org/wiki/Activation_function)



![](https://img-blog.csdn.net/2018051815045137)
然后对一个2*2的矩阵进行计算，代码如下：

```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

#1.激励函数
#activation function example
x = T.dmatrix('x')
#计算概率 logistic or soft step
s = 1/(1+T.exp(-x)) #np.exp() 这里用的theano中的T.exp()
#输入x 输出s
logistic =theano.function([x],s)
#两行两列矩阵
print(logistic([[0,1],[2,3]]))
#wikipedia: Activation function
#输入0 输出s=1/(1+e0次方)=0.5
```

输出结果如下所示，比如当数值为0时，其结果为s=1/(1+e的0次方)=0.5。


```python
[[0.5        0.73105858]
 [0.88079708 0.95257413]]
```


**2.返回多个值**
输入为a和b变量，输出为差值、差值绝对值、差值平法和。核心代码如下：
f = theano.function([a,b],[diff,abs_diff,diff_squared])


```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

#2.返回多个值或结果
#multiply outputs for a function
a,b = T.dmatrices('a','b')
#定义两个的差的绝对值和平方
diff = a - b
abs_diff = abs(diff)   #差值绝对值
diff_squared = diff*2  
f = theano.function([a,b],[diff,abs_diff,diff_squared])

#[1,1,1,1] [0,1,2,3]
x1,x2,x3 = f(np.ones((2,2)),
        np.arange(4).reshape((2,2)))
print(x1)
print(x2)
print(x3)
```

输出结果如下所示：

```python
[[ 1.  0.]
 [-1. -2.]]
[[1. 0.]
 [1. 2.]]
[[ 2.  0.]
 [-2. -4.]]
```




**3.函数赋初值**

```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

#3.如何运用function名字更加方便
#name for a function
#定义存量
x,y,w = T.dscalars('x','y','w')
z = (x+y)*w
#定义默认值和名字
f = theano.function([x,theano.In(y,value=1),
                     theano.In(w, value=2, name='weight')],
                    z)
print(f(23,2))
#(23+2)*2=50
print(f(23,2,weight=4))
#(23+2)*4=100
```

输出结果如下所示：

```python
50.0
100.0
```

(By:Eastmount 2018-05-18 下午4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))






