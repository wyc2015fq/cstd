
# pytorch 学习笔记之自定义 Module - 丁丁的博客 - CSDN博客


2017年04月22日 14:42:39[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：6952


[pytorch](http://pytorch.org/)是一个基于 python 的深度学习库。pytorch 源码库的抽象层次少，结构清晰，代码量适中。相比于非常工程化的 tensorflow，pytorch 是一个更易入手的，非常棒的深度学习框架。
对于系统学习 pytorch，官方提供了非常好的[入门教程](http://pytorch.org/tutorials/)，同时还提供了面向深度学习的[示例](https://github.com/pytorch/examples)，同时热心网友分享了更简洁的[示例](https://github.com/yunjey/pytorch-tutorial)。
# 1. overview
不同于 theano，tensorflow 等低层程序库，或者 keras、sonnet 等高层 wrapper，pytorch 是一种自成体系的深度学习库（图1）。
![这里写图片描述](https://img-blog.csdn.net/20170419173041252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170419173041252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图1. 几种深度学习程序库对比**
[
](https://img-blog.csdn.net/20170419173041252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)如图2所示，pytorch 由低层到上层主要有三大块功能模块。
[
](https://img-blog.csdn.net/20170419173041252?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图2. pytorch 主要功能模块**
[

](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 1.1 张量计算引擎（tensor computation）
[
](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)Tensor 计算引擎，类似 numpy 和 matlab，基本对象是tensor（类比 numpy 中的 ndarray 或 matlab 中的 array）。除提供基于 CPU 的常用操作的实现外，pytorch 还提供了高效的 GPU 实现，这对于深度学习至关重要。
[

](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 1.2 自动求导机制（autograd）
[
](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)由于深度学习模型日趋复杂，因此，对自动求导的支持对于学习框架变得必不可少。pytorch 采用了动态求导机制，使用类似方法的框架包括： chainer，dynet。作为对比，theano，tensorflow 采用静态自动求导机制。
[

](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 1.3 神经网络的高层库（NN）
[
](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)pytorch 还提供了高层的[神经网络模块](http://pytorch.org/docs/nn.html#)。对于常用的网络结构，如全连接、卷积、RNN 等。同时，pytorch 还提供了常用的[目标函数](http://pytorch.org/docs/nn.html#loss-functions)、[optimizer](http://pytorch.org/docs/optim.html)及参数[初始化方法](http://pytorch.org/docs/nn.html#torch-nn-init)。
[
](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)这里，我们重点关注如何自定义神经网络结构。
[

](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 2. 自定义 Module
[
](https://img-blog.csdn.net/20170420110015608?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![这里写图片描述](https://img-blog.csdn.net/20170419173147126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170419173147126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图3. pytorch Module**
module 是 pytorch 组织神经网络的基本方式。Module 包含了模型的参数以及计算逻辑。Function 承载了实际的功能，定义了前向和后向的计算逻辑。
Module 是任何神经网络的基类，pytorch 中所有模型都必需是 Module 的子类。 Module 可以套嵌，构成树状结构。一个 Module 可以通过将其他 Module 做为属性的方式，完成套嵌。
> 注意：真到目前（04/2018），pytorch 这部分的接口都没稳定下来，下面的阐述已经和最新版本不一致，甚至不正确。在接口最终稳定之前，内容不再更新，请直接查阅 pytorch 的最新源码。
下面以最简单的 MLP 网络结构为例，介绍下如何实现自定义网络结构。完整代码可以参见[repo](https://github.com/DingKe/pytorch_workplace/tree/master/basic)。
## 2.1 Function
> 注：为支持高阶导数（i.e. 梯度的梯度），pytorch 0.2 收入新的定义 Function 的机制。如果不考虑高阶，旧的方法依然 work。
Function 是 pytorch 自动求导机制的核心类。Function 是无参数或者说无状态的，它只负责接收输入，返回相应的输出；对于反向，它接收输出相应的梯度，返回输入相应的梯度。
这里我们只关注如何自定义 Function。Function 的定义见[源码](https://github.com/pytorch/pytorch/blob/master/torch/autograd/function.py#L108)。下面是简化的代码段：
```python
class
```
```python
Function
```
```python
(object)
```
```python
:
```
```python
def
```
```python
forward
```
```python
(self, *input)
```
```python
:
```
```python
raise
```
```python
NotImplementedError
```
```python
def
```
```python
backward
```
```python
(self, *grad_output)
```
```python
:
```
```python
raise
```
```python
NotImplementedError
```
**forward  和 backward 的输入和输出都是 Tensor 对象**
Function 对象是 callable 的，即可以通过()的方式进行调用。其中调用的输入和输出都为 Variable 对象。下面的[代码](https://github.com/DingKe/pytorch_workplace/blob/master/basic/functions.py#L8)示例了如何实现一个 ReLU 激活函数并进行调用：
```python
import
```
```python
torch
```
```python
from
```
```python
torch.autograd
```
```python
import
```
```python
Function
```
```python
class
```
```python
ReLUF
```
```python
(Function)
```
```python
：
```
```python
def
```
```python
forward
```
```python
(self, input)
```
```python
:
```
```python
self.save_for_backward(input)
        output = input.clamp(min=
```
```python
0
```
```python
)
```
```python
return
```
```python
output
```
```python
def
```
```python
backward
```
```python
(self, output_grad)
```
```python
:
```
```python
input, = self.saved_tensors
        input_grad = output_grad.clone()
        input_grad[input <
```
```python
0
```
```python
] =
```
```python
0
```
```python
return
```
```python
input_grad
```
```python
## Test
```
```python
if
```
```python
__name__ ==
```
```python
"__main__"
```
```python
:
```
```python
from
```
```python
torch.autograd
```
```python
import
```
```python
Variable
      torch.manual_seed(
```
```python
1111
```
```python
)  
      a = torch.randn(
```
```python
2
```
```python
,
```
```python
3
```
```python
)
      va = Variable(a, requires_grad=
```
```python
True
```
```python
)
      vb = ReLUF()(va)
```
```python
print
```
```python
va.data, vb.data
      vb.backward(torch.ones(va.size()))
```
```python
print
```
```python
vb.grad.data, va.grad.data
```
如果 backward 中需要用到 forward 的输入，需要在 forward 中显式的保存需要的输入。在上面的代码中，forward 利用**self.save_for_backward**函数，将输入暂时保存，并在 backward 中利用**saved_tensors**(python tuple 对象) 取出。
显然，forward 的输入应该和 backward 的输入相对应；同时，forward 的输出应该和 backward 的输入相匹配。
> 由于 Function 可能需要暂存 input tensor，因此，建议不复用 Function 对象，以避免遇到内存提前释放的问题。如
> [示例代码](https://github.com/DingKe/pytorch_workplace/blob/master/basic/custom_function.py#L46)
> 所示，forward的每次调用都重新生成一个 ReLUF 对象，而不能在初始化时生成在 forward 中反复调用。

## 2.2 Module
类似于 Function，Module 对象也是 callable 是，输入和输出也是 Variable。不同的是，Module 是[可以]有参数的。Module 包含两个主要部分：参数及计算逻辑（Function 调用）。由于 ReLU 激活函数没有参数，这里我们以最基本的全连接层为例来说明如何自定义 Module。
全连接层的运算逻辑定义如下 Function:
```python
import
```
```python
torch
```
```python
from
```
```python
torch.autograd
```
```python
import
```
```python
Function
```
```python
class
```
```python
LinearF
```
```python
(Function)
```
```python
:
```
```python
def
```
```python
forward
```
```python
(self, input, weight, bias=None)
```
```python
:
```
```python
self.save_for_backward(input, weight, bias)
         output = torch.mm(input, weight.t())
```
```python
if
```
```python
bias
```
```python
is
```
```python
not
```
```python
None
```
```python
:
             output += bias.unsqueeze(
```
```python
0
```
```python
).expand_as(output)
```
```python
return
```
```python
output
```
```python
def
```
```python
backward
```
```python
(self, grad_output)
```
```python
:
```
```python
input, weight, bias = self.saved_tensors
         grad_input = grad_weight = grad_bias =
```
```python
None
```
```python
if
```
```python
self.needs_input_grad[
```
```python
0
```
```python
]:
             grad_input = torch.mm(grad_output, weight)
```
```python
if
```
```python
self.needs_input_grad[
```
```python
1
```
```python
]:
             grad_weight = torch.mm(grad_output.t(), input)
```
```python
if
```
```python
bias
```
```python
is
```
```python
not
```
```python
None
```
```python
and
```
```python
self.needs_input_grad[
```
```python
2
```
```python
]:
             grad_bias = grad_output.sum(
```
```python
0
```
```python
).squeeze(
```
```python
0
```
```python
)
```
```python
if
```
```python
bias
```
```python
is
```
```python
not
```
```python
None
```
```python
:
```
```python
return
```
```python
grad_input, grad_weight, grad_bias
```
```python
else
```
```python
:
```
```python
return
```
```python
grad_input, grad_weight
```
**[needs_input_grad](https://github.com/pytorch/pytorch/blob/master/torch/autograd/function.py#L31)**为一个元素为 bool 型的 tuple，长度与 forward 的参数数量相同，用来标识各个输入是否输入计算梯度；对于无需梯度的输入，可以减少不必要的计算。
Function（此处为 LinearF） 定义了基本的计算逻辑，Module 只需要在初始化时为参数分配内存空间，并在计算时，将参数传递给相应的 Function 对象。代码如下：
```python
import
```
```python
torch
```
```python
import
```
```python
torch.nn
```
```python
as
```
```python
nn
```
```python
class
```
```python
Linear
```
```python
(nn.Module)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self, in_features, out_features, bias=True)
```
```python
:
```
```python
super(Linear, self).__init__()
         self.in_features = in_features
         self.out_features = out_features
         self.weight = nn.Parameter(torch.Tensor(out_features, in_features))
```
```python
if
```
```python
bias:
             self.bias = nn.Parameter(torch.Tensor(out_features))
```
```python
else
```
```python
:
            self.register_parameter(
```
```python
'bias'
```
```python
,
```
```python
None
```
```python
)
```
```python
def
```
```python
forward
```
```python
(self, input)
```
```python
:
```
```python
return
```
```python
LinearF()(input, self.weight, self.bias)
```
需要注意的是，参数是内存空间由 tensor 对象维护，但 tensor 需要包装为一个[Parameter](https://github.com/pytorch/pytorch/blob/master/torch/nn/parameter.py)对象。Parameter 是[Variable](https://github.com/pytorch/pytorch/blob/master/torch/autograd/variable.py)的特殊子类，仅有是不同是 Parameter 默认 requires_grad 为 True。Varaible 是自动求导机制的核心类，此处暂不介绍，参见[教程](http://pytorch.org/tutorials/beginner/blitz/autograd_tutorial.html#sphx-glr-beginner-blitz-autograd-tutorial-py)。
# 3. 自定义循环神经网络（RNN）
我们尝试自己定义一个更复杂的 Module ——RNN。这里，我们只定义最基础的 vanilla RNN（图4），基本的计算公式如下：

$$
h_t = relu(W \cdot x + U \cdot h_{t-1} )
$$
![RNN](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图4. RNN【****[来源](http://www.2cto.com/kf/201609/550394.html)****】**
[
](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)更复杂的 LSTM、GRU 或者其他变种的实现也非常类似。
[

](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3.1 定义 Cell
[

](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
import
```
```python
torch
```
```python
from
```
```python
torch.nn
```
```python
import
```
```python
Module, Parameter
```
```python
class
```
```python
RNNCell
```
```python
(Module)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self, input_size, hidden_size)
```
```python
:
```
```python
super(RNNCell, self).__init__()
        self.input_size = input_size
        self.hidden_size = hidden_size
        self.weight_ih = Parameter(torch.Tensor(hidden_size, input_size))
        self.weight_hh = Parameter(torch.Tensor(hidden_size, hidden_size))
        self.bias_ih = Parameter(torch.Tensor(hidden_size))
        self.bias_hh = Parameter(torch.Tensor(hidden_size))
        self.reset_parameters()
```
```python
def
```
```python
reset_parameters
```
```python
(self)
```
```python
:
```
```python
stdv =
```
```python
1.0
```
```python
/ math.sqrt(self.hidden_size)
```
```python
for
```
```python
weight
```
```python
in
```
```python
self.parameters():
            weight.data.uniform_(-stdv, stdv)
```
```python
def
```
```python
forward
```
```python
(self, input, h)
```
```python
:
```
```python
output = LinearF()(input, self.weight_ih, self.bias_ih) + LinearF()(h, self.weight_hh, self.bias_hh)
        output = ReLUF()(output)
```
```python
return
```
```python
output
```
[

](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
## 3.2 定义完整的 RNN
[

](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
```python
import
```
```python
torch
```
```python
from
```
```python
torch.nn
```
```python
import
```
```python
Module
```
```python
class
```
```python
RNN
```
```python
(Moudule)
```
```python
:
```
```python
def
```
```python
__init__
```
```python
(self, input_size, hidden_size)
```
```python
:
```
```python
super(RNN, self).__init__()
        self.input_size = input_size
        self.hidden_size = hidden_size
        sef.cell = RNNCell(input_size, hidden_size)
```
```python
def
```
```python
forward
```
```python
(self, inputs, initial_state)
```
```python
:
```
```python
time_steps = inputs.size(
```
```python
1
```
```python
)
        state = initial_state
        outputs = []
```
```python
for
```
```python
t
```
```python
in
```
```python
range(time_steps):
            state = self.cell(inputs[:, t, :], state)
            outputs.append(state)
```
```python
return
```
```python
outputs
```
[
](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)可运行的完整代码见[repo](https://github.com/DingKe/pytorch_workplace/tree/master/rnn)。
[

](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 讨论
[
](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)pytorch 的 Module 结构是传承自 torch，这一点也同样被 keras （functional API）所借鉴。 在 caffe 等一些[早期的]深度学习框架中，network 是由于若干 layer ，经由不同的拓扑结构组成的。而在 (pyt)torch 中没有 layer 和 network 是区分，一切都是 callable 的 Module。Module 的调用的输入和输出都是 tensor （由 Variable 封装），用户可以非常自然的构造任意有向无环的网络结构（DAG）。
[
](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)同时， pytorch 的 autograd 机制封装的比较浅，可以比较容易的定制反传或修改梯度。这对有些算法是非常重要。
[
](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)总之，仅就自定义算法而言，pytorch 是一个非常优雅的深度学习框架。
[            ](https://img-blog.csdn.net/20170422124737125?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

[
  ](https://img-blog.csdn.net/20170419173147126?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)