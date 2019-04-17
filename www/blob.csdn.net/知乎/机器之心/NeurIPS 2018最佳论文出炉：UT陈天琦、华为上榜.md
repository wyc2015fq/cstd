# NeurIPS 2018最佳论文出炉：UT陈天琦、华为上榜 - 知乎
# 



> 经历了[改名](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650752070%26idx%3D1%26sn%3Dfa0e1a78c28df5ca82cf7f713f11ae6d%26chksm%3D871a8038b06d092e2fd505d41855d3b79f031002acf0fb47d13a31b868d64deb14609766bf43%26scene%3D21%23wechat_redirect)、[抢票](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650748057%26idx%3D1%26sn%3D055d227136f71e8626c42ed4340adc67%26chksm%3D871af0e7b06d79f1cae98d469e01178c8ef563f0986da8d7fa8a5bca7d12cc97954bd506036c%26scene%3D21%23wechat_redirect)和[论文评审](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650745817%26idx%3D1%26sn%3D9d41c0c8275698ca0f55f2b03080981c%26chksm%3D871aefa7b06d66b14c215dd8e8baed7df7d188c9101d7affc2e0c71c7f7a62127e7e93b6b17c%26scene%3D21%23wechat_redirect)等等风波的「预热」，第 32 届 NeurIPS 于当地时间 12 月 3 日在加拿大蒙特利尔正式开幕。机器之心有幸参与了「第一届 NeurIPS」。

*机器之心报道，机器之心编辑部。*

在大会第一天的 Opening Remarks 上，NeurIPS 2018 公布了本届大会的获奖论文。来自多伦多大学的陈天琦、麦克马斯特大学的 Hassan Ashtiani、谷歌 AI 的 Tyler Lu，以及华为诺亚方舟实验室的 Kevin Scaman 等人成为了最佳论文奖的获得者。

作为人工智能的顶级会议，NeurIPS 究竟有多火？首先，让我们看看参会人数的变化：2016 年有 5000 人注册参加该会议，2017 年参会人数飙升至 8000，今年参会人数近 9000，且出现了 11 分钟大会门票被抢光的盛况，仅次于 Beyonce 音乐会的售票速度。

在活动方面，今年新增了 Expo 环节，吸引了全球 32 家公司的参与。在 12 月 2 日的 Expo 中，这些公司共组织了 15 场 Talk&Pannel、17 场 Demonstration、10 场 workshop。此外，为期一周的整个大会包含 4 个 tutorial session、5 场 invited talk、39 场 workshop 等。
![](https://pic1.zhimg.com/v2-4448d34566529c1e07ced38ed0d2d314_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)
至于论文方面，NeurIPS 2018 共收到 4856 篇投稿，创历史最高记录，最终录取了 1011 篇论文，其中 Spotlight 168 篇 (3.5%)，oral 论文 30 篇 (0.6%)。

而这些论文涉及的主题包括算法、深度学习、应用、强化学习与规划等。大会程序委员联合主席表示，这些提交论文中，69% 的论文作者表示将放出代码（结果只有 44%），42% 将公布数据。
![](https://pic2.zhimg.com/v2-05f9c7543fd4eab1e26464675181af69_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='810'></svg>)接收论文主题 Top 10
介绍完大会基本信息，我们来看看今年的最佳论文：

## **4 篇最佳论文（Best paper awards）**

**论文：Neural Ordinary Differential Equations**
- 作者：Ricky T. Q. Chen, Yulia Rubanova, Jesse Bettencourt, David Duvenaud（四人均来自多伦多大学向量研究所）
- 链接：[https://papers.nips.cc/paper/7892-neural-ordinary-differential-equations.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7892-neural-ordinary-differential-equations.pdf)

**摘要：**本研究介绍了一种新的深度神经网络模型家族。我们并没有规定一个离散的隐藏层序列，而是使用神经网络将隐藏状态的导数参数化。然后使用黑箱微分方程求解器计算该网络的输出。这些连续深度模型（continuous-depth model）的内存成本是固定的，它们根据输入调整评估策略，并显式地用数值精度来换取运算速度。我们展示了连续深度残差网络和连续时间隐变量模型的特性。此外，我们还构建了连续的归一化流，即一个可以使用最大似然训练的生成模型，无需对数据维度进行分割或排序。至于训练，我们展示了如何基于任意 ODE 求解器进行可扩展的反向传播，无需访问 ODE 求解器的内部操作。这使得在较大模型内也可以实现 ODE 的端到端训练。
![](https://pic1.zhimg.com/v2-6098cd5654a8317bed443e72ee6e5b44_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='373' height='270'></svg>)
图 1：左：残差网络定义离散序列的有限变换。右：ODE 网络定义向量场，其连续变换状态。两张图中的黑色圆圈表示评估位置（evaluation location）。
![](https://pic4.zhimg.com/v2-347f0fe13c26d3e8314b4d9cab09901f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='426'></svg>)
图 4：归一化流和连续归一化流的对比。归一化流的模型容量由深度（K）决定，而连续归一化流可以通过增加宽度（M）来增加模型容量，这使其更加容易训练。
![](https://pic2.zhimg.com/v2-68ba93a5840f212c06396aa6b093bc79_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='996' height='321'></svg>)
图 6：隐 ODE 模型的计算图。

**论文：Nearly tight sample complexity bounds for learning mixtures of Gaussians via sample compression schemes**
- 作者：Hassan Ashtiani、Shai Ben-David 等（麦克马斯特大学、滑铁卢大学等）
- 链接：[https://papers.nips.cc/paper/7601-nearly-tight-sample-complexity-bounds-for-learning-mixtures-of-gaussians-via-sample-compression-schemes.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7601-nearly-tight-sample-complexity-bounds-for-learning-mixtures-of-gaussians-via-sample-compression-schemes.pdf)

本文作者证明了 
![](https://pic4.zhimg.com/v2-751b30119a524566d719a5463a4f1e53_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='98' height='30'></svg>)
 样本对于学习 R^d 中 k-高斯混合模型是必要及充分的，总变分距离的误差为ε。这一研究改善了该问题的已知上限和下限。对于轴对齐高斯混合模型，本文表明
![](https://pic1.zhimg.com/v2-52ab5260773c764f86fafad8771d6a0c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='94' height='33'></svg>)
样本是充分的，与已知的下限相匹配。其上限的证明基于一种基于样本压缩概念的分布学习新技术。任何允许这种样本压缩方案的类别分布也可以用很少的样本来学习。此外，如果一类分布有这样的压缩方案，那么这些分布的乘积和混合也是如此。本研究的核心结果是证明了 R^d 中的高斯类别具有有效的样本压缩。

**论文：Non-delusional Q-learning and value-iteration**
- 作者：Tyler Lu（Google AI）、Dale Schuurmans（Google AI）、Craig Boutilier（Google AI）
- 链接：[https://papers.nips.cc/paper/8200-non-delusional-q-learning-and-value-iteration.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/8200-non-delusional-q-learning-and-value-iteration.pdf)

**摘要：**本研究发现了使用函数近似的 Q-learning 和其它形式的动态规划中的一个基本误差源。当近似架构限制了可表达贪婪策略的类别时，就会产生 delusional bias。由于标准 Q 更新对于可表达的策略类作出了全局不协调的动作选择，因此会导致不一致甚至冲突的 Q 值估计，进而导致高估/低估、不稳定、发散等病态行为。为了解决这个问题，作者引入了一种新的策略一致性概念，并定义了一个本地备份过程，该过程通过使用信息集（这些信息记录了与备份 Q 值一致的策略约束）来确保全局一致性。本文证明，基于模型和无模型的算法都可以利用这种备份消除 delusional bias，并产生了第一批能够保证一般情况下最佳结果的已知算法。此外，这些算法只需要多项式的信息集（源于潜在的指数支持）。最后，作者建议了其它尝试消除 delusional bias 的实用价值迭代和 Q-learning 的启发式方法。
![](https://pic2.zhimg.com/v2-5ea074ed8790af56132f5bf1671ec051_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='737' height='150'></svg>)
图 1：一个简单的 MDP，展示了 delusional bias。
![](https://pic3.zhimg.com/v2-03ad029bfd640fcff74a5faf8614b5be_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1005' height='341'></svg>)
图 2：在一个具有随机特征表征的网格世界中进行规划及学习。图中的「iterations」是指遍历状态-动作对，Q-learning 和 PCQL 除外。深色线：估计可达到的最大期望值。浅色线：贪婪策略所达到的实际期望值。

**论文：Optimal Algorithms for Non-Smooth Distributed Optimization in Networks**
- 作者：Kevin Scaman（华为诺亚方舟实验室）、Francis Bach（PSL 研究大学）、Sébastien Bubeck（微软研究院）、Yin Tat Lee（微软研究院）、Laurent Massoulié（PSL 研究大学）

- 链接：[https://papers.nips.cc/paper/7539-optimal-algorithms-for-non-smooth-distributed-optimization-in-networks.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/7539-optimal-algorithms-for-non-smooth-distributed-optimization-in-networks.pdf)

**摘要：**本研究考虑使用计算单元网络对非光滑凸函数进行分布式优化。我们在两种规则假设下研究该问题：1）全局目标函数的利普希茨连续；2）局部单个函数的利普希茨连续。在局部假设下，我们得到了最优一阶分散式算法（decentralized algorithm）——多步原始对偶（multi-step primal-dual，MSPD）及其对应的最优收敛速率。该结果重要的方面在于，对于非光滑函数，尽管误差的主要项在 O(1/ sqrt(t)) 中，但通讯网络（communication network）的结构仅影响 O(1/t) 中的二阶项（t 是时间）。也就是说，通讯资源限制导致的误差会以非常快的速度降低，即使是在非强凸目标函数中。在全局假设下，我们得到了一个简单但高效的算法——分布式随机平滑（distributed randomized smoothing，DRS）算法，它基于目标函数的局部平滑。研究证明 DRS 的最优收敛速率在 d^(1/4) 乘积因子内（d 是潜在维度）。

## **经典论文奖（Test of time award）**

去年的经典论文颁给了核函数加速训练方法，今年的经典论文也是一篇偏理论的研究论文，它们都是 2007 年的研究。

**论文：The Tradeoffs of Large Scale Learning**
- 作者：Le´on Bottou（NEC laboratories of America）、Olivier Bousquet（谷歌）
- 链接：[https://papers.nips.cc/paper/3323-the-tradeoffs-of-large-scale-learning.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/3323-the-tradeoffs-of-large-scale-learning.pdf)

该论文的贡献在于开发了一个理论框架，其考虑了近似优化对学习算法的影响。该分析展示了小规模学习和大规模学习的显著权衡问题。小规模学习受到一般近似估计权衡的影响，而大规模学习问题通常要在质上进行不同的折中，且这种权衡涉及潜在优化算法的计算复杂度，它基本上是不可求解的。
![](https://pic4.zhimg.com/v2-ba508c8de9285d6e240c2e1b8e8181f7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='311'></svg>)
表 2：梯度下降算法的渐进结果，倒数第二列为到达准确率的优化时间，最后一列为到达超过测试误差率 epsilon 的优化时间。其中 d 为参数维度、n 为样本数。

作为本次大会的受邀媒体，机器之心来到了蒙特利尔，参与了本次 NeruIPS 大会。未来几天，我们还将发来最新现场报道，敬请期待。
![](https://pic2.zhimg.com/v2-65dfb17d8c666837a6bdb48562516d89_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='776' height='576'></svg>)
NeurIPS 2018相关论文链接：

[NeurIPS 2018提前看：生物学与学习算法](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650753041%26idx%3D4%26sn%3Dc37a5318fdaa9e7a6d9ec94ee5c08079%26chksm%3D871a8c6fb06d0579fc4c4a1809f17179858e375bb1a830d1d095c514a029de12ebb755c5c694%26scene%3D21%23wechat_redirect)
[NeurIPS 2018，最佳论文也许就藏在这30篇oral论文中](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650753041%26idx%3D3%26sn%3D9fa5fb26cd697bbdc1b66dd935cb36c0%26chksm%3D871a8c6fb06d05790cbd2fdb6ef86eefb88d64bfcbd4285fbab4dd2ada3d56e51709c527ea2f%26scene%3D21%23wechat_redirect)
[NeurIPS 2018 | 腾讯AI Lab&北大提出基于随机路径积分的差分估计子非凸优化](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650753062%26idx%3D4%26sn%3D2882c2d07c63cc71191ed76405fae380%26chksm%3D871a8c58b06d054e445c6a4b0507d55c3cd2f44e99ba886d8cbe11cd47477f33fd1f7c42f25e%26scene%3D21%23wechat_redirect)
[NeurIPS 2018亮点选读：深度推理学习中的图网络与关系表征](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650752712%26idx%3D4%26sn%3D85f9c70c1dea66e0e67f2817634da897%26chksm%3D871a82b6b06d0ba0f7c28e8d39829384c3294a9bf0605ccaf58bb2eb8939e1687a848bb82e80%26scene%3D21%23wechat_redirect)
[NeurIPS 2018提前看：可视化神经网络泛化能力](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650752789%26idx%3D4%26sn%3D121f969feac41931e8f73adf73a71c0c%26chksm%3D871a836bb06d0a7d07351f71cdb69c53b00dbed1631044db6d8dc5bab623259764974054357e%26scene%3D21%23wechat_redirect)
[Google AI提出物体识别新方法：端到端发现同类物体最优3D关键点——NeurIPS 2018提前看](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650752922%26idx%3D3%26sn%3D7fc9f923bcd6322524dfcedc15c30405%26chksm%3D871a83e4b06d0af2969e41be14bcb47f3ddb2eb7b6f4f1d40e0492788ee66692c415bf9ad546%26scene%3D21%23wechat_redirect)
[MIT等提出NS-VQA：结合深度学习与符号推理的视觉问答](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749745%26idx%3D5%26sn%3D95f4237d6b8f1fac9f96e8c8691fe828%26chksm%3D871aff4fb06d7659552cac6653f3efc4cd037543a4a7cecfbe9f7a31081a318da574d9363220%26scene%3D21%23wechat_redirect)
[画个草图生成2K高清视频，这份效果惊艳研究值得你跑一跑](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650747202%26idx%3D1%26sn%3D9e4328401b67aae43254230b7745a7eb%26chksm%3D871af53cb06d7c2addcebc264cc45c59b8dd599284fb6f6f9f3ee737ad4e6499db0913705959%26scene%3D21%23wechat_redirect)
[下一个GAN？OpenAI提出可逆生成模型Glow](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650745032%26idx%3D1%26sn%3Da889433dd4c4d9f62bfab347909d9d28%26chksm%3D871aecb6b06d65a02625abdf4b21a2116251e311a49508db587b76ae8f76d7a9e03d4a6ab80a%26scene%3D21%23wechat_redirect)
[CMU、NYU与FAIR共同提出GLoMo：迁移学习新范式](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650743815%26idx%3D4%26sn%3Df03cab0a6e172a4abf2337bb6b940dd3%26chksm%3D871ae079b06d696f5fd477fbafc9ef89c69698680f692afd641586b26567cb9853937642d8be%26scene%3D21%23wechat_redirect)
[Quoc Le提出卷积网络专属正则化方法DropBlock](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751601%26idx%3D5%26sn%3D6ba09bea3acb116eb9f4902af5261e72%26chksm%3D871a860fb06d0f194c4c0452e53d21cc6c537b4e33a5aea4e3c0a067db0c46d41168afabcc0c%26scene%3D21%23wechat_redirect)
[Edward2.2，一种可以用TPU大规模训练的概率编程](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751806%26idx%3D5%26sn%3D492141cdf64aca6194267dbe9228e738%26chksm%3D871a8740b06d0e5664ce39f468f1f6723967e2088016f223d02235896c3ff37b96e281b80c43%26scene%3D21%23wechat_redirect)
[南大周志华等人提出无组织恶意攻击检测算法UMA](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751497%26idx%3D4%26sn%3D7d0a7fd832eac52bee56bef836700aec%26chksm%3D871a8677b06d0f61b2fb61d6c35bc47b7a5d3f350146f03e893fd7552c7ca35677cb5b02beb2%26scene%3D21%23wechat_redirect)
[MIT新研究参透批归一化原理](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751889%26idx%3D3%26sn%3Db3fce2d756c342d79218222590b94703%26chksm%3D871a87efb06d0ef994983e11f55f8205b7e5d2bfb08922b8438a0152cb6231fa1115bed77fe7%26scene%3D21%23wechat_redirect)
[程序翻译新突破：UC伯克利提出树到树的程序翻译神经网络](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751324%26idx%3D5%26sn%3Dae0bb6cc918e1f1da9a6ceb8890f10b1%26chksm%3D871a8522b06d0c3424677e0067ea17060b4f00cba105e75efdc8468341cabf423450a8da302c%26scene%3D21%23wechat_redirect)
[将RNN内存占用缩小90%：多伦多大学提出可逆循环神经网络](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650751177%26idx%3D3%26sn%3Dda21add720d1f7d76ce615bd887a8a80%26chksm%3D871a84b7b06d0da1e12402aab3facf56d102dc6b3a4c92600aca1470ddc99c27d8ffa1786e86%26scene%3D21%23wechat_redirect)
[哪种特征分析法适合你的任务？Ian Goodfellow提出显著性映射的可用性测试](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750932%26idx%3D4%26sn%3D9ba2cb01b067bc3ff580e6b77d858e73%26chksm%3D871afbaab06d72bc9f924e84c0bfcccebe53f3bd5a482419dd2d0451a53c055dad52991db26c%26scene%3D21%23wechat_redirect)
[行人重识别告别辅助姿势信息，商汤、中科大提出姿势无关的特征提取GAN](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750864%26idx%3D5%26sn%3D09a9188fbf547ac965ea9f8047627454%26chksm%3D871afbeeb06d72f8e0310177edebcb5c8d361c66a9df1747d5047957b60194fe595b33476ab1%26scene%3D21%26token%3D1494667857%26lang%3Dzh_CN%23wechat_redirect)
[作为多目标优化的多任务学习：寻找帕累托最优解](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650750805%26idx%3D4%26sn%3D2b9f8fa9d5654279beaf424abb41c30e%26chksm%3D871afb2bb06d723deb4a6a9537ac669c01490e75286911da73c4ac816d67dd48c78362f3082e%26scene%3D21%23wechat_redirect)
[Dropout可能要换了，Hinton等研究者提出神似剪枝的Targeted Dropout](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650752571%26idx%3D1%26sn%3D8417645148afd8eebdb79c91b37a7409%26chksm%3D871a8245b06d0b53115d79f1ce42bc5a03aad5d038fe51c2f237c5848c41c51c5b756aaa8937%26scene%3D21%23wechat_redirect)
[利用Capsule重构过程，Hinton等人实现对抗样本的自动检测](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650752465%26idx%3D4%26sn%3D1ba51dcedcfeb9b6e07414747b35839b%26chksm%3D871a81afb06d08b963ea3a837cddf1dc0dce673ea5b87316d873913f47084351751f74d07985%26scene%3D21%23wechat_redirect)


