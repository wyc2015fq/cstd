# RealNVP与Glow：流模型的传承与升华 - Paper weekly - CSDN博客





2018年08月27日 12:29:43[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：4723









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨苏剑林

单位丨广州火焰信息科技有限公司

研究方向丨NLP，神经网络

个人主页丨kexue.fm




# 话在开头




在上一篇文章[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)中，我们介绍了 flow 模型中的一个开山之作：NICE 模型。从 NICE 模型中，我们能知道 flow 模型的基本概念和基本思想，最后笔者还给出了 Keras 中的 NICE 实现。




本文我们来关心 **NICE 的升级版**：RealNVP 和 Glow。




**Glow 模型的采样演示：**








**精巧的flow**




不得不说，flow 模型是一个在设计上非常精巧的模型。总的来看，flow 就是想办法得到一个 encoder 将输入 x 编码为隐变量 z，并且使得 z 服从标准正态分布。**得益于 flow 模型的精巧设计，这个 encoder 是可逆的，从而我们可以立马从 encoder 写出相应的 decoder（生成器）出来，因此，只要 encoder 训练完成，我们就能同时得到 decoder，完成生成模型的构建。**




为了完成这个构思，不仅仅要使得模型可逆，还要使得对应的雅可比行列式容易计算，为此，NICE 提出了加性耦合层，通过多个加性耦合层的堆叠，使得模型既具有强大的拟合能力，又具有单位雅可比行列式。就这样，一种不同于 VAE 和 GAN 的生成模型——flow 模型就这样出来了，它通过巧妙的构造，让我们能直接去拟合概率分布本身。



**待探索的空间**




NICE 提供了 flow 模型这样一种新的思路，并完成了简单的实验，但它同时也留下了更多的未知的空间。flow 模型构思巧妙，相比之下，NICE 的实验则显得过于粗糙：只是简单地堆叠了全连接层，并没有给出诸如卷积层的用法，论文虽然做了多个实验，但事实上真正成功的实验只有 MNIST，说服力不够。 




因此，flow 模型还需要进一步挖掘，才能在生成模型领域更加出众。**这****些拓展，由它的“继承者”RealNVP 和 Glow 模型完成了，可以说，它们的工作使得 flow 模型大放异彩，成为生成模型领域的佼佼者。**




# RealNVP




这部分我们来介绍 RealNVP 模型，它是 NICE 的改进，来自论文 ***Density estimation using Real NVP***[1]。**它一般化了耦合层，并成功地在耦合模型中引入了卷积层，使得可以更好地处理图像问题。更进一步地，它还提出了多尺度层的设计，这能够降低计算量，通过还提供了强大的正则效果，使得生成质量得到提升**。至此，flow 模型的一般框架开始形成。 




后面的 Glow 模型基本上沿用了 RealNVP 的框架，只是对部分内容进行了修改（比如引入了可逆 1x1 卷积来代替排序层）。不过值得一提的是，Glow 简化了 RealNVP 的结构，表明 RealNVP 中某些比较复杂的设计是没有必要的。因此本文在介绍 RealNVP 和 Glow 时，并没有严格区分它们，而只是突出它们的主要贡献。  




**仿射耦合层**




其实 NICE 和 RealNVP 的第一作者都是 Laurent Dinh，他是 Bengio 的博士生，他对 flow 模型的追求和完善十分让我钦佩。在第一篇 NICE 中，他提出了加性耦合层，事实上也提到了乘性耦合层，只不过没有用上；而在 RealNVP 中，加性和乘性耦合层结合在一起，成为一个一般的“仿射耦合层”。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8AkB0b1cyfQoXlYxefIlOmyp2N7DibiaXZQbeDk3LibzVKicb7OkicSZ9vYg/640)




这里的 s,t 都是 x1 的向量函数，形式上第二个式子对应于 x2 的一个仿射变换，因此称为“仿射耦合层”。




仿射耦合的雅可比矩阵依然是一个三角阵，但对角线不全为 1，用分块矩阵表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8gicUX4lavGfUCF8SNuk080ORm0GDBhD0IKwicDZM5tebichrib6NOibaaGQ/640)




很明显，它的行列式就是 s 各个元素之积。为了保证可逆性，一般我们约束 s 各个元素均大于零，所以一般情况下，我们都是直接用神经网络建模输出 log s，然后取指数形式![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8OZgFiaqlVT0DC95ho8ExTJPP0FKs2whVQkTfodvqkO3MKpBwFu2jBAQ/640)。 




注：从仿射层大概就可以知道 RealNVP 的名称来源了，它的全称为“real-valued non-volume preserving”，强行翻译为“实值非体积保持”。相对于加性耦合层的行列式为 1，RealNVP 的雅可比行列式不再恒等于 1，而我们知道行列式的几何意义就是体积（请参考**《新理解矩阵5：体积=行列式》**[2]），所以行列式等于 1 就意味着体积没有变化，而仿射耦合层的行列式不等于 1 就意味着体积有所变化，所谓“非体积保持”。 




**随机打乱维度**




在 NICE 中，作者通过交错的方式来混合信息流（这也理论等价于直接反转原来的向量），如下图（对应地，这里已经换为本文的仿射耦合层图示）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8hW9n5NnImm4GkZBemfP2mEN88RxjbLsjL9gl8XKk2fU9sQ1DTWmuTw/640)

▲ NICE通过交叉耦合，充分混合信息




而 RealNVP 发现，通过随机的方式将向量打乱，可以使信息混合得更加充分，最终的 loss 可以更低，如图：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8OozojlQo6Wzvn9kBqThJicpOWQSJejFXv7uQp9sy215QVJNseqkfTvw/640)

▲ RealNVP通过随机打乱每一步输出的整个向量，使得信息混合得更充分均匀




这里的随机打乱，就是指将每一步 flow 输出的两个向量 h1,h2 拼接成一个向量 h，然后将这个向量重新随机排序。




**引入卷积层**




RealNVP 中给出了在 flow 模型中合理使用卷积神经网络的方案，这使得我们可以更好地处理图像问题，并且减少参数量，还可以更充分发挥模型的并行性能。




注意，不是任意情况下套用卷积都是合理的，**用卷积的前提是输入（在空间维度）具有局部相关性**。图像本身是具有局部相关性的，因为相邻之间的像素是有一定关联的，因此一般的图像模型都可以使用卷积。




**但是我们注意 flow 中的两个操作：**




1. 将输入分割为两部分 x1,x2，然后输入到耦合层中，而模型 s,t 事实上只对 x1 进行处理；




2. 特征输入耦合层之前，要随机打乱原来特征的各个维度（相当于乱序的特征）。这两个操作都会破坏局部相关性，比如分割操作有可能割裂原来相邻的像素，随机打乱也可能将原来相邻的两个像素分割得很远。




所以，如果还要坚持使用卷积，就要想办法保留这种空间的局部相关性。我们知道，一幅图像有三个轴：高度（height）、宽度（width）、通道（channel），前两个属于空间轴，显然具有局部相关性，因此能“搞”的就只有“通道”轴。




为此，**RealNVP 约定分割和打乱操作，都只对“通道”轴执行**。也就是说，沿着通道将输入分割为 x1,x2 后，x1 还是具有局部相关性的，还有沿着通道按着同一方式打乱整体后，空间部分的相关性依然得到保留，因此在模型 s,t 中就可以使用卷积了。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8pgqUJHZWWl0CC6QwicraiageqIUV4m4K0RzXRnQMaCsazicRoicY6FWXCA/640)

▲ 沿着通道轴进行分割，不损失空间上的局部相关性




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8xFCTFHgHp3JWq8htsnS5v5jOPY5SPCQF0WvDOhn4BRADicKO2cuuROA/640)

▲ 沿着空间轴交错（棋盘）分割，也是一种保持空间局部相关性的方案




注：在 RealNVP 中，将输入分割为两部分的操作称为 mask，因为这等价于用 0/1 来区别标注原始输入。除了前面说的通过通道轴对半分的 mask 外，RealNVP 事实上还引入了一种空间轴上的交错 mask，如上图的右边，这种 mask 称为棋盘式 mask（格式像国际象棋的棋盘）。




这种特殊的分割也保留了空间局部相关性，原论文中是两种 mask 方式交替使用的，但这种棋盘式 mask 相对复杂，也没有什么特别明显的提升，所以在 Glow 中已经被抛弃。




不过想想就会发现有问题。一般的图像通道轴就只有三维，像 MNIST 这种灰度图还只有一维，怎么分割成两半？又怎么随机打乱？为了解决这个问题，RealNVP 引入了称为 squeeze 的操作，来让通道轴具有更高的维度。




其思想很简单：直接 reshape，但 reshape 时局部地进行。具体来说，假设原来图像为 h×w×c 大小，前两个轴是空间维度，然后沿着空间维度分为一个个 2×2×c 的块（这个 2 可以自定义），然后将每个块直接 reshape 为 1×1×4c，也就是说最后变成了 h/2×w/2×4c。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8USLzZR8Ria58K2Y2JTyPa35KsDjibtcAoCCJlNIGJY7lfH4wG6fuibVicA/640)

▲ squeeze操作图示，其中2x2的小区域可以换为自定义大小的区域




有了 squeeze 这个操作，我们就可以增加通道轴的维数，但依然保留局部相关性，从而我们前面说的所有事情都可以进行下去了，所以 squeeze 成为 flow 模型在图像应用中的必备操作。




**多尺度结构**




除了成功地引入卷积层外，RealNVP 的另一重要进展是加入了多尺度结构。跟卷积层一样，这也是一个既减少了模型复杂度、又提升了结果的策略。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8YH8yic8KISolIQ1iaaJCyntia3nF0IyOibb5ArVbBKDYyzuB5pmH0QJYbw/640)

▲ RealNVP中的多尺度结构图示




多尺度结构其实并不复杂，如图所示。原始输入经过第一步 flow 运算（“flow 运算”指的是多个仿射耦合层的复合）后，输出跟输入的大小一样，这时候将输入对半分开两半 z1,z2（自然也是沿着通道轴），其中 z1 直接输出，而只将 z2 送入到下一步 flow 运算，后面的依此类推。比如图中的特例，最终的输出由 z1,z3,z5 组成，总大小跟输入一样。




**多尺度结构有点“分形”的味道，原论文说它启发于 VGG**。每一步的多尺度操作直接将数据尺寸减少到原来的一半，显然是非常可观的。但有一个很重要的细节，在 RealNVP 和 Glow 的论文中都没有提到，我是看了源码才明白的，那就是**最终的输出 [z1,z3,z5] 的先验分布应该怎么取？**按照 flow 模型的通用假设，直接设为一个标准正态分布？




事实上，作为不同位置的多尺度输出，z1,z3,z5 的地位是不对等的，而如果直接设一个总体的标准正态分布，那就是强行将它们对等起来，这是不合理的。最好的方案，应该是写出条件概率公式：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8CJwCLd6vmGPyZibf9ANl2gYXteGsKia2qJ0S45VHiaSENibR71qpvoODGQ/640)




由于 z3,z5 是由 z2 完全决定的，z5 也是由 z4 完全决定的，因此条件部分可以改为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8hzSy9NLeR0qUoxw0S1PX3gmMWDDQTAutHxVqAqDho8qqDvyqGybX4w/640)




RealNVP 和 Glow 假设右端三个概率分布都是正态分布，其中 p(z1|z2) 的均值方差由 z2 算出来（可以直接通过卷积运算，这有点像 VAE），p(z3|z4) 的均值方差由 z4 算出来，p(z5) 的均值方差直接学习出来。




显然这样的假设会比简单认为它们都是标准正态分布要有效得多。我们还可以换一种表述方法：上述的先验假设相当于做了如下的变量代换：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8XTIJ0fibME1rC6R51SichtgXKHfPOLwW12wicWvcq9HRFBdosZJFdKTHQ/640)




然后认为 [ẑ1,ẑ3,ẑ5] 服从标准正态分布。同 NICE 的尺度变换层一样，这三个变换都会导致一个非 1 的雅可比行列式，也就是要往 loss 中加入形如![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd837uGJoj4ERkgNukK3VicHVg9sIz9Zxicm9GhW2b4DpqlRKnbTHVbOuug/640)的这一项。 




乍看之下多尺度结构就是为了降低运算量，但并不是那么简单。由于 flow 模型的可逆性，输入输出维度一样，事实上这会存在非常严重的维度浪费问题，这往往要求我们需要用足够复杂的网络去缓解这个维度浪费。




**多尺度结构相当于抛弃了 p(z) 是标准正态分布的直接假设，而采用了一个组合式的条件分布**，这样尽管输入输出的总维度依然一样，但是不同层次的输出地位已经不对等了，模型可以通过控制每个条件分布的方差来抑制维度浪费问题（极端情况下，方差为 0，那么高斯分布坍缩为狄拉克分布，维度就降低 1），条件分布相比于独立分布具有更大的灵活性。而如果单纯从 loss 的角度看，多尺度结构为模型提供了一个强有力的正则项（相当于多层图像分类模型中的多条直连边）。




# Glow




整体来看，Glow 模型在 RealNVP 的基础上引入了 1x1 可逆卷积来代替前面说的打乱通道轴的操作，并且对 RealNVP 的原始模型做了简化和规范，使得它更易于理解和使用。




■ 论文 | https://www.paperweekly.site/papers/2101

■ 博客 | https://blog.openai.com/glow/

■ 源码 | https://github.com/openai/glow




**可逆1x1卷积**




这部分介绍 Glow 的主要改进工作：可逆 1x1 卷积。 




**置换矩阵**




可逆 1x1 卷积源于我们对置换操作的一般化。我们知道，在 flow 模型中，一步很重要的操作就是将各个维度重新排列，NICE 是简单反转，而 RealNVP 则是随机打乱。不管是哪一种，都对应着向量的置换操作。 




事实上，对向量的置换操作，可以用矩阵乘法来描述，比如原来向量是 [1,2,3,4]，分别交换第一、二和第三、四两个数，得到 [2,1,4,3]，这个操作可以用矩阵乘法来描述：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8uzEL7loVC2BjvsDSSbflibNk74XHg7A5AJ8MW0XcicaajibjMYmkDhR2Q/640)




其中右端第一项是“由单位矩阵不断交换两行或两列最终得到的矩阵”称为置换矩阵。




**一般化置换**




既然这样，那很自然的想法就是：为什么不将置换矩阵换成一般的可训练的参数矩阵呢？所谓 1x1 可逆卷积，就是这个想法的结果。 




注意，我们一开始提出 flow 模型的思路时就已经明确指出，flow 模型中的变换要满足两个条件：一是可逆，二是雅可比行列式容易计算。如果直接写出变换：





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8HHaEZbAMESv2O0hc9r90uZrkibKWgribD6Iztx1PwXK9VsQ0iaMl0IJqg/640)




那么它就只是一个普通的没有 bias 的全连接层，并不能保证满足这两个条件。为此，我们要做一些准备工作。首先，我们让 h 和 x 的维度一样，也就是说 W 是一个方阵，这是最基本的设置；其次，由于这只是一个线性变换，因此它的雅可比矩阵就是![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8C1GzVKPCmrgLsykbgOEAMQh6W61ezxGyW905giaYYy1F6uJLDMhApPg/640)，所以它的行列式就是 det *W*，因此我们需要把 −log |det *W*| 这一项加入到 loss 中；最后，初始化时为了保证 W 的可逆性，一般使用“随机正交矩阵”初始化。



**利用LU分解**




以上做法只是一个很基本的解决方案，我们知道，算矩阵的行列式运算量特别大，还容易溢出。而 Glow 给出了一个非常巧妙的解决方案：LU 分解的逆运用。具体来说，是因为任意矩阵都可以分解为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8Vb6liaOrhmsJ4MglnxCvBvmsHKFb3p2k3tbRSt3KuwuZSydCfMt0dhw/640)




其中 P 是一个置换矩阵，也就是前面说的 shuffle 的等价矩阵；L 是一个下三角阵，对角线元素全为 1；U 是一个上三角阵。这种形式的分解称为 LU 分解。如果知道这种矩阵的表达形式，显然求雅可比行列式是很容易的，它等于：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8IhJTkSXROqXW0145qibvBIj246ia7nzVZXUvnhRwCY0p9CWqkwS8BK8g/640)




也就是 U 的对角线元素的绝对值对数之和。既然任意矩阵都可以分解成 (7) 式，我们何不直接设W的形式为 (7) 式？这样一来矩阵乘法计算量并没有明显提升，但求行列式的计算量大大降低，而且计算起来也更为容易。




这就是 Glow 中给出的技巧：先随机生成一个正交矩阵，然后做 LU 分解，得到 P,L,U，固定 P，也固定 U 的对角线的正负号，然后约束 L 为对角线全 1 的下三角阵，U 为上三角阵，优化训练 L,U 的其余参数。 




**结果分析**




上面的描述只是基于全连接的。如果用到图像中，那么就要在每个通道向量上施行同样的运算，这等价于 1x1 的卷积，这就是所谓的可逆 1x1 卷积的来源。事实上我觉得这个名字起得不大好，它本质上就是共享权重的、可逆的全连接层，单说 1x1 卷积，就把它局限在图像中了，不够一般化。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8lFKnUVXfuxWFPlbAI6YuicVnx33lI8OicRbabHt1YkePkuxQxgZN0Xiaw/640)

▲ 三种不同的打乱方案最终的loss曲线比较（来自OpenAI博客）




Glow 的论文做了对比实验，表明相比于直接反转，shuffle 能达到更低的 loss，而相比 shuffle，可逆 1x1 卷积能达到更低的 loss。我自己的实验也表明了这一点。




不过要指出的是：可逆 1x1 卷积虽然能降低 loss，但是有一些要注意的问题。**第一，loss 的降低不代表生成质量的提高**，比如 A 模型用了 shuffle，训练 200 个 epoch 训练到 loss=-50000，B 模型用了可逆卷积，训练 150 个 epoch 就训练到 loss=-55000，那么通常来说在当前情况下 B 模型的效果还不如 A（假设两者都还没有达到最优）。事实上可逆 1x1 卷积只能保证大家都训练到最优的情况下，B 模型会更优。**第二，在我自己的简单实验中貌似发现，用可逆 1x1 卷积达到饱和所需要的 epoch 数，要远多于简单用 shuffle 的 epoch 数。**



**Actnorm**




 RealNVP 中用到了 BN 层，而 Glow 中提出了名为 Actnorm 的层来取代 BN。不过，所谓 Actnorm 层事实上只不过是 NICE 中的尺度变换层的一般化，也就是 (5) 式提到的缩放平移变换：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8siabfCrvW8vcSluKGMW6UicjPr9hxumkmAUsfxGcfqTvvicmVFQjGbE9Q/640)




其中 μ,σ 都是训练参数。Glow 在论文中提出的创新点是用初始的 batch 的均值和方差去初始化 μ,σ 这两个参数，但事实上所提供的源码并没有做到这一点，纯粹是零初始化。




所以，这一点是需要批评的，纯粹将旧概念换了个新名字罢了。当然，批评的是 OpenAI 在 Glow 中乱造新概念，而不是这个层的效果。缩放平移的加入，确实有助于更好地训练模型。而且，**由于 Actnorm 的存在，仿射耦合层的尺度变换已经显得不那么重要了**。




我们看到，相比于加性耦合层，仿射耦合层多了一个尺度变换层，从而计算量翻了一倍。但事实上相比加性耦合，仿射耦合效果的提升并不高（尤其是加入了 Actnorm 后），**所以要训练大型的模型，为了节省资源，一般都只用加性耦合**，比如 Glow 训练 256x256 的高清人脸生成模型，就只用到了加性耦合。




**源码分析**




事实上 Glow 已经没有什么可以特别解读的了。但是 Glow 整体的模型比较规范，我们可以逐步分解一下 Glow 的模型结构，为我们自己搭建类似的模型提供参考。这部分内容源自我对 Glow 源码的阅读，主要以示意图的方式给出。




**模型总图**




整体来看，Glow 模型并不复杂，就是在输入加入一定量的噪声，然后输入到一个 encoder 中，最终用“输出的平均平方和”作为损失函数（可以将模型中产生的对数雅可比行列式视为正则项），注意，loss 不是“平方平均误差（MSE）”，而仅仅是输出的平方和，也就是不用减去输入。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8ZiatcAPKGxV30qsQy106TcT2wTYYhyGZLK8ByX3FO5OcQwIOgsnpDPg/640)

▲ Glow模型总图




**encoder **




下面对总图中的 encoder 进行分解，大概流程为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd86uNcDSRatDKhCVGSsJLM4QFdB4BCuEDKDrecg76wzcXWxr87umGia6A/640)

▲ encoder流程图




encoder 由 L 个模块组成，这些模块在源码中被命名为 revnet，每个模块的作用是对输入进行运算，然后将输出对半分为两份，一部分传入下一个模块，一部分直接输出，这就是前面说的多尺度结构。Glow 源码中默认 L=3，但对于 256x256 的人脸生成则用到 L=6。



**revnet **




现在来进一步拆解 encoder，其中 revnet 部分为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd843KxA3HTic0I3dia0UwcKyIEP4CGDaGgdJkjIoVEiazuyvTW4ibIXRtJdg/640)

▲ revnet结构图




其实它就是前面所说的单步 flow 运算，在输入之前进行尺度变换，然后打乱轴，并且进行分割，接着输入到耦合层中。如此训练 K 次，这里的 K 称为“深度”，Glow 中默认是 32。其中 actnorm 和仿射耦合层会带来非 1 的雅可比行列式，也就是会改动 loss，在图上也已注明。 




**split2d**




Glow 中的定义的 split2d 不是简单的分割，而是混合了对分割后的变换运算，也就是前面所提到的多尺度输出的先验分布选择。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8rlb6p55qGZC1WrMqgptlBIAxMbAbUEyO08bhlLIAb2ztunXQse4Xiaw/640)

▲ glow中的split2d并不是简单的分割




对比 (5) 和 (9)，我们可以发现条件先验分布与 Actnorm 的区别仅仅是缩放平移量的来源，Actnorm 的缩放平移参数是直接优化而来，而先验分布这里的缩放平移量是由另一部分通过某个模型计算而来，事实上我们可以认为这种一种条件式 Actnorm（Cond Actnorm）。




**f**




最后是 Glow 中的耦合层的模型（放射耦合层的 s,t），源码中直接命名为 f，它用了三层 relu 卷积：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8bjKgSl5KeiaO2hGWxj4xnhlYf3OvkqbpeZCVj6TepLcBFTRjUDHeUmA/640)

▲ glow中耦合层的变换模型




其中最后一层使用零初始化，这样就使得初始状态下输入输出一样，即初始状态为一个恒等变换，这有利于训练深层网络。




# 复现




可以看到 RealNVP 其实已经做好了大部分工作，而 Glow 在 RealNVP 的基础上进行去芜存菁，并加入了自己的一些小修改（1x1 可逆卷积）和规范。但不管怎么样，这是一个值得研究的模型。




**Keras版本**




官方开源的 Glow 是 TensorFlow 版的。这么有意思的模型，怎么能少得了 Keras 版呢，先奉上笔者实现的 Keras 版：




https://github.com/bojone/flow/blob/master/glow.py 




已经 pull request 到 Keras 官方的 examples，希望过几天能在 Keras 的 github 上看到它。




由于某些函数的限制，目前只支持 TensorFlow 后端，我的测试环境包括：Keras 2.1.5 + tensorflow 1.2 和 Keras 2.2.0 + tensorflow 1.8，均在 Python 2.7 下测试。




**效果测试**




刚开始读到 Glow 时，我感到很兴奋，仿佛像发现了新大陆一样。经过一番学习后，我发现......Glow 确实是一块新大陆，然而却非我等平民能轻松登上的。 




让我们来看 Glow 的 github 上的两个 issue：




> 
***How many epochs will be take when training celeba?*** [3]

The samples we show in the paper are after about 4000 training epochs... 




***Anyone reproduced the celeba-HQ results in the paper？ ***[4]

Yes we trained with 40 GPU's for about a week, but samples did start to look good after a couple of days...





我们看到 256x256 的高清人脸图像生成，需要训练 4000 个 epoch，用 40 个 GPU 训练了一周，简单理解就是用 1 个 GPU 训练一年...（卒）




好吧，我还是放弃这可望而不可及的任务吧，我们还是简简单单玩个 64x64，不，还是 32x32 的人脸生成，做个 demo 出来就是了。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd83pb9ppaZgE7KSIfvwYtzxECt6mXpNE1hYbvTkyfRCibf80ZnD9srS7g/640)

▲ 用glow模型生成的32x32人脸，150个epoch




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8rTDIdrBibWSGhsOg6eyK3w9jPUrnp9T3RqACZnyuYGInOALSOgAPf3w/640)

▲ 用glow模型生成的cifar10，700个epoch




感觉还可以吧，我用的是 L=3,K=6，每个 epoch 要 70s 左右（GTX1070）。跑了 150 个 epoch，这里的 epoch 跟通常概念的 epoch 不一样，我这里的一个 epoch 就是随机抽取的 3.2 万个样本，如果每次跑完完整的 epoch，那么用时更久。同样的模型，顺手也跑了一下 cifar10，跑了 700 个 epoch，不过效果不大好。就是远看似乎还可以，近看啥都不是的那种。




当然，其实 cifar10 虽然不大（32x32），但事实上生成 cifar10 可比生成人脸难多了（不管是哪种生成模型），我们就跳过吧。话说 64x64 的人脸，我也作死地尝试了一下，这时候用了 L=3,K=10，跑了 200 个 epoch（这时候每个 epoch 要 6 分钟了）。结果..……




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgk14Nr7zvT2HUSjRzU7HXd8J4ziclNjmP698sZb9U2ZDV5gZ2aoCuaGOuqVTheK7h41cYzLvjdKDdA/640)

▲ 用glow模型生成的64x64人脸，230个epoch




人脸是人脸了，不过看上去更像妖魔脸。看来网络深度和 epoch 数都还不够，我也跑不下去了。




# 艰难结束




好了，对 RealNVP 和 Glow 的介绍终于可以结束了。本着对 Glow 的兴趣，利用前后两篇文章把三个 flow 模型都捋了一遍，希望对读者有帮助。




**总体来看，诸如 Glow 的 flow 模型整体确实很优美，但运算量还是偏大了，训练时间过长，不像一般的 GAN 那么友好。**个人认为 flow 模型要在当前以 GAN 为主的生成模型领域中站稳脚步，还有比较长的路子要走，可谓任重而道远呀。




# 参考文献




[1]. Dinh, L., Sohl-Dickstein, J., and Bengio, S. (2016). Density estimation using Real NVP. arXiv preprint arXiv:1605.08803.

[2]. https://kexue.fm/archives/2208

[3]. https://github.com/openai/glow/issues/14#issuecomment-406650950

[4]. https://github.com/openai/glow/issues/37#issuecomment-410019221




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[从无监督构建词库看「最小熵原理」](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488802&idx=1&sn=eb35229374ee283d5c54d58ae277b9f0&chksm=96e9caa2a19e43b4f624eac3d56532cb9dc7ca017c9e0eaf96387e20e5f985e37da833fbddfd&scene=21#wechat_redirect)

- 
[基于CNN的阅读理解式问答模型：DGCNN](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488480&idx=1&sn=7bb9a4dd60680df5013670287a9e0cc2&chksm=96e9cc60a19e457618b2ffdea2a7e13ba172ea3fdfbfda07de53aae57126047f1b71a6969d76&scene=21#wechat_redirect)

- 
[再谈最小熵原理：飞象过河之句模版和语言结构](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247489564&idx=1&sn=a5d2191dd978e7917f06f66022ce8358&chksm=96e9c79ca19e4e8ac874f0fa5808b99b7323149d42a78fade690c6d60c0337ecabe043f616d1&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)


- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

- 
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)

- 
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)












**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**



总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。









