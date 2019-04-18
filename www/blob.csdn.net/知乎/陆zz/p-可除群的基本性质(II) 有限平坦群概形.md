# p-可除群的基本性质(II):有限平坦群概形 - 知乎
# 

希望通过p-可除群理解Abel簇的挠点的结构，而p-可除群是一些有限平坦群概形的极限，为此需要发展有限平坦群概形的理论。这里我们的讨论包括一般的底概形S，这是因为处理Abel簇的约化问题时要考虑 ![\Bbb Z_p](https://www.zhihu.com/equation?tex=%5CBbb+Z_p) 上的群概形和它在 ![\Bbb Q_p](https://www.zhihu.com/equation?tex=%5CBbb+Q_p) 上的基变换的关系。而为了处理p-可除群和Abel簇的联系时，我们应当把Abel簇上的操作（取对偶、取Tate模）翻译到p-可除群上，为此需要建立有限交换平坦群概形的Cartier对偶理论。例如p-可除群也有Tate猜想（ 对特征0完备离散赋值环上p-可除群有同构![Hom(G,H) \rightarrow Hom_{\Bbb Z [\Gamma]}(TG,TH)](https://www.zhihu.com/equation?tex=Hom%28G%2CH%29+%5Crightarrow+Hom_%7B%5CBbb+Z+%5B%5CGamma%5D%7D%28TG%2CTH%29) ），其证明对有好约化的Abel簇的Tate猜想是有一定帮助的。

参考：《FINITE FLAT GROUP SCHEMES》 by J. Tate  （并补充一些例子和细节）

## 二. 有限平坦群概形初探

固定概形S，考虑S上概形构成范畴 ![Sch/S](https://www.zhihu.com/equation?tex=Sch%2FS) ，其中群对象称为S上群概形，于是一个群概形G配有S态射![m: G \times_S G \rightarrow G, e:S \rightarrow G, i: G \rightarrow G](https://www.zhihu.com/equation?tex=m%3A+G+%5Ctimes_S+G+%5Crightarrow+G%2C+e%3AS+%5Crightarrow+G%2C+i%3A+G+%5Crightarrow+G) 作为乘法、单位元、取逆。等价地，S上群概形是一个可表的反变函子 ![Sch/S \rightarrow \text{Group}](https://www.zhihu.com/equation?tex=Sch%2FS+%5Crightarrow+%5Ctext%7BGroup%7D) ，易见群概形在基变换下保持，两个群概形的fiber product也是群概形。任给一群同态 ![f: G\rightarrow H](https://www.zhihu.com/equation?tex=f%3A+G%5Crightarrow+H) ，把它看成函子的自然变换 ![F_G \rightarrow F_H](https://www.zhihu.com/equation?tex=F_G+%5Crightarrow+F_H) ，作为函子的Kernel定义为 ![T \rightarrow Ker (G(T) \rightarrow H(T))](https://www.zhihu.com/equation?tex=T+%5Crightarrow+Ker+%28G%28T%29+%5Crightarrow+H%28T%29%29) ，其也是群概形（由 ![G\times_{H}S](https://www.zhihu.com/equation?tex=G%5Ctimes_%7BH%7DS) 表出， 其中![e_H: S \rightarrow H, f: G \rightarrow H](https://www.zhihu.com/equation?tex=e_H%3A+S+%5Crightarrow+H%2C+f%3A+G+%5Crightarrow+H) ）

例子：(1) ![\Bbb G_a, \Bbb G_m, GL_n, \mu_n](https://www.zhihu.com/equation?tex=%5CBbb+G_a%2C+%5CBbb+G_m%2C+GL_n%2C+%5Cmu_n)

(2)对任何有限群G，定义 ![Sch/S ](https://www.zhihu.com/equation?tex=Sch%2FS+) 上常数层（fppf拓扑下）：T映为T的连通分支数个G的乘积，其为 ![Sch/S \rightarrow \text{Group}](https://www.zhihu.com/equation?tex=Sch%2FS+%5Crightarrow+%5Ctext%7BGroup%7D) 的函子，由![ \bigsqcup_{g \in G} S](https://www.zhihu.com/equation?tex=+%5Cbigsqcup_%7Bg+%5Cin+G%7D+S) 表出，对应群概形仍记为G

习题：定义群概形的半直积

> 定义.  群概形 ![G ](https://www.zhihu.com/equation?tex=G+) 称为有限/平坦/平展/，若结构映射 ![G \rightarrow S](https://www.zhihu.com/equation?tex=G+%5Crightarrow+S) 具有对应性质。

回忆：[Finite morphism - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Finite_morphism)，[Flat morphism - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Flat_morphism)，[Étale morphism - Wikipedia](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/%25C3%2589tale_morphism)

下面总假设 ![S=\text{Spec}R](https://www.zhihu.com/equation?tex=S%3D%5Ctext%7BSpec%7DR) 仿射，G是有限平坦群概形，R诺特环（下面使用到诺特条件时会特别说明）。此时 ![A=O_G(G)](https://www.zhihu.com/equation?tex=A%3DO_G%28G%29) 是有限生成局部自由R模，其秩称为G的阶，记为 ![|G|](https://www.zhihu.com/equation?tex=%7CG%7C) 。将G的运算翻译到A上，其成为一个Hopf代数（配有R代数同态![m: A \rightarrow A \otimes_RA, e:A \rightarrow R, i: A \rightarrow A](https://www.zhihu.com/equation?tex=m%3A+A+%5Crightarrow+A+%5Cotimes_RA%2C+e%3AA+%5Crightarrow+R%2C+i%3A+A+%5Crightarrow+A) ），这些运算满足对应的交换图，如 ![(R \rightarrow A \overset{e}\rightarrow R) =id](https://www.zhihu.com/equation?tex=%28R+%5Crightarrow+A+%5Coverset%7Be%7D%5Crightarrow+R%29+%3Did) 。 定义![I=Ker(A \overset{e}\rightarrow R)](https://www.zhihu.com/equation?tex=I%3DKer%28A+%5Coverset%7Be%7D%5Crightarrow+R%29) 为配值理想，注意这是一个投射R模。

习题：利用上述性质，说明①![m(x)-x \otimes 1 -1 \otimes x \in I \otimes I, \forall x \in I](https://www.zhihu.com/equation?tex=m%28x%29-x+%5Cotimes+1+-1+%5Cotimes+x+%5Cin+I+%5Cotimes+I%2C+%5Cforall+x+%5Cin+I) ②|G|在基变换下不变。

例子：

(1) |G|=1时，局部化知 ![I=0](https://www.zhihu.com/equation?tex=I%3D0) ， ![G=S](https://www.zhihu.com/equation?tex=G%3DS) ，乘法运算等为恒等映射

(2) |G|=2时，I是秩1投射R模。假设I自由，x是一个生成元，由于I是理想因此可设 ![x^2=ax, a \in R](https://www.zhihu.com/equation?tex=x%5E2%3Dax%2C+a+%5Cin+R) 。则 ![A=R1 \oplus I](https://www.zhihu.com/equation?tex=A%3DR1+%5Coplus+I) 作为R代数唯一由a决定。群结构由乘法m唯一决定，由习题①知可设 ![m(x)-x \otimes 1 -1 \otimes x=b(x \otimes x), b \in R](https://www.zhihu.com/equation?tex=m%28x%29-x+%5Cotimes+1+-1+%5Cotimes+x%3Db%28x+%5Cotimes+x%29%2C+b+%5Cin+R) ，代入知m为R代数同态的充分必要条件为 ![(ab+1)(ab+2)=0](https://www.zhihu.com/equation?tex=%28ab%2B1%29%28ab%2B2%29%3D0) 。此时G对应的函子是 ![T \mapsto G(T)=\{t \in T| t^2=at\}](https://www.zhihu.com/equation?tex=T+%5Cmapsto+G%28T%29%3D%5C%7Bt+%5Cin+T%7C+t%5E2%3Dat%5C%7D) ，乘法运算定义为 ![t_1\circ t_2 :=t_1+t_2+bt_1t_2, t_i \in G(T)](https://www.zhihu.com/equation?tex=t_1%5Ccirc+t_2+%3A%3Dt_1%2Bt_2%2Bbt_1t_2%2C+t_i+%5Cin+G%28T%29) ，特别地G一定交换（注意到一般情况可把S用一些仿射开集覆盖使得I限制在其上自由，利用基变换仍知G交换）。注意a，b满足的条件等价于e=ab+2满足e^2=e，于是e为幂等元，这给出 分解![S=S_1 \bigsqcup S_2](https://www.zhihu.com/equation?tex=S%3DS_1+%5Cbigsqcup+S_2) ，故只需处理 ![ab+1=0](https://www.zhihu.com/equation?tex=ab%2B1%3D0) ， ![ab+2=0](https://www.zhihu.com/equation?tex=ab%2B2%3D0) 的情况。检查知 ![ab=-1](https://www.zhihu.com/equation?tex=ab%3D-1) 时G只为半群而非群，故必须 ![ab+2=0](https://www.zhihu.com/equation?tex=ab%2B2%3D0) ，此时得到群概形记为 ![G_{a,b}](https://www.zhihu.com/equation?tex=G_%7Ba%2Cb%7D) 。

习题：验证 ![G_{-2,1} \cong \mu_2, G_{1,-2} \cong \Bbb Z/2\Bbb Z](https://www.zhihu.com/equation?tex=G_%7B-2%2C1%7D+%5Ccong+%5Cmu_2%2C+G_%7B1%2C-2%7D+%5Ccong+%5CBbb+Z%2F2%5CBbb+Z) ， ![G_{a,b} \cong G_{c,d} \Leftrightarrow \exists u\in R^{\times}, a=cu, b=du^{-1}](https://www.zhihu.com/equation?tex=G_%7Ba%2Cb%7D+%5Ccong+G_%7Bc%2Cd%7D+%5CLeftrightarrow+%5Cexists+u%5Cin+R%5E%7B%5Ctimes%7D%2C+a%3Dcu%2C+b%3Ddu%5E%7B-1%7D)

于是我们得到

**定理 1. 设p=2，则R上p阶有限平坦群概形都是交换的。进一步假设R是主理想整环，则R上p阶有限平坦群概形与 ![\{(a,b) \in R^{\times} |ab+2=0\}/ R^{\times}](https://www.zhihu.com/equation?tex=%5C%7B%28a%2Cb%29+%5Cin+R%5E%7B%5Ctimes%7D+%7Cab%2B2%3D0%5C%7D%2F+R%5E%7B%5Ctimes%7D) 一一对应。**

例子：R是 ![\Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z) 或特征非2的域，则只有 ![\mu_2, \Bbb Z/2\Bbb Z](https://www.zhihu.com/equation?tex=%5Cmu_2%2C+%5CBbb+Z%2F2%5CBbb+Z) ；R是特征为2的域，则有 ![\mu_2, \Bbb Z/2 \Bbb Z, \alpha_2= \text{Spec} \Bbb F_2[x]/(x^2) \times S \cong G_{0,0}](https://www.zhihu.com/equation?tex=%5Cmu_2%2C+%5CBbb+Z%2F2+%5CBbb+Z%2C+%5Calpha_2%3D+%5Ctext%7BSpec%7D+%5CBbb+F_2%5Bx%5D%2F%28x%5E2%29+%5Ctimes+S+%5Ccong+G_%7B0%2C0%7D) 三种

上面的讨论给人一种感觉，**特征p的域上有限p群概形的理论更丰富。**

类比有限群自由、可容许作用在概形上（可容许即每一个轨道都含于某个仿射开集）可以良好定义有限群的商，对带有有限群概形的自由可容许作用也可以定义商，这里略去细节。特别地，对于有限平坦闭子群概形 ![H \subseteq G](https://www.zhihu.com/equation?tex=H+%5Csubseteq+G) ，可以定义S上有限平坦的商概形 ![G/H](https://www.zhihu.com/equation?tex=G%2FH) （这里有限平坦类似Artin-Tate lemma，需要诺特性），其秩称为H的指标，记为 ![[G:H]=\frac{|G|}{|H|}](https://www.zhihu.com/equation?tex=%5BG%3AH%5D%3D%5Cfrac%7B%7CG%7C%7D%7B%7CH%7C%7D) 。H正规(定义为H在G/H上左作用平凡)时G/H可配有有限群概形结构，有正合列 ![1 \rightarrow H \rightarrow G \rightarrow G/H \rightarrow 1](https://www.zhihu.com/equation?tex=1+%5Crightarrow+H+%5Crightarrow+G+%5Crightarrow+G%2FH+%5Crightarrow+1) （正合是指 ![G \rightarrow G/H](https://www.zhihu.com/equation?tex=G+%5Crightarrow+G%2FH) 为忠实平坦且Kernel为H）

注：对S上scheme T，只能得到 抽象群的正合列![1 \rightarrow H(T) \rightarrow G(T) \rightarrow G/H(T)](https://www.zhihu.com/equation?tex=1+%5Crightarrow+H%28T%29+%5Crightarrow+G%28T%29+%5Crightarrow+G%2FH%28T%29) 而最右边通常不正合；一种理解方式是将群概形看成函子，H和G都是fppf拓扑下的层，但Naive的quotient得到的预层并不是层，G/H实际上是它的层化（通过f.f descent），因此上述 ![1 \rightarrow H \rightarrow G \rightarrow G/H \rightarrow 1](https://www.zhihu.com/equation?tex=1+%5Crightarrow+H+%5Crightarrow+G+%5Crightarrow+G%2FH+%5Crightarrow+1) 应看成fppf sheaf (取值为群)的正合列，下同。

**一类有限平坦群概形是常值群概形G**（上面的例子(2)），不难看出G对应的函子 ![F_G](https://www.zhihu.com/equation?tex=F_G) 满足性质：任给S上kernel幂零闭浸入 ![T \rightarrow T'](https://www.zhihu.com/equation?tex=T+%5Crightarrow+T%27) (从而给出拓扑空间的同胚)， ![F_G(T')\rightarrow F_G(T)](https://www.zhihu.com/equation?tex=F_G%28T%27%29%5Crightarrow+F_G%28T%29) 是双射，因此根据平展的无穷小判别法，我们得到：

性质. G是S上有限平展群概形。

注：同理n与域k特征互素时，![\mu_n](https://www.zhihu.com/equation?tex=%5Cmu_n) 在k上是平展的，当 ![\mu_n \not \subseteq k](https://www.zhihu.com/equation?tex=%5Cmu_n+%5Cnot+%5Csubseteq+k) 时这给出平展但不常值的例子。

我们下一个目标是分类所有有限平展群概形，下设S连通。finite表明G to S是闭映射，flat表明 G to S是开映射，由S的连通性我们得到G to S是满射，故忠实平坦。由于G平坦，其平展等价于非分歧，根据Kahler微分的两个标准正合列， ![(R \rightarrow A \overset{e}\rightarrow R) =id](https://www.zhihu.com/equation?tex=%28R+%5Crightarrow+A+%5Coverset%7Be%7D%5Crightarrow+R%29+%3Did) 以及m的性质可知 ![\Omega^1_{G/S}=I/I^2 \otimes_R A](https://www.zhihu.com/equation?tex=%5COmega%5E1_%7BG%2FS%7D%3DI%2FI%5E2+%5Cotimes_R+A) ，再根据忠实平坦性和I的有限生成性（S诺特）我们得到：

**性质. G是S上有限平坦群概形，则G平展等价于 ![I=I^2](https://www.zhihu.com/equation?tex=I%3DI%5E2) 等价于 ![e: S \rightarrow G](https://www.zhihu.com/equation?tex=e%3A+S+%5Crightarrow+G) 是开浸入（注意e本身就是闭浸入）**

习题：证明 ![G \rightarrow S](https://www.zhihu.com/equation?tex=G+%5Crightarrow+S)  separated 当且仅当 ![e: S \rightarrow G](https://www.zhihu.com/equation?tex=e%3A+S+%5Crightarrow+G) 是闭浸入（由于有限态射一定仿射从而separated，故e本身确实是闭浸入；证明有一边是显然的，另一边注意到对角映射实际上是e的基变换）

另一个分类方式是Grothendieck的Galois理论：取S一个几何点 ![\eta](https://www.zhihu.com/equation?tex=%5Ceta) 定义平展基本群 ![\pi_1(S,\eta)](https://www.zhihu.com/equation?tex=%5Cpi_1%28S%2C%5Ceta%29) ，**定理2. {有限平展概形/S} 与 {带有![\pi_1(S,\eta)](https://www.zhihu.com/equation?tex=%5Cpi_1%28S%2C%5Ceta%29) 作用的有限集}范畴等价**

**推论：**（1）{有限平展群概形/S} 与{带有![\pi_1(S,\eta)](https://www.zhihu.com/equation?tex=%5Cpi_1%28S%2C%5Ceta%29) 作用的有限群}范畴等价（注意这里的作用都要求是群自同构，下同）；

（2）{有限平展交换群概形/S} 与 {![\pi_1(S,\eta)](https://www.zhihu.com/equation?tex=%5Cpi_1%28S%2C%5Ceta%29) 在有限交换群上的表示}范畴等价；

（3）假设 ![S=\text{Spec}R](https://www.zhihu.com/equation?tex=S%3D%5Ctext%7BSpec%7DR) ，R是代数闭域（更一般地，严格Hensel环），则S上任何有限平展群概形都是常值的。

（4）对任何域k上有限平展群概形G，存在有限Galois扩张L/k使得G_L是常值的。

于是我们得到有限平展群概形的分类。现在希望考虑一般群概形，这必然要对S加好的假设（也是实际中会用到的情况，下面某些结果可通过局部环的Hensel化过渡到任何S）

**假设. ![S=\text{Spec}R](https://www.zhihu.com/equation?tex=S%3D%5Ctext%7BSpec%7DR) ，R为Hensel局部环（特例是完备诺特局部环），m是R极大理想，k=R/m是剩余类域。**

此时 ![\pi_1(S,\eta)\cong Gal(k^s/k)](https://www.zhihu.com/equation?tex=%5Cpi_1%28S%2C%5Ceta%29%5Ccong+Gal%28k%5Es%2Fk%29) , {有限平展群概形/S}与{带有![\pi_1(S,\eta)](https://www.zhihu.com/equation?tex=%5Cpi_1%28S%2C%5Ceta%29) 作用的有限群}范畴的具体等价函子是 ![G \rightarrow G(\overline k)](https://www.zhihu.com/equation?tex=G+%5Crightarrow+G%28%5Coverline+k%29) 。

回忆Hensel局部环的刻画性质是其上作为模有限的代数都是局部环的乘积，而完备离散赋值环的赋值可以向上唯一延拓成完备赋值。

推论：给局部环间的有限扩张 ![B_1 \rightarrow B_2](https://www.zhihu.com/equation?tex=B_1+%5Crightarrow+B_2) ，如果 ![B_1](https://www.zhihu.com/equation?tex=B_1) 是Hensel，则 ![B_2](https://www.zhihu.com/equation?tex=B_2) 也是

![e:S \rightarrow G](https://www.zhihu.com/equation?tex=e%3AS+%5Crightarrow+G) 的像（由于S连通，e为闭浸入，像为连通闭集）所在连通分支记为 ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 。则根据上面讨论， ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 是一个完备离散赋值环（Hensel局部环） ![A^0](https://www.zhihu.com/equation?tex=A%5E0) 的素谱( ![A^0](https://www.zhihu.com/equation?tex=A%5E0) 是 ![A](https://www.zhihu.com/equation?tex=A) 作为R代数分解成局部环乘积中的一项)，且 ![A^0](https://www.zhihu.com/equation?tex=A%5E0) 有限生成平坦R模（这里不需要诺特，因为 ![A^0](https://www.zhihu.com/equation?tex=A%5E0) 是直和因子）。注意局部环间平坦同态一定是局部同态，因此由 ![(R \rightarrow A \overset{e}\rightarrow R) =id](https://www.zhihu.com/equation?tex=%28R+%5Crightarrow+A+%5Coverset%7Be%7D%5Crightarrow+R%29+%3Did) 可知 ![A^0](https://www.zhihu.com/equation?tex=A%5E0) 的剩余类域与R相同。现在关键是说明 ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 是一个子群概形，即说明 ![G^0 \times_S G^0 \rightarrow G](https://www.zhihu.com/equation?tex=G%5E0+%5Ctimes_S+G%5E0+%5Crightarrow+G) 能通过 ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 分解。这等价于说明 ![G^0 \times_S G^0 ](https://www.zhihu.com/equation?tex=G%5E0+%5Ctimes_S+G%5E0+) 是连通的。由于Hensel局部环上作为模有限的代数的素谱连通分支和它的special fiber的连通分支有一一对应（从分解成局部环看出，也可以考虑用Hensel引理得到幂等元的唯一提升性），我们可以化归到R是一个域的情况。

回忆：对域k上有限型概形X，可定义0阶平展同伦群 ![\pi_0(X)](https://www.zhihu.com/equation?tex=%5Cpi_0%28X%29) 为k上有限平展概形， ![\pi_0(X)](https://www.zhihu.com/equation?tex=%5Cpi_0%28X%29) 作为集合与X的连通分支一一对应。![\pi_0](https://www.zhihu.com/equation?tex=%5Cpi_0) 为一与基域变换交换的函子，有自然的满射 ![X \rightarrow \pi_0(X)](https://www.zhihu.com/equation?tex=X+%5Crightarrow+%5Cpi_0%28X%29) 。

Pf: 参考J.S Milne的代数群，或见unpublished note by [???]

推论1：X几何连通（即 ![X_Y:=X \times_k Y](https://www.zhihu.com/equation?tex=X_Y%3A%3DX+%5Ctimes_k+Y) 连通对所有Y）当且仅当 ![\pi_0(X)=\text{Spec}k](https://www.zhihu.com/equation?tex=%5Cpi_0%28X%29%3D%5Ctext%7BSpec%7Dk)

推论2：假设X连通且有k有理点，则X几何连通

Pf: 1是由于可做基域变换假设可分闭；2是将 ![\pi_0](https://www.zhihu.com/equation?tex=%5Cpi_0) 函子作用到Spec k to X to Spec k上。

推论2的另一个证明见[Tag 0361: Geometrically connected schemes](https://link.zhihu.com/?target=https%3A//stacks.math.columbia.edu/tag/0361)，作为应用我们得到 ![G^0 \times_S G^0 ](https://www.zhihu.com/equation?tex=G%5E0+%5Ctimes_S+G%5E0+) 是连通的(由于 ![\pi_0(G^0)=\text{Spec} k](https://www.zhihu.com/equation?tex=%5Cpi_0%28G%5E0%29%3D%5Ctext%7BSpec%7D+k) )，并且对任何基变换有 ![(G^0)_{S^{'}}=(G_{S'})^0](https://www.zhihu.com/equation?tex=%28G%5E0%29_%7BS%5E%7B%27%7D%7D%3D%28G_%7BS%27%7D%29%5E0) 。因此 ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 是G的有限正规平坦子概形（正规性容易从共轭作用保持S看出），不难看出R是域时 ![G/G^0=\pi_0(G)](https://www.zhihu.com/equation?tex=G%2FG%5E0%3D%5Cpi_0%28G%29) ：之前的性质表明有限平坦概形是平展的当且仅当S的像是开且闭的即 ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 就是S，由此可知 ![G/G^0](https://www.zhihu.com/equation?tex=G%2FG%5E0) 平展，并且G到有限平展群概形的同态都通过 ![G/G^0](https://www.zhihu.com/equation?tex=G%2FG%5E0) ，再注意到也通过 ![\pi_0(G)](https://www.zhihu.com/equation?tex=%5Cpi_0%28G%29) (函子性)。一般情况定义 ![\pi_0(G)=G/G^0](https://www.zhihu.com/equation?tex=%5Cpi_0%28G%29%3DG%2FG%5E0) （作为概形而非仅仅集合），于是我们得到

**定理3**

**S满足之前的假设，则任何S上有限平坦群概形G都可分解成连通和平展的部分。更精确来说，有对G具有函子性的短正合列（称为连通-平展正合列）：**

![1 \rightarrow G^0 \rightarrow G \rightarrow G^{et} \rightarrow 1](https://www.zhihu.com/equation?tex=1+%5Crightarrow+G%5E0+%5Crightarrow+G+%5Crightarrow+G%5E%7Bet%7D+%5Crightarrow+1)

**其中 ![G^{et}=\pi_0(G)](https://www.zhihu.com/equation?tex=G%5E%7Bet%7D%3D%5Cpi_0%28G%29) 是有限平展群概形， ![G^0](https://www.zhihu.com/equation?tex=G%5E0) 是有限连通群概形且整体截面是剩余类域为k的局部环。**

习题：证明 ![G^0, \pi_0(G)](https://www.zhihu.com/equation?tex=G%5E0%2C+%5Cpi_0%28G%29) 都是正合函子。

注： 可能存在![(G^0)_T \not= (G_T)^0](https://www.zhihu.com/equation?tex=%28G%5E0%29_T+%5Cnot%3D+%28G_T%29%5E0) 的情况（对基变换S to T），一个例子是 ![\Bbb Z_p \rightarrow \Bbb Q_p](https://www.zhihu.com/equation?tex=%5CBbb+Z_p+%5Crightarrow+%5CBbb+Q_p) , ![G=\mu_p](https://www.zhihu.com/equation?tex=G%3D%5Cmu_p) ：由于 ![\Bbb Z_p[x]/(x^p-1)](https://www.zhihu.com/equation?tex=%5CBbb+Z_p%5Bx%5D%2F%28x%5Ep-1%29) 是局部环，故 ![G^0=G](https://www.zhihu.com/equation?tex=G%5E0%3DG) ， ![(G^0)_{\Bbb Q_p}=\mu_p](https://www.zhihu.com/equation?tex=%28G%5E0%29_%7B%5CBbb+Q_p%7D%3D%5Cmu_p) 但 ![(G_{\Bbb Q_p})^0=\text{Spec} \Bbb Q_p](https://www.zhihu.com/equation?tex=%28G_%7B%5CBbb+Q_p%7D%29%5E0%3D%5Ctext%7BSpec%7D+%5CBbb+Q_p) 平凡（特征0域上有限群概形都平展）

习题：假设 ![S \rightarrow T](https://www.zhihu.com/equation?tex=S+%5Crightarrow+T) 是局部同态且T满足假设，证明 ![G^0, G^{et}](https://www.zhihu.com/equation?tex=G%5E0%2C+G%5E%7Bet%7D) 及上述正合列与S到T的基变换相容。（注意Hensel局部环上有限概形的连通分支与其special fiber的一样）

注：我们的证明和Tate的原始证明有一些不同，一个反例表明上面的论证并非显然：我们来考察域上群概形G， ![G_{red}](https://www.zhihu.com/equation?tex=G_%7Bred%7D) 上是否也有子群概形结构？结论一般来说是否定的，但k perfect情况是正确的（后面的讨论可见，perfect条件下可以得到很多好结果）。即使如此， ![G_{red}](https://www.zhihu.com/equation?tex=G_%7Bred%7D) 也未必正规，见[Is there a connected $k$-group scheme $G$ such that $G_{red}$ is not a subgroup?](https://link.zhihu.com/?target=https%3A//mathoverflow.net/questions/38891/is-there-a-connected-k-group-scheme-g-such-that-g-red-is-not-a-subgroup)

因此次转而考虑有限连通群概形的分类，上面例子中连通的仅有![\alpha_p= \text{Spec} \Bbb F_p[x]/x^p](https://www.zhihu.com/equation?tex=%5Calpha_p%3D+%5Ctext%7BSpec%7D+%5CBbb+F_p%5Bx%5D%2Fx%5Ep) ，其生活在正特征中且阶是p的幂次，一般地我们有：

**定理4. **

**S满足之前假设，则**

**（1）若k的特征是0，则S上有限平坦群概形均平展**

**（2）若k的特征是p>0，则 ![|G^0|](https://www.zhihu.com/equation?tex=%7CG%5E0%7C) 是p的幂。**

Pf (Tate)：根据之前正合列不妨设G连通，根据Nakayama Lemma不妨设R=k是域。则 ![G=\text{Spec} A](https://www.zhihu.com/equation?tex=G%3D%5Ctext%7BSpec%7D+A) ，A是k上有限维局部代数故Artin，剩余类域为k， ![I=Ker(A \rightarrow k)](https://www.zhihu.com/equation?tex=I%3DKer%28A+%5Crightarrow+k%29) 是极大理想且幂零。取 ![\tilde{x}_i \in A, i=1,…,r](https://www.zhihu.com/equation?tex=%5Ctilde%7Bx%7D_i+%5Cin+A%2C+i%3D1%2C%E2%80%A6%2Cr) 使得 mod I后构成 ![I/I^2](https://www.zhihu.com/equation?tex=I%2FI%5E2) 一组k基。则可定义自然的k代数同态： ![\phi : x_i \mapsto \tilde{x_i} \text{mod} I \ k[X]:=k[x_i] \rightarrow Gr_I(A):=\bigoplus_{n=0}^{\infty} I^n/I^{n+1} ](https://www.zhihu.com/equation?tex=%5Cphi+%3A+x_i+%5Cmapsto+%5Ctilde%7Bx_i%7D+%5Ctext%7Bmod%7D+I+%5C+k%5BX%5D%3A%3Dk%5Bx_i%5D+%5Crightarrow+Gr_I%28A%29%3A%3D%5Cbigoplus_%7Bn%3D0%7D%5E%7B%5Cinfty%7D+I%5En%2FI%5E%7Bn%2B1%7D+) ，由于I幂零，其为满射。

之前已证明Kahler微分 ![\Omega_{A/R}=I/I^2 \otimes_R A](https://www.zhihu.com/equation?tex=%5COmega_%7BA%2FR%7D%3DI%2FI%5E2+%5Cotimes_R+A) ，万有导子 ![A \rightarrow I/I^2 \otimes_R A](https://www.zhihu.com/equation?tex=A+%5Crightarrow+I%2FI%5E2+%5Cotimes_R+A)为![d=(\pi \otimes id) \circ m](https://www.zhihu.com/equation?tex=d%3D%28%5Cpi+%5Cotimes+id%29+%5Ccirc+m)其中 ![\pi](https://www.zhihu.com/equation?tex=%5Cpi)为 ![I \hookrightarrow A](https://www.zhihu.com/equation?tex=I+%5Chookrightarrow+A)的截面 mod ![ I^2](https://www.zhihu.com/equation?tex=+I%5E2) 得到，因此![Der_R(A,A)=Hom_A(\Omega_{A/R},A)=Hom_R(I/I^2,A)](https://www.zhihu.com/equation?tex=Der_R%28A%2CA%29%3DHom_A%28%5COmega_%7BA%2FR%7D%2CA%29%3DHom_R%28I%2FI%5E2%2CA%29) ，结果是存在导子![D_i: A \rightarrow A](https://www.zhihu.com/equation?tex=D_i%3A+A+%5Crightarrow+A) 使得  ![D_i|_{I/I^2}: \tilde{x_j} \mapsto \delta_{ij}](https://www.zhihu.com/equation?tex=D_i%7C_%7BI%2FI%5E2%7D%3A+%5Ctilde%7Bx_j%7D+%5Cmapsto+%5Cdelta_%7Bij%7D) ，根据Leibiniz法则![D_i I^{n+1} \subseteq I^n](https://www.zhihu.com/equation?tex=D_i+I%5E%7Bn%2B1%7D+%5Csubseteq+I%5En) 故 ![D_i](https://www.zhihu.com/equation?tex=D_i) 进一步诱导 ![Gr_I(A)](https://www.zhihu.com/equation?tex=Gr_I%28A%29)上次数-1的导子。有 ![D_i \circ \phi = \phi \circ \frac{\partial}{\partial x_i}](https://www.zhihu.com/equation?tex=D_i+%5Ccirc+%5Cphi+%3D+%5Cphi+%5Ccirc+%5Cfrac%7B%5Cpartial%7D%7B%5Cpartial+x_i%7D) （只需在基上验证），由此可知齐次真理想 ![Ker\phi](https://www.zhihu.com/equation?tex=Ker%5Cphi) 对求偏导稳定，这迫使其为0（若特征0），同理考虑特征p情况有

**性质. k特征0，则 ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) 为同构；k特征p>0，则有![\phi: k[X]/(x_1^p,…,x_r^p) \cong Gr_I(A)/(\tilde{x_1}^p,…,\tilde{x_r}^p)](https://www.zhihu.com/equation?tex=%5Cphi%3A+k%5BX%5D%2F%28x_1%5Ep%2C%E2%80%A6%2Cx_r%5Ep%29+%5Ccong+Gr_I%28A%29%2F%28%5Ctilde%7Bx_1%7D%5Ep%2C%E2%80%A6%2C%5Ctilde%7Bx_r%7D%5Ep%29)**

但注意到 ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) 的像是有限维k向量空间（I幂零），特征0时迫使r=0，于是I=0，A=k，这就证明了（1）。若特征p>0，注意A是由 ![\tilde{x_i}](https://www.zhihu.com/equation?tex=%5Ctilde%7Bx_i%7D) 生成的k代数， ![Gr_I(A/(x_i^p))=Gr_I(A)/(\tilde{x_1}^p,…,\tilde{x_r}^p)](https://www.zhihu.com/equation?tex=Gr_I%28A%2F%28x_i%5Ep%29%29%3DGr_I%28A%29%2F%28%5Ctilde%7Bx_1%7D%5Ep%2C%E2%80%A6%2C%5Ctilde%7Bx_r%7D%5Ep%29) ，我们如果能够找到群概形表出 ![A/(\tilde {x_i}^p)](https://www.zhihu.com/equation?tex=A%2F%28%5Ctilde+%7Bx_i%7D%5Ep%29) ，便可使用归纳法，一个想法是考虑Frobenius.

回忆：![X](https://www.zhihu.com/equation?tex=X) 是k上有限型概形，

①定义 ![X \rightarrow X](https://www.zhihu.com/equation?tex=X+%5Crightarrow+X) 的绝对Frobenius![X \rightarrow X](https://www.zhihu.com/equation?tex=X+%5Crightarrow+X)![x \mapsto x^p, \forall x \in O_X(U), U\subseteq X](https://www.zhihu.com/equation?tex=x+%5Cmapsto+x%5Ep%2C+%5Cforall+x+%5Cin+O_X%28U%29%2C+U%5Csubseteq+X)

②![X^{(p)}](https://www.zhihu.com/equation?tex=X%5E%7B%28p%29%7D) 为X通过 ![ x \mapsto x^p: k \rightarrow k](https://www.zhihu.com/equation?tex=+x+%5Cmapsto+x%5Ep%3A+k+%5Crightarrow+k) 的基变换，得到算术Frobenius ![X^{(p)} \rightarrow X](https://www.zhihu.com/equation?tex=X%5E%7B%28p%29%7D+%5Crightarrow+X)

③根据fiber product的万有性，由绝对Frobenius得到相对Frobenius ![Fr: X \rightarrow X^{(p)}](https://www.zhihu.com/equation?tex=Fr%3A+X+%5Crightarrow+X%5E%7B%28p%29%7D)

④如果 ![X=\text{Spec}k[x_i]/(f_i)](https://www.zhihu.com/equation?tex=X%3D%5Ctext%7BSpec%7Dk%5Bx_i%5D%2F%28f_i%29) ，那么相对Frob可写成![k[x_i]/(f_i^{(p)}) \rightarrow k[x]/(f_i) : x_i \rightarrow x_i^p](https://www.zhihu.com/equation?tex=k%5Bx_i%5D%2F%28f_i%5E%7B%28p%29%7D%29+%5Crightarrow+k%5Bx%5D%2F%28f_i%29+%3A+x_i+%5Crightarrow+x_i%5Ep) ，其中 ![f_i^{(p)}](https://www.zhihu.com/equation?tex=f_i%5E%7B%28p%29%7D) 表示f的系数全变成p次方后得到的新多项式。

习题：若G是群概形，则 ![G^{(p)}](https://www.zhihu.com/equation?tex=G%5E%7B%28p%29%7D) 也是且![Fr: G \rightarrow G^{(p)}](https://www.zhihu.com/equation?tex=Fr%3A+G+%5Crightarrow+G%5E%7B%28p%29%7D) 是群同态。（可看成函子验证）

根据这些讨论， ![H=Ker(Fr)](https://www.zhihu.com/equation?tex=H%3DKer%28Fr%29) 就是G的有限平坦正规子概形（域上平坦自动，Kernel自动正规子群），其整体截面环就是 ![A/(\tilde {x_i}^p)](https://www.zhihu.com/equation?tex=A%2F%28%5Ctilde+%7Bx_i%7D%5Ep%29) ，所以由上面性质得到![|H|=dim_k A/(\tilde{x_i}^p)=dim_k Gr_I(A/(\tilde{x_i}^p))\overset{\phi}=dim_k k[X]/(x_i^p)=p^r](https://www.zhihu.com/equation?tex=%7CH%7C%3Ddim_k+A%2F%28%5Ctilde%7Bx_i%7D%5Ep%29%3Ddim_k+Gr_I%28A%2F%28%5Ctilde%7Bx_i%7D%5Ep%29%29%5Coverset%7B%5Cphi%7D%3Ddim_k+k%5BX%5D%2F%28x_i%5Ep%29%3Dp%5Er)

r=0的情况平凡，r>0时对|G|的阶归纳，我们便证明了（2），即证。

对整数n，通过对角嵌入可以自然定义自乘n次运算 ![[n]: G \rightarrow G](https://www.zhihu.com/equation?tex=%5Bn%5D%3A+G+%5Crightarrow+G) ，或者直接通过函子定义。下一个定理是关于Langrange定理的推广：

**定理5.**

**对域上有限群概形G，**![[|G|]=id](https://www.zhihu.com/equation?tex=%5B%7CG%7C%5D%3Did)**。对任何S上有限局部自由（即平坦+有限表现）交换群概形，也有**![[|G|]=id](https://www.zhihu.com/equation?tex=%5B%7CG%7C%5D%3Did) 。

虽然这个定理看起来很简单，但注意到可能存在连通群概形这一不平凡情况，域上具体证明是根据短正合列划归到连通情况（平展时先基变换使其平凡，再利用f.f descent回归到原来域上），然后证明配置理想I满足 ![[p]I \subseteq I^p](https://www.zhihu.com/equation?tex=%5Bp%5DI+%5Csubseteq+I%5Ep) （这一步需要先利用正则表示将G嵌入GL_n，然后利用GL_n的余乘法的具体公式进行计算），再注意到I幂零即得，具体略。（任何S+G交换的情况则可用下面提到的Cartier对偶理论证明）

最后我们关心连通-平展正合列是否splits，回忆：

（generic smoothness）设k是perfect field，则k上separated+reduced+finite type的概形一定有光滑点。

于是我们得到

**性质.**设k是perfect field，则k上有限型群概形G如果reduced，那么就是平展的

Pf. 首先说明G光滑，根据f.f descent可假设k代数闭（注意perfect field上reduced+有有理点推出几何reduced），由于generic smoothness得到G有光滑闭点，又G在自身左乘作用限制在闭点集上可迁，所以G的闭点都光滑，于是G光滑（这里用到Jacobson概形的性质：任何开集都含闭点）。由于光滑+相对维数0等于平展，即证。（有限态射的相对维数是0）

**定理 6.**

**perfect field上连通-平展正合列分裂：**

![1 \rightarrow G^0 \rightarrow G \rightarrow G^{et} \rightarrow 1](https://www.zhihu.com/equation?tex=1+%5Crightarrow+G%5E0+%5Crightarrow+G+%5Crightarrow+G%5E%7Bet%7D+%5Crightarrow+1)

因此![G \cong G^0 \rtimes G_{red}](https://www.zhihu.com/equation?tex=G+%5Ccong+G%5E0+%5Crtimes+G_%7Bred%7D) （特别地，如果G交换则为乘积）

Pf. 考虑同胚![G_{red} \rightarrow G](https://www.zhihu.com/equation?tex=G_%7Bred%7D+%5Crightarrow+G) ，根据假设 ![G_{red}](https://www.zhihu.com/equation?tex=G_%7Bred%7D) 平展且 ![G_{red} \times G_{red}](https://www.zhihu.com/equation?tex=G_%7Bred%7D+%5Ctimes+G_%7Bred%7D) 也reduced，于是 ![G_{red} \rightarrow G](https://www.zhihu.com/equation?tex=G_%7Bred%7D+%5Crightarrow+G) 是有限平展群子概形，再利用同胚可知 ![\pi_0(G_{red})=\pi_0(G)](https://www.zhihu.com/equation?tex=%5Cpi_0%28G_%7Bred%7D%29%3D%5Cpi_0%28G%29) ，故 ![G_{red} \rightarrow G \rightarrow G^{et}](https://www.zhihu.com/equation?tex=G_%7Bred%7D+%5Crightarrow+G+%5Crightarrow+G%5E%7Bet%7D) 是同构，给出一个section，即证。

利用 ![G_{red}](https://www.zhihu.com/equation?tex=G_%7Bred%7D) 的光滑性，Cartier将定理4的（1）推广为：

**定理 7. (Cartier)**

**k是特征0的域，则k上有限型仿射群概形都光滑。(特别地总reduced)**

Pf. 由f.f descent不妨k代数闭(![k=\Bbb C](https://www.zhihu.com/equation?tex=k%3D%5CBbb+C))，由齐次性只需说明单位元处局部环是正则局部环，即I/I^2维数是dimA。由上 ![G_{red}](https://www.zhihu.com/equation?tex=G_%7Bred%7D) 的光滑性知只用说明I/I^2到I/rad A / (I/radA)^2的自然映射是单射（总是满射），即说明A中幂零元一定属于I^2。设x是幂零元，一开始便说明了![m(x)-x \otimes 1 -1 \otimes x \in I \otimes I, \forall x \in I](https://www.zhihu.com/equation?tex=m%28x%29-x+%5Cotimes+1+-1+%5Cotimes+x+%5Cin+I+%5Cotimes+I%2C+%5Cforall+x+%5Cin+I) ，利用x^n=0，x^{n-1}非0对某个n，代入展开并 mod I^2(以及注意到n在k中可逆)即得x属于I^2，即证。

最后，和开头说的那样我们需要发展有限交换群的对偶理论。回忆对局部紧交换拓扑群G有 [Pontryagin duality](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Pontryagin_duality)![ {\displaystyle {\widehat {G}}}:=Hom_{cont} (G, S^1)](https://www.zhihu.com/equation?tex=+%7B%5Cdisplaystyle+%7B%5Cwidehat+%7BG%7D%7D%7D%3A%3DHom_%7Bcont%7D+%28G%2C+S%5E1%29) ，这是一个正合函子，并且有著名的

> The Pontryagin duality theorem
**Theorem.** There is a canonical isomorphism ![ {\displaystyle G\cong {\widehat {\widehat {G}}}}](https://www.zhihu.com/equation?tex=+%7B%5Cdisplaystyle+G%5Ccong+%7B%5Cwidehat+%7B%5Cwidehat+%7BG%7D%7D%7D%7D)  between any locally compact abelian group ![{\displaystyle G}](https://www.zhihu.com/equation?tex=%7B%5Cdisplaystyle+G%7D)  and its double dual.

这允许我们谈G上的Fourier分析，一些对偶的例子： ![\Bbb R/ \Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+R%2F+%5CBbb+Z) 的对偶是 ![\Bbb Z](https://www.zhihu.com/equation?tex=%5CBbb+Z) ，![\Bbb R, \Bbb Q_p, \Bbb A_{\Bbb Q}](https://www.zhihu.com/equation?tex=%5CBbb+R%2C+%5CBbb+Q_p%2C+%5CBbb+A_%7B%5CBbb+Q%7D) 自对偶……

回到我们的情形，设R是诺特环，M是有限生成平坦R模，自然定义M的对偶为 ![M'=Hom(M,R)](https://www.zhihu.com/equation?tex=M%27%3DHom%28M%2CR%29) ，我们有

习题.  ![M'](https://www.zhihu.com/equation?tex=M%27) 也是有限生成平坦R模，且有典范同构 ![M \cong M''](https://www.zhihu.com/equation?tex=M+%5Ccong+M%27%27) ，对偶与张量积交换

特别地，假设M=A是一个R上有限平坦交换群概形G的整体截面环，那么A上的乘法和余乘法会对偶地给出 ![A'](https://www.zhihu.com/equation?tex=A%27) 的余乘法和乘法，验证相应的交换图表，于是 ![G'=Spec A'](https://www.zhihu.com/equation?tex=G%27%3DSpec+A%27) 也是有限平坦交换群概形（G交换是为了 ![A'](https://www.zhihu.com/equation?tex=A%27) 交换于是可以定义素谱）。于是我们得到（细节可自行验证）

**定理 8. （R诺特）**

**{R上有限平坦交换群}范畴上存在一个对偶函子 ![G \mapsto G'](https://www.zhihu.com/equation?tex=G+%5Cmapsto+G%27) , 其给出范畴到自身的反等价，且有自然同构![G'' \cong G](https://www.zhihu.com/equation?tex=G%27%27+%5Ccong+G) ，并且该对偶与R的基变换交换。**

我们也用 ![G^D](https://www.zhihu.com/equation?tex=G%5ED) 表示G的对偶。

**习题（重要）**

验证上面对偶群概形作为函子是 ![T \rightarrow Hom_{Group}(G_T,(\Bbb G_m)_T)](https://www.zhihu.com/equation?tex=T+%5Crightarrow+Hom_%7BGroup%7D%28G_T%2C%28%5CBbb+G_m%29_T%29)  （根据基变换与对偶交换，不妨T=S，注意 ![G'(S)=Hom_{R-alg}(A',R) ](https://www.zhihu.com/equation?tex=G%27%28S%29%3DHom_%7BR-alg%7D%28A%27%2CR%29+) 可嵌入 ![Hom_R(A',R)=A](https://www.zhihu.com/equation?tex=Hom_R%28A%27%2CR%29%3DA) 并且这是乘法半群的同态，同理 ![Hom_{group}(G,\Bbb G_m) \hookrightarrow Hom_{R-alg}(R[x,x^{-1}],A)=A^{\times}](https://www.zhihu.com/equation?tex=Hom_%7Bgroup%7D%28G%2C%5CBbb+G_m%29+%5Chookrightarrow+Hom_%7BR-alg%7D%28R%5Bx%2Cx%5E%7B-1%7D%5D%2CA%29%3DA%5E%7B%5Ctimes%7D) ，然后验证余代数同态的条件我们有 ![Hom_{group}(G,\Bbb G_m)=\{a \in A^{\times}|m(a)=a\otimes a\}](https://www.zhihu.com/equation?tex=Hom_%7Bgroup%7D%28G%2C%5CBbb+G_m%29%3D%5C%7Ba+%5Cin+A%5E%7B%5Ctimes%7D%7Cm%28a%29%3Da%5Cotimes+a%5C%7D) ，同理验证前者发现也是如此）。于是对任何T，有自然的pairing ![G(T) \times G'(T)\rightarrow \Bbb G_m(T)](https://www.zhihu.com/equation?tex=G%28T%29+%5Ctimes+G%27%28T%29%5Crightarrow+%5CBbb+G_m%28T%29) 。

有了上面一些定理，可以说对于有限平坦群概形有了初步理解（例子、分类、性质），也可以得到一些有趣的推论例如：

① ![\Bbb C](https://www.zhihu.com/equation?tex=%5CBbb+C)上有限群概形只有G，G是有限群

② k是域，![F](https://www.zhihu.com/equation?tex=F)是{k上代数}到Group的可表函子，并且表示F的k代数是有限生成的。若 ![F(L)](https://www.zhihu.com/equation?tex=F%28L%29) 是有限集，则可证明表示F的群概形是有限平坦群概形。此时利用Langrange定理，我们知道存在正整数n使得对所有的k代数R，F(R)作为抽象群都满足 ![x^n=1, \forall x \in F(A)](https://www.zhihu.com/equation?tex=x%5En%3D1%2C+%5Cforall+x+%5Cin+F%28A%29) 。

③k是域且n在k中可逆，则 ![\mu_n^D=\Bbb Z/n \Bbb Z, (\Bbb Z/n \Bbb Z)^D=\mu_n](https://www.zhihu.com/equation?tex=%5Cmu_n%5ED%3D%5CBbb+Z%2Fn+%5CBbb+Z%2C+%28%5CBbb+Z%2Fn+%5CBbb+Z%29%5ED%3D%5Cmu_n) ，自然的pairing是单位根的若干次方

OK，时间不多了，在此打住。下一次我们将讨论一般底概形上的Abel概形以及相关的有限平坦群概形内容，当然会讨论一些例子~

注：一个开问题是Lagrange定理对任何概形S上的有限局部自由群概形是否成立？

注：没想到挺多人对这个topic感兴趣，有更多讨论的话会努力继续更新~

