# BERT相关论文、文章和代码资源汇总

[![Pan Yang](https://pic2.zhimg.com/7acfe956b_xs.jpg)](https://www.zhihu.com/people/ainlp)

[Pan Yang](https://www.zhihu.com/people/ainlp)

我爱自然语言处理、课程图谱、NLPJob 保姆



BERT最近太火，蹭个热点，整理一下相关的资源，包括Paper, 代码和文章解读。

## 1、Google官方:

1) [BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.04805)

一切始于10月Google祭出的这篇Paper, 瞬间引爆整个AI圈包括自媒体圈: [https://arxiv.org/abs/1810.04805](http://link.zhihu.com/?target=https%3A//arxiv.org/abs/1810.04805)

2) Github: [https://github.com/google-research/bert](http://link.zhihu.com/?target=https%3A//github.com/google-research/bert)

11月Google推出了代码和预训练模型，再次引起群体亢奋。

3) Google AI Blog: [Open Sourcing BERT: State-of-the-Art Pre-training for Natural Language Processing](http://link.zhihu.com/?target=https%3A//ai.googleblog.com/2018/11/open-sourcing-bert-state-of-art-pre.html)

## 2、第三方解读：

1) 张俊林博士的解读， 知乎专栏：[从Word Embedding到Bert模型—自然语言处理中的预训练技术发展史](https://zhuanlan.zhihu.com/p/49271699)

我们在AINLP微信公众号上转载了这篇文章和张俊林博士分享的PPT，欢迎关注：

- [从Word Embedding到Bert模型—自然语言处理中的预训练技术发展史](http://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/p16IEzlaDGRNt8h6WkP-dQ)
- [预训练在自然语言处理的发展: 从Word Embedding到BERT模型](http://link.zhihu.com/?target=https%3A//mp.weixin.qq.com/s/LGJvvhotSg7XMn8mg3TZUw)

2) [知乎: 如何评价 BERT 模型？](https://www.zhihu.com/question/298203515)

3) [【NLP】Google BERT详解](https://zhuanlan.zhihu.com/p/46652512)

4) [[NLP自然语言处理\]谷歌BERT模型深度解析](http://link.zhihu.com/?target=https%3A//blog.csdn.net/qq_39521554/article/details/83062188)

5) [BERT Explained: State of the art language model for NLP](http://link.zhihu.com/?target=https%3A//towardsdatascience.com/bert-explained-state-of-the-art-language-model-for-nlp-f8b21a9b6270)

6) [BERT介绍](http://link.zhihu.com/?target=https%3A//blog.csdn.net/triplemeng/article/details/83053419)

7) [论文解读:BERT模型及fine-tuning](https://zhuanlan.zhihu.com/p/46833276)

8) [NLP突破性成果 BERT 模型详细解读](https://zhuanlan.zhihu.com/p/46997268)

## 3、第三方代码：

1) pytorch-pretrained-BERT: [https://github.com/huggingface/pytorch-pretrained-BERT](http://link.zhihu.com/?target=https%3A//github.com/huggingface/pytorch-pretrained-BERT)
Google官方推荐的PyTorch BERB版本实现，可加载Google预训练的模型：PyTorch version of Google AI's BERT model with script to load Google's pre-trained models

2) BERT-pytorch: [https://github.com/codertimo/BERT-pytorch](http://link.zhihu.com/?target=https%3A//github.com/codertimo/BERT-pytorch)
另一个Pytorch版本实现：Google AI 2018 BERT pytorch implementation

3) BERT-tensorflow: [https://github.com/guotong1988/BERT-tensorflow](http://link.zhihu.com/?target=https%3A//github.com/guotong1988/BERT-tensorflow)
Tensorflow版本：BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding

4) bert-chainer: [https://github.com/soskek/bert-chainer](http://link.zhihu.com/?target=https%3A//github.com/soskek/bert-chainer)
Chanier版本: Chainer implementation of "BERT: Pre-training of Deep Bidirectional Transformers for Language Understanding"

5) bert-as-service: [https://github.com/hanxiao/bert-as-service](http://link.zhihu.com/?target=https%3A//github.com/hanxiao/bert-as-service)
将不同长度的句子用BERT预训练模型编码，映射到一个固定长度的向量上：Mapping a variable-length sentence to a fixed-length vector using pretrained BERT model
这个很有意思，在这个基础上稍进一步是否可以做一个句子相似度计算服务？有没有同学一试？

6) bert_language_understanding: [https://github.com/brightmart/bert_language_understanding](http://link.zhihu.com/?target=https%3A//github.com/brightmart/bert_language_understanding)
BERT实战：Pre-training of Deep Bidirectional Transformers for Language Understanding: pre-train TextCNN

7) sentiment_analysis_fine_grain: [https://github.com/brightmart/sentiment_analysis_fine_grain](http://link.zhihu.com/?target=https%3A//github.com/brightmart/sentiment_analysis_fine_grain)
BERT实战，多标签文本分类，在 AI Challenger 2018 细粒度情感分析任务上的尝试：Multi-label Classification with BERT; Fine Grained Sentiment Analysis from AI challenger

8) BERT-NER: [https://github.com/kyzhouhzau/BERT-NER](http://link.zhihu.com/?target=https%3A//github.com/kyzhouhzau/BERT-NER)
BERT实战，命名实体识别: Use google BERT to do CoNLL-2003 NER !

9) BERT-keras: [https://github.com/Separius/BERT-keras](http://link.zhihu.com/?target=https%3A//github.com/Separius/BERT-keras)
Keras版: Keras implementation of BERT with pre-trained weights

10) tbert: [https://github.com/innodatalabs/tbert](http://link.zhihu.com/?target=https%3A//github.com/innodatalabs/tbert)
PyTorch port of BERT ML model

持续更新，BERT更多相关资源欢迎补充，欢迎关注我们的微信公众号：AINLP

