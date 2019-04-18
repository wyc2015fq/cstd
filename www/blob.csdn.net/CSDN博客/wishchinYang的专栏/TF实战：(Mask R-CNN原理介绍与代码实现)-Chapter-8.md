# TF实战：(Mask R-CNN原理介绍与代码实现)-Chapter-8 - wishchinYang的专栏 - CSDN博客
2018年04月03日 15:28:50[wishchin](https://me.csdn.net/wishchin)阅读数：461
       这篇文章写的挺辛苦，不过对于Mask介绍着墨不多。
       文章： TF实战：[Chapter-8上(Mask R-CNN](https://blog.csdn.net/u011974639/article/details/78483779?locationNum=9&fps=1)介绍与实现) 
...............................................
     MRCNN采用和Faster R-CNN相同的两个阶段，具有相同的第一层(即RPN)，第二阶段，除了预测种类和bbox回归，并且并行的对每个RoI预测了对应的二值掩膜(`binary mask`)。示意图如下：
![mark](http://owv7la1di.bkt.clouddn.com/blog/171108/5J742G9cG7.png)
这样做可以将整个任务简化为mulit-stage pipeline，解耦了多个子任务的关系，现阶段来看，这样做好处颇多。
## 主要工作
### **损失函数的定义**
依旧采用的是多任务损失函数，针对每个每个RoI定义为
与Faster R-CNN的定义类似，这里主要看。
掩膜分支针对每个RoI产生一个2
的输出，**即K个分辨率为的二值的掩膜**，为分类物体的种类数目。依据预测类别分支预测的类型，只将第的二值掩膜输出记为
。 
掩膜分支的损失计算如下示意图：
- mask branch 预测
个种类的
二值掩膜输出依据种类预测分支(Faster R-CNN部分)预测结果：当前RoI的物体种类为
第个二值掩膜输出就是该RoI的损失- 
![mark](http://owv7la1di.bkt.clouddn.com/blog/171108/1BJ2IFEB0g.png)
对于预测的二值掩膜输出，我们对每个像素点应用sigmoid函数，整体损失定义为平均二值交叉损失熵。 
**引入预测**
**个输出的机制，允许每个类都生成独立的掩膜，避免类间竞争。这样做解耦了掩膜和种类预测**。不像是FCN的方法，在每个像素点上应用softmax函数，整体采用的多任务交叉熵，这样会导致类间竞争，最终导致分割效果差。
### **掩膜表示到RoIAlign层**
在Faster R-CNN上预测物体标签或bbox偏移量是将feature map压缩到FC层最终输出vector，压缩的过程丢失了空间上(平面结构)的信息，而掩膜是对输入目标做空间上的编码，直接用卷积形式表示像素点之间的对应关系那是最好的了。
输出掩膜的操作是不需要压缩输出vector，所以可以使用FCN(Full Convolutional Network)，不仅效率高，而且参数量还少。为了更好的表示出RoI输入和FCN输出的feature之间的像素对应关系，提出了RoIAlign层。
先回顾一下RoIPool层：
其核心思想是将不同大小的RoI输入到RoIPool层，RoIPool层将RoI量化成不同粒度的特征图（量化成一个一个bin），在此基础上使用池化操作提取特征。
下图是SPPNet内对RoI的操作，在Faster R-CNN中只使用了一种粒度的特征图：
![mark](http://owv7la1di.bkt.clouddn.com/blog/171108/djfmJE0HFg.png)
平面示意图如下：
![mark](http://owv7la1di.bkt.clouddn.com/blog/171108/E4cF3J308a.png)
这里面存在一些问题，在上面量操作上，实际计算中是使用的是16
，16的量化的步长，
是舍入操作(`rounding`)。这套量化舍入操作在提取特征时有着较好的鲁棒性(检测物体具有平移不变性等)，但是这很不利于掩膜定位，有较大负面效果。
针对这个问题，提出了RoIAlign层：避免了对RoI边界或bin的量化操作，在扩展feature map时使用双线性插值算法。这里实现的架构要看FPN论文：
![mark](http://owv7la1di.bkt.clouddn.com/blog/171108/k4LL4ADhKK.png)
一开始的Faster R-CNN是基于最上层的特征映射做分割和预测的，这会丢失高分辨下的信息，直观的影响就是丢失小目标检测，对细节部分丢失不敏感。受到SSD的启发，FPN也使用了多层特征做预测。这里使用的top-down的架构，是将高层的特征反卷积带到低层的特征(即有了语义，也有精度)，而在MRCNN论文里面说的双线性差值算法就是这里的top-down反卷积是用的插值算法。
### 总结
MRCNN有着优异的效果，除去了掩膜分支的作用，很大程度上是因为基础特征网络的增强，论文使用的是ResNeXt101+FPN的top-down组合，有着极强的特征学习能力，并且在实验中夹杂这多种工程调优技巧。
但是吧，MRCNN的缺点也很明显，需要大的计算能力并且速度慢，这离实际应用还是有很长的路，坐等大神们发力！
...................
请参考原文.............
