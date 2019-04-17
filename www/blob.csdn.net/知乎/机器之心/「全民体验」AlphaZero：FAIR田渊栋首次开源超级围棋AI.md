# 「全民体验」AlphaZero：FAIR田渊栋首次开源超级围棋AI - 知乎
# 



> 现在，一块 GPU 的计算机就可以运行超越人类水平的围棋程序了。刚刚，Facebook 人工智能研究所（FAIR）的 ELF OpenGo 预训练模型与代码已经开源。田渊栋等人重现了 DeepMind 围棋程序 [AlphaZero](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757075%26idx%3D2%26sn%3Da12253a013431ece39814a2ca6975b46%26chksm%3D871a93adb06d1abb6ad5014b9dbcb2dc0345653fce6ae1d04a71c4446d87166ec37badf3fa02%26token%3D90907666%26lang%3Dzh_CN)，这是超级围棋 AI 的首次开源。与此同时，研究人员顺便也发现了人工智能围棋的一个「bug」。
- 项目：[https://facebook.ai/developers/tools/elf-opengo](https://link.zhihu.com/?target=https%3A//facebook.ai/developers/tools/elf-opengo)
- 论文：[https://arxiv.org/abs/1902.04522](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1902.04522)

对于有计算机基础的围棋爱好者来说，你也可以下载 ELF OpenGo 最终版本模型进行编译，体验与超越人类顶尖水平的 AI 围棋对战了（这需要你的电脑里有支持英伟达 CUDA 的 GPU）。

围棋程序链接：[https://dl.fbaipublicfiles.com/elfopengo/play/play_opengo_v2.zip](https://link.zhihu.com/?target=https%3A//dl.fbaipublicfiles.com/elfopengo/play/play_opengo_v2.zip)

田渊栋等人开源的 ELF OpenGo 基于 DeepMind 著名人工智能围棋 AlphaGoZero / AlphaZero，但无需使用谷歌 TPU 进行训练。OpenGo 的最新版本使用 2000 块 GPU 训练了 15 天实现了超人类的性能。在只有一块 GPU 的情况下，OpenGo 在与 4 个人类排名前 30 的的职业选手进行比赛时，在慢棋赛中取得了 20 比 0 全胜的战绩，人类棋手没有时间限制。

「我可以肯定地说，ELF OpenGo 项目为韩国围棋界带来了巨大影响，」韩国围棋协会公关助理负责人 Beomgeun Cho 表示，「自从它问世以来，几乎所有韩国的职业比赛选手都在使用 ELF Go 分析自己和其他棋手的对局。这不仅使得韩国棋手的水平提升，也对整个世界的围棋水平有很大帮助。」

ELF OpenGo 的最终模型是 2000 万局自我对弈训练的结果，其水平稳超人类。但正如 AlphaGo 曾经展示出自己在「打劫」问题上的 bug 一样，研究人员也发现了 ELF OpenGo 在训练过程中展示出了强化学习特有的限制：OpenGo 和 AlphaZero 一样无法完全理解围棋的「扭羊头」（又称征吃）概念，而这是围棋初学者就能理解的常见情况。与其他变化相比，征吃更多依赖于稍远期的预测。虽然对于人类棋手来说，预计未来 30 步或更多的征吃局面是轻而易举的，但 DeepMind 曾经指出这些预测是在计算机训练后期才能完成的。
![](https://pic4.zhimg.com/v2-dfbfb5f806bab4d2aa2290577db4e637_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='640' height='640'></svg>)围棋征吃的示意图。人类玩家很快就能学会预测这种局面的发展，但计算机学习的速度要慢得多，而且无法从个别例子中进行推广
有趣的是，研究人员发现 ELF OpenGo 以和人类玩家相反的方式学习围棋。其基于强化学习的方法更多关注游戏的后半段，而不是开局或中盘阶段。通过设置激励 AI 获胜的机制，强化学习会推动 OpenGo 更多地了解棋局如何收官——而非如何布局。与此同时，人类则倾向于评价当前棋盘上的局面，专注于近期变化和局部形式，同时逐步推进。

尽管这一研究结论仅限于围棋，但它也表明了[强化学习](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650757075%26idx%3D2%26sn%3Da12253a013431ece39814a2ca6975b46%26chksm%3D871a93adb06d1abb6ad5014b9dbcb2dc0345653fce6ae1d04a71c4446d87166ec37badf3fa02%26token%3D90907666%26lang%3Dzh_CN)具有某些局限性，可能会导致整体表现令人印象深刻却又遭遇失败——或被利用，因为 AI 过于关注最终结果，忽视了近期变化。

以下摘取介绍了 ELF OpenGo 论文的部分内容。
![](https://pic2.zhimg.com/v2-8f0d7c90c15b4c197b4a7016b2f09d79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='454'></svg>)FAIR 的 ELF OpenGo 分析了从 GoGod 数据集中抽取的 87,000 盘职业棋手对决与计算机落子的匹配程度，其时间跨度从 1700 年到 2018 年
围棋有超过 4000 年的传奇历史，并被视为一种最复杂的具有完整信息的回合制棋盘游戏。AlphaGo（Silver et al., 2016）及其后代 AlphaGo Zero（Silver et al., 2017）和 AlphaZero（Silver et al., 2018）的出现表明，即使没有人类对弈数据集的监督，深度强化学习（deep RL）也能达到超越人类的表现。

在下棋能力方面的这些进展具有显著的计算成本。单次训练过程就需要数百万局自我对弈，这需要在数千个 TPU 上训练数天时间；但研究社区中绝大多数人都无法获得这样的计算能力。再加上无法获得代码和模型，就导致这种方法非常难以甚至不可能得到重现、研究、改进和扩展。

我们在这篇论文中提出了 ELF OpenGo，这是针对围棋的 AlphaZero（Silver et al., 2018）算法的开源的再实现。我们还应用了 ELF OpenGo，实现了下列另外三个贡献。

第一，我们为 ELF OpenGo 训练了一个超人级模型。在 2000 个 GPU 上运行了我们的 AlphaZero 风格的训练软件 9 天时间后，我们的有 20 个模块的模型超越了人类水平，并且可以说达到了与 Silver et al. (2017) 和 Silver et al. (2018) 中描述的 20 模块模型相媲美的程度。为了助力这一领域的研究，我们将提供预训练过的超人级模型、用于训练该模型的代码、全面的训练轨迹数据集（包含 2000 万局自对弈，分为 150 多万个训练 minibatch）、以及辅助数据。我们将深入描述该系统和算法的设计，而且我们会包含很多在开发和训练我们的模型时得到的经验教训，我们希望这能帮助社区更好地理解大规模深度强化学习的很多考虑因素。

第二，我们会提供对模型在训练过程中的行为的分析。（1）随着训练的推进，我们观察到，与其它模型相比，该模型的棋力存在较大的变化。即使学习率下降了，这一性质依然会保持。（2）对于需要显著的前瞻才能决定是否应该下的落子（比如「征子」）, 模型学习的速度较慢，也永远不能完全掌握。（3）我们探索了模型学习一局对弈中不同阶段的高质量下法的速度。相对于 tabular RL（表格式强化学习）的典型行为，该模型学习游戏中期和游戏末期走法的进展速度基本相同。

第三，我们执行了广泛的 ablation 实验，以研究 AlphaZero 风格算法的性质。我们确定了几个 Silver et al. (2018) 中没有描述清楚的重要参数，并会给出它们对成功训练的作用的见解。我们简单比较了 AlphaGoZero 和 AlphaZero 的训练过程。最后我们发现，即使对于最终模型而言，在对弈中将 rollout 加倍依然能将其棋力提升大约 200 ELO，这说明 AI 的棋力会受到模型体量的限制。

我们的最终目标是提供必要的资源和探索性见解，让 AI 研究社区和围棋界都能研究、改进和测试这些很有前景的先进方法。
![](https://pic3.zhimg.com/v2-5e328c9c3c49839c12aca1946b5196ee_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='720' height='720'></svg>)OpenGo 分析了 19 世纪日本著名棋手本因坊秀策的名局「耳赤之局」，秀策执黑为世人称道的第 127 手置于 a 位，但人工智能认为黑棋应该下在 b 位。
**ELF OpenGo**

我们提出的 ELF OpenGo 的目标是忠实地重新实现 AlphaGoZero（AGZ）和 AlphaZero（AZ），消除原始论文中含混不清的地方，并提出多种创新让我们的系统可以完全在商品级硬件上工作。为了行文简洁，我们会在附录 A 中详细全面地讨论 ELF OpenGo 的系统和软件设计；亮点包括（1）在 GPU 上多个自对弈的工作器（worker）的共同安置（colocation），从而提升吞吐量，（2）一种异步的自对弈工作流程，用以处理越来越高的每局游戏延迟。

我们的训练和推理都使用了英伟达 Tesla V100 GPU，带有 16GB 内存；但是可以预期模型在大多数有 Tensor Core 的英伟达 GPU 上的表现都差不多（比如 RTX 2060 商品级 GPU）。
![](https://pic2.zhimg.com/v2-fed0c9d33b823df1cc69931a5051c549_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='467'></svg>)表 1：AGZ、AZ 和 ELF OpenGo 的超参数和训练细节。「?」表示 Silver et al. (2017) 或 Silver et al. (2018) 中模糊不清或没有明确指出的细节
总体而言，我们很大程度上遵照了 AZ 的训练细节。但我们没有使用 5000 个自对弈 TPU 和 64 个训练 TPU，而是使用了 2000 个自对弈 GPU 和 8 个训练 GPU。因为 Silver et al. (2018) 中没有明确说明 AZ 的 replay buffer（重放缓冲）大小，所以我们使用了 AGZ 的 50 万局游戏的设置。我们也使用了 AGZ 的每一步 1600 的自对弈 rollout 设置。
![](https://pic1.zhimg.com/v2-73cf6c14420218f1175c6df9c43acd3c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1052' height='644'></svg>)
图 2：训练期间模型能力的发展。「Selfplay ELO 25,000」和「Selfplay ELO 50,000」分别指基于间隔为 25000 和 50000 的训练 minibatch 的连续模型对而计算得到的未归一化的自对弈 ELO 等级。「Nash Averaging 50,000」是指纳什平均等级（Balduzzi et al., 2018），基于与「Selfplay ELO 50,000」中那样的同样模型之间进行轮询调度（round-robin）（成对的）竞标赛而计算得到。

**与人类对弈的比较**

因为我们的模型显著强于表现出了超人级棋力的原型模型，所以我们假设我们的模型的棋力也超过人类。在图 3(c) 中，我们表明该模型预测的下法与人类职业棋手给出的下法是一致的。这些下法是从 2011 年到 2015 年的 1000 场职业比赛中提取出来的。模型的人类下法匹配率很快在 minibatch 125000 左右收敛到了大约 46%。这表明超过这一点后该模型的棋力可能并不是由于更好的人类专业预测，而且正如 Silver et al. (2016) 表明的那样，使用人类对弈来监督训练可能存在局限性。

**MCTS rollout 数量 **

直观而言，增大 MCTS 迭代次数（rollout 数量）能通过探索博弈树中的更多部分来提升 AI 的棋力。为了更好地理解 rollout 数量对棋力的影响，我们使用最终模型进行了自对弈分析，其中一个棋手使用的 MCTS rollout 数是另一个的两倍。我们在范围广泛的 rollout 数量上执行了这一分析（800-25600）。
![](https://pic3.zhimg.com/v2-163b9d70321caf804fad8e91184297d6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1014' height='620'></svg>)图 9：使用 2 倍 rollout 的模型相对于使用 1 倍 rollout 的同样模型的胜率
结果如图 9 所示，可以发现当将执白子者的 rollout 数量翻倍时，ELF OpenGo 能稳定地享有 80%-90% 的胜率（ELO 大约多 250-400）。另一方面，当将执黑子者的 rollout 数量翻倍时，ELF OpenGo 的胜率仅有 55%-75%（ELO 大约多 35-200）。此外，随着 rollout 数量增大，执黑时 rollout 翻倍的增量收益缩减至近 50%，这说明我们的模型在执黑时在 rollout 数量方面存在一个能力的天花板。而在模型执白时这个天花板却不存在，这说明 7.5 贴目（白棋分数奖励）对黑棋有相当大的影响。

因为双方都使用同样的模型可能引入偏差（rollout 更多的一方能看到对方探索过的所有分支），所以我们还实验了原型/最终模型，同样也观察了类似的趋势——rollout 翻倍能为 ELO 带来约 200 的提升。

**论文：ELF OpenGo：对AlphaZero的分析和开放再实现**
![](https://pic2.zhimg.com/v2-699f111ed57d35b2dcbdf6b62461d475_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='224'></svg>)
[https://arxiv.org/abs/1902.04522](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1902.04522)

AlphaGo、AlphaGo Zero 和 AlphaZero 算法系列出色地表明了深度强化学习的能力，在复杂的围棋游戏上达到了超人级水平，而且自主性也越来越高。但是，研究界在理解和使用这些很有前景的方法上仍存在很多障碍。为了阐明尚未解决的谜题和促进未来的研究，我们提出了 ELF OpenGo，这是 AlphaZero 算法的一个开源的再实现。ELF OpenGo 是首个开源的超人级围棋人工智能，其在与全球顶级职业棋手的对弈中获得了令人信服的完美战绩（20:0）。我们应用 ELF OpenGo 进行了广泛的 ablation 研究，也识别和分析了模型训练以及对弈推理过程中很多有趣的现象。我们的代码、模型、自对弈数据集和辅助数据都会公开。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
*参考链接：*[https://ai.facebook.com/blog/open-sourcing-new-elf-opengo-bot-and-go-research/](https://link.zhihu.com/?target=https%3A//ai.facebook.com/blog/open-sourcing-new-elf-opengo-bot-and-go-research/)




