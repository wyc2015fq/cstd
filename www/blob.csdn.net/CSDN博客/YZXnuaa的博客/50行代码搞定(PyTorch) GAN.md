# 50行代码搞定(PyTorch) GAN - YZXnuaa的博客 - CSDN博客
2018年03月27日 14:27:02[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：323
2014年，Ian Goodfellow和他在蒙特利尔大学的同事们发表了一篇令人惊叹的论文，正式把生成对抗网络(GANs)介绍给全世界。通过把计算图和博弈论创新性的结合起来，GANs有能力让两个互相对抗的模型通过反向传播共同训练。
模型中有两个相互对抗的角色，我们分别称为G和D，简单解释如下：G是一个生成器，它试图通过学习真实数据集R，来创建逼真的假数据；D是鉴别器，从R和G处获得数据并标记差异。
Goodfellow有个很好的比喻：G是一个造假团队，试图造出跟真画一样的赝品；D是鉴定专家，试图找出真画和赝品的差异。当然在GANs的设定里，G是一群永远见不到真画的造假团队，他们能够获得的反馈只有D的鉴定意见。
![](https://t10.baidu.com/it/u=3540049203,2589763188&fm=170&s=1F8DCC03153079809CC488D10300D0B3&w=639&h=307&img.PNG)
在理想情况下，D和G都会随着时间的推移变得更好，直到G变成一个造假大师，最终让D无法区分出真画和赝品。实际上，Goodfellow已经表明G能够对原始数据集进行无监督学习，并且找到这些数据的低维表达方式。
这么厉害的技术，代码怎么也得一大堆吧？
并不是。使用刚刚发布的PyTorch，实际上可以只用不到50行代码，就能创建一个GAN。我们需要考虑的组件只有下面五个：
 R：原始的真实数据集
 I：作为熵源输入生成器的随机噪声
 G：尝试复制/模仿原始数据集的生成器
 D：尝试分辨G输出的鉴别器
 训练循环：我们教G造假，再教D来鉴定
1）R： 我们将从最简单的R，一个钟形曲线开始。这个函数以平均值和标准偏差为参数，然后返回一个函数。在我们的示例代码中，使用了平均值4.0和标准差1.25。
![](https://t11.baidu.com/it/u=515861848,1023607145&fm=170&w=640&h=64&img.JPG)
2）I： 输入生成器的噪声也是随机的，但是为了增加点难度，我们使用了一个均匀分布，而不是正态分布。这意味着模型G不能简单地通过移动/缩放复制R，而必须以非线性的方式重塑数据。
![](https://t12.baidu.com/it/u=2763010091,1902354906&fm=170&s=C9C292444E2E3F304A6DEC89020070C3&w=427&h=72&img.JPG)
3）G： 生成器是一个标准的前馈图，包含两个隐藏层，三个线性映射。在这里，我们使用了ELU（指数线性单位）。G将从I获得均匀分布的数据样本，并以某种方式模仿来自R的正态分布样本。
![](https://t11.baidu.com/it/u=2226021241,1360037389&fm=170&s=69E2834683F0BD725C49DC03020070C3&w=639&h=230&img.JPG)
4）D： 鉴别器与生成器G的代码非常相似，都是有两个隐藏层和三个线性映射的前馈图。它将从R或G获取样本，并输出介于0和1之间的单个标量，0和1分别表示“假”和“真”。
![](https://t11.baidu.com/it/u=253401499,3664184788&fm=170&s=E9E08346D3E0B9724E419C030200E0C1&w=640&h=223&img.JPG)
5）训练循环 最后，训练循环在两种模式之间交替：首先，用带有准确标签的真实数据和假数据来训练D；然后，训练G来愚弄D。
![](https://t12.baidu.com/it/u=1500762716,238158756&fm=170&s=A0D231C14AA4916E06CDF5010300A0C0&w=640&h=312&img.PNG)
即使你从没用过PyTorch，也大致能看出发生了什么。在上图标为绿色的第一部分，我们将不同类型的数据输入D，并对D的猜测结果和实际的标签进行评判。这一步是“正向”的，然后我们用“反向”来计算梯度，并用它来更新d_optimizer step()调用的D参数。
上面，我们用到了G，但没有训练它。
在标为红色的下半部分中，我们对G做了同样的事情，注意：我们还会通过D来运行G的输出，相当于给了造假者一个侦探练习。但是在这一步中，我们不会对D进行优化或更改，因为我们不希望D学到错误的标签。因此，我们只调用g_optimizer.step()。
就这些啦，还有一些其他的样本代码，但是针对GAN的只有这五个组件。
对D和G进行几千轮训练之后，我们能得到什么？鉴别器D优化得很快，而G一开始优化得比较慢，不过，一旦到达了特定水平，G就开始迅速成长。
两万轮训练过后，G的输出的平均值超过4.0，但随后回到一个相当稳定，正确的范围（如左图）。同样，标准偏差最初在错误的方向下降，但随后上升到所要求的1.25范围（右图），与R相当。
![](https://t10.baidu.com/it/u=1311660789,2756729209&fm=170&s=1820543203746C2054D8F4C80300F0B3&w=640&h=216&img.PNG)
所以，基本的统计最终与R相当，那么高阶矩如何呢？分布的形状是否正确？毕竟，你当然可以有一个平均值为4.0、标准差为1.25的均匀分布，但这不会真正与R相匹配。让我们看看G形成的最终分布。
![](https://t11.baidu.com/it/u=883758673,395658103&fm=170&s=780C34721967693A5F44C7C20000F0B7&w=640&h=303&img.JPG)
还不错。左尾比右边稍微长了一点，但是我们可以说，它的偏斜和峰态符合原始的高斯函数。
G几乎完美还原了R的原始分布，而D独自在角落徘徊，无法分清真伪。这正是我们想要的结果。用不到50行的代码，就能实现。
代码：
#!/usr/bin/env python
# Generative Adversarial Networks (GAN) example in PyTorch.
# See related blog post at https://medium.com/@devnag/generative-adversarial-networks-gans-in-50-lines-of-code-pytorch-e81b79659e3f#.sch4xgsa9
import numpy as np
import torch
import torch.nn as nn
import torch.nn.functional as F
import torch.optim as optim
from torch.autograd import Variable
# Data params
data_mean = 4
data_stddev = 1.25
# Model params
g_input_size = 1     # Random noise dimension coming into generator, per output vector
g_hidden_size = 50   # Generator complexity
g_output_size = 1    # size of generated output vector
d_input_size = 100   # Minibatch size - cardinality of distributions
d_hidden_size = 50   # Discriminator complexity
d_output_size = 1    # Single dimension for 'real' vs. 'fake'
minibatch_size = d_input_size
d_learning_rate = 2e-4  # 2e-4
g_learning_rate = 2e-4
optim_betas = (0.9, 0.999)
num_epochs = 30000
print_interval = 200
d_steps = 1  # 'k' steps in the original GAN paper. Can put the discriminator on higher training freq than generator
g_steps = 1
# ### Uncomment only one of these
#(name, preprocess, d_input_func) = ("Raw data", lambda data: data, lambda x: x)
(name, preprocess, d_input_func) = ("Data and variances", lambda data: decorate_with_diffs(data, 2.0), lambda x: x * 2)
print("Using data [%s]" % (name))
# ##### DATA: Target data and generator input data
# '''原始的真实数据集,钟形曲线。这个函数以平均值和标准偏差为参数，返回一个函数'''
def get_distribution_sampler(mu, sigma):
    return lambda n: torch.Tensor(np.random.normal(mu, sigma, (1, n)))  # Gaussian
def get_generator_input_sampler():
    '''I： 输入生成器的噪声也是随机的，但是为了增加点难度，我们使用了一个均匀分布，而不是正态分布。
       这意味着模型G不能简单地通过移动/缩放复制R，而必须以非线性的方式重塑数据。'''
return lambda m, n: torch.rand(m, n)  # Uniform-dist data into generator, _NOT_ Gaussian
# ##### MODELS: Generator model and discriminator model
'''G： 生成器是一个标准的前馈图，包含两个隐藏层，三个线性映射。在这里，我们使用了ELU（指数线性单位）。   G将从I获得均匀分布的数据样本，并以某种方式模仿来自R的正态分布样本。'''
class Generator(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(Generator, self).__init__()
        self.map1 = nn.Linear(input_size, hidden_size)
        self.map2 = nn.Linear(hidden_size, hidden_size)
        self.map3 = nn.Linear(hidden_size, output_size)
    def forward(self, x):
        x = F.elu(self.map1(x))
        x = F.sigmoid(self.map2(x))
        return self.map3(x)
'''D： 鉴别器与生成器G的代码非常相似，都是有两个隐藏层和三个线性映射的前馈图。
   它将从R或G获取样本，并输出介于0和1之间的单个标量，0和1分别表示“假”和“真”。'''
class Discriminator(nn.Module):
    def __init__(self, input_size, hidden_size, output_size):
        super(Discriminator, self).__init__()
        self.map1 = nn.Linear(input_size, hidden_size)
        self.map2 = nn.Linear(hidden_size, hidden_size)
        self.map3 = nn.Linear(hidden_size, output_size)
    def forward(self, x):
        x = F.elu(self.map1(x))
        x = F.elu(self.map2(x))
        return F.sigmoid(self.map3(x))
def extract(v):
    return v.data.storage().tolist()
def stats(d):
    return [np.mean(d), np.std(d)]
def decorate_with_diffs(data, exponent):
    mean = torch.mean(data.data, 1, keepdim=True)
    mean_broadcast = torch.mul(torch.ones(data.size()), mean.tolist()[0][0])
    diffs = torch.pow(data - Variable(mean_broadcast), exponent)
    return torch.cat([data, diffs], 1)
d_sampler = get_distribution_sampler(data_mean, data_stddev)
gi_sampler = get_generator_input_sampler()
G = Generator(input_size=g_input_size, hidden_size=g_hidden_size, output_size=g_output_size)
D = Discriminator(input_size=d_input_func(d_input_size), hidden_size=d_hidden_size, output_size=d_output_size)
criterion = nn.BCELoss()  # Binary cross entropy: http://pytorch.org/docs/nn.html#bceloss
d_optimizer = optim.Adam(D.parameters(), lr=d_learning_rate, betas=optim_betas)
g_optimizer = optim.Adam(G.parameters(), lr=g_learning_rate, betas=optim_betas)
for epoch in range(num_epochs):
    # '''训练循环 最后，训练循环在两种模式之间交替：首先，用带有准确标签的真实数据和假数据来训练D；然后，训练G来愚弄D。'''
for d_index in range(d_steps):
        # 1. Train D on real+fake
D.zero_grad()
        #  1A: Train D on real
d_real_data = Variable(d_sampler(d_input_size))
        d_real_decision = D(preprocess(d_real_data))
        d_real_error = criterion(d_real_decision, Variable(torch.ones(1)))  # ones = true
d_real_error.backward()  # compute/store gradients, but don't change params
        #  1B: Train D on fake
d_gen_input = Variable(gi_sampler(minibatch_size, g_input_size))
        d_fake_data = G(d_gen_input).detach()  # detach to avoid training G on these labels
d_fake_decision = D(preprocess(d_fake_data.t()))
        d_fake_error = criterion(d_fake_decision, Variable(torch.zeros(1)))  # zeros = fake
d_fake_error.backward()
        d_optimizer.step()  # Only optimizes D's parameters; changes based on stored gradients from backward()
for g_index in range(g_steps):
        # 2. Train G on D's response (but DO NOT train D on these labels)
G.zero_grad()
        gen_input = Variable(gi_sampler(minibatch_size, g_input_size))
        g_fake_data = G(gen_input)
        dg_fake_decision = D(preprocess(g_fake_data.t()))
        g_error = criterion(dg_fake_decision, Variable(torch.ones(1)))  # we want to fool, so pretend it's all genuine
g_error.backward()
        g_optimizer.step()  # Only optimizes G's parameters
if epoch % print_interval == 0:
        print("%s: D: %s/%s G: %s (Real: %s, Fake: %s) " % (epoch,
extract(d_real_error)[0],
extract(d_fake_error)[0],
extract(g_error)[0],
stats(extract(d_real_data)),
stats(extract(d_fake_data))))
