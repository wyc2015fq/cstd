# 图像局部显著性—点特征（Fast） - wishchinYang的专栏 - CSDN博客
2017年03月16日 16:58:02[wishchin](https://me.csdn.net/wishchin)阅读数：775
        fast作为几乎最快的角点检测算法，一般说明不附带描述子。参考综述：[图像的显著性检测——点特征](http://blog.csdn.net/wishchin/article/details/45146117)
        详细内容，请拜访原=文章：[Fast特征点检测算法](http://blog.csdn.net/hujingshuang/article/details/46898007)
        在局部特征点检测快速发展的时候，人们对于特征的认识也越来越深入，近几年来许多学者提出了许许多多的特征检测算法及其改进算法，在众多的特征提取算法中，不乏涌现出佼佼者。
        Edward Rosten和Tom Drummond两位大神经过研究，于2006年在《Machine learning for high-speed corner detection》中提出了一种FAST特征点，并在2010年稍作修改后发表了《Features From Accelerated Segment Test》，简称[FAST](http://www.edwardrosten.com/work/fast.html)。**注意：**FAST只是一种特征点检测算法，并不涉及特征点的特征描述
       此文对Fast的介绍比较详细：[SLAM：图像角点检测的Fast算法](http://blog.csdn.net/wishchin/article/details/47977325)（OpenCV文档） 
       此文对Fast的试验结果介绍：[SLAM：图像角点检测的Fast算法](http://blog.csdn.net/wishchin/article/details/49404029)（时间阈值实验）
参考资料：
      Fast原始论文：《Machine
 learning for high-speed corner detection》
      详细工程页：[http://www.edwardrosten.com/work/fast.html](http://www.edwardrosten.com/work/fast.html)
