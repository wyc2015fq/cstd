# tensorflow错误总结 - Keith - CSDN博客





2017年03月14日 22:55:00[ke1th](https://me.csdn.net/u012436149)阅读数：4668








在写`tensorflow`代码的时候，经常会出现一些错误，在此记录一下，希望不要采同样的坑。

## 错误总结
- 
`bias = tf.get_variable("bias", shape=[out_channels], initializer=tf.zeros_initializer())` 中`tf.zeros_initializer`一定要加括号，不然会报错，因为`tf.zeros_initializer`是个类，加了括号表示创建了这个类的一个对象。

- 
一定要指定好 数据的`dtype`

- 
记录 `global_step`的 `Variable`一定要设置`trainable=False`

- tf.constant() 不能用`Tensor`初始化

## tips
- shape = [] ，创建标量



