# DeepMind给AI出了200万道数学题，果然不如计算器哈哈哈哈 | 数据集出来了 - 知乎
# 



> 计算栗 郭一璞 发自 凹非寺 
量子位 报道 | 公众号 QbitAI

人生何处不数学？

这是越野车上的口算题。
![](https://pic1.zhimg.com/v2-3da45268ec7f31306968ca63656481a8_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='340' height='274'></svg>)
这是被小朋友玩坏的应用题。
![](https://pic4.zhimg.com/v2-a9ac2edad6fe42c0960fddada6d3179f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='666' height='378'></svg>)
额，这是别人家的奥数题。
![](https://pic3.zhimg.com/v2-2888c2a90e4638ded8987c04c80779c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='200'></svg>)△ 罗马尼亚数学大师赛，中国队全军覆没的一题
不过，现在是9102年了，几乎每天都有“AI超越人类”的新闻。所以，把我们中学时候写过的那些数学作业，扔给神经网络，它们做得出来么？

地球的另一个角落，**DeepMind**读懂了你的心，给神经网络们出了一套数学题，**200万道**。数据集已经放出来了。

算术，代数，概率论，微积分……不管是**算式**，还是**人类语言**描述的题目，只要能用文本写出来。

比如，这道排列组合 (中译版) ：

> 问：从这串字母qqqkkklkqkkk里面，不放回地取三个出来，求取出qql的概率。
答：1/110。

再比如，这道复合函数：

> 问：求g(h(f(x))), f(x) = 2x + 3, g(x) = 7x − 4, h(x) = −5x − 8。
答：−70x − 165

都是AI的数学考题了。

消息一出，人类纷纷欢呼了起来。天道好轮回，苍天饶过谁 (误) 。
![](https://pic3.zhimg.com/v2-8e66ad325a3fb1b6d2287e6fd5869b6e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='946' height='206'></svg>)
## **200万道，都有什么题型**

为什么突然想知道AI的数学好不好？

DeepMind说，AI和人类学数学的方法不一样。

我们主要是靠推理、学习、利用法则和运算符号，而AI依赖的是经验和证据。

举个耳熟能详的栗子，机器学习面试梗。

> 考官：你的强项是什么？
我：我是机器学习专家。
考官：9+10得多少？
我：3。
考官：差太远了，得19。
我：16。
考官：错了，是19。
我：18。
考官：不对，19。
我：19。
考官：录取你了。

AI的答案，是归纳出来的答案。

DeepMind觉得，没有人类的推理能力，AI学数学会很困难。可数学领域，对神经网络架构的研究来说，又很重要。

所以团队想要看看，用归纳来学数学，究竟怎么样。

那考试范围是什么？
![](https://pic2.zhimg.com/v2-ea996a30ff2ede23a56a30e148ceb869_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='350' height='263'></svg>)
最初的样本，是16岁以下 (大概是英国) 公立学校的数学课程。

团队把考纲扩展了一下，一共包括这几个方面：

**一是**代数，比如求解二元一次方程组，多项式求根，以及求数列的通项。

**二是**算术，比如四则运算，计算有特定顺序的 (比如带括号的) 的算式，把带根号的表达式简化一下之类的。

**三是**微积分和多项式求导。

**四是**比较，判断数的大小，从一列数里面找出和某个数最接近的数等等。

**五是**测量，比如在不同的长度单位之间转换，计算时间间隔之类的。

**六是**数字，找约数，四舍五入，整数的数位，因式分解，质数合数等等。

**七是**多项式运算，合并同类项等等。

**八是**概率，比如在一堆红白蓝色的球里面，选出红红白的概率。

200万题库是用刚才说的16岁以下教材样本，拿算法生成的。
![](https://pic4.zhimg.com/v2-5ff587b489db9aa4d99a0d21ab6f83db_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='225' height='225'></svg>)
所以，以上各类问题，是可以有机组合到一起的。这样就有趣了，因为许多数学法则，也是用各种概念合成的。

还是刚才的栗子，复合函数和求导，合在一起，就是复合函数求导。高中学过的，你还记得么：

***[f(g(x))]’=f’(g(x))g’(x)***

先求外面这一层，再求里面这一层，乘到一起就可以了。

坐等AI的答题结果。

## **都有什么选手**

DeepMind举办的考试中，有两位考生，一位是循环神经网络 (**RNN**) ，另一位是**Transformer**。

RNN一方派出的是**LSTM **(长短期记忆) ，有两个模型参加了考试。

第一个比较简单，直接把问题喂给LSTM，一次喂一个字符，模型就输出一个字符。

第二个复杂一些 (如下图) ，是带有**注意力**机制的编码器+解码器。这个模型在如今的机器翻译里面很常用：它不一定要按照字符输入的顺序来理解来计算，比如8/(1+3) ，就要先算1+3。
![](https://pic2.zhimg.com/v2-a18e47da3ca193698533e25c36f6cca5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='790' height='520'></svg>)
第二位考生，则是Transformer模型。作为一种seq2seq模型，它在机器翻译里的表现很好。


先来看一下它的结构：
![](https://pic4.zhimg.com/v2-a7e53dc6b49aaaeeff89fdfd1ab36ddf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='809'></svg>)
有一个编码器，可以把数学题矢量序列转换成一个长度相同的序列；还有一个解码器，生成预测的答案。

## **考试结果怎么样**

相比LSTM，Transformer模型要更胜一筹，两者有差不多数量的参数。

两个网络都没有进行太多“算法推理”，但Transformer模型比LSTM架构更适合学数学：

1、用同样多的参数，可以做更多的计算；

2、具有更浅的架构，更好的梯度传播；

3、有顺序内部存储器，可以更方便的设置数字序列等数学对象。

对AI来说，最简单的数学题是处理小数、整数，以及比较大小，还有不同模块组成的问题，比如：

> 已知k(c) = -611*c + 2188857，k(-103)! = 2251790是否正确？（否）

或者这种：

> 将-139/4, 40.8, -555, 607从小到大排列。

最难的数学题则是偏理论一些的题目，比如判断一个数字是否为素数，以及因式分解。

不过，Transformer模型依然能给出看起来有点靠谱的答案。

比如，把235232673分解为几个素数的成绩，它的答案是3,11,13,19,23,1487，正确的答案应该是3,13,19,317453。

虽然没答对，但是看起来长得很像呀。

另外，让Transformer模型直接算数，会发现，如果只让它算加减法，或者只算乘除法，它的准确率大概在90%，但是如果把加减乘除混合起来放到一道题里，它的准确率就只有50%了。
![](https://pic4.zhimg.com/v2-ac66f151739f55bb681255602d9c9d73_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='290' height='290'></svg>)
还真的不如计算器呀！

这证明，AI在算数学题的时候，完全靠归纳总结，并没有使用代数技能。

手里有的计算器，都不知道用，真是个老实人。

现在，你可以出去吹牛：

我数学比AI都好。

## **One More Thing**

很遗憾，以目前的结果，AI是不能替我们去考高数了。
![](https://pic1.zhimg.com/v2-ce72d2b2b321a334539fa4501ca50f94_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='719'></svg>)
不过，这项研究本身就不是在帮你考高数呀，DeepMind作为一个能造出AlphaGo的公司，应该理解不了学渣的痛苦。

搞懂了「AI答数学题，全靠归纳总结」，DeepMind就可以把相关原理拓展到其他更丰富的领域，那些需要归纳总结的问题，或许可以靠AI来解决了。

要不，下次试试让AI去答文科的主观题？

## **传送门**

☞论文
**Analysing Mathematical Reasoning Abilities of Neural Models**
David Saxton, Edward Grefenstette, Felix Hill, Pushmeet Kohli
[https://arxiv.org/abs/1904.01557](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1904.01557)

☞数据集
[https://github.com/deepmind/mathematics_dataset](https://link.zhihu.com/?target=https%3A//github.com/deepmind/mathematics_dataset)

— **完** —

量子位 · QbitAI

վ'ᴗ' ի 追踪AI技术和产品新动态

戳右上角「+关注」获取最新资讯↗↗

如果喜欢，请分享or点赞吧~比心❤


