# 自然底数e怎么就“自然”了？ - 知乎
# 

       在《[飞蛾真的是因为趋光所以扑火？](https://zhuanlan.zhihu.com/p/48391829)》中，通过对飞蛾运动轨迹的简化表达得到了与自然常数 ![e](https://www.zhihu.com/equation?tex=e)相关的**等角螺旋线**。自然常数 ![e](https://www.zhihu.com/equation?tex=e) 确实是一个奇妙的数字，这里的 ![e](https://www.zhihu.com/equation?tex=e)并不仅仅代表一个字母，它还是一个数学中的**无理常数**，约等于 ![2.718281828459](https://www.zhihu.com/equation?tex=2.718281828459) 。

    但你是否有想过，它到底怎么来的呢？为啥一个无理数却被人们称之为“**自然常数**”？
![](https://pic1.zhimg.com/v2-2af979a12cb34fc478a6a288f68bf618_b.jpg)
    说到 ![e](https://www.zhihu.com/equation?tex=e) ，我们会很自然地想起另一个无理常数 ![π](https://www.zhihu.com/equation?tex=%CF%80)。 ![π](https://www.zhihu.com/equation?tex=%CF%80) 的含义可以通过下图中的内接与外切多边形的边长逼近来很形象的理解。
![](https://pic4.zhimg.com/v2-fcc2169c98ae911f07ac090f6192319f_b.jpg)(图片来源: betterexplained)
    假设一个圆的直径为 ![1](https://www.zhihu.com/equation?tex=1) ，其外切与内接多边形的周长可以构成 ![π](https://www.zhihu.com/equation?tex=%CF%80) 的估计值的取值范围上下限，内接与外切多边形的边越多，取值范围就越窄，只要边数足够多，取值范围上下限就可以越来越逼近圆周率 ![π](https://www.zhihu.com/equation?tex=%CF%80) 。![π](https://www.zhihu.com/equation?tex=%CF%80) 的手动推导过程详见：《[古人是如何寻找到π的？](https://zhuanlan.zhihu.com/p/48307582)》

    如果说*π *的计算很直观，那 ![e](https://www.zhihu.com/equation?tex=e) 呢？所以在此也用一种图解法来直观理解 ![e](https://www.zhihu.com/equation?tex=e) 。

    首先，我们需要知道 ![e](https://www.zhihu.com/equation?tex=e) 这个表示自然底数的符号是由瑞士数学和物理学家Leonhard Euler(莱昂纳德·欧拉)命名的，取的正是Euler的首字母“ ![e](https://www.zhihu.com/equation?tex=e)”。
![](https://pic1.zhimg.com/v2-70d7da8dba8aa7f0c1e31eccca736f18_b.jpg)Leonhard Euler (1707-1783) (图片来源: Wikipedia)
    但实际上，第一个发现这个常数的，并非欧拉本人，而是**雅可比·伯努利**（Jacob Bernoulli）。
![](https://pic4.zhimg.com/v2-3ea20cadea9ff60a1e8fb596a50e345f_b.jpg)伯努利家族
    伯努利家族是17〜18世纪瑞士的一个赫赫有名的家族，其中出了很多著名的数理科学家，雅可比·伯努利是约翰·伯努利（Johann Bernoulli）的哥哥，而约翰·伯努利则是欧拉的数学老师。总之，大佬们之间有着千丝万缕的联系。
![](https://pic3.zhimg.com/v2-5461b90f2dbcb027ce1b60145e66f3a6_b.jpg)
    要了解 ![e](https://www.zhihu.com/equation?tex=e) 的由来，一个最直观的方法是引入一个经济学名称“**复利**(Compound Interest)”。

> **复利率法**（英文：compound interest），是一种计算利息的方法。按照这种方法，利息除了会根据本金计算外，新得到的利息同样可以生息，因此俗称“利滚利”、“驴打滚”或“利叠利”。只要计算利息的周期越密，财富增长越快，而随着年期越长，复利效应亦会越为明显。—— 维基百科

    在引入“**复利模型**”之前，先试着看看更基本的 “指数增长模型”。

    我们知道，大部分细菌是通过二分裂进行繁殖的，假设某种细菌 ![1](https://www.zhihu.com/equation?tex=1) 天会分裂一次，也就是一个增长周期为 ![1](https://www.zhihu.com/equation?tex=1) 天，如下图，这意味着：**每一天，细菌的总数量都是前一天的两倍**。
![](https://pic3.zhimg.com/v2-6cd13069aa56509ba50753f6b387394a_b.jpg)(图片来源: betterexplained)
    显然，如果经过 ![x ](https://www.zhihu.com/equation?tex=x+) 天（或者说，经过 ![x ](https://www.zhihu.com/equation?tex=x+) 个增长周期）的分裂，就相当于翻了 ![x ](https://www.zhihu.com/equation?tex=x+) 倍。在第 ![x](https://www.zhihu.com/equation?tex=x)天时，细菌总数将是初始数量的 ![2^x](https://www.zhihu.com/equation?tex=2%5Ex) 倍。如果细菌的初始数量为 ![1](https://www.zhihu.com/equation?tex=1) ，那么 ![x](https://www.zhihu.com/equation?tex=x) 天后的细菌数量即为 ![2^x](https://www.zhihu.com/equation?tex=2%5Ex)：
![](https://pic4.zhimg.com/v2-c3bb0fee4601b283c534f5ae4d4433bf_b.jpg)
    如果假设初始数量为 ![K](https://www.zhihu.com/equation?tex=K) ，那么 ![x ](https://www.zhihu.com/equation?tex=x+) 天后的细菌数量则为 ![K ·2^x ](https://www.zhihu.com/equation?tex=K+%C2%B72%5Ex+) ：

![](https://pic3.zhimg.com/v2-34713063f14a3270e7f040809db96a7a_b.jpg)

    因此，只要保证所有细菌一天分裂一次，不管初始数量是多少，最终数量都将是初始数量的 ![2^x](https://www.zhihu.com/equation?tex=2%5Ex) 倍。因此也可以写为：

![](https://pic3.zhimg.com/v2-4884234b74f6894b06682338cb5414b6_b.jpg)

     上式含义是：**第**![x ](https://www.zhihu.com/equation?tex=x+)**天时，细菌总数量是细菌初始数量的**![Q ](https://www.zhihu.com/equation?tex=Q+)**倍**。

    如果将 “**分裂**”或“**翻倍**”换一种更文艺的说法，也可以说是：“**增长率为**![100\%](https://www.zhihu.com/equation?tex=100%5C%25) ”。那我们可以将上式写为：

![](https://pic1.zhimg.com/v2-e23601d8bfe9a9f4ce10ae9d034500fc_b.jpg)

    当增长率不是 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) ，而是 ![50\%](https://www.zhihu.com/equation?tex=50%5C%25) 、 ![25\%](https://www.zhihu.com/equation?tex=25%5C%25) 之类的时候，则只需要将上式的 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) 换成想要的增长率即可。这样就可以得到更加普适的公式：

![](https://pic3.zhimg.com/v2-6606218aee174d059d6035502d540aee_b.jpg)

**    这个公式的数学内涵是：一个增长周期内的增长率为**![r](https://www.zhihu.com/equation?tex=r)***，*在增长了**![x](https://www.zhihu.com/equation?tex=x)**个周期之后，总数量将为初始数量的**![Q ](https://www.zhihu.com/equation?tex=Q+)**倍。**

    以上为指数增长的简单实例，下面来看看雅可比·伯努利的发现：

    假设你有 ![1](https://www.zhihu.com/equation?tex=1) 元钱存在银行里，此时发生了严重的通货膨胀，银行的利率飙到了 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) （夸张一下，为了方便计算）。如果银行一年付一次利息，自然在一年后你可以拿到 ![1](https://www.zhihu.com/equation?tex=1) 元的本金（蓝色圆）和 ![1](https://www.zhihu.com/equation?tex=1) 元的利息（绿色圆），总共两元的余额。

![](https://pic1.zhimg.com/v2-bd8ec7a7d1a815b6ff4d8dcca487f994_b.jpg)(图片来源: betterexplained)

    现在银行的年利率不变，但银行为了招揽客户，推出一项惠民政策，每半年就付一次利息。那么到第六个月的时候，你就能够提前从银行拿到 ![0.5](https://www.zhihu.com/equation?tex=0.5) 元的利息了。

![](https://pic4.zhimg.com/v2-b22a35f824c18e9e4fba574c84d556c7_b.jpg)(图片来源: betterexplained)

    机智的你会马上把这 ![0.5](https://www.zhihu.com/equation?tex=0.5) 元的利息再次存入银行，这 ![0.5](https://www.zhihu.com/equation?tex=0.5) 元的利息也将在下一结算周期产生利息(红色圆)，专业术语叫“**复利**”，那么年底的存款余额将等于 ![2.25](https://www.zhihu.com/equation?tex=2.25) 元。

![](https://pic1.zhimg.com/v2-a1aebb4c461987fab18195fb993c319c_b.jpg)(图片来源: betterexplained)

     此时，我们可以换个角度这样看：即，每个结算（增长）周期为半年，每半年的利率是 ![50\%](https://www.zhihu.com/equation?tex=50%5C%25) （或者说 ![\frac{100\%}{2}](https://www.zhihu.com/equation?tex=%5Cfrac%7B100%5C%25%7D%7B2%7D) ），一年结算两次利息，且第一次结算完后，立马将利息存入。此时我们的计算公式和结果如下：

![](https://pic1.zhimg.com/v2-4fae6f72d337876f4bb34a15cc1474d8_b.jpg)

    继续，假设现在银行为了和其他银行抢生意，短期不想赚钱了，每四个月就付一次利息！而机智的你依然一拿到利息就立马存入，与半年结算一次利息类似：即，每个结算周期为四个月，每四个月的利率是 ![33.33\%](https://www.zhihu.com/equation?tex=33.33%5C%25) （或者说 ![\frac{100\%}{3}](https://www.zhihu.com/equation?tex=%5Cfrac%7B100%5C%25%7D%7B3%7D) ），一年结算三次利息，且前两次结算完后，都立马将所有利息存入。

![](https://pic2.zhimg.com/v2-a15d2cd3adb5d7eaa7be2f4f1e47b161_b.jpg)(图片来源: betterexplained)

    此时计算公式和结果如下：
![](https://pic1.zhimg.com/v2-112f2a3390fdc16a4e6ec3eba1cedca0_b.jpg)
**    我的天，年利率虽然没有变，但随着每年利息交付次数的增加，你年底能从银行拿到的钱居然也在增加！**

    那么是不是会一直增大到无穷大呢？想得倒美…

    现在假设存款人和银行都疯了，银行在保证年利率为 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) 的前提下连续不断地付给存款人利息，存款人天天呆在银行不走，拿到利息就往银行里存。这样，所得利息即所谓“**连续复利**”。

    但是，你会发现，似乎有一个“天花板”挡住了你企图靠 ![1](https://www.zhihu.com/equation?tex=1) 块钱疯狂赚取 ![1](https://www.zhihu.com/equation?tex=1) 个亿的小目标，这个“天花板”就是 ![e](https://www.zhihu.com/equation?tex=e) ！

![](https://pic4.zhimg.com/v2-31bfaee69fa6ad7b325fad16376294bf_b.jpg)

    如果，我们进行一系列的迭代运算，我们将看到以下结果：

![](https://pic4.zhimg.com/v2-18d8d73f2762ad58d48ba80f9ef8f57f_b.jpg)

    其中， ![n](https://www.zhihu.com/equation?tex=n) 指的是一年中结算利息的次数。

    只要在年利率保持 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) 不变的情况下，不断地提高利息的结算次数，余额就将会逼近 ![e =2.71828182845… ](https://www.zhihu.com/equation?tex=e+%3D2.71828182845%E2%80%A6+)

    然后，终于可以祭出这个高等数学微积分里计算*e *的一个重要极限了：
![](https://pic4.zhimg.com/v2-0d3d32d5779408991961da23b7269803_b.jpg)
    现在再回头看这个重要极限，想必会有更加直观的理解。

    也就是说，就算银行的年利率是 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) ，再怎么求银行给你“复利”，年底也不可能得到超过本金![e ](https://www.zhihu.com/equation?tex=e+) 倍的余额。况且，我是没见过哪个银行的年利率是 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) 。

![](https://pic3.zhimg.com/v2-4a4f601b8e21547f7f2600802950a54e_b.jpg)

    虽然正常的银行不会推出连续复利这种优惠政策，但在自然界中，大多数事物都处在一种“无意识的连续增长”状态中。对于一个连续增长的事物，如果单位时间的增长率为 ![100\%](https://www.zhihu.com/equation?tex=100%5C%25) ，那么经过一个单位时间后，其将变成原来的 ![e ](https://www.zhihu.com/equation?tex=e+) 倍。生物的生长与繁殖，就也类似于“利滚利”的过程。

    再比如，在《[飞蛾真的是因为趋光所以扑火？](https://zhuanlan.zhihu.com/p/48391829)》中所说的等角螺线：

![](https://pic1.zhimg.com/v2-6e39d7fea9bd58e3189277b3e2204f64_b.jpg)等角螺线 (图片来源: Wikipedia)

    如果用极坐标表示，其通用数学表达式为：

![](https://pic4.zhimg.com/v2-02a49cab81299e09aaf1551ecf1f09a7_b.jpg)

    其中， ![a](https://www.zhihu.com/equation?tex=a) 、 ![b](https://www.zhihu.com/equation?tex=b)为系数，![r ](https://www.zhihu.com/equation?tex=r+) 螺线上的点到坐标原点的距离， ![θ ](https://www.zhihu.com/equation?tex=%CE%B8+) 为转角。**这正是一个以自然常数**![e ](https://www.zhihu.com/equation?tex=e+)**为底的指数函数。**

    例如，鹦鹉螺外壳切面就呈现优美的等角螺线：

![](https://pic3.zhimg.com/v2-fa610e46b2106be67f50bfa65f1b1312_b.jpg)鹦鹉螺外壳 (图片来源: Wikipedia)
    温带低气压的外观也像等角螺线：
![](https://pic3.zhimg.com/v2-0384eb721cac015a0304ee72bd7e236e_b.jpg)温带低气压 (图片来源: Wikipedia)
    就连旋涡星系的旋臂都像等角螺线：
![](https://pic3.zhimg.com/v2-47d27d6452520fb20abc22d05cba9aba_b.jpg)旋涡星系 (图片来源: Wikipedia)
    或许这也是 ![e](https://www.zhihu.com/equation?tex=e)被称为“**自然常数**”的原因吧。当然，自然常数 ![e ](https://www.zhihu.com/equation?tex=e+) 的奇妙之处还远不止这些，一本书都写不完。想了解更多内容，请扫描最下方二维码，关注“**科研狗**”公众号，发现更多精彩。

**Reference：**

[1] An Intuitive Guide To Exponential Functions & e, 

[https://betterexplained.com/articles/an-intuitive-guide-to-exponential-functions-e/](https://link.zhihu.com/?target=https%3A//betterexplained.com/articles/an-intuitive-guide-to-exponential-functions-e/)

[2] Prehistoric Calculus: Discovering Pi,

[https://betterexplained.com/articles/prehistoric-calculus-discovering-pi/](https://link.zhihu.com/?target=https%3A//betterexplained.com/articles/prehistoric-calculus-discovering-pi/)

[3] Compound interest, [https://en.wikipedia.org/wiki/Compound_interest](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Compound_interest)

[4] Leonhard Euler, [https://en.wikipedia.org/wiki/Leonhard_Euler](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Leonhard_Euler)

[5] Logarithmic spiral, [https://en.wikipedia.org/wiki/Logarithmic_spiral](https://link.zhihu.com/?target=https%3A//en.wikipedia.org/wiki/Logarithmic_spiral)

**推荐阅读**
[直观の数学​zhuanlan.zhihu.com![图标](https://pic1.zhimg.com/v2-9c87d956f28e27a418de774c1720e214_ipico.jpg)](https://zhuanlan.zhihu.com/c_1041689010053341184)
[李狗嗨：飞蛾真的是因为趋光，所以扑火？](https://zhuanlan.zhihu.com/p/48391829)

[李狗嗨：自然底数e怎么就“自然”了？](https://zhuanlan.zhihu.com/p/48391055)

[李狗嗨：拉普拉斯变换中的S是个什么鬼？](https://zhuanlan.zhihu.com/p/48314585)

[李狗嗨：虚数i真的很“虚”吗？](https://zhuanlan.zhihu.com/p/48392273)

[李狗嗨："上帝公式"(欧拉公式)真的神到无法触碰？](https://zhuanlan.zhihu.com/p/48392958)

[李狗嗨：为什么说"对数"可以延长天文学家寿命？](https://zhuanlan.zhihu.com/p/48393525)

[李狗嗨：0的0次方为何等于1？](https://zhuanlan.zhihu.com/p/48319167)

[李狗嗨：古人是如何寻找到π的？](https://zhuanlan.zhihu.com/p/48307582)

[李狗嗨：为什么e^x 的导数是还是其自身？](https://zhuanlan.zhihu.com/p/48678377)

[李狗嗨：一圈为何是360°？](https://zhuanlan.zhihu.com/p/48767522)

[李狗嗨：如何给文科生解释傅里叶变换？](https://zhuanlan.zhihu.com/p/48305950)

**更多精彩请扫码关注微信公众号，因为可能有重名的情况，最好是在微信主页面往下滑在搜索框内输入并搜索：“researchdog” 就能找到，公众号名字叫“科研狗”，公众号的头像是只戴着镜框的金毛犬。或扫下方二维码关注，谢谢噢~**

![](https://pic3.zhimg.com/v2-13d90069591d4dd15c9998d6d8889216_b.jpg)

