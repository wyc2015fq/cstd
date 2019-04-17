# 利用AllenNLP，百行的Python代码训练情感分类器 - 知乎
# 



选自realworldnlpbook，作者：Masato Hagiwara，机器之心编译，参与：Geek AI，路。

> 本文介绍了如何利用AllenNLP，使用不到一百行代码训练情感分类器。

**什么是情感分析？**

情感分析是一种流行的文本分析技术，用来对文本中的主观信息进行自动识别和分类。它被广泛用于量化观点，情感等通常以非结构化方式记录的信息，而这些信息也因此很难用其他方式量化。情感分析技术可被用于多种文本资源，例如调查报告，评论，社交媒体上的帖子等。

情感分析最基本的任务之一是极性分类，换句话说，该任务需要判断语言所表达的观点是正面的，负面的还是中性的具体而言，可能有三个以上的类别，例如：极其正面，正面，中性，消极，极其消极。这有些类似于你使用某些网站时的评价行为（比如亚马逊），人们可以用星星数表示5个等级来对物品进行评论（产品，电影或其他任何东西）。




**斯坦福的情感分析树库（树库）**

目前，研究人员发布了一些公开的情感分类数据集。在本文中，我们将使用斯坦福的情感分析树库（或称SST），这可能是最广为使用的情感分析数据集之一.SST与其它数据集最大的不同之处是，在SST中情感标签不仅被分配到句子上，句子中的每个短语和单词也会带有情感标签。这使我们能够研究单词和短语之间复杂的语义交互例如，对下面这个句子的极性进行分析：

> 这部电影实际上既不那么有趣，也不是超级诙谐。

这个句子肯定是消极的。但如果只看单个单词（「滑稽」，「机智」）可能会被误导，认为它的情感是积极的。只关注单个单词的朴素词袋分类器很难对上面的翻译进行正确的分类。要想正确地对上述例句的极性进行分类，你需要理解否定词（既不...也不......）对语义的影响。由于SST具备这样的特性，它被用作获取句子句法结构的神经网络模型的标准对比基准（[https://nlp.stanford.edu/~socherr/EMNLP2013_RNTN.pdf](https://link.zhihu.com/?target=https%3A//nlp.stanford.edu/~socherr/EMNLP2013_RNTN.pdf)）。




**Pytorch和AllenNLP**

PyTorch是我最喜欢的深度学习框架。它提供了灵活，易于编写的模块，可动态运行，且速度相当快。在过去一年中，PyTorch在科研社区中的使用实现了[爆炸性增长](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749471%26idx%3D1%26sn%3D3b4dfc47cbb81af816b980c1a5f21abb%26chksm%3D871afe61b06d77774acaf05a4b5b7d70735855363263394b3873d40a7628cf3c7d7ba7bc0e2b%26scene%3D21%23wechat_redirect)。

尽管PyTorch是一个非常强大的框架，但是自然语言处理往往涉及底层的公式化的事务处理，包括但不限于：阅读和编写数据集，分词，建立单词索引，词汇管理，mini-batch批处理，排序和填充等。尽管在NLP任务中正确地使用这些构建块是至关重要的，但是当你快速迭代时，你需要一次又一次地编写类似的设计模式，这会浪费很多时间。而这正是AllenNLP这类库的亮点所在。

AllenNLP是艾伦人工智能研究院开发的开源NLP平台。它的设计初衷是为了NLP研究和开发（尤其是语义和语言理解任务）的快速迭代提供支持。它提供了灵活的API，对NLP很实用的抽象，以及模块化的实验框架，从而加速NLP的研究进展。

本文将向大家介绍如何使用AllenNLP一步一步构建自己的情感分类器。由于AllenNLP会在后台处理好底层事务，提供训练框架，所以整个脚本只有不到100行Python代码，你可以很容易地使用其它神经网络架构进行实验。

代码地址：HTTPS：//[http://github.com/mhagiwara/realworldnlp/blob/master/examples/sentiment/sst_classifier.py](https://link.zhihu.com/?target=http%3A//github.com/mhagiwara/realworldnlp/blob/master/examples/sentiment/sst_classifier.py)

接下来，下载SST数据集，你需要将数据集分割成PTB树格式的训练集，开发集和测试集，你可以通过下面的链接直接下载：https：//[http://nlp.stanford.edu/sentiment/trainDevTestTrees_PTB](https://link.zhihu.com/?target=http%3A//nlp.stanford.edu/sentiment/trainDevTestTrees_PTB) .zip。我们假设这些文件是在data / stanfordSentimentTreebank / trees下进行扩展的。

注意，在下文的代码片段中，我们假设你已经导入了合适的模块，类和方法（详情参见完整脚本）。你会注意到这个脚本和AllenNLP的词性标注教程非常相似 - 在AllenNLP中很容易在只进行少量修改的情况下使用不同的模型对不同的任务进行实验。




**数据集读取和预处理**

AllenNLP已经提供了一个名为StanfordSentimentTreeBankDatasetReader的便捷数据集读取器，它是一个读取SST数据集的接口。你可以通过将数据集文件的路径指定为为read（）方法的参数来读取数据集：


```
reader = StanfordSentimentTreeBankDatasetReader()

train_dataset = reader.read('data/stanfordSentimentTreebank/trees/train.txt')
dev_dataset = reader.read('data/stanfordSentimentTreebank/trees/dev.txt')
```


几乎任何基于深度学习的NLP模型的第一步都是指定如何将文本数据转换为张量。该工作包括把单词和标签（在本例中指的是“积极”和“消极”这样的极性标签）转换为整型ID。在AllenNLP中，该工作是由Vocabulary类来处理的，它存储从单词/标签到ID的映射。


```
# You can optionally specify the minimum count of tokens/labels.
# `min_count={'tokens':3}` here means that any tokens that appear less than three times
# will be ignored and not included in the vocabulary.
vocab = Vocabulary.from_instances(train_dataset + dev_dataset,
                                  min_count={'tokens': 3})
```


下一步是将单词转换为嵌入。在深度学习中，嵌入是离散，高维数据的连续向量表征。你可以使用嵌入创建这样的映射，使用BasicTextFieldEmbedder将ID转换为嵌入向量。


```
token_embedding = Embedding(num_embeddings=vocab.get_vocab_size('tokens'),
                            embedding_dim=EMBEDDING_DIM)
# BasicTextFieldEmbedder takes a dict - we need an embedding just for tokens,
# not for labels, which are used unchanged as the answer of the sentence classification
word_embeddings = BasicTextFieldEmbedder({"tokens": token_embedding})
```





**句子分类模型**
![](https://pic3.zhimg.com/v2-255fc886ba7b34a040584282de0b5dd2_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='956' height='160'></svg>)LSTM-RNN句子分类模型
现在，我们来定义一个句子分类模型这段代码看起来很多，但是别担心，我在代码片段中添加了大量注释：


```
# Model in AllenNLP represents a model that is trained.
class LstmClassifier(Model):
    def __init__(self,
                 word_embeddings: TextFieldEmbedder,
                 encoder: Seq2VecEncoder,
                 vocab: Vocabulary) -> None:
        super().__init__(vocab)
        # We need the embeddings to convert word IDs to their vector representations
        self.word_embeddings = word_embeddings

        # Seq2VecEncoder is a neural network abstraction that takes a sequence of something
        # (usually a sequence of embedded word vectors), processes it, and returns it as a single
        # vector. Oftentimes, this is an RNN-based architecture (e.g., LSTM or GRU), but
        # AllenNLP also supports CNNs and other simple architectures (for example,
        # just averaging over the input vectors).
        self.encoder = encoder

        # After converting a sequence of vectors to a single vector, we feed it into
        # a fully-connected linear layer to reduce the dimension to the total number of labels.
        self.hidden2tag = torch.nn.Linear(in_features=encoder.get_output_dim(),
                                          out_features=vocab.get_vocab_size('labels'))
        self.accuracy = CategoricalAccuracy()

        # We use the cross-entropy loss because this is a classification task.
        # Note that PyTorch's CrossEntropyLoss combines softmax and log likelihood loss,
        # which makes it unnecessary to add a separate softmax layer.
        self.loss_function = torch.nn.CrossEntropyLoss()

    # Instances are fed to forward after batching.
    # Fields are passed through arguments with the same name.
    def forward(self,
                tokens: Dict[str, torch.Tensor],
                label: torch.Tensor = None) -> torch.Tensor:
        # In deep NLP, when sequences of tensors in different lengths are batched together,
        # shorter sequences get padded with zeros to make them of equal length.
        # Masking is the process to ignore extra zeros added by padding
        mask = get_text_field_mask(tokens)

        # Forward pass
        embeddings = self.word_embeddings(tokens)
        encoder_out = self.encoder(embeddings, mask)
        logits = self.hidden2tag(encoder_out)

        # In AllenNLP, the output of forward() is a dictionary.
        # Your output dictionary must contain a "loss" key for your model to be trained.
        output = {"logits": logits}
        if label is not None:
            self.accuracy(logits, label)
            output["loss"] = self.loss_function(logits, label)

        return output
```


这里的关键是Seq2VecEncoder，它基本上使用张量序列作为输入，然后返回一个向量。我们在这里使用LSTM-RNN作为编码器（如有需要，可参阅文档[https://allenai.github.io/allennlp](https://link.zhihu.com/?target=https%3A//allenai.github.io/allennlp) -docs / API / allennlp.modules.seq2vec_encoders.html＃allennlp.modules.seq2vec_encoders.pytorch_seq2vec_wrapper.PytorchSeq2VecWrapper）。


```
lstm = PytorchSeq2VecWrapper(
    torch.nn.LSTM(EMBEDDING_DIM, HIDDEN_DIM, batch_first=True))

model = LstmClassifier(word_embeddings, lstm, vocab)
```





**训练**

一旦你定义了这个模型，其余的训练过程就很容易了。这就是像AllenNLP这样的高级框架的亮点所在。你只需要指定如何进行数据迭代并将必要的参数传递给训练器，而无需像PyTorch和TensorFlow那样编写冗长的批处理和训练循环。


```
optimizer = optim.Adam(model.parameters(), lr=1e-4, weight_decay=1e-5)

iterator = BucketIterator(batch_size=32, sorting_keys=[("tokens", "num_tokens")])
iterator.index_with(vocab)

trainer = Trainer(model=model,
                  optimizer=optimizer,
                  iterator=iterator,
                  train_dataset=train_dataset,
                  validation_dataset=dev_dataset,
                  patience=10,
                  num_epochs=20)

trainer.train()
```


这里的BucketIterator会根据令牌的数量对训练实例进行排序，从而使得长度类似的实例在同一个批中。注意，我们使用了验证集，在测试误差过大时采用了早停法避免过拟合。

如果将上面的代码运行20个epoch，则模型在训练集上的准确率约为0.75，在验证集上的准确率约为0.35。这听起来很低，但是请注意，这是一个5类的分类问题，随机基线的准确率只有0.20。




**测试**

为了测试刚刚训练的模型是否如预期，你需要构建一个预测器（预测器）.predictor是一个提供基于JSON的接口的类，它被用于将输入数据传递给你的模型或将输出数据从模型中导出。接着，我便写了一个句子分类预测器（[https://github.com/mhagiwara/realworldnlp/blob/master/realworldnlp/predictors.py#L10](https://link.zhihu.com/?target=https%3A//github.com/mhagiwara/realworldnlp/blob/master/realworldnlp/predictors.py%23L10)),将其用作句子分类模型的基于JSON的接口。


```
tokens = ['This', 'is', 'the', 'best', 'movie', 'ever', '!']
predictor = SentenceClassifierPredictor(model, dataset_reader=reader)
logits = predictor.predict(tokens)['logits']
label_id = np.argmax(logits)

print(model.vocab.get_token_from_index(label_id, 'labels'))
```


运行这段代码后，你应该看到分类结果为「4」，「4「对应的是」非常积极」。所以你刚刚训练的模型正确地预测出了这是一个非常正面的电影评论。
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*
原文链接：[http://www.realworldnlpbook.com/blog/training-sentiment-analyzer-using-allennlp.html](https://link.zhihu.com/?target=http%3A//www.realworldnlpbook.com/blog/training-sentiment-analyzer-using-allennlp.html)


