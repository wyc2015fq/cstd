# 【Tensorflow】超参调整时对于模型重加载轻量化的测试 - CD's Coding - CSDN博客





2019年01月02日 11:39:55[糖果天王](https://me.csdn.net/okcd00)阅读数：181








## 0x00 前言

由于各类模型在落地使用时都或多或少地需要一些超参的调整（学名调参、俗称炼丹），

但如果每次修改少量超参之后，都要把网络和模型重新初始化一遍，这样就太花时间了，

所以考虑能否尽量减少，甚至可以一次初始化，N个 for 循环的形式来解决超参测试呢？
## 0x01 测试代码

```python
import os
import tensorflow as tf

class test_class(object):
    def __init__(self, options):
        self.sess = self.init_session()
        self.options = options
        
        # init placeholders
        self.x = tf.placeholder(
            tf.float32, [None], name='x')
        
        # init network
        self.output = self.network_ge()
    
    def init_session(dynamic_gpu=False):
        # only use GPU:0
        os.environ['CUDA_VISIBLE_DEVICES'] = '0'
        tf_config = tf.ConfigProto()
        # access GPU capacity on demand
        tf_config.allow_soft_placement = True
        if dynamic_gpu:  # automatically swap to empty GPU
            tf_config.gpu_options.allow_growth = True
        return tf.Session(config=tf_config)
    
    def network_ge(self):
        # y is the hyper-parameter here.
        return tf.greater_equal(
            x=self.x,
            y=self.options.get('a', 0.),
            name='judge_ge')
    
    def network_drop(self):
        return tf.nn.dropout(
            x=self.x,
            keep_prob=self.options.get('a', 0.),
            name='judge_drop')
    
    def update_options(self, options):
        self.options = options
        
    def show(self, x):
        opt = self.options
        print 'option is:', opt
        with tf.name_scope('infer'):
            return self.sess.run(
                # `fetches=self.output` will not work here.
                fetches=self.network_ge(),  # need re-generate network
                feed_dict={self.x: x})
```

## 0x03 测试输出

> 
通过测试我们可以看出，如果使用实现预生成的 output，即不重新生成计算图的情况下

网络是不会因为更新了传入的超参而改变的，需要再 call 一次计算图的生成函数才起效


### 测试用例

```python
import numpy as np
arr = np.random.rand(5)
print arr
tc = test_class({'a': 0.7})
print tc.show(arr)
tc.update_options({'a': 0.3})
print tc.show(arr)
```

### 未重建网络时

```python
# `fetches=self.output` doesn't work here.
[0.27085583 0.47824313 0.03399892 0.79969376 0.22676119]
option is: {'a': 0.7}
[False False False  True False]
option is: {'a': 0.3}
[False False False  True False]
```

### 重建网络时

```python
# `fetches=self.network_ge()` works here
[0.3715132  0.97066691 0.05802148 0.38615892 0.61126987]
option is: {'a': 0.7}
[False  True False False False]
option is: {'a': 0.3}
[ True  True False  True  True]
```

### 超参的修改

> 
此外，options的传入实际是**引用**而非**复制**，

所以直接在实现类外**修改** 也可达到相同的效果。

注意：应该使用 update/append 等方式，不要重实例化

例如 `f_opt = {'a': 0.3}` 就是重开了一个词典并赋值
```python
import numpy as np
arr = np.random.rand(5)
print arr
f_opt = {'a': 0.7}
tc = test_class(f_opt)
print tc.show(arr)
f_opt.update({'a': 0.3})
# But `f_opt = {'a': 0.3}` is Wrong
print tc.show(arr)
tc.update_options(f_opt)
print tc.show(arr)

"""
[0.03253315 0.60373154 0.55212969 0.52420715 0.67525966]
option is: {'a': 0.7}
[False False False False False]
option is: {'a': 0.3}
[False  True  True  True  True]
option is: {'a': 0.3}
[False  True  True  True  True]
"""
```

## 0x04 结论

即便是常数也会被Tensorflow的代码转化为 tf.Constant 写入计算图，

所以在不改动计算图的情况下通过修改传入超参的方式会导致超参的修改不起效果，

故修改超参后按超参所在位置初始化即可，其他部分由于是逐层引用所以更新底层即可，

（测试前绕晕了，测试完……这不是理所当然嘛！QvQ）：- （采用的是 **Network**$\subset$**Model**$\subset$**Api** 的架构）
- 超参在计算图中： **仅初始化计算图** 即可 （如 dropout 超参）
- 超参在计算图外： **仅初始化模型** 即可 （如 clip、greater 超参）
- 超参在计算图内外都有使用： **初始化计算图&模型**（如 mask 超参）







