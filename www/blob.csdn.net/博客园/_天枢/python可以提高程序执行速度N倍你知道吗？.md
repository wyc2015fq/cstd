# python可以提高程序执行速度N倍你知道吗？ - _天枢 - 博客园
## [python可以提高程序执行速度N倍你知道吗？](https://www.cnblogs.com/yhleng/p/9920666.html)
2018-11-07 09:33 by _天枢, ... 阅读, ... 评论, [收藏](#),  [编辑](https://i.cnblogs.com/EditPosts.aspx?postid=9920666)
# 1.1。Numba的约5分钟指南
Numba是Python的即时编译器，它最适用于使用NumPy数组和函数以及循环的代码。使用Numba的最常用方法是通过其装饰器集合，可以应用于您的函数来指示Numba编译它们。当调用Numba修饰函数时，它被编译为机器代码“及时”执行，并且您的全部或部分代码随后可以以本机机器代码速度运行！
开箱即用的Numba使用以下方法：
- 操作系统：Windows（32位和64位），OSX和Linux（32位和64位）
- 架构：x86，x86_64，ppc64le。在armv7l，armv8l（aarch64）上进行实验。
- GPU：Nvidia CUDA。AMD ROC的实验。
- CPython的
- NumPy 1.10 - 最新
## 1.1.1。我怎么得到它？
Numba可作为[畅达](https://conda.io/docs/)包为 [蟒蛇Python发布](https://www.anaconda.com/)：
```
$ conda install numba
```
Numba还有pip可供选择：
```
$ pip install numba
```
Numba也可以 [从源代码编译](http://numba.pydata.org/numba-doc/latest/user/installing.html#numba-source-install-instructions)，虽然我们不建议首次使用Numba用户。
Numba通常用作核心包，因此其依赖性保持在绝对最小值，但是，可以按如下方式安装额外的包以提供其他功能：
- `scipy`- 支持编译`numpy.linalg`功能。
- `colorama` - 支持回溯/错误消息中的颜色突出显示。
- `pyyaml` - 通过YAML配置文件启用Numba配置。
- `icc_rt` - 允许使用Intel SVML（高性能短矢量数学库，仅限x86_64）。安装说明在 [性能提示中](http://numba.pydata.org/numba-doc/latest/user/performance-tips.html#intel-svml)。
## 1.1.2。Numba会为我的代码工作吗？
这取决于你的代码是什么样的，如果你的代码是以数字为导向的（做了很多数学运算），经常使用NumPy和/或有很多循环，那么Numba通常是一个不错的选择。在这些例子中，我们将应用最基本的Numba的JIT装饰器，`@jit`试图加速一些函数来演示哪些有效，哪些无效。
Numba在代码看起来像这样：
```
from numba import jit
import numpy as np
x = np.arange(100).reshape(10, 10)
@jit(nopython=True) # Set "nopython" mode for best performance
def go_fast(a): # Function is compiled to machine code when called the first time
    trace = 0
    for i in range(a.shape[0]):   # Numba likes loops
        trace += np.tanh(a[i, i]) # Numba likes NumPy functions
    return a + trace              # Numba likes NumPy broadcasting
print(go_fast(x))
```
对于看起来像这样的代码，如果有的话，它将无法正常工作：
```
from numba import jit
import pandas as pd
x = {'a': [1, 2, 3], 'b': [20, 30, 40]}
@jit
def use_pandas(a): # Function will not benefit from Numba jit
    df = pd.DataFrame.from_dict(a) # Numba doesn't know about pd.DataFrame
    df += 1                        # Numba doesn't understand what this is
    return df.cov()                # or this!
print(use_pandas(x))
```
请注意，Numba不理解Pandas，因此Numba只是通过解释器运行此代码，但增加了Numba内部开销的成本！
## 1.1.3。什么是`nopython`模式？
Numba `@jit`装饰器从根本上以两种编译模式运行， `nopython`模式和`object`模式。在`go_fast`上面的例子中， `nopython=True`在`@jit`装饰器中设置，这是指示Numba在`nopython`模式下操作。`nopython`编译模式的行为本质上是编译装饰函数，以便它完全运行而不需要Python解释器的参与。这是使用Numba `jit`装饰器的推荐和最佳实践方式，因为它可以带来最佳性能。
如果编译`nopython`模式失败，Numba可以编译使用 ，如果没有设置，这是装饰器的 后退模式（如上例所示）。在这种模式下，Numba将识别它可以编译的循环并将它们编译成在机器代码中运行的函数，并且它将运行解释器中的其余代码。为获得最佳性能，请避免使用此模式`objectmode``@jit``nopython=True``use_pandas`
## 1.1.4。如何衡量Numba的表现？
首先，回想一下，Numba必须为执行函数的机器代码版本之前给出的参数类型编译函数，这需要时间。但是，一旦编译完成，Numba会为所呈现的特定类型的参数缓存函数的机器代码版本。如果再次使用相同的类型调用它，它可以重用缓存的版本而不必再次编译。
测量性能时，一个非常常见的错误是不考虑上述行为，并使用一个简单的计时器来计算一次，该计时器包括在执行时编译函数所花费的时间。
例如：
```
from numba import jit
import numpy as np
import time
x = np.arange(100).reshape(10, 10)
@jit(nopython=True)
def go_fast(a): # Function is compiled and runs in machine code
    trace = 0
    for i in range(a.shape[0]):
        trace += np.tanh(a[i, i])
    return a + trace
# DO NOT REPORT THIS... COMPILATION TIME IS INCLUDED IN THE EXECUTION TIME!
start = time.time()
go_fast(x)
end = time.time()
print("Elapsed (with compilation) = %s" % (end - start))
# NOW THE FUNCTION IS COMPILED, RE-TIME IT EXECUTING FROM CACHE
start = time.time()
go_fast(x)
end = time.time()
print("Elapsed (after compilation) = %s" % (end - start))
```
这，例如打印：
```
Elapsed (with compilation) = 0.33030009269714355
Elapsed (after compilation) = 6.67572021484375e-06
```
衡量Numba JIT对您的代码的影响的一个好方法是使用[timeit](https://docs.python.org/3/library/timeit.html)模块函数来执行时间，这些函数测量多次执行迭代，因此可以在第一次执行时适应编译时间。
作为旁注，如果编译时间成为问题，Numba JIT支持 编译函数[的磁盘缓存](http://numba.pydata.org/numba-doc/latest/reference/jit-compilation.html#jit-decorator-cache)，并且还具有[Ahead-Of-Time](http://numba.pydata.org/numba-doc/latest/reference/aot-compilation.html#aot-compilation)编译模式。
## 1.1.5。它有多快？
假设Numba可以在`nopython`模式下运行，或者至少编译一些循环，它将针对您的特定CPU进行编译。加速因应用而异，但可以是一到两个数量级。Numba有一个 [性能指南](http://numba.pydata.org/numba-doc/latest/user/performance-tips.html#performance-tips)，涵盖了获得额外性能的常用选项。
## 1.1.6。Numba如何运作？
Numba读取装饰函数的Python字节码，并将其与有关函数输入参数类型的信息相结合。它分析并优化您的代码，最后使用LLVM编译器库生成函数的机器代码版本，根据您的CPU功能量身定制。每次调用函数时都会使用此编译版本。
## 1.1.7。其他感兴趣的东西：
Numba有相当多的装饰，我们看到`@jit`和`@njit`，但也有：
- `@vectorize`- 生成NumPy `ufunc`（`ufunc`支持所有方法）。[文件在这里](http://numba.pydata.org/numba-doc/latest/user/vectorize.html#vectorize)。
- `@guvectorize`- 产生NumPy广义`ufunc`s。 [文件在这里](http://numba.pydata.org/numba-doc/latest/user/vectorize.html#guvectorize)。
- `@stencil` - 将函数声明为类似模板操作的内核。 [文件在这里](http://numba.pydata.org/numba-doc/latest/user/stencil.html#numba-stencil)。
- `@jitclass` - 对于jit感知类。[文件在这里](http://numba.pydata.org/numba-doc/latest/user/jitclass.html#jitclass)。
- `@cfunc` - 声明一个函数用作本机回调（从C / C ++等调用）。[文件在这里](http://numba.pydata.org/numba-doc/latest/user/cfunc.html#cfunc)。
- `@overload`- 注册您自己的函数实现，以便在nopython模式下使用，例如`@overload(scipy.special.j0)`。 [文件在这里](http://numba.pydata.org/numba-doc/latest/extending/high-level.html#high-level-extending)。
一些装饰者提供额外选项：
- `parallel = True`- [启用功能](http://numba.pydata.org/numba-doc/latest/reference/jit-compilation.html#jit-decorator-parallel)的 [自动并行](http://numba.pydata.org/numba-doc/latest/user/parallel.html#numba-parallel)化。
- `fastmath = True`- 为该功能启用[快速数学](http://numba.pydata.org/numba-doc/latest/user/jit-decorator-fastmath)行为。
ctypes / cffi / cython互操作性：
- `cffi`- 模式支持调用[CFFI](http://numba.pydata.org/numba-doc/latest/reference/pysupported.html#cffi-support)函数`nopython`。
- `ctypes`- 模式支持调用[ctypes](http://numba.pydata.org/numba-doc/latest/reference/pysupported.html#ctypes-support)包装函数`nopython`。。
- Cython导出的函数[是可调用的](http://numba.pydata.org/numba-doc/latest/extending/high-level.html#cython-support)。
### 1.1.7.1。GPU目标：
Numba可以针对[Nvidia CUDA](https://developer.nvidia.com/cuda-zone)和（实验性）[AMD ROC](https://rocm.github.io/) GPU。您可以使用纯Python编写内核，让Numba处理计算和数据移动（或明确地执行此操作）。单击关于[CUDA](http://numba.pydata.org/numba-doc/latest/cuda/index.html#cuda-index)或[ROC的](http://numba.pydata.org/numba-doc/latest/roc/index.html#roc-index) Numba文档 。
示例：接下来我们写一段简单的代码，来计算一下执行时间：
示例1：不使用numba的:
```
import time
def num():
    arr = []
    for i in range(10000000):
        arr.append(i)
stime = time.time()
num()
etime = time.time() - stime
# print(arr)
print('用时:{}秒'.format(etime))
```
示例输出时间:
```
用时:1.4500024318695068秒
```
示例2：使用numba @jit
```
import time
from numba import jit
@jit
def num():
    arr = []
    for i in range(10000000):
        arr.append(i)
stime = time.time()
num()
etime = time.time() - stime
# print(arr)
print('用时:{}秒'.format(etime))
```
示例输出:
```
用时:0.5530002117156982秒
```
结论：
上述两个示例代码，一个使用了numba，另一个没有使用numba；可以看出使用numba @jit装饰后，时间明显快了很多倍。
这只是一个简单示例；对于复杂计算提高速度更明显。
