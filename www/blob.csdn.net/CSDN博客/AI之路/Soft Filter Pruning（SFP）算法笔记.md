# Soft Filter Pruning（SFP）算法笔记 - AI之路 - CSDN博客





2018年07月18日 23:14:42[AI之路](https://me.csdn.net/u014380165)阅读数：2409








论文：Soft Filter Pruning for Accelerating Deep Convolutional Neural Networks 

论文链接：[https://www.ijcai.org/proceedings/2018/0309.pdf](https://www.ijcai.org/proceedings/2018/0309.pdf)

代码链接：[https://github.com/he-y/soft-filter-pruning](https://github.com/he-y/soft-filter-pruning)
**这篇是IJCAI 2018的关于模型加速压缩的文章**，思想简洁而且有效，主要是和Hard Filter Pruning（HFP）做对比，HFP是比较常见的剪枝方式，一般是按照某些指标对卷积核进行排序，然后直接剪掉不符合指标的卷积核，然后做fine tune，fine tune的时候网络中就不包含那些被剪掉的卷积核。这篇文章主要提出了通过Soft Filter Pruning (SFP)做模型加速，SFP和HFP的不同点在于剪掉的卷积核依然参与下一次迭代更新，而并不是剪掉一次就永远没有了，而且SFP在不采用fine tune的情况下依然能够有不错的效果，因为SFP在每个epoch结束后会进行剪枝，剪枝后就会再训一个epoch，然后继续剪枝，这种方式显然也比HFP省时间。之所以要采用SFP，很重要的一个原因是为了提高模型的效果，因为HFP那种方式相当于直接丢掉剪掉的卷积核，而SFP相对而言没那么极端，这也是这两种剪枝方式名称中soft和hard的含义。

**Figure1是关于Hard Filter Pruning和Soft Filter Pruning的对比图。Hard Filter Pruning**是直接删掉被剪枝的卷积核，虽然为了减少剪枝带来的效果丢失，在剪枝后还会做一些fine tune的工作，但是性能很难恢复到剪枝前的水平。**Soft Filter Pruning**在剪枝后进行训练时，上一个epoch中被剪掉的卷积核在当前epoch训练时仍参与迭代，因此那些卷积核不会被直接丢弃。 
![这里写图片描述](https://img-blog.csdn.net/2018071823114054?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**SFP算法示意图如Algorithm 1所示**。总体而言包含两个循环结构，第一个循环是epoch，其实就是常规的训练过程，每次训完一个epoch就要开始执行剪枝操作。第二个循环是遍历网络的每一层，计算每一层的每个卷积核的L2-norm（L2范数），剪枝概率Pi和第i层的卷积核数量Ni+1相乘表示一个网络层中被剪枝的卷积核数量，将卷积核的L2范数最低的Ni+1*Pi个卷积核剪枝，剪枝是通过将该卷积核的值置为0来实现，从而完成第i层卷积核的剪枝操作。这样随着epoch的迭代，最终得到的模型会包含一些值为0的卷积核，这些卷积核就可以直接去掉从而得到最终的模型。 
![这里写图片描述](https://img-blog.csdn.net/20180718231201818?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Lp-norm（Lp范数）的式子如公式2所示，实际中用的是L2-norm，也就是p等于2。 
![这里写图片描述](https://img-blog.csdn.net/20180718231214734?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**Figure2是Soft Filter Pruning（SFP）的示意图**。（a）表示剪枝之前的某个网络层的卷积核情况和对应的L2范数，L2范数比较低的用紫色表示。（b）表示对L2范数比较低的一些卷积核执行剪枝操作，也就是将卷积核的值置为0。（c）表示基于剪枝后的网络执行下一个epoch的迭代更新，和Hard Filter Pruning不同的是这里依然会对前面置零的卷积核执行更新操作，也就是参与此次迭代更新的卷积核数量和上一个epoch一模一样，只不过两次迭代前数值为0的卷积核不完全相同而已。 
![这里写图片描述](https://img-blog.csdn.net/20180718231234804?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
文中同时给出了关于具体能减少多少计算量，这里也简单分析一下。假设第i层的剪枝概率是Pi，第i层的输入特征通道数是Ni，输出特征通道数是Ni+1，那么就意味着剪枝时要将Ni+1*Pi个卷积核置零，也就是说仅有Ni+1*(1-Pi)个卷积核是有效的。假设第i层的输入特征大小是Hi*Wi，输出特征大小是Hi+1*Wi+1，那么剪枝以后第i层的输出特征维度就从 
![这里写图片描述](https://img-blog.csdn.net/20180718231250155?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

变成 
![这里写图片描述](https://img-blog.csdn.net/20180718231308312?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

如果继续对第i+1层做剪枝，剪枝概率用Pi+1表示，剪枝前第i+1层的计算量是 
![这里写图片描述](https://img-blog.csdn.net/20180718231323930?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

剪枝后的计算量变成 
![这里写图片描述](https://img-blog.csdn.net/20180718231339309?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

这两个式子也非常容易理解，首先第一个式子中，Ni+2*Hi+2*Wi+2表示第i+1层的输出特征大小，这其中每个点都要通过Ni+1*k^2个计算量才能得到，所以就有了上面这个剪枝之前第i+1层的计算量。至于剪枝后的计算量，主要差别就在于输入特征通道数和输出特征通道数的差异，输入特征通道数变成Ni+1*(i-Pi)，这个根据第i层的剪枝概率Pi和原输出特征通道数Ni+1就能得到，而输出特征通道数变成Ni+2*(1-Pi+1)，这个根据第i+1层的剪枝概率Pi+1和原输出特征通道数Ni+2就能得到，因此就有了上面这个剪枝后第i+1层的计算量。通过相除就能得到剪枝前后计算量的倍数，再用1减去该倍数就得到减少的计算量，如下所示。 
![这里写图片描述](https://img-blog.csdn.net/20180718231353922?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
**实验结果：**

Table1是在CIFAR-10数据集上的剪枝效果对比，网络都是采用ResNet系列。 
![这里写图片描述](https://img-blog.csdn.net/2018071823141082?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
Table2是在ImageNet数据集上的剪枝效果对比，网络也是采用ResNet系列。注意对于ResNet-50而言，基于fine tune的SFP效果下降比较明显。 
![这里写图片描述](https://img-blog.csdn.net/20180718231426252?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3UwMTQzODAxNjU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
文中还有一些关于Lp范数的选择、剪枝率的选择、迭代次数的选择所带来的实验效果差异对比。另外一点想要强调的是关于不同层用不同剪枝率的问题，这篇文章主要采用的还是一整个网络的每个层都用相同的剪枝概率，但其实不同层用不同剪枝概率的做法在剪枝中还是比较常见的，只不过这种额外的超参数要么手动设置，要么就丢给网络自主学习（比如Network sliming那篇是整个网络设置一个剪枝率），可能这篇文章偏向简洁，所以暂不引入过多的超参数吧。











