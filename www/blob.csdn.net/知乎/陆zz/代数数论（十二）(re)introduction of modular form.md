# 代数数论（十二）(re)introduction of modular form - 知乎
# 

## 一.Basic definition

模形式到处可见，例如

**从复平面上的双周期函数角度引入：**

![\Lambda \subset \mathbb C](https://www.zhihu.com/equation?tex=%5CLambda+%5Csubset+%5Cmathbb+C)一个格(如![\mathbb Z^2](https://www.zhihu.com/equation?tex=%5Cmathbb+Z%5E2)),可定义Weierstrass function

![P_{\Lambda}(z)=\frac{1}{z^2}+\sum_{0 \not =w\in \Lambda}\frac{1}{(z-w)^2}- \frac{1}{w^2}](https://www.zhihu.com/equation?tex=P_%7B%5CLambda%7D%28z%29%3D%5Cfrac%7B1%7D%7Bz%5E2%7D%2B%5Csum_%7B0+%5Cnot+%3Dw%5Cin+%5CLambda%7D%5Cfrac%7B1%7D%7B%28z-w%29%5E2%7D-+%5Cfrac%7B1%7D%7Bw%5E2%7D)

其在复平面上亚纯且双周期，在0处有二阶极点，在原点展开我们有
![](https://pic3.zhimg.com/v2-7afbcba2c0c202f097d449ac8f7d94ce_b.png)其中

![G_{2k}(\Lambda)=\sum_{0 \not =w\in \Lambda} \frac{1}{w^{2k}} \ \ (k \in \mathbb N , k \geq 2)](https://www.zhihu.com/equation?tex=G_%7B2k%7D%28%5CLambda%29%3D%5Csum_%7B0+%5Cnot+%3Dw%5Cin+%5CLambda%7D+%5Cfrac%7B1%7D%7Bw%5E%7B2k%7D%7D+%5C+%5C+%28k+%5Cin+%5Cmathbb+N+%2C+k+%5Cgeq+2%29)
write ![\Lambda = \mathbb Z w_1 + \mathbb Z w_2, \tau=\frac{w_1}{w_2}, Im \tau >0](https://www.zhihu.com/equation?tex=%5CLambda+%3D+%5Cmathbb+Z+w_1+%2B+%5Cmathbb+Z+w_2%2C+%5Ctau%3D%5Cfrac%7Bw_1%7D%7Bw_2%7D%2C+Im+%5Ctau+%3E0) we find

![](https://pic1.zhimg.com/v2-94f5144372ba9b9a95ce458495c41780_b.png)格![\Lambda \subset \mathbb C](https://www.zhihu.com/equation?tex=%5CLambda+%5Csubset+%5Cmathbb+C)在差一个伸缩下由![\tau(Im \tau >0)](https://www.zhihu.com/equation?tex=%5Ctau%28Im+%5Ctau+%3E0%29)唯一决定，并且![G_{2k}(\lambda \Lambda)=\lambda^{-2k}G_{2k}(\Lambda)](https://www.zhihu.com/equation?tex=G_%7B2k%7D%28%5Clambda+%5CLambda%29%3D%5Clambda%5E%7B-2k%7DG_%7B2k%7D%28%5CLambda%29)

因此G_2k可看成定义在上半平面的关于![\tau](https://www.zhihu.com/equation?tex=%5Ctau)的函数,![G_{2k}(\lambda \Lambda)=\lambda^{-2k}G_{2k}(\Lambda)](https://www.zhihu.com/equation?tex=G_%7B2k%7D%28%5Clambda+%5CLambda%29%3D%5Clambda%5E%7B-2k%7DG_%7B2k%7D%28%5CLambda%29)翻译成![\tau(Im \tau >0)](https://www.zhihu.com/equation?tex=%5Ctau%28Im+%5Ctau+%3E0%29)的变换关系就引入模形式定义：

Def:

上半平面H上的全纯函数全体A上可以承载![GL_2(\mathbb Q)^{+}](https://www.zhihu.com/equation?tex=GL_2%28%5Cmathbb+Q%29%5E%7B%2B%7D)(+表示行列式为正)的一个权为m的右作用(m是任何整数):

![ \gamma = \begin{pmatrix}
   a & b  \\
   c & d 
  \end{pmatrix} \in GL_2(\mathbb Q)^{+}](https://www.zhihu.com/equation?tex=+%5Cgamma+%3D+%5Cbegin%7Bpmatrix%7D%0A+++a+%26+b++%5C%5C%0A+++c+%26+d+%0A++%5Cend%7Bpmatrix%7D+%5Cin+GL_2%28%5Cmathbb+Q%29%5E%7B%2B%7D),f是H上全纯函数,则作用定义为
![f|_{\gamma }(z)=(det \gamma)^{\frac{m}{2}}(cz+d)^{-m}f(\frac{az+b}{cz+d})](https://www.zhihu.com/equation?tex=f%7C_%7B%5Cgamma+%7D%28z%29%3D%28det+%5Cgamma%29%5E%7B%5Cfrac%7Bm%7D%7B2%7D%7D%28cz%2Bd%29%5E%7B-m%7Df%28%5Cfrac%7Baz%2Bb%7D%7Bcz%2Bd%7D%29)

**设![\Gamma \subset SL_2(\mathbb Z)](https://www.zhihu.com/equation?tex=%5CGamma+%5Csubset+SL_2%28%5Cmathbb+Z%29)同余子群,如果f在![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)作用下不动，并且在每个cusp处全纯，则f称为![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)的weight k的modular form.(如果还在cusp处取值都是0，则称为cusp form)**
上面的cusp是指![](https://pic3.zhimg.com/v2-e971c65a718889e3b965d9294f30060e_b.png)

在![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)作用下的轨道类,例如无穷远点.由于同余子群有限指标,因此cusp有限个.

通常用![q=e^{2\pi i \tau }](https://www.zhihu.com/equation?tex=q%3De%5E%7B2%5Cpi+i+%5Ctau+%7D)(更一般的![q=e^{2\pi iN \tau }](https://www.zhihu.com/equation?tex=q%3De%5E%7B2%5Cpi+iN+%5Ctau+%7D))在无穷远点展开modular form为Laurent级数,得到无穷远点的表达式(此时用q代替z作为局部参数)，在无穷远点全纯是指在这个表达式下不出现z的负数次幂.

(在其他cusp处全纯，则利用一个SL_2(Z)元A将cusp映成无穷远点,然后A共轭作用f,![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma),看共轭之后的函数是否关于共轭后的群在无穷远点全纯)

上面的同余子群是指

![](https://pic2.zhimg.com/v2-7f801c6345b67c5c5c4a87fb43edee29_b.png)重要的同余子群有:

![](https://pic1.zhimg.com/v2-013f05098e63e8ca24eb289b6f489b2c_b.png)![](https://pic3.zhimg.com/v2-8ebbe017140ace793532d40d582806f6_b.png)（记忆方法是:下标0的对应模N后左下角是0,下标1的对于模N后对角都是1）

**基本而重要的观察是:**

**权k的modular form全体构成一个有限维复线性空间,所有modular form构成一个交换幺环.(**![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)固定**)**

研究modular form的第一件事是搞清楚群![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)的结构,例如全模群![SL_2(\mathbb Z)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+Z%29)

(更多可见[子群计数（二）：置换表示、全模群](https://zhuanlan.zhihu.com/p/24672622?refer=shuxuehenyouyisisuoyiyaohaohaoxue))
![](https://pic1.zhimg.com/v2-b74924e73ca7542e274a23f3959e6fd0_b.png)![](https://pic3.zhimg.com/v2-066b9cfbb8d4e977efb9e4b062237482_b.png)
而同余子群在SL_2(Z)中指标有限，因此也是有限生成的,**为了检验一个全纯函数是不是模形式，我们只需要对生成元检验.(因此从计算的角度来说求生成元是比较重要的准备工作)**

例如上面的![G_{2k}](https://www.zhihu.com/equation?tex=G_%7B2k%7D)就是一个weight 2k, level 1 的 modular form

所有以![\Lambda \subset \mathbb C](https://www.zhihu.com/equation?tex=%5CLambda+%5Csubset+%5Cmathbb+C)为周期的复平面上亚纯函数构成一个域![\mathbb C(P_\Lambda ,\frac{dP_\Lambda}{dz})](https://www.zhihu.com/equation?tex=%5Cmathbb+C%28P_%5CLambda+%2C%5Cfrac%7BdP_%5CLambda%7D%7Bdz%7D%29)，即环面![\mathbb C/\Lambda](https://www.zhihu.com/equation?tex=%5Cmathbb+C%2F%5CLambda)的函数域.

我们可以定义一个映射：![\mathbb C/\Lambda \hookrightarrow \mathbb {CP}^2 : z \mapsto [P_{\Lambda}(z),\frac{dP_{\Lambda}}{dz}(z),1]](https://www.zhihu.com/equation?tex=%5Cmathbb+C%2F%5CLambda+%5Chookrightarrow+%5Cmathbb+%7BCP%7D%5E2+%3A+z+%5Cmapsto+%5BP_%7B%5CLambda%7D%28z%29%2C%5Cfrac%7BdP_%7B%5CLambda%7D%7D%7Bdz%7D%28z%29%2C1%5D)

(由紧 函数域可分离点知其为嵌入)

则其像是一条genus 1的affine的代数曲线,其上点(x,y,1)均满足
![y^2=4x^3-g_2x-g_3](https://www.zhihu.com/equation?tex=y%5E2%3D4x%5E3-g_2x-g_3)

这是一条椭圆曲线(补上无穷远点),其中

![](https://pic1.zhimg.com/v2-0d8f2ede02ca7bd72e23f20c4e297d48_b.png)最后的![\Delta](https://www.zhihu.com/equation?tex=%5CDelta)为![y^2=4x^3-g_2x-g_3](https://www.zhihu.com/equation?tex=y%5E2%3D4x%5E3-g_2x-g_3)右边三次式的判别式,可见![\Delta](https://www.zhihu.com/equation?tex=%5CDelta)为weight 12,leve 1的cusp form.
**从组合角度入手:**

在[分划数与有限群的复表示](https://zhuanlan.zhihu.com/p/22222172)中，已经定义P(n)为把n划分若干个正整数之和的方法数
![](https://pic1.zhimg.com/v2-4a97af258c62578bcdac943a02fa3750_b.png)
考虑组合计数问题时，我们经常考虑数列{![a_n](https://www.zhihu.com/equation?tex=a_n)}的母函数:![\sum_{n=0}^{\infty}a_nq^n](https://www.zhihu.com/equation?tex=%5Csum_%7Bn%3D0%7D%5E%7B%5Cinfty%7Da_nq%5En)

而分划数的母函数为![](https://pic2.zhimg.com/v2-d81cbfa6894135cbe47ce05b3237d855_b.png)

则![](https://pic4.zhimg.com/v2-867fc2b41532a237b73f532294b0009f_b.png)也就是说![\Delta=q \prod_{n=1}^{\infty} (1-q^n)^{24}](https://www.zhihu.com/equation?tex=%5CDelta%3Dq+%5Cprod_%7Bn%3D1%7D%5E%7B%5Cinfty%7D+%281-q%5En%29%5E%7B24%7D)(这里我们正规化系数,忽略掉系数因子![(2\pi)^{12}](https://www.zhihu.com/equation?tex=%282%5Cpi%29%5E%7B12%7D))
从而一些母函数也来自于模形式.

**从二次型问题入手:**

见[代数数论（4.2）：modular form and sum of squares](https://zhuanlan.zhihu.com/p/24170999?refer=shuxuehenyouyisisuoyiyaohaohaoxue)

想法是考虑![\vartheta(z) =\sum_{a \in \mathbb Z}^{}{q^{a^2}} ](https://www.zhihu.com/equation?tex=%5Cvartheta%28z%29+%3D%5Csum_%7Ba+%5Cin+%5Cmathbb+Z%7D%5E%7B%7D%7Bq%5E%7Ba%5E2%7D%7D+),![q=e^{2 \pi i z}, Im z >0](https://www.zhihu.com/equation?tex=q%3De%5E%7B2+%5Cpi+i+z%7D%2C+Im+z+%3E0)

对![f(x)=e^{-\pi t x^2},t>0](https://www.zhihu.com/equation?tex=f%28x%29%3De%5E%7B-%5Cpi+t+x%5E2%7D%2Ct%3E0)使用Poisson求和则得到 ![\vartheta(\frac{-1}{4z}) =\sqrt{-2iz} \vartheta{(z)}](https://www.zhihu.com/equation?tex=%5Cvartheta%28%5Cfrac%7B-1%7D%7B4z%7D%29+%3D%5Csqrt%7B-2iz%7D+%5Cvartheta%7B%28z%29%7D)

令

![G_2(z)=\sum_{(c,d)}^{}{\frac{1}{(cz+d)^2}} ](https://www.zhihu.com/equation?tex=G_2%28z%29%3D%5Csum_%7B%28c%2Cd%29%7D%5E%7B%7D%7B%5Cfrac%7B1%7D%7B%28cz%2Bd%29%5E2%7D%7D+),其中c，d跑遍所有不同时为0的整数对

由于

![](https://pic2.zhimg.com/v2-e6071fe4bc893c656bffcdefc6170e8d_b.png)复分析告诉我们左边是：
![](https://pic4.zhimg.com/v2-99620db3f7d3763a41abcc1ccd53a7c3_b.png)所以![](https://pic4.zhimg.com/v2-64261d6a4a18bc8c531e9189221d403b_b.png)

求导得到
![](https://pic2.zhimg.com/v2-69a4818b034b7ac1b730822115a97781_b.png)
这个式子代入![G_2(z)=\sum_{(c,d)}^{}{\frac{1}{(cz+d)^2}} ](https://www.zhihu.com/equation?tex=G_2%28z%29%3D%5Csum_%7B%28c%2Cd%29%7D%5E%7B%7D%7B%5Cfrac%7B1%7D%7B%28cz%2Bd%29%5E2%7D%7D+)得到![G_2(\tau )](https://www.zhihu.com/equation?tex=G_2%28%5Ctau+%29)实际上等于：
![](https://pic3.zhimg.com/v2-ed4af70be5a22d10a98b7164103d98c6_b.png)
这个表示用来算，而![G_2(z)=\sum_{(c,d)}^{}{\frac{1}{(cz+d)^2}} ](https://www.zhihu.com/equation?tex=G_2%28z%29%3D%5Csum_%7B%28c%2Cd%29%7D%5E%7B%7D%7B%5Cfrac%7B1%7D%7B%28cz%2Bd%29%5E2%7D%7D+)告诉我们G_2(z)在变化下满足一定关系的，例如

①![\frac{1}{z^2}G_2(-\frac{1}{z})=G_2(z)-\frac{2\pi i}{z}](https://www.zhihu.com/equation?tex=%5Cfrac%7B1%7D%7Bz%5E2%7DG_2%28-%5Cfrac%7B1%7D%7Bz%7D%29%3DG_2%28z%29-%5Cfrac%7B2%5Cpi+i%7D%7Bz%7D)

②![G_2(z+1) =G_2{(z)}](https://www.zhihu.com/equation?tex=G_2%28z%2B1%29+%3DG_2%7B%28z%29%7D)

这里，我们碰到了一个类似模形式的但是又不是模形式的玩意G_2(多出来一个1/z项)，不如叫做‘’伪模形式’’,利用它可以构造模形式![G_{2,4}(z)=G_2(z) -4G_2(4z) ](https://www.zhihu.com/equation?tex=G_%7B2%2C4%7D%28z%29%3DG_2%28z%29+-4G_2%284z%29+)
然后得到它就是表正整数为四平方和方法数的母函数。(验证前面若干项)

推论：

**任何正整数都是四个平方数的和**

由上面的讨论可见,模形式可以来自于许多构造过程,其应用不止这些,下面的应用摘录自Modular Forms:
A Computational Approach
William A. Stein：
![](https://pic2.zhimg.com/v2-aeb29a07d05072c33e67e04f75044fd9_b.png)![](https://pic3.zhimg.com/v2-e68dc53a84470c586150edb901ce1dce_b.png)
![](https://pic2.zhimg.com/v2-97bf84510f15face6bdc197bfaa70909_b.png)![](https://pic4.zhimg.com/v2-e124df47bb6cf152bc6fe1be15fc7b0b_b.png)![](https://pic1.zhimg.com/v2-06076b7248e0ae7f96956f115a6750a4_b.png)
## 二.Examples we should keep in our mind

著名level 1的模形式(k=2,3,4,5,…)

![](https://pic4.zhimg.com/v2-e3303d9d6fe726b4bbf75abdc75dd3e3_b.png)其实对奇数也可以定义,不过由对称性结果是0,

对k为偶数我们有:

![](https://pic3.zhimg.com/v2-e72a4b8e32f7d96dd0054b7be00dd722_b.png)证明基本上是利用:
![](https://pic1.zhimg.com/v2-55f58a425d4ce8c984986db84568a2b4_b.png)引入伯努利数B_n(都是有理数，并且n是大于1的奇数时是0)
![](https://pic1.zhimg.com/v2-a2dfce19f8aa9a356d0036ff9644e70c_b.png)![](https://pic1.zhimg.com/v2-01afdd29341778f50d20b96b3527328c_b.png)对k是偶数成立:
![](https://pic1.zhimg.com/v2-38801bb105da2414b01b6b51b68fcf48_b.png)系数有理化
![](https://pic2.zhimg.com/v2-1e0f48f1e520f8e29b2a139337f7ff21_b.png)![](https://pic1.zhimg.com/v2-008116c79c41155839d6b04ec4ff5434_b.png)模形式空间有限维：
![](https://pic2.zhimg.com/v2-5a5fb60f699f495f86617568a9db68c5_b.png)
(![\rho=e^{\frac{2\pi i}{3}} ](https://www.zhihu.com/equation?tex=%5Crho%3De%5E%7B%5Cfrac%7B2%5Cpi+i%7D%7B3%7D%7D+))
Pf:沿基本区域积分然后使用留数定理

维数公式 for level 1:

Pf:

构造法以及上面的极点公式得到

![](https://pic4.zhimg.com/v2-e2282144ab67711f93fcbab561317c6b_b.png)k=2是0(由于X(1)是个球)

再注意到
![](https://pic2.zhimg.com/v2-019edb627ff7058607301ff0da23ec85_b.png)得到
![](https://pic3.zhimg.com/v2-57144a825262bbb4c6e65cda65983242_b.png)进一步分析得到
![](https://pic3.zhimg.com/v2-72f59ae9220f180a6a7c69a000bace26_b.png)我们将暂时忽略Hecke operator,不过注意其能存在一部分是因为![\mathbb Z^2](https://www.zhihu.com/equation?tex=%5Cmathbb+Z%5E2)的有限指标子群比较好刻画，子群之间的包含结构给出Hecke operator作用于格全体，从而作用于模形式.

## 三.How do we regard modular form as automorphic form?

这需要我们重新审视上半平面![\mathbb H](https://www.zhihu.com/equation?tex=%5Cmathbb+H)

(下面的讨论基本来自于讲义,增加一些补充)

![\mathbb H](https://www.zhihu.com/equation?tex=%5Cmathbb+H)装备上双曲度量![ds^2=\frac{dxdx+dydy}{y^2}](https://www.zhihu.com/equation?tex=ds%5E2%3D%5Cfrac%7Bdxdx%2Bdydy%7D%7By%5E2%7D) (![E=G=y^2](https://www.zhihu.com/equation?tex=E%3DG%3Dy%5E2)![,F=0](https://www.zhihu.com/equation?tex=%2CF%3D0),面积元 ![d\sigma=\frac{dxdy}{y^2})

](https://www.zhihu.com/equation?tex=d%5Csigma%3D%5Cfrac%7Bdxdy%7D%7By%5E2%7D%29%0A%0A)

引入这个度量，就可以使用**双曲体积来描述区域的大小。**

![GL_2(\mathbb R)^{+}](https://www.zhihu.com/equation?tex=GL_2%28%5Cmathbb+R%29%5E%7B%2B%7D)以分式线性变换的形式等距地、可迁地作用在![\mathbb H](https://www.zhihu.com/equation?tex=%5Cmathbb+H)上,即

![ \gamma = \begin{pmatrix}
   a & b  \\
   c & d 
  \end{pmatrix} ](https://www.zhihu.com/equation?tex=+%5Cgamma+%3D+%5Cbegin%7Bpmatrix%7D%0A+++a+%26+b++%5C%5C%0A+++c+%26+d+%0A++%5Cend%7Bpmatrix%7D+)作用为![z \mapsto \frac{az+b}{cz+d}](https://www.zhihu.com/equation?tex=z+%5Cmapsto+%5Cfrac%7Baz%2Bb%7D%7Bcz%2Bd%7D),有如下定义:

![](https://pic2.zhimg.com/v2-19bb42bd632cce713ec639c985f14005_b.png)
证明略去.

注意到![ \gamma = \begin{pmatrix}
   a & b  \\
   c & d 
  \end{pmatrix} , \hat\gamma = \begin{pmatrix}
   at & bt  \\
   ct & dt 
  \end{pmatrix} ,t>0](https://www.zhihu.com/equation?tex=+%5Cgamma+%3D+%5Cbegin%7Bpmatrix%7D%0A+++a+%26+b++%5C%5C%0A+++c+%26+d+%0A++%5Cend%7Bpmatrix%7D+%2C+%5Chat%5Cgamma+%3D+%5Cbegin%7Bpmatrix%7D%0A+++at+%26+bt++%5C%5C%0A+++ct+%26+dt+%0A++%5Cend%7Bpmatrix%7D+%2Ct%3E0)作用相同,所以适当选取t，我们只需要关心![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)

在![\mathbb H](https://www.zhihu.com/equation?tex=%5Cmathbb+H)上的作用.

我们可以重新定义cusp:
![](https://pic2.zhimg.com/v2-bf8d778a3590bdc9c9e718fd348ecb6d_b.png)
![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的离散子群称为Fuchsian group:
![](https://pic2.zhimg.com/v2-53682309824e873b0a7356681280f50d_b.png)实际上加入cusp凑成H^{*}本身就是为了紧化,由于我们这里考虑![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)而不是全模群,因此仅凭cusp的补填不一定能够紧化,为此我们还必须限制在第一类Fuchsian group上.而同余子群是cocompact可由![SL_2(\mathbb Z)\backslash\mathbb H^{*} \cong \mathbb {CP}^1](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+Z%29%5Cbackslash%5Cmathbb+H%5E%7B%2A%7D+%5Ccong+%5Cmathbb+%7BCP%7D%5E1)的紧性看出(紧流形的有限cover还是紧的)

我们有:

> 
**(Siegel)离散子群![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)是第一类的当且仅当![\Gamma \backslash\mathbb H](https://www.zhihu.com/equation?tex=%5CGamma+%5Cbackslash%5Cmathbb+H)体积有限，此时![\Gamma \backslash\mathbb H^{*}](https://www.zhihu.com/equation?tex=%5CGamma+%5Cbackslash%5Cmathbb+H%5E%7B%2A%7D)的cusp和elliptic point都是有限多个.**

我们模仿模形式的定义,引入自守因子的概念：
![](https://pic4.zhimg.com/v2-f40aa13961bf33df086afff5f9462a03_b.png)![](https://pic1.zhimg.com/v2-39a4b3ac822e2a43c96a05be7297b7e4_b.png)
给出模形式的重新定义:
![](https://pic3.zhimg.com/v2-a76598f569fb39ed98cb655c89903afe_b.png)
由于cusp form边界vanish很快，这允许我们定义内积:
![](https://pic1.zhimg.com/v2-644f231eec5edc7f4cc25b071481a460_b.png)

**接下来是一个重要的观察:**

注意到![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)可迁地作用在![\mathbb H](https://www.zhihu.com/equation?tex=%5Cmathbb+H)上,而点i的稳定子群是![SO(2)](https://www.zhihu.com/equation?tex=SO%282%29),因此我们得到

![\mathbb H \cong SL_2(\mathbb R) / SO(2): x \mapsto x SO(2)](https://www.zhihu.com/equation?tex=%5Cmathbb+H+%5Ccong+SL_2%28%5Cmathbb+R%29+%2F+SO%282%29%3A+x+%5Cmapsto+x+SO%282%29)
因此![\mathbb H](https://www.zhihu.com/equation?tex=%5Cmathbb+H)上的函数可看成李群![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上的函数,并且在SO(2)的右作用下不变.

我们逐渐又与线性代数群产生了联系(automorphic side)。

这么来看,模形式其实可以看成![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上的一类**特殊函数**,

其在![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的**极大紧子群**![SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=SO_2%28%5Cmathbb+R%29)的右作用下不变并且满足一定解析条件(C-R方程);

其在![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的**离散子群**![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)的slash action下保持不变并且满足一定边界条件(holomorphic at cusp);

为了进一步发展理论和把上面的想法精细化**(主要是为了引入测度与积分，这样才能用调和分析的结论)**

首先要理解我们研究的object——![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的结构,下面是一些记号：

![](https://pic3.zhimg.com/v2-65def337f9a15894eaac734695aae85a_b.png)
(注意![SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=SO_2%28%5Cmathbb+R%29)是交换群)

我们有著名的**Iwasawa decomposition:**

![](https://pic4.zhimg.com/v2-9ef4a9c6a9633db18034f76840a57a23_b.png)即每个![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)中元都可以唯一写成三个分别属于N,A,K的矩阵的乘积.

**结论：**

![](https://pic1.zhimg.com/v2-aa1be8e952508c3f896658a996af1bf0_b.png)给出光滑流形之间的同构(非群同构！![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)不是Abel群)
![(x,y,\theta)](https://www.zhihu.com/equation?tex=%28x%2Cy%2C%5Ctheta%29)称为![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上的双曲坐标
我们知道，局部紧群上都有左不变Haar测度,而![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的不变测度为:

![](https://pic2.zhimg.com/v2-0fe4b3173ec49c0310f135109d2f2f4d_b.png)（注意这里是**双不变**的！）

Pf:

![GL_2(\mathbb R)^+](https://www.zhihu.com/equation?tex=GL_2%28%5Cmathbb+R%29%5E%2B)上有一个双不变测度定义如下:

![dA=\frac{da_{11}da_{12}da_{21}da_{22}}{(detA)^2}](https://www.zhihu.com/equation?tex=dA%3D%5Cfrac%7Bda_%7B11%7Dda_%7B12%7Dda_%7B21%7Dda_%7B22%7D%7D%7B%28detA%29%5E2%7D)

把它限制到![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上,再转为双曲坐标即得,中间过程是一个很好的练习.

现在给一个weight k的模形式f,按照刚才的想法我们借助
![](https://pic3.zhimg.com/v2-ddd253dc629cd5b58a8c0c95dfb21eba_b.png)
把它理解成![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上的一个函数![\varphi_f:SL_2(\mathbb R) \mapsto \mathbb C](https://www.zhihu.com/equation?tex=%5Cvarphi_f%3ASL_2%28%5Cmathbb+R%29+%5Cmapsto+%5Cmathbb+C):
![\begin{pmatrix}
   a & b  \\
   c & d 
  \end{pmatrix}  \mapsto f(\frac{ai+b}{ci+d})(ci+d)^{-k}](https://www.zhihu.com/equation?tex=%5Cbegin%7Bpmatrix%7D%0A+++a+%26+b++%5C%5C%0A+++c+%26+d+%0A++%5Cend%7Bpmatrix%7D++%5Cmapsto+f%28%5Cfrac%7Bai%2Bb%7D%7Bci%2Bd%7D%29%28ci%2Bd%29%5E%7B-k%7D)

我们把![f \mapsto \varphi_f](https://www.zhihu.com/equation?tex=f+%5Cmapsto+%5Cvarphi_f)这一对应称为**自守提升(automorphic lift)**

**模形式通常是定义在几何的对象上(上半平面等等),自守形式通常是定义在李群/代数群上(![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)等等),通过上面的讨论可知前者可以看成后者的特殊case.**

我们希望逐字逐句地把所有模形式的条件全部翻译成自守的条件,即下面的定理:

![](https://pic4.zhimg.com/v2-a39fffa0a7122abb4c9460a7b367999f_b.png)Pf:

(1)就是模形式在![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)作用下满足的变换关系

(2)就是上半平面看成![SL_2(\mathbb R) / SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29+%2F+SO_2%28%5Cmathbb+R%29),其上函数在![SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=SO_2%28%5Cmathbb+R%29)右作用下不变,而![e^{ik\theta}](https://www.zhihu.com/equation?tex=e%5E%7Bik%5Ctheta%7D)这一因子的出现是因为我们定义![\varphi_f:SL_2(\mathbb R) \mapsto \mathbb C](https://www.zhihu.com/equation?tex=%5Cvarphi_f%3ASL_2%28%5Cmathbb+R%29+%5Cmapsto+%5Cmathbb+C)用到了自守因子

(3)即解析函数要满足柯西-黎曼方程.
然后借助![](https://pic1.zhimg.com/v2-aa7ff284fa41a97263f233a5d454e64c_b.png)

计算
从而建立了两种函数的一一对应.

还差几点，那就是

模形式的cusp form这一条件应该被翻译成什么?

刚才用上半平面的双曲积分定义的cusp form的Petersson内积又应该被翻译成什么?

我们有:
![](https://pic3.zhimg.com/v2-d71d4f2491e62aef10452321c91dff62_b.png)
(第(5)个条件中,![\sigma_s](https://www.zhihu.com/equation?tex=%5Csigma_s)为任取SL_2(R)中元使得![s=\sigma_s(\infty)](https://www.zhihu.com/equation?tex=s%3D%5Csigma_s%28%5Cinfty%29),而h>0是通过![\sigma_s](https://www.zhihu.com/equation?tex=%5Csigma_s)把cusp变为无穷远点后,使得![f|_{[\sigma_s]}(z+h)=f|_{[\sigma_s]}(z)](https://www.zhihu.com/equation?tex=f%7C_%7B%5B%5Csigma_s%5D%7D%28z%2Bh%29%3Df%7C_%7B%5B%5Csigma_s%5D%7D%28z%29)对任何z成立的最小的正数h)

证明如果有机会再补,并非hard的练习.

注意此时![\varphi ](https://www.zhihu.com/equation?tex=%5Cvarphi+)有界因此属于![L^2(\Gamma \backslash SL_2(\mathbb R))](https://www.zhihu.com/equation?tex=L%5E2%28%5CGamma+%5Cbackslash+SL_2%28%5Cmathbb+R%29%29),而![L^2(\Gamma \backslash SL_2(\mathbb R))](https://www.zhihu.com/equation?tex=L%5E2%28%5CGamma+%5Cbackslash+SL_2%28%5Cmathbb+R%29%29)自然给出![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的一个表示,这一表示的分解,将有助于尖点形式的分类.

(回忆![L^2(\mathbb R / \mathbb Z)](https://www.zhihu.com/equation?tex=L%5E2%28%5Cmathbb+R+%2F+%5Cmathbb+Z%29)的分解就给出[0,1]上的Fourier 变换)

这就自然把话题引向非紧群的**表示论**。

OK,我们小结一下:

通过
![](https://pic3.zhimg.com/v2-ddd253dc629cd5b58a8c0c95dfb21eba_b.png)
我们把定义在上半平面的模形式f与定义在![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上的一类“自守形式”![\varphi ](https://www.zhihu.com/equation?tex=%5Cvarphi+)通过automorphic lift联系起来:

① f在![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)作用下满足变换关系 ![f(\frac{ai+b}{ci+d})=f(z)(ci+d)^{k}](https://www.zhihu.com/equation?tex=f%28%5Cfrac%7Bai%2Bb%7D%7Bci%2Bd%7D%29%3Df%28z%29%28ci%2Bd%29%5E%7Bk%7D) 对应 

![\varphi ](https://www.zhihu.com/equation?tex=%5Cvarphi+)在![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)左作用下不变 ![\varphi(\gamma g)=\varphi (g)](https://www.zhihu.com/equation?tex=%5Cvarphi%28%5Cgamma+g%29%3D%5Cvarphi+%28g%29)

② f通过直接回拉![SL_2(\mathbb R) \rightarrow H](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29+%5Crightarrow+H)看成![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)上函数在![SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=SO_2%28%5Cmathbb+R%29)右作用下不变 对应 

![\varphi ](https://www.zhihu.com/equation?tex=%5Cvarphi+)在 ![SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=SO_2%28%5Cmathbb+R%29)满足变换关系![\varphi(gr(\theta) ) =e^{ik\theta}\varphi(g)](https://www.zhihu.com/equation?tex=%5Cvarphi%28gr%28%5Ctheta%29+%29+%3De%5E%7Bik%5Ctheta%7D%5Cvarphi%28g%29)

③ f全纯 等价于  ![\varphi ](https://www.zhihu.com/equation?tex=%5Cvarphi+)满足 ![](https://pic3.zhimg.com/v2-4884340c35fecfe306d7f42493659e3a_b.png)④f cusp form 等价于 ![\varphi ](https://www.zhihu.com/equation?tex=%5Cvarphi+)有界函数并且还满足积分条件(以无穷远这一cusp为例)

![](https://pic4.zhimg.com/v2-3503ca5fea345a096df157d3f995c00b_b.png)(上面的结论可以大概理解为![\Gamma \backslash SL_2(\mathbb R) / SO_2(\mathbb R)](https://www.zhihu.com/equation?tex=%5CGamma+%5Cbackslash+SL_2%28%5Cmathbb+R%29+%2F+SO_2%28%5Cmathbb+R%29)先右"作用"再左"作用"和先左"作用"再右"作用"结果一样)

因此,问题转为研究![L^2(\Gamma \backslash SL_2(\mathbb R))](https://www.zhihu.com/equation?tex=L%5E2%28%5CGamma+%5Cbackslash+SL_2%28%5Cmathbb+R%29%29)给出的![SL_2(\mathbb R)](https://www.zhihu.com/equation?tex=SL_2%28%5Cmathbb+R%29)的酉表示以及李群的结构,希望通过调和分析得到自守表示的一些性质。

为此，我们通常假定![\Gamma](https://www.zhihu.com/equation?tex=%5CGamma)就是某个同余子群.

Ref:

1.上学期的自守表示over GL_2的讲义

2.Modular Forms:
A Computational Approach
William A. Stein

