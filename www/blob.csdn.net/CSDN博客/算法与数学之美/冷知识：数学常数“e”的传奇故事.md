# 冷知识：数学常数“e”的传奇故事 - 算法与数学之美 - CSDN博客
2018年09月28日 21:28:21[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：155
自然对数的底e是一个令人不可思议的常数，一个由 lim (1+1/n)^n 定义出的常数，居然在数学和物理中频频出现，简直可以说是无处不在。这实在是让我们不得不敬畏这神奇的数学世界。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuqyqjJKX2XcNE43Bfg6pHWvRKYwQAfKQQfBnc9PYvAia4F6xShgoic8zw/640?wx_fmt=jpeg)
**欧拉恒等式**
但凡说起e，一个必定要提到的公式就是欧拉恒等式——被誉为世界上最美丽的公式。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXudCGvj6DEkuJTEThRgE8tzRCPmobHESialia4Upz9dD7lbajpicEgcKrYg/640?wx_fmt=jpeg)
数学中最基本的5个常数——0、1、圆周率π、自然对数的底e和虚数单位i，以及数学中最基本的两个符号——等号和加号，就这样通过一个简单的恒等式联系在了一起，实在是让人叹服。
这个等式有个一几何的直观解释。一个实数在实数轴上可以用一个向量表示，旋转这个向量，就相当于乘以一个虚数i。据此建立一个以实数为横轴，虚数为纵轴的坐标系。
实单位向量，每次逆时针旋转π/2, 可以分别得到结果1,i,-1,-i,1. 即转4次以后就回到了原位。而当实单位向量保持长度不变旋转θ角度，得到的向量就是：cosθ+isinθ。
根据欧拉公式 e^iθ = cosθ+isinθ可以看出 e^iθ 就代表实单位向量1旋转θ角后而得到的向量。所以 e^iπ 意味着单位向量逆时针旋转了π，结果显然是-1。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXugq1mrcaaeZsocSJDpO8BucictrolufdemfNH9I16HmDOdRBFH5A1D7A/640?wx_fmt=jpeg)
**增长规律**
这个世界上有许许多多的事物满足这样的变化规律：
- 
增长率正比于变量自身的大小。例如放射性元素衰变的时候，衰变率就和现存的放射性物质多少成正比；
- 
资源无穷多的社会，人口出生率将（近似的）和现存人口数成正比等等。
而此类变化规律所确定的解，则是由以e为底的指数增长所描述的：如果x的变化率等于变量x自身的λ倍，那么该变量随时间t的函数则为
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXu46icfaA2KCLqLFXj9MddABnZrYxjmuc87j25Q19RuWApcicmERLIhTaw/640?wx_fmt=jpeg)
其中C是任意常数。而e的直观含义正是增长的极限，这个问题在 数学常数e的含义 中有过详细的介绍。
**正态分布**
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuY1JuMb5jbyFsHrLasUlibUYKDj6NPysuUYFZLibrHPzgFCRBJjgSFzyA/640?wx_fmt=jpeg)
正态分布是自然科学与行为科学中的定量现象的一个统计模型。各种各样的心理学测试分数和物理现象比如光子计数都被发现近似地服从正态分布，尽管这些现象的根本原因经常是未知的。
而理论上则可以证明如果把许多小作用加起来看做一个变量，那么这个变量服从正态分布。
正态分布在生活中也可谓是无处不在：
- 
多次反复测量一个物理量，测出来的值一般来说总是呈正态分布；
- 
瓶装可乐的实际体积，也是正态分布；
- 
一大群人的寿命分布、智商分布等，也都是正态分布。
而正态分布的表达式中，也神奇的出现了e。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuiaqYMsYp2P9XMsYjtZ3Z5BB7uKVEl3ibibZ2UtZweJSYHxUOIa27icpibtg/640?wx_fmt=jpeg)
**伽马函数与斯特林公式**
阶乘运算n！本来是定义在正整数上的。数学家最爱做的事情就是推广，因此阶乘函数自然不能幸免。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXu1nCulbsicib6N8yMq6pntvhSEtvbzIO8HxFCQGOS4ZMbbJGjY8seZ4Iw/640?wx_fmt=jpeg)
当把阶乘函数推广到定义域为复数的时候，我们要寻找的函数就是一条通过了所有（n+1,n!）点的函数。所谓的伽马函数Γ(x)满足了这个性质，而伽马函数的表达式中又出现了e:
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuZt5swkR0maJNdyN8FNiaCz9hmJbGicGbgfzUbBNjqFPNXu4gZ1RmBktA/640?wx_fmt=jpeg)
阶乘n!与e还有另一层神秘的联系。
当n趋于无穷大的时候，n!满足下面的近似关系式——斯特林公式：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuoBUAhMT2ojfMkRb9Gv2miczwCYibBWLJnhp7KAG1lnSMnW1T4egoMs9g/640?wx_fmt=jpeg)
（其中“~”符号表示同阶，可以大致认为是n趋于无穷大时的约等于）
要计算很大的阶乘值，位数受限而不能直接用计算机求出时，就可以用斯特林公式近似求出了。
**调和级数**
所谓调和级数，即1+1/2+1/3+1/4+…+1/n+...。它是一个发散级数，当n趋于无穷大的时候，这个和也将趋于无穷大。但是同样是发散的级数，发散也有快慢之分。调和级数发散速度是怎样的呢？伟大的欧拉发现的一个著名极限给出了答案：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuG9tqLNCOiaddwjvuznsDNqAqNf0VHsQ63tDxl0ybibZYrNEYiaZ5u4Kag/640?wx_fmt=jpeg)
因此调和级数的发散速度正是和以e为底的对数——ln函数的发散速度一致。
**素数与e**
素数（或称质数）是指除了1和它本身之外，无法被其他自然数整除的数。素数看似和e毫无联系，可是，素数分布的理论指出，素数的分布与e息息相关。如果用π(x)表示不大于x的素数个数（注意这里的π不是圆周率！），那么素数分布中心定理指出：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXucZLnOsSXRE2icicVdibDxdxx9WoSxBXjgfgguib832NbgegGOFicMnFliaLA/640?wx_fmt=jpeg)
或者可以写成
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuz2OHSa9DVlYe6DqvbBvRn2akAstI2tThiaEsKakKGtawJId7CPiaMcsQ/640?wx_fmt=jpeg)
注意到ln正是以e为底的对数。看，e就这样出现在了看似毫无关系的领域！
**悬链线**
数学史上曾经有一个著名问题，称之为悬链线问题：一根柔软不可伸长的链子，两头固定在空间中的两个定点上（这两个点不一定要等高），链子形成的曲线是怎样一条曲线呢？
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXuMtxqRIxicf2icaRHxPUZHrTW2kIhlUyI1opJJcGo9INyx281NaXbVzxw/640?wx_fmt=jpeg)
这个问题和最速降线问题提出的时间很接近，而且参与者也大多相同。早在文艺复兴时代它就已经被达芬奇研究过，可惜并没有得到答案。伽利略猜想答案是抛物线，这也和很多人最初的感觉是一致的，可惜后来被惠更斯在17岁的时候证明是错的。
和最速降线问题一样，这一问题伯努利兄弟中的一个也曾公开征集解答，不过这次是哥哥雅各布，他在1690年的《教师学报》中发表了这个问题。
在雅各布提出这一问题一年后的1691年6月，《教师学报》发表了惠更斯（当时已经62岁）、莱布尼茨以及约翰•伯努利提交的三份正确答案。
三人的方法都不一样，但最终的结果却是一致的。而雅各布自己则并没能把它解出来，这让弟弟约翰•伯努利异常兴奋。悬链线的正确方程是这样的：
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXugvN0MXa58YeOQsluOtcibZ4SibOV0tMMphkezeg9ehOyGhcVfuFibPBgQ/640?wx_fmt=jpeg)
它的发现在当时被看做是新微积分伟大成果的重要标志。而现在，悬链线则在世界著名的标志性建筑物——密苏里的圣路易斯大拱门——中永垂不朽了。
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkx3Y1OJt9uqWJGTxlym6AXubqiawlNjmjHRwS2xYj2OrzfzJnM3wgQOH6JibrbFibS4VjX1oe4umDCeg/640?wx_fmt=jpeg)
e一次次如幽灵般恰当的出现在了每一处，时常给人们带来惊喜。而上述这些，只不过它的冰山一角而已。
END
∑编辑 | Gemini
来源 |冷知识王子
更多精彩：
☞  [哈尔莫斯：怎样做数学研究](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=1&sn=35143b89b06fe4f5273f210b2d6a7c91&chksm=8b7e3290bc09bb86f7bb3f158d993df3f019a7e9ce3bc8897e164e35a2ebe5a4e0bdcc111089&scene=21#wechat_redirect)
☞  [扎克伯格2017年哈佛大学毕业演讲](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554187&idx=2&sn=c75293463823e4d6769638e54b64f3ec&chksm=8b7e3290bc09bb86dc1e3f8e78d0b6de8811d75f3dcb092766fcb8ba0bab1cd9ba1ddfcef3b9&scene=21#wechat_redirect)
☞  [线性代数在组合数学中的应用](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554141&idx=1&sn=74a74c4e4d08eba0dd734528aa0b08e7&chksm=8b7e32c6bc09bbd073b34c22004ac6e4d99c8a0caa64c7d3dbaa8fd55e6ef1fc87ed545b8b7e&scene=21#wechat_redirect)
☞  [你见过真的菲利普曲线吗？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554105&idx=1&sn=224ab0d38fb57facea70081385360d58&chksm=8b7e3222bc09bb34d3b6df665087e64b233778ed427598d08e809f96261e898c1c0de6188bbc&scene=21#wechat_redirect)
☞  [支持向量机(SVM)的故事是这样子的](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554096&idx=1&sn=46783e6ace661a3ccbd8a6e00fb17bf9&chksm=8b7e322bbc09bb3d73dc240f2280bddf2ef8b7824a459a24bd7f6eeadd60edb96e690d467f6e&scene=21#wechat_redirect)
☞  [深度神经网络中的数学，对你来说会不会太难？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554077&idx=2&sn=3ecd63f2205fd59df8c360c97c943ef6&chksm=8b7e3206bc09bb10a36b09547efe0c54f41423b180622c1fdc7f14747ccc8f8fecee3a12e2cd&scene=21#wechat_redirect)
☞  [编程需要知道多少数学知识？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652554062&idx=1&sn=17f0a88d5e15d1adfc29c690a0b1b89b&chksm=8b7e3215bc09bb038c6caa59d0f49cedd929f9be1104beea3411186cf4c81de69efc71a17883&scene=21#wechat_redirect)
☞  [陈省身——什么是几何学](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553994&idx=2&sn=74f67a1a3ac5c705f51f2ba619b717f6&chksm=8b7e3251bc09bb47dce73319948780081efe0333ffae99ea04a9eeabbcfcb38a29b4b73fb7c1&scene=21#wechat_redirect)
☞  [模式识别研究的回顾与展望](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553949&idx=2&sn=d171680964df774397efd9db81c00347&chksm=8b7e3386bc09ba90bf0f6e1cabf82ba86ff94630cb5ee2e0f14ff9455db52be32ddbc289d237&scene=21#wechat_redirect)
☞  [曲面论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553747&idx=1&sn=e25f866d510cf2338b6d9e1b32bafb62&chksm=8b7e3348bc09ba5ea1caaf2a7bfcd80a7e7559b1983e473eda2206e56df7f38ef3cecf2f77c7&scene=21#wechat_redirect)
☞  [自然底数e的意义是什么？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553811&idx=1&sn=000305074471c3d4c681c9cfd4e4bc93&chksm=8b7e3308bc09ba1e3043f5568a3a75a045285a1de97e4da36918bac68e7c6d579ad5d8cc25ab&scene=21#wechat_redirect)
☞  [如何向5岁小孩解释什么是支持向量机（SVM）？](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553670&idx=1&sn=ea75a448c016f7229e4cb298f6017614&chksm=8b7e309dbc09b98bc622acdf1223c7c2f743609d0a577dd43c9e9d98ab4da4314be7c1002bd5&scene=21#wechat_redirect)
☞  [华裔天才数学家陶哲轩自述](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553646&idx=2&sn=bbf8f1be1ca1c66ad3f3270babea6885&chksm=8b7e30f5bc09b9e3e1a4fa735412e2fcb20df9e78f2f346bf578018ceab77de6326095d1bf71&scene=21#wechat_redirect)
☞  [代数，分析，几何与拓扑，现代数学的三大方法论](http://mp.weixin.qq.com/s?__biz=MzA5ODUxOTA5Mg==&mid=2652553596&idx=1&sn=bc5064e871831f862db6d19c3de6327e&chksm=8b7e3027bc09b93194fa09b25e2df400421c062927bb9120912875f8aaf0bb25553fc8f51e3b&scene=21#wechat_redirect)
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
