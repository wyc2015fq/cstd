# NeurIPS 2018 | 如何用循环关系网络机智地解决数独类关系推理任务？ - 知乎
# 



选自 arXiv，作者：Rasmus Berg Palm、Ulrich Paquet、Ole Winther，机器之心编译，参与：李诗萌、王淑婷。

> 本文引入循环关系网络来解决步骤相互依赖的关系推理任务，举个栗子，数独任务。以往的传统深度学习方法虽然也能解决，却总是会出现一些问题。本文提出的 RNN 模型解决了 96.6% 的最难数独，而且与其它方法相比结果最佳。

人类智能的核心组成部分是对目标及其相互作用进行抽象推理的能力 [Spelke 等人，1995，Spelke 和 Kinzler，2007]。举个例子，假设要解数独问题。数独盘面中有 81 个格，按 9*9 的方式排列，要用数字 1～9 填满这些格子，每个数字在每行、每列以及每一个 3*3 的非重叠格中都只能出现一次，有些数字已经给定为 1。要解数独，就得用方法推理出盘面上的格子以及它们在许多步骤中的相互作用关系。有人试着将数字放进格子中，并观察它会对其它格子产生怎样的影响，迭代地解决这一问题。

将这种方式与传统的深度学习方法（如多层感知器（MLP）或多层卷积神经网络（CNN））进行对比来解决问题。上述架构将整个数独盘作为输入，在一次正向传递过程中输出了完整的解决方案，但在这个过程中它们忽视了目标之间存在的归纳偏置，以及它们是以一致的方式互相作用的。当面对需要推理基本关系的问题时，这些模型会出现问题也就不足为奇了 [Lake 等人，2016，Santoro 等人，2017]。

Santoro 等人的关系网络 [2017] 是用简单模块推理目标及其相互作用的重要第一步，但它的局限性在于只能进行单个关系运算，而在数据集上评估时最多需要三个推理步骤（令人惊讶的是，如文中所示，这一问题可通过单个关系推理步骤解决）。除了关系网络，在人工智能和机器学习领域还有关于逻辑和推理方面的文献，我们将在第 5 节中讨论。

为了实现在多个步骤中有条理地推理目标及其相互作用的能力，本文引入了一个复合函数，循环关系网络。它是端到端可微分学习系统中多步关系推理的模块化组件。它将归纳偏置编码为：1）存在于这个世界的目标；2）可以通过属性充分描述；3）属性可以随时间的变化而改变；4）目标之间会互相影响；5）在给定属性的情况下，目标对彼此的影响不变。

从 Santoro 等人 [2017] 的研究中得出的重要见解是将关系推理函数分解成两个组件或「模块」：一个感知前端（其任务是识别原始输入中的目标，并将其表示为向量）和一个关系推理模块（使用这些表征来推理目标及其相互作用）。这两个模块都是用端到端的方法联合训练的。用计算机科学中的术语来说，关系推理模块实现了一个接口：它在有向边和节点的图上进行操作，其中节点由实值向量表示，并且是可微分的。本文主要开发该接口的关系推理方面。

我们评估的一些任务也可以通过在符号级别上操作的手工算法高效、完美地解决。例如，可以用约束传播和搜索 [Norvig，2006] 或舞蹈链 [Kuth，2000] 的方法在零点几秒内解决 9*9 的数独问题。从各个方面看这些符号算法都很优越，只有一点除外：它们不符合接口，因为它们不可微也不适用于实值向量描述。因此它们不能用于具有深度学习感知前端和端到端学习的组合模型中。

继 Santoro 等人 [2017] 后，我们用「关系推理」一词来表示以目标和交互为中心的问题解决方法。尽管「关系推理」与「关系逻辑」或「一阶逻辑」这种其他科学分支的术语相似，但这并不意味着直接的并行。

本文认为多步关系推理是深度学习架构中一项极具挑战的任务。我们开发了循环关系推理模块，它即这篇文章的主要贡献。我们在三个不同的数据集上进行多步关系推理，证实了这是一个强大的架构，它在 bAbI 和数独游戏上都实现了当前最佳的结果。




**论文：Recurrent Relational Networks**
![](https://pic3.zhimg.com/v2-732d9f351c9607a4920b2b520c3016ea_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='694' height='222'></svg>)
论文链接：[https://arxiv.org/pdf/1711.08028.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1711.08028.pdf)

**摘要**：本文关注的是学习解决需要一系列相互依赖步骤的关系推理任务，比如回答关于目标之间关系的复杂问题，或者是解决在解决方案中小元素相互约束的问题。我们引入了循环关系网络，这是一个在目标的图表征上运行的通用模块。正如 Santoro 等人 [2017] 所做的关于关系网络的概括，该网络可以增强任何能够进行多步关系推理的神经网络模型。我们用循环关系网络在 bAbI 文本问答数据集上实现了当前最佳结果，连续解决了 20/20 的任务。从关系推理的角度看，bAbI 并不是特别具有挑战性，因此我们引入了 Pretty-CLEVR 数据集，这是一个用于关系推理的新诊断数据集。在 Pretty-CLEVR 的建立过程中，我们可以通过改变问题来控制获得答案所需的关系推理步骤数量。最后，我们展示了循环关系网络是如何从监督训练数据中学会解决数独问题的，这是一项极具挑战的任务，需要 64 个以上的关系推理步骤。我们解决了 96.6% 最难的数独问题，而在所有可比较的方法中该方法实现了当前最佳的结果。

**循环关系网络**

我们以解决数独问题这种大家都很熟悉的事物为例来讨论循环关系网络。有一个简单的策略，如果在某个数独格子里填了「7」，那么在同一行、同一列以及对应的 3*3 格子中就可以安全地删去「7」这个选项。在信息传递框架中，这个格子需要向同一行、同一列以及对应 3*3 格子中的其它格子传递信息，告诉它们它的值是「7」，不要再接受「7」了。在一个迭代 t 中，这些信息是同时、并行地在所有格子之间发送的。然后每一个格子 i 就要思考所有的输入信息，并将其内部状态 
![](https://pic4.zhimg.com/v2-82c4ed8cfd23be2ef938ac7c8fcc0657_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='23'></svg>)
 更新为 
![](https://pic4.zhimg.com/v2-71cdfe299af4c9a111c2666141d6a237_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='45' height='27'></svg>)
 i。更新状态后每一个格子都要向外发送新的信息，然后重复这个过程。

**在图上传递信息**：循环关系网络要学习在图上传递信息。就数独游戏而言，图有 i ∈ {1, 2, ..., 81} 个节点，每个节点表示数独盘中的一个格子。每个节点都有一个输入特征向量 
![](https://pic2.zhimg.com/v2-5e28256c7e54f76e96b5a57220846cc1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='19' height='14'></svg>)
 和与数独盘中同一行、同一列以及同一 3*3 格子中所有节点相连的边。图是关系推理模块的输入，向量 
![](https://pic2.zhimg.com/v2-5e28256c7e54f76e96b5a57220846cc1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='19' height='14'></svg>)
 一般是感知前端的输出，例如卷积神经网络。继续以数独游戏为例，每一个 
![](https://pic2.zhimg.com/v2-5e28256c7e54f76e96b5a57220846cc1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='19' height='14'></svg>)
 都对初始的格子内容（空的或给定数字）以及格子中行和列的位置进行了编码。




在每个步骤 t 中，每个节点都有隐藏状态向量 
![](https://pic4.zhimg.com/v2-82c4ed8cfd23be2ef938ac7c8fcc0657_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='23'></svg>)
，将这个向量初始化为特征，使 
![](https://pic4.zhimg.com/v2-622d9a47d34ed320933f360d4ad562ff_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='72' height='24'></svg>)
。在每一步 t 中，每个节点会给它的相邻节点发送信息。我们通过以下方式定义在第 t 步时从节点 i 发送到节点 j 的信息 
![](https://pic4.zhimg.com/v2-1236a9f66006f711093f2b11579ab80f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='36' height='27'></svg>)
：
![](https://pic3.zhimg.com/v2-ad52d2563774aef3fafe70410dcba5a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='552' height='63'></svg>)
其中信息函数 f 是多层感知机，它使得网络能够了解要发送的信息类型，我们在实验中使用了具有线性输出的 MLP。由于节点要考虑所有输入的信息，我们用以下方式对其进行求和：
![](https://pic1.zhimg.com/v2-2af2cc45cf35df3d490e66a557ad83e4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='509' height='52'></svg>)
式中 N（j）是所有和节点 j 相连的节点。就数独游戏而言，N（j）包含所有与 j 相同的行、列、3*3 格子的节点。在我们的实验中，因为在（1）中的信息是线性的，这有点类似于信念传播中如何对对数几率求和 [Murphy 等人，1999]。

**循环节点更新**：最后我们要通过以下方式更新节点隐藏状态，
![](https://pic4.zhimg.com/v2-d235688c744fdcdba6b0d561e6dc98a7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='542' height='49'></svg>)
式中的节点函数 g 是另一个学习过的神经网络。对先前节点隐藏状态 
![](https://pic2.zhimg.com/v2-6db1f385fbd5011ef219c6d34b9d9aed_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='44' height='32'></svg>)
 的依赖使得网络能够迭代地寻找解决方案，而不是每一步都从头开始。像这样在每一步都输入特征向量 
![](https://pic4.zhimg.com/v2-4b0b4062a63e9ccf3b4b21033e3043df_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='21' height='18'></svg>)
，就能将节点函数的注意力集中于来自其它节点的信息，而不是试图记住输入。

**监督训练：**上述用于发送信息和更新节点状态的等式定义了循环关系网络的核心。为了以有监督的方式训练一个求解数独的循环关系网络，我们在图中每个节点的数字 1～9 上引入了输出概率分布。节点 i 在第 t 步的输出概率 
![](https://pic2.zhimg.com/v2-4dd3e9aa7398e611d55425092a468419_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='24'></svg>)
 由下式给出：
![](https://pic1.zhimg.com/v2-c2c7c8fd7940911182762323100ba59c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='500' height='49'></svg>)
式中 r 是将节点隐藏状态映射到输出概率的 MLP，例如用 softmax 非线性。在给定目标数字 y = {y_1, y_2, ..., y_81} 的情况下，第 t 步的损失是交叉熵项的和，每个节点的交叉熵项如下：
![](https://pic2.zhimg.com/v2-85b1061b0cd128dd079ee4a554f302f9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='200' height='30'></svg>)
，式中 
![](https://pic2.zhimg.com/v2-60b11a00192be00d22a280c8f103e999_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='49' height='25'></svg>)
是 o_i 的第 y_i 个组件。等式（1）到（4）在图 1 中阐释。
![](https://pic3.zhimg.com/v2-ce92d7282de972d817b34e40428737c2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='378' height='309'></svg>)
*图 1：在全连接图上有 3 个节点的循环关系网络。绿色高亮表示的是节点的隐藏状态  *
*![](https://pic4.zhimg.com/v2-82c4ed8cfd23be2ef938ac7c8fcc0657_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='23'></svg>)*
*，红色高亮表示的是输入*
*![](https://pic2.zhimg.com/v2-5e28256c7e54f76e96b5a57220846cc1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='19' height='14'></svg>)*
*，蓝色高亮表示的是输出*
*![](https://pic2.zhimg.com/v2-4dd3e9aa7398e611d55425092a468419_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='22' height='24'></svg>)*
*。虚线表示循环连接。下标表示节点索引，上标表示 t 步。通过两步展开的同一图形请参阅补充材料。*

**收敛性信息的传递**：本文提出模型的显著特征是我们在每一步都最小化了输出和目标分布之间的交叉熵。

测试时我们只考虑了最后一步的输出概率，但训练时每一步都有损失是有益的。因为目标数字y_i 是在步骤上是恒定的，所以它鼓励网络学习收敛信息传递算法。其次，它有助于解决梯度消失问题。

**Variation**：如果边是未知的，可以将图视为全连接的。在这种情况中，网络需要学习哪些目标是彼此相互作用的。如果边有属性 
![](https://pic2.zhimg.com/v2-be035be03804228f555acc1151c7a00d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='27' height='20'></svg>)
，等式 1 中的信息函数可以修改为



![](https://pic1.zhimg.com/v2-0eab5be9d5a205c345e3bd61b38b8860_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='226' height='29'></svg>)
。如果想要整个图而不是每个节点的输出，可以将等式 4 中的输出修改为单个输出，即
![](https://pic3.zhimg.com/v2-cb39422ed7f3e145568098dd7a9d353e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='120' height='29'></svg>)
。也可以根据需要相应地修改损失。

**实验**
![](https://pic1.zhimg.com/v2-ae1a64b995383170ed3f792fbf56e434_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='605' height='245'></svg>)表 1：bAbI 的结果。用 10，000 个训练样本在所有 20 个任务上进行联合训练。用星号标记的条目是我们自己的实验，其它结果都来自于各自的论文。![](https://pic4.zhimg.com/v2-c9004ac6b671474978ad607c5111b323_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='617' height='360'></svg>)
图 2：2（a）是 Pretty-CLEVR 诊断数据集中的两个样本。每个样本都有 128 个相关问题，这表现出了不同等级的关系推理难度。对于最顶层的样本，用箭头表示的问题的解决方案是：「绿色，跳 3 次」，也就是「加号」。2（b）对应的是从 8 个可能的输出中随机挑选一个（形状和颜色取决于输入）。RRN 经过了四个步骤的训练，但因为它在每一步都进行了预测，因此我们可以评估每一步的表现。括号中标出了步数。
![](https://pic2.zhimg.com/v2-dcecabcc91161ee031b122a664064ba5_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='672' height='331'></svg>)
图 3：训练后的网络如何解决部分数独问题的示例。清晰起见，仅显示了完整 9*9 数独盘的最顶行。
![](https://pic1.zhimg.com/v2-4939f432e91a35c73df8e97d1edbb588_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='511' height='193'></svg>)
表 2：求解数独的方法比较。只比较了可微的方法。用星号标记的条目是我们的实验，其它都来自于各自的论文。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*







