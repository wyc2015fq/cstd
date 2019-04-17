# 预训练BERT，官方代码发布前他们是这样用TensorFlow解决的 - 知乎
# 



机器之心编辑，参与：思源、王淑婷。

> 这个月谷歌提出的 [BERT](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749886%26idx%3D1%26sn%3D87080bc474d144b286d4673383f4b6d6%26chksm%3D871affc0b06d76d6ab49b2828a8b795b18e3d762b3978a83704a01dd60fb6dbcde2ed00e163d%26scene%3D21%23wechat_redirect) 受到了很多关注，该研究凭借预训练模型刷新了 11 项 NLP 任务的当前最优性能记录。论文作者表示这个月月末会放出代码与预训练模型，但目前还没有发布。因此很多研究者尝试使用中等数据集降低计算力，或使用 OpenAI 的 Transformer 预训练模型作为初始化条件。

本文介绍的两个 BERT 实现项目分别基于 TensorFlow 和 Keras，其中基于 TensorFlow 的项目会使用中等数据集与其它技巧降低计算力，并发现使用 TextCNN 代替 Transformer 主干网络，且保留 BERT 预训练任务也能得到非常好的效果。而基于 Keras 的项目尝试使用预训练的 OpenAI Transformer 作为初始化权重，并以较小的计算力重新训练 BERT 预训练模型，再将该预训练的 BERT 应用到不同任务。

这两个项目都在尝试使用 BERT 核心思想，并以较小的计算成本应用于其它 NLP 任务。当然如果读者希望使用大型 BERT 预训练模型，还需要等谷歌官方发布代码与模型。

## **BERT 简介**

BERT 的全称是基于 Transformer 的双向编码器表征，其中「双向」表示模型在处理某一个词时，它能同时利用前面的词和后面的词两部分信息。这种「双向」的来源在于 BERT 与传统语言模型不同，它不是在给定所有前面词的条件下预测最可能的当前词，而是随机遮掩一些词，并利用所有没被遮掩的词进行预测。下图展示了三种预训练模型，其中 BERT 和 ELMo 都使用双向信息，OpenAI GPT 使用单向信息。
![](https://pic3.zhimg.com/v2-833e9a80f13bd24bc0a3f22b55536802_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='286'></svg>)
如上所示为不同预训练模型的架构，BERT 可以视为结合了 OpenAI GPT 和 ELMo 优势的新模型。其中 ELMo 使用两条独立训练的 LSTM 获取双向信息，而 OpenAI GPT 使用新型的 Transformer 和经典语言模型只能获取单向信息。BERT 的主要目标即在 OpenAI GPT 的基础上对预训练任务做一些改进，以同时利用 Transformer 深度模型与双向信息的优势。

BERT 的核心过程非常简洁，它会先从数据集抽取两个句子，其中第二句是第一句的下一句概率是 50%，这样就能学习句子之间的关系。其次随机去除两个句子中的一些词，并要求模型预测这些词是什么，这样就能学习句子内部的关系。最后再将经处理的句子传入大型 Transformer 模型，并通过两个损失函数同时学习上面两个目标就能完成训练。

## **TensorFlow 实现项目简介**

BERT 最近在 10 几项 NLP 任务上取得了新进展，这个项目是《[BERT：Pre-training of Deep Bidirectional Transformers for Language Understanding](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749886%26idx%3D1%26sn%3D87080bc474d144b286d4673383f4b6d6%26chksm%3D871affc0b06d76d6ab49b2828a8b795b18e3d762b3978a83704a01dd60fb6dbcde2ed00e163d%26scene%3D21%23wechat_redirect)》和《[Attention is all You Need](https://link.zhihu.com/?target=http%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650727887%26idx%3D5%26sn%3Da5991e49a4b1df2ea70ddbb84631ff9c%26chksm%3D871b21b1b06ca8a71be9ec7ee1d7d5ef46eef1116c3d396f3cc467b95186d278bcdd8e6ac07c%26scene%3D21%23wechat_redirect)》这两篇论文的 tensorflow 实现。

这个项目提供了预训练方法与代码，并做了一些调整以加快收敛速度。这一份 TensorFlow 实现在使用中等数据集下计算力并不是太大，所以感兴趣的读者也可以尝试使用。当然，希望使用大型预训练 BERT 模型的读者可以等谷歌发布官方模型。

项目地址：[https://github.com/brightmart/bert_language_understanding](https://link.zhihu.com/?target=https%3A//github.com/brightmart/bert_language_understanding)

## **预训练和微调实验**

项目作者把 Transfprmer 换成 TextCNN，替换了 BERT 的主干网络，结果发现使用大量原始数据用遮蔽语言模型预训练的模型可以显著提高性能，因此他们认为预训练和微调策略是独立于模型和预训练任务的。因此，可以修正主干网络添加更多的预训练任务或者定义一些新的预训练任务，预训练不限于遮蔽语言模型或预测下一句的任务。让人惊讶的是，对于中等规模的数据集（比如说一百万条数据）来说，即使不使用外部数据，只要借助于预训练任务（如带掩码的语言模型），性能也可以大幅提升，而且模型可以更快地收敛。有时在微调阶段，训练仅需几个 epoch。

**目的**

虽然开放源码（tensor2tensor）和 Transformer 与 BERT 的官方实现实现即将到来，但是有可能很难懂，不易理解。该项目的作者并不打算完全复制原始文件，而是要应用主要思想，以更好的方式解决 NLP 问题。本文的大部分工作是去年由另一个 GitHub 项目修改完成的：文本分类（[https://github.com/brightmart/text_classification](https://link.zhihu.com/?target=https%3A//github.com/brightmart/text_classification)）。

**性能**

下图是在中等规模数据集（cail2018,45 万）上文本分类任务的测试效果，其中采用 BERT 预训练思想的 TextCNN 要显著优于其它模型：
![](https://pic1.zhimg.com/v2-6d874e968f4a326318dd5b61d7719bd0_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='937' height='279'></svg>)
下图是在小规模数据集（private，10 万）上的测试效果：
![](https://pic1.zhimg.com/v2-be5b49b9475ae3501152dd48be4e0450_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='958' height='196'></svg>)
## **TensorFlow 实现项目细节**

**使用方法**

如果想在 Masked 语言模型上预训练 BERT 模型，并在新 NLP 任务上使用它，那么使用方法主要可以分为两步骤。值得注意的是，该项目并没有提供预训练模型，所以需要大量计算力的预训练过程仍然需要自行执行。

1. 通过 BERT 预训练语言模型

` python train_bert_lm.py [DONE]`

2. 在新任务微调模型

` python train_bert_fine_tuning.py [Done]`

在项目作者的试验中，即使在微调的起点，刚刚从预训练模型恢复参数也能获得比从头训练更低的损失。预训练模型的 F1 值同样要比从头训练的高，且从头训练的 F1 值还需要从零开始增长。

此外为了快速测试新想法与模型，可以将超参数 test_mode 设置为 True，在这种模式下模型只会加载少量的数据进行测试，因此训练非常迅速。

在基本步骤中，还可以通过 Transform 解决文本分类问题：

`python train_transform.py [DONE, but a bug exist prevent it from converge, welcome you to fix, email: brightmart@hotmail.com]`

预训练和微调过程还有其它一些可选超参数，它们控制了模型的大小与训练过程：
- d_model：模型维度，默认为 [512]。
- num_layer：层级数，默认为 [6]。
- num_header：自注意力机制的 Head 数，默认为 [8]。
- d_k：Key（K）和 Query（Q）的维度，默认为 [64]。
- d_v：Value（V）的维度，默认为 [64]。


```
default hyperparameter is d_model=512,h=8,d_k=d_v=64(big). if you have want to train the model fast, or has a small data set 

or want to train a small model, use d_model=128,h=8,d_k=d_v=16(small), or d_model=64,h=8,d_k=d_v=8(tiny).
```


**实现细节**

首先，TensorFlow 的实现环境比较简单：python 3+ tensorflow 1.10。其次，实现时要注意以下问题：

1. 预训练和微调阶段之间有哪些能够共享和无法共享的参数？

基本来说，预训练和微调阶段的主干网络使用的所有参数都能共享。

既然可以共享很多参数，那微调阶段只需学习很少的参数，此外这两个阶段的词嵌入也可以共享。

因此，在微调的初始阶段已经学习了大部分参数。

2. 如何实现带 Mask 的语言模型？

为了让事情变得简单点，可以根据文件生成很多句子。把每个句子截断并填充到相同的长度，然后随机选择一个单词，用 [MASK]、单词本身或一个随机单词替换它。

3. 如何使微调阶段变得更高效并同时不影响在预训练阶段学到的结果和知识？

在微调阶段使用较小的学习率，因此只需在很小的范围内进行调整。

## **Keras 实现**

基于 TensorFlow 的实现同样没有提供预训练语言模型，这样的模型在预训练阶段会需要大量的计算力，这样的计算力需求对于很多研究者与开发者都是接受不了的。但是现在的官方实现与预训练模型仍然没有放出来，因此有开发者利用 OpenAI 预训练的 Transformer 作为初始化参数，并训练新的 BERT 预训练模型，这种方式大大降低了计算力需求。

项目地址：[https://github.com/Separius/BERT-keras](https://link.zhihu.com/?target=https%3A//github.com/Separius/BERT-keras)

在这个 Keras 实现项目中，作者用预训练的 OpenAI Transformer 作为初始化条件，并训练新的 BERT，项目作者表示这样可以不使用 TPU 而实现预训练。以下展示了 Keras 实现的主要语句，包括加载 OpenAI Transformer 预训练模型、加载 BERT 模型和保存新的预训练权重等。


```
# this is a pseudo code you can read an actual working example in tutorial.ipynb
text_encoder = MyTextEncoder(**my_text_encoder_params) # you create a text encoder (sentence piece and openai's bpe are included)
lm_generator = lm_generator(text_encoder, **lm_generator_params) # this is essentially your data reader (single sentence and double sentence reader with masking and is_next label are included)
task_meta_datas = [lm_task, classification_task, pos_task] # these are your tasks (the lm_generator must generate the labels for these tasks too)
encoder_model = create_transformer(**encoder_params) # or you could simply load_openai()
trained_model = train_model(encoder_model, task_meta_datas, lm_generator, **training_params) # it does both pretraing and finetuning
trained_model.save_weights('my_awesome_model') # save it
model = load_model('my_awesome_model', encoder_model) # load it later and use it!
```


作者表示这个项目有一些很重要说明，针对不同的任务与需求，可以根据这些说明修改模型结构和预训练过程。
- 这个库的核心观点是使用 OpenAI 的预训练模型作为训练新模型的初始状态，因此通过 GPU 就能训练 BERT。
- 通过 Keras 加载 OpenAI 模型已经在 TensorFlow 后端和 Theano 后端得到测试。
- 对于大多数 NLP 模型，能使用这个项目定义的数据生成器和任务元数据，即使在其它框架中也是。
- 数据集和 Transformer 都会执行一些单元测试，如果你不太了解代码可以阅读这些测试。
- 还可以使用其它编码器进行训练，例如 LSTM 或 BiQRNN 等。
- 当官方代码发布后会发生什么？数据读取器仍然会保持稳定，甚至可以导入官方发布的权重到这个库中（作者认为他会完成这一过程，因为实际的 Transformer 还是比较容易实现的）
- 作者强烈建议阅读项目中的 tutorial.ipynb 文件，它展示了整个项目的使用过程。

**重要的代码概念**
- 任务：有两个一般任务，句子级任务（如下一句预测和情感分析）和 token 级任务（如词性标注和命名实体识别）。
- 句子：「句子」表示一段带有标签和所有内容的实例，它为每个任务提供了一个目标（句子级任务的单个标注值，token 级任务的每个 token 标签）和一个掩码；对于 token 级任务，除了忽略 padding 外，还要使用第一个符号向量预测类别（BERT 中的 [CLS] 符号）。
- TaskWeightScheduler：项目作者希望从语言模型开始训练，然后迁移到到分类任务，我们可以用这个类快速实现。
- special_tokens：pad, start, end, delimiter, mask 
*![](https://pic1.zhimg.com/v2-18cc987d5f379a82f1208b6d90722318_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='73' height='24'></svg>)*



