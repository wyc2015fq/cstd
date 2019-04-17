# Pytorch - 长歌行 - CSDN博客





2017年11月26日 20:03:02[iuhsihsow](https://me.csdn.net/iuhsihsow)阅读数：1066








2017年6月份做的一个ppt：




![](https://img-blog.csdn.net/20171126195735985?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171126195751046?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```
http://pytorch.org/
https://zhuanlan.zhihu.com/p/24953160
2017 年初

开发团队表示，除 Facebook之外，它还已经被推特、卡内基梅隆大学和 Salesforce 等机构采用。

Beta is here.
@soumith soumith released this on Feb 2
本周，Facebook 的 AI 研究团队发布了一个 Python 工具包，专门针对 GPU 加速的深度神经网络（DNN）编程。
```



![](https://img-blog.csdn.net/20171126195807185?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171126195838060?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
（GPU是怎么样起作用的，numpy）
（图是什么？）

加入了autograd，不用自己写backward了
可以动态生成computational graph，对NLP一些model来说这个特性很有用
集中管理内存，和Torch比可以省不少内存
Python好用库太多了，多进程啥的搞起来也方便，Lua基本上啥都得自己写
更容易debug。PyTorch能够逐层打印出计算结果
支持动态图的创建。简单来说动态图模型允许你在运行程序的时候动态去修正你的模型结构，来处理各种奇奇怪怪的边角输入，这在学术研究的时候的灵活性就体现出来了。
运行在 GPU 或 CPU 之上、基础的张量操作库，
内置的神经网络库
模型训练功能
支持共享内存的多进程并发（multiprocessing ）库。PyTorch 开发团队表示：这对数据载入和 hogwild 训练十分有帮助。
```



![](https://img-blog.csdn.net/20171126195850767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
http://pytorch.org/about/
https://discuss.pytorch.org/t/does-it-support-multi-gpu-card-on-a-single-node/75
The biggest difference between a numpy array and a PyTorch Tensor is that a PyTorch Tensor can run on either CPU or GPU. To run operations on the GPU, just cast the Tensor to a cuda datatype.

https://blogs.nvidia.com/blog/2012/09/10/what-is-cuda-2/

torch.cuda.is_available()  #看看是否支持cuda
dtype = torch.FloatTensor 
# dtype = torch.cuda.FloatTensor # Uncomment this to run on GPU
>>>x = x.cuda()
>>>y = y.cuda()
>>>x+y   
http://pytorch.org/docs/notes/cuda.html#cuda-semantics

仅有CNTK这一种深度学习库通过Microsoft特殊的 1-bit量化并行算法（效率较高）和块动量算法（效率很高），能在多个GPU和多台计算机上高效地执行算法。（参见CNTK Parallel 1-bit SGD,需要安装微软MPI或OpenMPI。）在96颗GPU的集群上用CNTK，可以获得90-95倍的速度提升。

CNTK，Torch或Pytorch。速度可提升3.6-3.8倍。这些库包含了一些算法，能在4颗GPU的单机上并行执行。 其他支持并行的库，要么慢（比如TensorFlow能加速2-3倍）
```



![](https://img-blog.csdn.net/20171126195905140?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

```
http://pytorch.org/static/img/dynamic_graph.gif


Most frameworks such as TensorFlow, Theano, Caffe and CNTK have a static view of the world. One has to build a neural network, and reuse the same structure again and again. Changing the way the network behaves means that one has to start from scratch.

With PyTorch, we use a technique called Reverse-mode auto-differentiation, which allows you to change the way your network behaves arbitrarily with zero lag or overhead. Our inspiration comes from several research papers on this topic, as well as current and past work such as autograd, autograd, Chainer, etc.
```



![](https://img-blog.csdn.net/20171126195917470?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
https://link.zhihu.com/?target=http%3A//cs231n.stanford.edu/slides/2017/cs231n_2017_lecture8.pdf

https://www.zhihu.com/question/54914188
https://www.zhihu.com/question/59274399
Torch
Tensorflow

如果你用Theano，建议转向TensorFlow，或者PyTorch，后者更灵活一些。
如果你用Torch，强烈建议转向PyTorch。已有模型可以考虑torch2caffe来部署。
如果你用TensorFlow，开心就好，performance的确是个问题，但是毕竟社区好。
如果你想认真学machine learning，那请不要用keras，我一般收到的反馈是，keras做简单的东西容易，一旦你要做点真research，就很难改，因为包装太多。
作者：知乎用户链接：https://www.zhihu.com/question/59274399/answer/163537353来源：知乎著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。
```



![](https://img-blog.csdn.net/20171126195927116?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171126195936333?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171126195939812?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
You can pick out bright spots off the main axis that show which languages it guesses incorrectly, e.g. Chinese for Korean, and Spanish for Italian. It seems to do very well with Greek, and very poorly with English (perhaps because of overlap with other languages).
```



![](https://img-blog.csdn.net/20171126195943794?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
作者：知乎用户链接：https://www.zhihu.com/question/55720139/answer/147148105来源：知乎著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。第一步 github的 tutorials 尤其是那个60分钟的入门。只能说比tensorflow简单许多, 我在火车上看了一两个小时就感觉基本入门了. 另外jcjohnson 的Simple examples to introduce PyTorch 也不错 
第二步 example 参考 pytorch/examples 实现一个最简单的例子(比如训练mnist )。
第三步 通读doc PyTorch doc 尤其是autograd的机制，和nn.module ,optim 等相关内容。doc还不是很完善，但是绝对够用.
第四步 论坛讨论 PyTorch Forums 。论坛帖不是很多，但是质量很高，pytorch的维护者(作者)回帖很及时的。每天刷一刷帖可以少走很多弯路，避开许多陷阱,消除很多思维惯性.还能从作者那里学会如何写出bug-free clean and elegant 的代码。 
第五步 阅读源代码 fork pytorch，pytorch-vision等。相比其他框架，pytorch代码量不大，而且抽象层次没有那么多，很容易读懂的。通过阅读代码可以了解函数和类的机制，此外它的很多函数,模型,模块的实现方法都如教科书般经典。
```



![](https://img-blog.csdn.net/20171126195947158?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171126195950376?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


![](https://img-blog.csdn.net/20171126195953667?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)




```
模型
hadoop和spark
算法
较强的理论知识，能推导所有公式

观察数据--->找特征--->设计算法--->算法验证--->洗数据--->工程化--->上线看效果--->goto 观察数据

观察力，归纳能力，想象力，经验值，数学抽象能力，工程化
数据敏感性，观察力
数学抽象能力，数学建模能力和数学工具的熟练使用的能力
能随手编脚本代码的能力，强大的计算机算法编程能力，高级开发工程师的素质
想象力，耐性和信心，较强的语言表达能力，抗打击能力
```



![](https://img-blog.csdn.net/20171126195957846?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdW5pbnRlcnJ1cHRlZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







