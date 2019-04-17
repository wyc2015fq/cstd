# 3分钟tips：高斯分布和高斯积分的关系 - Machine Learning with Peppa - CSDN博客





2018年04月06日 18:20:39[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：1800








众所周知，高斯积分在概率论和连续傅里叶变换等的统一化等计算中有广泛的应用。在误差函数的定义中它也出现。虽然误差函数没有初等函数，但是高斯积分可以通过微积分学的手段解析求解。将高斯函数调整到高斯积分在概率论和连续傅里叶变换等的统一化等计算中有广泛的应用。在误差函数的定义中它也出现。虽然误差函数没有初等函数，但是高斯积分可以通过微积分学的手段解析求解。

![](https://img-blog.csdn.net/20180406181753961?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

（Gaussian quadrature）  表示f(x,y)在曲线L上的第一型曲线积分。

![](https://gss2.bdstatic.com/-fo3dSag_xI4khGkpoWK1HF6hhy/baike/s%3D124/sign=35aed77cd558ccbf1fbcb1382dd9bcd4/7c1ed21b0ef41bd52b1e963a55da81cb38db3daa.jpg)


而正态分布（又名高斯分布）是标准的一类位置-尺度族分布，且在数学、物理及工程等领域都非常重要的概率分布，在统计学的许多方面有着重大的影响力。若随机变量X服从一个数学期望为μ、方差为σ^2的高斯分布，记为N(μ，σ^2)。其概率密度函数为

![](https://img-blog.csdn.net/20180406181833294?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



此即正态分布函数，期望值μ决定了其位置，标准差σ决定了分布的幅度。**将高斯函数调整到单位面积，即为正态分布。**


