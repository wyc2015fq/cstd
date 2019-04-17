# 在TensorFlow中实现文本分类的CNN - 博客堂 - CSDN博客





2017年09月06日 09:25:08[最小森林](https://me.csdn.net/u012052268)阅读数：11893








# 在TensorFlow中实现文本分类的CNN


- [在TensorFlow中实现文本分类的CNN](#在tensorflow中实现文本分类的cnn)- [数据和预处理](#1-数据和预处理)
- [模型](#2-模型)
- [实现](#3-实现)- [1 输入占位符](#31-输入占位符)
- [2 向量层](#32-向量层)
- [3 卷积层和池化层](#33-卷积层和池化层)
- [4 Dropout 层](#34-dropout-层)
- [5 得分和预测](#35-得分和预测)
- [6 loss 和 Accuracy](#36-loss-和-accuracy)
- [7 可视化网络](#37-可视化网络)

- [训练过程](#4-训练过程)- [1 实例化CNN并尽可能减少损失](#41-实例化cnn并尽可能减少损失)
- [2 Summaries 摘要](#42-summaries-摘要)
- [3 Checkpointing 检查点](#43-checkpointing-检查点)
- [4 Initializing the variables初始化变量](#44-initializing-the-variables初始化变量)
- [5Defining a single training step定义单一培训步骤](#45defining-a-single-training-step定义单一培训步骤)
- [6 Training loop训练循环](#46-training-loop训练循环)

- [Visualizing Results in TensorBoard 在TensorBoard中可视化结果](#5-visualizing-results-in-tensorboard-在tensorboard中可视化结果)
- [Extensions and Exercises扩展和练习](#6-extensions-and-exercises扩展和练习)




**注意**： 

- 本文翻译自WILDML的博客Implementing a CNN for Text Classification in TensorFlow 

- 博客地址： [http://www.wildml.com/2015/12/implementing-a-cnn-for-text-classification-in-tensorflow/](http://www.wildml.com/2015/12/implementing-a-cnn-for-text-classification-in-tensorflow/)

- github地址：[https://github.com/dennybritz/cnn-text-classification-tf](https://github.com/dennybritz/cnn-text-classification-tf)
在这篇文章中，我们将实现一个类似于Kim Yoon的卷积神经网络语句分类模型。 本文提出的模型在一系列文本分类任务（如情绪分析）中实现了良好的分类性能，并已成为新的文本分类架构的标准基准。

我假设你已经熟悉了应用于NLP的卷积神经网络的基础知识。 如果没有，我建议先阅读NLP的理解卷积神经网络，以获得必要的背景。

## 1 数据和预处理

我们在这篇文章中使用的数据集是Rotten Tomatoes的[Movie Review数据](http://www.cs.cornell.edu/people/pabo/movie-review-data/)，也是原始文件中使用的数据集之一。 数据集包含10,662个实例审查句子，半正负半数。 数据集的大小约为20k。 请注意，由于这个数据集很小，我们很可能会使用强大的模型。 此外，数据集不附带官方列车/测试拆分，因此我们只需将10％的数据用作开发套件。 原始文件报告了对数据进行10倍交叉验证的结果。

我不会在这篇文章中介绍数据预处理代码，但是它可以在Github上使用，并执行以下操作：
- 从原始数据文件中加载正负句子。
- 使用与原始论文相同的代码清理文本数据。
- 将每个句子加到最大句子长度，结果是59.我们将特殊的标记附加到所有其他句子，使其成为59个字。 填充句子相同的长度是有用的，因为它允许我们有效地批量我们的数据，因为批处理中的每个示例必须具有相同的长度。
- 构建词汇索引，并将每个单词映射到0到18,765之间的整数（词汇大小）。 每个句子都成为整数向量。

## 2 模型

我们将在这篇文章中建立的网络大致如下： 
![image](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/11/Screen-Shot-2015-11-06-at-8.03.47-AM.png)

第一层将单词嵌入到低维向量中。 下一层使用多个过滤器大小对嵌入的字矢量进行卷积。 例如，一次滑过3个，4个或5个字。 接下来，我们将卷积层的结果最大化为一个长的特征向量，添加退出正则化，并使用softmax层对结果进行分类。
因为这是一个教育文章，我决定从原始文件中简化模型： 

- 我们不会将预先训练的word2vec矢量用于我们的词嵌入。 相反，我们从头开始学习嵌入。 

- 我们不会对权重向量执行L2规范约束。 （和从业者指南）对句子分类的卷积神经网络的敏感性分析发现，约束对最终结果几乎没有影响。 

- 原始实验用两个输入数据通道 - 静态和非静态字矢量。 我们只使用一个通道。
将这些扩展代码添加到这里是比较简单的（几十行代码）。 看看帖子结尾的练习。

让我们开始吧！

## 3 实现

为了允许各种超参数配置，我们将代码放入一个TextCNN类中，在init函数中生成模型图。

```python
import tensorflow as tf
import numpy as np

class TextCNN(object):
    """
    A CNN for text classification.
    Uses an embedding layer, followed by a convolutional, max-pooling and softmax layer.
    """
    def __init__(
      self, sequence_length, num_classes, vocab_size,
      embedding_size, filter_sizes, num_filters):
        # Implementation...
```

要实例化类，我们传递以下参数：
- sequence_length - 我们的句子的长度。 请记住，我们填充所有句子的长度相同（59为我们的数据集）。
- num_classes - 输出层中的类数，在我们的例子中为正（负）。
- vocab_size - 我们的词汇量。 这需要定义我们的嵌入层的大小，它将具有[vocabulary_size，embedding_size]的形状。
- embedding_size - 我们嵌入的维度。
- filter_sizes - 我们想要卷积过滤器覆盖的字数。 我们将为此处指定的每个大小设置num_filters。 例如，[3,4,5]意味着我们将有一个过滤器，分别滑过3个，4个和5个字，总共有3 * num_filters过滤器。
- num_filters - 每个过滤器大小的过滤器数量（见上文）。

### 3.1 输入占位符

我们首先定义我们传递给我们网络的输入数据：

```
# Placeholders for input, output and dropout
self.input_x = tf.placeholder(tf.int32, [None, sequence_length], name="input_x")
self.input_y = tf.placeholder(tf.float32, [None, num_classes], name="input_y")
self.dropout_keep_prob = tf.placeholder(tf.float32, name="dropout_keep_prob")
```

tf.placeholder创建一个占位符变量，当我们在火车或测试时间执行它时，我们馈送到网络。 第二个参数是输入张量的形状。 无意味着该维度的长度可以是任何东西。 在我们的情况下，第一个维度是批量大小，并且使用“无”允许网络处理任意大小的批次。

将神经元保留在丢失层中的概率也是网络的输入，因为我们仅在训练期间启用退出。 我们在评估模型时禁用它（稍后再说）。

### 3.2 向量层

我们定义的第一层是嵌入层，它将词汇词索引映射到低维向量表示中。 它本质上是一个从数据中学习的查找表。

```
with tf.device('/cpu:0'), tf.name_scope("embedding"):
    W = tf.Variable(
        tf.random_uniform([vocab_size, embedding_size], -1.0, 1.0),
        name="W")
    self.embedded_chars = tf.nn.embedding_lookup(W, self.input_x)
    self.embedded_chars_expanded = tf.expand_dims(self.embedded_chars, -1)
```

我们在这里使用了几个新功能，让我们来看看：
- tf.device（“/ cpu：0”）强制在CPU上执行操作。 默认情况下，TensorFlow将尝试将操作放在GPU上（如果有的话）可用，但是嵌入式实现当前没有GPU支持，并且如果放置在GPU上则会抛出错误。
- tf.name_scope创建一个名称范围，名称为“embedding”。 范围将所有操作添加到名为“嵌入”的顶级节点中，以便在TensorBoard中可视化您的网络时获得良好的层次结构。

W是我们在训练中学习的嵌入矩阵。 我们使用随机均匀分布来初始化它。 tf.nn.embedding_lookup创建实际的嵌入操作。 嵌入操作的结果是形状为[None，sequence_length，embedding_size]的三维张量。

TensorFlow的卷积conv2d操作期望具有对应于批次，宽度，高度和通道的尺寸的四维张量。 我们嵌入的结果不包含通道尺寸，所以我们手动添加它们，留下一层形状[None，sequence_length，embedding_size，1]。

### 3.3 卷积层和池化层

现在我们已经准备好构建卷积层，然后再进行最大化。 请记住，我们使用不同大小的过滤器。 因为每个卷积产生不同形状的张量，我们需要迭代它们，为它们中的每一个创建一个层，然后将结果合并成一个大的特征向量。

```
pooled_outputs = []
for i, filter_size in enumerate(filter_sizes):
    with tf.name_scope("conv-maxpool-%s" % filter_size):
        # Convolution Layer
        filter_shape = [filter_size, embedding_size, 1, num_filters]
        W = tf.Variable(tf.truncated_normal(filter_shape, stddev=0.1), name="W")
        b = tf.Variable(tf.constant(0.1, shape=[num_filters]), name="b")
        conv = tf.nn.conv2d(
            self.embedded_chars_expanded,
            W,
            strides=[1, 1, 1, 1],
            padding="VALID",
            name="conv")
        # Apply nonlinearity
        h = tf.nn.relu(tf.nn.bias_add(conv, b), name="relu")
        # Max-pooling over the outputs
        pooled = tf.nn.max_pool(
            h,
            ksize=[1, sequence_length - filter_size + 1, 1, 1],
            strides=[1, 1, 1, 1],
            padding='VALID',
            name="pool")
        pooled_outputs.append(pooled)

# Combine all the pooled features
num_filters_total = num_filters * len(filter_sizes)
self.h_pool = tf.concat(3, pooled_outputs)
self.h_pool_flat = tf.reshape(self.h_pool, [-1, num_filters_total])
```

这里，W是我们的滤波器矩阵，h是将非线性应用于卷积输出的结果。 每个过滤器在整个嵌入中滑过，但是它涵盖的字数有所不同。 “VALID”填充意味着我们将过滤器滑过我们的句子而不填充边缘，执行一个窄的卷积，给出一个形状[1，sequence_length - filter_size + 1,1,1]的输出。 在特定过滤器大小的输出上执行最大化池将留下一张张量[batch_size，1，num_filters]。 这本质上是一个特征向量，其中最后一个维度对应于我们的特征。 一旦我们从每个过滤器大小得到所有的汇集输出张量，我们将它们组合成一个长形特征向量[batch_size，num_filters_total]。 在tf.reshape中使用-1可以告诉TensorFlow在可能的情况下平坦化维度。

花一些时间，尝试了解每个操作的输出形状。 您还可以参考NLP的理解卷积神经网络来获得一些直觉。 可视化TensorBoard中的操作也可以帮助（对于特定的过滤器大小3,4和5）

![image](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/12/Screen-Shot-2015-12-10-at-10.13.50-AM1-1024x525.png)

![image](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/12/Screen-Shot-2015-12-10-at-10.22.29-AM.png)

### 3.4 Dropout 层

花一些时间，尝试了解每个操作的输出形状。 您还可以参考NLP的理解卷积神经网络来获得一些直觉。 可视化TensorBoard中的操作也可以帮助（对于特定的过滤器大小3,4和5）……

```
# Add dropout
with tf.name_scope("dropout"):
    self.h_drop = tf.nn.dropout(self.h_pool_flat, self.dropout_keep_prob)
```

### 3.5 得分和预测

使用max-pooling中的特征向量（使用退出），我们可以通过执行矩阵乘法并选择具有最高分数的类来生成预测。 我们还可以应用softmax函数将原始分数转换为归一化概率，但这不会改变我们的最终预测。

```
with tf.name_scope("output"):
    W = tf.Variable(tf.truncated_normal([num_filters_total, num_classes], stddev=0.1), name="W")
    b = tf.Variable(tf.constant(0.1, shape=[num_classes]), name="b")
    self.scores = tf.nn.xw_plus_b(self.h_drop, W, b, name="scores")
    self.predictions = tf.argmax(self.scores, 1, name="predictions")
```

这里，tf.nn.xw_plus_b是执行Wx + b矩阵乘法的便利包装器。

### 3.6 loss 和 Accuracy

使用我们的分数我们可以定义损失函数。 损失是我们网络造成的错误的衡量标准，我们的目标是尽量减少网络的误差。 分类问题的标准损失函数是交叉熵损失。

```
# Calculate mean cross-entropy loss
with tf.name_scope("loss"):
    losses = tf.nn.softmax_cross_entropy_with_logits(self.scores, self.input_y)
    self.loss = tf.reduce_mean(losses)
```

这里，tf.nn.softmax_cross_entropy_with_logits是一个方便的函数，计算每个类的交叉熵损失，给定我们的分数和正确的输入标签。 然后我们把损失的平均值。

我们也可以使用这个总和，但这比较难以比较不同批量大小和列车/开发数据的损失。

```
# Calculate Accuracy
with tf.name_scope("accuracy"):
    correct_predictions = tf.equal(self.predictions, tf.argmax(self.input_y, 1))
    self.accuracy = tf.reduce_mean(tf.cast(correct_predictions, "float"), name="accuracy")
```

### 3.7 可视化网络

就这样，我们完成了我们的网络定义。 完整的代码网络定义代码在这里可用。 为了获得大图，我们还可以在TensorBoard中可视化网络：![image](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/12/Screen-Shot-2015-12-10-at-10.25.46-AM.png)

## 4 训练过程

在我们为网络定义培训程序之前，我们需要了解一些关于TensorFlow如何使用会话和图形的基础知识。 如果您已经熟悉这些概念，请随时跳过本节。

在TensorFlow中，会话是您正在执行图形操作的环境，它包含有关变量和队列的状态。 每个会话都在单个图形上运行。 如果在创建变量和操作时未明确使用会话，则使用TensorFlow创建的当前默认会话。 您可以通过执行session.as_default（）块中的命令来更改默认会话（见下文）。

图形包含操作和张量。 您可以在程序中使用多个图形，但大多数程序只需要一个图形。 您可以在多个会话中使用相同的图表，但在一个会话中不能使用多个图表。 TensorFlow始终创建一个默认图形，但您也可以手动创建一个图形，并将其设置为新的默认图像，如下所示。 显式创建会话和图表可确保在不再需要资源时正确释放资源。

```
with tf.Graph().as_default():
    session_conf = tf.ConfigProto(
      allow_soft_placement=FLAGS.allow_soft_placement,
      log_device_placement=FLAGS.log_device_placement)
    sess = tf.Session(config=session_conf)
    with sess.as_default():
        # Code that operates on the default graph and session comes here...
```

allow_soft_placement设置允许TensorFlow在首选设备不存在的情况下实现特定操作的设备上回退。 例如，如果我们的代码在GPU上执行操作，并且我们在没有GPU的机器上运行代码，则不使用allow_soft_placement将导致错误。 如果设置了log_device_placement，TensorFlow会登录哪些设备（CPU或GPU）进行操作。 这对调试非常有用。 标记是我们程序的命令行参数。

### 4.1 实例化CNN并尽可能减少损失

当我们实例化我们的TextCNN模型时，所有定义的变量和操作将被放置在上面创建的默认图形和会话中。

```
cnn = TextCNN(
    sequence_length=x_train.shape[1],
    num_classes=2,
    vocab_size=len(vocabulary),
    embedding_size=FLAGS.embedding_dim,
    filter_sizes=map(int, FLAGS.filter_sizes.split(",")),
    num_filters=FLAGS.num_filters)
```

接下来，我们定义如何优化网络损耗功能。 TensorFlow有几个内置优化器。 我们正在使用Adam优化器。

```
global_step = tf.Variable(0, name="global_step", trainable=False)
optimizer = tf.train.AdamOptimizer(1e-4)
grads_and_vars = optimizer.compute_gradients(cnn.loss)
train_op = optimizer.apply_gradients(grads_and_vars, global_step=global_step)
```

在这里，train_op这里是一个新创建的操作，我们可以运行它来对我们的参数执行渐变更新。 train_op的每次执行都是一个训练步骤。 TensorFlow自动计算哪些变量是“可训练的”并计算它们的梯度。 通过定义一个global_step变量并将其传递给优化器，我们允许TensorFlow处理对我们的培训步骤的计数。 每次执行train_op时，全局步骤将自动递增1。

### 4.2 Summaries 摘要

TensorFlow有一个Summaries，可以让您在培训和评估过程中跟踪和查看各种数量。 例如，您可能想要跟踪您的损失和准确性随时间的变化。 您还可以跟踪更复杂的数量，例如图层激活的直方图。 汇总是序列化的对象，它们使用SummaryWriter写入磁盘。

```
# Output directory for models and summaries
timestamp = str(int(time.time()))
out_dir = os.path.abspath(os.path.join(os.path.curdir, "runs", timestamp))
print("Writing to {}\n".format(out_dir))

# Summaries for loss and accuracy
loss_summary = tf.scalar_summary("loss", cnn.loss)
acc_summary = tf.scalar_summary("accuracy", cnn.accuracy)

# Train Summaries
train_summary_op = tf.merge_summary([loss_summary, acc_summary])
train_summary_dir = os.path.join(out_dir, "summaries", "train")
train_summary_writer = tf.train.SummaryWriter(train_summary_dir, sess.graph_def)

# Dev summaries
dev_summary_op = tf.merge_summary([loss_summary, acc_summary])
dev_summary_dir = os.path.join(out_dir, "summaries", "dev")
dev_summary_writer = tf.train.SummaryWriter(dev_summary_dir, sess.graph_def)
```

在这里，我们分别跟踪培训和评估的总结。 在我们的情况下，这些数量是相同的，但您可能只有在培训期间跟踪的数量（如参数更新值）。 tf.merge_summary是将多个摘要操作合并到可以执行的单个操作中的便利函数。

### 4.3 Checkpointing 检查点

您通常想要使用的另一个TensorFlow功能是检查点 - 保存模型的参数以便稍后恢复。 检查点可用于稍后继续训练，或使用提前停止选择最佳参数设置。 使用Saver对象创建检查点。

```
# Checkpointing
checkpoint_dir = os.path.abspath(os.path.join(out_dir, "checkpoints"))
checkpoint_prefix = os.path.join(checkpoint_dir, "model")
# Tensorflow assumes this directory already exists so we need to create it
if not os.path.exists(checkpoint_dir):
    os.makedirs(checkpoint_dir)
saver = tf.train.Saver(tf.all_variables())
```

### 4.4 Initializing the variables初始化变量

在我们可以训练我们的模型之前，我们还需要在图中初始化变量。
`sess.run(tf.initialize_all_variables())`
initialize_all_variables函数是一个方便的函数，运行我们为变量定义的所有初始化器。 您也可以手动调用变量的初始值。 如果您希望使用预先训练的值初始化嵌入，这很有用。

### 4.5Defining a single training step定义单一培训步骤

现在我们来定义一个单一的训练步骤的功能，评估一批数据上的模型和更新模型参数。

```python
def train_step(x_batch, y_batch):
    """
    A single training step
    """
    feed_dict = {
      cnn.input_x: x_batch,
      cnn.input_y: y_batch,
      cnn.dropout_keep_prob: FLAGS.dropout_keep_prob
    }
    _, step, summaries, loss, accuracy = sess.run(
        [train_op, global_step, train_summary_op, cnn.loss, cnn.accuracy],
        feed_dict)
    time_str = datetime.datetime.now().isoformat()
    print("{}: step {}, loss {:g}, acc {:g}".format(time_str, step, loss, accuracy))
    train_summary_writer.add_summary(summaries, step)
```

feed_dict包含我们传递到我们网络的占位符节点的数据。您必须为所有占位符节点提供值，否则TensorFlow将抛出错误。使用输入数据的另一种方法是使用队列，但这超出了这篇文章的范围。

接下来，我们使用session.run执行我们的train_op，它返回我们要求它进行评估的所有操作的值。请注意，train_op什么都不返回，它只是更新我们网络的参数。最后，我们打印当前培训批次的损失和准确性，并将总结保存到磁盘。请注意，如果批量大小，培训批次的损失和准确性可能会有很大差异。而且因为我们使用辍学，您的培训指标可能开始比您的评估指标更差。

我们编写一个类似的函数来评估任意数据集的丢失和准确性，例如验证集或整个训练集。本质上这个功能与上述相同，但没有训练操作。它也禁用退学。

```python
def dev_step(x_batch, y_batch, writer=None):
    """
    Evaluates model on a dev set
    """
    feed_dict = {
      cnn.input_x: x_batch,
      cnn.input_y: y_batch,
      cnn.dropout_keep_prob: 1.0
    }
    step, summaries, loss, accuracy = sess.run(
        [global_step, dev_summary_op, cnn.loss, cnn.accuracy],
        feed_dict)
    time_str = datetime.datetime.now().isoformat()
    print("{}: step {}, loss {:g}, acc {:g}".format(time_str, step, loss, accuracy))
    if writer:
        writer.add_summary(summaries, step)
```

### 4.6 Training loop训练循环

最后，我们准备好编写我们的训练循环。 我们迭代我们的数据批次，为每个批次调用train_step函数，并偶尔评估和检查我们的模型：

```
# Generate batches
batches = data_helpers.batch_iter(
    zip(x_train, y_train), FLAGS.batch_size, FLAGS.num_epochs)
# Training loop. For each batch...
for batch in batches:
    x_batch, y_batch = zip(*batch)
    train_step(x_batch, y_batch)
    current_step = tf.train.global_step(sess, global_step)
    if current_step % FLAGS.evaluate_every == 0:
        print("\nEvaluation:")
        dev_step(x_dev, y_dev, writer=dev_summary_writer)
        print("")
    if current_step % FLAGS.checkpoint_every == 0:
        path = saver.save(sess, checkpoint_prefix, global_step=current_step)
        print("Saved model checkpoint to {}\n".format(path))
```

这里，batch_iter是一个我写的批处理数据的帮助函数，而tf.train.global_step是返回global_step值的便利函数。 培训的完整代码也在这里提供。

## 5 Visualizing Results in TensorBoard 在TensorBoard中可视化结果

我们的训练脚本将摘要写入输出目录，并通过将TensorBoard指向该目录，我们可以将图形和我们创建的摘要可视化。
`tensorboard --logdir /PATH_TO_CODE/runs/1449760558/summaries/`
使用默认参数（128维嵌入，过滤器尺寸为3，4和5，每个过滤器尺寸为0.5和128个过滤器）的运行训练过程导致以下损失和精度图（蓝色是训练数据，红色为10％ 开发数据）。

![image](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/12/Screen-Shot-2015-12-11-at-6.29.14-AM-768x260.png)

![image](http://d3kbpzbmcynnmx.cloudfront.net/wp-content/uploads/2015/12/Screen-Shot-2015-12-11-at-6.27.48-AM-768x262.png)

有几件事情脱颖而出： 

- 我们的培训指标不顺利，因为我们使用小批量大小。 如果我们使用较大的批次（或在整个训练集上评估），我们将获得更平滑的蓝线。 

- 因为开发者的准确性显着低于训练准确度，我们的网络似乎超过了训练数据，这表明我们需要更多的数据（MR数据集非常小），更强的正则化或更少的模型参数。 例如，我尝试在最后一层添加额外的L2惩罚权重，并能够将准确度提高到76％，接近于原始报告。 

- 培训损失和准确性开始大大低于开发商指标，因为应用了退出。
您可以玩代码，并尝试运行具有各种参数配置的模型。 Github提供了代码和说明。

## 6 Extensions and Exercises扩展和练习

以下是一些有用的练习，可以提高模型的性能：
- 使用预先训练的word2vec向量初始化嵌入。 为了使这项工作，您需要使用300维嵌入，并用预先训练的值初始化它们。
- 限制最后一层权重向量的L2范数，就像原始的纸张一样。 您可以通过定义一个新的操作，在每个培训步骤之后更新权重值。
- 将L2正规化添加到网络以防止过度配对，同时也提高辍学率。 （Github上的代码已经包括L2正则化，但默认情况下禁用）
- 添加重量更新和图层操作的直方图摘要，并在TensorBoard中进行可视化。







