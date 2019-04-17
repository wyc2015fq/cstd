# 嵌入技术在Dota2人工智能战队OpenAI Five中的应用 - 知乎
# 



选自COMPUTATIONAL NEUROSCIENCE，作者：Tambet Matiisen ，机器之心编译。

> 本文重点讨论了 OpenAI 在处理大量不断变化的策略输入和输出时对嵌入技术的创造性运用。作者从嵌入技术的概念入手，先后介绍了 OpenAI Five 的网络架构、在观测状态中使用的嵌入以及动作中的嵌入。在分析之后，作者得出结论：OpenAI Five 的大多数网络都聚焦于感知任务（对观测状态进行预处理）和运动控制（对动作进行解码）。所有的策略和战术都依赖于一个包含 1024 个神经元的 LSTM。 
![](https://pic2.zhimg.com/v2-3eed899a8823df1e8135f76bf29676d1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='750' height='200'></svg>)
尽管人工智能社区中关于 Dota2 机器人 OpenAI Five 性能表现及其通用意义的讨论层出不穷，但对其训练方法和网络架构技术的分析却少得惊人。到目前为止，这些分析主要的信息来源主要都是 OpenAI Five 的博客（[https://blog.openai.com/openai-five/](https://link.zhihu.com/?target=https%3A//blog.openai.com/openai-five/)）。从某种程度上来说，这是说得通的，它们使用了标准的近端策略优化（PPO）算法（OpenAI 于几年前在其内部发布）和经过验证的[自我博弈的训练方案](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650731830%26idx%3D4%26sn%3Dbb6d502083c605583b1038078e38a62c%26chksm%3D871b3148b06cb85eab096fd10907f13ce5556c5827da75a2579fc0feb2aa546e7ea7311786d9%26scene%3D21%23wechat_redirect)。但是，在他们的博文中，我们往往容易忽视一个很重要的技术细节（嵌入技术）。下面我们给出他们网络架构示意图的链接：[https://d4mucfpksywv.cloudfront.net/research-covers/openai-five/network-architecture.pdf](https://link.zhihu.com/?target=https%3A//d4mucfpksywv.cloudfront.net/research-covers/openai-five/network-architecture.pdf)。

在这篇博客文章中，我想重点讨论他们的网络架构的一个方面——在处理大量数量不断变化的策略输入和输出时对嵌入技术的创造性运用。尽管将嵌入及其内积用于注意力机制在自然语言处理任务中已经是一种标准做法，但在强化学习中这种方法还没有得到广泛应用。

最新消息：在写完这篇博文后，我发现 OpenAI 的博客中更新了一个新的网络架构示意图（[https://s3-us-west-2.amazonaws.com/openai-assets/dota_benchmark_results/network_diagram_08_06_2018.pdf](https://link.zhihu.com/?target=https%3A//s3-us-west-2.amazonaws.com/openai-assets/dota_benchmark_results/network_diagram_08_06_2018.pdf)）。由于新的架构对嵌入技术的使用没有太多的变化，我决定并不重新对新的示意图进行分析，将其留给读者自行进行思考。


**什么是嵌入技术？**

从数学意义上来说，嵌入是指从空间 X 到 Y 的映射，同时保留对象的某些结构（如它们之间的距离）。然而，在神经网络中使用嵌入通常意味着将分类（离散的）变量（如单词索引）转化为连续向量。将单词索引直接作为输入传给神经网络将使处理工作变得非常困难，因为它需要为索引的每个值构建一个二分特征（假设索引之间是不相关的）。因此，我们会事先帮网络将分类值转化到 one-hot 编码向量中。如果你将 one-hot 向量与权值矩阵相乘，那么它就会从权值矩阵中选择出一行。在深度学习工具包中，通常会跳过这个将分类变量转化为 one-hot 向量并与权值矩阵相乘的步骤，直接使用索引从权值矩阵中选出一行，利用类似于查表法的方法进行处理。最重要的是学习了嵌入向量，方式与学习带 one-hot 向量的权值矩阵的方法相同。
![](https://pic2.zhimg.com/v2-2dfe185c492a75d8fbefa58d38bdb791_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='962' height='251'></svg>)



嵌入技术最广为人知的应用场景是自然语言处理。在自然语言处理任务中，单词索引会被转化成词向量（或嵌入）。人们已经证明，当一个网络被训练利用给定的词向量预测与其相近的词向量，词向量就会获得语义，你可以对他们进行算术运算。例如：「女人」-「男人」+「国王」将得到与「女王」相近的词向量。你可以想象得到，「女人」-「男人」会产生一个性别迁移向量，将其与「国王」相加会将其转化成女性统治者。或者，如果你进行「国王」-「男人」+「女人」的计算，「国王」-「男人」将产生一个「统治者」的向量，将其与「女人」相加也会得到「女王」。
![](https://pic1.zhimg.com/v2-5252fe0b8492414c632a3f7be9903abc_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='300' height='233'></svg>)
*图片来源：Mikolov 等人于 2013 年发表的《Linguistic Regularities in Continuous Space Word Representations》（*[https://www.aclweb.org/anthology/N13-1090](https://link.zhihu.com/?target=https%3A//www.aclweb.org/anthology/N13-1090)*）。*




**OpenAI Five 的网络架构**

在深入分析技术细节前，我们将简要介绍网络的通用架构。
![](https://pic3.zhimg.com/v2-1aafdd697024a3e7a1fe1d7a4254ca8a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='614'></svg>)图片来源：OpenAI
五个 OpenAI 机器人都有具有特定输入和动作的神经网络（目前还不清楚五个神经网络是否共享了参数。）。机器人与机器人之间仅仅通过游戏进行通信。我们不妨想象一下，这些机器人「像蜜蜂一样跳舞」，从而与其他机器人交流敌人的位置信息，但是我认为它们还没有做到这一点，甚至我都不确定它们是否需要这么做。根据最新消息，在该神经网络的新版本（[https://s3-us-west-2.amazonaws.com/openai-assets/dota_benchmark_results/network_diagram_08_06_2018.pdf](https://link.zhihu.com/?target=https%3A//s3-us-west-2.amazonaws.com/openai-assets/dota_benchmark_results/network_diagram_08_06_2018.pdf)）中，在层与层之间进行了最大池化操作，这可以看作一种单向传播通信信道。

示意图中神经网络的上半部份会对观测数据进行处理。它将来自多个数据源的数据连接起来，并将所有结果作为输入传给一个 LSTM 神经元。LSTM 神经元的输出通过图中神经网络的下半部分被使用，从而生成动作（action）。网络中具体的复杂细节当然是十分令人头疼的，我们将在下面的章节对此进行讨论。




**在观测状态中使用的嵌入**

OpenAI Five 机器人使用 Dota 2 提供的 API「观察」周围的单位和建筑。这会生成与相关变量长度相同的单位的列表（英雄、小兵、防御塔等）及其相关属性。OpenAI 在它们的博客中对观测状态空间和动作空间进行了很好的可视化工作，我建议大家去仔细观察这些可视化结果（[https://blog.openai.com/openai-five/#dota-diagram](https://link.zhihu.com/?target=https%3A//blog.openai.com/openai-five/%23dota-diagram)）。
![](https://pic3.zhimg.com/v2-b78aa70ee5ab4c084a0ebda84649e97e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='264' height='300'></svg>)图片来源：OpenAI
下图总结了对某个单位的属性的处理过程。
![](https://pic1.zhimg.com/v2-85912c4f9a009ba76365413c3626840c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='393'></svg>)图像来源：OpenAI
在图的左上部分，我们可以看到每个单位被编码成了一个嵌入。这样做是完全说得通的，因为 116 个 Dota 2 英雄中的每一个英雄都可以按照以下标准进行分类：
- 基本属性：力量、敏捷、智力。
- 攻击类型：远程或近战。
- 定位：核心、控制、对线型辅助、先手、打野、辅助、肉盾、爆发、推进、逃生。

以上属性都可在嵌入向量中形成一个维度，网络会自动学习每个英雄在核心、辅助或打野等不同定位中的能力值。同样的嵌入技术也适用于小兵和建筑（例如，防御塔也有远程攻击）。这种嵌入技术为网络提供了一种通用的表示不同单位的方法。嵌入向量与其它单位的属性相连接，如生命值、与其他英雄的距离等。

但是这些嵌入不仅用于单位类型，它们也可以被用于施法效果、技能和道具物品。
![](https://pic1.zhimg.com/v2-bd0f88ec78fc727950d37f1e86536748_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='386'></svg>)图片来源：OpenAI
这种做法也完全是合乎情理的，尽管所有英雄的技能并不相同，它们仍然具有一些共同之处。例如，这些技能是主动技能还是被动技能，它们是否需要施法目标，施法目标是另一个单位还是一片区域... 有的物品能够对你进行治疗，有的能赋予你神奇的力量，有的会被立即消耗掉，有的会提升你的状态。嵌入是一种自然的表示方式，表示具有很多不同但可能有所重叠的特性的事物，这些事物可能有相似的效果，但是程度不尽相同。

请注意，尽管有各种各样的施法效果、技能和道具物品，网络在这些列表上仍然能进行最大池化操作。这意味着在所有的维度中只有最大的值才能被传递出来。这一开始看起来似乎说不通，它可能给人的印象是你有一种将所有存在的技能融合起来的能力，例如远程被动治疗。但是事实上这种操作对它们似乎是有效的。

我们会对附近的每个单位分别进行上述处理，通用属性、英雄施法效果、技能和道具的嵌入结果会被连接在一起。后期的处理方法会根据该单位是否是敌人的非英雄单位、盟友非英雄单位、中立单位、盟友英雄或敌人英雄而定。
![](https://pic3.zhimg.com/v2-2d5bea05fa6115c06f00e1e3fe4712c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='362'></svg>)图片来源：OpenAI
最后，所有这类单位的后期处理结果会经过最大池化处理。同样地，乍一看这似乎有点问题，因为附近单位的不同属性被组合到了一起。例如，如果其中一个维度代表一个单位的生命值，那么网络只能看到这类单位的最大生命值。但是，在这种情况下它的效果仍然很好。


对每个单元类型的最大池化的结果会被连接起来，然后馈送给 LSTM。还有一些前半部分输出的切片（slice），但是当我们谈论动作目标时，我们会回过头来讨论这个问题。




**动作中的嵌入**

据估计，DOTA2 的动作空间中包含大约 17 万个动作，包括移动和攻击等基本动作，也包含使用技能、使用物品、提升状态等动作。但并不是每一步都可以选择所有的动作，这可能是因为你还没学习某种特定技能，也可能是因为你的背包中还没有某种物品。尽管如此，你仍可以选择 1000 多种动作。除此之外，很多动作还包含参数，例如你想要移动到的区域或你想攻击的敌人。同样地，OpenAI 在他们的博客中也给出了动作空间的很棒的可视化结果（[https://blog.openai.com/openai-five/#dota-diagram](https://link.zhihu.com/?target=https%3A//blog.openai.com/openai-five/%23dota-diagram)）。
![](https://pic4.zhimg.com/v2-f7c48eb650e7b2bc2496c64c52ceb50b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='640'></svg>)图片来源：OpenAI
如此广阔的动作空间为强化学习带来了巨大的探索问题，因为一开始智能体会随机尝试执行各种动作。一种直接的思路是，输出所有 17 万种动作的得分，并将 sotfmax 的输出限制为 1000 种目前可能采取的动作。但是 OpenAI 通过嵌入技术以及具有可变长度的 softmax 输出巧妙地解决了这个问题。
![](https://pic3.zhimg.com/v2-a6696baec3891987d6142732e90a0da6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='480'></svg>)图片来源：OpenAI
正如你在上图的顶部所看到的，每种动作都有其相应的嵌入表示，例如，该动作究竟是一个远程攻击，还是用某种物品进行治疗或传送到某个特定目的地。动作嵌入的内积和 LSTM 的输出被用来生成不同动作的得分。这些得分会传递给 softmax 层，得到的概率分布会被用于选择一个可以采取的动作。

注：两个向量的内积会将二者相同位置的元素对应相乘，然后将所有的乘式算子求和得到最终的内积结果。内积又称标量积，因为内积运算会得到一个单一的标量值。它与余弦相似度有着紧密的关系，当两个向量方向相同时内积结果会很大；方向相反时内积结果会很小。余弦相似度经常被用于快速度量两个向量的相似程度。事实上，卷积操作就是这样做的，它会生成过滤结果和输入之间相似度的特征映射。
![](https://pic4.zhimg.com/v2-3ba454d5b6674fc2b275ad991dd43af3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='620' height='408'></svg>)打开原文可拖动箭头，看看内积（点乘）、余弦相似度和余弦距离如何变化
我们认为，在实际的动作选择过程中，LSTM 生成的结果在原则上可以被称为「意图向量」。例如，如果你陷入了困境、生命值超低，那么你的意图就可能是「离开这里」。这样的意图将与可以进行的动作相匹配，如果意图与其中一个动作的效果相一致，就会得到高分。举例而言，动作「移动」和「传送」可能与「离开这里」的意图相一致。而传送可能稍微更好一些，因为在传送过程中你不会被敌人追打，因此它会产生更高的得分，在 softmax 层的输出中得到更高的概率。但是，如果目前无法进行传送，那么该嵌入就无法匹配，此时「移动」可能得到最高的得分和概率。

有的动作包含如目的地或操作对象等参数。所有这些参数都是以简单明了的方式通过 softmax 建模的。例如，X 和 Y 坐标被离散成范围，而不是使用连续的输出和高斯分布。我认为 softmax 能对多模态分布进行更好的处理。在这里，一个重要的观测结果是，采取动作的输出结果似乎并没有对动作和目标之间的联合分布进行建模。我认为这并不是一个大问题，因为所有动作的输出都是以 LSTM 的输出作为条件的。因此，LSTM 输出已经对「意图」进行了「编码」，而这些全连接（FC）层只是对此「意图」的不同因素（动作和目标）进行了解码。

但是在整个框架中，我最欣赏 OpenAI Five 处理动作目标的方式。还记得那些单位观测状态输出中奇怪的切片吗？它们在图中用蓝色表示，意味着这是针对单个单位而言的。这样的向量被称为「单位注意力键」（unit attention key），它们与 LSTM 的「意图」相匹配，以便为每个单位生成得分。这些得分会被传递给 softmax 并决定攻击对象。在这里我们用 softmax 的结果取代了观测到的单位，正如决定动作的 softmax 取代了可用的动作一样。

我认为它的工作流程是这样的：网络会根据观测状态判定某个单位的生命值非常低，而且机器人有机会完成最后一击。LSTM 会生成「尝试最后一击」的意图，这与「攻击」的动作相匹配。此外，这种「尝试最后一击」的意图也与对观测状态处理中的每个单位的输出相匹配，并与生命值最低的单位相匹配。「BANG！」，机器人会对该单位作出最后一击，并且得到额外的奖励。

更新：在网络的最新版本中（[https://s3-us-west-2.amazonaws.com/openai-assets/dota_benchmark_results/network_diagram_08_06_2018.pdf](https://link.zhihu.com/?target=https%3A//s3-us-west-2.amazonaws.com/openai-assets/dota_benchmark_results/network_diagram_08_06_2018.pdf)），他们在使用带单位注意力键的内积前使用动作嵌入对 LSTM 的输出做变换。我想，如果不这样做，不同的动作（例如，攻击和治疗）可能会试图应用于类似的单位目标。




**结语**

在分析了 OpenAI Five 的网络后，我们可以清楚地看到，大多数网络都聚焦于感知任务（对观测状态进行预处理）和运动控制（对动作进行解码）。所有的策略和战术都依赖于一个包含 1024 个神经元的 LSTM。
![](https://pic1.zhimg.com/v2-0574161da33a44ee976b6430e20315c4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='238' height='109'></svg>)图片来源：OpenAI
我认为一个相对简单的数学结构就能够生成如此复杂的行为是十分令人惊讶的。或许这是一种对 Dota 2 的复杂性的解释？结合快速反应时间的短期战术是否优于长期战略？
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*






*原文链接：[https://neuro.cs.ut.ee/the-use-of-embeddings-in-openai-five/](https://link.zhihu.com/?target=https%3A//neuro.cs.ut.ee/the-use-of-embeddings-in-openai-five/)*


