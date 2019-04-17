# 【Tensorflow slim】slim learning包 - zj360202的专栏 - CSDN博客





2017年11月21日 16:39:09[zj360202](https://me.csdn.net/zj360202)阅读数：5785








TF-Slim在learning.py中的训练模型提供了一套简单但功能强大的工具。 这些功能包括一个训练函数可以反复测量损失，计算梯度并将模型保存到磁盘，以及用于操纵梯度的几个便利函数。 例如，一旦我们指定了模型，损失函数和优化方案，我们可以调用slim.learning.create_train_op和slim.learning.train来执行优化：



```python
g = tf.Graph()

# Create the model and specify the losses...
...

total_loss = slim.losses.get_total_loss()
optimizer = tf.train.GradientDescentOptimizer(learning_rate)

# create_train_op ensures that each time we ask for the loss, the update_ops
# are run and the gradients being computed are applied too.
train_op = slim.learning.create_train_op(total_loss, optimizer)
logdir = ... # Where checkpoints are stored.

slim.learning.train(
    train_op,
    logdir,
    number_of_steps=1000,
    save_summaries_secs=300,
    save_interval_secs=600):
```

在这个例子中，slim.learning.train与train_op一起提供，用于（a）计算损失和（b）应用梯度步骤。 logdir指定检查点和事件文件的存储目录。 我们可以限制采取任何数字的梯度步数。 在这种情况下，我们要求采取1000个步骤。 最后，save_summaries_secs = 300表示我们将每隔5分钟计算摘要，save_interval_secs = 600表示我们将每10分钟保存一次模型检查点。




### Working Example: Training the VGG16 Model

```python
import tensorflow as tf

slim = tf.contrib.slim
vgg = tf.contrib.slim.nets.vgg

...

train_log_dir = ...
if not tf.gfile.Exists(train_log_dir):
  tf.gfile.MakeDirs(train_log_dir)

with tf.Graph().as_default():
  # Set up the data loading:
  images, labels = ...

  # Define the model:
  predictions = vgg.vgg_16(images, is_training=True)

  # Specify the loss function:
  slim.losses.softmax_cross_entropy(predictions, labels)

  total_loss = slim.losses.get_total_loss()
  tf.summary.scalar('losses/total_loss', total_loss)

  # Specify the optimization scheme:
  optimizer = tf.train.GradientDescentOptimizer(learning_rate=.001)

  # create_train_op that ensures that when we evaluate it to get the loss,
  # the update_ops are done and the gradient updates are computed.
  train_tensor = slim.learning.create_train_op(total_loss, optimizer)

  # Actually runs training.
  slim.learning.train(train_tensor, train_log_dir)
```






