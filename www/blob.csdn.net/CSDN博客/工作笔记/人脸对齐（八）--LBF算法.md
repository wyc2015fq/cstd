# 人脸对齐（八）--LBF算法 - 工作笔记 - CSDN博客





2017年11月29日 10:35:10[Eason.wxd](https://me.csdn.net/App_12062011)阅读数：5707
所属专栏：[机器视觉](https://blog.csdn.net/column/details/33959.html)









整体来看，其实 ，ESR是基础版本的形状回归，ERT将回归树修改为GBDT，由原始的直接回归形状，改进为回归形状残差，而LBF，是加速特征提取，由原来的像素差分特征池，改为随机选择点。



转自：http://blog.csdn.net/qq_14845119/article/details/53575091

基于LBF方法的人脸对齐，出自Face Alignment at3000 FPS via Regressing Local Binary Features，由于该方法提取的是局部二值特征（LBF），所以特征提取这个过程速度非常快，导致整个的算法都速快相当快，论文作者的LBF fast达到了3000fps的速度，网上热心网友分享的程序也达到了近300fps的速度，绝对是人脸对齐方面速度最快的一种算法。因此，好多网友也将该方法称为，3000fps。



该方法主要体现在2个方面，

（1）LBF特征的提取

作者通过在特征点附近随机选择点做残差来学习LBF特征，每一个特征点都会学到由好多随机树组成的随机森林，因此，一个特征点就得用一个随机森林生成的0,1特征向量来表示，将所有的特征点的随机森林都连接到一起，生成一个全局特征，后续过程就可以使用该全局特征做全局线性回归了。

![](https://img-blog.csdn.net/20161211201826488)



（2）基于cascade的级联的随机森林做全局线性回归

         所谓的线性回归，其实就是一个不断迭代的过程，对于每一个stage中，用上一个stage的状态作为输入来跟新，产生下一个stage的输入，以此类推，直到达到最底层stage。

![](https://img-blog.csdn.net/20161211201838176)

如上公式所示，I为输入图像，St-1为第t-1stage的形状，fait为t stage的特征匹配函数，Wt为线性回归矩阵。

![](https://img-blog.csdn.net/20161211201857277)



训练过程，就是学习fait和wt的一个过程，测试过程就是用训练好的fait和wt对提取的LBF特征做回归的过程。

         对于每个stage，越往下，所选择的随机点的范围就越小，特征点定位精度就越好。

![](https://img-blog.csdn.net/20161211201934505)



本人下载的是c++版本的程序，最终的运行效果如下，

         实际运行效果，速度绝对是人脸对齐中速度最快的一个算法了，精度的话，比起sdm来，还是逊色一点，尽管作者论文中分析的数据来看比sdm好，但是不得不承认，还是比sdm的精度差点。

![](https://img-blog.csdn.net/20161212093029745)

![](https://img-blog.csdn.net/20161212093044839)

github上热心网友的程序，

matab版本：[https://github.com/jwyang/face-alignment](https://github.com/jwyang/face-alignment)

c++版本：[https://github.com/yulequan/face-alignment-in-3000fps](https://github.com/yulequan/face-alignment-in-3000fps)





