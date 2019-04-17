# 【Tensorflow】自主实现部分连接层（Partial Connect Layer） - CD's Coding - CSDN博客





2018年06月28日 15:54:40[糖果天王](https://me.csdn.net/okcd00)阅读数：524








## 0x00 前言

通常而言，在NLP领域的机器学习中时常会有这样的需求：$ argmax P(w_i \mid \theta) $

较为常见的做法是有多少个单词就做一个多少维的全连接层加softmax，但是， 

如果词汇表 $|V|$ 很大的情况下，会有大量的计算（例如目前的工作中，词汇表的数量为80k）， 

在我们已知只需要计算其中某些词语的时候，完全可以屏蔽掉其中多余的计算， 

（即使增加一个`mask`把非计算部分都设为 $0$，乘以 $0$ 的乘法也不要做比较好）， 

所以这里想到新构造一个 **partial connect layer** 来解决这个问题。 

（因为懒得更新TF，各位如果新配的TF环境完全可以试着去官方文档里搜搜 `sparse_` 开头的各种方法）
## 0x01 构造思路

就个人而言，其实比较懒，肯定不会过于深入透彻的去从很底层写； 

所以从思路上而言，就是拼了拼 `embedding_layer` 和 `full_connect_layer`； 

基本的 `EmbLayer` 和 `FCLayer` 如下所示： (Thanks for **@lhw446**)
```python
class EmbLayer(object):
    """embedding layer"""
    def __init__(self, word_emb_shape, word_embeddings_path=None, voc_path=None,
                 partition_strategy='mod', validate_indices=True, max_norm=None,
                 weight_decay=None, stop_gradient=False, show_no_word=True, name='emb'):
        # params
        self.partition_strategy = partition_strategy
        self.validate_indices = validate_indices
        self.word_embeddings = None
        self.max_norm = max_norm
        self.name = name

        with tf.name_scope('{}_def'.format(self.name)):
            scale = math.sqrt(2.0 / np.prod(word_emb_shape[-1]))
            self.word_embeddings = scale * np.random.standard_normal(size=word_emb_shape)

            if word_embeddings_path:
                assert voc_path is not None
                idx2word = pickle.load(open(voc_path, 'rb'))['idx2word']
                word2vec = pickle.load(open(word_embeddings_path, 'rb'))
                for idx in range(word_emb_shape[0]):
                    word = idx2word[idx]
                    if word in word2vec:
                        self.word_embeddings[idx] = scale * 0.1 * word2vec[word][:word_emb_shape[1]]
                    elif show_no_word:
                        print('word2vec no word {}: {}'.format(idx, word.encode('utf-8')))

            self.word_embeddings = tf.Variable(
                initial_value=self.word_embeddings, dtype=tf.float32, name='word_embeddings')

            if stop_gradient:
                self.word_embeddings = tf.stop_gradient(self.word_embeddings)

            if weight_decay:
                tf.add_to_collection(
                    'losses', tf.multiply(tf.nn.l2_loss(self.word_embeddings),
                                          weight_decay, name='weight_decay_loss'))

    def __call__(self, ids):
        with tf.name_scope('{}_cal'.format(self.name)):
            outputs = tf.nn.embedding_lookup(self.word_embeddings, ids,
                                             partition_strategy=self.partition_strategy,
                                             validate_indices=self.validate_indices,
                                             max_norm=self.max_norm,
                                             name=self.name)
            return outputs
```

```python
class FulLayer(object):
    """Full Connect Layer"""
    def __init__(self, input_dim, output_dim, activation_fn=tf.sigmoid,
                 weight_decay=None, name="ful"):
        weight_shape = (input_dim, output_dim)
        self.activation_fn = activation_fn
        self.name = name

        with tf.name_scope('{}_def'.format(self.name)):
            # weight matrix
            scale = math.sqrt(2.0 / np.prod(weight_shape[:-1]))
            init_value = scale * np.random.standard_normal(size=weight_shape)
            self.weight = tf.Variable(init_value, dtype=tf.float32, name='weight')

            if weight_decay:
                tf.add_to_collection(
                    'losses', tf.multiply(tf.nn.l2_loss(self.weight),
                                          weight_decay, name='weight_decay_loss_w'))

            # bias vector
            self.bias = tf.Variable(
                initial_value=tf.constant(0.0, shape=[output_dim]),
                dtype=tf.float32, name='bias')
            if weight_decay:
                tf.add_to_collection(
                    'losses', tf.multiply(tf.nn.l2_loss(self.bias),
                                          weight_decay, name='weight_decay_loss_b'))

    def __call__(self, inputs):
        with tf.name_scope('{}_cal'.format(self.name)):
            shape = tf.concat([tf.shape(inputs)[:-1], [tf.shape(self.weight)[-1]]], 0)
            # shape = tf.concat([tf.shape(inputs)[:-1], [self.weight.shape[-1]]], 0)

            inputs = tf.reshape(inputs, [-1, tf.shape(inputs)[-1]])
            outputs = tf.add(tf.matmul(inputs, self.weight), self.bias)
            if self.activation_fn is not None:
                outputs = self.activation_fn(outputs)

            outputs = tf.reshape(outputs, shape)
            return outputs
```

可以看出，`Embedding Layer` 核心思想是维护一个 $N$ 行，每行为 $C$ 维的参数空间， 

输入一个下标矩阵（矩阵可以为一维或多维）——如 [0, 54, 900, 233] 或者 [[0, 2], [3, 6]] 之类， 

返回一个多一维的矩阵——如(10) -> (10, C) 或者 (2, 3) -> (2, 3, C) 之类， 

每个下标 $i$ 被替换成了 $C$ 维的该参数空间内的 第 $i$ 行。
而 `Full Connect Layer` 则是维护一个Weight矩阵和一个bias向量，就是常说的 $y=Wx+b$ 中的那个 $W$ 和 $b$， 

对于输入的 $x$，进行上述计算后输出结果，反传时会同时更新两者。

那么就很明显了，我们的 `Partial Connect Layer` 只需要在 `Full Connect Layer` 的基础上， 

维护两个用来被 `embedding_lookup` 的参数空间 $W$ 和 $b$，在做乘法与加法之前，获得需要计算的行下标， 

采用 `Embedding Layer` 的方法获得实际需要计算的 $W' \subseteq W$ 和 $b' \subseteq b$，输出 $y=W'x+b'$ 即可。
## 0x02 Source Code

```python
class PartialLayer(object):
    """Partial Connect Layer"""
    def __init__(self, input_dim, output_dim, partial_dim,
                 activation_fn=tf.sigmoid, weight_decay=None, name="par"):
        self.partial_dim = partial_dim
        self.activation_fn = activation_fn
        self.weight_shape = (input_dim, output_dim)
        self.name = name

        with tf.name_scope('{}_def'.format(self.name)):
            # weight matrix
            scale = math.sqrt(2.0 / np.prod(self.weight_shape[:-1]))
            init_value = scale * np.random.standard_normal(size=self.weight_shape)
            self.weight = tf.Variable(init_value, dtype=tf.float32, name='weight')

            # bias vector
            self.bias = tf.Variable(
                initial_value=tf.constant(0.0, shape=[output_dim]),
                dtype=tf.float32, name='bias')

            if weight_decay:
                tf.add_to_collection(
                    'losses', tf.multiply(tf.nn.l2_loss(self.weight),
                                          weight_decay, name='weight_decay_loss_w'))
                tf.add_to_collection(
                    'losses', tf.multiply(tf.nn.l2_loss(self.bias),
                                          weight_decay, name='weight_decay_loss_b'))

            self.transposed_weight = tf.transpose(self.weight)
            self.transposed_bias = tf.expand_dims(self.bias, -1)

    def get_partial_weight(self, targets):
        return tf.nn.embedding_lookup(
            self.transposed_weight, targets,
            partition_strategy='mod',
            validate_indices=True,
            max_norm=None,
            name='partial_weight'
        )

    def get_partial_bias(self, targets):
        return tf.nn.embedding_lookup(
            self.transposed_bias, targets,
            partition_strategy='mod',
            validate_indices=True,
            max_norm=None,
            name='partial_bias'
        )

    def __call__(self, inputs, targets):
        """
        global weight is lstm_dim*2, n_words
        :param inputs: batch, seg_len, lstm_dim*2
        :param targets: batch, seg_len, can_len
        :return: batch, seg_len, can_len
        """
        with tf.name_scope('{}_cal'.format(self.name)):
            inputs = tf.expand_dims(inputs, -1)
            # print(inputs.shape)

            partial_weight = self.get_partial_weight(targets)
            partial_bias = self.get_partial_bias(targets)

            # batch*seg_len, can_len
            # print (inputs.shape, partial_weight.shape, partial_bias.shape)
            # print (type(inputs), type(partial_weight), type(partial_bias))

            outputs = tf.add(tf.matmul(partial_weight, inputs), partial_bias)
            if self.activation_fn is not None:
                outputs = self.activation_fn(outputs)

            # batch, seg_len, can_len
            outputs = tf.reshape(outputs, tf.shape(outputs)[:-1])
            return outputs
```

```
# layer initial in network's __init__()
from path.to.my.utils import options
self.partial_layer = PartialLayer(input_dim=2 * options.get('lstm_dim'),
                         output_dim=options.get('n_words'),
                         partial_dim=options.get('max_can_len'),
                         activation_fn=None,
                         weight_decay=self.options.get('weight_decay'),
                         name='pc_layer')
```

```python
# example network construction.
def get_network(self):
    # [batch, seg_len + 2] -> [batch, seg_len + 2, emb_dim]
    word_emb = self.emb_layer(self.input_data)

    # [batch, seg_len(+2), emb_dim] -> [batch, seg_len, lstm_dim*2]
    forward_hidden, backward_hidden = self.lstm_layer(word_emb)
    context_hidden = tf.concat([forward_hidden, backward_hidden])

    # [batch, seg_len, lstm_dim*2] -> [batch, seg_len, can_len]
    partial_hidden = self.partial_layer(context_hidden, self.candidates)
```

## 0x03 后记

简单的测评一下： 

这种方法可以通过下标搜索快速获取需要被计算的区域，大量降低计算，但计算过程中的拼接会产生少量的额外显存消耗， 

占比为candidates的数量和参数空间的行数之比（例如Vocabulary是80k，candidates是80，会额外产生0.1%的显存消耗）。 

此外，对于sparse族的方法我很感兴趣，之后可以好好读读其效果和用法。







