# 不再需要动作捕捉，伯克利推出「看视频学杂技」的AI智能体 - 知乎
# 



选自BAIR，作者：Xue Bin (Jason) Peng 和 Angjoo Kanazawa，机器之心编译。

> 伯克利曾经提出 [DeepMimic](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650740688%26idx%3D1%26sn%3Df296a07a2195f115c5c51a1c45e15b09%26chksm%3D871ad3aeb06d5ab88ca3f558d533144d93bb132d54fae3c9fa5d61b820b353a86aca660bc65c%26scene%3D21%23wechat_redirect)框架，让智能体模仿参考动作片段来学习高难度技能。但这些参考片段都是经过动作捕捉合成的高度结构化数据，数据本身的获取需要很高的成本。而近日，他们又更进一步，提出了可以直接模仿 Youtube 视频人物高难度动作的新框架 SFV。
![](https://pic1.zhimg.com/v2-575c60e8957922c6cac72a983fa07d88_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='160'></svg>)从 YouTube 视频中学习技能的智能体
通过 SFV 学习到的智能体动作还原度很高，并且有很好的泛化至新环境的能力，例如从平地泛化到不规则地形。当然，仍然存在一些难以模仿的动作，例如某某鬼畜骑马舞。

无论是像洗手这样的日常任务还是惊人的杂技表演，人类都可以通过观察他人来学习一些令人难以置信的技能。随着 YouTube 等开源视频数据的激增，现在我们比以往任何时候都更容易找到感兴趣技能的视频片段。每分钟都会有 300 小时的视频传到 YouTube 上。但是，对机器人来说，从这些海量的视频中学习技能仍是一项非常艰巨的任务。大多数模仿学习方法都需要简洁的表征，比如从动作捕捉（mocap）中记录的表征。但是获取 mocap 数据相当麻烦，通常需要大量仪器。Mocap 系统容易局限于遮挡较小的室内环境，这会限制能够记录的技能类型。所以，如果智能体可以从视频片段中学习技能就再好不过了。
![](https://pic1.zhimg.com/v2-337a00f2d44cd23ffeb2511c9ae7e4f0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='652' height='357'></svg>)使用 Mocap 捕捉演员动作并复现 3D 角色的动作（电影《指环王》）
在本文中，我们展示了一个从视频中学习技能的框架（SFV）。通过将计算机视觉领域最先进的技术与强化学习相结合，我们的系统使智能体能够从视频中学习各种技能。给定一个单目视频，视频中有人表演侧手翻或后空翻等动作，我们的智能体能够学习在物理模拟中再现该技能的策略，而无需任何手动姿势注释。
[https://v.qq.com/x/page/h0739u33100.html​v.qq.com![图标](https://pic3.zhimg.com/v2-a772a2982020f0c43d39432a93d041da_180x120.jpg)](https://link.zhihu.com/?target=https%3A//v.qq.com/x/page/h0739u33100.html)
从视频中学习全身动作技能的问题在计算机图形学中受到了一些关注。以前的技术通常依赖于手工制作的控制结构，这些结构对要做的动作加了很大的限制。因此，这些方法限制了可以学习的技能类型，由此产生的动作也看起来非常不自然。最近，深度学习技术在 Atari 游戏和简单的机器人任务中进行视觉模仿取得了不错的结果。但是这些任务在演示和智能体的环境之间通常只有适度的领域变化，且主要是在动态相对简单的任务上进行持续控制。




**框架**

该框架包括三个阶段：姿势估计、动作重建和动作模仿。输入视频首先接受第一阶段姿势估计的处理，预测每一帧中角色的姿势。接下来，动作重建阶段将姿势估计结果固化为参考动作，并修复姿势估计结果可能带来的失真。最后，将参考动作输入到动作模仿阶段，利用强化学习训练智能体模仿动作。
![](https://pic4.zhimg.com/v2-d885588c0dfcd94507608230dd495b8f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='617'></svg>)
*整个流程包含三个步骤：姿势估计、动作重建及动作模仿。执行特定动作的人物视频片段和智能体模型充当输入，并学习一种控制策略，使智能体能够在物理模拟中再现该动作。*




**姿势估计**

我们在一个给定的视频片段中使用一个基于视觉的姿势估计器来预测动作执行者在每一帧中的姿势 q_t hat。姿势估计器建立在人体网格恢复（human mesh recovery）的基础上，后者使用弱监督对抗性方法来训练姿势估计器以预测单目图像中的姿势。虽然需要姿势相关注释来训练姿势估计器，但一旦训练完毕，姿势估计器就可以应用于没有任何标注的新图像。
![](https://pic3.zhimg.com/v2-6372ac4c65264c728ab77572c070b8f6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='862' height='373'></svg>)基于视觉的姿势估计器用于预测人物在每一帧中的姿势
**动作重建**

由于姿势估计器针对每个视频帧独立预测人物的姿势，因此各帧之间的预测可能不一致，导致出现抖动噪声。而且，尽管基于视觉的姿势估计器在近些年取得了显著进展，但它们仍然偶尔出一些大错，导致出现一些奇怪姿势。这些噪声可以呈现物理上无法模仿的姿势。因此，动作重建阶段的作用就是减轻这些噪声，以产生一些物理上更加可行的参照，使智能体更加容易模仿。为了做到这一点，我们优化了一种新的参考动作
![](https://pic4.zhimg.com/v2-6baa8da6fac36c16d83c021399936737_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='250' height='46'></svg>)
，以实现以下目标：
![](https://pic2.zhimg.com/v2-52db1a1a5176d72e16a384dac4ea6b91_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='319' height='60'></svg>)
公式中的 l_p(Q hat) 使得参考动作与原始姿势预测更加接近，L_sm(Q hat) 使得相邻帧的姿势更加相似，以生成更加连贯的动作。此外，W_p 和 W_sm 是不同损失的权重。

这一过程可以显著提高参考动作的质量，并且可以修复原始姿势预测里的大量噪声。
![](https://pic4.zhimg.com/v2-aa3a9313a77aec4461c544ae3918dc63_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='550' height='158'></svg>)动作重建前后的参考动作对比。动作重建修复了一些失真，使参考动作更加流畅
**动作模仿**

现在我们得到了参考动作  
![](https://pic4.zhimg.com/v2-ca7b8c7708b7420c31de747e022e23e7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='163' height='30'></svg>)
，接下来我们将训练智能体模仿该技能。动作模仿阶段使用与模拟 mocap 数据类似的强化学习方法。奖励函数鼓励策略将每一帧 t 中模仿智能体的姿势和重建参考动作 q^_t 之间的区别最小化，
![](https://pic1.zhimg.com/v2-3b488e71c3dd785698d69c03d95476e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='293' height='57'></svg>)
这种简单的方法效果非常的好，智能体能够学习多种高难度杂技技巧，且每个技巧都仅通过一次视频展示习得。
![](https://pic3.zhimg.com/v2-4ba4d679153a6b4c4002e53793b8af1e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='824' height='396'></svg>)智能体通过模仿视频片段学习多种技巧
**结果**

总之，我们的智能体可以从 Youtube 的多种视频剪辑中学习 20 多种不同的技能。
![](https://pic4.zhimg.com/v2-d5f71cbcfd5c5fe3681108a0b04de3ab_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='310' height='140'></svg>)我们的框架可以从视频演示中学习大量的技能组合
即使智能体的形态和视频中的人物通常很不相同，其学习到的策略仍然可以重现很多种技能。作为更加极端的形态差异的示例，我们还可以训练一个虚拟的 Atlas 机器人来模仿人类的视频剪辑中的动作。
![](https://pic4.zhimg.com/v2-0383a64e194f85dbc520f9d2b73383f7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='833' height='405'></svg>)虚拟仿真机器人（Atlas）可以通过模仿视频剪辑学习多种动作组合
使用虚拟智能体的一个好处是我们可以利用模仿过程来将其行为泛化到新环境中。这里我们模拟了让智能体学习适应不规则地形的动作，其中原始的视频剪辑记录的是在平地上的人物动作。
![](https://pic2.zhimg.com/v2-052fdad9e70e17ea93791e853305b5a5_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='277'></svg>)智能体在不规则的地形中也能做出模仿动作
即使新环境和原始视频的环境很不相同，学习算法依然发展出了非常合理的策略来应对新的环境。

总而言之，我们的框架真的仅仅是使用了任何人都能想到用来解决视频模仿问题的明显方法。关键在于将问题分解为更好掌控的组成，为这些组成选择正确的方法，并有效地将它们整合到一起。然而，视频模仿技能仍然是一个极有挑战性的问题，目前仍然有大量的视频剪辑是我们无法重现的。
![](https://pic2.zhimg.com/v2-2862a6a4d5634f6a1ea382930c9c51f9_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='400' height='120'></svg>)灵巧的舞步，例如江南 Style，仍然是很难模仿的
但令人鼓舞的是，仅通过整合已有的技术，我们就能在这个难题上走出一大步。




**论文：SFV: Reinforcement Learning of Physical Skills from Videos**
![](https://pic4.zhimg.com/v2-b2136ed5074ff07ab6ae9251e94d4cb7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='590' height='153'></svg>)- 项目展示页：[https://xbpeng.github.io/projects/SFV/index.html](https://link.zhihu.com/?target=https%3A//xbpeng.github.io/projects/SFV/index.html)
- 论文地址：[https://xbpeng.github.io/projects/SFV/2018_TOG_SFV.pdf](https://link.zhihu.com/?target=https%3A//xbpeng.github.io/projects/SFV/2018_TOG_SFV.pdf)

基于动作捕捉的数据驱动的智能体可以生成高度自然的动作，而且与物理模拟相结合时可以提供对物理干扰、环境变化和形态差异的自然过程响应。动作捕捉仍然是最流行的动作数据来源，但是收集 mocap 数据通常需要配备大量仪器的环境和表演者。在本文中，我们提出了一种方法，使智能体能从视频中学习技能（SFV）。该方法基于深度姿势估计和深度强化学习，允许数据驱动的模仿利用网络上的大量公开视频片段，如 YouTube 上的视频。这有可能通过查询所需动作的视频记录来实现快速而简单的人物控制器设计。所产生的控制器对干扰具有鲁棒性，能够适应新设置，执行基本的对象交互，并能通过强化学习重新定位新形态。我们还进一步证明，该方法可以通过对观察到的姿势进行初始化得到的学习控制器进行正向模拟，进而根据静止图像预测人物的可能动作。我们的框架能够学习各种动态技能，包括运动、杂技和武术。

原文链接：[https://bair.berkeley.edu/blog/2018/10/09/sfv/](https://link.zhihu.com/?target=https%3A//bair.berkeley.edu/blog/2018/10/09/sfv/)


