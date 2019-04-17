# Dark Knowledge by Hinton - 家家的专栏 - CSDN博客





2014年11月21日 21:53:15[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：4688








视频：https://www.youtube.com/watch?v=EK61htlw8hY

PPT: http://www.ttic.edu/dl/dark14.pdf

分析材料：http://deepdish.io/2014/10/28/hintons-dark-knowledge/ 


                http://fastml.com/geoff-hintons-dark-knowledge/




Dark Knowledge。。当我看到这个题目的时候，感觉有点被吓到了的感觉。。

这几天真正静下心来看懂了整个思想。。。感觉是很接地气的一个东东。。




dark knownledge，可以看作是softmax function 之后的probability，这些probability包含着类别之间的相关性。

比如，猫和狗的相似性，要远远大于猫和船的相似性。。。

而这种相似性，会在probability中有所体现。。。

而这部分信息一致没有被很好的利用，所以称之为dark knowledge。。




dark knowledge主要利用在两个方面：

**一是：模型压缩。**  这个思想，早在2006 caruana提出过。在很多竞赛时，会利用不同的网络架构，不同的初始化或者不同的训练数据等，来学习到很多网络模型，然后combine                               多个model来提高performance。  如何训练一个模型，达到这个combine model的效果呢？  减小测试时间复杂度。

**二是：特殊网络的学习**。 类似于一种subcategory的思想，用kmean把容易混淆的一些类聚合在一起，然后针对某些易混淆的类别，进行特殊网络的学习。。当然要解决的问题便是overfitting策略，在hinton的ppt里面有讲到。




无论是模型压缩，还是在特殊网络学习，都提到了基于**soft target**和**hard target**两种目标函数。 也是整个ppt的中心吧。

一是**hard target**就是label信息，0 1 0 0

二是**soft target**是由已经训练好的model的出来的各个类别的probability信息。  比如：0.1 0.7 0.2 0.001。

       其中在soft target时，用到了raise the temperature策略，来提高soft target对应的cross entropy。




在进行模型压缩的时候，我们利用hard target 和soft target的loss function sum，来训练单一模型，努力达到很多模型combine的结果。

在进行特殊网路训练的时候，利用hard target loss来优化易混淆的某一子类，用soft target loss来优化其他所有类。




大致整理了一个简单的ppt，贴出来共享一下：

![](https://img-blog.csdn.net/20141122170703900?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

![](https://img-blog.csdn.net/20141122170712511?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


![](https://img-blog.csdn.net/20141122170721887?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141122170731605?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141122170756968?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141122170805875?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141122170801745?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141122170809045?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20141122170816955?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveWloYWl6aGl5YW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







