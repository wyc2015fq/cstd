# TensorFlow新功能：将Python代码转换为计算图 - 知乎
# 



> 伊瓢 编译自 TensorFlow博客  
量子位 报道 | 公众号 QbitAI
![](https://pic1.zhimg.com/v2-26a6ce3b05d4f6e5d9cc22d286bc87e8_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='2460' height='1440'></svg>)
昨天，TensorFlow推出了一个新功能**「AutoGraph」**，可以将Python代码（包括控制流print()和其他Python原生特性）转换为TensorFlow的计算图（Graph）代码。

在不使用Eager Execution的情况下，编写TensorFlow代码需要进行一些元编程——先编写一个创建计算图的程序，然后稍后执行该程序。这就比较麻烦了，尤其是对新手来说。一些特别棘手的情况涉及更复杂的模型，例如使用if和while，或者是有副作用的模型print()，或接受结构化输入。

## **AutoGraph**

所以为什么要转化成计算图呢？计算图可以做各种优化，例如删除常见的子表达式和内核融合。

而且计算图让分布式训练和部署到各种环境更为容易，因为它们形成了独立于平台的计算模型。这对于多个GPU或TPU上的分布式训练尤为重要，或者通过TensorFlow Lite在移动或物联网等其他平台上分发模型。

下面这个例子你可能想要添加到计算图中： 


```
1def huber_loss(a):
2  if tf.abs(a) <= delta:
3    loss = a * a / 2
4  else:
5    loss = delta * (tf.abs(a) - delta / 2)
6  return loss
```


如果照常使用Eager Execution，它完全可以“正常工作”，但是由于Python解释器开销或者没有进行程序优化，它可能执行的很慢。

所以，计算图执行需要一个前提条件：用类似tf.cond()的结构重写它，就是可能会比较无聊，并且难以实现。

现在，AutoGraph可以自动完成这个转换的过程，这样可以既简单又能获取基于计算图执行的性能优势。

在这个例子中，我们可以用autograph.convert()来装饰函数，AutoGraph将自动生成计算图就绪代码。

使用AutoGraph，原来的这段代码：


```
1@autograph.convert()
2def huber_loss(a):
3  if tf.abs(a) <= delta:
4    loss = a * a / 2
5  else:
6    loss = delta * (tf.abs(a) - delta / 2)
7  return loss
```


在装饰器的作用下变成下面这段：


```
1def tf__huber_loss(a):
 2  with tf.name_scope('huber_loss'):
 3
 4    def if_true():
 5      with tf.name_scope('if_true'):
 6        loss = a * a / 2
 7        return loss,
 8
 9    def if_false():
10      with tf.name_scope('if_false'):
11        loss = delta * (tf.abs(a) - delta / 2)
12        return loss,
13    loss = ag__.utils.run_cond(tf.less_equal(tf.abs(a), delta), if_true,
14        if_false)
15    return loss
```


现在，就可以直接调用代码，就像调用TensorFlow op一样：


```
1with tf.Graph().as_default():  
2  x_tensor = tf.constant(9.0)
3
4  # The converted function works like a regular op: tensors in, tensors out.
5  huber_loss_tensor = huber_loss(x_tensor)
6
7  with tf.Session() as sess:
8    print('TensorFlow result: %2.2f\n' % sess.run(huber_loss_tensor))
```


就这样，AutoGraph填补了eager execution和计算图之间的空白，AutoGraph可以把eager-style的Python代码转换为graph-generating的代码。

AutoGraph不仅仅是一组有用的宏; 它使用源代码转换来Python的任何部分，包括控制流、函数应用程序和赋值、生成样板代码、以及重构常用的Python代码使其容易转换为计算图。

另外，不管使用什么编译器，都需要保证报错信息可读。为此，AutoGraph设置了创建错误消息和堆栈跟踪，可以帮你找到代码中的错误源，而不是仅仅是引用错误代码。

## **可运行的例子**

这里TensorFlow官方展示了一个**用循环和分支检查Collat​​z猜想**的例子，用AutoGraph的 .to_graph()函数将其转换为计算图：


```
1def collatz(a):
 2    counter = 0
 3    while a != 1:
 4        if a % 2 == 0:
 5            a = a // 2
 6        else:
 7            a = 3 * a + 1
 8        counter = counter + 1
 9    return counter
10
11graph_mode_collatz = autograph.to_graph(collatz)
12# The code is human-readable, too
13print(autograph.to_code(collatz))
14
15collatz_tensor = graph_mode_collatz(tf.constant(n))
```


AutoGraph可以支持任意嵌套控制流，例如：


```
1def f(n):
2  if n >= 0:
3    while n < 5:
4      n += 1
5      print(n)
6  return n
```


AutoGraph允许您将元素追加到循环内的数组中，可以通过使用一些AutoGraph助手，比如set_element_type和stack来实现。


```
1def f(n):
 2  z = []
 3  # We ask you to tell us the element dtype of the list
 4  autograph.set_element_type(z, tf.int32)
 5  for i in range(n):
 6    z.append(i)
 7  # when you're done with the list, stack it
 8  # (this is just like np.stack)
 9  return autograph.stack(z) 
10view raw
```


我们还支持像break、continue、print、assert等这些结构，转换后，该部分Python代码中的assert将转换为tf.Assert来表示计算图。


```
1def f(x):
2  assert x != 0, 'Do not pass zero!'
3  return x * x
```


能够轻松地添加循环，控制流程以及更多计算图，意味着可以轻松地将训练循环移动到计算图中。另外的一个例子是采用RNN训练循环并通过一次sess.run()调用执行它。在需要将整个训练循环传递给加速器而不是通过CPU控制器管理训练的情况下，这可能很有用。

如果想看更多的例子，本文文末有TensorFlow官方github例子传送门。

## **转换为计算图 vs Eager Execution**

虽然Eager Execution很有用，但是计算图更快。虽然基准很复杂（并且依赖于程序和硬件本身），但在这个例子（链接：[http://t.cn/RgCsKOe](https://link.zhihu.com/?target=http%3A//t.cn/RgCsKOe)）里，我们看到了从eager execution切换到AutoGraph代码后速度大大提升。

最后，AutoGraph可以让你在GPU或者云端TPU等加速器硬件上使用动态模型或者重度控制流模型，用大数据训练大型模型时这是必须的。

## **AutoGraph和Eager Execution**

在使用eager execution时， 你仍然可以用tf.contrib.eager.defun来把部分代码转换为计算图，需要使用图形TensorFlow ops比如tf.cond()。

将来，AutoGraph将和defun无缝集成，以在eager-style的代码中生成计算图。届时，你可以通过把eager代码转换为计算图片段来使用AutoGraph加速。

## **然鹅……还是试验工具**

虽然AutoGraph看起来很好用，不过TensorFlow官方博客的最后还是说，它还是contrib里的实验工具，不过，官方会尽快将其转移到核心TensorFlow中。

## **传送门**

AutoGraph：[https://github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/autograph](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/tree/master/tensorflow/contrib/autograph)

代码示例：[https://github.com/tensorflow/models/blob/master/samples/core/guide/autograph.ipynb](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/models/blob/master/samples/core/guide/autograph.ipynb)




— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态 


