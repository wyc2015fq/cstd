# 声学特征（三） pitch - xmdxcsj的专栏 - CSDN博客





2016年04月25日 20:50:42[xmucas](https://me.csdn.net/xmdxcsj)阅读数：6834
个人分类：[声学模型](https://blog.csdn.net/xmdxcsj/article/category/6166365)









## 基本含义

pitch跟声音的基频fundamental frequency（F0）有关，反应的是音高的信息，即声调。计算F0也被称为‘‘pitch detection algorithms（PDA）。

## YIN算法

sphinx使用的YIN算法提取pitch特征，相对简单而且进行了定点化。 

YIN来自于“阴阳”哲学，寓意在autocorrelation和cancellation之间的变换。YIN算法的演化流程：

### Step 1: The autocorrelation method

autocorrelation function（ACF）运算可以用于寻找周期信号的周期。因为周期信号的自相关函数也是周期信号，而且周期一致。 


$r_t(\tau)=\sum_{j=t+1}^{t+W}x_jx_{j+\tau}\ \ \ \ \  \ \ \ \ \ \ \ \ (1)$

W表示相关函数的窗口，$\tau$表示相关函数的延时。
### Step 2: Difference function



$d_t(\tau)=\sum_{j=1}^{W}(x_j-x_{j+\tau})^2$

YIN算法使用该函数替换上面的第一步的自相关函数，优势在于： 

当时域信号随着时间幅值变大的时候，导致自相关函数的峰值也会逐渐变大，最大的峰值就会后移，从而导致**周期变长**。而对于差分函数，幅值的变化会导致所有的$\tau$对应的差分值都会改变，减弱了这种影响。
### Step 3: Cumulative mean normalized difference function



$d_t'(\tau) =  \begin{cases}    1       & \quad \text{if } \tau=0\\    \frac{d_t(\tau)}{\frac{1}{\tau}\sum_{j=1}^{\tau}d_t(j)}  & \quad \text{otherwise}\\  \end{cases}$

相比于step2，减少了$\tau$比较小的时候出现零点的情况，减弱了**小周期**的产生。

### Step 4: Absolute threshold

step3，如果极小值出现的时间点比周期对应的时间点要长，会出现**周期变长**的情况。为了解决这类问题，可以设置一个绝对的门限（比如0.1），找到低于这个门限的每一段的极小值，这些极小值中的第一个极小值对应$\tau$作为周期（第三步是使用这些极小值的中的最小值对应的$\tau$作为周期）。如果没有低于门限的点，那么选择全局最小值作为周期。

### Step 5: Parabolic interpolation

前面的步骤有一个假设，即信号的周期是采样周期的倍数。如果不满足的话，会引起错误。 

为了解决这个问题，每个$d'(\tau)$的局部最小值以及相邻的点，可以使用二次函数抛物线进行拟合。后续周期计算的时候使用每个局部最小值对应的拟合值。找到的最小值对应的时间即为周期。使用这种方法有可能存在bias，可以用$d(\tau)$来避免这个问题。

### Step 6: Best local estimate

在找到的周期值附近，在时间窗口$[t-T_{max}/2, t+T_{max}/2]$寻找极小值对应的时间作为周期，$T_{max}$是最大期望周期值。 
**总体上，YIN算法使用$d'(\tau)$进行周期估计，同时设置一个绝对门限，在找得到周期值附近重新进行搜索获得最优值。**

## 参考

《YIN, a fundamental frequency estimator for speech and music》





