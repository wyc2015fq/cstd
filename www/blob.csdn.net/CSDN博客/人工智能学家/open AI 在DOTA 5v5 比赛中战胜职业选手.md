# open AI 在DOTA 5v5 比赛中战胜职业选手 - 人工智能学家 - CSDN博客
2018年08月06日 23:25:17[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：119
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jIkB6Erpz54Ux1WRtATY10eIibiblElhvV3yjOVZxo8D8xiaem5ST6Q7wLg/640?wx_fmt=png)
来源：AI科技大本营
摘要：去年，OpenAI 在 DOTA 的 1v1 比赛中战胜了职业玩家 Dendi，而在距离进阶版 OpenAI Five 系统战胜人类业余玩家不过一个月的时间，今天凌晨，它又以 2:1 的战绩再次完成对人类高级玩家的“屠杀”，GG（人类赢的最后一局纯属耍赖）。
去年，OpenAI 在 DOTA 的 1v1 比赛中战胜了职业玩家 Dendi，而在距离进阶版 OpenAI Five 系统战胜人类业余玩家不过一个月的时间，凌晨，它又以 2:1 的战绩再次完成对人类高级玩家的“屠杀”，GG（人类赢的最后一局纯属耍赖）。
相比之下，人类这次输给的是怎样的进阶版“AI 英雄”？
此次，OpenAI Five 对阵 5 个高级玩家（解说员+前职业玩家）——Blitz, Cap, Fogged, Merlini 和 Moonmeander，他们的平均天梯分 6000 以上。反观 OpenAI Five，根据公开资料，它的实力相当于人类玩了 180 年的游戏，而且每天都与自己进行对抗学习，学习过程非常复杂，需要在 256 个 GPU 和 128,000 个 CPU 上运行扩展版本的近端策略优化（PPO）进行训练。
﻿![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jIgGSPlmSshawhmhzMFETxUUU5Y9OWudtUas160y1JSSgbPRZrfuvHJw/640?wx_fmt=png)﻿
它对每个英雄使用了单独的 LSTM（长短期记忆递归神经网络），并且没有人类数据，它会学习可识别的策略，这表明强化学习可以产生可实现规模的长期规划。
此外，就应用环境而言，不同于棋牌游戏的固定规则，像 DOTA2 这样的复杂视频游戏是 5v5 对决的战略游戏，况且，DOTA 游戏已经不断开发了十几年，游戏逻辑中有数十万行代码，且每两周更新一次，游戏语义在不断产生变化。
因此，AI 玩 DOTA 的难度可想而知，它首先需要解决以下四大问题：长时视野；局部观察状态；高维、连续的动作空间；高维、连续的观察空间。
**模型架构**
OpenAI Five 的每个网络都包含一个单层的、1024-unit 的 LSTM，它可以查看当前的游戏状态（从 Valve 的 Bot API 中抓取），并通过几个可能的 action heads 发出动作。每个  head 都具有语义含义，例如延迟动作的刻度数，选择哪一个动作，该动作在单元周围网格中的 X 或 Y 坐标等。Action heads 是独立计算的。
OpenAI Five 使用观察空间（observation space）和动作空间（action space）进行交互式演示。OpenAI Five 将世界视为 20000 个数字的列表，并通过发出一个包含 8 个枚举值（enumeration values）的列表来执行操作。通过选择不同的行动和目标，我们可以了解 OpenAI Five 如何编码每个动作，以及如何观察世界。下图是人类会看到的场景。
﻿﻿![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jIWXFdjXr4ukj0HZib0iaBgiaIO4iaYWd0xn90wN504ric3y8cAMhPnPN1YKQ/640?wx_fmt=png)﻿﻿
OpenAI Five 可以对与它所看到的相关的丢失状态片段做出反应。例如，直到最近，OpenAI Five 的观察区域才包括狙击手的技能范围（子弹落在敌人身上的区域）。然而，我们观察到 OpenAI Five 可以学习走出（虽然不能避免进入）狙击手的技能范围，因为当进入这个区域时，它可以看到自己的血量是在减少的。
**探索**
就算有学习算法能够处理较长的视野，我们仍然需要对环境进行探索。因为即使我们设定了各种限制，仍然有数百种道具、几十种建筑、法术、单元类型、长尾游戏机制，以及因此产生的各种组合，想要有效地探索这个巨大的空间其实并不容易。
OpenAI Five 可以从随机权重开始，从自我博弈中学习。 为了避免“策略崩溃”，智能体在训练的时候，80％ 的游戏都是自我对抗， 另外 20％ 则是与过去的自己进行对抗。在自我对抗时，英雄首先会漫无目的绕着地图游走。经过几个小时的训练后，智能体开始有了一些概念，例如建造、中路对线等。几天之后，他们始终采用基本的人类策略：试图从对手那里偷走 Bountyrunes等。 通过进一步的训练，它们可以熟练掌握 5 个英雄集中推塔的高级策略。
OpenAI Five 使用了 1v1 机器人里的随机化的方法 。它还使用了一个新的路线分配（lane assignment）策略。 在每个训练游戏开始时，他们随机地将每个英雄“分配”到一些 lane 的子集，在到随机选择的时间之前，如果英雄偏离这些路线，就会受到惩罚。
当然，也有奖励来帮助智能体探索环境，主要包括净值（net worth）、杀敌数（kills）、死亡数（deaths）、助攻（assists）、最后一击（last hits） 等指标。他们通过减少其他团队的平均奖励，来对每个智能体的奖励进行后续处理，以防止智能体找到正和博弈（positive-sum）的情况。
他们也对道具和技能构建进行了硬编码，同时，也通过脚本基线（ scripted baseline）引入了信使管理（ Courier management）。
**Rapid**
﻿![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jIBFNhat47ib1oMic1fGPtL7br36CuDKqWxkTiaf6uMLGvfsbNicmQP7Y1kg/640?wx_fmt=png)﻿
这个系统的实现使用了被称为“Rapid”的通用 RL 训练系统，它适用于任何多人模式环境。
﻿![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jILpgicvibyo6icWia20bSEmOVXp2zs90kjBzIn0h2eUIiaP8iaVHOgwfvZbqw/640?wx_fmt=png)﻿
训练系统分为 rollout workers，运行游戏副本，智能体（agent），用来收集经验，优化器节点（optimizer nodes）执行跨 GPU 组的同步梯度下降。每次训练还包括分别对训练机器人以及样本机器人进行评估的组件，以及监视软件，比如 TensorBoard，Sentry 以及 Grafana。
﻿![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jIprU8kicsTrYGHRLd33ZB63z28uibVuIH8lzRS8XyboEDlgsXibibWaZhzQ/640?wx_fmt=png)﻿
在同步梯度下降运算过程中，每一个 GPU 组件都会运算自己负责的批处理部分的梯度计算，随后整体梯度再进行平均计算。他们原本使用消息传递借口的规约算法进行平均计算，现在则使用英伟达的多卡通型框架 NCCL2 的封装函数来实行 GPU 并行计算以及网络间数据传输。同步 58MB 大小数据（用于 OpenAI Five 的参数）的延迟显示在表格之中，延迟时间足够低能满足大部分数据被进行并行运算的 GPU 标记。
**与人类的不同**
OpenAI Five 获取的信息和人类完全一致，但是系统能马上反应到类似位置、生命值以及物品更新情况等等人类玩家需要定时观察的信息。OpenAI Five 的平均 APM 在 150-170 之间(理论上最快可以达到 450 考虑到每四帧一动)，平均反应时间为 80 毫秒，比人类平均速度要快很多。
很多职业选手在去年 TI 结束后都使用 bot 进行训练。根据 Blitz 的说法 solo bot已经改变了人们对 solo 赛节奏的看法，bot 偏向于快节奏风格，现在大多数选手也已经使用快节奏风格来和 bot 抗衡。
﻿![640?wx_fmt=gif](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/BnSNEaficFAZ9LHlgpf8QfXL14hc3h1jI2w6yomkDtpbohwanwKafdKWplDeVApG9YaWTNxGZfJmaiadS2eIqiacw/640?wx_fmt=gif)﻿
AI 在 Dota2 中的节奏和执行力非常强了，这是不是意味着它没有优化空间了？当然不是， 此次的 OpenAI Five 还是有诸多限制，比如系统在进行最后一击时较弱，其客观优先级与一个共同的专业策略相匹配，获得战略地图控制等长期奖励往往需要牺牲短期奖励。
﻿
Open AI 方面称，在今年后续的 TI 表演赛上，还会有职业玩家继续挑战 AI，但结果想来也是实力“嘲讽”人类。或许，更让人期待的是，在 Dota2 这样的复杂游戏中，是否会出现“AI vs AI”的神仙打架比赛？
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
