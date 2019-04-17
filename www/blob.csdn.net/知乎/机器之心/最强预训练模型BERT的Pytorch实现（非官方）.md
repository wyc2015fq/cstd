# 最强预训练模型BERT的Pytorch实现（非官方） - 知乎
# 



选自GitHub，作者：Junseong Kim，机器之心编译，参与：路雪、张倩。

> 近期，Google AI 公开了一篇 NLP 论文，介绍了新的语言表征模型 [BERT](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650749886%26idx%3D1%26sn%3D87080bc474d144b286d4673383f4b6d6%26scene%3D21%23wechat_redirect)，该模型被认为是最强 NLP 预训练模型，刷新了 11 项 NLP 任务的当前最优性能记录。今日，机器之心发现 GitHub 上出现了 BERT 的 Pytorch 实现，出自 Scatter Lab 的 Junseong Kim。

## **简介**

谷歌 AI 关于 BERT 的论文展示了该模型在多个 NLP 任务上取得的惊艳结果，包括在 SQuAD v1.1 QA 任务上的 F1 得分超过人类。该论文证明，基于 Transformer（自注意力）的编码器可以有力地替代之前以合理方式训练的语言模型。更重要的是，该论文表明这一预训练语言模型可用于任何 NLP 任务，而无需针对任务定制模型架构。

本文主要阐述 BERT 的实现。它的代码非常简单、易懂。一些代码基于《[Attention is All You Need](https://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s%3F__biz%3DMzA3MzI4MjgzMw%3D%3D%26mid%3D2650727887%26idx%3D5%26sn%3Da5991e49a4b1df2ea70ddbb84631ff9c%26scene%3D21%23wechat_redirect)》一文中的 annotated Transformer。

该项目目前还在进展阶段。代码尚未得到验证。

## **语言模型预训练**

在这篇论文中，作者展示了语言模型训练的新方法，即「遮蔽语言模型」（masked language model，MLM）和「预测下一句」。




**Masked LM**

> 见原论文：3.3.1 Task #1: Masked LM


```
Input Sequence  : The man went to [MASK] store with [MASK] dog
Target Sequence :                  the                his
```





**规则：**

基于以下子规则，随机 15% 的输入 token 将被改变：
- 80% 的 token 是 [MASK] token。
- 10% 的 token 是 [RANDOM] token（另一个单词）。
- 10% 的 token 将维持不变，但是需要预测。




**预测下一句**

> 见原论文：3.3.2 Task #2: Next Sentence Prediction


```
Input : [CLS] the man went to the store [SEP] he bought a gallon of milk [SEP]
Label : Is Next

Input = [CLS] the man heading to the store [SEP] penguin [MASK] are flight ##less birds [SEP]
Label = NotNext
```


「这个句子可以继续和下一个句子联系起来吗？」

理解两个文本句子之间的关系，这是无法通过语言建模直接获取的。

规则：
- 下一句有 50% 的可能是连续的句子。
- 下一句有 50% 的可能是无关的句子。

## **使用**

注意：你的语料库一行应该有两个句子，中间用 (\t) 分隔符隔开。


```
Welcome to the \t the jungle \n
I can stay \t here all night \n
```


**1. 基于自己的语料库构建 vocab**

`python build_vocab.py -c data/corpus.small -o data/corpus.small.vocab`





```
usage: build_vocab.py [-h] -c CORPUS_PATH -o OUTPUT_PATH [-s VOCAB_SIZE]
                      [-e ENCODING] [-m MIN_FREQ]

optional arguments:
  -h, --help            show this help message and exit
  -c CORPUS_PATH, --corpus_path CORPUS_PATH
  -o OUTPUT_PATH, --output_path OUTPUT_PATH
  -s VOCAB_SIZE, --vocab_size VOCAB_SIZE
  -e ENCODING, --encoding ENCODING
  -m MIN_FREQ, --min_freq MIN_FREQ
```





**2. 使用自己的语料库构建 BERT 训练数据集**

`python build_dataset.py -d data/corpus.small -v data/corpus.small.vocab -o data/dataset.small`





```
usage: build_dataset.py [-h] -v VOCAB_PATH -c CORPUS_PATH [-e ENCODING] -o
                        OUTPUT_PATH

optional arguments:
  -h, --help            show this help message and exit
  -v VOCAB_PATH, --vocab_path VOCAB_PATH
  -c CORPUS_PATH, --corpus_path CORPUS_PATH
  -e ENCODING, --encoding ENCODING
  -o OUTPUT_PATH, --output_path OUTPUT_PATH
```





**3. 训练自己的 BERT 模型**

`python train.py -d data/dataset.small -v data/corpus.small.vocab -o output/`





```
usage: train.py [-h] -d TRAIN_DATASET [-t TEST_DATASET] -v VOCAB_PATH -o
                OUTPUT_DIR [-hs HIDDEN] [-n LAYERS] [-a ATTN_HEADS]
                [-s SEQ_LEN] [-b BATCH_SIZE] [-e EPOCHS]

optional arguments:
  -h, --help            show this help message and exit
  -d TRAIN_DATASET, --train_dataset TRAIN_DATASET
  -t TEST_DATASET, --test_dataset TEST_DATASET
  -v VOCAB_PATH, --vocab_path VOCAB_PATH
  -o OUTPUT_DIR, --output_dir OUTPUT_DIR
  -hs HIDDEN, --hidden HIDDEN
  -n LAYERS, --layers LAYERS
  -a ATTN_HEADS, --attn_heads ATTN_HEADS
  -s SEQ_LEN, --seq_len SEQ_LEN
  -b BATCH_SIZE, --batch_size BATCH_SIZE
  -e EPOCHS, --epochs EPOCHS
```


原文链接：[https://github.com/codertimo/BERT-pytorch](https://link.zhihu.com/?target=https%3A//github.com/codertimo/BERT-pytorch)


