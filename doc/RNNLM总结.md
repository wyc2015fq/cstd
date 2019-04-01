# RNNLM



# 深度学习-->NLP-->RNNLM实现

2017年11月20日 17:40:39 [村头陶员外](https://me.csdn.net/Mr_tyting) 阅读数：1147



 版权声明：本文为博主原创文章，未经博主允许不得转载。	https://blog.csdn.net/Mr_tyting/article/details/78579555

本篇博文将详细总结RNNLMRNNLM 的原理以及如何在tensorflowtensorflow 上实现RNNLMRNNLM。

我们要实现的网络结构如下：

![这里写图片描述](https://img-blog.csdn.net/20171120101106859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

## **数据预处理**

### 创建vocabvocab

#### 分词：

将句子中的每个单词以空格，符号分开，形成一个单词列表

```
def blank_tokenizer(sentence):
    ##以空格对句子进行切分
    return sentence.strip().split()

def basic_tokenizer(sentence):
    '''
    _WORD_SPLIT=re.compile(b"([.,!?\"':;)(])")
    首先以空格对句子进行切分，然后再以标点符号切分，切分出一个个词，然后词列表
    '''
    words=[]
    for space_separated_fragment in sentence.strip().split():
        words.extend(_WORD_SPLIT.split(space_separated_fragment))
    return [w for w in words if w]12345678910111213
```

#### 对单词列表添加特殊词汇：

- _PAD_PAD 填充词汇
- _GO_GO 句子开始
- _EOS_EOS 句子结束
- _UNK_UNK 未知词(低频的词替换为UNK)

如"i love you""i love you" 创建成vocabvocab 时，应为： 
"_GO i love you _EOS"_GO i love you _EOS

### 将单词替换成数字

对vocabvocab 内的单词按出现频率排序，用其索引代替单词。 
如：1 3 102 3424 2

```
def create_vocabulary(vocabulary_path,data_paths,max_vocabulary_size,tokenizer=None,normalize_digits=False):
    '''
    读取data_paths路径下的文件，并且一行行的读取，对每句做分词处理，得到每个词的频率，然后存储频率最高的max_vocabulary_size的词，存入vocabulary_path
    :param vocabulary_path: 新建的文件夹，将返回的结果写入
    :param data_paths:存储原始文件的路径
    :param max_vocabulary_size:最大存储的词的个数
    :param tokenizer:对句子做分词处理
    :param normalize_digits:是否对句子中的数字以0替换
    :return:返回的vocabulary_path中一行一个词
    '''
    if not gfile.Exists(vocabulary_path):
        print ("Create vocabulary %s from data %s" %(vocabulary_path,",".join(data_paths)))
        vocab={}
        for data_path in data_paths:
            with gfile.GFile(data_path,mode='rb') as f:
                print (data_path)
                counter=0
                for line in f:
                    counter+=1
                    if counter%100000==0:
                        print ("processing line %d" %counter)
                    #Converts either bytes or unicode to bytes, using utf-8 encoding for text.
                    line=tf.compat.as_bytes(line)
                    tokens=tokenizer(line) if tokenizer else blank_tokenizer(line)
                    for w in tokens:
                        #replace digit to 0
                        #_DIGIT_RE=re.compile(br"\d")
                        word=_DIGIT_RE.sub(b"0",w) if normalize_digits else w
                        if word in vocab:
                            vocab[word]+=1
                        else:
                            vocab[word]=1
                print (len(vocab))
        # _START_VOCAB=[_PAD,_GO,_EOS,_UNK]
        # 按词频率降序排序
        vocab_list=_START_VOCAB+sorted(vocab,key=vocab.get,reverse=True)
        if len(vocab_list)>max_vocabulary_size:
            vocab_list=vocab_list[:max_vocabulary_size]##只取出现频率最高的max_vocabulary_size
        with gfile.GFile(vocabulary_path,mode='rb') as vocab_file:
            for w in vocab_list:
                vocab_file.write(w+b'\n')##注意将分出的单词一行一行的写入到vocabulary_path


def initialize_vocabulary(vocabulary_path):
    '''
    :param vocabulary_path:一行一个词
    读取vocabulary_path文件内每行的每个单词到rev_vocab，然后枚举rev_vocab，然后字典列表[(word,index)]
    :return:
    '''
    if gfile.Exists(vocabulary_path):
        rev_vocab=[]
        with gfile.GFile(vocabulary_path,mode='rb') as f:
            rev_vocab.extend(f.readlines())
        rev_vocab=[tf.compat.as_bytes(line.strip()) for line in rev_vocab]
        vocab=dict([(x,y) for (y,x) in enumerate(rev_vocab)])
        return vocab,rev_vocab
    else:
        raise ValueError("Vocabulary file % not found",vocabulary_path)


def sentence_to_token_ids(sentence,vocabulary,tokenizer=None,normalize_digits=False,with_start=True,with_end=True):
    '''
    对sentence句子进行分词处理，并且用其在vocabulary中的索引代替其词，并且加上GO_ID,EOS_ID,UNK等特殊数字，返回数字列表。
    :param sentence:需要分词的句子
    :param vocabulary:字典列表[(word,index)]
    :param tokenizer:分词处理方法
    :param normalize_digits:是否将句子中数字用0替换
    :param with_start:是否在句头带上GO_ID
    :param with_end:是否在句尾带上EOS_ID
    :return:
    '''
    if tokenizer:
        #对sentence进行分词处理
        words=tokenizer(sentence)
    else:
        # 对sentence进行分词处理
        words=basic_tokenizer(sentence)
    if not normalize_digits:
        #在vocabulary中找到Word，返回其index,否则以UNK_ID代替返回
        #UNK_ID=3
        ids=[vocabulary.get(w,UNK_ID) for w in words]
    else:
        #_DIGIT_RE=re.compile(br"\d")
        ids=[vocabulary.get(_DIGIT_RE.sub(b"0",w),UNK_ID) for w in words]

    if with_start:
        ids=[GO_ID]+ids
    if with_end:
        ids=ids+[EOS_ID]
    return ids


def data_to_token_ids(data_path,target_path,vocabulary_path,tokenizer=None,normalize_digits=False,with_go=True,with_end=True):
    '''
    读取data_path路径下的文件内容，读取其每一行，喂给sentence_to_token_ids方法处理，得到所有词的索引列表，然后存入到target_path
    :param data_path:原文件
    :param target_path:原文件处理完要存入的地址
    :param vocabulary_path:一行一个词
    :param tokenizer:
    :param normalize_digits:
    :param with_go:
    :param with_end:
    :return:
    '''
    if not gfile.Exists(target_path):
        print ("Tokenizing data in %s" % data_path)
        vocab,_=initialize_vocabulary(vocabulary_path)
        #vocab是字典列表[(word,index)]
        with gfile.GFile(data_path,mode='rb') as data_file:
            with gfile.GFile(target_path,mode='w') as tokens_file:
                counter=0
                for line in data_file:
                    counter+=1
                    if counter%100000==0:
                        print ("tokenizing line %d" % counter)
                    token_ids=sentence_to_token_ids(tf.compat.as_bytes(line),vocab,tokenizer,normalize_digits)
                    tokens_file.write(" ".join([str(tok) for tok in token_ids])+'\n')#注意一行一句话123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117
```

## **训练RNN模型**

### Mini−batch Gradient DescentMini−batch Gradient Descent 梯度下降法

适当的条件更新learning rate ηlearning rate η，直到收敛。 
适当的条件: 
每处理了一半的训练数据,就去**验证集** 计算perplexityperplexity

- 如果perplexityperplexity 比上次下降了,保持learning ratelearning rate不变, 记录下现在最好的参数。
- 否则, learning rate∗=0.5learning rate∗=0.5 缩小一半。

如果连续10次learning ratelearning rate 没有变,就停止训练。

1. 读取训练数据 traintrain 和验证数据devdev
2. 建立模型; patience=0patience=0
3. whilewhile 
   从数据中随机取mm 个句子进行训练 
   到达半个epochepoch，计算ppx(dev)ppx(dev) 
     比之前降低：更新best parametersbest parameters，patience=0patience=0 
     比之前升高：learning ratelearning rate 减半，patience+=1patience+=1 
   if (patience>10):breakif (patience>10):break

### **mini−batchmini−batch 在RNNRNN 上问题**

句子的长度不一样

![这里写图片描述](https://img-blog.csdn.net/20171120114954684?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

解决方法：句子的长度不一样: 增加paddingpadding

![这里写图片描述](https://img-blog.csdn.net/20171120115312417?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

**lossloss 增大了**





loss=logP(I)+logP(like)+logP(it)+logP(.)+logP(_EOS)+logP(YES)+logP(_EOS)+logP(_PAD)+logP(_PAD)+logP(_PAD)loss=logP(I)+logP(like)+logP(it)+logP(.)+logP(_EOS)+logP(YES)+logP(_EOS)+logP(_PAD)+logP(_PAD)+logP(_PAD)



**解决方法：乘以一个0/1 mask矩阵**





LOSS=[[logP(I),logP(like),logP(it),logP(.),logP(_EOS)],[logP(YES),logP(_EOS),logP(_PAD),logP(_PAD),logP(_PAD)]]∗[[1,1,1,1,1],[1,1,0,0,0]]=logP(I)+logP(like)+logP(it)+logP(.)+logP(_EOS)+logP(YES)+logP(_EOS)LOSS=[[logP(I),logP(like),logP(it),logP(.),logP(_EOS)],[logP(YES),logP(_EOS),logP(_PAD),logP(_PAD),logP(_PAD)]]∗[[1,1,1,1,1],[1,1,0,0,0]]=logP(I)+logP(like)+logP(it)+logP(.)+logP(_EOS)+logP(YES)+logP(_EOS)



**效率过低问题**

随之而来另外一个问题，我们在增加paddingpadding 填充时，以什么样的标准长度进行填充？以所有句子中最长长度进行填充？

例如：我们有长度为10的句子有1101句，长度为11的句子有1226句，长度为81的只有一句，长度为82的也只有1句，那么我们尝试将所有句子补齐到82个字。

- 实际计算了(1101++1226+1+1) * 82 = 190978 步
- 有效的步数:1101*10 +1226 * 11 + 1* 81+ 1*82 = 24659
- 利用率: 12.9% 浪费!

**解决低效问题** 
将句子分成两组, 一组补齐到11,一组补齐到82，相当于建两个RNN，一个11步，另外一个82步。

- (1101+1226) * 11 + (1+1)*82 = 25761
- 利用率: 24659 / 25761 = 95.7%

当然也可以建四个RNN，分别为11步，10步，81步，82步，这样效率就到达100%了。但是显然四个RNN训练比较耗时耗存。

显然，这就有一个问题了，该如何决定分组个数？该如何决定每组的应补齐的步长。

### **best_buckets问题**

这里采用一种贪心算法，贪心的最后结果可能不是全局最优，但肯定不会太差。

我们以下为例： 
length_arraylength_array：表示所有句子长度的列表。 
length_array=[1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,4,4]length_array=[1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,4,4]

max_bucketsmax_buckets：表示计划分的组数 
max_buckets=3max_buckets=3

max_lengthmax_length：表示最长的句子长度 
max_length=4max_length=4

running_sumrunning_sum：元祖列表形式。表示长度小于等于1的有5句，长度小于等于有15句,…. 
running_sum=[(1,5),(2,15),(3,18),(4,20)]running_sum=[(1,5),(2,15),(3,18),(4,20)]

下面是尝试分组： 
①：不作分组，相当于只分一组。 
  running_sum=[(1,5),(2,15),(3,18),(4,20)]running_sum=[(1,5),(2,15),(3,18),(4,20)] 
  灰色面积是 **有效计算步数** 
  空白面积是 **无效计算步数**

![这里写图片描述](https://img-blog.csdn.net/20171120140939650?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

横坐标：running_sumrunning_sum 所有元组的第一个数。 
纵坐标：running_sumrunning_sum 所有元组的第二个数。

由图可以看出这种分组方式效率较低。

②分为两组。 
  如果buckets = [2,4]; 
  实际 = 红框 – 红色区域 
  红色区域：在当前这种分组下，可以去掉的无效计算。

![这里写图片描述](https://img-blog.csdn.net/20171120141916008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果buckets = [3,4]

![这里写图片描述](https://img-blog.csdn.net/20171120142552761?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

如果buckets = [1,4]

![这里写图片描述](https://img-blog.csdn.net/20171120142804594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

比较以上三种二分方式，得出以句子长度为2划分方式效率最高。然后我们再尝试在这中最优二分划分方式基础上再进行划分。

③分为三组。在buckets = [2,4]基础上载进行划分分组。 
  如果buckets = [2,4,3] 
  实际 = 红框 – 红色区域 
  红色区域：在当前这种分组下，可以去掉的无效计算。

![这里写图片描述](https://img-blog.csdn.net/20171120143553024?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

buckets = [2,4,1]

![这里写图片描述](https://img-blog.csdn.net/20171120143710679?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvTXJfdHl0aW5n/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

比较以上两种三分组划分方式，显然最好的buckets = [1,2,4]。

```
def calculate_buckets(length_array, max_length, max_buckets):
    '''

    :param length_array:所有句子的长度列表[1,1,1,1,1,2,2,2,2,2,2,2,2,2,2,3,3,3,4,4]
    :param max_length:最长句子的长度4
    :param max_buckets:分为几个组
    :return:
    '''
    d = {}
    for length in length_array:
        if not length in d:
            d[length] = 0
        d[length] += 1

    #dd:[(句子长度,该长度出现次数)]
    dd = [(x, d[x]) for x in d]
    dd = sorted(dd, key=lambda x: x[0])##以长度升序排序

    #计算running_sum
    running_sum = []
    s = 0
    for l, n in dd:
        s += n
        running_sum.append((l, s))#running_sum = [(1,5),(2,15),(3,18),(4,20)]

    def best_point(ll):
        ## ll即running_sum：[(句子长度,小于等于该长度出现次数)]
        #找出最大可以去掉的无效面积
        index = 0
        maxv = 0
        base = ll[0][1]
        for i in xrange(len(ll)):
            l, n = ll[i]
            v = (ll[-1][0] - l) * (n - base)
            if v > maxv:
                maxv = v
                index = i
        return index, maxv

    def arg_max(array, key):
        # 找出最大可以去掉的无效面积
        maxv = -10000
        index = -1

        for i in xrange(len(array)):
            item = array[i]
            v = key(item)
            if v > maxv:
                maxv = v
                index = i
        return index

    end_index = 0
    for i in xrange(len(running_sum) - 1, -1, -1):
        if running_sum[i][0] <= max_length:
            end_index = i + 1
            break

    # print "running_sum [(length, count)] :"
    # print running_sum

    if end_index <= max_buckets:
        buckets = [x[0] for x in running_sum[:end_index]]
    else:
        '''
        不断递归的以可以去掉最大的无效面积为原则不断的划分
        '''
        buckets = []
        # (array,  maxv, index)
        states = [(running_sum[:end_index], 0, end_index - 1)]#[([(1,5),(2,15),(3,18),(4,20)],0,end_index-1)],列表长度为1
        while len(buckets) < max_buckets:
            index = arg_max(states, lambda x: x[1])##最大可以去掉的无效面积对应的索引
            state = states[index]
            del states[index]
            # split state
            array = state[0]
            split_index = state[2]
            buckets.append(array[split_index][0])
            array1 = array[:split_index + 1]
            array2 = array[split_index + 1:]
            if len(array1) > 0:
                id1, maxv1 = best_point(array1)
                states.append((array1, maxv1, id1))
            if len(array2) > 0:
                id2, maxv2 = best_point(array2)
                states.append((array2, maxv2, id2))
    return sorted(buckets)

def split_buckets(array, buckets, withOrder=False):
    """

    :param array:句子的集合
    :param buckets:上面计算出来的最优划分组
    :param withOrder:
    :return:d[buckets_id,属于该组的items];order((buckets_id,len(d[buckets_id]) - 1))
    """
    order = []
    d = [[] for i in xrange(len(buckets))]
    for items in array:
        index = get_buckets_id(len(items), buckets)
        if index >= 0:
            d[index].append(items)
            order.append((index, len(d[index]) - 1))
    return d, order


def get_buckets_id(l, buckets):
    '''
    将某句子长度划到对应的分组中，返回该句子的组号
    :param l:
    :param buckets:
    :return:
    '''
    id = -1
    for i in xrange(len(buckets)):
        if l <= buckets[i]:
            id = i
            break
    return id123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960616263646566676869707172737475767778798081828384858687888990919293949596979899100101102103104105106107108109110111112113114115116117118119
```

我们计算处buckets，需要对其中不同的bucket建立不同步长的RNN模型。并且在对不同模型的loss求和。

```
    def model_with_buckets(self, inputs, targets, weights,
                           buckets, cell, dtype,
                           per_example_loss=False, name=None, devices=None):

        all_inputs = inputs + targets + weights

        losses = []
        hts = []
        logits = []
        topk_values = []
        topk_indexes = []

        # initial state
        with tf.device(devices[1]):
            init_state = cell.zero_state(self.batch_size, dtype)

        # softmax
        with tf.device(devices[2]):
            softmax_loss_function = lambda x, y: tf.nn.sparse_softmax_cross_entropy_with_logits(logits=x, labels=y)

        with tf.name_scope(name, "model_with_buckets", all_inputs):
            for j, bucket in enumerate(buckets):
                with variable_scope.variable_scope(variable_scope.get_variable_scope(), reuse=True if j > 0 else None):

                    # ht
                    with tf.device(devices[1]):
                        _hts, _ = tf.contrib.rnn.static_rnn(cell, inputs[:bucket], initial_state=init_state)
                        hts.append(_hts)

                    # logits / loss / topk_values + topk_indexes
                    with tf.device(devices[2]):
                        _logits = [tf.add(tf.matmul(ht, tf.transpose(self.output_embedding)), self.output_bias) for ht
                                   in _hts]
                        logits.append(_logits)

                        if per_example_loss:
                            losses.append(sequence_loss_by_example(
                                logits[-1], targets[:bucket], weights[:bucket],
                                softmax_loss_function=softmax_loss_function))

                        else:
                            losses.append(sequence_loss(
                                logits[-1], targets[:bucket], weights[:bucket],
                                softmax_loss_function=softmax_loss_function))

                        topk_value, topk_index = [], []

                        for _logits in logits[-1]:
                            value, index = tf.nn.top_k(tf.nn.softmax(_logits), self.topk_n, sorted=True)
                            topk_value.append(value)
                            topk_index.append(index)
                        topk_values.append(topk_value)
                        topk_indexes.append(topk_index)

        self.losses = losses
        self.hts = hts
        self.logits = logits
        self.topk_values = topk_values
        self.topk_indexes = topk_indexes
123456789101112131415161718192021222324252627282930313233343536373839404142434445464748495051525354555657585960
```

### **如何随机选择m个数据?**

inputs, outputs, weights, _ = self.model.get_batch(self.data_set, bucket_id)

1. 先随机一个buckets
2. 再随机取m个数据
3. 将m个数据变成一个矩阵,加上padding

```
    def get_batch(self, data_set, bucket_id, start_id=None):
        '''
        :param data_set:[ [ s1,s1,s1,s1,s1] , [s2,s2,s2,s2,s2,s2,s2,s2,s2,s2],
[s3,s3,s3,s4,s4] ]，注意每个字母表示一个句子。
        :param bucket_id:第几个分组
        :param buckets:[1,2,4]
        :param batch_size
        :param start_id:
        :return:
        '''
        length = self.buckets[bucket_id]##当前组的句子长度，即需要补齐的长度

        input_ids, output_ids, weights = [], [], []

        for i in xrange(self.batch_size):##获取batch_size个句子。
            if start_id == None:
                word_seq = random.choice(data_set[bucket_id])
            else:
                if start_id + i < len(data_set[bucket_id]):
                    word_seq = data_set[bucket_id][start_id + i]
                else:
                    word_seq = []

            word_input_seq = word_seq[:-1]  # without _EOS
            word_output_seq = word_seq[1:]  # target without _GO

            target_weight = [1.0] * len(word_output_seq) + [0.0] * (length - len(word_output_seq))
            word_input_seq = word_input_seq + [self.PAD_ID] * (length - len(word_input_seq))
            word_output_seq = word_output_seq + [self.PAD_ID] * (length - len(word_output_seq))

            input_ids.append(word_input_seq)
            output_ids.append(word_output_seq)
            weights.append(target_weight)

        # Now we create batch-major vectors from the data selected above.
        def batch_major(l):
            output = []
            for i in xrange(len(l[0])):
                temp = []
                for j in xrange(self.batch_size):
                    temp.append(l[j][i])
                output.append(temp)
            return output

        batch_input_ids = batch_major(input_ids)
        batch_output_ids = batch_major(output_ids)
        batch_weights = batch_major(weights)

        finished = False
        if start_id != None and start_id + self.batch_size >= len(data_set[bucket_id]):
            finished = True

        return batch_input_ids, batch_output_ids, batch_weights, finished1234567891011121314151617181920212223242526272829303132333435363738394041424344454647484950515253
```

### 模型训练

```python
    def step(self, session, inputs, targets, target_weights,
             bucket_id, forward_only=False, dump_lstm=False):

        length = self.buckets[bucket_id]

        input_feed = {}
        for l in xrange(length):
            input_feed[self.inputs[l].name] = inputs[l]
            input_feed[self.targets[l].name] = targets[l]
            input_feed[self.target_weights[l].name] = target_weights[l]

        # output_feed
        if forward_only:
            output_feed = [self.losses[bucket_id]]
            if dump_lstm:
                output_feed.append(self.states_to_dump[bucket_id])

        else:
            output_feed = [self.losses[bucket_id]]
            output_feed += [self.updates[bucket_id], self.gradient_norms[bucket_id]]

        outputs = session.run(output_feed, input_feed, options=self.run_options, run_metadata=self.run_metadata)

        if forward_only and dump_lstm:
            return outputs
        else:
            return outputs[0]  # only return losses
```

## **总结**

1. 分词 
   将所有句子按空格，符号切分成单词列表，转成数字，并添加上特殊数字。然后再按照已经获取的单词和其对应的数字元组列表，将指定的文件内容进行转换，以一句话作为单位进行转换，存到指定文件内，并且一行一句话。
2. 分组 
   计算获取best_bucketsbest_buckets，然后还需要对上面获取的分词结果按照句子长度和best_bucketsbest_buckets进行分组，如：[ [ s1,s1,s1,s1,s1] , [s2,s2,s2,s2,s2,s2,s2,s2,s2,s2],[s3,s3,s3,s4,s4] ]，每一个字母表示一句话。
3. 随机选取m个样本 
   随机选择bucket_idbucket_id，然后在该组内随机选取m个样本，即m个句子，得到每个句子对应的InputInput和outputoutput，并计算出该句对应的mask矩阵。
4. 如果分为n组，则需要训练n个RNN模型。将上面所得的训练样本丢进对应RNN模型中进行训练预测。并且计算loss之和。









# rwthlm工具包安装步骤

2015年04月18日 15:39:18 [a635661820](https://me.csdn.net/a635661820) 阅读数：2199



本篇介绍rwthlm在linux上的安装，我用的系统是ubuntu, 在windows上的安装我没去试，可以参考看看[这个博客](http://blog.csdn.net/yuntrans/article/details/43371081)。





**1.首先编译器得支持c++11, 可以在终端中输入**

g++ -v

在我的机器上得到如下结果：gcc version 4.8.2 (Ubuntu 4.8.2-19ubuntu1) ，官方文档上写的是g++ 4.6.3 or 4.8.0





**2.安装GSL(也可以不安装GSL，选择Intel Composer XE,或AMD的)，ubuntu下安装gsl特别方便，输入下面两条命令直接搞定：**

apt-get install libgsl0-dev
apt-get install libgsl0ldbl





**3.安装boost,要求是version 1.53.0 or 1.55.0**
我这边用的是1.53.0版本的，点击进入[下载地址](http://sourceforge.net/projects/boost/files/boost/1.53.0/)， 我下载的文件是boost_1_53_0.tar.gz。将这个包解压，安装下面的步骤来：

> \1. 打开tools/build/v2/user-config.jam准备进行一些更改
>
> \2. 更改user-config.jam内容中# using gcc : 3.2 : g++-3.2 ;换行输入如下：
>
> ​      using gcc
>
> ​          :4.8.2
>
> ​          : "g++"
>
> ​          : <cxxflags>-std=c++11 -O2 -mtune=native
>
> ​          ;
>
> 其中gcc后面的4.8.2是以自己的机器为准的，仍然可用g++ -v来查看
>
> \3. 到目录boost_1_53_0/下面，执行下面的命令：
>
> ./bootstrap.sh --with-icu --prefix=/opt/boost/boost_1_53_0
>
> 注意ubuntu用户尽量切换到su，否则可能会造成没有足够权限来对安装文件进行拷贝。这里--frefix后面指定的是boost安装的路径，如果不指定的话，默认安装头文件在目录/usr/local/include，而库文件在目录/usr/local/lib。这里这样指定--frefix是为了和rwthlm的makefile对应起来，**如果这里是安装的路径默认的话，请注意后面rwthlm的makefile相应指定的boost路径也会不一样，需要手动更改。**
>
> \4. 最后执行下面命令，就完成boost的安装了，一般需要等待10多分钟：
>
> ./b2 install

另外注意ubuntu上安装boost可以用直接用下面命令很快搞定(我没有尝试这种，因为看到网上有人说什么这种方式没开启mpi什么的)：

apt-get install libboost-dev

如果安装过程中报错比较多，导致安装boost失败，看看自己是否少一些bz2，或者python相关的，可以试试sudo apt-get install python2.7-dev，或者sudo apt-get install libboost-mpi-python-dev，sudo apt-get install libboost-python-dev，sudo apt-get install bzip2-devel。这些命令我是在网上查的，具体我也不太了解，放上来仅供参考。



**4.将gsl文件夹中的fast.h, Makefile, random.h放到外层的目录来，覆盖原来的相关文件**



**5.更改上面的Makefile文件**

需要更改的地方应该只有两个，一个是其中的

BOOST =   /opt/boost/boost_1_53_0

我安装的boost的路径就是这个，所以无需更改，如果是默认安装的，应该设置成：

BOOST =  /usr/local/

另外一个需要更改的地方是：

CXXFLAGS = -std=c++0x -O2 -m64 -mtune=native -fopenmp \

如果机器是32位的话，应该改为：

CXXFLAGS = -std=c++0x -O2 -m32 -mtune=native -fopenmp \



**6.编译，直接终端输入：**

make -j



结果会生成二进制的rwthlm，，这样rwthlm的安装就完成了，结果如下：



![img](https://img-blog.csdn.net/20150418164509280?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)









# A Toolkit For Language Modeling rnnlm

# 基本功能命令详细介绍

 







*本文来自CSDN博客，转载请注明出处：http://blog.csdn.net/a635661820/article/details/44748583*

参考文献：RNNLM - Recurrent Neural Network Language Modeling Toolkit（[点此打开](http://research.microsoft.com/pubs/175562/ASRU-Demo-2011.pdf)）



参考文献：STATISTICAL LANGUAGE MODELS BASED ON NEURAL NETWORKS（[点此打开](http://www.fit.vutbr.cz/~imikolov/rnnlm/google.pdf)）







​      由Mikolov用rnn对语言进行建模所获得的效果已经超过传统的N-GRAM了，具体的实验结果在他的系列论文中都有解释，不过现在可以看看 他的PHD论文（[点此打开](http://www.fit.vutbr.cz/~imikolov/rnnlm/google.pdf)），里面有各种对比实验。随即之后，他将rnnlm toolkit开源，可供其他人作为一个baseline，[点此进入](http://rnnlm.org/)rnnlm toolkit的下载地址

​     这一篇文章是简要介绍rnnlm tookit的使用的，下面的内容我会把下载好的开源包里面的example.sh的shell脚本命令分开执行，并且详细说明每条命令所带参数的含义，更高级的用处在下载地址那个网页里面有专门的例子。由于可供选择的命令参数确实很多，不会全部都写，更详细的可以看一看相关文档，以及源代码。本文旨在把rnnlm最基本的功能介绍清楚，好了，下面是正文，初次写博客不久，如果有哪里错误的地方，还请大家多多指正哈。







1.编译

我的软件包版本是0.4b的，首先在在终端中进入到rnnlm-0.4b的目录(我的版本是0.4b的)，然后编译，执行下面的命令：

make



有可能会出现如下错误：





make: x86_64-linux-g++-4.6: Command not found

make: *** [rnnlmlib.o] Error 127

这个时候可以将makefile文件的内容改一下：



CC = x86_64-linux-g++-4.6 

更改为==>

CC = g++

重新执行即可，这里的错误处理参考了(http://blog.csdn.net/wangxinginnlp/article/details/17754173 )





2.用rnnlm训练模型





我们先将训练数据放上来，内容大概如下：









![img](https://img-blog.csdn.net/20150330115109221?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

也就是train文件里面的训练数据都是一个一个的句子，注意无论是训练数据还是测试数据，或是验证数据最后一行都最好换行。然后执行下列命令：





./rnnlm -train train -valid valid -rnnlm model -hidden 15 -rand-seed 1 -debug 2 -class 100 -bptt 4 -bptt-block 10 -direct-order 3 -direct 2 -binary





现在我们来解释一下各项命令的具体含义：

-train train：



这个命令是指定训练文件的名字，注意文件名不能超过100个字符







-valid valid：



这个命令是指定验证数据的名字，也是不能超过100个字符，这个文件的作用是用来early stopping的，诶，抱歉，我不太知道如何翻译这个词，就是当你的训练文件训练一遍完毕时，会马上将训练后的模型在valid文件上面试一下，看一下效果如何，如果这一遍训练的效果还不错的话，那么继续同样打学习率来训练train这个文件，如果效果没太多打提升，就将学习率降低为一半，继续学习，直到没太大的提升就不再训练了。至于这个效果怎么看，是指训练打模型在valid上面的困惑度。这个valid文件作用可以防止过拟合，因为我们是一边训练一边看效果的嘛。



-rnnlm model 

这个命令是指定训练好后的语言模型的参数所存放打文件，也就是这个文件里面存放了一大堆浮点数和一些基本可读信息



-hidden 15 



这个命令是用来指定网络隐层神经元的个数的，这里为15,也就是隐层神经元的大小是15。hidden所指定的数大的话网络可容纳的信息就多，但运算量也会增大。



-rand-seed 1

 这个是指定随机种子，用来初始化网络的权值的，比如指定为1,那么内部会执行srand(1)，网络的权值会初始化为一堆随机小数，如果你指定为2,那么网络会被初始化为另一堆不同于值为1的随机小数。



-debug 2 



这个是一个控制开关，控制训练网络所输出的信息，默认为1,训练时会输出一些基本的信息，如果为debug > 2则会输出更详细的信息



-class 100 

这个是指定单词的分类，100表示将训练文件中的词语分为100类，这个作用是用来加速网络计算的，但会丧失一些准确性，论文中指出，一般可以取值为sqrt(|V|),其中|V|是训练数据中不重复的单词数量



-bptt 4 

这个命令参数如果为1,表示网络的一个常规的BPTT算法，即隐层只会往到前一个状态层，这么说比较抽象，见我的另一篇文章，介绍BPTT的。如果该参数值大于1,那么算法会学习向前到bptt+bptt_block-2那个时刻的状态







-bptt-block 10 

这个参数是来控制BPTT学习算法的，表示每学习10个词，就进行一次BPTT算法。如果不控制的话，每训练一个词语时，都进行深度的学习，会使训练一个词语的时间比较长







-direct-order 3 

这个参数是指定rnn中me（最大熵模型）部分特征的阶数。最大是不会超过20的，超过20也会自动指定为20







-direct 2 



这个参数的含义就比较技术细节了，它来指定网络输入层到输出层所存放权值的一维数组的大小，并且单位是一百万，比如现在指定的值为2,其内部大小是2000000。







-binary 

这个参数如果没有，则默认为text方式，区别在于binary是用二进制方式存储数据，text是以ascii方式，对于大量的浮点数来说，binary能更省文件大小，但是缺点是用文件打开，里面都是乱码。



下面是命令的执行结果：

debug mode: 2train file: trainvalid file: validclass size: 100Hidden layer size: 15Direct connections: 2MOrder of direct connections: 3BPTT: 4BPTT block: 10Rand seed: 1Model will be saved in binary formatrnnlm file: modelStarting training using file trainRestoring network from file to continue training...Iter:   9 Alpha: 0.006250    TRAIN entropy: 8.9646    Progress: 12.29%   WIter:   9 53238.Alpha: 0.006250    TRAIN entropy: 6.8535    Progress: 24.59%   WIter:   9 60260.Alpha: 0.006250    TRAIN entropy: 6.1981    Progress: 36.88%   Wter:   9 63232.Alpha: 0.006250    TRAIN entropy: 5.8457    Progress: 49.17%   WIter:   9 65056.Alpha: 0.006250    TRAIN entropy: 5.6123    Progress: 61.46%   WIter:   9 66436.Alpha: 0.006250    TRAIN entropy: 5.4747    Progress: 73.76%   WIter:   9 67084.Alpha: 0.006250    TRAIN entropy: 5.3561    Progress: 86.05%   WIter:   9 67802.Alpha: 0.006250    TRAIN entropy: 5.2776    Progress: 98.34%   WIter:   9 68190.Alpha: 0.006250    TRAIN entropy: 5.2688    Words/sec: 68239.9   VALID entropy: 6.1285 real 0m1.509suser 0m1.420ssys 0m0.064s  

看一下输出信息里面关键字的含义：

Iter ：所指的数字是对训练数据的第几次训练

Alpha：所指的数字是网络此刻的学习率

TRAIN entropy：所指的是熵，这个是在验证数据上的

Progress：表示在此刻处理的词语在训练数据中的位置的，也即进度

Words/sec：表示每秒训练的词语数







3.用srilm训练模型





执行如下命令：







ngram-count -text train -order 5 -lm templm -kndiscount -interpolate -gt3min 1 -gt4min 1



ngram -lm templm -order 5 -ppl test -debug 2 > temp.ppl





第一条命令就是用srlim建立了一个语言模型，第二条命令是用上面训练好的模型来测试，并且相关信息存放到temp.ppl，关于这个不细写啦，因为这完全是另一个工具的使用啦，可以看看我的另一篇博客，[srilm的使用记录](http://blog.csdn.net/a635661820/article/details/43939773)。





4.将temp.ppl转格式





命令如下：







gcc convert.c -O2 -o convert



./convert <temp.ppl >srilm.txt



这个命令是将上面生成的temp.ppl转成rnnlm能读取的数据格式。





5.插值两个模型并测试







命令如下：





./rnnlm -rnnlm model -test test -lm-prob srilm.txt -lambda 0.5







我们看一下各个参数的含义：





-rnnlm model



这个是指定训练好的模型文件，文件名不能超过100字符





-test test 



这个命令是指测试文件，文件名不能超过100字符





-lm-prob srilm.txt 



指明要使用其他的语言模型，并且文件所在为srilm.txt





-lambda 0.5 





这个是指两个模型的插值系数，目前为0.5就表明，两个模型各占一半

我们看一下执行的结果：

rnnlm file: modeltest log probability: -15398.941513test log probability given by other lm: -15498.214844test log probability 0.500000*rnn + 0.500000*other_lm: -15218.938477PPL net: 72.331327PPL other: 74.355495PPL combine: 68.800594 







test log probability 是指在test文件上，对数累加概率和，即logp = log10pw1+log10pw2...

test log probability given by other lm和上面的计算方式是一样的，只不过它是用srilm训练的模型

test log probability 0.500000*rnn + 0.500000*other_lm：这个是指插值后的模型的对数累加概率





后面三个PPL net, PPL other, PPL combine,分别指三个模型在测试集上面的困惑度







好啦，rnnlm的基本功能就介绍到此，本文结束，有任何错误的地方欢迎指出，更详细的好请看相关代码和文档











# rnnlm源码分析(一)

2015年03月30日 19:45:34 [a635661820](https://me.csdn.net/a635661820) 阅读数：10329





系列前言

参考文献：

1. RNNLM - Recurrent Neural Network  Language Modeling Toolkit([点此阅读](http://research.microsoft.com/pubs/175562/ASRU-Demo-2011.pdf))
2. Recurrent neural network based language model([点此阅读](http://www.fit.vutbr.cz/research/groups/speech/publi/2010/mikolov_interspeech2010_IS100722.pdf))
3. EXTENSIONS OF RECURRENT NEURAL NETWORK LANGUAGE MODEL([点此阅读](http://www.fit.vutbr.cz/research/groups/speech/publi/2011/mikolov_icassp2011_5528.pdf))
4. Strategies for Training Large Scale Neural Network  Language Models([点此阅读](http://www.fit.vutbr.cz/~imikolov/rnnlm/asru_large_v4.pdf))
5. STATISTICAL LANGUAGE MODELS BASED ON NEURAL  NETWORKS([点此阅读](http://www.fit.vutbr.cz/~imikolov/rnnlm/google.pdf))
6. A guide to recurrent neural networks and backpropagation([点此阅读](http://axon.cs.byu.edu/~martinez/classes/678/Papers/RNN_Intro.pdf))
7. A Neural Probabilistic Language Model([点此阅读](http://jmlr.org/papers/volume3/bengio03a/bengio03a.pdf))
8. Learning Long-Term Dependencies with Gradient Descent is Difficult([点此阅读](http://www.dsi.unifi.it/~paolo/ps/tnn-94-gradient.pdf))
9. Can Artificial Neural Networks Learn Language Models?([点此阅读](http://repository.cmu.edu/cgi/viewcontent.cgi?article=2405&context=compsci))



最近学习完系列神经网络语言模型后，最好的更为深刻的理解方式就是阅读源码，看一看究竟怎么实现的，认真的分析过源码，并且画出内部的数据结构图之后，才觉的了解的更透彻，甚至感觉自己能换一种语言重写这个开源工具。而且从阅读源码的过程中学到了很多实现技巧。当然自己的理解很多地方是比较狭隘的，可能有许多错误，而且某些地方是感觉那么回事，也没太仔细的思考，可能很多就没想清楚。而有些地方只有几行代码，却无奈的坐在那里想了一个下午╮(╯▽╰)╭，连吃饭、走路的时候也在想，我想这是快走火入魔了吧，哈哈。由于我在代码中注释很多，几乎是每行一注释，很多想仔细说的也在源码注释里面，所以内容主要以代码注释为主，外加对源码内部数据结构的图解。并且原谅我喜欢把参考内容放到最上面，养成习惯了，上面的8篇文章一开始读的话最好读第5篇，其他的内容都会在第5篇中大概的提到。另外为了懒得跑到另一篇文章去看图解，所以图解可能会反复出现在每篇文章中。



前言完毕了，第一篇本来是想直接把rnnlmlib.h直接放上来，然后介绍整个网络轮廓，大概看一下相关接口函数功能，但是那样内容发现有点长了，看上去都容易打瞌睡。于是把rnnlmlib.h分成两部分，第一篇文章介绍其中的成员变量，因为理解这些成语变量才能清楚的知道rnnlm toolkit命令参数的具体含义；第二篇介绍其中的成员函数，不涉及具体实现(那是后续文章的事儿)，只是大概知道函数什么功能即可，估计第二篇内容稍短。



我先把整个网络的图放上来，然后可以对应着看，下面的图是对应源代码所抽象出来的，我会把实际论文的模型图放上来做参照对比。

![img](https://img-blog.csdn.net/20150330194742979?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







下面的图是论文中的图，可以很明显的注意到实现和模型图还是有点差别，实现的时候把w(t)和s(t-1)、y(t)和c(t)合并成一个层了.







![img](https://img-blog.csdn.net/20150330194746037?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)







这里的图的输出层是经过分解的，为了加速。我后面的文章会介绍到和未分解的图和分解的计算。第一篇的目的就在于大概的了解真个网络的结构，宏观的看一看。下面是rnnlmlib.h文件的内容，虽然直接把成员变量切出来让类不完整，但没办法，成员函数在第二篇中介绍。

rnnlmlib.h内容如下：

------

```cpp
///////////////////////////////////////////////////////////////////////  
//  
// Recurrent neural network based statistical language modeling toolkit  
// Version 0.4a  
// (c) 2010-2012 Tomas Mikolov (tmikolov@gmail.com)  
// (c) 2013 Cantab Research Ltd (info@cantabResearch.com)  
//  
///////////////////////////////////////////////////////////////////////  
  
  
//这里的作用是防止rnnlmlib.h重复被include  
//如果程序第一次包含rnnlmlib.h,将会把#ifndef到文件最后一行的#endif之间的内容都执行  
//如果程序不是第一次包含rnnlmlib.h,则该文件的内容会被跳过  
#ifndef _RNNLMLIB_H_          
#define _RNNLMLIB_H_          
  
//最大字符串的长度                        
#define MAX_STRING 100  
  
//防止WEIGHTTYPE被重复定义  
#ifndef WEIGHTTYPE        
//权重类型,这里可以手动更改为float     
#define WEIGHTTYPE double  
#endif  
  
//real用于rnn中神经元的激活值,误差值类型  
typedef WEIGHTTYPE real;    // NN weights  
  
  
//direct_t表示最大熵模型中输入层到输出层权值类型  
typedef WEIGHTTYPE direct_t;    // ME weights  
  
  
//rnn中神经元结构,两部分  
//ac表示激活值,er表示误差值,er用在网络学习时  
struct neuron {  
    real ac;        //actual value stored in neuron  
    real er;        //error value in neuron, used by learning algorithm  
};  
  
  
//突触,这里是表示网络层与层之间参数权值的结构  
//其实就是浮点类型,只是包上了一层,这样更形象                  
struct synapse {  
    real weight;    //weight of synapse  
};  
  
  
//这是一个word的结构定义  
struct vocab_word {  
      
    //cn表示这个word在train_file中出现的频数  
    int cn;  
      
    //这个表示word本身,是字符串,但长度不能超过100  
    char word[MAX_STRING];  
      
    //这个应该是在概率分布时表示当前词在历史下的条件概率  
    //但是后面的代码中我没看到怎么使用这个定义,感觉可以忽略  
    real prob;  
      
    //这个表示当前词所在哪个类别  
    int class_index;  
};  
  
  
  
//PRIMES[]这个数组装都是质数,质数的用处是来做散列函数的  
//对散列函数了解不多,个人理解可以使散列函数更少的冲突吧  
const unsigned int PRIMES[]={108641969, 116049371, 125925907, 133333309, 145678979, 175308587, 197530793, 234567803, 251851741, 264197411, 330864029, 399999781,  
407407183, 459258997, 479012069, 545678687, 560493491, 607407037, 629629243, 656789717, 716048933, 718518067, 725925469, 733332871, 753085943, 755555077,  
782715551, 790122953, 812345159, 814814293, 893826581, 923456189, 940740127, 953085797, 985184539, 990122807};  
  
//PRIMES数组长度,这个用法可以积累一下,以后自己的程序也可以使用  
const unsigned int PRIMES_SIZE=sizeof(PRIMES)/sizeof(PRIMES[0]);  
  
  
//最大阶数,这个是用来限制最大熵模型的N元模型特征的,N不能无穷大,这里最大是20  
const int MAX_NGRAM_ORDER=20;  
  
  
//文件存储类型,TEXT表示ASCII存储,对存储网络权值时,有点浪费空间  
//BINARY表示二进制方式存储,对网络权值进行存储时,能更省空间,但是不便于阅读  
enum FileTypeEnum {TEXT, BINARY, COMPRESSED};       //COMPRESSED not yet implemented  
  
  
//这个类就是RNN的结构定义  
class CRnnLM{  
protected:  
      
    ////训练数据集的文件名  
    char train_file[MAX_STRING];          
      
    //验证数据集的文件名  
    char valid_file[MAX_STRING];                  
      
    //测试数据集的文件名  
    char test_file[MAX_STRING];           
      
    //RNN训练好后的模型所存储的文件  
    char rnnlm_file[MAX_STRING];  
      
    //其他语言模型对测试数据的生成文件,比如用SRILM  
    char lmprob_file[MAX_STRING];  
      
      
    //随机种子,不同的rand_seed,可以导致网络权值初始化为不同的随机数  
    int rand_seed;  
      
    //debug_mode分为两个级别,debug_mode>0会输出一些基本信息  
    //debug_mode>1会输出更详细的信息  
    int debug_mode;  
      
    //rnn toolkit的版本号  
    int version;  
      
    //用来指示存储模型参数时用TEXT, 还是用BINARY  
    int filetype;  
      
    //控制开关,use_lmprob为0时表示不使用  
    //为1时表示使用了其他语言模型,并会将RNN和其他语言模型插值  
    int use_lmprob;  
      
    //上面所说的插值系数  
    real lambda;  
      
    //防止误差过大增长,用gradient_cutoff进行限制  
    //gradient_cutoff的使用在矩阵相乘那个函数里面可以看到  
    real gradient_cutoff;  
      
    //dynamic如果大于0表示在测试时,边测试边学习   
    real dynamic;  
      
    //学习率  
    real alpha;  
      
    //训练初始的学习率  
    real starting_alpha;  
      
    //变量控制开关,为0表明不将alpha减半,具体见代码  
    int alpha_divide;  
      
    //logp表示累计对数概率,即logp = log10w1 + log10w2 + log10w3...  
    //llogp是last logp,即上一个logp  
    double logp, llogp;   
      
    //最小增长倍数  
    float min_improvement;  
      
    //iter表示整个训练文件的训练次数  
    int iter;         
      
    //vocab_max_size表示vocab最大容量,但是在代码中这个是动态增加的  
    int vocab_max_size;  
      
    //表示vocab的实际容量  
    int vocab_size;  
      
    //记录train_file有多少word  
    int train_words;          
      
    //指示当前所训练的词在train_file是第几个  
    int train_cur_pos;  
    int counter;  
      
    //one_iter==1的话,只会训练一遍  
    int one_iter;  
      
    //对train_file最大的训练遍数  
    int maxIter;  
      
    //表示每训练anti_k个word,会将网络信息保存到rnnlm_file  
    int anti_k;       
      
      
    //L2正规化因子  
    //实际在用的时候,是用的beta*alpha  
    real beta;  
      
    //指定单词所要分类别  
    int class_size;  
      
    //class_words[i-1][j-1]表示第i类别中的第j个词在vocab中的下标  
    int **class_words;  
      
    //class_cn[i-1]表示第i个类别中有多少word  
    int *class_cn;  
      
    //class_max_cn[i-1]表示第i类别最多有多少word  
    int *class_max_cn;  
      
    //old_classes大于0时用一种分类词的算法,否则用另一种  
    int old_classes;  
      
      
    //vocab里面存放的是不会重复的word,类型为vocab_word  
    struct vocab_word *vocab;  
      
    //选择排序,将vocab[1]到vocab[vocab_size-1]按照他们出现的频数从大到小排序  
    void sortVocab();  
      
    //里面存放word在vocab中的下标,这些下标是通过哈希函数映射来的  
    int *vocab_hash;  
      
    //vocab_hash的大小  
    int vocab_hash_size;  
      
    //输入层的大小  
    int layer0_size;  
      
    //隐层的大小  
    int layer1_size;  
      
    //压缩层的大小  
    int layerc_size;  
      
    //输出层的大小  
    int layer2_size;  
      
    //表示输出层到输出层直接连接的权值数组的大小  
    long long direct_size;  
      
    //最大熵模型所用特征的阶数  
    int direct_order;  
      
    //history从下标0开始存放的是wt, wt-1,wt-2...  
    int history[MAX_NGRAM_ORDER];         
      
    //bptt<=1的话,就是常规的bptt,即只从st展开到st-1  
    int bptt;  
      
    //每训练bptt_block个单词时,才会使用BPTT(或设置indenpendt不等于0,在句子结束时也可以进行BPTT)  
    int bptt_block;  
      
    //bptt_history从下标0开始存放的是wt,wt-1,wt-2...  
    int *bptt_history;        
      
    //bptt_hidden从下标0开始存放的是st,st-1,st-2...  
    neuron *bptt_hidden;      
      
    //隐层到输入层的权值,这个使用在BPTT时的  
    struct synapse *bptt_syn0;  
      
    int gen;  
      
    //independent非0,即表示要求每个句子独立训练  
    //如果independent==0,表面上一个句子对下一个句子的训练时算作历史信息的  
    //这控制还得看句子与句子之间的相关性如何了  
    int independent;  
      
    //下面就只用用源码中的英文注释了,懒得敲了,感觉英文注释的很清楚  
    //neurons in input layer  
    struct neuron *neu0;          
      
    //neurons in hidden layer  
    struct neuron *neu1;          
      
    //neurons in hidden layer  
    struct neuron *neuc;          
      
    //neurons in output layer  
    struct neuron *neu2;          
      
    //weights between input and hidden layer  
    struct synapse *syn0;         
      
    //weights between hidden and output layer (or hidden and compression if compression>0)  
    struct synapse *syn1;         
      
    //weights between hidden and compression layer  
    struct synapse *sync;         
      
    //direct parameters between input and output layer (similar to Maximum Entropy model parameters)  
    direct_t *syn_d;          
      
    //backup used in training:  
    struct neuron *neu0b;  
    struct neuron *neu1b;  
    struct neuron *neucb;  
    struct neuron *neu2b;  
      
    struct synapse *syn0b;  
    struct synapse *syn1b;  
    struct synapse *syncb;  
    direct_t *syn_db;  
      
    //backup used in n-bset rescoring:  
    struct neuron *neu1b2;              
    public:            
        int alpha_set, train_file_set;  
};  
  
#endif 
```



# rnnlm源码分析(二)

上一篇是介绍成员变量的大概含义，这一篇是介绍一些成员函数的功能，仍然是大体了解一下个别函数的功能，在第三篇才正式写实现分析。这一篇稍短，里面大部分是代码，因为很多涉及变量相关的在上一篇注释过了，所以这篇就没注释那么详细。如下：

```cpp


 //构造函数,这里进行很多变量的初始化    
CRnnLM()        //constructor initializes variables    
{    
    //这里的很多变量在前面都前一篇都有详细的介绍  
    //这里的初始值只要初始是为非0的可以留意一下  
    version=10;    
    filetype=TEXT;    
        
    use_lmprob=0;    
    lambda=0.75;    
    gradient_cutoff=15;    
    dynamic=0;    
        
    train_file[0]=0;    
    valid_file[0]=0;    
    test_file[0]=0;    
    rnnlm_file[0]=0;    
        
    alpha_set=0;    
    train_file_set=0;    
        
    alpha=0.1;    
    beta=0.0000001;    
    //beta=0.00000;    
    alpha_divide=0;    
    logp=0;    
    llogp=-100000000;    
    iter=0;    
        
    min_improvement=1.003;    
        
    train_words=0;    
    train_cur_pos=0;    
    vocab_max_size=100;    
    vocab_size=0;    
        
    vocab=(struct vocab_word *)calloc(vocab_max_size, sizeof(struct vocab_word));    
        
    layer1_size=30;    
        
    direct_size=0;    
    direct_order=0;    
        
    bptt=0;    
    bptt_block=10;    
    bptt_history=NULL;    
    bptt_hidden=NULL;    
    bptt_syn0=NULL;    
        
    gen=0;    
        
    independent=0;    
        
    neu0=NULL;    
    neu1=NULL;    
    neuc=NULL;    
    neu2=NULL;    
        
    syn0=NULL;    
    syn1=NULL;    
    sync=NULL;    
    syn_d=NULL;    
    syn_db=NULL;    
    //backup    
    neu0b=NULL;    
    neu1b=NULL;    
    neucb=NULL;    
    neu2b=NULL;    
        
    neu1b2=NULL;    
        
    syn0b=NULL;    
    syn1b=NULL;    
    syncb=NULL;    
    //    
        
    rand_seed=1;    
        
    class_size=100;    
    old_classes=0;    
        
    one_iter=0;    
    maxIter=0;    
        
    debug_mode=1;    
    srand(rand_seed);    
        
    //word映射为哈希的值小于100000000    
    vocab_hash_size=100000000;          
        
    //动态分配内存,calloc会自动将申请的内存初始化为0,但这里奇怪申请这么大空间，这里没对vocab_hash做检查    
    vocab_hash=(int *)calloc(vocab_hash_size, sizeof(int));    
}    
    
//析构函数,释放申请的空间    
~CRnnLM()       //destructor, deallocates memory    
{    
    int i;    
        
    if (neu0!=NULL) {    
        free(neu0);    
        free(neu1);    
        if (neuc!=NULL) free(neuc);    
        free(neu2);    
            
        free(syn0);    
        free(syn1);    
        if (sync!=NULL) free(sync);    
            
        if (syn_d!=NULL) free(syn_d);    
            
        if (syn_db!=NULL) free(syn_db);    
            
        //    
        free(neu0b);    
        free(neu1b);    
        if (neucb!=NULL) free(neucb);    
        free(neu2b);    
            
        free(neu1b2);    
            
        free(syn0b);    
        free(syn1b);    
        if (syncb!=NULL) free(syncb);    
        //    
            
        for (i=0; i<class_size; i++) free(class_words[i]);    
        free(class_max_cn);    
        free(class_cn);    
        free(class_words);    
            
        free(vocab);    
        free(vocab_hash);    
            
        if (bptt_history!=NULL) free(bptt_history);    
        if (bptt_hidden!=NULL) free(bptt_hidden);    
        if (bptt_syn0!=NULL) free(bptt_syn0);    
            
        //todo: free bptt variables too    
    }    
}                      
    
//返回值类型为real且范围在[min, max]的数    
real random(real min, real max);    
    
//设置训练数据的文件名    
void setTrainFile(char *str);    
    
//设置验证数据集的文件名    
void setValidFile(char *str);    
    
//设置测试集的文件名    
void setTestFile(char *str);    
    
//设置模型保存文件,即该文件用来存储模型的信息,以及各类参数    
void setRnnLMFile(char *str);    
    
    
//下面的函数没写注释,{}里面有语句,变量的含义可以看前篇    
//就不再一一注释了,这些参数都是从main主函数里面传来的，具体来说  
//这些参数就是来自shell命令里面的，这里可以看看我的一篇rnnlm toolkit命令参数介绍          
void setLMProbFile(char *str) {strcpy(lmprob_file, str);}    
    
void setFileType(int newt) {filetype=newt;}    
    
void setClassSize(int newSize) {class_size=newSize;}    
void setOldClasses(int newVal) {old_classes=newVal;}    
void setLambda(real newLambda) {lambda=newLambda;}    
void setGradientCutoff(real newGradient) {gradient_cutoff=newGradient;}    
void setDynamic(real newD) {dynamic=newD;}    
void setGen(real newGen) {gen=newGen;}    
void setIndependent(int newVal) {independent=newVal;}    
    
void setLearningRate(real newAlpha) {alpha=newAlpha;}    
void setRegularization(real newBeta) {beta=newBeta;}    
void setMinImprovement(real newMinImprovement) {min_improvement=newMinImprovement;}    
void setHiddenLayerSize(int newsize) {layer1_size=newsize;}    
void setCompressionLayerSize(int newsize) {layerc_size=newsize;}    
void setDirectSize(long long newsize) {direct_size=newsize;}    
void setDirectOrder(int newsize) {direct_order=newsize;}    
void setBPTT(int newval) {bptt=newval;}    
void setBPTTBlock(int newval) {bptt_block=newval;}    
void setRandSeed(int newSeed) {rand_seed=newSeed; srand(rand_seed);}    
void setDebugMode(int newDebug) {debug_mode=newDebug;}    
void setAntiKasparek(int newAnti) {anti_k=newAnti;}    
void setOneIter(int newOneIter) {one_iter=newOneIter;}    
void setMaxIter(int newMaxIter) {maxIter=newMaxIter;}    
    
//返回单词的哈希值    
int getWordHash(char *word);    
    
//从文件中读取一个单词到word    
void readWord(char *word, FILE *fin);    
    
//查找word，找到返回word在vocab中的索引,没找到返回-1    
int searchVocab(char *word);    
    
//读取当前文件指针所指的单词,并返回该单词在vocab中的索引    
int readWordIndex(FILE *fin);    
    
//将word添加到vocab中，并且返回刚添加word在vocab中的索引    
int addWordToVocab(char *word);    
    
//从train_file中读数据,相关数据会装入vocab,vocab_hash    
//这里假设vocab是空的    
void learnVocabFromTrainFile();     //train_file will be used to construct vocabulary    
    
//保存当前的权值,以及神经元信息值    
void saveWeights();         //saves current weights and unit activations    
    
//上面是暂存当前权值及神经元值，这里是从前面存下的数据中恢复    
void restoreWeights();      //restores current weights and unit activations from backup copy    
//void saveWeights2();      //allows 2. copy to be stored, useful for dynamic rescoring of nbest lists    
//void restoreWeights2();           
    
//保存隐层神经元的ac值    
void saveContext();    
    
//恢复隐层神经元的ac值    
void restoreContext();    
    
//保存隐层神经元的ac值    
void saveContext2();    
    
//恢复隐层神经元的ac值    
void restoreContext2();    
    
//初始化网络    
void initNet();    
    
//保存网络的所有信息到rnnlm_file    
void saveNet();    
    
//从文件流中读取一个字符使其ascii等于delim    
//随后文件指针指向delim的下一个    
void goToDelimiter(int delim, FILE *fi);    
    
//从rnnlm_file中读取网络的所有信息    
void restoreNet();    
    
//清除神经元的ac,er值    
void netFlush();    
    
//隐层神经元(论文中的状态层s(t))的ac值置1    
//s(t-1),即输入层layer1_size那部分的ac值置1    
//bptt+history清0    
void netReset();    //will erase just hidden layer state + bptt history + maxent history (called at end of sentences in the independent mode)    
    
//网络前向,计算概率分布    
void computeNet(int last_word, int word);    
    
//反传误差,更新网络权值    
void learnNet(int last_word, int word);    
    
////将隐层神经元的ac值复制到输出层后layer1_size那部分    
void copyHiddenLayerToInput();    
    
//训练网络    
void trainNet();    
void useLMProb(int use) {use_lmprob=use;}    
    
//测试网络    
void testNet();    
    
//这两个我还没看~    
void testNbest();    
void testGen();    
    
//矩阵和向量相乘  
//1.type == 0时,计算的是神经元ac值,相当于计算srcmatrix × srcvec, 其中srcmatrix是(to-from)×(to2-from2)的矩阵 
//srcvec是(to2-from2)×1的列向量,得到的结果是(to-from)×1的列向量,该列向量的值存入dest中的ac值  
//2.type == 1, 计算神经元的er值,即(srcmatrix)^T × srcvec,T表示转置,转置后是(to2-from2)×(to-from),srcvec是(to-from)×1的列向量    
  
void matrixXvector(struct neuron *dest, struct neuron *srcvec, struct synapse *srcmatrix, int matrix_width, int from, int to, int from2, int to2, int type);
```





# rnnlm源码分析(三)

2015年03月31日 14:41:52 [a635661820](https://me.csdn.net/a635661820) 阅读数：3804



这一篇开始介绍函数实现，对.cpp文件的函数内部语句分别进行走读，函数的顺序我没去组织，就按照文件的顺序进行。如果需要对某个函数配图说明，我会在下面注明，由于自己知识面比较窄，难免很多错误之处，欢迎看到的朋友指出~



好了，我们看一下开头的这部分，内容如下：

```cpp
#ifdef USE_BLAS
extern "C" {
#include <cblas.h>
}
#endif
```

其中有一个cblas.h的头文件，blas的全称是basic linear algebra subprograms,用于向量和矩阵计算的高性能数学库， blas本身是Fortran写的,cblas是blas的c语言接口库，rnnlmlib.cpp文件本身是用c++写的,需要调用c语言的cblas,所以需要用extern "C"来表明{}里面的内容需要按c语言的规范进行编译和链接，这是因为C＋＋和C程序编译完成后在目标代码中命

名规则不同,extern "C"实现了c和c++的混合编程。更详细的可以参考这篇博文

C++中extern “C”含义深层探索

，以及

CBLAS的安装与使用

，通过这两篇文章能了解更多。



下面继续看第一个函数，一个生成随机小数的函数，如下：

```cpp
real CRnnLM::random(real min, real max)
{
    return rand()/(real)RAND_MAX*(max-min)+min;
}
```

这里RAND_MAX是VC中stdlib.h中宏定义的一个字符常量,#define RAND_MAX 0x7FFF,其值为32767，通常在产生随机小数时可以使用RAND_MAX。里面的rand()返回值在[0, RAND_MAX]，[]表示闭区间，即能取到边界值。这样

return返回值范围在[min, max]。如果我们返回[min, max)之间数,可以用下面语句：

return rand() / (real)(RAND_MAX+1) * (max - min) + min;

若要返回随机的整数，可以用rand() % 整数 来获取。



下面几个设置文件名的函数，很容易理解。为了完整性，还是贴出来，如下

```cpp
//设置训练数据的文件名
void CRnnLM::setTrainFile(char *str)
{	
    strcpy(train_file, str);
}
 
//设置验证数据集的文件名
void CRnnLM::setValidFile(char *str)
{	
    strcpy(valid_file, str);
}
 
//设置测试集的文件名
void CRnnLM::setTestFile(char *str)
{	
    strcpy(test_file, str);
}
 
//设置模型保存文件,即该文件用来存储模型的信息,以及各类参数
void CRnnLM::setRnnLMFile(char *str)
{
```

下面这个函数是一个基本的函数，在其他函数里面会反复的用到，功能是从文件中读取一个单词到word，但要注意两点：

1.单词最长不能超过99(最后一个字符得为'\0')，否则会被截断

2.训练集中每个句子结尾都会自动生成</s>作为一个单独的词，被复制到word返回，这在后面也是用来判断一个句子是否结束的标志。



```cpp
void CRnnLM::readWord(char *word, FILE *fin)
{
    int a=0, ch;
	
	//feof(FILE *stream)当到达文件末尾时返回一个非0数
    while (!feof(fin)) {
		
		//从流中读取一个字符到ch
		ch=fgetc(fin);
		
		//ascii为13表示回车,\r,即回到一行的开头
		//注意\r与\n不同，后者是换行
		// \r\n主要是在文本文件中出现的
		if (ch==13) continue;
		
		if ((ch==' ') || (ch=='\t') || (ch=='\n')) {
			
			
			if (a>0) {			
				//将'\n'送回到字符流中,下次读取时还会读取到,这里标记一个句子的结束
                if (ch=='\n') ungetc(ch, fin);		
                break;
            }
			
			//如果a=0的情况就遇到换行,即上一个句子的结束,这里把句子的结束标记为</s>单独作为一个word
            if (ch=='\n') {
                strcpy(word, (char *)"</s>");
                return;
            }
            else continue;
        }
		
        word[a]=ch;
        a++;
		
		//过长的单词会被截断,过长的结果word[99] = '\0'
        if (a>=MAX_STRING) {
            //printf("Too long word found!\n");   //truncate too long words
            a--;
        }
    }
	
	//字符串结尾用'\0'，其ascii码为0
    word[a]=0;
}
```

下面这个函数查找word，找到返回word在vocab中的索引,没找到返回-1，有关前面变量只是做了简要的解释，这里简要的理解一下word,getWordHash(word),vocab_hash[],vocab[]的关系,见图。观察图，看到给定一个word,可以在O(1)的时间内得到word在vocab中的索引:vacab[vocab_hash[getWordHash(word)]]，这里应用哈希映射是典型的用空间换时间的方法,但是哈希映射有个问题就是冲突，所以这里加了三层查找,如果发生了冲突，那么就在vocab中顺序查找,时间复杂度为O(vocab_size)。



![img](https://img-blog.csdn.net/20150331144429610?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```
//返回单词的哈希值
int CRnnLM::getWordHash(char *word)
{
    unsigned int hash, a;
    
    hash=0;
	
	//单词哈希值的计算方式
    for (a=0; a<strlen(word); a++) hash=hash*237+word[a];
	
	//vocab_hash_size在CRnnLm的构造函数初始化为1亿即100000000
    hash=hash%vocab_hash_size;
    
    return hash;
}
```



```cpp
int CRnnLM::searchVocab(char *word)
{
    int a;
    unsigned int hash;
    
    hash=getWordHash(word);
    
	//第一层查找,vocab_hash[hash]==-1表示当前word不在vocab中
    if (vocab_hash[hash]==-1) return -1;
	
	//第二层查找,这里确认当前word并未被其他word给冲突掉
    if (!strcmp(word, vocab[vocab_hash[hash]].word)) return vocab_hash[hash];
    
	//第三层查找,走到了这里,说明当前word与其他word的哈希值有冲突,直接线性查找
    for (a=0; a<vocab_size; a++) {				
        if (!strcmp(word, vocab[a].word)) {			
			//这里把查找到的当前词的哈希值覆盖,这样vocab_hash总是保持最近查找词的hash值
			//越是频繁查找的词,通过这种方式即便冲突了,下次也会在O(1)的时间内查找到！
			vocab_hash[hash]=a;
			return a;
		}
    }
	
	//没找到,即该词不在vocab内,即out-of-vocabulary
    return -1;							
}
```

下面这个函数读取当前文件指针所指的单词,并返回该单词在vocab中的索引，注意无论是训练数据、验证数据、测试数据文件的格式都是文件末尾空行，所以按照文件内容顺序查找，查找到文件末尾一定是</s>，然后fin就到文件末尾了。



```cpp
int CRnnLM::readWordIndex(FILE *fin)
{
    char word[MAX_STRING];
	
    readWord(word, fin);
    if (feof(fin)) return -1;
	
    return searchVocab(word);
}
```

接下来这个函数将word添加到vocab中，并且返回刚添加word在vocab中的索引，并将word与vocab_hash与vocab通过word哈希值关联起来，可以看到这的内存是动态管理的，代码注释如下：



```cpp
int CRnnLM::addWordToVocab(char *word)
{
    unsigned int hash;
    
    strcpy(vocab[vocab_size].word, word);
    vocab[vocab_size].cn=0;
    vocab_size++;
	
	//vocab是动态管理的,当数组内存快不够了,再扩大数组内存,每次增加100个单位,每个单位是vocab_word类型
    if (vocab_size+2>=vocab_max_size) {        
        vocab_max_size+=100;
		
		//realloc是用来扩大或缩小内存的,扩大时原来的内容不变,系统直接
		//在后面找空闲内存,如果没找到，则会把前面的数据重新移动到一个够大的地方
		//即realloc可能会导致数据的移动,这算自己顺便看源码边复习一些c的知识吧
        vocab=(struct vocab_word *)realloc(vocab, vocab_max_size * sizeof(struct vocab_word));
    }
    
	//将word的哈希值作为vocab_hash的下标,下标所对应的整型值为vocab中对该word的索引
    hash=getWordHash(word);
    vocab_hash[hash]=vocab_size-1;
	
    return vocab_size-1;
}
```

下面是一个选择排序的算法，将vocab[1]到vocab[vocab_size-1]按照他们出现的频数从大到小排序。



```cpp
void CRnnLM::sortVocab()
{
    int a, b, max;
    vocab_word swap;
    
	//注意这里下标是从1开始，并未把vocab[0]考虑进来
	//实际上vocab[0]是存放的</s>,从后面的learnVocabFromTrainFile()可以看到
    for (a=1; a<vocab_size; a++) {
        max=a;
        for (b=a+1; b<vocab_size; b++) if (vocab[max].cn<vocab[b].cn) max=b;
		
        swap=vocab[max];
        vocab[max]=vocab[a];
        vocab[a]=swap;
    }
}
```

然后这个函数是从train_file中读数据,相关数据会装入vocab,vocab_hash，这里假设vocab是空的。



```cpp
void CRnnLM::learnVocabFromTrainFile()    
{
    char word[MAX_STRING];
    FILE *fin;
    int a, i, train_wcn;
    
	//这里对vocab_hash的初始化说明不在vocab中的word,其vocab_hash[getWordHash(word)]为-1
    for (a=0; a<vocab_hash_size; a++) vocab_hash[a]=-1;
	
	//以二进制模式读取文件
	//关于二进制和文本文件的区别，可以参考这篇博文：http://www.cnblogs.com/flying-roc/articles/1798817.html
	//当train_file是文本文件存储时,即句子结尾是\r\n,前面readWord()函数有一个条件语句if处理掉了\r
	//如果train_file是二进制存储时,句子结尾只有\n，所以对于字符组成的文件来说两者差别不大
    fin=fopen(train_file, "rb");
	
    vocab_size=0;
	
	//也就是vocab[0]是存放的</s>
    addWordToVocab((char *)"</s>");
	
	//记录train_file中tokens数量
    train_wcn=0;
    while (1) {
        readWord(word, fin);
        if (feof(fin)) break;
        
        train_wcn++;
		
		//vocab存放的word不会重复,重复的word让其词频加1
        i=searchVocab(word);
        if (i==-1) {
            a=addWordToVocab(word);
            vocab[a].cn=1;
        } else vocab[i].cn++;
    }
	
	//注意这里在读入train_file后，会将vocab排序,后面会看到对词语分类有帮助
    sortVocab();
    
    //select vocabulary size
    /*a=0;
    while (a<vocab_size) {
	a++;
	if (vocab[a].cn==0) break;
    }
    vocab_size=a;*/
	
    if (debug_mode>0) {
		printf("Vocab size: %d\n", vocab_size);
		printf("Words in train file: %d\n", train_wcn);
    }
    
	//train_words表示训练文件中的词数
    train_words=train_wcn;
	
    fclose(fin);
}
```

由于本篇长度差不多了，下一篇继续函数实现的分析







# rnnlm源码分析(四)

2015年03月31日 15:03:41 [a635661820](https://me.csdn.net/a635661820) 阅读数：2925



上一篇写到learnVocabFromTrainFile()，下面继续，下面连续几个函数的功能都是保存网络部分信息，或者恢复信息，先把前面的图放在这儿供对比

![img](https://img-blog.csdn.net/20150331150923028?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





```cpp
//保存当前的权值,以及神经元信息值,网络的数据结构见图
void CRnnLM::saveWeights()      
{
    int a,b;
	
	//暂存输入层神经元值
    for (a=0; a<layer0_size; a++) {
        neu0b[a].ac=neu0[a].ac;
        neu0b[a].er=neu0[a].er;
    }
	
	//暂存隐层神经元值
    for (a=0; a<layer1_size; a++) {
        neu1b[a].ac=neu1[a].ac;
        neu1b[a].er=neu1[a].er;
    }
    
	//暂存压缩层神经元值
    for (a=0; a<layerc_size; a++) {
        neucb[a].ac=neuc[a].ac;
        neucb[a].er=neuc[a].er;
    }
    
	//暂存输出层神经元值
    for (a=0; a<layer2_size; a++) {
        neu2b[a].ac=neu2[a].ac;
        neu2b[a].er=neu2[a].er;
    }
    
	//暂存输入层到隐层的权值
    for (b=0; b<layer1_size; b++) for (a=0; a<layer0_size; a++) {
		
		//这里输入到隐层的所有权值可以理解为一个layer_size*layer0_size的矩阵,只不过用了一维数组来装	
		//而对应的parameter[b][a]映射到一维数组的下标就是a + b*layer0_size
		//对其他层到层的权值存储也是同理的
		syn0b[a+b*layer0_size].weight=syn0[a+b*layer0_size].weight;
    }
    
	//如果有压缩层
    if (layerc_size>0) {
		//暂存隐层到压缩层的权值
		for (b=0; b<layerc_size; b++) for (a=0; a<layer1_size; a++) {
			
			syn1b[a+b*layer1_size].weight=syn1[a+b*layer1_size].weight;
		}
		
		//暂存压缩层到输出层的权值
		for (b=0; b<layer2_size; b++) for (a=0; a<layerc_size; a++) {
			
			syncb[a+b*layerc_size].weight=sync[a+b*layerc_size].weight;
		}
    }
    else {		//如果没有压缩层
		
		//直接暂存隐层到输出层的权值
		for (b=0; b<layer2_size; b++) for (a=0; a<layer1_size; a++) {
			syn1b[a+b*layer1_size].weight=syn1[a+b*layer1_size].weight;
		}
    }
    
	//因为被注释掉了，这里并没有存储输入层到输出层的直接连接参数
    //for (a=0; a<direct_size; a++) syn_db[a].weight=syn_d[a].weight;
}
 
 
//上面是暂存当前权值及神经元值，这里是从前面存下的数据中恢复
//含义都差不多,不做具体注释
void CRnnLM::restoreWeights()      
{
    int a,b;
	
    for (a=0; a<layer0_size; a++) {
        neu0[a].ac=neu0b[a].ac;
        neu0[a].er=neu0b[a].er;
    }
	
    for (a=0; a<layer1_size; a++) {
        neu1[a].ac=neu1b[a].ac;
        neu1[a].er=neu1b[a].er;
    }
    
    for (a=0; a<layerc_size; a++) {
        neuc[a].ac=neucb[a].ac;
        neuc[a].er=neucb[a].er;
    }
    
    for (a=0; a<layer2_size; a++) {
        neu2[a].ac=neu2b[a].ac;
        neu2[a].er=neu2b[a].er;
    }
	
    for (b=0; b<layer1_size; b++) for (a=0; a<layer0_size; a++) {
        syn0[a+b*layer0_size].weight=syn0b[a+b*layer0_size].weight;
    }
    
    if (layerc_size>0) {
		for (b=0; b<layerc_size; b++) for (a=0; a<layer1_size; a++) {
			syn1[a+b*layer1_size].weight=syn1b[a+b*layer1_size].weight;
		}
		
		for (b=0; b<layer2_size; b++) for (a=0; a<layerc_size; a++) {
			sync[a+b*layerc_size].weight=syncb[a+b*layerc_size].weight;
		}
    }
    else {
		for (b=0; b<layer2_size; b++) for (a=0; a<layer1_size; a++) {
			syn1[a+b*layer1_size].weight=syn1b[a+b*layer1_size].weight;
		}
    }
    
    //for (a=0; a<direct_size; a++) syn_d[a].weight=syn_db[a].weight;
}
 
//保存隐层神经元的ac值
void CRnnLM::saveContext()		//useful for n-best list processing
{
    int a;
    
    for (a=0; a<layer1_size; a++) neu1b[a].ac=neu1[a].ac;
}
 
//恢复隐层神经元的ac值
void CRnnLM::restoreContext()
{
    int a;
    
    for (a=0; a<layer1_size; a++) neu1[a].ac=neu1b[a].ac;
}
 
//保存隐层神经元的ac值
void CRnnLM::saveContext2()
{
    int a;
    
    for (a=0; a<layer1_size; a++) neu1b2[a].ac=neu1[a].ac;
}
 
//恢复隐层神经元的ac值
void CRnnLM::restoreContext2()
{
    int a;
    
    for (a=0; a<layer1_size; a++) neu1[a].ac=neu1b2[a].ac;
}
```

至于为什么会建立压缩层,见论文EXTENSIONS OF RECURRENT NEURAL NETWORK LANGUAGE MODEL，里面说的压缩层是为了减少输出到隐层的参数,并且减小了总的计算复杂度，至于为什么增加压缩层能够使计算量减小,我也不太明白,如果明白的朋友看到还请告知一下哈。



下面这个函数是初始化网络，内容有点多，并且里面有的内容需要图解才能更清楚，所以把这个函数分成两部分写。下面是该函数内部前面的内容，主要是完成分配内存，初始化等工作，这个过程也就相当于把上图那个网络给搭建起来了，参照图能更清楚一些。



```cpp
void CRnnLM::initNet()
{
    int a, b, cl;
	
    layer0_size=vocab_size+layer1_size;		//layer1_size初始为30
    layer2_size=vocab_size+class_size;		//class_size初始时为100
	
	//calloc是经过初始化的内存申请
	//分别建立输入层,隐层,压缩层,输出层
    neu0=(struct neuron *)calloc(layer0_size, sizeof(struct neuron));		
    neu1=(struct neuron *)calloc(layer1_size, sizeof(struct neuron));
    neuc=(struct neuron *)calloc(layerc_size, sizeof(struct neuron));
    neu2=(struct neuron *)calloc(layer2_size, sizeof(struct neuron));
	
	//建立隐层到输入层的权值参数
    syn0=(struct synapse *)calloc(layer0_size*layer1_size, sizeof(struct synapse));
	
	//如果没有设置压缩层
    if (layerc_size==0)
		//建立压缩层到隐层的权值参数
		syn1=(struct synapse *)calloc(layer1_size*layer2_size, sizeof(struct synapse));
	
    else {		//含有压缩层
		
		//建立压缩层到隐层的权值参数
		syn1=(struct synapse *)calloc(layer1_size*layerc_size, sizeof(struct synapse));
		//建立输出层到压缩层的权值参数
		sync=(struct synapse *)calloc(layerc_size*layer2_size, sizeof(struct synapse));
    }
	
    if (syn1==NULL) {
		printf("Memory allocation failed\n");
		exit(1);
    }
    
    if (layerc_size>0) if (sync==NULL) {
		printf("Memory allocation failed\n");
		exit(1);
    }
    
	//建立输入层到输出层的参数,direct_size是long long类型的,由-direct参数指定,单位是百万
	//比如-direct传进来的是2，则真实的direct_size = 2*10^6
    syn_d=(direct_t *)calloc((long long)direct_size, sizeof(direct_t));
	
    if (syn_d==NULL) {
		printf("Memory allocation for direct connections failed (requested %lld bytes)\n", (long long)direct_size * (long long)sizeof(direct_t));
		exit(1);
    }
	
	//创建神经元备份空间
    neu0b=(struct neuron *)calloc(layer0_size, sizeof(struct neuron));
    neu1b=(struct neuron *)calloc(layer1_size, sizeof(struct neuron));
    neucb=(struct neuron *)calloc(layerc_size, sizeof(struct neuron));
    neu1b2=(struct neuron *)calloc(layer1_size, sizeof(struct neuron));
    neu2b=(struct neuron *)calloc(layer2_size, sizeof(struct neuron));
	
	//创建突触(即权值参数)的备份空间
    syn0b=(struct synapse *)calloc(layer0_size*layer1_size, sizeof(struct synapse));
    //syn1b=(struct synapse *)calloc(layer1_size*layer2_size, sizeof(struct synapse));
    if (layerc_size==0)
		syn1b=(struct synapse *)calloc(layer1_size*layer2_size, sizeof(struct synapse));
    else {
		syn1b=(struct synapse *)calloc(layer1_size*layerc_size, sizeof(struct synapse));
		syncb=(struct synapse *)calloc(layerc_size*layer2_size, sizeof(struct synapse));
    }
	
    if (syn1b==NULL) {
		printf("Memory allocation failed\n");
		exit(1);
    }
    
	//下面对所有神经元进行初始化，值为0
    for (a=0; a<layer0_size; a++) {
        neu0[a].ac=0;
        neu0[a].er=0;
    }
	
    for (a=0; a<layer1_size; a++) {
        neu1[a].ac=0;
        neu1[a].er=0;
    }
    
    for (a=0; a<layerc_size; a++) {
        neuc[a].ac=0;
        neuc[a].er=0;
    }
    
    for (a=0; a<layer2_size; a++) {
        neu2[a].ac=0;
        neu2[a].er=0;
    }
	
	//将所有权值参数全部初始化为随机数,范围为[-0.3, 0.3]
    for (b=0; b<layer1_size; b++) for (a=0; a<layer0_size; a++) {
        syn0[a+b*layer0_size].weight=random(-0.1, 0.1)+random(-0.1, 0.1)+random(-0.1, 0.1);
    }
	
    if (layerc_size>0) {
		for (b=0; b<layerc_size; b++) for (a=0; a<layer1_size; a++) {
			syn1[a+b*layer1_size].weight=random(-0.1, 0.1)+random(-0.1, 0.1)+random(-0.1, 0.1);
		}
		
		for (b=0; b<layer2_size; b++) for (a=0; a<layerc_size; a++) {
			sync[a+b*layerc_size].weight=random(-0.1, 0.1)+random(-0.1, 0.1)+random(-0.1, 0.1);
		}
    }
    else {
		for (b=0; b<layer2_size; b++) for (a=0; a<layer1_size; a++) {
			syn1[a+b*layer1_size].weight=random(-0.1, 0.1)+random(-0.1, 0.1)+random(-0.1, 0.1);
		}
    }
    
	//输入到输出直连的参数初始化为0
    long long aa;
    for (aa=0; aa<direct_size; aa++) syn_d[aa]=0;
    
	
    if (bptt>0) {
		bptt_history=(int *)calloc((bptt+bptt_block+10), sizeof(int));
		for (a=0; a<bptt+bptt_block; a++) bptt_history[a]=-1;
		//
		bptt_hidden=(neuron *)calloc((bptt+bptt_block+1)*layer1_size, sizeof(neuron));
		for (a=0; a<(bptt+bptt_block)*layer1_size; a++) {
			bptt_hidden[a].ac=0;
			bptt_hidden[a].er=0;
		}
		//
		bptt_syn0=(struct synapse *)calloc(layer0_size*layer1_size, sizeof(struct synapse));
		if (bptt_syn0==NULL) {
			printf("Memory allocation failed\n");
			exit(1);
		}
    }
	
	//saveWeights里面并没有保存输入层到输出层的参数，即syn_d
    saveWeights();		
```

第二部分是对单词的分类，注意下面的vocab是从大到小排好序的，下面都是对word进行分类,分类的依据就是他们的一元词频，分类的最终结果就是越靠近前面类别的word很少,他们出现的频数比较高，越靠近后面的类别所包含的word就非常多,他们在语料中出现比较稀疏。

下面这段代码所建立的结构如下图：

![img](https://img-blog.csdn.net/20150331152547218?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```cpp
double df, dd;
    int i;
    
    df=0;
    dd=0;
    a=0;
    b=0;
	
	//注意这里vocab是从大到小排好序的
	//下面都是对word进行分类,分类的依据就是他们的一元词频
	//分类的最终结果就是越靠近前面类别的word很少,他们出现的频数比较高
	//越靠近后面的类别所包含的word就非常多,他们在语料中出现比较稀疏
    if (old_classes) {  	// old classes
        for (i=0; i<vocab_size; i++) b+=vocab[i].cn;
        for (i=0; i<vocab_size; i++) {
			df+=vocab[i].cn/(double)b;
			if (df>1) df=1;
			if (df>(a+1)/(double)class_size) {
				vocab[i].class_index=a;
				if (a<class_size-1) a++;
			}
			else {
				vocab[i].class_index=a;
			}
        }
    } else {			// new classes
        for (i=0; i<vocab_size; i++) b+=vocab[i].cn;
        for (i=0; i<vocab_size; i++) dd+=sqrt(vocab[i].cn/(double)b);
        for (i=0; i<vocab_size; i++) {
			df+=sqrt(vocab[i].cn/(double)b)/dd;
            if (df>1) df=1;
            if (df>(a+1)/(double)class_size) {
				vocab[i].class_index=a;
				if (a<class_size-1) a++;
            } else {
				vocab[i].class_index=a;
            }
		}
    }
    
    //allocate auxiliary class variables (for faster search when normalizing probability at output layer)
    
	//下面是为了加速查找,最终达到的目的就是给定一个类别，能很快的遍历得到该类别的所有word,该结构见图
    class_words=(int **)calloc(class_size, sizeof(int *));
    class_cn=(int *)calloc(class_size, sizeof(int));
    class_max_cn=(int *)calloc(class_size, sizeof(int));
    
    for (i=0; i<class_size; i++) {
		class_cn[i]=0;
		class_max_cn[i]=10;
		class_words[i]=(int *)calloc(class_max_cn[i], sizeof(int));
    }
    
    for (i=0; i<vocab_size; i++) {
		cl=vocab[i].class_index;
		class_words[cl][class_cn[cl]]=i;
		class_cn[cl]++;
		if (class_cn[cl]+2>=class_max_cn[cl]) {
			class_max_cn[cl]+=10;
			class_words[cl]=(int *)realloc(class_words[cl], class_max_cn[cl]*sizeof(int));
		}
    }
}
```



上面的函数初始化网络涉及最大熵模型，即可以简单的理解为输入层到输出层的直接连接，虽然作者在论文中总是强调可以这么认为，但我觉的并不是那样简单的直接连接着，中间会有一个历史数组，这个后面会谈到。下一篇的几个函数很容易明白，直接上注释即可，为了省篇幅，放在下一篇





# rnnlm源码分析(五)

2015年03月31日 21:28:52 [a635661820](https://me.csdn.net/a635661820) 阅读数：2505



这篇内容的函数比较好理解，很多地方含义都差不多，某处的注释适用于其他地方，就无需赘述了，直接上代码。



```cpp
//保存网络的所有信息到rnnlm_file
void CRnnLM::saveNet()       //will save the whole network structure                                                        
{
    FILE *fo;
    int a, b;
    char str[1000];
    float fl;
    
	//这里把rnnlm_file的文件名加上.temp送入到str
    sprintf(str, "%s.temp", rnnlm_file);
	
	//以二进制方式创建文件
    fo=fopen(str, "wb");
    if (fo==NULL) {
        printf("Cannot create file %s\n", rnnlm_file);
        exit(1);
    }
    fprintf(fo, "version: %d\n", version);		//初始化时version=10
    fprintf(fo, "file format: %d\n\n", filetype);		//初始化时filetype=TEXT
	
    fprintf(fo, "training data file: %s\n", train_file);
    fprintf(fo, "validation data file: %s\n\n", valid_file);
	
    fprintf(fo, "last probability of validation data: %f\n", llogp);//TBD
    fprintf(fo, "number of finished iterations: %d\n", iter);
	
    fprintf(fo, "current position in training data: %d\n", train_cur_pos);
    fprintf(fo, "current probability of training data: %f\n", logp);
    fprintf(fo, "save after processing # words: %d\n", anti_k);
    fprintf(fo, "# of training words: %d\n", train_words);
	
    fprintf(fo, "input layer size: %d\n", layer0_size);
    fprintf(fo, "hidden layer size: %d\n", layer1_size);
    fprintf(fo, "compression layer size: %d\n", layerc_size);
    fprintf(fo, "output layer size: %d\n", layer2_size);
	
    fprintf(fo, "direct connections: %lld\n", direct_size);
    fprintf(fo, "direct order: %d\n", direct_order);
    
    fprintf(fo, "bptt: %d\n", bptt);
    fprintf(fo, "bptt block: %d\n", bptt_block);
    
    fprintf(fo, "vocabulary size: %d\n", vocab_size);
    fprintf(fo, "class size: %d\n", class_size);
    
    fprintf(fo, "old classes: %d\n", old_classes);
    fprintf(fo, "independent sentences mode: %d\n", independent);
    
    fprintf(fo, "starting learning rate: %f\n", starting_alpha);
    fprintf(fo, "current learning rate: %f\n", alpha);
    fprintf(fo, "learning rate decrease: %d\n", alpha_divide);
    fprintf(fo, "\n");
	
    fprintf(fo, "\nVocabulary:\n");
    for (a=0; a<vocab_size; a++) fprintf(fo, "%6d\t%10d\t%s\t%d\n", a, vocab[a].cn, vocab[a].word, vocab[a].class_index);
	
    //以文本方式存入,即以ascii来存,能够方便阅读,不会乱码
    if (filetype==TEXT) {
		fprintf(fo, "\nHidden layer activation:\n");
		for (a=0; a<layer1_size; a++) fprintf(fo, "%.4f\n", neu1[a].ac);
    }
    if (filetype==BINARY) {
		for (a=0; a<layer1_size; a++) {
			fl=neu1[a].ac;
			//fwrite()是以二进制方式输出到文件
			//第一个参数表示获取数据的地址
			//第二个参数表示要写入内容的单字节数
			//第三个参数表示要进行写入size字节的数据项的个数
			fwrite(&fl, sizeof(fl), 1, fo);
		}
    }
    //////////
    if (filetype==TEXT) {
		fprintf(fo, "\nWeights 0->1:\n");
		for (b=0; b<layer1_size; b++) {
			for (a=0; a<layer0_size; a++) {
				fprintf(fo, "%.4f\n", syn0[a+b*layer0_size].weight);
			}
		}
    }
    if (filetype==BINARY) {
		for (b=0; b<layer1_size; b++) {
			for (a=0; a<layer0_size; a++) {
				fl=syn0[a+b*layer0_size].weight;
				fwrite(&fl, sizeof(fl), 1, fo);
			}
		}
    }
    /////////
    if (filetype==TEXT) {
		if (layerc_size>0) {
			fprintf(fo, "\n\nWeights 1->c:\n");
			for (b=0; b<layerc_size; b++) {
				for (a=0; a<layer1_size; a++) {
					fprintf(fo, "%.4f\n", syn1[a+b*layer1_size].weight);
				}
			}
			
			fprintf(fo, "\n\nWeights c->2:\n");
			for (b=0; b<layer2_size; b++) {
				for (a=0; a<layerc_size; a++) {
					fprintf(fo, "%.4f\n", sync[a+b*layerc_size].weight);
				}
			}
		}
		else
		{
			fprintf(fo, "\n\nWeights 1->2:\n");
			for (b=0; b<layer2_size; b++) {
				for (a=0; a<layer1_size; a++) {
					fprintf(fo, "%.4f\n", syn1[a+b*layer1_size].weight);
				}
			}
		}
    }
    if (filetype==BINARY) {
		if (layerc_size>0) {
			for (b=0; b<layerc_size; b++) {
				for (a=0; a<layer1_size; a++) {
					fl=syn1[a+b*layer1_size].weight;
					fwrite(&fl, sizeof(fl), 1, fo);
				}
			}
			
			for (b=0; b<layer2_size; b++) {
				for (a=0; a<layerc_size; a++) {
					fl=sync[a+b*layerc_size].weight;
					fwrite(&fl, sizeof(fl), 1, fo);
				}
			}
		}
		else
		{
			for (b=0; b<layer2_size; b++) {
				for (a=0; a<layer1_size; a++) {
					fl=syn1[a+b*layer1_size].weight;
					fwrite(&fl, sizeof(fl), 1, fo);
				}
			}
		}
    }
    ////////
    if (filetype==TEXT) {
		fprintf(fo, "\nDirect connections:\n");
		long long aa;
		for (aa=0; aa<direct_size; aa++) {
			fprintf(fo, "%.2f\n", syn_d[aa]);
		}
    }
    if (filetype==BINARY) {
		long long aa;
		for (aa=0; aa<direct_size; aa++) {
			fl=syn_d[aa];
			fwrite(&fl, sizeof(fl), 1, fo);
			
			//这里被注释掉的代码，没看懂
			//不知道为啥可以省50%的空间
			//希望明白的朋友告知一下哈
			/*fl=syn_d[aa]*4*256;			//saving direct connections this way will save 50% disk space; several times more compression is doable by clustering
			if (fl>(1<<15)-1) fl=(1<<15)-1;
			if (fl<-(1<<15)) fl=-(1<<15);
			si=(signed short int)fl;
			fwrite(&si, 2, 1, fo);*/
		}
    }
    ////////    
    fclose(fo);
    
	//最后将名字更改为指定的rnnlm_file，那为啥最开始要改呢?
	//这里不太明白，希望明白的朋友告知一下哈
    rename(str, rnnlm_file);
}
 
//从文件流中读取一个字符使其ascii等于delim
//随后文件指针指向delim的下一个
void CRnnLM::goToDelimiter(int delim, FILE *fi)
{
    int ch=0;
	
    while (ch!=delim) {
        ch=fgetc(fi);
        if (feof(fi)) {
            printf("Unexpected end of file\n");
            exit(1);
        }
    }
}
 
//从rnnlm_file中读取网络的所有信息
void CRnnLM::restoreNet()    //will read whole network structure
{
    FILE *fi;
    int a, b, ver;
    float fl;
    char str[MAX_STRING];
    double d;
	
    fi=fopen(rnnlm_file, "rb");
    if (fi==NULL) {
		printf("ERROR: model file '%s' not found!\n", rnnlm_file);
		exit(1);
    }
	
	//注意前面一些基本的信息,如version，filetype等都是以ascii输入的
	//前面均是用:做标记
	//ver表示该模型被哪个rnnlm版本的程序所训练得到的
	//version表示现在rnnlm的版本号
	//下面几个跟ver有关的条件判断，应该是解决兼容问题,因为新的版本加了新的功能
    goToDelimiter(':', fi);
    fscanf(fi, "%d", &ver);
    if ((ver==4) && (version==5)) /* we will solve this later.. */ ; else
		if (ver!=version) {
			printf("Unknown version of file %s\n", rnnlm_file);
			exit(1);
		}
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &filetype);
		//
		goToDelimiter(':', fi);
		if (train_file_set==0) {
			fscanf(fi, "%s", train_file);
		} else fscanf(fi, "%s", str);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%s", valid_file);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%lf", &llogp);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &iter);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &train_cur_pos);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%lf", &logp);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &anti_k);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &train_words);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &layer0_size);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &layer1_size);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &layerc_size);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &layer2_size);
		//
		if (ver>5) {
			goToDelimiter(':', fi);
			fscanf(fi, "%lld", &direct_size);
		}
		//
		if (ver>6) {
			goToDelimiter(':', fi);
			fscanf(fi, "%d", &direct_order);
		}
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &bptt);
		//
		if (ver>4) {
			goToDelimiter(':', fi);
			fscanf(fi, "%d", &bptt_block);
		} else bptt_block=10;
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &vocab_size);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &class_size);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &old_classes);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &independent);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%lf", &d);
		starting_alpha=d;
		//
		goToDelimiter(':', fi);
		if (alpha_set==0) {
			fscanf(fi, "%lf", &d);
			alpha=d;
		} else fscanf(fi, "%lf", &d);
		//
		goToDelimiter(':', fi);
		fscanf(fi, "%d", &alpha_divide);
		//
		
		
		//下面是把vocab从train_file中恢复过来
		if (vocab_max_size<vocab_size) {
			if (vocab!=NULL) free(vocab);
			vocab_max_size=vocab_size+1000;
			vocab=(struct vocab_word *)calloc(vocab_max_size, sizeof(struct vocab_word));    //initialize memory for vocabulary
		}
		//
		goToDelimiter(':', fi);
		for (a=0; a<vocab_size; a++) {
			//fscanf(fi, "%d%d%s%d", &b, &vocab[a].cn, vocab[a].word, &vocab[a].class_index);
			fscanf(fi, "%d%d", &b, &vocab[a].cn);
			readWord(vocab[a].word, fi);
			fscanf(fi, "%d", &vocab[a].class_index);
			//printf("%d  %d  %s  %d\n", b, vocab[a].cn, vocab[a].word, vocab[a].class_index);
		}
		//
		if (neu0==NULL) initNet();		//memory allocation here
		//
		
		//由于对网络的权值分为两种模式,所以这里也应该分情况读入
		//对于大量的实数，二进制模式肯定更省空间
		if (filetype==TEXT) {
			goToDelimiter(':', fi);
			for (a=0; a<layer1_size; a++) {
				fscanf(fi, "%lf", &d);
				neu1[a].ac=d;
			}
		}
		if (filetype==BINARY) {
			fgetc(fi);
			for (a=0; a<layer1_size; a++) {
				fread(&fl, sizeof(fl), 1, fi);
				neu1[a].ac=fl;
			}
		}
		//
		if (filetype==TEXT) {
			goToDelimiter(':', fi);
			for (b=0; b<layer1_size; b++) {
				for (a=0; a<layer0_size; a++) {
					fscanf(fi, "%lf", &d);
					syn0[a+b*layer0_size].weight=d;
				}
			}
		}
		if (filetype==BINARY) {
			for (b=0; b<layer1_size; b++) {
				for (a=0; a<layer0_size; a++) {
					fread(&fl, sizeof(fl), 1, fi);
					syn0[a+b*layer0_size].weight=fl;
				}
			}
		}
		//
		if (filetype==TEXT) {
			goToDelimiter(':', fi);
			if (layerc_size==0) {	//no compress layer
				for (b=0; b<layer2_size; b++) {
					for (a=0; a<layer1_size; a++) {
						fscanf(fi, "%lf", &d);
						syn1[a+b*layer1_size].weight=d;
					}
				}
			}
			else
			{				//with compress layer
				for (b=0; b<layerc_size; b++) {
					for (a=0; a<layer1_size; a++) {
						fscanf(fi, "%lf", &d);
						syn1[a+b*layer1_size].weight=d;
					}
				}
				
				goToDelimiter(':', fi);
				
				for (b=0; b<layer2_size; b++) {
					for (a=0; a<layerc_size; a++) {
						fscanf(fi, "%lf", &d);
						sync[a+b*layerc_size].weight=d;
					}
				}
			}
		}
		if (filetype==BINARY) {
			if (layerc_size==0) {	//no compress layer
				for (b=0; b<layer2_size; b++) {
					for (a=0; a<layer1_size; a++) {
						fread(&fl, sizeof(fl), 1, fi);
						syn1[a+b*layer1_size].weight=fl;
					}
				}
			}
			else
			{				//with compress layer
				for (b=0; b<layerc_size; b++) {
					for (a=0; a<layer1_size; a++) {
						fread(&fl, sizeof(fl), 1, fi);
						syn1[a+b*layer1_size].weight=fl;
					}
				}
				
				for (b=0; b<layer2_size; b++) {
					for (a=0; a<layerc_size; a++) {
						fread(&fl, sizeof(fl), 1, fi);
						sync[a+b*layerc_size].weight=fl;
					}
				}
			}
		}
		//
		if (filetype==TEXT) {
			goToDelimiter(':', fi);		//direct conenctions
			long long aa;
			for (aa=0; aa<direct_size; aa++) {
				fscanf(fi, "%lf", &d);
				syn_d[aa]=d;
			}
		}
		//
		if (filetype==BINARY) {
			long long aa;
			for (aa=0; aa<direct_size; aa++) {
				fread(&fl, sizeof(fl), 1, fi);
				syn_d[aa]=fl;
				
				/*fread(&si, 2, 1, fi);
				fl=si/(float)(4*256);
				syn_d[aa]=fl;*/
			}
		}
		//
		
		saveWeights();
		
		fclose(fi);
}
 
 
//清除神经元的ac,er值
void CRnnLM::netFlush()   //cleans all activations and error vectors
{
    int a;
	
    for (a=0; a<layer0_size-layer1_size; a++) {
        neu0[a].ac=0;
        neu0[a].er=0;
    }
	
    for (a=layer0_size-layer1_size; a<layer0_size; a++) {   //last hidden layer is initialized to vector of 0.1 values to prevent unstability
        neu0[a].ac=0.1;
        neu0[a].er=0;
    }
	
    for (a=0; a<layer1_size; a++) {
        neu1[a].ac=0;
        neu1[a].er=0;
    }
    
    for (a=0; a<layerc_size; a++) {
        neuc[a].ac=0;
        neuc[a].er=0;
    }
    
    for (a=0; a<layer2_size; a++) {
        neu2[a].ac=0;
        neu2[a].er=0;
    }
}
```

下面这个函数将隐层神经元(论文中的状态层s(t))的ac值置1，s(t-1),即输入层layer1_size那部分的ac值置1，bptt+history清0，相关变量的含义在下面的图中：

![img](https://img-blog.csdn.net/20150331214301177?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

![img](https://img-blog.csdn.net/20150331214619921?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



```cpp
void CRnnLM::netReset()   //cleans hidden layer activation + bptt history
{
    int a, b;
	
	//将隐层神经元ac值置1
    for (a=0; a<layer1_size; a++) {
        neu1[a].ac=1.0;
    }
	
	//这个函数将隐层的神经元的ac值复制到输入层layer1_size部分
	//也就是输入层的layer1_size那部分的ac值置1
    copyHiddenLayerToInput();
	
    if (bptt>0) {
		//这里见图,容易理解,下标为0没被清除,下标为0没被清除是因为后面学习算法中会使用这个空位
		//这个在后面会看到
        for (a=1; a<bptt+bptt_block; a++) bptt_history[a]=0;
        for (a=bptt+bptt_block-1; a>1; a--) for (b=0; b<layer1_size; b++) {
            bptt_hidden[a*layer1_size+b].ac=0;
            bptt_hidden[a*layer1_size+b].er=0;
        }
    }
	//todo
    for (a=0; a<MAX_NGRAM_ORDER; a++) history[a]=0;
}
```

下面这个函数用于权值矩阵乘以神经元向量,并将计算结果存入目的神经元向量，type == 0时,计算的是神经元ac值,相当于计算srcmatrix × srcvec, 其中srcmatrix是(to-from)×(to2-from2)的矩阵，srcvec是(to2-from2)×1的列向量,得到的结果是(to-from)×1的列向量,该列向量的值存入dest中的ac值；type == 1, 计算神经元的er值,即(srcmatrix)^T × srcvec,T表示转置,转置后是(to2-from2)×(to-from),srcvec是(to-from)×1的列向量。这里的矩阵相乘比下面被注释掉的的快,好像是叫做Strassen’s method，记不太清楚了,很久之前看算法导论时学的,感兴趣的可以看看算法导论英文版第三版的79页，如果这不是Strassen’s method麻烦懂的朋友纠正一下~

```cpp
void CRnnLM::matrixXvector(struct neuron *dest, struct neuron *srcvec, struct synapse *srcmatrix, int matrix_width, int from, int to, int from2, int to2, int type)
{
    int a, b;
    real val1, val2, val3, val4;
    real val5, val6, val7, val8;
    
    if (type==0) {		//ac mod
		for (b=0; b<(to-from)/8; b++) {
			val1=0;
			val2=0;
			val3=0;
			val4=0;
			
			val5=0;
			val6=0;
			val7=0;
			val8=0;
			
			for (a=from2; a<to2; a++) {
				val1 += srcvec[a].ac * srcmatrix[a+(b*8+from+0)*matrix_width].weight;
				val2 += srcvec[a].ac * srcmatrix[a+(b*8+from+1)*matrix_width].weight;
				val3 += srcvec[a].ac * srcmatrix[a+(b*8+from+2)*matrix_width].weight;
				val4 += srcvec[a].ac * srcmatrix[a+(b*8+from+3)*matrix_width].weight;
				
				val5 += srcvec[a].ac * srcmatrix[a+(b*8+from+4)*matrix_width].weight;
				val6 += srcvec[a].ac * srcmatrix[a+(b*8+from+5)*matrix_width].weight;
				val7 += srcvec[a].ac * srcmatrix[a+(b*8+from+6)*matrix_width].weight;
				val8 += srcvec[a].ac * srcmatrix[a+(b*8+from+7)*matrix_width].weight;
			}
			dest[b*8+from+0].ac += val1;
			dest[b*8+from+1].ac += val2;
			dest[b*8+from+2].ac += val3;
			dest[b*8+from+3].ac += val4;
			
			dest[b*8+from+4].ac += val5;
			dest[b*8+from+5].ac += val6;
			dest[b*8+from+6].ac += val7;
			dest[b*8+from+7].ac += val8;
		}
		
		for (b=b*8; b<to-from; b++) {
			for (a=from2; a<to2; a++) {
				dest[b+from].ac += srcvec[a].ac * srcmatrix[a+(b+from)*matrix_width].weight;
			}
		}
    }
    else {		//er mod
		for (a=0; a<(to2-from2)/8; a++) {
			val1=0;
			val2=0;
			val3=0;
			val4=0;
			
			val5=0;
			val6=0;
			val7=0;
			val8=0;
			
			for (b=from; b<to; b++) {
				val1 += srcvec[b].er * srcmatrix[a*8+from2+0+b*matrix_width].weight;
				val2 += srcvec[b].er * srcmatrix[a*8+from2+1+b*matrix_width].weight;
				val3 += srcvec[b].er * srcmatrix[a*8+from2+2+b*matrix_width].weight;
				val4 += srcvec[b].er * srcmatrix[a*8+from2+3+b*matrix_width].weight;
				
				val5 += srcvec[b].er * srcmatrix[a*8+from2+4+b*matrix_width].weight;
				val6 += srcvec[b].er * srcmatrix[a*8+from2+5+b*matrix_width].weight;
				val7 += srcvec[b].er * srcmatrix[a*8+from2+6+b*matrix_width].weight;
				val8 += srcvec[b].er * srcmatrix[a*8+from2+7+b*matrix_width].weight;
			}
			dest[a*8+from2+0].er += val1;
			dest[a*8+from2+1].er += val2;
			dest[a*8+from2+2].er += val3;
			dest[a*8+from2+3].er += val4;
			
			dest[a*8+from2+4].er += val5;
			dest[a*8+from2+5].er += val6;
			dest[a*8+from2+6].er += val7;
			dest[a*8+from2+7].er += val8;
		}
		
		for (a=a*8; a<to2-from2; a++) {
			for (b=from; b<to; b++) {
				dest[a+from2].er += srcvec[b].er * srcmatrix[a+from2+b*matrix_width].weight;
			}
		}
		
		//这里防止梯度向量突发增长,导致训练失败
		//论文中有提及,少数情况下,误差可能会增长过大,这里限制
		if (gradient_cutoff>0)
			for (a=from2; a<to2; a++) {
				if (dest[a].er>gradient_cutoff) dest[a].er=gradient_cutoff;
				if (dest[a].er<-gradient_cutoff) dest[a].er=-gradient_cutoff;
			}
    }
    
	//struct neuron *dest, struct neuron *srcvec, struct synapse *srcmatrix, int matrix_width, int from, int to, int from2, int to2, int type
    //this is normal implementation (about 3x slower):
    
    /*if (type==0) {		//ac mod
	for (b=from; b<to; b++) {
	for (a=from2; a<to2; a++) {
	dest[b].ac += srcvec[a].ac * srcmatrix[a+b*matrix_width].weight;
	}
	}
    }
    else 		//er mod
    if (type==1) {
	for (a=from2; a<to2; a++) {
	for (b=from; b<to; b++) {
	dest[a].er += srcvec[b].er * srcmatrix[a+b*matrix_width].weight;
	}
	}
    }*/
}
```



# rnnlm源码分析(六)

2015年04月01日 10:10:14 [a635661820](https://me.csdn.net/a635661820) 阅读数：3023



这篇主要介绍一个网络前向计算的函数，内容量也挺大的。在此之前，解释一下rnn的输出层分解，和从神经网络的角度去看最大熵模型。先看一下原论文中最"标准"的rnn结构，这个结构是最原始的，后面会有系列的扩展，详见参考文献的第3篇。

![img](https://img-blog.csdn.net/20150401101611664?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



上图是最原始的循环神经网络的结构，关于它的前向计算和学习算法我在rnnlm原理以及bptt数学推导这篇文章有详细的写过。简要在写一下。上面这个网络的输出层有|V|维，在整个前向计算完毕后，我们得到的结果就是预测词的概率分布，即yt = P(wt+1 | wt,st-1), wt+1是要预测的词.

![img](https://img-blog.csdn.net/20150401102914270?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

这是我从前篇文章截图来的，由于网络输出层部分计算量很大，特别是当|V|很大时，计算量会更大。于是Mikolov提出了将输出层分解，并不是在所有单词上计算概率的分布，而是在某一类单词中计算概率分布，这样就会大大降低计算量。于是有了如下图：

![img](https://img-blog.csdn.net/20150401103424754?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

现在的计算预测词的概率分布变成了如下：

![img](https://img-blog.csdn.net/20150401103738491?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



其中，yV'(t)表示在V中的部分词语，至于这个部分是哪一部分，得看怎么把这些单词分类了，ci表示类别, wt+1表示期望词。这样在前向计算概率分布时，就先计算在期望词的类别的概率分布，然后在当前预测词所在类别的词语上计算概率分布，这个待会会在源代码中看到。



```cpp
void CRnnLM::computeNet(int last_word, int word)
{
	//last_word表示当前输入层所在的词
	//word表示要预测的词
    int a, b, c;
    real val;
    double sum;   //sum is used for normalization: it's better to have larger precision as many numbers are summed together here
    
	//将last_word对应的神经元ac值为1,也可以看做是对该词的1-of-V的编码
    if (last_word!=-1) neu0[last_word].ac=1;
	
    //下面计算输入到隐层的部分
    for (a=0; a<layer1_size; a++) neu1[a].ac=0;
    for (a=0; a<layerc_size; a++) neuc[a].ac=0;
    
	//这里计算的是s(t-1)与syn0的乘积
    matrixXvector(neu1, neu0, syn0, layer0_size, 0, layer1_size, layer0_size-layer1_size, layer0_size, 0);
	
	//这里计算将last_word编码后的向量(大小是vocab_size,分量只有一个为1,其余为0)与syn0的乘积
    for (b=0; b<layer1_size; b++) {
        a=last_word;
        if (a!=-1) neu1[b].ac += neu0[a].ac * syn0[a+b*layer0_size].weight;
    }
	
    //这里计算将上面隐层所得到的输入(ac值)经过sigmoid函数的映射结果
    for (a=0; a<layer1_size; a++) {
		//为数值稳定,将ac值大小限制在[-50,50]
		//论文中有提到模型的参数小一些泛化的结果好一些
		if (neu1[a].ac>50) neu1[a].ac=50;  
        if (neu1[a].ac<-50) neu1[a].ac=-50;  
        val=-neu1[a].ac;
		
		//fasterexp函数在fasexp.h中实现,应该比math.h中的exp快吧
        neu1[a].ac=1/(1+fasterexp(val));		//sigmoid函数即1/(1+e^(-x))
    }
    
    if (layerc_size>0) {
		
		//计算隐层到压缩层的结果
		matrixXvector(neuc, neu1, syn1, layer1_size, 0, layerc_size, 0, layer1_size, 0);
		
		//和上面类似，这里计算的是压缩层
		for (a=0; a<layerc_size; a++) {
			if (neuc[a].ac>50) neuc[a].ac=50;  //for numerical stability
			if (neuc[a].ac<-50) neuc[a].ac=-50;  //for numerical stability
			val=-neuc[a].ac;
			neuc[a].ac=1/(1+fasterexp(val));
		}
    }
	
    //1->2 class
	//输出层class_size部分ac值置0
    for (b=vocab_size; b<layer2_size; b++) neu2[b].ac=0;
    
	//计算压缩层到class层(输出层的一部分)
    if (layerc_size>0) {
		matrixXvector(neu2, neuc, sync, layerc_size, vocab_size, layer2_size, 0, layerc_size, 0);
    }
    else
    {
		//无压缩层,直接计算隐层到输出层
		matrixXvector(neu2, neu1, syn1, layer1_size, vocab_size, layer2_size, 0, layer1_size, 0);
    }
```



另外一个要说明的是最大熵模型，rnn结合了最大熵模型，直观的看上去是输入层与输出层连接了起来（虽然作者总是这么说，但我总觉的不能叫输入层和输出层连接起来，中间有过渡）。我们先看一下从神经网络的视角去看一个最大熵模型，这个神经网络就是没有隐层而已，其他和三层结构的一样，并且学习算法也是一样的。如下图：

![img](https://img-blog.csdn.net/20150401104524573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这是以二元模型为特征的最大熵模型,图是神经网络的角度看待的,无隐层,可以看到,如果是以三元模型为特征,那么输入层大小就是|V|^2, 并且任意一个时刻其中的神经元的激活值只可能有一个值为1，其余都是0。从这里面我们可以看到输入层中对应的任意一个历史特征，只有某个对应的神经值为1（记作第i个神经元），那么输出层的某个神经元激活值(记作第j个)就只与权值W中的第i列有关，某种程度由于输入层是1-of-V编码方式，我们可以认为一个特定的特征它所对应的参数就是W中某列，这一点就会RNN+ME中的基于哈希的实现中谈到。



```cpp
//最大熵模型n元特征计算
    if (direct_size>0) {
		//注意这是hash定义在if内的,也就是出了if外面就无法访问了
		//下面会看到每次都单独定义了局部的hash
		//hash[i]里面存放的是i+1元模型的特征在syn_d中对应的下标
		unsigned long long hash[MAX_NGRAM_ORDER];	//this will hold pointers to syn_d that contains hash parameters
		
		for (a=0; a<direct_order; a++) hash[a]=0;
		
		//下面就是将n元特征单独映射为一个值,这里的权值是针对class部分的
		for (a=0; a<direct_order; a++) {
			b=0;
			if (a>0) if (history[a-1]==-1) break;	//if OOV was in history, do not use this N-gram feature and higher orders
			hash[a]=PRIMES[0]*PRIMES[1];
			
			for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);	//update hash value based on words from the history
			
			//ME中对class的权值是在syn_d的前半部分,见图
			hash[a]=hash[a]%(direct_size/2);		//make sure that starting hash index is in the first half of syn_d (second part is reserved for history->words features)
		}
```



我们看一下表示rnn中最大熵模型的存储结构，如下图：



![img](https://img-blog.csdn.net/20150401105701882?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



我们把这段代码展开细走一下，假设direct_order = 3,并且没有OOV，如下：



```cpp
		out loop 1st:
		a = 0; a < 4
		b = 0;
		hash[0]=PRIMES[0]*PRIMES[1] = 108641969 * 116049371;
		
			  inner loop 1st:
			  b = 1; b<=0
			  退出内循环
			  hash[0]=hash[0]%(direct_size/2)
		  
		out loop 2nd:
		a = 1; a < 4;
		b = 0;
		hash[1]=PRIMES[0]*PRIMES[1] = 108641969 * 116049371;
		
			  inner loop 1st:
			  b = 1; b <= 1;
			  hash[a]= hash[a] + PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1)
			  = hash[1] + PRIMES[(1*PRIMES[1]+1)%36]*(history[0]+1);
			  退出内循环
			  hash[1]=hash[1]%(direct_size/2);
			  
		out loop 3rd:
		a = 2; a < 4;
		b = 0;
		hash[2]=PRIMES[0]*PRIMES[1] = 108641969 * 116049371;
		
			  inner loop 1st:
			  b = 1; b <= 2;
			  hash[2]= hash[2] + PRIMES[(2*PRIMES[1]+1)%36]*(history[0]+1);
			  
			  inner loop 2nd:
			  b = 2; b <= 2;
			  hash[2]= hash[2] + PRIMES[(2*PRIMES[2]+2)%PRIMES_SIZE]*(history[1]+1)
			  退出内循环
```


大概能看出，hash[i]表示i+1元模型的历史映射，因为在计算hash[i]时，考虑了history[0..i-1], 这个映射结果是作为syn_d数组的下标,i+1元词作为特征与输出层的连接真正的参数值在syn_d中。最后把这个函数剩下的部分贴出来，后面以注释为主了。如下：





```cpp
		//ME部分,计算在class层的概率分布,即P(c i |s(t))
		for (a=vocab_size; a<layer2_size; a++) {
			for (b=0; b<direct_order; b++) if (hash[b]) {
				neu2[a].ac+=syn_d[hash[b]];		//apply current parameter and move to the next one
				
				//这里解释一下,i+1元特征与输出层所连接的参数是放在syn_d中
				//是连续的,这里连续的长度分两种情况,一种是对class计算的,有class_size的长度
				//另一种是对word的，连续的长度是word所对应类别的词数
				//后面类似的代码同理
				hash[b]++;		//todo
			} else break;
		}
    }
	
    //activation 2   --softmax on classes
    // 20130425 - this is now a 'safe' softmax
	
	//这里softmax归一概率
	//这种方式主要是防止溢出,比如当ac值过大,exp(ac)可能就会溢出
    sum=0;
    real maxAc=-FLT_MAX;		//FLT_MAX表示float最大值
    for (a=vocab_size; a<layer2_size; a++)
        if (neu2[a].ac>maxAc) maxAc=neu2[a].ac; //this prevents the need to check for overflow
		for (a=vocab_size; a<layer2_size; a++)
			sum+=fasterexp(neu2[a].ac-maxAc);
		for (a=vocab_size; a<layer2_size; a++)
			neu2[a].ac=fasterexp(neu2[a].ac-maxAc)/sum; 
		
		if (gen>0) return;	//if we generate words, we don't know what current word is -> only classes are estimated and word is selected in testGen()
		
		
		//1->2 word
		
		if (word!=-1) {
			//置word所在类别的所有词的ac值为0
			for (c=0; c<class_cn[vocab[word].class_index]; c++) neu2[class_words[vocab[word].class_index][c]].ac=0;
			
			//计算待预测词的概率分布,这个概率分布是在word所在类别中的词上的
			if (layerc_size>0) {
				//word所在类别的词与压缩层的计算
				matrixXvector(neu2, neuc, sync, layerc_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layerc_size, 0);
			}
			else
			{
				//word所在类别的词与隐层的计算
				matrixXvector(neu2, neu1, syn1, layer1_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layer1_size, 0);
			}
		}
		
		//apply direct connections to words
		if (word!=-1) if (direct_size>0) {
			
			//ME中计算n元模型特征,这个是在word上的
			unsigned long long hash[MAX_NGRAM_ORDER];
			
			for (a=0; a<direct_order; a++) hash[a]=0;
			
			for (a=0; a<direct_order; a++) {
				b=0;
				if (a>0) if (history[a-1]==-1) break;
				hash[a]=PRIMES[0]*PRIMES[1]*(unsigned long long)(vocab[word].class_index+1);
				
				for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);
				hash[a]=(hash[a]%(direct_size/2))+(direct_size)/2;
			}
			
			//计算ME部分,待预测词的概率分布,这个概率分布是在word所在类别中的词上的
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				
				//这里的代码细节和上面是类似的
				for (b=0; b<direct_order; b++) if (hash[b]) {
					neu2[a].ac+=syn_d[hash[b]];
					hash[b]++;
					hash[b]=hash[b]%direct_size;
				} else break;
			}
		}
		
		//activation 2   --softmax on words
		// 130425 - this is now a 'safe' softmax
		
		//这里的归一概率和上面也是一样
		sum=0;
		if (word!=-1) { 
			maxAc=-FLT_MAX;
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				if (neu2[a].ac>maxAc) maxAc=neu2[a].ac;
			}
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				sum+=fasterexp(neu2[a].ac-maxAc);
			}
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				neu2[a].ac=fasterexp(neu2[a].ac-maxAc)/sum; //this prevents the need to check for overflow
			}
		}
}
```

**由于这个函数无奈的被分乱了，必须连起来一起看才行，所以最后把这个函数完整的带注释的放在下面：**



```cpp
//网络前向,计算概率分布
void CRnnLM::computeNet(int last_word, int word)
{
	//last_word表示当前输入层所在的词
	//word表示要预测的词
    int a, b, c;
    real val;
    double sum;   //sum is used for normalization: it's better to have larger precision as many numbers are summed together here
    
	//将last_word对应的神经元ac值为1,也可以看做是对该词的1-of-V的编码
    if (last_word!=-1) neu0[last_word].ac=1;
	
    //下面计算输入到隐层的部分
    for (a=0; a<layer1_size; a++) neu1[a].ac=0;
    for (a=0; a<layerc_size; a++) neuc[a].ac=0;
    
	//这里计算的是s(t-1)与syn0的乘积
    matrixXvector(neu1, neu0, syn0, layer0_size, 0, layer1_size, layer0_size-layer1_size, layer0_size, 0);
	
	//这里计算将last_word编码后的向量(大小是vocab_size,分量只有一个为1,其余为0)与syn0的乘积
    for (b=0; b<layer1_size; b++) {
        a=last_word;
        if (a!=-1) neu1[b].ac += neu0[a].ac * syn0[a+b*layer0_size].weight;
    }
	
    //这里计算将上面隐层所得到的输入(ac值)经过sigmoid函数的映射结果
    for (a=0; a<layer1_size; a++) {
		//为数值稳定,将ac值大小限制在[-50,50]
		//论文中有提到模型的参数小一些泛化的结果好一些
		if (neu1[a].ac>50) neu1[a].ac=50;  
        if (neu1[a].ac<-50) neu1[a].ac=-50;  
        val=-neu1[a].ac;
		
		//fasterexp函数在fasexp.h中实现,应该比math.h中的exp快吧
        neu1[a].ac=1/(1+fasterexp(val));		//sigmoid函数即1/(1+e^(-x))
    }
    
    if (layerc_size>0) {
		
		//计算隐层到压缩层的结果
		matrixXvector(neuc, neu1, syn1, layer1_size, 0, layerc_size, 0, layer1_size, 0);
		
		//和上面类似，这里计算的是压缩层
		for (a=0; a<layerc_size; a++) {
			if (neuc[a].ac>50) neuc[a].ac=50;  //for numerical stability
			if (neuc[a].ac<-50) neuc[a].ac=-50;  //for numerical stability
			val=-neuc[a].ac;
			neuc[a].ac=1/(1+fasterexp(val));
		}
    }
	
    //1->2 class
	//输出层class_size部分ac值置0
    for (b=vocab_size; b<layer2_size; b++) neu2[b].ac=0;
    
	//计算压缩层到class层(输出层的一部分)
    if (layerc_size>0) {
		matrixXvector(neu2, neuc, sync, layerc_size, vocab_size, layer2_size, 0, layerc_size, 0);
    }
    else
    {
		//无压缩层,直接计算隐层到输出层
		matrixXvector(neu2, neu1, syn1, layer1_size, vocab_size, layer2_size, 0, layer1_size, 0);
    }
	
    //最大熵模型n元特征计算
    if (direct_size>0) {
		//注意这是hash定义在if内的,也就是出了if外面就无法访问了
		//下面会看到每次都单独定义了局部的hash
		//hash[i]里面存放的是i+1元模型的特征在syn_d中对应的下标
		unsigned long long hash[MAX_NGRAM_ORDER];	//this will hold pointers to syn_d that contains hash parameters
		
		for (a=0; a<direct_order; a++) hash[a]=0;
		
		//下面就是将n元特征单独映射为一个值,这里的权值是针对class部分的
		for (a=0; a<direct_order; a++) {
			b=0;
			if (a>0) if (history[a-1]==-1) break;	//if OOV was in history, do not use this N-gram feature and higher orders
			hash[a]=PRIMES[0]*PRIMES[1];
			
			for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);	//update hash value based on words from the history
			
			//ME中对class的权值是在syn_d的前半部分,见图
			hash[a]=hash[a]%(direct_size/2);		//make sure that starting hash index is in the first half of syn_d (second part is reserved for history->words features)
		}
		
		/*我们把这段代码展开细走一下，假设direct_order = 3,并且没有OOV
		out loop 1st:
		a = 0; a < 4
		b = 0;
		hash[0]=PRIMES[0]*PRIMES[1] = 108641969 * 116049371;
		
			  inner loop 1st:
			  b = 1; b<=0
			  退出内循环
			  hash[0]=hash[0]%(direct_size/2)
		  
		out loop 2nd:
		a = 1; a < 4;
		b = 0;
		hash[1]=PRIMES[0]*PRIMES[1] = 108641969 * 116049371;
		
			  inner loop 1st:
			  b = 1; b <= 1;
			  hash[a]= hash[a] + PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1)
			  = hash[1] + PRIMES[(1*PRIMES[1]+1)%36]*(history[0]+1);
			  退出内循环
			  hash[1]=hash[1]%(direct_size/2);
			  
		out loop 3rd:
		a = 2; a < 4;
		b = 0;
		hash[2]=PRIMES[0]*PRIMES[1] = 108641969 * 116049371;
		
			  inner loop 1st:
			  b = 1; b <= 2;
			  hash[2]= hash[2] + PRIMES[(2*PRIMES[1]+1)%36]*(history[0]+1);
			  
			  inner loop 2nd:
			  b = 2; b <= 2;
			  hash[2]= hash[2] + PRIMES[(2*PRIMES[2]+2)%PRIMES_SIZE]*(history[1]+1)
			  退出内循环
					
		大概能看出，hash[i]表示i+1元模型的历史映射，因为在计算hash[i]时，考虑了history[0..i-1]
					  这个映射结果是作为syn_d数组的下标,i+1元词作为特征与输出层的连接真正的参数值在syn_d中
		*/	
		
		//ME部分,计算在class层的概率分布,即P(c i |s(t))
		for (a=vocab_size; a<layer2_size; a++) {
			for (b=0; b<direct_order; b++) if (hash[b]) {
				neu2[a].ac+=syn_d[hash[b]];		//apply current parameter and move to the next one
				
				//这里解释一下,i+1元特征与输出层所连接的参数是放在syn_d中
				//是连续的,这里连续的长度分两种情况,一种是对class计算的,有class_size的长度
				//另一种是对word的，连续的长度是word所对应类别的词数
				//后面类似的代码同理
				hash[b]++;		//todo
			} else break;
		}
    }
	
    //activation 2   --softmax on classes
    // 20130425 - this is now a 'safe' softmax
	
	//这里softmax归一概率
	//这种方式主要是防止溢出,比如当ac值过大,exp(ac)可能就会溢出
    sum=0;
    real maxAc=-FLT_MAX;		//FLT_MAX表示float最大值
    for (a=vocab_size; a<layer2_size; a++)
        if (neu2[a].ac>maxAc) maxAc=neu2[a].ac; //this prevents the need to check for overflow
		for (a=vocab_size; a<layer2_size; a++)
			sum+=fasterexp(neu2[a].ac-maxAc);
		for (a=vocab_size; a<layer2_size; a++)
			neu2[a].ac=fasterexp(neu2[a].ac-maxAc)/sum; 
		
		if (gen>0) return;	//if we generate words, we don't know what current word is -> only classes are estimated and word is selected in testGen()
		
		
		//1->2 word
		
		if (word!=-1) {
			//置word所在类别的所有词的ac值为0
			for (c=0; c<class_cn[vocab[word].class_index]; c++) neu2[class_words[vocab[word].class_index][c]].ac=0;
			
			//计算待预测词的概率分布,这个概率分布是在word所在类别中的词上的
			if (layerc_size>0) {
				//word所在类别的词与压缩层的计算
				matrixXvector(neu2, neuc, sync, layerc_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layerc_size, 0);
			}
			else
			{
				//word所在类别的词与隐层的计算
				matrixXvector(neu2, neu1, syn1, layer1_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layer1_size, 0);
			}
		}
		
		//apply direct connections to words
		if (word!=-1) if (direct_size>0) {
			
			//ME中计算n元模型特征,这个是在word上的
			unsigned long long hash[MAX_NGRAM_ORDER];
			
			for (a=0; a<direct_order; a++) hash[a]=0;
			
			for (a=0; a<direct_order; a++) {
				b=0;
				if (a>0) if (history[a-1]==-1) break;
				hash[a]=PRIMES[0]*PRIMES[1]*(unsigned long long)(vocab[word].class_index+1);
				
				for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);
				hash[a]=(hash[a]%(direct_size/2))+(direct_size)/2;
			}
			
			//计算ME部分,待预测词的概率分布,这个概率分布是在word所在类别中的词上的
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				
				//这里的代码细节和上面是类似的
				for (b=0; b<direct_order; b++) if (hash[b]) {
					neu2[a].ac+=syn_d[hash[b]];
					hash[b]++;
					hash[b]=hash[b]%direct_size;
				} else break;
			}
		}
		
		//activation 2   --softmax on words
		// 130425 - this is now a 'safe' softmax
		
		//这里的归一概率和上面也是一样
		sum=0;
		if (word!=-1) { 
			maxAc=-FLT_MAX;
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				if (neu2[a].ac>maxAc) maxAc=neu2[a].ac;
			}
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				sum+=fasterexp(neu2[a].ac-maxAc);
			}
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				neu2[a].ac=fasterexp(neu2[a].ac-maxAc)/sum; //this prevents the need to check for overflow
			}
		}
}
```









# rnnlm源码分析(七)

2015年04月01日 11:13:23 [a635661820](https://me.csdn.net/a635661820) 阅读数：2529



前一篇是网络的前向计算，这篇是网络的学习算法，学习算法我在rnnlm原理及BPTT数学推导中介绍了。学习算法主要更新的地方在网络中的权值，这个最终版本的网络的权值大体可以分为三个部分来看：第一个是网络中类似输入到隐层的权值，隐层到输出层的权值。第二个是网络中ME的部分，即输入层到输出层的权值部分。第三个来看是BPTT的部分。我先把整个网络的ME+Rnn图放上来，然后再贴带注释的源码，结构图如下：



![img](https://img-blog.csdn.net/20150401111923391?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



下面代码还是分成两部分来看，一部分是更新非BPTT部分，一个是更新BPTT部分，如下：



```cpp
//反传误差,更新网络权值
void CRnnLM::learnNet(int last_word, int word)
{
	//word表示要预测的词,last_word表示当前输入层所在的词
    int a, b, c, t, step;
    real beta2, beta3;
	
	//alpha表示学习率,初始值为0.1, beta初始值为0.0000001;
    beta2=beta*alpha;
	//这里注释不懂，希望明白的朋友讲下~
    beta3=beta2*1;	//beta3 can be possibly larger than beta2, as that is useful on small datasets (if the final model is to be interpolated wich backoff model) - todo in the future
	
    if (word==-1) return;
	
    //compute error vectors,计算输出层的(只含word所在类别的所有词)误差向量
    for (c=0; c<class_cn[vocab[word].class_index]; c++) {
		a=class_words[vocab[word].class_index][c];
        neu2[a].er=(0-neu2[a].ac);
    }
    neu2[word].er=(1-neu2[word].ac);	//word part
	
    //flush error
    for (a=0; a<layer1_size; a++) neu1[a].er=0;
    for (a=0; a<layerc_size; a++) neuc[a].er=0;
	
	//计算输出层的class部分的误差向量
    for (a=vocab_size; a<layer2_size; a++) {
        neu2[a].er=(0-neu2[a].ac);
    }
    neu2[vocab[word].class_index+vocab_size].er=(1-neu2[vocab[word].class_index+vocab_size].ac);	//class part
    
    //计算特征所在syn_d中的下标，和上面一样，针对ME中word部分
    if (direct_size>0) {	//learn direct connections between words
		if (word!=-1) {
			unsigned long long hash[MAX_NGRAM_ORDER];
			
			for (a=0; a<direct_order; a++) hash[a]=0;
			
			for (a=0; a<direct_order; a++) {
				b=0;
				if (a>0) if (history[a-1]==-1) break;
				hash[a]=PRIMES[0]*PRIMES[1]*(unsigned long long)(vocab[word].class_index+1);
				
				for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);
				hash[a]=(hash[a]%(direct_size/2))+(direct_size)/2;
			}
			
			//更新ME中的权值部分，这部分是正对word的
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				
				//这里的更新公式很容易推导,利用梯度上升,和RNN是一样的
				//详见我的这篇文章,另外这里不同的是权值是放在一维数组里面的,所以程序写法有点不同
				for (b=0; b<direct_order; b++) if (hash[b]) {
					syn_d[hash[b]]+=alpha*neu2[a].er - syn_d[hash[b]]*beta3;
					hash[b]++;
					hash[b]=hash[b]%direct_size;
				} else break;
			}
		}
    }
    
	//计算n元模型特征,这是对class计算的
    //learn direct connections to classes
    if (direct_size>0) {	//learn direct connections between words and classes
		unsigned long long hash[MAX_NGRAM_ORDER];
		
		for (a=0; a<direct_order; a++) hash[a]=0;
		
		for (a=0; a<direct_order; a++) {
			b=0;
			if (a>0) if (history[a-1]==-1) break;
			hash[a]=PRIMES[0]*PRIMES[1];
			
			for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);
			hash[a]=hash[a]%(direct_size/2);
		}
		
		//和上面一样，更新ME中权值部分，这是对class的
		for (a=vocab_size; a<layer2_size; a++) {
			for (b=0; b<direct_order; b++) if (hash[b]) {
				syn_d[hash[b]]+=alpha*neu2[a].er - syn_d[hash[b]]*beta3;
				hash[b]++;
			} else break;
		}
    }
    //
    
    //含压缩层的情况，更新sync, syn1
    if (layerc_size>0) {
		//将输出层的误差传递到压缩层,即计算部分V × eo^T(符号含义表示见图)
		matrixXvector(neuc, neu2, sync, layerc_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layerc_size, 1);
		
		//这里把一维的权值转换为二维的权值矩阵好理解一些
		//注意这里的t相当于定位到了参数矩阵的行,下面的下标a相当于列
		t=class_words[vocab[word].class_index][0]*layerc_size;
		for (c=0; c<class_cn[vocab[word].class_index]; c++) {
			b=class_words[vocab[word].class_index][c];
			//这里的更新公式见我写的另一篇文章的rnn推导
			//并且每训练10次才L2正规化一次 
			if ((counter%10)==0)	//regularization is done every 10. step
				for (a=0; a<layerc_size; a++) sync[a+t].weight+=alpha*neu2[b].er*neuc[a].ac - sync[a+t].weight*beta2;
				else
					for (a=0; a<layerc_size; a++) sync[a+t].weight+=alpha*neu2[b].er*neuc[a].ac;
					//参数矩阵下移动一行
					t+=layerc_size;
		}
		
		//将输出层的误差传递到压缩层,即计算部分V × eo^T(符号含义表示见图)
		//这里计算输出层中class部分到压缩层
		matrixXvector(neuc, neu2, sync, layerc_size, vocab_size, layer2_size, 0, layerc_size, 1);		//propagates errors 2->c for classes
		
		//这里同样相当于定位的权值矩阵中的行,下面的a表示列
		c=vocab_size*layerc_size;
		
		//更新和上面公式是一样的,不同的是更新的权值数组中不同的部分
		for (b=vocab_size; b<layer2_size; b++) {
			if ((counter%10)==0) {	
				for (a=0; a<layerc_size; a++) sync[a+c].weight+=alpha*neu2[b].er*neuc[a].ac - sync[a+c].weight*beta2;	//weight c->2 update
			}
			else {
				for (a=0; a<layerc_size; a++) sync[a+c].weight+=alpha*neu2[b].er*neuc[a].ac;	//weight c->2 update
			}
			//下一行
			c+=layerc_size;
		}
		
		//这里是误差向量,即论文中的e hj (t)
		for (a=0; a<layerc_size; a++) neuc[a].er=neuc[a].er*neuc[a].ac*(1-neuc[a].ac);    //error derivation at compression layer
		
		////
		//下面都是同理，将误差再往下传,计算syn1(二维) × ec^T, ec表示压缩层的误差向量
		matrixXvector(neu1, neuc, syn1, layer1_size, 0, layerc_size, 0, layer1_size, 1);		//propagates errors c->1
		
		//更新syn1，相应的见公式
		for (b=0; b<layerc_size; b++) {
			for (a=0; a<layer1_size; a++) syn1[a+b*layer1_size].weight+=alpha*neuc[b].er*neu1[a].ac;	//weight 1->c update
		}
    }
    else		//无压缩层的情况，更新syn1
    {
		//下面的情况和上面类似，只是少了一个压缩层
		matrixXvector(neu1, neu2, syn1, layer1_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layer1_size, 1);
		
		t=class_words[vocab[word].class_index][0]*layer1_size;
		for (c=0; c<class_cn[vocab[word].class_index]; c++) {
			b=class_words[vocab[word].class_index][c];
			if ((counter%10)==0)	//regularization is done every 10. step
				for (a=0; a<layer1_size; a++) syn1[a+t].weight+=alpha*neu2[b].er*neu1[a].ac - syn1[a+t].weight*beta2;
				else
					for (a=0; a<layer1_size; a++) syn1[a+t].weight+=alpha*neu2[b].er*neu1[a].ac;
					t+=layer1_size;
		}
		//
		matrixXvector(neu1, neu2, syn1, layer1_size, vocab_size, layer2_size, 0, layer1_size, 1);		//propagates errors 2->1 for classes
		
		c=vocab_size*layer1_size;
		for (b=vocab_size; b<layer2_size; b++) {
			if ((counter%10)==0) {	//regularization is done every 10. step
				for (a=0; a<layer1_size; a++) syn1[a+c].weight+=alpha*neu2[b].er*neu1[a].ac - syn1[a+c].weight*beta2;	//weight 1->2 update
			}
			else {
				for (a=0; a<layer1_size; a++) syn1[a+c].weight+=alpha*neu2[b].er*neu1[a].ac;	//weight 1->2 update
			}
			c+=layer1_size;
		}
    }
    
    //
    //到这里,上面部分把到隐层的部分更新完毕
    ///////////////
	
	//这里就是最常规的RNN,即t时刻往前只展开了s(t-1)
    if (bptt<=1) {		//bptt==1 -> normal BP
		
		//计算隐层的误差,即eh(t)
		for (a=0; a<layer1_size; a++) neu1[a].er=neu1[a].er*neu1[a].ac*(1-neu1[a].ac);    //error derivation at layer 1
		
		//这部分更新隐层到word部分的权值
		//注意由于word部分是1-of-V编码
		//所以这里更新的循环式上有点不同,并且下面的更新都是每10次训练更新才进行一次L2正规化
		a=last_word;
		if (a!=-1) {
			if ((counter%10)==0)			
				for (b=0; b<layer1_size; b++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac - syn0[a+b*layer0_size].weight*beta2;
				else
					//我们可以把这里的权值看做一个矩阵
					//b表示行,a表示列,非L2正规化的完整更新式如下：
					/*for (b=0; b<layer1_size; b++) {
					for (k=0; k<vocab_size; k++) {
					syn0[k+b*layer0_size].weight+=alpha*neu1[b].er*neu0[k].ac;
					}
		}*/
		//但由于neu0[k]==1只有当k==a时,所以为了加快循环计算的速度,更新就变为如下了
		//下面的代码是类似的道理,其实这里的neu0[a].ac可以直接省略掉,因为本身值为1				
		for (b=0; b<layer1_size; b++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac;
		}
		
		//这部分更新隐层到s(t-1)的权值
		if ((counter%10)==0) {
			for (b=0; b<layer1_size; b++) for (a=layer0_size-layer1_size; a<layer0_size; a++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac - syn0[a+b*layer0_size].weight*beta2;
		}
		else {
			for (b=0; b<layer1_size; b++) for (a=layer0_size-layer1_size; a<layer0_size; a++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac;
		}
    }
```

下面是更新BPTT的部分, 还是先把BPTT算法部分的数据结构图上来，这样更方便对照：



![img](https://img-blog.csdn.net/20150401112314489?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



![img](https://img-blog.csdn.net/20150401162235718?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





```cpp
else		//BPTT
    {
		//BPTT部分权值就不在用syn0了，而是用bptt_syn0
		//神经元部分的存储用bptt_hidden
		
		//将隐层的神经元信息复制到bptt_hidden，这里对neu1做备份
		for (b=0; b<layer1_size; b++) bptt_hidden[b].ac=neu1[b].ac;
		for (b=0; b<layer1_size; b++) bptt_hidden[b].er=neu1[b].er;
		
		//bptt_block第一个条件是控制BPTT的,因为不能每训练一个word都用BPTT来深度更新参数
		//这样每进行一次训练计算量太大,过于耗时
		//word==0表示当前一个句子结束
		//或者当independent开关不为0时,下一个词是句子的结束,则进行BPTT
		if (((counter%bptt_block)==0) || (independent && (word==0))) {
			
			//step表示
			for (step=0; step<bptt+bptt_block-2; step++) {
				
				//计算隐层的误差向量,即eh(t)
				for (a=0; a<layer1_size; a++) neu1[a].er=neu1[a].er*neu1[a].ac*(1-neu1[a].ac);    //error derivation at layer 1
				
				//更新的是输入层中vocab_size那部分权值
				//bptt_history下标从0开始依次存放的是wt, wt-1, wt-2...
				//比如当step == 0时,相当于正在进行普通的BPTT,这时bptt_history[step]表示当前输入word的在vocab中的索引
				a=bptt_history[step];	
				if (a!=-1)
					for (b=0; b<layer1_size; b++) {
						//由于输入层word部分是1-of-V编码,所以neu0[a].ac==1 所以这里后面没有乘以它
						//在step == 0时,bptt_syn0相当于原来的syn0
						bptt_syn0[a+b*layer0_size].weight+=alpha*neu1[b].er;
					}
					
					//为从隐层往状态层传误差做准备
					for (a=layer0_size-layer1_size; a<layer0_size; a++) neu0[a].er=0;
					
					//从隐层传递误差到状态层
					matrixXvector(neu0, neu1, syn0, layer0_size, 0, layer1_size, layer0_size-layer1_size, layer0_size, 1);		//propagates errors 1->0
					
					//更新隐层到状态层的权值
					//之所以先计算误差的传递就是因为更新完权值会发生改变
					for (b=0; b<layer1_size; b++) for (a=layer0_size-layer1_size; a<layer0_size; a++) {
						//neu0[a].er += neu1[b].er * syn0[a+b*layer0_size].weight;
						bptt_syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac;
					}
					
					//todo这里我理解是把论文中的s(t-1)复制到s(t)，准备下一次循环
					for (a=0; a<layer1_size; a++) {		//propagate error from time T-n to T-n-1
						//s(t-1)的误差来自于两部分(见图更明显)：1.s(t)传过来的 2.t-1时刻的输出层传过来的,谢谢博友qq_16478429提出来的 
						neu1[a].er=neu0[a+layer0_size-layer1_size].er + bptt_hidden[(step+1)*layer1_size+a].er;
					}
					
					//历史中的s(t-2)复制到s(t-1)
					if (step<bptt+bptt_block-3)
						for (a=0; a<layer1_size; a++) {
							neu1[a].ac=bptt_hidden[(step+1)*layer1_size+a].ac;
							neu0[a+layer0_size-layer1_size].ac=bptt_hidden[(step+2)*layer1_size+a].ac;
						}
```

到这里，无奈再把代码打断一下，把上面两个关键循环按照step的增加走一下,并假设当前时刻为t，展开一下上面的循环：



```cpp
step = 0:
第一个for循环完成:
neu1.er = neu0.er + btpp_hidden.er -> s(t) = s(t-1) + s(t-1)
第二个for循环完成:
neu1.ac = bptt_hidden.ac -> s(t) = s(t-1)
neu0.ac = bptt_hidden.ac -> s(t-1) = s(t-2)
step = 1:
第一个for循环完成:
neu1.er = neu0.er + btpp_hidden.er -> s(t-1) = s(t-2) + s(t-2)
第二个for循环完成:
neu1.ac = bptt_hidden.ac -> s(t-1) = s(t-2)
neu0.ac = bptt_hidden.ac -> s(t-2) = s(t-3)
```

通过上面列出的步骤，大概能知道原理，其中neu0,和bptt_hidden所对应的s(t-1)有不同,前者的er值隐层传下来的 

后者的er值是在t-1时刻,压缩层传下来的



然后继续被打断的代码：



```cpp
}
			
			//清空历史信息中的er值
			for (a=0; a<(bptt+bptt_block)*layer1_size; a++) {
				bptt_hidden[a].er=0;
			}
			
			//这里恢复neu1,因为neu1反复在循环中使用
			for (b=0; b<layer1_size; b++) neu1[b].ac=bptt_hidden[b].ac;		//restore hidden layer after bptt
			
			
			//将BPTT后的权值改变作用到syn0上
			for (b=0; b<layer1_size; b++) {		//copy temporary syn0
				
				//bptt完毕,将bptt_syn0的权值复制到syn0中来,这是复制状态层部分
				if ((counter%10)==0) {
					for (a=layer0_size-layer1_size; a<layer0_size; a++) {
						syn0[a+b*layer0_size].weight+=bptt_syn0[a+b*layer0_size].weight - syn0[a+b*layer0_size].weight*beta2;
						bptt_syn0[a+b*layer0_size].weight=0;
					}
				}
				else {
					for (a=layer0_size-layer1_size; a<layer0_size; a++) {
						syn0[a+b*layer0_size].weight+=bptt_syn0[a+b*layer0_size].weight;
						bptt_syn0[a+b*layer0_size].weight=0;
					}
				}
				
				//bptt完毕,将bptt_syn0的权值复制到syn0中来,这是word部分
				if ((counter%10)==0) {
					for (step=0; step<bptt+bptt_block-2; step++) if (bptt_history[step]!=-1) {
						syn0[bptt_history[step]+b*layer0_size].weight+=bptt_syn0[bptt_history[step]+b*layer0_size].weight - syn0[bptt_history[step]+b*layer0_size].weight*beta2;
						bptt_syn0[bptt_history[step]+b*layer0_size].weight=0;
					}
				}
				else {
					//因为输入的word层是1-of-V编码,并不是全部的权值改变
					//这样写可以加快计算速度,避免不必要的循环检索
					for (step=0; step<bptt+bptt_block-2; step++) if (bptt_history[step]!=-1) {
						syn0[bptt_history[step]+b*layer0_size].weight+=bptt_syn0[bptt_history[step]+b*layer0_size].weight;
						bptt_syn0[bptt_history[step]+b*layer0_size].weight=0;
					}
				}
			}
		}
    }	
}
 
//将隐层神经元的ac值复制到输出层后layer1_size那部分
void CRnnLM::copyHiddenLayerToInput()
{
    int a;
	
    for (a=0; a<layer1_size; a++) {
        neu0[a+layer0_size-layer1_size].ac=neu1[a].ac;
    }
}
```

**最后仍然和前面一样，把整个函数完整的注释版贴在下面：**



```cpp
//反传误差,更新网络权值
void CRnnLM::learnNet(int last_word, int word)
{
	//word表示要预测的词,last_word表示当前输入层所在的词
    int a, b, c, t, step;
    real beta2, beta3;
	
	//alpha表示学习率,初始值为0.1, beta初始值为0.0000001;
    beta2=beta*alpha;
	//这里注释不懂，希望明白的朋友讲下~
    beta3=beta2*1;	//beta3 can be possibly larger than beta2, as that is useful on small datasets (if the final model is to be interpolated wich backoff model) - todo in the future
	
    if (word==-1) return;
	
    //compute error vectors,计算输出层的(只含word所在类别的所有词)误差向量
    for (c=0; c<class_cn[vocab[word].class_index]; c++) {
		a=class_words[vocab[word].class_index][c];
        neu2[a].er=(0-neu2[a].ac);
    }
    neu2[word].er=(1-neu2[word].ac);	//word part
	
    //flush error
    for (a=0; a<layer1_size; a++) neu1[a].er=0;
    for (a=0; a<layerc_size; a++) neuc[a].er=0;
	
	//计算输出层的class部分的误差向量
    for (a=vocab_size; a<layer2_size; a++) {
        neu2[a].er=(0-neu2[a].ac);
    }
    neu2[vocab[word].class_index+vocab_size].er=(1-neu2[vocab[word].class_index+vocab_size].ac);	//class part
    
    //计算特征所在syn_d中的下标，和上面一样，针对ME中word部分
    if (direct_size>0) {	//learn direct connections between words
		if (word!=-1) {
			unsigned long long hash[MAX_NGRAM_ORDER];
			
			for (a=0; a<direct_order; a++) hash[a]=0;
			
			for (a=0; a<direct_order; a++) {
				b=0;
				if (a>0) if (history[a-1]==-1) break;
				hash[a]=PRIMES[0]*PRIMES[1]*(unsigned long long)(vocab[word].class_index+1);
				
				for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);
				hash[a]=(hash[a]%(direct_size/2))+(direct_size)/2;
			}
			
			//更新ME中的权值部分，这部分是正对word的
			for (c=0; c<class_cn[vocab[word].class_index]; c++) {
				a=class_words[vocab[word].class_index][c];
				
				//这里的更新公式很容易推导,利用梯度上升,和RNN是一样的
				//详见我的这篇文章,另外这里不同的是权值是放在一维数组里面的,所以程序写法有点不同
				for (b=0; b<direct_order; b++) if (hash[b]) {
					syn_d[hash[b]]+=alpha*neu2[a].er - syn_d[hash[b]]*beta3;
					hash[b]++;
					hash[b]=hash[b]%direct_size;
				} else break;
			}
		}
    }
    
	//计算n元模型特征,这是对class计算的
    //learn direct connections to classes
    if (direct_size>0) {	//learn direct connections between words and classes
		unsigned long long hash[MAX_NGRAM_ORDER];
		
		for (a=0; a<direct_order; a++) hash[a]=0;
		
		for (a=0; a<direct_order; a++) {
			b=0;
			if (a>0) if (history[a-1]==-1) break;
			hash[a]=PRIMES[0]*PRIMES[1];
			
			for (b=1; b<=a; b++) hash[a]+=PRIMES[(a*PRIMES[b]+b)%PRIMES_SIZE]*(unsigned long long)(history[b-1]+1);
			hash[a]=hash[a]%(direct_size/2);
		}
		
		//和上面一样，更新ME中权值部分，这是对class的
		for (a=vocab_size; a<layer2_size; a++) {
			for (b=0; b<direct_order; b++) if (hash[b]) {
				syn_d[hash[b]]+=alpha*neu2[a].er - syn_d[hash[b]]*beta3;
				hash[b]++;
			} else break;
		}
    }
    //
    
    //含压缩层的情况，更新sync, syn1
    if (layerc_size>0) {
		//将输出层的误差传递到压缩层,即计算部分V × eo^T(符号含义表示见图)
		matrixXvector(neuc, neu2, sync, layerc_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layerc_size, 1);
		
		//这里把一维的权值转换为二维的权值矩阵好理解一些
		//注意这里的t相当于定位到了参数矩阵的行,下面的下标a相当于列
		t=class_words[vocab[word].class_index][0]*layerc_size;
		for (c=0; c<class_cn[vocab[word].class_index]; c++) {
			b=class_words[vocab[word].class_index][c];
			//这里的更新公式见我写的另一篇文章的rnn推导
			//并且每训练10次才L2正规化一次 
			if ((counter%10)==0)	//regularization is done every 10. step
				for (a=0; a<layerc_size; a++) sync[a+t].weight+=alpha*neu2[b].er*neuc[a].ac - sync[a+t].weight*beta2;
				else
					for (a=0; a<layerc_size; a++) sync[a+t].weight+=alpha*neu2[b].er*neuc[a].ac;
					//参数矩阵下移动一行
					t+=layerc_size;
		}
		
		//将输出层的误差传递到压缩层,即计算部分V × eo^T(符号含义表示见图)
		//这里计算输出层中class部分到压缩层
		matrixXvector(neuc, neu2, sync, layerc_size, vocab_size, layer2_size, 0, layerc_size, 1);		//propagates errors 2->c for classes
		
		//这里同样相当于定位的权值矩阵中的行,下面的a表示列
		c=vocab_size*layerc_size;
		
		//更新和上面公式是一样的,不同的是更新的权值数组中不同的部分
		for (b=vocab_size; b<layer2_size; b++) {
			if ((counter%10)==0) {	
				for (a=0; a<layerc_size; a++) sync[a+c].weight+=alpha*neu2[b].er*neuc[a].ac - sync[a+c].weight*beta2;	//weight c->2 update
			}
			else {
				for (a=0; a<layerc_size; a++) sync[a+c].weight+=alpha*neu2[b].er*neuc[a].ac;	//weight c->2 update
			}
			//下一行
			c+=layerc_size;
		}
		
		//这里是误差向量,即论文中的e hj (t)
		for (a=0; a<layerc_size; a++) neuc[a].er=neuc[a].er*neuc[a].ac*(1-neuc[a].ac);    //error derivation at compression layer
		
		////
		//下面都是同理，将误差再往下传,计算syn1(二维) × ec^T, ec表示压缩层的误差向量
		matrixXvector(neu1, neuc, syn1, layer1_size, 0, layerc_size, 0, layer1_size, 1);		//propagates errors c->1
		
		//更新syn1，相应的见公式
		for (b=0; b<layerc_size; b++) {
			for (a=0; a<layer1_size; a++) syn1[a+b*layer1_size].weight+=alpha*neuc[b].er*neu1[a].ac;	//weight 1->c update
		}
    }
    else		//无压缩层的情况，更新syn1
    {
		//下面的情况和上面类似，只是少了一个压缩层
		matrixXvector(neu1, neu2, syn1, layer1_size, class_words[vocab[word].class_index][0], class_words[vocab[word].class_index][0]+class_cn[vocab[word].class_index], 0, layer1_size, 1);
		
		t=class_words[vocab[word].class_index][0]*layer1_size;
		for (c=0; c<class_cn[vocab[word].class_index]; c++) {
			b=class_words[vocab[word].class_index][c];
			if ((counter%10)==0)	//regularization is done every 10. step
				for (a=0; a<layer1_size; a++) syn1[a+t].weight+=alpha*neu2[b].er*neu1[a].ac - syn1[a+t].weight*beta2;
				else
					for (a=0; a<layer1_size; a++) syn1[a+t].weight+=alpha*neu2[b].er*neu1[a].ac;
					t+=layer1_size;
		}
		//
		matrixXvector(neu1, neu2, syn1, layer1_size, vocab_size, layer2_size, 0, layer1_size, 1);		//propagates errors 2->1 for classes
		
		c=vocab_size*layer1_size;
		for (b=vocab_size; b<layer2_size; b++) {
			if ((counter%10)==0) {	//regularization is done every 10. step
				for (a=0; a<layer1_size; a++) syn1[a+c].weight+=alpha*neu2[b].er*neu1[a].ac - syn1[a+c].weight*beta2;	//weight 1->2 update
			}
			else {
				for (a=0; a<layer1_size; a++) syn1[a+c].weight+=alpha*neu2[b].er*neu1[a].ac;	//weight 1->2 update
			}
			c+=layer1_size;
		}
    }
    
    //
    //到这里,上面部分把到隐层的部分更新完毕
    ///////////////
	
	//这里就是最常规的RNN,即t时刻往前只展开了s(t-1)
    if (bptt<=1) {		//bptt==1 -> normal BP
		
		//计算隐层的误差,即eh(t)
		for (a=0; a<layer1_size; a++) neu1[a].er=neu1[a].er*neu1[a].ac*(1-neu1[a].ac);    //error derivation at layer 1
		
		//这部分更新隐层到word部分的权值
		//注意由于word部分是1-of-V编码
		//所以这里更新的循环式上有点不同,并且下面的更新都是每10次训练更新才进行一次L2正规化
		a=last_word;
		if (a!=-1) {
			if ((counter%10)==0)			
				for (b=0; b<layer1_size; b++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac - syn0[a+b*layer0_size].weight*beta2;
				else
					//我们可以把这里的权值看做一个矩阵
					//b表示行,a表示列,非L2正规化的完整更新式如下：
					/*for (b=0; b<layer1_size; b++) {
					for (k=0; k<vocab_size; k++) {
					syn0[k+b*layer0_size].weight+=alpha*neu1[b].er*neu0[k].ac;
					}
		}*/
		//但由于neu0[k]==1只有当k==a时,所以为了加快循环计算的速度,更新就变为如下了
		//下面的代码是类似的道理,其实这里的neu0[a].ac可以直接省略掉,因为本身值为1				
		for (b=0; b<layer1_size; b++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac;
		}
		
		//这部分更新隐层到s(t-1)的权值
		if ((counter%10)==0) {
			for (b=0; b<layer1_size; b++) for (a=layer0_size-layer1_size; a<layer0_size; a++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac - syn0[a+b*layer0_size].weight*beta2;
		}
		else {
			for (b=0; b<layer1_size; b++) for (a=layer0_size-layer1_size; a<layer0_size; a++) syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac;
		}
    }
    else		//BPTT
    {
		//BPTT部分权值就不在用syn0了，而是用bptt_syn0
		//神经元部分的存储用bptt_hidden
		
		//将隐层的神经元信息复制到bptt_hidden，这里对neu1做备份
		for (b=0; b<layer1_size; b++) bptt_hidden[b].ac=neu1[b].ac;
		for (b=0; b<layer1_size; b++) bptt_hidden[b].er=neu1[b].er;
		
		//bptt_block第一个条件是控制BPTT的,因为不能每训练一个word都用BPTT来深度更新参数
		//这样每进行一次训练计算量太大,过于耗时
		//word==0表示当前一个句子结束
		//或者当independent开关不为0时,下一个词是句子的结束,则进行BPTT
		if (((counter%bptt_block)==0) || (independent && (word==0))) {
			
			//step表示
			for (step=0; step<bptt+bptt_block-2; step++) {
				
				//计算隐层的误差向量,即eh(t)
				for (a=0; a<layer1_size; a++) neu1[a].er=neu1[a].er*neu1[a].ac*(1-neu1[a].ac);    //error derivation at layer 1
				
				//更新的是输入层中vocab_size那部分权值
				//bptt_history下标从0开始依次存放的是wt, wt-1, wt-2...
				//比如当step == 0时,相当于正在进行普通的BPTT,这时bptt_history[step]表示当前输入word的在vocab中的索引
				a=bptt_history[step];	
				if (a!=-1)
					for (b=0; b<layer1_size; b++) {
						//由于输入层word部分是1-of-V编码,所以neu0[a].ac==1 所以这里后面没有乘以它
						//在step == 0时,bptt_syn0相当于原来的syn0
						bptt_syn0[a+b*layer0_size].weight+=alpha*neu1[b].er;
					}
					
					//为从隐层往状态层传误差做准备
					for (a=layer0_size-layer1_size; a<layer0_size; a++) neu0[a].er=0;
					
					//从隐层传递误差到状态层
					matrixXvector(neu0, neu1, syn0, layer0_size, 0, layer1_size, layer0_size-layer1_size, layer0_size, 1);		//propagates errors 1->0
					
					//更新隐层到状态层的权值
					//之所以先计算误差的传递就是因为更新完权值会发生改变
					for (b=0; b<layer1_size; b++) for (a=layer0_size-layer1_size; a<layer0_size; a++) {
						//neu0[a].er += neu1[b].er * syn0[a+b*layer0_size].weight;
						bptt_syn0[a+b*layer0_size].weight+=alpha*neu1[b].er*neu0[a].ac;
					}
					
					//todo这里我理解是把论文中的s(t-1)复制到s(t)，准备下一次循环
					for (a=0; a<layer1_size; a++) {		//propagate error from time T-n to T-n-1
						//后面为什么会加bptt_hidden呢
						neu1[a].er=neu0[a+layer0_size-layer1_size].er + bptt_hidden[(step+1)*layer1_size+a].er;
					}
					
					//历史中的s(t-2)复制到s(t-1)
					if (step<bptt+bptt_block-3)
						for (a=0; a<layer1_size; a++) {
							neu1[a].ac=bptt_hidden[(step+1)*layer1_size+a].ac;
							neu0[a+layer0_size-layer1_size].ac=bptt_hidden[(step+2)*layer1_size+a].ac;
						}
					
			}
			
			//清空历史信息中的er值
			for (a=0; a<(bptt+bptt_block)*layer1_size; a++) {
				bptt_hidden[a].er=0;
			}
			
			//这里恢复neu1,因为neu1反复在循环中使用
			for (b=0; b<layer1_size; b++) neu1[b].ac=bptt_hidden[b].ac;		//restore hidden layer after bptt
			
			
			//将BPTT后的权值改变作用到syn0上
			for (b=0; b<layer1_size; b++) {		//copy temporary syn0
				
				//bptt完毕,将bptt_syn0的权值复制到syn0中来,这是复制状态层部分
				if ((counter%10)==0) {
					for (a=layer0_size-layer1_size; a<layer0_size; a++) {
						syn0[a+b*layer0_size].weight+=bptt_syn0[a+b*layer0_size].weight - syn0[a+b*layer0_size].weight*beta2;
						bptt_syn0[a+b*layer0_size].weight=0;
					}
				}
				else {
					for (a=layer0_size-layer1_size; a<layer0_size; a++) {
						syn0[a+b*layer0_size].weight+=bptt_syn0[a+b*layer0_size].weight;
						bptt_syn0[a+b*layer0_size].weight=0;
					}
				}
				
				//bptt完毕,将bptt_syn0的权值复制到syn0中来,这是word部分
				if ((counter%10)==0) {
					for (step=0; step<bptt+bptt_block-2; step++) if (bptt_history[step]!=-1) {
						syn0[bptt_history[step]+b*layer0_size].weight+=bptt_syn0[bptt_history[step]+b*layer0_size].weight - syn0[bptt_history[step]+b*layer0_size].weight*beta2;
						bptt_syn0[bptt_history[step]+b*layer0_size].weight=0;
					}
				}
				else {
					//因为输入的word层是1-of-V编码,并不是全部的权值改变
					//这样写可以加快计算速度,避免不必要的循环检索
					for (step=0; step<bptt+bptt_block-2; step++) if (bptt_history[step]!=-1) {
						syn0[bptt_history[step]+b*layer0_size].weight+=bptt_syn0[bptt_history[step]+b*layer0_size].weight;
						bptt_syn0[bptt_history[step]+b*layer0_size].weight=0;
					}
				}
			}
		}
    }	
}
```







# rnnlm源码分析(八)

2015年04月01日 15:56:09 [a635661820](https://me.csdn.net/a635661820) 阅读数：3122



由于testNbest(), testGen()我没去看，剩下两个主干函数，一个是训练函数，另一个是测试函数，这两个函数都调用前面介绍过的函数。训练时，每当文件训练一遍完毕时，会马上将训练后的模型在valid文件上面试一下，看一下效果如何，如果这一遍训练的效果还不错的话，那么继续同样的学习率来训练文件，如果效果没太多打提升，就将学习率降低为一半，继续学习，直到没太大的提升就不再训练了。至于这个效果怎么看，是指训练打模型在valid上面的困惑度。测试函数是直接将训练好的模型在测试文件上计算所有的对数概率和，并换算成PPL，里面存着一个动态模型的概念，意思就是边测试的同时，还去更新网络的参数，这样测试文件也可以对模型参数进行更新。里面很重要的一个计算量是PPL, 下面的公式是PPL的公式，以便贴上来和程序代码部分对照：

![img](https://img-blog.csdn.net/20150401160703055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvYTYzNTY2MTgyMA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)



这是对一个序列w1w2w3...wk计算困惑度的公式，后面的c值在程序取的是10，后面会在代码中看到。下面就直接贴代码以及注释：



```cpp
//训练网络
void CRnnLM::trainNet()
{
    int a, b, word, last_word, wordcn;
    char log_name[200];
    FILE *fi, *flog;
	
	//在time.h中 typedef long clock_t
    clock_t start, now;
	
	//log_name中字串是rnnlm_file.output.txt
    sprintf(log_name, "%s.output.txt", rnnlm_file);
	
    printf("Starting training using file %s\n", train_file);
    starting_alpha=alpha;
    
	//打开rnnlm_file文件
    fi=fopen(rnnlm_file, "rb");
	
    if (fi!=NULL) {		//打开成功,即存在训练好的文件模型
		fclose(fi);
		printf("Restoring network from file to continue training...\n");
		//将rnnlm_file中模型信息恢复
		restoreNet();
	} 
	else {		//rnnlm_file打开失败
		
		//从train_file中读数据,相关数据会装入vocab,vocab_hash
		learnVocabFromTrainFile();
		//分配内存,初始化网络
		initNet();
		//iter表示整个训练文件的训练次数
		iter=0;		
	}
	
    if (class_size>vocab_size) {
		printf("WARNING: number of classes exceeds vocabulary size!\n");
    }
    
	//counter含义:当前训练的word是train_file的第counter个词
    counter=train_cur_pos;
    //saveNet();
	
	//最外层循环,循环一遍表示整个训练文件完成一次训练,用iter指示
    while (iter < maxIter) {
        printf("Iter: %3d\tAlpha: %f\t   ", iter, alpha);
		
		//fflush(stdout)刷新标准输出缓冲区，把输出缓冲区里的东西打印到标准输出设备上
		//即将上面要输出的内容立马输出
        fflush(stdout);
        
		//初始化bptt_history, history
        if (bptt>0) for (a=0; a<bptt+bptt_block; a++) bptt_history[a]=0;
        for (a=0; a<MAX_NGRAM_ORDER; a++) history[a]=0;
		
        //TRAINING PHASE
		
		//清除神经元的ac,er值
        netFlush();
		
		//打开训练文件
        fi=fopen(train_file, "rb");
		//在vocab中下标为0表示一个句子的结束即</s>
        last_word=0;
        
		//todo
        if (counter>0) for (a=0; a<counter; a++) word=readWordIndex(fi);	//this will skip words that were already learned if the training was interrupted
        
		//记录每次语料库开始训练的时间
        start=clock();
        
        while (1) {
			counter++;
			
			//下面信息每训练1万个词语才输出
			if ((counter%10000)==0) if ((debug_mode>1)) {
				now=clock();
				////train_words表示训练文件中的词数
				if (train_words>0)
				
					//输出的第一个%c,后面跟的是13表示回车键的ASCII,注意不同于换行键的10
					//对熵我不太了解,所以不太明白train entropy具体含义
					//Progress表示当前所训练的单词在整个训练文件中的位置,即训练进度
					//Words/sec表示每一秒中训练了多少个word
					printf("%cIter: %3d\tAlpha: %f\t   TRAIN entropy: %.4f    Progress: %.2f%%   Words/sec: %.1f ", 13, iter, alpha, -logp/log10(2)/counter, counter/(real)train_words*100, counter/((double)(now-start)/1000000.0));
				else
					printf("%cIter: %3d\tAlpha: %f\t   TRAIN entropy: %.4f    Progress: %dK", 13, iter, alpha, -logp/log10(2)/counter, counter/1000);
				fflush(stdout);
			}
			
			//表示每训练anti_k个word,会将网络信息保存到rnnlm_file
			if ((anti_k>0) && ((counter%anti_k)==0)) {
				train_cur_pos=counter;
				//保存网络的所有信息到rnnlm_file
				saveNet();
			}
			
			//读取下一个词,该函数返回下一个word在vocab中的下标
			word=readWordIndex(fi);     //read next word
			
			//注意训练文件中第一个词时,即counter=1时,last_word表示一个句子的结束
            computeNet(last_word, word);      //compute probability distribution
			
            if (feof(fi)) break;        //end of file: test on validation data, iterate till convergence
			
			//logp表示累计对数概率,即logp = log10w1 + log10w2 + log10w3...
            if (word!=-1) logp+=log10(neu2[vocab[word].class_index+vocab_size].ac * neu2[word].ac);
			
			//第一个条件没看懂,第二个条件似乎isinf(x)C99新增加的数学函数,如果x无穷返回非0的宏值
			//判断数值是否出错吧
			if ((logp!=logp) || (isinf(logp))) {
				printf("\nNumerical error %d %f %f\n", word, neu2[word].ac, neu2[vocab[word].class_index+vocab_size].ac);
				exit(1);
			}
			
            
            if (bptt>0) {		//shift memory needed for bptt to next time step
			
				////这里进行移动,结果就是bptt_history从下标0开始存放的是wt,wt-1,wt-2...
				for (a=bptt+bptt_block-1; a>0; a--) bptt_history[a]=bptt_history[a-1];
				bptt_history[0]=last_word;
				
				
				//这里进行移动,结果就是bptt_hidden从下标0开始存放的是st,st-1,st-2...
				for (a=bptt+bptt_block-1; a>0; a--) for (b=0; b<layer1_size; b++) {
					bptt_hidden[a*layer1_size+b].ac=bptt_hidden[(a-1)*layer1_size+b].ac;
					bptt_hidden[a*layer1_size+b].er=bptt_hidden[(a-1)*layer1_size+b].er;
				}
            }
            //反向学习,调整参数
            learnNet(last_word, word);
            
			//将隐层神经元的ac值复制到输出层后layer1_size那部分,即s(t-1)
            copyHiddenLayerToInput();
			
			//准备对下一个词所在的输入层进行编码
            if (last_word!=-1) neu0[last_word].ac=0;  //delete previous activation
			
            last_word=word;
            
			//移动,结果就是history从下标0开始存放的是wt, wt-1,wt-2...
            for (a=MAX_NGRAM_ORDER-1; a>0; a--) history[a]=history[a-1];
            history[0]=last_word;
			
			//word==0表示当前句子结束,independent非0,即表示要求每个句子独立训练
			//这个控制表面是否将一个句子独立训练,如果independent==0,表面上一个句子对下一个句子的训练时算作历史信息的
			//这控制还得看句子与句子之间的相关性如何了
			if (independent && (word==0)) netReset();
        }
        //关闭文件(train_file)
		fclose(fi);
		
		now=clock();
		//输出整个文件训练完毕的相关信息,具体见上面
		printf("%cIter: %3d\tAlpha: %f\t   TRAIN entropy: %.4f    Words/sec: %.1f   ", 13, iter, alpha, -logp/log10(2)/counter, counter/((double)(now-start)/1000000.0));
		
		//训练文件只会进行一遍,然后保存
		if (one_iter==1) {	//no validation data are needed and network is always saved with modified weights
			printf("\n");
			logp=0;
			////保存网络的所有信息到rnnlm_file
			saveNet();
            break;
		}
		
        //VALIDATION PHASE
		
		//上面训练一遍,下面进行验证,使用early-stopping
		//注意这里和上面TRAIN PHASE不同的是,下面的内容只是做计算,计算概率分布
		//并且测试整个validation文件的概率,下面不会有learNet的部分,如果有是属于dynamic models
		
		//清除神经元的ac,er值
        netFlush();
		
		//打开验证数据文件
        fi=fopen(valid_file, "rb");
		if (fi==NULL) {
			printf("Valid file not found\n");
			exit(1);
		}
		
		//ab方式打开文件:b表示二进制方式
		//a表示若文件不存在，则会建立该文件，如果文件存在，写入的数据会被加到文件尾后
		//log_name中字串是rnnlm_file.output.txt
		flog=fopen(log_name, "ab");
		if (flog==NULL) {
			printf("Cannot open log file\n");
			exit(1);
		}
        
        //fprintf(flog, "Index   P(NET)          Word\n");
        //fprintf(flog, "----------------------------------\n");
        
        last_word=0;
        logp=0;
		//wordcn的含义跟counter一样,只不过wordcn不包括OOV的词
        wordcn=0;
        while (1) {
			
			//读取下一个词,该函数返回下一个word在vocab中的下标
            word=readWordIndex(fi);     
			//计算下一个词的概率分布
            computeNet(last_word, word);      
            if (feof(fi)) break;        //end of file: report LOGP, PPL
            
			if (word!=-1) {
				//logp表示累计对数概率,即logp = log10w1 + log10w2 + log10w3...
				logp+=log10(neu2[vocab[word].class_index+vocab_size].ac * neu2[word].ac);
				wordcn++;
			}
			
            /*if (word!=-1)
			fprintf(flog, "%d\t%f\t%s\n", word, neu2[word].ac, vocab[word].word);
            else
			fprintf(flog, "-1\t0\t\tOOV\n");*/
			
            //learnNet(last_word, word);    //*** this will be in implemented for dynamic models
			
			////将隐层神经元的ac值复制到输出层后layer1_size那部分,即s(t-1)
            copyHiddenLayerToInput();
			
			////准备对下一个词所在的输入层进行编码
            if (last_word!=-1) neu0[last_word].ac=0;  //delete previous activation
			
            last_word=word;
            
			//移动,结果就是history从下标0开始存放的是wt, wt-1,wt-2...
            for (a=MAX_NGRAM_ORDER-1; a>0; a--) history[a]=history[a-1];
            history[0]=last_word;
			
			//word==0表示当前句子结束,independent非0,即表示要求每个句子独立训练
			//这个控制表面是否将一个句子独立训练,如果independent==0,表面上一个句子对下一个句子的训练时算作历史信息的
			if (independent && (word==0)) netReset();
        }
        fclose(fi);
        
		//表示第iter次训练train_file
        fprintf(flog, "\niter: %d\n", iter);
        fprintf(flog, "valid log probability: %f\n", logp);
		
		//这里实在没弄明白exp10()这个函数哪里来的,函数什么意思我也不能确定,希望明白的朋友告知一下~
		//但是按照PPL定义来推导,不难发现exp10啥意思,见PPL公式,公式里面我们取常数c = 10即可
		//所以exp10(x)就是10^(x)的意思吧
        fprintf(flog, "PPL net: %f\n", exp10(-logp/(real)wordcn));
        
        fclose(flog);
		//entropy不太熟悉,这里没去了解了
        printf("VALID entropy: %.4f\n", -logp/log10(2)/wordcn);
        
        counter=0;
		train_cur_pos=0;
		
		//llogp前面的l表示上一次last
		//这里的判断表示如果本次训练的结果没有上一次好,那么恢复到上一次
		//否则保存当前网络
        if (logp<llogp)
            restoreWeights();
        else
            saveWeights();
		
		//logp是越大说明训练得越好
		//初始时min_improvement=1.003,alpha_divide=0
		//这里表示如果本次训练的效果没有那么显著(提高min_improvement倍)则进入循环
		//训练的效果比较显著时,不进入循环,alpha保持不变
		//这里可以参考原论文第30页有更详细的说明
        if (logp*min_improvement<llogp) {
			//如果没显著的提高,打开alpha_divide控制
            if (alpha_divide==0) alpha_divide=1;
            else {
				//如果没显著的提高,且alpha_divide开关是打开的,那么退出训练,这时说明训练得不错了
                saveNet();
                break;
            }
        }
		
		//如果没有显著的提高,则将学习率降低一半
        if (alpha_divide) alpha/=2;
		
        llogp=logp;
        logp=0;
        iter++;
        saveNet();
    }
}
 
//测试网络
void CRnnLM::testNet()
{
    int a, b, word, last_word, wordcn;
    FILE *fi, *flog, *lmprob=NULL;
    real prob_other, log_other, log_combine;
    double d;
    
	//将rnnlm_file中模型信息恢复
    restoreNet();
    
	//use_lmprob这个控制开关等于1时,表示使用其他训练好的语言模型
    if (use_lmprob) {
		//打开其他语言模型文件
		lmprob=fopen(lmprob_file, "rb");
    }
	
    //TEST PHASE
    //netFlush();
	
	//打开测试文件
    fi=fopen(test_file, "rb");
    //sprintf(str, "%s.%s.output.txt", rnnlm_file, test_file);
    //flog=fopen(str, "wb");
	
	//stdout是一个文件指针,C己经在头文件中定义好的了，可以直接使用，把它赋给另一个文件指针,这样直接为标准输出
	//printf其实就是fprintf的第一个参数设置为stdout 
    flog=stdout;
	
    if (debug_mode>1)	{
		if (use_lmprob) {
			fprintf(flog, "Index   P(NET)          P(LM)           Word\n");
			fprintf(flog, "--------------------------------------------------\n");
		} else {
			fprintf(flog, "Index   P(NET)          Word\n");
			fprintf(flog, "----------------------------------\n");
		}
    }
	
	//在vocab中下标为0表示一个句子的结束即</s>,即last_word初始时,即 等于 end of sentence
    last_word=0;	
	//rnn对测试文件的对数累加概率
    logp=0;
	//其他语言模型对测试文件的对数累加概率
    log_other=0;
	//rnn与其他语言模型的结合对数累加概率
    log_combine=0;
	//其他语言模型某个词的概率
    prob_other=0;
	//wordcn的含义跟trainNet里面的counter一样,只不过wordcn不包括OOV的词
    wordcn=0;
	//将隐层神经元的ac值复制到输出层后layer1_size那部分,即s(t-1)
    copyHiddenLayerToInput();
    
	//清空历史信息
    if (bptt>0) for (a=0; a<bptt+bptt_block; a++) bptt_history[a]=0;
    for (a=0; a<MAX_NGRAM_ORDER; a++) history[a]=0;
    if (independent) netReset();
    
    while (1) {
        
		//读取下一个词,该函数返回下一个word在vocab中的下标
        word=readWordIndex(fi);	
		//计算下一个词的概率分布		
        computeNet(last_word, word);		
        if (feof(fi)) break;		//end of file: report LOGP, PPL
        
        if (use_lmprob) {
            fscanf(lmprob, "%lf", &d);
			prob_other=d;
			
            goToDelimiter('\n', lmprob);
        }
		
		//log_combine通过系数lambda插值
        if ((word!=-1) || (prob_other>0)) {
			if (word==-1) {
				//这里不太懂为啥要惩罚
				logp+=-8;		//some ad hoc penalty - when mixing different vocabularies, single model score is not real PPL
				//插值
				log_combine+=log10(0 * lambda + prob_other*(1-lambda));
			} else {
				//计算rnn累加对数概率
				logp+=log10(neu2[vocab[word].class_index+vocab_size].ac * neu2[word].ac);
				//插值
				log_combine+=log10(neu2[vocab[word].class_index+vocab_size].ac * neu2[word].ac*lambda + prob_other*(1-lambda));
			}
			log_other+=log10(prob_other);
            wordcn++;
        }
		
		if (debug_mode>1) {
			if (use_lmprob) {
				if (word!=-1) fprintf(flog, "%d\t%.10f\t%.10f\t%s", word, neu2[vocab[word].class_index+vocab_size].ac *neu2[word].ac, prob_other, vocab[word].word);
				else fprintf(flog, "-1\t0\t\t0\t\tOOV");
			} else {
				if (word!=-1) fprintf(flog, "%d\t%.10f\t%s", word, neu2[vocab[word].class_index+vocab_size].ac *neu2[word].ac, vocab[word].word);
				else fprintf(flog, "-1\t0\t\tOOV");
			}
			
			fprintf(flog, "\n");
		}
		
		//这部分是 dynamic model 在测试时还能让rnn进行学习更新参数
        if (dynamic>0) {
            if (bptt>0) {
				//将bptt_history往后移动一个位置,将最近的word装入bptt_history第一个位置
                for (a=bptt+bptt_block-1; a>0; a--) bptt_history[a]=bptt_history[a-1];
                bptt_history[0]=last_word;
				
				//将bptt_hidden往后移动一个位置,将第一个位置留出来,第一个位置的赋值是在learnNet里面
                for (a=bptt+bptt_block-1; a>0; a--) for (b=0; b<layer1_size; b++) {
                    bptt_hidden[a*layer1_size+b].ac=bptt_hidden[(a-1)*layer1_size+b].ac;
                    bptt_hidden[a*layer1_size+b].er=bptt_hidden[(a-1)*layer1_size+b].er;
				}
            }
            //动态模型时的学习率
            alpha=dynamic;
			learnNet(last_word, word);    //dynamic update
		}
		//将隐层神经元的ac值复制到输出层后layer1_size那部分,即s(t-1)
        copyHiddenLayerToInput();
        
		//准备对下一个词所在的输入层进行编码
        if (last_word!=-1) neu0[last_word].ac=0;  //delete previous activation
		
        last_word=word;
        
		//将ME部分的history往后移动一个位置,第一个位置放最近的词
        for (a=MAX_NGRAM_ORDER-1; a>0; a--) history[a]=history[a-1];
        history[0]=last_word;
		
		//这个和前面同理
		if (independent && (word==0)) netReset();
    }
    fclose(fi);
    if (use_lmprob) fclose(lmprob);
	
	//这里输出对测试文件的信息
    //write to log file
    if (debug_mode>0) {
		fprintf(flog, "\ntest log probability: %f\n", logp);
		if (use_lmprob) {
			fprintf(flog, "test log probability given by other lm: %f\n", log_other);
			fprintf(flog, "test log probability %f*rnn + %f*other_lm: %f\n", lambda, 1-lambda, log_combine);
		}
		
		fprintf(flog, "\nPPL net: %f\n", exp10(-logp/(real)wordcn));
		if (use_lmprob) {
			fprintf(flog, "PPL other: %f\n", exp10(-log_other/(real)wordcn));
			fprintf(flog, "PPL combine: %f\n", exp10(-log_combine/(real)wordcn));
		}
    }
    
    fclose(flog);
}
```

好了，rnnlm toolkit源码走读就暂告一段落了，内容肯定会很有多自己理解不正确的地方，还是一样欢迎明白的朋友指出，一起讨论，因为图解过于分散在每一篇了，最后我会在把rnnlm toolkit的内部数据结构图作为单独一篇文章全部贴出来。







