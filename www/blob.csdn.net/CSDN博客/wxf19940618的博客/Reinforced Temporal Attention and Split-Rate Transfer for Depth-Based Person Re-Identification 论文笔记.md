# Reinforced Temporal Attention and Split-Rate Transfer for Depth-Based Person Re-Identification 论文笔记 - wxf19940618的博客 - CSDN博客





2019年01月04日 14:59:49[rxdffz](https://me.csdn.net/wxf19940618)阅读数：83








**Reinforced Temporal Attention and Split-Rate Transfer for Depth-Based Person Re-Identification ****论文笔记**

一、提出问题
- 利用深度学习方法进行行人重识别时的数据稀缺问题
- 基于视频的行人重识别
- 同一行人穿着不同的衣服时识别精度低

二、论文贡献
- 为了解决数据稀缺问题，本文提出分流率RGB深度传输，以有效地利用来自大型RGB数据的预训练模型，并学习强大的帧级特征 ；
- 为了增强视频序列的重新识别，本文提出了增强时间注意单元，它位于帧级功能之上，不依赖于网络架构。 大量实验表明，本文的方法在基于深度学习的行人重识别方面优于现有技术；
- 在行人更换衣服的情况下，本文的方法比基于RGB对应的方法更有效。

三、模型结构

2.1 输入表示

整个系统的输入为Kinect V2获取的深度图像![](https://img-blog.csdnimg.cn/20190104145826127.png)，图像中的每一个像素![](https://img-blog.csdnimg.cn/20190104145826133.png)![](https://img-blog.csdnimg.cn/20190104145826137.png)，该图像包含从图像平面到特定坐标（i，j）处的最近对象的笛卡尔距离（以毫米为单位）。在“默认范围”设置中，间隔为[0,0.4m）和（8.0m，∞）被认为是未知测量，[0.4,0.8] [m]被视为“太近”，（4.0,8.0）[m]视为“太远”，[0.8,4.0] [m]作为“正常”值。当骨架跟踪有效时，身体指数![](https://img-blog.csdnimg.cn/20190104145826148.png)由Kinect SDK提供，其中0对应于背景，正整数i对于属于行人i的每个像素。

在提取人物区域![](https://img-blog.csdnimg.cn/20190104145826142.png)之后，“正常”区域内的测量值在[1,256]范围内被归一化，而“太远”和“未知”范围内的值被设置为256，并且“太近”范围内的设置为1。实际上，为了避免将值集中在256附近，所以需要引入偏移![](https://img-blog.csdnimg.cn/20190104145826150.png)并归一化![](https://img-blog.csdnimg.cn/20190104145826186.png)。“灰度”人物表示为![](https://img-blog.csdnimg.cn/20190104145826160.png)，当身体指数可用时，在深度区域Dp上部署Bp⊂B作为掩模，以便在应用范围归一化之前实现背景减法（参见图3-1）。

![](https://img-blog.csdnimg.cn/20190104145826225.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-1 灰度图表示

3.2 模型结构

本文将行人重识别问题阐述为agent的顺序决策过程，该agent通过视频序列从部分可观察的环境执行行人识别。在每个时间步内，agent通过深度相机观察环境，基于深度卷积神经网络（CNN）计算特征向量，并使用新的强化时间注意力（RTA）主动推断当前帧对重识别任务的重要性。在CNN特征之上，长短期记忆（LSTM）单元模拟短程时间动态。在每个时间步骤内，agent根据其分类任务的成功或失败获得奖惩，其目标是随着时间的推移使得奖励总和最大化。

![](https://img-blog.csdnimg.cn/20190104145826237.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图 3-2 模型结构

**Agent****：**问题设置是部分可观察的马尔可夫决策过程（POMDP），环境的真实状态是未知的。agent学习随机策略为![](https://img-blog.csdnimg.cn/20190104145826211.png)，参数为![](https://img-blog.csdnimg.cn/20190104145826210.png)，在每个步骤t中，过去历史![](https://img-blog.csdnimg.cn/20190104145826213.png)对离散动作的有两种分布：帧权重wt（子策略π1）和类后验ct（子策略π2）。权重wt在时间t由RTA单元![](https://img-blog.csdnimg.cn/20190104145826213.png)参数化的二进制分布随机采样：![](https://img-blog.csdnimg.cn/20190104145826214.png)。类后验分布在分类器模块条件下，分类器模块附加到LSTM输出![](https://img-blog.csdnimg.cn/20190104145826214.png)。向量ht维持环境的内部状态，是过去观察的总结。为了简单表示，时间t处的输入图像表示为It，但实际输入是人物区域Dp。

**帧级特征嵌入**![](https://img-blog.csdnimg.cn/20190104145826219.png)**：**假设深度数据很少但有大量的RGB数据可用于行人重识别，本文希望可以利用RGB数据来训练帧级特征提取的深度模型。深度神经网络底部卷积层的参数可以通过简单的深度编码直接在RGB和深度数据之间共享，也就是说深度为D的每个像素都被复制到三个通道并编码为（D，D，D），它对应于三个RGB通道，这使本文选择预训练的RGB模型。

       本文采用Learning deep feature representations with domain guided dropout for person re-identification论文的模型进行帧级特征提取，该网络在性质上类似于GoogleNet；包括BN和3×3卷积层，接着是6个Inception模块和2个全连接层。为了使该网络适用于本文得场景，需要进行两处修改。第一，用256×N全连接层替换顶部分类层，其中N是目标数据集中的主体数量，并且其权重从具有标准偏差0.01的零均值高斯分布随机初始化；第二，在全连接的层之间添加了dropout正则化。

**循环模块**![](https://img-blog.csdnimg.cn/20190104145826233.png)**：**长短期记忆（LSTM）单元能有效地对视频识别和字幕生成进行时间动态建模。假设σ()是sigmoid函数，g [t]是时间帧t的输入，h [t - 1]是模块的前一帧输出，c [t - 1]是前一个单元，更新策略如下：

![](https://img-blog.csdnimg.cn/20190104145826248.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

其中![](https://img-blog.csdnimg.cn/20190104145826249.png)是每个门q从源s到目标q的权重矩阵，![](https://img-blog.csdnimg.cn/20190104145826240.png)是q的偏差，i [t]是输入门，f [t]是遗忘门，z [t]是输入到单元格，c [t]是单元格，o [t]是输出门，h [t]是该模块的输出，x⊙y表示向量x和y的元素乘积。

**强化时间注意力**![](https://img-blog.csdnimg.cn/20190104145826242.png)**：**在每个时间步骤t，RTA单元推断图像帧It的重要性wt，因为It由特征编码gt表示。该模块由一个线性层组成，该线性层将256×1矢量gt映射到一个标量，然后通过Sigmoid函数将值压缩到[0,1]范围内。wt由具有概率质量函数的伯努利随机变量定义：

![](https://img-blog.csdnimg.cn/20190104145826479.png)

伯努利参数以Sigmoid输出![](https://img-blog.csdnimg.cn/20190104145826256.png)为条件，形成Bernoulli-Sigmoid单元。在训练时，输出得wt随机采样为{0,1}中的二进制值。在评价时，wt不是从分布中采样，而是等于伯努利参数，即![](https://img-blog.csdnimg.cn/20190104145826273.png)。

**分类器**![](https://img-blog.csdnimg.cn/20190104145826265.png)**和奖励：**分类器由整流线性单元序列、r = 0.4的dropout、全连接层和Softmax组成。参数层将256×1隐藏向量ht映射到N×1类后向量ct，其长度等于类N的数量。RTA注意力的multi-shot预测是帧级预测的加权和ct， RTA权重为![](https://img-blog.csdnimg.cn/20190104145826272.png)。

       Bernoulli-Sigmoid单元在训练是是随机的，所以需要基于强化学习算法获得反向传播的梯度。奖励函数定义为：

![](https://img-blog.csdnimg.cn/20190104145826290.png)

其中rt是原始奖励，gt是帧t的真实类。因此，在每个时间步骤t，agent接收奖励rt，当帧被正确分类时rt等于1，错误时等于0。

3.3 模型训练

在实验中，首先预训练帧级特征嵌入的参数，然后附加LSTM，RTA和新的分类器以训练整个模型。在第二步，冻结嵌入的权重，同时随机初始化添加的层。采用这种模块化训练，可以进行single-shot和multishot的评估，如果仅处理视频序列，则可以从头开始对整个架构进行端对端训练。然后，进行帧级嵌入的迁移学习，并使用具有时间注意力的递归模型的混合监督训练算法。

**用于特征嵌入的分速传输学习**![](https://img-blog.csdnimg.cn/20190104145826295.png)**：**为了利用大量RGB数据，本文的方法基于从RGB预训练模型传输参数θg进行初始化。与前人研究不同的是，首先，本文发现即使RGB和深度是完全不同的模态（参见图3-3），RGB模型的底层也可以与深度数据共享（无需微调）。其次，对RGB传输的参数进行微调比从头开始训练更有效。第三，对底层使用较慢（或零）的学习速率，对顶层使用较高的学习速率比在整个层级中使用统一的速率更有效。因此，本文的方法称为分流率迁移。本文方法的可视化见图3-4，与Yosinski方法的对比结果见图4-1。

![](https://img-blog.csdnimg.cn/20190104145826366.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-3 RGB图像与深度图像

![](https://img-blog.csdnimg.cn/20190104145826400.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-4可视化结果

**CNN-LSTM****的混合学习和强化时间注意力：**CNN-LSTM的参数{θg，θh，θc}是通过最小化LSTM单元上反向传播整个网络的分类损失来学习的。为了将识别任务中的交叉熵损失降至最低，所以目标是在给定观察值的情况下最大化真实标签的条件概率，即最大化![](https://img-blog.csdnimg.cn/20190104145826372.png)，其中![](https://img-blog.csdnimg.cn/20190104145826412.png)是步骤t的真实类别。

学习CNN和RTA的参数{θg，θw}，以便agent最大化其总奖励![](https://img-blog.csdnimg.cn/20190104145826372.png)，这涉及计算期望![](https://img-blog.csdnimg.cn/20190104145826477.png)超过所有可能序列![](https://img-blog.csdnimg.cn/20190104145826370.png)的分布，这是很难处理的。因此，样本近似也就是强化学习规则，可以应用于建模子策略![](https://img-blog.csdnimg.cn/20190104145826376.png)的Bernoulli-Sigmoid单元。给定概率质量函数![](https://img-blog.csdnimg.cn/20190104145826376.png)和伯努利参数![](https://img-blog.csdnimg.cn/20190104145826412.png)，梯度近似为：

![](https://img-blog.csdnimg.cn/20190104145826491.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

其中，agent运行M episodes时得到序列i ![](https://img-blog.csdnimg.cn/20190104145826440.png)，且![](https://img-blog.csdnimg.cn/20190104145826425.png)是在收集样本![](https://img-blog.csdnimg.cn/20190104145826431.png)之后获得的第i episode的累积奖励。梯度估计由基线奖励bt偏向以实现较低的方差。本文设置![](https://img-blog.csdnimg.cn/20190104145826443.png)，因为Rti和bt之间的均方误差也通过反向传播最小化。

在每个步骤t，agent预测wt，并且奖励信号Rti评估agent对分类任务的有效性。强化策略更新增加了高于预期累积奖励的动作的对数概率（如通过增加伯努利参数![](https://img-blog.csdnimg.cn/20190104145826444.png)），否则，对于导致低奖励的帧序列，对数概率降低。总之，agent共同优化累积奖励和分类损失，这构成了混合监督目标。

四、实验结果

数据集：DPI-T、BIWI、IIT PAVIS、TUM-GAID

结果：

![](https://img-blog.csdnimg.cn/20190104145826532.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

图3-5 与Yosinski方法进行对比

![](https://img-blog.csdnimg.cn/20190104145826536.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190104145826524.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)

![](https://img-blog.csdnimg.cn/20190104145826571.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d4ZjE5OTQwNjE4,size_16,color_FFFFFF,t_70)



