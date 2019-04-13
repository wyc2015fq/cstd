
# pytorch 学习笔记之编写 C 扩展 - 丁丁的博客 - CSDN博客


2017年05月01日 16:53:29[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：2434



> 注，pytorch 0.4 提供了更加方便的工具来添加 C++ 扩展，参见
> [这里](http://pytorch.org/docs/master/cpp_extension.html)
> 及
> [这里](https://github.com/pytorch/extension-cpp)
> 。
pytorch利用[CFFI](http://cffi.readthedocs.io/en/latest/)进行 C 语言扩展。包括两个基本的步骤（[docs](https://github.com/pytorch/tutorials/blob/master/advanced_source/c_extension.rst)）：
编写 C 代码；
python 调用 C 代码，实现相应的 Function 或 Module。
在之前的文章中，我们已经了解了[如何自定义 Module](http://blog.csdn.net/jackytintin/article/details/70233044)。至于 [py]torch 的 C 代码库的结构，我们留待之后讨论； 这里，重点关注，如何在 pytorch C 代码库高层接口的基础上，编写 C 代码，以及如何调用自己编写的 C 代码。
官方示例了如何定义一个加法运算（见[repo](https://github.com/pytorch/extension-ffi)）。这里我们定义ReLU函数（见[repo](https://github.com/DingKe/pytorch_workplace/tree/master/cffi/)）。
# 1. C 代码
pytorch C 的基本数据结构是 THTensor（THFloatTensor、THByteTensor等）。我们以简单的 ReLU 函数为例，示例编写 C 。

$$
y = ReLU(x) = max(x, 0)
$$
Function 需要定义前向和后向两个方向的操作，因此，C 代码要实现相应的功能。
## 1.1 头文件声明
```python
/* ext_lib.h */
int relu_forward(THFloatTensor *input, THFloatTensor *output);
int relu_backward(THFloatTensor *grad_output, THFloatTensor *input, THFloatTensor *grad_input);
```
## 1.2 函数实现
TH/TH.h 包括了 pytorch C 代码数据结构和函数的声明，这是唯一需要添加的 include 依赖。
```python
/* ext_lib.c */
```
```python
#include <TH/TH.h>
```
```python
int relu_forward(THFloatTensor *input, THFloatTensor *output)
{
  THFloatTensor_resizeAs(output, input);
  THFloatTensor_clamp(output, input,
```
```python
0
```
```python
, INFINITY);
```
```python
return
```
```python
1
```
```python
;
}
int relu_backward(THFloatTensor *grad_output, THFloatTensor *input, THFloatTensor *grad_input)
{
  THFloatTensor_resizeAs(grad_input, grad_output);
  THFloatTensor_zero(grad_input);
  THLongStorage* size = THFloatTensor_newSizeOf(grad_output);
  THLongStorage *stride = THFloatTensor_newStrideOf(grad_output);
  THByteTensor *mask = THByteTensor_newWithSize(size, stride);
  THFloatTensor_geValue(mask, input,
```
```python
0
```
```python
);
  THFloatTensor_maskedCopy(grad_input, mask, grad_output);
```
```python
return
```
```python
1
```
```python
;
}
```
# 2. 编译代码
## 2.1 依赖
由于 pytorch 的代码是纯 C 的，因此没有过多的依赖，只需要安装：
* pytorch - 安装方法见[官网](http://pytorch.org/)
* cffi - pip install cffi
编译文件非常简单，主要是添加头文件和实现文件，以及相关的宏定义; 同时文件还指定了编译后的调用位置（此外为_ext.ext_lib）：
```python
# build.py
```
```python
import
```
```python
os
```
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
torch.utils.ffi
```
```python
import
```
```python
create_extension

sources = [
```
```python
'src/ext_lib.c'
```
```python
]
headers = [
```
```python
'src/ext_lib.h'
```
```python
]
defines = []
with_cuda =
```
```python
False
```
```python
if
```
```python
torch.cuda.is_available():
    print(
```
```python
'Including CUDA code.'
```
```python
)
    sources += [
```
```python
'src/ext_lib_cuda.c'
```
```python
]
    headers += [
```
```python
'src/ext_lib_cuda.h'
```
```python
]
    defines += [(
```
```python
'WITH_CUDA'
```
```python
,
```
```python
None
```
```python
)]
    with_cuda =
```
```python
True
```
```python
ffi = create_extension(
```
```python
'_ext.ext_lib'
```
```python
,
    headers=headers,
    sources=sources,
    define_macros=defines,
    relative_to=__file__,
    with_cuda=with_cuda
)
```
```python
if
```
```python
__name__ ==
```
```python
'__main__'
```
```python
:
    ffi.build()
```
```python
python build.py
```
# 3. python 调用
## 3.1 编写配置文件
python 的调用非常简单——pytorch 的 tensor 对象，对应 C 代码的 THTensor 对象，以此作参数进行调用即可。配置文件如下：
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
from
```
```python
_ext
```
```python
import
```
```python
ext_lib
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
:
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
        output = input.new()
```
```python
if
```
```python
not
```
```python
input.is_cuda:
            ext_lib.relu_forward(input, output)
```
```python
else
```
```python
:
```
```python
raise
```
```python
Exception,
```
```python
"No CUDA Implementation"
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
input, = self.saved_tensors
        grad_input = grad_output.new()
```
```python
if
```
```python
not
```
```python
grad_output.is_cuda:
            ext_lib.relu_backward(grad_output, input, grad_input)
```
```python
else
```
```python
:
```
```python
raise
```
```python
Exception,
```
```python
"No CUDA Implementation"
```
```python
return
```
```python
grad_input
```
## 3.2 测试
此处省略 Module 的定义。下面测试下新定义的基于 C 的 ReLU 函数。
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
```
```python
from
```
```python
modules.relu
```
```python
import
```
```python
ReLUM
torch.manual_seed(
```
```python
1111
```
```python
)
```
```python
class
```
```python
MyNetwork
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
(self)
```
```python
:
```
```python
super(MyNetwork, self).__init__()
        self.relu = ReLUM()
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
self.relu(input)
model = MyNetwork()
x = torch.randn(
```
```python
1
```
```python
,
```
```python
25
```
```python
).view(
```
```python
5
```
```python
,
```
```python
5
```
```python
)
input = Variable(x, requires_grad=
```
```python
True
```
```python
)
output = model(input)
print(output)
print(input.clamp(min=
```
```python
0
```
```python
))
output.backward(torch.ones(input.size()))
print(input.grad.data)
```
输出结果如下：
```python
Variable
```
```python
containing:
```
```python
0.8749
```
```python
0.5990
```
```python
0.6844
```
```python
0.0000
```
```python
0.0000
```
```python
0.6516
```
```python
0.0000
```
```python
1.5117
```
```python
0.5734
```
```python
0.0072
```
```python
0.1286
```
```python
1.4171
```
```python
0.0796
```
```python
1.0355
```
```python
0.0000
```
```python
0.0000
```
```python
0.0000
```
```python
0.0312
```
```python
0.0999
```
```python
0.0000
```
```python
1.0401
```
```python
1.0599
```
```python
0.0000
```
```python
0.0000
```
```python
0.0000
```
```python
[torch.FloatTensor
```
```python
of
```
```python
size
```
```python
5
```
```python
x5]
```
```python
Variable
```
```python
containing:
```
```python
0.8749
```
```python
0.5990
```
```python
0.6844
```
```python
0.0000
```
```python
0.0000
```
```python
0.6516
```
```python
0.0000
```
```python
1.5117
```
```python
0.5734
```
```python
0.0072
```
```python
0.1286
```
```python
1.4171
```
```python
0.0796
```
```python
1.0355
```
```python
0.0000
```
```python
0.0000
```
```python
0.0000
```
```python
0.0312
```
```python
0.0999
```
```python
0.0000
```
```python
1.0401
```
```python
1.0599
```
```python
0.0000
```
```python
0.0000
```
```python
0.0000
```
```python
[torch.FloatTensor
```
```python
of
```
```python
size
```
```python
5
```
```python
x5]
```
```python
1
```
```python
1
```
```python
1
```
```python
0
```
```python
0
```
```python
1
```
```python
0
```
```python
1
```
```python
1
```
```python
1
```
```python
1
```
```python
1
```
```python
1
```
```python
1
```
```python
0
```
```python
0
```
```python
0
```
```python
1
```
```python
1
```
```python
0
```
```python
1
```
```python
1
```
```python
0
```
```python
0
```
```python
0
```

