# 硬核NeruIPS 2018最佳论文，一个神经了的常微分方程 - Paper weekly - CSDN博客





2018年12月28日 12:02:38[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：113









机器之心原创

****作者：蒋思源****




> 
这是一篇神奇的论文，以前一层一层叠加的神经网络似乎突然变得连续了，反向传播也似乎不再需要一点一点往前传、一层一层更新参数了。





在最近结束的 NeruIPS 2018 中，来自多伦多大学的陈天琦等研究者成为最佳论文的获得者。他们提出了一种名为神经常微分方程的模型，这是新一类的深度神经网络。神经常微分方程不拘于对已有架构的修修补补，它完全从另外一个角度考虑如何以连续的方式借助神经网络对数据建模。在陈天琦的讲解下，机器之心将向各位读者介绍这一令人兴奋的神经网络新家族。




在与机器之心的访谈中，陈天琦的导师 David Duvenaud 教授谈起这位学生也是赞不绝口。Duvenaud 教授认为陈天琦不仅是位理解能力超强的学生，钻研起问题来也相当认真透彻。他说：「天琦很喜欢提出新想法，他有时会在我提出建议一周后再反馈：『老师你之前建议的方法不太合理。但是我研究出另外一套合理的方法，结果我也做出来了。』」Ducenaud 教授评价道，现如今人工智能热度有增无减，教授能找到优秀博士生基本如同「鸡生蛋还是蛋生鸡」的问题，顶尖学校的教授通常能快速地招纳到博士生，「我很幸运地能在事业起步阶段就遇到陈天琦如此优秀的学生。」




本文主要介绍神经常微分方程背后的细想与直观理解，很多延伸的概念并没有详细解释，例如大大降低计算复杂度的连续型流模型和官方 PyTorch 代码实现等。这一篇文章重点对比了神经常微分方程（ODEnet）与残差网络，我们不仅能通过这一部分了解如何从熟悉的 ResNet 演化到 ODEnet，同时还能还有新模型的前向传播过程和特点。




其次文章比较关注 ODEnet 的反向传播过程，即如何通过解常微分方程直接把梯度求出来。这一部分与传统的反向传播有很多不同，因此先理解反向传播再看源码可能是更好的选择。值得注意的是，ODEnet 的反传只有常数级的内存占用成本。



- 
ODEnet 的 PyTorch 实现地址：https://github.com/rtqichen/torchdiffeq

- 
ODEnet 论文地址：https://arxiv.org/abs/1806.07366





如下展示了文章的主要结构：



- 
常微分方程

- 
从残差网络到微分方程

- 
    从微分方程到残差网络


- 
    网络对比

- 
神经常微分方程

- 
   反向传播

- 
    反向传播怎么做

- 
连续型的归一化流

- 
    变量代换定理






**常微分方程**




其实初读这篇论文，还是有一些疑惑的，因为很多概念都不是我们所熟知的。因此如果想要了解这个模型，那么同学们，你们首先需要回忆高数上的微分方程。有了这样的概念后，我们就能愉快地连续化神经网络层级，并构建完整的神经常微分方程。




常微分方程即只包含单个自变量 x、未知函数 f(x) 和未知函数的导数 f'(x) 的等式，所以说 f'(x) = 2x 也算一个常微分方程。但更常见的可以表示为 df(x)/dx = g(f(x), x)，其中 g(f(x), x) 表示由 f(x) 和 x 组成的某个表达式，这个式子是扩展一般神经网络的关键，我们在后面会讨论这个式子怎么就连续化了神经网络层级。




一般对于常微分方程，我们希望解出未知的 f(x)，例如 f'(x) = 2x 的通解为 f(x)=x^2 +C，其中 C 表示任意常数。而在工程中更常用数值解，即给定一个初值 f(x_0)，我们希望解出末值 f(x_1)，这样并不需要解出完整的 f(x)，只需要一步步逼近它就行了。




现在回过头来讨论我们熟悉的神经网络，本质上不论是全连接、循环还是卷积网络，它们都类似于一个非常复杂的复合函数，复合的次数就等于层级的深度。例如两层全连接网络可以表示为 Y=f(f(X, θ1), θ2)，因此每一个神经网络层级都类似于万能函数逼近器。




因为整体是复合函数，所以很容易接受复合函数的求导方法：链式法则，并将梯度从最外一层的函数一点点先向里面层级的函数传递，并且每传到一层函数，就可以更新该层的参数 θ。现在问题是，我们前向传播过后需要保留所有层的激活值，并在沿计算路径反传梯度时利用这些激活值。这对内存的占用非常大，因此也就限制了深度模型的训练过程。




神经常微分方程走了另一条道路，它使用神经网络参数化隐藏状态的导数，而不是如往常那样直接参数化隐藏状态。这里参数化隐藏状态的导数就类似构建了连续性的层级与参数，而不再是离散的层级。因此参数也是一个连续的空间，我们不需要再分层传播梯度与更新参数。总而言之，神经微分方程在前向传播过程中不储存任何中间结果，因此它只需要近似常数级的内存成本。




**从残差网络到微分方程**




残差网络是一类特殊的卷积网络，它通过残差连接而解决了梯度反传问题，即当神经网络层级非常深时，梯度仍然能有效传回输入端。下图为原论文中残差模块的结构，残差块的输出结合了输入信息与内部卷积运算的输出信息，这种残差连接或恒等映射表示深层模型至少不能低于浅层网络的准确度。这样的残差模块堆叠几十上百个就是非常深的残差神经网络。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgkZ8ae0J1Ha7ZwFzQHWqV64mnPxqXJWpFGyM8RDqoKYTdjUtAljY5sw/640?wx_fmt=jpeg)




如果我们将上面的残差模块更加形式化地表示为以下方程：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgaaS1DuBGLjoibtqrGfaJwcpwzZXAXDWxibYFPs42MxLljBe7MCF0hgsw/640?wx_fmt=png)




其中 h_t 是第 t 层隐藏单元的输出值，f 为通过θ_t 参数化的神经网络。该方程式表示上图的整个残差模块，如果我们其改写为残差的形式，即 h_t+1 - h_t = f(h_t, θ_t )。那么我们可以看到神经网络 f 参数化的是隐藏层之间的残差，f 同样不是直接参数化隐藏层。




ResNet 假设层级的离散的，第 t 层到第 t+1 层之间是无定义的。那么如果这中间是有定义的呢？残差项 h_t0 - h_t1 是不是就应该非常小，以至于接近无穷小？这里我们少考虑了分母，即残差项应该表示为 (h_t+1 - h_t )/1，分母的 1 表示两个离散的层级之间相差 1。所以再一次考虑层级间有定义，我们会发现残差项最终会收敛到隐藏层对 t 的导数，而神经网络实际上参数化的就是这个导数。




所以若我们在层级间加入更多的层，且最终趋向于添加了无穷层时，神经网络就连续化了。可以说残差网络其实就是连续变换的欧拉离散化，是一个特例，我们可以将这种连续变换形式化地表示为一个常微分方程：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgeN4BZPSibmZcUia2DHSI71QEWgvv4JaicGEgVibQ3nyDOjEQjGxNPxPSdg/640?wx_fmt=png)




如果从导数定义的角度来看，当 t 的变化趋向于无穷小时，隐藏状态的变化 dh(t) 可以通过神经网络建模。当 t 从初始一点点变化到终止，那么 h(t) 的改变最终就代表着前向传播结果。这样利用神经网络参数化隐藏层的导数，就确确实实连续化了神经网络层级。




现在若能得出该常微分方程的数值解，那么就相当于完成了前向传播。具体而言，若 h(0)=X 为输入图像，那么终止时刻的隐藏层输出 h(T) 就为推断结果。这是一个常微分方程的初值问题，可以直接通过黑箱的常微分方程求解器（ODE Solver）解出来。而这样的求解器又能控制数值误差，因此我们总能在计算力和模型准确度之间做权衡。




形式上来说，现在就需要变换方程 (2) 以求出数值解，即给定初始状态 h(t_0) 和神经网络的情况下求出终止状态 h(t_1)：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgbLVGB7G9qVMfHtziasPfWlUBibryyL5rBVyGx3Ricp2KicgsJzm6Sj4QuQ/640?wx_fmt=png)




如上所示，常微分方程的数值解 h(t_1) 需要求神经网络 f 从 t_0 到 t_1 的积分。我们完全可以利用 ODE solver 解出这个值，这在数学物理领域已经有非常成熟的解法，我们只需要将其当作一个黑盒工具使用就行了。




**从微分方程到残差网络**




前面提到过残差网络是神经常微分方程的特例，可以说残差网络是欧拉方法的离散化。两三百年前解常微分方程的欧拉法非常直观，即 h(t +Δt) = h(t) + Δt×f(h(t), t)。每当隐藏层沿 t 走一小步Δt，新的隐藏层状态 h(t +Δt) 就应该近似在已有的方向上迈一小步。如果这样一小步一小步从 t_0 走到 t_1，那么就求出了 ODE 的数值解。




如果我们令 Δt 每次都等于 1，那么离散化的欧拉方法就等于残差模块的表达式 h(t+1) = h(t) + f(h(t), t)。但是欧拉法只是解常微分方程最基础的方法，它每走一步都会产生一点误差，且误差会累积起来。近百年来，数学家构建了很多现代 ODE 求解方法，它们不仅能保证收敛到真实解，同时还能控制误差水平。




陈天琦等研究者构建的 ODE 网络就使用了一种适应性的 ODE solver，它不像欧拉法移动固定的步长，相反它会根据给定的误差容忍度选择适当的步长逼近真实解。如下图所示，左边的残差网络定义有限转换的离散序列，它从 0 到 1 再到 5 是离散的层级数，且在每一层通过激活函数做一次非线性转换。此外，黑色的评估位置可以视为神经元，它会对输入做一次转换以修正传递的值。而右侧的 ODE 网络定义了一个向量场，隐藏状态会有一个连续的转换，黑色的评估点也会根据误差容忍度自动调整。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgfHwia9icPQgNP7MJWB18MCib3a5eJNvSdF6ZBm03pKsl8TQDHlTa0IBvg/640?wx_fmt=png)




**网络对比**




在 David 的 Oral 演讲中，他以两段伪代码展示了 ResNet 与 ODEnet 之间的差别。如下展示了 ResNet 的主要过程，其中 f 可以视为卷积层，ResNet 为整个模型架构。在卷积层 f 中，h 为上一层输出的特征图，t 确定目前是第几个卷积层。ResNet 中的循环体为残差连接，因此该网络一共 T 个残差模块，且最终返回第 T 层的输出值。




```python
def f(h, t, θ):
    return nnet(h, θ_t)

def resnet(h):
    for t in [1:T]:
        h = h + f(h, t, θ)
    return h
```




相比常见的 ResNet，下面的伪代码就比较新奇了。首先 f 与前面一样定义的是神经网络，不过现在它的参数θ是一个整体，同时 t 作为独立参数也需要馈送到神经网络中，这表明层级之间也是有定义的，它是一种连续的网络。而整个 ODEnet 不需要通过循环搭建离散的层级，它只要通过 ODE solver 求出 t_1 时刻的 h 就行了。




```python
def f(h, t, θ):
    return nnet([h, t], θ)

def ODEnet(h, θ):
    return ODESolver(f, h, t_0, t_1, θ)
```




除了计算过程不一样，陈天琦等研究者还在 MNSIT 测试了这两种模型的效果。他们使用带有 6 个残差模块的 ResNet，以及使用一个 ODE Solver 代替这些残差模块的 ODEnet。以下展示了不同网络在 MNSIT 上的效果、参数量、内存占用量和计算复杂度。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgolwAicDWMCqow8p1CiaUI14VZw0Q8GRptnrwghaxicw1lvlgska4uXxuQ/640?wx_fmt=png)




其中单个隐藏层的 MLP 引用自 LeCun 在 1998 年的研究，其隐藏层只有 300 个神经元，但是 ODEnet 在有相似参数量的情况下能获得显著更好的结果。上表中 L 表示神经网络的层级数，L tilde 表示 ODE Solver 中的评估次数，它可以近似代表 ODEnet 的「层级深度」。值得注意的是，ODEnet 只有常数级的内存占用，这表示不论层级的深度如何增加，它的内存占用基本不会有太大的变化。




**神经常微分方程**




在与 ResNet 的类比中，我们基本上已经了解了 ODEnet 的前向传播过程。首先输入数据 Z(t_0)，我们可以通过一个连续的转换函数（神经网络）对输入进行非线性变换，从而得到 f。随后 ODESolver 对 f 进行积分，再加上初值就可以得到最后的推断结果。如下所示，残差网络只不过是用一个离散的残差连接代替 ODE Solver。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgJuopKFGibgIKzm3ibzTd4icWAKN3AYCCcrgqU640XwzhAcm0GibVwGrE9Q/640?wx_fmt=png)




在前向传播中，ODEnet 还有几个非常重要的性质，即模型的层级数与模型的误差控制。首先因为是连续模型，其并没有明确的层级数，因此我们只能使用相似的度量确定模型的「深度」，作者在这篇论文中采用 ODE Solver 评估的次数作为深度。




其次，深度与误差控制有着直接的联系，ODEnet 通过控制误差容忍度能确定模型的深度。因为 ODE Solver 能确保在误差容忍度之内逼近常微分方程的真实解，改变误差容忍度就能改变神经网络的行为。一般而言，降低 ODE Solver 的误差容忍度将增加函数的评估的次数，因此类似于增加了模型的「深度」。调整误差容忍度能允许我们在准确度与计算成本之间做权衡，因此我们在训练时可以采用高准确率而学习更好的神经网络，在推断时可以根据实际计算环境调整为较低的准确度。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgOAWa4AKRUy5vicaBahdCaCLEgMzLLbAB40NVDWica3HiaTiacaAL4luXGQ/640?wx_fmt=png)




如原论文的上图所示，a 图表示模型能保证在误差范围为内，且随着误差降低，前向传播的函数评估数增加。b 图展示了评估数与相对计算时间的关系。d 图展示了函数评估数会随着训练的增加而自适应地增加，这表明随着训练的进行，模型的复杂度会增加。




c 图比较有意思，它表示前向传播的函数评估数大致是反向传播评估数的一倍，这恰好表示反向传播中的 adjoint sensitivity 方法不仅内存效率高，同时计算效率也比直接通过积分器的反向传播高。这主要是因为 adjoint sensitivity 并不需要依次传递到前向传播中的每一个函数评估，即梯度不通过模型的深度由后向前一层层传。




**反向传播**




师从同门的 Jesse Bettencourt 向机器之心介绍道，「天琦最擅长的就是耐心讲解。」当他遇到任何无论是代码问题，理论问题还是数学问题，一旦是问了同桌的天琦，对方就一定会慢慢地花时间把问题讲清楚、讲透彻。而 ODEnet 的反向传播，就是这样一种需要耐心讲解的问题。





ODEnet 的反向传播与常见的反向传播有一些不同，我们可能需要仔细查阅原论文与对应的附录证明才能有较深的理解。此外，作者给出了 ODEnet 的 PyTorch 实现，我们也可以通过它了解实现细节。




正如作者而言，训练一个连续层级网络的主要技术难点在于令梯度穿过 ODE Solver 的反向传播。其实如果令梯度沿着前向传播的计算路径反传回去是非常直观的，但是内存占用会比较大而且数值误差也不能控制。作者的解决方案是将前向传播的 ODE Solver 视为一个黑箱操作，梯度很难或根本不需要传递进去，只需要「绕过」就行了。




作者采用了一种名为 adjoint method 的梯度计算方法来「绕过」前向传播中的 ODE Solver，即模型在反传中通过第二个增广 ODE Solver 算出梯度，其可以逼近按计算路径从 ODE Solver 传递回的梯度，因此可用于进一步的参数更新。这种方法如上图 c 所示不仅在计算和内存非常有优势，同时还能精确地控制数值误差。




具体而言，若我们的损失函数为 L()，且它的输入为 ODE Solver 的输出：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgZhObFY9jsSNvicYZ6ic2gV0nib8K01w1eIxF9U9vlQS6lPiaRxm2SuYYSA/640?wx_fmt=png)




我们第一步需要求 L 对 z(t) 的导数，或者说模型损失的变化如何取决于隐藏状态 z(t) 的变化。其中损失函数 L 对 z(t_1) 的导数可以为整个模型的梯度计算提供入口。作者将这一个导数称为 adjoint a(t) = -dL/z(t)，它其实就相当于隐藏层的梯度。




在基于链式法则的传统反向传播中，我们需要从后一层对前一层求导以传递梯度。而在连续化的 ODEnet 中，我们需要将前面求出的 a(t) 对连续的 t 进行求导，由于 a(t) 是损失 L 对隐藏状态 z(t) 的导数，这就和传统链式法则中的传播概念基本一致。下式展示了 a(t) 的导数，它能将梯度沿着连续的 t 向前传，附录 B.1 介绍了该式具体的推导过程。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgG3KKQjaFuzNReUu0jGZTYRibm8WicqNQCRqtsGrCQT1ayUicrpwDtSS2A/640?wx_fmt=png)




在获取每一个隐藏状态的梯度后，我们可以再求它们对参数的导数，并更新参数。同样在 ODEnet 中，获取隐藏状态的梯度后，再对参数求导并积分后就能得到损失对参数的导数，这里之所以需要求积分是因为「层级」t 是连续的。这一个方程式可以表示为：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hghImWDRQw68bWooTJKH8ticrib3E9Typ3ar3CmuX2ORqG3aY4FfKSib4icg/640?wx_fmt=png)




综上，我们对 ODEnet 的反传过程主要可以直观理解为三步骤，即首先求出梯度入口伴随 a(t_1)，再求 a(t) 的变化率 da(t)/dt，这样就能求出不同时刻的 a(t)。最后借助 a(t) 与 z(t)，我们可以求出损失对参数的梯度，并更新参数。当然这里只是简要的直观理解，更完整的反传过程展示在原论文的算法 1。




**反向传播怎么做**




在算法 1 中，陈天琦等研究者展示了如何借助另一个 OED Solver 一次性求出反向传播的各种梯度和更新量。要理解算法 1，首先我们要熟悉 ODESolver 的表达方式。例如在 ODEnet 的前向传播中，求解过程可以表示为 ODEsolver(z(t_0), f, t_0, t_1, θ)，我们可以理解为从 t_0 时刻开始令 z(t_0) 以变化率 f 进行演化，这种演化即 f 在 t 上的积分，ODESolver 的目标是通过积分求得 z(t_1)。




同样我们能以这种方式理解算法 1，我们的目的是利用 ODESolver 从 z(t_1) 求出 z(t_0)、从 a(t_1) 按照方程 4 积出 a(t_0)、从 0 按照方程 5 积出 dL/dθ。最后我们只需要使用 dL/dθ 更新神经网络 f(z(t), t, θ) 就完成了整个反向传播过程。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgGicd3p8NKjAVibicicjNRIlhX6VCYiaiazeaDmrPkBgkjibtIKkRRIdsXJH2Q/640?wx_fmt=png)




如上所示，若初始给定参数θ、前向初始时刻 t_0 和终止时刻 t_1、终止状态 z(t_1) 和梯度入口 ∂L/∂z(t_1)。接下来我们可以将三个积分都并在一起以一次性解出所有量，因此我们可以定义初始状态 s_0，它们是解常微分方程的初值。




注意第一个初值 z(t_1)，其实在前向传播中，从 z(t_0) 到 z(t_1) 都已经算过一遍了，但是模型并不会保留计算结果，因此也就只有常数级的内存成本。此外，在算 a(t) 时需要知道对应的 z(t)，例如 ∂L/∂z(t_0) 就要求知道 z(t_0) 的值。如果我们不能保存中间状态的话，那么也可以从 z(t_1) 到 z(t_0) 反向再算一遍中间状态。这个计算过程和前向过程基本一致，即从 z(t_1) 开始以变化率 f 进行演化而推出 z(t_0)。




定义 s_0 后，我们需要确定初始状态都是怎样「演化」到终止状态的，定义这些演化的即前面方程 (3)、(4) 和 (5) 的被积函数，也就是算法 1 中 aug_dynamics() 函数所定义的。




其中 f(z(t), t, θ) 从 t_1 到 t_0 积出来为 z(t_0)，这第一个常微分方程是为了给第二个提供条件。而-a(t)*∂L/∂z(t) 从 t_1 到 t_0 积出来为 a(t_0)，它类似于传统神经网络中损失函数对第一个隐藏层的导数，整个 a(t) 就相当于隐藏层的梯度。只有获取积分路径中所有隐藏层的梯度，我们才有可能进一步解出损失函数对参数的梯度。




因此反向传播中的第一个和第二个常微分方程 都是为第三个微分方程提供条件，即 a(t) 和 z(t)。最后，从 t_1 到 t_0 积分 -a(t)*∂f(z(t), t, θ)/∂θ 就能求出 dL/dθ。只需要一个积分，我们不再一层层传递梯度并更新该层特定的参数。




如下伪代码所示，完成反向传播的步骤很简单。先定义各变量演化的方法，再结合将其结合初始化状态一同传入 ODESolver 就行了。




```python
def f_and_a([z, a], t):
    return[f, -a*df/da, -a*df/dθ]

[z0, dL/dx, dL/dθ] = 
        ODESolver([z(t1), dL/dz(t), 0], f_and_a, t1, t0)
```




**连续型的归一化流**




这种连续型转换有一个非常重要的属性，即流模型中最基础的变量代换定理可以便捷快速地计算得出。在论文的第四节中，作者根据这样的推导结果构建了一个新型可逆密度模型，它能克服 Glow 等归一化流模型的缺点，并直接通过最大似然估计训练。




**变量代换定理**




对于概率密度估计中的变量代换定理，我们可以从单变量的情况开始。若给定一个随机变量 z 和它的概率密度函数 z∼π(z)，我们希望使用映射函数 x=f(z) 构建一个新的随机变量。函数 f 是可逆的，即 z=g(x)，其中 f 和 g 互为逆函数。现在问题是如何推断新变量的未知概率密度函数 p(x)？




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgscXjTIeBCqb5gWV8QVibyzicCQmlMpca0E9bBzLrwMy7O4DywA040icNQ/640?wx_fmt=png)




通过定义，积分项 ∫π(z)dz 表示无限个无穷小的矩形面积之和，其中积分元Δz 为积分小矩形的宽，小矩形在位置 z 的高为概率密度函数 π(z) 定义的值。若使用 f^−1(x) 表示 f(x) 的逆函数，当我们替换变量的时候，z=f^−1(x) 需要服从 Δz/Δx=(f^−1(x))′。多变量的变量代换定理可以从单变量推广而出，其中 det ∂f/∂z 为函数 f 的雅可比行列式：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgibNOeTMtQBNDIhwJUOkHAz1XmOrjyIlQQ7PAGDpRKn1z4SLn69zEIdA/640?wx_fmt=png)




一般使用变量代换定理需要计算雅可比矩阵∂f/∂z 的行列式，这是主要的限制，最近的研究工作都在权衡归一化流模型隐藏层的表达能力与计算成本。但是研究者发现，将离散的层级替换为连续的转换，可以简化计算，我们只需要算雅可比矩阵的迹就行了。核心的定理 1 如下所示：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgKj7TNrgibrPHibd1cN80N2T7pAtGK9zKRrazdayRs8bmcR8KKNylICWQ/640?wx_fmt=png)




在普通的变量代换定理中，分布的变换函数 f（或神经网络）必须是可逆的，而且要制作可逆的神经网络也很复杂。在陈天琦等研究者定理里，不论 f 是什么样的神经网络都没问题，它天然可逆，所以这种连续化的模型对流模型的应用应该非常方便。




如下所示，随机变量 z(t_0) 及其分布可以通过一个连续的转换演化到 z(t_1) 及其分布：




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/KmXPKA19gW8JHk6AWP4iaCbj8okFsb9hgdjfzewiczkwbap1hH63MOUN26yhIm0jpjexxUtWQz7xUvBg2maiaBfLA/640?wx_fmt=png)







此外，连续型流模型还有很多性质与优势，但这里并不展开。变量代换定理 1 在附录 A 中有完整的证明，感兴趣的读者可查阅原论文了解细节。




最后，神经常微分方程是一种全新的框架，除了流模型外，很多方法在连续变换的改变下都有新属性，这些属性可能在离散激活的情况下很难获得。也许未来会有很多的研究关注这一新模型，连续化的神经网络也会变得多种多样。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看作者其他文章：**




- 
[变分自编码器VAE：原来是这么一回事 | 附开源代码](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)

- 
[再谈变分自编码器VAE：从贝叶斯观点出发](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488093&idx=1&sn=08a77550c0cc7309c34a0a38bad0bcba&chksm=96e9cddda19e44cb7ce6143a7990eb4fc47d114b55b564e727a014538402f7218fc89bf1f3c0&scene=21#wechat_redirect)

- 
[变分自编码器VAE：这样做为什么能成？](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247488238&idx=1&sn=06ffb033332a54279e600c511e1c5c5f&chksm=96e9cd6ea19e44781ee1313b349e0e77631781a2a163e2fd845c841dc2200d988424bd73c4c7&scene=21#wechat_redirect)

- 
[从变分编码、信息瓶颈到正态分布：论遗忘的重要性](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493326&idx=1&sn=7ba19fe14ee11bff0e1c865adcb52ca1&chksm=96ea394ea19db0587dc096898730f7522a8e3a7bb3b55bac576422eea63a987ea97ad5886bca&scene=21#wechat_redirect)


- 
[深度学习中的互信息：无监督提取特征](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492040&idx=1&sn=f90a6b899e62748c4db489ce06276869&chksm=96ea3e48a19db75e8c07d942a4772bb6c784fac7bcb117da2023186546cfe1876b121a8121cc&scene=21#wechat_redirect)

- 
[全新视角：用变分推断统一理解生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)

- 
[细水长flow之NICE：流模型的基本概念与实现](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490842&idx=1&sn=840d5d8038cd923af827eef497e71404&chksm=96e9c29aa19e4b8c45980b39eb28d80408632c8f9a570c9413748b2b5699260190e0d7b4ed16&scene=21#wechat_redirect)

- 
[细水长flow之f-VAEs：Glow与VAEs的联姻](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491695&idx=1&sn=21c5ffecfd6ef87cd4f1f754795d2d63&chksm=96ea3fefa19db6f92fe093e914ac517bd118e80e94ae61b581079023c4d29cedaaa559cb376e&scene=21#wechat_redirect)

- 
[深度学习中的Lipschitz约束：泛化与生成模型](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492180&idx=1&sn=3ea92a3a9f1306efde89ce1777b80da6&chksm=96ea3dd4a19db4c20dcbc9627b0eb307672b4d61008a93c42814fa6728ca7b6f7c293cff1d80&scene=21#wechat_redirect)












**![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/xuKyIMVqtF2cO2WSmiccOqL8YlIwp5Xv2cqdDp6ANbUt8yibCc1cgQQrPHLKhf73icQGHves57M2XMZLJxIhF0e7g/640?)****#****投 稿 通 道#**

** 让你的论文被更多人看到 **





如何才能让更多的优质内容以更短路径到达读者群体，缩短读者寻找优质内容的成本呢？ **答案就是：你不认识的人。**




总有一些你不认识的人，知道你想知道的东西。PaperWeekly 或许可以成为一座桥梁，促使不同背景、不同方向的学者和学术灵感相互碰撞，迸发出更多的可能性。 




PaperWeekly 鼓励高校实验室或个人，在我们的平台上分享各类优质内容，可以是**最新论文解读**，也可以是**学习心得**或**技术干货**。我们的目的只有一个，让知识真正流动起来。




📝 **来稿标准：**

• 稿件确系个人**原创作品**，来稿需注明作者个人信息（姓名+学校/工作单位+学历/职位+研究方向） 

• 如果文章并非首发，请在投稿时提醒并附上所有已发布链接 

• PaperWeekly 默认每篇文章都是首发，均会添加“原创”标志




**📬 投稿邮箱：**

• 投稿邮箱：hr@paperweekly.site

• 所有文章配图，请单独在附件中发送 

• 请留下即时联系方式（微信或手机），以便我们在编辑发布时和作者沟通










🔍




现在，在**「知乎」**也能找到我们了

进入知乎首页搜索**「PaperWeekly」**

点击**「关注」**订阅我们的专栏吧







**关于PaperWeekly**





PaperWeekly 是一个推荐、解读、讨论、报道人工智能前沿论文成果的学术平台。如果你研究或从事 AI 领域，欢迎在公众号后台点击**「交流群」**，小助手将把你带入 PaperWeekly 的交流群里。




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgkXb8A1kiafKxib8NXiaPMU8mQvRWVBtFNic4G5b5GDD7YdwrsCAicOc8kp5tdEOU3x7ufnleSbKkiaj5Dg/640?)







