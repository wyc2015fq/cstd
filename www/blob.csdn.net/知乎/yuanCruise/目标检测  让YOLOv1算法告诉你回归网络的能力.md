# 目标检测 | 让YOLOv1算法告诉你回归网络的能力 - 知乎
# 

### 0.简介

这段时间YOLO系列算法很火，尤其是YOLOv3，很多大牛都复现了其在各种平台下的实现（tensorflow，pytorch，caffe...）。主要还是因为YOLOv3算法结合了很多有用的trick，兼顾了速度和精度。但究其本质仍然是回归算法，所以我们在这里先详细介绍下YOLOv1的实现细节，好了解回归算法的特性。网上有很多教程，但质量参差不齐，而且本人阅读起来很多细节比较难理解。因此还是打算总结一下，加深印象。后续还会写v2和v3的实现细节。我们主要分为三部分来介绍，算法整体结构+算法前向计算+算法训练过程。总体来说，YOLOv1算法的特点为算法结构简单，但具体实现细节比较多。

### 1.YOLO算法结构图

YOLO是利用一个简单的回归网络实现目标检测分类算法。直接放一张图来说明YOLO算法结构之简单。下图不是YOLO论文中的原始结构，而是用一个24层的网络替代了论文中原作者用的GoogleNet作为YOLO检测算法的特征提取器（我们可以用任意的分类网络替代，比如MobileNet，SqueezeNet，ShuffleNet等）。 
![](https://pic2.zhimg.com/v2-a25163c4296146293b040ee096bc40b9_b.jpg)
 那么有人会问这么简单的网络是怎么实现目标检测和分类的呢？其所有的秘密就在如下所示的相对复杂的损失函数中(所以损失函数才是深度学习任务的核心嘛，我一直这么认为)。详细细节我们下面展开讨论。（下述代码为网络最后一层的输出层，和损失函数层）

```
layer {
  name: "reg_reshape"
  type: "Reshape"
  bottom: "conv_reg"
  top: "regression"
  reshape_param {
    axis: 1
    shape {
      dim: 1470
    }
  }
}

layer {
  name: "det_loss"
  type: "DetectionLoss"
  bottom: "result"
  bottom: "label"
  top: "det_loss"
  loss_weight: 1
  detection_loss_param {
    side: 7
    num_class: 20
    num_object: 2
    object_scale: 1.0
    noobject_scale: 0.5
    class_scale: 1.0
    coord_scale: 5.0
    sqrt: true
    constriant: true
  }
}
```

### 2.利用YOLO作前向测试

### （1）简介

YOLO的前向测试为：将一张图片送入YOLO网络后网络输出检测结果的整体流程。从上述的代码结构中我们可以发现，网络最终的输出是一个1470维的向量，接下来我们将基于该向量来展开讨论这1470维向量和最终输出的检测分类结果之间的关系。

首先我们先把输出维度切分成1470 =（7*7）*（5+5+20）。这里用一张网上很常见的图来辅助解释这个过程。 
![](https://pic4.zhimg.com/v2-a21e822d4383ea248fef74de87af184b_b.jpg)

先解释下上述的（7*7）*（5+5+20）中各个数字的含义：在解释之前首先区分两个概念：cell(红色框) 和 bounding box(黄色框)。

先介绍cell，本网络输出49个cell（和最后的特征图大小对等）。 - （7*7）：表示将图片分成49个等大小的cell格子（如图红色格子所示）。

对于每个cell而言，网络输出2个bounding box，每个bounding box都有一个confidence。（注意confidence和confidence score的区别，因为我发现很多博客的介绍中没有严格区分。） - （5+5）：每一个5对应一个bounding box(黄色框)，其中前4个值表示boundxing box位置（x,y,w,h），后1个值表示当前框的置信度confidence。 - 20：代表了类别的概率，作者使用的数据集含有20个类别，因此该值为20，该值可以根据自己的数据类别进行定制。需要注意的是我们发现图中有两个黄色框（bounding box）但确只有一组概率值，因为概率值是针对于红色框（cell）而言的，即不论一个cell有多少个bounding box，它最终都只有一组类概率。

### 2.开始介绍测试过程：
- 1.计算每个bounding box的confidence score 在得到了1470维向量后，一共49个cell，我们对每一个cell的30维向量来分析，因为上面提到每个cell有两个bounding box却只有1组类概率（20个），那么为了使每个bounding box都能得到一个类概率，因此我们要对每个bounding box作如下的乘法操作。 
![](https://pic2.zhimg.com/v2-02e8b2df5732ec76fa0b1c15ab27eb99_b.png)
 Pr(Object)乘以IOU（bounding box和标签的IOU）就是上面提到的confidence，而Pr(Class|Object)就是上面提到的类概率。最终的乘积就是最终当前bounding box的confidence score（注意和confidence显然不是一个意思）。上式中乘以IOU，说明了IOU越大那么confidence score也越大，所以最终的confidence score结合了坐标最优和类别最优。上述公式中左边两项的乘法操作示意图如下。 
![](https://pic3.zhimg.com/v2-7d5bbbef50b5f7197dd4b47241b5f1e2_b.jpg)

最终对49个cell分别做上述操作后，可以得到如下图所示的98个bounding box且每一个box都会有一个对应的20维度的confidence score。下图中的每一列黄色的表示一个bounding box，下图中的每一行黑色的表示一种类别。 
![](https://pic3.zhimg.com/v2-09d2c1541a4f1870f07ef8fd88c34722_b.jpg)

-  2.基于阈值删除低分box 在该过程中，以每一个类别为单位，将confidence score低于0.2的值直接置为零（相当于删除）。伪代码如下图所示，阈值是一个超参数。代码中的i,j参照上图。相当于对于当前类别，将confidence score小于阈值的box过滤掉，剩下的分数高的box。 
![](https://pic2.zhimg.com/v2-7ce1b5989bf3f73d91f732ab043e1575_b.jpg)

-  3.利用NMS算法去重 利用非极大值抑制法，进一步剔除质量较差的框。非极大值抑制（Non-Maximum Suppression，NMS），顾名思义就是抑制不是极大值的元素，可以理解为局部最大搜索，保留局部最好的框，不好的如同阈值筛选一样置为零。示意图如下（来自网络） 
![](https://pic2.zhimg.com/v2-d279e4b1eefc95493a74a9f4a105fe29_b.jpg)

### 3.训练过程详系介绍

要了解训练过程主要就是了解两个要点：标签和损失函数。 - 1.标签

要介绍训练过程，对标签的理解是必须的。所以接下来介绍一下YOLO算法训练时候的标签。这有助于更好的理解YOLO的训练原理。在目标检测任务中我们输入的标签往往如下所示：第一个字段代表的是图片名，最后四个字段代表的是目标框位置（有多种表达方式，如相对于左上角的百分比，绝对位置等），第二个字段代表的是当前目标框中目标类别。第一行代表图1中有两个目标。YOLO也不例外，其标签也与之类似（当然具体示代码而定）。

```
0000001.jpg 1 72 79 232 273  2  67 59 155 161
0000002.jpg 2 67 59 155 161
```

通过上述给定的标签我们可以计算得到损失函数会用到的ground truth部分。在介绍损失函数的过程中会详细说到需要通过标签计算，并用于损失函数构建的部分。
- 2.损失函数

YOLO算法通过回归的方式实现目标检测和分类。 因此YOLO算法的损失函数分为两大部分：定位误差和分类误差。 其中分类误差又分为：置信度误差和类概率误差。 其中置信度误差又分为：含有物体的置信度误差和不含物体的置信度误差。 所以最终其损失函数如下图所示。

![](https://pic2.zhimg.com/v2-2ca43dccb76737858c8312071bc005a1_b.jpg)
 从图中的红色框我们可以发现，我们对每一个bounding box都要计算定位误差和置信度误差，而计算类概率误差是针对于每个cell的。 从图中的蓝色框我们可以发现，我们对每一个包含有目标的bounding box都会计算定位误差，置信度误差，且对包含目标的cell计算类概率误差。但是对于不包含目标的bounding box我们只需要计算置信度误差。（至于什么叫做bounding box是否包含目标，cell是否包含目标，见如下注释）。

>  1.怎么判断cell或bounding box是否包含目标？ 每一个目标位置就相当于我们输入的标签信息，有了该信息后，也就知道每个目标的中心坐标的位置。所以目标的中心坐标坐落于某个cell中，那么我们就说当前cell包含目标，随之也就说当前cell的两个bounding box也包含目标。我们发现只要cell包含了目标，其对应的两个bounding box一定就随之被认为也包含目标。所以在正常的描述中我们就不怎么将bounding box是否包含目标，我们只说cell是否包含目标。且这种情况下我们认为该目标由当前cell负责预测。

需要标签计算些什么呢？ 如上图的损失函数所示，一共有四组误差：每个bounding box的定位误差，cell包含物体时的confidence误差，cell不包含物体时的confidence误差，cell的类概率误差。所以与之对应的我们需要通过标签得到四个对应的ground truth，如下图所示，右边图中的四个选项都是可以通过输入标签计算得到的。 
![](https://pic1.zhimg.com/v2-99165a234bb9df405c4e395cf97bcd1c_b.jpg)

最后介绍一些小细节：

```
side: 7                 #将原始图片分成7*7的cell
    num_class: 20           #当前数据集一共有20个类别的目标
    num_object: 2           #每个cell都会有两个bounding box
    object_scale: 1.0 #计算损失函数时，包含object的bounding box的confidence loss系数
  noobject_scale: 0.5#计算损失函数时，不包含object的bounding box的confidence loss系数
                    #（因为不包含的多，所以讲系数设小防止其loss过大，影响整体训练）
    class_scale: 1.0       #计算损失函数时，每个cell所预测的类别概率误差系数
    coord_scale: 5.0       #计算损失函数时，每个bounding box的坐标误差系数
sqrt: true             #原始box的宽高（w，h）进行均方根转换     
 constriant: true       #
```

下面两段源码说明了下述的两个参数的含义，constriant表示将原始bounding box的中心点坐标（x，y）进行与grid cell对齐。sqrt表示将原始box的宽高（w，h）进行均方根转换。转换原因如下：

>  这里注意用宽和高的开根号代替原来的宽和高，这样做主要是因为相同的宽和高误差对于小的目标精度影响比大的目标要大。举个例子，原来w=10，h=20，预测出来w=8，h=22，跟原来w=3，h=5，预测出来w1，h=7相比，其实前者的误差要比后者小，但是如果不加开根号，那么损失都是一样：4+4=8，但是加上根号后，变成0.15和0.7。 （来自网络）

```
if (constriant_) {
          box[0] = (j % side_ + box[0]) / side_;
          box[1] = (j / side_ + box[1]) / side_;
        }
        if (sqrt_) {
          box[2] = pow(box[2], 2);
          box[3] = pow(box[3], 2);
        }
```

YOLO算法的细节比较多~若是有讲错没讲清楚的欢迎指出。

