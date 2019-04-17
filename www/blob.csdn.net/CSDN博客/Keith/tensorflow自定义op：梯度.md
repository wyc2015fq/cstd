# tensorflow自定义op：梯度 - Keith - CSDN博客





2017年07月09日 21:39:47[ke1th](https://me.csdn.net/u012436149)阅读数：6268
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# tensorflow自定义op，梯度

> 
tensorflow 是 自动微分的，但是如果你不给它定义微分方程的话，它啥也干不了


在使用 `tensorflow` 的时候，有时不可避免的会需要自定义 `op`，官方文档对于 定义 `op` 的前向过程介绍挺详细，但是对于 梯度 的介绍有点随意。 本文主要介绍在 `python` 端，和在 `c++` 端对 `op` 的梯度进行定义。

## 1.使用python定义op的梯度

**第一个例子：**

```python
from tensorflow.python.framework import ops
from tensorflow.python.ops import array_ops
from tensorflow.python.ops import sparse_ops

@ops.RegisterGradient("ZeroOut")
def _zero_out_grad(op, grad):
  """The gradients for `zero_out`.

  Args:
    op: The `zero_out` `Operation` that we are differentiating, which we can use
      to find the inputs and outputs of the original op.
    grad: Gradient with respect to the output of the `zero_out` op.

  Returns:
    Gradients with respect to the input of `zero_out`.
  """
  to_zero = op.inputs[0]
  shape = array_ops.shape(to_zero)
  index = array_ops.zeros_like(shape)
  first_grad = array_ops.reshape(grad, [-1])[0]
  to_zero_grad = sparse_ops.sparse_to_dense([index], shape, first_grad, 0)
  return [to_zero_grad]  # List of one Tensor, since we have one input
```

这个是官网的示例，从这个例子中，我们可以学到，在 python 中定义 op 的梯度的时候：
- 需要一个 装饰器， `@ops.RegisterGradient("OpName")`
- 梯度函数的签名是 `def _computer_gradient(op, grad)`，第一个用来接收 要计算梯度的 op，第二个用来接收 上一层传回来的梯度。
- 梯度的计算，依旧是 `op` 的组合。

尚不清楚的是：
- 如果 op 有多个输出的话，grad应该怎么处理？
- 梯度计算函数中的 操作 依旧是 `tensorflow` 已有的操作，如果 `tensorflow` 没有想要的操作，应该怎么办？

**第二个例子：**[用 roi 层的实现来作为例子](https://github.com/KeithYin/Faster-RCNN_TFpy2/tree/master/lib/roi_pooling_layer)

```python
@ops.RegisterGradient("RoiPool")
def _roi_pool_grad(op, grad, _):
  """The gradients for `roi_pool`.
  Args:
    op: The `roi_pool` `Operation` that we are differentiating, which we can use
      to find the inputs and outputs of the original op.
    grad: Gradient with respect to the output of the `roi_pool` op.
  Returns:
    Gradients with respect to the input of `RoiPool`.
  """
  data = op.inputs[0]
  rois = op.inputs[1]
  argmax = op.outputs[1]
  pooled_height = op.get_attr('pooled_height')
  pooled_width = op.get_attr('pooled_width')
  spatial_scale = op.get_attr('spatial_scale')

  # compute gradient
  data_grad = roi_pooling_op.roi_pool_grad(data, rois, argmax, grad, pooled_height, pooled_width, spatial_scale)

  return [data_grad, None]  # roi有两个输入，但是第二个输入不需要 梯度
```

这是 `roi` 层 定义 梯度的代码 `roi` 层有两个输入，两个输出。从这里我们可以学到：
- 如果想获取 `op` 的属性，使用 `op.get_attr("attr_name")`
- `op.inputs[i]` 可以获取 `op` 的 第 `i` 个输入。
- `op.outputs[j]`，可以获取 `op` 的第 `i`  个输出。
- `roi` 梯度的核心还是 `roi_pool_grad` 计算的，这个 `op` 不是 `tensorflow`本身自带的，而是后期注册的。这个告诉我们，如果没有合适的 `op` 帮助我们计算 梯度，我们可以 注册一个 `op` ，用这个`op` 计算 梯度。

**关于多个输出的 op **

> 
tensorflow 中到底有没有多输出的 op ， 这个不太清楚，但是我根据官网的 `zero_out` 代码写了一個鬼畜的多输出代码，没有任何实用价值，仅供娱乐


```cpp
#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/op_kernel.h"

#include <cfloat>

#include "third_party/eigen3/unsupported/Eigen/CXX11/Tensor"
#include "tensorflow/core/framework/tensor_shape.h"
#include <iostream>
using namespace std;
using namespace tensorflow;

REGISTER_OP("ZeroOut")
    .Input("to_zero: int32")
    .Output("zeroed: int32")
    .Output("indice: int32")
    ;

class ZeroOutOp : public OpKernel {
 public:
  explicit ZeroOutOp(OpKernelConstruction* context) : OpKernel(context) {
      cout<<"hello, there"<<endl;
    }

  void Compute(OpKernelContext* context) override {
    // Grab the input tensor
    const Tensor& input_tensor = context->input(0);
    auto input = input_tensor.flat<int32>();

    // Create an output tensor
    Tensor* output_tensor = NULL;
    Tensor* output_tensor_indice = NULL;
    TensorShape indice_shape;
    int dims[] = {1};
    TensorShapeUtils::MakeShape(dims, 1, &indice_shape);

    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
    OP_REQUIRES_OK(context, context->allocate_output(1, indice_shape,
                                                     &output_tensor_indice));
    auto output_flat = output_tensor->flat<int32>();
    auto indice_flat = output_tensor_indice->flat<int32>();
    indice_flat(0) = 3;
    // Set all but the first element of the output tensor to 0.
    const int N = input.size();
    for (int i = 1; i < N; i++) {
      output_flat(i) = 0;
    }

    // Preserve the first input value if possible.
    if (N > 0) output_flat(0) = input(0);
  }
};

REGISTER_KERNEL_BUILDER(Name("ZeroOut").Device(DEVICE_CPU), ZeroOutOp);

// 编译命令
// TF_INC=$(python -c 'import tensorflow as tf; print(tf.sysconfig.get_include())')
//  g++ -std=c++11 -shared zero_out.cc -o zero_out.so -fPIC -I $TF_INC -O2 -D_GLIBCXX_USE_CXX11_ABI=0
```

通过这个例子，学到了：
- 对于多输出的 `op` ，`run` 时候返回的不是 `ndarray` ，而是 一个对象 `<class '598f596b9f78ee7154fbfc866bcdc81d.ZeroOut'>`，我们可以通过下标索引来获取 `op` 的 第一，第二个 `outputs`。

**多输出梯度的定义**

> 
当 ZeroOut 有两个输出的时候，下面这个代码会报错，`TypeError: _zero_out_grad() takes 2 positional arguments but 3 were given`， 这就说明，如果 `op` 有 N 个输出的话，那么`gradient` 函数就应该有 `N+1` 个参数，分别代表，`op` 和 各个输出的梯度。


```python
# wrong!!
@ops.RegisterGradient("ZeroOut")
def _zero_out_grad(op, grad):
    to_zero = op.inputs[0]
    shape = array_ops.shape(to_zero)
    index = array_ops.zeros_like(shape)
    first_grad = array_ops.reshape(grad, [-1])[0]
    to_zero_grad = sparse_ops.sparse_to_dense([index], shape, first_grad, 0)
    return [to_zero_grad]

# right!!
@ops.RegisterGradient("ZeroOut")
def _zero_out_grad(op, grad, _):
    to_zero = op.inputs[0]
    shape = array_ops.shape(to_zero)
    index = array_ops.zeros_like(shape)
    first_grad = array_ops.reshape(grad, [-1])[0]
    to_zero_grad = sparse_ops.sparse_to_dense([index], shape, first_grad, 0)
    return [to_zero_grad]
```

## 2. C++端定义op的梯度

> 
c++ 定义 op 的梯度主要使用的 REGISTER_OP_GRADIENT  宏 和 FDH (Function Define Helper)


先从最简单的看起 `ReluGrad`[源码地址](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/core/ops/nn_grad.cc#L52)

```
#include "tensorflow/core/framework/function.h"
#include "tensorflow/core/lib/core/errors.h"
#include "tensorflow/core/util/padding.h"
#include "tensorflow/core/util/tensor_format.h"

namespace tensorflow {

typedef FunctionDefHelper FDH;
Status ReluGrad(const AttrSlice& attrs, FunctionDef* g) {
  // clang-format off
  *g = FDH::Define(
      // Arg defs，定义参数，前向过程的输入 和 输出的梯度
      {"x: T", "dy: T"},
      // Ret val defs，梯度返回值定义
      {"dx: T"},
      // Attr defs，属性定义，
      {{"T: {float, double}"}},
      // Nodes，节点定义：用已有的 op 计算梯度，下面的定义过程就是梯度的计算过程。
      {
        {{"dx"}, "ReluGrad", {"dy", "x"}, {{"T", "$T"}}}
      });
  // clang-format on
  return Status::OK();
}
REGISTER_OP_GRADIENT("Relu", ReluGrad);

}
```

**一些核心 op 的 REGISTER 部分 [github](https://github.com/tensorflow/tensorflow/blob/9fd7cf0547e2f93502c50088879ba30e4e53ad2c/tensorflow/core/ops/nn_ops.cc)**

## 参考资料

[https://zhuanlan.zhihu.com/p/25929909](https://zhuanlan.zhihu.com/p/25929909)



