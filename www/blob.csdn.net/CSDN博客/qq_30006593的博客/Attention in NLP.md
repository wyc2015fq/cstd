# Attention in NLP - qq_30006593的博客 - CSDN博客





2017年09月24日 11:03:11[lshiwjx](https://me.csdn.net/qq_30006593)阅读数：118
个人分类：[文献](https://blog.csdn.net/qq_30006593/article/category/7186720)









1.      **Attention in NLP**（list some important）



1.1.        Sequence to s**equencelearning with neural networks **1



使用encoder-decoder模型，一个rnn以待翻译语句为输入，编码待翻译句子为中间语言，另一个rnn以上一次输出的结果为输入，解码中间语言为目标语言。

![](https://img-blog.csdn.net/20170924112333008?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcXFfMzAwMDY1OTM=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

1.2.        Neural machine translation byjoinly learning to align and translate2

在encoder-decoder模型中加入attention机制，attention由输入的隐状态和输出的隐状态的关系得到，与输入隐状态**点乘后得到输出的隐状态。**

文章使用双层rnn，可以分别记录前后单词。

![](https://img-blog.csdn.net/20170924112347563)




1.3.        Convolutional sequence tosequence learning3

只使用卷积来替代rnn，卷积的堆叠也可以使距离较远的词语产生联系，只不过需要在较高的层，操作数为n/k，n为单词距离，k为卷积核大小。

![](https://img-blog.csdn.net/20170924112351716)



1.4.        Attention is all you need4

  抛弃cnn与rnn，只是用attention机制，但实际上还是有很多cnn的影子。

其中的attention采用self attention，muliti head用于并行，类似于channel。

![](https://img-blog.csdn.net/20170924112355460)


  一个不错的对attention的定义：Q,K,V

![](https://img-blog.csdn.net/20170924112359155)





Reference：

1.     Sutskever,I., Vinyals, O. & Le, Q. V. Sequence to sequence learning with neuralnetworks. in*Advances in neural information processing systems* 3104–3112(2014).

2.     Bahdanau,D., Cho, K. & Bengio, Y. Neural machine translation by jointly learning toalign and translate.*ArXiv Prepr. ArXiv14090473* (2014).

3.     Gehring,J., Auli, M., Grangier, D., Yarats, D. & Dauphin, Y. N. ConvolutionalSequence to Sequence Learning.*ArXiv Prepr. ArXiv170503122* (2017).

4.     Vaswani,A. *et al.* Attention Is All You Need.*ArXiv Prepr. ArXiv170603762*(2017).



