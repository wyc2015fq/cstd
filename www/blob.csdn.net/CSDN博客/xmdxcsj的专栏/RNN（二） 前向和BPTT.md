# RNN（二） 前向和BPTT - xmdxcsj的专栏 - CSDN博客





2015年11月28日 23:26:12[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3772








# RNN（二） 前向和BPTT

标签（空格分隔）： RNN BPTT

## basic definition

To simply notation, the RNN here only contains one input layer, one hidden layer and one putput layer. Notations are listed below:
|neural layer|node|index|number|
|----|----|----|----|
|input layer|x(t)|i|N|
|previous hidden layer|s(t)|h|M|
|hidden layer|s(t-1)|j|M|
|output layer|y(t)|k|O|
|input->hidden|**V**(t)|i,j|N->M|
|previous hidden->hidden|**U**(t)|h,j|M->M|
|hidden->output|**W**(t)|j,k|M->O|

Besides, P is the total number of available training samples which are indexed by l

## forward

![RNN forward](http://f.cl.ly/items/3L2C1I0v380W3N2D1C0c/rnn-forward.PNG)

1. input->hidden 


$net_j(t)=\sum_{i}^Nx_i(t)v_{ji}+\sum_{h}^Ms_h(t-1)u_{jh}+\theta_j$


$s_j(t)=f(net_j(t))$

2. hidden->output 


$net_k(t)=\sum_{j}^Ms_j(t)w_{kj}+\theta_k$


$y_k(t)=g(net_k(t))$
f and g are the activate functions of hidden layer and output layer respectively.

## backpropagation

### prerequisite

Any network structure can be trained with backpropagation when desired output patterns exist and each function that has been used to calculate the actual output patterns is differentiable.

### cost function

1.summed squared error(SSE) 

The cost function can be any differentiable function that is able to measure the loss of the predicted values from the gold answers. The SSE is frequently-used, and works well in the training of conventional feed-forward neural networks. 


$C=\frac{1}{2}\sum_l^P\sum_k^O(d_{lk}-y_{lk})^2$

2.cross extropy(CE) 

The cross-entropy loss is used in Recurrent Neural Network Language Models(RNNLM) and performs well. 


$C=-\sum_l^P\sum_k^Od_{lk}\ln y_{lk}+(1-d_{lk})\ln(1-y_{lk})$
Discussion below is based on SSE.

### error component
- error for output nodes 
![output node](http://f.cl.ly/items/1G0H3g0W022m2u17332q/bptt-output.PNG)


$\delta_{lk}=-\frac{\partial C}{\partial net_{lk}}=-\frac{\partial C}{\partial y_{lk}}\frac{\partial y_{lk}}{\partial net_{lk}}=(d_{lk}-y_{lk})g'(y_{lk})$- error for hidden nodes 
![hidden node](http://f.cl.ly/items/0F0H2Q3B3y1u0L20221E/bptt-hidden.PNG)


$\delta_{lj}=-(\sum_k^O\frac{\partial C}{\partial y_{lk}}\frac{\partial y_{lk}}{\partial net_{lk}}\frac{\partial net_{lk}}{\partial s_{lj}})\frac{\partial s_{lj}}{\partial net_{lj}}=\sum_k^O\delta_{lk}w_{kj}f'(net_{lj})$
### activate function
- sigmoid 


$f(net)=\frac{1}{1+e^{-net}}$


$f'(net)=f(net)\{1-f(net)\}$- softmax 


$g(net_k)=\frac{e^{net_k}}{\sum_k^Oe^{net_k}}$


$g'(net_k)=\frac{e^{net_k}(\sum_j^Oe^{net_j}-e^{net_k})}{{(\sum_j^Oe^{net_j}})^2}$
### gradient descent

According to the gradient descent, each weight change in the network should be proportional to the negative gradient of the cost function, with respect to the specic weight: 


$\Delta w=-\eta \frac{\partial C}{\partial w}$

where $\eta$ is the learning rate. 

1. hidden->output 


$\Delta w_{kj}=-\eta \frac{\partial C}{\partial w_{kj}}=\eta \sum_l^P(-\frac{\partial C}{\partial net_{lk}})\frac{\partial net_{lk}}{\partial w_{kj}}=\eta \sum_l^P\delta_{lk}\frac{\partial net_{lk}}{\partial w_{kj}}=\eta \sum_l^P\delta_{lk}s_{lj}$

2. input->hidden 


$\Delta v_{ji}=-\eta \frac{\partial C}{\partial v_{ji}}=\eta \sum_l^P\delta_{lj}x_{li}$

3. previous hidden->hidden 


$\Delta u_{jh}=-\eta \frac{\partial C}{\partial u_{jh}}=\eta \sum_l^P\delta_{lj}s_{(l-1)h}$
### unfolding

In a recurrent neural network, errors can be propagated further, i.e. more than 2 layers, in order to capture longer history information. This process is usually called unfolding. 

In an unfolded RNN, the recurrent weight is duplicated spatially for an arbitrary number of time steps, here refered to as T.  


$net_{lj}(t)=\sum_{i}^Nx_{li}(t)v_{ji}+\sum_{h}^Ms_{(l-1)h}u_{jh}+\theta_j$


$s_{(l-1)h}=f(net_{(l-1)h})$
![t-1 hidden node](http://f.cl.ly/items/423R243d0b421N1w3z0q/bptt-hidden(t-1).PNG)

Error for hidden nodes through time as: 


$\delta_{lj}(t-1)=-\frac{\partial C}{\partial net_{(l-1)j}}=-\sum_h^M\frac{\partial C}{\partial net_{lh}}\frac{\partial net_{lh}}{\partial net_{(l-1)j}}$


$=(-\sum_h^M\frac{\partial C}{\partial net_{lh}})(\frac{\partial net_{lh}}{\partial s_{(l-1)j}})(\frac{\partial s_{(l-1)j}}{\partial net_{(l-1)j}})$


$=\sum_h^M\delta_{lh}(t)u_{hj}f'(net_{(l-1)j})$

where h is the index for the hidden node at time step t, and j for the hidden node at time step t-1. 

此处原始论文使用的是$s_{lj}(t-1)$，个人感觉应该是$net_{lj}(t-1)$，但是这种表示方式又不好解释，因为$t$时刻对应的下标是$l$，$t-1$时刻对应的下标也是$l$，所以修改成了$net_{(l-1)j}$，认为$t$时刻对应的为$l$，$t-1$时刻对应的是$l-1$.
After all error deltas have been obtained, weights are folded back adding up to one big change for each unfolded weights. 

1. input->hidden 


$\Delta v_{ji}(t)=\eta \sum_z^T\sum_l^P\delta_{lj}(t-z)x_{(l-z)i}$

2. previous hidden->hidden 


$\Delta u_{jh}(t)=\eta \sum_z^T\sum_l^P\delta_{lj}(t-z)s_{(l-1-z)h}$
### summary
- input->hidden 


$v_{ji}(t+1)=v_{ji}(t)+\eta \sum_z^T\sum_l^P\delta_{lj}(t-z)x_{(l-z)i}$
- previous hidden->hidden 


$u_{jh}(t+1)=u_{jh}(t)+\eta \sum_z^T\sum_l^P\delta_{lj}(t-z)s_{(l-1-z)h}$
- hidden->output 


$w_{kj}(t+1)=w_{kj}(t)+\eta \sum_l^P\delta_{lk}s_{lj}$

## references

[BackPropagation Through Time](http://citeseerx.ist.psu.edu/viewdoc/download?doi=10.1.1.16.6652&rep=rep1&type=pdf)
[A guide to recurrent neural networks and backpropagation](http://ir.hit.edu.cn/~jguo/docs/notes/bptt.pdf)












