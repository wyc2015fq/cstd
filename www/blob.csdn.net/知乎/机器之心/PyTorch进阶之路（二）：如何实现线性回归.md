# PyTorch进阶之路（二）：如何实现线性回归 - 知乎
# 



> PyTorch 是 Facebook 开发和维护的一个开源的神经网络库，近来的发展势头相当强劲，也有越来越多的开发者为其撰写教程，本文也是其中之一。这是「PyTorch: Zero to GANs」系列教程的第二篇，介绍了在 PyTorch 中实现线性回归和梯度下降的基本方法。

**选自medium，作者：Aakash N S，机器之心编译，参与：Panda。**

这篇文章将讨论机器学习的一大基本算法：线性回归。我们将创建一个模型，使其能根据一个区域的平均温度、降雨量和湿度（输入变量或特征）预测苹果和橙子的作物产量（目标变量）。训练数据如下：
![](https://pic4.zhimg.com/v2-ad22857fb005da5418f0493a8971b8d3_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1022' height='258'></svg>)
在线性回归模型中，每个目标变量的估计方式都是作为输入变量的一个加权和，另外还会有某个常量偏移（也被称为偏置量）：

yield_apple = w11 * temp + w12 * rainfall + w13 * humidity + b1

yield_orange = w21 * temp + w22 * rainfall + w23 * humidity + b2 

可视化地看，这意味着苹果或橙子的产量是温度、降雨量或湿度的线性函数或平面函数：
![](https://pic1.zhimg.com/v2-d7f00d76b8939e3b879080aa6e1feed8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1024' height='682'></svg>)因为我们只能展示三个维度，所以此处没有给出湿度
线性回归的「学习」部分是指通过检视训练数据找到一组权重（w11、w12…w23）和偏置 b1 和 b2），从而能根据新数据得到准确的预测结果（即使用一个新地区的平均温度、降雨量和湿度预测苹果和橙子的产量）。为了得到更好的结果，这个过程会对权重进行许多次调整，其中会用到一种名为「梯度下降」的优化技术。

**系统设置**

如果你想一边阅读一边运行代码，你可以通过下面的链接找到本教程的 Jupyter Notebook：

[https://jvn.io/aakashns/e556978bda9343f3b30b3a9fd2a25012](https://link.zhihu.com/?target=https%3A//jvn.io/aakashns/e556978bda9343f3b30b3a9fd2a25012)

你可以克隆这个笔记，安装必要的依赖包，然后通过在终端运行以下命令来启动 Jupyter：[Share Jupyter notebooks instantly​jvn.io![图标](https://pic2.zhimg.com/v2-3b918d75cc4e95c20869560cc408a22d_ipico.jpg)](https://link.zhihu.com/?target=https%3A//jvn.io/aakashns/e556978bda9343f3b30b3a9fd2a25012)你可以克隆这个笔记，安装必要的依赖包，然后通过在终端运行以下命令来启动 Jupyter：


```
$ pip install jovian --upgrade # Install the jovian library 
$ jovian clone e556978bda9343f3b30b3a9fd2a25012 # Download notebook & dependencies
$ cd 02-linear-regression # Enter the created directory 
$ conda env update # Install the dependencies
$ conda activate 02-linear-regression # Activate virtual environment
$ jupyter notebook # Start Jupyter
```


如果你的 conda 版本更旧一些，你也许需要运行 source activate 02-linear-regression 来激活环境。对以上步骤的更详细的解释可参阅[本教程的前一篇文章](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758447%26idx%3D3%26sn%3D6e224d27d99794c7259a29d3165b0163%26chksm%3D871a9951b06d104739d353904ee4d418e2c67ee1c326cadfc81559d715f605336d302c4391c8%26scene%3D21%23wechat_redirect)。

首先我们导入 Numpy 和 PyTorch：
![](https://pic2.zhimg.com/v2-ffec30b92d609c98942433e5c5fa23ad_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1030' height='124'></svg>)
**训练数据**

训练数据可以使用两个矩阵表示：输入矩阵和目标矩阵；其中每个矩阵的每一行都表示一个观察结果，每一列都表示一个变量。
![](https://pic3.zhimg.com/v2-ea3ee56e5e1e98572eb527d10e198e5a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1022' height='502'></svg>)
我们已经分开了输入变量和目标变量，因为我们将分别操作它们。另外，我们创建的是 numpy 数组，因为这是常用的操作训练数据的方式：将某些 CSV 文件读取成 numpy 数组，进行一些处理，然后再将它们转换成 PyTorch 张量，如下所示：
![](https://pic4.zhimg.com/v2-9de50658f4f8a0a7a572d1e4c18adbb7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='564'></svg>)
**从头开始构建线性回归模型**

权重和偏置（w11、w12…w23、b1 和 b2）也可表示成矩阵，并初始化为随机值。w 的第一行和 b 的第一个元素用于预测第一个目标变量，即苹果的产量；对应的第二个则用于预测橙子的产量。
![](https://pic3.zhimg.com/v2-df4ca2baf76bfeeb0f197fc5fa5a2b46_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='336'></svg>)
torch.randn 会创建一个给定形状的张量，其中的元素随机选取自一个均值为 0 且标准差为 1 的正态分布。

该模型实际上就是一个简单的函数：执行输入 x 和权重 w 的矩阵乘法，再加上偏置 b（每个观察都会重复该计算）。
![](https://pic4.zhimg.com/v2-bb697be97d95926e3a5c1acbc2d0f94b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='844' height='302'></svg>)
我们可将该模型定义为：
![](https://pic2.zhimg.com/v2-a2d81654d54b97d8fcec1fba695e3c01_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='110'></svg>)
@ 表示 PyTorch 中的矩阵乘法，.t 方法会返回一个张量的转置。

通过将输入数据传入模型而得到的矩阵是目标变量的一组预测结果。
![](https://pic3.zhimg.com/v2-c6de8805f0ede1553f199870afcb0e6a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='346'></svg>)
接下来比较我们的模型的预测结果与实际的目标。
![](https://pic3.zhimg.com/v2-c0f036164b7dfae33c6308a35b624b4a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='286'></svg>)
可以看到，我们的模型的预测结果与目标变量的实际值之间差距巨大。很显然，这是由于我们的模型的初始化使用了随机权重和偏置，我们可不能期望这些随机值就刚好合适。

**损失函数**

在我们改进我们的模型之前，我们需要一种评估模型表现优劣的方法。我们可以使用以下方法比较模型预测和实际目标：
- 计算两个矩阵（preds 和 targets）之间的差异；
- 求这个差异矩阵的所有元素的平方以消除其中的负值；
- 计算所得矩阵中元素的平均值。

结果是一个数值，称为均方误差（MSE）。
![](https://pic4.zhimg.com/v2-661c5b720a2f3e276620126f41ca0437_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='174'></svg>)
torch.sum 返回一个张量中所有元素的和，.numel 方法则返回一个张量中元素的数量。我们来计算一下我们模型的当前预测的均方误差：
![](https://pic1.zhimg.com/v2-2ad50eb22ee7f31c71dae48856f59ec4_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='196'></svg>)
我们解读一下这个结果：平均而言，预测结果中每个元素与实际目标之间的差距大约为 215（46194 的平方根）。考虑到我们所要预测的数值的范围本身只有 50-200，所以这个结果实在相当糟糕。我们称这个结果为损失（loss），因为它指示了模型在预测目标变量方面的糟糕程度。损失越低，模型越好。

**计算梯度**

使用 PyTorch，我们可以根据权重和偏置自动计算 loss 的梯度和导数，因为它们已将 requires_grad 设置为 True。
![](https://pic1.zhimg.com/v2-82617d189d59b523441443e863fee83c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='110'></svg>)
这些梯度存储在各自张量的 .grad 属性中。注意，根据权重矩阵求得的 loss 的导数本身也是一个矩阵，且具有相同的维度。
![](https://pic3.zhimg.com/v2-ea43f8b7a691d866f0fd8ddb04018fbe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='296'></svg>)
这个损失是我们的权重和偏置的一个二次函数，而我们的目标是找到能使得损失最低的权重集。如果我们绘制出任意单个权重或偏置元素下的损失的图表，我们会得到类似下图的结果。通过微积分，我们可以了解到梯度表示损失的变化率，即与权重和偏置相关的损失函数的斜率。

如果梯度元素为正数，则：
- 稍微增大元素的值会增大损失。
- 稍微减小元素的值会降低损失。
![](https://pic4.zhimg.com/v2-191d8c5e494b1c512a76eebdd41a4607_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='675'></svg>)作为权重的函数的 MSE 损失（蓝线表示梯度）
如果梯度元素为负数，则：
- 稍微增大元素的值会降低损失。
- 稍微减小元素的值会增大损失。
![](https://pic4.zhimg.com/v2-ed8a4ad40a364ee20113d6a8bf24e2bf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='675'></svg>)作为权重的函数的 MSE 损失（绿线表示梯度）
通过改变一个权重元素而造成的损失的增减正比于该元素的损失的梯度值。这就是我们用来提升我们的模型的优化算法的基础。

在我们继续之前，我们通过调用 .zero() 方法将梯度重置为零。我们需要这么做的原因是 PyTorch 会累积梯度，也就是说，我们下一次在损失上调用 .backward 时，新的梯度值会被加到已有的梯度值上，这可能会导致意外结果出现。
![](https://pic2.zhimg.com/v2-7210e07ce53542875acae6422368d0e9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='324'></svg>)
**使用梯度下降调整权重和偏置**

我们将使用梯度下降优化算法来降低损失和改善我们的模型，步骤如下：
- 生成预测
- 计算损失
- 根据权重和偏置计算梯度
- 按比例减去少量梯度来调整权重
- 将梯度重置为零

下面我们一步步地实现：
![](https://pic3.zhimg.com/v2-7bdb8653970221c98d4a0f132fee2a76_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='334'></svg>)
注意，这里的预测结果和之前的一样，因为我们还未对我们的模型做出任何修改。损失和梯度也是如此。
![](https://pic1.zhimg.com/v2-82c7b781df6cedd68914af9072363378_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='504'></svg>)
最后，使用上面计算得到的梯度更新权重和偏置。
![](https://pic3.zhimg.com/v2-2f14582c6ad0227712551ffeb942101e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='248'></svg>)
上面需要注意的几点：
- 我们使用 torch.no_grad 指示 PyTorch 我们在更新权重和偏置时不应该跟踪、计算或修改梯度。
- 我们为梯度乘上了一个非常小的数值（这个案例中为 10^-5），以确保我们不会改变权重太多，因为我们只想在梯度的下降方向上迈出一小步。这个数值是这个算法的学习率（learning rate）。
- 在更新权重之后，我们将梯度重置为零，以免影响后续计算。

现在我们来看看新的权重和偏置：
![](https://pic3.zhimg.com/v2-4fb5b95fae933cdbc0991a81b91d064a_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1014' height='218'></svg>)
使用新的权重和偏置，模型的损失应更低。
![](https://pic3.zhimg.com/v2-5b6a469dfad7e6d65bb26ac4bd169736_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1022' height='222'></svg>)
只是简单地使用梯度下降来稍微调整权重和偏置，我们就已经实现了损失的显著下降。

**多次训练**

为了进一步降低损失，我们可以多次使用梯度重复调整权重和偏置的过程。一次迭代被称为一个 epoch。我们训练模型 100 epoch 看看。
![](https://pic2.zhimg.com/v2-f742bc25da2b98524851acf1f3430aa9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='386'></svg>)
再次验证，现在损失应该会更低：
![](https://pic4.zhimg.com/v2-14ef044c0ee1697681cb34c418450acb_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='226'></svg>)
可以看到，现在的损失比我们开始时低了很多。我们看看模型的预测结果，并将其与目标比较一下。
![](https://pic2.zhimg.com/v2-be6248cb56f102d2e3248be4763e7469_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='604'></svg>)
现在的预测结果已非常接近目标变量；而且通过训练模型更多 epoch，我们还能得到甚至更好的结果。

**使用 PyTorch 内置的线性回归**

上面的模型和训练过程是使用基本的矩阵运算实现的。但因为这是一种非常常用的模式，所以 PyTorch 配备了几个内置函数和类，让人能很轻松地创建和训练模型。

首先从 PyTorch 导入 torch.nn 软件包，其中包含了用于创建神经网络的效用程序类。
![](https://pic1.zhimg.com/v2-f91d94abe2689f1d76dac8fa3b9a6d10_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='70'></svg>)
和之前一样，我们将输入和目标表示成矩阵形式。
![](https://pic2.zhimg.com/v2-ee3c971ca5129cd802de0574832c8145_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='654'></svg>)
我们这一次使用 15 个训练样本，以演示如何以小批量的形式处理大数据集。

**数据集和数据加载器**

我们将创建一个 TensorDataset，这让我们可以读取 inputs 和 targets 的行作为元组，并提供了 PyTorch 中用于处理许多不同类型的数据集的标准 API。
![](https://pic4.zhimg.com/v2-6b42e4b67610d60ae2d1128d46f31f4b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='406'></svg>)
TensorDataset 让我们可以使用数组索引表示法（上面代码中的 [0:3]）读取一小部分训练数据。它会返回一个元组（或配对），其中第一个元素包含所选行的输入变量，第二个元素包含目标，

我们还将创建一个 DataLoader，它可以在训练时将数据分成预定义大小的批次。它还能提供其它效用程序，如数据的混洗和随机采样。
![](https://pic4.zhimg.com/v2-2bcce705ef4a395bdefb852bb1df2e2b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='232'></svg>)
数据加载器通常搭配 for-in 循环使用。举个例子：
![](https://pic1.zhimg.com/v2-c5e234362a3df7b40394b8ec5984bc30_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='524'></svg>)



在每次迭代中，数据加载器都会返回一批给定批大小的数据。如果 shuffle 设为 True，则在创建批之前会对训练数据进行混洗。混洗能帮助优化算法的输入随机化，这能实现损失的更快下降。

**nn.Linear**

除了人工地实现权重和偏置的初始化，我们还可以使用 PyTorch 的 nn.Linear 类来定义模型，这能自动完成初始化。
![](https://pic2.zhimg.com/v2-f954bb7b1d8f8abf732259a36a2294c9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='354'></svg>)
PyTorch 模型还有一个很有用的 .parameters 方法，这能返回一个列表，其中包含了模型中所有的权重和偏置矩阵。对于我们的线性回归模型，我们有一个权重矩阵和一个偏置矩阵。
![](https://pic2.zhimg.com/v2-f1625980f62942c5fe3b72f0022a1ee1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='328'></svg>)
我们可以使用之前一样的方式来得到模型的预测结果：
![](https://pic4.zhimg.com/v2-2fb4eb99b462b6d5cc3b2f268610de77_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1014' height='664'></svg>)
**损失函数**

除了手动定义损失函数，我们也可使用内置的损失函数 mse_loss：
![](https://pic1.zhimg.com/v2-fa96f5fec032eebbcc3b2bc7c7a692b0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='110'></svg>)
nn.functional 软件包包含很多有用的损失函数和其它几个效用程序。
![](https://pic4.zhimg.com/v2-04ffdaf5b43745cc4e5a3791df05a01b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='104'></svg>)
我们计算一下我们模型的当前预测的损失。
![](https://pic1.zhimg.com/v2-83368c0d7212a3ca00ef219c7fd9a470_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='150'></svg>)
**优化器**

除了以人工方式使用梯度操作模型的权重和偏置，我们也可使用优化器 optim.SGD。SGD 表示「[随机梯度下降](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758606%26idx%3D2%26sn%3Dc77656ddc45118b238521ad92350425d%26chksm%3D871a99b0b06d10a657b802fe2d4d8f5d234b54960395647dfbc9ac51b3ed074257fec3f5a03b%26token%3D1620590487%26lang%3Dzh_CN)」。之所以是「随机」，原因是样本是以批的形式选择（通常会用到随机混洗），而不是作为单独一个数据组。
![](https://pic4.zhimg.com/v2-9f7650b6b50fcd6c53da734c62c1f95b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='110'></svg>)
注意，这里的 model.parameters() 是 optim.SGD 的一个参数，这样优化器才知道在训练步骤中应该修改哪些矩阵。另外，我们还可以指定一个[学习率](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650758606%26idx%3D2%26sn%3Dc77656ddc45118b238521ad92350425d%26chksm%3D871a99b0b06d10a657b802fe2d4d8f5d234b54960395647dfbc9ac51b3ed074257fec3f5a03b%26token%3D1620590487%26lang%3Dzh_CN)来控制参数每次的修改量。

**训练模型**

我们现在已经准备好训练模型了。我们将遵循实现梯度下降的同一过程：
- 生成预测
- 计算损失
- 根据权重和偏置计算梯度
- 按比例减去少量梯度来调整权重
- 将梯度重置为零

唯一变化的是我们操作的是分批的数据，而不是在每次迭代中都处理整个训练数据集。我们定义一个效用函数 fit，可训练模型给定的 epoch 数量。
![](https://pic4.zhimg.com/v2-a348e4ad33a5fe48f1bf4c361670a003_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='996'></svg>)
上面需要注意的几点：
- 我们使用之前定义的数据加载器来为每个迭代获取数据批次。
- 我们没有手动更新参数（权重和偏置），而是使用了 opt.step 来执行更新，并使用了 opt.zero_grad 来将梯度重置为零。
- 我们还添加了一个日志语句，能够显示每第 10 个 epoch 的最后一批数据的损失，从而可让我们跟踪训练进程。loss.item 会返回存储在损失张量中的实际值。

训练模型 100 epoch。
![](https://pic4.zhimg.com/v2-2d89e9b0861a64b701ff87fe9283ad03_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1018' height='426'></svg>)
接下来使用我们的模型生成预测结果，再验证它们与目标的接近程度。
![](https://pic4.zhimg.com/v2-798fa4a5c3ce3262122700823755a043_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1020' height='664'></svg>)![](https://pic2.zhimg.com/v2-7b00f65a4da347a3580954dd5a807431_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1016' height='630'></svg>)
实际上，这个预测结果非常接近我们的目标。现在，我们有了一个相当好的预测模型，可以根据一个地区的平均温度、降雨量和湿度预测苹果和橙子的产量。

**进阶阅读**

本教程覆盖了很多基础内容，包括线性回归和梯度下降优化算法。如果你想进一步探索这些主题，可参考以下资源：
- 对导数和梯度下降的更详尽的解释可参考这些 Udacity 课程笔记

[https://storage.googleapis.com/supplemental_media/udacityu/315142919/Gradient%20Descent.pdf](https://link.zhihu.com/?target=https%3A//storage.googleapis.com/supplemental_media/udacityu/315142919/Gradient%2520Descent.pdf)
- 线性回归工作方式的可视化动画

[https://hackernoon.com/visualizing-linear-regression-with-pytorch-9261f49edb09](https://link.zhihu.com/?target=https%3A//hackernoon.com/visualizing-linear-regression-with-pytorch-9261f49edb09)
- 想从数学方面理解矩阵微积分、线性回归和梯度下降？那你不能错过吴恩达的斯坦福大学 CS229 的课程笔记

[https://github.com/Cleo-Stanford-CS/CS229_Notes/blob/master/lectures/cs229-notes1.pdf](https://link.zhihu.com/?target=https%3A//github.com/Cleo-Stanford-CS/CS229_Notes/blob/master/lectures/cs229-notes1.pdf)

*原文链接：*[https://medium.com/jovian-io/linear-regression-with-pytorch-3dde91d60b50](https://link.zhihu.com/?target=https%3A//medium.com/jovian-io/linear-regression-with-pytorch-3dde91d60b50)




