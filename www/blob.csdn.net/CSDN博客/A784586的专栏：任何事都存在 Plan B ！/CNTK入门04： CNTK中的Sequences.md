# CNTK入门04： CNTK中的Sequences - A784586的专栏：任何事都存在 Plan B ！ - CSDN博客





2017年05月17日 13:28:07[QuJack](https://me.csdn.net/A784586)阅读数：1034








**CNTK入门04： CNTK中的Sequences**


**-----Working with Sequences**




参考：https://www.cntk.ai/pythondocs/sequence.html

声明：请直接参看官方文档上述链接，本文只做个人阅读笔记。错误不当之处请指教。



CNTK的输入，输出和参数被组织成张量（tensors）.每个张量具有等级：标量是等级0的张量，向量是等级1的张量，矩阵是等级2的张量，依此类推。我们参照这不同的维度作为轴。

每个CNTK tensors（张量）有一些静态轴和一些动态轴；

在整个网络生命周期中，这些静态轴有这相同的长度；下列情况除外时候，动态轴在他们定义有意义的张量组时候像静态轴。

a.动态轴的长度会随着实例变化而变化；

b.动态轴的长度在minibatch表示之前不清楚；

c.动态轴或许是有序的；

一个 minibatch 也是一个张量（tesnsor）.因此他有一个动态轴，叫做batch axis.他的长度因minibatch的不同而不同;在写这篇文档时候，CNTK支持一个简单的附件的动态轴。他有时候被称为序列轴，但是它没有一个专门的名字。

这个轴使用序列使得以一种高级的方式工作。当在操作序列时候，CNTK总是执行一个简单的类型检查确保合并两个序列时安全的。

为了使得意思更具体清楚，让我们考虑两个例子：**首先，**让我们看看一个minibatch的短视频剪辑在CNTK中是如何表示的，假设视频全是640*480并且是在三基色下拍摄的（应该是彩色的视频的意思吧）。这意味着我们的矩阵有三个静态轴：640,480,3。它有两个动态轴：视频的长度和minibatch轴。因此一个大小为16的minibatch，每个视频长度为240帧，将会被表示为一个16
  * 240 * 3 * 640 * 480的张量tensor。

**另外个例子，**动态轴提供一个优雅的解决方案的另一个例子是在学习排名文档查询。典型的，训练数据这个过程包含一系列的查询。每一个查询有一个相关文档的变量数字。查询文档对 the
 query-document pairs 之一包含一个相关的判断者或者标签 judgment or label（例如这个文档是否是查询相关的）。现在可以依据我们在每个文档里怎么对待文字，我们也能如何把他们放在一个静态轴或者动态轴上。为了把他们放在一个静态轴上，我们处理每个文档作为一个向量，向量的大小等于词汇包含每个单词出现的次数的大小。然而我们也能利用一个动态轴处理文档成一个单词序列。

在这个案例中，我们有如下的嵌套：




- Query: CNTK
- Document 1:
- Microsoft
- Cognitive
- Toolkit

- Document 2:
- Cartoon
- Network

- Document 3:
- NVIDIA
- Microsoft
- Accelerate
- AI


- Query: flower
- Document 1:
- Flower
- Wikipedia

- Document 2:
- Local
- Florist
- Flower
- Delivery



最外层是batch轴，文档层应该有一个动态轴，因为我们有一个每个查询的候选文档变量数字。最内层应该有一个动态轴，因为每个文档有一个单词的变量数字。一个描述描叙minibatch的张量也有一个或者多个静态轴，描述特征，例如在查询和文档中的特性。有足够的训练数据，在多元相关查询上，可能有另一个水平的嵌套，命名为session。



## Sequence classification

序列分类器

在深度学习中最令人兴奋的领域之一是recurrent neural networks (RNNs)递归神经网络强大的想法。RNNs在某种程度上是深度学习世界中的隐马尔可夫模型。他们是用一个修正的参数集合来处理变量长度序列的网络。因此他不得不在输入序列上学习，总结所有的观察，以转化到一个有限维度的状态，用这个状态预测下一个观察，转化他们当前的状态，观察输入到下一个状态的转化。换句话说，他们允许信息持久化。因此，当一个传统的神经网络层被看做如下左图的数据流，一个RNN层看做右边的图：

![](https://img-blog.csdn.net/20170519152859465?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvQTc4NDU4Ng==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





      正如上右图所示，RNNs是一个处理序列自然的结构。这包含;文本，音乐，视频；当前状态依赖先前状态的一切；RNNs的确是强大的，a “vanilla” RNNs ，它的状态是一个关于先前状态和当前观察的非线性函数。是一个通过基于梯度方法很难学习的。因为梯度需要一个反馈流在网络中来学习。开始元素的贡献（例如一个句子看是的一个单词）在后来的元素之后，例如在一个长句子中最后一个单词的分类，会本质上消失。

 解决上述问题是研究领域中很活跃的一个领域。一个架构看起来在实践上是成功的，LSTM【Long Short
 Term Memory (LSTM) network】。LSTMs是一中RNN，它是及其有用的，在实践中执行一个RNN时候通常会使用这个。一个关于它的很好的解释链接：[http://colah.github.io/posts/2015-08-Understanding-LSTMs](http://colah.github.io/posts/2015-08-Understanding-LSTMs).一个LSTM是一个可微的函数，一个输入，一个状态，处理一个输出和一个新的状态。

 
        在我们的例子中，我们将会用一个LSTM作为一个序列分类器【sequence classification】。但是为了更好的结果，我们也会介绍一个传统的概念：[word
 embeddings](https://en.wikipedia.org/wiki/Word_embedding)。在传统的自然语言处理方法中NLP，单词通过标准的高纬度空间的偏向被识别。第一个单词（1,0,0,0,0,0,0,0,0），第二个单词（0,1,0,0,0,0,0,0,0）.以此类推。这种叫做one-hot编码。每个单词都和其他单词正交，但是那个不好抽象化。在真实的语言中，一些单词是很相似的（我们称为同义词），或者他们的功能是相似的(例如:巴黎、西雅图、东京)。观察的关键是单词出现在相似的文本里应该是相似的。我们让神经网络通过使得每一个单词表示成一个学习后的简短的向量，使得解决这些细节。为了使网络在它的任务上做的好，它不得不学习单词到这些向量的映射map。例如：代表单词“cat”的向量在某种意义上接近代表“dog”的向量。在我们的任务,我们将学习这些词从头嵌入。然而，它也可能用一个预计算嵌入词，例如在上亿单词库上训练后的GloVe,初始化。

现在我们来决定我们的单词表示和递归神经网络的类型，让我们来定义用于序列分类器的网络。我们能够想到添加一系列的层：

1.
 嵌入层；（在某个序列中个性的单词变为向量）；

2.
 LSTM层；（允许每个单词依赖先前的一个单词）；

3.
 softmax层；（一个额外的参数和每个类别输出可能性的集合）

一个相似的例子：Examples/SequenceClassification/SimpleExample/Python/SequenceClassification.py

让我们看看在CNTK中他是如何通过序列工作的。



```python
import sys
import os
from cntk import Trainer, Axis
from cntk.io import MinibatchSource, CTFDeserializer, StreamDef, StreamDefs,\
        INFINITELY_REPEAT
from cntk.learners import sgd, learning_rate_schedule, UnitType
from cntk import input, cross_entropy_with_softmax, \
        classification_error, sequence
from cntk.logging import ProgressPrinter
from cntk.layers import Sequential, Embedding, Recurrence, LSTM, Dense

# Creates the reader
def create_reader(path, is_training, input_dim, label_dim):
    return MinibatchSource(CTFDeserializer(path, StreamDefs(
        features=StreamDef(field='x', shape=input_dim, is_sparse=True),
        labels=StreamDef(field='y', shape=label_dim, is_sparse=False)
        )), randomize=is_training,
        max_sweeps=INFINITELY_REPEAT if is_training else 1)


# Defines the LSTM model for classifying sequences
def LSTM_sequence_classifier_net(input, num_output_classes, embedding_dim,
                                LSTM_dim, cell_dim):
    lstm_classifier = Sequential([Embedding(embedding_dim),
                                  Recurrence(LSTM(LSTM_dim, cell_dim))[0],
                                  sequence.last,
                                  Dense(num_output_classes)])
    return lstm_classifier(input)


# Creates and trains a LSTM sequence classification model
def train_sequence_classifier():
    input_dim = 2000
    cell_dim = 25
    hidden_dim = 25
    embedding_dim = 50
    num_output_classes = 5

    # Input variables denoting the features and label data
    features = sequence.input(shape=input_dim, is_sparse=True)
    label = input(num_output_classes)

    # Instantiate the sequence classification model
    classifier_output = LSTM_sequence_classifier_net(
        features, num_output_classes, embedding_dim, hidden_dim, cell_dim)

    ce = cross_entropy_with_softmax(classifier_output, label)
    pe = classification_error(classifier_output, label)

    rel_path = ("../../../Tests/EndToEndTests/Text/" +
                "SequenceClassification/Data/Train.ctf")
    path = os.path.join(os.path.dirname(os.path.abspath(__file__)), rel_path)

    reader = create_reader(path, True, input_dim, num_output_classes)

    input_map = {
            features: reader.streams.features,
            label:    reader.streams.labels
    }

    lr_per_sample = learning_rate_schedule(0.0005, UnitType.sample)
    # Instantiate the trainer object to drive the model training
    progress_printer = ProgressPrinter(0)
    trainer = Trainer(classifier_output, (ce, pe),
                      sgd(classifier_output.parameters, lr=lr_per_sample),
                      progress_printer)
# Get minibatches of sequences to train with and perform model training
    minibatch_size = 200

    for i in range(255):
        mb = reader.next_minibatch(minibatch_size, input_map=input_map)
        trainer.train_minibatch(mb)

    evaluation_average = float(trainer.previous_minibatch_evaluation_average)
    loss_average = float(trainer.previous_minibatch_loss_average)
    return evaluation_average, loss_average

if __name__ == '__main__':
    error, _ = train_sequence_classifier()
    print(" error: %f" % error)
```

上述rel_path = ("../../../Tests/EndToEndTests/Text/" +"SequenceClassification/Data/Train.ctf")中，如果输入数据自己下载的例子中没有，请到官网源码中找到，加入对应的路径中即可。


让我们一窥上述网络的定义。通常我们首先设置模型的参数。在这个例子中，我们哟一个额输入维度为2000的词汇，LSTM隐藏层，每个单元的维度为25，一个维度为50的嵌入层，我们的序列有五个可能的类别。我们然后实例化我们的模型。

LSTM_sequence_classifier_net是一个简单的函数，在嵌入矩阵中查找并返回相关的表示。通过LSTM RNN层作为输入，从LSTM通过选择最后一个隐藏的LSTM状态返回一个修正后的输出。


```python
lstm_classifier = Sequential([Embedding(embedding_dim),
                              Recurrence(LSTM(LSTM_dim, cell_dim))[0],
                              sequence.last,
                              Dense(num_output_classes)])
```
这是一个完整的网络定义。第二行第一个，我们选择LSTM的第一个输出。

在这个LSTM的实现中，这是实际输出，而第二个输出是LSTM的状态。

我们现在简单地设置我们的标准节点（例如我们使用思想向量对标签进行分类）和我们的训练循环。

在上面的例子中，我们使用200大小的minibatch，并使用默认参数的基本SGD和0.0005的学习率。

这可以得到一个强大的最先进的序列分类模型，可以扩展大量的训练数据。

请注意，随着培训数据大小的增长，您应该通过增加隐藏维度的数量为LSTM提供更多的容量。

此外，可以通过层叠LSTM来获得更复杂的网络。从概念上讲，堆叠LSTM层类似于前馈网络中的堆叠层。

选择一个好的架构是非常特别的任务。
**用NumPy提供序列**

虽然CNTK具有非常高效的内置读取器，可以为您处理许多细节（随机化，预取，减少内存使用等），有时您的数据已经在numpy数组中。因此，重要的是要知道如何指定一个输入序列以及如何指定一个序列minibatch。

到目前为止，我们已经讨论过文本，所以让我们来换个齿轮并用图片做一个例子。通过NumPy数组提供文本数据并没有很大区别。

每个序列必须是自己的NumPy数组。因此，如果您有一个输入变量表示像这样的一个小的彩色图像：

```python
x = sequence.input((3,32,32))
```
并且您要将一系列4张图像img1转换为一个img4，然后您需要创建一个包含所有4个图像的张量。例如：

```python
img_seq = np.stack([img1, img2, img3, img4])
output = network.eval({x:[img_seq]})
```
NumPy中的堆栈功能将沿新轴的输入堆栈（默认为起始位置），因此img_seq的形状为4 × 3 × 32 × 324×3×32×32。您可能已经注意到，在将img_seq绑定到x之前， 我们将其包装到列表中。该列表一个大小为1的minibatch和minibatches被指定为列表。这样做的原因是因为不同的迷你batch元素可以具有不同的长度。如果所有minibatch元素都是相同长度的序列，那么可以将该小批量作为尺寸的一个大张量b
 × s × d1× ... × dk。其中b是批量大小，s是序列长度，di 是输入变量的第i个静态轴的维数。
















