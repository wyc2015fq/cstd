# 论文阅读：《 Lip Reading Sentences in the Wild》 - Soul Joy Hub - CSDN博客

2018年10月26日 17:02:57[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：250
个人分类：[深度学习																[自然语言处理																[计算机视觉](https://blog.csdn.net/u011239443/article/category/8257568)](https://blog.csdn.net/u011239443/article/category/7603940)](https://blog.csdn.net/u011239443/article/category/6990149)


论文：[https://arxiv.org/abs/1611.05358](https://arxiv.org/abs/1611.05358)

原文：[http://www.hankcs.com/nlp/cs224n-lip-reading.html](http://www.hankcs.com/nlp/cs224n-lip-reading.html)

## 唇语翻译

将视频处理为以嘴唇为中心的图片序列，给或不给语音，预测正在讲的话。

![hankcs.com 2017-06-30 下午3.16.19.png](http://wx4.sinaimg.cn/large/006Fmjmcly1fh392dhnmxj31du0okn12.jpg)

这些数据可能来自新闻直播：

![hankcs.com 2017-06-30 下午3.16.41.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fh3934jqslj31d40tmnpd.jpg)

动画演示：

![](http://wx1.sinaimg.cn/large/6cbb8645ly1fh3cs80if6g20ci057e8a.gif)

![](http://wx2.sinaimg.cn/large/6cbb8645ly1fh3cfc6k0eg20hs0a07wr.gif)

这里唇语和语音的识别、卡拉OK效果式的对齐，都是模型自动完成的。

## 架构

![hankcs.com 2017-06-30 下午3.40.00.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fh39r03yj7j31520kq41e.jpg)

视觉和听觉两个模块或者混合交火或者单独使用，每次输出一个字母。

### 视觉

取嘴唇时序上的sliding window，先喂给CNN，再喂给LSTM，生成一个output vector$s$：

![hankcs.com 2017-06-30 下午3.42.20.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fh39tj93trj31bs0u6ajx.jpg)

### 听觉

类似地，取音频上的窗口分片：

![hankcs.com 2017-06-30 下午3.44.03.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fh39v4e4dcj31di0potf7.jpg)

### Attention与Spell

将上述两个LSTM输出的output states送入一个带两个attention拓展的LSTM：

![hankcs.com 2017-06-30 下午3.48.34.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fh39zvt92oj31dq0oiak8.jpg)

### Curriculum Learning

![hankcs.com 2017-06-30 下午3.52.37.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fh3a44o2m2j31c20osad3.jpg)

通常训练seq2seq模型时喂进去的是完整的句子，但Curriculum Learning的手法是，一次只喂几个单词，逐步增长。这样可以加快收敛速度，降低过拟合。

### Scheduled Sampling

![hankcs.com 2017-06-30 下午5.03.54.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fh3c6bbaa6j31d00rkti4.jpg)

通常训练递归模型的时候，使用的是前一个时刻的“标准答案”的one-hot向量，而这里根据前一个时刻的预测结果采样，让训练和测试统一起来。

## 数据集

![hankcs.com 2017-06-30 下午5.06.20.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fh3c8tujp5j31dc0fykgi.jpg)

来自BBC新闻的五千个小时的视频，对齐字幕，做了嘴唇位置等预处理。

## 结果

![hankcs.com 2017-06-30 下午5.11.40.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fh3ced66x0j30se0oo7kb.jpg)

有趣之处在于，他们将模型效果与专业做唇语翻译的公司做了对比，发现比专业人士还要准，而且错误率低了20个百分点。（竟然还有公司专门做这个）

在同时输入语音和唇语的情况下，错误可以做到更低。

