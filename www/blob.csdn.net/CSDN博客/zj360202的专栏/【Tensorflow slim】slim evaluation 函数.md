# 【Tensorflow slim】slim evaluation 函数 - zj360202的专栏 - CSDN博客





2017年11月21日 15:48:23[zj360202](https://me.csdn.net/zj360202)阅读数：2855








TF-Slim提供一个评估模块（evaluation.py），其中包含用于使用metric_ops.py模块中的指标编写模型评估脚本的帮助函数。 这些功能包括定期运行评估，评估各批数据的指标以及打印和汇总度量结果。 例如：



```python
import tensorflow as tf

slim = tf.contrib.slim

# Load the data
images, labels = load_data(...)

# Define the network
predictions = MyModel(images)

# Choose the metrics to compute:
names_to_values, names_to_updates = slim.metrics.aggregate_metric_map({
    'accuracy': slim.metrics.accuracy(predictions, labels),
    'precision': slim.metrics.precision(predictions, labels),
    'recall': slim.metrics.recall(mean_relative_errors, 0.3),
})

# Create the summary ops such that they also print out to std output:
summary_ops = []
for metric_name, metric_value in names_to_values.iteritems():
  op = tf.summary.scalar(metric_name, metric_value)
  op = tf.Print(op, [metric_value], metric_name)
  summary_ops.append(op)

num_examples = 10000
batch_size = 32
num_batches = math.ceil(num_examples / float(batch_size))

# Setup the global step.
slim.get_or_create_global_step()

output_dir = ... # Where the summaries are stored.
eval_interval_secs = ... # How often to run the evaluation.
slim.evaluation.evaluation_loop(
    'local',
    checkpoint_dir,
    log_dir,
    num_evals=num_batches,
    eval_op=names_to_updates.values(),
    summary_op=tf.summary.merge(summary_ops),
    eval_interval_secs=eval_interval_secs)
```




Evalution提供evaluation_loop和evaluation_once两种方法



