# pytorch扩展 - qq_30006593的博客 - CSDN博客





2017年09月24日 16:47:20[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：137








# python

## __init__.py

导入包时实际导入的是__init__.py，可以通过__init__.py来控制可以使用的包和函数。 

__all__关键字可以方便得用来指定要导入的包。

## ctypes

cffi的wraper

## cffi

# pytorch

## c文件

需要一个h文件

## python文件

### build so

ffi.create_extension和build

### 扩展autograd

需要init forward 和 backward 

tensor和variable的函数基本一样 

function的属性 

Attributes: 

        saved_tensors: Tuple of Tensors that were saved in the call to 

            :func:`forward`. 

        saved_variables: Tuple of Variables that correspond to the tensors 

            saved in the call to :func:`forward`. 

        needs_input_grad: Tuple of booleans of length :attr:`num_inputs`, 

            indicating whether a given input requires gradient. This can be 

            used to optimize buffers saved for backward, and ignoring gradient 

            computation in :func:`~Function.backward`. 

        num_inputs: Number of inputs given to :func:`forward`. 

        num_outputs: Number of tensors returned by :func:`forward`. 

        requires_grad: Boolean indicating whether the :func:`backward` will 

            ever need to be called.
### 扩展nn

#### modules版本

可以保存参数 

需要init（用于初始化参数） forward（调用autograd即可）

#### functional版本




