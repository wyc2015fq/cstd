# DeepMind集成AI智能体架构「MERLIN」：基于目标导向智能体中的无监督预测记忆 - 人工智能学家 - CSDN博客
2018年04月01日 00:00:00[人工智能学家](https://me.csdn.net/cf2SudS8x8F0v)阅读数：205
![640?wx_fmt=png&wxfrom=5&wx_lazy=1](https://ss.csdn.net/p?http://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0c1DRoCLLYHITn4Sr47n52qX1MmdkJia9zkVmbich9yLbhmph5jGQRxNSP1QicAwhaeL8HMpib7bDeDLQ/640?wx_fmt=png&wxfrom=5&wx_lazy=1)
来源：arXiv
摘要：在自然界中，动物往往会执行目标导向的行为，尽管它们的传感器的范围有限。
作者：Greg Wayne、 Chia-Chun Hung、David Amos、Mehdi Mirza、Arun Ahuja、Agnieszka Grabska-Barwinska、Jack Rae、Piotr Mirowski、Joel Z. Leibo、Adam Santoro、Mevlana Gemici、Malcolm Reynolds、Tim Harley、Josh Abramson、Shakir Mohamed、Danilo Rezende、David Saxton、Adam Cain、Chloe Hillier、David Silver、Koray Kavukcuoglu、Matt Botvinick、Demis Hassabis、Timothy Lillicrap
「雷克世界」编译：嗯~是阿童木呀、KABUDA、EVA
在自然界中，动物往往会执行目标导向的行为，尽管它们的传感器的范围有限。为了应对环境问题,它们会对环境进行探索并储存记忆，保持对目前无法获得的重要信息的估计。类似地，最近，DeepMind提出了MERLIN，这是一种集成的AI智能体架构，它可在部分观察到的虚拟现实环境中操作，并基于不同于现有的端到端的AI体系的原理将信息存储于记忆中。
在自然界中，动物往往会执行目标导向的行为，尽管它们的传感器的范围有限。为了应对环境问题，它们探索环境并储存记忆，保持对目前无法获得的重要信息的估计。
最近,人工智能(AI)智能体在这方面取得了进展,它们通过将强化学习算法与深度神经网络相合并，学会从感官输入执行任务，甚至达到了人类水平。这些结果激起了科学家们对相关想法的追求，比如解释非人类动物的学习。
然而，我们证明了，当有足够的信息隐藏在智能体的传感器中时，当代的强化学习算法很难解决简单的任务，这种属性被称为“局部可观测性”（partial observability）。处理局部观察到的任务的一个明显的需求是访问大量的记忆，但是我们发现记忆不够，正确的信息以正确的格式进行存储是至关重要的。
我们开发了一个模型，记忆、强化学习和推理网络（Memory, RL, and Inference Network，MERLIN）。在这个模型中，记忆的形成是由一个预测建模的过程进行指导的。MERLIN使在3D虚拟现实环境中解决任务变得容易，在这种环境中，局部可观测性是非常严重的，而且必须在长时间内保持记忆。我们的模型展示了一个单一的学习智能体的架构，它可以解决心理学和神经生物学中的典型行为任务，而不需要对感官输入的维度和经验的持续时间做出强力的简化假设。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0c1DRoCLLYHITn4Sr47n52qjGVxWZLiaKzvdQO7c5qMdOt8HoJTWYv9pq7htHicYqb8RQtUZtaRnYKw/640?wx_fmt=png)
智能体模型
现如今，人工智能研究正在经历一场复兴，因为强化学习技术，解决了优化序列决策的问题，与深度神经网络相结合形成人工智能体，从而可以通过处理复杂的感官数据，做出最佳决策。同时，科学家们开发了新的深度网络结构，将重要的先验知识编码到学习问题中。其中一个重要的创新是利用外部记忆系统开发神经网络，使得来自大量的历史事件中合成信息的计算能够被学习到。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0c1DRoCLLYHITn4Sr47n52qrB6l8w0eWAYJo92ia5fKPKDibRWWh2JAvIt8GiaAJtWyCwqMVia27QIbsA/640?wx_fmt=png)
对于所提出模型进行的实验
在强化学习智能体中，带有外部记忆系统的神经网络已经进行了“端到端”的优化，以最大化在任务环境中进行交互时获得的奖励数量。也就是说，系统学会了如何从输入（感官）数据中选择相关信息，将其存储在记忆中，并仅从试验和错误的操作选择中读取出相关的内存条目，从而使任务的奖励高于预期。
虽然这种针对人工记忆的方法被证明是成功的，但我们发现，它不能解决心理学和神经科学中行为研究里的简单任务，特别是那些涉及在相关刺激和后面决策之间存在长时间延迟的任务。这包括但不限于这些问题：导航回到之前访问过的目标、快速奖励评估，其中智能体必须在几乎没有曝光的情况下理解不同对象的值、以及潜在学习（latent learning），一个智能体在进行探索之前通过特定任务获得关于环境的未说明的知识。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0c1DRoCLLYHITn4Sr47n52q1837WmdHK4KGF2XUVIhOF00NMZrTyiaxbaplXmpq05icTavmZnzgb4Dg/640?wx_fmt=png)
潜在学习
我们提出了MERLIN，这是一种集成的AI智能体架构，它可在部分观察到的虚拟现实环境中操作，并基于不同于现有端到端的AI体系的原理将信息存储于记忆中。它学习处理高维感官流，对其进行压缩和存储，并且回忆对任务奖励依赖性较小的事件。
我们将来自外部记忆系统、强化学习和状态估计（推理）模型的成分汇聚在一起，利用从心理学和神经科学的三种观点（预测感觉编码Gluck和Myers的海马表象理论（hippocampal representation theory）时间语境模型和后继表象）中获得的灵感，将它们组合为一个统一的系统。
为了测试MERLIN，我们将它置于一组来自心理学和神经科学的典型任务中，以此来表明，它可以找到解决对现有AI构成严峻挑战的问题的方法。MERLIN给出了一种超越端到端RL限制的方法，这可用于对未来计算智能体的记忆展开研究。
强化学习使得从感官观察o到行动a找到一个策略π或映射变得形式化。强化学习的一个主要方法是考虑随机策略，使该策略便于描述操作上的分布。在感知数据不完整的部分观察环境中，直接将瞬时感知数据映射到动作的无记忆RL策略（Memory-free RL policies）会失败。因此，在这项研究中，我们将注意力集中于依赖记忆的策略上，在这些策略中，动作的分布依赖于过去观察到的全部序列。
![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/waLJGrhJM0c1DRoCLLYHITn4Sr47n52qX1MmdkJia9zkVmbich9yLbhmph5jGQRxNSP1QicAwhaeL8HMpib7bDeDLQ/640?wx_fmt=png)
端到端的学习问题，会随着AI接近具有长期记忆的长效智能体前沿而变得更加紧迫。例如，思考一个需要记住24小时前发生的事件的智能体。存储的记忆可以通过记忆读取操作来恢复，但在整个时间间隔内（如反向传播时间）优化网络动态或信息存储的方法，需要在24小时内记录网络状态的确切数据。
这实际上是一个让人望而却步的规定，它的神经可信度表明，至少对于记忆和时间信用分配的其他方面而言，存在比端到端梯度计算更好的算法解决方案。我们注意到，MERLIN专门使用1.3s的窗口来解决需要更长时间间隔的任务（即6分钟内最长任务长度的0.36%）。
当端到端RL具有足够大的网络、足够丰富的经验以及优化时，它理论上可以学会将相关信息存储于记忆中，以供后期决策使用，但我们已经证实，实际需求往往令人望而却步。正如我们在目标识别和视觉等领域所熟知的，结构创新（如卷积神经网络）对于实际系统而言是至关重要的。
尽管实现细节可能会发生变化，但我们相信，将记忆和预测建模相结合，对于未来在AI乃至神经科学领域，构建大型智能体模型而言至关重要。
原文链接：https://arxiv.org/pdf/1803.10760.pdf
未来智能实验室是人工智能学家与科学院相关机构联合成立的人工智能，互联网和脑科学交叉研究机构。
未来智能实验室的主要工作包括：建立AI智能系统智商评测体系，开展世界人工智能智商评测；开展互联网（城市）云脑研究计划，构建互联网（城市）云脑技术和企业图谱，为提升企业，行业与城市的智能水平服务。
*如果您对实验室的研究感兴趣，欢迎加入未来智能实验室线上平台。扫描以下二维码或点击本文左下角“阅读原文”*
![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/f84kJBXzrBXtjwXLOH13nsYuQKfVHbapnHFO9iacHnzft3Q7mqEeqVf6phSiam3I17pVBMLp18riaEpPOlp4xIxzA/640?wx_fmt=jpeg)
