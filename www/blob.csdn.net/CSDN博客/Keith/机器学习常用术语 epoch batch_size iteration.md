# 机器学习常用术语: epoch batch_size iteration - Keith - CSDN博客





2017年03月14日 22:02:26[ke1th](https://me.csdn.net/u012436149)阅读数：3424







- 
batch size = **the number of training examples in one forward/backward pass.** The higher the batch size, the more memory space you’ll need.

- 
number of iterations = **number of passes**, each pass using [batch size] number of examples. To be clear, one pass = one forward pass + one backward pass (we do not count the forward pass and backward pass as two different passes).


When you train your neural network for example, or evaluate it, you do the feedforward not only with one sample but with a lot of them, that’s what we call a batch. You’re gonna feed forward your NN with all your batches. Using all your batches once is 1 epoch. If you have 10 epochs it mean that you’re gonna use all your data 10 times (split in batches).
- 
epoch: 所有的样本都进入NN计算一遍叫做1 epoch

- 
num of iterations: 计算了多少次 batch


## 参考资料

[https://www.quora.com/What-is-epochs-in-machine-learning](https://www.quora.com/What-is-epochs-in-machine-learning)
[http://stackoverflow.com/questions/4752626/epoch-vs-iteration-when-training-neural-networks](http://stackoverflow.com/questions/4752626/epoch-vs-iteration-when-training-neural-networks)



