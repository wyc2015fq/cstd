# PyBrain-Python的人工神经网络 - lyx的专栏 - CSDN博客





2016年06月08日 19:56:37[钱塘小甲子](https://me.csdn.net/qtlyx)阅读数：8311








python有一个很好用的神经网络库。虽然sklearn很强大，但是好像对神经网络的支持并不是那么那好。

1.安装

安装很方便，项目是一个开源的，托管在github上。如果你用的也是Anconda平台，那么在Anconda Prompt的命令行中敲下


pip install git+https://github.com/pybrain/pybrain.git@0.3.3

等一会儿就会装好了。装好了是这样的：

![](https://img-blog.csdn.net/20160608193238796?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQv/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)

然后就能在ide里面import pybrain了。

2.建立神经网络的拓扑结构

有两种方式，一种比较简单，直接指明层的数目就可以了



```python
net = buildNetwork(4, 10, 1)
```
这样你就建立了一个输入层为4个节点，隐藏层为10个节点，一个输出节点的神经网络拓扑结构。

当时，个人还是建议使用复杂一点的神经网络建立方法。



```python
def netBuild(ds):
#    net = buildNetwork(4, 10, 1)

    # 建立神经网络fnn
    fnn = FeedForwardNetwork()

# 设立三层，一层输入层（3个神经元，别名为inLayer），一层隐藏层，一层输出层
    inLayer = LinearLayer(4, name='inLayer')
    hiddenLayer = SigmoidLayer(13, name='hiddenLayer0')
    outLayer = LinearLayer(1, name='outLayer')

# 将三层都加入神经网络（即加入神经元）
    fnn.addInputModule(inLayer)
    fnn.addModule(hiddenLayer)
    fnn.addOutputModule(outLayer)

# 建立三层之间的连接
    in_to_hidden = FullConnection(inLayer, hiddenLayer)
    hidden_to_out = FullConnection(hiddenLayer, outLayer)

# 将连接加入神经网络
    fnn.addConnection(in_to_hidden)
    fnn.addConnection(hidden_to_out)

# 让神经网络可用
    fnn.sortModules()
    
    print "Trainging"
    trainer = BackpropTrainer(net, ds,verbose = True, learningrate=0.01)
    #trainer.train()
    trainer.trainUntilConvergence(maxEpochs=100)
    print "Finish training"
    return fnn
```
上面的代码，我们写了一个函数，这个函数返回了fnn，也就是一个神经网络结构。在建立了fnn后，其实我们还做了一个工作，就是训练。首先我们是设置了个训练器traine，verbose=True这一条还是建议打开的，这样在训练的过程中我们能够看到当前训练的情况。learningrate则是学习率。

最后开启训练器，trainer.trainUntilConergence这个函数可以设置最大的训练次数。

这时候有同学就要问了，怎么没提到数据。训练的样本数据是这个函数的参数。

3.训练样本def readData(path):


```python
def readData(path):
    reader = csv.reader(file(path, 'rb'))
    data = []
    for line in reader:
        data.append((float(line[1]),float(line[2]),float(line[3]),float(line[4]),float(line[0])))
    return data
```
我们的神经网络四个输入一个输出的，所以对于学习样本就应该有五个记录。在我的数据中，有五个记录，也就是五列数据。第一列是结果，后面四列是输入。

这个函数，传入的参数path就是文件的路径。这里，文件的读取我们用了一点点tip。首先是




```python
reader = csv.reader(file(path, 'rb'))
```
这里，file（path，‘rb’）返回的是一个文件句柄，外面用csv流做一个包装，然后就可以很方便的读取了，源文件的每一行都变成了一个list，list中的元素就是被“，”分开的元素。



```python
def dsBuild(data):
    ds = SupervisedDataSet(4, 1)
    for ele in data:
        ds.addSample((ele[0],ele[1],ele[2],ele[3]), (ele[4]))
    dsTrain,dsTest = ds.splitWithProportion(0.8)
    return dsTrain,dsTest
```
有了这样的数据也不能直接放到神经网络里面去训练。PyBrain有自己的训练数据结构，当然，说白了就是ndarray。



```python
ds = SupervisedDataSet(4, 1)
```
这里定义了一个SupervisedData的数据结构，四个输入，一个输出。这个数据结构还有一个方法，能够给使用者随机按比例切割整个数据集，这个方法就是
```python
splitWithProportion()
```

传入的参数是分割数据集的比例。如0.8则表示2/8分割。


4.使用神经网络



```python
dsTrain,dsTest = dsBuild(readData(path))
netModel = netBuild(dsTrain)
dsTest = dsTrain
#pred=[]
for i in range(0,len(dsTest['input'])):
    error = dsTest['target'][i]-netModel.activate((dsTest['input'][i][0],dsTest['input'][i][1],dsTest['input'][i][2],dsTest['input'][i][3]))
    print error
```


最后把上面的函数用起来就可以了。模型训练完之后，只要用activate方法，传入输入的量，就可以得到结果。

5.更多的内容，可以去官网看哦。

http://www.pybrain.org/docs/index.html

不过似乎官网干货并不多。







