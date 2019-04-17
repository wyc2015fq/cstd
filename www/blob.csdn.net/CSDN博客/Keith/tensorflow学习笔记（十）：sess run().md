# tensorflow学习笔记（十）：sess.run() - Keith - CSDN博客





2016年10月24日 09:04:08[ke1th](https://me.csdn.net/u012436149)阅读数：78358
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









## session.run([fetch1, fetch2])

关于 session.run([fetch1, fetch2])，请看[http://stackoverflow.com/questions/42407611/how-tensorflow-handle-the-computional-graph-when-executing-sess-run/42408368?noredirect=1#comment71994086_42408368](http://stackoverflow.com/questions/42407611/how-tensorflow-handle-the-computional-graph-when-executing-sess-run/42408368?noredirect=1#comment71994086_42408368)

## 执行sess.run()时，tensorflow是否计算了整个图

**我们在编写代码的时候，总是要先定义好整个图，然后才调用sess.run()。那么调用sess.run()的时候，程序是否执行了整个图**

```python
import tensorflow as tf
state = tf.Variable(0.0,dtype=tf.float32)
one = tf.constant(1.0,dtype=tf.float32)
new_val = tf.add(state, one)
update = tf.assign(state, new_val) #返回tensor， 值为new_val
update2 = tf.assign(state, 10000)  #没有fetch，便没有执行
init = tf.initialize_all_variables()
with tf.Session() as sess:
    sess.run(init)
    for _ in range(3):
        print sess.run(update)
```

**和上个程序差不多，但我们这次仅仅是fetch “update”,输出是1.0  ,  2.0,  3.0,可以看出，tensorflow并没有计算整个图，只是计算了与想要fetch 的值相关的部分**

## sess.run() 中的feed_dict

**我们都知道feed_dict的作用是给使用placeholder创建出来的tensor赋值。其实，他的作用更加广泛：feed 使用一个 值临时替换一个 op 的输出结果. 你可以提供 feed 数据作为 run() 调用的参数. feed 只在调用它的方法内有效, 方法结束, feed 就会消失.**

```python
import tensorflow as tf
y = tf.Variable(1)
b = tf.identity(y)
with tf.Session() as sess:
    tf.global_variables_initializer().run()
    print(sess.run(b,feed_dict={y:3})) #使用3 替换掉
    #tf.Variable(1)的输出结果，所以打印出来3 
    #feed_dict{y.name:3} 和上面写法等价

    print(sess.run(b))  #由于feed只在调用他的方法范围内有效，所以这个打印的结果是 1
```

**输出是3    1 **

参考文献： 

[1] [tensorflow whitepaper](http://download.tensorflow.org/paper/whitepaper2015.pdf)



