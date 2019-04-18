# R-CNN算法详解 - AI之路 - CSDN博客





2017年06月03日 11:24:40[AI之路](https://me.csdn.net/u014380165)阅读数：6707








**这是一篇比较早的Object Detection算法，发表在2014年的CVPR，也是R-CNN系列算法的开山之作，网上可以搜到很多相关的博客讲解，本篇博文没有按论文顺序来讲述，而是结合自己经验来看这个算法，希望给初学者一个直观的感受，细节方面不需要太纠结，因为很多部分在后来的算法中都改进了。**

论文：Rich feature hierarchies for accurate object detection and semantic segmentation

[论文链接](http://www.cv-foundation.org/openaccess/content_cvpr_2014/papers/Girshick_Rich_Feature_Hierarchies_2014_CVPR_paper.pdf)

## **一、解决的问题**

本文主要讲R-CNN（Regions with CNN features）这个算法，该算法是用来做object detection的经典算法，2014年提出。object detection的问题简单讲就是两方面：localization和recognition，即知道object在哪，以及这个object是什么。

R-CNN在pascal VOC 2012数据集上取得了mAP 53.3%的成绩，在当时已经很不错了。

## **二、算法简述**

本文数据集采用pascal VOC，这个数据集的object一共有20个类别。**首先用select search方法在每张图像上选取约2000个region proposal，region proposal就是object有可能出现的位置。然后根据这些region proposal构造训练和测试样本，注意这些region proposal的大小不一，另外样本的类别是21个（包括了背景）。然后是预训练，即在ImageNet数据集下，用AlexNet进行训练。然后再在我们的数据集上fine-tuning，网络结构不变（除了最后一层输出由1000改为21），输入是前面的region proposal进行尺寸变换到一个统一尺寸227*227，保留f7的输出特征2000*4096维。针对每个类别（一共20类）训练一个SVM分类器，以f7层的输出作为输入，训练SVM的权重4096*20维，所以测试时候会得到2000*20的得分输出，且测试的时候会对这个得分输出做NMS（non-maximun suppression），简单讲就是去掉重复框的过程。同时针对每个类别（一共20类）训练一个回归器，输入是pool5的特征和每个样本对的坐标即长宽。**

## **三、算法详解**

**训练过程：**

1、**准备region proposal**。对于训练集中的所有图像，采用selective search方式来获取，最后每个图像得到2000个region proposal。

2、**准备正负样本**。如果某个region proposal和当前图像上的所有ground truth中重叠面积最大的那个的IOU大于等于0.5，则该region proposal作为这个ground truth类别的正样本，否则作为负样本。另外正样本还包括了Ground Truth。因为VOC一共包含20个类别，所以这里region proposal的类别为20+1=21类，1表示背景。简单说下IOU的概念，IOU是计算矩形框A、B的重合度的公式：IOU=(A∩B)/(A∪B)，重合度越大，说明二者越相近。

3、**预训练**。这一步主要是因为检测问题中带标签的样本数据量比较少，难以进行大规模训练。采用的是Krizhevsky在2012年的著名网络AlexNet来学习特征，包含5个卷积层和2个全连接层，在Caffe框架下利用ILSVRC 2012的数据集进行预训练，其实就是利用大数据集训练一个分类器，这个ILSVRC 2012数据集就是著名的ImageNet比赛的数据集，也是彩色图像分类。

4、**fine-tuning**。将2中得到的样本进行尺寸变换，使得大小一致，这是由于2中得到的region proposal大小不一，所以需要将region proposal变形成227*227。本文中对所有不管什么样大小和横纵比的region proposal都直接拉伸到固定尺寸。然后作为3中预训练好的网络的输入，继续训练网络，继续训练其实就是迁移学习。另外由于ILSVRC 2012是一个1000类的数据集，而本文的数据集是21类（包括20个VOC类别和一个背景类别），迁移的时候要做修改，将最后一个全连接层的输出由1000改成21，其他结构不变。训练结束后保存f7的特征。

5、**针对每个类别训练一个SVM的二分类器**。输入是f7的特征，f7的输出维度是2000*4096，输出的是是否属于该类别，训练结果是得到SVM的权重矩阵W，W的维度是4096*20。这里负样本的选定和前面的有所不同，将IOU的阈值从0.5改成0.3，即IOU<0.3的是负样本，正样本是Ground Truth。IOU的阈值选择和前面fine-tuning不一样，这里链接3的解释是：前面fine-tuning需要大量的样本，所以设置成0.5会比较宽松。而在SVM阶段是由于SVM适用于小样本，所以设置0.3会更严格一点。

6、**回归**。用pool5的特征6*6*256维和bounding box的ground truth来训练回归，每种类型的回归器单独训练。输入是pool5的特征，以及每个样本对的坐标和长宽值。另外只对那些跟ground truth的IOU超过某个阈值且IOU最大的proposal回归，其余的region proposal不参与。具体参考链接3。详细说一下：对于某个region proposal：R，以及其对应的Ground truth：G，我们希望预测结果是：P，那么我们肯定希望P尽可能接近G。这里通过对pool5层的特征X做线性变换WX得到变换函数F（X），这些变换函数作用于R的坐标达到回归的作用（包括对x，y的平移以及对w，h的缩放）。因此损失函数可以表达为：R和G的差距减去P和G的差距要尽可能小。

**测试过程：**

1、输入一张图像，利用selective search得到2000个region proposal。

2、对所有region proposal变换到固定尺寸并作为已训练好的CNN网络的输入，得到f7层的4096维特征，所以f7层的输出是2000*4096。

3、对每个类别，采用已训练好的这个类别的svm分类器对提取到的特征打分，所以SVM的weight matrix是4096*N，N是类别数，这里一共有20个SVM，N=20注意不是21。得分矩阵是2000*20，表示每个region proposal属于某一类的得分。

4、采用non-maximun suppression（NMS）对得分矩阵中的每一列中的region proposal进行剔除，就是去掉重复率比较高的几个region proposal，得到该列中得分最高的几个region proposal。NMS的意思是：举个例子，对于2000*20中的某一列得分，找到分数最高的一个region proposal，然后只要该列中其他region proposal和分数最高的IOU超过某一个阈值，则剔除该region proposal。这一轮剔除完后，再从剩下的region proposal找到分数最高的，然后计算别的region proposal和该分数最高的IOU是否超过阈值，超过的继续剔除，直到没有剩下region proposal。对每一列都这样操作，这样最终每一列（即每个类别）都可以得到一些region proposal。

5、用N=20个回归器对第4步得到的20个类别的region proposal进行回归，要用到pool5层的特征。pool5特征的权重W是在训练阶段的结果，测试的时候直接用。最后得到每个类别的修正后的bounding box。

## **缺点**

**R-CNN流程较多，包括region proposal的选取，训练卷积神经网络（softmax classifier，log loss），训练SVM（hinge loss）和训练 regressor（squared loss），这使得训练时间非常长（84小时），占用磁盘空间也大。在训练卷积神经网络的过程中对每个region proposal都要计算卷积，这其中重复的太多不必要的计算，试想一张图像可以得到2000多个region proposal，大部分都有重叠，因此基于region proposal卷积的计算量太大，而这也正是之后Fast R-CNN主要解决的问题，留在下一篇博客讲解。**

参考资料：[http://blog.csdn.net/shenxiaolu1984/article/details/51066975](http://blog.csdn.net/shenxiaolu1984/article/details/51066975)
[http://www.robots.ox.ac.uk/~tvg/publications/talks/fast-rcnn-slides.pdf](http://www.robots.ox.ac.uk/~tvg/publications/talks/fast-rcnn-slides.pdf)
[http://www.jianshu.com/p/deb0f69f5597](http://www.jianshu.com/p/deb0f69f5597)
[http://blog.csdn.net/WoPawn/article/details/52133338](http://blog.csdn.net/WoPawn/article/details/52133338)



