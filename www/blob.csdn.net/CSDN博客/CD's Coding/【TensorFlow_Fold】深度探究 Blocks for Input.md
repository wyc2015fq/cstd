# 【TensorFlow_Fold】深度探究 Blocks for Input - CD's Coding - CSDN博客





2017年04月14日 16:30:36[糖果天王](https://me.csdn.net/okcd00)阅读数：1894








## 0x00 前言

想写点东西试试，结果输入就死活搞不定，一万个TypeError； 

决定学跑之前先学爬，老老实实啃一下源码和官方文档，虽然官方还在一点点更新，不少地方还是空白的，不过先动起来多敲点试试看，老等着别人喂饭多不好呀； 

TFF的基本单位之一是Block，看了下Markdown的结构，个人打算按照这个顺序来看看： 

-> [Blocks for input](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md#blocks-for-input)

-> [Blocks for composition](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md#blocks-for-composition)

-> [Blocks for tensors](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md#blocks-for-tensors)

-> [Blocks for sequences](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md#blocks-for-sequences)

-> [Other blocks](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md#other-blocks)

-> [Primitive blocks](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/blocks.md#primitive-blocks)

-> [Block composition](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/blocks.md#block-composition)

虽说……刚列完表就有种想弃坑的冲动，不过毕竟万事开头难嘛，一个一个来，那第一件事就先吃透Blocks4Input好了。
### Reference
- [Blocks for Input](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md#blocks-for-input) 官方文档出处
- [Blocks](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/blocks.md) 的官方解释及示例Markdown
- Source : TensorFlow_Fold / [Python Blocks API](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/g3doc/py/td.md)
- Source Code : Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py)

### About Block_Info()

因为在探究block，所以时不时看看block的类型以及输入输出是必要的，代码中会常使用block_info()函数来查询与验证。 

如果没有特殊说明，block_info()函数指代TFF官方代码里的block描述函数，如下：

```python
# Function for describe blocks
def block_info(block):
    print("%s: %s -&gt; %s" % (block, block.input_type, block.output_type))
```

## 0x01 td.Tensor()

### 说明文档

> 
class td.Tensor 

  A block that converts its input from a python object to a tensor. 

  td.Tensor.**init**(shape, dtype=’float32’, name=None)
### Testing

可以看出，这个Tensor类提供了一种将Python对象转化为Tensor类型的方法， 

总结一下的话：
- `input` : PyObjectType(),
- `output` : TensorType(shape, dtype),
- `name` : name

```
td.Tensor([1])
# => <td.Tensor dtype='float32' shape=(1,)>
td.Tensor([1,2])
# => <td.Tensor dtype='float32' shape=(1,2)>
td.Tensor([1,2,3])
# => <td.Tensor dtype='float32' shape=(1,2,3)>
td.Tensor([3,3],'int32','matx')
# => <td.Tensor 'matx' dtype='int32' shape=(3,3)>
Tblock = td.Tensor([2,2],'int32','matrix')
block_info(Tblock)
# => <td.Tensor 'matrix' dtype='int32' shape=(2,2)>: PyObjectType() -> TensorType((2,2), 'int32')
Tblock.eval([[1,0],[0,1]])
# => array([[1, 0],
            [0, 1], dtype=int32)
```

### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 291

```python
from tensorflow_fold.blocks import result_types as tdt

class Tensor(Block):
  """A block that converts its input from a python object to a tensor."""

  def __init__(self, shape, dtype='float32', name=None):
    super(Tensor, self).__init__(input_type=tdt.PyObjectType(),
                                 output_type=tdt.TensorType(shape, dtype),
                                 name=name)
    self._dtype = np.dtype(self.output_type.dtype)
    if not shape and tf.as_dtype(dtype).is_integer:  # memoize scalar ints
      self._evaluate = self._evaluate_memoized

  def _repr_kwargs(self):
    kwargs = {'dtype': self.output_type.dtype}
    if self._constructor_name == 'td.Vector':
      kwargs['size'] = self.output_type.shape[0]
    elif self._constructor_name != 'td.Scalar':
      kwargs['shape'] = self.output_type.shape
    return kwargs

  def _evaluate_memoized(self, eval_ctx, x):
    array = np.asarray(x, self._dtype)
    return eval_ctx.memoize_constant(self, array.item(), array)

  def _evaluate(self, eval_ctx, x):
    return eval_ctx.constant(np.asarray(x, self._dtype))
```

## 0x02 td.Scalar() & td.Vector()

### 说明文档

> 
td.Scalar(dtype=’float32’, name=None) 

  A block that converts its input to a scalar.

td.Vector(size, dtype=’float32’, name=None) 

  A block that converts its input to a vector.

### Testing

基于Tensor()类，特化出来的两种简便的Block生成形式，Scalar()为标量，Vector(N)为[1,N]的矢量，可以使用这两种方式生成简单的block。
- `input` : PyObjectType(),
- `output` : TensorType(shape, dtype),
- `name` : name

```
# Basic Form: Blocks
scalar_block = td.Scalar()
vector3_block = td.Vector(3)

# TypeError: shape must be non-negative integers
vector34_block = td.Vector([3,4]) # Wrong

block_info(scalar_block)
block_info(vector3_block)
# => <td.scalar dtype='float32'>: PyObjectType() ->; TensorType((), 'float32')
# => <td.vector dtype='float32' size="3">: PyObjectType() ->; TensorType((3,), 'float32')

vector3_block.eval([1,2,3])
# => array([ 1.,  2.,  3.], dtype=float32)
```

### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 318

```python
def Scalar(dtype='float32', name=None):  # pylint: disable=invalid-name
  """A block that converts its input to a scalar."""
  return Tensor(shape=[], dtype=dtype, name=name).set_constructor_name(
      'td.Scalar')


def Vector(size, dtype='float32', name=None):  # pylint: disable=invalid-name
  """A block that converts its input to a vector."""
  return Tensor(shape=[size], dtype=dtype, name=name).set_constructor_name(
      'td.Vector')
```

## 0x03 td.InputTransform()

### 说明文档

> 
class td.InputTransform 

  A Python function, lifted to a block. 

  td.InputTransform.**init**(py_fn, name=None) 

  td.InputTransform.py_fn
### Testing
- `input` & `output` : 由`py_fn`而定

```python
"""
td.InputTransform(fn):
Python Function to Blocks
"""

def func_node(alist):
    return alist[0], {'h':alist[1], 'c':alist[2]}
    # return (alist[0], alist[1], alist[2])

def func_tree(alist):
    return (alist[0], alist[1], alist[2], alist[3])

IT_node = td.InputTransform(func_node)
IT_tree = td.InputTransform(func_tree)
IT_node.eval([1,2,'a']) # => (1, {'c':'a', 'h':2})
IT_tree.eval([1,2,'+',7]) # => (1, 2, '+', 7)

block_info(IT_node)
block_info(IT_tree)
# => <td.inputtransform py_fn='func_node'>: PyObjectType() -> PyObjectType()
# => <td.inputtransform py_fn='func_tree'>: PyObjectType() -> PyObjectType()
```

### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 468

```python
class InputTransform(Block):
  """A Python function, lifted to a block."""

  def __init__(self, py_fn, name=None):
    if not callable(py_fn):
      raise TypeError('py_fn is not callable: %s' % str(py_fn))
    self._py_fn = py_fn
    super(InputTransform, self).__init__(
        [], input_type=tdt.PyObjectType(), output_type=tdt.PyObjectType(),
        name=name)

  def _repr_kwargs(self):
    return dict(py_fn=self.py_fn)

  @property
  def py_fn(self):
    return self._py_fn

  def _evaluate(self, _, x):
    return self._py_fn(x)
```

## 0x04 td.SerializedMessageToTree()

### 说明文档

> 
td.SerializedMessageToTree(message_type_name) 

  A block that turns serialized protobufs into nested Python dicts and lists. 

  The block’s input and output types are both PyObjectType.
### Testing

这是一个可以把序列化的Protobufs转化为嵌套形式字典或列表的Block。 

输入一个字符串，表明你想要的消息类型的全名，返回一个字典，这个字典呢：

> 
of the message’s values by fieldname where the func renders repeated fields as lists 

  学渣：“以字段名称作为消息的值，其中函数将重复字段呈现为列表的形式……？”

submessages via recursion enums as dictionaries whose keys are name, index, and number 

  学渣：“子信息以字典的形式（这个子信息字典的键为name\index\number）递归地枚举出来……？”

这是定语从句八级考试么？！上述翻译是我瞎编的，英语只有57分的我表示不为上述翻译负责（如果有英语Master的读者请务必帮忙翻译一下告诉我，万分感谢QvQ） 

由于比想象中的晦涩难懂，我本来想去`Blocks_test`里看看怎么调用……

```python
from tensorflow_fold.util import test_pb2
def test_serialized_message_to_tree(self):
  block = tdb.SerializedMessageToTree('tensorflow.fold.Nested3')
  self.assertEqual(
      {'foo': 'x', 'nested2': {'bar': 'y', 'nested1': {'baz': None}}},
      block.eval(test_pb2.Nested3(
          foo='x', nested2=test_pb2.Nested2(
              bar='y', nested1=test_pb2.Nested1())).SerializeToString()))
```

看到了`test_pb2.Nested3`,`test_pb2.Nested2`和`test_pb2.Nested1`…… 

这是什么东西？Run一下看看输出？…… 报错ImportError的时候我还是不信的，肯定是我自己哪里错了，但是调了半天，路径层数没问题啊，然后我就将信将疑的去utils里看了一眼……根本没有`test_pb2`呀……QvQ
- `input` : PyObjectType(),
- `output` : PyObjectType(),
- `py_fn` : Message_Type_Name,
- `name` : Message_Type_Name

```
# trBlock = td.SerializedMessageToTree('tensor.flow.proto3')
trBlock = td.SerializedMessageToTree('okcd00.chen.dian')

block_info(trBlock)
<td.SerializedMessageToTree 'okcd00.chen.dian' py_fn='serialized_message_to_tree'>: PyObjectType() -> PyObjectType()

trBlock.eval('1,2,3')
# ValueError: No descriptor for proto:okcd00.chen.dian
# found. Try Importing the .proto file.
```

[x] 存疑：一直因为ValueError无法看到输出结果，大概能理解是做什么用的不过无奈没有proto这个东西无法验证，[util文件夹](https://github.com/tensorflow/fold/tree/master/tensorflow_fold/util%20util%E6%96%87%E4%BB%B6%E5%A4%B9)中的`proto_tools.cc`、`proto_test.py`稍微看了看，感觉上序列化可以理解，dict转化可能也可以brainstorm一下，但是ToTree究竟是什么呢…… 纠结，关于这个先存疑等以后看到再来讨论好了。

BTW：谷歌表示，嗯至少在这方面我能搜到的不比你多。 
![GoogleSearch](https://img-blog.csdn.net/20170414135432370?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvb2tjZDAw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 490

```python
from tensorflow_fold.util import proto_tools

def SerializedMessageToTree(message_type_name):  # pylint: disable=invalid-name 
"""
Args:
  message_type_name: A string; the full name of the expected message type.
Returns:
  A dictionary of the message's values by fieldname, where the function renders repeated fields as lists, submessages via recursion, and enums as dictionaries whose keys are name, index, and number. Missing optional fields are rendered as None. Scalar field values are rendered as themselves.
Raises:
  TypeError: If message_type_name is not a string.
"""
  if not isinstance(message_type_name, six.string_types):
    raise TypeError('message type name must be a string; %s has %s' %
                    (message_type_name, type(message_type_name)))
  return InputTransform(functools.partial(
      proto_tools.serialized_message_to_tree, message_type_name),
                        name=message_type_name).set_constructor_name(
                            'td.SerializedMessageToTree')
```

Github / [proto_test.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/util/proto_test.py) ： Line 75

```python
class ProtoTest(tf.test.TestCase):

  def testSerializedMessageToTree(self):
    example = MakeCyclicProto(
        "some_same<"
        "  many_int32: 1"
        "  many_int32: 2"
        "  some_same<"
        "    many_int32: 3"
        "    many_int32: 4"
        "    some_bool: false"
        "  >"
        ">"
        "some_enum: THAT")
    result = proto_tools.serialized_message_to_tree(
        "tensorflow.fold.CyclicType", example.SerializeToString())
    self.assertEqual(result["some_same"]["many_int32"], [1, 2])
    self.assertEqual(result["some_same"]["some_same"]["many_int32"], [3, 4])
    self.assertEqual(result["some_same"]["some_same"]["some_bool"], False)
    self.assertEqual(result["many_bool"], [])
    self.assertEqual(result["some_bool"], None)
    self.assertEqual(result["some_same"]["many_bool"], [])
    self.assertEqual(result["some_same"]["some_bool"], None)
    self.assertEqual(result["some_enum"]["name"], "THAT")
    self.assertEqual(result["some_enum"]["index"], 1)
    self.assertEqual(result["some_enum"]["number"], 1)

# 限于篇幅，中间略去

  def testNonConsecutiveEnum(self):
    name = "tensorflow.fold.NonConsecutiveEnumMessage"
    msg = test_pb2.NonConsecutiveEnumMessage(
        the_enum=test_pb2.NonConsecutiveEnumMessage.THREE)
    self.assertEqual(
        {"the_enum": {"name": "THREE", "index": 1, "number": 3}},
        proto_tools.serialized_message_to_tree(name, msg.SerializeToString()))
    msg.the_enum = test_pb2.NonConsecutiveEnumMessage.SEVEN
    self.assertEqual(
        {"the_enum": {"name": "SEVEN", "index": 0, "number": 7}},
        proto_tools.serialized_message_to_tree(name, msg.SerializeToString()))
```

## 0x05 td.OneHot()

### 说明文档

> 
class td.OneHot 

  A block that converts PyObject input to a one-hot encoding. 

  Will raise an KeyError if the block is applied to an out-of-range input. 

  td.OneHot.**init**(start, stop=None, dtype=’float32’, name=None)
### Testing

传说中的One-Hot Encoding(好像也有人叫独热或单热编码)。 

提到这个就不得不自来水地吹嘘一下我（们）的好朋友[Licstar](http://licstar.net/)的文章了（Friends in Ingress）

> 
[词向量是什么](http://licstar.net/archives/tag/%E8%AF%8D%E5%90%91%E9%87%8F)

  　　自然语言理解的问题要转化为机器学习的问题，第一步肯定是要找一种方法把这些符号数学化。 

  　　NLP 中最直观，也是到目前为止最常用的词表示方法是 One-hot Representation，这种方法把每个词表示为一个很长的向量。这个向量的维度是词表大小，其中绝大多数元素为 0，只有一个维度的值为 1，这个维度就代表了当前的词。 

  　　举个栗子， 

  　　“话筒”表示为 [0 0 0 1 0 0 0 0 0 0 0 0 0 0 0 0 …] 

  　　“麦克”表示为 [0 0 0 0 0 0 0 0 1 0 0 0 0 0 0 0 …] 

  　　每个词都是茫茫 0 海中的一个 1。 

  　　这种 One-hot Representation 如果采用稀疏方式存储，会是非常的简洁：也就是给每个词分配一个数字 ID。比如刚才的例子中，话筒记为 3，麦克记为 8（假设从 0 开始记）。如果要编程实现的话，用 Hash 表给每个词分配一个编号就可以了。这么简洁的表示方法配合上最大熵、SVM、CRF 等等算法已经很好地完成了 NLP 领域的各种主流任务。 

  　　当然这种表示方法也存在一个重要的问题就是“词汇鸿沟”现象：任意两个词之间都是孤立的。光从这两个向量中看不出两个词是否有关系，哪怕是话筒和麦克这样的同义词也不能幸免于难。
类的初始化定义即宣称了这个具有N个维度的空间，于是这个Block的作用便是： 

对于输入的这个元素X，是空间中的第几个，那么输出的OneHot张量即为一个长度为N的向量，其中第X个位置上（第一个位置即list[0]，第二个位置即list[1]）的值为1。
- `input` : PyObjectType(),
- `output` : TensorType(shape, dtype),
- `name` : name

```python
ohBlock = td.OneHot(5)

block_info(ohBlock)
# <td.OneHot dtype='float32' start=0 stop=5>: PyObjectType() -> TensorType((5,), 'float32')

ohBlock.eval(0) # array([ 1.,  0.,  0.,  0.,  0.], dtype=float32)
ohBlock.eval(3) # array([ 0.,  0.,  0.,  1.,  0.], dtype=float32)

ohBlock2 = td.OneHot(101,105)

block_info(ohBlock2)
# <td.OneHot dtype='float32' start=101 stop=105>: PyObjectType() -> TensorType((4,), 'float32')

ohBlock2.eval(2) # KeyError: 2
ohBlock2.eval(102) # array([ 0.,  1.,  0.,  0.], dtype=float32)
```

### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 1874

```python
class OneHot(Block):
  """A block that converts PyObject input to a one-hot encoding.
  Will raise an `KeyError` if the block is applied to an out-of-range input.
  """

  def __init__(self, start, stop=None, dtype='float32', name=None):
    """Initializes the block.
    Args:
      start: The start of the input range.
      stop: Upper limit (exclusive) on the input range. If stop is `None`, the range is `[0, start)`, like the Python range function.
      dtype: The dtype for the output array.
      name: An optional string name for the block.
    Raises:
      IndexError: If the range is empty.
    """
    if stop:
      n = stop - start
    else:
      n = start
      start = 0
    if n <= 0:
      raise IndexError('range [%d, %d) is empty.' % (start, start + n))
    self._start = start
    super(OneHot, self).__init__(name=name, input_type=tdt.PyObjectType(),
                                 output_type=tdt.TensorType([n], dtype))

  def _repr_kwargs(self):
    return dict(dtype=self.output_type.dtype, start=self._start,
                stop=self._start + self.output_type.shape[0])

  def _compile(self, compiler_ctx):
    array = np.identity(self.output_type.shape[0], self.output_type.dtype)
    self._tensor_names = {
        index: compiler_ctx.register_tensor(tf.constant(row), self.name)
        for index, row in enumerate(array, self._start)}

  def _evaluate(self, eval_ctx, x):
    return eval_ctx.named_tensor(self._tensor_names[x])
```

Github / [blocks_test.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks_test.py) ： Line 855

```python
def test_one_hot(self):
    self.assertBuildsConst([1., 0.], tdb.OneHot(2), 0)
    self.assertBuildsConst([0., 1.], tdb.OneHot(2), 1)
    self.assertBuildsConst([1., 0.], tdb.OneHot(100, 102), 100)
    self.assertBuildsConst([0., 1., 0.], tdb.OneHot(100, 103), 101)
```

## 0x06 td.OneHotFromList()

### 说明文档

> 
td.OneHotFromList(elements, dtype=’float32’, strict=True, name=None) 

  A block that converts PyObject input to a one-hot encoding. 

  Differs from OneHot in that the user specifies the elements covered by the one-hot encoding rather than assuming they are consecutive integers.
### Testing

对于简单的OneHot而言，OneHotFromList则是更加普适了一点。OneHot需要的是一个连续的数组，而OneHotFromList则可以给定一个包含各种元素的List。 

使用OneHotFromList生成的Block接受输入元素X，输出该元素在列表中位置的OneHot表示，需要注意的是，List中不可以存在重复的元素。 

特别地，OneHotFromList具有一个`strict`属性，这个属性决定了当输入元素不存在List中时的反应，若strict为True，则不存在时抛出KeyError，若strict为False，则不存在时返回一个元素全为0的向量。- `input` : PyObjectType(),
- `output` : TensorType(shape, dtype),
- `name` : name

```python
ofBlock = td.OneHotFromList([1,2,'a',4,5]) # strict is default True.

block_info(ofBlock)
# <td.OneHotFromList>: PyObjectType() -> TensorType((5,), 'float32')

ofBlock.eval('a') # array([ 0.,  0.,  1.,  0.,  0.], dtype=float32)
ofBlock.eval(3) # KeyError: 3

ofBlock = td.OneHotFromList([1,2,3,3,5],strict=True)
# AssertionError: OneHotFromList was passed duplicate elements.

ofBlock = td.OneHotFromList([1,2,3,3,5],strict=False)
# AssertionError: OneHotFromList was passed duplicate elements.

ofBlock = td.OneHotFromList([1,2,9,4,5], strict=False)
ofBlock.eval(3) # array([ 0.,  0.,  0.,  0.,  0.], dtype=float32)
```

### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 1918

```python
def OneHotFromList(elements, dtype='float32', strict=True, name=None):  # pylint: disable=invalid-name
  """A block that converts PyObject input to a one-hot encoding.
  Differs from `OneHot` in that the user specifies the elements covered by the one-hot encoding rather than assuming they are consecutive integers.
  Args:
    elements: The list of elements to be given one-hot encodings.
    dtype: The type of the block's return value.
    strict: Whether the block should throw a KeyError if it encounters an input which wasn't in elements.  Default: True.
    name: An optional string name for the block.
  Raises:
    AssertionError: if any of the `elements` given are equal.
  Returns:
    A Block that takes a PyObject and returns a tensor of type `dtype` and shape `[len(elements)]`.  If passed any member of `elements` the block will return a basis vector corresponding to the position of the element in the list.  If passed anything else the block will throw a KeyError if `strict` was set to True, and return the zero vector if `strict` was set to False.
  """
  dimension = len(elements)

  tensors = {}
  for idx, basis_vector in enumerate(np.identity(dimension, dtype)):
    tensors[idx] = FromTensor(basis_vector)

  indices = {elt: idx for idx, elt in enumerate(elements)}
  assert len(indices) == dimension, (
      'OneHotFromList was passed duplicate elements.')

  if strict:
    key_fn = lambda x: indices[x]
  else:
    tensors[-1] = Zeros([dimension, dtype])
    key_fn = lambda x: indices.get(x, -1)

  return OneOf(key_fn, tensors, pre_block=Void(),
               name=name).set_constructor_name('td.OneHotFromList')
```

Github / [blocks_test.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks_test.py) ： Line 861

```
def test_one_hot_from_list(self):
    for strict in [True, False]:
      self.assertBuildsConst([1., 0.], tdb.OneHotFromList(
          [37, 23], strict=strict), 37)
      self.assertBuildsConst([0., 1.], tdb.OneHotFromList(
          [37, 23], strict=strict), 23)
      self.assertBuildsConst([0., 1., 0.], tdb.OneHotFromList(
          [5, 9, 6], strict=strict), 9)

    self.assertRaisesWithLiteralMatch(
        AssertionError, 'OneHotFromList was passed duplicate elements.',
        lambda: tdb.OneHotFromList([1, 3, 3], strict=True))
    self.assertRaisesWithLiteralMatch(
        AssertionError, 'OneHotFromList was passed duplicate elements.',
        lambda: tdb.OneHotFromList([1, 3, 3], strict=False))

    self.assertBuildsConst([0., 0.], tdb.OneHotFromList([37, 23], strict=False), 100)

    strict_one_hot = tdb.OneHotFromList([37, 23], strict=True)
    self.assertRaisesRegexp(
        KeyError, '',
        lambda: self.assertBuildsConst([0., 0.], strict_one_hot, 100))
```

## 0x07 td.Optional()

### 说明文档

> 
class td.Optional 

  Dispatches its input based on whether the input exists, or is None. 

  Similar to OneOf(lambda x: x is None, {True: none_block, False: some_block}) except that none_block has input_type VoidType. 

  td.Optional.**init**(some_case, none_case=None, name=None) 

  Creates an Optional block.
### Testing

简单的来说，Optional这个Block相当于预设了一个默认值，在规定数据类型的情况下，假如输入了该数据类型下的值，则输入什么返回什么（`x if x exists`），但是假如输入的为空，即输入是None，则返回的是初始设置的默认值(`if x is None -- Default/Zero/Empty`)。 

以下述个人测试代码为例，假如不设定默认初始值，那么输入为None的时候就输出全为0.0的同结构数据；设定时，如12.0，在输入数据为None时，输出则是12.0。
- `input` : PyObjectType(),
- `output` : TensorType(shape, dtype) / TupleType(shape, dtype),
- `name` : name

```php
opBlock = td.Optional(td.Scalar(), np.array(12.0, dtype='float32'))
# block_info(opBlock)
# <td.Optional some_case_block=<td.Scalar dtype='float32'>>: PyObjectType() -> TensorType((), 'float32')

opBlock.eval(None) # array(12.0, dtype=float32)
opBlock.eval(34.0) # array(34.0, dtype=float32)

opBlock = td.Optional({'a': td.Map({'b': td.Scalar(), 'c': td.Scalar()}),
                       'd': td.Vector(3)})
block_info(opBlock)
# <td.Optional some_case_block=<td.Record ordered=False>>: 
# PyObjectType() -> TupleType(SequenceType(TupleType(TensorType((), 'float32'), TensorType((), 'float32'))), TensorType((3,), 'float32'))

opBlock.eval(None) # ([], array([ 0.,  0.,  0.], dtype=float32))

opBlock.eval({'a':{'b':1.0, 'c':2.0}, 'd':[3.0, 4.0, 5.0]})
# TypeError: string indices must be integers, not str

opBlock.eval({'a':[{'b':1.0, 'c':2.0}], 'd':[3.0, 4.0, 5.0]})
# ([(array(1.0, dtype=float32), array(2.0, dtype=float32))],
# array([ 3.,  4.,  5.], dtype=float32))
```

不要问我为什么，我也很疑惑啊…… 两个TupleType中间究竟是什么时候混入的这个SequenceType…… 这时候是不是block_info的价值就体现出来啦？ 多用info看看输入输出格式才能构造正确的`feed_dict`哦~

### Source Code

Github / [blocks.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks.py) : Line 1644

```python
class Optional(Block):
  """Dispatches its input based on whether the input exists, or is None.
  Similar to `OneOf(lambda x: x is None, {True: none_block, False: some_block})` except that `none_block` has `input_type` `VoidType`.
  """

  def __init__(self, some_case, none_case=None, name=None):
    """Creates an Optional block.
    Args:
      some_case: The block to evaluate on x if x exists.
      none_case: The block to evaluate if x is None -- defaults to zeros for tensor types, and an empty sequence for sequence types.
      name: An optional string name for the block.
    """
    self._some_case = convert_to_block(some_case)
    self._some_case.set_input_type(tdt.PyObjectType())
    children = [self._some_case]
    if none_case is None:
      self._none_case = None
    else:
      self._none_case = convert_to_block(none_case)
      children.append(self._none_case.set_input_type(tdt.VoidType()))
    super(Optional, self).__init__(
        children=children, input_type=tdt.PyObjectType(), name=name)

  def _repr_kwargs(self):
    return dict(some_case_block=self._some_case)

  def _update_output_type(self):
    self._some_case.set_output_type(self.output_type)
    if self._none_case is None:
      self._none_case = Zeros(self.output_type).set_input_type(tdt.VoidType())
      self._add_child(self._none_case)
    self._none_case.set_output_type(self.output_type)

  def _propagate_types_from_child(self, _):
    self.set_output_type(self._some_case.output_type)
    if self._none_case is not None:
      self.set_output_type(self._none_case.output_type)

  def _evaluate(self, eval_ctx, x):
    # pylint: disable=protected-access
    if x is None: return self._none_case._evaluate(eval_ctx, x)
    return self._some_case._evaluate(eval_ctx, x)
```

Github / [blocks_test.py](https://github.com/tensorflow/fold/blob/master/tensorflow_fold/blocks/blocks_test.py) ： Line 715

```python
def test_optional(self):
    block = tdb.Optional(tdb.Vector(4))
    self.assertBuildsConst([1.0, 2.0, 3.0, 4.0], block, [1, 2, 3, 4])
    self.assertBuildsConst([0.0, 0.0, 0.0, 0.0], block, None)

    block2 = tdb.Optional(tdb.Scalar(), np.array(42.0, dtype='float32'))
    self.assertBuildsConst(6.0, block2, 6)
    self.assertBuildsConst(42.0, block2, None)

  def test_optional_default_none(self):
    block = tdb.Optional({'a': tdb.Map({'b': tdb.Scalar(), 'c': tdb.Scalar()}), 'd': tdb.Vector(3)})
    self.assertBuildsConst(([(0., 1.)], [2., 3., 4.]), block,
                           {'a': [{'b': 0, 'c': 1}], 'd': [2, 3, 4]})
    self.assertBuildsConst(([], [0., 0., 0.]), block, None)

  def test_optional_default_none_type_inference(self):
    child = tdb.Scalar() >> tdb.Function(tf.negative)
    block = tdb.Optional(child)
    self.assertEqual(child.output_type, None)
    child.set_output_type([])
    self.assertEqual(block.output_type, tdt.TensorType([]))
```






















