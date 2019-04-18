# 论文阅读：《Google's Multilingual Neural Machine Translation System: Enabling Zero-Shot Translation》 - Soul Joy Hub - CSDN博客

2018年10月25日 19:37:35[卓寿杰_SoulJoy](https://me.csdn.net/u011239443)阅读数：318


论文：[https://arxiv.org/pdf/1611.04558.pdf](https://arxiv.org/pdf/1611.04558.pdf)

原文：[http://www.hankcs.com/nlp/cs224n-google-nmt.html](http://www.hankcs.com/nlp/cs224n-google-nmt.html)

## 双语NMT

一般“瘦弱”的NMT系统只支持双语单向翻译，比如课上常见的这种：

![hankcs.com 2017-06-27 下午4.54.35.png](http://wx1.sinaimg.cn/large/006Fmjmcly1fgzv2vf2zhj317o0u6qbq.jpg)

如果想实现一个模型支持多语种互译怎么办呢？

## 土办法

之前的尝试是使用多对encoder-decoder pair、一个encoder多个decoder、多个encoder一个decoder：

![hankcs.com 2017-06-27 下午4.59.24.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fgzv6rf19lj31e60tcn3t.jpg)

## Google的多语种NMT系统

![hankcs.com 2017-06-27 下午5.03.45.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgzvb9vz5lj31dq0t4n55.jpg)

谷歌的NMT系统有什么不同呢？
- 
单个统一的模型

- 
通过对其他语种的学习可以大幅提升语料匮乏的语种的翻译效果

- 
可以直接翻译训练语料中不存在的语种组合方式（通过葡萄牙语->芬兰语、芬兰语->英语学习葡萄牙语->英语的翻译）

### 架构

![hankcs.com 2017-06-27 下午5.07.36.png](http://wx3.sinaimg.cn/large/006Fmjmcly1fgzvfcvbuqj31cu0vcdse.jpg)

他们使用的架构与常见的encoder-decoder没有什么不同，其中attention会在接下来的课程中介绍。

聪明之处在于不修改网络架构，而是在输入数据上做手脚。

只需在输入平行语料pair中人工加入目标语种的标示符，就可以把所有语种的平行语料混合在一起训练了：

![hankcs.com 2017-06-27 下午5.09.49.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fgzvie5xvpj316k0qetfz.jpg)

真聪明。

### 效果

![hankcs.com 2017-06-27 下午5.11.44.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fgzvji03ebj31cs0iw0ve.jpg)

### Zero-Shot Translation

![hankcs.com 2017-06-27 下午5.16.13.png](http://wx2.sinaimg.cn/large/006Fmjmcly1fgzvo5bvklj31760vi7ht.jpg)

啰嗦一下Zero-Shot Translation的细节。

在训练的时候只使用葡萄牙语到英语、英语到西班牙语的语料，就可以让模型学会葡萄牙语到西班牙语的翻译。而且效果可以与phrase-based MT相比。如果再加一点葡萄牙语到西班牙语的语料，该系统就可以击败上述所有模型。

