# 局部类域论 4 Dimension shifting - 知乎
# 

这一篇note我们介绍局部类域论 cohomologcial approach 的 technical part，也就是*dimension shifting*。粗略地说，它提供了一个联系不同阶数Tate group ![H^{r}_{T}(G,M),H^{s}_{T}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%2CH%5E%7Bs%7D_%7BT%7D%28G%2CM%29) 的办法。我们分成三个部分：
- 介绍Dimension shifting的定义。然后我们先大致地分析一下用它可以做什么事情，需要哪些其他的东西来辅助它。
- 利用Dimension shifting来证明一个重要的正合列：Inf-Res exact sequence。确定一类Tate cohomology平凡的G-module(induced modules)。
- 计算有限循环群的Tate group。

在下一篇note里我们将证明Tate-Nakayama Theorem，基本就完成了同调代数的准备工作。之后我们只需要复习一下local field ![L](https://www.zhihu.com/equation?tex=L) 的算术信息，就可以使用前面这些同调代数的工具，开始local class field theory的证明了。

- **Introduction**

**Dimension shifting**：假设有G-module的正合列，

![0\rightarrow M\rightarrow J^{1}\rightarrow J^{2}\rightarrow...\rightarrow J^{s}\rightarrow N\rightarrow0,f_{i}:J^{i}\rightarrow J^{i+1}](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow+J%5E%7B1%7D%5Crightarrow+J%5E%7B2%7D%5Crightarrow...%5Crightarrow+J%5E%7Bs%7D%5Crightarrow+N%5Crightarrow0%2Cf_%7Bi%7D%3AJ%5E%7Bi%7D%5Crightarrow+J%5E%7Bi%2B1%7D)

（1）如果 ![H^{r}(G,J^{i})=0,r,s>0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CJ%5E%7Bi%7D%29%3D0%2Cr%2Cs%3E0)，那么 ![H^{r}(G,N)\simeq H^{r+s}(G,M),r\geq1](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CN%29%5Csimeq+H%5E%7Br%2Bs%7D%28G%2CM%29%2Cr%5Cgeq1) ；

（2）如果 ![H_{r}(G,J^{i})=0,r,s>0](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CJ%5E%7Bi%7D%29%3D0%2Cr%2Cs%3E0)，那么 ![H_{r}(G,M)\simeq H_{r+s}(G,N),r\geq1](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CM%29%5Csimeq+H_%7Br%2Bs%7D%28G%2CN%29%2Cr%5Cgeq1) ；

（3）如果 ![H^{r}_{T}(G,J^{i})=0,r,s>0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CJ%5E%7Bi%7D%29%3D0%2Cr%2Cs%3E0)，那么 ![H^{r}_{T}(G,N)\simeq H^{r+s}_{T}(G,M),r\in\mathbb{Z}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CN%29%5Csimeq+H%5E%7Br%2Bs%7D_%7BT%7D%28G%2CM%29%2Cr%5Cin%5Cmathbb%7BZ%7D) 。

证明非常简单，只需要把这个正合列拆成若干短正合列，比如对于（1）

![0\rightarrow N^{i-1}\rightarrow J^{i}\rightarrow N^{i}\rightarrow 0,N^{i}=\ker f_{i+1}={\rm im}f_{i},N^{0}=M,N^{s}=N](https://www.zhihu.com/equation?tex=0%5Crightarrow+N%5E%7Bi-1%7D%5Crightarrow+J%5E%7Bi%7D%5Crightarrow+N%5E%7Bi%7D%5Crightarrow+0%2CN%5E%7Bi%7D%3D%5Cker+f_%7Bi%2B1%7D%3D%7B%5Crm+im%7Df_%7Bi%7D%2CN%5E%7B0%7D%3DM%2CN%5E%7Bs%7D%3DN)

每个短正合列由derived functor诱导长正合列，并且提供同构： 

![H^{r}(G,N)\simeq H^{r+1}(G,N^{s-1}),H^{r+1}(G,N^{s-1})\simeq H^{r+2}(G,N^{s-2})...](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CN%29%5Csimeq+H%5E%7Br%2B1%7D%28G%2CN%5E%7Bs-1%7D%29%2CH%5E%7Br%2B1%7D%28G%2CN%5E%7Bs-1%7D%29%5Csimeq+H%5E%7Br%2B2%7D%28G%2CN%5E%7Bs-2%7D%29...)

于是得到 

![H^{r}(G,N)\simeq H^{r+1}(G,N^{s-1})\simeq H^{r+2}(G,N^{s-2})...\simeq H^{r+s}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CN%29%5Csimeq+H%5E%7Br%2B1%7D%28G%2CN%5E%7Bs-1%7D%29%5Csimeq+H%5E%7Br%2B2%7D%28G%2CN%5E%7Bs-2%7D%29...%5Csimeq+H%5E%7Br%2Bs%7D%28G%2CM%29)

**Application**：现在我们看利用它可能做到哪些事情，一个自然的问题就是：对于满足某种性质的G-module M，我们能否得到周期性：![H^{r}_{T}(G,M)\simeq H^{r+k}_{T}(G,M),r\in\mathbb{Z}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%5Csimeq+H%5E%7Br%2Bk%7D_%7BT%7D%28G%2CM%29%2Cr%5Cin%5Cmathbb%7BZ%7D) 。

根据dimension shifting，自然的想法是去构造一个正合列

![0\rightarrow M\rightarrow J^{1}\rightarrow J^{2}\rightarrow...\rightarrow J^{k}\rightarrow M\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow+J%5E%7B1%7D%5Crightarrow+J%5E%7B2%7D%5Crightarrow...%5Crightarrow+J%5E%7Bk%7D%5Crightarrow+M%5Crightarrow0)

![H^{r}_{T}(G,J^{i})=0,r,s>0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CJ%5E%7Bi%7D%29%3D0%2Cr%2Cs%3E0) 。

但在实际的操作中，我们可能需要间接地构造这个正合列。

**Attempt：**

这个自然的想法来自于简单的观察：即**正合列两头都是 ![M](https://www.zhihu.com/equation?tex=M) ，它可以看作 ![\mathbb{Z}\otimes_{\mathbb{Z}}M](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM)** 。于是我们可以尝试去构造abelian groups（ ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D) -module）的正合列

![0\rightarrow \mathbb{Z}\rightarrow G_{1}\rightarrow G_{2}\rightarrow...\rightarrow G_{k}\rightarrow \mathbb{Z}\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+%5Cmathbb%7BZ%7D%5Crightarrow+G_%7B1%7D%5Crightarrow+G_%7B2%7D%5Crightarrow...%5Crightarrow+G_%7Bk%7D%5Crightarrow+%5Cmathbb%7BZ%7D%5Crightarrow0)   （*）

去**还原出来G-module的正合列**，但是**这样做必须让很多东西满足flatness。**（See Wikipedia：[flat module](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Flat_module)）

回忆以下事实：

**Lemma 1. **假设 ![F](https://www.zhihu.com/equation?tex=F) 是一个flat ![A](https://www.zhihu.com/equation?tex=A)-module，并且有短正合列 ![0\rightarrow N\rightarrow M\rightarrow F\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+N%5Crightarrow+M%5Crightarrow+F%5Crightarrow+0) 。那么对于任意![A](https://www.zhihu.com/equation?tex=A)-module ![E](https://www.zhihu.com/equation?tex=E) ，我们有短正合列：

![0\rightarrow N\otimes_{A} E\rightarrow M\otimes_{A} E\rightarrow F\otimes_{A} E\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+N%5Cotimes_%7BA%7D+E%5Crightarrow+M%5Cotimes_%7BA%7D+E%5Crightarrow+F%5Cotimes_%7BA%7D+E%5Crightarrow+0)

比如在我们的例子里，把（*）拆成短正合列：![0\rightarrow N_{i-1}\rightarrow G_{i}\rightarrow N_{i}\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+N_%7Bi-1%7D%5Crightarrow+G_%7Bi%7D%5Crightarrow+N_%7Bi%7D%5Crightarrow+0) 。**假如我们让每个 ![N_{i}](https://www.zhihu.com/equation?tex=N_%7Bi%7D) 都是flat ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-module**，就得到短正合列：

![0\rightarrow N_{i-1}\otimes_{\mathbb{Z}}M\rightarrow G_{i}\otimes_{\mathbb{Z}}M\rightarrow N_{i}\otimes_{\mathbb{Z}}M\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+N_%7Bi-1%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+G_%7Bi%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+N_%7Bi%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+0)

于是拼成正合列：

![0\rightarrow M\rightarrow G_{1}\otimes_{\mathbb{Z}}M\rightarrow G_{2}\otimes_{\mathbb{Z}}M\rightarrow...\rightarrow G_{k}\otimes_{\mathbb{Z}}M\rightarrow M\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow+G_%7B1%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+G_%7B2%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow...%5Crightarrow+G_%7Bk%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+M%5Crightarrow0)

同时我们希望![H^{r}_{T}(G,G_{k}\otimes_{\mathbb{Z}}M)=0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CG_%7Bk%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%29%3D0)（**）。这个条件往往是**technical point**。

一个特殊的情形（我们在后面证明）是由Shapiro's lemma得到的：induced module的Tate cohomology为0。比如，如果这里的 ![G_{i}](https://www.zhihu.com/equation?tex=G_%7Bi%7D) 是 ![\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D) ，那么 ![\mathbb{Z}[G]\otimes_{\mathbb{Z}}M](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM) 是induced module，![H^{r}_{T}(G,G_{k}\otimes_{\mathbb{Z}}M)=0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CG_%7Bk%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%29%3D0) 。但实际情况中，我们可能在证明（**）时要分成三种情况 ![r\geq1;r=0,-1;r\leq-2](https://www.zhihu.com/equation?tex=r%5Cgeq1%3Br%3D0%2C-1%3Br%5Cleq-2) 。比如说，![r\leq -2](https://www.zhihu.com/equation?tex=r%5Cleq+-2) 时Tate group对应的是homology group，此时如果![G_{k}\otimes_{\mathbb{Z}}M](https://www.zhihu.com/equation?tex=G_%7Bk%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM) 是free G-module，那么自然是projective G-module， ![H^{r}_{T}(G,G_{k}\otimes_{\mathbb{Z}}M)=0,r\leq-2](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CG_%7Bk%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%29%3D0%2Cr%5Cleq-2)。而利用tensor product构造free module的办法有**extension of scalar**，我们后面也会用到。

**Lemma 2**. 假设 ![f:A\rightarrow B](https://www.zhihu.com/equation?tex=f%3AA%5Crightarrow+B) 是一个 ![A](https://www.zhihu.com/equation?tex=A)-algebra，![N](https://www.zhihu.com/equation?tex=N) 是一个 ![A](https://www.zhihu.com/equation?tex=A)-module，那么 ![B\otimes_{A}N](https://www.zhihu.com/equation?tex=B%5Cotimes_%7BA%7DN) 具有 ![B ](https://www.zhihu.com/equation?tex=B+)-module structure： ![b^{\prime}\cdot(b\otimes n)=b^{\prime}b\otimes n](https://www.zhihu.com/equation?tex=b%5E%7B%5Cprime%7D%5Ccdot%28b%5Cotimes+n%29%3Db%5E%7B%5Cprime%7Db%5Cotimes+n) 。特别地，如果 ![N](https://www.zhihu.com/equation?tex=N) 是一个free ![A](https://www.zhihu.com/equation?tex=A)-module，基为 ![\{e_{n}\}](https://www.zhihu.com/equation?tex=%5C%7Be_%7Bn%7D%5C%7D) ，那么 ![B\otimes_{A}N](https://www.zhihu.com/equation?tex=B%5Cotimes_%7BA%7DN) 是一个free ![B ](https://www.zhihu.com/equation?tex=B+)-module，基为 ![\{1\otimes e_{n}\}](https://www.zhihu.com/equation?tex=%5C%7B1%5Cotimes+e_%7Bn%7D%5C%7D) 。

**Summary**：根据我们的目的（ ![H^{r}_{T}(G,M)\simeq H^{r+k}_{T}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%5Csimeq+H%5E%7Br%2Bk%7D_%7BT%7D%28G%2CM%29) 或 ![H^{r}_{T}(G,M)\simeq H^{r+k}_{T}(G,N)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%5Csimeq+H%5E%7Br%2Bk%7D_%7BT%7D%28G%2CN%29) 或 ![H^{r}_{T}(G,M)=0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%3D0) 等等）去构造相应的正合列：

(1) 找到合适的 ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D) 的resolution，满足一定的flatness，做tensor product还原出我们需要的正合列：        

![0\rightarrow M\rightarrow J^{1}\rightarrow J^{2}\rightarrow...\rightarrow J^{k}\rightarrow M\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow+J%5E%7B1%7D%5Crightarrow+J%5E%7B2%7D%5Crightarrow...%5Crightarrow+J%5E%7Bk%7D%5Crightarrow+M%5Crightarrow0)

(2) 对于![H_{T}^{r}(G,J^{i})=0,r,s>0](https://www.zhihu.com/equation?tex=H_%7BT%7D%5E%7Br%7D%28G%2CJ%5E%7Bi%7D%29%3D0%2Cr%2Cs%3E0) ，当 ![r\leq-1](https://www.zhihu.com/equation?tex=r%5Cleq-1) 时，尝试构造free G-module ![J^{i}](https://www.zhihu.com/equation?tex=J%5E%7Bi%7D) 。

2.1 **Induced modules**

**Proposition 1. **M是一个induced G-module，G有限，那么 ![H^{r}_{T}(G,M)=0,r\in\mathbb{Z}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%3D0%2Cr%5Cin%5Cmathbb%7BZ%7D)。

（1）![r\geq 1](https://www.zhihu.com/equation?tex=r%5Cgeq+1) ：之前我们证明对于有限群G，induced G-module可以写成： ![M=\mathbb{Z}[G]\otimes_{\mathbb{Z}}X](https://www.zhihu.com/equation?tex=M%3D%5Cmathbb%7BZ%7D%5BG%5D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DX) ，X是一个abelian group。并且此时Tate group和上同调群一样。利用Shapiro's lemma，得到：

![H^{r}(G,\text{Ind}^{G}(X))=H^{r}(\{1\},X)=0](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2C%5Ctext%7BInd%7D%5E%7BG%7D%28X%29%29%3DH%5E%7Br%7D%28%5C%7B1%5C%7D%2CX%29%3D0)

（2） ![r=0](https://www.zhihu.com/equation?tex=r%3D0) ：此时 ![H^{0}_{T}(G,M)=M^{G}/\text{Nm}_{G}M](https://www.zhihu.com/equation?tex=H%5E%7B0%7D_%7BT%7D%28G%2CM%29%3DM%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7DM) 。M中任意一个元素都可以写成 ![m=\sum_{g\in G}g\otimes x_{g}](https://www.zhihu.com/equation?tex=m%3D%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+x_%7Bg%7D) ，如果 ![m\in M^{G}](https://www.zhihu.com/equation?tex=m%5Cin+M%5E%7BG%7D) ，那么对于任意 ![g^{\prime}\in G,](https://www.zhihu.com/equation?tex=g%5E%7B%5Cprime%7D%5Cin+G%2C) 有

![g^{\prime}\big(\sum_{g\in G}g\otimes x_{g}\big)=\sum_{g\in G}g\otimes x_{g}](https://www.zhihu.com/equation?tex=g%5E%7B%5Cprime%7D%5Cbig%28%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+x_%7Bg%7D%5Cbig%29%3D%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+x_%7Bg%7D)

![LHS=\sum_{g\in G}g^{\prime}g\otimes x_{g}=_{g^{\prime}g\mapsto h}\sum_{h\in G}h\otimes x_{g^{\prime -1}h}=\sum_{g\in G}g\otimes x_{g^{\prime -1}g}](https://www.zhihu.com/equation?tex=LHS%3D%5Csum_%7Bg%5Cin+G%7Dg%5E%7B%5Cprime%7Dg%5Cotimes+x_%7Bg%7D%3D_%7Bg%5E%7B%5Cprime%7Dg%5Cmapsto+h%7D%5Csum_%7Bh%5Cin+G%7Dh%5Cotimes+x_%7Bg%5E%7B%5Cprime+-1%7Dh%7D%3D%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+x_%7Bg%5E%7B%5Cprime+-1%7Dg%7D) 。 

于是得到

![\sum_{g\in G}g\otimes (x_{g^{\prime -1}g}-x_{g})=0,{\rm all}~g^{\prime}\in G](https://www.zhihu.com/equation?tex=%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+%28x_%7Bg%5E%7B%5Cprime+-1%7Dg%7D-x_%7Bg%7D%29%3D0%2C%7B%5Crm+all%7D~g%5E%7B%5Cprime%7D%5Cin+G)

所以 ![x_{g}=x_{g^{\prime -1}g},g\in G,g^{\prime}\in G](https://www.zhihu.com/equation?tex=x_%7Bg%7D%3Dx_%7Bg%5E%7B%5Cprime+-1%7Dg%7D%2Cg%5Cin+G%2Cg%5E%7B%5Cprime%7D%5Cin+G)，特别地，![x_{e}=x_{g^{\prime -1}},g^{\prime}\in G](https://www.zhihu.com/equation?tex=x_%7Be%7D%3Dx_%7Bg%5E%7B%5Cprime+-1%7D%7D%2Cg%5E%7B%5Cprime%7D%5Cin+G) 。于是，

![m=\sum_{g\in G}g\otimes x_{e}=\sum_{g\in G}g(e\otimes x_{e})=\text{Nm}_{G}(e\otimes x_{e}) ](https://www.zhihu.com/equation?tex=m%3D%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+x_%7Be%7D%3D%5Csum_%7Bg%5Cin+G%7Dg%28e%5Cotimes+x_%7Be%7D%29%3D%5Ctext%7BNm%7D_%7BG%7D%28e%5Cotimes+x_%7Be%7D%29+)

所以![M^{G}=\text{Nm}_{G}M,H^{0}_{T}(G,M)=0](https://www.zhihu.com/equation?tex=M%5E%7BG%7D%3D%5Ctext%7BNm%7D_%7BG%7DM%2CH%5E%7B0%7D_%7BT%7D%28G%2CM%29%3D0) 。

（3） ![r=-1](https://www.zhihu.com/equation?tex=r%3D-1) ：![H^{-1}_{T}(G,M)=\ker(\text{Nm}_{G})/I_{G}M](https://www.zhihu.com/equation?tex=H%5E%7B-1%7D_%7BT%7D%28G%2CM%29%3D%5Cker%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM)。若 ![m\in\ker(\text{Nm}_{G})](https://www.zhihu.com/equation?tex=m%5Cin%5Cker%28%5Ctext%7BNm%7D_%7BG%7D%29) ，那么

![\sum_{g^{\prime}\in G}g^{\prime}\big(\sum_{g\in G}g\otimes x_{g}\big)=0](https://www.zhihu.com/equation?tex=%5Csum_%7Bg%5E%7B%5Cprime%7D%5Cin+G%7Dg%5E%7B%5Cprime%7D%5Cbig%28%5Csum_%7Bg%5Cin+G%7Dg%5Cotimes+x_%7Bg%7D%5Cbig%29%3D0)

交换求和顺序（finite sum）得到：

![0=\sum_{g\in G}g^{\prime}\big(\sum_{g^{\prime}\in G}g\otimes x_{g}\big)=\sum_{g\in G}\big((\sum_{g^{\prime}\in G}g^{\prime}g)\otimes x_{g}\big)=\sum_{g\in G}\big((\sum_{g^{\prime}\in G}g^{\prime})\otimes x_{g}\big)\\=\big(\sum_{g^{\prime}\in G}g^{\prime}\big)\otimes\big(\sum_{g\in G}x_{g}\big)=\sum_{g^{\prime}\in G}g^{\prime}\otimes(\sum_{g\in G}x_{g})](https://www.zhihu.com/equation?tex=0%3D%5Csum_%7Bg%5Cin+G%7Dg%5E%7B%5Cprime%7D%5Cbig%28%5Csum_%7Bg%5E%7B%5Cprime%7D%5Cin+G%7Dg%5Cotimes+x_%7Bg%7D%5Cbig%29%3D%5Csum_%7Bg%5Cin+G%7D%5Cbig%28%28%5Csum_%7Bg%5E%7B%5Cprime%7D%5Cin+G%7Dg%5E%7B%5Cprime%7Dg%29%5Cotimes+x_%7Bg%7D%5Cbig%29%3D%5Csum_%7Bg%5Cin+G%7D%5Cbig%28%28%5Csum_%7Bg%5E%7B%5Cprime%7D%5Cin+G%7Dg%5E%7B%5Cprime%7D%29%5Cotimes+x_%7Bg%7D%5Cbig%29%5C%5C%3D%5Cbig%28%5Csum_%7Bg%5E%7B%5Cprime%7D%5Cin+G%7Dg%5E%7B%5Cprime%7D%5Cbig%29%5Cotimes%5Cbig%28%5Csum_%7Bg%5Cin+G%7Dx_%7Bg%7D%5Cbig%29%3D%5Csum_%7Bg%5E%7B%5Cprime%7D%5Cin+G%7Dg%5E%7B%5Cprime%7D%5Cotimes%28%5Csum_%7Bg%5Cin+G%7Dx_%7Bg%7D%29)

所以，

![\sum_{g\in G}x_{g}=0,m=\sum_{g\in G}(g-1)\otimes x_{g}\in I_{G}M](https://www.zhihu.com/equation?tex=%5Csum_%7Bg%5Cin+G%7Dx_%7Bg%7D%3D0%2Cm%3D%5Csum_%7Bg%5Cin+G%7D%28g-1%29%5Cotimes+x_%7Bg%7D%5Cin+I_%7BG%7DM)

（4） ![r\leq-2](https://www.zhihu.com/equation?tex=r%5Cleq-2) ：根据dimension shifting，我们希望构造正合列

![0\rightarrow M_{1}\rightarrow M_{0}\rightarrow M\rightarrow0,H^{r}_{T}(G,M_{1})=H^{r}_{T}(G,M_{0})=0,r\leq -2](https://www.zhihu.com/equation?tex=0%5Crightarrow+M_%7B1%7D%5Crightarrow+M_%7B0%7D%5Crightarrow+M%5Crightarrow0%2CH%5E%7Br%7D_%7BT%7D%28G%2CM_%7B1%7D%29%3DH%5E%7Br%7D_%7BT%7D%28G%2CM_%7B0%7D%29%3D0%2Cr%5Cleq+-2) 。

根据我们一开始的分析，我们**先构造一个abelian group的正合列，然后tensor回去，**注意到 ![M=\mathbb{Z}[G]\otimes_{\mathbb{Z}}X](https://www.zhihu.com/equation?tex=M%3D%5Cmathbb%7BZ%7D%5BG%5D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DX)， ![\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D) 是free ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-module ![\Rightarrow](https://www.zhihu.com/equation?tex=%5CRightarrow) flat ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-module，那么我们可以考虑构造一个和 ![X](https://www.zhihu.com/equation?tex=X) 相关的正合列，然后 ![\otimes_{\mathbb{Z}}\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cotimes_%7B%5Cmathbb%7BZ%7D%7D%5Cmathbb%7BZ%7D%5BG%5D) 。这里有一个自然的正合列，即利用结论：任意一个abelian group都可以写成某个free ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-module的quotient，![X=X_{0}/X_{1},X_{0}~{\rm is}~{\rm free}](https://www.zhihu.com/equation?tex=X%3DX_%7B0%7D%2FX_%7B1%7D%2CX_%7B0%7D~%7B%5Crm+is%7D~%7B%5Crm+free%7D)，并且 ![X_{1}](https://www.zhihu.com/equation?tex=X_%7B1%7D) 是 ![X_{0}](https://www.zhihu.com/equation?tex=X_%7B0%7D) 的子群，也是free的（**subgroup of a free abelian group is free**)。

![0\rightarrow X_{1}\rightarrow X_{0}\rightarrow X\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+X_%7B1%7D%5Crightarrow+X_%7B0%7D%5Crightarrow+X%5Crightarrow+0)

做tensor product ![\otimes_{\mathbb{Z}}\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cotimes_%7B%5Cmathbb%7BZ%7D%7D%5Cmathbb%7BZ%7D%5BG%5D) 得到：

![0\rightarrow X_{1}\otimes_{\mathbb{Z}}\mathbb{Z}[G]\rightarrow X_{0}\otimes_{\mathbb{Z}}\mathbb{Z}[G]\rightarrow M\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+X_%7B1%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7D%5Cmathbb%7BZ%7D%5BG%5D%5Crightarrow+X_%7B0%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7D%5Cmathbb%7BZ%7D%5BG%5D%5Crightarrow+M%5Crightarrow+0)

记![M_{1}=X_{1}\otimes_{\mathbb{Z}}\mathbb{Z}[G],M_{0}=X_{0}\otimes_{\mathbb{Z}}\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=M_%7B1%7D%3DX_%7B1%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7D%5Cmathbb%7BZ%7D%5BG%5D%2CM_%7B0%7D%3DX_%7B0%7D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7D%5Cmathbb%7BZ%7D%5BG%5D)。接着我们使用extension of scalar： ![X_{1},X_{0}](https://www.zhihu.com/equation?tex=X_%7B1%7D%2CX_%7B0%7D) 是free ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-module， ![\mathbb{Z}\rightarrow\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5Crightarrow%5Cmathbb%7BZ%7D%5BG%5D) 是 ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-algebra，那么 ![M_{1},M_{0}](https://www.zhihu.com/equation?tex=M_%7B1%7D%2CM_%7B0%7D) 是free ![\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D)-module，自然是free G-module和projective  G-module，于是

![H^{r}_{T}(G,M_{1})=H^{r}_{T}(G,M_{0})=0,r\leq -2](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM_%7B1%7D%29%3DH%5E%7Br%7D_%7BT%7D%28G%2CM_%7B0%7D%29%3D0%2Cr%5Cleq+-2)

由dimension shifting，![H^{r}_{T}(G,M)=0,r\leq-2](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%3D0%2Cr%5Cleq-2) 。

2.2  **Inf-Res exact sequence**

**Proposition 2.** 假设H是G的正规子群，M是一个G-module。r是一个正整数。如果 ![H^{i}(G,M)=0,0<i<r](https://www.zhihu.com/equation?tex=H%5E%7Bi%7D%28G%2CM%29%3D0%2C0%3Ci%3Cr) 。那么有正合列：

![0\rightarrow H^{r}(G/H,M^{H})\rightarrow^{\text{Inf}}H^{r}(G,M)\rightarrow^{{\rm Res}} H^{r}(H,M)](https://www.zhihu.com/equation?tex=0%5Crightarrow+H%5E%7Br%7D%28G%2FH%2CM%5E%7BH%7D%29%5Crightarrow%5E%7B%5Ctext%7BInf%7D%7DH%5E%7Br%7D%28G%2CM%29%5Crightarrow%5E%7B%7B%5Crm+Res%7D%7D+H%5E%7Br%7D%28H%2CM%29)

*Proof*. 我们大致叙述 ![r=1](https://www.zhihu.com/equation?tex=r%3D1) 的情形，重点放在 ![r>1](https://www.zhihu.com/equation?tex=r%3E1) 时怎么利用dimension shifting和induction。考虑：![C^{1}(G/H,M^{H})\rightarrow^{\text{Inf}}C^{1}(G,M)\rightarrow^{{\rm Res}} C^{1}(H,M)](https://www.zhihu.com/equation?tex=C%5E%7B1%7D%28G%2FH%2CM%5E%7BH%7D%29%5Crightarrow%5E%7B%5Ctext%7BInf%7D%7DC%5E%7B1%7D%28G%2CM%29%5Crightarrow%5E%7B%7B%5Crm+Res%7D%7D+C%5E%7B1%7D%28H%2CM%29) 。它由

![M^{H}\rightarrow M\rightarrow M](https://www.zhihu.com/equation?tex=M%5E%7BH%7D%5Crightarrow+M%5Crightarrow+M) ，以及相对应的 ![G\rightarrow G/H,H\rightarrow G](https://www.zhihu.com/equation?tex=G%5Crightarrow+G%2FH%2CH%5Crightarrow+G) 诱导。

任取 ![\varphi\in Z^{1}(G,M)](https://www.zhihu.com/equation?tex=%5Cvarphi%5Cin+Z%5E%7B1%7D%28G%2CM%29) ，使得 ![{\rm Res}\varphi\in B^{1}(H,M),\varphi(h)=hm_{0}-m_{0},m_{0}\in M](https://www.zhihu.com/equation?tex=%7B%5Crm+Res%7D%5Cvarphi%5Cin+B%5E%7B1%7D%28H%2CM%29%2C%5Cvarphi%28h%29%3Dhm_%7B0%7D-m_%7B0%7D%2Cm_%7B0%7D%5Cin+M)。我们希望找到 ![\phi\in Z^{1}(G/H,M^{H})](https://www.zhihu.com/equation?tex=%5Cphi%5Cin+Z%5E%7B1%7D%28G%2FH%2CM%5E%7BH%7D%29) 使得 ![{\rm Inf}\phi\equiv\varphi~{\rm mod}~B^{1}(G,M)](https://www.zhihu.com/equation?tex=%7B%5Crm+Inf%7D%5Cphi%5Cequiv%5Cvarphi~%7B%5Crm+mod%7D~B%5E%7B1%7D%28G%2CM%29) ：

首先定义 ![\psi\in B^{1}(G,M),\psi(g)=gm_{0}-m_{0}](https://www.zhihu.com/equation?tex=%5Cpsi%5Cin+B%5E%7B1%7D%28G%2CM%29%2C%5Cpsi%28g%29%3Dgm_%7B0%7D-m_%7B0%7D)以及 

![\phi\in C^{1}(G/H,M^{H}),\phi(gH)=\varphi^{*}(g)](https://www.zhihu.com/equation?tex=%5Cphi%5Cin+C%5E%7B1%7D%28G%2FH%2CM%5E%7BH%7D%29%2C%5Cphi%28gH%29%3D%5Cvarphi%5E%7B%2A%7D%28g%29)

其中，![\varphi^{*}(g)=(\varphi-\psi)(g),\varphi^{*}(h)=0,h\in H](https://www.zhihu.com/equation?tex=%5Cvarphi%5E%7B%2A%7D%28g%29%3D%28%5Cvarphi-%5Cpsi%29%28g%29%2C%5Cvarphi%5E%7B%2A%7D%28h%29%3D0%2Ch%5Cin+H) 。

(1) ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) is well-defined（ ![{\rm im}\phi\subseteq M^{H}](https://www.zhihu.com/equation?tex=%7B%5Crm+im%7D%5Cphi%5Csubseteq+M%5E%7BH%7D) ）：

![\forall h\in H,h\phi(gH)=h\varphi^{*}(g)=\varphi^{*}(hg)-\varphi^{*}(h)=\varphi^{*}(hg),{\rm since}~\varphi^{*}\in Z^{1}(G,M)](https://www.zhihu.com/equation?tex=%5Cforall+h%5Cin+H%2Ch%5Cphi%28gH%29%3Dh%5Cvarphi%5E%7B%2A%7D%28g%29%3D%5Cvarphi%5E%7B%2A%7D%28hg%29-%5Cvarphi%5E%7B%2A%7D%28h%29%3D%5Cvarphi%5E%7B%2A%7D%28hg%29%2C%7B%5Crm+since%7D~%5Cvarphi%5E%7B%2A%7D%5Cin+Z%5E%7B1%7D%28G%2CM%29)

由于H是G的正规子群，那么 

![\exists h_{1}\in H,hg=gh_{1},\varphi^{*}(hg)=\varphi^{*}(gh_{1})=g\varphi^{*}(h_{1})+\varphi^{*}(g)=\varphi^{*}(g)](https://www.zhihu.com/equation?tex=%5Cexists+h_%7B1%7D%5Cin+H%2Chg%3Dgh_%7B1%7D%2C%5Cvarphi%5E%7B%2A%7D%28hg%29%3D%5Cvarphi%5E%7B%2A%7D%28gh_%7B1%7D%29%3Dg%5Cvarphi%5E%7B%2A%7D%28h_%7B1%7D%29%2B%5Cvarphi%5E%7B%2A%7D%28g%29%3D%5Cvarphi%5E%7B%2A%7D%28g%29)

于是有：![h\phi(gH)=\varphi^{*}(g)=\phi(gH)](https://www.zhihu.com/equation?tex=h%5Cphi%28gH%29%3D%5Cvarphi%5E%7B%2A%7D%28g%29%3D%5Cphi%28gH%29) 。

(2) ![\phi\in Z^{1}(G/H,M^{H})](https://www.zhihu.com/equation?tex=%5Cphi%5Cin+Z%5E%7B1%7D%28G%2FH%2CM%5E%7BH%7D%29) ：

![\phi(g_{1}g_{2}H)=\varphi^{*}(g_{1}g_{2})=g_{1}\varphi^{*}(g_{2})+\varphi^{*}(g_{1})\\=g_{1}H\varphi^{*}(g_{2})+\varphi^{*}(g_{1})=g_{1}H\cdot\phi(g_{2}H)+\phi(g_{1}H)](https://www.zhihu.com/equation?tex=%5Cphi%28g_%7B1%7Dg_%7B2%7DH%29%3D%5Cvarphi%5E%7B%2A%7D%28g_%7B1%7Dg_%7B2%7D%29%3Dg_%7B1%7D%5Cvarphi%5E%7B%2A%7D%28g_%7B2%7D%29%2B%5Cvarphi%5E%7B%2A%7D%28g_%7B1%7D%29%5C%5C%3Dg_%7B1%7DH%5Cvarphi%5E%7B%2A%7D%28g_%7B2%7D%29%2B%5Cvarphi%5E%7B%2A%7D%28g_%7B1%7D%29%3Dg_%7B1%7DH%5Ccdot%5Cphi%28g_%7B2%7DH%29%2B%5Cphi%28g_%7B1%7DH%29)

(3） ![({\rm Inf}\phi)(g)=\phi(gH)=\varphi^{*}(g)=\varphi(g)-\psi(g)](https://www.zhihu.com/equation?tex=%28%7B%5Crm+Inf%7D%5Cphi%29%28g%29%3D%5Cphi%28gH%29%3D%5Cvarphi%5E%7B%2A%7D%28g%29%3D%5Cvarphi%28g%29-%5Cpsi%28g%29)，所以

![{\rm Inf}\phi\equiv\varphi~{\rm mod}~B^{1}(G,M)](https://www.zhihu.com/equation?tex=%7B%5Crm+Inf%7D%5Cphi%5Cequiv%5Cvarphi~%7B%5Crm+mod%7D~B%5E%7B1%7D%28G%2CM%29)

即 ![\ker({\rm Res})={\rm im}({\rm Inf})](https://www.zhihu.com/equation?tex=%5Cker%28%7B%5Crm+Res%7D%29%3D%7B%5Crm+im%7D%28%7B%5Crm+Inf%7D%29) 。 ![{\rm Inf}](https://www.zhihu.com/equation?tex=%7B%5Crm+Inf%7D) 为单射更容易证明(exercise)。

现在使用归纳和dimension shifting：

假设case r-1 成立，现考虑case r，假设 ![H^{i}(H,M)=0,0<i\leq r-1](https://www.zhihu.com/equation?tex=H%5E%7Bi%7D%28H%2CM%29%3D0%2C0%3Ci%5Cleq+r-1) 。

我们构造一个exact sequence：

忽略M的 G-module structure 得到abelian group X，构造出induced G-module ![ M_{*}={\rm Ind}^{G}(X)](https://www.zhihu.com/equation?tex=+M_%7B%2A%7D%3D%7B%5Crm+Ind%7D%5E%7BG%7D%28X%29) ，以及单射 ![M\rightarrow X,m\mapsto\varphi_{m};\varphi_{m}(g)=gm](https://www.zhihu.com/equation?tex=M%5Crightarrow+X%2Cm%5Cmapsto%5Cvarphi_%7Bm%7D%3B%5Cvarphi_%7Bm%7D%28g%29%3Dgm) 。令 ![M^{*}=M_{*}/M](https://www.zhihu.com/equation?tex=M%5E%7B%2A%7D%3DM_%7B%2A%7D%2FM) ，即得到exact sequence ![0\rightarrow M\rightarrow M_{*}\rightarrow M^{*}\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow+M_%7B%2A%7D%5Crightarrow+M%5E%7B%2A%7D%5Crightarrow0) 。

其中 ![H^{r}_{T}(G,M_{*})=0,r\in \mathbb{Z}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM_%7B%2A%7D%29%3D0%2Cr%5Cin+%5Cmathbb%7BZ%7D)，由dimension shifting：

![H^{i}(H,M_{*})=H^{i+1}(H,M),i>0](https://www.zhihu.com/equation?tex=H%5E%7Bi%7D%28H%2CM_%7B%2A%7D%29%3DH%5E%7Bi%2B1%7D%28H%2CM%29%2Ci%3E0)

由归纳假设![H^{i}(H,M_{*})=0,0<i\leq r-2](https://www.zhihu.com/equation?tex=H%5E%7Bi%7D%28H%2CM_%7B%2A%7D%29%3D0%2C0%3Ci%5Cleq+r-2) 以及case r-1，有exact

![0\rightarrow H^{r-1}(G/H,M^{*H})\rightarrow^{\text{Inf}}H^{r-1}(G,M^{*})\rightarrow^{{\rm Res}} H^{r-1}(H,M^{*})](https://www.zhihu.com/equation?tex=0%5Crightarrow+H%5E%7Br-1%7D%28G%2FH%2CM%5E%7B%2AH%7D%29%5Crightarrow%5E%7B%5Ctext%7BInf%7D%7DH%5E%7Br-1%7D%28G%2CM%5E%7B%2A%7D%29%5Crightarrow%5E%7B%7B%5Crm+Res%7D%7D+H%5E%7Br-1%7D%28H%2CM%5E%7B%2A%7D%29)

然而 ![H^{r-1}(G/H,M^{*H})=H^{r}(G/H,M^{H}),...](https://www.zhihu.com/equation?tex=H%5E%7Br-1%7D%28G%2FH%2CM%5E%7B%2AH%7D%29%3DH%5E%7Br%7D%28G%2FH%2CM%5E%7BH%7D%29%2C...)

所以得到exact

![0\rightarrow H^{r}(G/H,M^{H})\rightarrow^{\text{Inf}}H^{r}(G,M)\rightarrow^{{\rm Res}} H^{r}(H,M)](https://www.zhihu.com/equation?tex=0%5Crightarrow+H%5E%7Br%7D%28G%2FH%2CM%5E%7BH%7D%29%5Crightarrow%5E%7B%5Ctext%7BInf%7D%7DH%5E%7Br%7D%28G%2CM%29%5Crightarrow%5E%7B%7B%5Crm+Res%7D%7D+H%5E%7Br%7D%28H%2CM%29)

3. **有限循环群的Tate group **

**Proposition 3**. 设G是n阶有限循环群，![G=\langle\sigma\rangle](https://www.zhihu.com/equation?tex=G%3D%5Clangle%5Csigma%5Crangle) 。那么有同构：

![H^{r}_{T}(G,M)\simeq H^{r+2}_{T}(G,M),r\in\mathbb{Z}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%5Csimeq+H%5E%7Br%2B2%7D_%7BT%7D%28G%2CM%29%2Cr%5Cin%5Cmathbb%7BZ%7D)

Key observation 在于

**（1）**当G是**有限循环群**时，有正合列：
![](https://pic1.zhimg.com/v2-57391df1e542f8a519035a1f23149a50_b.jpg)
注意到 ![\sigma^{i}\mapsto 1](https://www.zhihu.com/equation?tex=%5Csigma%5E%7Bi%7D%5Cmapsto+1) 即augmentation map，ker为 ![I_{G}](https://www.zhihu.com/equation?tex=I_%7BG%7D)。**当G是循环群时**， 

![\ker (\sigma^{i}\mapsto1)=I_{G}=\{~\sum_{i=1}^{n}n_{i}(\sigma^{i}-1)~|~n_{i}\in\mathbb{Z}~\}](https://www.zhihu.com/equation?tex=%5Cker+%28%5Csigma%5E%7Bi%7D%5Cmapsto1%29%3DI_%7BG%7D%3D%5C%7B~%5Csum_%7Bi%3D1%7D%5E%7Bn%7Dn_%7Bi%7D%28%5Csigma%5E%7Bi%7D-1%29~%7C~n_%7Bi%7D%5Cin%5Cmathbb%7BZ%7D~%5C%7D)

于是，![(\sigma^{i}-1)=(\sigma-1)(1+\sigma+\sigma^{2}+...+\sigma^{i-1})](https://www.zhihu.com/equation?tex=%28%5Csigma%5E%7Bi%7D-1%29%3D%28%5Csigma-1%29%281%2B%5Csigma%2B%5Csigma%5E%7B2%7D%2B...%2B%5Csigma%5E%7Bi-1%7D%29)

![(1+\sigma+\sigma^{2}+...+\sigma^{i-1})\in\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%281%2B%5Csigma%2B%5Csigma%5E%7B2%7D%2B...%2B%5Csigma%5E%7Bi-1%7D%29%5Cin%5Cmathbb%7BZ%7D%5BG%5D) ，所以 ![I_{G}](https://www.zhihu.com/equation?tex=I_%7BG%7D) 是principal ideal， ![I_{G}=(\sigma-1)\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=I_%7BG%7D%3D%28%5Csigma-1%29%5Cmathbb%7BZ%7D%5BG%5D) 。 ![\ker (\sigma^{i}\mapsto1)=I_{G}={\rm im}(\sigma-1)](https://www.zhihu.com/equation?tex=%5Cker+%28%5Csigma%5E%7Bi%7D%5Cmapsto1%29%3DI_%7BG%7D%3D%7B%5Crm+im%7D%28%5Csigma-1%29) 。

**Exercise.**![\ker(\sigma-1)={\rm im}(m\mapsto\Sigma_{g\in G}gm)](https://www.zhihu.com/equation?tex=%5Cker%28%5Csigma-1%29%3D%7B%5Crm+im%7D%28m%5Cmapsto%5CSigma_%7Bg%5Cin+G%7Dgm%29)

**（2）**按照一开始我们所说，满足足够的flatness，可以做tensor product还原成关于M的正合列。

detail：拆成两个short：![0\rightarrow\mathbb{Z}\rightarrow\mathbb{Z}[G]\rightarrow I_{G}\rightarrow0,0\rightarrow I_{G}\rightarrow\mathbb{Z}[G]\rightarrow\mathbb{Z}\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow%5Cmathbb%7BZ%7D%5Crightarrow%5Cmathbb%7BZ%7D%5BG%5D%5Crightarrow+I_%7BG%7D%5Crightarrow0%2C0%5Crightarrow+I_%7BG%7D%5Crightarrow%5Cmathbb%7BZ%7D%5BG%5D%5Crightarrow%5Cmathbb%7BZ%7D%5Crightarrow0)

其中 ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D) 当然是free ![\mathbb{Z}-{\rm module}\Rightarrow {\rm flat}~ \mathbb{Z}-{\rm module}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D-%7B%5Crm+module%7D%5CRightarrow+%7B%5Crm+flat%7D~+%5Cmathbb%7BZ%7D-%7B%5Crm+module%7D) ， ![I_{G}](https://www.zhihu.com/equation?tex=I_%7BG%7D) 是自由 ![\mathbb{Z}](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D)-module ![\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D) 的subgroup，仍是free abelian group ![\Rightarrow {\rm flat}~ \mathbb{Z}-{\rm module}](https://www.zhihu.com/equation?tex=%5CRightarrow+%7B%5Crm+flat%7D~+%5Cmathbb%7BZ%7D-%7B%5Crm+module%7D) 。

于是tensor ![\otimes_{\mathbb{Z}}M](https://www.zhihu.com/equation?tex=%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM) ，再拼起来：

![0\rightarrow M\rightarrow\mathbb{Z}[G]\otimes_{\mathbb{Z}}M\rightarrow \mathbb{Z}[G]\otimes_{\mathbb{Z}}M\rightarrow M\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow%5Cmathbb%7BZ%7D%5BG%5D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+%5Cmathbb%7BZ%7D%5BG%5D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM%5Crightarrow+M%5Crightarrow0)

中间的 ![ \mathbb{Z}[G]\otimes_{\mathbb{Z}}M](https://www.zhihu.com/equation?tex=+%5Cmathbb%7BZ%7D%5BG%5D%5Cotimes_%7B%5Cmathbb%7BZ%7D%7DM) 是induced G-module，Tate group=0，于是由dimension shifting

![H^{r}_{T}(G,M)\simeq H^{r+2}_{T}(G,M),r\in\mathbb{Z}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29%5Csimeq+H%5E%7Br%2B2%7D_%7BT%7D%28G%2CM%29%2Cr%5Cin%5Cmathbb%7BZ%7D)

