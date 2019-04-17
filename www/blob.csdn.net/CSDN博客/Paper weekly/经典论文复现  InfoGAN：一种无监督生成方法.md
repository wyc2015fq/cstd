# 经典论文复现 | InfoGAN：一种无监督生成方法 - Paper weekly - CSDN博客





2018年10月26日 14:31:52[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：340









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




过去几年发表于各大 AI 顶会论文提出的 400 多种算法中，公开算法代码的仅占 6%，其中三分之一的论文作者分享了测试数据，约 54% 的分享包含“伪代码”。这是今年 AAAI 会议上一个严峻的报告。 人工智能这个蓬勃发展的领域正面临着实验重现的危机，就像实验重现问题过去十年来一直困扰着心理学、医学以及其他领域一样。**最根本的问题是研究人员通常不共享他们的源代码。**




可验证的知识是科学的基础，它事关理解。随着人工智能领域的发展，打破不可复现性将是必要的。为此，**PaperWeekly 联手百度 PaddlePaddle 共同发起了**[**本次论文有奖复现**](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490664&idx=1&sn=c716049d2a63a4ffef0d25e35f3168fb&chksm=96e9c3e8a19e4afe04f844dc25fee0adbae5345eefe177c611e18b37761d8d9a8aed1769a943&scene=21#wechat_redirect)，我们希望和来自学界、工业界的研究者一起接力，为 AI 行业带来良性循环。




作者丨黄涛 

学校丨中山大学数学学院18级本科生

研究方向丨图像识别、VQA、生成模型和自编码器




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojQOkGZYDtJ5wKkJjSwseY4Rib0PSs7gyYt56r3WqSBajJaxA1kFypQrw/640)




**论文复现代码： **




http://aistudio.baidu.com/#/projectdetail/23600




# GAN




生成对抗网络（Generative Adversarial Nets）是一类新兴的生成模型，由两部分组成：一部分是判别模型（discriminator）D(·)，用来判别输入数据是真实数据还是生成出来的数据；另一部分是是生成模型（generator）G(·)，由输入的噪声生成目标数据。GAN 的优化问题可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojicBWGLTk63ljOFehUOjVjoqd6lWvZ0uEqvFTXcuGBOaceSnz1Q54pJw/640)




其中 Pdata 是生成样本，noise 是随机噪声。而对于带标签的数据，通常用潜码（latent code）c 来表示这一标签，作为生成模型的一个输入，这样我们有：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojTibicWBkfLcPpicUVvESZ8icgeCqmm6l2XTXee6JiaWfv6gFy3SzvwSFVDw/640)




然而当我们遇到存在潜在的类别差别而没有标签数据，要使 GAN 能够在这类数据上拥有更好表现，**我们就需要一类****能够无监督地辨别出这类潜在标签的数据**，InfoGAN 就给出了一个较好的解决方案。




# 互信息（Mutual Information）




互信息是两个随机变量依赖程度的量度，可以表示为：





![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojpiaO1rnd2yPQ48rep07RFP99eWKjpqLGFt8DaVnXVB8MicibrCWtCygXQ/640)




要去直接优化 I(c;G(z,c)) 是极其困难的，因为这意味着我们要能够计算后验概率（posterior probability）P(c|x)，但是我们可以用一个辅助分布（auxiliary distribution）Q(c|x)，来近似这一后验概率。这样我们能够给出互信息的一个下界（lower bounding）：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojHabwjtymRCk0ulibs0uTRgmv5rial65DCfK1htYTTWLCcRicVA02xblag/640)




# InfoGAN




在 InfoGAN 中，为了能够增加潜码和生成数据间的依赖程度，我们可以增大潜码和生成数据间的互信息，使生成数据变得与潜码更相关：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojju5dybWSMYsqlQGnkia466B1rorKxuhzP26FgeGJP75LcFXOptfXTSw/640)




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojqPqaXibrSrDkCjttfGbEAZya6ubEhTLLuKvoiarbjyNxhQagsX7d5vyw/640)

▲ 图1. InfoGAN的整体结构图




由上面的，对于一个极大化互信息的问题转化为一个极大化互信息下界的问题，我们接下来就可以定义：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojv9s3icLdnEOlQm3IoRWNvXXSjazGIlmyyopWh4OcsS0QezlFxZliajfA/640)




在论文的附录中，作者证明了：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojjypOIpPf3pKSphp4RmEjz2PRIMTmOAHtgzRoCfxfDIbfpctZUgJ8ew/640)




 于是：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojhRclKtfq6h0V0VlzFjWGTibqJblr8mmnG4wpF21dtLMl11Q1cWjdQnA/640)




故 LI (G, Q) 是互信息的一个下界。作者指出，用蒙特卡罗模拟（Monte Carlo simulation）去逼近 LI (G, Q) 是较为方便的，这样我们的优化问题就可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojpGy5iaKA7C7N694cmD4sLr83My6lSPH60KPxy7R0oDtB4GqCiaxdibVhw/640)




# 实现




在实现中，D(x)、G(z, c) 和 Q(x) 分别用一个 CNN (Convolutional Neural Networks)、CNN、DCNN (DeConv Neural Networks) 来实现。同时，潜码 c 也包含两部分：一部分是类别，服从 Cat(K = N,p = 1/N)，其中 N 为类别数量；另一部分是连续的与生成数据有关的参数，服从 Unif(−1,1) 的分布。 




在此应指出，Q(c|x) 可以表示为一个神经网络 Q(x) 的输出。对于输入随机变量 z 和类别潜码 c，实际的 LI(G, Q) 可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhoj3K6Ymh993qNkWIO6rN6ohC0gj9K1y6xoTu26VtemEicXjmHUsdjEvQg/640)




其中 · 表示内积（inner product），c 是一个选择计算哪个 log 的参数，例如 ci = 1 而 cj = 0(∀j = 1,2,···,i − 1,i + 1,···,n)，那么 z 这时候计算出的 LI(G,Q) 就等于 log(Q(z,c)i)。这里我们可以消去 H(c)，因为 c 的分布是固定的，即优化目标与 H(c) 无关：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojCxCh2qWwrgr4nsZlXkCvSQ3KgiaYSgjL04X33GNWkXVLeuY5YAriceAA/640)




而对于参数潜码，我们假设它符合正态分布，神经网络 Q(x) 则输出其预测出的该潜码的均值和标准差， 我们知道，对于均值 μ，标准差 σ 的随机变量，其概率密度函数为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojaicem9wjFLuAkicKBJiccL8xncLdSialsEAClPUncFQM3BodTiaSBHywB5g/640)




要计算参数潜码 c 的![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojvX4gx0vpyUMcDbQ2DDRRVtzSh7lpDp4ic2ewLGGG6lyQvweKSticHLmQ/640)，就是要计算 log p(c)，即：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojTgC6Utz8k6XIV2HjP6GKNGrjY1q6Ew35IYibUINJEkkDmKVoZBtj15w/640)




设 Q(x) 输出的参数潜码 c 的均值 μ，标准差 σ 分别为 Q(x)μ 和 Q(x)σ，那么对于参数潜码 c：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojIRQ1J1zHmdF35bXm2rAU2wNtsibyLyoCIq7zvWYndxJGkj5pricmF25Q/640)




同样的，我们可以消去 H(c)，因为 c 的分布是固定的，那么：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojG8PKfxDs8bqkH7GgO5RYUL6kEVecABJFwLpLDD0nw1Y57Id6s25P4g/640)




# 实验




首先，通过和普通的 GAN 比较 LI ，作者证明了 InfoGAN 确实能够优化这一互信息的下界 2。 




作者在 MNIST 手写数字数据集（3）、3D 面部数据集（4）、3D 椅子数据集（5）、SVHN 街景房号数据集（6）以及 CelebA 人脸数据集（7）上进行了模型的相关测试。 




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojl0l6qZzDMP4HodGsZTmrw1FRlcst5W126dicWhHYXJEoic0RQ4Knp6kw/640)

▲ 图2. MNIST手写字符数据集上的结果




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhoj0D5ia8LCGVNd0Lgo4jzLcSibebcia8kXOgLFgkKMdfM0e0hyozXzy1NKQ/640)

▲ 图3. 3D面部数据集上的结果




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojP4PLrwf5wy81aoXib0q93w9YKpkzlDpPWYgatojqMICs33D9FSoks2w/640)

▲ 图4. 3D椅子数据集上的结果




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojiapgSE1JjujWOWCnEYhIVTtpoNE66PRDOZAiabsicIVAeKmPrzIJ64HBg/640)

▲ 图5. SVHN街景房号数据集上的结果




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojUAncPvhBmMvBR6Zrz14OgBPjYFhF5YL9WGZzXesrXrqpoxzsgANChg/640)

▲ 图6. CelebA人脸数据集上的结果




作者展示了这些数据集上学习到的类别潜码（从上至下变化）和参数潜码（从左至右变化，由 -2 到 2），我们可以看出，**InfoGAN 不仅能够很好地学习数据之间的类型差别，也能够很好地学习到数据本身的一些易于区分的特点**，而且生成模型对这些特点的泛化能力还是很好的。




# 再论InfoGAN的*LI*




读完论文，我们发现，对于类别潜码，这个 LI 本质上是 x 与 G(z, c) 之间的 KL 散度：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojUC4NxJVlcrNCEicYFYcIyFp1CElcXib37hfblQiamnpZQAxM2HiatMdqdw/640)




也就是说：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojj8b8wy6vTdoLrhiaoHlgFMXwUhQswDPwQFthvPWBRbtOrMgVOJph9tw/640)




而 min DKL(c||Q(G(z, c))) 意味着减小 c 与 Q(G(z, c)) 的差别。




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojRrx2LQerqicumEFlhxsGsflQwDNf2zF120KnuibPRcgKJNphicCx7JKlQ/640)

▲ 图7. 普通GAN和InfoGAN的LI在训练过程中的比较




如果我们不考虑 Q(x)σ 的影响，LI 的优化过程：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojoBDictFjrDDVgcdrmZSu2rhLMmFsLvcMT5W4kGdCfMMOuOuFz4f7R2g/640)




而![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojfILRFVxCrPyibaFs2qXfbyp1ictZhKWxoGXTO7EzdNgQD3hibpmia8KNibw/640)也意味着减小 c 与 Q(G(z, c))μ 的差。




再纵观整个模型，我们会发现这一对 LI 优化的过程，实质上是以 G 为编码器（Encoder）， Q 为解码器（Decoder），生成的图像作为我们要编码的码（code），训练一个自编码器（Autoencoder），也就是说，作者口中的信息论优化问题，本质上是无监督训练问题。




# 关于PaddlePaddle




在 PaddlePaddle 中，一个极为重要的概念即是 fluid.Program()，在官方文档里常见的 exe.run(program= fluid.default_startup_program())的 fluid.default_startup_program() 就是其中一个例子。




在这一使用中可以了解到，我们要用 exe.run() 中的 program 参数运行指定的 fluid.Program()，而官方文档指出，当该参数未指定时，会运行 fluid.default_main_program()，而 fluid.default_main_program() 代表的是未指定 fluid.Program() 的**所有操作**。




注意，这里说的是“所有”，由于 PaddlePaddle 没有计算依赖检测机制，即使在计算 fetch_list 中的值的时候不会用到操作也会被计算，这一点与 TensorFlow 极其不同，作者本人在使用过程中踩了很大的坑，还望各位注意。在执行多种任务的时候不要一股脑全部写在 fluid.default_main_program() 之中， 这样极其浪费资源，也容易造成一些问题。




一个新的 fluid.Program() 被创建之后，可以在 fluid.program_guard() 中指定该 fluid.Program() 中的操作与变量：




#创建Infer_program
Infer_program = fluid.Program()
#在这里面定义Infer_program中的操作与变量
with fluid.program_guard(main_program = Infer_program):
#从外部通过feed传入的变量，一般是输入、标签等
    X = fluid.layers.data(name='X', shape=[X_dim], dtype='float32')
#全链接层
    output = fluid.layers.fc(input = X, size = 128) 





PaddlePaddle 中还需要注意的一点是，fluid.Variable 的命名空间是全局的，也就是说在同一或者不同 fluid. Program() 间，同名（fluid.Variable 的 name 属性相同）的 fluid.Variable 所指向的变量是相同的，所以同一名称在同一或者不同 fluid.Program () 中可以被使用多次，而不用担心 TensorFlow 中会出现的 reuse 问题。 




要对一个操作的中的权值的名称进行定义（权值命名为 W1，偏置命名为 b1）：




output = fluid.layers.fc(input = X, 
                         size = 10, 
                         param_attr = fluid.ParamAttr(name="W1"), 
                         bias_attr = fluid.ParamAttr(name="b1"))





要在之后使用这些 fluid.Variable，例如在 Optimizer 中使用：




#可以直接用名称指代对应的fluid.Variable
parameter_list = ["W1", "b1"]
#构建optimizer
optimizer = fluid.optimizer.AdamOptimizer()
#指定optimizer优化的目标和对象
optimizer.minimize(loss, parameter_list=parameter_list)




在构建完基本的运算操作后，便可以开始初始化操作了：




#初始化fluid.Executor（指定执行程序位置）
exe = fluid.Executor(fluid.CPUPlace())
#执行fluid.default_startup_program()，在fluid.program_guard()中
#若没有指定初始化program,则默认为此program
exe.run(program=fluid.default_startup_program())





初始化完成后，可以开始训练啦：




#在从外部传入数据的时候要注意，传入数据的数据类型必须与fluid.layers.data
#中定义的类型一致，否则会报错
#如果传入数据是list类型，建议转换为np.array，否则可能回报错：
#fedding的数据中包含lod信息，请您转换成lodtensor
#（渣翻译， 原因是list被默认为含有变长数据）
feeding = {"X" : np.array(Z_noise).astype('float32')}
#传入feeding中的数据，执行program程序，从计算结果中获取loss
#（默认会被转换成np.array，可在函数参数中设置）
loss_curr = exe.run(feed = feeding, program = program, fetch_list = [loss])




# GAN实现




生成对抗网络（Generative Adversarial Nets）是一类新兴的生成模型，由两部分组成：一部分是判别模型（discriminator）D(·)，用来判别输入数据是真实数据还是生成出来的数据；另一部分是是生成模型（generator）G(·)，由输入的噪声生成目标数据。GAN 的优化问题可以表示为：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojQ6UycngBAcgj33Xn9fjqU4CVsCOe0sIa6OPBAYAibYEazTvQ3nGD5Cg/640)




其中 Pdata 是生成样本，noise 是随机噪声。我们用一个双层的 MLP 来演示：




#判别模型
defdiscriminator(x):
#使用fluid.unique_name.guard()添加模型内参数名称的前缀
with fluid.unique_name.guard('D_'):
        D_h1 = fluid.layers.fc(input = x, size = 256, act = "relu")
        D_logit = fluid.layers.fc(input = D_h1, size = 1, act = "sigmoid")
return D_logit

#生成模型
defgenerator(inputs):
with fluid.unique_name.guard('G_'):
        D_h1 = fluid.layers.fc(input = inputs, size = 256, act = "relu")
        D_logit = fluid.layers.fc(input = D_h1, size = 784, act = "sigmoid")
return D_logit




通常，一个 GAN 的训练由两部分组成，第一部分是对 D(·) 进行训练，极大化目标函数：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojgQeYvkUiaRia1XibwXA4HW7icibaTTicUGlsY33ewgFe0hk7XBEaoUJH6mMg/640)




第二部分是对 G(·) 进行训练，极小化目标函数：




![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglwzhtGaDRefl2Iibw9prhojibFZNKatsFRI7oxyNONbletj1DgFSQqCLLtLbicrIFPDmHnrrUPsssGQ/640)




以下是两部分优化的定义：




#参考Todd的LSGAN的实现，使用函数获取模型所有变量
defget_params(program, prefix):
    all_params = program.global_block().all_parameters()
return [t.name for t in all_params if t.name.startswith(prefix)]

#G优化程序
G_program = fluid.Program()
with fluid.program_guard(main_program = G_program):
#定义输入数据
    Z = fluid.layers.data(name='Z', shape=[Z_dim], dtype='float32')

#执行相关模型的计算
    G_sample = generator(Z)
    D_fake = discriminator(G_sample) 

#计算损失函数
    G_loss = 0.0 - fluid.layers.reduce_mean(fluid.layers.log(D_fake + 1e-8))

#定义optimizer优化的变量的范围
    theta_G = get_params(G_program, "G")
    G_optimizer = fluid.optimizer.AdamOptimizer()
    G_optimizer.minimize(G_loss, parameter_list=theta_G)

#D优化程序
D_program = fluid.Program()
with fluid.program_guard(main_program = D_program):
    Z = fluid.layers.data(name='Z', shape=[Z_dim], dtype='float32')
    X = fluid.layers.data(name='X', shape=[784], dtype='float32')
#在使用数据集时，要注意相应接口传入数据的值的范围
#paddle.dataset.mnist中的数据，范围在[-1, 1]
#要将其转换到sigmoid函数的值域内
    X = X * 0.5 + 0.5

    G_sample = generator(Z)
    D_real = discriminator(X)
    D_fake = discriminator(G_sample)  

    D_loss = 0.0 - fluid.layers.reduce_mean(fluid.layers.log(D_real + 1e-8) 
    + fluid.layers.log(1.0 - D_fake + 1e-8))

    theta_D = get_params(G_program, "D")
    D_optimizer = fluid.optimizer.AdamOptimizer()
    D_optimizer.minimize(D_loss, parameter_list=theta_D)




在定义好这些之后，是时候开训练了：




#定义传入的数据
feeding_withx= {"X" : np.array(X_mb).astype('float32'), 
"Z" : np.array(Z_noise).astype('float32')}
feeding = {"Z" : np.array(Z_noise).astype('float32')}
#执行训练操作并获取当前损失函数的值
D_loss_curr = exe.run(feed = feeding_withx, program = D_program, 
                      fetch_list = [D_loss])
G_loss_curr = exe.run(feed = feeding, program = G_program, 
                      fetch_list = [G_loss])




若欲测试模型效果，可再定义一个 Inference：




#Inference
Infer_program = fluid.Program()
with fluid.program_guard(main_program = Infer_program):   
    Z = fluid.layers.data(name='Z', shape=[Z_dim], dtype='float32')
    G_sample = generator(Z)




然后再这样获取 samples：




feeding = {"Z" : np.array(Z_noise).astype('float32')}
samples = exe.run(feed = feeding, program = Infer_program, 
                  fetch_list = [G_sample])



# 后记




本文先前于今年 8 月完成，共享于 PaddlePaddle 论文复现群内，在 10 月 LSGAN 的复现公开之 后，参考该复现更改了模型参数命名和参数列表的实现方法，在此感谢 Todd 同学的复现对本文的帮助。





![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击标题查看更多论文解读：**




- 
[在全景视频中预测头部运动：一种深度强化学习方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492263&idx=1&sn=5c904bcb3205815849d9382fbe0cd41e&chksm=96ea3d27a19db43116635fef31260582cb8c7ea5cec3cc2e5191718603cc5ea89d364c77f949&scene=21#wechat_redirect)

- 
[网络表示学习综述：一文理解Network Embedding](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490862&idx=1&sn=707fe122dfc5d961a22771111514fada&chksm=96e9c2aea19e4bb8755d6759dd8e70cb44d2da2c454947395dede0268f511c140441cd1fb5ce&scene=21#wechat_redirect)

- 
[神经网络架构搜索（NAS）综述](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491248&idx=1&sn=d2946d8a37f7c6567b1a767a497006fb&chksm=96e9c130a19e48267f72ad32c527ec4a1697741e409d865d9233c5d7035a1f66a59b5e40792d&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247487949&idx=1&sn=e09391933f3c4493cfb737b0ea2cf0af&chksm=96e9ce4da19e475b0c789088d403a0f49449b8ba0c43734aa835c5d2a7cb69c3d839c7ce056c&scene=21#wechat_redirect)[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)


- 
[深度](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)[解读DeepMind新作：史上最强GAN图像生成器](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492065&idx=1&sn=a91d7ae724eed652ca87f647910bf666&chksm=96ea3e61a19db777c615d64abf564110ee4d1c32ecca2a629a7c0158a98bf6ccb2ec0fc05814&scene=21#wechat_redirect)

- 
[](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490514&idx=1&sn=c066be4f8d2ac3afa8378d180864eed0&chksm=96e9c452a19e4d44eb6a879c5eb4a1426d6de370a0f3c5b6a27c6b8dfc6a938a3851baa258e5&scene=21#wechat_redirect)[ACL2018高分论文：混合高斯隐向量文法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247490152&idx=1&sn=ee9c70c701d5ba74423318865ecdb44f&chksm=96e9c5e8a19e4cfeddb4d92d86415c54f511427f8851c5f22b596c68128b85512bf7a62cf729&scene=21#wechat_redirect)

- 
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[EMNLP 2018论文解读 | 对话生成 & 文本风格转化](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492361&idx=1&sn=af65ef32752cefec12d99727d7fd0867&chksm=96ea3c89a19db59ffead464756c146e53b6ae80519c46c0ed9b3e110e3b0375d7018d6291243&scene=21#wechat_redirect)











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

▽ 点击 | 阅读原文| 收藏复现代码




