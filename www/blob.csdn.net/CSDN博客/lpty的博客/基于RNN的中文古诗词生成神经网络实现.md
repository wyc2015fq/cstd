
# 基于RNN的中文古诗词生成神经网络实现 - lpty的博客 - CSDN博客

2017年08月29日 19:43:19[lpty](https://me.csdn.net/sinat_33741547)阅读数：6774



## 一、前言
1、[图解RNN](https://zhuanlan.zhihu.com/p/28054589)
2、[Tensorflow中RNN实现的正确打开方式](https://zhuanlan.zhihu.com/p/28196873)
## 二、实战
**1、训练数据处理**
(1)文字转为向量
```python
def _get_poetry(self):
        with open(self.poetry_file, "r", encoding='utf-8') as f:
            poetry_list = [line for line in f]
        return poetry_list
    def _gen_poetry_vectors(self):
        words = sorted(set(''.join(self.poetry_list)+' '))
        # 每一个字符分配一个索引 为后续诗词向量化做准备
        int_to_word = {i: word for i, word in enumerate(words)}
        word_to_int = {v: k for k, v in int_to_word.items()}
        to_int = lambda word: word_to_int.get(word)
        poetry_vectors = [list(map(to_int, poetry)) for poetry in self.poetry_list]
        return poetry_vectors, word_to_int, int_to_word
```
在这里将训练数据中所有的字生成了一个"文字==>数字"的词袋，并将所有的诗词按行分割转化为数字表示。
(2)生成器

```python
def batch(self):
        # 生成器
        start = 0
        end = self.batch_size
        for _ in range(self.chunk_size):
            batches = self.poetry_vectors[start:end]
            # 输入数据 按每块数据中诗句最大长度初始化数组，缺失数据补全
            x_batch = np.full((self.batch_size, max(map(len, batches))), self.word_to_int[' '], np.int32)
            for row in range(self.batch_size): x_batch[row, :len(batches[row])] = batches[row]
            # 标签数据 根据上一个字符预测下一个字符 所以这里y_batch数据应为x_batch数据向后移一位
            y_batch = np.copy(x_batch)
            y_batch[:, :-1], y_batch[:, -1] = x_batch[:, 1:], x_batch[:, 0]
            yield x_batch, y_batch
            start += self.batch_size
            end += self.batch_size
```
x_batch作为输入，y_batch为标签。诗词生成模型根据上一个字符生成下一个字符，所以这里的标签数据应该是和输入数据的shape一致，但序列字符后移一位。y_batch的最后一位，理论上来说应该是本行诗词的下一行的第一个字，简单起见，这里用本行的第一个字代替。
**2、RNN模型**
(1)模型初始化

```python
class PoetryModel:
    def __init__(self):
        # 诗歌生成
        self.poetry = Poetry()
        # 单个cell训练序列个数
        self.batch_size = self.poetry.batch_size
        # 所有出现字符的数量
        self.word_len = len(self.poetry.word_to_int)
        # 隐层的数量
        self.rnn_size = 128
```
rnn_size代表RNN模型中隐层的数量,这个概念要和RNN模型中time_step的数量分开，这里time_step数量指的是每一行诗词的文字数量，即x。batch_size数量指每次训练时，使用的诗词行数。
(2)变量定义

```python
# 输入句子长短不一致 用None自适应
        inputs = tf.placeholder(tf.int32, shape=(self.batch_size, None), name='inputs')
        # 输出为预测某个字后续字符 故输出也不一致
        targets = tf.placeholder(tf.int32, shape=(self.batch_size, None), name='targets')
        # 防止过拟合
        keep_prob = tf.placeholder(tf.float32, name='keep_prob')
```
(3)embedding定义

```python
def embedding_variable(inputs, rnn_size, word_len):
        with tf.variable_scope('embedding'):
            # 这里选择使用cpu进行embedding
            with tf.device("/cpu:0"):
                # 默认使用'glorot_uniform_initializer'初始化，来自源码说明:
                # If initializer is `None` (the default), the default initializer passed in
                # the variable scope will be used. If that one is `None` too, a
                # `glorot_uniform_initializer` will be used.
                # 这里实际上是根据字符数量分别生成state_size长度的向量
                embedding = tf.get_variable('embedding', [word_len, rnn_size])
                # 根据inputs序列中每一个字符对应索引 在embedding中寻找对应向量,即字符转为连续向量:[字]==>[1]==>[0,1,0]
                lstm_inputs = tf.nn.embedding_lookup(embedding, inputs)
        return lstm_inputs
```
将诗词的文字对应索引分别转化为变量，引入另一个新概念，input_size，即每一个字所对应的变量长度，这里是训练数据中所有字符的数量，所以实际上我们每次训练使用的数据shape应该是(batch_size，time_step，input_size)。
(4)模型计算图定义

```python
def rnn_graph(self, batch_size, rnn_size, word_len, lstm_inputs, keep_prob):
        # cell.state_size ==> 128
        # 基础cell 也可以选择其他基本cell类型
        lstm = tf.nn.rnn_cell.BasicLSTMCell(num_units=rnn_size)
        drop = tf.nn.rnn_cell.DropoutWrapper(lstm, output_keep_prob=keep_prob)
        # 多层cell 前一层cell作为后一层cell的输入
        cell = tf.nn.rnn_cell.MultiRNNCell([drop] * 2)
        # 初始状态生成(h0) 默认为0
        # initial_state.shape ==> (64, 128)
        initial_state = cell.zero_state(batch_size, tf.float32)
        # 使用dynamic_rnn自动进行时间维度推进 且 可以使用不同长度的时间维度
        # 因为我们使用的句子长度不一致
        lstm_outputs, final_state = tf.nn.dynamic_rnn(cell, lstm_inputs, initial_state=initial_state)
        seq_output = tf.concat(lstm_outputs, 1)
        x = tf.reshape(seq_output, [-1, rnn_size])
        # softmax计算概率
        w, b = self.soft_max_variable(rnn_size, word_len)
        logits = tf.matmul(x, w) + b
        prediction = tf.nn.softmax(logits, name='predictions')
        return logits, prediction, initial_state, final_state
```
(5)权重及偏置定义

```python
def soft_max_variable(rnn_size, word_len):
        # 共享变量
        with tf.variable_scope('soft_max'):
            w = tf.get_variable("w", [rnn_size, word_len])
            b = tf.get_variable("b", [word_len])
        return w, b
```
RNN与CNN不同的一点在于，RNN的权重及偏置在所有cell中是一样的，这里使用了共享变量。
(6)损失及优化图定义

```python
@staticmethod
    def loss_graph(word_len, targets, logits):
        # 将y序列按序列值转为one_hot向量
        y_one_hot = tf.one_hot(targets, word_len)
        y_reshaped = tf.reshape(y_one_hot, [-1, word_len])
        loss = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=logits, labels=y_reshaped))
        return loss
    @staticmethod
    def optimizer_graph(loss, learning_rate):
        grad_clip = 5
        # 使用clipping gradients
        tvars = tf.trainable_variables()
        grads, _ = tf.clip_by_global_norm(tf.gradients(loss, tvars), grad_clip)
        train_op = tf.train.AdamOptimizer(learning_rate)
        optimizer = train_op.apply_gradients(zip(grads, tvars))
        return optimizer
```
RNN会遇到梯度爆炸（gradients exploding）和梯度弥散（gradients disappearing)的问题。LSTM解决了梯度弥散的问题，但是gradients仍然可能会爆炸，因此我们采用gradient
 clippling的方式来防止梯度爆炸。即通过设置一个阈值，当gradients超过这个阈值时，就将它重置为阈值大小，这就保证了梯度不会变得很大。
(7)开始训练
```python
# 开始训练
        saver = tf.train.Saver()
        sess = tf.Session()
        sess.run(tf.global_variables_initializer())
        step = 0
        new_state = sess.run(initial_state)
        for i in range(epoch):
            # 训练数据生成器
            batches = self.poetry.batch()
            # 随模型进行训练 降低学习率
            sess.run(tf.assign(learning_rate, 0.001 * (0.97 ** i)))
            for batch_x, batch_y in batches:
                feed = {inputs: batch_x, targets: batch_y, initial_state: new_state, keep_prob: 0.5}
                batch_loss, _, new_state = sess.run([loss, optimizer, final_state], feed_dict=feed)
                print(datetime.datetime.now().strftime('%c'), ' i:', i, 'step:', step, ' batch_loss:', batch_loss)
                step += 1
        model_path = os.getcwd() + os.sep + "poetry.model"
        saver.save(sess, model_path, global_step=step)
        sess.close()
```
随着模型的训练，逐步降低学习率。
(8)生成古诗词
```python
def gen(self, poem_len):
        def to_word(weights):
            t = np.cumsum(weights)
            s = np.sum(weights)
            sample = int(np.searchsorted(t, np.random.rand(1) * s))
            return self.poetry.int_to_word[sample]
        # 输入
        # 句子长短不一致 用None自适应
        self.batch_size = 1
        inputs = tf.placeholder(tf.int32, shape=(self.batch_size, 1), name='inputs')
        # 防止过拟合
        keep_prob = tf.placeholder(tf.float32, name='keep_prob')
        lstm_inputs = self.embedding_variable(inputs, self.rnn_size, self.word_len)
        # rnn模型
        _, prediction, initial_state, final_state = self.rnn_graph(self.batch_size, self.rnn_size, self.word_len, lstm_inputs, keep_prob)
        saver = tf.train.Saver()
        with tf.Session() as sess:
            sess.run(tf.global_variables_initializer())
            saver.restore(sess, tf.train.latest_checkpoint('.'))
            new_state = sess.run(initial_state)
            # 在所有字中随机选择一个作为开始
            x = np.zeros((1, 1))
            x[0, 0] = self.poetry.word_to_int[self.poetry.int_to_word[random.randint(1, self.word_len-1)]]
            feed = {inputs: x, initial_state: new_state, keep_prob: 1}
            predict, new_state = sess.run([prediction, final_state], feed_dict=feed)
            word = to_word(predict)
            poem = ''
            while len(poem) < poem_len:
                poem += word
                x = np.zeros((1, 1))
                x[0, 0] = self.poetry.word_to_int[word]
                feed = {inputs: x, initial_state: new_state, keep_prob: 1}
                predict, new_state = sess.run([prediction, final_state], feed_dict=feed)
                word = to_word(predict)
            return poem
```
在sep-sep模型中，上一个网络的状态输出应该作为下一个网络的状态输入，所以初始状态为零，后续输入状态都是上一次的输出状态。
(9)结果

```python
筋烛升玄净，云光理片溪。
美子相离处，离来下路疏。
策变因坚薛，灵行一更颇。
卖上狎城天，穿洄欹笔软。
授名时依寂，快举即芳眉。
吊此之当主，长期动世迟。
```
看起来有模有样的，基本模型是成功了。在这个模型的基础上进行改动，还可以生成小说等。
**三、其他**
具体源码可以在我的github上找到：[https://github.com/lpty/tensorflow_tutorial](https://github.com/lpty/tensorflow_tutorial)






