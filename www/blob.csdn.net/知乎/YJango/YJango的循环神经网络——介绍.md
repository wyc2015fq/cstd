# YJango的循环神经网络——介绍 - 知乎
# 

## **介绍**

该文主要目的是让大家体会循环神经网络在与前馈神经网络的不同之处。

大家貌似都叫Recurrent Neural Networks为循环神经网络。

我之前是查维基百科的缘故，所以一直叫它递归网络。

**下面我所提到的递归网络全部都是指Recurrent Neural Networks。**

递归神经网络的讨论分为三部分
- 介绍：描述递归网络和前馈网络的差别和优劣
- 实现：梯度消失和梯度爆炸问题，及解决问题的LSTM和GRU
- 代码：用tensorflow实际演示一个任务的训练和使用

## **时序预测问题**

[YJango的前馈神经网络--代码LV3](https://zhuanlan.zhihu.com/p/27854076)已经展示了如何用前馈神经网络（feedforward）来做时序信号预测。

***一、用前馈神经网络来做时序信号预测有什么问题？***
- **依赖受限**：前馈网络是利用窗处理将不同时刻的向量并接成一个更大的向量。以此利用前后发生的事情预测当前所发生的情况。如下图所示：
![](https://pic3.zhimg.com/v2-c984ea3ba696a0a786bb0bb52f0d772e_b.gif)- 但其所能考虑到的前后依赖受限于将多少个向量（window size）并接在一起。所能考虑的依赖始终是固定长度的。
- **网络规格**：想要更好的预测，需要让网络考虑更多的前后依赖。

**例**：若仅给“国（）”，让玩家猜括号中的字时，所能想到的可能性非常之多。但若给“中国（）”时，可能性范围降低。当给“我是中国（）”时，猜中的可能性会进一步增加。

那么很自然的做法就是扩大并接向量的数量，但这样做的同时也会使输入向量的维度和神经网络第一层的权重矩阵的大小快速增加。如[YJango的前馈神经网络--代码LV3](https://zhuanlan.zhihu.com/p/27854076)中每个输入向量的维度是39，41帧的窗处理之后，维度变成了1599，并且神经网络第一层的权重矩阵也变成了1599 by n（n为下一层的节点数）。其中很多权重都是冗余的，但却不得不一直存在于每一次的预测之中。
- **训练所需样本数**：前两点可能无伤大雅，而真正使得递归神经网络（recurrent）在时序预测中击败前馈神经网络的关键在于训练网络所需要的数据量。

## **网络差异之处**

几乎所有的神经网络都可以看作为一种特殊制定的前馈神经网络，这里“特殊制定”的作用在于缩减寻找映射函数的搜索空间，也正是因为搜索空间的缩小，才使得网络可以用相对较少的数据量学习到更好的规律。

**例**：解一元二次方程![y=ax+b](https://www.zhihu.com/equation?tex=y%3Dax%2Bb)。我们需要两组配对的![(x,y)](https://www.zhihu.com/equation?tex=%28x%2Cy%29)来解该方程。但是如果我们知道![y=ax+b](https://www.zhihu.com/equation?tex=y%3Dax%2Bb)实际上是![y=ax](https://www.zhihu.com/equation?tex=y%3Dax)，这时就只需要一对![(x,y)](https://www.zhihu.com/equation?tex=%28x%2Cy%29)就可以确定![x](https://www.zhihu.com/equation?tex=x)与![y](https://www.zhihu.com/equation?tex=y)的关系。递归神经网络和卷积神经网络等神经网络的变体就具有类似的功效。

***二、相比前馈神经网络，递归神经网络究竟有何不同之处？***

需要注意的是递归网络并非只有一种结构，这里介绍一种最为常用和有效的递归网络结构。

## **数学视角**

首先让我们用从输入层到隐藏层的空间变换视角来观察，不同的地方在于，这次将**时间维度**一起考虑在内。

**注**：这里的圆圈不再代表节点，而是状态，是输入向量和输入经过隐藏层后的向量。

**例子**：用目前已说的所有字预测下一个字。

## 前馈网络：window size为3帧的窗处理后的前馈网络
- **动态图**：左侧是时间维度展开前，右侧是展开后（单位时刻实际工作的只有灰色部分。）。前馈网络的特点使不同时刻的预测完全是独立的。我们只能通过窗处理的方式让其照顾到前后相关性。
![](https://pic4.zhimg.com/v2-8cc0db5acd643dd7ca3668ad6e35aabb_b.gif)

- **数学式子**：![h_t= \phi(W_{xh} \cdot concat(x_{t-1}, x_t, x_{t+1}) + {b})](https://www.zhihu.com/equation?tex=h_t%3D+%5Cphi%28W_%7Bxh%7D+%5Ccdot+concat%28x_%7Bt-1%7D%2C+x_t%2C+x_%7Bt%2B1%7D%29+%2B+%7Bb%7D%29)，concat表示将向量并接成一个更大维度的向量。
- **学习参数**：需要从大量的数据中学习![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)和![b](https://www.zhihu.com/equation?tex=b)。
- 要学习各个时刻（3个）下所有维度（39维）的关系（39*3个），就需要很多数据。

## **递归网络**：不再有window size的概念，而是time step
- **动态图**：左侧是时间维度展开前，回路方式的表达方式，其中黑方框表示时间延迟。右侧展开后，可以看到当前时刻的![h_t](https://www.zhihu.com/equation?tex=h_t)并不仅仅取决于当前时刻的输入![x_t](https://www.zhihu.com/equation?tex=x_t)，同时与上一时刻的![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)也相关。
![](https://pic3.zhimg.com/v2-db52470df88f53271d8e06722da39122_b.gif)

- **数学式子**：![h_t= \phi(W_{xh} \cdot x_t + W_{hh} \cdot h_{t-1} + {b})](https://www.zhihu.com/equation?tex=h_t%3D+%5Cphi%28W_%7Bxh%7D+%5Ccdot+x_t+%2B+W_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%7D%29)。![h_t](https://www.zhihu.com/equation?tex=h_t)同样也由![x_t](https://www.zhihu.com/equation?tex=x_t)经![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)的变化后的信息决定，
- 但这里多另一份信息：![W_{hh}\cdot h_{t-1}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D%5Ccdot+h_%7Bt-1%7D)，而该信息是从上一时刻的隐藏状态![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)经过一个不同的![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)变换后得出的。
- **注**：![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)的形状是行为dim_input，列为dim_hidden_state，而![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)是一个行列都为dim_hidden_state的方阵。
- **学习参数**：前馈网络需要3个时刻来帮助学习一次![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)，而递归网络可以用3个时刻来帮助学习3次![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)和![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)。换句话说：**所有时刻的权重矩阵都是共享的。这是递归网络相对于前馈网络而言最为突出的优势。**

> **递归神经网络是在时间结构上存在共享特性的神经网络变体。**

**时间结构共享**是递归网络的核心中的核心。

## **物理视角**

共享特性给网络带来了诸多好处，但也产生了另一个问题：

***三、为什么可以共享？***

在物理视角中，YJango想给大家展示的第一点就是为什么我们可以用这种共享不同时刻权重矩阵的网络进行时序预测。

下图可以从直觉上帮助大家感受日常生活中很多时序信号是如何产生的。
- **例1**：轨迹的产生，如地球的轨迹有两条线索决定，其中一条是地球自转，另一条是地球围绕太阳的公转。下图是太阳和其他星球。自转相当于![W_{xh} \cdot x_t](https://www.zhihu.com/equation?tex=W_%7Bxh%7D+%5Ccdot+x_t)，而公转相当于![W_{hh}\cdot h_{t-1}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D%5Ccdot+h_%7Bt-1%7D)。二者共同决定实际轨迹。
![](https://pic4.zhimg.com/v2-8b7c5c5225b6a7337e23786ecb5b2fe3_b.jpg)

- **例2**：同理万花尺
![](https://pic2.zhimg.com/v2-9e770433dc6a146e7625dd155ba00ec5_b.gif)

- **例3**：演奏音乐时，乐器将力转成相应的震动产生声音，而整个演奏拥有一个主旋律贯穿全曲。其中乐器的物理特性就相当于![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)，**同一乐器在各个时刻物理特性在各个时刻都是共享的。**其内在也有一个隐藏的主旋律基准![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)，旋律信息![W_{hh}\cdot h_{t-1}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D%5Ccdot+h_%7Bt-1%7D)与音乐信息![W_{xh} \cdot x_t](https://www.zhihu.com/equation?tex=W_%7Bxh%7D+%5Ccdot+x_t)共同决定下一时刻的实际声音。
- 上述例子中所产生的轨迹、音乐都是我们所能观察到的observations，我们常常会利用这些observation作为依据来做出决策。

下面的例子可能更容易体会共享特性对于数据量的影响。
- **实例**：捏陶瓷：不同角度相当于不同的时刻
![](https://pic2.zhimg.com/v2-c84b8f3c65a62e4ceee81899c5126365_b.gif)

- **若用前馈网络**：网络训练过程相当于不用转盘，而是徒手将各个角度捏成想要的形状。不仅工作量大，效果也难以保证。
- **若用递归网络**：网络训练过程相当于在不断旋转的转盘上，以一种手势捏造所有角度。工作量降低，效果也可保证。

## **递归网络特点**
- **时序长短可变**：只要知道上一时刻的隐藏状态![h_{t-1}](https://www.zhihu.com/equation?tex=h_%7Bt-1%7D)与当前时刻的输入![x_t](https://www.zhihu.com/equation?tex=x_t)，就可以计算当前时刻的隐藏状态![h_{t}](https://www.zhihu.com/equation?tex=h_%7Bt%7D)。并且由于计算所用到的![W_{xh}](https://www.zhihu.com/equation?tex=W_%7Bxh%7D)与![W_{hh}](https://www.zhihu.com/equation?tex=W_%7Bhh%7D)在任意时刻都是共享的。递归网络可以处理任意长度的时间序列。
- **顾及时间依赖**：若当前时刻是第5帧的时序信号，那计算当前的隐藏状态![h_{5}](https://www.zhihu.com/equation?tex=h_%7B5%7D)就需要当前的输入![x_{5}](https://www.zhihu.com/equation?tex=x_%7B5%7D)和第4帧的隐藏状态![h_{4}](https://www.zhihu.com/equation?tex=h_%7B4%7D)，而计算![h_{4}](https://www.zhihu.com/equation?tex=h_%7B4%7D)又需要![h_{3}](https://www.zhihu.com/equation?tex=h_%7B3%7D)，这样不断逆推到初始时刻为止。意味着常规递归网络对过去所有状态都存在着依赖关系。
- **注**：在计算![h_{0}](https://www.zhihu.com/equation?tex=h_%7B0%7D)的值时，若没有特别指定初始隐藏状态，则会将![h_{-1}](https://www.zhihu.com/equation?tex=h_%7B-1%7D)全部补零，表达式会变成前馈神经网络：![h_t= \phi(W_{xh} \cdot x_t+ 0 + {b})](https://www.zhihu.com/equation?tex=h_t%3D+%5Cphi%28W_%7Bxh%7D+%5Ccdot+x_t%2B+0+%2B+%7Bb%7D%29)
- **未来信息依赖**：前馈网络是通过并接未来时刻的向量来引入未来信息对当前内容判断的限制，但常规的递归网络只对所有过去状态存在依赖关系。所以递归网络的一个扩展就是双向（bidirectional）递归网络：两个不同方向的递归层叠加。
- **关系图**：正向（forward）递归层是从最初时刻开始，而反向（backward）递归层是从最末时刻开始。
![](https://pic1.zhimg.com/v2-a9c81692067bca508bc88ba3f3ecb7b8_b.jpg)

- **数学式子**：
- **正向递归层**：![h^f_t= \phi(W^f_{xh} \cdot x_t + W^f_{hh} \cdot h_{t-1} + {b^f})](https://www.zhihu.com/equation?tex=h%5Ef_t%3D+%5Cphi%28W%5Ef_%7Bxh%7D+%5Ccdot+x_t+%2B+W%5Ef_%7Bhh%7D+%5Ccdot+h_%7Bt-1%7D+%2B+%7Bb%5Ef%7D%29)
- **反向递归层**：![h^b_t= \phi(W^b_{xh} \cdot x_t + W^b_{hh} \cdot h_{t+1} + {b^b})](https://www.zhihu.com/equation?tex=h%5Eb_t%3D+%5Cphi%28W%5Eb_%7Bxh%7D+%5Ccdot+x_t+%2B+W%5Eb_%7Bhh%7D+%5Ccdot+h_%7Bt%2B1%7D+%2B+%7Bb%5Eb%7D%29)
- **双向递归层**：![h_t=h^f_t+h^b_t](https://www.zhihu.com/equation?tex=h_t%3Dh%5Ef_t%2Bh%5Eb_t)
- **注**：还有并接的处理方式，即![h_t=concat(h^f_t,h^b_t)](https://www.zhihu.com/equation?tex=h_t%3Dconcat%28h%5Ef_t%2Ch%5Eb_t%29)，但反向递归层的作用是引入未来信息对当前预测判断的额外限制。并不是信息维度不够。至少在我所有的实验中，相加（sum）的方式往往优于并接。
- **注**：也有人将正向递归层和反向递归层中的权重![W^f_{xh}](https://www.zhihu.com/equation?tex=W%5Ef_%7Bxh%7D)与![W^b_{xh}](https://www.zhihu.com/equation?tex=W%5Eb_%7Bxh%7D)共享，![W^f_{hh}](https://www.zhihu.com/equation?tex=W%5Ef_%7Bhh%7D)与![W^b_{hh}](https://www.zhihu.com/equation?tex=W%5Eb_%7Bhh%7D)共享。我没有做实验比较过。但直觉上![W^f_{hh}](https://www.zhihu.com/equation?tex=W%5Ef_%7Bhh%7D)与![W^b_{hh}](https://www.zhihu.com/equation?tex=W%5Eb_%7Bhh%7D)共享在某些任务中可能会有些许提升。![W^f_{hh}](https://www.zhihu.com/equation?tex=W%5Ef_%7Bhh%7D)与![W^b_{hh}](https://www.zhihu.com/equation?tex=W%5Eb_%7Bhh%7D)的共享恐怕并不会起到什么作用（要贴合任务而言）。
- **注**：隐藏状态![h_t](https://www.zhihu.com/equation?tex=h_t)通常不会是网络的最终结果，一般都会将![h_t](https://www.zhihu.com/equation?tex=h_t)再接着另一个![\phi(W_{ho} \cdot h_{t} + {b_o})](https://www.zhihu.com/equation?tex=%5Cphi%28W_%7Bho%7D+%5Ccdot+h_%7Bt%7D+%2B+%7Bb_o%7D%29)将其投射到输出状态![o_{t}](https://www.zhihu.com/equation?tex=o_%7Bt%7D)。一个最基本的递归网络不会出现前馈神经网络那样从输入层直接到输出层的情况，而是至少会有一个隐藏层。
- **注**：双向递归层可以提供更好的识别预测效果，但却不能实时预测，由于反向递归的计算需要从最末时刻开始，网络不得不等待着完整序列都产生后才可以开始预测。在对于实时识别有要求的线上语音识别，其应用受限。
- **递归网络输出**：递归网络的出现实际上是对前馈网络在时间维度上的扩展。
- **关系图**：常规网络可以将输入和输出以向量对向量（无时间维度）的方式进行关联。而递归层的引入将其扩展到了序列对序列的匹配。从而产生了one to one右侧的一系列关联方式。较为特殊的是最后一个many to many，发生在输入输出的序列长度不确定时，其实质两个递归网络的拼接使用，公共点在紫色的隐藏状态![h_{t+1}](https://www.zhihu.com/equation?tex=h_%7Bt%2B1%7D)。
![](https://pic3.zhimg.com/v2-78c48d9ca0b66d6b46202b019b5ecb66_b.jpg)

- many to one：常用在情感分析中，将一句话关联到一个情感向量上去。
- many to many：第一个many to many在DNN-HMM语音识别框架中常有用到
- many to many(variable length)：第二个many to many常用在机器翻译两个不同语言时。
- **递归网络数据**：递归网络由于引入time step的缘故，使得其训练数据与前馈网络有所不同。
- **前馈网络**：输入和输出：矩阵
- 输入矩阵形状：(n_samples, dim_input)
- 输出矩阵形状：(n_samples, dim_output)
- **注**：真正测试/训练的时候，网络的输入和输出就是向量而已。加入n_samples这个维度是为了可以实现一次训练多个样本，求出平均梯度来更新权重，这个叫做Mini-batch gradient descent。
- 如果n_samples等于1，那么这种更新方式叫做Stochastic Gradient Descent (SGD)。
- **递归网络**：输入和输出：维度至少是3的张量，如果是图片等信息，则张量维度仍会增加。
- 输入张量形状：(time_steps, n_samples, dim_input)
- 输出张量形状：(time_steps, n_samples, dim_output)
- **注**：同样是保留了Mini-batch gradient descent的训练方式，但不同之处在于多了time step这个维度。
- Recurrent 的任意时刻的输入的本质还是单个向量，只不过是将不同时刻的向量按顺序输入网络。你可能更愿意理解为一串向量 a sequence of vectors，或者是矩阵。

## **网络对待**

请以**层的概念**对待所有网络。递归神经网络是指拥有递归层的神经网络，其关键在于网络中存在递归层。

每一层的作用是将数据从一个空间变换到另一个空间下。可以视为特征抓取方式，也可以视为分类器。二者没有明显界限并彼此包含。关键在于使用者如何理解。

以层的概念理解网络的好处在于，今后的神经网络往往并不会仅用到一种处理手段。往往是前馈、递归、卷积混合使用。 这时就无法再以递归神经网络来命名该结构。

**例**：下图中就是在双向递归层的前后分别又加入了两个前馈隐藏层。也可以堆积更多的双向递归层，人们也会在其前面加入“深层”二字，提高逼格。
![](https://pic2.zhimg.com/v2-1c85ff7e786721b62d2f9b15991a6871_b.jpg)

**注**：层并不是图中所画的圆圈，而是连线。圆圈所表示的是穿过各层前后的状态。

## **递归网络问题**

常规递归网络从理论上应该可以顾及所有过去时刻的依赖，然而实际却无法按人们所想象工作。原因在于梯度消失（vanishinggradient）和梯度爆炸（exploding gradient）问题。下一节就是介绍Long Short Term Memory（LSTM）和Gated Recurrent Unit（GRU）：递归网络的特别实现算法。

**最后点个题，重要的事情说一万遍**

> **Recurrent Layer在时间结构上存在共享特性。         **

