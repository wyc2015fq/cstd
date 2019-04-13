
# 基于 NVRTC 和 CUPY 的 Python CUDA 扩展 - 丁丁的博客 - CSDN博客


2018年01月26日 16:30:20[MoussaTintin](https://me.csdn.net/JackyTintin)阅读数：2283


在之前的[文章](http://blog.csdn.net/jackytintin/article/details/71056076)中，我们探讨了如何通过 cffi，扩展 python （pytorch）。利用 cffi 方法，我们需要单独的 C 和 CUDA 源文件，还需要事先进行编译，不但过程比较繁琐，代码结构也稍显凌乱。对于一些简单的 CUDA 扩展（代码量不大，没有复杂的库依赖），显得不够友好。
这里，我们介绍如何通过在线编译的方式，直接为 pytorch 提供 CUDA 扩展（当然，也可以是纯 C 的扩展）。
> 注： pytorch 0.4+ 提供了新的辅助工具，简化了 C++/CUDA 扩展，参见
> [tutorial](https://pytorch.org/tutorials/advanced/cpp_extension.html)
> 。

# 1. 基本流程
这里， 我们尝试为 python （具体的为 pytorch） 添加 CUDA 扩展。
基本的，我们利用[pynvrtc](https://github.com/NVIDIA/pynvrtc)（NVRTC 的官方python封装） 在线编译 CUDA 代码，利用[cupy](https://github.com/cupy/cupy)（Chainer 的低层计算引擎，我们只利用其封闭 CUDA 函数的功能），来对编译后的 CUDA 代码提供 python 调用接口。完整流程如图1所示。
![这里写图片描述](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)[ ](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
**图1 扩展流程**
[

](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
# 2. 示例
[
](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)做为例子，我们试着用 CUDA 实现[ReLU](https://en.wikipedia.org/wiki/Rectifier_%28neural_networks%29)激活函数。完整代码见[这里](https://github.com/DingKe/pytorch_workplace/tree/master/nvrtc)。
[

](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
$$
ReLU(x) = max(0, x)
$$
[
](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)[ ](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
**图2. ReLU 激活函数【****[src](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)****】**
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
## 2.1 编写 CUDA 代码
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)基于 numpy 的实现如下：
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
```python
def
```
```python
relu_forward
```
```python
(x)
```
```python
:
```
```python
y = x.copy()
    y[x <
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
y
```
```python
def
```
```python
relu_backward
```
```python
(y_grad, x)
```
```python
:
```
```python
x_grad = y_grad.copy()
    x_grad[x <
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
x_grad
```
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)前后向都只涉及 element-wise 的操作，适合于 GPU 并行实现。对应的 CUDA 代码如下：
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
```python
kernel =
```
```python
'''
extern "C"
__global__ void relu_forward(float *output, const float *input, int num)
{
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (; tid < num; tid += stride) {
     output[tid] = input[tid] >= 0 ? input[tid] : 0;
  }
}
extern "C"
__global__ void relu_backward(float *input_grad, const float *output_grad, const float *input, int num)
{
  int tid = blockIdx.x * blockDim.x + threadIdx.x;
  int stride = blockDim.x * gridDim.x;
  for (; tid < num; tid += stride) {
     input_grad[tid] = input[tid] >= 0 ? output_grad[tid] : 0;
  }
}
'''
```
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)这里， 我们将 CUDA 做为 python 的字符串，直接定在文件中，无需单独的 .cu 文件。
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
## 2.2 在线编译 CUDA 源码
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)我们使用 pynvrtc 提供的高层接口来编译上面定义的 CUDA 代码（更多低层接口，详细[官方文档](https://github.com/NVIDIA/pynvrtc)）。。编译过各如下：
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
```python
from
```
```python
pynvrtc.compiler
```
```python
import
```
```python
Program
program = Program(kernel,
```
```python
'relu.cu'
```
```python
)
ptx = program.compile()
```
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)这里， 我们将 CUDA 源码编译为 PTX （GPU上的汇编语言）。这实际运行中，GPU 驱动会负责将 PTX 翻译为机器码进行执行。
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
## 2.3 封装 CUDA 函数
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)为了方便在 python 程序中直接调用，我们需要将 PTX 函数进行封装。这个可以借助 cupy 方便的实现。方法如下：
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
```python
from
```
```python
cupy.cuda
```
```python
import
```
```python
function
m = function.Module()
m.load(bytes(ptx))
self.relu_forward = m.get_function(
```
```python
'relu_forward'
```
```python
)
self.relu_backward = m.get_function(
```
```python
'relu_backward'
```
```python
)
```
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
## 2.4 调用 CUDA 函数
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)已经有了 python 接口，通过传入 GPU 指针，可以进行函数调用。具体方法如下：
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
```python
y = x.new(*x.size())
```
```python
###
```
```python
batch_size, hidden_size = x.size()
num = batch_size * hidden_size
grid_hidden_size = min(num,
```
```python
512
```
```python
)
grid = (int(math.ceil(num / grid_hidden_size)),
```
```python
1
```
```python
)
```
```python
# CUDA syntax: relu_forward<<<grid, block, 0, stream>>>(...)
```
```python
self.relu_forward(grid=grid, block=(grid_hidden_size,
```
```python
1
```
```python
),
                  stream=stream, 
                  args=[y.data_ptr(), x.data_ptr(), num])
```
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)对照 CUDA 调用的语法，可以看到，cupy 的封装将 CUDA 所需参数都以 python 参数的形式进行指定。
[

](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
# 结语
[
](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)可以使用[pycuda](https://github.com/inducer/pycuda)实现 nvrtc + cupy 类似的功能，但 pycuda 社区似乎并不是特别活跃，项目更新也比较慢。
这里没有讨论 CUDA 的 stream 参数, 得到 stream 具体方法可以参见[完整的代码](https://github.com/DingKe/pytorch_workplace/blob/master/nvrtc/relu.py#L52)， 及pytorch 的相关文档。
[            ](https://qph.ec.quoracdn.net/main-qimg-4229dd280e03b7b3a5dc26c808c4b15b)
[
](https://img-blog.csdn.net/20180126121444951?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvSmFja3lUaW50aW4=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
