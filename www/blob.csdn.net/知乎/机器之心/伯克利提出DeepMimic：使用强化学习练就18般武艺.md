# 伯克利提出DeepMimic：使用强化学习练就18般武艺 - 知乎
# 



选自BAIR Blog，作者：Xue Bin (Jason) Peng，机器之心编译。

> 运动控制问题已经成为强化学习的标准基准，深度强化学习方法被证明适用于包括操纵和移动的多种任务。但是，使用深度强化学习训练的智能体通常行为不太自然，会出现抖动、步伐不对称、四肢动作过多等现象。而最近 BAIR 实验室将模仿真实动作片段与强化学习结合，令智能体能从参考动作片段中学习流畅和高难度的动作。

我们从计算机绘图中得到了大量灵感，基于物理的对自然动作的模拟数十年来已经成为广泛研究的主题。对动作质量的关注通常受电影、视觉效果和游戏应用的推动。这些年来，关于基于物理的人物动画的大量工作开发出控制器，可生成适用于大量任务和人物的鲁棒性、自然动作。这些方法通常利用人类洞察纳入特定任务的控制结构，从而为智能体可执行的运动提供强大的归纳偏置（这些智能体包括有限状态机、简化模型和逆动力学）。但是由于这些设计决策，控制器通常特定于某种智能体或任务，为行走开发的控制器可能无法扩展至更加动态的技巧（缺乏人类洞察）。 

在本研究中，我们将从两个领域中获取灵感，以利用深度学习模型的泛化性能，同时生成更自然的动作，可匹敌计算机绘图中全身运动模拟的当前最优水平。我们展示了一种概念层面上很简单的强化学习框架，使模拟智能体从参考运动片段中学习高动态、高难度动作，参考运动片段的格式是人类的动作捕捉数据。给出一个简单的技巧展示，如回旋踢或后空翻，我们的智能体能够学习一个鲁棒的策略来模仿该技巧。这些策略生成的动作与动作捕捉数据几乎难以区分。 




## **动作模仿**

在大部分强化学习基准中，模拟智能体使用简单模型来呈现，模型仅提供粗糙的现实世界动态近似。因此智能体倾向于利用模拟的特性开发出现实中不可行的不自然行为。通过结合更真实的生物工程学模型可以使动作更加自然。但是构建高保真模型难度非常大，而且得到的动作有可能还是不自然。 

另一种方法是数据驱动，人类运动的参考片段提供自然运动的样本。然后训练智能体通过模仿参考运动来生成更自然的动作。模仿运动数据的模拟在计算机动画中有很长的历史，近期也出现了一些使用深度强化学习的案例，如《DeepLoco: Dynamic Locomotion Skills Using Hierarchical Deep Reinforcement Learning》。尽管结果看起来更加自然，但是离生动再现大量运动还有很远的距离。 

本研究中，我们的策略将通过一个运动模拟任务来训练，智能体的目标是再现给出的参考运动。每个参考运动都以目标姿势序列的形式呈现：
![](https://pic3.zhimg.com/v2-5e8d8d3726532f7113f5b395b92d9b32_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='153' height='28'></svg>)
，其中 q_t hat 是时间步 t 的目标姿势。奖励函数旨在最小化目标姿势 q_t hat 和模拟智能体姿势 q_t 之间的最小平方误差。
![](https://pic2.zhimg.com/v2-371e35f7f2558ff323c026323bcff2f1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='429' height='114'></svg>)
尽管运动模仿应用了更复杂的方法，但是我们发现仅仅最小化追踪误差（和一些额外洞察）就可以达到非常好的效果。这些策略是使用 PPO 优化目标来训练得到的。 

有了这个框架，我们能够开发出适合大量高难度技巧（包括移动、杂技动作、武术和跳舞）的策略。 
![](https://pic3.zhimg.com/v2-407b65eeda154f1b9cd5937dcb25885e_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='358' height='160'></svg>)





![](https://pic4.zhimg.com/v2-c3fbc52af7015aa47126cb2fddd0672b_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='351' height='157'></svg>)





![](https://pic3.zhimg.com/v2-9ef9dfa26978f021f69986ca5fbba3d6_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='179'></svg>)


![](https://pic1.zhimg.com/v2-937fe7b9d29e1a30cb7d119eeca86954_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='179'></svg>)仿真机器人学习模仿不同技巧。蓝色智能体是模拟智能体，绿色智能体在重演对应的动作捕捉片段。从上到下分别为侧空翻、侧手翻、蹬足上和单手跨栏。
下一步，我们把现有方法与之前模仿动作捕捉片段的结果（比如生成对抗模仿学习（GAIL））进行了对比。我们的方法要比 GAIL 简单很多，而且能够更好地复现参考动作。由此得到的策略避免了深度强化学习方法很多常见的 artifacts，使得智能体的运动可以像人一样流畅。 
![](https://pic1.zhimg.com/v2-958c58b45cb4787c8e4f43e3d083d1d4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='993' height='449'></svg>)我们的方法（左）与 Merel et al. [2017] 方法的对比，后者使用 GAIL 模仿动作捕捉数据。相比之前使用深度强化学习的研究，我们的智能体动作显然更加自然。



## **洞见**

**参考状态初始化（RSI）**

假设智能体正在尝试模仿后空翻，那么它如何知道在半空中做一个完整旋转会得到高奖励呢？由于大多数强化学习算法是回溯性的，它们只能观察到访问过的状态奖励。在后空翻实例中，智能体必须先观察成功的后空翻轨迹，才能了解哪些状态能够得到高奖励。但由于后空翻对于起跳和着地的初始条件很敏感，智能体不大可能通过随机探索偶然性地执行成功的后空翻轨迹。为了给智能体一个提示，在每个 episode 开始时，我们把智能体初始化至从参考动作随机采样的状态。因此，智能体有时候会从地面开始，有时候会从后空翻的中间状态开始。这样即使智能体尚未熟悉如何达到这些状态，它们也能学习到哪些状态能获得高奖励。 
![](https://pic4.zhimg.com/v2-86351dcb74a18b89e63fea732f1015c3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='428'></svg>)RSI 通过将智能体初始化至从参考动作随机采样的状态，为它提供丰富的初始状态分布。 
以下是使用 RSI 训练和未用 RSI 训练的后空翻策略对比，在后者的训练中智能体总是被初始化到固定的初始状态，即后空翻动作的起始点）。没有用 RSI 训练时，智能体没有学习到「空翻」，只学到了「后」。 




**提前终止（ET）**

提前终止对于强化学习从业者来说至关重要，它通常用来提高仿真效率。如果智能体在某个状态被困住了，不再可能成功学习到动作，那么该 episode 将提前终止，以避免继续模拟。这里我们证明提前终止实际上对结果有显著影响。还以后空翻为例。在训练的早期阶段，其策略是很糟糕的，智能体大部分时间都在失败。智能体一旦摔倒，就很难再恢复到原来的状态，因此智能体大部分时间内都在地上挣扎。这和其它方法论如监督学习中遇到了类别不均衡问题相似。一旦智能体到达了这样的状态（例如摔倒），就终止这个 episode，以缓解该问题。ET 结合 RSI 有助于确保数据集的主要部分是由接近参考轨迹的样本构成的。没有 ET，智能体永远无法学会「空翻」，而只是摔倒然后在地上尝试表演这个动作。 

## **更多结果**

通过提供不同的参考动作，类人体总共已学会超过 24 种技能。 

除了模仿动作捕捉片段，我们还可以训练类人体执行其他任务，比如踢一个随机放置的目标，或者把球扔向目标。
![](https://pic2.zhimg.com/v2-edf679f46865a9687a620cb1bb734e2d_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='235'></svg>)训练向随机目标扔球的策略
我们也可以训练一个仿造的 Atlas 机器人模仿人类的动作捕捉片段，尽管 Atlas 的形态和质量分布非常不同，它依然可以重现预期的动作。该策略不仅可以模仿参考动作，还可以从相当显著的干扰中恢复过来。
![](https://pic4.zhimg.com/v2-a40de56e558980980babec5d981d8613_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='322' height='183'></svg>)训练 Atlas 来完成回旋踢，即使存在外部扰动，训练出的策略也极为稳健
但是如果我们没有动作捕捉片段，该怎么办？假设我们想要模拟霸王龙。因各种原因，捕捉霸王龙的动作不现实。所以，作为替代，我们可以用手绘的艺术动画来做关键帧，然后训练策略来模拟这些动作。
![](https://pic1.zhimg.com/v2-ec3c807a17799a4a1ea70194b8c76480_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='429'></svg>)模拟训练霸王龙，来模仿艺术创造的关键帧
而且不只是霸王龙，让我们训练一头狮子：
![](https://pic2.zhimg.com/v2-c6908137b2a8726ff6895f6a01feb79d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1076' height='472'></svg>)模拟狮子，参考动作由 Ziva Dynamics 提供![](https://pic4.zhimg.com/v2-1daae03a8c0d0b6eafcf0b016160de9b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='933' height='496'></svg>)用 418D 状态空间和 94D 动作空间模拟一条龙
这里想说的是，一个简单的方法表现都很惊人。只是最小化循迹误差（tracking error），我们就能训练掌握不同特征和技能的策略。我们希望该研究有助于启发为虚拟和现实机器人开发更多动态动作技能。探索从更多源（例如视频）中模拟动作也是令人激动的研究方向，例如动物和杂乱环境中比较难的动作捕捉。 

**论文：DeepMimic: Example-Guided Deep Reinforcement Learning of Physics-Based Character Skills**
![](https://pic3.zhimg.com/v2-e279990c2787a83c6335fb2a3b908602_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='280'></svg>)
论文链接：[https://xbpeng.github.io/projects/DeepMimic/index.html](https://link.zhihu.com/?target=https%3A//xbpeng.github.io/projects/DeepMimic/index.html)

摘要：角色动画（character animation）的一个长期目标是把行为的数据驱动性与可在物理模拟中执行相似行为的系统结合起来，从而逼真地回应扰动和环境改变。我们的研究表明，强化学习方法可用于学习模仿大量样本动作片段的鲁棒控制策略，同时也可以学习复杂的恢复，以适应形态的改变，完成用户特定的目标。我们的方法处理关键帧动作、高度动态动作（比如动作捕捉的翻转和旋转）以及重定向动作。 

通过结合动作-模仿（motion-imitation）目标与任务目标，我们在交互式设定中训练能够智能反应的智能体，例如向目标方向行走或朝用户指定的目标扔球等。这种方法具有使用动作片段的便利性与动作质量，以高效定义所需的风格和外观，还结合了 RL 方法和基于物理的动画所提供的灵活性和泛化性。我们进一步探索了大量将多个片段集成到学习过程的方法，以构建出能执行多个不同技能的智能体。我们还展示了使用多个对象（人、Atlas 机器人、双足恐龙和龙）和多种技能的学习结果，包括移动、武术和杂技等。
![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)
原文地址：[http://bair.berkeley.edu/blog/2018/04/10/virtual-stuntman/](https://link.zhihu.com/?target=http%3A//bair.berkeley.edu/blog/2018/04/10/virtual-stuntman/)


