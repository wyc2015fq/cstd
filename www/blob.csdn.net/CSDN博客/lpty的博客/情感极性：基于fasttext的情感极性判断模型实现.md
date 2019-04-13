
# 情感极性：基于fasttext的情感极性判断模型实现 - lpty的博客 - CSDN博客

2017年12月14日 16:33:10[lpty](https://me.csdn.net/sinat_33741547)阅读数：2908所属专栏：[自然语言基础](https://blog.csdn.net/column/details/22512.html)



## 一、前言
本模型主要基于京东评论数据进行训练，相关参考如下：
[1、什么是fasttext？](http://blog.csdn.net/sinat_33741547/article/details/78784234)
[2、什么是情感极性？](http://blog.csdn.net/sinat_33741547/article/details/78731246)
[3、中文分词与jieba](http://blog.csdn.net/sinat_33741547/article/details/78690440)
[4、数据来源](http://blog.csdn.net/sinat_33741547/article/details/78658321)
## 二、实战
### 1、语料处理
语料处理方面，使用jieba分词，添加自定义词典进行分词：

```python
def __load_user_dict(cls):
        """
        加载用户词典
        """
        config = get_config()
        user_dict_path = config.get('train', 'user_dict_path')
        gr = gzip.open(user_dict_path)
        lines = gr.readlines()
        words = set([line.strip() for line in lines if line.strip()])
        user_dict = ['{} {} n'.format(word, len(word)*1000) for word in words]
        buff_file = StringIO('\n'.join(user_dict))
        jieba.load_userdict(buff_file)
        cls._jieba = jieba
        gr.close()
```

### 2、模型训练
模型采用fasttext的分类方法，如下：

```python
def train(cls, input_file, output, **kwargs):
        """
        模型训练
        * input_file             training file path (required)
        * output                 output file path (required)
        * label_prefix           label prefix ['__label__']
        * lr                     learning rate [0.1]
        * lr_update_rate         change the rate of updates for the learning rate [100]
        * dim                    size of word vectors [100]
        * ws                     size of the context window [5]
        * epoch                  number of epochs [5]
        * min_count              minimal number of word occurences [1]
        * neg                    number of negatives sampled [5]
        * word_ngrams            max length of word ngram [1]
        * loss                   loss function {ns, hs, softmax} [softmax]
        * bucket                 number of buckets [0]
        * minn                   min length of char ngram [0]
        * maxn                   max length of char ngram [0]
        * thread                 number of threads [12]
        * t                      sampling threshold [0.0001]
        * silent                 disable the log output from the C++ extension [1]
        * encoding               specify input_file encoding [utf-8]
        * pretrained_vectors     pretrained word vectors (.vec file) for supervised learning []
        """
        config = get_config()
        kwargs.setdefault('lr', config.get('model', 'lr'))
        kwargs.setdefault('lr_update_rate', config.get('model', 'lr_update_rate'))
        kwargs.setdefault('dim', config.get('model', 'dim'))
        kwargs.setdefault('ws', config.get('model', 'ws'))
        kwargs.setdefault('epoch', config.get('model', 'epoch'))
        kwargs.setdefault('word_ngrams', config.get('model', 'word_ngrams'))
        kwargs.setdefault('loss', config.get('model', 'loss'))
        kwargs.setdefault('bucket', config.get('model', 'bucket'))
        kwargs.setdefault('thread', config.get('model', 'thread'))
        kwargs.setdefault('silent', config.get('model', 'silent'))
        cls.__model = ft.supervised(input_file, output, **kwargs)
        return cls.__model
```

### 3、模型测试
使用一批未参与训练的语料，分词后进行模型测试

```python
def test(cls, test_file_path):
        """
        模型测试
        """
        return cls.__model.test(test_file_path)
```

### 4、结果
使用京东80w条评论数据训练，10ｗ条评论数据测试，模型参数如下：

```python
lr = 0.01
lr_update_rate = 100
dim = 300
ws = 5
epoch = 10
word_ngrams = 3
loss = hs
bucket = 2000000
thread = 4
```
效果如下：

```python
('precision:', 0.85055)
('recall:', 0.85055)
('examples:', 100000)
```

## 三、其他
具体代码可在我的github上找到：[https://github.com/lpty/nlp_base](https://github.com/lpty/nlp_base)





