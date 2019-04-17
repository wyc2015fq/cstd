# tensorflow学习笔记（五）：变量保存与导入 - Keith - CSDN博客





2016年10月21日 15:26:35[ke1th](https://me.csdn.net/u012436149)阅读数：16938
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









### 如何使用tensorflow内置的参数导出和导入方法：基本用法

#### 如果你还在纠结如何保存tensorflow训练好的模型参数，用这个方法就对了

`The Saver class adds ops to save and restore variables to and from checkpoints. It also provides convenience methods to run these ops.`来自官网的介绍。

```python
import tensorflow as tf
"""
变量声明，运算声明 例：w = tf.get_variable(name="vari_name", shape=[], dtype=tf.float32)
初始化op声明
"""
#创建saver对象，它添加了一些op用来save和restore模型参数
saver = tf.train.Saver()

with tf.Session() as sess:
    sess.run(init_op)
    #训练模型。。。
    #使用saver提供的简便方法去调用 save op
    saver.save(sess, "save_path/file_name.ckpt") #file_name.ckpt如果不存在的话，会自动创建
#后缀可加可不加
```

**现在，训练好的模型参数已经存储好了，我们来看一下怎么调用训练好的参数**
**变量保存的时候，保存的是 变量名：value，键值对。restore的时候，也是根据`key-value`来进行的([详见](http://blog.csdn.net/u012436149/article/details/56665612))**

```python
import tensorflow as tf
"""
变量声明，运算声明
初始化op声明
"""
#创建saver 对象
saver = tf.train.Saver()

with tf.Session() as sess:
    sess.run(init_op)#在这里，可以执行这个语句，也可以不执行，即使执行了，初始化的值也会被restore的值给override
    saver.restore(sess, "save_path/file_name.ckpt-???") 
    #会将已经保存的变量值resotre到变量中,自己看好要restore哪步的
```

## 如何restore变量的子集,然后使用初始化op初始化其他变量

```python
#想要实现这个功能的话,必须从Saver的构造函数下手
saver=tf.train.Saver([sub_set])
init = tf.initialize_all_variables()
with tf.Session() as sess:
  #这样你就可以使用restore的变量替换掉初始化的变量的值,而其它初始化的值不受影响
  sess.run(init)
  if restor_from_checkpoint:
      saver.restore(sess,"file.ckpt")
  # train
  saver.save(sess,"file.ckpt")
```

**参考资料**
[https://www.tensorflow.org/versions/r0.12/how_tos/variables/index.html#saving-and-restoring](https://www.tensorflow.org/versions/r0.12/how_tos/variables/index.html#saving-and-restoring)
[https://www.tensorflow.org/versions/r0.12/api_docs/python/state_ops.html#Saver](https://www.tensorflow.org/versions/r0.12/api_docs/python/state_ops.html#Saver)



