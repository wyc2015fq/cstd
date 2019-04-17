# trian和val结果相差很大。 - Snoopy_Dream - CSDN博客





2018年12月15日 22:29:46[Snoopy_Dream](https://me.csdn.net/e01528)阅读数：112








我说的是如果你含有BN层的话，有些人选择把BN层去掉之后，发现trian和val相差不大了。

存在的问题： 对训练集 accuracy可达0.99 loss=1e-2 -3，然而验证集 accuracy 0.5，loss 很高，试了多个初始学习率（0.1 — 0.0001）都不行

解决上述问题： **采取warm up方法 ，对上述问题有点帮助,或者换成SN层，但是显存会增大很多。**

训练resnet，由于不finetune，很容易过拟合，paper《Deep Residual Learning for Image Recognition》中对cifar10的实验用了一个trick是** warm up（热身），就是先采用小的学习率（0.01）进行训练，**训练了400iterations之后将学习率调整至0.1开始正式训练。

一开始不以为然，我分别用了四个初始学习率 lr=0.1; 0.01; 0.001; 0.0001; 然后每1000个iterations就降低一次lr。然而这四种初始学习率都试过之后，发现，验证集的accuracy怎么都上不去，都是0.5~0.6之间，而训练集的可以到0.99。而resnet采用了batch normalization,在caffe中**，batch normalization 就有一个“坑”，就是use_global_stats 的设置问题。**training时候是关闭，testing的时候是要打开，deploy也是要打开的。 

对于训练集accuracy达0.99，验证集accuracy是0.5+的情况，我以为是batch normalization出问题，于是乎各种捣鼓BN去了，最后试了一下warm up，网络在验证集的loss才有所下降。







