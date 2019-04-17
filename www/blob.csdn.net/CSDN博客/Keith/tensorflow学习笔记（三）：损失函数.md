# tensorflow学习笔记（三）：损失函数 - Keith - CSDN博客





2016年10月20日 17:00:19[ke1th](https://me.csdn.net/u012436149)阅读数：23797
个人分类：[tensorflow](https://blog.csdn.net/u012436149/article/category/6461700)

所属专栏：[tensorflow学习笔记](https://blog.csdn.net/column/details/13300.html)









#### tf.python.ops.nn_ops.sparse_softmax_cross_entropy_with_logits(logits, labels, name=None)

```python
def sparse_softmax_cross_entropy_with_logits(logits, labels, name=None):
#logits是最后一层的z（输入）
#A common use case is to have logits of shape `[batch_size, num_classes]` and
#labels of shape `[batch_size]`. But higher dimensions are supported.
#Each entry in `labels` must be an index in `[0, num_classes)`
#输出：loss [batch_size]
```

#### tf.python.ops.nn_ops.softmax_cross_entropy_with_logits(logits, targets, dim=-1, name=None)

```python
def softmax_cross_entropy_with_logits(logits, targets, dim=-1, name=None):
#`logits` and `labels` must have the same shape `[batch_size, num_classes]`
#return loss:[batch_size], 里面保存是batch中每个样本的cross entropy
```

#### tf.nn.sigmoid_cross_entropy_with_logits(logits, targets, name=None)

```python
def sigmoid_cross_entropy_with_logits(logits, targets, name=None):
#logits:[batch_size, num_classes],targets:[batch_size, size].logits作为用最后一层的输入就好，不需要进行sigmoid运算，函数内部进行了sigmoid操作。
#输出loss [batch_size, num_classes]。。。说的是logits，其实内部实现是relu
```

#### tf.nn.nce_loss(nce_weights, nce_biases, embed, train_labels, num_sampled, vocabulary_size)

```python
def nce_loss(nce_weights, nce_biases, embed, train_labels, num_sampled, vocabulary_size):
#word2vec中用到了这个函数
#weights: A `Tensor` of shape `[num_classes, dim]`, or a list of `Tensor`
#        objects whose concatenation along dimension 0 has shape
#        [num_classes, dim].  The (possibly-partitioned) class embeddings.
#biases: A `Tensor` of shape `[num_classes]`.  The class biases.
#inputs: A `Tensor` of shape `[batch_size, dim]`.  The forward
#        activations of the input network.
#labels: A `Tensor` of type `int64` and shape `[batch_size,
#    num_true]`. The target classes.
#num_sampled: An `int`.  The number of classes to randomly sample per batch.
#num_classes: An `int`. The number of possible classes.
#num_true: An `int`.  The number of target classes per training example.
```

#### tf.nn.sequence_loss_by_example(logits, targets, weights,average_across_timesteps=True,softmax_loss_function=None, name=None):

```python
def sequence_loss_by_example(logits, targets, weights,
                             average_across_timesteps=True,
                             softmax_loss_function=None, name=None):
#logits: List of 2D Tensors of shape [batch_size x num_decoder_symbols].
#targets: List of 1D batch-sized int32 Tensors of the same length as logits.
#weights: List of 1D batch-sized float-Tensors of the same length as logits.
#return:log_pers 形状是 [batch_size].
   for logit, target, weight in zip(logits, targets, weights):
      if softmax_loss_function is None:
        # TODO(irving,ebrevdo): This reshape is needed because
        # sequence_loss_by_example is called with scalars sometimes, which
        # violates our general scalar strictness policy.
        target = array_ops.reshape(target, [-1])
        crossent = nn_ops.sparse_softmax_cross_entropy_with_logits(
            logit, target)
      else:
        crossent = softmax_loss_function(logit, target)
      log_perp_list.append(crossent * weight)
    log_perps = math_ops.add_n(log_perp_list)
    if average_across_timesteps:
      total_size = math_ops.add_n(weights) 
      total_size += 1e-12  # Just to avoid division by 0 for all-0 weights.
      log_perps /= total_size
  return log_perps
```

关于weights：形状应该是[T, batch_size] ,如果input包含填充的数据，对应的weights置0，其余置1。这样就可以保证，填充的数据不会进行梯度下降。



