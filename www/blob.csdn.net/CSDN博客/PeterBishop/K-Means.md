# K-Means - PeterBishop - CSDN博客





2019年01月13日 11:22:58[PeterBishop0](https://me.csdn.net/qq_40061421)阅读数：51








使用TensorFlow实现K-Means算法，并将其应用于分类

手写的数字图像。 此示例使用的是MNIST数据库

手写数字作为训练样本（http://yann.lecun.com/exdb/mnist/）。

Note: This example requires TensorFlow v1.1.0 or over.

Author: Aymeric Damien

Project: https://github.com/aymericdamien/TensorFlow-Examples/



```python
from __future__ import print_function

import numpy as np
import tensorflow as tf
from tensorflow.contrib.factorization import KMeans

# 忽视所有的GPU, tf的随机森林不受影响.
import os
os.environ["CUDA_VISIBLE_DEVICES"] = ""

# 导入 MNIST 数据
from tensorflow.examples.tutorials.mnist import input_data
mnist = input_data.read_data_sets("/tmp/data/", one_hot=True)
full_data_x = mnist.train.images

# 变量
num_steps = 50 # 训练步数
batch_size = 1024 # 每一次训练的个数
k = 25 # 聚类个数
num_classes = 10 #10个数字
num_features = 784 # 每个图片是28x28像素

# 输入图像
X = tf.placeholder(tf.float32, shape=[None, num_features])
# Labels (for assigning a label to a centroid and testing)
Y = tf.placeholder(tf.float32, shape=[None, num_classes])

# K-Means 变量
kmeans = KMeans(inputs=X, num_clusters=k, distance_metric='cosine',
                use_mini_batch=True)

# 建立 KMeans 图
training_graph = kmeans.training_graph()

if len(training_graph) > 6: # Tensorflow 1.4+
    (all_scores, cluster_idx, scores, cluster_centers_initialized,
     cluster_centers_var, init_op, train_op) = training_graph
else:
    (all_scores, cluster_idx, scores, cluster_centers_initialized,
     init_op, train_op) = training_graph

cluster_idx = cluster_idx[0] # fix for cluster_idx being a tuple
avg_distance = tf.reduce_mean(scores)

# 初始化变量
init_vars = tf.global_variables_initializer()

# 开始tf的Session
sess = tf.Session()

#初始化
sess.run(init_vars, feed_dict={X: full_data_x})
sess.run(init_op, feed_dict={X: full_data_x})

# 训练
for i in range(1, num_steps + 1):
    _, d, idx = sess.run([train_op, avg_distance, cluster_idx],
                         feed_dict={X: full_data_x})
    if i % 10 == 0 or i == 1:
        print("Step %i, Avg Distance: %f" % (i, d))

#每一个中心分配一个label
# 使用每一个训练的label来计算每一个中心的label个数
# sample to their closest centroid (given by 'idx')
counts = np.zeros(shape=(k, num_classes))
for i in range(len(idx)):
    counts[idx[i]] += mnist.train.labels[i]
# Assign the most frequent label to the centroid
labels_map = [np.argmax(c) for c in counts]
labels_map = tf.convert_to_tensor(labels_map)

# Evaluation ops
# Lookup: centroid_id -> label
cluster_label = tf.nn.embedding_lookup(labels_map, cluster_idx)
# Compute accuracy
correct_prediction = tf.equal(cluster_label, tf.cast(tf.argmax(Y, 1), tf.int32))
accuracy_op = tf.reduce_mean(tf.cast(correct_prediction, tf.float32))

# Test Model
test_x, test_y = mnist.test.images, mnist.test.labels
print("Test Accuracy:", sess.run(accuracy_op, feed_dict={X: test_x, Y: test_y}))
```





