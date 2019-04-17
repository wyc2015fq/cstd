# 人脸检测（十七）--Faceness-Net - 工作笔记 - CSDN博客





2018年06月01日 10:07:13[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：7139
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









转自：https://blog.csdn.net/zhangjunhit/article/details/78337570?locationnum=4&fps=1

Faceness-Net: Face Detection through Deep Facial Part Responses PAMI2017

From Facial Parts Responses to Face Detection: A Deep Learning Approach ICCV2015
[http://shuoyang1213.me/projects/Faceness/Faceness.html](http://shuoyang1213.me/projects/Faceness/Faceness.html)
[https://pan.baidu.com/s/1qWFwqFM](https://pan.baidu.com/s/1qWFwqFM) Password: 4q8y

本文针对人脸检测问题，从 facial attributes 的角度来解决人脸检测中的 severe occlusions 和 large pose variation

在人脸检测中使用 facial attributes supervision 的好处：

1）Discovering facial parts responses supervised by facial attributes 人脸具有唯一的结构

2） Computing faceness score from responses configurations 由人脸的局部到整体的推导

3） Refining the face hypotheses 人脸候选区域的进一步验证





![](https://img-blog.csdn.net/20180601101225622)

![](https://img-blog.csdn.net/20180601101438339)

本文和我们以前的工作【24,25】相比较，主要的改进在于设计了一个更有效的CNN，提高了速度和精度，具体改进有：

1）以前我们是用独立的网络来学习各个 facial parts，现在 我们共享了这些部件特征， share feature representations between these attribute-aware networks，这么做降低网络的参数量（( ∼ 83% fewer parameters），同时提升了网络的鲁棒性

2）候选区域提取模块嵌入到整个网络中，而不是独立的模块 This design not only leads to improved computation efficiency but also higher recall rate

3）更多的分析和实验

3 Faceness-Net
![](https://img-blog.csdn.net/20180601101447160)

算法的整个流程是首先进行人脸局部特征的检测，然后从局部到整体得到人脸候选区域，再对人脸候选区域进行人脸识别和矩形框坐标回归





First stage

每个人脸局部特征我们使用一个CNN来检测 attribute-aware networks，一共五个特征属性： hair, eyes, nose, mouth, and beard

有了局部特征属性的响应，有局部推理出人脸候选区域 

![](https://img-blog.csdn.net/20180601101456518)

Second stage

训练一个多任务的CNN来完成人脸二分类和矩形框坐标回归 face classification and bounding box regression are jointly optimized





3.1 Attribute-Aware Networks

Network structure 网络结构 

![](https://img-blog.csdn.net/20180601101512294)





Shared representation 特征共享 网络的前半部分网络参数基本一样，所以可以实现共享， 降低网络参数量 83% 

![](https://img-blog.csdn.net/2018060110152626)





3.2 Learning to Generate Partness Maps
![](https://img-blog.csdn.net/20180601101534609)

![](https://img-blog.csdn.net/20180601101547485)

人脸局部属性归类

![](https://img-blog.csdn.net/2018060110164379)

3.3 Generating Candidate Windows

一般来说有两种方法可以用于候选区域生成：Generic object proposal 和 Template proposal（RPN）。但是这两种方法都是针对 generic objects 设计的，not suitable to propose windows specific to faces。我们提出了一个针对人脸的好方法 3.4





3.4 Ranking Windows by Faceness Score 

![](https://img-blog.csdn.net/20180601101655716)

3.5 Face Detection

These face proposals can be subsequently fed to the multi-task CNN

fine-tune the first branch of the multi-task CNN for face classification and the second branch for bounding box regresssion

![](https://img-blog.csdn.net/20180601101709965)

![](https://img-blog.csdn.net/2018060110171875)

![](https://img-blog.csdn.net/20180601101725185)





The runtime of the proposed Faceness-Net-SR-TP is 40 ms on a single Nvidia Titan X GPU. The time includes 10 ms to generate faceness proposals with the height of testing image no more than 300 pixels.

Faceness的整体性能在当时看来非常令人兴奋。此前学术界在FDDB上取得的最好检测精度是在100个误检时达到84%的检测率，Faceness在100个误检时，检测率接近88%，提升了几乎4个百分点；除了算法本身的精度有很大提升，作者还做了很多工程上的优化比如：通过多个网络共享参数，降低网络参数量 83%；采用多任务的训练方式同一网络实现不同任务等。



