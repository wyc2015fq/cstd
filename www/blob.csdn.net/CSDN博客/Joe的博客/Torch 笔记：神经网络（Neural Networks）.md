# Torch 笔记：神经网络（Neural Networks） - Joe的博客 - CSDN博客





2017年03月08日 16:34:16[Joe-Han](https://me.csdn.net/u010089444)阅读数：1443








# **1. Sequential**

> 
require ‘nn’;


```
mlp = nn.Sequential()
mlp:add(nn.Linear(10, 25)) -- Linear module (10 inputs, 25 hidden units)
mlp:add(nn.Tanh())         -- apply hyperbolic tangent transfer function on each hidden units
mlp:add(nn.Linear(25, 1))  -- Linear module (25 inputs, 1 output)

> mlp
nn.Sequential {
  [input -> (1) -> (2) -> (3) -> output]
  (1): nn.Linear(10 -> 25)
  (2): nn.Tanh
  (3): nn.Linear(25 -> 1)
}

> print(mlp:forward(torch.randn(10)))
-0.1815
[torch.Tensor of dimension 1]
```


![图片名称](https://img-blog.csdn.net/20170308150840767?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


**插入网络层**

```
model = nn.Sequential()
model:add(nn.Linear(10, 20))
model:add(nn.Linear(20, 30))
model:insert(nn.Linear(20, 20), 2)

> model
nn.Sequential {
  [input -> (1) -> (2) -> (3) -> output]
  (1): nn.Linear(10 -> 20)
  (2): nn.Linear(20 -> 20)      -- The inserted layer
  (3): nn.Linear(20 -> 30)
}
```

**删除网络层**

```
model = nn.Sequential()
model:add(nn.Linear(10, 20))
model:add(nn.Linear(20, 20))
model:add(nn.Linear(20, 30))
model:remove(2)
> model
nn.Sequential {
  [input -> (1) -> (2) -> output]
  (1): nn.Linear(10 -> 20)
  (2): nn.Linear(20 -> 30)
}
```

# **2. Parallel**

> 
module = Parallel( inputDimension, outputDimension )


```
mlp = nn.Parallel(2,1);   -- Parallel container will associate a module to each slice of dimension 2
                           -- (column space), and concatenate the outputs over the 1st dimension.

mlp:add(nn.Linear(10,3)); -- Linear module (input 10, output 3), applied on 1st slice of dimension 2
mlp:add(nn.Linear(10,2))  -- Linear module (input 10, output 2), applied on 2nd slice of dimension 2

                                  -- After going through the Linear module the outputs are
                                  -- concatenated along the unique dimension, to form 1D Tensor
> mlp:forward(torch.randn(10,2)) -- of size 5.
-0.5300
-1.1015
 0.7764
 0.2819
-0.6026
[torch.Tensor of dimension 5]
```


![图片名称](https://img-blog.csdn.net/20170308151044659?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# **3. Concat**

> 
module = nn.Concat( dim )


```
mlp = nn.Concat(1);
mlp:add(nn.Linear(5,3))
mlp:add(nn.Linear(5,7))

> print(mlp:forward(torch.randn(5)))
 0.7486
 0.1349
 0.7924
-0.0371
-0.4794
 0.3044
-0.0835
-0.7928
 0.7856
-0.1815
[torch.Tensor of dimension 10]
```


![图片名称](https://img-blog.csdn.net/20170308151155847?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# **4. Convolutional Neural Network**

```
net = nn.Sequential()
net:add(nn.SpatialConvolution(1, 6, 5, 5)) -- 1 input image channel, 6 output channels, 5x5 convolution kernel
net:add(nn.ReLU())                       -- non-linearity 
net:add(nn.SpatialMaxPooling(2,2,2,2))     -- A max-pooling operation that looks at 2x2 windows and finds the max.
net:add(nn.SpatialConvolution(6, 16, 5, 5))
net:add(nn.ReLU())                       -- non-linearity 
net:add(nn.SpatialMaxPooling(2,2,2,2))
net:add(nn.View(16*5*5))                    -- reshapes from a 3D tensor of 16x5x5 into 1D tensor of 16*5*5
net:add(nn.Linear(16*5*5, 120))             -- fully connected layer (matrix multiplication between input and weights)
net:add(nn.ReLU())                       -- non-linearity 
net:add(nn.Linear(120, 84))
net:add(nn.ReLU())                       -- non-linearity 
net:add(nn.Linear(84, 10))                   -- 10 is the number of outputs of the network (in this case, 10 digits)
net:add(nn.LogSoftMax())                     -- converts the output to a log-probability. Useful for classification problems

print('Lenet5\n' .. net:__tostring());
```


![](https://img-blog.csdn.net/20170308152540385?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvdTAxMDA4OTQ0NA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)


# **5. Training**

## **Loss Function（Criterion）**
- 对于回归或二分类问题，可采用Mean Squared Error loss（MSE） ：
`criterion = nn.MSECriterion()`- 对于多分类问题或输出层为 Log-Softmax 的网络，可采用Negative Log-likelihood ：
`criterion = nn.ClassNLLCriterion() `
## **Stochastic Gradient Descent**

```
trainer = nn.StochasticGradient(mlp, criterion)
trainer.learningRate = 0.001
trainer.maxIteration = 5
trainer:train(trainset)
```

数据集有两点要求：首先dataset[index]返回第Index个数据；其次就是dataset:size()函数返回整个数据集的example的个数。

以上为一种简单的训练方式，更多的模型优化方式可参考[Optimization package](https://github.com/torch/optim/blob/master/README.md)

# **参考资料**
- [Deep Learning with Torch: the 60-minute blitz](https://github.com/soumith/cvpr2015/blob/master/Deep%20Learning%20with%20Torch.ipynb)
- [Package Documentation](http://torch.ch/docs/package-docs.html#_)
- [Understanding Natural Language with Deep Neural Networks Using Torch](https://devblogs.nvidia.com/parallelforall/understanding-natural-language-deep-neural-networks-using-torch/)
- [Torch7. Hello World, Neural Networks!](http://mdtux89.github.io/2015/12/11/torch-tutorial.html)



