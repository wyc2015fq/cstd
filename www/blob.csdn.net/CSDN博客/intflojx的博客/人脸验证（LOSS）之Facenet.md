
# 人脸验证（LOSS）之Facenet - intflojx的博客 - CSDN博客


2018年09月04日 09:51:31[paulpanwang](https://me.csdn.net/intflojx)阅读数：272


**FaceNet**是目前引用量最高的人脸识别方法，没有用Softmax，而是提出了Triple Loss：
![](https://pic3.zhimg.com/80/v2-7b15707ebe7e520155d798e53f6fac34_hd.jpg)
以三元组（a, p, n）形式进行优化，**不同类特征的L2距离要比同类特征的L2距离大margin m，同时获得类内紧凑和类间分离**。FaceNet用200M训练数据，仅128维特征映射，在LFW上达到了99.63%，非常犀利。但代码、模型和训练数据集都没有开源，三元组选择极具技巧性，复现非常困难。
![](https://pic2.zhimg.com/80/v2-292c1ac77db14c455db9de6efdd198f8_hd.jpg)
学习渠道：
视频
吴恩达的介绍：[https://www.deeplearning.ai/](https://www.deeplearning.ai/)（需要申请助学金）
**或者到网易云课堂观看（免费） 4.3章**
![](https://img-blog.csdn.net/20180904094937238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2ludGZsb2p4/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
[https://mooc.study.163.com/learn/2001281004?tid=2001392030\#/learn/content?type=detail&id=2001729341&cid=2001724535](https://mooc.study.163.com/learn/2001281004?tid=2001392030#/learn/content?type=detail&id=2001729341&cid=2001724535)
博客
孪生神经网络（Siamese neural network）：
[http://www.sohu.com/a/214143960_465975](http://www.sohu.com/a/214143960_465975)
[http://blog.csdn.net/stdcoutzyx/article/details/46776415](http://blog.csdn.net/stdcoutzyx/article/details/46776415)
代码
**目前开源效果比较好的：Deepface****、faceNet****等**
**其中**FaceNet，在LFW(Labeled Faces in the Wild)上达到99.63%准确率(新纪录)
**https://github.com/davidsandberg/facenet****：github****源码**
**[http://blog.csdn.net/tmosk/article/details/78087122](http://blog.csdn.net/tmosk/article/details/78087122)****:****博客教程**
**参考文献：**
Taigman Y, Yang M, Ranzato M A, et al. Deepface: Closing the gap to human-level performance in face verification[C]//Computer Vision and Pattern Recognition (CVPR), 2014 IEEE Conference on. IEEE, 2014: 1701-1708.
Schroff F, Kalenichenko D, Philbin J. FaceNet: A unified embedding for face recognition and clustering[J]. 2015:815-823.

