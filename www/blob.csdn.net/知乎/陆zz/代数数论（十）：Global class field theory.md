# 代数数论（十）：Global class field theory - 知乎
# 

Class field theory中的class 既可以指理想群中的理想类，也可以指idele class group与Hilbert class field。

在[代数数论（八）：Hilbert 94 and class field theory](https://zhuanlan.zhihu.com/p/24216795?refer=shuxuehenyouyisisuoyiyaohaohaoxue)已经提到过，class field theory可以看成**寻找arithmetic scheme的基本群的内在刻画**，因此可以看成是Galois theory for number field的进一步加强。

至于Global class field theory ,其可用local case所得到，具体来说是因为idele是local field的restrict product，因此idele的取范映射就是local field的取范的复合，而整体Galois扩张又诱导每个位上的Galois扩张。

idele group是集local于一体的对象，单独来看并没有反映整体的信息，但数域也可以嵌入idele和adele中，因此其包含了整体的对象。

而idele class group![C_K= \mathbb I_K /K^{\times}](https://www.zhihu.com/equation?tex=C_K%3D+%5Cmathbb+I_K+%2FK%5E%7B%5Ctimes%7D)是把局部的对象整合起来商掉一个整体的关系，因此其能反映local information 如何通过global field 联系，因此可用来描述整体信息。

上面说了一堆废话，Global class field theory就是说：

**number field K的有限Abel扩张 与 ![C_K= \mathbb I_K /K^{\times}](https://www.zhihu.com/equation?tex=C_K%3D+%5Cmathbb+I_K+%2FK%5E%7B%5Ctimes%7D)的有限指标的开子群一一反序对应，具体对应为：**

![E/K](https://www.zhihu.com/equation?tex=E%2FK)对应![N_{L/K}C_L](https://www.zhihu.com/equation?tex=N_%7BL%2FK%7DC_L),且![Gal(E/K) \cong C_K/N_{L/K}C_L](https://www.zhihu.com/equation?tex=Gal%28E%2FK%29+%5Ccong+C_K%2FN_%7BL%2FK%7DC_L)

并且Global与local的类域论可以通过local field 嵌入 idele group 联系起来，其可以用来反映数域的分歧情况

应用1：

我们之前已经提到过Hilbert class field这一应用，想法就是取![C_K= \mathbb I_K /K^{\times}](https://www.zhihu.com/equation?tex=C_K%3D+%5Cmathbb+I_K+%2FK%5E%7B%5Ctimes%7D)的一个好一些的子群，使得其在每个位上都不分歧，那么其对应域扩张就是Hilbert class field：

应用2：

如果我们可以取 ![C_K= \mathbb I_K /K^{\times}](https://www.zhihu.com/equation?tex=C_K%3D+%5Cmathbb+I_K+%2FK%5E%7B%5Ctimes%7D)一些有限指标的开子群![H_1\supseteq H_2 \supseteq  \hdots](https://www.zhihu.com/equation?tex=H_1%5Csupseteq+H_2+%5Csupseteq++%5Chdots),使得其构成1处一个open basis，设H_i对应域为E_i

那么对于任何一个![C_K= \mathbb I_K /K^{\times}](https://www.zhihu.com/equation?tex=C_K%3D+%5Cmathbb+I_K+%2FK%5E%7B%5Ctimes%7D)的有限指标的开子群，其都会包含某个H_i，

由class field theory，那么任何一个Abel扩张![E/K](https://www.zhihu.com/equation?tex=E%2FK)，都会包含在某个E_i,这样的想法就导致了

# Kronecker–Weber theorem

**任何一个Q的Abel扩张都是某个分圆域的子域**

因为我们可以求出![C_{\mathbb Q}= \mathbb I_{\mathbb Q} /{\mathbb Q}^{\times}](https://www.zhihu.com/equation?tex=C_%7B%5Cmathbb+Q%7D%3D+%5Cmathbb+I_%7B%5Cmathbb+Q%7D+%2F%7B%5Cmathbb+Q%7D%5E%7B%5Ctimes%7D)的结构：
![C_{\mathbb Q}= \mathbb I_{\mathbb Q} /{\mathbb Q}^{\times} \cong \mathbb R_{>0} \times \prod_{p}^{} \mathbb Z_p^{\times}  ](https://www.zhihu.com/equation?tex=C_%7B%5Cmathbb+Q%7D%3D+%5Cmathbb+I_%7B%5Cmathbb+Q%7D+%2F%7B%5Cmathbb+Q%7D%5E%7B%5Ctimes%7D+%5Ccong+%5Cmathbb+R_%7B%3E0%7D+%5Ctimes+%5Cprod_%7Bp%7D%5E%7B%7D+%5Cmathbb+Z_p%5E%7B%5Ctimes%7D++)

然后计算出分圆域![\mathbb Q(\zeta_n)](https://www.zhihu.com/equation?tex=%5Cmathbb+Q%28%5Czeta_n%29)对应开子群，再验证他们确实是一组开集基即可。

故事似乎还可以进行下去，比如

①Proof class field theory in detail

②some generalizations of class field theory， like higher class field theory (generalization to higher dimensional scheme ) and Langlands program (generalization to nonabelian case)

但如果试图叙述Class field theory的证明又稍显冗长，而推广又缺少太多背景知识，因此不如待以后有空再做进一步的讨论，我们接下来应该讨论一些更有趣的东西。

作为补充，下面是两个非常好的东西：

内容总结：

[A BRIEF SUMMARY OF THE STATEMENTS OF CLASS FIELD THEORY BJORN POONEN](https://link.zhihu.com/?target=http%3A//www-math.mit.edu/~poonen/papers/cft.pdf)

历史（强烈推荐，介绍了class field theory的历史）：

[HISTORY OF CLASS FIELD THEORY KEITH CONRAD](https://link.zhihu.com/?target=http%3A//www.math.uconn.edu/~kconrad/blurbs/gradnumthy/cfthistory.pdf)

