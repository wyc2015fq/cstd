# DNN结构构建：NAS网络结构搜索和强化学习、迁移学习 - wishchinYang的专栏 - CSDN博客
2018年05月14日 15:23:20[wishchin](https://me.csdn.net/wishchin)阅读数：1657
**前言**
        谷歌推出的NASNet架构，用于大规模图像分类和识别。NASNet架构特点是由两个AutoML设计的Layer组成——Normal Layer and Reduction Layer，这样的效果是不再需要相关专家用human knowledge来搭建卷积网络架构，直接用RNN把Hyperparameter计算出来，这样就实现了网络结构自动学习。
        论文：[Learning Transferable Architectures for Scalable](http://arxiv.org/pdf/1707.07012.pdf) Image Recognition 
**强化学习**
       论文:《Neural ，Architecture Search with Reinforcement Learning》。 网站链接（开源代码）： [https:// github .com / tensorflow /models](https://github.com/tensorflow/models)
        论文中的核心是：利用Reinforcement Learning（强化学习）机制训练一个RNN（循环神经网路）controller（控制器）去自动产生一个神经网络，无需人为手动设计网络，设计出的网络在相关有权威的数据集上都取得了很好的成绩。看完论文后发现没有硬件资源根本是无法使得训练跑起来的，有钱就是任性，论文用了800个GPU跑起来的。
        RNN在处理变长问题和生成变长式问题时得到广泛应用，RNN通过循环单元可以展开为一个多长度概率模型，把变长问题使用一个概率框架来表示。在进行模型生成时，可以使用同样的方法进行模型长度枚举和选择优化，生成变长模型或者可变模型。
        关于RNN模型：[DNN结构进化之RNN](https://blog.csdn.net/wishchin/article/details/47948923)，[DNN结构进化之LSTM](https://blog.csdn.net/wishchin/article/details/42425087)，[DNN结构进化之NTM/DNC](https://blog.csdn.net/wishchin/article/details/53084685)。用于处理变长问题，发展出RNN；用以解决RNN梯度消失问题，发展出LSTM；NTM模型抽象LSTM的cell内存，模型可以直接训练简短的生成模式，比如copy、循环、排序、NGM文法描述等简单内存操作模式。    
        强化学习用于结构搜索：controller控制器给出个action，action去环境中做出动作并得到一个结果result，最后将result作为反馈信号反馈给controller控制器，controller控制器根据反馈值进行修改，然后一直迭代这个过程直到到达目标。
![](https://img-blog.csdn.net/20180514145930188)
       论文的思想是：通过一个controllerRNN在搜索空间（search space）中得到一个网络结构（论文中称为child network），然后用这个网络结构在数据集上训练，在验证集上测试得到准确率R，再将这个**准确率回传给controller**，**controller继续优化得到另一个网络结构**，如此**反复进行**直到得到最佳的结果，整个过程称为Neural Architecture Search。后面讲详细介绍整个流程，本总结主要是围绕生成CNN来介绍，论文中也可以生成RNN）。
**模型生成过程**
       以卷积层的生成为例，使用RNN结构作为控制器用于生成conv层的超参数-conv个数、filter的高和宽、stride的高和宽等。下图为RNN的展开式结构，真正的RNN结构为一个recurrent单元。
![](https://img-blog.csdn.net/20180514150206925)
      控制一个RNN网络的unit的参数值，根据RNN生成变长串的一般法则，可以生成一个对应 结构的CNN网络描述。当然这个结构描述是变长的，长度和结构由RNN的参数决定，这样在一个可变CNN网络结构和RNN参数之间建立起一个函数映射。
      根据论文的描述，使用所谓的anchor方法，一个RNN网络综合Conv层、Pooling层、SoftMax等多个RNN单元，综合为一个大的RNN网络，作为控制器。
**反馈学习-训练RNN**
       文章描述联合RNN的优化方法和CNN的评价方法，使用CNN在测试集合上的表现作为CNN的评价，并把CNN的评价反传给RNN，用以训练RNN的结构和参数。 
       从整个过程可以看出，用以生成CNN的RNN结构的训练涉及到两部分，相对于一般使用数据直接训练RNN，多了一个CNN的自动生成，优化和测试，优化函数转化过程，直觉上需要极大的运算量，比训练通常RNN运算复杂度多几个数量级。
**论文描述细节**
      Training details: The controller RNN is a two-layer LSTM with 35 hidden units on each layer.I t is trained with the ADAM optimizer (Kingma & Ba, 2015) with a learning rate of 0.0006. Theweights of the controller are initialized uniformly between -0.08 and 0.08.
      For the distributed training ,we set the number of parameter server shards S to 20, the num be r of controller replicas K to100 and the number of child replicas m to 8, which means there are 800 networks being trained on 800 GPUs concurrently at any time.
**生成模型用于迁移学习**
       迁移学习即是把一个领域的框架复用到另一个领域，两种应用领域具有特定的结构相似性，模型复用之时可以稍作修改或者稍加训练即可使用。
![](https://img-blog.csdn.net/20180514154217301)
       Training deta.............................
