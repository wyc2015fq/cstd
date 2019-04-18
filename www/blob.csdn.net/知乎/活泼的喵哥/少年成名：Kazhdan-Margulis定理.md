# 少年成名：Kazhdan-Margulis定理 - 知乎
# 



Kazhdan和Margulis都是大名鼎鼎的数学大师。他们研究的领域有很远的距离，但是他们少年时期在一起上学，还一起合作证明了当时很著名的Selberg猜想。两人因为这个结果而一举成名，并在之后成为各自领域的奠基性人物。

这个结果现在被称为Kazhdan-Margulis定理，它刻画了半单李群的离散子群的格子群的一个很重要的结构上的特点，暗示了半单子群格子群的刚性，为之后人们研究半单李群的离散子群的结构指明了方向。这个工作发表于1968年，在1972年Raghunathan在他的著作"Discrete subgroups of Lie groups"中用了一章的篇幅详细介绍了这个定理，足见这个工作的重要性。

今天我们来简单介绍一下这个定理以及它的证明。

我们考虑一个半单李群 ![G](https://www.zhihu.com/equation?tex=G) 和它的一个格子群 ![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma) ，使得齐性空间 ![X = G/\Gamma](https://www.zhihu.com/equation?tex=X+%3D+G%2F%5CGamma) 不是紧致的。 ![X](https://www.zhihu.com/equation?tex=X) 在无穷远点附近的形状对我们理解 ![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma) 的结构至关重要。首先我们简单回顾一下在这方面的一些简单结论。

定理：首先，一个 ![X](https://www.zhihu.com/equation?tex=X) 点序列 ![\{x_i = g_i \Gamma\}](https://www.zhihu.com/equation?tex=%5C%7Bx_i+%3D+g_i+%5CGamma%5C%7D) 趋于无穷远点等价于 ![x_i](https://www.zhihu.com/equation?tex=x_i) 的稳定子群 ![\mathrm{Stab}(x_i)= g_i \Gamma g_i^{-1}](https://www.zhihu.com/equation?tex=%5Cmathrm%7BStab%7D%28x_i%29%3D+g_i+%5CGamma+g_i%5E%7B-1%7D) 中包含离单位元 ![e](https://www.zhihu.com/equation?tex=e) 很近(但不是 ![e](https://www.zhihu.com/equation?tex=e) )的元素：存在 ![\gamma_i \in \Gamma \setminus\{e\}](https://www.zhihu.com/equation?tex=%5Cgamma_i+%5Cin+%5CGamma+%5Csetminus%5C%7Be%5C%7D) ，使得当 ![i \to \infty](https://www.zhihu.com/equation?tex=i+%5Cto+%5Cinfty) 时， ![g_i \gamma_i g_i^{-1} \to e](https://www.zhihu.com/equation?tex=g_i+%5Cgamma_i+g_i%5E%7B-1%7D+%5Cto+e) 。这个定理用定义就可以证明（主要用 ![X](https://www.zhihu.com/equation?tex=X) 的体积为有限这个条件）。

其次，Zassenhaus证明了下面这个简单但是很有用的定理：存在一个 ![G](https://www.zhihu.com/equation?tex=G) 中 ![e](https://www.zhihu.com/equation?tex=e) 的领域 ![W](https://www.zhihu.com/equation?tex=W) ，满足下面的性质：对于任何一个 ![G](https://www.zhihu.com/equation?tex=G) 的离散子群 ![\Lambda](https://www.zhihu.com/equation?tex=%5CLambda) , ![\Lambda \cap W](https://www.zhihu.com/equation?tex=%5CLambda+%5Ccap+W) 生成的子群（我们记作 ![\Lambda_W](https://www.zhihu.com/equation?tex=%5CLambda_W) ）是一个幂零子群（nilpotent）。我们称这个领域为Zassenhaus领域。

一个群 ![H](https://www.zhihu.com/equation?tex=H) 是幂零的，如果 ![C^k H =\{e\}](https://www.zhihu.com/equation?tex=C%5Ek+H+%3D%5C%7Be%5C%7D) 对于某个 ![k](https://www.zhihu.com/equation?tex=k) 成立。这里 ![C^k H](https://www.zhihu.com/equation?tex=C%5Ek+H) 定义如下：

![C^1 H = [H, H]](https://www.zhihu.com/equation?tex=C%5E1+H+%3D+%5BH%2C+H%5D) , ![C^k H = [H, C^{k-1}H]](https://www.zhihu.com/equation?tex=C%5Ek+H+%3D+%5BH%2C+C%5E%7Bk-1%7DH%5D) 。其中的方括号表示交换子 ![[a,b] = a b a^{-1} b^{-1}](https://www.zhihu.com/equation?tex=%5Ba%2Cb%5D+%3D+a+b+a%5E%7B-1%7D+b%5E%7B-1%7D) 生成的子群。

这个证明也很简单，主要用到的是对于 ![e](https://www.zhihu.com/equation?tex=e) 的足够小的领域 ![U](https://www.zhihu.com/equation?tex=U) ，交换子组成的集合

![[U,U] = \{[a,b], a, b \in U\}](https://www.zhihu.com/equation?tex=%5BU%2CU%5D+%3D+%5C%7B%5Ba%2Cb%5D%2C+a%2C+b+%5Cin+U%5C%7D) 会收缩到一个更小的领域。于是当我们对 ![\Lambda_W](https://www.zhihu.com/equation?tex=%5CLambda_W) 的生成元 ![\Lambda \cap W](https://www.zhihu.com/equation?tex=%5CLambda+%5Ccap+W) 做交换子操作时，这些元素会收缩到一个更小的领域中。由于 ![\Lambda](https://www.zhihu.com/equation?tex=%5CLambda) 是离散的，这个操作会在有限步之后收缩成 ![\{e\}](https://www.zhihu.com/equation?tex=%5C%7Be%5C%7D) 。这个结论就能推出 ![\Lambda_W](https://www.zhihu.com/equation?tex=%5CLambda_W) 是一个幂零群。

由于幂零和幂幺在某种意义下很接近，Selberg提出了下面这个猜想：

一个 ![X](https://www.zhihu.com/equation?tex=X) 中的点列 ![\{x_i = g_i \Gamma\}](https://www.zhihu.com/equation?tex=%5C%7Bx_i+%3D+g_i+%5CGamma%5C%7D) 趋于无穷远点，则存在幂幺元素序列 ![\gamma_i \in \Gamma \setminus \{e\}](https://www.zhihu.com/equation?tex=%5Cgamma_i+%5Cin+%5CGamma+%5Csetminus+%5C%7Be%5C%7D) 使得当 ![i \to \infty](https://www.zhihu.com/equation?tex=i+%5Cto+%5Cinfty) 时 ![g_i \gamma_i g_i^{-1} \to \infty](https://www.zhihu.com/equation?tex=g_i+%5Cgamma_i+g_i%5E%7B-1%7D+%5Cto+%5Cinfty) 。也就是说： ![x_i](https://www.zhihu.com/equation?tex=x_i) 的稳定子群 ![g_i \Gamma g_i^{-1}](https://www.zhihu.com/equation?tex=g_i+%5CGamma+g_i%5E%7B-1%7D) 中存在很小的幂幺元素。

这个猜想由Kazhdan和Margulis合作解决，被称为Kazhdan-Margulis定理。

Kazhdan-Margulis定理（定理一）： 假设 ![G](https://www.zhihu.com/equation?tex=G) 为一个线性半单李群，不包含至今因子(即 ![G](https://www.zhihu.com/equation?tex=G) 的每一个正规单李子群都是非紧的)， ![\Gamma \subset G](https://www.zhihu.com/equation?tex=%5CGamma+%5Csubset+G) 为 ![G](https://www.zhihu.com/equation?tex=G) 的一个格子群。则存在一个有限子集 ![S_1 \subset \Gamma \setminus\{e\}](https://www.zhihu.com/equation?tex=S_1+%5Csubset+%5CGamma+%5Csetminus%5C%7Be%5C%7D) ，由幂幺元素组成，据有下面的性质：对于 ![e](https://www.zhihu.com/equation?tex=e) 的任意一个领域 ![W](https://www.zhihu.com/equation?tex=W) ，存在另一个领域 ![W_0 \subset W](https://www.zhihu.com/equation?tex=W_0+%5Csubset+W) 使得当 ![x = g \Gamma](https://www.zhihu.com/equation?tex=x+%3D+g+%5CGamma) 足够远，即 ![g\Gamma g^{-1} \cap W_0 \neq \{e\}](https://www.zhihu.com/equation?tex=g%5CGamma+g%5E%7B-1%7D+%5Ccap+W_0+%5Cneq+%5C%7Be%5C%7D) 时， ![\Gamma \cap g^{-1} W g ](https://www.zhihu.com/equation?tex=%5CGamma+%5Ccap+g%5E%7B-1%7D+W+g+) 包含一个幂幺元素 ![\gamma](https://www.zhihu.com/equation?tex=%5Cgamma) 使得![\gamma_1 \gamma \gamma_1^{-1} \in S_1](https://www.zhihu.com/equation?tex=%5Cgamma_1+%5Cgamma+%5Cgamma_1%5E%7B-1%7D+%5Cin+S_1) 对于某一个 ![\gamma_1 \in \Gamma](https://www.zhihu.com/equation?tex=%5Cgamma_1+%5Cin+%5CGamma) 成立。

这个定理很容易推出Selberg猜想。

这个定理的一个副产品是下面这个定理：

定理二（Kazhdan-Margulis）： ![G](https://www.zhihu.com/equation?tex=G) 如上所诉，则存在一个 ![e](https://www.zhihu.com/equation?tex=e) 的一个领域 ![W \subset G](https://www.zhihu.com/equation?tex=W+%5Csubset+G) ，使得对于任意一个 ![G](https://www.zhihu.com/equation?tex=G) 的离散子群 ![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma) ，存在 ![g \in G](https://www.zhihu.com/equation?tex=g+%5Cin+G) 使得 ![g \Gamma g^{-1} \cap W = \{e\}](https://www.zhihu.com/equation?tex=g+%5CGamma+g%5E%7B-1%7D+%5Ccap+W+%3D+%5C%7Be%5C%7D) 。

这个定理很容易推出存在一个只与 ![G](https://www.zhihu.com/equation?tex=G) 有关的常数 ![\mu>0](https://www.zhihu.com/equation?tex=%5Cmu%3E0) ，使得对于任意 ![G](https://www.zhihu.com/equation?tex=G) 的任意格子集 ![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma) ， ![G/\Gamma](https://www.zhihu.com/equation?tex=G%2F%5CGamma) 的体积至少为 ![\mu](https://www.zhihu.com/equation?tex=%5Cmu) 。这个现象和交换李群，比如 ![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En) ，有本质的区别。在 ![\mathbb{R}^n](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En) 中，格子群 ![\Lambda](https://www.zhihu.com/equation?tex=%5CLambda) 可以非常密，使得商空间 ![\mathbb{R}^n/\Lambda](https://www.zhihu.com/equation?tex=%5Cmathbb%7BR%7D%5En%2F%5CLambda) 的体积要多小有多小。而在半单李群中，商空间的体积却有一个固定的下界。这揭示了半单李群的格子群具有某种刚性。这个结论成为了之后很多对于半单李群的格子群的刚性研究的出发点和原始动机。

下面我们简单解释一下定理二的证明思路。

首先我们可以证明对于 ![e](https://www.zhihu.com/equation?tex=e) 的足够小的领域 ![W](https://www.zhihu.com/equation?tex=W) ，如果 ![g\Gamma g^{-1} \cap W \neq \{e\}](https://www.zhihu.com/equation?tex=g%5CGamma+g%5E%7B-1%7D+%5Ccap+W+%5Cneq+%5C%7Be%5C%7D) ，则存在一个 ![h \in G](https://www.zhihu.com/equation?tex=h+%5Cin+G) 使得对于 ![g\Gamma g^{-1} \cap W \setminus\{e\}](https://www.zhihu.com/equation?tex=g%5CGamma+g%5E%7B-1%7D+%5Ccap+W+%5Csetminus%5C%7Be%5C%7D) 中的任何元素 ![w](https://www.zhihu.com/equation?tex=w) , ![h w h^{-1}](https://www.zhihu.com/equation?tex=h+w+h%5E%7B-1%7D) 与 ![e](https://www.zhihu.com/equation?tex=e) 的距离比 ![w](https://www.zhihu.com/equation?tex=w) 与 ![e](https://www.zhihu.com/equation?tex=e) 的距离大一个固定的倍数。也就是在 ![h](https://www.zhihu.com/equation?tex=h) 的共轭作用下，

中的每个元素都会以一定的速度远离 ![e](https://www.zhihu.com/equation?tex=e) 。这里的 ![W](https://www.zhihu.com/equation?tex=W) 我们可以取做Zassenhauss领域。更进一步， ![h](https://www.zhihu.com/equation?tex=h) 可以在一个固定的紧集 ![E \subset G](https://www.zhihu.com/equation?tex=E+%5Csubset+G) 中取值。

证明要用到半单李群的伴随表示的特征空间分解：

![\mathfrak{g} = \bigoplus_{\lambda} \mathfrak{g}^{\lambda}](https://www.zhihu.com/equation?tex=%5Cmathfrak%7Bg%7D+%3D+%5Cbigoplus_%7B%5Clambda%7D+%5Cmathfrak%7Bg%7D%5E%7B%5Clambda%7D) 。在 ![e](https://www.zhihu.com/equation?tex=e) 附近，每个元素 ![w\in W](https://www.zhihu.com/equation?tex=w%5Cin+W) 都可以写成 ![\mathrm{exp}(X)](https://www.zhihu.com/equation?tex=%5Cmathrm%7Bexp%7D%28X%29) ， ![X \in \mathfrak{g}](https://www.zhihu.com/equation?tex=X+%5Cin+%5Cmathfrak%7Bg%7D) 是一个很小的向量。所以我们可以把李群中的计算转化为李代数中的计算。

对于指定的Cartan李代数 ![\mathfrak{a}](https://www.zhihu.com/equation?tex=%5Cmathfrak%7Ba%7D) 以及对应的对角李群 ![A](https://www.zhihu.com/equation?tex=A) ，对于 ![a = \exp(H) \in A](https://www.zhihu.com/equation?tex=a+%3D+%5Cexp%28H%29+%5Cin+A) ， ![\|\mathrm{Ad}_a w\| \geq (1+c) \|w\|](https://www.zhihu.com/equation?tex=%5C%7C%5Cmathrm%7BAd%7D_a+w%5C%7C+%5Cgeq+%281%2Bc%29+%5C%7Cw%5C%7C) 成立的条件是 ![w](https://www.zhihu.com/equation?tex=w) 在空间 ![\bigoplus_{\lambda(H) > 0} \mathfrak{g}^{\lambda}](https://www.zhihu.com/equation?tex=%5Cbigoplus_%7B%5Clambda%28H%29+%3E+0%7D+%5Cmathfrak%7Bg%7D%5E%7B%5Clambda%7D) 的投影非零。这是一个很容易达到的条件。这一步的证明主要在这个思路的引导下做一些细致的讨论就可以得到。

接下来我们来证明定理二（当然也只是梗概）。我们记Zassenhauss领域为 ![W_1](https://www.zhihu.com/equation?tex=W_1) ,并取一个更小的领域 ![W](https://www.zhihu.com/equation?tex=W) 使得 ![h W h^{-1} \subset W_1](https://www.zhihu.com/equation?tex=h+W+h%5E%7B-1%7D+%5Csubset+W_1) 对所有 ![h \in E \cup E^{-1}](https://www.zhihu.com/equation?tex=h+%5Cin+E+%5Ccup+E%5E%7B-1%7D) 都成立。我们要证明这样选择的 ![W](https://www.zhihu.com/equation?tex=W) 满足我们想要的条件。如果不然，则对于任何的 ![g \in G](https://www.zhihu.com/equation?tex=g+%5Cin+G) ， ![g \Gamma g^{-1} \cap W \neq \{e\}](https://www.zhihu.com/equation?tex=g+%5CGamma+g%5E%7B-1%7D+%5Ccap+W+%5Cneq+%5C%7Be%5C%7D) 。我们记 ![a(g)](https://www.zhihu.com/equation?tex=a%28g%29) 为 ![g\Gamma g^{-1} \cap W](https://www.zhihu.com/equation?tex=g%5CGamma+g%5E%7B-1%7D+%5Ccap+W) 中离 ![e](https://www.zhihu.com/equation?tex=e) 最近的元素与 ![e](https://www.zhihu.com/equation?tex=e) 距离，并记 ![b](https://www.zhihu.com/equation?tex=b) 为所有 ![a(g)](https://www.zhihu.com/equation?tex=a%28g%29) 的上极限。我们选择 ![g \in G](https://www.zhihu.com/equation?tex=g+%5Cin+G) 使得 ![a(g) ](https://www.zhihu.com/equation?tex=a%28g%29+) 与 ![b](https://www.zhihu.com/equation?tex=b) 足够接近（几乎相等）。我们选择 ![h \in E](https://www.zhihu.com/equation?tex=h+%5Cin+E) 使得对于任意的 ![w \in g\Gamma g^{-1} \cap W_1](https://www.zhihu.com/equation?tex=w+%5Cin+g%5CGamma+g%5E%7B-1%7D+%5Ccap+W_1) ,

![h w h^{-1}](https://www.zhihu.com/equation?tex=h+w+h%5E%7B-1%7D) 都会以一定的速度远离 ![e](https://www.zhihu.com/equation?tex=e) 。我们可以使得这个原理的速度超过 ![a(g)](https://www.zhihu.com/equation?tex=a%28g%29) 与 ![b](https://www.zhihu.com/equation?tex=b) 之间的距离。再加上我们对 ![W](https://www.zhihu.com/equation?tex=W) 的选择， ![W_1](https://www.zhihu.com/equation?tex=W_1) 之外的元素通过 ![h](https://www.zhihu.com/equation?tex=h) 的共轭根本进不了 ![W](https://www.zhihu.com/equation?tex=W) ，从而得到这个上极限其实比 ![b](https://www.zhihu.com/equation?tex=b) 要大，这就是一个矛盾。于是定理二得证。

定理一的证明也基于类似的想法，即通过 ![E](https://www.zhihu.com/equation?tex=E) 中的元素的共轭作用使得离散子群中的小元素都以一定的速度远离 ![e](https://www.zhihu.com/equation?tex=e) 。我们考虑 ![K =\{ x = g \Gamma : g\Gamma g^{-1} \cap W = \{e\}\}](https://www.zhihu.com/equation?tex=K+%3D%5C%7B+x+%3D+g+%5CGamma+%3A+g%5CGamma+g%5E%7B-1%7D+%5Ccap+W+%3D+%5C%7Be%5C%7D%5C%7D) 。 ![K](https://www.zhihu.com/equation?tex=K) 是 ![X](https://www.zhihu.com/equation?tex=X) 的一个紧集，于是存在 ![G](https://www.zhihu.com/equation?tex=G) 中的一个紧集 ![Z](https://www.zhihu.com/equation?tex=Z) 使得 ![K \subset Z\Gamma](https://www.zhihu.com/equation?tex=K+%5Csubset+Z%5CGamma) 。我们考虑下面的有限集合：

![S = \{\gamma \in \Gamma: Z\gamma \cap EWE^{-1}Z \neq \emptyset\}](https://www.zhihu.com/equation?tex=S+%3D+%5C%7B%5Cgamma+%5Cin+%5CGamma%3A+Z%5Cgamma+%5Ccap+EWE%5E%7B-1%7DZ+%5Cneq+%5Cemptyset%5C%7D) 。我们记其中的幂幺元素组成的集合为 ![S_1](https://www.zhihu.com/equation?tex=S_1) ，其中的非幂幺元素组成的集合为 ![S_2](https://www.zhihu.com/equation?tex=S_2) 。 ![S_2](https://www.zhihu.com/equation?tex=S_2) 中的元素在共轭作用下的轨道的闭包不包含 ![e](https://www.zhihu.com/equation?tex=e) ，于是我们可以假设这些共轭轨道都不在 ![W](https://www.zhihu.com/equation?tex=W) 中。接下来，我们从 ![g\Gamma g^{-1}\cap W](https://www.zhihu.com/equation?tex=g%5CGamma+g%5E%7B-1%7D%5Ccap+W) 出发，连续的应用 ![E](https://www.zhihu.com/equation?tex=E) 中的元素 ![h_0, h_1, \dots, h_k](https://www.zhihu.com/equation?tex=h_0%2C+h_1%2C+%5Cdots%2C+h_k) 进行共轭作用，每一步都把 ![W_1](https://www.zhihu.com/equation?tex=W_1) 中的所有元素以一定的速度往外推。假设我们在第 ![k](https://www.zhihu.com/equation?tex=k) 步刚好把所有的元素都推出 ![W](https://www.zhihu.com/equation?tex=W) 。这样我们得到两个结论：

（1）： ![h_k \cdots h_1 g \Gamma (h_k \cdots h_1 g)^{-1} \cap W = \{e\}](https://www.zhihu.com/equation?tex=h_k+%5Ccdots+h_1+g+%5CGamma+%28h_k+%5Ccdots+h_1+g%29%5E%7B-1%7D+%5Ccap+W+%3D+%5C%7Be%5C%7D)

（2）： ![h_{k-1} \cdots h_1 g \Gamma (h_{k-1}\cdots h_1 g)^{-1}\cap W \neq \{e\}](https://www.zhihu.com/equation?tex=h_%7Bk-1%7D+%5Ccdots+h_1+g+%5CGamma+%28h_%7Bk-1%7D%5Ccdots+h_1+g%29%5E%7B-1%7D%5Ccap+W+%5Cneq+%5C%7Be%5C%7D)

让我们记 ![g_1 = h_k \cdots h_1 g](https://www.zhihu.com/equation?tex=g_1+%3D+h_k+%5Ccdots+h_1+g) 。则根据第一个结论，我们有 ![g_1 \in Z\Gamma](https://www.zhihu.com/equation?tex=g_1+%5Cin+Z%5CGamma)，记存在 ![\gamma \in \Gamma](https://www.zhihu.com/equation?tex=%5Cgamma+%5Cin+%5CGamma) ，使得 ![g_1 \gamma \in Z](https://www.zhihu.com/equation?tex=g_1+%5Cgamma+%5Cin+Z) 。根据第二个结论，我们有存在 ![\gamma_1 \in \Gamma \setminus\{e\}](https://www.zhihu.com/equation?tex=%5Cgamma_1+%5Cin+%5CGamma+%5Csetminus%5C%7Be%5C%7D) 使得 ![h_{k-1}\cdots h_1 g \gamma_1 (h_{k-1}\cdots h_1)^{-1} = h_k^{-1}g_1 \gamma_1 g_1^{-1} h_k \in W](https://www.zhihu.com/equation?tex=h_%7Bk-1%7D%5Ccdots+h_1+g+%5Cgamma_1+%28h_%7Bk-1%7D%5Ccdots+h_1%29%5E%7B-1%7D+%3D+h_k%5E%7B-1%7Dg_1+%5Cgamma_1+g_1%5E%7B-1%7D+h_k+%5Cin+W) 。

于是 ![g_1 \gamma_1\gamma = h_k ( h_k^{-1} g_1 \gamma_1 g_1^{-1} h_k) h_k^{-1} g_1\gamma \in E W E^{-1} Z](https://www.zhihu.com/equation?tex=g_1+%5Cgamma_1%5Cgamma+%3D+h_k+%28+h_k%5E%7B-1%7D+g_1+%5Cgamma_1+g_1%5E%7B-1%7D+h_k%29+h_k%5E%7B-1%7D+g_1%5Cgamma+%5Cin+E+W+E%5E%7B-1%7D+Z) 。

于是我们得到 ![\gamma^{-1} \gamma_1 \gamma \in S](https://www.zhihu.com/equation?tex=%5Cgamma%5E%7B-1%7D+%5Cgamma_1+%5Cgamma+%5Cin+S) 。又因为 ![\gamma_1](https://www.zhihu.com/equation?tex=%5Cgamma_1) 的共轭与 ![W](https://www.zhihu.com/equation?tex=W) 有交集，我们断定 ![\gamma_1](https://www.zhihu.com/equation?tex=%5Cgamma_1) 为幂幺元素。这个 ![\gamma_1](https://www.zhihu.com/equation?tex=%5Cgamma_1) 就是我们想要的，从而定理得证。

这个证明没有用到任何高级的数学工具，证明的结论却非常强，证明也非常简单，巧妙。

借助于一些比较高级的工具，Raghunathan证明了下面这个更强的结论：

定理（Raghunathan）： ![G](https://www.zhihu.com/equation?tex=G) 和 ![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma) 如前所述。则存在一个 ![e](https://www.zhihu.com/equation?tex=e) 的领域 ![V](https://www.zhihu.com/equation?tex=V) 满足下面的性质： ![\gamma \in \Gamma](https://www.zhihu.com/equation?tex=%5Cgamma+%5Cin+%5CGamma) 是幂幺的当且仅当它有一个共轭在 ![V](https://www.zhihu.com/equation?tex=V) 中。

这个定理说的是：当 ![x = g\Gamma](https://www.zhihu.com/equation?tex=x+%3D+g%5CGamma) 比较远时，它的稳定子群 ![g\Gamma g^{-1}](https://www.zhihu.com/equation?tex=g%5CGamma+g%5E%7B-1%7D) 中比较小的元素都是幂幺的。

注：本文主要参考的是Raghunathan的专著：Discrete subgroups of Lie groups。




