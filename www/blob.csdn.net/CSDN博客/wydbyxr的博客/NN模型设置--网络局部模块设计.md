# NN模型设置--网络局部模块设计 - wydbyxr的博客 - CSDN博客
2018年12月04日 10:34:37[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：50
# 1）批量归一化和最近的SeLU非线性技术“解决”爆炸梯度问题的技术
# 2）ResNets具有明显较低的梯度，因此可以规避爆炸的梯度问题，从而能够对更深层的网络进行有效的训练
# 3）BN之后Relu
  BN的两个缺陷也很明显，因为是对mini-batch求统计信息，因此具有数据依赖性，数据的随机性会导致训练的不稳定，且batch=1时无法使用。而各种变体本质上就是在寻找Natural Gradient，在加速收敛的同时，保证模型的泛化能力。
  但一般开源框架中使用的是优化的BN。这样的修正使得minibatch很小甚至为1时的仍能发挥其作用，且即使在minibatch中的数据是非独立同分布的，也会因为这个修正而消除对训练集合的过拟合。
# 4）语义分割网络的训练
  使用FCN和dilate-conv。
  在我个人使用过程中，相对于FCN等带skip connection结构的网络，我更喜欢类似于Dilated Net这种桶状结构的网络，原因是带skip connection的网络由于需要normalize不同layer之间的activation, 比较难训练。
  Liu Wei有一篇专门分析这个layer之间normalization trick的论文（[http://www.cs.unc.edu/~wliu/papers/parsenet.pdf）。](http://www.cs.unc.edu/~wliu/papers/parsenet.pdf%EF%BC%89%E3%80%82)
# 5）关于sigmoid
  sigmoid函数对于那些真实输出在0~1之间的情况，很好；但若输出接近饱和或急剧变化，这种收缩会有些不利影响。
  使用：最好不要用 sigmoid，你可以试试 tanh，不过可以预期它的效果会比不上 ReLU 和 Maxout。还有，通常来说，很少会把各种激活函数串起来在一个网络中使用的。
