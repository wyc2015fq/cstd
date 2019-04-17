# 谷歌开放GNMT教程：如何使用TensorFlow构建自己的神经机器翻译系统 - 知乎
# 



> 近日，谷歌官方在 Github 开放了一份神经机器翻译教程，该教程从基本概念实现开始，首先搭建了一个简单的NMT模型，随后更进一步引进注意力机制和多层 LSTM 加强系统的性能，最后谷歌根据 GNMT 提供了更进一步改进的技巧和细节，这些技巧能令该NMT系统达到极其高的精度。机器之心对该教程进行简要的描述，跟详细和精确的内容请查看项目原网站。


GitHub 链接：[https://github.com/tensorflow/nmt](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/nmt)


机器翻译，即跨语言间的自动翻译，是机器学习社区最活跃的研究领域。在机器翻译的众多方法中，序列到序列（sequence-to-sequence，seq2seq）模型 [1, 2] 近期获得巨大成功。由于其可以使用深度神经网络获取句义，该模型成为谷歌翻译等多数商业翻译系统事实上的标准模型。但是，尽管有关 OpenNMT 或 tf-seq2seq 等 seq2seq 模型的资料已经非常丰富，但能够指导人们快速构建高质量翻译系统的知识和技能仍然略显不足。



今天，我们很高兴能够发布最新的 TensorFlow 神经机器翻译教程，帮助读者全面了解 seq2seq 模型，并介绍如何从头开始构建有竞争力的翻译模型。该教程尽可能简单地呈现该过程，教程首先介绍神经机器翻译的背景知识，并讲述构建 vanilla 系统的具体代码。然后介绍注意力机制 [3, 4] 这一帮助 NMT 系统处理长句的重要工具。最后，本教程将讲解如何复制谷歌 NMT（GNMT）系统 [5] 的关键特征，以实现在多个 GPU 上训练模型。



本教程包括具体的基准测试结果，用户可自行复制。我们的模型提供强大的开源基准，其性能不亚于 GNMT 的结果 [5]。我们在 WMT 2014 的英语转德语翻译任务中取得了 BLEU 得分 24.4 的成绩。



本教程还包括其他的基准测试结果（英语转越南语、德语转英语）。



另外，本教程将开放全动态的 seq2seq API（随 TensorFlow 1.2 发布），该 API 使 seq2seq 模型的构建过程干净、简单、易读：



a.使用 tf.contrib.data 中最新输入的管道对动态调整的输入序列进行预处理。


b.使用批量填充和序列长度 bucketing，提高训练速度和推理速度。


c.使用通用结构和训练时间表训练 seq2seq 模型，包括多种注意力机制和固定抽样。


d.使用 in-graph 集束搜索在 seq2seq 模型中进行推理。


e.优化 seq2seq 模型，以实现在多 GPU 设置中的模型训练。



下文我们将简要地介绍该 Github 教程项目。


**引言**


序列到序列（seq2seq）模型（Sutskever et al., 2014, Cho et al., 2014）在机器翻译、语音识别和文本摘要等任务上取得了巨大的成功。本教程致力于帮助读者全面掌握 seq2seq 模型，并且展示了如何从头开始构建一个强大的 seq2seq 模型。我们该教程会注重神经机器翻译（NMT）任务，神经机器翻译是 seq2seq 模型很好的试验台，并且已经获得了广泛的成功。我们使用的代码是极其轻量、高质量、可投入生产并且结合了最新研究思路的实现。我们通过以下方式实现这一目标：



a.使用最新的解码器/attention wrapper API、TensorFlow 1.2 数据迭代器。


b.结合了我们在构建循环型和 seq2seq 型模型的专业知识。


c.提供了可构建最好 NMT 模型的技巧，同时还复制了谷歌的 NMT（GNMT）系统。



我们相信提供所有人都很容易复制的基准是非常重要的。因此，我们基于以下公开的数据集提供了全部的试验结果和预训练模型：



小规模数据集：TED 演讲的英语-越南语平行语料库（133K 个句子对），该数据集由 IWSLT Evaluation Campaign 提供。


大规模数据集：德语-英语平行语料库（4.5M 个句子对），该数据集由 WMT Evaluation Campaign 提供。



我们首先需要了解用于 NMT 任务的 seq2seq 模型的基本知识，并需要理解如何构建和训练一个 vanilla NMT 模型。第二部分将更进一步详细地解释如何构建带注意力机制的强大神经机器翻译模型。然后我们会讨论构建更好神经机器翻译模型（翻译速度和质量）可能的技巧，例如 TensorFlow 最好的实践方法（batching, bucketing）、双向循环神经网络和集束搜索等。


**关于神经机器翻译**


以词组为基础的传统翻译系统将源语言句子拆分成多个词块，然后进行词对词的翻译。这使得翻译输出结果流畅性大打折扣，远远不如人类译文。我们会通读整个源语言句子、了解句子含义，然后输出翻译结果。神经机器翻译（NMT）竟然可以模仿人类的翻译过程！




![](https://pic2.zhimg.com/v2-80a32f88be6064ad5de0219a9f66a019_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='728' height='123'></svg>)

图 1. 编码器-解码器结构——神经机器翻译的通用方法实例。编码器将源语言句子转换成「意义」向量，然后通过解码器输出翻译结果。



具体来说，神经机器翻译系统首先使用编码器读取源语言句子，构建一个「思想」向量，即代表句义的一串数字；然后使用解码器处理该容器，并输出翻译结果，如图 1 所示。这就是我们通常所说的编码器-解码器结构。神经机器翻译用这种方法解决以词组为基础的传统翻译系统遇到的翻译问题：神经机器翻译能够捕捉语言中的长距离依赖结构，如词性一致、句法结构等，然后输出流利度更高的翻译结果，正如谷歌神经机器翻译系统已经做到的那样。



NMT 模型在具体的结构中会发生变化。对于序列数据而言，最好的选择是循环神经网络（RNN），这也被大多数 NMT 模型采用。通常情况下，编码器和解码器都可使用循环神经网络。但是，循环神经网络模型在下列情况下发生变化：（a）方向性（directionality），单向或双向；（b）深度，单层或多层；（c）类型，通常是 vanilla RNN、长短期记忆（Long Short-term Memory，LSTM），或门控循环单元（gated recurrent unit，GRU）。



感兴趣的读者可打开该网址（[https://colah.github.io/posts/2015-08-Understanding-LSTMs/](https://link.zhihu.com/?target=https%3A//colah.github.io/posts/2015-08-Understanding-LSTMs/)），获取 RNN 和 LSTM 的更多信息。



本教程中，我们将以单向的深度多层 RNN（deep multi-layer RNN）为例，它使用 LSTM 作为循环单元。模型实例如图 2 所示。我们在该实例中构建了一个模型，将源语言句子「I am a student」翻译成目标语言「Je suis étudiant」。该 NMT 模型包括两个循环神经网络：编码器 RNN，在不预测的情况下将输入的源语言单词进行编码；解码器，在预测下一个单词的条件下处理目标句子。



若想参考更多信息，请查看 Luong（2016）（[https://github.com/lmthang/thesis](https://link.zhihu.com/?target=https%3A//github.com/lmthang/thesis)）。


![](https://pic1.zhimg.com/v2-1b5e053c99d040149e467a10e74a55f4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='557' height='370'></svg>)

图 2. 神经机器翻译——一个深度循环结构实例：将源语言句子「I am a student」翻译成目标语言句子「Je suis étudiant」。此处，「<s>」代表解码过程开始，「</s>」代表解码过程结束。


**安装该教程**


为了安装该教程，我们需要先安装 TensorFlow。本教程需要最新的 TensorFlow 教程（目前为 1.2.1 版本）。为了安装 TensorFlow，请按照以下安装指导：[https://www.tensorflow.org/install/](https://link.zhihu.com/?target=https%3A//www.tensorflow.org/install/)。



在安装 TensorFlow 之后，我们需要运行以下命令安装本教程的源代码：



```
git clone https://github.com/tensorflow/nmt/
```


**训练-如何构建我们第一个 NMT 系统**


我们首先需要了解构建一个 NMT 模型具体代码的核心，我们会在图 2 中更详细地讲解。我们后面会介绍数据准备和全部的代码，这一部分是指 model.py 文件。



在网络的底层，编码器和解码器 RNN 接收到以下输入：首先是原句子，然后是从编码到解码模式的过渡边界符号「<s>」，最后是目标语句。对于训练来说，我们将为系统提供以下张量，它们是以时间为主（time-major）的格式，并包括了单词索引：



a. encoder_inputs [max_encoder_time, batch_size]：源输入词。
b. decoder_inputs [max_decoder_time, batch_size]：目标输入词。
c. decoder_outputs [max_decoder_time, batch_size]：目标输出词，这些是 decoder_inputs 按一个时间步向左移动，并且在右边有句子结束符。



为了更高的效率，我们一次用多个句子（batch_size）进行训练。测试略有不同，我们会在后面讨论。


**1.嵌入**


给定单词的分类属性，模型首先必须查找词来源和目标嵌入以检索相应的词表征。为了令该嵌入层能够运行，我们首先需要为每一种语言选定一个词汇表。通常，选定词汇表大小 V，那么频率最高的 V 个词将视为唯一的。而所有其他的词将转换并打上「unknown」标志，因此所有的词将有相同的嵌入。我们通常在训练期间嵌入权重，并且每种语言都有一套。



```
# Embedding
embedding_encoder = variable_scope.get_variable(
    "embedding_encoder", [src_vocab_size, embedding_size], ...)# Look up embedding:#   encoder_inputs: [max_time, batch_size]#   encoder_emp_inp: [max_time, batch_size, embedding_size]
encoder_emb_inp = embedding_ops.embedding_lookup(
    embedding_encoder, encoder_inputs)
```



我们同样可以构建 embedding_decoder 和 decoder_emb_inp。注意我们可以选择预训练的词表征如 word2vec 或 Glove vectors 初始化嵌入权重。通常给定大量的训练数据，我们能从头学习这些嵌入权重。


**2.编码器**


一旦可以检索到，词嵌入就能作为输入馈送到主神经网络中。该网络有两个多层循环神经网络组成，一个是原语言的编码器，另一个是目标语言的解码器。这两个 RNN 原则上可以共享相同的权重，然而在实践中，我们通常使用两组不同的循环神经网络参数（这些模型在拟合大型训练数据集上做得更好）。解码器 RNN 使用零向量作为它的初始状态，并且可以使用如下代码构建：



```
# Build RNN cell
encoder_cell = tf.nn.rnn_cell.BasicLSTMCell(num_units)
# Run Dynamic RNN#   encoder_outpus: [max_time, batch_size, num_units]#   encoder_state: [batch_size, num_units]
encoder_outputs, encoder_state = tf.nn.dynamic_rnn(
    encoder_cell, encoder_emb_inp,
    sequence_length=source_seqence_length, time_major=True)
```



注意语句有不同的长度以避免浪费计算力，因此我们会通过 source_seqence_length 告诉 dynamic_rnn 精确的句子长度。因为我们的输入是以时间为主（time major）的，我们需要设定 time_major=True。现在我们暂时只需要构建单层 LSTM、encoder_cell。我们后面会详细描述怎样构建多层 LSTM、添加 dropout 并使用注意力机制。


**3.解码器**


decoder 也需要访问源信息，一种简单的方式是用编码器最后的隐藏态 encoder_state 对其进行初始化。在图 2 中，我们将源词「student」中的隐藏态传递到了解码器。



```
# Build RNN cell
decoder_cell = tf.nn.rnn_cell.BasicLSTMCell(num_units)
# Helper
helper = tf.contrib.seq2seq.TrainingHelper(
    decoder_emb_inp, decoder_lengths, time_major=True)# Decoder
decoder = tf.contrib.seq2seq.BasicDecoder(
    decoder_cell, helper, encoder_state,
    output_layer=projection_layer)# Dynamic decoding
outputs, _ = tf.contrib.seq2seq.dynamic_decode(decoder, ...)
logits = outputs.rnn_output
```



此处代码的核心是 BasicDecoder、获取 decoder_cell(类似于 encoder_cell) 的 decoder、helper 以及之前作为输入的 encoder_state。



通过分离 decoders 和 helpers，我们能重复使用不同的代码库，例如 TrainingHelper 可由 GreedyEmbeddingHelper 进行替换，来做贪婪解码。



最后，我们从未提到过的 projection_layer 是一个密集矩阵，将顶部的隐藏态转变为维度 V 的逻辑向量。我们在图 2 的上部展示了此过程。



```
projection_layer = layers_core.Dense(
    tgt_vocab_size, use_bias=False)
```


**4.损失**


给出以上的 logits，可计算训练损失：



```
crossent = tf.nn.sparse_softmax_cross_entropy_with_logits(
    labels=decoder_outputs, logits=logits)
train_loss = (tf.reduce_sum(crossent * target_weights) /
    batch_size)
```



以上代码中，target_weights 是一个与 decoder_outputs 大小一样的 0-1 矩阵。该矩阵将目标序列长度以外的其他位置填充为标量值 0。



我们需要指出来的是，训练损失可以由 batch_size 分割，因此我们的超参数 batch_size 是「不变量」。也有些人将训练损失按照 batch_size * num_time_steps 分割，这样可以减少短句所造成的误差。更巧妙的，我们的超参数（应用于前面的方法）不能用于后面的方法。例如，如果两种方法都是用学习率为 1.0 的随机梯度下降，后面的方法将更有效地利用一个较小的学习率，即 1 / num_time_steps。


**5.梯度计算和优化**


现在是时候定义我们的 NMT 模型的前向传播了。计算反向传播只需要写几行代码：



```
# Calculate and clip gradients
parameters = tf.trainable_variables()
gradients = tf.gradients(train_loss, params)
clipped_gradients, _ = tf.clip_by_global_norm(
    gradients, max_gradient_norm)
```



训练 RNN 的一个重要步骤是梯度截断（gradient clipping）。这里，我们使用全局范数进行截断操作。最大值 max_gradient_norm 通常设置为 5 或 1。最后一步是选择优化器。Adam 优化器是最常见的选择。我们还要选择一个学习率，learning_rate 的值通常在 0.0001 和 0.001 之间，且可设置为随着训练进程逐渐减小。



```
# Optimization
optimizer = tf.train.AdamOptimizer(learning_rate)
update_step = optimizer.apply_gradients(
    zip(clipped_gradients, params))
```



在我们的实验中，我们使用标准的随机梯度下降（tf.train.GradientDescentOptimizer），并采用了递减的学习率方案，因此也就有更好的性能。


**开始训练 NMT 模型**


让我们开始训练第一个 NMT 模型，将越南语翻译为英语。代码的入口是** **nmt.py。



我们将使用小规模的 Ted 演讲并行语料库（133k 的训练样本）进行训练。所有的数据都可从以下链接找到：[https://nlp.stanford.edu/projects/nmt/](https://link.zhihu.com/?target=https%3A//nlp.stanford.edu/projects/nmt/)。



我们将使用 tst2012 作为开发数据集，tst 2013 作为测试数据集。运行以下命令行下载数据训练 NMT 模型：



```
nmt/scripts/download_iwslt15.sh /tmp/nmt_data
```



运行以下命令行开始训练：



```
mkdir /tmp/nmt_model
python -m nmt.nmt \
    --src=vi --tgt=en \
    --vocab_prefix=/tmp/nmt_data/vocab  \
    --train_prefix=/tmp/nmt_data/train \
    --dev_prefix=/tmp/nmt_data/tst2012  \
    --test_prefix=/tmp/nmt_data/tst2013 \
    --out_dir=/tmp/nmt_model \
    --num_train_steps=12000 \
    --steps_per_stats=100 \
    --num_layers=2 \
    --num_units=128 \
    --dropout=0.2 \
    --metrics=bleu
```



以上命令行训练一个 2 层的 LSTM seq2seq 模型，带有 128-dim 的隐藏单元和 12 个 epochs 的嵌入。我们使用 0.2（或然率为 0.8）的 dropout 值。如果没误差，在我们训练中随着降低混淆度，我们应该能看到类似于以下的 logs。



```
# First evaluation, global step 0
  eval dev: perplexity 17193.66
  eval test: perplexity 17193.27
# Start epoch 0, step 0, lr 1, Tue Apr 25 23:17:41 2017
  sample train data:
    src_reverse: </s> </s> Điều đó , dĩ nhiên , là câu chuyện trích ra từ học thuyết của Karl Marx .
    ref: That , of course , was the <unk> distilled from the theories of Karl Marx . </s> </s> </s>
  epoch 0 step 100 lr 1 step-time 0.89s wps 5.78K ppl 1568.62 bleu 0.00
  epoch 0 step 200 lr 1 step-time 0.94s wps 5.91K ppl 524.11 bleu 0.00
  epoch 0 step 300 lr 1 step-time 0.96s wps 5.80K ppl 340.05 bleu 0.00
  epoch 0 step 400 lr 1 step-time 1.02s wps 6.06K ppl 277.61 bleu 0.00
  epoch 0 step 500 lr 1 step-time 0.95s wps 5.89K ppl 205.85 bleu 0.00
```



更多细节，请查看：train.py。



我们可以使用 Tensorboard 在训练过程中查看模型的总结：



```
tensorboard --port 22222 --logdir /tmp/nmt_model/
```



通过以下简单的变化，就能逆向完成英语到越南语的翻译。



```
--src=en --tgt=vi
```


**推理——如何生成翻译**


当你训练你的 NMT 模型时（并且一旦你已经训练了模型），可以在给定之前不可见的源语句的情况下获得翻译。这一过程被称作推理。训练与推理之间有一个明确的区分（测试）：在推理时，我们只访问源语句，即 encoder_inputs。解码的方式有很多种，包括 greedy 解码、采样解码和束搜索解码（beam-search）。下面我们讨论一下 greedy 解码策略。



其想法简单，我们将在图 3 中作说明：



a.在训练获取 encoder_state 的过程中，我们依然以相同方式编码源语句，并且 encoder_state 用于初始化解码器。
b.一旦解码器接收到开始符 <s>（在我们的代码中指 tgt_sos_id），就开始解码处理（翻译）。
c.最大的单词，其 id 与最大的 logit 值相关联，正如被发出的词（这是 greedy 行为）。例如在图 3 中，单词 moi 在第一个解码步中具有最高的翻译概率。接着我们把这一单词作为输入馈送至下一个时间步。
d.这一过程会持续到这句话的终止符「</s>」，然后输出（在我们的代码中是 tgt_eos_id）。





![](https://pic2.zhimg.com/v2-648766ad7538222ea0bf8cf00275f72d_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='546' height='402'></svg>)

图 3. Greedy 解码——一个实例：已训练的 NMT 模型如何使用 greedy 搜索为源语句 Je suis étudiant 生成翻译。



推理与训练的区别在于步骤 3。推理不总是馈送作为输入的正确目标词，而是使用被模型预测的单词。下面是实现 greedy 解码的代码。它与训练解码器非常相似。



```
# Helper
helper = tf.contrib.seq2seq.GreedyEmbeddingHelper(
    embedding_decoder,
    tf.fill([batch_size], tgt_sos_id), tgt_eos_id)
# Decoder
decoder = tf.contrib.seq2seq.BasicDecoder(
    decoder_cell, helper, encoder_state,
    output_layer=projection_layer)# Dynamic decoding
outputs, _ = tf.contrib.seq2seq.dynamic_decode(
    decoder, maximum_iterations=maximum_iterations)
translations = outputs.sample_id
```



我们在本文中使用了 GreedyEmbeddingHelper 而不是 TrainingHelper。由于无法提前知道目标语句的长度，我们使用 maximum_iterations 限制翻译的长度。一个启发是解码最多两倍的源语句长度。



```
maximum_iterations = tf.round(tf.reduce_max(source_sequence_length) * 2)
```



我们已经训练了一个模型，现在可以创建一个推理文件并翻译一些语句：



```
cat > /tmp/my_infer_file.vi# (copy and paste some sentences from /tmp/nmt_data/tst2013.vi)

python -m nmt.nmt \
    --model_dir=/tmp/nmt_model \
    --inference_input_file=/tmp/my_infer_file.vi \
    --inference_output_file=/tmp/nmt_model/output_infer

cat /tmp/nmt_model/output_infer # To view the inference as output
```



注意上述指令也可在模型被训练时运行，只要存在一个训练检查点。详见 inference.py。


**中级**


在训练了一些最基本的序列到序列模型之后，我们现在更进一步。为了打造当前最优的神经机器翻译系统，我们需要更多的秘诀：注意力机制。该机制由 Bahdanau 等人在 2015 年首次提出（[https://arxiv.org/abs/1409.0473](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1409.0473)），稍后 Luong 等人和其他人完善了它，其核心思想是当我们翻译时通过「注意」相关的源内容，建立直接的短连接。注意力机制的一个很好副产品是源语句和目标语句之间的一个易于可视化的对齐矩阵（如图 4 所示）。





![](https://pic1.zhimg.com/v2-670eb2565fc403e4ffabb340bf8978e4_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='415' height='374'></svg>)

图 4. 注意力可视化——源语句与目标语句之间对齐的实例。图片来自 2015 年 Bahdanau 等人的论文。



请记住在 vanilla 序列到序列模型中，当开始编码处理时，我们把最后的源状态从编码器传递到解码器。这对短、中长度的语句效果很好；对于长句子，单一固定大小的隐状态成为了信息瓶颈。注意力机制没有摈弃源 RNN 中计算的所有隐状态，而是提出了允许解码器窥探它们的方法（把它们看作是源信息的动态存储）。如此，注意力机制提升了长句的翻译质量。现在，注意力机制实至名归，已成功应用于其他诸多任务（比如语音识别）。


**注意力机制背景**


我们现在描述一下注意力机制的实例（Luong et al., 2015），它已经被应用到几个最新型的系统当中了，包括开源工具，比如 OpenNMT（[http://opennmt.net/about/](https://link.zhihu.com/?target=http%3A//opennmt.net/about/)）和此教程中的 TF seq2seq API。我们还将会提供注意力机制相关变体的内容。

![](https://pic1.zhimg.com/v2-2758ff14ecb4d6a1eadb4db4388e8a04_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='452' height='336'></svg>)

图 5. 注意力机制——基于注意力的 NMT 系统（Luong et al., 2015 中有具体描述）。我们重点详解注意力计算过程中的第一步。为了更加清晰，我们没有展示图（2）中的嵌入层和投影层。



如图 5 所示，注意力计算发生在解码步骤中的每一步。它包含下列步骤：



当前目标隐蔽状态和所有源状态（source state）进行比较，以导出权重（weight），见图 4。
基于注意力权重，我们计算了一个背景向量（context vector），作为源状态的平均权值。
将背景向量与当前目标隐蔽态进行结合以生成最终的注意力向量。
此注意力向量将作为下一时序步骤的输入。前三个步骤可以由下列公式总结：

![](https://pic3.zhimg.com/v2-6d9bd4a85ad41032613d91677fdb0e16_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='729' height='180'></svg>)




这里，函数 score 用于将目标隐蔽状态 ht 和每一个源状态 hs 进行比较，结果会被标准化成生成式注意力权重（一个源位置的分布）。其实有很多种关于评分函数（scoring function）的选择；比较流行的评分函数包括公式（4）中给出的乘法与加法形式。一旦被计算，注意力向量 at 就会用于推导 softmax logit 和损失。这与 vanilla seq2seq 模型顶层的目标隐蔽态相似。函数 f 也可以利用其它形式。

![](https://pic2.zhimg.com/v2-e530ab037a3b08270f4d26b84dd8c655_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='718' height='97'></svg>)




注意力机制的多种实现方法可由以下链接获得：[https://github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/seq2seq/python/ops/attention_wrapper.py](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/tensorflow/blob/master/tensorflow/contrib/seq2seq/python/ops/attention_wrapper.py)。



注意力机制中有什么相关注意事项呢？



上述公式表明注意力机制有很多种变体。这些变体依赖于评分函数（scoring function）和注意力函数（attention function）的形式，也依赖于前一状态 ht-1，而不依赖于开始建议的评分函数 ht（Bahdanau et al., 2015）。实际上我们发现的只有一些选择上的注意事项。一，注意力的基本形式，例如，目标和源之间的直接联系需要被呈现。二，把注意力向量输入给下一时间步骤，以把之前的注意力决策告知给网络（Luong et al., 2015）。最后，评分函数的选择经常可以造成不同的性能表现。


**Attention Wrapper API**


在我们的 Attention Wrapper API 的实现中，借鉴了 Weston et al., 2015 在 onmemory network 工作中的术语。相比于拥有可读、可写的记忆，此教程中的 attention 机制仅是可读的记忆。特别是对隐藏态（或者隐藏态的变体，例如 $$W\overline{h}_s$$ in Luong's scoring style or $$W_2\overline{h}_s$$ ) 的设定，被认为是「记忆」。在每个时间步下，我们使用现有的目标隐藏态作为「query」决定读取哪一部分记忆。通常情况下，query 需要与单个记忆条相对应的 keys 进行对比。在上面对注意机制的演示中，我们偶然使用一套源隐藏态（或者其变体，例如$$W_1h_t$$）作为「key」。你们可以从这种记忆网络术语中获取灵感，找到其他形式的 attention。



由于 attention wrapper，就不再需要扩展我们带有 attention 的 vanilla seq2seq 代码。这部分文件为 attention_model.py。



首先，我们需要定义一种注意机制，例如采用 Luong et al., 2015 的研究。



```
# attention_states: [batch_size, max_time, num_units]
attention_states = tf.transpose(encoder_outputs, [1, 0, 2])
# Create an attention mechanism
attention_mechanism = tf.contrib.seq2seq.LuongAttention(
    num_units, attention_states,
    memory_sequence_length=source_sequence_length)
```



在之前的 Encoder 部分，encoder_outputs 是一套顶层的掩藏态源，形式为 [max_time, batch_size, num_units]（因为我们使用 dynamic_rnn with time_major 设定）。在注意机制上，我们需要保证通过的「memory」是批次为主的，所以需要调换 attention_states。我们通过 source_sequence_length 保证注意机制的权重有适当的规范化（只在 non-padding 的位置）。定义完注意机制之后，我们使用 AttentionWrapper 来包裹解码单元。



```
decoder_cell = tf.contrib.seq2seq.AttentionWrapper(
    decoder_cell, attention_mechanism,
    attention_layer_size=num_units)
```



剩下的代码基本和编码器一转样 ([https://github.com/tensorflow/nmt#decoder](https://link.zhihu.com/?target=https%3A//github.com/tensorflow/nmt%23decoder))!



上手—打造一个基于注意力的 NMT 模型



为了使注意力发挥作用，我们需要用到 luong、scaled_luong、bahdanau 或 normed_bahdanau 其中的一个作为训练期间注意力标志（attention flag）的值。该标志指定了我们将要使用的注意力机制。除此之外，我们需要为注意力模型创建一个新目录，因此无需重新使用之前训练的基本 NMT 模型。



运行以下指令开始训练：



```
mkdir /tmp/nmt_attention_model

python -m nmt.nmt \
    --attention=scaled_luong \
    --src=vi --tgt=en \
    --vocab_prefix=/tmp/nmt_data/vocab  \
    --train_prefix=/tmp/nmt_data/train \
    --dev_prefix=/tmp/nmt_data/tst2012  \
    --test_prefix=/tmp/nmt_data/tst2013 \
    --out_dir=/tmp/nmt_attention_model \
    --num_train_steps=12000 \
    --steps_per_stats=100 \
    --num_layers=2 \
    --num_units=128 \
    --dropout=0.2 \
    --metrics=bleu
```



训练之后，我们可以使用带有新 model_dir 的相同推理指令进行推理：



```python
python -m nmt.nmt \
    --model_dir=/tmp/nmt_attention_model \
    --inference_input_file=/tmp/my_infer_file.vi \
    --inference_output_file=/tmp/nmt_attention_model/output_infer
```


**基准**

IWSLT 英语-越南语

训练：133k 的样本，dev=tst2012，test=tst2013


![](https://pic4.zhimg.com/v2-64f20fa245f2a182476d52d5c1427393_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='471' height='164'></svg>)




训练速度：在英伟达 K40m 上是 0.37s 的时间步、15.3k 的 wps，在 Titan X 上是 0.17 s 的时间步，32.2k 的 wps.。



WMT 德语-英语



训练：4.5M 的样本量，dev=newstest2013，test=newtest2015

![](https://pic1.zhimg.com/v2-27e2bdfe9716e5e62a9350cadb661d70_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='579' height='198'></svg>)


训练速度：在英伟达 K40m 上是 2.1s 的时间步，3.4k 的 wps，在英伟达 Titan X 上是 0.7s 的时间步，8.7k 的 wps。



为了查看 GNMT 注意的加速度，我们只在 K40m 上做了基准测试：


![](https://pic1.zhimg.com/v2-062a22e6d9d071317a12df8a59ca7648_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='557' height='201'></svg>)


WMT 英语-德语 全对比

第二行是我们 GNMT 注意模型：模型 1（4 层），模型 2（8 层）。


![](https://pic4.zhimg.com/v2-89faf23cdf9e9fabaf3e677603b35eb3_b.png)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='583' height='272'></svg>)



**其他资源**


若想深入了解神经机器翻译和序列-序列模型，我们非常推荐以下资源：



Neural Machine Translation and Sequence-to-sequence Models: A Tutorial：[https://arxiv.org/abs/1703.01619](https://link.zhihu.com/?target=https%3A//arxiv.org/abs/1703.01619)
Neural Machine Translation - Tutorial ACL 2016：[https://sites.google.com/site/acl16nmt/](https://link.zhihu.com/?target=https%3A//sites.google.com/site/acl16nmt/)
Thang Luong's Thesis on Neural Machine Translation：[https://github.com/lmthang/thesis](https://link.zhihu.com/?target=https%3A//github.com/lmthang/thesis)


用于构建 seq2seq 模型的工具很多：



Stanford NMT [https://nlp.stanford.edu/projects/nmt/](https://link.zhihu.com/?target=https%3A//nlp.stanford.edu/projects/nmt/) [Matlab] 
tf-seq2seq [https://github.com/google/seq2seq](https://link.zhihu.com/?target=https%3A//github.com/google/seq2seq) [TensorFlow] 
Nemantus [https://github.com/rsennrich/nematus](https://link.zhihu.com/?target=https%3A//github.com/rsennrich/nematus) [Theano] 
OpenNMT [http://opennmt.net/](https://link.zhihu.com/?target=http%3A//opennmt.net/) [Torch]


**参考内容**


[1] Sequence to sequence learning with neural networks ([https://papers.nips.cc/paper/5346-sequence-to-sequence-learning-with-neural-networks.pdf](https://link.zhihu.com/?target=https%3A//papers.nips.cc/paper/5346-sequence-to-sequence-learning-with-neural-networks.pdf)), Ilya Sutskever, Oriol Vinyals, and Quoc V. Le. NIPS, 2014.
[2] Learning phrase representations using RNN encoder-decoder for statistical machine translation ([http://aclweb.org/anthology/D/D14/D14-1179.pdf](https://link.zhihu.com/?target=http%3A//aclweb.org/anthology/D/D14/D14-1179.pdf)), Kyunghyun Cho, Bart Van Merrienboer, Caglar Gulcehre, Dzmitry Bahdanau, Fethi Bougares, Holger Schwenk, and Yoshua Bengio. EMNLP 2014.
[3] Neural machine translation by jointly learning to align and translate ([https://arxiv.org/pdf/1409.0473.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1409.0473.pdf)), Dzmitry Bahdanau, Kyunghyun Cho, and Yoshua Bengio. ICLR, 2015. 
[4] Effective approaches to attention-based neural machine translation ([https://arxiv.org/pdf/1508.04025.pdf](https://link.zhihu.com/?target=https%3A//arxiv.org/pdf/1508.04025.pdf)), Minh-Thang Luong, Hieu Pham, and Christopher D Manning. EMNLP, 2015. 
[5] Google‘s Neural Machine Translation System: Bridging the Gap between Human and Machine Translation ([http://arxiv.org/abs/1609.08144](https://link.zhihu.com/?target=http%3A//arxiv.org/abs/1609.08144))，2016.




选自GitHub  **机器之心编译**


