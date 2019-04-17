# torch学习（二） nn类结构-Module - xmdxcsj的专栏 - CSDN博客





2015年10月21日 23:36:02[xmucas](https://me.csdn.net/xmdxcsj)阅读数：10387








Module

|-Container

   |-Sequential

   |-Parallel

   |-Concat

      |-DepthConcat

安装使用torch以后，接下来就可以开始构建模型和训练了，在这之前，一定要先花半个小时的时间了解一下torch里面nn部分使用到的基本类型，否则容易混乱。

一、Module

Module是神经网络的基本组成部分，作为一个抽象类，可以通过定义成员函数实现不同的神经网络结构，nn.*都是Module的子类。

成员变量有output和gradInput，成员函数包括：

(一)、[output] forward(input)

根据input计算更新output，不建议重写该函数

1.调用updateOutput(input)

对module的输出进行处理，可以重写该函数




(二)、[gradInput] backward(input, gradOutput)

在使用backward之前需要首先调用forward，不建议重写，可以重写它调用的两个子函数

input：该层的输入

gradOutput：对该层输出的梯度

gradInput：对该层输入的梯度

1.updateGradInput(input, gradOutput)

计算相对于输入input的梯度，更新gradInput

2. accGradParameters(input, gradOutput, scale)

计算相对于该Module的参数的梯度

gradParameters：对该层参数的梯度，比如sigmoid层没有参数就没有该变量

scale： gradParameters乘以系数scale，然后累加




3.zeroGradParameters()

重置gradParameters为0




4.updateParameters(learningRate)

根据backward()累加的gradParameters，更新Module的参数


parameters = parameters - learningRate * gradients_wrt_parameters

5.accUpdateCradParameters(input,gradOutput,learningRate)

累积参数梯度，同时更新参数

......




二、Container

父类Module，子类包括Sequential、Parallel、Concat等

新增成员函数包括：

1.add(module)

增加Module

2.get(index)

根据下标获取Module

3.size()

包含的modules




三、Sequential

以feed-forward全连接的形式组织网络

1.remove([index])

删除第index层

2.insert(module,[index])

在中间插入层




四、Parallel

module = Parallel(inputDimension,outputDimension)




Creates a container module that applies its ith child module to the ith slice of the input Tensor by using [select](https://github.com/torch/torch7/blob/master/doc/tensor.md#tensor-selectdim-index) on dimension inputDimension.
 It concatenates the results of its contained modules together along dimensionoutputDimension.

五、Concat

module = nn.Concat(dim)




Concat concatenates the output of one layer of "parallel" modules along the provided dimension dim: they take the same inputs, and their output is concatenated.

六、DepthConcat

DepthConcat concatenates the output of one layer of "parallel" modules along the provided dimension dim: they take the same inputs, and their output is concatenated. For dimensions other than dim having different sizes, the smaller tensors are copied in
 the center of the output tensor, effectively padding the borders with zeros.




参考：

[https://github.com/torch/nn/blob/master/README.md](https://github.com/torch/nn/blob/master/README.md)



