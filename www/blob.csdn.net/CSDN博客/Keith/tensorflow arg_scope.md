# tensorflow: arg_scope - Keith - CSDN博客





2017年06月03日 16:33:23[ke1th](https://me.csdn.net/u012436149)阅读数：7626








# arg_scope

```python
tf.contrib.framework.arg_scope(list_ops_or_scope, **kwargs)
#或者
tf.contrib.slim.arg_scope(list_ops_or_scope, **kwargs)

# 为给定的 list_ops_or_scope 存储默认的参数
```

**示例：**

```python
with slim.arg_scope([slim.conv2d, slim.fully_connected],
                        weights_initializer=tf.truncated_normal_initializer(stddev=0.1),
                        weights_regularizer=slim.l2_regularizer(weight_decay),
                        normalizer_fn=slim.batch_norm,
                        normalizer_params=batch_norm_params):
```

就这样给`slim.conv2d`和`slim.fully_connected`准备了默认参数。

## 如何给自定义的函数也附上这种功能

```python
from tensorflow.contrib import framework
from tensorflow.contrib.framework.python.ops.arg_scope import add_arg_scope

@add_arg_scope
def haha(name, age):
    print(name, age)

with framework.arg_scope([haha], age = 15):
    haha("keith")
# 输出
# keith 15
```

```python
with slim.arg_scope(...) as argScope:
    ...
with slim.arg_scope(argScope):
    ...
# argScope 是一个字典。这个字典可以继续使用，下面的arg_scope配置和上面的是一样的。
```



