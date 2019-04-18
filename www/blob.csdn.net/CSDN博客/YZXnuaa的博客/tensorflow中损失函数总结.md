# tensorflow中损失函数总结 - YZXnuaa的博客 - CSDN博客
2018年03月07日 11:35:32[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：269
(1)sequence_loss_by_example(logits, targets, weights) 
这个函数用于计算所有examples的加权交叉熵损失，logits参数是一个2D Tensor构成的列表对象，每一个2D Tensor的尺寸为[batch_size x num_decoder_symbols]，函数的返回值是一个1D float类型的Tensor，尺寸为batch_size，其中的每一个元素代表当前输入序列example的交叉熵。另外，还有一个与之类似的函数sequence_loss，它对sequence_loss_by_example函数返回的结果进行了一个tf.reduce_sum运算，因此返回的是一个标称型float Tensor。（我所理解的是用于LSTM） 
[http://www.2cto.com/kf/201612/575911.html](http://www.2cto.com/kf/201612/575911.html)
(2)tf.nn.softmax_cross_entropy_with_logits 
[http://blog.csdn.net/mao_xiao_feng/article/details/53382790](http://blog.csdn.net/mao_xiao_feng/article/details/53382790)
[http://www.jianshu.com/p/fb119d0ff6a6](http://www.jianshu.com/p/fb119d0ff6a6)
[http://stackoverflow.com/questions/35241251/in-tensorflow-what-is-the-difference-between-sampled-softmax-loss-and-softmax-c](http://stackoverflow.com/questions/35241251/in-tensorflow-what-is-the-difference-between-sampled-softmax-loss-and-softmax-c)
