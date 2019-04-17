# tensorflow：自定义op简单介绍 - Keith - CSDN博客





2017年06月26日 13:32:55[ke1th](https://me.csdn.net/u012436149)阅读数：15345
所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# tensorflow 自定义 op

本文只是简单的翻译了 [https://www.tensorflow.org/extend/adding_an_op](https://www.tensorflow.org/extend/adding_an_op) 的简单部分，高级部分请移步官网。

可能需要新定义 `c++ operation` 的几种情况：
- 现有的 `operation` 组合不出来你想要的 `op`
- 现有的 `operation` 组合 出来的 `operation` 十分低效
- 如果你想要手动融合一些操作。

为了实现你的自定义操作，你需要做一下几件事：
- 在 c++ 文件中注册一个新`op`： `Op registration` 定义了 `op` 的功能接口，它和 `op` 的实现是独立的。例如：`op registration` 定义了 `op` 的名字和 `op`的输出输出。它同时也定义了 `shape` 方法，被用于 `tensor` 的 `shape` 接口。 
- 在 `c++` 中实现 `op`：`op` 的实现称之为 `kernel` ，它是`op` 的一个具体实现。对于不同的输入输出类型或者 架构（CPUs，GPUs）可以有不同的 `kernel` 实现 。
- 创建一个 `python wrapper`（可选的）： 这个 `wrapper` 是一个 公开的 `API`，用来在 `python`中创建 `op`。 `op registration` 会生成一个默认的 `wrapper`，我们可以直接使用或者自己添加一个。
- 写一个计算 `op` 梯度的方法（可选）。
- 测试 `op`：为了方便，我们通常在 `python` 中测试 `op`，但是你也可以在 `c++` 中进行测试。如果你定义了 `gradients`，你可以 通过 `Python` 的  [gradient checker](https://www.tensorflow.org/api_docs/python/tf/test/compute_gradient_error) 验证他们。  这里有个例子[`relu_op_test.py`](https://www.github.com/tensorflow/tensorflow/blob/r1.2/tensorflow/python/kernel_tests/relu_op_test.py) ，测试 `ReLU-like` 的 `op` 的 前向和梯度过程。

## Define the op’s interface

**You define the interface of an op by registering it with the TensorFlow system. **

在注册 `op` 的时候，你需要指定：
- `op` 的名字
- `op` 的输入（名字，类型），`op` 的输出（名字，类型）
- `docstrings`
- `op` 可能需要的 一些  [attrs](https://www.tensorflow.org/extend/adding_an_op#attrs)

**为了演示这个到底怎么工作的，我们来看一个简单的例子：**
- 定义一个 `op` ：输入是一个 `int32` 的 `tensor` ，输出是输入的 拷贝，除了第一个元素保留，其它全都置零。

为了创建这个 `op` 的接口， 我们需要：
- 创建一个文件，名字为  `zero_out.cc`.  然后调用 `REGISTER_OP` 宏，使用这个宏来定义 `op` 的接口 ：

```
#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"

using namespace tensorflow;

REGISTER_OP("ZeroOut")
      .Input("to_zero: int32")
      .Output("zeroed: int32")
      .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
        c->set_output(0, c->input(0));
        return Status::OK();
      });
```

​

这个 `ZeroOut op` 接收一个 `int 32` 的 `tensor` 作为输入，输出同样也是一个 `int32`的 `tensor`。这个 `op` 也使用了一个 `shape` 方法来确保输入和输出的维度是一样的。例如，如果输入的`tensor` 的shape 是 `[10, 20]`，那么，这个 `shape` 方法保证输出的 `shape` 也是 `[10, 20]`。 

> 
注意： op 的名字必须遵循驼峰命名法，而且要保证 op 的名字的唯一性。


## Implement the kernel for the op

当你 定义了 `op` 的接口之后，你可以提供一个或多个 关于`op` 的实现。

为了实现这些 `kernels`：
- 创建一个类，继承 `OpKernel` 类
- 重写 `OpKernel` 类的 `Compute` 方法 
- `Compute` 方法提供了一个 类型为 `OpKernelContext*` 的`context` 参数 ，从这里，我们可以访问到一些有用的信息，比如 输入 和 输出 `tensor`


将 `kernel` 代码也放到 之前创建的 `zero_out.cc` 文件中：

```
#include "tensorflow/core/framework/op_kernel.h"
using namespace tensorflow;

class ZeroOutOp : public OpKernel {
 public:
  explicit ZeroOutOp(OpKernelConstruction* context) : OpKernel(context) {}

  void Compute(OpKernelContext* context) override {
    // 获取输入 tensor
    const Tensor& input_tensor = context->input(0);
    auto input = input_tensor.flat<int32>();

    // 创建输出 tensor, context->allocate_output 用来分配输出内存？
    Tensor* output_tensor = NULL;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
    auto output_flat = output_tensor->flat<int32>();

    // 执行计算操作。
    const int N = input.size();
    for (int i = 1; i < N; i++) {
      output_flat(i) = 0;
    }

    // Preserve the first input value if possible.
    if (N > 0) output_flat(0) = input(0);
  }
};
```

在实现了 `kernel` 之后，就可以将这个注册到 `tensorflow` 系统中去了。在注册时，你需要对 `op` 的运行环境指定一些限制。例如，你可能有一个 `kernel` 代码是给 `CPU` 用的，另一个是给 `GPU`用的。通过把下列代码添加到 `zero_out.cc` 中来完成这个功能：
`REGISTER_KERNEL_BUILDER(Name("ZeroOut").Device(DEVICE_CPU), ZeroOutOp);`
> 
注意：你实现的 `OpKernel` 的实例可能会被并行访问，所以，请确保 `Compute`方法是线程安全的。保证访问 类成员的 方法都加上 mutex。或者更好的选择是，不要通过 类成员来分享 状态。考虑使用 [`ResourceMgr`](https://www.github.com/tensorflow/tensorflow/blob/r1.2/tensorflow/core/framework/resource_mgr.h) 来追踪状态。


### **Multi-threaded CPU kernels**

多线程主要由 `work shard` 搞定。[work shard](http://blog.csdn.net/u012436149/article/details/74890173)

### **GPU kernels**

[请移步官网](https://www.tensorflow.org/extend/adding_an_op)

## Build the op library

**使用系统编译器 编译 定义的 `op`**

我们可以使用 系统上的 `c++` 编译器 `g++` 或者 `clang` 来编译 `zero_out.cc` 。二进制的 `PIP 包` 已经将编译所需的 头文件 和 库 安装到了系统上。`Tensorflow` 的 `python library` 提供了一个用来获取 头文件目录的函数 `get_include`。下面是这个函数在 `ubuntu` 上的输出：

```python
$ python
>>> import tensorflow as tf
>>> tf.sysconfig.get_include()
'/usr/local/lib/python2.7/site-packages/tensorflow/include'
```

假设你已经安装好了 `g++` ，你可以使用 下面一系列的命令 将你的 `op` 编译成一个 动态库。

```
TF_INC=$(python -c 'import tensorflow as tf; print(tf.sysconfig.get_include())')

g++ -std=c++11 -shared zero_out.cc -o zero_out.so -fPIC -I $TF_INC -O2
```

> 
如果你的 g++ 版本>5.0 的话，加上这个参数 `-D_GLIBCXX_USE_CXX11_ABI=0`


## Use the op in Python

`Tensorflow` 的 python 接口提供了 `tf.load_op_library` 函数用来加载动态 `library`，同时将 `op` 注册到`tensorflow` 框架上。`load_op_library` 返回一个 `python module`，它包含了 `op`和 `kernel` 的 `python wrapper` 。因此，一旦你编译好了一个 `op`，就可以使用下列代码通过 `python`来执行它：

```python
import tensorflow as tf
zero_out_module = tf.load_op_library('./zero_out.so')
with tf.Session(''):
  zero_out_module.zero_out([[1, 2], [3, 4]]).eval()

# Prints
array([[1, 0], [0, 0]], dtype=int32)
```

记住：生成的函数的名字是 `snake_case` name。如果在`c++`文件中， `op` 的名字是`ZeroOut`，那么在`python` 中，名字是 `zero_out`。

[完整的代码在文章的最后](#代码)

## Verify that the op works

一个验证你的自定义的`op`是否正确工作的一个好的方法是 为它写一个测试文件。创建一个 `zero_out_op_test.py` 文件，内容为：

```python
import tensorflow as tf

class ZeroOutTest(tf.test.TestCase):
  def testZeroOut(self):
    zero_out_module = tf.load_op_library('./zero_out.so')
    with self.test_session():
      result = zero_out_module.zero_out([5, 4, 3, 2, 1])
      self.assertAllEqual(result.eval(), [5, 0, 0, 0, 0])

if __name__ == "__main__":
  tf.test.main()
```

然后运行这个 `test`

## 代码

```
//zero_out.cc 文件
#include "tensorflow/core/framework/op.h"
#include "tensorflow/core/framework/shape_inference.h"
#include "tensorflow/core/framework/op_kernel.h"
using namespace tensorflow;

REGISTER_OP("ZeroOut")
    .Input("to_zero: int32")
    .Output("zeroed: int32")
    .SetShapeFn([](::tensorflow::shape_inference::InferenceContext* c) {
      c->set_output(0, c->input(0));
      return Status::OK();
    });

class ZeroOutOp : public OpKernel {
 public:
  explicit ZeroOutOp(OpKernelConstruction* context) : OpKernel(context) {}

  void Compute(OpKernelContext* context) override {
    // 将输入 tensor 从 context 中取出。
    const Tensor& input_tensor = context->input(0);
    auto input = input_tensor.flat<int32>();

    // 创建一个 ouput_tensor, 使用 context->allocate_ouput() 给它分配空间。
    Tensor* output_tensor = NULL;
    OP_REQUIRES_OK(context, context->allocate_output(0, input_tensor.shape(),
                                                     &output_tensor));
    auto output_flat = output_tensor->flat<int32>();

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
```

```
#创建动态链接库的命令
g++ -std=c++11 -shared zero_out.cc -o zero_out.so -fPIC -D_GLIBCXX_USE_CXX11_ABI=0 -I $TF_INC -O2
```

## 总结

`tensorflow` 自定义 `op` 的方法可以总结为：
- 写个 `diy_op.cc` 文件
- 用 `g++` 把这个文件编译成动态链接库
- 在 `python` 中使用 `tf.load_op_library`  将库导入。
- 就可以使用了。

还有一种方法是用 `bazel` 编译。

## 参考资料

[https://www.tensorflow.org/extend/adding_an_op](https://www.tensorflow.org/extend/adding_an_op)



