# Parseval’s theorem帕萨瓦尔定理及其证明过程 - qq_32515081的博客 - CSDN博客
2018年03月29日 20:52:24[TengAdventure](https://me.csdn.net/qq_32515081)阅读数：989
      Parseval 定理是信号视频分析，相关推导过程汇总最常用的定理之一，我们较为常用的表述是，信号在时域和频域上的功率相等，现在找到一个较为详细的帕萨瓦尔定理的原始版本及其证明过程， 现做一个记录。
       帕萨瓦尔定理通过下述公式揭示了信号在时域和频域上的能量 关系，它的定义如下：
![](https://img-blog.csdn.net/20180305090523462?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ1OTgzODc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
     其中f(t)是任意一个在时域上变化的信号，F（f）是其等价的在傅里叶频域上的变化结果。
     在 [物理学](http://zh.wikipedia.org/wiki/%E7%89%A9%E7%90%86%E5%AD%A6) 和 [工程学](http://zh.wikipedia.org/wiki/%E5%B7%A5%E7%A8%8B%E5%AD%A6) 中,  帕塞瓦尔定理通常描述如下:
![\int_{-\infty}^\infty | x(t) |^2 \, dt   =   \int_{-\infty}^\infty | X(f) |^2 \, df](http://upload.wikimedia.org/math/2/8/5/2851d1df3d56f6f7060863dd5c5ac0ff.png)
其中![X(f) = \mathcal{F} \{ x(t) \}](http://upload.wikimedia.org/math/0/6/9/069c8ea91f3319acd3b009143043b126.png) 为 x(t)  的[连续傅立叶变换](http://zh.wikipedia.org/wiki/%E8%BF%9E%E7%BB%AD%E5%82%85%E7%AB%8B%E5%8F%B6%E5%8F%98%E6%8D%A2)（以归一化酉形式），而f代表x的频率分量（非[角频率](http://zh.wikipedia.org/wiki/%E8%A7%92%E9%A2%91%E7%8E%87)）
帕塞瓦尔定理的此表达形式解释了波形x(t)依时间域t累积的总[能量](http://zh.wikipedia.org/wiki/%E8%83%BD%E9%87%8F)与该波形的傅立叶变换X(f)在频域域f累积的总能量相等。
        对于[离散时间](http://zh.wikipedia.org/w/index.php?title=%E7%A6%BB%E6%95%A3%E6%97%B6%E9%97%B4&action=edit&redlink=1)[信号](http://zh.wikipedia.org/wiki/%E4%BF%A1%E5%8F%B7)，该理论表达式变换为：
![\sum_{n=-\infty}^\infty | x[n] |^2 = \frac{1}{2\pi} \int_{-\pi}^\pi | X(e^{i\phi}) |^2 d\phi](http://upload.wikimedia.org/math/1/7/4/1747ac6e2c1bfde886693070da5c84c4.png)
其中，X为x的[离散时间傅立叶变换](http://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E6%97%B6%E9%97%B4%E5%82%85%E7%AB%8B%E5%8F%B6%E5%8F%98%E6%8D%A2)(DTFT)，而Φ为x的[角频率](http://zh.wikipedia.org/wiki/%E8%A7%92%E9%A2%91%E7%8E%87)（[度](http://zh.wikipedia.org/wiki/%E5%BA%A6)每样本）。
       此外，对于[离散傅立叶变换](http://zh.wikipedia.org/wiki/%E7%A6%BB%E6%95%A3%E5%82%85%E7%AB%8B%E5%8F%B6%E5%8F%98%E6%8D%A2) (DFT)，表达式变换为：
![\sum_{n=0}^{N-1} | x[n] |^2  =   \frac{1}{N} \sum_{k=0}^{N-1} | X[k] |^2](http://upload.wikimedia.org/math/d/d/3/dd3e997e03ee61f3c43c2e96731deb83.png)
其中，X[k]为x[n]的DFT变换，变换前后样本长度皆为N。
其证明过程如下：
![](https://img-blog.csdn.net/20180305090743882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ1OTgzODc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
    该定理说明了信号f(t)的总能量等于其傅里叶变换后频域上的面积积分。![](https://img-blog.csdn.net/20180305090958139?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ1OTgzODc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)一般被称作能量密度，谱密度，或者功率谱密度函数并且![](https://img-blog.csdn.net/20180305091052856?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMjQ1OTgzODc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)描述了在微分频带f到f+dF上所包含的信号能量。
转自：https://blog.csdn.net/qq_24598387/article/details/79441702
