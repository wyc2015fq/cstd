# Reverse原理背后的数学和魔幻艺术 - 算法与数学之美 - CSDN博客
2018年12月04日 21:00:00[算法与数学之美](https://me.csdn.net/FnqTyr45)阅读数：175
第一次变用这个原理的魔术已经有十几年了，看起来就是拿起一叠牌发来发去，最后总能发出一些规律来，比如每一叠顶部都是Ace，在说上一些应景的话语，形成一个寓意美好的ending。Reverse这个名字是我自己取的，取自python中的list翻转函数（当然不同语言中都有类似的操作啦），这可以看作是一个纯self-working的魔术原理了，联想了一些大师作品和自己的创作，发现这个品类实乃奇妙的数学和美丽的魔术结合的又一瑰宝。
数学原理
这再简单不过了，我们知道如果f(x) = f ^ - 1(x)，或者ff(x) = x，即一个函数存在反函数且和原函数相等时，那么在坐标轴上看起来应该是沿着y = x方向对称的，函数的方程表达式上看则应该是x，y在表达式上的地位应该相当，比如xy = 1的反函数和 x ^ 2 + y ^ 2 = 1的圆。（所谓相当就是互相替换以后式子含义等价，这个等价一般来源于乘法加法的交换律哦~再想想，乘法和加法的交换律，分配率的原理又是什么？文末留言答对有奖哈~）
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/FibaF9tMKEDkWOjgCib1fNbQxqBmyxpnpe4su5gAjNTic985o0CUI5bw33lk3epB2OIiaQq8cfcd0gqoxVibDKHg8eA/640?wx_fmt=png)
**图1 反函数等于原函数的例子：反比例函数和1 / 4圆**
这些是初中数学的知识点了，我们再抽象一点，把f看作是对象（不一定是数）上的一个操作（operation），这个操作同样有定义域和值域，以及对应关系，这个对应（x, y）的全体构成的集合也就定义了这个操作了。那么同实数上的函数，这个操作的性质则为：该操作和反操作的效果完全相同，或者，两个同样的操作以后会恢复原状！注意哦，这是可以早就两种完全不同魔术效果的原理哦，一个是“相同”，构造coincidence，还有一个是，还原（哈哈，这里不是撕牌还原的restoration啦，不要想偏了）！
巧了，上面描述的这个事情在数学上早就有名字了，他们统一叫做对称关系，即对任意关系集A，若(a, b)属于A则(b, a)也属于A，按理说这种元素倒转后形成的新元祖所在集合可以是A’，当且仅当A = A’（这种样子的表达式是不是经常见到，那个矩阵的逆和自身相等则为单位阵的结论是不是长的差不多？）时候，这个关系称之为对称关系。当然对称关系只是等价关系的条件之一，后面有机会再分析其他两个（传递性，自反性）。当关系满足一一对应的时候我们称之为函数关系，这里即对称函数，在不同领域有不同名字，比如实数上的函数叫沿y = x对称，空间上我们称之为镜像关系（mirror），而在扑克牌这里，我取名reverse操作，它是对称关系在序列空间上的具体形式，与镜像这些概念构成counterpart关系，当然，counterpart关系也是对称关系了。
于是，扑克牌手法中的普通的一张一张发牌（dealing），恰好等价于对扑克牌顶部的一部分牌执行reverse操作，嗯嗯，在程序员看来就是这样的：
#! /usr/bin/env python
# -*- coding:utf8 -*-
import numpy as np
import random
poker_list = range(54)
np.random.shuffle(poker_list)# shuffle the deck
n = random.randint(1, 54)# choose a position randomly
result_list = poker_list[:n]# get the pack from the top
result_list.reverse()# reverse it
我们可以看到，无论n为多少（这个选择其实很有限哦，信息量仅为54种可能而已，看起来自由其实束缚很大，一个隐形的限制就是，是从头开始的n张，前面若干张总是在你选择范围内。），其原来的顶牌在经过了两次reverse以后，依然会恢复到顶牌位置，而对于观众来说，如果表演得当，这些牌已经经过了充分的混乱和随机了，任何效果看起来都是那么不可思议！而做到这些效果，只需要想方设法，让这一操作的执行过程变得合理而有趣，这些就是魔术设计的艺术啦。
说了这么多，最后总结一下：扑克牌上的dealing手法等价于选序列头部进行reverse 操作，reverse是一个对称变换，反操作效果等价于原操作，操作两次可恢复原自变量值，而魔术效果也产生于这一性质。
嗯嗯，严谨的数学终于讲完了，下面是美妙的魔幻艺术。
不说废话，上表演！
魔术部分
第一个流程我印象很深刻，源于傅琰东老师在走进科学的一次访谈，嗯嗯，那时候我应该还是个10岁左右的翩翩少年。
**视频1. 4Ace聚首**
原理讲解：Reverse部分的变种之一，即，在执行时候发成二叠甚至更多，这样在每一叠顶部的牌恰为原来顶部若干张，注意哦，由于张数不一样，那些牌的排列是起点可能不相同的循环队列哦，队列长度是牌叠数量m，起始位置取决与发出张数n模m的值。当然这些都不重要啦，最后的效果是4张一样的Ace的组合，组合！谁会在这美妙的时刻去考虑他们的排列？（排列和组合是两种魔术效果，往往后者已经足够惊艳，而忘却了其实他们已经排列得很混乱了，以后的文章中还会有一系列效果用到了这个现象。）
表演关键：两个关键点，注意台词“一半多一点”让你有理由去放回2张，但是后面的表演已经要让这个不得不做的dirty work被让观众看起来更重要的流程盖掉乃至遗忘；另外，第一次发牌让观众停的时候，一定要发出去两张以后再说（当然如果只发了一张，那么发出的一叠和剩余顶牌就是Ace，那可能也是一个好结局吧）。
第二个流程也是来自我很小时候在一本厚厚的用照片拼成的魔术书上的，抱歉已经忘了书名和作者了，回家要是翻到了一定补上，致歉！
还真回忆起来的，此流程来自于张德金先生的《图说魔术入门》
**![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/FibaF9tMKEDkWOjgCib1fNbQxqBmyxpnpegkYvIXnpGZhY1fqiajNsGp1kx5v5wB7nBKs34XhdDvBsjb4KbblnWkA/640?wx_fmt=jpeg)**
**图2 张德金先生的《图说魔术入门》封面**
这本书至今应该还收藏在我老家爸妈的床底下，应该早就布满了灰尘吧。里面东西也没太大难度和深度，但至今我都视为至宝之一（嘿嘿，还有好几个宝贝呢~），因为她曾经是我童年的希望。
**视频2. 表里如一（4Ace聚首2）**
原理讲解：经过后来的学习才领悟到，这里用到了Dai Vernon 的 The trick that cannot be explained里面的一个经典思想，因为没有人知道我要做什么，所以只要看起来也还合理，还不太可能发生，甚至寓意还挺美好，那就是好魔术。像magician’s choice 和一些心灵类魔术也都由此原理而来。因此，一开始桌上5张牌，如果选了前4个Ace中的一个，没问题，如果选了最后一个，嗯嗯，会让他一直选到只剩下一张为止。现在桌上是Ace顶部3张Ace，一切就绪，执行reverse操作就可以了（第二次发成3叠）。
魔术对数学的画龙点睛，如此美妙！
最后还有一个流程，是最近看的一个shin lim的一个表演，让我久违地感叹魔术对原理改造以后的美的极致追求。就不再附上讲解啦，相信聪明的小伙伴们一定能够解析其中奥秘，获取理性和感性艺术的双重体验。
**视频3.All you have chosen**
怎么样，数学的原理多么简单而纯粹，魔术的展现多么挑战而美丽，他们合二为一的时候何不是天作之合呢！我仿佛在接受上帝赏给人类的文明和智慧一般地在接受洗礼，聪明的你，感受到了吗？
最后感谢腾讯的几位小伙伴帮忙拍摄和当托~
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkz5W6YnMibwvofSWKL71I7nsYgP6UiaPmnY2qF3Gsh83whYkL2AmDr0U2TwEtK1picKSNyb3bFpUMz5Q/640?wx_fmt=png)![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkz5W6YnMibwvofSWKL71I7nsCBr4Fc7e8H4uaZUtSJwX0mMTanh3JjCeqVk0ia1cLicfdSpr4aPfV9Nw/640?wx_fmt=png)
**作者简介**
![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/951TjTgiabkz5W6YnMibwvofSWKL71I7nsGia98EHJ9EkbmLic2stZ9fMrzsIHgtGj9sWydYrKvB565AWZibOmhhhfQ/640?wx_fmt=gif)
magic2728，现就职于腾讯。自幼以数学和魔术为最大爱好，从参加建模比赛到培训到一线互联网从业者，其建模经验遍历金融，生物，互联网；魔术表演从学校走向比赛和商演，又回归扑克牌魔术理论的研究。他通过建模来思考，参与和改造这个世界，也希望能同步把这些精彩分享给感兴趣的朋友们！
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkz5W6YnMibwvofSWKL71I7ns8n7OFtcZoG28lb0smKD9ly19gZHG3okJdnnDWsblZCdfG38yZf2VTQ/640?wx_fmt=jpeg)
**长按识别二维码，关注mathemagician！**
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/951TjTgiabkz5W6YnMibwvofSWKL71I7ns1f3Zh2Gcxn3eCS2VEhpVJAzCWrPUEHXKXa74rb70H8cKVon6pqmqzg/640?wx_fmt=png)
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/951TjTgiabkyYz3lMMNbPl0dUclk5ZD734Kj8DWohsobaLo6s6Du4t8OuyibNGicpMWnrTZyNSzrnQC9cnaiaicTejw/640?wx_fmt=jpeg)
算法数学之美微信公众号欢迎赐稿
稿件涉及数学、物理、算法、计算机、编程等相关领域，经采用我们将奉上稿酬。
投稿邮箱：math_alg@163.com
