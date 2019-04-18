# 迹公式 Trace Formula (2) - 知乎
# 



上一篇没有出现公式，这篇专门看看公式。如果你在网络上搜索迹公式，很可能只会看见 Selberg 迹公式，就是那个双曲空间上的那个：
![\sum_{n=0}^\infty h(r_n) =  \frac{\mu(X)}{4 \pi } \int_{-\infty}^\infty r \, h(r) \tanh(\pi r)\,dr  +  \sum_{ \{T\} } \frac{ \log N(T_0) }{ N(T)^{\frac{1}{2}} - N(T)^{-\frac{1}{2}} } g(\log N(T))](https://www.zhihu.com/equation?tex=%5Csum_%7Bn%3D0%7D%5E%5Cinfty+h%28r_n%29+%3D++%5Cfrac%7B%5Cmu%28X%29%7D%7B4+%5Cpi+%7D+%5Cint_%7B-%5Cinfty%7D%5E%5Cinfty+r+%5C%2C+h%28r%29+%5Ctanh%28%5Cpi+r%29%5C%2Cdr++%2B++%5Csum_%7B+%5C%7BT%5C%7D+%7D+%5Cfrac%7B+%5Clog+N%28T_0%29+%7D%7B+N%28T%29%5E%7B%5Cfrac%7B1%7D%7B2%7D%7D+-+N%28T%29%5E%7B-%5Cfrac%7B1%7D%7B2%7D%7D+%7D+g%28%5Clog+N%28T%29%29)

但是 Langlands 说 Selberg zeta 函数对于他是不存在的，因为 Selberg 迹公式并不是一个数论公式！

真正和数论有关的迹公式是怎样的？最简单的是 GL(2) 上的迹公式，这就有点长了，我甚至不想把它打出来了，我们看图，下面 G 就是 GL(2)，上划线是模掉中心 ![\overline G = Z(G) \backslash G](https://www.zhihu.com/equation?tex=%5Coverline+G+%3D+Z%28G%29+%5Cbackslash+G)：
![](https://pic3.zhimg.com/v2-a9f2a6cfa307395badaf5c829cf4e90e_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1134' height='826'></svg>)
如果觉得还不够长，那么还有 GL(3) 的迹公式（真心很长），等等等等。

为什么这个公式这么长？前一篇我们说过，迹公式的基本形式是：

> ***几何面 = 谱面***

那么，对于 ![L^2(GL_2(\mathbb{Q}) \backslash GL_2(\mathbb{A}))](https://www.zhihu.com/equation?tex=L%5E2%28GL_2%28%5Cmathbb%7BQ%7D%29+%5Cbackslash+GL_2%28%5Cmathbb%7BA%7D%29%29) 的情况（先不要问下面的每一项是什么意思，说来话长呀），共轭类可以分为 central，elliptic，hyperbolic，unipotent；表示可以分为 cuspidal，continuous，residue；再把每一项的具体形式化简，就出来一堆项目了：

![J_{{central}} +J_{{elliptic}} +  J_{{hyperbolic}} + J_{{unipotent}} = J_{{cuspidal}} + J_{{continuous}} + J_{{residue}}](https://www.zhihu.com/equation?tex=J_%7B%7Bcentral%7D%7D+%2BJ_%7B%7Belliptic%7D%7D+%2B++J_%7B%7Bhyperbolic%7D%7D+%2B+J_%7B%7Bunipotent%7D%7D+%3D+J_%7B%7Bcuspidal%7D%7D+%2B+J_%7B%7Bcontinuous%7D%7D+%2B+J_%7B%7Bresidue%7D%7D)

此前公式右边的 (i) 是 central，(ii) 是 elliptic，(iii) 是 unipotent，(iv) 是 hyperbolic，(v) 是 continuous，(vi) 和 (vii) 是 residue；而公式的左边就是 cuspidal，我们最想要算的东西。


举个例子。对于尖点形式的空间 ![S_k(N, \omega^\prime)](https://www.zhihu.com/equation?tex=S_k%28N%2C+%5Comega%5E%5Cprime%29) 上的 Hecke 算子 ![T_n](https://www.zhihu.com/equation?tex=T_n)，可以找到合适的场景，使得它的迹可以通过迹公式运算。经过一系列漂亮的运算，可以将此时的迹公式化简为（里面的项目很有趣，譬如 h 来自类数）：

![](https://pic1.zhimg.com/v2-baa91ca39049d4a2273f9f86e6a4f0dc_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='619' height='215'></svg>)这就很厉害了。最简单的运用是令 ![n=1](https://www.zhihu.com/equation?tex=n%3D1)，即可得到 ![S_k(N, \omega^\prime)](https://www.zhihu.com/equation?tex=S_k%28N%2C+%5Comega%5E%5Cprime%29) 的维数公式。更重要的是，我们可以再算 ![tr(T_n^m)](https://www.zhihu.com/equation?tex=tr%28T_n%5Em%29)（需要用递归公式，运算量颇大）从而得到 Hecke eigenvalue 的大量信息（请回忆起你的线性代数知识）。

然而，上述的运算都非常说来话长。下篇我们看个相对简单点的数论公式，Petersson 迹公式：

![\frac{\Gamma(k-1)}{(4\pi \sqrt{mn})^{k-1}} \sum_{f \in \mathcal{F}} \bar{f}(m) f(n) = \delta_{mn} + 2\pi i^{-k} \sum_{c > 0}\frac{S(m,n;c)}{c} J_{k-1}\left(\frac{4\pi \sqrt{mn}}{c}\right)](https://www.zhihu.com/equation?tex=%5Cfrac%7B%5CGamma%28k-1%29%7D%7B%284%5Cpi+%5Csqrt%7Bmn%7D%29%5E%7Bk-1%7D%7D+%5Csum_%7Bf+%5Cin+%5Cmathcal%7BF%7D%7D+%5Cbar%7Bf%7D%28m%29+f%28n%29+%3D+%5Cdelta_%7Bmn%7D+%2B+2%5Cpi+i%5E%7B-k%7D+%5Csum_%7Bc+%3E+0%7D%5Cfrac%7BS%28m%2Cn%3Bc%29%7D%7Bc%7D+J_%7Bk-1%7D%5Cleft%28%5Cfrac%7B4%5Cpi+%5Csqrt%7Bmn%7D%7D%7Bc%7D%5Cright%29)

> 如需转载本系列，请先与本人联系，谢谢。小广告：晚上工作学习是否觉得光线不够舒服，精神不够集中，眼睛容易疲劳？不妨点击看看我们的自然全光谱灯系列：[Blink Sunshine护眼LED灯泡 高显指97显色指数无频闪学习台灯床头](https://link.zhihu.com/?target=https%3A//item.taobao.com/item.htm%3Fid%3D40134613056)  如果需要好用的耳机或钱包，我们也有 :-)


