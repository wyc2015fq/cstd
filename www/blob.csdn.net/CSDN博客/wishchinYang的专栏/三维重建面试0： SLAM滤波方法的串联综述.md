# 三维重建面试0：*SLAM滤波方法的串联综述 - wishchinYang的专栏 - CSDN博客
2017年05月31日 17:43:29[wishchin](https://me.csdn.net/wishchin)阅读数：1300
           知乎上的提问，高翔作了回答：[能否简单并且易懂地介绍一下多个基于滤波方法的SLAM算法原理](https://www.zhihu.com/question/46916554/answer/103411007)？
           写的比较通顺，抄之。如有异议，请拜访原文。如有侵权，请联系删除。
我怎么会写得那么长……如果您有兴趣可以和我一块把公式过一遍。
要讲清这个问题，得从状态估计理论来说。先摆上一句名言：
> 状态估计乃传感器之本质。（To understand the need for state estimation is to understand the nature of sensors.）
任何传感器，激光也好，视觉也好，整个SLAM系统也好，要解决的问题只有一个：**如何通过数据来估计自身状态。**每种传感器的测量模型不一样，它们的精度也不一样。换句话说，状态估计问题，也就是“**如何最好地使用传感器数据**”。可以说，SLAM是状态估计的一个特例。
=====================**离散时间系统的状态估计**======================
记机器人在各时刻的状态为![x_1,\ldots,x_k](https://www.zhihu.com/equation?tex=x_1%2C%5Cldots%2Cx_k)，其中![k](https://www.zhihu.com/equation?tex=k)是离散时间下标。在SLAM中，我们通常要估计机器人的位置，那么系统的状态就指的是机器人的位姿。用两个方程来描述状态估计问题：
![\[\left\{ \begin{array}{l}{x_k} = f\left( {{x_{k - 1}},{u_k},{w_k}} \right)\\{y_k} = g\left( {{x_k},{n_k}} \right)\end{array} \right.\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cleft%5C%7B+%5Cbegin%7Barray%7D%7Bl%7D%0A%7Bx_k%7D+%3D+f%5Cleft%28+%7B%7Bx_%7Bk+-+1%7D%7D%2C%7Bu_k%7D%2C%7Bw_k%7D%7D+%5Cright%29%5C%5C%0A%7By_k%7D+%3D+g%5Cleft%28+%7B%7Bx_k%7D%2C%7Bn_k%7D%7D+%5Cright%29%0A%5Cend%7Barray%7D+%5Cright.%5C%5D)
解释一下变量：
![f](https://www.zhihu.com/equation?tex=f)-运动方程
![u](https://www.zhihu.com/equation?tex=u)- 输入
![w](https://www.zhihu.com/equation?tex=w)- 输入噪声
![g](https://www.zhihu.com/equation?tex=g)- 观测方程
![y](https://www.zhihu.com/equation?tex=y)- 观测数据
![n](https://www.zhihu.com/equation?tex=n)- 观测噪声
运动方程描述了状态![x_{k-1}](https://www.zhihu.com/equation?tex=x_%7Bk-1%7D)是怎么变到![x_k](https://www.zhihu.com/equation?tex=x_k)的，而观测方程描述的是从![x_k](https://www.zhihu.com/equation?tex=x_k)是怎么得到观察数据![y_k](https://www.zhihu.com/equation?tex=y_k)的。
请注意这是一种抽象的写法。当你有实际的机器人，实际的传感器时，方程的形式就会变得具体，也就是所谓的**参数化**。例如，当我们关心机器人空间位置时，可以取![x_k = [x,y,z]_k](https://www.zhihu.com/equation?tex=x_k+%3D+%5Bx%2Cy%2Cz%5D_k)。进而，机器人携带了里程计，能够得到两个时间间隔中的相对运动，像这样![\Delta x_k=[\Delta x, \Delta y, \Delta z]_k](https://www.zhihu.com/equation?tex=%5CDelta+x_k%3D%5B%5CDelta+x%2C+%5CDelta+y%2C+%5CDelta+z%5D_k)，那么运动方程就变为：
![x_{k+1}=x_k+\Delta x_k+w_k](https://www.zhihu.com/equation?tex=x_%7Bk%2B1%7D%3Dx_k%2B%5CDelta+x_k%2Bw_k)
同理，观测方程也随传感器的具体信息而变。
例如激光传感器可以得到空间点离机器人的距离和角度，记为![y_k=[r,\theta]_k](https://www.zhihu.com/equation?tex=y_k%3D%5Br%2C%5Ctheta%5D_k)，那么观测方程为：
![\[{\left[ \begin{array}{l}r\\\theta \end{array} \right]_k} = \left[ \begin{array}{l}\sqrt {{{\left\| {{x_k} - {L_k}} \right\|}_2}} \\{\tan ^{ - 1}}\frac{{{L_{k,y}} - {x_{k,y}}}}{{{L_{k,x}} - {x_{k,x}}}}\end{array} \right] + {n_k}\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%5Cleft%5B+%5Cbegin%7Barray%7D%7Bl%7D%0Ar%5C%5C%0A%5Ctheta+%0A%5Cend%7Barray%7D+%5Cright%5D_k%7D+%3D+%5Cleft%5B+%5Cbegin%7Barray%7D%7Bl%7D%0A%5Csqrt+%7B%7B%7B%5Cleft%5C%7C+%7B%7Bx_k%7D+-+%7BL_k%7D%7D+%5Cright%5C%7C%7D_2%7D%7D+%5C%5C%0A%7B%5Ctan+%5E%7B+-+1%7D%7D%5Cfrac%7B%7B%7BL_%7Bk%2Cy%7D%7D+-+%7Bx_%7Bk%2Cy%7D%7D%7D%7D%7B%7B%7BL_%7Bk%2Cx%7D%7D+-+%7Bx_%7Bk%2Cx%7D%7D%7D%7D%0A%5Cend%7Barray%7D+%5Cright%5D+%2B+%7Bn_k%7D%5C%5D)，其中![L_k=[L_{k,x},L_{k,y}]](https://www.zhihu.com/equation?tex=L_k%3D%5BL_%7Bk%2Cx%7D%2CL_%7Bk%2Cy%7D%5D)是一个2D路标点。
举这几个例子是为了说明，**运动方程和观测方程具体形式是会变化的**。但是，我们想讨论更一般的问题：当我不限制传感器的具体形式时，能否设计一种方式，从已知的![u,y](https://www.zhihu.com/equation?tex=u%2Cy)（输入和观测数据）从，估计出![x](https://www.zhihu.com/equation?tex=x)呢？
这就是最一般的状态估计问题。我们会根据![f,g](https://www.zhihu.com/equation?tex=f%2Cg)是否线性，把它们分为**线性/非线性系统**。同时，对于噪声![w,n](https://www.zhihu.com/equation?tex=w%2Cn)，根据它们是否为高斯分布，分为**高斯/非高斯噪声**系统。最一般的，也是最困难的问题，是非线性-非高斯(NLNG,
 Nonlinear-Non Gaussian)的状态估计。下面先说最简单的情况：线性高斯系统。
=====================**线性高斯系统**============================
**线性高斯系统(LG，Linear Gaussian)**
在线性高斯系统中，运动方程、观测方程是线性的，且两个噪声项服从零均值的高斯分布。这是最简单的情况。简单在哪里呢？主要是因为**高斯分布经过线性变换之后仍为高斯分布**。而对于一个高斯分布，只要计算出它的一阶和二阶矩，就可以描述它（高斯分布只有两个参数![\mu, \Sigma](https://www.zhihu.com/equation?tex=%5Cmu%2C+%5CSigma)）。
线性系统形式如下：
![\left\{\begin{array}{l}{x_k} = {A_{k - 1}}{x_{k - 1}} + {u_k} + {w_k}\\{y_k} = {C_k}{x_k} + {n_k}\\{w_k}\sim N\left( {0,{Q_k}} \right)\\{n_k}\sim N(0,{R_k})\end{array} \right.](https://www.zhihu.com/equation?tex=%5Cleft%5C%7B%0A%5Cbegin%7Barray%7D%7Bl%7D%0A%7Bx_k%7D+%3D+%7BA_%7Bk+-+1%7D%7D%7Bx_%7Bk+-+1%7D%7D+%2B+%7Bu_k%7D+%2B+%7Bw_k%7D%5C%5C%0A%7By_k%7D+%3D+%7BC_k%7D%7Bx_k%7D+%2B+%7Bn_k%7D%5C%5C%0A%7Bw_k%7D%5Csim+N%5Cleft%28+%7B0%2C%7BQ_k%7D%7D+%5Cright%29%5C%5C%0A%7Bn_k%7D%5Csim+N%280%2C%7BR_k%7D%29%0A%5Cend%7Barray%7D%0A+%5Cright.)
 其中![Q_k,R_k](https://www.zhihu.com/equation?tex=Q_k%2CR_k)是两个噪声项的协方差矩阵。![A,C](https://www.zhihu.com/equation?tex=A%2CC)为转移矩阵和观测矩阵。
对LG系统，可以用贝叶斯法则，计算![x](https://www.zhihu.com/equation?tex=x)的后验概率分布——这条路直接通向**卡尔曼滤波器**。卡尔曼是线性系统的递推形式（recursive，也就是从![x_{k-1}](https://www.zhihu.com/equation?tex=x_%7Bk-1%7D)估计![x_k](https://www.zhihu.com/equation?tex=x_k)）的无偏最优估计。由于解释EKF和UKF都得用它，所以我们来推一推。如果读者不感兴趣，可以跳过公式推导环节。
符号：用![\hat{x}](https://www.zhihu.com/equation?tex=%5Chat%7Bx%7D)表示![x](https://www.zhihu.com/equation?tex=x)的后验概率，用![\[\tilde x\]](https://www.zhihu.com/equation?tex=%5C%5B%5Ctilde+x%5C%5D)表示它的先验概率。因为系统是线性的，噪声是高斯的，所以状态也服从高斯分布，需要计算它的均值和协方差矩阵。记第![k](https://www.zhihu.com/equation?tex=k)时刻的状态服从：![x_k\sim N({{\bar x}_k},{P_k})](https://www.zhihu.com/equation?tex=x_k%5Csim+N%28%7B%7B%5Cbar+x%7D_k%7D%2C%7BP_k%7D%29)
我们希望得到状态变量![x](https://www.zhihu.com/equation?tex=x)的最大后验估计（MAP，Maximize a Posterior），于是计算：
![\[\begin{array}{ccl}\hat x &=& \arg \mathop {\max }\limits_x p\left( {x|y,u} \right)\\ &=& \arg \max \frac{{p\left( {y|x,u} \right)p\left( {x|u} \right)}}{{p\left( {y|v} \right)}} \\ &=& \arg \max p(y|x)p\left( {x|u} \right)\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bccl%7D%0A%5Chat+x+%26%3D%26+%5Carg+%5Cmathop+%7B%5Cmax+%7D%5Climits_x+p%5Cleft%28+%7Bx%7Cy%2Cu%7D+%5Cright%29%5C%5C%0A+%26%3D%26+%5Carg+%5Cmax+%5Cfrac%7B%7Bp%5Cleft%28+%7By%7Cx%2Cu%7D+%5Cright%29p%5Cleft%28+%7Bx%7Cu%7D+%5Cright%29%7D%7D%7B%7Bp%5Cleft%28+%7By%7Cv%7D+%5Cright%29%7D%7D+%5C%5C%0A+%26%3D%26+%5Carg+%5Cmax+p%28y%7Cx%29p%5Cleft%28+%7Bx%7Cu%7D+%5Cright%29%0A%5Cend%7Barray%7D%5C%5D)
第二行是贝叶斯法则，第三行分母和![x](https://www.zhihu.com/equation?tex=x)无关所以去掉。
第一项即观测方程，有：![\[p\left( {y|x} \right) = \prod\limits_{k = 0}^K {p\left( {{y_k}|{x_k}} \right)} \]](https://www.zhihu.com/equation?tex=%5C%5Bp%5Cleft%28+%7By%7Cx%7D+%5Cright%29+%3D+%5Cprod%5Climits_%7Bk+%3D+0%7D%5EK+%7Bp%5Cleft%28+%7B%7By_k%7D%7C%7Bx_k%7D%7D+%5Cright%29%7D+%5C%5D)，很简单。
第二项即运动方程，有：![\[p\left( {x|v} \right) = \prod\limits_{k = 0}^K {p\left( {{x_k}|{x_{k - 1}},v_k} \right)} \]](https://www.zhihu.com/equation?tex=%5C%5Bp%5Cleft%28+%7Bx%7Cv%7D+%5Cright%29+%3D+%5Cprod%5Climits_%7Bk+%3D+0%7D%5EK+%7Bp%5Cleft%28+%7B%7Bx_k%7D%7C%7Bx_%7Bk+-+1%7D%7D%2Cv_k%7D+%5Cright%29%7D+%5C%5D)，也很简单。
现在的问题是如何求解这个最大化问题。对于高斯分布，最大化问题可以变成最小化它的负对数。当我对一个高斯分布取负对数时，它的指数项变成了一个二次项，而前面的因子则变为一个无关的常数项，可以略掉（这部分我不敲了，有疑问的同学可以问）。于是，定义以下形式的最小化函数：
![\[\begin{array}{l}{J_{y,k}}\left( x \right) = \frac{1}{2}{\left( {{y_k} - {C_k}{x_k}} \right)^T}R_k^{ - 1}\left( {{y_k} - {C_k}{x_k}} \right)\\{J_{v,k}}\left( x \right) = \frac{1}{2}{\left( {{x_k} - {A_{k - 1}}{x_{k - 1}} - {v_k}} \right)^T}Q_k^{ - 1}\left( {{x_k} - {A_{k - 1}}{x_{k - 1}} - {v_k}} \right)\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D%0A%7BJ_%7By%2Ck%7D%7D%5Cleft%28+x+%5Cright%29+%3D+%5Cfrac%7B1%7D%7B2%7D%7B%5Cleft%28+%7B%7By_k%7D+-+%7BC_k%7D%7Bx_k%7D%7D+%5Cright%29%5ET%7DR_k%5E%7B+-+1%7D%5Cleft%28+%7B%7By_k%7D+-+%7BC_k%7D%7Bx_k%7D%7D+%5Cright%29%5C%5C%0A%7BJ_%7Bv%2Ck%7D%7D%5Cleft%28+x+%5Cright%29+%3D+%5Cfrac%7B1%7D%7B2%7D%7B%5Cleft%28+%7B%7Bx_k%7D+-+%7BA_%7Bk+-+1%7D%7D%7Bx_%7Bk+-+1%7D%7D+-+%7Bv_k%7D%7D+%5Cright%29%5ET%7DQ_k%5E%7B+-+1%7D%5Cleft%28+%7B%7Bx_k%7D+-+%7BA_%7Bk+-+1%7D%7D%7Bx_%7Bk+-+1%7D%7D+-+%7Bv_k%7D%7D+%5Cright%29%0A%5Cend%7Barray%7D%5C%5D)
那么最大后验估计就等价于：
![\[\hat x = \arg \min \sum\limits_{k = 0}^K {{J_{y,k}} + {J_{v,k}}} \]](https://www.zhihu.com/equation?tex=%5C%5B%5Chat+x+%3D+%5Carg+%5Cmin+%5Csum%5Climits_%7Bk+%3D+0%7D%5EK+%7B%7BJ_%7By%2Ck%7D%7D+%2B+%7BJ_%7Bv%2Ck%7D%7D%7D+%5C%5D)
这个问题现在是二次项和的形式，写成矩阵形式会更加清晰。定义：
![\[\begin{array}{l}z = \left[ \begin{array}{l}{x_0}\\{v_1}\\ \vdots \\{v_K}\\{y_0}\\ \vdots \\{y_K}\end{array} \right],x = \left[ \begin{array}{l}{x_0}\\ \vdots \\{x_K}\end{array} \right]\\H = \left[ {\begin{array}{*{20}{c}}1&{}&{}&{}\\{ - {A_0}}&1&{}&{}\\{}& \ddots & \ddots &{}\\{}&{}&{ - {A_{K - 1}}}&1\\{{C_0}}&{}&{}&{}\\{}& \ddots &{}&{}\\{}&{}& \ddots &{}\\{}&{}&{}&{{C_K}}\end{array}} \right],W = \left[ {\begin{array}{*{20}{c}}{{P_0}}&{}&{}&{}&{}&{}&{}\\{}&{{Q_1}}&{}&{}&{}&{}&{}\\{}&{}& \ddots &{}&{}&{}&{}\\{}&{}&{}&{{Q_K}}&{}&{}&{}\\{}&{}&{}&{}&{{R_1}}&{}&{}\\{}&{}&{}&{}&{}& \ddots &{}\\{}&{}&{}&{}&{}&{}&{{R_K}}\end{array}} \right]\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D%0Az+%3D+%5Cleft%5B+%5Cbegin%7Barray%7D%7Bl%7D%0A%7Bx_0%7D%5C%5C%0A%7Bv_1%7D%5C%5C%0A+%5Cvdots+%5C%5C%0A%7Bv_K%7D%5C%5C%0A%7By_0%7D%5C%5C%0A+%5Cvdots+%5C%5C%0A%7By_K%7D%0A%5Cend%7Barray%7D+%5Cright%5D%2Cx+%3D+%5Cleft%5B+%5Cbegin%7Barray%7D%7Bl%7D%0A%7Bx_0%7D%5C%5C%0A+%5Cvdots+%5C%5C%0A%7Bx_K%7D%0A%5Cend%7Barray%7D+%5Cright%5D%5C%5C%0AH+%3D+%5Cleft%5B+%7B%5Cbegin%7Barray%7D%7B%2A%7B20%7D%7Bc%7D%7D%0A1%26%7B%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B+-+%7BA_0%7D%7D%261%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26+%5Cddots+%26+%5Cddots+%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26%7B+-+%7BA_%7BK+-+1%7D%7D%7D%261%5C%5C%0A%7B%7BC_0%7D%7D%26%7B%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26+%5Cddots+%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26+%5Cddots+%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26%7B%7D%26%7B%7BC_K%7D%7D%0A%5Cend%7Barray%7D%7D+%5Cright%5D%2CW+%3D+%5Cleft%5B+%7B%5Cbegin%7Barray%7D%7B%2A%7B20%7D%7Bc%7D%7D%0A%7B%7BP_0%7D%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7BQ_1%7D%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26+%5Cddots+%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26%7B%7D%26%7B%7BQ_K%7D%7D%26%7B%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7BR_1%7D%7D%26%7B%7D%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26+%5Cddots+%26%7B%7D%5C%5C%0A%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7D%26%7B%7BR_K%7D%7D%0A%5Cend%7Barray%7D%7D+%5Cright%5D%0A%5Cend%7Barray%7D%5C%5D)
就得到矩阵形式的，类似最小二乘的问题：
![\[J\left( x \right) = \frac{1}{2}{\left( {z - Hx} \right)^T}{W^{ - 1}}\left( {z - Hx} \right)\]](https://www.zhihu.com/equation?tex=%5C%5BJ%5Cleft%28+x+%5Cright%29+%3D+%5Cfrac%7B1%7D%7B2%7D%7B%5Cleft%28+%7Bz+-+Hx%7D+%5Cright%29%5ET%7D%7BW%5E%7B+-+1%7D%7D%5Cleft%28+%7Bz+-+Hx%7D+%5Cright%29%5C%5D)
于是令它的导数为零，得到：
![\[\frac{{\partial J}}{{\partial {x^T}}} =  - {H^T}{W^{ - 1}}\left( {z - Hx} \right) = 0 \Rightarrow \left( {{H^T}{W^{ - 1}}H} \right)x = {H^T}{W^{ - 1}}z\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cfrac%7B%7B%5Cpartial+J%7D%7D%7B%7B%5Cpartial+%7Bx%5ET%7D%7D%7D+%3D++-+%7BH%5ET%7D%7BW%5E%7B+-+1%7D%7D%5Cleft%28+%7Bz+-+Hx%7D+%5Cright%29+%3D+0+%5CRightarrow+%5Cleft%28+%7B%7BH%5ET%7D%7BW%5E%7B+-+1%7D%7DH%7D+%5Cright%29x+%3D+%7BH%5ET%7D%7BW%5E%7B+-+1%7D%7Dz%5C%5D)
 (*)
读者会问，这个问题和卡尔曼滤波有什么问题呢？事实上，**卡尔曼滤波就是递推地求解(*)式的过程**。所谓递推，就是只用![x_{k-1}](https://www.zhihu.com/equation?tex=x_%7Bk-1%7D)来计算![x_k](https://www.zhihu.com/equation?tex=x_k)。对(*)进行Cholesky分解，就可以推出卡尔曼滤波器。详细过程限于篇幅就不推了，把卡尔曼的结论写一下：
![\[\begin{array}{l}{{\tilde P}_k} = {A_{k - 1}}{{\hat P}_{k - 1}}A_{k - 1}^T + {Q_k}\\{{\tilde x}_k} = {A_{k - 1}}{{\hat x}_{k - 1}} + {v_k}\\{K_k} = {{\tilde P}_k}C_k^T{\left( {{C_k}{{\tilde P}_k}C_k^T + {R_k}} \right)^{ - 1}}\\{{\hat P}_k} = \left( {I - {K_k}{C_k}} \right){{\tilde P}_k}\\{{\hat x}_k} = {{\tilde x}_k} + {K_k}\left( {{y_k} - {C_k}{{\tilde x}_k}} \right)\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D%0A%7B%7B%5Ctilde+P%7D_k%7D+%3D+%7BA_%7Bk+-+1%7D%7D%7B%7B%5Chat+P%7D_%7Bk+-+1%7D%7DA_%7Bk+-+1%7D%5ET+%2B+%7BQ_k%7D%5C%5C%0A%7B%7B%5Ctilde+x%7D_k%7D+%3D+%7BA_%7Bk+-+1%7D%7D%7B%7B%5Chat+x%7D_%7Bk+-+1%7D%7D+%2B+%7Bv_k%7D%5C%5C%0A%7BK_k%7D+%3D+%7B%7B%5Ctilde+P%7D_k%7DC_k%5ET%7B%5Cleft%28+%7B%7BC_k%7D%7B%7B%5Ctilde+P%7D_k%7DC_k%5ET+%2B+%7BR_k%7D%7D+%5Cright%29%5E%7B+-+1%7D%7D%5C%5C%0A%7B%7B%5Chat+P%7D_k%7D+%3D+%5Cleft%28+%7BI+-+%7BK_k%7D%7BC_k%7D%7D+%5Cright%29%7B%7B%5Ctilde+P%7D_k%7D%5C%5C%0A%7B%7B%5Chat+x%7D_k%7D+%3D+%7B%7B%5Ctilde+x%7D_k%7D+%2B+%7BK_k%7D%5Cleft%28+%7B%7By_k%7D+-+%7BC_k%7D%7B%7B%5Ctilde+x%7D_k%7D%7D+%5Cright%29%0A%5Cend%7Barray%7D%5C%5D)
前两个是预测，第三个是卡尔曼增益，四五是校正。
另一方面，能否直接求解(*)式，得到![\hat{x}](https://www.zhihu.com/equation?tex=%5Chat%7Bx%7D)呢？答案是可以的，而且这就是优化方法（batch optimization）的思路：将所有的状态放在一个向量里，进行求解。与卡尔曼滤波不同的是，在估计前面时刻的状态（如![x_1](https://www.zhihu.com/equation?tex=x_1)）时，会用到后面时刻的信息（![y_2,y_3](https://www.zhihu.com/equation?tex=y_2%2Cy_3)等）。从这点来说，优化方法和卡尔曼处理信息的方式是相当不同的。
==================**扩展卡尔曼滤波器**===================
线性高斯系统当然性质很好啦，但许多现实世界中的系统都不是线性的，状态和噪声也不是高斯分布的。例如上面举的激光观测方程就不是线性的。当系统为非线性的时候，会发生什么呢？
一件悲剧的事情是：**高斯分布经过非线性变换后，不再是高斯分布。而且，是个什么分布，基本说不上来**。（摊手）
如果没有高斯分布，上面说的那些都不再成立了。**于是EKF说，嘛，我们睁一只眼闭一只眼，用高斯分布去近似它，并且，在工作点附近对系统进行线性化**。当然这个近似是很成问题的，有什么问题我们之后再说。
EKF的做法主要有两点。其一，在工作点附近![\[{{\hat x}_{k - 1}},{{\tilde x}_k}\]](https://www.zhihu.com/equation?tex=%5C%5B%7B%7B%5Chat+x%7D_%7Bk+-+1%7D%7D%2C%7B%7B%5Ctilde+x%7D_k%7D%5C%5D)，对系统进行线性近似化：
![\[\begin{array}{l}f\left( {{x_{k - 1}},{v_k},{w_k}} \right) \approx f\left( {{{\hat x}_{k - 1}},{v_k},0} \right) + \frac{{\partial f}}{{\partial {x_{k - 1}}}}\left( {{x_{k - 1}} - {{\hat x}_{k - 1}}} \right) + \frac{{\partial f}}{{\partial {w_k}}}{w_k}\\g\left( {{x_k},{n_k}} \right) \approx g\left( {{{\tilde x}_k},0} \right) + \frac{{\partial g}}{{\partial {x_k}}}{n_k}\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D%0Af%5Cleft%28+%7B%7Bx_%7Bk+-+1%7D%7D%2C%7Bv_k%7D%2C%7Bw_k%7D%7D+%5Cright%29+%5Capprox+f%5Cleft%28+%7B%7B%7B%5Chat+x%7D_%7Bk+-+1%7D%7D%2C%7Bv_k%7D%2C0%7D+%5Cright%29+%2B+%5Cfrac%7B%7B%5Cpartial+f%7D%7D%7B%7B%5Cpartial+%7Bx_%7Bk+-+1%7D%7D%7D%7D%5Cleft%28+%7B%7Bx_%7Bk+-+1%7D%7D+-+%7B%7B%5Chat+x%7D_%7Bk+-+1%7D%7D%7D+%5Cright%29+%2B+%5Cfrac%7B%7B%5Cpartial+f%7D%7D%7B%7B%5Cpartial+%7Bw_k%7D%7D%7D%7Bw_k%7D%5C%5C%0Ag%5Cleft%28+%7B%7Bx_k%7D%2C%7Bn_k%7D%7D+%5Cright%29+%5Capprox+g%5Cleft%28+%7B%7B%7B%5Ctilde+x%7D_k%7D%2C0%7D+%5Cright%29+%2B+%5Cfrac%7B%7B%5Cpartial+g%7D%7D%7B%7B%5Cpartial+%7Bx_k%7D%7D%7D%7Bn_k%7D%0A%5Cend%7Barray%7D%5C%5D)
这里的几个偏导数，都在工作点处取值。于是呢，它就被**活生生地当成了一个线性系统**。
第二，在线性系统近似下，把噪声项和状态都**当成了高斯分布**。这样，只要估计它们的均值和协方差矩阵，就可以描述状态了。经过这样的近似之后呢，后续工作都和卡尔曼滤波是一样的了。所以EKF是卡尔曼滤波在NLNG系统下的直接扩展（所以叫扩展卡尔曼嘛）。EKF给出的公式和卡尔曼是一致的，用线性化之后的矩阵去代替卡尔曼滤波器里的转移矩阵和观测矩阵即可。
![\[\begin{array}{l}{{\tilde P}_k} = {F_{k - 1}}{{\hat P}_{k - 1}}F_{k - 1}^T + Q_k'\\{{\tilde x}_k} = f\left( {{{\hat x}_{k - 1}},{v_k},0} \right)\\{K_k} = {{\tilde P}_k}G_k^T{\left( {{G_k}{{\tilde P}_k}G_k^T + R_k'} \right)^{ - 1}}\\{{\hat P}_k} = \left( {I - {K_k}{G_k}} \right){{\tilde P}_k}\\{{\hat x}_k} = {{\tilde x}_k} + {K_k}\left( {{y_k} - g({{\tilde x}_k},0)} \right)\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D%0A%7B%7B%5Ctilde+P%7D_k%7D+%3D+%7BF_%7Bk+-+1%7D%7D%7B%7B%5Chat+P%7D_%7Bk+-+1%7D%7DF_%7Bk+-+1%7D%5ET+%2B+Q_k%27%5C%5C%0A%7B%7B%5Ctilde+x%7D_k%7D+%3D+f%5Cleft%28+%7B%7B%7B%5Chat+x%7D_%7Bk+-+1%7D%7D%2C%7Bv_k%7D%2C0%7D+%5Cright%29%5C%5C%0A%7BK_k%7D+%3D+%7B%7B%5Ctilde+P%7D_k%7DG_k%5ET%7B%5Cleft%28+%7B%7BG_k%7D%7B%7B%5Ctilde+P%7D_k%7DG_k%5ET+%2B+R_k%27%7D+%5Cright%29%5E%7B+-+1%7D%7D%5C%5C%0A%7B%7B%5Chat+P%7D_k%7D+%3D+%5Cleft%28+%7BI+-+%7BK_k%7D%7BG_k%7D%7D+%5Cright%29%7B%7B%5Ctilde+P%7D_k%7D%5C%5C%0A%7B%7B%5Chat+x%7D_k%7D+%3D+%7B%7B%5Ctilde+x%7D_k%7D+%2B+%7BK_k%7D%5Cleft%28+%7B%7By_k%7D+-+g%28%7B%7B%5Ctilde+x%7D_k%7D%2C0%29%7D+%5Cright%29%0A%5Cend%7Barray%7D%5C%5D)
 其中![\[F_{k-1} = {\left. {\frac{{\partial f}}{{\partial {x_{k - 1}}}}} \right|_{{{\hat x}_{k - 1}}}},{G_k} = {\left. {\frac{{\partial g}}{{\partial {x_k}}}} \right|_{{{\tilde x}_k}}}\]](https://www.zhihu.com/equation?tex=%5C%5BF_%7Bk-1%7D+%3D+%7B%5Cleft.+%7B%5Cfrac%7B%7B%5Cpartial+f%7D%7D%7B%7B%5Cpartial+%7Bx_%7Bk+-+1%7D%7D%7D%7D%7D+%5Cright%7C_%7B%7B%7B%5Chat+x%7D_%7Bk+-+1%7D%7D%7D%7D%2C%7BG_k%7D+%3D+%7B%5Cleft.+%7B%5Cfrac%7B%7B%5Cpartial+g%7D%7D%7B%7B%5Cpartial+%7Bx_k%7D%7D%7D%7D+%5Cright%7C_%7B%7B%7B%5Ctilde+x%7D_k%7D%7D%7D%5C%5D)
这样做听起来还是挺有道理的，实际上也是能用的，但是问题还是很多的。
考虑一个服从高斯分布的变量![x \sim N(0,1)](https://www.zhihu.com/equation?tex=x+%5Csim+N%280%2C1%29)，现在![y=x^2](https://www.zhihu.com/equation?tex=y%3Dx%5E2)，问![y](https://www.zhihu.com/equation?tex=y)服从什么分布？
我概率比较差，不过这个似乎是叫做卡尔方布。![y](https://www.zhihu.com/equation?tex=y)应该是下图中k=1那条线。
![](https://pic3.zhimg.com/a0dc2dcc745ddc48115d7d35fc6729ea_b.png)
但是按照EKF的观点，我们要用一个高斯分布去近似![y](https://www.zhihu.com/equation?tex=y)。假设我们采样时得到了一个![x=0.5](https://www.zhihu.com/equation?tex=x%3D0.5)，那么就会近似成一个均值为0.25的高斯分布，然而卡方分布的期望应该是1。……但是各位真觉得k=1那条线像哪个高斯分布吗？
所以EKF面临的一个重要问题是，当一个高斯分布经过非线性变换后，如何用另一个高斯分布近似它？按照它现在的做法，存在以下的局限性：（注意是滤波器自己的局限性，还没谈在SLAM问题里的局限性）。
- 即使是高斯分布，经过一个非线性变换后也不是高斯分布。EKF只计算均值与协方差，是在用高斯近似这个非线性变换后的结果。（实际中这个近似可能很差）。
- 系统本身线性化过程中，丢掉了高阶项。
- 线性化的工作点往往不是输入状态真实的均值，而是一个估计的均值。于是，在这个工作点下计算的![F,G](https://www.zhihu.com/equation?tex=F%2CG)，也不是最好的。
- 在估计非线性输出的均值时，EKF算的是![\mu_y=f(\mu_x)](https://www.zhihu.com/equation?tex=%5Cmu_y%3Df%28%5Cmu_x%29)的形式。这个结果几乎不会是输出分布的真正期望值。协方差也是同理。
那么，怎么克服以上的缺点呢？途径很多，主要看我们想不想维持EKF的假设。如果我们比较乖，希望维持高斯分布假设，可以这样子改：
- 为了克服第3条工作点的问题，我们以EKF估计的结果为工作点，重新计算一遍EKF，直到这个工作点变化够小。是为迭代EKF（Iterated EKF, IEKF）。
- 为了克服第4条，我们除了计算![\mu_y=f(\mu_x)](https://www.zhihu.com/equation?tex=%5Cmu_y%3Df%28%5Cmu_x%29)，再计算其他几个精心挑选的采样点，然后用这几个点估计输出的高斯分布。是为Sigma Point KF（SPKF，或UKF）。
如果不那么乖，可以说：**我们不要高斯分布假设，凭什么要用高斯去近似一个长得根本不高斯的分布呢？**于是问题变为，丢掉高斯假设后，怎么描述输出函数的分布就成了一个问题。一种比较暴力的方式是：用足够多的采样点，来表达输出的分布。这种蒙特卡洛的方式，也就是粒子滤波的思路。
如果再进一步，可以丢弃滤波器思路，说：**为什么要用前一个时刻的值来估计下一个时刻呢**？**我们可以把所有状态看成变量，把运动方程和观测方程看成变量间的约束，构造误差函数，然后最小化这个误差的二次型。**这样就会得到非线性优化的方法，在SLAM里就走向图优化那条路上去了。不过，非线性优化也需要对误差函数不断地求梯度，并根据梯度方向迭代，因而局部线性化是不可避免的。
可以看到，在这个过程中，我们逐渐放宽了假设。
============== **UKF 无迹卡尔曼 **（投影方法）==================
由于题主问题里没谈IEKF，我们就简单说说UKF和PF。
UKF主要解决一个高斯分布经过非线性变换后，怎么用另一个高斯分布近似它。假设![x \sim N(\mu_x \Sigma_{xx} ), y=g(x)](https://www.zhihu.com/equation?tex=x+%5Csim+N%28%5Cmu_x+%5CSigma_%7Bxx%7D+%29%2C+y%3Dg%28x%29)，我们希望用![N(\mu_y, \Sigma_{yy})](https://www.zhihu.com/equation?tex=N%28%5Cmu_y%2C+%5CSigma_%7Byy%7D%29)近似![y](https://www.zhihu.com/equation?tex=y)。按照EKF，需要对![g](https://www.zhihu.com/equation?tex=g)做线性化。但在UKF里，不必做这个线性化。
UKF的做法是找一些叫做Sigma Point的点，把这些点用![g](https://www.zhihu.com/equation?tex=g)投影过去。然后，用投影之后的点做出一个高斯分布，如下图：
![](https://pic2.zhimg.com/4ec9ce019bfa0ca1602c11349c3cca41_b.png)这里选了三个点：![\mu_x, \mu_x+\sigma_x, \mu_x-\sigma_x](https://www.zhihu.com/equation?tex=%5Cmu_x%2C+%5Cmu_x%2B%5Csigma_x%2C+%5Cmu_x-%5Csigma_x)。对于维数为N的分布，需要选2N+1个点。篇幅所限，这里就不解释这些点怎么选，以及为何要这样选了。总之UKF的好处就是：
- 不必线性化，也不必求导，对![g](https://www.zhihu.com/equation?tex=g)没有光滑性要求。
- 计算量随维数增长是线性的。
=============== **PF 粒子滤波** （蒙特卡洛方法）==================
UKF的一个问题是输出仍假设成高斯分布。然而，即使在很简单的情况下，高斯的非线性变换仍然不是高斯。并且，仅在很少的情况下，输出的分布有个名字（比如卡方），多数时候你都不知道他们是啥……更别提描述它们了。
因为描述很困难，所以粒子滤波器采用了一种暴力的，用大量采样点去描述这个分布的方法（老子就是无参的你来打我呀）。框架大概像下面这个样子，就是一个不断采样——算权重——重采样的过程：
![](https://pic4.zhimg.com/792fab6b2722deeaede9264c32480dbf_b.png)
越符合观测的粒子拥有越大的权重，而权重越大就越容易在重采样时被采到。当然，每次采样数量、权重的计算策略，则是粒子滤波器里几个比较麻烦的问题，这里就不细讲了。
这种采样思路的最大问题是：**采样所需的粒子数量，随分布是指数增长的**。所以仅限于低维的问题，高维的基本就没办法了。
=============== **非线性优化** ==================
非线性优化，计算的也是最大后验概率估计（MAP），但它的处理方式与滤波器不同。对于上面写的状态估计问题，可以简单地构造误差项：
![\[\begin{array}{l}{e_{v,k}}\left( x \right) = {x_k} - f\left( {{x_{k - 1}},{v_k},0} \right)\\{e_{y,k}}\left( x \right) = {y_k} - g\left( {{x_k},0} \right)\end{array}\]](https://www.zhihu.com/equation?tex=%5C%5B%5Cbegin%7Barray%7D%7Bl%7D%0A%7Be_%7Bv%2Ck%7D%7D%5Cleft%28+x+%5Cright%29+%3D+%7Bx_k%7D+-+f%5Cleft%28+%7B%7Bx_%7Bk+-+1%7D%7D%2C%7Bv_k%7D%2C0%7D+%5Cright%29%5C%5C%0A%7Be_%7By%2Ck%7D%7D%5Cleft%28+x+%5Cright%29+%3D+%7By_k%7D+-+g%5Cleft%28+%7B%7Bx_k%7D%2C0%7D+%5Cright%29%0A%5Cend%7Barray%7D%5C%5D)
然后最小化这些误差项的二次型：
![\[\min J\left( x \right) = \sum\limits_{k = 1}^K {\left( {\frac{1}{2}{e_{v,k}}{{\left( x \right)}^T}W_{v,k}^{ - 1}{e_{v,k}}\left( x \right)} \right) + \sum\limits_{k = 1}^K {\left( {\frac{1}{2}{e_{y,k}}{{\left( x \right)}^T}W_{v,k}^{ - 1}{e_{v,k}}\left( x \right)} \right)} } \]](https://www.zhihu.com/equation?tex=%5C%5B%5Cmin+J%5Cleft%28+x+%5Cright%29+%3D+%5Csum%5Climits_%7Bk+%3D+1%7D%5EK+%7B%5Cleft%28+%7B%5Cfrac%7B1%7D%7B2%7D%7Be_%7Bv%2Ck%7D%7D%7B%7B%5Cleft%28+x+%5Cright%29%7D%5ET%7DW_%7Bv%2Ck%7D%5E%7B+-+1%7D%7Be_%7Bv%2Ck%7D%7D%5Cleft%28+x+%5Cright%29%7D+%5Cright%29+%2B+%5Csum%5Climits_%7Bk+%3D+1%7D%5EK+%7B%5Cleft%28+%7B%5Cfrac%7B1%7D%7B2%7D%7Be_%7By%2Ck%7D%7D%7B%7B%5Cleft%28+x+%5Cright%29%7D%5ET%7DW_%7Bv%2Ck%7D%5E%7B+-+1%7D%7Be_%7Bv%2Ck%7D%7D%5Cleft%28+x+%5Cright%29%7D+%5Cright%29%7D+%7D+%5C%5D)
这里仅用到了噪声项满足高斯分布的假设，再没有更多的了。当构建一个非线性优化问题之后，就可以从一个初始值出发，计算梯度（或二阶梯度），优化这个目标函数。常见的梯度下降策略有牛顿法、高斯-牛顿法、Levenberg-Marquardt方法，可以在许多讲数值优化的书里找到。
非线性优化方法现在已经成为视觉SLAM里的主流，尤其是在它的稀疏性质被人发现且利用起来之后。它与滤波器最大不同点在于， 一次可以考虑整条轨迹中的约束。它的线性化，即雅可比矩阵的计算，也是相对于整条轨迹的。相比之下，滤波器还是停留在马尔可夫的假设之下，只用上一次估计的状态计算当前的状态。可以用一个图来表达它们之间的关系：
![](https://pic1.zhimg.com/b67b863d41f5e4e762dde44c516cb35c_b.png)
当然优化方式也存在它的问题。例如优化时间会随着节点数量增长——所以有人会提double window optimization这样的方式，以及可能落入局部极小。但是就目前而言，它比EKF还是优不少的。
=============== **小结** ==================
- **卡尔曼滤波是递归的线性高斯系统最优估计。**
- **EKF将NLNG系统在工作点附近近似为LG进行处理。**
- **IEKF对工作点进行迭代。**
- **UKF没有线性化近似，而是把sigma point进行非线性变换后再用高斯近似。**
- **PF去掉高斯假设，以粒子作为采样点来描述分布。**
- **优化方式同时考虑所有帧间约束，迭代线性化求解。**
呃好像题主还问了FastSLAM，有空再写吧……
注：* 本文大量观点来自Timothy. Barfoot, "State estimation for Robotics: A Matrix Lei Group Approach", 2016. 图片若有侵权望告知。
[编辑于 2017-01-07](https://www.zhihu.com/question/46916554/answer/103411007)
