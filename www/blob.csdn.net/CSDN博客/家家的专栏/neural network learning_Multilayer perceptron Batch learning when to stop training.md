# neural network learning_Multilayer perceptron  Batch learning  when to stop training - 家家的专栏 - CSDN博客





2014年07月24日 10:53:25[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：850
个人分类：[菜鸟从零开始学习Deep learning](https://blog.csdn.net/yihaizhiyan/article/category/2388401)









**Multilayer perceptron** (**MLP**)
 is a **[feedforward](http://en.wikipedia.org/wiki/Feedforward_neural_network)**[artificial
 neural network](http://en.wikipedia.org/wiki/Artificial_neural_network) model that maps sets of input data onto a set of appropriate outputs. 

MLP utilizes a [supervised
 learning](http://en.wikipedia.org/wiki/Supervised_learning) technique called [**backpropagation**](http://en.wikipedia.org/wiki/Backpropagation)for
 training the network.




**Single-step learning** is faster

**Batch learning **yields lower residual error. Batch-learning
 is working with the true gradient. Thus, the residual error is often smaller than the single-step residual error. But, since
 one batch-learning step is only performed after a full set of training data is presented, the weight update frequency is rather slow.

**Combination of both**: Start with single step-learning to get a faster improvement, and later on switch to batch-learning to
 get better final result. 






The question of**when to stop training** is very complicated. Some of the possibilities are:
- Stop when the average error function for the training set becomes small.
- Stop when the gradient of the average error function for the training set becomes small.
- Stop when the average error function for the validation set starts to go up, and use the weights from the step that yielded the smallest validation error.
- Stop when your boredom level is no longer tolerable.





From:

http://www.researchgate.net/post/Which_one_is_better_between_online_and_offline_trained_neural_network


ftp://ftp.sas.com/pub/neural/FAQ2.html#A_functions


http://neuralnetworksanddeeplearning.com/chap1.html


http://en.wikipedia.org/wiki/Multilayer_perceptron




