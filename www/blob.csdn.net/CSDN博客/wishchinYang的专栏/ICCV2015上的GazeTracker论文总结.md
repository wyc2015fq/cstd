# ICCV2015上的GazeTracker论文总结 - wishchinYang的专栏 - CSDN博客
2016年03月24日 13:45:01[wishchin](https://me.csdn.net/wishchin)阅读数：2681
        SLAM问题先慢慢编译一段时间，赶紧拾起来GazeTrack的事情......
        ICCV2015的大部分paper已经可以下载，文章列表在这个位置、
[http://www.cvpapers.com/iccv2015.html](http://www.cvpapers.com/iccv2015.html)
        文章题目关于Gaze的论文有
-         Rendering of Eyes for Eye-Shape Registration and Gaze Estimation
     Erroll Wood, Tadas BaltruÅ¡aitis, Xucong Zhang, Yusuke Sugano, Peter Robinson, Andreas Bulling-         此文章已经可以下载，这是页面：
- [http://www.cl.cam.ac.uk/research/rainbow/projects/syntheseyes/](http://www.cl.cam.ac.uk/research/rainbow/projects/syntheseyes/)
- 
        可视化的思路：
![](https://img-blog.csdn.net/20160224151405542?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
             对于眼睛附近面部模型的获取，使用了
             文章：[Constrained Local Neural Fields for robust facial landmark detection in the wild](http://www.cl.cam.ac.uk/~tb346/pub/papers/iccv2013.pdf)
           里面的CLNF特征
关于瞳孔拟合方案的几点限制：
       1.在不关心头动的情况下，一般情况眼球为上下覆盖和眼球左右移动，则 椭圆约束为瘦椭圆，且拟合约束为瘦椭圆拟合。
       2.在不构建详细折射模型的情况下，一般只建立一个线性的椭圆变换模型。
       3.........................
