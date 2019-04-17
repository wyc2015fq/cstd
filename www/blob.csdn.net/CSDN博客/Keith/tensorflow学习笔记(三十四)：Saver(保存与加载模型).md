# tensorflow学习笔记(三十四)：Saver(保存与加载模型) - Keith - CSDN博客





2017年02月23日 09:41:40[ke1th](https://me.csdn.net/u012436149)阅读数：11071
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# Saver

`tensorflow` 中的 `Saver` 对象是用于 参数保存和恢复的。如何使用呢？ 
[这里](http://blog.csdn.net/u012436149/article/details/52883747)介绍了一些基本的用法。 

官网中给出了这么一个例子：
```python
v1 = tf.Variable(..., name='v1')
v2 = tf.Variable(..., name='v2')

# Pass the variables as a dict:
saver = tf.train.Saver({'v1': v1, 'v2': v2})

# Or pass them as a list.
saver = tf.train.Saver([v1, v2])
# Passing a list is equivalent to passing a dict with the variable op names
# as keys:
saver = tf.train.Saver({v.op.name: v for v in [v1, v2]})

#注意，如果不给Saver传var_list 参数的话， 他将已 所有可以保存的 variable作为其var_list的值。
```

这里使用了三种不同的方式来创建 `saver` 对象， 但是它们内部的原理是一样的。我们都知道，参数会保存到 `checkpoint` 文件中，通过键值对的形式在 `checkpoint`中存放着。如果 `Saver` 的构造函数中传的是 `dict`，那么在 `save` 的时候，`checkpoint`文件中存放的就是对应的 `key-value`。如下：

```python
import tensorflow as tf
# Create some variables.
v1 = tf.Variable(1.0, name="v1")
v2 = tf.Variable(2.0, name="v2")

saver = tf.train.Saver({"variable_1":v1, "variable_2": v2})
# Use the saver object normally after that.
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    saver.save(sess, 'test-ckpt/model-2')
```

我们通过官方提供的工具来看一下 `checkpoint` 中保存了什么

```python
from tensorflow.python.tools.inspect_checkpoint import print_tensors_in_checkpoint_file

print_tensors_in_checkpoint_file("test-ckpt/model-2", None, True)
# 输出:
#tensor_name:  variable_1
#1.0
#tensor_name:  variable_2
#2.0
```

如果构建`saver`对象的时候，我们传入的是 `list`， 那么将会用对应 `Variable` 的 `variable.op.name` 作为 `key`。

```python
import tensorflow as tf
# Create some variables.
v1 = tf.Variable(1.0, name="v1")
v2 = tf.Variable(2.0, name="v2")

saver = tf.train.Saver([v1, v2])
# Use the saver object normally after that.
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    saver.save(sess, 'test-ckpt/model-2')
```

我们再使用官方工具打印出 `checkpoint` 中的数据，得到

```python
tensor_name:  v1
1.0
tensor_name:  v2
2.0
```

。 
**如果我们现在想将 checkpoint 中v2的值restore到v1 中，v1的值restore到v2中，我们该怎么做？**

这时，我们只能采用基于 `dict` 的 `saver`
```python
import tensorflow as tf
# Create some variables.
v1 = tf.Variable(1.0, name="v1")
v2 = tf.Variable(2.0, name="v2")

saver = tf.train.Saver({"variable_1":v1, "variable_2": v2})
# Use the saver object normally after that.
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    saver.save(sess, 'test-ckpt/model-2')
```

`save` 部分的代码如上所示，下面写 `restore` 的代码，和`save`代码有点不同。

```python
```python
import tensorflow as tf
# Create some variables.
v1 = tf.Variable(1.0, name="v1")
v2 = tf.Variable(2.0, name="v2")
#restore的时候，variable_1对应到v2，variable_2对应到v1，就可以实现目的了。
saver = tf.train.Saver({"variable_1":v2, "variable_2": v1})
# Use the saver object normally after that.
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    saver.restore(sess, 'test-ckpt/model-2')
    print(sess.run(v1), sess.run(v2))
# 输出的结果是 2.0 1.0，如我们所望
```

我们发现，其实 创建 `saver`对象时使用的键值对就是表达了一种对应关系：
- save时， 表示：`variable`的值应该保存到 `checkpoint`文件中的哪个 `key`下
- restore时，表示：`checkpoint`文件中`key`对应的值，应该`restore`到哪个`variable`

## 其它

一个快速找到ckpt文件的方式

```python
ckpt = tf.train.get_checkpoint_state(ckpt_dir)
if ckpt and ckpt.model_checkpoint_path:
    saver.restore(sess, ckpt.model_checkpoint_path)
```

## 参考资料

[https://www.tensorflow.org/api_docs/python/tf/train/Saver](https://www.tensorflow.org/api_docs/python/tf/train/Saver)





