
# 词向量：基于Skip-Gram的Word2Vec神经网络实现 - lpty的博客 - CSDN博客

2017年09月13日 11:42:09[lpty](https://me.csdn.net/sinat_33741547)阅读数：2075所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、前言
1、[理解Word2Vec之Skip-Gram模型](https://zhuanlan.zhihu.com/p/27234078)
## 二、实战
**1、数据源**：[cvpr2016_flowers](https://drive.google.com/file/d/0B0ywwgffWnLLcms2WWJQRFNSWXM/view)，国内下载比较麻烦，数据量也不多，建议用其它数据
**2、数据处理**

```python
class Text:
    def __init__(self):
        self.data_name = 'text_c10'
        self.file_list = self._get_list()
        self.text_list = [self._get_text(file_name) for file_name in self.file_list]
        self.vocab, self.words, self.vocab_to_int, self.int_to_vocab = self._get_words()
        self.batch_size = 200
        self.chunk_size = len(self.words) // self.batch_size
    def _get_list(self):
        # 获取文本列表
        path = os.path.join(os.getcwd(), self.data_name, '*', '*.txt')
        return glob(path)
    def _get_text(self, file_name):
        # 获取文本内容
        f = open(file_name, 'r', encoding='utf-8')
        text = self._process_text(f.read())
        return text
    def _get_words(self, freq=15, t=1e-5, threshold=0.981):
        # 所有词
        all_word = ''.join(self.text_list).split()
        word_counts = Counter(all_word)
        # 剔除出现频率低的词, 减少噪音
        words = [word for word in all_word if word_counts[word] > freq]
        # 统计单词出现频次
        word_counts = Counter(words)
        total_count = len(words)
        # 计算单词频率
        word_freqs = {w: c / total_count for w, c in word_counts.items()}
        # 计算被删除的概率
        prob_drop = {w: 1 - np.sqrt(t / word_freqs[w]) for w in word_counts}
        # 剔除出现频率太高的词
        train_words = [w for w in words if prob_drop[w] < threshold]
        vocab = sorted(set(train_words))
        vocab_to_int = {w: c for c, w in enumerate(vocab)}
        int_to_vocab = {c: w for c, w in enumerate(vocab)}
        return vocab, train_words, vocab_to_int, int_to_vocab
    @staticmethod
    def _get_target(words, index, window_size=8):
        # 获取上下文单词
        window = np.random.randint(1, window_size+1)
        start = index - window if (index - window) else 0
        end = index + window
        targets = set(words[start:index] + words[index+1:end])
        return list(targets)
    def _get_vector(self, words):
        return [self.vocab_to_int[word] for word in words]
    @staticmethod
    def _process_text(text):
        marks = ['.', ',', '"', ';', '!', '?', '(', ')', '--', ':', '-']
        for mark in marks:
            text = text.replace(mark, '')
        return text
    def batch(self):
        # 生成器
        start, end = 0, self.batch_size
        for _ in range(self.chunk_size):
            batch_x, batch_y = [], []
            words = self.words[start:end]
            for index in range(self.batch_size):
                x = words[index]
                y = self._get_target(words, index)
                batch_x.extend([x] * len(y))
                batch_y.extend(y)
            yield self._get_vector(batch_x), self._get_vector(batch_y)
            start += self.batch_size
            end += self.batch_size
```
**3、模型参数定义**

```python
def __init__(self):
        self.train_text = Text()
        self.batch = self.train_text.batch()
        self.batch_size = self.train_text.batch_size
        self.chunk_size = self.train_text.chunk_size
        self.vocab_size = len(self.train_text.vocab)
        # 权重矩阵维度 即最终每个词对应向量维度
        self.embedding_size = 200
        # 负采样数量
        self.sample_size = 100
        # 循环次数
        self.epoch_size = 10
        # 可视化单词数量
        self.viz_words = 100
```
**4、embedding**

```python
def embedding(self, inputs=None):
        # 将int_word转化为embedding_size维度的向量
        # 这也是模型训练完后我们最终想要的矩阵
        with tf.variable_scope('embedding'):
            embedding = tf.get_variable('embedding', [self.vocab_size, self.embedding_size])
            embed = tf.nn.embedding_lookup(embedding, inputs) if inputs is not None else None
        return embedding, embed
```
**5、weight & biases**

```python
def softmax(self):
        w = tf.Variable(tf.truncated_normal([self.vocab_size, self.embedding_size], stddev=0.1))
        b = tf.Variable(tf.zeros(self.vocab_size))
        return
```
**6、损失**

```python
def loss(self, w, b, labels, embed):
        # 采用负样本采样 加快收敛速度
        return tf.reduce_mean(tf.nn.sampled_softmax_loss(weights=w, biases=b, labels=labels, inputs=embed,
                                                         num_sampled=self.sample_size, num_classes=self.vocab_size))
```
**7、最小化损失优化**

```python
def optimizer(self, loss):
        return tf.train.AdamOptimizer().minimize(loss)
```
**8、训练**

```python
# train
        saver = tf.train.Saver()
        sess = tf.Session()
        sess.run(tf.global_variables_initializer())
        step = 0
        for epoch in range(self.epoch_size):
            batch = self.train_text.batch()
            for batch_x, batch_y in batch:
                feed = {inputs: batch_x, labels: np.array(batch_y)[:, None]}
                train_loss, _ = sess.run([loss, optimizer], feed_dict=feed)
                print(datetime.datetime.now().strftime('%c'), ' epoch:', epoch, 'step:', step, ' train_loss:', train_loss)
                step += 1
        model_path = os.getcwd() + os.sep + "skipGramVec.model"
        saver.save(sess, model_path, global_step=step)
        sess.close()
```
**9、结果**
![](https://img-blog.csdn.net/20170913113658572?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
放大一些局部看看
![](https://img-blog.csdn.net/20170913113735577?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)![](https://img-blog.csdn.net/20170913113750062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvc2luYXRfMzM3NDE1NDc=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)
可以看到训练完的权重矩阵已经对词做出了一定的聚类效果
## 三、其他
具体源码可以在我的github上找到：[https://github.com/lpty/tensorflow_tutorial](https://github.com/lpty/tensorflow_tutorial)



