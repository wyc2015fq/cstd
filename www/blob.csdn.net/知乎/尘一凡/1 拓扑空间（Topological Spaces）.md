# 1 拓扑空间（Topological Spaces） - 知乎
# 

实数集![R](https://www.zhihu.com/equation?tex=R)的子集可分为「开子集」和「非开子集」两大类。我们发现![R](https://www.zhihu.com/equation?tex=R)的开子集具有这样的性质：（a）![R](https://www.zhihu.com/equation?tex=R)和空集![\varnothing](https://www.zhihu.com/equation?tex=%5Cvarnothing)都是开子集；（b）有限个开子集之交仍是开子集；（c）任意个开子集之并仍是开子集。- 注意，任一子集要么是**开的**，要么是**非开的**。不要把非开子集称为闭子集！因为非开子集可以是不开不闭等等。

我们将这三个性质推广，就可定义任何集合![X](https://www.zhihu.com/equation?tex=X)的的开子集。对集合的开子集的「指定方式」给集合![X](https://www.zhihu.com/equation?tex=X)赋予了一种附加结构，称为「拓扑结构」。所谓**给集合![X](https://www.zhihu.com/equation?tex=X)指定拓扑**，就是**指定集合![X](https://www.zhihu.com/equation?tex=X)中的哪些子集是开集**，指定的方式**必须**满足上述三个性质*。*

**定义1    **设![X](https://www.zhihu.com/equation?tex=X)是一个集合，![X](https://www.zhihu.com/equation?tex=X)上的拓扑（topology)![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D)是![X](https://www.zhihu.com/equation?tex=X)的**子集族**，满足：

（a）![X,\varnothing \in  \mathcal{T}](https://www.zhihu.com/equation?tex=X%2C%5Cvarnothing+%5Cin++%5Cmathcal%7BT%7D)；

（b）若![O_{i} \in \mathcal{T}](https://www.zhihu.com/equation?tex=O_%7Bi%7D+%5Cin+%5Cmathcal%7BT%7D)，![i=1,2,...,n](https://www.zhihu.com/equation?tex=i%3D1%2C2%2C...%2Cn)，则![\bigcap_{i=1}^{n} O_{i} \in \mathcal{T}](https://www.zhihu.com/equation?tex=%5Cbigcap_%7Bi%3D1%7D%5E%7Bn%7D+O_%7Bi%7D+%5Cin+%5Cmathcal%7BT%7D)（有限个![O_i](https://www.zhihu.com/equation?tex=O_i)之交）；

（c）若![O_{\alpha } \in \mathcal{T}](https://www.zhihu.com/equation?tex=O_%7B%5Calpha+%7D+%5Cin+%5Cmathcal%7BT%7D)，则![\bigcup_{\alpha }^{} O_{\alpha } \in \mathcal{T}](https://www.zhihu.com/equation?tex=%5Cbigcup_%7B%5Calpha+%7D%5E%7B%7D+O_%7B%5Calpha+%7D+%5Cin+%5Cmathcal%7BT%7D)（任意个![O_\alpha ](https://www.zhihu.com/equation?tex=O_%5Calpha+)之并）

> 
**DEFINITION 1   **Let ![X](https://www.zhihu.com/equation?tex=X) be a set. A ***t******opology***![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D) on ![X](https://www.zhihu.com/equation?tex=X) is a collection of subsets of ![X](https://www.zhihu.com/equation?tex=X), each called an **open set**, such that

(a)![X](https://www.zhihu.com/equation?tex=X) and ![\varnothing](https://www.zhihu.com/equation?tex=%5Cvarnothing) are open sets;

(b) The intersection of finitely many open sets is an open set;

(c)The union of any collection of open set is an open set.

Thus a collection of subsets of a ![X](https://www.zhihu.com/equation?tex=X) is a topology on ![X](https://www.zhihu.com/equation?tex=X), if it includes the empty set and ![X](https://www.zhihu.com/equation?tex=X), and if finite intersections and arbitrary union of sets in the collection are also in the collection.

**定义2  **  指定了拓扑![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D)的**集合**![X](https://www.zhihu.com/equation?tex=X)称为拓扑空间（topology space）。

- 定义2告诉我们，拓扑空间就是集合![X](https://www.zhihu.com/equation?tex=X)「如果![X](https://www.zhihu.com/equation?tex=X)指定了拓扑」。·
- **集合![X](https://www.zhihu.com/equation?tex=X)**及**定义在![X](https://www.zhihu.com/equation?tex=X)上的拓扑![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D)**是拓扑空间的两个要素，因此在正式场合用![(X, \mathcal{T})](https://www.zhihu.com/equation?tex=%28X%2C+%5Cmathcal%7BT%7D%29)代表拓扑空间。于是，![(X, \mathcal{T}_1)](https://www.zhihu.com/equation?tex=%28X%2C+%5Cmathcal%7BT%7D_1%29)和![(X, \mathcal{T}_2)](https://www.zhihu.com/equation?tex=%28X%2C+%5Cmathcal%7BT%7D_2%29)代表两个拓扑空间。在明确选取拓扑![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D)后，也可以只用![X](https://www.zhihu.com/equation?tex=X)表示拓扑空间。

**定义3  **设拓扑空间![(X, \mathcal{T})](https://www.zhihu.com/equation?tex=%28X%2C+%5Cmathcal%7BT%7D%29)有子集![O](https://www.zhihu.com/equation?tex=O),若![O\in \mathcal{T}](https://www.zhihu.com/equation?tex=O%5Cin+%5Cmathcal%7BT%7D),则![O](https://www.zhihu.com/equation?tex=O)称为「**开子集**」（open set）。
- 先有拓扑![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D),然后如果![X](https://www.zhihu.com/equation?tex=X)中的子集![O](https://www.zhihu.com/equation?tex=O)属于这个拓扑T,才有![O](https://www.zhihu.com/equation?tex=O)是开集的说法。

- 开集![O](https://www.zhihu.com/equation?tex=O)一方面是拓扑空间![X](https://www.zhihu.com/equation?tex=X)的**子集**，另一方面是拓扑![\mathcal{T}](https://www.zhihu.com/equation?tex=%5Cmathcal%7BT%7D)的**元素**。

> 
**![\Delta ](https://www.zhihu.com/equation?tex=%5CDelta+)例 **设![X](https://www.zhihu.com/equation?tex=X)是3个元素的集合![\{a, b, c\}](https://www.zhihu.com/equation?tex=%5C%7Ba%2C+b%2C+c%5C%7D)。 如图1所示是![X](https://www.zhihu.com/equation?tex=X)的四个子集族1,2,3,4，其中哪些子集族是![X](https://www.zhihu.com/equation?tex=X)上的拓扑呢？

**![](https://pic1.zhimg.com/v2-9be6c3776f887bf632f77bf22b855cec_b.png)分析   **记![X=\{a, b, c\}](https://www.zhihu.com/equation?tex=X%3D%5C%7Ba%2C+b%2C+c%5C%7D)，由图可知子集族![S_1=\{\varnothing,\{a\},\{a,b\}, X\}](https://www.zhihu.com/equation?tex=S_1%3D%5C%7B%5Cvarnothing%2C%5C%7Ba%5C%7D%2C%5C%7Ba%2Cb%5C%7D%2C+X%5C%7D)，我们只需验证拓扑定义的三个条件即可。我们发现：（1）空集![\varnothing](https://www.zhihu.com/equation?tex=%5Cvarnothing)和![X](https://www.zhihu.com/equation?tex=X)都属于![S_1](https://www.zhihu.com/equation?tex=S_1)；（2）![S_1](https://www.zhihu.com/equation?tex=S_1)中有限个子集的交集也在原来集族![S_1](https://www.zhihu.com/equation?tex=S_1)中；（3）![S_1](https://www.zhihu.com/equation?tex=S_1)中任意个子集的并集也在原集族![S_1](https://www.zhihu.com/equation?tex=S_1)中。因此![S_1](https://www.zhihu.com/equation?tex=S_1)是![X](https://www.zhihu.com/equation?tex=X)上的拓扑。同理可以证明集族2和3也是![X](https://www.zhihu.com/equation?tex=X)上的拓扑。

对于集族![S_4=\{\varnothing,\{a\},\{b\},\{c\},\{a,b\}, X\}](https://www.zhihu.com/equation?tex=S_4%3D%5C%7B%5Cvarnothing%2C%5C%7Ba%5C%7D%2C%5C%7Bb%5C%7D%2C%5C%7Bc%5C%7D%2C%5C%7Ba%2Cb%5C%7D%2C+X%5C%7D)。集合![\{a\},\{b\}](https://www.zhihu.com/equation?tex=%5C%7Ba%5C%7D%2C%5C%7Bb%5C%7D)都属于![S_4](https://www.zhihu.com/equation?tex=S_4)，但它们的并![\{a, c\}](https://www.zhihu.com/equation?tex=%5C%7Ba%2C+c%5C%7D)不属于![S_4](https://www.zhihu.com/equation?tex=S_4)，因此集族![S_4](https://www.zhihu.com/equation?tex=S_4)不是![X](https://www.zhihu.com/equation?tex=X)上的拓扑。

