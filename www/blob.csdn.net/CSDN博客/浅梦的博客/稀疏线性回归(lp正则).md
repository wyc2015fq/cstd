# 稀疏线性回归(lp正则) - 浅梦的博客 - CSDN博客





2017年08月30日 11:32:08[浅梦s](https://me.csdn.net/u012151283)阅读数：975








# lp范数惩罚项

$||w||_p=(\sum\limits_{j=1}^d|w_j|^p)^{\frac{1}{p}}\text{for}0<p\le\infty$

# lp回归

$w_{l_p}=\arg\min\limits_w||y-Xw||_2^2+\lambda||w||_p^p$

# 图解

![这里写图片描述](https://img-blog.csdn.net/20170830113010917?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

# 求解方法

![这里写图片描述](https://img-blog.csdn.net/20170830113123119?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMjE1MTI4Mw==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)



