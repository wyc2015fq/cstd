# 迹公式 Trace Formula (1) - 知乎
# 



知乎好像没这方面的文章，在此写一点。考虑群 G 和 G 的子群 H。

例如，![G = \mathbb{R}](https://www.zhihu.com/equation?tex=G+%3D+%5Cmathbb%7BR%7D)，![H = \mathbb{Z}](https://www.zhihu.com/equation?tex=H+%3D+%5Cmathbb%7BZ%7D)，则![H \backslash G = S^1](https://www.zhihu.com/equation?tex=H+%5Cbackslash+G+%3D+S%5E1)。

注意到 G 自然作用在![L^2(G)](https://www.zhihu.com/equation?tex=L%5E2%28G%29)上。令![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29) 是![L^2(G)](https://www.zhihu.com/equation?tex=L%5E2%28G%29)中在 H 的左作用下不变的函数。所以 G 也自然作用在![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29)上。


所以![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29) 是 G 的表示；那么，它如何分解为 G 的不可约表示？这个问题，很重要，很复杂，很本质。举例：
- 对于![L^2(\mathbb{Z} \backslash \mathbb{R})](https://www.zhihu.com/equation?tex=L%5E2%28%5Cmathbb%7BZ%7D+%5Cbackslash+%5Cmathbb%7BR%7D%29)的研究，是 Fourier 级数的理论。

- 对于![L^2(SL_2\mathbb{Z} \backslash SL_2\mathbb{R})](https://www.zhihu.com/equation?tex=L%5E2%28SL_2%5Cmathbb%7BZ%7D+%5Cbackslash+SL_2%5Cmathbb%7BR%7D%29)的研究，是模形式的理论。

- 对于![L^2(G(F) \backslash G(\mathbb{A}))](https://www.zhihu.com/equation?tex=L%5E2%28G%28F%29+%5Cbackslash+G%28%5Cmathbb%7BA%7D%29%29)的研究，是自守表示的理论（这里![\mathbb{A}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BA%7D)是 adèle）。



看最简单的例子：
- 如果 H = G 是整个 G，那么 ![L^2(H \backslash G) = \mathbb{C}](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29+%3D+%5Cmathbb%7BC%7D) 无疑是 G 的平凡表示，分解为 G 的不可约表示的系数是 ![(1, 0, 0, 0, \cdots)](https://www.zhihu.com/equation?tex=%281%2C+0%2C+0%2C+0%2C+%5Ccdots%29) 。

- 如果 H = 1 是平凡子群，那么 ![L^2(H \backslash G) = L^2(G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29+%3D+L%5E2%28G%29) 是 G 的左正则表示，分解为 G 的不可约表示的系数是 ![(d_{\pi_1}, d_{\pi_2}, d_{\pi_3}, d_{\pi_4}, \cdots)](https://www.zhihu.com/equation?tex=%28d_%7B%5Cpi_1%7D%2C+d_%7B%5Cpi_2%7D%2C+d_%7B%5Cpi_3%7D%2C+d_%7B%5Cpi_4%7D%2C+%5Ccdots%29) 。



以上第 2 点是说，可在![L^2(G)](https://www.zhihu.com/equation?tex=L%5E2%28G%29)找到 G 的所有不可约表示![\{\pi_i\}](https://www.zhihu.com/equation?tex=%5C%7B%5Cpi_i%5C%7D)（忽略一些细节），且分解为![\pi_i](https://www.zhihu.com/equation?tex=%5Cpi_i)的系数恰好等于![\pi_i](https://www.zhihu.com/equation?tex=%5Cpi_i)的维数![d_{\pi_i}](https://www.zhihu.com/equation?tex=d_%7B%5Cpi_i%7D)。这是 Peter-Weyl 定理。

可以想象，如果 H 不大不小，![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29)的分解，会很微妙。**迹公式的最直接用途，正是回答这个问题。这个问题在![H \backslash G](https://www.zhihu.com/equation?tex=H+%5Cbackslash+G)非紧的情况下最有趣**（非紧群的表示本来就很微妙了）。

迹公式的通用形式来自于 Arthur-Selberg。回顾表示论的核心对应：

> ***共轭类*** <=> ***表示（不可约酉表示)***

例如：
- 对于有限群的情况，两边的元素一样多。

- 对于对称群![S_n](https://www.zhihu.com/equation?tex=S_n)的情况，甚至可以一一对应到 Young 图。

- 但是，绝大多数情况下，两边并没有明确的一一对应。这就像某个矢量空间如有两种方法构造基，两组基的维数一定相同，但不见得能一一对应。



为将此对应精确化，不妨令测试函数 f 是 G 上的类函数，经过一系列简单变换，即可证明 Arthur-Selberg 迹公式：

> 
将 f 对于 H 中的 ***共轭类 ***加权求和（权，是每个 ***共轭类*** 在 G 中的 ***相对体积 ***）

等于 

将 f 的 ***傅里叶变换 ***对于 G 的 ***表示 ***加权求和（权，是每个 ***表示*** 在![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29)中的 ***分解系数*** ）


以上的傅里叶变换是指 G 上的傅里叶变换，就是把 f 乘上表示的特征在 G 上积分。

这个公式在![L^2(\mathbb{Z} \backslash \mathbb{R})](https://www.zhihu.com/equation?tex=L%5E2%28%5Cmathbb%7BZ%7D+%5Cbackslash+%5Cmathbb%7BR%7D%29)的情况，就是 Poisson 求和公式。

虽然有了公式，不代表两边都可以计算，有可能两边都不能计算。幸运的是：
- 对于共轭类的求和，写出来是轨道积分，可以用 Weyl 积分公式等等处理，叫做公式的 ***几何面*** ，经常很混乱，但可以算。

- 对于表示的求和，叫做公式的 ***谱面*** ，其中的分解系数很神秘，正好可由迹公式告诉我们。
- 对于非紧的情况，收敛性无疑是问题。关于此，有很多技巧。


最后，为什么这叫做迹公式？迹，在哪里？

**这是因为，迹公式的证明，是通过考虑某个算子的迹的两种计算方法。**如果把算子写成矩阵，有两种方法计算算子的迹：
- 把对角线上的元素加起来（这就是几何面）。

- 把矩阵的本征值加起来（这就是谱面）。



令人欣慰的是，本篇没有出现任何充满符号的公式，下篇继续。

如果需要符号，这是 Arthur-Selberg 迹公式（大家习惯把 H 写成 ![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)，对应所谓 G 的离散子群的情况）：

![\sum_{[\gamma] \in Conj(\Gamma)} vol(\Gamma_{\gamma} \backslash G_\gamma) \int_{G_{\gamma} \backslash G}  f(g^{-1} \gamma g)\,dg = tr R_f = \sum_\pi m(\pi, R) \int_G f(g) \chi_\pi(g) dg](https://www.zhihu.com/equation?tex=%5Csum_%7B%5B%5Cgamma%5D+%5Cin+Conj%28%5CGamma%29%7D+vol%28%5CGamma_%7B%5Cgamma%7D+%5Cbackslash+G_%5Cgamma%29+%5Cint_%7BG_%7B%5Cgamma%7D+%5Cbackslash+G%7D++f%28g%5E%7B-1%7D+%5Cgamma+g%29%5C%2Cdg+%3D+tr+R_f+%3D+%5Csum_%5Cpi+m%28%5Cpi%2C+R%29+%5Cint_G+f%28g%29+%5Cchi_%5Cpi%28g%29+dg)

两道思考题：
- 令![G = S_n](https://www.zhihu.com/equation?tex=G+%3D+S_n)是对称群，令![H](https://www.zhihu.com/equation?tex=H)是你喜欢的子群。请问![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29)如何分解？

- 令![G = H \times H](https://www.zhihu.com/equation?tex=G+%3D+H+%5Ctimes+H)，注意到 H 可以对角线嵌入![H \times H](https://www.zhihu.com/equation?tex=H+%5Ctimes+H)。请问![L^2(H \backslash G)](https://www.zhihu.com/equation?tex=L%5E2%28H+%5Cbackslash+G%29)如何分解？

> 如需转载本系列，请先与本人联系，谢谢。小广告：晚上工作学习是否觉得光线不够舒服，精神不够集中，眼睛容易疲劳？不妨点击看看我们的自然全光谱灯系列：[Blink Sunshine护眼LED灯泡 高显指97显色指数无频闪学习台灯床头](https://link.zhihu.com/?target=https%3A//item.taobao.com/item.htm%3Fid%3D40134613056) 如果需要好用的耳机或钱包，我们也有 :-)


