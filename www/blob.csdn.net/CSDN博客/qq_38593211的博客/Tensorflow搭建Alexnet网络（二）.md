# Tensorflow搭建Alexnet网络（二） - qq_38593211的博客 - CSDN博客





2018年08月12日 17:15:00[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：69标签：[Deeplearning																[Alexnet																[CNN](https://so.csdn.net/so/search/s.do?q=CNN&t=blog)
个人分类：[深度学习](https://blog.csdn.net/qq_38593211/article/category/7811815)





## 1.训练网络

  1.session：完成全部的构建准备、生成全部所需的操作之后，我们就可以创建一个[`tf.Session`](http://www.tensorfly.cn/tfdoc/api_docs/python/client.html#Session)，用于运行图表，也可以利用`with`代码块生成`Session`，限制作用域：with tf.Session() as sess:

  2.生成会话之后，所有`tf.Variable`实例都会立即通过调用各自初始化操作中的[`sess.run()`](http://www.tensorfly.cn/tfdoc/api_docs/python/client.html#Session.run)函数进行初始化。

```python
init = tf.initialize_all_variables()
sess.run(init)
或者：sess.run(tf.global_variables_initializer())
```

  3.完成会话中变量的初始化之后，就可以开始训练了，训练的每一步都是通过用户代码控制，同时获取loss和acrrucy，如果模型在训练中出现偏差，`loss` Tensor的值可能会变成NaN，所以我们要获取它的值，并记录下来。

```python
for step in xrange(max_steps):
    sess.run(train_op)
    feed_dict = fill_feed_dict(data_sets.train,
                               images_placeholder,
                               labels_placeholder)
    _, loss_value = sess.run([train_op, loss],
                             feed_dict=feed_dict)
```

  4.为了得到可以用来后续恢复模型以进一步训练或评估的检查点文件（checkpoint file），可以实例化一个[`tf.train.Saver，`](http://www.tensorfly.cn/tfdoc/api_docs/python/state_ops.html#Saver)在训练循环中，将定期调用[`saver.save()`](http://www.tensorfly.cn/tfdoc/api_docs/python/state_ops.html#Saver.save)方法，向训练文件夹中写入包含了当前所有可训练变量值得检查点文件。

##  2.代码实现

> 
代码中含有一个数据输入的文件，这个需要自己去准备数据，同时如果可视化文件打不开，可以到log目录下，按住shift打开powershell进行操作。


```python
import input_data
import tensorflow as tf
from Alexnet import Network
import numpy as np
import random
import os
class Train:
    def __init__(self):
        self.net = Network()
        self.sess = tf.InteractiveSession()#运行在没有指定会话对象的情况下运行变量。
        self.sess.run(tf.global_variables_initializer())
        self.saver = tf.train.Saver()
        self.writer = tf.summary.FileWriter("F:/PCB/elements/R/r_board", self.sess.graph)
    def train(self):
        for i in range(5):
            ran = random.randint(0, 1358)
            batch1 = input_data.data_x[ran:ran + 1]
            batch2 = input_data.data_y[ran:ran + 1]
            if i % 1 == 0:
                train_accuracy= self.net.accuracy.eval(feed_dict=\
                    {self.net.tf_x: batch1, self.net.tf_y: batch2,self.net.keep_prob: 1.0})
                pred_conv = self.net.y_conv.eval(feed_dict=\
                    {self.net.tf_x: batch1, self.net.tf_y: batch2,self.net.keep_prob: 1.0})
                cross_entropy = self.net.cross_entropy.eval(feed_dict= \
                    {self.net.tf_x: batch1, self.net.tf_y: batch2, self.net.keep_prob: 1.0})
                summary_str = self.net.merged.eval(feed_dict= \
                    {self.net.tf_x: batch1, self.net.tf_y: batch2, self.net.keep_prob: 1.0})
                print( " step=",i,"\n","train_accuracy=",train_accuracy,"\n","cross_entropy=",cross_entropy,"\n","pred_conv=\n",\
                    pred_conv)
                self.save_path = self.saver.save(self.sess, "F:/PCB/elements/model/model.ckpt")
                self.writer.add_summary(summary_str, i)
            if(i < 150):
                self.net.train.run(feed_dict={self.net.tf_x: batch1, self.net.tf_y: batch2, self.net.keep_prob: 1, \
                                              self.net.learning_rate: 0.000001})  # 在训练时，dropout才生效
            if (i > 150):
                self.net.train.run(feed_dict={self.net.tf_x: batch1, self.net.tf_y: batch2, self.net.keep_prob: 1, \
                                              self.net.learning_rate: 0.0000001})  # 在训练时，dropout才生效

if __name__ == "__main__":
    r = Train()
    r.train()
```](https://so.csdn.net/so/search/s.do?q=Alexnet&t=blog)](https://so.csdn.net/so/search/s.do?q=Deeplearning&t=blog)




