# 瞎逛mathoverflow之[finite-fields] tag - 知乎
# 

逛mathoverflow发现了一些有趣的问题，记录于此。每次选择尽量少而有趣。

往期：

[瞎逛mathoverflow之[linear-algebra] tag](https://zhuanlan.zhihu.com/p/29614515) 9.23

——————————————————

下面的问题都带有[finite-fields]的tag:

1.[Non-split extensions of GL_n(F_q) by F_q^n  ?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/29417/non-split-extensions-of-gl-nf-q-by-f-qn)

由于有限域的结构较简单（乘法群和Galois群都有限循环）以及Hochschild-Serre谱序列，上同调的计算变得容易。由此可导出一些有趣的结论，如有限域间范数映射总是满的，![GL_n(\Bbb F_q)](https://www.zhihu.com/equation?tex=GL_n%28%5CBbb+F_q%29) 通过 ![\Bbb F_q^n](https://www.zhihu.com/equation?tex=%5CBbb+F_q%5En) 上的扩张都是平凡的（q>2），Jacquet函子的正合性等。

2.[Good source for representation of GL(n) over finite fields?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/270889/good-source-for-representation-of-gln-over-finite-fields)

对于有限域 ![F](https://www.zhihu.com/equation?tex=F) 上线性代数群 ![G](https://www.zhihu.com/equation?tex=G) ，![G(F)](https://www.zhihu.com/equation?tex=G%28F%29) 是有限群，自然可考虑其常表示理论。由线性代数可知![GL_n(F)](https://www.zhihu.com/equation?tex=GL_n%28F%29) 的共轭类由有理标准型刻画，而通过抛物子群的诱导表示可得到 ![GL_n(F)](https://www.zhihu.com/equation?tex=GL_n%28F%29) 一大类特征标，其余特征标称为尖的。n=2时的尖特征标可由其他子群如非分裂环面的诱导表示得到，n>2时也有完全分类。尖特征对应表示的具体构造可利用[Deligne-Lusztig](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/203602/learning-deligne-lusztig-theory)理论，基本的例子是 ![SL_2(\Bbb F_q)](https://www.zhihu.com/equation?tex=SL_2%28%5CBbb+F_q%29) 作用在 ![\Bbb A^2](https://www.zhihu.com/equation?tex=%5CBbb+A%5E2) 保持曲线 ![C:xy^q-yx^q=1](https://www.zhihu.com/equation?tex=C%3Axy%5Eq-yx%5Eq%3D1) ，进而 ![H^1_c(C,\bar {\Bbb Q}_l)](https://www.zhihu.com/equation?tex=H%5E1_c%28C%2C%5Cbar+%7B%5CBbb+Q%7D_l%29)是![SL_2(\Bbb F_q)](https://www.zhihu.com/equation?tex=SL_2%28%5CBbb+F_q%29) 的一个表示。最后值得指出的是， ![GL_n(F)](https://www.zhihu.com/equation?tex=GL_n%28F%29) 的不可约表示与其共轭类可建立一一对应。

（更深入的讨论可见[FINITE GROUPS OF LIE TYPE AND THEIR REPRESENTATIONS](https://link.zhihu.com/?target=http%3A//www.math.rwth-aachen.de/~Gerhard.Hiss/Preprints/StAndrewsBath09.pdf)）

3. [Can you use Chevalley‒Warning to prove existence of a solution?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/242680/can-you-use-chevalley-warning-to-prove-existence-of-a-solution)

（Chevalley‒Warning theorem）

Let ![f_1, \ldots, f_r \in \mathbb F_q[x_1,\ldots,x_n]](https://www.zhihu.com/equation?tex=f_1%2C+%5Cldots%2C+f_r+%5Cin+%5Cmathbb+F_q%5Bx_1%2C%5Cldots%2Cx_n%5D) be polynomials of degrees ![d_i](https://www.zhihu.com/equation?tex=d_i) . If

![d=d_1+…+d_r<n](https://www.zhihu.com/equation?tex=d%3Dd_1%2B%E2%80%A6%2Bd_r%3Cn)

then the number of common zeroes of ![f_i](https://www.zhihu.com/equation?tex=f_i) is ![0 \mod p](https://www.zhihu.com/equation?tex=0+%5Cmod+p).

4. [Is there a proof of Warning's Second Theorem using p-adic cohomology?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/185830/is-there-a-proof-of-warnings-second-theorem-using-p-adic-cohomology)

（Warning's Second Theorem）

If  ![d=d_1+…+d_r<n](https://www.zhihu.com/equation?tex=d%3Dd_1%2B%E2%80%A6%2Bd_r%3Cn)  and ![f_i](https://www.zhihu.com/equation?tex=f_i)  have common zero, then the number of common zeroes of ![f_i](https://www.zhihu.com/equation?tex=f_i)  is at least ![q^{n-d}](https://www.zhihu.com/equation?tex=q%5E%7Bn-d%7D)

对于有限域 ![\Bbb F_q](https://www.zhihu.com/equation?tex=%5CBbb+F_q) 上的射影簇X，Grothendieck有著名的trace formula：

![\#X(\Bbb F_{q})= \sum_{i=0}^{2 dim X} (-1)^i Tr (Frob_{q}|_{H^i_{et}(X_{\bar {\Bbb F_q}},\bar {\mathbb Q}_l)})](https://www.zhihu.com/equation?tex=%5C%23X%28%5CBbb+F_%7Bq%7D%29%3D+%5Csum_%7Bi%3D0%7D%5E%7B2+dim+X%7D+%28-1%29%5Ei+Tr+%28Frob_%7Bq%7D%7C_%7BH%5Ei_%7Bet%7D%28X_%7B%5Cbar+%7B%5CBbb+F_q%7D%7D%2C%5Cbar+%7B%5Cmathbb+Q%7D_l%29%7D%29)

不使用etale cohomology的语言，仅用层的上同调也可以得到一些信息，这就是Fulton trace

formula( 参见[fultontrace.pdf](https://link.zhihu.com/?target=http%3A//math.stanford.edu/~dlitt/exposnotes/fultontrace.pdf) ）：

![\# X(\mathbb{F}_q)=\sum_{i=0}^{\text{dim}(X)} (-1)^i Tr({Frob_q}|_{ H^i(X, \mathcal{O}_X)})\bmod p.](https://www.zhihu.com/equation?tex=%5C%23+X%28%5Cmathbb%7BF%7D_q%29%3D%5Csum_%7Bi%3D0%7D%5E%7B%5Ctext%7Bdim%7D%28X%29%7D+%28-1%29%5Ei+Tr%28%7BFrob_q%7D%7C_%7B+H%5Ei%28X%2C+%5Cmathcal%7BO%7D_X%29%7D%29%5Cbmod+p.)

5.[Order of finite unitary group](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/34387/order-of-finite-unitary-group)

![U_n(\mathbb F_q) = \left\lbrace A \in GL_n(\mathbb F_{q^2}) | A A^{*} = I_{n} \right\rbrace](https://www.zhihu.com/equation?tex=U_n%28%5Cmathbb+F_q%29+%3D+%5Cleft%5Clbrace+A+%5Cin+GL_n%28%5Cmathbb+F_%7Bq%5E2%7D%29+%7C+A+A%5E%7B%2A%7D+%3D+I_%7Bn%7D+%5Cright%5Crbrace) 是有限域上的酉群，其中 ![*: A=(a_{ij}) \mapsto A^*= (a_{ji}^q)](https://www.zhihu.com/equation?tex=%2A%3A+A%3D%28a_%7Bij%7D%29+%5Cmapsto+A%5E%2A%3D+%28a_%7Bji%7D%5Eq%29) 。则 ![\#U_n(\Bbb F_q)=q^{(n^2-n)/2} \prod\limits_{i=1}^{n} (q^i - (-1)^i)](https://www.zhihu.com/equation?tex=%5C%23U_n%28%5CBbb+F_q%29%3Dq%5E%7B%28n%5E2-n%29%2F2%7D+%5Cprod%5Climits_%7Bi%3D1%7D%5E%7Bn%7D+%28q%5Ei+-+%28-1%29%5Ei%29)

证明并不如同 ![GL_n](https://www.zhihu.com/equation?tex=GL_n) 一样简单（这可类比求李群 ![U(n)](https://www.zhihu.com/equation?tex=U%28n%29) 的维数，一个方法是线性化改为求李代数的维数，但这里是离散的问题），可见 [maths.qmul.ac.uk/~pjc/class_gps](https://link.zhihu.com/?target=http%3A//www.maths.qmul.ac.uk/~pjc/class_gps/)

6.[What is an example of a smooth variety over a finite field F_p which does not lift to Z_p?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/423/what-is-an-example-of-a-smooth-variety-over-a-finite-field-f-p-which-does-not-li)

BMY inequality

7.[Maximal order of elements in SL(n,q)](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/272440/maximal-order-of-elements-in-sln-q)

上次已提到![GL_n(\Bbb F_q)](https://www.zhihu.com/equation?tex=GL_n%28%5CBbb+F_q%29) 中元的最大阶是 ![q^n-1](https://www.zhihu.com/equation?tex=q%5En-1) ，而 ![SL_n(\Bbb F_q)](https://www.zhihu.com/equation?tex=SL_n%28%5CBbb+F_q%29) (n>2)为 ![\frac{q^n-1}{q-1}](https://www.zhihu.com/equation?tex=%5Cfrac%7Bq%5En-1%7D%7Bq-1%7D) ，但n=2时情况有些不同。

8.[Does there exist a polar decomposition of matrices over finite fields?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/129464/does-there-exist-a-polar-decomposition-of-matrices-over-finite-fields)

9.[How to recognise that the polynomial method might work](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/43317/how-to-recognise-that-the-polynomial-method-might-work)

用多项式方法解决组合问题，一个例子是![\Bbb R^n](https://www.zhihu.com/equation?tex=%5CBbb+R%5En) 中一族两两夹角都相同的直线至多 ![n(n-1)/2](https://www.zhihu.com/equation?tex=n%28n-1%29%2F2) 个。

9.25


