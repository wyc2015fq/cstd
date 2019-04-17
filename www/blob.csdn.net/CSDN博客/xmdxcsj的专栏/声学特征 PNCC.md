# 声学特征 PNCC - xmdxcsj的专栏 - CSDN博客





2017年11月12日 16:25:02[xmucas](https://me.csdn.net/xmdxcsj)阅读数：2331








## 特点

power-normalized cepstral coefficients相比于MFCC特征： 

- 在噪声和混响场景下提升识别效果，尤其在训练语料是clean语音的时候 

- 相比于MFCC，计算量提升34.6%
使用pncc相比mfcc，噪声和口音测试集可以得到10-15%的相对提升

## 细节

![这里写图片描述](https://img-blog.csdn.net/20171112162400318?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

和MFCC/PLP特征的整体对比如上图
### filter bank

相比于MFCC的triangular filters，PNCC使用gammatone filters，40维，截止频率分别是200/8000。 
![这里写图片描述](https://img-blog.csdn.net/20171112162420061?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

获得$P[m,l]$,m表示frame，l表示channel
### medium-time power calculation

由于噪声的能量变化相比语音慢很多，所以更大的时间窗口可以得到更好的性能，所以对每一帧进行了平滑处理(左右2帧做平均)。 


$\bar Q[m,l]=\frac{1}{2M+1}\sum_{m'=m-M}^{m+M}P[m',l]$

得到的$\bar Q[m,l]$用于后面的噪声估计和补偿
### asymmetric noise suppression

因为语音的能量相比噪声变化快，所以使用谱减法来过滤掉低频部分以达到抑制噪声的目的。 
![这里写图片描述](https://img-blog.csdn.net/20171112162435460?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQveG1keGNzag==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

其中的temperal masking的引入可以减弱混响的影响，首先获得每个channel的moving peak，如果某一帧的能量低于这个peak曲线，缩小对应的能量。 

获得$\bar R[m,l]$
### spectral weight smoothing

在不同的channel之间做平滑。 


$\bar s[m,l]=(\frac{1}{l_2-L_1+1}\sum_{l'=l_1}^{l_2}\frac{\bar R[m,l']}{\bar Q[m,l']})$

其中$l_2=min(l+N,L)$，$L$表示channel的个数，$l_1=max(l-N,1)$，$N$设为4 
**$\bar S[m,l]$通过medium-time power calculation实现了在时间维度上的平滑，通过spectral weight smoothing则实现了在频率维度上的平滑，时域是左右2帧，频域是左右4个channel**

最后调制$P[m,l]$得到 


$T[m,l]=P[m,l]\bar S[m,l]$
### mean power normalization

均值的获取使用online的形式 


$\mu[m]=\lambda_{\mu}\mu [m-1]+\frac{1-\lambda_{\mu}}{L}\sum_{l=0}^{L-1}T[m,l]$

归一化以后： 


$U[m,l]=k\frac{T[m,l]}{\mu [m]}$
### rate-level nonlinearity

相比于MFCC使用的log非线性函数，PNCC使用指数函数，更符合人耳听觉神经的压缩感知 


$V[m,l]=U[m,l]^{1/15}$

## 参考

Power-Normalized Cepstral Coefficients (PNCC) for Robust Speech Recognition 
[http://www.cs.cmu.edu/~robust/archive/algorithms/PNCC_C](http://www.cs.cmu.edu/~robust/archive/algorithms/PNCC_C)










