# DFT与DTFT区别 - qq_32515081的博客 - CSDN博客
2018年04月10日 22:04:32[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：1134
DFT是有限长信号的傅立叶表示，DTFT是无限长信号的傅立叶表示。
DFT的定义为：
![X\left[k\right]=\sum_{n=0}^{N-1}{x\left[n\right]e^{-j{\frac{2\pi}{N}}kn}} ,0\le k \le N-1](https://www.zhihu.com/equation?tex=X%5Cleft%5Bk%5Cright%5D%3D%5Csum_%7Bn%3D0%7D%5E%7BN-1%7D%7Bx%5Cleft%5Bn%5Cright%5De%5E%7B-j%7B%5Cfrac%7B2%5Cpi%7D%7BN%7D%7Dkn%7D%7D+%2C0%5Cle+k+%5Cle+N-1)
无论![N](https://www.zhihu.com/equation?tex=N)如何变化，DFT的谐波频率始终在区间![\left[0,2\pi\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B0%2C2%5Cpi%5Cright%5D)之内：
![0\le \omega_k=\frac{2\pi}{N}k < 2\pi](https://www.zhihu.com/equation?tex=0%5Cle+%5Comega_k%3D%5Cfrac%7B2%5Cpi%7D%7BN%7Dk+%3C+2%5Cpi)
当![N\to\infty](https://www.zhihu.com/equation?tex=N%5Cto%5Cinfty)，DFT对于区间![\left[0,2\pi\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B0%2C2%5Cpi%5Cright%5D)的划分越来越细致，![\omega_k](https://www.zhihu.com/equation?tex=%5Comega_k)从离散的值最终变成整个实数区间。（相当于往区间![\left[0,2\pi\right]](https://www.zhihu.com/equation?tex=%5Cleft%5B0%2C2%5Cpi%5Cright%5D)中不断插值），或者说DFT是DTFT在![\omega =\frac{2\pi}{N}k](https://www.zhihu.com/equation?tex=%5Comega+%3D%5Cfrac%7B2%5Cpi%7D%7BN%7Dk)处的采样。
DTFT的定义为：
![X\left(\omega \right)=\sum_{n=-\infty}^{\infty}{x\left[n\right]e^{-j\omega n}} ,0\le \omega  < 2\pi](https://www.zhihu.com/equation?tex=X%5Cleft%28%5Comega+%5Cright%29%3D%5Csum_%7Bn%3D-%5Cinfty%7D%5E%7B%5Cinfty%7D%7Bx%5Cleft%5Bn%5Cright%5De%5E%7B-j%5Comega+n%7D%7D+%2C0%5Cle+%5Comega++%3C+2%5Cpi)
DTFT是![N\to\infty](https://www.zhihu.com/equation?tex=N%5Cto%5Cinfty)时DFT的极限。
DTFT的概念性更强，主要用于理论分析，可以统一于![Z](https://www.zhihu.com/equation?tex=Z)变换。
DFT可以应用于实践，对于计算机来说比较友好。
作者：psyduck
链接：https://www.zhihu.com/question/23137926/answer/25327783
来源：知乎
著作权归作者所有。商业转载请联系作者获得授权，非商业转载请注明出处。            
