# AI 学会你最拿手的小游戏之后 - 知乎
# 

本文由公众号 “**把科学带回家**” 提供

作者 美图君

一个懂写代码的程序员平常是怎么玩游戏的？想知道吗，一个网名叫做 Code Bullet 的澳大利亚程序员就经常自己在家制造和训练 AI 帮他打游戏。

看过他训练出的 AI 玩的游戏，感觉…再也不想玩游戏了呢。

先来看 AI 玩 Flappy bird 会怎样。

![](https://pic3.zhimg.com/v2-df5d9c04ff0622ced20a092c96b8412a_b.gif)
小哥使用的是NEAT（neuro evolution of augmenting topologies）算法，也就是增强拓扑的神经演化算法。

NEAT 是一种结合神经网络和遗传算法（genetic algorithm）的好算法。

遗传算法是什么？

遗传算法类似于一个代码进化系统，它是一种优胜劣汰 AI 的算法。

你要做的，就是告诉计算机，什么是“成功”的标准，还要告诉AI，它可以看到什么，可以做什么。成功的AI可以进行一次繁殖，留下后代，不成功的 AI 就当场去世了。

为了模拟演化，子代 AI 还会和爸爸/妈妈有一点点的不同，类似于遗传突变。

神经网络则是一种具有学习能力的算法，它模拟的是生物神经网络的结构，可以根据经验不断调整。

在 Flappy bird 里，小哥设定 AI 可以看到鸟鸟和柱子的距离、鸟鸟相对于柱子的速度等信息。剩下的该怎么玩 AI 自便。


![](https://pic3.zhimg.com/v2-ea488500d0623f9e8be3b30c0e2fbc4a_b.gif)

在每一代的 AI 里，玩得最好的可以进行繁殖，并把自己的优秀基因遗传给下一代。当然了，下一代也不会照单全收，还是会有小小的突变。

![](https://pic2.zhimg.com/v2-baa9f0556e5a31f1efecebd1df8130f9_b.gif)

第六代目就可以妥妥地拿下2000分了...

![](https://pic2.zhimg.com/v2-2df7df9a5568c8c8c6c342120a3860b5_b.gif)

好的，小哥用 NEAT 玩坏的第二个游戏，就是 Chrome 浏览器连不上网时候出现的彩蛋小游戏——跳跳恐龙。

![](https://pic4.zhimg.com/v2-40c2c5567c496ec8c1bbd064e765ea6f_b.gif)

你还不知道有这个游戏？

你能玩到第几关呢？

好的，发现自己技术太差以后，小哥就自行复制出了游戏，并开始无情的 AI 模式了。

第一代AI有500只，每一只控制着一只恐龙。发挥最好的 AI 可以进入繁殖阶段，不行的AI就莫得繁殖。而且下一代的策略会和上一代有一点点不同，类似于遗传突变。


![](https://pic1.zhimg.com/v2-8db2e1cf5d7b9d907b0de6355a2349b4_b.gif)

上面的那些网络就是控制这些 AI 的神经网络，它会一代代地发生变化。

这个神经网络的输入信号就是每个 AI 看到的东西，输出就是每个AI的动作。新的神经网络连接的出现代表着 AI 学会了某种特定的策略。

第一代的基本设定很简单，就是不停地随便乱跳。就和你一开始玩的时候一样。如果你仔细看上面的图，会发现有只AI是一直趴着前进的。

_(:з」∠)_

就这样，傻 fufu 的 AI 被一代代刷掉之后，终于有只 AI 搞明白要看清楚距离再跳了。

![](https://pic2.zhimg.com/v2-dfa0dc22e501aaeebe38934078c24b05_b.gif)

好的，玩过这个游戏的人知道，到了一定时间，速度会变得飞快，而且一些鸟鸟飞得太高踩不到，所以在大鸟鸟面前要主动弯腰。

利用遗传算法训练到第八代 AI 的时候，它们看起来就已经很聪明了。

![](https://pic2.zhimg.com/v2-8c6a9f27a22a0fd858faad7b2e780fd5_b.gif)

但是第八代还是没学会在踩不扁的大鸟鸟面前低头。

![](https://pic4.zhimg.com/v2-a5eb9c52be3d5e666ec70ecd25a34893_b.gif)

到了25代，AI 终于学会了，如果你不能碾压别人，那就要立刻向大佬低头，晚0.1秒都不行。

![](https://pic2.zhimg.com/v2-d9d3a89a9a7471b605a700fd95ca4245_b.gif)

到了第40代AI，见识一下什么叫做恐怖。

![](https://pic4.zhimg.com/v2-e3763d89a42761f6c42d2b42c50498e3_b.gif)

第25代 AI 妥妥地拿下了2万分。后来因为小哥的电脑实在吃不消了才没有继续刷出高分。

还有这个世界最难游戏。

![](https://pic4.zhimg.com/v2-22bc5233177ca8d813b458e771686c43_b.gif)

小哥也是因为自己玩得太烂，于是决定用遗传算法算法来海选 AI 帮自己刷分。

每个 AI 依照一套指令（术语叫做 vector）。

和自然选择理论一致，适合度最高的 AI 可以繁殖，做不到的 AI 就只能去死了。小哥规定，AI 要在有限的步数之内完成特定的指令。完成的指令越多，就可以获得更多的步数。

一伙 AI 冲上场的画面。有不怕死的已经当场去世了。

![](https://pic3.zhimg.com/v2-6eb41078454504e008ba26e0d8aab3fe_b.gif)

注意到头是绿的 AI 吗？它们是上一代 AI 里最优秀的后代。

![](https://pic1.zhimg.com/v2-b8d6a36564ee382d4aad2bd7bf6c90bc_b.gif)

终于到了第50代，第50代啊同志们，有一个AI来到了终点。

![](https://pic1.zhimg.com/v2-c9566424672afbe5846faa9fb1ac20bc_b.gif)

老母亲的泪目。

也是用 NEAT 算法训练的 AI 玩越野赛车。

从没眼看的个位数代目——

![](https://pic1.zhimg.com/v2-06a228b0cd2a9751279db5a68cddfd6c_b.gif)

![](https://pic2.zhimg.com/v2-bb48c407f48fb73a0661304255bf82b9_b.gif)

![](https://pic4.zhimg.com/v2-bbfc35e423a9c8293b5c1ed71d3152d3_b.gif)

再到彪悍的十七代目——

![](https://pic4.zhimg.com/v2-9b9be99f554304f9fb168682b017a5e3_b.gif)

再来看看 AI 玩贪吃蛇。

小哥先利用遗传算法和深度学习（deep learning）训练出了几条优秀的 AI 贪吃蛇，然后利用融合（fusion）的方法把它们的“大脑”合体。相当于一个贪吃蛇的身体里藏着好几条蛇。人类是不是管这个叫做精分？

![](https://pic1.zhimg.com/v2-413425e3211136fd8f5d41cca9f9e7a0_b.gif)

小哥另外让高级版的贪吃蛇 AI 在不同贪吃蛇 AI 附体时对外展现出不同的颜色。

![](https://pic4.zhimg.com/v2-f868814205c52925a4c20f3df4deda6f_b.gif)

比如，在发现食物，并且确定能够吃到的时候变成绿色，蓝色表示它确定能离食物越来越近，白色表示它看不到食物在哪里，只能尽量不让自己死掉。

这个精分版的贪吃蛇 AI 是这样玩的。

![](https://pic1.zhimg.com/v2-b5d000c17e4969fb3ca9b6aad0d42e48_b.gif)

![](https://pic3.zhimg.com/v2-0d00445958463712c2de034214e54356_b.gif)

总之，小哥用 NEAT 算法的套路玩转了各种你耳熟能详的小游戏。效果都是杠杠的。

除了 NEAT 算法，他还用其他算法破解了另一些游戏。

比如，小哥还教 AI 玩55 x 55 x 55的魔方。

![](https://pic3.zhimg.com/v2-b79f902cd69f9bbb34751d976795bd52_b.gif)

![](https://pic2.zhimg.com/v2-2878aad1a8150efe9a9e46980c5d6dc9_b.gif)

![](https://pic3.zhimg.com/v2-d853a413108a23ea512069b4db45deb2_b.gif)

小哥说不能做更大的魔方了，它电脑要中风了。

小哥用Q学习（Q-Learning）练的玩钢琴块游戏的 AI。

![](https://pic1.zhimg.com/v2-a61585034f95ecd13c21527d69b65f30_b.gif)

Q学习属于增强学习算法，增强学习算法就是要记录学习过的策略，并且根据奖励（目标）来调整策略的算法。

AI 的目标是找到屏幕上的黑框，然后使劲地摁就好了。一键玩到电脑中风的样子——

![](https://pic3.zhimg.com/v2-857415833dbc205a64d1a24adc7d738e_b.gif)

你玩过扫雷吧。小哥也造了一个扫雷 AI。看 AI 玩 64 x 36 的扫雷觉得莫名舒爽有木有？

![](https://pic3.zhimg.com/v2-486ceaf6ac0a27359bd670fc96d5601e_b.gif)

![](https://pic4.zhimg.com/v2-f6a0ad511fb488cf5680268b840e52bf_b.gif)

![](https://pic2.zhimg.com/v2-b68f2632f5829444ae40e2f5fd01b88d_b.gif)

![](https://pic4.zhimg.com/v2-d6527a6546971e39f7b4d0a14642d9bb_b.gif)

好的，看完了上面这些 AI 打游戏的精彩片段我们知道，永远也不要嘲笑一个会繁殖的 AI 和一个玩不好游戏的程序员。


