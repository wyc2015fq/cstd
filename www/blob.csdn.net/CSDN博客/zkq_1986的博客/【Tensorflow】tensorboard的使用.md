# 【Tensorflow】tensorboard的使用 - zkq_1986的博客 - CSDN博客





2018年10月25日 16:03:26[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：50








代码例子：

```python
import tensorflow as tf


class TCNNConfig(object):
    # class TCNNConfig(filename):
    """CNN配置参数"""

    embedding_dim = 8  # 词向量维度
    seq_length = 3  # 序列长度
    num_classes = 2  # 类别数
    num_filters = 1  # 卷积核数目
    kernel_size = 2  # 卷积核尺寸
    vocab_size = 50  # 字典大小
    # vocab_size = 5000       # 字典大小

    hidden_dim = 10  # 全连接层神经元

    dropout_keep_prob = 0.5  # dropout保留比例
    learning_rate = 1e-3  # 学习率

    batch_size = 1  # 每批训练大小
    num_epochs = 1  # 总迭代轮次

    print_per_batch = 10  # 每多少轮输出一次结果
    save_per_batch = 10  # 每多少轮存入tensorboard


class TextCNN(object):
    """文本分类，CNN模型"""

    def __init__(self, config):
        self.config = config

        # 三个待输入的数据
        self.input_x = tf.placeholder(tf.int32, [None, self.config.seq_length], name='input_x')
        self.input_y = tf.placeholder(tf.float32, [None, self.config.num_classes], name='input_y')
        self.keep_prob = tf.placeholder(tf.float32, name='keep_prob')

        self.cnn()

    def cnn(self):
        """CNN模型"""
        # 词向量映射
        with tf.device('/cpu:0'):
            embedding = tf.get_variable('embedding', [self.config.vocab_size, self.config.embedding_dim])
            embedding_inputs = tf.nn.embedding_lookup(embedding, self.input_x)
            # self.embedding_inputs = embedding_inputs

        with tf.name_scope("cnn"):
            # CNN layer
            conv = tf.layers.conv1d(embedding_inputs, self.config.num_filters, self.config.kernel_size, name='conv')
            # self._conv = conv
            # global max pooling layer
            gmp = tf.reduce_max(conv, reduction_indices=[1], name='gmp')

        with tf.name_scope("score"):
            # 全连接层，后面接dropout以及relu激活
            fc = tf.layers.dense(gmp, self.config.hidden_dim, name='fc1')
            fc = tf.contrib.layers.dropout(fc, self.keep_prob)
            fc = tf.nn.relu(fc)

            # 分类器
            self.logits = tf.layers.dense(fc, self.config.num_classes, name='fc2')
            self.y_pred_cls = tf.argmax(tf.nn.softmax(self.logits), 1)  # 预测类别

        with tf.name_scope("optimize"):
            # 损失函数，交叉熵
            cross_entropy = tf.nn.softmax_cross_entropy_with_logits(logits=self.logits, labels=self.input_y)
            self.loss = tf.reduce_mean(cross_entropy)
            # 优化器
            self.optim = tf.train.AdamOptimizer(learning_rate=self.config.learning_rate).minimize(self.loss)

        with tf.name_scope("accuracy"):
            # 准确率
            correct_pred = tf.equal(tf.argmax(self.input_y, 1), self.y_pred_cls)
            self.acc = tf.reduce_mean(tf.cast(correct_pred, tf.float32))


model = TextCNN(TCNNConfig())
import os

tensorboard_dir = 'D:/学习/Tensorflow/dev/test/tensorboard/textcnn'
if not os.path.exists(tensorboard_dir):
    os.makedirs(tensorboard_dir)

tf.summary.scalar("loss", model.loss)
tf.summary.scalar("accuracy", model.acc)
merged_summary = tf.summary.merge_all()
writer = tf.summary.FileWriter(tensorboard_dir)

# 创建session
session = tf.Session()
session.run(tf.global_variables_initializer())
writer.add_graph(session.graph)

for step in range(1):
    session.run(model.loss, feed_dict={model.input_x: [[1, 2, 3]], model.input_y: [[0, 1]], model.keep_prob: 0.5})
```

在anaconda prompt命令行中输入：tensorboard --logdir=D:\学习\Tensorflow\dev\test\tensorboard\textcnn

 C:\Users\zkq>  tensorboard --logdir=D:\学习\Tensorflow\dev\test\tensorboard\textcnn

然后使用chrome浏览器，输入：[http://ip:6006](http://LAPTOP-0O311GUO:6006)

网络结构如下图所示。

![](https://img-blog.csdn.net/20181025160233379?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3prcV8xOTg2/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



