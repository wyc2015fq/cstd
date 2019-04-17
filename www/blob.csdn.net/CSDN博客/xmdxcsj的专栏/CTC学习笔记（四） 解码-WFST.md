# CTC学习笔记（四） 解码-WFST - xmdxcsj的专栏 - CSDN博客





2016年07月04日 22:19:16[xmucas](https://me.csdn.net/xmdxcsj)阅读数：5959
个人分类：[神经网络](https://blog.csdn.net/xmdxcsj/article/category/5855803)









类似于HCLG的wfst结构，[EESEN: END-TO-END SPEECH RECOGNITION USING DEEP RNN MODELS AND WFST-BASED DECODING](http://arxiv.org/pdf/1507.08240v3.pdf)文章提出了TLG的网络结构

## Grammar

![这里写图片描述](https://img-blog.csdn.net/20160704221701104)

## Lexicon

有两种形式，可以基于characters和phonemes。 
![这里写图片描述](https://img-blog.csdn.net/20160704221759589)
## Token

对应于传统的state，前后添加blank，而且状态存在自旋 
![这里写图片描述](https://img-blog.csdn.net/20160704221808105)
## 网络



$S=T\circ min(det(L\circ G))$

## 结论

相比于传统的hybrid方法，准确率差不多，解码速度有三倍以上的提升。原因在于，状态数从几千个降到了几十个，减小了网络复杂度。





