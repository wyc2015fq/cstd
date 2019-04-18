# p-可除群的基本性质(III): Dieudonne理论 - 知乎
# 

本来想直接讨论Abel概形，但是发现部分工具缺失（例如素数阶群的分类），另外有一些Abel簇的结果往Abel概形上推广遇到了一些困难，故继续做一些简单的观察，同时给出Dieudonne理论基础的一个详细讨论。有了这次讨论，我们可以在下次讨论证明如下结果：

![E](https://www.zhihu.com/equation?tex=E) 是 ![\Bbb F_p](https://www.zhihu.com/equation?tex=%5CBbb+F_p) 上椭圆曲线，那么作为群概形有![E[p]_{\overline{\Bbb F_p}} \cong \mu_p \oplus \Bbb Z/ p \Bbb Z](https://www.zhihu.com/equation?tex=E%5Bp%5D_%7B%5Coverline%7B%5CBbb+F_p%7D%7D+%5Ccong+%5Cmu_p+%5Coplus+%5CBbb+Z%2F+p+%5CBbb+Z) （通常）或者 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) 通过 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) 的一个非分裂扩张（超奇异）

**目标：利用线性代数，理解域k上有限交换群概形的结构**

根据之前的结果，不妨 ![char k=p>0](https://www.zhihu.com/equation?tex=char+k%3Dp%3E0)

参考：[https://people.math.ethz.ch/~pink/FiniteGroupSchemes.html](https://link.zhihu.com/?target=https%3A//people.math.ethz.ch/~pink/FiniteGroupSchemes.html)

**定理1. 特征p>0的代数闭域k上p阶交换群概形只有 **![\Bbb Z/p\Bbb Z,\mu_p,\alpha_p](https://www.zhihu.com/equation?tex=%5CBbb+Z%2Fp%5CBbb+Z%2C%5Cmu_p%2C%5Calpha_p)

Pf. 不妨G连通（平展情况只能是常值 ![\Bbb Z/ p \Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z%2F+p+%5CBbb+Z) ）。 G=SpecA，I=Ker(A to k)为A幂零的极大理想，根据 ![\Omega_{G/S}^1=I/I^2 \otimes_{k} A \not=0](https://www.zhihu.com/equation?tex=%5COmega_%7BG%2FS%7D%5E1%3DI%2FI%5E2+%5Cotimes_%7Bk%7D+A+%5Cnot%3D0) （因为I非零故I/I^2非零）从而存在非零导子 ![d: A \rightarrow k](https://www.zhihu.com/equation?tex=d%3A+A+%5Crightarrow+k) ，于是 ![d \in A^D](https://www.zhihu.com/equation?tex=d+%5Cin+A%5ED) （Cartier duality）且 ![m_D(d)=1 \otimes d+ d\otimes 1](https://www.zhihu.com/equation?tex=m_D%28d%29%3D1+%5Cotimes+d%2B+d%5Cotimes+1) （因为d是导子）。 ![k[d]](https://www.zhihu.com/equation?tex=k%5Bd%5D) 是 ![A^D](https://www.zhihu.com/equation?tex=A%5ED) 的子代数比较维数知必须 ![A^D=k[d]](https://www.zhihu.com/equation?tex=A%5ED%3Dk%5Bd%5D) 。如果 ![G^D](https://www.zhihu.com/equation?tex=G%5ED) 平展那么只能是常值 ![\Bbb Z/p \Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z%2Fp+%5CBbb+Z) ，再对偶一次知 ![G=\mu_p](https://www.zhihu.com/equation?tex=G%3D%5Cmu_p) 。假设其也平展，那么由于d是k导子（k上为0）从而 ![d \in I^D](https://www.zhihu.com/equation?tex=d+%5Cin+I%5ED) 幂零。又比较维数知 ![d^p=0，d^{p-1}\not=0](https://www.zhihu.com/equation?tex=d%5Ep%3D0%EF%BC%8Cd%5E%7Bp-1%7D%5Cnot%3D0) ，又 ![m_D(d)=1 \otimes d+ d\otimes 1](https://www.zhihu.com/equation?tex=m_D%28d%29%3D1+%5Cotimes+d%2B+d%5Cotimes+1) ，可知必须 ![G^D=\alpha_p](https://www.zhihu.com/equation?tex=G%5ED%3D%5Calpha_p) ，对偶知 ![G=\alpha_p](https://www.zhihu.com/equation?tex=G%3D%5Calpha_p) 。

注：这里用到特征p的S上有对偶关系： ![(\Bbb Z/p \Bbb Z)^D=\mu_p,\mu_p^D=\Bbb Z/p \Bbb Z,\alpha_p^D=\alpha_p](https://www.zhihu.com/equation?tex=%28%5CBbb+Z%2Fp+%5CBbb+Z%29%5ED%3D%5Cmu_p%2C%5Cmu_p%5ED%3D%5CBbb+Z%2Fp+%5CBbb+Z%2C%5Calpha_p%5ED%3D%5Calpha_p) ，这可通过将对应的余乘法、乘法具体写出得到。

注：实际上任意S上p阶平坦群概形（要求结构层局部自由，例如S诺特）必交换，另外在较好的S上（要求over一个特殊的环，这样的S有 ![\Bbb Z_p](https://www.zhihu.com/equation?tex=%5CBbb+Z_p) 和特征0的代数闭域，可惜 ![\Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z) 不是这样的例子）下Tate和Oort有完整的分类（类似之前提过的p=2情况），参见他们的文章Group schemes of prime order，也有一篇ALGANT的M2论文写了这个话题，见[GALVAGNI.pdf](https://link.zhihu.com/?target=https%3A//www.math.u-bordeaux.fr/~ybilu/algant/documents/theses/GALVAGNI.pdf)。
![](https://pic4.zhimg.com/v2-83874114416044641324a2fedadb2de3_b.jpg)

![](https://pic2.zhimg.com/v2-f62c8282232f700a74de5dd52665525d_b.jpg)
注：一般概形S上有限交换平坦群概形范畴不一定Abelian，例子见[The category of finite locally-free commutative group schemes](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/7688/the-category-of-finite-locally-free-commutative-group-schemes)（另外子对象的平坦性也是一个问题）
![](https://pic4.zhimg.com/v2-865add80d65fda60f6033f392a0ae47f_b.jpg)
（这方面Raynaud有一个定理将一些较好的环上情况与其分式域上情况联系）

特证p>0的域k上一个重要工具是相对Frob， ![Fr: G \rightarrow G^{(p)}](https://www.zhihu.com/equation?tex=Fr%3A+G+%5Crightarrow+G%5E%7B%28p%29%7D) ，上次证明定理4时说明了:

**性质. G是连通交换群概形，则**![0 \rightarrow H \rightarrow G \overset{Fr}\rightarrow G^{(p)}](https://www.zhihu.com/equation?tex=0+%5Crightarrow+H+%5Crightarrow+G+%5Coverset%7BFr%7D%5Crightarrow+G%5E%7B%28p%29%7D)** 中H的阶为 **![p^{r}, r=dim_k I/I^2](https://www.zhihu.com/equation?tex=p%5E%7Br%7D%2C+r%3Ddim_k+I%2FI%5E2)

利用 ![(G^{(p)})^D \cong (G^D)^{(p)}](https://www.zhihu.com/equation?tex=%28G%5E%7B%28p%29%7D%29%5ED+%5Ccong+%28G%5ED%29%5E%7B%28p%29%7D) 进一步可定义 Verschiebung of G为同态![Ver: G^{(p)} \rightarrow G](https://www.zhihu.com/equation?tex=Ver%3A+G%5E%7B%28p%29%7D+%5Crightarrow+G) (其中 ![Ver := (Fr_{G^D})^D](https://www.zhihu.com/equation?tex=Ver+%3A%3D+%28Fr_%7BG%5ED%7D%29%5ED) )，两个同态都对G具有函子性且与基域变换、乘积可交换，一个重要性质是
![](https://pic3.zhimg.com/v2-9a29ec45c45e81c42810320d8a0c8756_b.jpg)


现在假设k **perfect**，那么Cartier对偶和连通-平展正合列函子性的分裂可有范畴的分裂：

![FC/k=0-0 \oplus0-et \oplus et-0 \oplus et-et](https://www.zhihu.com/equation?tex=FC%2Fk%3D0-0+%5Coplus0-et+%5Coplus+et-0+%5Coplus+et-et) ，其中 ![1 - 2](https://www.zhihu.com/equation?tex=1+-+2) 中对象是自身满足1而对偶满足2的有限交换群概形，也就是说任何G都可分成四部分。我们已经知道有限平展交换群概形范畴和Galois群的有限表示范畴等价，故通过Galois理论有很好的理解。通过对偶，只需着重研究：

**![G, G^D](https://www.zhihu.com/equation?tex=G%2C+G%5ED) 都连通(i.e 局部)的情况(称为 local-local type)**

此时不难看出 ![Fr,Ver](https://www.zhihu.com/equation?tex=Fr%2CVer) 都是幂零算子（ ![0 \rightarrow H \rightarrow G \overset{Fr}\rightarrow G^{(p)}](https://www.zhihu.com/equation?tex=0+%5Crightarrow+H+%5Crightarrow+G+%5Coverset%7BFr%7D%5Crightarrow+G%5E%7B%28p%29%7D) 中H的阶是 ![p^{dimI/I^2}](https://www.zhihu.com/equation?tex=p%5E%7BdimI%2FI%5E2%7D) ，故非平凡群的Fr的Kernel非平凡，于是再对 ![G/H](https://www.zhihu.com/equation?tex=G%2FH) 考虑即知Fr幂零，再使用对偶；当然Fr的幂零性也可由I幂零和绝对Frobenius的作用是p次方看出）。

重要例子： ![\alpha_p=Ker( \Bbb G_{a} \overset{Fr} \rightarrow \Bbb G_a)](https://www.zhihu.com/equation?tex=%5Calpha_p%3DKer%28+%5CBbb+G_%7Ba%7D+%5Coverset%7BFr%7D+%5Crightarrow+%5CBbb+G_a%29)

注意local-local type全体也构成Abel范畴 ![FC^{0,0}/k](https://www.zhihu.com/equation?tex=FC%5E%7B0%2C0%7D%2Fk) （利用连通集的像还连通，local-local type的子群和商群也是local-local type），在k代数闭的情况已经知道这个范畴里单对象只有 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) ，实际上对一般perfect field k也对，只需要证明

**性质. **![G](https://www.zhihu.com/equation?tex=G) local-local， ![Fr=0, Ver=0](https://www.zhihu.com/equation?tex=Fr%3D0%2C+Ver%3D0) ，那么 ![G](https://www.zhihu.com/equation?tex=G) 是 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) 的直和

Pf. 沿用记号 ![G=\text{Spec}A](https://www.zhihu.com/equation?tex=G%3D%5Ctext%7BSpec%7DA) ，I是配边理想。已证 ![0 \rightarrow H \rightarrow G \overset{Fr}\rightarrow G^{(p)}](https://www.zhihu.com/equation?tex=0+%5Crightarrow+H+%5Crightarrow+G+%5Coverset%7BFr%7D%5Crightarrow+G%5E%7B%28p%29%7D) 中H的阶是 ![p^{dimI/I^2}](https://www.zhihu.com/equation?tex=p%5E%7BdimI%2FI%5E2%7D) ，故Fr=0推出 ![dimT_{G/k}=dim I/I^2=log_p|G|:=n](https://www.zhihu.com/equation?tex=dimT_%7BG%2Fk%7D%3Ddim+I%2FI%5E2%3Dlog_p%7CG%7C%3A%3Dn) 。注意 ![T_{G/k}=Hom(G^D,\Bbb G_a)](https://www.zhihu.com/equation?tex=T_%7BG%2Fk%7D%3DHom%28G%5ED%2C%5CBbb+G_a%29) （习题（4）），而 ![Ver=0](https://www.zhihu.com/equation?tex=Ver%3D0) 以及Ver的函子性表明有同构 ![Hom(G,\Bbb G_a)=Hom(G,\alpha_p)](https://www.zhihu.com/equation?tex=Hom%28G%2C%5CBbb+G_a%29%3DHom%28G%2C%5Calpha_p%29) （即任何G到G_a的态射都复合Fr后为0），于是存在非平凡同态 ![f_1:G^D \rightarrow \alpha_p](https://www.zhihu.com/equation?tex=f_1%3AG%5ED+%5Crightarrow+%5Calpha_p) (由于后者单，必为满射)，由维数考虑(![Hom(\alpha_p,\alpha_p)=k](https://www.zhihu.com/equation?tex=Hom%28%5Calpha_p%2C%5Calpha_p%29%3Dk) )可找到 另一个非平凡同态![f_2 :G^D \rightarrow \alpha_p](https://www.zhihu.com/equation?tex=f_2+%3AG%5ED+%5Crightarrow+%5Calpha_p) 限制在 ![f_1](https://www.zhihu.com/equation?tex=f_1) 的Kernel上非平凡，继续操作最终得到满射 ![f=\oplus f_i: G^D \rightarrow \alpha_p^{\oplus n}](https://www.zhihu.com/equation?tex=f%3D%5Coplus+f_i%3A+G%5ED+%5Crightarrow+%5Calpha_p%5E%7B%5Coplus+n%7D) ，比较阶数知为同构，对偶即证。

**推论. k是perfect field，则![FC^{0,0}/k](https://www.zhihu.com/equation?tex=FC%5E%7B0%2C0%7D%2Fk)中单对象只有**![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p)

即使知道了单对象，还需要对extension作分类，这是一件很麻烦的事。回忆[Mitchell's embedding theorem](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Mitchell%2527s_embedding_theorem)说任何Abel范畴都可以嵌入环R的模范畴，模范畴是具体而可以用**线性代数**处理的。找到合适的环R（根据Morita等价这样的R不唯一）并证明范畴等价，**便是Dieudonne 理论**。

在 ![FC^{0,0}/k](https://www.zhihu.com/equation?tex=FC%5E%7B0%2C0%7D%2Fk) 里我们有 ![F=Fr, V=Ver](https://www.zhihu.com/equation?tex=F%3DFr%2C+V%3DVer) ，并且两者的性质似乎决定了对象（例如上面说明了 ![Fr=0, Ver=0](https://www.zhihu.com/equation?tex=Fr%3D0%2C+Ver%3D0) ，那么 ![G](https://www.zhihu.com/equation?tex=G) 是 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) 的直和），我们希望找这样的环R，使得上面有类似地Frobenius、Verschiebung算子的作用（Verschiebung 是德语，意思为平移），并且R的构造是关于k有函子性的。

问题：符合这些条件的R是什么？

答案：Witt环！

回忆：[Witt vector - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Witt_vector)（这里可见定义的动机）

基本性质证明参考：[[1409.7445] The Theory of Witt Vectors](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1409.7445)

构造想法很简单： ![R](https://www.zhihu.com/equation?tex=R) 是特征0的完备离散赋值环，p生成极大理想且k=R/p，那么 ![R](https://www.zhihu.com/equation?tex=R) 中每个元都可写成p的级数（k系数），但乘法和加法和级数不一样（因为进位），具体写出来就是Witt环。（k不perfect时应考虑更一般的Cohen环）

我们有：

**定理4.(Witt)  Witt环的构造给出了{特征p>0的perfect field}到{p生成极大理想、特征0的完备离散赋值环}的范畴等价。**

例子： ![W(\Bbb F_p)=\Bbb Z_p](https://www.zhihu.com/equation?tex=W%28%5CBbb+F_p%29%3D%5CBbb+Z_p)

![W](https://www.zhihu.com/equation?tex=W)的构造可进一步推广到一般环上，得到 ![\Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z) 上Witt概形 ![W](https://www.zhihu.com/equation?tex=W) （W是交换群概形，作为概形是 ![\text{Spec} \Bbb Z[x_0,x_1,x_2,…]](https://www.zhihu.com/equation?tex=%5Ctext%7BSpec%7D+%5CBbb+Z%5Bx_0%2Cx_1%2Cx_2%2C%E2%80%A6%5D),），W上群结构是唯一使得Ghost map ![\Phi_n: W \rightarrow \Bbb G_a](https://www.zhihu.com/equation?tex=%5CPhi_n%3A+W+%5Crightarrow+%5CBbb+G_a) 都是群同态的群结构。现在回到我们的情况，k是特征p的perfect field，通过基变换将W看成k上交换群概形，注意到对仿射群概形都可以定义Frobenius和Verschiebung算子，于是得到W上的算子 ![F: W \rightarrow W, V: W \rightarrow W](https://www.zhihu.com/equation?tex=F%3A+W+%5Crightarrow+W%2C+V%3A+W+%5Crightarrow+W) （由于Frob在k上是同构，这里自然等同 ![W^{(p)}](https://www.zhihu.com/equation?tex=W%5E%7B%28p%29%7D) 与 ![W](https://www.zhihu.com/equation?tex=W) ）

**定理 5.**

**①**![F \circ V=V \circ F=p\cdot\text{id}](https://www.zhihu.com/equation?tex=F+%5Ccirc+V%3DV+%5Ccirc+F%3Dp%5Ccdot%5Ctext%7Bid%7D)

**②**![F((x_0,x_1,\cdots))=(x_0^p,x_1^p,\cdots)](https://www.zhihu.com/equation?tex=F%28%28x_0%2Cx_1%2C%5Ccdots%29%29%3D%28x_0%5Ep%2Cx_1%5Ep%2C%5Ccdots%29)  （于是F是满的环同态）

**③ ![V((x_0,x_1,\cdots))=(0,x_0,x_1,\cdots)](https://www.zhihu.com/equation?tex=V%28%28x_0%2Cx_1%2C%5Ccdots%29%29%3D%280%2Cx_0%2Cx_1%2C%5Ccdots%29)**

**④ ![p\cdot(x_0,x_1,\cdots)=(0,x_0^p,x_1^p,\cdots)](https://www.zhihu.com/equation?tex=p%5Ccdot%28x_0%2Cx_1%2C%5Ccdots%29%3D%280%2Cx_0%5Ep%2Cx_1%5Ep%2C%5Ccdots%29)**

现在的想法是截断W，令![W_n=W/V^nW](https://www.zhihu.com/equation?tex=W_n%3DW%2FV%5EnW) （作为群概形的商）为长度为n的Witt环（其为k上有限型交换群概形），令 ![W_n^m=Ker(F^m:W_n \rightarrow W_n)](https://www.zhihu.com/equation?tex=W_n%5Em%3DKer%28F%5Em%3AW_n+%5Crightarrow+W_n%29) 为Frobenius的m次方的Kernel（其为k上有限交换群概形）

注意到截断给出满同态 ![r:W_{n+1}^m \rightarrow W^m_n](https://www.zhihu.com/equation?tex=r%3AW_%7Bn%2B1%7D%5Em+%5Crightarrow+W%5Em_n) ，Verschiebung给出单同态 ![v:W_n^m \hookrightarrow W_{n+1}^m](https://www.zhihu.com/equation?tex=v%3AW_n%5Em+%5Chookrightarrow+W_%7Bn%2B1%7D%5Em) ，自然包含给出单同态 ![i:W_n^m \rightarrow W_n^{m+1}](https://www.zhihu.com/equation?tex=i%3AW_n%5Em+%5Crightarrow+W_n%5E%7Bm%2B1%7D) ，Frobenius给出满同态 ![f:W_n^{m+1} \rightarrow W_n^{m}](https://www.zhihu.com/equation?tex=f%3AW_n%5E%7Bm%2B1%7D+%5Crightarrow+W_n%5E%7Bm%7D) ，有对应的正合列： ![0 \overset{}\rightarrow W_n^{m} \overset{v^{n'}}\rightarrow W_{n+n'}^m \overset{r^{n}}\rightarrow W_{n'}^m \rightarrow 0](https://www.zhihu.com/equation?tex=0+%5Coverset%7B%7D%5Crightarrow+W_n%5E%7Bm%7D+%5Coverset%7Bv%5E%7Bn%27%7D%7D%5Crightarrow+W_%7Bn%2Bn%27%7D%5Em+%5Coverset%7Br%5E%7Bn%7D%7D%5Crightarrow+W_%7Bn%27%7D%5Em+%5Crightarrow+0) ， ![0 \overset{}\rightarrow W_n^{m} \overset{i^{m'}}\rightarrow W_{n}^{m+m'} \overset{f^{m}}\rightarrow W_{n}^{m'} \rightarrow 0](https://www.zhihu.com/equation?tex=0+%5Coverset%7B%7D%5Crightarrow+W_n%5E%7Bm%7D+%5Coverset%7Bi%5E%7Bm%27%7D%7D%5Crightarrow+W_%7Bn%7D%5E%7Bm%2Bm%27%7D+%5Coverset%7Bf%5E%7Bm%7D%7D%5Crightarrow+W_%7Bn%7D%5E%7Bm%27%7D+%5Crightarrow+0) (注意rv=vr=V，fi=if=F，r带有section ![x \rightarrow (x,0)](https://www.zhihu.com/equation?tex=x+%5Crightarrow+%28x%2C0%29) 故正合)

例子： ![W_1=\Bbb G_a, W_1^1=\alpha_p](https://www.zhihu.com/equation?tex=W_1%3D%5CBbb+G_a%2C+W_1%5E1%3D%5Calpha_p) ,由正合列得 ![W_n^{m}](https://www.zhihu.com/equation?tex=W_n%5E%7Bm%7D) 是mn个 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) 的扩张(因此local-local type)

定义 ![W_n^{m}](https://www.zhihu.com/equation?tex=W_n%5E%7Bm%7D) 的原因是它们代表了最典型的local-local type群概形，首先由F，V的函子性易得

**性质**. G local-local type且 ![V_G^{n}=0, F_G^{m}=0](https://www.zhihu.com/equation?tex=V_G%5E%7Bn%7D%3D0%2C+F_G%5E%7Bm%7D%3D0) ，则通过i，v的复合诱导映射 ( ![m' \geq m, n' \geq n](https://www.zhihu.com/equation?tex=m%27+%5Cgeq+m%2C+n%27+%5Cgeq+n) )![Hom(G,W_n^m) \rightarrow Hom(G,W_{n'}^{m'})](https://www.zhihu.com/equation?tex=Hom%28G%2CW_n%5Em%29+%5Crightarrow+Hom%28G%2CW_%7Bn%27%7D%5E%7Bm%27%7D%29) 是同构

**重要性质. **![(\text{Ext}^1(\alpha_p,W_{n}^{m}) \rightarrow Ext^1(\alpha_p,W_{n+1}^{m+1}) )=0](https://www.zhihu.com/equation?tex=%28%5Ctext%7BExt%7D%5E1%28%5Calpha_p%2CW_%7Bn%7D%5E%7Bm%7D%29+%5Crightarrow+Ext%5E1%28%5Calpha_p%2CW_%7Bn%2B1%7D%5E%7Bm%2B1%7D%29+%29%3D0) （映射由iv诱导）

Pf: 这将涉及过多抽象废话，证明基本上基于 ![End(\alpha_p)=k](https://www.zhihu.com/equation?tex=End%28%5Calpha_p%29%3Dk) 以及F，V的函子性，见Ref。

**定理 6.  k上任何local-local的G都可嵌入某个**![(W_n^m)^{\oplus r}](https://www.zhihu.com/equation?tex=%28W_n%5Em%29%5E%7B%5Coplus+r%7D)

Pf: 对|G|归纳，之前已说明单对象只有 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) ，故可找 ![1 \rightarrow G' \rightarrow G \rightarrow \alpha_p \rightarrow 1](https://www.zhihu.com/equation?tex=1+%5Crightarrow+G%27+%5Crightarrow+G+%5Crightarrow+%5Calpha_p+%5Crightarrow+1)   ，找一个嵌入 ![(\psi_i): G' \rightarrow (W_n^m)^{\oplus r}](https://www.zhihu.com/equation?tex=%28%5Cpsi_i%29%3A+G%27+%5Crightarrow+%28W_n%5Em%29%5E%7B%5Coplus+r%7D) ，于是可补全正合列如图：
![](https://pic1.zhimg.com/v2-a3e2ec775ece0052a906e08efd8539ac_b.jpg)
由于 ![(\text{Ext}^1(\alpha_p,W_{n}^{m}) \rightarrow Ext^1(\alpha_p,W_{n+1}^{m+1}) )=0](https://www.zhihu.com/equation?tex=%28%5Ctext%7BExt%7D%5E1%28%5Calpha_p%2CW_%7Bn%7D%5E%7Bm%7D%29+%5Crightarrow+Ext%5E1%28%5Calpha_p%2CW_%7Bn%2B1%7D%5E%7Bm%2B1%7D%29+%29%3D0) ，把 ![W_n^m ](https://www.zhihu.com/equation?tex=W_n%5Em+) 换成 ![W_{n+1}^{m+1}](https://www.zhihu.com/equation?tex=W_%7Bn%2B1%7D%5E%7Bm%2B1%7D) 不妨设下面的正合列分裂(对每个i)，这样通过section得到 ![ G \rightarrow (W_n^m)^{\oplus r}](https://www.zhihu.com/equation?tex=+G+%5Crightarrow+%28W_n%5Em%29%5E%7B%5Coplus+r%7D) 再加上最初的 ![G \rightarrow \alpha_1=W_1^{1} \hookrightarrow W^m_n](https://www.zhihu.com/equation?tex=G+%5Crightarrow+%5Calpha_1%3DW_1%5E%7B1%7D+%5Chookrightarrow+W%5Em_n) 即可。

**推论. G local-local type且 ![V_G^{n}=0, F_G^{m}=0](https://www.zhihu.com/equation?tex=V_G%5E%7Bn%7D%3D0%2C+F_G%5E%7Bm%7D%3D0) ，则有正合列**![0 \rightarrow G \rightarrow (W_n^m)^{\oplus r} \rightarrow (W_n^m)^{\oplus s}](https://www.zhihu.com/equation?tex=0+%5Crightarrow+G+%5Crightarrow+%28W_n%5Em%29%5E%7B%5Coplus+r%7D+%5Crightarrow+%28W_n%5Em%29%5E%7B%5Coplus+s%7D)

于是现在我们不仅知道 ![FC^{0,0}/k](https://www.zhihu.com/equation?tex=FC%5E%7B0%2C0%7D%2Fk) 的单对象只有 ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) ，也知道任何对象有好的表现，是时候来寻找合适的环了。注意到W上同时带有 ![F,V,W(k)](https://www.zhihu.com/equation?tex=F%2CV%2CW%28k%29) 的作用（ ![W(k)](https://www.zhihu.com/equation?tex=W%28k%29) 通过W的群运算作用）。令 ![\sigma : W(k) \rightarrow W(k)](https://www.zhihu.com/equation?tex=%5Csigma+%3A+W%28k%29+%5Crightarrow+W%28k%29) 是k上Frobenius ![x \rightarrow x^p](https://www.zhihu.com/equation?tex=x+%5Crightarrow+x%5Ep) 的提升，这些作用满足的关系可具体写出，但这里比较微妙的一点是作用定义需要与同态相容，因此有：

定义.Dieudonne环为 ![D=W(k)[F,V]/ \sim](https://www.zhihu.com/equation?tex=D%3DW%28k%29%5BF%2CV%5D%2F+%5Csim) 模掉关系 ![FV=VF=p,F \xi= \sigma(\xi)F, V \sigma(\xi)=\xi V](https://www.zhihu.com/equation?tex=FV%3DVF%3Dp%2CF+%5Cxi%3D+%5Csigma%28%5Cxi%29F%2C+V+%5Csigma%28%5Cxi%29%3D%5Cxi+V)

例子： ![k=\Bbb F_p \Rightarrow D=\Bbb Z_p[x,y]/(xy-p)](https://www.zhihu.com/equation?tex=k%3D%5CBbb+F_p+%5CRightarrow+D%3D%5CBbb+Z_p%5Bx%2Cy%5D%2F%28xy-p%29) (一般情况D不交换)

**定理 7.  D可作用在 ![W_n^m](https://www.zhihu.com/equation?tex=W_n%5Em) 上，其中F，V作用如上， ![\xi \in W(k)](https://www.zhihu.com/equation?tex=%5Cxi+%5Cin+W%28k%29) 作用是![\sigma^{-n}(\xi)](https://www.zhihu.com/equation?tex=%5Csigma%5E%7B-n%7D%28%5Cxi%29) 的乘法作用，且作用与 ![W_n^m](https://www.zhihu.com/equation?tex=W_n%5Em)之间同态![i,v](https://www.zhihu.com/equation?tex=i%2Cv)相容。**

现在任给一个k上local-local type的G，定义 ![M(G)= \varinjlim_{n,m} Hom(G,W_n^m)=\varinjlim_{n} Hom(G,W_n)](https://www.zhihu.com/equation?tex=M%28G%29%3D+%5Cvarinjlim_%7Bn%2Cm%7D+Hom%28G%2CW_n%5Em%29%3D%5Cvarinjlim_%7Bn%7D+Hom%28G%2CW_n%29) 是一个D模。注意到若 ![F_G^m=0,V_G^n=0](https://www.zhihu.com/equation?tex=F_G%5Em%3D0%2CV_G%5En%3D0) 则由F，V的函子性得到 ![M(G)=Hom(G,W_n^m)](https://www.zhihu.com/equation?tex=M%28G%29%3DHom%28G%2CW_n%5Em%29) ，于是 ![End(W_n^m)=M(W_n^m)](https://www.zhihu.com/equation?tex=End%28W_n%5Em%29%3DM%28W_n%5Em%29) ，故 ![M(\alpha_p)=k](https://www.zhihu.com/equation?tex=M%28%5Calpha_p%29%3Dk) 。

接下来我们证明：

（main theorem of contravariant Dieudonn´e theory in the local-local
case）

**主要定理：**

**函子M定义了Abel范畴间的反等价：**

**{k上有限local-local交换群概形} ![\Leftrightarrow](https://www.zhihu.com/equation?tex=%5CLeftrightarrow)  {F，V作用幂零的有限**![W(k)](https://www.zhihu.com/equation?tex=W%28k%29)**长度的左D模}**

想法是：Hom函子本身左正合，但取极限再利用 ![(\text{Ext}^1(\alpha_p,W_{n}^{m}) \rightarrow Ext^1(\alpha_p,W_{n+1}^{m+1}) )=0](https://www.zhihu.com/equation?tex=%28%5Ctext%7BExt%7D%5E1%28%5Calpha_p%2CW_%7Bn%7D%5E%7Bm%7D%29+%5Crightarrow+Ext%5E1%28%5Calpha_p%2CW_%7Bn%2B1%7D%5E%7Bm%2B1%7D%29+%29%3D0) 可以证明M在特殊情况正合，然后数维数说明M正合，再进一步论证。

首先我们说明

① ![\text{length}_{W(k)} M(G)=\text{log}_p |G|](https://www.zhihu.com/equation?tex=%5Ctext%7Blength%7D_%7BW%28k%29%7D+M%28G%29%3D%5Ctext%7Blog%7D_p+%7CG%7C)  （这顺便说明上面M的像确实在右边）

Pf. 对|G|归纳， ![G=\alpha_p](https://www.zhihu.com/equation?tex=G%3D%5Calpha_p) 时![M(\alpha_p)=k](https://www.zhihu.com/equation?tex=M%28%5Calpha_p%29%3Dk) 故正确，一般先找 ![1 \rightarrow G' \rightarrow G \rightarrow \alpha_p \rightarrow 1](https://www.zhihu.com/equation?tex=1+%5Crightarrow+G%27+%5Crightarrow+G+%5Crightarrow+%5Calpha_p+%5Crightarrow+1) ，只用说明M作用后还正合便可归纳得到结果。根据左正合性只需说明 ![M(G) \rightarrow M(G')](https://www.zhihu.com/equation?tex=M%28G%29+%5Crightarrow+M%28G%27%29) 满射。任取 ![M(G)](https://www.zhihu.com/equation?tex=M%28G%29) 中元 ![\psi_i: G' \rightarrow W_n^m](https://www.zhihu.com/equation?tex=%5Cpsi_i%3A+G%27+%5Crightarrow+W_n%5Em) ，补全正合列如图：
![](https://pic1.zhimg.com/v2-a3e2ec775ece0052a906e08efd8539ac_b.jpg)
由于 ![(\text{Ext}^1(\alpha_p,W_{n}^{m}) \rightarrow Ext^1(\alpha_p,W_{n+1}^{m+1}) )=0](https://www.zhihu.com/equation?tex=%28%5Ctext%7BExt%7D%5E1%28%5Calpha_p%2CW_%7Bn%7D%5E%7Bm%7D%29+%5Crightarrow+Ext%5E1%28%5Calpha_p%2CW_%7Bn%2B1%7D%5E%7Bm%2B1%7D%29+%29%3D0) ，不妨把 ![W_n^m ](https://www.zhihu.com/equation?tex=W_n%5Em+) 换成 ![W_{n+1}^{m+1}](https://www.zhihu.com/equation?tex=W_%7Bn%2B1%7D%5E%7Bm%2B1%7D) （这也是为什么我们的M要取Hom极限）设下面的正合列分裂，则section给出 ![M(G)](https://www.zhihu.com/equation?tex=M%28G%29) 中合适的元，即证。

②M是正合函子，并且 ![M(W_n^m)=End(W_n^m)=D/(DF^m+DV^n):=D^m_n](https://www.zhihu.com/equation?tex=M%28W_n%5Em%29%3DEnd%28W_n%5Em%29%3DD%2F%28DF%5Em%2BDV%5En%29%3A%3DD%5Em_n)

Pf. M左正合，由于①可以两边数阶数/长度得到M是正合的。首先有自然的同态（即D的作用）![D^m_n \rightarrow M(W_n^m)](https://www.zhihu.com/equation?tex=D%5Em_n+%5Crightarrow+M%28W_n%5Em%29) ，注意 ![D^m_n](https://www.zhihu.com/equation?tex=D%5Em_n) 任何非平凡子模一定包括 ![F^{m-1}V^{m-1}](https://www.zhihu.com/equation?tex=F%5E%7Bm-1%7DV%5E%7Bm-1%7D) ，而其在 ![W_n^m](https://www.zhihu.com/equation?tex=W_n%5Em) 上作用不是0，故自然同态是单射，两边数阶数/长度即得。（上面已经说明 ![|W_n^m|=p^{mn}](https://www.zhihu.com/equation?tex=%7CW_n%5Em%7C%3Dp%5E%7Bmn%7D) ）

③M is fully faithful.（即M诱导的Hom间态射是同构）

Pf. 这里根据5-引理和有限表现技巧（之前说明每个G都有 ![0 \rightarrow G \rightarrow (W_n^m)^{\oplus r} \rightarrow (W_n^m)^{\oplus s}](https://www.zhihu.com/equation?tex=0+%5Crightarrow+G+%5Crightarrow+%28W_n%5Em%29%5E%7B%5Coplus+r%7D+%5Crightarrow+%28W_n%5Em%29%5E%7B%5Coplus+s%7D) ,其中m，n只要分别使得F，V的次方=0，故可取任意大），只需要说明对充分大的m，n ![Hom(G,W_n^m) \rightarrow Hom_D(M(W_n^m),M(G))](https://www.zhihu.com/equation?tex=Hom%28G%2CW_n%5Em%29+%5Crightarrow+Hom_D%28M%28W_n%5Em%29%2CM%28G%29%29) 是同构。可是左边= ![M(G)](https://www.zhihu.com/equation?tex=M%28G%29) （定义），右边= ![Hom_D(D^m_n,M(G))=Hom_D(D,M(G))=M(G)](https://www.zhihu.com/equation?tex=Hom_D%28D%5Em_n%2CM%28G%29%29%3DHom_D%28D%2CM%28G%29%29%3DM%28G%29) ，即证。

④M is essentially surjective（即M的像同构意义下包含所有对象）

Pf. 这是因为右边每个模都有 ![D_n^m](https://www.zhihu.com/equation?tex=D_n%5Em)有限表现 ![({D^{m}_n})^{\oplus s} \rightarrow ({D^{m}_n})^{\oplus s} \rightarrow M \rightarrow 0](https://www.zhihu.com/equation?tex=%28%7BD%5E%7Bm%7D_n%7D%29%5E%7B%5Coplus+s%7D+%5Crightarrow+%28%7BD%5E%7Bm%7D_n%7D%29%5E%7B%5Coplus+s%7D+%5Crightarrow+M+%5Crightarrow+0) .

于是完成了证明。上面论证可以一般化（通过取内射包对偶， ![\alpha_p](https://www.zhihu.com/equation?tex=%5Calpha_p) 的内射包就是M定义里的 ![\lim W_n^m](https://www.zhihu.com/equation?tex=%5Clim+W_n%5Em) ），另一个例子是[Matlis duality - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Matlis_duality)：
![](https://pic3.zhimg.com/v2-d0842011b7f3850176f2061e2a477c5e_b.jpg)![](https://pic2.zhimg.com/v2-03ebd029084c822c44fb9b0cfe4c4ca5_b.jpg)
现在我们希望把M推广到一般的阶为p的幂的有限交换群概形上（注意阶与p互素的一定平展，对偶也平展，比较简单），此时 ![\Bbb Z/p\Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z%2Fp%5CBbb+Z) 的对偶 ![\mu_p](https://www.zhihu.com/equation?tex=%5Cmu_p) 连通而不是平展，故四种情况中的etale-etale情形不存在，除了已处理的local-local外还剩下etale(-local), local-etale两种。

回忆 ![G^D=\underline{Hom}(G,\Bbb G_m)](https://www.zhihu.com/equation?tex=G%5ED%3D%5Cunderline%7BHom%7D%28G%2C%5CBbb+G_m%29) （实际上对 ![G, H \in FC/k](https://www.zhihu.com/equation?tex=G%2C+H+%5Cin+FC%2Fk) 定义 ![Scheme/k \rightarrow {Ab}](https://www.zhihu.com/equation?tex=Scheme%2Fk+%5Crightarrow+%7BAb%7D) 的函子![\underline{Hom}(G,H)=(T \rightarrow Hom(G_T,H_T))](https://www.zhihu.com/equation?tex=%5Cunderline%7BHom%7D%28G%2CH%29%3D%28T+%5Crightarrow+Hom%28G_T%2CH_T%29%29) 都可由有限交换群概形表出），并有自然的pairing： ![G \times G^D \rightarrow \Bbb G_m](https://www.zhihu.com/equation?tex=G+%5Ctimes+G%5ED+%5Crightarrow+%5CBbb+G_m) 。注意D模上也有对偶 ![D:=Hom_{W(k)} (-, W(k)[\frac{1}{p}]/W(k) )](https://www.zhihu.com/equation?tex=D%3A%3DHom_%7BW%28k%29%7D+%28-%2C+W%28k%29%5B%5Cfrac%7B1%7D%7Bp%7D%5D%2FW%28k%29+%29) ,可以说明 ![M(G^D)=M(G)^D](https://www.zhihu.com/equation?tex=M%28G%5ED%29%3DM%28G%29%5ED) (化归为说明![(W_n^m)^D=W_m^n](https://www.zhihu.com/equation?tex=%28W_n%5Em%29%5ED%3DW_m%5En) )。现在对于平展的G定义 ![M(G)= \varinjlim_n Hom(G,W_n)](https://www.zhihu.com/equation?tex=M%28G%29%3D+%5Cvarinjlim_n+Hom%28G%2CW_n%29) ，而local-etale的G，定义 ![M(G)=M(G^D)^D](https://www.zhihu.com/equation?tex=M%28G%29%3DM%28G%5ED%29%5ED) 。类似地有

**主要定理 ：**

**函子M定义了Abel范畴间的反等价：**

**{阶为p的幂的k上有限平展交换群概形} **![\Leftrightarrow](https://www.zhihu.com/equation?tex=%5CLeftrightarrow)**  {F作用为同构的有限**![W(k)](https://www.zhihu.com/equation?tex=W%28k%29)**长度的左D模}**

由于 ![VF=p](https://www.zhihu.com/equation?tex=VF%3Dp) 故 ![V=pF^{-1}](https://www.zhihu.com/equation?tex=V%3DpF%5E%7B-1%7D) 被F唯一决定，因此我们可重新把右边范畴写成：

**{ **![(M, F)](https://www.zhihu.com/equation?tex=%28M%2C+F%29)**  |M有限长度**![W(k)](https://www.zhihu.com/equation?tex=W%28k%29)**模，F是**![\sigma-](https://www.zhihu.com/equation?tex=%5Csigma-)**线性同构**![F: M \rightarrow M](https://www.zhihu.com/equation?tex=F%3A+M+%5Crightarrow+M)**}**

（**![\sigma-](https://www.zhihu.com/equation?tex=%5Csigma-) 线性即 **![F(\xi.v)=\sigma(\xi).v ,\ \ \forall \xi \in W(k), \ v \in M](https://www.zhihu.com/equation?tex=F%28%5Cxi.v%29%3D%5Csigma%28%5Cxi%29.v+%2C%5C+%5C+%5Cforall+%5Cxi+%5Cin+W%28k%29%2C+%5C+v+%5Cin+M) ）

这里也用了M这个记号，相信可以与函子M区分，下同。为了证明首先我们需要右边范畴一个结果：

①若k代数闭时，则 ![M^{F=1}=\{v \in M | Fv=v\}](https://www.zhihu.com/equation?tex=M%5E%7BF%3D1%7D%3D%5C%7Bv+%5Cin+M+%7C+Fv%3Dv%5C%7D) 是有限交换p群，且自然映射 ![\xi \otimes x \mapsto \xi.x](https://www.zhihu.com/equation?tex=%5Cxi+%5Cotimes+x+%5Cmapsto+%5Cxi.x) 诱导同构 ![W(k) \otimes_{\Bbb Z_p} M^{F=1} \cong M](https://www.zhihu.com/equation?tex=W%28k%29+%5Cotimes_%7B%5CBbb+Z_p%7D+M%5E%7BF%3D1%7D+%5Ccong+M) ，特别地 ![\text{length}_{W(k)} M=\text{log}_p|M^{F=1}|](https://www.zhihu.com/equation?tex=%5Ctext%7Blength%7D_%7BW%28k%29%7D+M%3D%5Ctext%7Blog%7D_p%7CM%5E%7BF%3D1%7D%7C) 。

Pf.  注意这与Fontaine在p-adic表示书中定义的 ![\varphi](https://www.zhihu.com/equation?tex=%5Cvarphi) -模概念类似，回忆Fontaine在特征p>0的域的情况时的证明（见[galoisrep.pdf](https://link.zhihu.com/?target=https%3A//www.math.u-psud.fr/~fontaine/galoisrep.pdf)）：
![](https://pic3.zhimg.com/v2-3db5770160d4fb8b57d02ec5beeaef62_b.jpg)![](https://pic4.zhimg.com/v2-d986fcfd3a146d3fb319a3b7e5b7d11f_b.jpg)![](https://pic3.zhimg.com/v2-8bf98b42cd2de0e32b5b6254f3efac56_b.jpg)![](https://pic2.zhimg.com/v2-e006fc4d9100397c401654286446fbf1_b.jpg)
我们的结果的类比是：
![](https://pic3.zhimg.com/v2-42e2d138e72bb8ac4f32917e5d43d942_b.jpg)
最终需要证明 ![\alpha_M](https://www.zhihu.com/equation?tex=%5Calpha_M) 是同构，关键的满射性将用到维数的比较：
![](https://pic2.zhimg.com/v2-caada4ae235c09c2d6a66a2b5ae77531_b.jpg)
这个引理是证明中关键一步，翻译一下即是说定义仿射空间自身的态射 ![\sigma: (X_j) \rightarrow (X_j^p -b_j)](https://www.zhihu.com/equation?tex=%5Csigma%3A+%28X_j%29+%5Crightarrow+%28X_j%5Ep+-b_j%29) ，那么任取一个dxd可逆矩阵B，解空间 ![Bx=\sigma(x)](https://www.zhihu.com/equation?tex=Bx%3D%5Csigma%28x%29) 作为 ![\Bbb F_p](https://www.zhihu.com/equation?tex=%5CBbb+F_p) 向量空间维数恰好是d，取其一组基也就是说存在可逆矩阵C，使得 ![BC=\sigma(C)](https://www.zhihu.com/equation?tex=BC%3D%5Csigma%28C%29) 。即考虑 ![G=GL_n](https://www.zhihu.com/equation?tex=G%3DGL_n) 与态射![\sigma: G \rightarrow G](https://www.zhihu.com/equation?tex=%5Csigma%3A+G+%5Crightarrow+G)，则 ![g \rightarrow \sigma(g)g^{-1}:G \rightarrow G](https://www.zhihu.com/equation?tex=g+%5Crightarrow+%5Csigma%28g%29g%5E%7B-1%7D%3AG+%5Crightarrow+G) 是满射（至少在代数闭域上）。

这件事其实有更一般的推广即[Lang's theorem - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Lang%2527s_theorem)，我们将看到它也是①证明中关键的一步（事实上还有别的许多应用，例如有限域上连通代数群的高阶上同调都消失）：
![](https://pic2.zhimg.com/v2-bf485a70126df59e7abcaf2a82455aad_b.jpg)
回到①的证明，首先 ![M=W_n(k),F=\sigma](https://www.zhihu.com/equation?tex=M%3DW_n%28k%29%2CF%3D%5Csigma) 时显然。我们只需要说明一般情况作为 ![\sigma](https://www.zhihu.com/equation?tex=%5Csigma) 模 M总同构于这种平凡情况的直和，想法是根据PID上有限生成模结构定理，我们可以先固定一个模同构 ![\phi: \bigoplus_iW_{n_i}(k) \cong M](https://www.zhihu.com/equation?tex=%5Cphi%3A+%5Cbigoplus_iW_%7Bn_i%7D%28k%29+%5Ccong+M) （仅仅作为模），然后令 ![G=\underline {Aut}_{W(k)}(M)](https://www.zhihu.com/equation?tex=G%3D%5Cunderline+%7BAut%7D_%7BW%28k%29%7D%28M%29)是k上连通代数群 （由于它是某个仿射空间概形 ![\underline{End}_{W(k)}M](https://www.zhihu.com/equation?tex=%5Cunderline%7BEnd%7D_%7BW%28k%29%7DM) 的开子概形，故仍不可约所以连通），给一个M的 ![\sigma](https://www.zhihu.com/equation?tex=%5Csigma) 线性同构相当于给一个线性同构 ![g \in G(k)](https://www.zhihu.com/equation?tex=g+%5Cin+G%28k%29) ，两个同构等价相当于左右分别twist ![\phi^{-1}, \sigma(\phi)](https://www.zhihu.com/equation?tex=%5Cphi%5E%7B-1%7D%2C+%5Csigma%28%5Cphi%29) ，根据上面的Lang定理这样twist会平凡，于是即证①。

②k代数闭时，上述函子是反等价

Pf: 注意p-平展交换群概形范畴此时等价于有限Abel p群范畴，函子M相当于 ![W(k) \otimes_{\Bbb Z_p} Hom(-, \Bbb Q_p/\Bbb Z_p)](https://www.zhihu.com/equation?tex=W%28k%29+%5Cotimes_%7B%5CBbb+Z_p%7D+Hom%28-%2C+%5CBbb+Q_p%2F%5CBbb+Z_p%29) 且 ![F=\sigma](https://www.zhihu.com/equation?tex=F%3D%5Csigma) ，①说明函子M的像essentially surjective，即证。

③对一般特征p的perfect field，也是范畴反等价。

Pf.通过取Galois不变量立得。
![](https://pic3.zhimg.com/v2-0a270461d3807dad88e7a0d80fb29a62_b.jpg)
通过对偶我们也完成了local-etale情形，对一般的G将其分裂成三部分用M作用再直和起来即完成M(G)，注意到任何有限长度左D模都唯一分解成三部分（Fitting lemma）
![](https://pic2.zhimg.com/v2-dd3e9a31eec7fd89bdf6068772073d1d_b.jpg)
另外注意到作为k向量空间有自然同构 ![T_{G/k} \cong (M(G)/FM(G))^{D}](https://www.zhihu.com/equation?tex=T_%7BG%2Fk%7D+%5Ccong+%28M%28G%29%2FFM%28G%29%29%5E%7BD%7D)

Pf. ![G^D=\underline{Hom}(G,\Bbb G_m)](https://www.zhihu.com/equation?tex=G%5ED%3D%5Cunderline%7BHom%7D%28G%2C%5CBbb+G_m%29) ，两边求切空间有 ![T_{G^D/k}\cong Hom(G, \Bbb G_a)=Hom(G,W_1)=Ker(V:M(G) \rightarrow M(G))=Coker(F: M(G) \rightarrow M(G))^D=(M(G)/FM(G))^D](https://www.zhihu.com/equation?tex=T_%7BG%5ED%2Fk%7D%5Ccong+Hom%28G%2C+%5CBbb+G_a%29%3DHom%28G%2CW_1%29%3DKer%28V%3AM%28G%29+%5Crightarrow+M%28G%29%29%3DCoker%28F%3A+M%28G%29+%5Crightarrow+M%28G%29%29%5ED%3D%28M%28G%29%2FFM%28G%29%29%5ED)

两边把G换成G的对偶即得结论。

（这说明关于D模可以立刻求出切空间维数）

于是我们完成了目标，总结如下：

考虑Dieudonne ring ![D=W(k)[F,V]/ \sim](https://www.zhihu.com/equation?tex=D%3DW%28k%29%5BF%2CV%5D%2F+%5Csim) 模掉关系 ![FV=VF=p,F \xi= \sigma(\xi)F, V \sigma(\xi)=\xi V](https://www.zhihu.com/equation?tex=FV%3DVF%3Dp%2CF+%5Cxi%3D+%5Csigma%28%5Cxi%29F%2C+V+%5Csigma%28%5Cxi%29%3D%5Cxi+V)

则上述函子M定义了Abelian范畴的反等价：

**{k上有限交换p群概形} **![\Leftrightarrow](https://www.zhihu.com/equation?tex=%5CLeftrightarrow)**  {有限**![W(k)](https://www.zhihu.com/equation?tex=W%28k%29) -**长度的左D模}**

**①**![\text{log}_p |G|=\text{length}_{W(k)} M(G)](https://www.zhihu.com/equation?tex=%5Ctext%7Blog%7D_p+%7CG%7C%3D%5Ctext%7Blength%7D_%7BW%28k%29%7D+M%28G%29)

**②**![M(G^D)=M(G)^D](https://www.zhihu.com/equation?tex=M%28G%5ED%29%3DM%28G%29%5ED)

**③**![M(\alpha_p)=(k,F=0,V=0)](https://www.zhihu.com/equation?tex=M%28%5Calpha_p%29%3D%28k%2CF%3D0%2CV%3D0%29) , ![M(\Bbb Z/p \Bbb Z)= (k,F=\sigma,V=0)](https://www.zhihu.com/equation?tex=M%28%5CBbb+Z%2Fp+%5CBbb+Z%29%3D+%28k%2CF%3D%5Csigma%2CV%3D0%29) ,![M(\mu_p)=(k,F=0,V=\sigma^{-1})](https://www.zhihu.com/equation?tex=M%28%5Cmu_p%29%3D%28k%2CF%3D0%2CV%3D%5Csigma%5E%7B-1%7D%29)

**④ **![T_{G/k} \cong (M(G)/FM(G))^{D}](https://www.zhihu.com/equation?tex=T_%7BG%2Fk%7D+%5Ccong+%28M%28G%29%2FFM%28G%29%29%5E%7BD%7D)

**⑤ ![M](https://www.zhihu.com/equation?tex=M) 与基域的基变换交换**

**⑥ ![M(W_n^m)=D^m_n](https://www.zhihu.com/equation?tex=M%28W_n%5Em%29%3DD%5Em_n)**

右侧范畴可自然考虑更一般的对象：有限生成而不仅仅是有限长度，一个方法是取极限，而左侧对象取极限就得到了p-可除群，这也可看成引入p-可除群的动机之一（在M正合的证明中我们也看到，取极限确实让事情变简单）

(更新速度绝赞指数下降中....)

