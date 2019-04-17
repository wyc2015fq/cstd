# 深度学习框架中的「张量」不好用？也许我们需要重新定义Tensor了 - 知乎
# 



> 本文介绍了张量的陷阱和一种可以闪避陷阱的替代方法 named tensor，并进行了概念验证。
选自harvardnlp，作者：Alexander Rush，机器之心编译，参与：李诗萌、路雪。

尽管张量在深度学习的世界中无处不在，但它是有破绽的。它催生出了一些坏习惯，比如公开专用维度、基于绝对位置进行广播，以及在文档中保存类型信息。这篇文章介绍了一种具有命名维度的替代方法 named tensor，并对其进行了概念验证。这一改变消除了对索引、维度参数、einsum 式解压缩以及基于文档的编码的需求。这篇文章附带的原型 PyTorch 库可以作为 namedtensor 使用。

PyTorch 库参见：[https://github.com/harvardnlp/NamedTensor](https://link.zhihu.com/?target=https%3A//github.com/harvardnlp/NamedTensor)

实现：
- Jon Malmaud 指出 xarray 项目（[http://xarray.pydata.org/en/stable/](https://link.zhihu.com/?target=http%3A//xarray.pydata.org/en/stable/)）的目标与 namedtensor 非常相似，xarray 项目还增加了大量 Pandas 和科学计算的支持。
- Tongfei Chen 的 Nexus 项目在 Scala 中提出了静态类型安全的张量。
- Stephan Hoyer 和 Eric Christiansen 为 TensorFlow 建立了标注张量库，Labed Tensor，和本文的方法是一样的。
- Nishant Sinha 有 TSA 库，它使用类型注释来定义维度名称。


```
#@title Setup
#!rm -fr NamedTensor/; git clone -q https://github.com/harvardnlp/NamedTensor.git
#!cd NamedTensor; pip install -q .; pip install -q torch numpy opt_einsum

import numpy
import torch
from namedtensor import NamedTensor, ntorch
from namedtensor import _im_init
_im_init()
```


**张量陷阱**

这篇文章是关于张量类的。张量类是多维数组对象，是 Torch、TensorFlow、Chainer 以及 NumPy 等深度学习框架的核心对象。张量具备大量存储空间，还可以向用户公开维度信息。


```
ims = torch.tensor(numpy.load('test_images.npy'))
ims.shape
```





`torch.Size([6, 96, 96, 3])`

该示例中有 4 个维度，对应的是 batch_size、height、width 和 channels。大多数情况下，你可以通过代码注释弄明白维度的信息，如下所示：


```
# batch_size x height x width x channels
ims[0]
```

![](https://pic3.zhimg.com/v2-0c73a02a683a044d7c6a66b234e69786_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
这种方法简明扼要，但从编程角度看来，这不是构建复杂软件的好方法。

**陷阱 1：按惯例对待专用维度**

代码通过元组中的维度标识符操纵张量。如果要旋转图像，阅读注释，确定并更改需要改变的维度。


```
def rotate(ims):
    # batch_size x height x width x channels
    rotated = ims.transpose(1, 2)

    # batch_size x width x height x channels
    return rotated
rotate(ims)[0]
```

![](https://pic2.zhimg.com/v2-96810c7c1389cee306b40f5f00ab2e65_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
这段代码很简单，而且从理论上讲记录详尽。但它并没有反映目标函数的语义。旋转的性质与 batch 或 channel 都无关。在确定要改变的维度时，函数不需要考虑这些维度。

这就产生了两个问题。首先，令人非常担心的是如果我们传入单例图像，函数可以正常运行但是却不起作用。

`rotate(ims[0]).shape`




`torch.Size([96, 3, 96])`

但更令人担忧的是，这个函数实际上可能会错误地用到 batch 维度，还会把不同图像的属性混到一起。如果在代码中隐藏了这个维度，可能会产生一些本来很容易避免的、讨厌的 bug。

**陷阱 2：通过对齐进行广播**

张量最有用的地方是它们可以在不直接需要 for 循环的情况下快速执行数组运算。为此，要直接对齐维度，以便广播张量。同样，这是按照惯例和代码文档实现的，这使排列维度变得「容易」。例如，假设我们想对上图应用掩码。


```
# height x width
mask = torch.randint(0, 2, [96, 96]).byte()
mask
```

![](https://pic3.zhimg.com/v2-c5f8efe159a3f9382fdf874373e9d6b6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)

```
try:
    ims.masked_fill(mask, 0)
except RuntimeError:
    error = "Broadcasting fail %s %s"%(mask.shape, ims.shape)
error
```






```
'Broadcasting fail torch.Size([96, 96]) torch.Size([6, 96, 96, 3])'
```


这里的失败的原因是：即便我们知道要建立掩码的形状，广播的规则也没有正确的语义。为了让它起作用，你需要使用 view 或 squeeze 这些我最不喜欢的函数。


```
# either
mask = mask.unsqueeze(-1)
# or
mask = mask.view(96, 96, 1)

# height x width x channels
ims.masked_fill(mask, 1)[0]
```

![](https://pic2.zhimg.com/v2-3cc981342e1057a5a1d8cc16a9fc4aa9_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
注意，最左边的维度不需要进行这样的运算，所以这里有些抽象。但阅读真正的代码后会发现，右边大量的 view 和 squeeze 变得完全不可读。

**陷阱 3：通过注释访问**

看过上面两个问题后，你可能会认为只要足够小心，运行时就会捕捉到这些问题。但是即使很好地使用了广播和索引的组合，也可能会造成很难捕捉的问题。


```
a = ims[1].mean(2, keepdim=True)
# height x width x 1

# (Lots of code in between)
#  .......................

# Code comment explaining what should be happening.
dim = 1
b = a + ims.mean(dim, keepdim=True)[0]


# (Or maybe should be a 2? or a 0?)
index = 2
b = a + ims.mean(dim, keepdim=True)[0]
b
```

![](https://pic2.zhimg.com/v2-5ea4e9186a933d15ce27ffa35cacc825_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
我们在此假设编码器试着用归约运算和维度索引将两个张量结合在一起。（说实话这会儿我已经忘了维度代表什么。）

重点在于无论给定的维度值是多少，代码都会正常运行。这里的注释描述的是在发生什么，但是代码本身在运行时不会报错。

**Named Tensor：原型**

根据这些问题，我认为深度学习代码应该转向更好的核心对象。为了好玩，我会开发一个新的原型。目标如下：
- 维度应该有人类可读的名字。
- 函数中不应该有维度参数。
- 广播应该通过名称匹配。
- 转换应该是显式的。
- 禁止基于维度的索引。
- 应该保护专用维度。

为了试验这些想法，我建立了一个叫做 NamedTensor 的库。目前它只用于 PyTorch，但从理论上讲类似的想法也适用于其他框架。

**建议 1：分配名称**

库的核心是封装了张量的对象，并给每个维度提供了名称。我们在此用维度名称简单地包装了给定的 torch 张量。


```
named_ims = NamedTensor(ims, ("batch", "height", "width", "channels"))
named_ims.shape
```





`OrderedDict([('batch', 6), ('height', 96), ('width', 96), ('channels', 3)])`

此外，该库有针对 PyTorch 构造函数的封装器，可以将它们转换为命名张量。


```
ex = ntorch.randn(dict(height=96, width=96, channels=3))
ex
```

![](https://pic4.zhimg.com/v2-80130f5bf33f6dec055706bd88700367_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
大多数简单的运算只是简单地保留了命名张量的属性。


```
ex.log()

# or

ntorch.log(ex)

None
```


**建议 2：访问器和归约**

名字的第一个好处是可以完全替换掉维度参数和轴样式参数。例如，假设我们要对每列进行排序。


```
sortex, _ = ex.sort("width")
sortex
```

![](https://pic4.zhimg.com/v2-7c380f0226fd217f7ae5b0347c43510f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
另一个常见的操作是在汇集了一个或多个维度的地方进行归约。

`named_ims.mean("batch")`
![](https://pic3.zhimg.com/v2-add19934a448ad03b5bbb9a0108431ae_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
`named_ims.mean(("batch", "channels"))`
![](https://pic4.zhimg.com/v2-6244c738c176eb94e38fee75b2cf094b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
**建议 3：广播和缩并**

提供的张量名称也为广播操作提供了基础。当两个命名张量间存在二进制运算时，它们首先要保证所有维度都和名称匹配，然后再应用标准的广播。为了演示，我们回到上面的掩码示例。在此我们简单地声明了一下掩码维度的名称，然后让库进行广播。


```
im = NamedTensor(ims[0], ("height", "width", "channels"))
im2 = NamedTensor(ims[1], ("height", "width", "channels"))

mask = NamedTensor(torch.randint(0, 2, [96, 96]).byte(), ("height", "width"))
im.masked_fill(mask, 1)
```

![](https://pic4.zhimg.com/v2-26045973c51bb6b0315e1f9396c3c077_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
加和乘等简单运算可用于标准矩阵。

`im * mask.double()`
![](https://pic3.zhimg.com/v2-cdb7e17a99d7a47c3278cc147cc6922e_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
在命名向量间进行张量缩并的更普遍的特征是 dot 方法。张量缩并是 einsum 背后的机制，是一种思考点积、矩阵-向量乘积、矩阵-矩阵乘积等泛化的优雅方式。


```
# Runs torch.einsum(ijk,ijk->jk, tensor1, tensor2)
im.dot("height", im2).shape
```





`OrderedDict([('width', 96), ('channels', 3)])`





```
# Runs torch.einsum(ijk,ijk->il, tensor1, tensor2)
im.dot("width", im2).shape
```





`OrderedDict([('height', 96), ('channels', 3)])`





```
# Runs torch.einsum(ijk,ijk->l, tensor1, tensor2)
im.dot(("height", "width"), im2).shape
```





`OrderedDict([('channels', 3)])`

类似的注释也可用于稀疏索引（受 einindex 库的启发）。这在嵌入查找和其他稀疏运算中很有用。


```
pick, _ = NamedTensor(torch.randint(0, 96, [50]).long(), ("lookups",)) \
             .sort("lookups")

# Select 50 random rows.
im.index_select("height", pick)
```

![](https://pic3.zhimg.com/v2-f65cbac4a7fa4e4d74dadd054ae454a6_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='50'></svg>)
**建议 4：维度转换**

在后台计算中，所有命名张量都是张量对象，因此维度顺序和步幅这样的事情就尤为重要。transpose 和 view 等运算对于保持维度的顺序和步幅至关重要，但不幸的是它们很容易出错。


那么，我们来考虑领域特定语言 shift，它大量借鉴了 Alex Rogozhnikov 优秀的 einops 包（[https://github.com/arogozhnikov/einops](https://link.zhihu.com/?target=https%3A//github.com/arogozhnikov/einops)）。


```
tensor = NamedTensor(ims[0], ("h", "w", "c"))
tensor
```

![](https://pic3.zhimg.com/v2-0c73a02a683a044d7c6a66b234e69786_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
维度转换的标准调用。

`tensor.transpose("w", "h", "c")`
![](https://pic2.zhimg.com/v2-96810c7c1389cee306b40f5f00ab2e65_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
拆分和叠加维度。


```
tensor = NamedTensor(ims[0], ("h", "w", "c"))
tensor.split(h=("height", "q"), height=8).shape
OrderedDict([('height', 8), ('q', 12), ('w', 96), ('c', 3)])
tensor = NamedTensor(ims, ('b', 'h', 'w', 'c'))
tensor.stack(bh = ('b', 'h')).shape
OrderedDict([('bh', 576), ('w', 96), ('c', 3)])
```


链接 Ops。

`tensor.stack(bw=('b', 'w')).transpose('h', 'bw', 'c')`
![](https://pic4.zhimg.com/v2-5e8f5fd4009183214739e72a5988093b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='576' height='96'></svg>)
这里还有一些 einops 包中有趣的例子。


```
tensor.split(b=('b1', 'b2'), b1=2).stack(a=('b2', 'h'), d=('b1', 'w'))\
      .transpose('a', 'd', 'c')
```

![](https://pic3.zhimg.com/v2-85dafddf920b70de6be1a475e54b37fe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='192' height='288'></svg>)
**建议 5：禁止索引**

一般在命名张量范式中不建议用索引，而是用上面的 index_select 这样的函数。

在 torch 中还有一些有用的命名替代函数。例如 unbind 将维度分解为元组。


```
tensor = NamedTensor(ims, ('b', 'h', 'w', 'c'))

# Returns a tuple
images = tensor.unbind("b")
images[3]
```

![](https://pic4.zhimg.com/v2-74f1be43395b1ffb3bf260bed7ef1f6f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
get 函数直接从命名维度中选择了一个切片。


```
# Returns a tuple
images = tensor.get("b", 0).unbind("c")
images[1]
```

![](https://pic4.zhimg.com/v2-0497876bbd9a0902287269d826966bc7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='96'></svg>)
最后，可以用 narrow 代替花哨的索引。但是你一定要提供一个新的维度名称（因为它不能再广播了）。

`tensor.narrow( 30, 50, h='narowedheight').get("b", 0)`
![](https://pic2.zhimg.com/v2-0dfeaca412b96dfa5a3d272d63383085_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='96' height='50'></svg>)
**建议 6：专用维度**

最后，命名张量尝试直接隐藏不应该被内部函数访问的维度。mask_to 函数会保留左边的掩码，它可以使任何早期的维度不受函数运算的影响。最简单的使用掩码的例子是用来删除 batch 维度的。


```
def bad_function(x, y):
    # Accesses the private batch dimension
    return x.mean("batch")

x = ntorch.randn(dict(batch=10, height=100, width=100))
y = ntorch.randn(dict(batch=10, height=100, width=100))

try:
    bad_function(x.mask_to("batch"), y)
except RuntimeError as e:
    error = "Error received: " + str(e)
error
```





`'Error received: Dimension batch is masked'`

这是弱动态检查，可以通过内部函数关闭。在将来的版本中，也许我们会添加函数注释来 lift 未命名函数，来保留这些属性。

**示例：神经注意力**

为了说明为什么这些选择会带来更好的封装属性，我们来思考一个真实世界中的深度学习例子。这个例子是我的同事 Tim Rocktashel 在一篇介绍 einsum 的博客文章中提出来的。和原始的 PyTorch 相比，Tim 的代码是更好的替代品。虽然我同意 enisum 是一个进步，但它还是存在很多上述陷阱。

下面来看神经注意力的问题，它需要计算，
![](https://pic3.zhimg.com/v2-cb63ffb96e114ee9c2be44047b0ae1aa_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='860' height='174'></svg>)
首先我们要配置参数。


```
def random_ntensors(names, num=1, requires_grad=False):
    tensors = [ntorch.randn(names, requires_grad=requires_grad)
               for i in range(0, num)]
    return tensors[0] if num == 1 else tensors

class Param:
    def __init__(self, in_hid, out_hid):
        torch.manual_seed(0)
        self.WY, self.Wh, self.Wr, self.Wt = \
            random_ntensors(dict(inhid=in_hid, outhid=out_hid),
                            num=4, requires_grad=True)
        self.bM, self.br, self.w = \
            random_ntensors(dict(outhid=out_hid),
                            num=3,
                            requires_grad=True)
```


现在考虑这个函数基于张量的 enisum 实现。


```
# Einsum Implementation
import torch.nn.functional as F
def einsum_attn(params, Y, ht, rt1):
    # -- [batch_size x hidden_dimension]
    tmp = torch.einsum("ik,kl->il", [ht, params.Wh.values]) + \
          torch.einsum("ik,kl->il", [rt1, params.Wr.values])

    Mt = torch.tanh(torch.einsum("ijk,kl->ijl", [Y, params.WY.values]) + \
                tmp.unsqueeze(1).expand_as(Y) + params.bM.values)
    # -- [batch_size x sequence_length]
    at = F.softmax(torch.einsum("ijk,k->ij", [Mt, params.w.values]), dim=-1)

    # -- [batch_size x hidden_dimension]
    rt = torch.einsum("ijk,ij->ik", [Y, at]) + \
         torch.tanh(torch.einsum("ij,jk->ik", [rt1, params.Wt.values]) +
                    params.br.values)

    # -- [batch_size x hidden_dimension], [batch_size x sequence_dimension]
    return rt, at
```


该实现是对原版 PyTorch 实现的改进。它删除了这项工作必需的一些 view 和 transpose。但它仍用了 squeeze，引用了 private batch dim，使用了非强制的注释。

接下来来看 namedtensor 版本：


```
def namedtensor_attn(params, Y, ht, rt1):
    tmp = ht.dot("inhid", params.Wh) + rt1.dot("inhid", params.Wr)
    at = ntorch.tanh(Y.dot("inhid", params.WY) + tmp + params.bM) \
         .dot("outhid", params.w) \
         .softmax("seqlen")

    rt = Y.dot("seqlen", at).stack(inhid=('outhid',)) + \
         ntorch.tanh(rt1.dot("inhid", params.Wt) + params.br)
    return rt, at
```


该代码避免了三个陷阱：
- （陷阱 1）该代码从未提及 batch 维度。
- （陷阱 2）所有广播都是直接用缩并完成的，没有 views。
- （陷阱 3）跨维度的运算是显式的。例如，softmax 明显超过了 seqlen。


```
# Run Einsum
in_hid = 7; out_hid = 7
Y = torch.randn(3, 5, in_hid)
ht, rt1 = torch.randn(3, in_hid), torch.randn(3, in_hid)
params = Param(in_hid, out_hid)
r, a = einsum_attn(params, Y, ht, rt1)
```






```
# Run Named Tensor (hiding batch)
Y = NamedTensor(Y, ("batch", "seqlen", "inhid"), mask=1)
ht = NamedTensor(ht, ("batch", "inhid"), mask=1)
rt1 = NamedTensor(rt1, ("batch", "inhid"), mask=1)
nr, na = namedtensor_attn(params, Y, ht, rt1)
```


**结论/请求帮助**

深度学习工具可以帮助研究人员实现标准模型，但它们也影响了研究人员的尝试。我们可以用现有工具很好地构建模型，但编程实践无法扩展到新模型。（例如，我们最近研究的是离散隐变量模型，它通常有许多针对特定问题的变量，每个变量都有自己的变量维度。这个设置几乎可以立即打破当前的张量范式。）

这篇博文只是这种方法的原型。如果你感兴趣，我很愿意为构建这个库作出贡献。还有一些想法：

扩展到 PyTorch 之外：我们是否可以扩展这种方法，使它支持 NumPy 和 TensorFlow？

与 PyTorch 模块交互：我们是否可以通过类型注释「lift」PyTorch 模块，从而了解它们是如何改变输入的？

错误检查：我们是否可以给提供前置条件和后置条件的函数添加注释，从而自动检查维度？
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[http://nlp.seas.harvard.edu/NamedTensor?fbclid=IwAR2FusFxf-c24whTSiF8B3R2EKz_-zRfF32jpU8D-F5G7rreEn9JiCfMl48](https://link.zhihu.com/?target=http%3A//nlp.seas.harvard.edu/NamedTensor%3Ffbclid%3DIwAR2FusFxf-c24whTSiF8B3R2EKz_-zRfF32jpU8D-F5G7rreEn9JiCfMl48)


