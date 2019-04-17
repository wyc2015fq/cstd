# tensorflow 0.10 word2vec 源码解析 - Keith - CSDN博客





2016年10月18日 15:20:37[ke1th](https://me.csdn.net/u012436149)阅读数：7228








关于word2vec 的解释见[word2vec的数学原理](http://blog.csdn.net/itplus/article/details/37998797)。 

本代码主要是实现了skip-gram模型，通过神经网络，对概率进行建模（概率模型中的最大似然，其实就是神经网络中的最小损失）

```python
# Copyright 2015 The TensorFlow Authors. All Rights Reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
# ==============================================================================

from __future__ import absolute_import
from __future__ import division
from __future__ import print_function

import collections
import math
import os
import random
import zipfile
import numpy as np    #在引用包方面，对源码进行了一些修改，因为源码应该是基于python3的 
import urllib         #而我使用的python2
import tensorflow as tf

# Step 1: Download the data.
url = 'http://mattmahoney.net/dc/'

def maybe_download(filename, expected_bytes):
  """Download a file if not present, and make sure it's the right size."""
  if not os.path.exists(filename):
    filename, _ = urllib.urlretrieve(url + filename, filename)
  statinfo = os.stat(filename)
  if statinfo.st_size == expected_bytes:
    print('Found and verified', filename)
  else:
    print(statinfo.st_size)
    raise Exception(
        'Failed to verify ' + filename + '. Can you get to it with a browser?')
  return filename

filename = maybe_download('text8.zip', 31344016)


# Read the data into a list of strings.
def read_data(filename):
  """Extract the first file enclosed in a zip file as a list of words"""
  with zipfile.ZipFile(filename) as f:
    data = tf.compat.as_str(f.read(f.namelist()[0])).split()
  return data

words = read_data(filename)
print('Data size', len(words))

# Step 2: Build the dictionary and replace rare words with UNK token.
vocabulary_size = 50000

def build_dataset(words): #words in corpus , list
  count = [['UNK', -1]]
  count.extend(collections.Counter(words).most_common(vocabulary_size - 1)) 
  # 因为使用了most_common,所以count中的word，是按照word在文本中出现的次数从大到小排列的
  dictionary = dict()
  for word, _ in count:
    dictionary[word] = len(dictionary)   # assign id to word
  data = list()
  unk_count = 0
  for word in words:
    if word in dictionary:
      index = dictionary[word]
    else:
      index = 0  # dictionary['UNK'] UNK:unknown
      unk_count += 1
    data.append(index) #translate word to id
  count[0][1] = unk_count
  reverse_dictionary = dict(zip(dictionary.values(), dictionary.keys()))
  return data, count, dictionary, reverse_dictionary #data:ids count:list of [word,num] dictionary:word->id ,

data, count, dictionary, reverse_dictionary = build_dataset(words)
del words  # Hint to reduce memory.
print('Most common words (+UNK)', count[:5])
print('Sample data', data[:10], [reverse_dictionary[i] for i in data[:10]])

data_index = 0


# Step 3: Function to generate a training batch for the skip-gram model.
#skip_skip:从span里面取出多少个word， skip_window:|contex(w)| / 2
#span: w上下文的word， 只能从span这个范围中获取
def generate_batch(batch_size, num_skips, skip_window):
  global data_index
  assert batch_size % num_skips == 0
  assert num_skips <= 2 * skip_window   # num_skip? skip_window? skip-gram
  batch = np.ndarray(shape=(batch_size), dtype=np.int32) #batch_size: the number of words in one batch
  labels = np.ndarray(shape=(batch_size, 1), dtype=np.int32)
  span = 2 * skip_window + 1 # [ skip_window target skip_window ]
  buffer = collections.deque(maxlen=span) #buffer用来存取w上下文word的id
  for _ in range(span):
    buffer.append(data[data_index]) # data:ids
    data_index = (data_index + 1) % len(data)
  for i in range(batch_size // num_skips): #how many num_skips in a batch
    target = skip_window  # target label at the center of the buffer
    targets_to_avoid = [ skip_window ] # extract the middle word
    for j in range(num_skips):
      while target in targets_to_avoid:#context中的word，一个只取一次
        target = random.randint(0, span - 1)
      targets_to_avoid.append(target) #
      batch[i * num_skips + j] = buffer[skip_window]
      labels[i * num_skips + j, 0] = buffer[target]
    buffer.append(data[data_index]) #update the buffer, append the next word to buffer
    data_index = (data_index + 1) % len(data)
  return batch, labels #batch: ids [batch_size] lebels:ids [batch_size*1]

batch, labels = generate_batch(batch_size=8, num_skips=2, skip_window=1)
for i in range(8):
  print(batch[i], reverse_dictionary[batch[i]],
      '->', labels[i, 0], reverse_dictionary[labels[i, 0]])
#========================================================
#上面的操作会形成一个这样的输出 batch中存储的是id， 假设我们去skip_size = 4, skip_window = 2
#那么，单词 as 所对应的context的word个数就是4个，所以batch中有4个as， 所对应的就是context中的word
# 12 as -> 195 term
# 12 as -> 5239 anarchism
# 12 as -> 6 a
# 12 as -> 3084 originated
# 6 a -> 12 as
# 6 a -> 3084 originated
# 6 a -> 2 of
# 6 a -> 195 term
#=======================================================
# Step 4: Build and train a skip-gram model.

#在本代码中，batch_size代表的是一个batch中，word的个数，而不是sentense的个数。
batch_size = 128
embedding_size = 128  # Dimension of the embedding vector.
skip_window = 1       # How many words to consider left and right.
num_skips = 2         # How many times to reuse an input(buffer) to generate a label.

# We pick a random validation set to sample nearest neighbors. Here we limit the
# validation samples to the words that have a low numeric ID, which by
# construction are also the most frequent.
valid_size = 16     # Random set of words to evaluate similarity on.
valid_window = 100  # Only pick dev samples in the head of the distribution.
valid_examples = np.random.choice(valid_window, valid_size, replace=False)
num_sampled = 64    # Number of negative examples to sample.

graph = tf.Graph()

with graph.as_default():

  # Input data.
  #在这里，我们只输入word对应的id，假设batch_size是128,那么我们第一次就输入文本前128个word所对应的id
  train_inputs = tf.placeholder(tf.int32, shape=[batch_size])
  #labels和inputs是一样的， 只不过一个是行向量（tensor），一个是列向量（tensor）
  train_labels = tf.placeholder(tf.int32, shape=[batch_size, 1])
  valid_dataset = tf.constant(valid_examples, dtype=tf.int32)

  # Ops and variables pinned to the CPU because of missing GPU implementation
  with tf.device('/cpu:0'):
    # Look up embeddings for inputs.
    embeddings = tf.Variable(
        tf.random_uniform([vocabulary_size, embedding_size], -1.0, 1.0))
    # embedding_look 返回值的shape 是 shape(train_inputs)+shape(embeddings)[1:]
    embed = tf.nn.embedding_lookup(embeddings, train_inputs)

    # Construct the variables for the NCE loss
    nce_weights = tf.Variable(   #every word has a corresponding nce_weight ad nce_biase
        tf.truncated_normal([vocabulary_size, embedding_size],
                            stddev=1.0 / math.sqrt(embedding_size)))
    nce_biases = tf.Variable(tf.zeros([vocabulary_size]))
```

在[word2vec的数学原理](http://blog.csdn.net/itplus/article/details/37998797)中的skip-gram模型解释中，作者提到了一个公式 
$\sigma(x_w^T\theta^u+b^u)$  其中 $x_w^T$ 是输入的word_vector ，$\theta^u$ 和 $b^u$ 就是上面的nce_weights nce_bias 其中nce_weights[u_id] nce_bias[u_id] 对应与单词u

```python
# Compute the average NCE loss for the batch.
  # tf.nce_loss automatically draws a new sample of the negative labels each
  # time we evaluate the loss.
  loss = tf.reduce_mean(
      tf.nn.nce_loss(nce_weights, nce_biases, embed, train_labels,
                     num_sampled, vocabulary_size))#关于nce_loss的介绍在文章最后

  # Construct the SGD optimizer using a learning rate of 1.0.
  optimizer = tf.train.GradientDescentOptimizer(1.0).minimize(loss)

  # Compute the cosine similarity between minibatch examples and all embeddings.
  norm = tf.sqrt(tf.reduce_sum(tf.square(embeddings), 1, keep_dims=True))
  normalized_embeddings = embeddings / norm
  valid_embeddings = tf.nn.embedding_lookup(
      normalized_embeddings, valid_dataset)
  similarity = tf.matmul(
      valid_embeddings, normalized_embeddings, transpose_b=True)

  # Add variable initializer.
  init = tf.initialize_all_variables()

# Step 5: Begin training.
num_steps = 10001

with tf.Session(graph=graph) as session:
  # We must initialize all variables before we use them.
  init.run()
  print("Initialized")

  average_loss = 0
  for step in xrange(num_steps):
    batch_inputs, batch_labels = generate_batch(
        batch_size, num_skips, skip_window)
    feed_dict = {train_inputs : batch_inputs, train_labels : batch_labels}

    # We perform one update step by evaluating the optimizer op (including it
    # in the list of returned values for session.run()
    _, loss_val = session.run([optimizer, loss], feed_dict=feed_dict)
    average_loss += loss_val

    if step % 2000 == 0:
      if step > 0:
        average_loss /= 2000
      # The average loss is an estimate of the loss over the last 2000 batches.
      print("Average loss at step ", step, ": ", average_loss)
      average_loss = 0

    # Note that this is expensive (~20% slowdown if computed every 500 steps)
    if step % 10000 == 0:
      sim = similarity.eval()
      for i in xrange(valid_size):
        valid_word = reverse_dictionary[valid_examples[i]]
        top_k = 8 # number of nearest neighbors
        nearest = (-sim[i, :]).argsort()[1:top_k+1]
        log_str = "Nearest to %s:" % valid_word
        for k in xrange(top_k):
          close_word = reverse_dictionary[nearest[k]]
          log_str = "%s %s," % (log_str, close_word)
        print(log_str)
  final_embeddings = normalized_embeddings.eval()

# Step 6: Visualize the embeddings.

def plot_with_labels(low_dim_embs, labels, filename='tsne.png'):
  assert low_dim_embs.shape[0] >= len(labels), "More labels than embeddings"
  plt.figure(figsize=(18, 18))  #in inches
  for i, label in enumerate(labels):
    x, y = low_dim_embs[i,:]
    plt.scatter(x, y)
    plt.annotate(label,
                 xy=(x, y),
                 xytext=(5, 2),
                 textcoords='offset points',
                 ha='right',
                 va='bottom')

  plt.savefig(filename)

try:
  from sklearn.manifold import TSNE
  import matplotlib.pyplot as plt

  tsne = TSNE(perplexity=30, n_components=2, init='pca', n_iter=5000)
  plot_only = 500
  low_dim_embs = tsne.fit_transform(final_embeddings[:plot_only,:])
  labels = [reverse_dictionary[i] for i in xrange(plot_only)]
  plot_with_labels(low_dim_embs, labels)

except ImportError:
  print("Please install sklearn, matplotlib, and scipy to visualize embeddings.")
```

**细说nce_loss**

nce_loss函数提供了，取negtive sample，计算loss一条龙服务，相当方便。源码中的注释是这么写的

```python
def nce_loss(weights, #[num_classes, dim] dim就是emdedding_size
             biases,  #[num_classes] num_classes就是word的个数（不包括重复的）
             inputs， #[batch_size, dim]
             labels,  #[batch_size, num_true] 这里，我们的num_true设置为1，就是一个输入对应一个输出
             num_sampled,#要取的负样本的个数（per batch）
             num_classes,#类别的个数（在这里就是word的个数（不包含重复的））
             num_true=1,
             sampled_values=None,
             remove_accidental_hits=False,
             partition_strategy="mod",
             name="nce_loss"):
      logits, labels = _compute_sampled_logits(
      weights,
      biases,
      inputs,
      labels,
      num_sampled,
      num_classes,
      num_true=num_true,
      sampled_values=sampled_values,
      subtract_log_q=True,
      remove_accidental_hits=remove_accidental_hits,
      partition_strategy=partition_strategy,
      name=name)
  sampled_losses = sigmoid_cross_entropy_with_logits(
      logits, labels, name="sampled_losses") 
      #此函数返回的tensor与输入logits同维度。 _sum_rows之后，就得到了每个样本的corss entropy。
  # sampled_losses is batch_size x {true_loss, sampled_losses...}
  # We sum out true and sampled losses.
  return _sum_rows(sampled_losses)
  #在word2vec中对此函数的返回调用了reduce_mean() 就获得了平均 cross entropy
```

nce_loss调用了_compute_sammpled_logits, 这个函数是搞什么的呢？ 

看源码

```python
def _compute_sampled_logits(weights,
                            biases,
                            inputs,
                            labels,
                            num_sampled,
                            num_classes,
                            num_true=1,
                            sampled_values=None,
                            subtract_log_q=True,
                            remove_accidental_hits=False,
                            partition_strategy="mod",
                            name=None):
  if not isinstance(weights, list):
    weights = [weights]

  with ops.op_scope(weights + [biases, inputs, labels], name,
                    "compute_sampled_logits"):
    if labels.dtype != dtypes.int64:
      labels = math_ops.cast(labels, dtypes.int64)
    labels_flat = array_ops.reshape(labels, [-1])

    # Sample the negative labels.
    #   sampled shape: [num_sampled] tensor
    #   true_expected_count shape = [batch_size, 1] tensor
    #   sampled_expected_count shape = [num_sampled] tensor
    if sampled_values is None:
      sampled_values = candidate_sampling_ops.log_uniform_candidate_sampler(
          true_classes=labels,
          num_true=num_true,
          num_sampled=num_sampled,
          unique=True,
          range_max=num_classes)
```

**NOTE：**这个函数是通过log-uniform进行取样的$P(class)=(\log(class+2)-\log(class+1))/\log(rang_max+1)$，取样范围是[0, range_max] ,用这种方法取样就要求我们的word是按照频率从高到低排列的。之前对word的处理的确是这样

```python
# NOTE: pylint cannot tell that 'sampled_values' is a sequence
    # pylint: disable=unpacking-non-sequence
    sampled, true_expected_count, sampled_expected_count = sampled_values
    # pylint: enable=unpacking-non-sequence

    # labels_flat is a [batch_size * num_true] tensor
    # sampled is a [num_sampled] int tensor
    all_ids = array_ops.concat(0, [labels_flat, sampled])

    # weights shape is [num_classes, dim]
    all_w = embedding_ops.embedding_lookup(
        weights, all_ids, partition_strategy=partition_strategy)
    all_b = embedding_ops.embedding_lookup(biases, all_ids)
    # true_w shape is [batch_size * num_true, dim]
    # true_b is a [batch_size * num_true] tensor
    true_w = array_ops.slice(
        all_w, [0, 0], array_ops.pack([array_ops.shape(labels_flat)[0], -1]))
    true_b = array_ops.slice(all_b, [0], array_ops.shape(labels_flat))

    # inputs shape is [batch_size, dim]
    # true_w shape is [batch_size * num_true, dim]
    # row_wise_dots is [batch_size, num_true, dim]
    dim = array_ops.shape(true_w)[1:2]
    new_true_w_shape = array_ops.concat(0, [[-1, num_true], dim])
    row_wise_dots = math_ops.mul(
        array_ops.expand_dims(inputs, 1),
        array_ops.reshape(true_w, new_true_w_shape))
    # We want the row-wise dot plus biases which yields a
    # [batch_size, num_true] tensor of true_logits.
    dots_as_matrix = array_ops.reshape(row_wise_dots,
                                       array_ops.concat(0, [[-1], dim]))
    true_logits = array_ops.reshape(_sum_rows(dots_as_matrix), [-1, num_true])
    true_b = array_ops.reshape(true_b, [-1, num_true])
    true_logits += true_b

    # Lookup weights and biases for sampled labels.
    #   sampled_w shape is [num_sampled, dim]
    #   sampled_b is a [num_sampled] float tensor
    sampled_w = array_ops.slice(
        all_w, array_ops.pack([array_ops.shape(labels_flat)[0], 0]), [-1, -1])
    sampled_b = array_ops.slice(all_b, array_ops.shape(labels_flat), [-1])

    # inputs has shape [batch_size, dim]
    # sampled_w has shape [num_sampled, dim]
    # sampled_b has shape [num_sampled]
    # Apply X*W'+B, which yields [batch_size, num_sampled]
    sampled_logits = math_ops.matmul(
        inputs, sampled_w, transpose_b=True) + sampled_b

    if remove_accidental_hits:
      acc_hits = candidate_sampling_ops.compute_accidental_hits(
          labels, sampled, num_true=num_true)
      acc_indices, acc_ids, acc_weights = acc_hits

      # This is how SparseToDense expects the indices.
      acc_indices_2d = array_ops.reshape(acc_indices, [-1, 1])
      acc_ids_2d_int32 = array_ops.reshape(
          math_ops.cast(acc_ids, dtypes.int32), [-1, 1])
      sparse_indices = array_ops.concat(1, [acc_indices_2d, acc_ids_2d_int32],
                                        "sparse_indices")
      # Create sampled_logits_shape = [batch_size, num_sampled]
      sampled_logits_shape = array_ops.concat(
          0,
          [array_ops.shape(labels)[:1], array_ops.expand_dims(num_sampled, 0)])
      if sampled_logits.dtype != acc_weights.dtype:
        acc_weights = math_ops.cast(acc_weights, sampled_logits.dtype)
      sampled_logits += sparse_ops.sparse_to_dense(
          sparse_indices,
          sampled_logits_shape,
          acc_weights,
          default_value=0.0,
          validate_indices=False)

    if subtract_log_q:
      # Subtract log of Q(l), prior probability that l appears in sampled.
      true_logits -= math_ops.log(true_expected_count)
      sampled_logits -= math_ops.log(sampled_expected_count)

    # Construct output logits and labels. The true labels/logits start at col 0.
    out_logits = array_ops.concat(1, [true_logits, sampled_logits])
    # true_logits is a float tensor, ones_like(true_logits) is a float tensor
    # of ones. We then divide by num_true to ensure the per-example labels sum
    # to 1.0, i.e. form a proper probability distribution.
    out_labels = array_ops.concat(1,
                                  [array_ops.ones_like(true_logits) / num_true,
                                   array_ops.zeros_like(sampled_logits)])

  return out_logits, out_labels
  """
  代码很长，但是我们主要关心的是返回的out_logits和out_labels是什么样子的，想搞清楚到底是怎么样用神经网络对概率分布进行建模。
  """
```

最终输出的logits是这样的,假设($u_i \in context(w)$, $n_i \notin context(w)$) [batch_size, 1+num_sampled] 

|$x_w^T\theta^{u_0}$| $x_w^T\theta^{n_0}$|$x_w^T\theta^{n_1}$|$x_w^T\theta^{n_2}$|$x_w^T\theta^{n_3}$|…| 

|$x_w^T\theta^{u_1}$| $x_w^T\theta^{n_0}$|$x_w^T\theta^{n_1}$|$x_w^T\theta^{n_2}$|$x_w^T\theta^{n_3}$|…| 

|$x_w^T\theta^{u_2}$| $x_w^T\theta^{n_0}$|$x_w^T\theta^{n_1}$|$x_w^T\theta^{n_2}$|$x_w^T\theta^{n_3}$|…| 

… 

是输出的out_labels是这样的 [batch_size, 1+num_sampled] 

|1                                  |0                                  |0                                  |0                                  |0                                  |…| 

|1                                  |0                                  |0                                  |0                                  |0                                  |…| 

|1                                  |0                                  |0                                  |0                                  |0                                  |…| 

… 
[word2vec的数学原理](http://blog.csdn.net/itplus/article/details/37998797)中告诉我们的是，要求logits的最大似然。就相当与最小化out_labels与logits的损失函数。 

之后将输出的logits和out_labels输入到sampled_losses = sigmoid_cross_entropy_with_logits( 

      logits, labels, name=”sampled_losses”)
```python
def sigmoid_cross_entropy_with_logits(logits, targets, name=None):
  with ops.name_scope(name, "logistic_loss", [logits, targets]) as name:
    logits = ops.convert_to_tensor(logits, name="logits")
    targets = ops.convert_to_tensor(targets, name="targets")
    try:
      targets.get_shape().merge_with(logits.get_shape())
    except ValueError:
      raise ValueError("logits and targets must have the same shape (%s vs %s)"
                       % (logits.get_shape(), targets.get_shape()))

    # The logistic loss formula from above is
    #   x - x * z + log(1 + exp(-x))
    # For x < 0, a more numerically stable formula is
    #   -x * z + log(1 + exp(x))
    # Note that these two expressions can be combined into the following:
    #   max(x, 0) - x * z + log(1 + exp(-abs(x)))
    # To allow computing gradients at zero, we define custom versions of max and
    # abs functions.
    zeros = array_ops.zeros_like(logits, dtype=logits.dtype)
    cond = (logits >= zeros)
    relu_logits = math_ops.select(cond, logits, zeros)
    neg_abs_logits = math_ops.select(cond, -logits, logits)
    return math_ops.add(relu_logits - logits * targets,
                        math_ops.log(1 + math_ops.exp(neg_abs_logits)),
                        name=name)
```

此函数返回的tensor与输入logits同维度。 _sum_rows之后，就得到了每个样本的corss entropy。

**水平有限，如有问题，请指正**




