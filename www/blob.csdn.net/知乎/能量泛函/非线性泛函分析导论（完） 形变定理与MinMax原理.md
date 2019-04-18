# 非线性泛函分析导论（完）:形变定理与MinMax原理 - 知乎
# 

本节是非线性泛函分析**导论**的完结篇。既然是导论，在深度上就有所控制。我们的拓扑工具仅仅限于形变定理的最简单形式以及拓扑度理论中最简单的部分。在变分学中，更为现代的处理方法是借助**代数拓扑**的**同调群**这一工具来判断泛函临界点的存在性。但更现代的处理方法也是建立在朴素思想（拓扑形变）之上的，大体说来：

对于两个泛函的水平集（注意泛函是定义在函数空间上的，因此泛函的水平集均为无限维流形，我们称为**Banach流形**，即使是在纯数学领域，遇到的Banach流形大都也都是可以赋予Finsler结构的，因此成为**Banach-Finsler流形**），我们可以判断它们的同调群是否同构来判断它们是否同伦等价。基于形变定理，如果两个水平集不是同伦等价的，则其中必存在临界点（临界点会导致泛函水平集的拓扑畸变），那么对应的非线性偏微分方程必有（弱）解。

上节我们已经简要说明了形变定理的几何思想，介绍了映射度，并利用这一最简单的拓扑学武器演示了如何证明两个流形是环绕的。我们介绍了一个非常抽象的定理：**环绕定理**。因为这个定理过于“一般化”，比较难以驾驭，所以我们指出了它的一个特例：**山路定理**。可以这么认为，环绕定理是山路定理的推广形式。

这一节我们正式介绍**形变定理**，并利用它证明环绕定理的**抽象化形式：MinMax原理**。

利用泛函水平集（水平流形）的“流动”（下降流），考察其拓扑性质的变化，是我们研究光滑泛函临界点的重要手段之一，这在上篇文章已经给出了直观的说明。

[来自远方的飘泊客：非线性泛函分析导论（二）：变分问题的拓扑结构](https://zhuanlan.zhihu.com/p/37695456)

我们重述**山路定理**：
![](https://pic3.zhimg.com/v2-85b9617dfd5401ba1095b039589ca95a_b.jpg)![](https://pic4.zhimg.com/v2-7d4b6131fdbfe2e460dc00af5a27a977_b.jpg)

这个定理的几何直观也已经在上篇文章中表述。

下面我们使用“形变”这一极为朴素的拓扑思想证明**极小极大原理**。
![](https://pic1.zhimg.com/v2-918f3dc2db5c5ab997875beaec6600e4_b.jpg)
我们可以在这短短的证明中一窥形变定理的标准使用方法——反证法。

最后我们列举几个例子，总结我们介绍的定理在非线性偏微分方程中的应用。
![](https://pic1.zhimg.com/v2-cd20969f12e82055d54b3c553019c500_b.jpg)![](https://pic2.zhimg.com/v2-16add3ecd8aeae3bb36f98f6283d2409_b.jpg)![](https://pic3.zhimg.com/v2-2013143222191acbb768520f6ec025f6_b.jpg)![](https://pic3.zhimg.com/v2-ff60e7668b6e723a0a6991f61f749e76_b.jpg)![](https://pic1.zhimg.com/v2-5a3771c6af322388eb0afd187a2fffb8_b.jpg)![](https://pic2.zhimg.com/v2-ddac4a2be7d99e9ea9cf10c8f16b437d_b.jpg)![](https://pic1.zhimg.com/v2-71553c1146620577074fc339b36c0530_b.jpg)![](https://pic1.zhimg.com/v2-90f60f1b12c6d6d22bf0cbd7ada974e4_b.jpg)![](https://pic2.zhimg.com/v2-6a4fef3935511c08ec9f91dc5ab3d4a9_b.jpg)![](https://pic3.zhimg.com/v2-d80624b2c10e761f9a0d50932b60005a_b.jpg)![](https://pic2.zhimg.com/v2-9727b661355be68217e9fdae5f0af849_b.jpg)![](https://pic2.zhimg.com/v2-64c9bb726e8b5706b527727580178949_b.jpg)![](https://pic1.zhimg.com/v2-8a3beac29d484a22d446ae2cb3934798_b.jpg)![](https://pic2.zhimg.com/v2-403fd49945b88f9aba46d3dd7f9714c5_b.jpg)![](https://pic2.zhimg.com/v2-f2a6a4c379c53687c1f056ffd8d688ad_b.jpg)![](https://pic3.zhimg.com/v2-f6830dd34165e88503e0aab346f29c72_b.jpg)![](https://pic4.zhimg.com/v2-c3674808a55d865f9fe046f4cb3a5897_b.png)![](https://pic4.zhimg.com/v2-cd9dcf37c14fc12b6e6c346128ef52d3_b.png)![](https://pic4.zhimg.com/v2-4c7cffaa1ca1f6317201ab1c6d5fb783_b.jpg)![](https://pic4.zhimg.com/v2-1a7d95f52f9e4dc4fa1b958620694b33_b.jpg)![](https://pic2.zhimg.com/v2-839cf0ed68295402a8d774b20959c855_b.jpg)![](https://pic3.zhimg.com/v2-3b8e4ac890665234fc76e28c0ffb88b2_b.jpg)![](https://pic1.zhimg.com/v2-52351a0ada01bd2ff41cb22d2dffd714_b.jpg)![](https://pic3.zhimg.com/v2-85429da0b7a5775788a7824ab14a46a6_b.jpg)![](https://pic2.zhimg.com/v2-5d0d908d5ba9daba0ddeff5f6aa19f21_b.jpg)![](https://pic4.zhimg.com/v2-626857556ac6fa52514f2b2815b2a10b_b.png)

