# Self-Attention GAN 中的 self-attention 机制 - Paper weekly - CSDN博客





2019年03月06日 08:39:00[Paper_weekly](https://me.csdn.net/c9Yv2cf9I06K2A9E)阅读数：91









![640](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_gif/VBcD02jFhgm9RFr5icmiaj0bibJxUeIGdAFHNM4G6PJEiccw293RuVnOiadQ4zcdibdJa5FFfn0ZMgpbKib4AAKD8dm2w/640)




作者丨尹相楠

学校丨里昂中央理工博士在读

研究方向丨人脸识别、对抗生成网络




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZc8XzGEyZSB8UtdOAsYnASMIVwbBywxiboWqzpAziciaKrc5XJPgK53JeEQ/640?wx_fmt=png)




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZcdUKVNLk8aL7AKw9PCSSgYq7icSQmqYuY4ZP81jRSrIPh3iaThDpmPg5g/640?wx_fmt=png)




Self Attention GAN 用到了很多新的技术。最大的亮点当然是 **self-attention 机制**，该机制是 ***Non-local Neural Networks***[1] 这篇文章提出的。其作用是能够更好地学习到全局特征之间的依赖关系。因为传统的 GAN 模型很容易学习到纹理特征：如皮毛，天空，草地等，不容易学习到特定的结构和几何特征，例如狗有四条腿，既不能多也不能少。 




除此之外，文章还用到了 ***Spectral Normalization for GANs***[2] 提出的**谱归一化**。谱归一化的解释见本人这篇文章：[详解GAN的谱归一化（Spectral Normalization）](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494882&idx=1&sn=beafd9829c874f131b5e9c62512e481c&chksm=96ea3362a19dba7429b0524a49a55ea0f1a84314e219ec9d62827d1aedfce3be319776acb335&scene=21#wechat_redirect)。




但是，**该文代码中的谱归一化和原始的谱归一化运用方式略有差别： **




1. 原始的谱归一化基于 W-GAN 的理论，只用在 Discriminator 中，用以约束 Discriminator 函数为 1-Lipschitz 连续。而在 Self-Attention GAN 中，Spectral Normalization 同时出现在了 Discriminator 和 Generator 中，用于使梯度更稳定。除了生成器和判别器的最后一层外，每个卷积/反卷积单元都会上一个 SpectralNorm。 




2. 当把谱归一化用在 Generator 上时，同时还保留了 BatchNorm。Discriminator 上则没有 BatchNorm，只有 SpectralNorm。 




3. 谱归一化用在 Discriminator 上时最后一层不加 Spectral Norm。 




最后，self-attention GAN 还用到了 cGANs With Projection Discriminator 提出的 **conditional normalization** 和 **projection in the discriminator**。这两个技术我还没有来得及看，而且 PyTorch 版本的 self-attention GAN 代码中也没有实现，就先不管它们了。




本文主要说的是 self-attention 这部分内容。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZcfuY9MUh008icMccWylfiaXsXhIOIcZhsDku5KqYJFXgLiahglmlPD0HvQ/640?wx_fmt=png)

**▲**图1. Self-Attention




# Self-Attention




在卷积神经网络中，每个卷积核的尺寸都是很有限的（基本上不会大于 5），因此每次卷积操作只能覆盖像素点周围很小一块邻域。




对于距离较远的特征，例如狗有四条腿这类特征，就不容易捕获到了（也不是完全捕获不到，因为多层的卷积、池化操作会把 feature map 的高和宽变得越来越小，越靠后的层，其卷积核覆盖的区域映射回原图对应的面积越大。但总而言之，毕竟还得需要经过多层映射，不够直接）。




Self-Attention 通过直接计算图像中任意两个像素点之间的关系，一步到位地获取图像的全局几何特征。 




论文中的公式不够直观，我们直接看文章的 PyTorch 的代码，核心部分为 sagan_models.py：




```python
class Self_Attn(nn.Module):
    """ Self attention Layer"""
    def __init__(self,in_dim,activation):
        super(Self_Attn,self).__init__()
        self.chanel_in = in_dim
        self.activation = activation

        self.query_conv = nn.Conv2d(in_channels = in_dim , out_channels = in_dim//8 , kernel_size= 1)
        self.key_conv = nn.Conv2d(in_channels = in_dim , out_channels = in_dim//8 , kernel_size= 1)
        self.value_conv = nn.Conv2d(in_channels = in_dim , out_channels = in_dim , kernel_size= 1)
        self.gamma = nn.Parameter(torch.zeros(1))

        self.softmax  = nn.Softmax(dim=-1) #
    def forward(self,x):
        """
            inputs :
                x : input feature maps( B X C X W X H)
            returns :
                out : self attention value + input feature 
                attention: B X N X N (N is Width*Height)
        """
        m_batchsize,C,width ,height = x.size()
        proj_query  = self.query_conv(x).view(m_batchsize,-1,width*height).permute(0,2,1) # B X CX(N)
        proj_key =  self.key_conv(x).view(m_batchsize,-1,width*height) # B X C x (*W*H)
        energy =  torch.bmm(proj_query,proj_key) # transpose check
        attention = self.softmax(energy) # BX (N) X (N) 
        proj_value = self.value_conv(x).view(m_batchsize,-1,width*height) # B X C X N

        out = torch.bmm(proj_value,attention.permute(0,2,1) )
        out = out.view(m_batchsize,C,width,height)

        out = self.gamma*out + x
        return out,attention
```




构造函数中定义了三个 1 × 1 的卷积核，分别被命名为 query_conv ， key_conv 和 value_conv 。




为啥命名为这三个名字呢？这和作者给它们赋予的含义有关。query 意为查询，我们希望输入一个像素点，查询（计算）到 feature map 上所有像素点对这一点的影响。而 key 代表字典中的键，相当于所查询的数据库。query 和 key 都是输入的 feature map，可以看成把 feature map 复制了两份，一份作为 query 一份作为 key。 




需要用一个什么样的函数，才能针对 query 的 feature map 中的某一个位置，计算出 key 的 feature map 中所有位置对它的影响呢？作者认为这个函数应该是可以通过“学习”得到的。那么，自然而然就想到要对这两个 feature map 分别做卷积核为 1 × 1 的卷积了，因为卷积核的权重是可以学习得到的。 




至于 value_conv ，可以看成对原 feature map 多加了一层卷积映射，这样可以学习到的参数就更多了，否则 query_conv 和 key_conv 的参数太少，按代码中只有 in_dims × in_dims//8 个。 




接下来逐行研究 forward 函数：




```python
proj_query  = self.query_conv(x).view(m_batchsize,-1,width*height).permute(0,2,1)
```




这行代码先对输入的 feature map 卷积了一次，相当于对 query feature map 做了一次投影，所以叫做 proj_query。由于是 1 × 1 的卷积，所以不改变 feature map 的长和宽。feature map 的每个通道为如 (1) 所示的矩阵，矩阵共有 N 个元素（像素）。




![640?wx_fmt=png](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZcgzwGYmuBKvxGSZ8X74fHHw70zQz4lNBgZZoQlJRicMO0UMJ4c4w8iaPA/640?wx_fmt=png)




 然后重新改变了输出的维度，变成：




 (m_batchsize,-1,width*height) 




batch size 保持不变，width 和 height 融合到一起，把如 (1) 所示二维的 feature map 每个 channel 拉成一个长度为 N 的向量。




因此，如果 m_batchsize 取 1，即单独观察一个样本，该操作的结果是得到一个矩阵，矩阵的的行数为 query_conv 卷积输出的 channel 的数目 C（ in_dim//8 ），列数为 feature map 像素数 N。




然后作者又通过 .permute(0, 2, 1) 转置了矩阵，矩阵的行数变成了 feature map 的像素数 N，列数变成了通道数 C。因此矩阵维度为 N × C 。该矩阵每行代表一个像素位置上所有通道的值，每列代表某个通道中所有的像素值。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZcKYNoubVG8A6GyMibEVl6EvGgxGYLP9o1D7Faqn9dvbfJMljfgLVxicicQ/640?wx_fmt=jpeg)

**▲**图2. proj_query 的维度




```python
proj_key =  self.key_conv(x).view(m_batchsize,-1,width*height)
```




这行代码和上一行类似，只不过取消了转置操作。得到的矩阵行数为通道数 C，列数为像素数 N，即矩阵维度为 C × N。该矩阵每行代表一个通道中所有的像素值，每列代表一个像素位置上所有通道的值。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZcYhu48zATxAj2zjXStVUn0CbySwSUUrHHyFcAu5WLb4QSqiaU8A6SD8Q/640?wx_fmt=jpeg)

**▲**图3. proj_key的维度




```python
energy =  torch.bmm(proj_query,proj_key)
```




这行代码中， torch.bmm 的意思是 batch matrix multiplication。就是说把相同 batch size 的两组 matrix 一一对应地做矩阵乘法，最后得到同样 batchsize 的新矩阵。




若 batch size=1，就是普通的矩阵乘法。已知 proj_query 维度是 N × C， proj_key 的维度是 C × N，因此 energy 的维度是 N × N：




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZc98OLX5rw3LqCFjrwLjze68S5jkrGKeJ19nNYUiaRH1taHhxo6KfrvQA/640?wx_fmt=jpeg)

**▲**图4. energy的维度




energy 是 attention 的核心，其中第 i 行 j 列的元素，是由 proj_query 第 i 行，和 proj_key 第 j 列通过向量点乘得到的。而 proj_query 第 i 行表示的是 feature map 上第 i 个像素位置上所有通道的值，也就是第 i 个像素位置的所有信息，而 proj_key 第 j 列表示的是 feature map 上第 j 个像素位置上的所有通道值，也就是第 j 个像素位置的所有信息。




这俩相乘，可以看成是第 j 个像素对第 i 个像素的影响。即，energy 中第 i 行 j 列的元素值，表示第 j 个像素点对第 i 个像素点的影响。




```python
attention = self.softmax(energy)
```




这里 sofmax 是构造函数中定义的，为按“行”归一化。这个操作之后的矩阵，各行元素之和为 1。这也比较好理解，因为 energy 中第 i 行元素，代表 feature map 中所有位置的像素对第 i 个像素的影响，而这个影响被解释为权重，故加起来应该是 1，故应对其按行归一化。attention 的维度也是 N × N。




```python
proj_value = self.value_conv(x).view(m_batchsize,-1,width*height)
```




上面的代码中，先对原 feature map 作一次卷积映射，然后把得到的新 feature map 改变形状，维度变为 C × N ，其中 C 为通道数（注意和上面计算 proj_query  proj_key 的 C 不同，上面的 C 为 feature map 通道数的 1/8，这里的 C 与 feature map 通道数相同），N 为 feature map 的像素数。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZccmD3VFc1Hgibev8Qyh5nicPpWUOI1FE9iaVN3VHg4yEsjqZMNuujDHlaQ/640?wx_fmt=jpeg)

**▲**图5. proj_value的维度




```python
out = torch.bmm(proj_value,attention.permute(0,2,1) )
out = out.view(m_batchsize,C,width,height)
```




然后，再把 proj_value （C × N）矩阵同  attention 矩阵的转置（N × N）相乘，得到 out （C × N）。之所以转置，是因为 attention 中每行的和为 1，其意义是权重，需要转置后变为每列的和为 1，施加于 proj_value 的行上，作为该行的加权平均。 proj_value 第 i 行代表第 i 个通道所有的像素值， attention 第 j 列，代表所有像素施加到第 j 个像素的影响。




因此， out 中第 i 行包含了输出的第 i 个通道中的所有像素，第 j 列表示所有像素中的第 j 个像素，合起来也就是： out 中的第 i 行第 j 列的元素，表示被 attention 加权之后的 feature map 的第 i 个通道的第 j 个像素的像素值。再改变一下形状， out 就恢复了 channel×width×height 的结构。




![640?wx_fmt=jpeg](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_jpg/VBcD02jFhglDJeeSicEOX4v6hKrsKqBZcEibyliaMkQdLtJnM6GTTryrQDlwoxP9m1jzwJqGBLIPuJAIt4LhzvKKA/640?wx_fmt=jpeg)

**▲**图6. out的维度




```python
out = self.gamma*out + x
```




最后一行代码，借鉴了残差神经网络（residual neural networks）的操作， gamma 是一个参数，表示整体施加了 attention 之后的 feature map 的权重，需要通过反向传播更新。而 x 就是输入的 feature map。




在初始阶段， gamma 为 0，该 attention 模块直接返回输入的 feature map，之后随着学习，该 attention 模块逐渐学习到了将 attention 加权过的 feature map 加在原始的 feature map 上，从而强调了需要施加注意力的部分 feature map。




# 总结




可以把 self attention 看成是 feature map 和它自身的转置相乘，让任意两个位置的像素直接发生关系，这样就可以学习到任意两个像素之间的依赖关系，从而得到全局特征了。看论文时会被它复杂的符号迷惑，但是一看代码就发现其实是很 naive 的操作。




# 参考文献




[1] Xiaolong Wang, Ross Girshick, Abhinav Gupta, Kaiming He, Non-local Neural Networks, CVPR 2018.

[2] Takeru Miyato, Toshiki Kataoka, Masanori Koyama, Yuichi Yoshida, Spectral Normalization for Generative Adversarial Networks, ICLR 2018.




![640?](https://ss.csdn.net/p?https://mmbiz.qpic.cn/mmbiz_png/VBcD02jFhgmPEF4lW0pL5weJia5y4xhJbog2pIZZ3ZCgVUDynvus6rCzNKGAAAI6R8jaXTpYPISCMicpFegVdG0g/640?)







**点击以下标题查看更多往期内容：**




- 
[Airbnb实时搜索排序中的Embedding技巧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494488&idx=1&sn=452ac80c593a9b31252031eac38d0e01&chksm=96ea34d8a19dbdce940ed25bb93507aa6c4d118f84dd0bb965b060f232fe5d41894bbc9edcb6&scene=21#wechat_redirect)

- 
[图神经网络综述：模型与应用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493906&idx=1&sn=15c9f18a1ce6baa15dc85ecb52e799f6&chksm=96ea3692a19dbf847c1711e6e194ad60d80d11138daf0938f90489a054d77cfd523bee2dc1d2&scene=21#wechat_redirect)

- 
[近期值得读的10篇GAN进展论文](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247493987&idx=1&sn=ce1bcdce28e78f4a307743e389f42b10&chksm=96ea36e3a19dbff5cff7f4f1c9d9fc482bb2144d80566319b3d26bce4d9ab80689d38ab2e427&scene=21#wechat_redirect)


- 
[自然语言处理中的语言模型预训练方法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247492317&idx=1&sn=e823a75d9463257ed9ea7b3e4677c1ae&chksm=96ea3d5da19db44be0872ff4e29043aa72c7a624a116196bfeeca092a15f9209d7cf8ce46eb5&scene=21#wechat_redirect)

- 
[从傅里叶分析角度解读深度学习的泛化能力](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247491082&idx=1&sn=d7c1cb39c3be43154c658ca5a791eb4c&chksm=96e9c18aa19e489c32fe36671e4208ce42bf200e3a7adeda200fa2785462d16f85c58bb455b4&scene=21#wechat_redirect)

- 
[深度思考 | 从BERT看大规模数据的无监督利用](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494978&idx=1&sn=529b8f661b13c7b6b729e9a926b0737a&chksm=96ea32c2a19dbbd43456086a2186480fa548fa982d536564e167739ef46282eaf6fca7316ced&scene=21#wechat_redirect)


- 
[AI Challenger 2018 机器翻译参赛总结](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247494103&idx=1&sn=fc372862e0369b1f6a943bf997f6fc1b&chksm=96ea3657a19dbf4108bbc4179e779aa04ef05fe84f0013fa6425b0cd7e761e9880917361c4c1&scene=21#wechat_redirect)

- 
[小米拍照黑科技：基于NAS的图像超分辨率算法](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495166&idx=1&sn=a158e603651bc4f26836151a9113e856&chksm=96ea327ea19dbb68b8987aca041bb21579a35b1c679e91fd2368c7f2fb7acd58508cd531bdfe&scene=21#wechat_redirect)

- 
[异构信息网络表示学习论文解读](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495219&idx=1&sn=b3a29b833fe8438e12b600650ec0245a&chksm=96ea31b3a19db8a5a335cc445f04eb13d5b1ee5451d688544240cce27dbc953993da4e842ab6&scene=21#wechat_redirect)

- 
[不懂Photoshop如何P图？交给深度学习吧](http://mp.weixin.qq.com/s?__biz=MzIwMTc4ODE0Mw==&mid=2247495276&idx=1&sn=99a84e81ba9f9a90b323c21c8c905765&chksm=96ea31eca19db8faca5cdd6447136e34a61a8a5a2d8e01489657f791841d30e0c78901a87400&scene=21#wechat_redirect)











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

▽ 点击 | 阅读原文| 获取最新论文推荐




