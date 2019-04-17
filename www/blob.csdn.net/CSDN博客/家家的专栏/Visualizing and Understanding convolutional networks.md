# Visualizing and Understanding convolutional networks - 家家的专栏 - CSDN博客





2014年07月17日 21:38:45[依海之燕](https://me.csdn.net/yihaizhiyan)阅读数：3016








**Large convolutional networks model on ImageNet **

(Krizhevsky, A., Sutskever, I. and Hinton, G. E. ImageNet
 Classification with Deep Convolutional Neural Networks , Advances in Neural Information Processing 25, 2012)

Why they perform so well?

How they migh be improved?




为什么CNN 在imagenet上取得如何显著的结果？可以归结为下面三个当面：

1、**larger training sets;**  millions of labeled examples. 

2、**GPU **makes practical

3、Better model regularization strategies: **Dropout**



输入：2D images;  输出：a probability vector over the C different classes.

**Convolutional layers** 包含：

(1) **Convolution **of the previous layer output with a set of learned filters

(2) Passing the **responses **through a rectified linear function (relu(x) = max(x,0));   Ensure the feature maps are always positive.

(3) **Max-pooling** over local neighborhoods (optionally) 

(4) **Normalize **the responses across feature maps.(optionally) 

Top few layers of NN: **Fully-connected networks**. 

**Final layer:** a softmax classifier.




Visualization with a Deconvnet *by mapping these activities back to the input pixel space.*






