# torch学习（六） rnn package - xmdxcsj的专栏 - CSDN博客





2015年11月30日 22:22:15[xmucas](https://me.csdn.net/xmdxcsj)阅读数：5046








一、安装

luarocks install moses

luarocks install fs

luarocks install dpnn

luarocks install rnn




rnn的地址：[](https://github.com/Element-Research/rnn)[](https://github.com/Element-Research/rnn)[https://github.com/Element-Research/rnn](https://github.com/Element-Research/rnn)

lstm：[https://github.com/wojzaremba/lstm.git](https://github.com/wojzaremba/lstm.git)

二、使用rnn训练语言模型

1.训练rnn

git clone [](https://github.com/Element-Research/rnn)[https://github.com/Element-Research/rnn](https://github.com/Element-Research/rnn)

到example目录下面执行：

recurrent-language-model.lua --cuda --useDevice 2 --progress --zeroFirst --cutoffNorm 4 --rho 10




下载的语料：~/data

结果和日志存在用户的~目录下面




三、RNN

（一）、AbstractRecurrent

Recurrent和LSTM的抽象类


rnn = nn.AbstractRecurrent(rho)

参数rho表示BPTT的最大步数。




（二）、Recurrent

1. 
nn.Recurrent(start, input, feedback, [transfer, rho, merge])

start：如果是size表示隐层大小，等鉴于nn.Add(start)；如果是Module，相当于在输入和transfer之间插入的层。

input：输入到隐层的weight矩阵

feedback；前一时刻隐层到当前时刻隐层的weight矩阵

transfer：非线性Module，隐层的输入到输出的激活函数

rho：BPTT的最大步数，由于vanishing gradient影响，一般设为5

merge：table Module，用于整合input和feedback的输出。

2.执行过程

对于一个输入sequence，一步一步的执行，forward保留每一步的input和Module.output；backward不会产生gradInput，只是保留每一步的gradOutput和scale；调用updateParameters和`backwardThroughTime`真正执行BPTT

例子：

require'rnn'


batchSize =8

rho =5

hiddenSize =10

nIndex =10000-- RNN

r = nn.Recurrent(

   hiddenSize, nn.LookupTable(nIndex, hiddenSize), 

   nn.Linear(hiddenSize, hiddenSize), nn.Sigmoid(), 

   rho

)


rnn = nn.Sequential()

rnn:add(r)

rnn:add(nn.Linear(hiddenSize, nIndex))

rnn:add(nn.LogSoftMax())


criterion = nn.ClassNLLCriterion()

-- dummy dataset (task is to predict next item, given previous)

sequence = torch.randperm(nIndex)


offsets = {}
for i=1,batchSize do
table.insert(offsets, math.ceil(math.random()*batchSize))
end

offsets = torch.LongTensor(offsets)


lr =0.1

updateInterval =4

i =1whiletruedo
-- a batch of inputs
local input = sequence:index(1, offsets)
local output = rnn:forward(input)
-- incement indices

   offsets:add(1)
for j=1,batchSize do
if offsets[j] > nIndex then

         offsets[j] =1
end
end
local target = sequence:index(1, offsets)
local err = criterion:forward(output, target)
print(err)
local gradOutput = criterion:backward(output, target)
-- the Recurrent layer is memorizing its gradOutputs (up to memSize)

   rnn:backward(input, gradOutput)


   i = i +1
-- note that updateInterval < rho
if i % updateInterval ==0then
-- backpropagates through time (BPTT) :
-- 1. backward through feedback and input layers,
-- 2. updates parameters

      rnn:backwardThroughTime()

      rnn:updateParameters(lr)

      rnn:zeroGradParameters()
endend

例子的几点注释：

-- offsets: sequece的下标

-- input: sequece下标对应的数值

-- batchSize: 一次前后向的个数

-- updateinterval: BPTT计算更新参数的间隔时间

-- rho: 保存t到t-rho个结果，updateinterval<rho（否则前面的中间结果都没有保存）

-- input: 比如输入nIndex=5，batchSize=3，input={2,4,1}，实际用向量表示如下：

--        (参考nn.Lookuptable的说明文档)

--        0 0 1

--        1 0 0

--        0 0 0

--        0 1 0

--        0 0 0

-- output:输出的对应于3*5的矩阵

3.使用Sequencer封装RNN

使用Sequence封装以后，就可以将t到t+updateInterval时间段内的序列使用一次forward和backward搞定（如果不封装的话，需要对每一个t进行一次前后向，优势在于可以进行实时预测）。

require'rnn'


batchSize =8

rho =5

hiddenSize =10

nIndex =10000-- RNN

r = nn.Recurrent(

   hiddenSize, nn.LookupTable(nIndex, hiddenSize), 

   nn.Linear(hiddenSize, hiddenSize), nn.Sigmoid(), 

   rho

)


rnn = nn.Sequential()

rnn:add(nn.Sequencer(r))

rnn:add(nn.Sequencer(nn.Linear(hiddenSize, nIndex)))

rnn:add(nn.Sequencer(nn.LogSoftMax()))


criterion = nn.SequencerCriterion(nn.ClassNLLCriterion())

-- dummy dataset (task is to predict next item, given previous)

sequence = torch.randperm(nIndex)


offsets = {}
for i=1,batchSize do
table.insert(offsets, math.ceil(math.random()*batchSize))
end

offsets = torch.LongTensor(offsets)


lr =0.1

i =1whiletruedo
-- prepare inputs and targets
local inputs, targets = {},{}
for step=1,rho do
-- a batch of inputs
table.insert(inputs, sequence:index(1, offsets))
-- incement indices

      offsets:add(1)
for j=1,batchSize do
if offsets[j] > nIndex then

            offsets[j] =1
end
end
-- a batch of targets
table.insert(targets, sequence:index(1, offsets))
end

local outputs = rnn:forward(inputs)
local err = criterion:forward(outputs, targets)
print(i, err/rho)

   i = i +1
local gradOutputs = criterion:backward(outputs, targets)

   rnn:backward(inputs, gradOutputs)

   rnn:updateParameters(lr)

   rnn:zeroGradParameters()
end




四、LSTM

1.概况

![](https://img-blog.csdn.net/20151130222329471?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)





i[t] = σ(W[x->i]x[t] + W[h->i]h[t−1] + W[c->i]c[t−1] + b[1->i]) 
     (1)

f[t] = σ(W[x->f]x[t] + W[h->f]h[t−1] + W[c->f]c[t−1] + b[1->f]) 
     (2)

z[t] =tanh(W[x->c]x[t] + W[h->c]h[t−1] + b[1->c]) 
                  (3)

c[t] = f[t]c[t−1] + i[t]z(t)                                         (4)

o[t] = σ(W[x->o]x[t] + W[h->o]h[t−1] + W[c->o]c[t] + b[1->o]) 
       (5)

h[t] = o[t]tanh(c[t])                                                (6)


2.实现

`nn.LSTM(inputSize, outputSize, [rho])`

成员函数包括

- `buildGate` : builds generic gate that is used to implement the input, forget and output gates;
- `buildInputGate` : builds the input gate (eq. 1). Currently calls `buildGate`;
- `buildForgetGate` : builds the forget gate (eq. 2). Currently calls `buildGate`;
- `buildHidden` : builds the hidden (eq. 3);
- `buildCell` : builds the cell (eq. 4);
- `buildOutputGate` : builds the output gate (eq. 5). Currently calls `buildGate`;
- `buildModel` : builds the actual LSTM model which is used internally (eq. 6).

五、 reference


https://github.com/Element-Research/rnn




