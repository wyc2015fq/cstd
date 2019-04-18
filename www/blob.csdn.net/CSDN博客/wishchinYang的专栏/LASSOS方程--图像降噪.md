# LASSOS方程--图像降噪 - wishchinYang的专栏 - CSDN博客
2015年08月10日 09:58:38[wishchin](https://me.csdn.net/wishchin)阅读数：1574
前言：
      Tibshirani(1996)提出了Lasso( The Least Absolute Shrinkage and Selectionator operator ) 算法。这种算法通过构造一个惩罚函数获得一个精炼的模型；通过最终确定一些指标的系数为零，LASSO算法实现了指标集合精简的目的。这是一种处理**具有复共线性数据的有偏估计**。Lasso的基本思想是在回归系数的绝对值之和小于一个常数的约束条件下，使残差平方和最小化，从而能够产生某些
 严格等于 0 的回归系数，得到解释力较强的模型。R统计软件的Lars算法的软件包提供了Lasso算法。根据模型改进的需要，数据挖掘工作者可以借助于Lasso算法，利用AIC准则和BIC准则精炼简化统计模型的变量集合，达到降维的目的。因此，Lasso算法是可以应用到数据挖掘中的实用算法。
        LASSO方程是一个图像处理中经典的目标方程
![](http://latex.codecogs.com/gif.latex?%5Carg%5Cmin_x%5Cleft&space;%5C%7C&space;Ax-b&space;%5Cright&space;%5C%7C%5E2+%5Clambda%5Cleft&space;%5C%7C&space;x&space;%5Cright&space;%5C%7C_1)
       第二项的1范数限制了x的稀疏性。
![](https://img-blog.csdn.net/20140803044006513?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXZlcnNvbl80OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
        比如在图像去模糊的问题中，已知模糊的图像b，和模糊函数R，我们想恢复去模糊的图像I。这些变量的关系可以表达成I*R=b，其中*为卷积。在理想状态下，b没有任何噪音，那么这个问题就很简单。基于卷积定理，两个函数在时域的卷积相当于频域的相乘，那么我们只需要求出b和R的傅里叶变换，然后相除得到I的傅里叶变换，再将其恢复到时域。但是一般来说模糊图像b含有噪声，这使得频域中的操作异常不稳定，所以更多时候，我们希望通过以下方程求得I
![](http://latex.codecogs.com/gif.latex?%5Carg%5Cmin_x%5Cleft&space;%5C%7C&space;RI-b&space;%5Cright&space;%5C%7C%5E2+%5Clambda&space;p%28I%29)
        其中模糊算子R表现成矩阵形式，I和b表示为1维向量，函数p作为规范项。我们将I小波分解，I=Wx,其中W为小波基，x为小波基系数。我们知道图像的小波表示是稀疏的，那么目标方程就变成了LASSO的形式
![](http://latex.codecogs.com/gif.latex?%5Carg%5Cmin_x%5Cleft&space;%5C%7C&space;Ax-b&space;%5Cright&space;%5C%7C%5E2+%5Clambda%5Cleft&space;%5C%7C&space;x&space;%5Cright&space;%5C%7C_1)
      其中A=RW。现在的问题是，这个方程由于L1范数的存在，不是处处可微的，如果用 subgradient 的方法，收敛的速度会很慢。
**ISTA(Iterative Shrinkage-Thresholding Algorithm)**
       这个算法可以解决以上f+g形式的最小化问题，但ISTA适用于以下形式问题的求解：1.目标方程是f+g的形式2.f和g是凸的，f是可导的，g无所谓3.g需要足够简单（可拆分的，可以做坐标下降的coordinate descent）我们首先看对f做一般的递归下降
![](http://latex.codecogs.com/gif.latex?x_%7Bk%7D=x_%7Bk-1%7D-t_k%5Cnabla&space;f%28x_%7Bk-1%7D%29)
        这种形式的递归下降可以写成等价的以下形式(general form)
![](http://latex.codecogs.com/gif.latex?x_%7Bk%7D=%5Carg%5Cmin_x&space;%5C%7Bf%28x_%7Bk-1%7D%29&space;+&space;%5Cleft&space;%5Clangle&space;x-x_%7Bk-1%7D,%5Cnabla&space;f%28x_%7Bk-1%7D%29&space;%5Cright&space;%5Crangle&space;+&space;%7B1&space;%5Cover&space;2t_k%7D%5Cleft&space;%5C%7C&space;x-x_%7Bk-1%7D&space;%5Cright&space;%5C%7C%5E2%5C%7D)
       因为在我们的问题中除了f还有一个非平滑的g存在，一个简单的想法是改变迭代公式成以下形式(也就是直接把g加到后面)
![](http://latex.codecogs.com/gif.latex?x_%7Bk%7D=%5Carg%5Cmin_x&space;%5C%7Bf%28x_%7Bk-1%7D%29&space;+&space;%5Cleft&space;%5Clangle&space;x-x_%7Bk-1%7D,%5Cnabla&space;f%28x_%7Bk-1%7D%29&space;%5Cright&space;%5Crangle&space;+&space;%7B1&space;%5Cover&space;2t_k%7D%5Cleft&space;%5C%7C&space;x-x_%7Bk-1%7D&space;%5Cright&space;%5C%7C%5E2+g%28x%29%5C%7D)
        通过基本的代数，消去和x无关的常数项，每一步的迭代公式就变成了
![](http://latex.codecogs.com/gif.latex?x_%7Bk%7D=%5Carg%5Cmin_x&space;%5C%7B&space;%7B1&space;%5Cover&space;2t_k%7D%5Cleft&space;%5C%7C&space;x-%28x_%7Bk-1%7D-t_k%5Cnabla&space;f%28x_%7Bk-1%7D%29%29&space;%5Cright&space;%5C%7C%5E2+g%28x%29%5C%7D)
        这时我们可以看到，假如g是一个开拆分的函数(比如L1范数)，我们就可以对每一维分别进行坐标下降，也就是将N维的最小值问题，转化成N个1D的最小值问题。我们发现，如果![](http://latex.codecogs.com/gif.latex?g=%5Clambda%5Cleft&space;%5C%7C&space;x&space;%5Cright&space;%5C%7C_1)的话，那么这个问题有解析解,即每步的迭代可以写成：
![](http://latex.codecogs.com/gif.latex?x_k=%5Ctau&space;_%7B%5Clambda&space;t_k%7D%28x_%7Bk-1%7D-t_k%5Cnabla&space;f%28x_%7Bk-1%7D%29%29)
       其中![](http://latex.codecogs.com/gif.latex?%5Ctau_%5Calpha%28x%29_i=%28%5Cleft&space;%7C&space;x_i&space;%5Cright&space;%7C-%5Calpha%29_+sign%28x_i%29)称作shrinkage
 operator。
**FISTA(FAST Iterative Shrinkage-Thresholding Algorithm)**
       FISTA其实就是对ISTA应用Nestrerov加速。一个普通的Nestrerov加速递归下降的迭代步骤是：
1.   ![](http://latex.codecogs.com/gif.latex?%5Cxi_0=0)
2.   ![](http://latex.codecogs.com/gif.latex?%5Cxi_k=%7B1+%5Csqrt%7B1+4%5Cxi_%7Bk-1%7D%5E2%7D&space;%5Cover&space;2%7D,%5Cgamma_k=%7B1-%5Cxi_%7Bk-1%7D&space;%5Cover&space;%5Cxi_k%7D)
3.   ![](http://latex.codecogs.com/gif.latex?y_%7Bk%7D=x_%7Bk-1%7D-t_k%5Cnabla&space;f%28x_%7Bk-1%7D%29)
4.   ![](http://latex.codecogs.com/gif.latex?x_k=%281-%5Cgamma_k%29y_%7Bk%7D+%5Cgamma_ky_%7Bk-1%7D)
       应用到FISTA上的话，就是把第3步换成ISTA的迭代步骤。可以证明FISTA可以达到![](http://latex.codecogs.com/gif.latex?1/t%5E2)的收敛速度。（t是迭代次数）通过下面的实验可以看到，同样迭代了300次，左图(ISTA)仍未收敛，图像仍然模糊。而右图(FISTA)已经基本还原了去模糊的原图。
![](https://img-blog.csdn.net/20140803044041239?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXZlcnNvbl80OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140803044100645?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvaXZlcnNvbl80OQ==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
