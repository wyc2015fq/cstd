# 【Tensorflow slim】slim metrics包 - zj360202的专栏 - CSDN博客





2017年11月21日 18:11:56[zj360202](https://me.csdn.net/zj360202)阅读数：2281







我们将度量定义为一个性能度量，而不是损失函数（损失在训练期间直接进行优化），但是我们仍然对他在评估模型方面感兴趣。例如，我们可能希望尽量减少对数损失，但我们的兴趣度量可能是F1分（测试准确度）或交汇分数（不可区分，因此不能用作损失）。

TF-Slim提供了一系列度量操作，使评估模型变得简单。抽象地说，计算度量的值可以分为三部分：
- 初始化：初始化用于计算度量的变量。
- 聚合：执行用于计算度量的操作（总和等）。
- 完成：（可选）执行任何最终操作来计算度量标准值。例如，计算手段，分钟，最大值等
例如，为了计算mean_absolute_error，计数和总变量这两个变量被初始化为零。在聚合过程中，我们观察了一些预测和标签集合，计算它们的绝对差异并将总数加到总数上。每次我们观察另一个值时，计数都会递增。最后，在定稿过程中，总数除以数量得到平均值。

以下示例演示了用于声明metrics的API。由于度量标准通常是在与训练集不同的测试集上进行评估的，因此我们假定我们使用的是测试数据：



```python
images, labels = LoadTestData(...)
predictions = MyModel(images)

mae_value_op, mae_update_op = slim.metrics.streaming_mean_absolute_error(predictions, labels)
mre_value_op, mre_update_op = slim.metrics.streaming_mean_relative_error(predictions, labels)
pl_value_op, pl_update_op = slim.metrics.percentage_less(mean_relative_errors, 0.3)
```
如示例所示，创建度量标准会返回两个值：value_op和update_op。 value_op是一个幂等操作，返回度量的当前值。 update_op是执行上述聚合步骤以及返回度量值的操作。


跟踪每个value_op和update_op可能会很费力。 为了解决这个问题，TF-Slim提供了两个便利功能：

```python
# Aggregates the value and update ops in two lists:
value_ops, update_ops = slim.metrics.aggregate_metrics(
    slim.metrics.streaming_mean_absolute_error(predictions, labels),
    slim.metrics.streaming_mean_squared_error(predictions, labels))

# Aggregates the value and update ops in two dictionaries:
names_to_values, names_to_updates = slim.metrics.aggregate_metric_map({
    "eval/mean_absolute_error": slim.metrics.streaming_mean_absolute_error(predictions, labels),
    "eval/mean_squared_error": slim.metrics.streaming_mean_squared_error(predictions, labels),
})
```


### Working example: Tracking Multiple Metrics




Putting it all together:



```python
import tensorflow as tf

slim = tf.contrib.slim
vgg = tf.contrib.slim.nets.vgg


# Load the data
images, labels = load_data(...)

# Define the network
predictions = vgg.vgg_16(images)

# Choose the metrics to compute:
names_to_values, names_to_updates = slim.metrics.aggregate_metric_map({
    "eval/mean_absolute_error": slim.metrics.streaming_mean_absolute_error(predictions, labels),
    "eval/mean_squared_error": slim.metrics.streaming_mean_squared_error(predictions, labels),
})

# Evaluate the model using 1000 batches of data:
num_batches = 1000

with tf.Session() as sess:
  sess.run(tf.global_variables_initializer())
  sess.run(tf.local_variables_initializer())

  for batch_id in range(num_batches):
    sess.run(names_to_updates.values())

  metric_values = sess.run(names_to_values.values())
  for metric, value in zip(names_to_values.keys(), metric_values):
    print('Metric %s has value: %f' % (metric, value))
```
请注意，可以单独使用metric_ops.py，而不使用layers.py或loss_ops.py







