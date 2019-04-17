# kaldi 神经网络 - xmdxcsj的专栏 - CSDN博客





2017年01月23日 19:50:58[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3241








## overview
|type|author|CPU/GPU|feature|
|----|----|----|----|
|nnet1|Karel|GPU单卡训练|使用pre-training，使用early stopping|
|nnet2|Dan|支持多GPU训练，以及多CPU多线程|使用固定epoch个数，最后几个epoch参数平均|
|nnet3|Dan|是nnet2的扩展|支持更多的网络类型（比如RNN/LSTM）|

Karel’s DNN和Dan’s DNN格式不兼容，可以使用egs/rm/s5/local/run_dnn_convert_nnet2.sh进行格式转化。

## nnet1

### 训练流程
- steps/nnet/make_fmllr_feats.sh 

获取特征
- steps/nnet/pretrain_dbn.sh 

rbm预训练
- steps/nnet/train.sh 

使用mini-batch SGD进行训练
- steps/nnet/train_mpe.sh 

sMBR序列区分性训练，学习率固定设为1e-5，训练3-5epoch；在第一个epoch以后重新产生lattice可以更快的收敛；计算准确率的时候排除silience帧。

## nnet2

### 训练流程
- initialization 

使用nnet-am-init读取配置文件
- training 

使用nnet-train-parallel训练，使用参数平均的并行训练方式，结合preconditioned SGD训练。
- final model combination 

将最后几个（比如20）iteration的模型按照一定比例combine得到最后的model。具体做法：每个模型的每个component的weight作为学习的对象，然后使用训练数据的子集，结合传统的目标函数，优化方法使用L-BFGS。
- mixing-up 

假设最后输出的目标个数为1483，而网络的softmax层节点的个数却为4000（大于输出个数），需要将softmax的输出进行group（一般大小设为1-6）作为最后的输出目标。
- shrinking和fixing 

p-norm网络不使用，一般用在tanh网络或者包含sigmoid激活函数的网络。详细介绍参考[这篇文章](http://www.danielpovey.com/files/2014_icassp_dnn.pdf)

shrinking是指对网络不通layer的参数进行scale，scale的参数由训练数据的非线性优化获得。因为一般scale的参数小于1，所以称之为shrinking。 

fixing目的是为了解决训练过程中神经元over-saturated问题：即神经元的导数值小于特定门限（比如0.1），此时需要减小weight和bias。
### 训练参数
- 
job number 

一般GPU设为4，CPU设为8或者16，因为GPU的训练速度比CPU快20%到50%。 

minibatch的大小跟-num-jobs-nnet有关，如果使用多线程（比如n个线程）更新参数的方式，那么minibatch size相当于变成了原来的n倍。 

学习率的设置跟-num-jobs-nnet有关，如果我们jobs变为原来的n倍，那么学习率也要变为原来的n倍。因为并行使用的是n个模型参数平均的方式。但是学习率不能设置过大，否者会引起训练的不稳定。- 
隐层数量 

一般tanh网络是2到5层，p-norm网络是2到4层，增加层数的时候一般保持节点数不变

- 节点数 

一般是512/1024/2048，一般1024的网络就比较大了，最多是2048。和训练数据量的增加成二次关系，比如数据量变为原来的10倍，节点数变为原来的2倍。
- 学习率 

小数据量（几个小时）的初始值和结束值分为设为0.04和0.004；数据量变大以后，学习率要调低。 

可以通过绘制目标函数和时间的关系图来判断学习率是否合适。如果学习率太大，一开始目标函数值提升很快，但是最终值缺不理想，或者发生震荡，目标函数值突然变得很差；如果学习率太小，需要花费很长的时间才能获得最优值。 

一般来说网络的最后两层参数学习的速度更快，可以通过–final-learning-rate-factor参数（比如0.5）使得最后两层学习率衰减。- minibatch size 

数值越大训练速度越快，但是数值过大会引起训练的不稳定性。一般设为2的倍数，多线程CPU设为128，GPU设为512.
- max-change 

训练的时候如果学习率设置太大，将会导致参数变化量过大，引起训练不稳定。该参数的设置为参数的变化量设定一个上限。当minibatch大小为512，max-change设为40，minibatch大小为128，max-change设为10，max-change和minibatch的大小成正比。
- epoch个数 

两个参数–num-epochs（一般15）和–num-epochs-extra（一般5）设置，从0到–num-epochs之间学习率会衰减，最后的–num-epochs-extra学习率保持不变。小数据量一般设置更多的epoch（20+5），大数据量设置更少的epoch。
- feature splice width 

对于MFCC+splice+LDA+MLLT+fMLLR这种经过特殊处理的特征，一般设为4，因为LDA+MLLT已经是基于spliced（3或者4）的特征了；对于原始的MFCC/fbank特征，一般设为5。 

如果数值设置的更大，对于帧准确率是有益的，但是对于语音识别却是有损的。或许是因为违反了HMM帧独立性的假设。
### preconditioned SGD

相比于传统的SGD使用标量作为学习率（即所有的参数使用的学习率都是一样的），preconditioned SGD使用矩阵形式的学习率，学习率矩阵每一个minibatch更新一次。这样做的出发点在于减小方差比较大的维度对应参数的学习率，以控制训练的不稳定性以及避免在某个方向上面参数变动太快。

### 异常处理

当训练发散时，降低学习率，或者减小minibatch的大小。

## reference

[http://kaldi-asr.org/doc/dnn.html](http://kaldi-asr.org/doc/dnn.html)








