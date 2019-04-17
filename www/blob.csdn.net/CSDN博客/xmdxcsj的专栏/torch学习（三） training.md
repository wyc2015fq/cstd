# torch学习（三） training - xmdxcsj的专栏 - CSDN博客





2015年10月21日 23:37:38[xmucas](https://me.csdn.net/xmdxcsj)阅读数：1895








有两种方法进行模型训练，一种是使用封装的训练方法- StochasticGradient类 ；另外一种是使用for构建基本的训练流程 。

一、StochasticGradient方法

1.调用形式

StochasticGradient(module, criterion)

2.参数

内部的成员变量，跟训练有关

- `learningRate`: This is the learning rate used during training. The update of the parameters will be 
```
parameters
 = parameters - learningRate * parameters_gradient
```
. Default value is `0.01`.
- `learningRateDecay`: The learning rate decay. If non-zero, the learning rate (note: the field learningRate will not
 change value) will be computed after each iteration (pass over the dataset) with: 
```
current_learning_rate =learningRate / (1 + iteration
 * learningRateDecay)
```

- `maxIteration`: The maximum number of iteration (passes over the dataset). Default is `25`.
- `shuffleIndices`: Boolean which says if the examples will be randomly sampled or not. Default is `true`.
 If `false`, the examples will be taken in the order of the dataset.
- `hookExample`: A possible hook function which will be called (if non-nil) during training after each example forwarded
 and backwarded through the network. The function takes `(self, example)` as parameters. Default is `nil`.
- `hookIteration`: A possible hook function which will be called (if non-nil) during training after a complete pass over
 the dataset. The function takes `(self, iteration)` as parameters. Default is `nil`.


3.训练函数

train(dataset)

输入训练数据dataset需要实现 
dataset[index]操作符和 
dataset:size()方法

4.例子

训练数据


dataset={};
functiondataset:size() return100end-- 100 examplesfor i=1,dataset:size() do
local input = torch.randn(2);     -- normally distributed example in 2d
local output = torch.Tensor(1);
if input[1]*input[2]>0then-- calculate label for XOR function

    output[1] =-1;
else

    output[1] =1
end

  dataset[i] = {input, output}
end

构建网络结构

require"nn"

mlp = nn.Sequential(); 
-- make a multi-layer perceptron

inputs =2; outputs =1; HUs =20; -- parameters

mlp:add(nn.Linear(inputs, HUs))

mlp:add(nn.Tanh())

mlp:add(nn.Linear(HUs, outputs))


开始训练


criterion = nn.MSECriterion() 

trainer = nn.StochasticGradient(mlp, criterion)

trainer.learningRate=0.01

trainer:train(dataset)


测试网络


x = torch.Tensor(2)

x[1] =0.5; x[2] =0.5; print(mlp:forward(x))

x[1] =0.5; x[2] =-0.5; print(mlp:forward(x))

x[1] =-0.5; x[2] =0.5; print(mlp:forward(x))

x[1] =-0.5; x[2] =-0.5; print(mlp:forward(x))


二、人工训练网络

1.构建网络

require"nn"

mlp = nn.Sequential(); 
-- make a multi-layer perceptron

inputs =2; outputs =1; HUs =20; -- parameters

mlp:add(nn.Linear(inputs, HUs))

mlp:add(nn.Tanh())

mlp:add(nn.Linear(HUs, outputs))


2.定义准则


criterion = nn.MSECriterion() 


3.训练

for i =1,2500do
-- random sample
local input= torch.randn(2);     -- normally distributed example in 2d
local output= torch.Tensor(1);
if input[1]*input[2] >0then-- calculate label for XOR function

    output[1] =-1
else

    output[1] =1
end

-- feed it to the neural network and the criterion

  criterion:forward(mlp:forward(input), output)

-- train over this example in 3 steps
-- (1) zero the accumulation of the gradients

  mlp:zeroGradParameters()
-- (2) accumulate gradients

  mlp:backward(input, criterion:backward(mlp.output, output))
-- (3) update parameters with a 0.01 learning rate

  mlp:updateParameters(0.01)
end

4.测试网络


x = torch.Tensor(2)

x[1] =0.5; x[2] =0.5; print(mlp:forward(x))

x[1] =0.5; x[2] =-0.5; print(mlp:forward(x))

x[1] =-0.5; x[2] =0.5; print(mlp:forward(x))

x[1] =-0.5; x[2] =-0.5; print(mlp:forward(x))










