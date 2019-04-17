# AlphaZero登上Science封面：从小白开始制霸多个游戏 - 知乎
# 



> DeepMind 推出的 AlphaGo 曾在围棋项目中取得了超越人类的表现，其研究曾经两次登上 Nature。近日，AlphaGo 的「完全自我博弈加强版」AlphaZero 的论文又登上另一大顶级期刊 Science 的封面。在论文中，[AlphaZero](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650734359%26idx%3D1%26sn%3D94e218848dcaff1c72fca46cbdc84eac%26chksm%3D871b3b69b06cb27f658f5ee9227b79623288dca458bb738a38d4d88d5f82c7537788302466e7%26scene%3D21%23wechat_redirect) 不仅征服了围棋，也在将棋、国际象棋等复杂游戏中实现了超越人类的表现。

此前不久，DeepMind 还推出了 [AlphaFold](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650753062%26idx%3D1%26sn%3D5c23afbc08a5e9fad2bb65416813452c%26chksm%3D871a8c58b06d054e5e96a556de6916dc040f3fd7f5377def8c88fbc1136e3a75095fe2730636%26scene%3D21%23wechat_redirect)，成功地根据基因序列预测出蛋白质的 3D 形状，将人工智能技术应用在了科学研究领域。

2017 年底，DeepMind 推出了 AlphaZero——一个从零开始自学国际象棋、将棋和围棋的系统。它最终在这三个领域都打败了世界最顶尖的程序。DeepMind 为这些初步结果而感到兴奋，也很高兴看到棋坛社区成员的反应，他们在和 AlphaZero 的对战中看到了一种灵活多变的「非常规」、突破性博弈风格，这种风格不同于之前的任何棋类程序。

今天，DeepMind 将为大家介绍 AlphaZero 的全面评估，此次评估确认并更新了那些初步结果，并已发表在 Science 杂志上。尽管 AlphaZero 是从随机游戏开始训练，并且只知道基本的游戏规则，没有内置专业知识，这一份评估与论文描述了 AlphaZero 如何快速学习每种棋类游戏并成为最强玩家。

前国际象棋世界冠军 Garry Kasparov 评价道：

> 我无法掩饰对它的欣赏，它下棋的风格灵活多变，和我很像！

这种不受人类玩法规范约束、从零开始学习每种游戏的能力产生了一种与众不同但富有创造力和活力的非传统下棋风格。国际象棋大师 Matthew Sadler 和 Natasha Reg 在他们即将出版的新书《Game Changer》（关于国际象棋的新书，将于 2019 年 1 月出版）中分析了数千场 AlphaZero 的国际象棋比赛。他们认为 AlphaZero 的棋风与任何传统的棋类程序都不相同。Matthew 表示，

> 「这种感觉就好像发现了一些过去棋坛高手的秘籍一样。」
[AlphaZero 在象棋、将棋和围棋比赛中的胜利_腾讯视频​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/c0809v7n857.html)
传统的国际象棋程序（包括世界计算机国际象棋冠军 Stockfish 和 IBM 开创性的 Deep Blue）依赖数以千计由人类玩家制定的规则和启发式方法，这些程序会试着解释游戏中每一种可能发生的情况。日本将棋程序也是将棋专用的，使用类似于国际象棋程序的搜索引擎和算法。

而 AlphaZero 采用了一种完全不同的方法，它用深度神经网络和通用算法取代了这些人工制定的规则，这些网络和算法的初始状态只知道基本规则。
![](https://pic4.zhimg.com/v2-560195d925f1e482ff4b1d63b80c1957_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='327'></svg>)
*在国际象棋中，AlphaZero 在 4 小时后首次超越了 Stockfish；在日本将棋中，AlphaZero 在 2 小时后首次超越了 Elmo；在围棋中，AlphaZero 在 30 小时后首次超越了打败李世石的那版 AlphaGo。注意：每个训练步代表 4096 个棋局。*

为了学习每种游戏，未训练的神经网络需要通过强化学习的试验和减少误差的过程进行上百万场自我对弈。起初，系统的走棋完全是随机的，但随着时间的增加，系统不断从赢、输和平局中学习经验，来调整神经网络的参数，使其在之后的棋局中选择更高胜率的走法。神经网络的训练时间依赖于游戏类型和复杂度，国际象棋需要 9 小时，日本将棋需要 12 小时，围棋需要 13 天。

> 系统的一些走法，例如把王将移动到棋盘的中心，这违反了日本将棋理论（从人类视角），似乎把 AlphaZero 推到了危险边缘。但令人难以置信的是，它仍然能掌控局面。它的独特走法预示着，日本将棋还存在其它的可能性。

已训练的神经网络被用于指导搜索算法（即蒙特卡洛树搜索/MCTS），来选择棋局中最有潜力的走法。在每一步中，相比传统棋类引擎，AlphaZero 仅搜索很少的走法。例如在国际象棋中，它仅每秒搜索 6 万种走法，而 Stockfish 每秒要搜索 6 千万种走法。
![](https://pic1.zhimg.com/v2-8be544d833d686ec54fdadcb94f9892c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)
训练完成的系统将通过和最强的人工引擎比赛进行测试，其国际象棋的对手是 Stockfish，日本将棋的对手是 Elmo，围棋的对手是 AlphaGo Zero（之前已知最强的围棋 AI 系统）。

每一个程序都在其最初设计的硬件上运行。Stockfish 和 Elmo 使用了 44 个 CPU 核心（正如在 TCEC 世界锦标赛中一样），而 AlphaZero 和 AlphaGo Zero 使用的是有 4 个第一代 TPU 和 44 个 CPU 核心的计算机。第一代 TPU 的推理速度和英伟达 Titan V GPU 大致相同，但两者的架构无法直接比较。

所有的比赛都采用了限时规则，每场比赛 3 小时，每一步棋限时 15 秒。

在每一场评估中，AlphaZero 都令人信服地打败了对手：

在国际象棋中，AlphaZero 打败了 2016 年 TCEC（第 9 季）世界冠军 Stockfish，在 1000 场比赛中赢了 155 场，仅输了 6 场。为了验证 AlphaZero 的鲁棒性，研究者还测试了一系列人类选手常见的开局。在每一个开局中，AlphaZero 都打败了对手。研究者还尝试了在 2016 年 TCEC 世界锦标赛中使用的开局，以及一系列和 Stockfish 最新版本的比赛，还有和使用了强劲开局的 Stockfish 变体的比赛。在所有比赛中，AlphaZero 都赢了。

在日本将棋中，AlphaZero 打败了 2017 年 CSA 世界锦标赛版本的 Elmo，赢了 91.2% 的棋局。

在围棋中，AlphaZero 打败了 AlphaGo Zero，赢了 61% 的棋局。
![](https://pic4.zhimg.com/v2-f55149dac67dcabf1d13e37f00ce955f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='668'></svg>)
但是，AlphaZero 的下棋风格或许是最令人类选手着迷的地方。拿国际象棋来说，AlphaZero 在自我对弈（self-play）训练中独立发现和执行人类的常见招数，比如开局（openings）、保王（king safety）和兵形（pawn structure）。通过自我学习并且不受限于传统的博弈智慧，AlphaZero 可以开发自己对博弈的认知和策略，添加大量新的有趣招数，从而扩充了数个世纪以来对国际象棋策略的认知。

> 一个世纪以来，国际象棋被作为人机感知的罗塞塔石碑。AlphaZero 更新了古老棋盘游戏和前沿科技之间的联系。

Matthew Sadler 称，选手首先注意到的是 AlphaZero 的风格，即「它用棋子围住对方国王的方式」。支撑这种风格的是 AlphaZero 灵活多变的游戏玩法，它最大程度上利用了己方棋子的活跃度和移动性，同时尽可能抑制对方棋子的活跃度和移动性。然而，AlphaZero 似乎对「子力」（material）不那么重视。现代国际象棋中每个棋子具备一个值，如果一名选手在棋盘上棋子的值大于对手，则他拥有子力优势（material advantage）。而 AlphaZero 会在比赛刚开始时就牺牲子力来获取优势，但长期来看这种做法得不偿失。

「令我印象深刻的是，AlphaZero 能够在非常广泛的位置和开局中展示自己的下棋风格。」Matthew 说道。他还观察到 AlphaZero 从第一步开始就以「人类的一贯宗旨」用非常审慎的风格下棋。

「传统程序非常强悍，很少犯明显的错误，但是当面对没有具体、可计算解的位置时会慌乱。而 AlphaZero 在面对这些需要「感觉」、「洞见」或「直觉」的位置时依然镇定自若。」

在最近 Magnus Carlsen 和 Fabiano Caruana 的世界国际象棋冠军赛中，这一其它传统国际象棋程序所不具备的独特能力被用于为国际象棋迷提供实时见解和评论，《Game Changer》一书将继续探讨 AlphaZero 的这一能力。Natasha Regan 表示：「看到 AlphaZero 的分析与其他顶级国际象棋程序甚至顶级大师的区别非常令人振奋，AlphaZero 可以成为整个国际象棋社区的强大教学工具。

AlphaZero 的教学能力从 2016 年 AlphaGo 挑战围棋冠军李世乭的比赛中就可见一斑。在博弈期间，AlphaGo 下了许多手具有高度创造性的棋招，包括第二场比赛的第 37 手，这些棋招推翻了数百年的围棋思维定势。这些及其它很多棋招已经被包括李世乭在内的棋手仔细研究过。李世乭在谈论第 37 手棋时说：「我本来认为 AlphaGo 是基于概率进行计算的，它只是一台机器。但是当我看到这一手棋后，我改变了想法，AlphaGo 确实具有创造力。」

> 其影响远远超出了我心爱的棋盘……不仅是因为这些自学成才的专家机器表现得非常出色，还因为我们可以从它们产生的新知识中学习。

和围棋一样，AlphaZero 对国际象棋的创造性反应同样令人兴奋，因为自计算机时代以来，这对于人工智能来说一直是一个巨大的挑战。早期的先驱者巴贝奇、图灵、香农、冯·诺依曼都曾尝试手动设计国际象棋程序。AlphaZero 的意义远不止是国际象棋、将棋或围棋。为了创建能够解决大量现实问题的智能系统，我们需要它们能够灵活适应新情况。虽然在实现这一目标方面取得了一些进展，但在人工智能研究领域创建能够以非常高的标准掌握特定技能的系统仍然是一个重大挑战，因为当任务稍有改动时，系统往往会失败。

AlphaZero 掌握了三种不同的复杂博弈游戏，并且有潜力完成任何完美信息博弈（perfect information game），因此 AlphaZero 是解决该问题的重要一步。AlphaZero 表明单个算法也能在一系列设定下学习新的知识。虽然目前它还处于早期阶段，但 AlphaZero 的创造性见解及其在 AlphaFold 等其它项目的优秀结果令 DeepMind 团队对构建通用学习系统充满了信心。这种通用学习系统也许有一天能在一些重要且复杂的科学问题上帮助我们找到新的解决方案。

**论文：A general reinforcement learning algorithm that masters chess, shogi, and Go through self-play**
![](https://pic4.zhimg.com/v2-7712ab7efe7a16bcdafeda493887890b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='897' height='370'></svg>)
**摘要**：棋类游戏是人工智能历史上研究时间最长的领域，其中最强的程序主要基于一系列复杂的搜索技术。这些搜索技术只适应特定的领域，且它们使用人工设计的评估函数，这些函数经过人类专家数十年的修正与精炼。相比之下，最近的 AlphaGo Zero 程序通过自我博弈及强化学习实现了超越人类的表现。在这篇论文中，我们将这一方法推广到单个 AlphaZero 算法，它在很多具有挑战性的博弈游戏上能实现超越人类的表现。从随机博弈开始，且除了游戏规则不提供任何领域相关的知识，AlphaZero 令人信服地击败了国际象棋、将棋（日本象棋）和围棋的世界冠军程序。

如果读者希望了解 AlphaZero 的完整资料，可以仔细阅读及实现这一启发性的工作：
- 阅读 Science 论文：[http://science.sciencemag.org/content/362/6419/1140](https://link.zhihu.com/?target=http%3A//science.sciencemag.org/content/362/6419/1140)
- 下载 PDF 版论文：[https://deepmind.com/documents/260/alphazero_preprint.pdf](https://link.zhihu.com/?target=https%3A//deepmind.com/documents/260/alphazero_preprint.pdf)
- 阅读 Science 社论文章：[http://science.sciencemag.org/content/362/6419/1087](https://link.zhihu.com/?target=http%3A//science.sciencemag.org/content/362/6419/1087)
- 阅读 Science 解读文章：[http://science.sciencemag.org/content/362/6419/1118](https://link.zhihu.com/?target=http%3A//science.sciencemag.org/content/362/6419/1118)
- 下载 AlphaZero 最擅长的 20 项博弈游戏：[https://deepmind.com/documents/259/alphazero_stockfish_top20.zip](https://link.zhihu.com/?target=https%3A//deepmind.com/documents/259/alphazero_stockfish_top20.zip)

*参考链接：*
[https://deepmind.com/blog/alphazero-shedding-new-light-grand-games-chess-shogi-and-go/](https://link.zhihu.com/?target=https%3A//deepmind.com/blog/alphazero-shedding-new-light-grand-games-chess-shogi-and-go/)


