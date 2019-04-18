# 图解虚数 - A Visual, Intuitive Gudie to Imaginary Numbers - 算法与数学之美 - CSDN博客
2018年10月07日 22:43:05[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：70
虚数总是让我困扰，就像在指数 e 的理解上，大多数解释都可以划分为这两类之中的一种：
- 
它是一个数学的抽象，解决了一些等式。**去好好地处理好它吧****~**
- 
相信我们，它用于高级物理。**等到大学你就可以学到了****。**
哈，这真是一个鼓励孩子去学习数学的一个“极佳”方式！今天，我们用一些我们最爱的工具来解决这个问题：
- 
把焦点放在「关系」上，而不是数学公式；
- 
将复数视为对现有数字系统的一次升级，就像曾经的 0，小数以及负数升级了当时的数字系统那样；
- 
通过视觉图表而不是文本来理解概念。
以及我们的秘密武器：**通过类比**。我们将会通过观察它的来源、负数来了解虚数。下面便是你的指南：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOB37BhRRgyLhX5CgfSm0bIXL9KnibqST2jYZibUdbIj08VhB249cZBY1Q/640?wx_fmt=png)
***sqrt(n) 指求 n 的平方根**
现在你可能还看不懂上面的指南，但是先放在这儿。最终我们会搞定虚数 i，然后将它存放在你深深的脑海里~
真正地了解负数
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOgnOM88k1vl6sVoHTxHBraKAF96Eoo9SVWvPPsFWLQ26iadSqIkiaAIzQ/640?wx_fmt=png)
负数并不简单。想像你是一位 18 世纪的欧洲数学家，你能写出 4-3=1，这很简单。
但是，如果是3-4呢？什么？这到底意味着什么呢？怎么能从 3 头奶牛中牵走 4 头呢？怎么可能比什么都没有还少呢？
负数曾被看作是**荒谬**的东西，是一种“使得等式的整个学说都变得灰暗”的东西(Francis Maseres, 1759)。然而在今天，把负数看成是没有逻辑或者没有用才是**荒谬**的。去问问你的老师，问他们负数是否改变了数学的整个根基。
这是发生了什么呢？是我们发明了一种*非常有用的理论数字*。我们不能触摸或者拿到负数，但是在描述某些关系时用负数非常方便（比如债务）。**它是一个非常有用的设想**。
相比于“我欠你 30”这种需要通过阅读词语来判断是负债与否，我可以写“-30”，这意味着我在负债。如果我挣到钱了，还清了债务(-30+1000=70)，我可以轻易地就记录下这笔交易。我有 +70 的富余，这意味着我的债务还清了。
正数和负数的符号**自动地追随了交易的流动方向**——你不再需要一个句子去描述每一笔交易对债务带来的变化。数学变得更加简单、更加优雅。负数可不可感知、是不是真实存在不再重要——因为它们拥有有用的属性，我们使用了它直到它成为了日常生活的每一个部分。在今天，如果有人“无法接受”负数，你可以说他们真的是骇人听闻。
但是还是不要对这种艰难的转变沾沾自喜：负数曾经是一个非常巨大的思想转变。即使如欧拉，这位发现了指数 e 等更多发现的天才，也无法像我们今天这样去理解负数。当时负数被当作是“无意义的”结果（后来他弥补了这一点，令人敬佩）。
这也证明了我们的思想潜能，即今天的孩子们期望去理解那些曾经困惑了古代数学家的问题。
进入虚数
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOgnOM88k1vl6sVoHTxHBraKAF96Eoo9SVWvPPsFWLQ26iadSqIkiaAIzQ/640?wx_fmt=png)
虚数有一个简单的故事。我们可以整天去解决这样的等式：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOefnOhCUlEISBGYqoGZACWAppRCSxC8o4SFGgBiapiaSYT2jpNQM2CBkw/640?wx_fmt=png)
它的答案是 3 和 -3。但是如果有一个聪明的人给它添加了一个小小的符号：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOCBBSFwbYA2ljxGUz6fnANtKFSJHvlof0fnMWR669qMF0aXZ4je7Akw/640?wx_fmt=png)
阿欧~这个问题让大多数人在看到它第一眼的时候就感觉到了尴尬。*你想要对一个小于 0 的数字求平方根？荒谬！*（历史上这个确实是要解决的问题，但我喜欢把它设想成一个聪明的人提出来的）
这个问题看起来好像很愚蠢，就好像负数、0、无理数（不循环的数）刚开始被提出来时一定也会被认为如此愚蠢。这个问题没有“实际”的意义，对吗？
错！所谓的“虚构的数字”与其他数字一样正常：它们都是描述这个世界的工具。就像假设 `-1`, `.3` 及 `0` “存在”一样，让我们假设有一个数字 i 存在：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHONGc2YhjjGaj2rGWyqcMHLgtdVuyYzftYFWAdRDH3oFg9w9mf59qPEA/640?wx_fmt=png)
就像这样，你把 i 乘以 i 可以得到 -1。那现在发生了什么？
当然，首先我们会感到头痛...但是“假设 i 存在”的游戏事实上让数学变得更加简单和优雅。一种我们可以更加方便地描述的**新的关系**就此浮现。
你也许不相信 i，就像那些固执的老数学家们一样不相信 -1 的存在。新的、绕脑的概念都很**难**，不能立刻理解，即使像欧拉这样的天才都不行。但是负数告诉我们，陌生的概念依然可以很有用。
我不喜欢“虚数”这个词语——它被看作是一种侮辱，伤害了 i 的感情。数字 i 就跟其他一样数字一样正常，但是“虚数”这个名字是摆脱不了了，我们还将会用它。
图解负数和复数
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOgnOM88k1vl6sVoHTxHBraKAF96Eoo9SVWvPPsFWLQ26iadSqIkiaAIzQ/640?wx_fmt=png)
正如上次我们看到的那样，等式 x^2=9 意味着：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOiafYvexDvrLhuVpHFtLcqCuMf0AhNzOEWBg2CVn8M1dKfEfJJWx91ibQ/640?wx_fmt=png)
或者：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHObQTpytkTjnGyvMlZ06ywicst28fdynicy3WOc5OtmRpKGo5aqPo8KvYQ/640?wx_fmt=png)
*x 是什么转换数，累乘两次，就能把 1 变成 9？*
答案有两个：“x = 3”和 “x = -3”：也就是说，通过将其扩大 3 倍后再扩大 3 倍来实现。
现在让我们考虑 x^2=-1，也就是
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOZiaFBMGoK7iagPiaMK4nvHnU7TicavEBvBSicmuicMl19Fx4ibzW11ib4lExhQ/640?wx_fmt=png)
*x 是什么转换数，累乘两次，就能把 1 变成 -1？*
- 
我们不能乘以一个正数乘两次，因为结果还是正数；
- 
我们不能乘以一个负数乘两次，因为结果在第二次乘之后会跳回至正数。
然而如果是...**旋转**呢！这听起来很疯狂，但是如果我们想像把 x “旋转 90 度”，乘以两次 x 的话，即为旋转 180 度，1 就会变成 -1。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHO2tsyj0pKq46BmbsI4Od9wEbvQ94g1X4YSp9KV53QDLh4Tq2MNanDTQ/640?wx_fmt=png)
呀！如果我们在想想，会发现将其在其他方向（顺时针）旋转两次也能将 1 转换为 -1。这是一个“负”旋转或者说乘以 -i：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOCY0NF5KXRJFiaqnSb6jw2iceFc7OqDE9lMviaqTmZ4mziaBnq3zwnGty9g/640?wx_fmt=png)
如果我们乘以 -i 两次，第一次乘法会将 1 转换成 -i，第二次将 -i 转换成 -1。所以这里实际上有两个 -1 的平方根：i 和 -i。
这非常酷！我们有了某种形式的答案，但是它们意味着什么呢？
- 
i 是一个“新设想出来的维度”用来衡量数字；
- 
i (or -i) 是数字在旋转中“形成的”；
- 
乘以 i 就是逆时针旋转 90 度；
- 
乘以 -i 就是顺时针旋转 90 度；
- 
两种旋转在各自的方向上都是 -1：它把我们带回了正数与负数所在的“常规”维度。
**数字成二维了****。**这是思维的拓展，就像小数或者长除法对一个古罗马人是思维拓展一样。（你认为 1 和 2 之间的数字有什么意义？）。这是一个新奇的看待数学的方式。
我们问“我们如何用两步实现 1 转换成 -1？”然后发现了答案：将其旋转 90 度。**这是一个新奇的看待数学的方式****。**但非常有用。（顺带提一下，直到 i 被发现后的数十年才有了这个关于复数的几何解释）。
同时也要记住逆时针旋转变成正数是一个人类的发明——有可能还存在其它更为简单的方式。
找到模式
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOgnOM88k1vl6sVoHTxHBraKAF96Eoo9SVWvPPsFWLQ26iadSqIkiaAIzQ/640?wx_fmt=png)
让我们深入到一点小细节中。当乘以一个负数时（就像 -1），你会得到一个模式：
- 
1, -1, 1, -1, 1, -1, 1, -1
-1 并没有改变数字的大小，只改变了符号，来来回回。而对于一些数字"x"，你可以得到：
- 
x, -x, x, -x, x, -x...
这个概念非常有用。数字"x"可以代表一个愉快或者糟糕的一周。假设每周被分为愉快的和糟糕的，现在是愉快的一周，那么在 47 周的时候是愉快的一周还是糟糕的一周呢？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHO9ic6HnMLbzWYJr5KntWnzKar4AGAzy84b2S7Gicib74puJs2gJYLnntUw/640?wx_fmt=png)
所以 -x 意味着是糟糕的一周。注意负数是如何“保持与符号的联系”的——我们可以把 (-1)^47 放到计算器里面而不用去算（“第一周愉快，第二周糟糕，第三周愉快...）。**这种可以来来回回的东西可以很好地应用负数这个模型****。**
那好，现在如果我们持续乘以 i 会怎么样？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOrzlicwnANFvDqAomSicSFSfPW8DuBxcyu1DbymUe3UicI3ZOSAia0v9drA/640?wx_fmt=png)
这非常有趣。让我们简化一点：
- 
1 = 1（这里无需简化）
- 
i = i （这里无需简化）
- 
i ^ 2 = -1 (这就是 i 的全部)
- 
i ^ 3 = (i · i) · i = -1 · i = -i （3 次逆时针旋转等于 1 次顺时针旋转，非常好）
- 
i ^ 4 = (i · ) · (i · i) = -1 · -1 = 1 (4 次旋转带来了一个“整圆”)
- 
i ^ 5 = i ^ 4 · i = 1 · i = i （这里开始重复...)
从视觉上来看：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOyiaBpibsdkgN6xibVSOJEHFMBicFwWJbzHkYJJIC3OczBgkGPVSDOXfu0w/640?wx_fmt=png)
每次循环，我们旋转 4 次。这就有意义了，对不对？任何一个小孩子都能告诉你旋转 4 次相当于没动。现在让我们将精力放在虚数(i, i^2)上，观察这个一般的模式：
- 
X, Y, -X, -Y, X, Y, -X, -Y...
就像负数模型那样来回翻转，虚数**可以作为 "X" 和 "Y" 这两个维度之间的任何东西的旋转模型**，或者是，任何只要有**循环****、****环状关系**的东西——有什么想法了吗？
Cos it’d be a sin if you didn’t. There’ll de Moivre be more in future articles.
*[译者注：作者在这里使用了双关，翻译成中文就失去了意义，此句不包含关键信息]*
理解复数
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOgnOM88k1vl6sVoHTxHBraKAF96Eoo9SVWvPPsFWLQ26iadSqIkiaAIzQ/640?wx_fmt=png)
这里还有一个细节要说：一个数字能既可以有“实部”又有“虚部”吗？
当然有。谁说旋转必须要旋转整个 90 度？如果我们在“实部”的维度和“虚部”的维度上各走一步，看来就是这样：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOfibztDOgxDwOjoAFFEH5PrakIvOXbcdUCowoxQxOChOgtjn0iaCrUuJA/640?wx_fmt=png)
现在我们处于 45 度，在真实的部分和虚构的部分拥有相等的部分(1+i)。这就好像一个热狗同时有芥末酱和番茄酱——谁说你必须要选择了？
事实上，我们可以选择任意的实部和虚部的数字来组成一个三角形。这个角度就成为了“旋转的角度”。**复数**是一个有趣的名字，就是说一个数字有实部和虚部两个部分。它们被写作 a + bi，其中：
- 
a 是实部
- 
b 是虚部
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOZV30cK7MpLC2XUNYNExja336pqP5HpCibs2RpiaZS4RaOxuEZIic1yqUw/640?wx_fmt=png)
这样还不错。但是有一个最后的问题：这个复数有多**大**呢？我们不能分开地计算实部和虚部的大小，这样就没有意义了。
让我们退回一点。一个负数的大小你也不能数出来——它是距离 0 的长度，在负数中：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOS1IEM5s3dnwv3ibIknUicT9L4T1OBBJic5icZtaiaibLjsKpvTSk8p3aehVg/640?wx_fmt=png)
这是另一种求绝对值的方式。但对于复数来说，当两个部分成 90 度时我们该如何去测量它的大小呢？
*它是一只鸟，是一架飞机，它是毕达哥拉斯！*
他的定理*[译者注：勾股定理]*出现在任何地方，甚至在他之后的 2000 年才发明了数字。我们制造了一种三角形，它的斜边就是距离 0 的长度：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOE6G0AVJvbXwJgibgJTMlpibtuTiaQlFpgRmuU68cLJ6HeAxiaxCVSxP24w/640?wx_fmt=png)
非常棒！尽管测量它的大小不像“丢弃掉负数的符号”那么简单，复数依然有它们的用处。让我们看一下它的一个实例。
## 一个实例：旋转
我们不必等到大学物理才去使用虚数，让我们**今天**就来使用它。关于复数的乘法有很多可说的，但把这个记在脑海里：
- 
通过乘以一个复数来旋转它的角度
下面让我们一探究竟。假设我有一条船，向东 3 个单位，向北 4 个单位的方向航行。我想将我的航行方向逆时针偏转 45 度，那么我的新方向是多少？
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHO798bOUgFXxogSicZxoZOibnuvr0Fs80XN2PwaAwXGsmxhubDOm5XgEIg/640?wx_fmt=png)
一些高手会说“这很简单！使用 sine，cosine...blabla...使用 tangent... blabla.. 以及...”。**崩溃**。抱歉，我打断了你的计算了吗？可以再重新关注一下那个问题吗？
让我们用一个简单的方法：我们现在航行在 3 + 4i（不用管什么角度，不需要关心），然后我们想偏转 45 度。那好，45 度是 1 + i（完美的对角线）,这样我就可以乘以这个数量！
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOIMt1Xbw4czE3VKK3eG9o9NZaDiciaicyGlnjE8CaKlsuyqKAd0u0UZ9Nw/640?wx_fmt=png)
这里是思路：
- 
初始的航行方向：向东 3 个单位，向北 4 个单位 = 3 + 4i；
- 
逆时针旋转 45 度 = 乘以 1 + i；
如果我们将它们相乘，可以得到：
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOycCdSvnem5m6pnFn1EWLhoqqeuvHhDCUp3UDKciafYibxUyy4eLpFBeg/640?wx_fmt=png)
所以新的航行方向是向西 1 个单位(即向东 -1 个单位)，向北 7 个单位，这样你就能画出来，并按照这个方向航行。
呀！我们在没有使用 sine 或 cosine 的情况下找到这个结果只花费了 10 秒钟。也没有用到向量、矩阵或者追踪我们所在的象限。它**仅仅**是一个使用了代数乘法的**算术**。旋转规则已经深深地嵌入了虚数中：**这种规则是有效的**。
更好的是，这个结果是**有用**的。我们航行在(-1, 7)而不是一个角度（arctan(7/-1)=98.13，记住我们在第二象限）。你计划画出或者跟随这个角度？在四周都用上量角器？
不行。你要把它转换为 cosine 和 sine (-.14 和 .99)，找到一个合理的比率（大约在 1 到 7 之间），然后描绘出这个三角形。而复数可以迅速、准确，且在没有计算器的情况下做这件事情。
如果你像我一样，你会发现这运用了**思维拓展****。**如果你没有...额...恐怕数学并不适合你。对不起...
三角学很伟大，但是复数可以让丑陋的计算变得简单（比如计算 cosine(a+b))。这篇文章仅仅是个预览，后续的文章会给你全餐。
**另外****：**一些人认为“Hey，用北/东指明航行方向来替代角度指明航行方向没有多大用处！”
不是吧？好吧。你看看你的右手。小手的中间到食指的尖端的角度是多少？自己算吧，祝你好运。
而用北/东航行法，你至少可以说：“横向距离 X 英寸，纵向距离 Y 英寸”，这样还有一些机会算出它的方向。
复数不“正常”
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkyooxWbmFwW0QBlVOicwkmHOgnOM88k1vl6sVoHTxHBraKAF96Eoo9SVWvPPsFWLQ26iadSqIkiaAIzQ/640?wx_fmt=png)
上面快速地过了一下我在复数上的基本观点。看看文章开头的第一个表格——现在应该能看懂了。
还有**非常多**美妙、好玩的数字，但是我的大脑已经很疲惫了。我的目的很简单：
- 
让你相信复数虽然被视为“疯狂”，但是很有用（就像曾经的负数那样）；
- 
展示了复数可以使某些问题变得简单，比如旋转。
如果我看起来对这个话题非常热心和关注，这里有个原因：我对虚数念念不忘很多**年**——对它缺少一种直观的见解，这让我很沮丧。
现在我终于有了这样的见解，我迫不及待地去分享它们。但如果你只是在一个狂热的人的博客上阅读了这篇文章，而不是在教室里，这会让我感到沮丧。我们束缚自己的疑问，缓缓前行——是因为我们没有去寻找、分享一些简洁、直观的见解。
但点亮一支蜡烛好过诅咒黑暗：这是我的想法，而且你们的其中之一也将会发出光亮。想想看，正是我们“解决”了像数字这样的问题才让我们能一直停留在罗马数字的大陆上。
## 后记：但是他们仍然很陌生!
我知道，复数对我来说，也依然很陌生。我试着把自己放到第一个发现 0 的人思维中。
0 是如此怪异的观点，有“0 个东西”却代表“没有东西”，罗马人避开了这个数字。复数也极其相似——它是一种新的思维方式。但是 0 和复数都让数学变得更加简单。如果我们从不采纳陌生的、新的数字系统，我们就只能靠手指数数了。
我重复了这个类比，是因为这样可以**很轻易**地去开始认为复数不“正常”。让我们打开思维：在未来，他们会为复数曾被不相信而轻声地笑：尽管都已经 21 世纪了。
∑编辑 | Gemini
来源 | cnblogs
 翻译/文之
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabky7x6u1VxMVMia4MLibNzC2nrumY3zDflTsCeoM04M1BrkvPny8tsw6hYkIicUr42iarLmadL2x6JwV6A/640?wx_fmt=png)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
