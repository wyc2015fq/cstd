# tensorflow学习笔记（1）：sess.run() - 站在巨人的肩膀上coding - CSDN博客





2018年04月19日 21:17:38[coding思想](https://me.csdn.net/LOVE1055259415)阅读数：7102








# session.run()

## session.run([fetch1, fetch2])



```
```java
import
```

```java
tensorflow as tf
```



```java
state = tf.Variable(
```

```java
0.0
```

```java
,dtype=tf.float32)
```



```java
one = tf.constant(
```

```java
1.0
```

```java
,dtype=tf.float32)
```



```java
new_val = tf.add(state, one)
```



```java
update = tf.assign(state, new_val)
```



```java
init = tf.initialize_all_variables()
```



```java
with tf.Session() as sess:
```



```java

```

```java
sess.run(init)
```



```java

```

```java
for
```

```java
_ in range(
```

```java
10
```

```java
):
```



```java

```

```java
u,s = sess.run([update,state])
```



```java

```

```java
print s
```
```



在我的电脑上输出是 0.0, 2.0, 3.0, 4.0, 5.0, 5.0, 6.0, 8.0, 9.0, 10.0
是不是很奇怪为什么输出的不是0.0–9.0, 或1.0–10.0
我的猜测是：底层在fectch这两个值的时候，是并行运算的，第一次：程序先fetch “state”的值，然后fetch “update”(导致了assign op)。 第二次，程序先fetch “update”(导致了 assign op)，然后fetch “state” . 所以导致了第一次输出是0.0， 第二次输出是2.0.
注：（update op 更新state的值， 而我在fetch update 的同时也fetch 了state， 只有在这种情况下才会出现上述的问题。 如果我不fetch state ，而去fetch add1的话， 就不会出现上述问题，可见底层是先更新了state的值，才去计算add1。这样的话我们经常使用的代码sess.run([train_op, loss]),获取的loss就是train_op执行完之后的loss了）

## 执行sess.run()时，tensorflow是否计算了整个图

**我们在编写代码的时候，总是要先定义好整个图，然后才调用sess.run()。那么调用sess.run()的时候，程序是否执行了整个图**



```
```java
import
```

```java
tensorflow as tf
```



```java
state = tf.Variable(
```

```java
0.0
```

```java
,dtype=tf.float32)
```



```java
one = tf.constant(
```

```java
1.0
```

```java
,dtype=tf.float32)
```



```java
new_val = tf.add(state, one)
```



```java
update = tf.assign(state, new_val) #返回tensor， 值为new_val
```



```java
update2 = tf.assign(state,
```

```java
10000
```

```java
)  #没有fetch，便没有执行
```



```java
init = tf.initialize_all_variables()
```



```java
with tf.Session() as sess:
```



```java

```

```java
sess.run(init)
```



```java

```

```java
for
```

```java
_ in range(
```

```java
3
```

```java
):
```



```java

```

```java
print sess.run(update)
```
```



和上个程序差不多，但我们这次仅仅是fetch “update”,输出是1.0 , 2.0, 3.0,可以看出，tensorflow并没有计算整个图，只是计算了与想要fetch 的值相关的部分

## sess.run() 中的feed_dict

我们都知道feed_dict的作用是给使用placeholder创建出来的tensor赋值。其实，他的作用更加广泛：feed 使用一个 值(不能是tensor，可以是tensor.eval())临时替换一个 op 的输出结果. 你可以提供 feed 数据作为 run() 调用的参数. feed 只在调用它的方法内有效, 方法结束, feed 就会消失.

```
```java
y = tf.Variable(
```

```java
1
```

```java
)
```



```java
with tf.Session() as sess:
```



```java

```

```java
tf.initialize_all_variables().run()
```



```java

```

```java
print sess.run(y,feed_dict={y:
```

```java
3
```

```java
}) #使用
```

```java
3
```

```java
替换掉tf.Variable(
```

```java
1
```

```java
)的输出结果，所以打印出来
```

```java
3
```



```java

```

```java
print sess.run(y)  #由于feed只在调用他的方法范围内有效，所以这个打印的结果是
```

```java
1
```
```



