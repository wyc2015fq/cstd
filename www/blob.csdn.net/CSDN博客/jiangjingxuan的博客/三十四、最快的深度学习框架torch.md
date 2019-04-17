# 三十四、最快的深度学习框架torch - jiangjingxuan的博客 - CSDN博客





2017年01月25日 14:00:14[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：557
个人分类：[动手做聊天机器人](https://blog.csdn.net/jiangjingxuan/article/category/6694788)













![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_449_default_big.png)



上节介绍了lstm的简单实现，为了能进一步应用到中文自动问答，我们需要了解一些开源的实现，考虑到基于torch实现的lstm应用较多，我们来熟悉一下torch

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## torch是什么

torch就是诸多深度学习框架中的一种

业界有几大深度学习框架：1）tensorflow，谷歌主推，时下最火，小型试验和大型计算都可以，基于python，缺点是上手相对较难，速度一般；2）torch，facebook主推，用于小型试验，开源应用较多，基于lua，上手较快，网上文档较全，缺点是lua语言相对冷门；3）mxnet，大公司主推，主要用于大型计算，基于python和R，缺点是网上开源项目较少；4）caffe，大公司主推，用于大型计算，基于c++、python，缺点是开发不是很方便；5）theano，速度一般，基于python，评价很好



## 为什么使用torch

只是因为github上lstm的实现项目比较多，完全为了学习用，个人不觉得语言和框架有好坏之分，各有千秋，谁行谁上



## 在mac上安装torch

如果你使用的是其他操作系统，可以参考[https://github.com/torch/torch7/wiki/Cheatsheet#installing-and-running-torch](https://github.com/torch/torch7/wiki/Cheatsheet#installing-and-running-torch)

执行

```php
git clone https://github.com/torch/distro.git ~/torch --recursive
cd ~/torch; bash install-deps;
./install.sh
```

可能会遇到qt安装不成功的问题，那么可以自己单独安装，如下：
`brew install cartr/qt4/qt`
安装后需要手工把下面这句加到~/.bash_profile中
`. ~/torch/install/bin/torch-activate`
这里的路径就是你安装的路径

source ~/.bash_profile之后就可以执行th使用torch了，如下：

```
[root@centos $] th

  ______             __   |  Torch7
 /_  __/__  ________/ /   |  Scientific computing for Lua.
  / / / _ \/ __/ __/ _ \  |  Type ? for help
 /_/  \___/_/  \__/_//_/  |  https://github.com/torch
                          |  http://torch.ch

th>
```

为了方便调试，我们还要安装itorch，首先安装依赖

```
brew install zeromq
brew install openssl
luarocks install luacrypto OPENSSL_DIR=/usr/local/opt/openssl/
```

然后按如下安装

```php
git clone https://github.com/facebook/iTorch.git
cd iTorch
luarocks make
```



## 用卷积神经网络实现图像识别

创建pattern_recognition.lua，内容如下：

```
require 'nn'
require 'paths'
if (not paths.filep("cifar10torchsmall.zip")) then
    os.execute('wget -c https://s3.amazonaws.com/torch7/data/cifar10torchsmall.zip')
    os.execute('unzip cifar10torchsmall.zip')
end
trainset = torch.load('cifar10-train.t7')
testset = torch.load('cifar10-test.t7')
classes = {'airplane', 'automobile', 'bird', 'cat',
'deer', 'dog', 'frog', 'horse', 'ship', 'truck'}
setmetatable(trainset,
{__index = function(t, i)
    return {t.data[i], t.label[i]}
end}
);
trainset.data = trainset.data:double() -- convert the data from a ByteTensor to a DoubleTensor.

function trainset:size()
    return self.data:size(1)
end
mean = {} -- store the mean, to normalize the test set in the future
stdv  = {} -- store the standard-deviation for the future
for i=1,3 do -- over each image channel
    mean[i] = trainset.data[{ {}, {i}, {}, {}  }]:mean() -- mean estimation
    print('Channel ' .. i .. ', Mean: ' .. mean[i])
    trainset.data[{ {}, {i}, {}, {}  }]:add(-mean[i]) -- mean subtraction

    stdv[i] = trainset.data[{ {}, {i}, {}, {}  }]:std() -- std estimation
    print('Channel ' .. i .. ', Standard Deviation: ' .. stdv[i])
    trainset.data[{ {}, {i}, {}, {}  }]:div(stdv[i]) -- std scaling
end
net = nn.Sequential()
net:add(nn.SpatialConvolution(3, 6, 5, 5)) -- 3 input image channels, 6 output channels, 5x5 convolution kernel
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
criterion = nn.ClassNLLCriterion()
trainer = nn.StochasticGradient(net, criterion)
trainer.learningRate = 0.001
trainer.maxIteration = 5
trainer:train(trainset)
testset.data = testset.data:double()   -- convert from Byte tensor to Double tensor
for i=1,3 do -- over each image channel
    testset.data[{ {}, {i}, {}, {}  }]:add(-mean[i]) -- mean subtraction
    testset.data[{ {}, {i}, {}, {}  }]:div(stdv[i]) -- std scaling
end
predicted = net:forward(testset.data[100])
print(classes[testset.label[100]])
print(predicted:exp())
for i=1,predicted:size(1) do
    print(classes[i], predicted[i])
end
correct = 0
for i=1,10000 do
    local groundtruth = testset.label[i]
    local prediction = net:forward(testset.data[i])
    local confidences, indices = torch.sort(prediction, true)  -- true means sort in descending order
    if groundtruth == indices[1] then
        correct = correct + 1
    end
end

print(correct, 100*correct/10000 .. ' % ')
class_performance = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
for i=1,10000 do
    local groundtruth = testset.label[i]
    local prediction = net:forward(testset.data[i])
    local confidences, indices = torch.sort(prediction, true)  -- true means sort in descending order
    if groundtruth == indices[1] then
        class_performance[groundtruth] = class_performance[groundtruth] + 1
    end
end

for i=1,#classes do
    print(classes[i], 100*class_performance[i]/1000 .. ' %')
end
```



请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

执行th pattern_recognition.lua，效果如下：

```
Channel 1, Mean: 125.83175029297
Channel 1, Standard Deviation: 63.143400842609
Channel 2, Mean: 123.26066621094
Channel 2, Standard Deviation: 62.369209019002
Channel 3, Mean: 114.03068681641
Channel 3, Standard Deviation: 66.965808411114
# StochasticGradient: training
# current error = 2.1494370275157
# current error = 1.8157547050302
# current error = 1.6500230258568
# current error = 1.5525866306324
# current error = 1.4768901429074
# StochasticGradient: you have reached the maximum number of iterations
# training error = 1.4768901429074
horse
 0.0150
 0.0034
 0.0350
 0.0490
 0.0294
 0.0942
 0.0161
 0.6958
 0.0030
 0.0591
[torch.DoubleTensor of size 10]

airplane	0.014967687525305
automobile	0.0034308404388647
bird	0.034966173679979
cat	0.04902915779719
deer	0.029421766137696
dog	0.094170890240524
frog	0.016105971372693
horse	0.69584966710414
ship	0.0029695320731486
truck	0.059088313630455
4519	45.19 %
airplane	61.3 %
automobile	40.6 %
bird	30.5 %
cat	22 %
deer	16.3 %
dog	47.4 %
frog	62 %
horse	61.4 %
ship	41.1 %
truck	69.3 %
```





## 解释一下

首先是下载cifar10torchsmall.zip样本，里面有50000张训练用的图片，10000张测试用的图片，分别都标注了是什么内容，包括airplane、automobile等10种分类，然后对trainset绑定__index和size方法，以兼容nn.Sequential的使用，有关绑定函数的内容可以看这个15分钟lua教程：[http://tylerneylon.com/a/learn-lua/](http://tylerneylon.com/a/learn-lua/),然后对trainset数据做正规化，把数据都转成均值为1方差为1的double类型的张量

然后就是初始化卷积神经网络模型，这个神经网络模型包括了了两层卷积、两层池化、一个全连接以及一个softmax层，接着进行训练，学习率为0.001，迭代5次，模型训练好后对测试机中的第100号图片做预测，然后就是打印出整体的正确率以及每种分类的准确率



也可以参考[https://github.com/soumith/cvpr2015/blob/master/Deep%20Learning%20with%20Torch.ipynb](https://github.com/soumith/cvpr2015/blob/master/Deep%20Learning%20with%20Torch.ipynb)



torch可以方便的支持gpu计算，如果使用gpu需要对代码做修改，这是与tensorflow相比的不足之处，后续就是继续学习基于torch有关lstm代码的内容了，有收获会随时发文




