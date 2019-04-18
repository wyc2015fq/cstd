# OpenCV中的Haar+Adaboost级联分类器分解（二）：级联分类器结构与XML文件含义 - YZXnuaa的博客 - CSDN博客
2018年05月08日 17:04:00[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：116
                众所周知，OpenCV中的Adaboost级联分类是树状结构，如图1，其中每一个stage都代表一级强分类器。当检测窗口通过所有的强分类器时才被认为是目标，否则拒绝。实际上，不仅强分类器是树状结构，强分类器中的每一个弱分类器也是树状结构。
![](https://img-blog.csdn.net/20160425143637082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15)
图1 强分类器和弱分类器示意图
    这篇文章将结合OpenCV-2.4.11中自带的haarcascade_frontalface_alt2.xml文件介绍整个级联分类器的结构。需要说明，自从2.4.11版本后所有存储得XML分类器都被替换成新式XML，所以本文对应介绍新式分类器结构。
### （一）XML的头部
    在了解OpenCV分类器结构之前，先来看看存储分类器的XML文件中有什么。图2中注释了分类器XML文件头部信息，括号中的参数为opencv_traincascade.exe训练程序对应参数，即训练时设置了多少生成的XML文件对应值就是多少（如果不明白，可以参考我的前一篇文章）。
![](https://img-blog.csdn.net/20160425143800426?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15)
图2 分类器XML文件头部含义
### （二）弱分类器结构
    之前看到有一部分文章将Haar特征和弱分类器的关系没有说清楚，甚至有些还把二者弄混了。其实Haar特征和弱分类器之间的关系很简单：
一个完整的弱分类器包含：Haar特征
+leftValue + rightValue + 弱分类器阈值(threshold)
这些元素共同构成了弱分类器，缺一不可。haarcascade_frontalface_alt2.xml的弱分类器深度为2，包含了2种形式，如图3。图3中的左边形式包含2个Haar特征、1个leftValue、2个rightValue和2个弱分类器阈（t1和t2）；左边形式包括2个Haar特征、2个leftValue、1个rightValue和2个弱分类器阈。﻿﻿
![](https://img-blog.csdn.net/20160425143819082?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15)
图3 OpenCV树状弱分类器示意图
    看图3应该明白了弱分类器的大致结构，接下来我们了解树状弱分类器是如何工作的。还是以图3左边的形式为例：
    1.计算第一个Haar特征的特征值haar1，与第一个弱分类器阈值t1对比，当haar1<t1时，进入步骤2；当haar1>t1时，该弱分类器输出rightValue2并结束。   
    2.计算第二个Haar特征值haar2，与第二个弱分类器阈值t2对比，当haar2<t2时输出leftValue；当haar2>t2时输出rightValue1。   
    即通过上述步骤计算弱分类器输出值，这与OpenCV的cascadedetect.hpp文件中的predictOrdered()函数代码对应（这里简单解释一下，在OpenCV中所有弱分类器的leftValue和rightValue都依次存储在一个一维数组中，代码中的leafOfs表示当前弱分类器中leftValue和rightValue在该数组中存储位置的偏移量，idx表示在偏移量leafOfs基础上的leftValue和rightValue值的索引，cascadeLeaves[leafOfs
-idx]就是该弱分类器的输出）：
**[cpp]**[view plain](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15#)[copy](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15#)
- do
-             {  
-                CascadeClassifier::Data::DTreeNode& node = cascadeNodes[root + idx];  
- double val =featureEvaluator(node.featureIdx);  
-                 idx = val < node.threshold ?node.left : node.right;  
-             }  
- while( idx > 0 );  
-             sum += cascadeLeaves[leafOfs -idx];  
    看到这里，你应该明白了弱分类器的工作方式，即通过计算出的Haar特征值与弱分类器阈值对比，从而选择最终输出leftValue和rightValue值中的哪一个。   
        那么第三个问题来了，这些Haar特征、leftValue、rightValue和弱分类器阈值t都是如何存储在xml文件中的？不妨来看haarcascade_frontalface_alt2.xml文件中的第一级的第三个弱分类器，如图4。图4中的弱分类器恰好是图3中左边类型，包含了<internalNodes>和<leafValues>两个标签。其中<leafValues>标签中的3个浮点数由左向右依次是rightValue2、leftValue和rightValue1（具体顺序参考下文图示）；而<internalNodes>中有6个整数和2个浮点数，其中2个浮点数依次分别是弱分类器阈值t1和t2，剩下的6个整数容我慢慢分解。   
        首先来看两个浮点数前的整数，即4和5。这两个整数用于标示所属本弱分类器Haar特征存储在<features>标签中的位置。比如数值4表示该弱分类器的haar1特征存储在xml文件下面<features>标签中第4个位置，即为：   
    （<rects>标签里面的5个数对应Haar特征的<x y width height weight>，请参考上一篇文章）。<internalNodes>的其他4个整数1、0、-1和-2则用于控制弱分类器树的形状，即OpenCV会把1赋值给当前的node.left，并把0赋值给node.right。请注意do-while代码中的条件，只有idx<=0时才停止循环，参考图3应该可以理解这4个整数的含义。如此，OpenCV通过这些巧妙的数值和结构，控制了整个分类器的运行（当然我举的例子alt2的弱分类器树深度为2，相对比较复杂，其他如alt等深度为1的分类器则更加简单）。其他弱分类器请类推。   
![](https://img-blog.csdn.net/20160425143831473?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15)
图4 OpenCV弱分类器运行示意图
        可以看到，每个弱分类器内部都是类似于这种树状的“串联”结构，所以我称其为“串联组成的的弱分类器”。（需要说明，本文为了介绍原理，选用了深度为2的分类器。而深度为1的分类器，如haarcascade_frontalface_alt.xml，则相比较简单，可以通过类比了解，不再赘述）﻿﻿   
### （三）强分类器结构
    在OpenCV中，强分类器是由多个弱分类器“并列”构成，即强分类器中的弱分类器是两两相互独立的。在检测目标时，每个弱分类器独立运行并输出cascadeLeaves[leafOfs
-idx]值，然后把当前强分类器中每一个弱分类器的输出值相加，即：
sum+= cascadeLeaves[leafOfs - idx];﻿﻿
![](https://img-blog.csdn.net/20160425143847361)![](https://blog.csdn.net/u012621115/article/details/51242029?locationNum=15)
图5 OpenCV强分类器运行示意图
之后与本级强分类器的stageThreshold阈值对比，当且仅当结果sum>stageThreshold时，认为当前检测窗口通过了该级强分类器。当前检测窗口通过所有强分类器时，才被认为是一个检测目标。
    可以看出，强分类器与弱分类器结构不同，是一种类似于“并联”的结构，我称其为“并联组成的强分类器”。
#### （四）级联分类器
    通过之前的介绍，到这应该可以理解OpenCV中：由弱分类器“并联”组成强分类器，而由强分类器“串联”组成级联分类器。那么还剩最后一个内容，那就是检测窗口大小固定（例如alt2是20*20像素）的级联分类器如何遍历图像，以便找到在图像中大小不同、位置不同的目标。
1. 为了找到图像中不同位置的目标，需要逐次移动检测窗口（随着检测窗口的移动，窗口中的Haar特征相应也随着窗口移动），这样就可以遍历到图像中的每一个位置；
2. 而为了检测到不同大小的目标，一般有两种做法：逐步缩小图像or逐步放大检测窗口。缩小图像就是把图像长宽同时按照一定比例（默认1.1 or 1.2）逐步缩小，然后检测；放大检测窗口是把检测窗口长宽按照一定比例逐步放大，这时位于检测窗口内的Haar特征也会对应放大，然后检测。一般来说，如果用软件实现算法，则放大检测窗口相比运行速度更快。
