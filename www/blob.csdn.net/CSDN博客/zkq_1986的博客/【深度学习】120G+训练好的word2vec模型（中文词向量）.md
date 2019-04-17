# 【深度学习】120G+训练好的word2vec模型（中文词向量） - zkq_1986的博客 - CSDN博客





2018年12月13日 16:14:00[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：356








很多人缺少大语料训练的word2vec模型，在此分享下使用268G+语料训练好的word2vec模型。

**训练语料**：
- 百度百科800w+条，26G+
- [搜狐新闻400w+条](https://www.jianshu.com/p/370d3e67a18f)，13G+
- 小说：229G+

![](https://upload-images.jianshu.io/upload_images/12267214-19cd31f19d4dd693.png?imageMogr2/auto-orient/strip%7CimageView2/2/w/629/format/webp)

image.png

**模型参数**：
- window=5
- min_count=10
- size=128
- hs=1
- negative=0
- iter=5
- ps：其它参数见gensim库，执行代码为：gensim.models.Word2Vec(sentence, window=5, min_count=10, size=128, workers=4,hs=1, negative=0, iter=5)

**其它相关**：
- 分词词典使用了130w+词典。分词代码：jieba.lcut(sentence)，默认使用了HMM识别新词；
- 剔除了所有非中文字符；
- 最终得到的词典大小为6115353；
- 模型格式有两种bin和model，使用方式：

a. bin模型：

```python
import gensim
model = gensim.models.KeyedVectors.load_word2vec_format('model.bin',binary=True)
print(model['love'])
```

b. model模式：model = gensim.models.Word2Vec.load(model_path)

文件压缩后大小与未压缩相近，因此未压缩。

**下载链接**：

链接:[https://pan.baidu.com/s/1ckkH_eT-WS4SN73Iq9Q_5A](https://pan.baidu.com/s/1ckkH_eT-WS4SN73Iq9Q_5A) 密码:9aza



转载自：[https://www.jianshu.com/p/ae5b45e96dbf](https://www.jianshu.com/p/ae5b45e96dbf)



