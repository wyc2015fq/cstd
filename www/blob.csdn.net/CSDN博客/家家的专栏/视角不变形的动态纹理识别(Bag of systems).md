# 视角不变形的动态纹理识别(Bag of systems)~ - 家家的专栏 - CSDN博客





2012年06月06日 16:48:38[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：1348








# 解读文献：View-invariant dynamic texture recognition using a bag of dynamical systems

**目标:**分类不同视角下的，动态纹理视频~

用先线性动态系统的集合，来建模每个视频~ 其中线性动态系统来描述视频patch~

Bag of systems (BOS)表示方法，相似于Bag of features (BOF)~ 本文利用线性动态系统作为特征描述子~

动态纹理的度量：

    Martin distance~ （principal angles between the subspaces~）

   Binert-Cauchy kernels

   KLD~

   Chernoff distance~

   等等。

**动态纹理模型**：

       z(t+1)=Az(t) + v(t)

       I(t)     =Cz(t) + w(t)

**典型的BOF框架**：

      1）从训练集图像中抽取特征及其相应的描述子~

      2）利用聚类（k-mean）的方法建立codebook~

     3）训练集中的每个图象用codebook进行表示~

      4）测试图像分类~

**BOS方法**：

**1、特征抽取和描述子~**

           从视频序列中抽取特征描述子。原有的时空描述子（Harris corner detector, spatiotemporal Hessian, LDS）

           本文采用LDA的参数，作为视频的特征描述子M=(A,C).

**     2、codebook的形成**

 因为视频描述子M=（A, C）并非处于欧式空间中，所以不能直接在欧式空间中应用聚类算法~

            可以发现一种低维欧式嵌入，并进行聚类~  利用Martin度量进行降维，然后聚类形成codebook。

            聚类后，有K个聚类中心~

**3、利用codebook表示视频~**

            W为权重向量。

            假设在第i个视频中，（第k个codeword出现的次数）/（本视频序列中的codeword数目）

             V是视频序列的总数据，codeword出现的总数据V(i)~

                  三种计算权重向量的方法：

![](https://img-my.csdn.net/uploads/201206/06/1338973877_2029.png)![](https://img-my.csdn.net/uploads/201206/06/1338973883_2481.png)![](https://img-my.csdn.net/uploads/201206/06/1338973889_5753.png)



** 4、度量和分类(SVM)~**



![](https://img-my.csdn.net/uploads/201206/06/1338973895_5290.png)





