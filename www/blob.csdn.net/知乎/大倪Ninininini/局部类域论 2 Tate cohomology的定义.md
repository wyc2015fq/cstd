# 局部类域论 2 Tate cohomology的定义 - 知乎
# 

这一节我们复习homology group和Tate cohomology的定义。

上次的笔记提到：研究G ![\approx](https://www.zhihu.com/equation?tex=%5Capprox) 研究 ![H^{r}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CM%29) 。我们一边复习概念，一边想着大概怎么实现这个principle。
- **首先，我们看上同调群（特别地，我们暂时只考虑 **![H^{1}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29)** ）可以带来哪些G的信息。**
- **接着，我们回忆同调群 **![H_{r}(G,M)](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CM%29)** 的定义，看它可给出哪些G的信息。**
- **最后，给出Tate cohomology的定义，将 **![H^{r}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CM%29)** 和 **![H_{r}(G,M)](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CM%29)** “连接“（"very" long exact sequence）起来。**
- **Information from**![H^{1}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29)

**[1].** 首先根据上次给出的 ![H^{r}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CM%29) 的description来计算 ![H^{1}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29) ：

![(d^{r}\varphi)(g_{1},...,g_{r+1})=g_{1}\varphi(g_{2},...,g_{r+1})+\sum_{j=0}^{i}(-1)^{j}\varphi(g_{1},g_{2},...,g_{j}g_{j+1},...,g_{r+1})\\+(-1)^{r+1}\varphi(g_{1},g_{2},...,g_{r})](https://www.zhihu.com/equation?tex=%28d%5E%7Br%7D%5Cvarphi%29%28g_%7B1%7D%2C...%2Cg_%7Br%2B1%7D%29%3Dg_%7B1%7D%5Cvarphi%28g_%7B2%7D%2C...%2Cg_%7Br%2B1%7D%29%2B%5Csum_%7Bj%3D0%7D%5E%7Bi%7D%28-1%29%5E%7Bj%7D%5Cvarphi%28g_%7B1%7D%2Cg_%7B2%7D%2C...%2Cg_%7Bj%7Dg_%7Bj%2B1%7D%2C...%2Cg_%7Br%2B1%7D%29%5C%5C%2B%28-1%29%5E%7Br%2B1%7D%5Cvarphi%28g_%7B1%7D%2Cg_%7B2%7D%2C...%2Cg_%7Br%7D%29)

如果 ![\varphi\in Z^{1}(G,M)](https://www.zhihu.com/equation?tex=%5Cvarphi%5Cin+Z%5E%7B1%7D%28G%2CM%29) ，那么对于任意G中的元素g1,g2，那么成立：

![d^{1}\varphi(g_{1}g_{2})=g_{1}\varphi(g_{2})-\varphi(g_{1}g_{2})+\varphi(g_{1})=0](https://www.zhihu.com/equation?tex=d%5E%7B1%7D%5Cvarphi%28g_%7B1%7Dg_%7B2%7D%29%3Dg_%7B1%7D%5Cvarphi%28g_%7B2%7D%29-%5Cvarphi%28g_%7B1%7Dg_%7B2%7D%29%2B%5Cvarphi%28g_%7B1%7D%29%3D0) ，即 

![Z^{1}(G,M)=\{\varphi:G\rightarrow M~|~\varphi(g_{1}g_{2})=g\varphi(g_{2})+\varphi(g_{1}),{\rm any}~g_{1},g_{2}\in G\}](https://www.zhihu.com/equation?tex=Z%5E%7B1%7D%28G%2CM%29%3D%5C%7B%5Cvarphi%3AG%5Crightarrow+M~%7C~%5Cvarphi%28g_%7B1%7Dg_%7B2%7D%29%3Dg%5Cvarphi%28g_%7B2%7D%29%2B%5Cvarphi%28g_%7B1%7D%29%2C%7B%5Crm+any%7D~g_%7B1%7D%2Cg_%7B2%7D%5Cin+G%5C%7D) 。

注意此时 ![\varphi(ee)=2\varphi(e)\Rightarrow\varphi(e)=0](https://www.zhihu.com/equation?tex=%5Cvarphi%28ee%29%3D2%5Cvarphi%28e%29%5CRightarrow%5Cvarphi%28e%29%3D0) 。

如果 ![\varphi\in B^{1}(G,M)](https://www.zhihu.com/equation?tex=%5Cvarphi%5Cin+B%5E%7B1%7D%28G%2CM%29) ，那么存在 ![\varphi_{0}\in C^{0}(G,M)=M](https://www.zhihu.com/equation?tex=%5Cvarphi_%7B0%7D%5Cin+C%5E%7B0%7D%28G%2CM%29%3DM) 对于任意G中元素g，成立：

![\varphi(g)=d^{0}\varphi_{0}(g)=(g-1)\varphi(1)=gm-m,\varphi(1)=m\in M](https://www.zhihu.com/equation?tex=%5Cvarphi%28g%29%3Dd%5E%7B0%7D%5Cvarphi_%7B0%7D%28g%29%3D%28g-1%29%5Cvarphi%281%29%3Dgm-m%2C%5Cvarphi%281%29%3Dm%5Cin+M) ，即

![B^{1}(G,M)=\{\varphi:G\rightarrow M~|~\varphi(g)=gm-m,{\rm any}~g\in G,m\in M\}](https://www.zhihu.com/equation?tex=B%5E%7B1%7D%28G%2CM%29%3D%5C%7B%5Cvarphi%3AG%5Crightarrow+M~%7C~%5Cvarphi%28g%29%3Dgm-m%2C%7B%5Crm+any%7D~g%5Cin+G%2Cm%5Cin+M%5C%7D) 。

通常我们记满足 ![\varphi(g_{1}g_{2})=g_{1}\varphi(g_{2})+\varphi(g_{1}),{\rm any}~g_{1},g_{2}\in G](https://www.zhihu.com/equation?tex=%5Cvarphi%28g_%7B1%7Dg_%7B2%7D%29%3Dg_%7B1%7D%5Cvarphi%28g_%7B2%7D%29%2B%5Cvarphi%28g_%7B1%7D%29%2C%7B%5Crm+any%7D~g_%7B1%7D%2Cg_%7B2%7D%5Cin+G) 的 ![\varphi:G\rightarrow M](https://www.zhihu.com/equation?tex=%5Cvarphi%3AG%5Crightarrow+M) 为***crossed homomorphism ***; 满足 ![\varphi(g)=gm-m,{\rm any}~g\in G,{\rm for~some}~m\in M](https://www.zhihu.com/equation?tex=%5Cvarphi%28g%29%3Dgm-m%2C%7B%5Crm+any%7D~g%5Cin+G%2C%7B%5Crm+for~some%7D~m%5Cin+M) 的 ![\varphi:G\rightarrow M](https://www.zhihu.com/equation?tex=%5Cvarphi%3AG%5Crightarrow+M) 记为***principal crossed homomorphism***.

因此，就有 ![H^{1}(G,M)=\frac{\{{\rm crossed~homomorphisms}~G\rightarrow M\}}{\{{\rm principal~crossed~homomorphisms}\}}](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29%3D%5Cfrac%7B%5C%7B%7B%5Crm+crossed~homomorphisms%7D~G%5Crightarrow+M%5C%7D%7D%7B%5C%7B%7B%5Crm+principal~crossed~homomorphisms%7D%5C%7D%7D) 。

特别地，如果G acts trivially on M，那么得到：

crossed homomorphism=group homomorphism，

principal crossed homomorphism=0。

此时成立：![H^{1}(G,M)=\text{Hom}_{\mathbb{Z}}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29%3D%5Ctext%7BHom%7D_%7B%5Cmathbb%7BZ%7D%7D%28G%2CM%29) 。

**[2]. **两个简单的例子

**Example 1**. 我们假设G是一个由g生成的n阶循环群，假设![\varphi\in B^{1}(G,M)](https://www.zhihu.com/equation?tex=%5Cvarphi%5Cin+B%5E%7B1%7D%28G%2CM%29) ，那么

![\varphi(g^{2})=g\varphi(g)+\varphi(g)](https://www.zhihu.com/equation?tex=%5Cvarphi%28g%5E%7B2%7D%29%3Dg%5Cvarphi%28g%29%2B%5Cvarphi%28g%29) ，![\varphi(g^{3})=\varphi(gg^{2})=g^{2}\varphi(g)+g\varphi(g)+\varphi(g)](https://www.zhihu.com/equation?tex=%5Cvarphi%28g%5E%7B3%7D%29%3D%5Cvarphi%28gg%5E%7B2%7D%29%3Dg%5E%7B2%7D%5Cvarphi%28g%29%2Bg%5Cvarphi%28g%29%2B%5Cvarphi%28g%29) ，

... ...   ![0=\varphi(g^{n})=g^{n-1}\varphi(g)+...+g\varphi(g)+\varphi(g)](https://www.zhihu.com/equation?tex=0%3D%5Cvarphi%28g%5E%7Bn%7D%29%3Dg%5E%7Bn-1%7D%5Cvarphi%28g%29%2B...%2Bg%5Cvarphi%28g%29%2B%5Cvarphi%28g%29) 。

于是 ![\varphi](https://www.zhihu.com/equation?tex=%5Cvarphi) 由 ![\varphi(g)=m](https://www.zhihu.com/equation?tex=%5Cvarphi%28g%29%3Dm) 以及方程 ![g^{n-1}m+...+gm+m=0](https://www.zhihu.com/equation?tex=g%5E%7Bn-1%7Dm%2B...%2Bgm%2Bm%3D0) 决定。

反过来，如果 ![g^{n-1}m+...+gm+m=0](https://www.zhihu.com/equation?tex=g%5E%7Bn-1%7Dm%2B...%2Bgm%2Bm%3D0) ，

那么 ![\varphi(g^{i})=g^{n-1}m+...+gm+m](https://www.zhihu.com/equation?tex=%5Cvarphi%28g%5E%7Bi%7D%29%3Dg%5E%7Bn-1%7Dm%2B...%2Bgm%2Bm) 决定了一个 ![\varphi\in Z^{1}(G,M)](https://www.zhihu.com/equation?tex=%5Cvarphi%5Cin+Z%5E%7B1%7D%28G%2CM%29) 。

注意到 ![\varphi\in B^{1}(G,M)\Leftrightarrow\varphi(g)=m=gx-x,{\rm for~some}~x\in M](https://www.zhihu.com/equation?tex=%5Cvarphi%5Cin+B%5E%7B1%7D%28G%2CM%29%5CLeftrightarrow%5Cvarphi%28g%29%3Dm%3Dgx-x%2C%7B%5Crm+for~some%7D~x%5Cin+M)。

根据上面的讨论，我们可以得到一个形式比较漂亮的 ![H^{1}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29) 的描述：

定义：![\text{Nm}_{G}:M\rightarrow M,m\mapsto\Sigma_{g\in G}gm](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D%3AM%5Crightarrow+M%2Cm%5Cmapsto%5CSigma_%7Bg%5Cin+G%7Dgm)

![g-1:M\rightarrow M,m\mapsto gm-m](https://www.zhihu.com/equation?tex=g-1%3AM%5Crightarrow+M%2Cm%5Cmapsto+gm-m)

那么得到同构，![H^{1}(G,M)\rightarrow\frac{{\rm Ker}({\rm Nm}_{G})}{(g-1)M},\varphi\mapsto \varphi(g)](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CM%29%5Crightarrow%5Cfrac%7B%7B%5Crm+Ker%7D%28%7B%5Crm+Nm%7D_%7BG%7D%29%7D%7B%28g-1%29M%7D%2C%5Cvarphi%5Cmapsto+%5Cvarphi%28g%29) 。

**Remark.** 如果M是乘法交换群，**注意要把＋写成乘法**。

**Example2. **特别地，我们可以用上面这个结果来看待Galois理论的内容：

设 ![L/K](https://www.zhihu.com/equation?tex=L%2FK) 是一个cyclic extension， ![G](https://www.zhihu.com/equation?tex=G) 是Galois group，那么 ![G](https://www.zhihu.com/equation?tex=G) naturally acts on ![L](https://www.zhihu.com/equation?tex=L) ，特别地， ![G](https://www.zhihu.com/equation?tex=G) 也作用在 ![L^{\times}](https://www.zhihu.com/equation?tex=L%5E%7B%5Ctimes%7D)上。

**如果考虑**![G](https://www.zhihu.com/equation?tex=G)**在**![L](https://www.zhihu.com/equation?tex=L)**上的作用，上述例子的**![\text{Nm}_{G}](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D)**是****Galois theory中的Trace映射**：

![\text{Tr}_{L/K}:L\rightarrow K,x\mapsto\sum_{g\in\text{Gal}(L/K)}gx](https://www.zhihu.com/equation?tex=%5Ctext%7BTr%7D_%7BL%2FK%7D%3AL%5Crightarrow+K%2Cx%5Cmapsto%5Csum_%7Bg%5Cin%5Ctext%7BGal%7D%28L%2FK%29%7Dgx)

**如果考虑**![G](https://www.zhihu.com/equation?tex=G)**在**![L^{\times}](https://www.zhihu.com/equation?tex=L%5E%7B%5Ctimes%7D)**上的作用，上述例子的**![\text{Nm}_{G}](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D)**是Galois theory中的Norm映射**：

![\text{Nm}_{L/K}:L^{\times}\rightarrow K^{\times},x\mapsto\prod_{g\in\text{Gal}(L/K)}gx](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BL%2FK%7D%3AL%5E%7B%5Ctimes%7D%5Crightarrow+K%5E%7B%5Ctimes%7D%2Cx%5Cmapsto%5Cprod_%7Bg%5Cin%5Ctext%7BGal%7D%28L%2FK%29%7Dgx)

实际上，我们可以证明 ![H^{1}(G,L^{\times})=0](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CL%5E%7B%5Ctimes%7D%29%3D0) （利用Dedekind's lemma on the independence of characters)以及 ![H^{r}(G,L)=0,r\in\mathbb{Z}_{>0}](https://www.zhihu.com/equation?tex=H%5E%7Br%7D%28G%2CL%29%3D0%2Cr%5Cin%5Cmathbb%7BZ%7D_%7B%3E0%7D) （利用Normal basis theorem)。

于是我们得到了Hilbert 90**：**

**Theorem. (Hilbert 90)**设 ![L/K](https://www.zhihu.com/equation?tex=L%2FK) 是一个循环扩张，![G](https://www.zhihu.com/equation?tex=G) 的生成元是 ![\sigma ](https://www.zhihu.com/equation?tex=%5Csigma+) 。

(1)如果 ![a\in L^{\times},\text{Nm}_{L/K}(a)=1](https://www.zhihu.com/equation?tex=a%5Cin+L%5E%7B%5Ctimes%7D%2C%5Ctext%7BNm%7D_%7BL%2FK%7D%28a%29%3D1) ，那么存在 ![b\in L^{\times}](https://www.zhihu.com/equation?tex=b%5Cin+L%5E%7B%5Ctimes%7D) 使得 ![a=\frac{\sigma b}{b}](https://www.zhihu.com/equation?tex=a%3D%5Cfrac%7B%5Csigma+b%7D%7Bb%7D) 。

(2)如果 ![a\in L,\text{Tr}_{L/K}(a)=1](https://www.zhihu.com/equation?tex=a%5Cin+L%2C%5Ctext%7BTr%7D_%7BL%2FK%7D%28a%29%3D1) ，那么存在 ![b\in L](https://www.zhihu.com/equation?tex=b%5Cin+L) 使得 ![a=\sigma b-b](https://www.zhihu.com/equation?tex=a%3D%5Csigma+b-b) 。

*Proof. *对于（1)，![\text{Nm}_{L/K}(a)=1\Leftrightarrow a\in{\rm Ker}({\rm Nm}_{L/K})](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BL%2FK%7D%28a%29%3D1%5CLeftrightarrow+a%5Cin%7B%5Crm+Ker%7D%28%7B%5Crm+Nm%7D_%7BL%2FK%7D%29) 。利用 ![H^{1}(G,L^{\times})=0](https://www.zhihu.com/equation?tex=H%5E%7B1%7D%28G%2CL%5E%7B%5Ctimes%7D%29%3D0) ，得到 ![a\in(\sigma-1)L^{\times}](https://www.zhihu.com/equation?tex=a%5Cin%28%5Csigma-1%29L%5E%7B%5Ctimes%7D)（注意这里是乘法），存在 ![b\in L^{\times}](https://www.zhihu.com/equation?tex=b%5Cin+L%5E%7B%5Ctimes%7D) 使得 ![a=\frac{\sigma b}{b}](https://www.zhihu.com/equation?tex=a%3D%5Cfrac%7B%5Csigma+b%7D%7Bb%7D) 。

对于（2）同理可证。

2. **Definition of homology groups**

**[1]. 同调群的定义**

同调群可以看作上同调群对偶的概念。

我们定义functor ![(~\dot~)_{G}:\text{Mod}_{G}\rightarrow \text{Ab}, M\mapsto M_{G}](https://www.zhihu.com/equation?tex=%28~%5Cdot~%29_%7BG%7D%3A%5Ctext%7BMod%7D_%7BG%7D%5Crightarrow+%5Ctext%7BAb%7D%2C+M%5Cmapsto+M_%7BG%7D) ，其中 ![M_{G}=M/\{gm-m~|~g\in G,m\in M\}](https://www.zhihu.com/equation?tex=M_%7BG%7D%3DM%2F%5C%7Bgm-m~%7C~g%5Cin+G%2Cm%5Cin+M%5C%7D) 即largest quotient of M on which G acts trivially。

**Fact 1.**![(~\dot~)_{G}](https://www.zhihu.com/equation?tex=%28~%5Cdot~%29_%7BG%7D) 是一个right exact functor。

*Proof. * 直接按照定义即可。

**Fact 2.**![\text{Mod}_{G}](https://www.zhihu.com/equation?tex=%5Ctext%7BMod%7D_%7BG%7D) has enough projectives.

*Proof. *任一个module是一个free module的quotient。

于是我们可以讨论 ![(~\dot~)_{G}](https://www.zhihu.com/equation?tex=%28~%5Cdot~%29_%7BG%7D) 的left derived functor ![L^{r}(~\dot~)_{G}](https://www.zhihu.com/equation?tex=L%5E%7Br%7D%28~%5Cdot~%29_%7BG%7D) ：对于一个G-module M，给一个projective resolution： ![...\rightarrow P_{2}\rightarrow P_{1}\rightarrow P_{0}\rightarrow M\rightarrow0](https://www.zhihu.com/equation?tex=...%5Crightarrow+P_%7B2%7D%5Crightarrow+P_%7B1%7D%5Crightarrow+P_%7B0%7D%5Crightarrow+M%5Crightarrow0)，apply ![(~\dot~)_{G}](https://www.zhihu.com/equation?tex=%28~%5Cdot~%29_%7BG%7D) 得到：

![...\rightarrow (P_{2})_{G}\rightarrow (P_{1})_{G}\rightarrow (P_{0})_{G}\rightarrow0](https://www.zhihu.com/equation?tex=...%5Crightarrow+%28P_%7B2%7D%29_%7BG%7D%5Crightarrow+%28P_%7B1%7D%29_%7BG%7D%5Crightarrow+%28P_%7B0%7D%29_%7BG%7D%5Crightarrow0) ， ![d_{r}:(P_{r})_{G}\rightarrow(P_{r-1})_{G}](https://www.zhihu.com/equation?tex=d_%7Br%7D%3A%28P_%7Br%7D%29_%7BG%7D%5Crightarrow%28P_%7Br-1%7D%29_%7BG%7D) 。

我们定义r-th homology group 为 ![H_{r}(G,M)=L^{r}(M)_{G}=\frac{{\rm Ker}d_{r}}{{\rm Im}d_{r+1}}](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CM%29%3DL%5E%7Br%7D%28M%29_%7BG%7D%3D%5Cfrac%7B%7B%5Crm+Ker%7Dd_%7Br%7D%7D%7B%7B%5Crm+Im%7Dd_%7Br%2B1%7D%7D) 。

由left derived functor的性质可知

(1) ![H_{0}(G,M)=M_{G}](https://www.zhihu.com/equation?tex=H_%7B0%7D%28G%2CM%29%3DM_%7BG%7D)

(2) 如果 ![M](https://www.zhihu.com/equation?tex=M) 是projective G-module，那么 ![H_{r}(G,M)=0,r\in\mathbb{Z}_{>0}](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CM%29%3D0%2Cr%5Cin%5Cmathbb%7BZ%7D_%7B%3E0%7D) 。

(3) Long exact sequences：若有exact ![0\rightarrow A\rightarrow B\rightarrow C\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+A%5Crightarrow+B%5Crightarrow+C%5Crightarrow+0) ，那么有

![...\rightarrow H_{r}(G,A)\rightarrow H_{r}(G,B)\rightarrow H_{r}(G,C)\rightarrow H_{r-1}(G,A)\rightarrow H_{r-1}(G,B)\rightarrow H_{r-1}(G,C)\rightarrow...](https://www.zhihu.com/equation?tex=...%5Crightarrow+H_%7Br%7D%28G%2CA%29%5Crightarrow+H_%7Br%7D%28G%2CB%29%5Crightarrow+H_%7Br%7D%28G%2CC%29%5Crightarrow+H_%7Br-1%7D%28G%2CA%29%5Crightarrow+H_%7Br-1%7D%28G%2CB%29%5Crightarrow+H_%7Br-1%7D%28G%2CC%29%5Crightarrow...)

其中 ![\delta_{r}: H_{r}(G,C)\rightarrow H_{r-1}(G,A)](https://www.zhihu.com/equation?tex=%5Cdelta_%7Br%7D%3A+H_%7Br%7D%28G%2CC%29%5Crightarrow+H_%7Br-1%7D%28G%2CA%29) 。

根据我们的需要，我们只给出 ![H_{1}(G,\mathbb{Z})](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29) 的刻画。

**[2]. Description of **![H_{1}(G,\mathbb{Z})](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29)

首先我们考虑 ![H_{0}(G,M)=M_{G}](https://www.zhihu.com/equation?tex=H_%7B0%7D%28G%2CM%29%3DM_%7BG%7D) 的另一种描述方式。

定义 ![\varepsilon:\mathbb{Z}[G]\rightarrow\mathbb{Z},\sum n_{i}g_{i}\mapsto\sum n_{i} ](https://www.zhihu.com/equation?tex=%5Cvarepsilon%3A%5Cmathbb%7BZ%7D%5BG%5D%5Crightarrow%5Cmathbb%7BZ%7D%2C%5Csum+n_%7Bi%7Dg_%7Bi%7D%5Cmapsto%5Csum+n_%7Bi%7D+) 。

**Fact 3.**![I_{G}=\text{Ker}\varepsilon](https://www.zhihu.com/equation?tex=I_%7BG%7D%3D%5Ctext%7BKer%7D%5Cvarepsilon) 是basis为 ![\{g-1~|~g\in G,g\ne1\}](https://www.zhihu.com/equation?tex=%5C%7Bg-1~%7C~g%5Cin+G%2Cg%5Cne1%5C%7D) 的 ![\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D) 的 free-Z 子模。

*Proof.* Obvious.

于是我们得到：

**Fact 4. **![H_{0}(G,M)=M_{G}=M/I_{G}M](https://www.zhihu.com/equation?tex=H_%7B0%7D%28G%2CM%29%3DM_%7BG%7D%3DM%2FI_%7BG%7DM) 。

现在我们利用exact sequence ![0\rightarrow I_{G}\rightarrow\mathbb{Z}[G]\rightarrow\mathbb{Z}\rightarrow 0](https://www.zhihu.com/equation?tex=0%5Crightarrow+I_%7BG%7D%5Crightarrow%5Cmathbb%7BZ%7D%5BG%5D%5Crightarrow%5Cmathbb%7BZ%7D%5Crightarrow+0) 来计算 ![H_{1}(G,\mathbb{Z})](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29) 。

（*一般地，这个technique称为****dimension shifting****，后面我们会反复利用*）

考虑long exact sequence：

![...\rightarrow H_{1}(G,\mathbb{Z}[G])\rightarrow H_{1}(G,\mathbb{Z})\rightarrow H_{0}(G,I_{G})\rightarrow H_{0}(G,\mathbb{Z}[G])\rightarrow H_{0}(G,\mathbb{Z})\rightarrow...](https://www.zhihu.com/equation?tex=...%5Crightarrow+H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%5BG%5D%29%5Crightarrow+H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29%5Crightarrow+H_%7B0%7D%28G%2CI_%7BG%7D%29%5Crightarrow+H_%7B0%7D%28G%2C%5Cmathbb%7BZ%7D%5BG%5D%29%5Crightarrow+H_%7B0%7D%28G%2C%5Cmathbb%7BZ%7D%29%5Crightarrow...)

注意到 

(1) ![\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=%5Cmathbb%7BZ%7D%5BG%5D) 作为free Z-module，它也是free Z[G] module，则 ![H_{r}(G,\mathbb{Z}[G])=0,r>0](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2C%5Cmathbb%7BZ%7D%5BG%5D%29%3D0%2Cr%3E0)

(2) 以及 **Fact 4** ，G acts trivially on Z ![\Rightarrow H_{0}(G,\mathbb{Z})=\mathbb{Z}/I_{G}\mathbb{Z}=\mathbb{Z}](https://www.zhihu.com/equation?tex=%5CRightarrow+H_%7B0%7D%28G%2C%5Cmathbb%7BZ%7D%29%3D%5Cmathbb%7BZ%7D%2FI_%7BG%7D%5Cmathbb%7BZ%7D%3D%5Cmathbb%7BZ%7D) 。

那么上面的exact sequence变成：

![0\rightarrow H_{1}(G,\mathbb{Z})\rightarrow I_{G}/I_{G}^{2}\rightarrow \mathbb{Z}[G]/I_{G}\rightarrow \mathbb{Z}\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29%5Crightarrow+I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%5Crightarrow+%5Cmathbb%7BZ%7D%5BG%5D%2FI_%7BG%7D%5Crightarrow+%5Cmathbb%7BZ%7D%5Crightarrow0)

中间的映射由 ![I_{G}\rightarrow\mathbb{Z}[G]](https://www.zhihu.com/equation?tex=I_%7BG%7D%5Crightarrow%5Cmathbb%7BZ%7D%5BG%5D) 诱导，那么 ![I_{G}/I_{G}^{2}\rightarrow0](https://www.zhihu.com/equation?tex=I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%5Crightarrow0) 。于是由以上exact sequence得到

**Fact 5. **![H_{1}(G,\mathbb{Z})= I_{G}/I_{G}^{2}](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29%3D+I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D) 。

**Proposition 1**. ![I_{G}/I_{G}^{2}\simeq G^{ab}](https://www.zhihu.com/equation?tex=I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%5Csimeq+G%5E%7Bab%7D) ，其中 ![ G^{ab}=G/G^{c}](https://www.zhihu.com/equation?tex=+G%5E%7Bab%7D%3DG%2FG%5E%7Bc%7D) ， ![G^{c}](https://www.zhihu.com/equation?tex=G%5E%7Bc%7D) 是commutator subgroup，![G^{c}=\{g^{-1}h^{-1}gh~|~h,g\in G\}](https://www.zhihu.com/equation?tex=G%5E%7Bc%7D%3D%5C%7Bg%5E%7B-1%7Dh%5E%7B-1%7Dgh~%7C~h%2Cg%5Cin+G%5C%7D) (***the smallest subgroup such that ***![G/G^{c}](https://www.zhihu.com/equation?tex=G%2FG%5E%7Bc%7D)*** is abelian***)。

*Proof.  *

**Define **![\varphi:G\rightarrow I_{G}/I_{G}^{2},g\mapsto(g-1)+I_{G}^{2}](https://www.zhihu.com/equation?tex=%5Cvarphi%3AG%5Crightarrow+I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%2Cg%5Cmapsto%28g-1%29%2BI_%7BG%7D%5E%7B2%7D) . 

(1) ![\varphi](https://www.zhihu.com/equation?tex=%5Cvarphi) is homomorphism:

Note that ![\varphi(gh)=(gh-1)+I_{G}^{2}](https://www.zhihu.com/equation?tex=%5Cvarphi%28gh%29%3D%28gh-1%29%2BI_%7BG%7D%5E%7B2%7D) , and ![(g-1)(h-1)+(g-1)+(h-1)=gh-1,(g-1)(h-1)\in I_{G}^{2}](https://www.zhihu.com/equation?tex=%28g-1%29%28h-1%29%2B%28g-1%29%2B%28h-1%29%3Dgh-1%2C%28g-1%29%28h-1%29%5Cin+I_%7BG%7D%5E%7B2%7D)

Therefore, ![\varphi(gh)=(g-1)+(h-1)+I_{G}^{2}=\varphi(g)\varphi(h)](https://www.zhihu.com/equation?tex=%5Cvarphi%28gh%29%3D%28g-1%29%2B%28h-1%29%2BI_%7BG%7D%5E%7B2%7D%3D%5Cvarphi%28g%29%5Cvarphi%28h%29) .

(2) ![\varphi](https://www.zhihu.com/equation?tex=%5Cvarphi) factors through ![G/G^{c}](https://www.zhihu.com/equation?tex=G%2FG%5E%7Bc%7D) :

We have to show that ![G^{c}\subseteq\text{Ker}\varphi](https://www.zhihu.com/equation?tex=G%5E%7Bc%7D%5Csubseteq%5Ctext%7BKer%7D%5Cvarphi) , but ![G/\text{Ker}\varepsilon\simeq {\rm a~subgroup~of}~I_{G}/I_{G}^{2}](https://www.zhihu.com/equation?tex=G%2F%5Ctext%7BKer%7D%5Cvarepsilon%5Csimeq+%7B%5Crm+a~subgroup~of%7D~I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D) which is abelian, then ![G^{c}\subseteq\text{Ker}\varphi](https://www.zhihu.com/equation?tex=G%5E%7Bc%7D%5Csubseteq%5Ctext%7BKer%7D%5Cvarphi) by definition of ![G^{c}](https://www.zhihu.com/equation?tex=G%5E%7Bc%7D).

Hence we obtain ![\widehat{\varphi}:G/G^{c}\rightarrow I_{G}/I_{G}^{2}:gG^{c}\mapsto(g-1)+I_{G}^{2}](https://www.zhihu.com/equation?tex=%5Cwidehat%7B%5Cvarphi%7D%3AG%2FG%5E%7Bc%7D%5Crightarrow+I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%3AgG%5E%7Bc%7D%5Cmapsto%28g-1%29%2BI_%7BG%7D%5E%7B2%7D) .

**Define**![\phi:I_{G}\rightarrow G/G^{c},g-1\mapsto gG^{c}](https://www.zhihu.com/equation?tex=%5Cphi%3AI_%7BG%7D%5Crightarrow+G%2FG%5E%7Bc%7D%2Cg-1%5Cmapsto+gG%5E%7Bc%7D).

(3) ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) is homomorphism: 

![\phi(\sum_{g}n_{g}(g-1)+\sum_{g}m_{g}(g-1))=\phi(\sum_{g}(n_{g}+m_{g})(g-1))\\=\prod_{g}g^{n_{g}+m_{g}}G^{c}=_{{\rm since}~G^{ab}~{\rm is~abelian}}\prod_{g}g^{n_{g}}G^{c}\cdot\prod_{g}g^{m_{g}}G^{c}\\=\phi(\sum_{g}n_{g}(g-1))+\phi(\sum_{g}m_{g}(g-1))](https://www.zhihu.com/equation?tex=%5Cphi%28%5Csum_%7Bg%7Dn_%7Bg%7D%28g-1%29%2B%5Csum_%7Bg%7Dm_%7Bg%7D%28g-1%29%29%3D%5Cphi%28%5Csum_%7Bg%7D%28n_%7Bg%7D%2Bm_%7Bg%7D%29%28g-1%29%29%5C%5C%3D%5Cprod_%7Bg%7Dg%5E%7Bn_%7Bg%7D%2Bm_%7Bg%7D%7DG%5E%7Bc%7D%3D_%7B%7B%5Crm+since%7D~G%5E%7Bab%7D~%7B%5Crm+is~abelian%7D%7D%5Cprod_%7Bg%7Dg%5E%7Bn_%7Bg%7D%7DG%5E%7Bc%7D%5Ccdot%5Cprod_%7Bg%7Dg%5E%7Bm_%7Bg%7D%7DG%5E%7Bc%7D%5C%5C%3D%5Cphi%28%5Csum_%7Bg%7Dn_%7Bg%7D%28g-1%29%29%2B%5Cphi%28%5Csum_%7Bg%7Dm_%7Bg%7D%28g-1%29%29)

(4) ![\phi](https://www.zhihu.com/equation?tex=%5Cphi) factors through ![I_{G}/I_{G}^{2}](https://www.zhihu.com/equation?tex=I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D) :

We have to show that ![I_{G}^{2}\subseteq\text{Ker}\phi](https://www.zhihu.com/equation?tex=I_%7BG%7D%5E%7B2%7D%5Csubseteq%5Ctext%7BKer%7D%5Cphi) . For any ![x\in I_{G}/I_{G}^{2}](https://www.zhihu.com/equation?tex=x%5Cin+I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D) , 

![x=(\sum_{u\ne1}m_{u}(u-1))(\sum_{v\ne1}n_{v}(v-1))=\sum_{u,v\ne1}m_{u}n_{v}[(uv-1)-(u-1)-(v-1)]](https://www.zhihu.com/equation?tex=x%3D%28%5Csum_%7Bu%5Cne1%7Dm_%7Bu%7D%28u-1%29%29%28%5Csum_%7Bv%5Cne1%7Dn_%7Bv%7D%28v-1%29%29%3D%5Csum_%7Bu%2Cv%5Cne1%7Dm_%7Bu%7Dn_%7Bv%7D%5B%28uv-1%29-%28u-1%29-%28v-1%29%5D)

Then we obtain 

![\phi(x)=\prod_{u,v}[(uv)\cdot u^{-1}\cdot v^{-1}]^{m_un_{v}}G^{c}](https://www.zhihu.com/equation?tex=%5Cphi%28x%29%3D%5Cprod_%7Bu%2Cv%7D%5B%28uv%29%5Ccdot+u%5E%7B-1%7D%5Ccdot+v%5E%7B-1%7D%5D%5E%7Bm_un_%7Bv%7D%7DG%5E%7Bc%7D)

But ![(uv)\cdot u^{-1}\cdot v^{-1}\in G^{c}](https://www.zhihu.com/equation?tex=%28uv%29%5Ccdot+u%5E%7B-1%7D%5Ccdot+v%5E%7B-1%7D%5Cin+G%5E%7Bc%7D) , so ![\phi(x)\in G^{c}](https://www.zhihu.com/equation?tex=%5Cphi%28x%29%5Cin+G%5E%7Bc%7D) ,i.e. ![I_{G}^{2}\subseteq\text{Ker}\phi](https://www.zhihu.com/equation?tex=I_%7BG%7D%5E%7B2%7D%5Csubseteq%5Ctext%7BKer%7D%5Cphi) . Hence we obtain

![\widehat{\phi}:I_{G}/I_{G}^{2}\rightarrow G/G^{c},(g-1)+I_{G}^{2}\mapsto gG^{c}](https://www.zhihu.com/equation?tex=%5Cwidehat%7B%5Cphi%7D%3AI_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%5Crightarrow+G%2FG%5E%7Bc%7D%2C%28g-1%29%2BI_%7BG%7D%5E%7B2%7D%5Cmapsto+gG%5E%7Bc%7D) .

Finally, ![\widehat{\phi}\widehat{\varphi}(gG^{c})=\widehat{\phi}(g-1+I_{G}^{2})=gG^{c}](https://www.zhihu.com/equation?tex=%5Cwidehat%7B%5Cphi%7D%5Cwidehat%7B%5Cvarphi%7D%28gG%5E%7Bc%7D%29%3D%5Cwidehat%7B%5Cphi%7D%28g-1%2BI_%7BG%7D%5E%7B2%7D%29%3DgG%5E%7Bc%7D) , so ![I_{G}/I_{G}^{2}\simeq G^{ab}](https://www.zhihu.com/equation?tex=I_%7BG%7D%2FI_%7BG%7D%5E%7B2%7D%5Csimeq+G%5E%7Bab%7D) .

**Corollary 1. **![H_{1}(G,\mathbb{Z})=G^{ab}](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2C%5Cmathbb%7BZ%7D%29%3DG%5E%7Bab%7D)

在我们关心的情形里，G是abelian extension L/K的Galois group，那么上述结果告诉我们一阶同调群即G。

我们希望得到：![\text{Gal}(L/K)=H_{1}(\text{Gal}(L/K),\mathbb{Z})\rightarrow?](https://www.zhihu.com/equation?tex=%5Ctext%7BGal%7D%28L%2FK%29%3DH_%7B1%7D%28%5Ctext%7BGal%7D%28L%2FK%29%2C%5Cmathbb%7BZ%7D%29%5Crightarrow%3F) ， ![?](https://www.zhihu.com/equation?tex=%3F) 是某个cohomology group/homology group，它的信息可以去反映 ![\text{Gal}(L/K)](https://www.zhihu.com/equation?tex=%5Ctext%7BGal%7D%28L%2FK%29) 的信息。

更一般地，能否连接 ![H_{r}(G,M)\rightarrow...?...\rightarrow H^{r}(G,M)](https://www.zhihu.com/equation?tex=H_%7Br%7D%28G%2CM%29%5Crightarrow...%3F...%5Crightarrow+H%5E%7Br%7D%28G%2CM%29) 。

3. **Tate cohomology groups**

Tate cohomology 可以将上同调群和同调群连接在一起，构成一个"very" long exact sequence。 我们熟知最基本的把short exact拉成long exact的方式是[Snake Lemma](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Snake_lemma)。

首先我们需要去构造一个short exact sequence。然后，这个tricky的构造使得我们可以用snake lemma把它拉长，从而把上同调群与同调群连接起来。下面我们写出细节。

特别地，**这里的G都是有限群**。

**[1]. Construction of the short exact sequence**

回忆我们一开始给出的Norm map， ![\text{Nm}_{G}:M\rightarrow M,m\mapsto\Sigma_{g\in G}gm](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D%3AM%5Crightarrow+M%2Cm%5Cmapsto%5CSigma_%7Bg%5Cin+G%7Dgm) 。（*well-defined， 因为G有限*）

我们发现两个简单的事实。

**Fact 6.**![\text{Im}(\text{Nm}_{G})\subseteq M^{G}, I_{G}M\subseteq \text{Ker}(\text{Nm}_{G})](https://www.zhihu.com/equation?tex=%5Ctext%7BIm%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%5Csubseteq+M%5E%7BG%7D%2C+I_%7BG%7DM%5Csubseteq+%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29) 。

*Proof. *第一个是显然的。第二个断言，假设 

![x\in I_{G}M,x=\sum_{g}n_{g}(g-1)m,\sum_{g}n_{g}=0](https://www.zhihu.com/equation?tex=x%5Cin+I_%7BG%7DM%2Cx%3D%5Csum_%7Bg%7Dn_%7Bg%7D%28g-1%29m%2C%5Csum_%7Bg%7Dn_%7Bg%7D%3D0) ，于是

![\text{Nm}_{G}(x)=\big(\sum_{u}u\sum_{g}n_{g}(g-1)\big)m](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D%28x%29%3D%5Cbig%28%5Csum_%7Bu%7Du%5Csum_%7Bg%7Dn_%7Bg%7D%28g-1%29%5Cbig%29m)

其中 ![\sum_{u}u\sum_{g}n_{g}(g-1)=\sum_{u}u(\sum_{g}n_{g}g-(\sum_{g}n_{g})\cdot1)=\sum_{u}u(\sum_{g}n_{g}g)](https://www.zhihu.com/equation?tex=%5Csum_%7Bu%7Du%5Csum_%7Bg%7Dn_%7Bg%7D%28g-1%29%3D%5Csum_%7Bu%7Du%28%5Csum_%7Bg%7Dn_%7Bg%7Dg-%28%5Csum_%7Bg%7Dn_%7Bg%7D%29%5Ccdot1%29%3D%5Csum_%7Bu%7Du%28%5Csum_%7Bg%7Dn_%7Bg%7Dg%29) 。

interchange sum 得到： ![\sum_{u}u(\sum_{g}n_{g}g)=\sum_{g}u(\sum_{u}n_{g}g)=(\sum_{g}n_{g})\cdot(\sum_{u}ug)=0](https://www.zhihu.com/equation?tex=%5Csum_%7Bu%7Du%28%5Csum_%7Bg%7Dn_%7Bg%7Dg%29%3D%5Csum_%7Bg%7Du%28%5Csum_%7Bu%7Dn_%7Bg%7Dg%29%3D%28%5Csum_%7Bg%7Dn_%7Bg%7D%29%5Ccdot%28%5Csum_%7Bu%7Dug%29%3D0) ，所以 ![\text{Nm}_{G}(x)=0](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D%28x%29%3D0) 。

于是 ![\text{Nm}_{G}:M\rightarrow M](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D%3AM%5Crightarrow+M) 诱导出 ![\text{Nm}_{G}:M/I_{G}M\rightarrow M^{G}](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D%3AM%2FI_%7BG%7DM%5Crightarrow+M%5E%7BG%7D) （即 ![\text{Nm}_{G}](https://www.zhihu.com/equation?tex=%5Ctext%7BNm%7D_%7BG%7D) factors through ![M/I_{G}M](https://www.zhihu.com/equation?tex=M%2FI_%7BG%7DM) ）以及exact sequence：

![0\rightarrow\frac{\text{Ker}(\text{Nm}_{G})}{I_{G}M}\rightarrow\frac{M}{I_{G}M}\rightarrow M^{G}\rightarrow\frac{M^{G}}{\text{Nm}_{G}(M)}\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow%5Cfrac%7B%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%7D%7BI_%7BG%7DM%7D%5Crightarrow%5Cfrac%7BM%7D%7BI_%7BG%7DM%7D%5Crightarrow+M%5E%7BG%7D%5Crightarrow%5Cfrac%7BM%5E%7BG%7D%7D%7B%5Ctext%7BNm%7D_%7BG%7D%28M%29%7D%5Crightarrow0)

即       ![0\rightarrow\frac{\text{Ker}(\text{Nm}_{G})}{I_{G}M}\rightarrow H_{0}(G,M)\rightarrow H^{0}(G,M)\rightarrow\frac{M^{G}}{\text{Nm}_{G}(M)}\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow%5Cfrac%7B%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%7D%7BI_%7BG%7DM%7D%5Crightarrow+H_%7B0%7D%28G%2CM%29%5Crightarrow+H%5E%7B0%7D%28G%2CM%29%5Crightarrow%5Cfrac%7BM%5E%7BG%7D%7D%7B%5Ctext%7BNm%7D_%7BG%7D%28M%29%7D%5Crightarrow0)**（*）**

**[2]. Apply the snake lemma**

现在任给一个short exact sequence ![0\rightarrow M\rightarrow N\rightarrow P\rightarrow0](https://www.zhihu.com/equation?tex=0%5Crightarrow+M%5Crightarrow+N%5Crightarrow+P%5Crightarrow0) 。我们得到：
![](https://pic4.zhimg.com/v2-e9eb54de8b92a9075e72ee7b2e6d7ceb_b.jpg)
**上下两个long exact sequences分别由left derived functor **![L^{r}(~\cdot)_{G}](https://www.zhihu.com/equation?tex=L%5E%7Br%7D%28~%5Ccdot%29_%7BG%7D)** 和right derived functor **![R^{r}(~\cdot)^{G}](https://www.zhihu.com/equation?tex=R%5E%7Br%7D%28~%5Ccdot%29%5E%7BG%7D)** 给出**；**三个竖直方向的mapping可以写成short exact sequence（*）。**

于是利用snake lemma得到：

![\text{Ker}f\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}M\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}N\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}P\rightarrow M^{G}/\text{Nm}_{G}(M)\rightarrow N^{G}/\text{Nm}_{G}(N)\rightarrow P^{G}/\text{Nm}_{G}(P)\rightarrow\text{Coker}g](https://www.zhihu.com/equation?tex=%5Ctext%7BKer%7Df%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DN%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DP%5Crightarrow+M%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28M%29%5Crightarrow+N%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28N%29%5Crightarrow+P%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28P%29%5Crightarrow%5Ctext%7BCoker%7Dg)

**（**）**

**Remark. 这里用到snake lemma的版本要包含ker f 和 Coker g，这样才方便说清楚下面的事实。**

我们说明

(1)![H_{1}(G,P)\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}M](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2CP%29%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM) , ![P^{G}/\text{Nm}_{G}(P)\rightarrow H^{1}(G,M)](https://www.zhihu.com/equation?tex=P%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28P%29%5Crightarrow+H%5E%7B1%7D%28G%2CM%29) 是**well-defined:(把同调**

**群从左边接上，上同调群从右边接上）**

事实上，注意到 ![\text{Ker}f=\text{Im}\big(H_{1}(G,P)\rightarrow H_{0}(G,M)\big)](https://www.zhihu.com/equation?tex=%5Ctext%7BKer%7Df%3D%5Ctext%7BIm%7D%5Cbig%28H_%7B1%7D%28G%2CP%29%5Crightarrow+H_%7B0%7D%28G%2CM%29%5Cbig%29) ，以及**（**）**中 ![\text{Ker}f\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}M](https://www.zhihu.com/equation?tex=%5Ctext%7BKer%7Df%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM) 单射，故 ![H_{1}(G,P)](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2CP%29) maps to ![\text{Ker}(\text{Nm}_{G})/I_{G}M](https://www.zhihu.com/equation?tex=%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM) 。

同样地， 我们有：![P^{G}/\text{Nm}_{G}(P)\rightarrow \text{Coker}g=H^{0}(G,P)/\text{Im}g= H^{0}(G,P)/\text{Ker}\big(H^{0}(G,P)\rightarrow H^{1}(G,M)\big)\subseteq H^{1}(G,M)](https://www.zhihu.com/equation?tex=P%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28P%29%5Crightarrow+%5Ctext%7BCoker%7Dg%3DH%5E%7B0%7D%28G%2CP%29%2F%5Ctext%7BIm%7Dg%3D+H%5E%7B0%7D%28G%2CP%29%2F%5Ctext%7BKer%7D%5Cbig%28H%5E%7B0%7D%28G%2CP%29%5Crightarrow+H%5E%7B1%7D%28G%2CM%29%5Cbig%29%5Csubseteq+H%5E%7B1%7D%28G%2CM%29)

(2) ![H_{1}(G,N)\rightarrow H_{1}(G,P)\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}M\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}N](https://www.zhihu.com/equation?tex=H_%7B1%7D%28G%2CN%29%5Crightarrow+H_%7B1%7D%28G%2CP%29%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DN) 与

![N^{G}/\text{Nm}_{G}(N)\rightarrow P^{G}/\text{Nm}_{G}(P)\rightarrow H^{1}(G,M)\rightarrow H^{1}(G,N)](https://www.zhihu.com/equation?tex=N%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28N%29%5Crightarrow+P%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28P%29%5Crightarrow+H%5E%7B1%7D%28G%2CM%29%5Crightarrow+H%5E%7B1%7D%28G%2CN%29)**是exact sequences**：

**(Exercise，hint：回忆snake lemma)**

于是我们得到一个"very" long exact sequence

![...\rightarrow H^{r}_{T}(G,M)\rightarrow H^{r}_{T}(G,N)\rightarrow H^{r}_{T}(G,P)\rightarrow H^{r+1}_{T}(G,M)\rightarrow H^{r+1}_{T}(G,N)\rightarrow H^{r+1}_{T}(G,P)\rightarrow...](https://www.zhihu.com/equation?tex=...%5Crightarrow+H%5E%7Br%7D_%7BT%7D%28G%2CM%29%5Crightarrow+H%5E%7Br%7D_%7BT%7D%28G%2CN%29%5Crightarrow+H%5E%7Br%7D_%7BT%7D%28G%2CP%29%5Crightarrow+H%5E%7Br%2B1%7D_%7BT%7D%28G%2CM%29%5Crightarrow+H%5E%7Br%2B1%7D_%7BT%7D%28G%2CN%29%5Crightarrow+H%5E%7Br%2B1%7D_%7BT%7D%28G%2CP%29%5Crightarrow...)

其中 ![H^{r}_{T}(G,M)](https://www.zhihu.com/equation?tex=H%5E%7Br%7D_%7BT%7D%28G%2CM%29) 称为***Tate cohomology group，***

![r>0,H^{r}_{T}(G,M)=H^{r}(G,M)\\r=0,H^{r}_{T}(G,M)=\frac{M^{G}}{\text{Nm}_{G}(M)}\\ r=-1,H^{r}_{T}(G,M)=\frac{\text{Ker}(\text{Nm}_{G})}{I_{G}M}\\ r<-1,H^{r}_{T}(G,M)=H_{r}(G,M)](https://www.zhihu.com/equation?tex=r%3E0%2CH%5E%7Br%7D_%7BT%7D%28G%2CM%29%3DH%5E%7Br%7D%28G%2CM%29%5C%5Cr%3D0%2CH%5E%7Br%7D_%7BT%7D%28G%2CM%29%3D%5Cfrac%7BM%5E%7BG%7D%7D%7B%5Ctext%7BNm%7D_%7BG%7D%28M%29%7D%5C%5C+r%3D-1%2CH%5E%7Br%7D_%7BT%7D%28G%2CM%29%3D%5Cfrac%7B%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%7D%7BI_%7BG%7DM%7D%5C%5C+r%3C-1%2CH%5E%7Br%7D_%7BT%7D%28G%2CM%29%3DH_%7Br%7D%28G%2CM%29)

于是我们完成了cohomology groups和homology groups的连接：

![...\rightarrow H_{1}(G,N)\rightarrow H_{1}(G,P)\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}M\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}N\rightarrow\text{Ker}(\text{Nm}_{G})/I_{G}P\rightarrow M^{G}/\text{Nm}_{G}(M)\rightarrow N^{G}/\text{Nm}_{G}(N)\rightarrow P^{G}/\text{Nm}_{G}(P)\rightarrow H^{1}(G,M)\rightarrow ...](https://www.zhihu.com/equation?tex=...%5Crightarrow+H_%7B1%7D%28G%2CN%29%5Crightarrow+H_%7B1%7D%28G%2CP%29%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DM%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DN%5Crightarrow%5Ctext%7BKer%7D%28%5Ctext%7BNm%7D_%7BG%7D%29%2FI_%7BG%7DP%5Crightarrow+M%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28M%29%5Crightarrow+N%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28N%29%5Crightarrow+P%5E%7BG%7D%2F%5Ctext%7BNm%7D_%7BG%7D%28P%29%5Crightarrow+H%5E%7B1%7D%28G%2CM%29%5Crightarrow+...)

