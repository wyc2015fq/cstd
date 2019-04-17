# torch学习（五） simply application - xmdxcsj的专栏 - CSDN博客





2015年10月19日 20:39:17[xmucas](https://me.csdn.net/xmdxcsj)阅读数：3274








一、supervised learning

（一）data

矩阵的操作

1.获得矩阵

t1 = torch.range(1,75):resize(3,5,5)

t2 = torch.range(1,25):resize(5,5)

2.使用[]取矩阵子集

t1[2]

3.重新赋值

t1[2]:fill(7)

4.使用[{}]slicing

t2[{ {},2 }] --第二列

t2[{ 2,{} }] --第二行数据，以列形式展示

t2[{ {2},{} }] --第二行数据，以行形式展示

t2[{ {1,3},{3,4} }] --第1-3行，第3-4列的矩阵

t2[{ {3},{4} }] --第3行第4列的数据

t2[{ 3,4 }] --第3行第4列的数据

t2[{ {},{2,-2} }] --第2列到倒数第2列

t2[{ -1,-1 }] --最后一行最后一列数据

4.使用[{}]复制tensors

t3 = torch.Tensor(5)

t3[{}] = t2[{ {},1 }]

t4 = torch.Tensor(5,2)

t4[{ {},1 }] = t2[{ {},2 }]

t4[{ {},2 }] = t2[{ {},5 }]

（二）model

1.logistic regression

   model = nn.Sequential()

   model:add(nn.Reshape(ninputs))

   model:add(nn.Linear(ninputs,noutputs))

2.MLP

   model = nn.Sequential()

   model:add(nn.Reshape(ninputs))

   model:add(nn.Linear(ninputs,nhiddens))

   model:add(nn.Tanh())

   model:add(nn.Linear(nhiddens,noutputs))

3.ConvNet

      model = nn.Sequential()


      -- stage 1 : filter bank -> squashing -> L2 pooling -> normalization

      model:add(nn.SpatialConvolutionMM(nfeats, nstates[1], filtsize, filtsize))

      model:add(nn.Tanh())

      model:add(nn.SpatialLPPooling(nstates[1],2,poolsize,poolsize,poolsize,poolsize))

      model:add(nn.SpatialSubtractiveNormalization(nstates[1], normkernel))


      -- stage 2 : filter bank -> squashing -> L2 pooling -> normalization

      model:add(nn.SpatialConvolutionMM(nstates[1], nstates[2], filtsize, filtsize))

      model:add(nn.Tanh())

      model:add(nn.SpatialLPPooling(nstates[2],2,poolsize,poolsize,poolsize,poolsize))

      model:add(nn.SpatialSubtractiveNormalization(nstates[2], normkernel))


      -- stage 3 : standard 2-layer neural network

      model:add(nn.Reshape(nstates[2]*filtsize*filtsize))

      model:add(nn.Linear(nstates[2]*filtsize*filtsize, nstates[3]))

      model:add(nn.Tanh())

      model:add(nn.Linear(nstates[3], noutputs))

（三）loss function

1.MSE（ 
 mean-square error）


criterion = nn.MSECriterion()

2. 
the negative log-likelihood


criterion = nn.ClassNLLCriterion()

3. 
multi-class margin loss


criterion = nn.MultiMarginCriterion()

（四）train

1.使用cuda

   model:cuda()

   criterion:cuda()

2.shuffle at each epoch

   shuffle = torch.randperm(trsize)

3.计算loss

   -- estimate f

   local output = model:forward(inputs[i])

   local err = criterion:forward(output, targets[i])

   -- estimate df/dW

   local df_do = criterion:backward(output, targets[i])

   model:backward(inputs[i], df_do)

4.optimize

   optimMethod(feval, parameters, optimState)

二、creating module


Module是神经网络的基本组成部分，作为一个抽象类，可以通过定义成员函数实现不同的神经网络结构。

(一)、forward(input)

1.调用updateOutput(input)

对module的输出进行处理

(二)、backward(input, gradOutput)

在使用backward之前需要首先调用forward

1.updateGradInput(input, gradOutput)

对gradOutput处理以后作为gradInput

2. accGradParameters(input, gradOutput)

计算该module对应的梯度

(三)、reset()

训练之前的初始化

local NewClass, Parent = torch.class('nn.NewClass', 'nn.Module')

function NewClass:__init()

   parent.__init(self)end

function NewClass:updateOutput(input)end

function NewClass:updateGradInput(input, gradOutput)end

function NewClass:accGradParameters(input, gradOutput)end

function NewClass:reset()end







[https://github.com/torch/nn/blob/master/doc/module.md#nn.Module](https://github.com/torch/nn/blob/master/doc/module.md#nn.Module)

[http://code.madbits.com/wiki/doku.php?id=tutorial_morestuff](http://code.madbits.com/wiki/doku.php?id=tutorial_morestuff)


http://code.madbits.com/wiki/doku.php?id=tutorial_supervised



