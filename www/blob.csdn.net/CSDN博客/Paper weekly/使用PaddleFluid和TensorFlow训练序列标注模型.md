# 使用PaddleFluid和TensorFlow训练序列标注模型 - Paper weekly - CSDN博客





2018年07月11日 12:06:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：587









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




**专栏介绍：**Paddle Fluid 是用来让用户像 PyTorch 和 Tensorflow Eager Execution 一样执行程序。在这些系统中，不再有模型这个概念，应用也不再包含一个用于描述 Operator 图或者一系列层的符号描述，而是像通用程序那样描述训练或者预测的过程。




本专栏将推出一系列技术文章，从框架的概念、使用上对比分析 TensorFlow 和 Paddle Fluid，为对 PaddlePaddle 感兴趣的同学提供一些指导。



上一篇我们[通过 RNN 语言模型初识 PaddleFluid 和 TensorFlow 中的循环神经网络模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490134&idx=1&sn=16558d00e0e267a1505102dba77a4e20&chksm=96e9c5d6a19e4cc07fe1fb6b2fa4c3bbae687777464862d75203aa604bafc2f41e427a2d16c7&scene=21#wechat_redirect)。了解了：



- 
在 PaddleFluid 和 TensorFlow 平台下如何组织序列输入数据；



- 
如何使用循环神经网络单元；



- 
使用中的注意事项。





可以看到 PaddleFluid 中的各种循环神经网络单元**都直接支持非填充序列**作为输入，用户在使用时无需对 mini-batch 中的不等长序列进行填充，无需关心填充位是否会对代价（loss）计算产生影响，从而需要在计算损失时对填充位置进行过滤这样的细节，对使用来说无疑是十分方便的。 




循环神经网络的是深度学习模型中最为重要的一部分，**这一篇我们以序列标注任务为例将会构造一个更加复杂的循环神经网络模型用于命名实体识别任务**。我们的关注点始终放在：在两个平台下：（1）如何组织序列数据；（2）如何使用序列处理单元（不限于循环神经网络）。




**这一篇会看到： **




1. PaddleFluid Data Feeder vs. 使用 TensorFlow r1.4 之后 release 的 Dataset API 读取数据； 




2. 在 PaddleFluid 和 TensorFlow 中，使用条件随机场（Conditional Random Field，CRF）单元； 




3. 在 PaddleFluid 和 TensorFlow 中，通过数据并行方式使用多块 GPU 卡进行训练。




# 如何使用代码




本篇文章配套有完整可运行的代码， **请随时从 github **** [1] ****上获取最新代码**。代码包括以下几个文件：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPnKibE5GEciaGq3YicqRtLTWqUCicOia5cLNl6k25PR1l5EU2VVmCjxOfB1Q/640)




**在执行训练任务前，请首先在终端执行下面的命令进行训练数据下载以及预处理。**




```python
sh download.sh
```




在终端运行以下命令便可以使用默认结构和默认参数运行 PaddleFluid 训练序列标注模型。




```python
python sequence_tagging_fluid.py
```




在终端运行以下命令便可以使用默认结构和默认参数运行 TensorFlow 训练序列标注模型。




```python
python sequence_tagging_tensorflow.py
```




# 背景介绍




**序列标注和命名实体识别**




序列标注是自然语言处理任务中的重要基础任务之一。常见的分词，词性标注，语义角色标注，命名实体识别，甚至自动问答（QA）都可以通过序列标注模型来实现。这一篇我们将训练一个序列标注模型完成命名实体识别的任务。




我们先来看看，什么是序列标注问题呢？请看下面一幅图：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPWtbzPibvyOibeH3eyMFGszMywbOBsvoIhAJIL7791htg1z0zLoichzT1A/640)▲ 图1. 序列标注问题





序列标注任务是为一个一维的线性输入序列中的每个元素打上标签集合中的某个标签。在上面的例子中，序列标注就是为图像序列中的每个元素贴上一个描述它们形状的标签。而**序列标注任务的难点在于：序列中 元素的标记和 它们在序列中的位置密切相关**。 




那么， 什么是命名实体识别呢？命名实体识别（Named Entity Recognition，NER）又称作“专名识别”，是指识别文本中具有特定意义的实体，主要包括：人名、地名、机构名、专有名词等。




**BIO 表示法 **




序列标注任务一般都会采用 BIO 表示方式来定义序列标注的标签集，B 代表句子的开始，I 代表句子中间，O 代表句子结束。通过 B、I、O 三种标记将不同的语块赋予不同的标签，例如：对于一个标记为 A 的命名实体，将它所包含的第一个语块赋予标签 B-A，将它所包含的其它语块赋予标签 I-A，不属于任何命名实体的语块赋予标签 O。图 2 是用 BIO 表示标注序列中命名实体的具体示例。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPhyVbjNaHdDDWnUwWOiaPD7d5Fklw7qAiaTctGwkRq3yHyQiarnicFJia3Yw/640)

▲ 图2. BIO标注方法示例




# 模型概览




图 3 是本篇模型的模型结构概览。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPPIya96Eft45q8aflBhh2ZHob0lXYEfgeIfQia8xa74Fszeibib3lXCLEw/640)

▲ 图3. 序列标注模型结构概览




我们要训练的序列标注模型，接受：一个文本序列作为输入，另一个与输入文本序列等长的标记序列作为学习的目标。首先通过上一篇介绍过的 word embedding 层的取词作用得到词向量， 接着经过一个双向 LSTM 单元学习序列的特征表示，这个特别表示最终作为条件随机场 CRF 的输入完成最终的序列标注任务。 




下面是对各个子模块的进一步说明。




**双向循环神经网络**




在循环神经网络模型中，t 时刻输出的隐藏层向量编码了到 t 时刻为止所有输入的信息，但由于循环神经网络单元计算的串行行：t 时刻循环神经网络但愿可以看到历史（t 时刻之前），却无法看到未来（t 时刻之后）。




一些自然语言处理任务总是能一次性拿到整个句子，这种情况下，在 t 时刻计算时，如果能够像获取历史信息一样得到未来的信息，对序列学习任务会有很大帮助，双向循环神经网络的出现正是为了解决这一问题。




它的思想简单且直接：使用两个循环神经网络单元（ simple RNN，GRU 或者 LSTM 均可）分别以正向和反向顺序学习输入序列，再将两者的输出 向量进行横向拼接。这样的一个输出向量中就既包含了 t 时刻之前的信息，也包含了 t 时刻之后的信息。




**条件随机场 **




使用神经网络模型解决问题的思路通常都是：前层网络学习输入的特征表示，网络的最后一层在特征基础上完成最终任务。在序列 标注任务中，双向循环神经网络学习输入的特征表示，条件随机场（Conditional Random Filed， CRF）正是在特征的基础上完成序列标注的一种计算单元，处于整个网络的末端。




CRF 是一种概率化结构模型，可以看作是一个概率无向图模型（也叫作马尔科夫随机场），结点表示随机变量，边表示随机变量之间的概率依赖关系。简单来讲 CRF 学习条件概率：P(X|Y)，其中 X=(x1,x2,...,xn) 是输入序列，Y=(y1,y2,...,yn) 是标记序列；解码过程是给定 X 序列求解令 P(Y|X) 最大的 Y 序列，即![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPf9IsIFbHlhn1Gdz2bFRPDd4ic7emn1TryPdribftia7LLAa5XXnusAuuw/640)。




**条件随机场是的定义：**设 G=(V,E) 是一个无向图， V 是结点的集合，E 是无向边的集合。V 中的每个结点对应一个随机变量 Yv，![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPGGBhNjtsDjnzmqEwsksflygYJ4FUObdu3ykOU0GQVeDqPwnRlA3cdQ/640)，其取值范围为可能的标记集合 {y}，如果以随机变量 X 为条件，每个随机变量 Yv 都满足以下马尔科夫特性： 




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPib1j3HHXNJcBka7c1YwE1QJlFxmuibOibtJ04dw2JPiaFpXqkX41icjh0QQ/640)




其中，ω∼v 表示两个结点在图 G 中是邻近结点，那么，(X,Y) 是一个条件随机场。




**线性链条件随机场 **




上面的定义并没有对 X 和 Y 的结构给出更多约束，理论上来讲只要标记序列表示了一定的条件独立性，G 的图结构可以是任意的。对序列标注任务，只需要考虑 X 和 Y 都是一个序列，于是可以形成一个如图 4 所示的简单链式结构图。在图中，输入序列 X 的元素之间并不存在图结构，因为我们只是将它作为条件，并不做任何条件独立假设。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPGz1cMXrTT3ZfKph8gTQN7CCs5F98ruLc1neysgZo6SZnaRlwPOxHjw/640)

▲ 图4. 输入序列和标记序列具有相同结构的线性链条件随机场




序列标注问题使用的是以上这种定义在线性链上的特殊条件随机场，称之为线性链条件随机场（Linear Chain Conditional Random Field）。下面，我们给出线性链条件随机场的数学定义： 




**定义 2 ：线性链条件随机场 ：**设 X=(x1,x2,...,xn)，Y=(y1,y2,...,yn) 均为线性链表示的随机变量序列，若在给定随机变量序列 X 的条件下，随机变量序列 Y 的条件概率分布 P(Y|X) 满足马尔科夫性：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPMLibwruz7LDuqsu9YQiauKZ6M8ECrKw0uIE2jCJkq6QkRyvelURNu6Fg/640)




i=1,2,...,n（在i=1和n时只考虑单边）则称 P(Y|X) 为线性链条件随机场。X 表示输入序列，Y 表示与之对应的标记序列。




根据线性链条件随机场上的因子分解定理，在给定观测序列 X 时，一个特定标记序列 Y 的概率可以定义为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPwyvDmb9fEHvca46DOFVibJrzBBLXDzib0z6o2rGOO7mlACf3pNlUwHpw/640)




其中：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPuKUpNwhEibkJfJOJD3t4JX8fetbP9It3FcxrElrm8bXaZYWWyz19icVw/640)




是规范化因子。




上面的式子中 tj 是定义在边上的特征函数，依赖于当前和前一个位置，称为转移特征，表示对于观察序列 X 及其标注序列在 i 及 i−1 位置上标记的转移概率。sk 是定义在结点上的特征函数，称为状态特征，依赖于当前位置，表示对于观察序列 X 及其 i 位置的标记概率。λj 和 μk 分别是转移特征函数和状态特征函数对应的权值。




**线性链条件随机场的优化目标**




实际上 ，t 和 s 可以用相同的数学形式表示，s 可以同样也写为以下形式：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPUMBbGjf5Ijmjm2ZqXqAT634mOxnhFqfvfXQfayMjQ6UyBOic6zuADmQ/640)




假设有 K1 个转移特征，K2 个状态特征，定义特征函数![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPy4427AwT9qWnfQqC4ndcOJ7tKc3lMajiaQPBNK3PzQYXIoJqqBvXzhg/640)：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPjMcsESnfmXNEA0jmhCYB7t1plbfWibAHlJRfat7qhFyMbGQrSPIyzcg/640)




再对转移特征和状态特在各个位置 i 求和有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPGtGDPhT4KdroootwL72Z9nMRDMdSMPG2Knfv0utxp3ot9JI3wZyKicg/640)




于是条件概率 P(Y|X) 可以写为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPx0Qfvje7hk0aeK4WCujRbhUhlKdbNaTn2zsFrLJoj0H0iaOF8dXR2Hg/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPaLyvibP0UUmA6MlkRFLO69hj0S25OdYVdTWib7uP8NT9cuyAmHib5GB3Q/640)




我们把 f 统称为特征函数，ω 是权值，是 CRF 模型要求解的参数。




学习时，对于给定的输入序列和对应的标记序列的集合 D=[(X1,Y1),(X2,Y2),...,(XN,YN)] ，通过正则化的极大似然估计，可以得到如下优化目标：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPFlhHzW6CmpskQ6bLmeu50tfISaBCrRiajRhmVsJ1SKmnf8A5PibNicT1A/640)




这个优化目标，可以通过反向传播算法和整个神经网络一起更新求解。 




解码时，对于给定的输入序列 X，通过解码算法（通常有：维特比算法、Beam Search）求令出条件概率![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPTd15XK83xYvOOO7LpRRF8XxqxmOcoAkCtOdQN7hGKePkziaIic33lb1g/640)最大的输出序列![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPVsj3MuGEJrzA4Wt9uodnvGDJ88996ASAibtm9YPJ5wfH4tYCQWlKGsA/640)。




**CRF小结**




条件随机场是这一篇网络中一个相对复杂的计算单元。值得庆幸的是，在各个深度学习框架的帮助下，大多数情况下，我们只需要知道其原理便可以非常方便的使用，而不必过于关注 其内部的实现细节。




这里我们再对上面的内容进行一个简单的总结，方便大家使用 CRF 单元：




1. 在序列标注网络中， CRF 以循环神经网络单元输出向量作为输入，学习状态特征和转移特征。




2. 状态特征只与当然输入有关；转移特征是一个矩阵，刻画了标记两两之间互相转移的强度。




3. 假设循环神经网络单元输出向量维度为 h ，序列中含有 t 个词语，共有 d 个标记：



- 
循环神经网络输入矩阵的大小为：Out=t×h；



- 
CRF 层以 Out 为输入学习转移特征：通过一个 全连接层将 Out 映射为一个 t×d 的矩阵，也就是转移特征； 



- 
状态特征是一个：(d+2)×d 维的矩阵，刻画了标记之前转移的强度。 这里的 +2 是需要学习序列开始 <s> 向句子首词转移和和句子末尾词向序列结束 <e> 转移这样两种特殊的状态转移；



- 
CRF 本质上计算了一个 softmax：给定标记序列出现的概率。但困难之处在于 softmax 的归一化分母是所有可能标记序列，计算量很大。但由于引入了马尔科夫假设，这个归一化分母可以巧妙地通过一个动态规划算法求解。 





4. CRF 的学习准则是令 negative log likelihood 最大化。




# 数据集介绍




这一篇我们使用 Standford CS224d 课程中作业 2 [2] 的 NER 任务数据作为训练数据源。 进入 data 目录运行 data/download.sh 脚本下载数据并预处理训练数据。预处理包括：1. 为输入文本序列建立词典；2. 组织输入数据格式。 




运行结束将会在 data 目录下看到如下内容。




```python
data
├── dev
├── dev_src.txt
├── dev_src.vocab
├── dev_trg.txt
├── dev_trg.vocab
├── download.sh
├── preprocess.py
├── train
├── train_src.txt
├── train_src.vocab
├── train_trg.txt
└── train_trg.vocab
```




其中需要重点关注的是 train_src.txt 、 train_trg.txt 、 train_src.vocab和train_trg.vocab 文件。它们分别是：输入文本序列；文本对应的标记序列；输入文本序列的词典以及标记序列词典。 train_src.txt 和 train_trg.txt 的一行是一条训练样本，他们严格一一对应。分别执行 head -n 1 train_src.txt 和 head -n 1 train_trg.t xt 会看到如下内容：




```python
EU rejects German call to boycott British lamb .
```




```python
B-ORG O B-MISC O O O B-MISC O O
```




# 程序结构




我们首先在此整体回顾一下使用 PaddleFluid 平台和 TensorFlow 运行神经网络模型的整体流程。




**PaddleFluid**




 1. 调用 PaddleFluid API 描述神经网络模型。PaddleFluid 中 一个神经网络训练任务被称之为一段 Fluid Program 。 




2. 定义 Fluid Program 执行设备： place 。常见的有 fluid.CUDAPlace(0) 和 fluid.CPUPlace() 。




```python
place = fluid.CUDAPlace(0) if conf.use_gpu else fluid.CPUPlace()
```




**注：**PaddleFluid 支持混合设备运行，一些 运算（operator）没有特定设备实现，或者为了提高全局资源利用率，可以为他们指定不同的计算设备。




3. 创建 PaddleFluid 执行器（Executor），需要为执行器指定运行设备。




```python
exe = fluid.Executor(place)
```




4. 让执行器执行 fluid.default_startup_program() ，初始化神经网络中的可学习参数，完成必要的初始化工作。 




5. 定义 DataFeeder，编写 data reader，**只需要关注如何返回一条训练/测试数据**。 




6. 进入训练的双层循环（外层在 epoch 上循环，内层在 mini-batch 上循环），直到训练结束。




**TensorFlow **




1. 调用 TensorFlow API 描述神经网络模型。 TensorFlow 中一个神经网络模型是一个 Computation Graph。




2. 创建 TensorFlow Session 用来执行计算图。




```python
sess = tf.Session()
```




3. 调用 sess.run(tf.global_variables_initializer()) 初始化神经网络中的可学习参数。




4. 编写**返回每个 mini-batch 数据**的数据读取脚本。




5. 进入训练的双层循环（外层在 epoch 上循环，内层在 mini-batch 上循环），直到训练结束。




如果不显示地指定使用何种设备进行训练，TensorFlow 会对机器硬件进行检测（是否有 GPU）， 选择能够尽可能利用机器硬件资源的方式运行。



# 构建网络




基于 PaddleFluid 和 TensorFlow 的序列标注网络分别定义在 sequence_tagging_fluid.py 和 sequence_tagging_tensorflow.py 的 NER_net 类中，详细信息请参考完整代码，这里对重要部分进行说明。 




**加载训练数据**




**PaddleFluid：编写Data Reader **




PaddleFluid 模型通过 fluid.layers.data 来接收输入数据。序列标注网络以图片以及图片对应的类别标签作为网络的输入：




```python
self.source = fluid.layers.data(
    name="source", shape=[1], dtype="int64", lod_level=1)
self.target = fluid.layers.data(
    name="target", shape=[1], dtype="int64", lod_level=1)
```



定义 data layer 的核心是指定输入 Tensor 的形状（ shape ）和类型。 




序列标注中，输入文本序列和标记序列都使用 one-hot 特征作为输入，一个词用一个和字典大小相同的向量表示，每一个位置对应了字典中的 一个词语。one-hot 向量仅有一个维度为 1， 其余全部为 0。在上面定义的 data layer 中 source 和 target 的形状都是 1，类型是 int64 。 




PaddleFluid 支持非填充的序列输入，这是通过 LoD Tensor 实现的。关于什么是 LoD Tensor 请参考上一篇[使用 PaddleFluid 和 TensorFlow 训练 RNN 语言模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490134&idx=1&sn=16558d00e0e267a1505102dba77a4e20&chksm=96e9c5d6a19e4cc07fe1fb6b2fa4c3bbae687777464862d75203aa604bafc2f41e427a2d16c7&scene=21#wechat_redirect)中的介绍，这一篇不再赘述。有了 LoD Tensor 的概念后，在 PaddleFluid 中，通过 DataFeeder 模块来为网络中的 data layer 提供数据，调用方式如下面的代码所示：




```python
train_reader = paddle.batch(
    paddle.reader.shuffle(
        data_reader(conf.train_src_file_name, conf.train_trg_file_name,
                    conf.src_vocab_file, conf.trg_vocab_file),
        buf_size=1024000),
    batch_size=conf.batch_size)

place = fluid.CUDAPlace(0) if conf.use_gpu else fluid.CPUPlace()
feeder = fluid.DataFeeder(feed_list=[net.source, net.target], place=place)
```




观察以上代码，需要用户完成的仅有：编写一个**实现读取一条数据**的 python 函数： data_reader 。 data_reader 的代码非常简单，我们再来看一下它的具体实现：




```python
def data_reader(src_file_name, trg_file_name, src_vocab_file, trg_vocab_file):
    def __load_dict(dict_file_path):
        word_dict = {}
        with open(dict_file_path, "r") as fdict:
            for idx, line in enumerate(fdict):
                if idx < 2: continue
                word_dict[line.strip().split("\t")[0]] = idx - 2

        return word_dict

    def __reader():
        src_dict = __load_dict(src_vocab_file)
        trg_dict = __load_dict(trg_vocab_file)

        with open(src_file_name, "r") as fsrc, open(trg_file_name,
                                                    "r") as ftrg:
            for src, trg in izip(fsrc, ftrg):
                src_words = src.strip().split()
                trg_words = trg.strip().split()

                src_ids = [src_dict[w] for w in src_words]
                trg_ids = [trg_dict[w] for w in trg_words]
                yield src_ids, trg_ids

    return __reader
```




在上面的代码中： 




 1. data_reader 是一个 python generator ，函数名字可以任意指定，无需固定。 




2. data_reader 打开输入序列文件和标记序列文件，每次从这两个文件读取一行，一行既是一条训练数据，返回一个 python list，这个 python list 既是序列中所有时间步。具体的数据组织方式如下表所示，其中i 代表一个整数：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRPvKqH9FJNMibaBT1DO0LTEyHQciaOKnvhw8eibjwVNreFeCp2j1cCjgkUA/640)




3. paddle.batch() 接口用来构造 mini-batch 输入，会调用 data_reader 将数据读入一个 pool 中，对 pool 中的数据进行 shuffle，然后依次返回每个 mini-batch 的数据。




**TensorFlow：使用Dataset API**




在之前的篇章中我们都使用 TensorFlow 的 placeholder 接入训练数据，这一篇我们使用一种新的方式 TensorFlow 在 r1.3 版本之后引入的 Dataset API 来读取数据。




参考 Google 官方给出的 Dataset API 中的类图 [3]，使用 TensorFlow 的 Dataset API，首先引入两个抽象概念：




1. tf.data.Dataset 表示一系列元素，其中每个元素包含一个或多个 Tensor 对象。




2. tf.data.Iterator 提供了从数据集中取出元素的方法。 Iterator.get_next() 会在执行时生成 Dataset 的下一个 /mini-batch 元素。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhgkF2L3afFJyicOO3e5zJjdRP0wnZGPnWCksceLkEYVGEw4Kib1ttRQl3UrUaoibxxH8FgwMFvXnye6rg/640)




**定义**** Dataset **




目前 Dataset API 还提供了三种预定义好的定义 Dataset 的方式。这一篇中我们主要面向文本数据的处理，使用其中的 TextLineDataset 接口。 




**tf.data.TextLineDataset：**接口的输入是一个文件列表，输出是一个 TensorFlow dataset ， dataset 中的每一个元素就对应了文件中的一行。通过下面的调用传入输入序列文本路径和标记序列文本路径便可返回一个 Dataset 。




```python
src_dataset = tf.data.TextLineDataset(src_file_name)
trg_dataset = tf.data.TextLineDataset(trg_file_name)
```




**获取 Iterator **




需要说明的是，TensorFlow 中的循环神经网络要求一个 mini-batch 之内序列长度相等，使用 Dynamic RNN 时，batch 和 batch 之间序列长度可以不相等，因此对一个 mini-batch 之内的数据需要进行填充。 




 Dataset API 提供了 padded_batch 帮助构造填充后的 mini-batch 数据。 




**提示：**使用 bucket 分桶，从桶内取 mini-batch 数据，填充至一个 batch 中的最长序列长度能够有效提高 dynamic rnn 的计算效率。 




下面的代码返回 Iterator ，使用先分桶，然后再取 mini-batch 数据填充至  batch 中最长序列长度的方式。完整代码请参考：iterator_helper_tf [4]。




```python
def get_data_iterator(src_file_name,
                      trg_file_name,
                      src_vocab_file,
                      trg_vocab_file,
                      batch_size,
                      pad_token="</p>",
                      max_sequence_length=None,
                      unk_id=1,
                      num_parallel_calls=4,
                      num_buckets=5,
                      output_buffer_size=102400,
                      is_training=True):

    def __get_word_dict(vocab_file_path, unk_id):
        return tf.contrib.lookup.index_table_from_file(
            vocabulary_file=vocab_file_path,
            key_column_index=0,
            default_value=unk_id)

    src_dataset = tf.data.TextLineDataset(src_file_name)
    trg_dataset = tf.data.TextLineDataset(trg_file_name)

    dataset = tf.data.Dataset.zip((src_dataset, trg_dataset))
    if is_training:
        dataset = dataset.shuffle(
            buffer_size=output_buffer_size, reshuffle_each_iteration=True)

    src_trg_dataset = dataset.map(
        lambda src, trg: (tf.string_split([src]).values, \
                tf.string_split([trg]).values),
        num_parallel_calls=num_parallel_calls).prefetch(output_buffer_size)

    src_dict = __get_word_dict(src_vocab_file, unk_id)
    trg_dict = __get_word_dict(trg_vocab_file, unk_id)

    src_pad_id = tf.cast(src_dict.lookup(tf.constant(pad_token)), tf.int32)
    trg_pad_id = tf.cast(trg_dict.lookup(tf.constant(pad_token)), tf.int32)

    # convert word string to word index
    src_trg_dataset = src_trg_dataset.map(
        lambda src, trg: (
                tf.cast(src_dict.lookup(src), tf.int32),
                tf.cast(trg_dict.lookup(trg), tf.int32)),
        num_parallel_calls=num_parallel_calls).prefetch(output_buffer_size)

    # Add in sequence lengths.
    src_trg_dataset = src_trg_dataset.map(
        lambda src, trg: (src, trg, tf.size(src)),
        num_parallel_calls=num_parallel_calls).prefetch(output_buffer_size)

    def __batching_func(x):
        return x.padded_batch(
            batch_size,
            padded_shapes=(
                tf.TensorShape([None]),  # src
                tf.TensorShape([None]),  # trg
                tf.TensorShape([]),  #seq_len
            ),
            padding_values=(src_pad_id, trg_pad_id, 0, ))

    if num_buckets > 1:

        def __key_func(unused_1, unused_2, seq_len):
            if max_sequence_length:
                bucket_width = (
                    max_sequence_length + num_buckets - 1) // num_buckets
            else:
                bucket_width = 10

            bucket_id = seq_len // bucket_width,
            return tf.to_int64(tf.minimum(num_buckets, bucket_id))

        def __reduce_func(unused_key, windowed_data):
            return __batching_func(windowed_data)

        batched_dataset = src_trg_dataset.apply(
            tf.contrib.data.group_by_window(
                key_func=__key_func,
                reduce_func=__reduce_func,
                window_size=batch_size))

    else:
        batched_dataset = __batching_func(curwd_nxtwd_dataset)

    batched_iter = batched_dataset.make_initializable_iterator()
    src_ids, trg_ids, seq_len = batched_iter.get_next()

    return BatchedInput(
        initializer=batched_iter.initializer,
        source=src_ids,
        target=trg_ids,
        sequence_length=seq_len)
```




**构建网络结构及运行 **




构建网络结构及运行的过程对两个平台上都是常规流程。




1. 构建网络时调用相关的 API 接口，令一个 计算单元的输出成为下一个计算单元的输入建立起网络的连通性；具体请参考 sequence_tagging_fluid.py 和 sequence_tagging_tensorflow.py 中 NER_net 类的实现。




2. 运行训练以及解码具体请参考 sequence_tagging_fluid.py 和 sequence_tagging_tensorflow.py 中 train 函数的实现。




模型中核心模块：LSTM 单元在两个平台下的差异及注意事项请参考上一篇：使用 PaddleFluid 和 TensorFlow 训练 RNN 语言模型，这里不再赘述。



# 总结




这一篇继续在序列标注模型中了解 PaddleFluid 和 TensorFlow 在接受序列输入，序列处理策略上的不同。 




1. PaddleFluid 引入了 LoD Tensor 的概念，所有序列处理模块（包括所有循环神经网络单元，文本卷积）都支持非填充的序列输入，使用时无需对 mini-batch 数据进行填充，也就避免了对填充位的各种特殊处理，这一点非常方便。 




2. TensorFlow 中的 Dynamic RNN 支持 mini-batch 之间序列不等长，但仍要求一个 mini-batch 内的数据填充至一样长。 




3. PaddleFluid 中通过 Data Feeder 提供训练数据，只需要编写一个 python generator 实现从原始输入文件中读取一条训练样本， 框架会完成数据 shuffle 和组织 mini-batchd 工作。 




4. 这一篇使用了 TensorFlow r1.3 后 release 的 Dataset API，数据读取部分也是一个 computation graph，能够提高 I/O 效率，使用相对复杂一些。 




本篇代码中提供了通过数据并行策略在 PaddleFluid 平台下使用多块 GPU 卡进行训练，在 TensorFlow 中使用多卡相对复杂一些，这些主题会在下面继续讨论。 




# 参考文献




**[1]. 本文配套代码**

https://github.com/JohnRabbbit/TF2Fluid/tree/master/05_sequence_tagging

**[2]. **** Standford CS224d课程作业2**

http://cs224d.stanford.edu/assignment2/index.html

**[3]. Google官方Dataset API**

https://developers.googleblog.com/2017/09/introducing-tensorflow-datasets.html

**[4]. iterator_helper_tf**

https://github.com/JohnRabbbit/TF2Fluid/blob/master/05_sequence_tagging/iterator_helper_tf.py







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)

▽ 点击 | 阅读原文| 加入社区刷论文




