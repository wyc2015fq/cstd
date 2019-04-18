# 【Tensorflow】 tf.assign探究 - YZXnuaa的博客 - CSDN博客
2018年03月07日 15:55:24[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：132
个人分类：[TFboy养成记](https://blog.csdn.net/YZXnuaa/article/category/7388667)
### tf.assign(ref, value, validate_shape=None, use_locking=None, name=None)
  Defined in [tensorflow/python/ops/state_ops.py](https://github.com/tensorflow/tensorflow/blob/r1.1/tensorflow/python/ops/state_ops.py).
  将　value 赋值给　ref，并输出 ref　
  这使得需要使用复位值的连续操作变简单
  Defined in [tensorflow/python/framework/tensor_shape.py](https://github.com/tensorflow/tensorflow/blob/r1.1/tensorflow/python/framework/tensor_shape.py).
|Args|Annotations|
|----|----|
|ref|A mutable Tensor. Should be from a Variable node. May be uninitialized.|
|value|A Tensor. Must have the same type as ref. The value to be assigned to the variable.|
|validate_shape|An optional bool. Defaults to True. If true, the operation will validate that the shape of ‘value’ matches the shape of the Tensor being assigned to. If false, ‘ref’ will take on the shape of ‘value’.|
|use_locking|An optional bool. Defaults to True. If True, the assignment will be protected by a lock; otherwise the behavior is undefined, but may exhibit less contention.|
|name|A name for the operation (optional).|
### Returns :
  Same as “ref”. Returned as a convenience for operations that want to use the new value after the variable has been reset.
