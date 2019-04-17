# 解读： FlowNet learning optical flow with convolutional networks - 家家的专栏 - CSDN博客





2017年11月30日 22:10:02[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：493








贡献点：

           Construct CNNs which are capable of solving the optical flow estimation problem as a supervised learning task. 

           Train CNN end-to-end to learn **predicting the optical flow field from a pair of images**

 Propose and compare two architectures: a genetic architecture and another one including a layer that correlates feature vectors at different image locations.




数据库： KITTI Sintel



算法：

          While optical flow estimation needs precise per-pixel localization, it also requires finding correspondences between two input images.

          Not only learning image feature representations, but also learn to match them at different locations in the two images. 




框架图：












