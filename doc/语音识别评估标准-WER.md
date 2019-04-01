# 语音识别评估标准-WER

2017年02月24日 11:05:30

 

quheDiegooo

 

阅读数：9927

 

标签： [语音识别](http://so.csdn.net/so/search/s.do?q=%E8%AF%AD%E9%9F%B3%E8%AF%86%E5%88%AB&t=blog)[标准](http://so.csdn.net/so/search/s.do?q=%E6%A0%87%E5%87%86&t=blog) 更多

个人分类： [声学模型训练](https://blog.csdn.net/quheDiegooo/article/category/6731970)[语音识别](https://blog.csdn.net/quheDiegooo/article/category/6693504)



在语音识别中，常用的评估标准为词错误率WER，

WER计算方式为：为了使识别出来的词序列和标准的词序列之间保持一致，需要进行替换，删除，或者插入某些词，

这些插入，替换，删除的词的总个数，除以标准的词序列中词的个数的百分比，即为WER，其计算公式如下所示：

![img](https://img-blog.csdn.net/20170224105020653?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

需要注意的是，因为有插入词，所以WER有可能大于100%，

例子：如下图所示：

第一行为标准的词序列，第二行为识别出来的词序列

![img](https://img-blog.csdn.net/20170224105225222?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

那么上述的WER计算如下所示：

增加词3个，替换的词6个，删除的词1个，那么WER为：

![img](https://img-blog.csdn.net/20170224105511086?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

在语音识别中，除了WER，还有句子识别错误率这个标准，即SER，SER表述为句子中如果有一个词识别错误，那么这个句子被认为识别错误，句子识别错误的的个数，除以总的句子个数即为SER

其计算公式如下所示：

![img](https://img-blog.csdn.net/20170224105916266?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXVoZURpZWdvb28=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

除了WER，SER，还有字错误率，即CER，不是很常用就不介绍了。