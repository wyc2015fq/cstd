# tensorflow学习笔记（四）：激活函数 - Keith - CSDN博客





2016年10月20日 17:15:34[ke1th](https://me.csdn.net/u012436149)阅读数：22852
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









# tesorflow中的激活函数

#### 所有激活函数 输入 和 输出 的维度是一样的

```python
tf.nn.relu()
tf.nn.sigmoid()
tf.nn.tanh()
tf.nn.elu()
tf.nn.bias_add()
tf.nn.crelu()
tf.nn.relu6()
tf.nn.softplus()
tf.nn.softsign()
tf.nn.dropout()
tf.nn.relu_layer(x, weights, biases,name=None)
def relu_layer(x, weights, biases, name=None):
  """Computes Relu(x * weight + biases).
  Args:
    x: a 2D tensor.  Dimensions typically: batch, in_units
    weights: a 2D tensor.  Dimensions typically: in_units, out_units
    biases: a 1D tensor.  Dimensions: out_units
    name: A name for the operation (optional).  If not specified
      "nn_relu_layer" is used.
  Returns:
    A 2-D Tensor computing relu(matmul(x, weights) + biases).
    Dimensions typically: batch, out_units.
  """
```



