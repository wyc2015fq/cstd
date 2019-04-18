# 深度学习基础--不同网络种类--siamese网络(孪生网络) - wydbyxr的博客 - CSDN博客
2018年11月27日 09:53:26[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：371
# siamese网络(孪生网络)
  05年Yann Lecun提出来的。提起siamese network一般都会引用这两篇文章： 《Learning a similarity metric discriminatively, with application to face verification》和《 Hamming Distance Metric Learning》。
  Siamese也算是降维方法的一种。
  Siamese网络是一种相似性度量方法，当类别数多，但每个类别的样本数量少的情况下可用于类别的识别、分类等。传统的用于区分的分类方法是需要确切的知道每个样本属于哪个类，需要针对每个样本有确切的标签。而且相对来说标签的数量是不会太多的。当类别数量过多，每个类别的样本数量又相对较少的情况下，这些方法就不那么适用了。
  参考资料：
[http://blog.csdn.net/sxf1061926959/article/details/54836696](http://blog.csdn.net/sxf1061926959/article/details/54836696)
[http://blog.csdn.net/langb2014/article/details/53036216](http://blog.csdn.net/langb2014/article/details/53036216)
[http://blog.csdn.net/hjimce/article/details/50098483](http://blog.csdn.net/hjimce/article/details/50098483)
## 主要思想
  通过一个函数将输入映射到目标空间，在目标空间使用简单的距离（欧式距离等）进行对比相似度。在训练阶段去最小化来自相同类别的一对样本的损失函数值，最大化来自不同类别的一堆样本的损失函数值。
  它的特点是它接收两个图片作为输入，而不是一张图片作为输入。
  主要思想就是三点：
  1）输入不再是单个样本，而是一对样本，不再给单个的样本确切的标签，而且给定一对样本是否来自同一个类的标签，是就是0，不是就是1
  2）设计了两个一模一样的网络，网络共享权值W，对输出进行了距离度量，可以说l1、l2等。
  3）针对输入的样本对是否来自同一个类别设计了损失函数，损失函数形式有点类似交叉熵损失。
## 损失函数
  当X1和X2描述来自相同类别的时候，最小化损失函数；当X1和X2来自不同类别的时候，最大化。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127095209673.png)
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127095219816.png)
## 网络结构
  因为针对成对样本输入，这里两个相同的函数G，拥有一份相同的参数W，即这个结构是对称的，我们将它叫做siamese architecture。
  在这篇论文中，作者用这个网络去做面部识别，比较两幅图片是不是同一个人，而且这个网络的一个优势是可以去区分那些新的没有经过训练的类别的样本。
![在这里插入图片描述](https://img-blog.csdnimg.cn/20181127095251386.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=,size_16,color_FFFFFF,t_70)
