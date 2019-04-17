# 完美解决Pytorch在Pycharm没有代码提示的问题 - DumpDoctorWang的博客 - CSDN博客





2019年04月13日 20:28:03[原我归来是少年](https://me.csdn.net/DumpDoctorWang)阅读数：32
个人分类：[Ubuntu软件以及配置](https://blog.csdn.net/DumpDoctorWang/article/category/7825323)









在Pytorch的旧版本中，没有`__init__.pyi`这个文件（Pycharm就从`__init__.pyi`读取函数的声明以及参数类型），因此在Pycharm中torch.sum、torch.abs等函数就没有代码提示。没有代码提示很不方便，经过一番折腾，完美地解决了Pytorch在Pycharm中没有代码提示的问题。
- 把Pytorch的版本升级到1.0.1.post2。

torch.sum、torch.abs等函数应该是有提示了，但是torch.nn、torch.optim等却没有提示了。解决方案如下：
- 
升级Pycharm到版本2019.1.1。

- 
在`site-packages/torch/__init__.pyi`中增加两行代码。

旧的`__init__.pyi`：

```python
from typing import Tuple, Optional, Union
import pickle

class Tensor:
    def abs(self, *, out: Optional['Tensor']=None) -> 'Tensor': ...
    ...
```

新的`__init__.pyi`：

```python
from typing import Tuple, Optional, Union
import pickle
from torch import nn, cuda, ops, functional, optim, autograd, onnx, utils
from torch import contrib, distributions, for_onnx, jit, multiprocessing

class Tensor:
    def abs(self, *, out: Optional['Tensor']=None) -> 'Tensor': ...
	...
```


又遇到了“class Tensor does not define `__sub__`, so the ‘-’ 	cannot be used in its instances”这样的错误，翻译过来就是Tensor类没有定义`__sub__`， ‘-’ 就不能用在实例之间。同样的还有加减乘除大于小于等符号都有类似的提示。于是继续修改`__init__.pyi`

旧的`Tensor`定义：

```python
class Tensor:
    def abs(self, *, out: Optional['Tensor']=None) -> 'Tensor': ...
   	...
```

新的`Tensor`定义：

```python
class Tensor:
    def __add__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __radd__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __sub__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __rsub__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __mul__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __rmul__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __truediv__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __rtruediv__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __ge__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __gt__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __le__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __lt__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __ne__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __eq__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __neg__(self, other: Union['Tensor', 'int', 'float'], out: Optional['Tensor']=None): ...

    def __getitem__(self, item) -> 'Tensor': ...

    def __setitem__(self, key, value) -> 'Tensor': ...

    def __abs__(self, out: Optional['Tensor']=None): ...

    def __ceil__(self, out: Optional['Tensor']=None): ...

    def __floor__(self, out: Optional['Tensor']=None): ...

    def size(self) -> 'torch.Size': ...

    def cpu(self) -> 'Tensor': ...

    def cuda(device:Optional['torch.device']=None, non_blocking: Optional['bool']=False) -> 'Tensor': ...

    def numpy(self) -> 'numpy.ndarray': ...

    def dim(self) -> 'int': ...

    def ndimension(self)  -> 'int': ...

    def double(self) -> 'Tensor': ...

    def float(self) -> 'Tensor': ...

    def half(self) -> 'Tensor': ...

    def int(self) -> 'Tensor': ...

    def long(self) -> 'Tensor': ...

    def short(self) -> 'Tensor': ...

    def abs(self, *, out: Optional['Tensor']=None) -> 'Tensor': ...
    ...
```

就把加减乘除还有`Tensor.size()`函数都给它添加上了。说了这么多，其实就是缺啥定义就在`__init__.pyi`补啥定义。

我修改的`__init__.pyi`可从项目[PytorchResources](https://github.com/Mannix1994/PytorchResources)下载（本博客不在更新）。下载完之后，替换掉`site-packages/torch/`中的`__init__.pyi`就行。



