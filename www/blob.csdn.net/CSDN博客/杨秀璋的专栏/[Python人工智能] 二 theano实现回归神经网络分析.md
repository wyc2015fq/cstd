# [Python人工智能] 二.theano实现回归神经网络分析 - 杨秀璋的专栏 - CSDN博客





2018年05月21日 15:14:46[Eastmount](https://me.csdn.net/Eastmount)阅读数：3362
所属专栏：[知识图谱、web数据挖掘及NLP](https://blog.csdn.net/column/details/eastmount-kgdmnlp.html)









从本篇文章开始，作者正式开始研究Python深度学习、神经网络及人工智能相关知识。前一篇文章主要讲解神经网络基础概念，同时讲解Theano库的安装过程及基础用法，这篇文章主要讲解theano实现回归神经网络，主要是学习"[莫烦大神](http://study.163.com/provider/1111519/course.htm)" 网易云视频的在线笔记，后面随着深入会讲解具体的项目及应用。基础性文章，希望对您有所帮助，也建议大家一步步跟着学习，同时文章中存在错误或不足之处，还请海涵~

["](http://study.163.com/provider/1111519/course.html)[莫烦大神](http://study.163.com/provider/1111519/course.htm)" 网易云视频地址：http://study.163.com/provider/1111519/course.html

同时推荐前面作者另外三个Python系列文章。
从2014年开始，作者主要写了三个Python系列文章，分别是基础知识、网络爬虫和数据分析。
- Python基础知识系列：[Pythonj基础知识学习与提升](http://blog.csdn.net/column/details/eastmount-python.html)
- Python网络爬虫系列：[Python爬虫之Selenium+Phantomjs+CasperJS](http://blog.csdn.net/column/details/eastmount-spider.html)
- Python数据分析系列：[知识图谱、web数据挖掘及NLP](http://blog.csdn.net/column/details/eastmount-kgdmnlp.html)

![这里写图片描述](https://img-blog.csdn.net/20180212160809122?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161506304?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)![这里写图片描述](https://img-blog.csdn.net/20180212161524640?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvRWFzdG1vdW50/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




## 一. 定义神经网络Layer类





如下图所示，通过该神经网络识别动物猫或狗，共包括输入层（Input Layer）、隐藏层3层（Hidden Layer）和输出层（Output Layer）。其中每一个神经元都有一个激励函数，被激励的神经元传递的信息最有价值，它也决定最后的输出结果，经过海量数据的训练，最终神经网络将可以用于识别猫或狗。


神经网络激励函数参考：
[http://deeplearning.net/software/theano/library/tensor/nnet/nnet.html](http://deeplearning.net/software/theano/library/tensor/nnet/nnet.html)
激励函数相当于一个过滤器或激励器，它把特有的信息或特征激活，常见的激活函数包括softplus、sigmoid、relu、softmax、elu、tanh等。对于隐藏层，我们可以使用relu、tanh、softplus等非线性关系；对于分类问题，我们可以使用sigmoid（值越小越接近于0，值越大越接近于1）、softmax函数，对每个类求概率，最后以最大的概率作为结果；对于回归问题，可以使用线性函数（linear function）来实验。








![](https://img-blog.csdn.net/20180518141209994)




神经网络首先需要添加神经层，将层（Layer）定义成类，通过类来添加神经层。神经层是相互链接，并且是全连接，从第一层输入层传入到隐藏层，最后传输至输出层。假设接下来需要定义两层内容：
**    L1 = Layer(inputs, in_size=1, out_size=10, activation_function)**参数包括输入值，输入节点数，输出节点数和激励函数
** L2 = Layer(L1.outputs, 10, 1, None)**
    参数中L1的输出作为输入值，L1的输出10个节点作为输入节点，输出节点1个，激励函数为None。

定义类的代码如下，包括权重和bias，其中参数为随机变量更有利于我们后面的更新，乱序更能促进神经网络的学习。

```python
class Layer(object):
    def __init__(self, inputs, in_size, out_size, activation_function=None):
        #权重: 平均值为0 方差为1 行数为in_size  列数为out_size
        self.W = theano.shared(np.random.normal(0,1,(in_size,out_size)))
        #bias
        self.b = theano.shared(np.zeros((out_size,) ) + 0.1)
        #乘法加bias
        self.Wx_plus_b = T.dot(inputs, self.W) + self.b #dot乘法
        #激励函数
        self.activation_function = activation_function

        #默认为None,否则进行激活
        if activation_function is None: 
            self.outputs = self.Wx_plus_b
        else: 
            self.outputs = self.activation_function(self.Wx_plus_b)
```








## 二. 回归神经网络实现

接下来开始跟着莫烦大声实现了第一个神经网络代码，步骤如下：

**1.制造虚拟数据**

通过numpy.linspace生成300个随机点进行训练，形成y=x^2-0.5的虚拟数据。代码如下：

```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

class Layer(object):
    def __init__(self, inputs, in_size, out_size, activation_function=None):
        #权重: 平均值为0 方差为1 行数为in_size  列数为out_size
        self.W = theano.shared(np.random.normal(0,1,(in_size,out_size)))
        #bias
        self.b = theano.shared(np.zeros((out_size,) ) + 0.1)
        #乘法加bias
        self.Wx_plus_b = T.dot(inputs, self.W) + self.b #dot乘法
        #激励函数
        self.activation_function = activation_function

        #默认为None,否则进行激活
        if activation_function is None: 
            self.outputs = self.Wx_plus_b
        else: 
            self.outputs = self.activation_function(self.Wx_plus_b)

            
#回归神经网络 Regression
            
#1.制造虚拟数据
import matplotlib.pyplot as plt
#Make up some fake data
x_data = np.linspace(-1,1,300)[:, np.newaxis] #300个点进行训练
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise   #y = x^2 - 0.5
#一元二次散点图
plt.scatter(x_data, y_data)
plt.show()
```

生产的一元二次随机散点图如下图所示：



![](https://img-blog.csdn.net/20180521143804374)
图1 散点图结果




**2.增加神经层**

定义输入x和y，注意其dtype类型为64位浮点型，整个代码前后类型需要保持一致。同时定义了L1层和L2层：
*L1 = Layer(x, 1, 10, T.nnet.relu)*
    输入为x，1维的data，神经元10个，relu非线性激励函数
*L2 = Layer(L1.outputs, 10, 1, None)*
    输入为L1输出值， L2的in_size为L1的神经元10，假设L2输出为最终output

```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

class Layer(object):
    def __init__(self, inputs, in_size, out_size, activation_function=None):
        #权重: 平均值为0 方差为1 行数为in_size  列数为out_size
        self.W = theano.shared(np.random.normal(0,1,(in_size,out_size)))
        #bias
        self.b = theano.shared(np.zeros((out_size,) ) + 0.1)
        #乘法加bias
        self.Wx_plus_b = T.dot(inputs, self.W) + self.b #dot乘法
        #激励函数
        self.activation_function = activation_function

        #默认为None,否则进行激活
        if activation_function is None: 
            self.outputs = self.Wx_plus_b
        else: 
            self.outputs = self.activation_function(self.Wx_plus_b)

            
#回归神经网络 Regression
            
#1.制造虚拟数据
import matplotlib.pyplot as plt
#Make up some fake data
x_data = np.linspace(-1,1,300)[:, np.newaxis] #300个点进行训练
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise   #y = x^2 - 0.5
#一元二次散点图
plt.scatter(x_data, y_data)
plt.show()

#2.定义输入和神经元
#determine the inputs dytpe
x = T.dmatrix('x')  #d代表64位float类型
y = T.dmatrix('y')
#add layers
L1 = Layer(x, 1, 10, T.nnet.relu) 
L2 = Layer(L1.outputs, 10, 1, None)
```

**3.计算误差与梯度下降更新**

定义cost变量计算误差，即预测值与真实值的差别；再定义梯度下降变量，其误差越大，降低趋势越大，通过梯度下降让预测值更接近真实值。代码中通过theano.function()函数更新神经网络的四个参数，计算公式如下啊：
L1.W, L1.W-learnging_rate*gW1：
(原始的权重-学习效率*下降幅度)并且更新为L1.W，通过该方法将L1.W、L1.b、L2.W、L2.b更新。


```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

class Layer(object):
    def __init__(self, inputs, in_size, out_size, activation_function=None):
        #权重: 平均值为0 方差为1 行数为in_size  列数为out_size
        self.W = theano.shared(np.random.normal(0,1,(in_size,out_size)))
        #bias
        self.b = theano.shared(np.zeros((out_size,) ) + 0.1)
        #乘法加bias
        self.Wx_plus_b = T.dot(inputs, self.W) + self.b #dot乘法
        #激励函数
        self.activation_function = activation_function

        #默认为None,否则进行激活
        if activation_function is None: 
            self.outputs = self.Wx_plus_b
        else: 
            self.outputs = self.activation_function(self.Wx_plus_b)

            
#回归神经网络 Regression
            
#1.制造虚拟数据
import matplotlib.pyplot as plt
#Make up some fake data
x_data = np.linspace(-1,1,300)[:, np.newaxis] #300个点进行训练
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise   #y = x^2 - 0.5
#一元二次散点图
plt.scatter(x_data, y_data)
plt.show()

#2.定义输入和神经元
#determine the inputs dytpe
x = T.dmatrix('x')  #d代表64位float类型
y = T.dmatrix('y')
#add layers
L1 = Layer(x, 1, 10, T.nnet.relu) 
L2 = Layer(L1.outputs, 10, 1, None)

#3.计算误差与梯度下降更新
#误差为预测值与真实值差别
cost = T.mean(T.square(L2.outputs - y)) #mean()求平均误差
#compute the gradients
#梯度下降，让预测值更接近真实值，误差越大，降低趋势越大
gW1, gb1, gW2, gb2 = T.grad(cost, [L1.W, L1.b, L2.W, L2.b]) #权重和bias
#apply gradient descent
#学习效率: 神经网络中learning_rate通常小于1的数字,0.05保证神经网络学习比较精细
learning_rate = 0.05

#更新四个神经网络的参数
train = theano.function(
    inputs = [x,y],
    outputs = cost,
    updates = [(L1.W, L1.W-learning_rate*gW1),
               (L1.b, L1.b-learning_rate*gb1),
               (L2.W, L2.W-learning_rate*gW2),
               (L2.b, L2.b-learning_rate*gb2)
               ] 
    )
#(L1.W, L1.W-learnging_rate*gW1)
#(原始的权重-学习的效率*下降的幅度)更新为L1.W
```

**4.预测结果**

最后是预测结果，训练时会给出x和y求cost，而预测时只给出输入x，用来做预测。最后每隔50步输出err，如果err不断减小，说明神经网络在学到东西，因为预测值与真实值误差在不断减小。

```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

class Layer(object):
    def __init__(self, inputs, in_size, out_size, activation_function=None):
        #权重: 平均值为0 方差为1 行数为in_size  列数为out_size
        self.W = theano.shared(np.random.normal(0,1,(in_size,out_size)))
        #bias
        self.b = theano.shared(np.zeros((out_size,) ) + 0.1)
        #乘法加bias
        self.Wx_plus_b = T.dot(inputs, self.W) + self.b #dot乘法
        #激励函数
        self.activation_function = activation_function

        #默认为None,否则进行激活
        if activation_function is None: 
            self.outputs = self.Wx_plus_b
        else: 
            self.outputs = self.activation_function(self.Wx_plus_b)

            
#回归神经网络 Regression
            
#1.制造虚拟数据
import matplotlib.pyplot as plt
#Make up some fake data
x_data = np.linspace(-1,1,300)[:, np.newaxis] #300个点进行训练
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise   #y = x^2 - 0.5
#一元二次散点图
plt.scatter(x_data, y_data)
plt.show()

#2.定义输入和神经元
#determine the inputs dytpe
x = T.dmatrix('x')  #d代表64位float类型
y = T.dmatrix('y')
#add layers
L1 = Layer(x, 1, 10, T.nnet.relu) 
L2 = Layer(L1.outputs, 10, 1, None)

#3.计算误差与梯度下降更新
#误差为预测值与真实值差别
cost = T.mean(T.square(L2.outputs - y)) #mean()求平均误差
#compute the gradients
#梯度下降，让预测值更接近真实值，误差越大，降低趋势越大
gW1, gb1, gW2, gb2 = T.grad(cost, [L1.W, L1.b, L2.W, L2.b]) #权重和bias
#apply gradient descent
#学习效率: 神经网络中learning_rate通常小于1的数字,0.05保证神经网络学习比较精细
learning_rate = 0.05

#更新四个神经网络的参数
train = theano.function(
    inputs = [x,y],
    outputs = cost,
    updates = [(L1.W, L1.W-learning_rate*gW1),
               (L1.b, L1.b-learning_rate*gb1),
               (L2.W, L2.W-learning_rate*gW2),
               (L2.b, L2.b-learning_rate*gb2)
               ] 
    )
#(L1.W, L1.W-learnging_rate*gW1)
#(原始的权重-学习的效率*下降的幅度)更新为L1.W

#4.预测结果
#训练时会给y求cost, 而预测输入只给出x用来做预测
predict = theano.function(inputs=[x],  outputs=L2.outputs)

for i in range(1000):
    #training 把x_data和y_data放到函数x、y中
    err = train(x_data, y_data) #误差

    #每隔50步输出err, 如果err不断减小说明神经网络在学到东西, 因为预测值与真实值误差在不断减小
    if i % 50 == 0: 
        print(err)
```

最后输出误差结果，可以看到在不断减小，通过不断更新权重和bias，神经网络在不断学习。

```python
2.079139917311914
0.019342171772016286
0.010080951605219858
0.007202397774306516
0.005702041299886045
0.004946926023254156
0.004565940080184372
0.0043433009094413985
0.00421325480276665
0.0041214880336559725
0.004046461715568916
0.003989685842213987
0.003934933552824453
0.003886586291155118
0.003843283475886867
0.0038068442317786316
0.0037721487639369986
0.0037432478192238835
0.003712756532212612
0.00368813308403329
```





## 三. 回归神经网络可视化

最后补充神经网络不断学习的拟合图形，从最早不合理的图形到后面基本拟合，同时cost误差也在不断减小，说明神经网络的真实值和预测值在不断更新接近，神经网络正常运行。结果如下：


![](https://img-blog.csdn.net/20180521145839195)
图2 第一次拟合结果

![](https://img-blog.csdn.net/20180521145910808)


图3 第二次拟合结果



![](https://img-blog.csdn.net/20180521150001980)


图4 最后一次拟合结果

完整代码及注释如下所示：

```python
#coding:utf-8
import numpy as np
import theano.tensor as T
import theano
from theano import function

class Layer(object):
    def __init__(self, inputs, in_size, out_size, activation_function=None):
        #权重: 平均值为0 方差为1 行数为in_size  列数为out_size
        self.W = theano.shared(np.random.normal(0,1,(in_size,out_size)))
        #bias
        self.b = theano.shared(np.zeros((out_size,) ) + 0.1)
        #乘法加bias
        self.Wx_plus_b = T.dot(inputs, self.W) + self.b #dot乘法
        #激励函数
        self.activation_function = activation_function

        #默认为None,否则进行激活
        if activation_function is None: 
            self.outputs = self.Wx_plus_b
        else: 
            self.outputs = self.activation_function(self.Wx_plus_b)

            
#回归神经网络 Regression
            
#1.制造虚拟数据
import matplotlib.pyplot as plt
#Make up some fake data
x_data = np.linspace(-1,1,300)[:, np.newaxis] #300个点进行训练
noise = np.random.normal(0, 0.05, x_data.shape)
y_data = np.square(x_data) - 0.5 + noise   #y = x^2 - 0.5
#一元二次散点图
plt.scatter(x_data, y_data)
plt.show()

#2.定义输入和神经元
#determine the inputs dytpe
x = T.dmatrix('x')  #d代表64位float类型
y = T.dmatrix('y')
#add layers
L1 = Layer(x, 1, 10, T.nnet.relu) 
L2 = Layer(L1.outputs, 10, 1, None)

#3.计算误差与梯度下降更新
#误差为预测值与真实值差别
cost = T.mean(T.square(L2.outputs - y)) #mean()求平均误差
#compute the gradients
#梯度下降，让预测值更接近真实值，误差越大，降低趋势越大
gW1, gb1, gW2, gb2 = T.grad(cost, [L1.W, L1.b, L2.W, L2.b]) #权重和bias
#apply gradient descent
#学习效率: 神经网络中learning_rate通常小于1的数字,0.05保证神经网络学习比较精细
learning_rate = 0.05

#更新四个神经网络的参数
train = theano.function(
    inputs = [x,y],
    outputs = cost,
    updates = [(L1.W, L1.W-learning_rate*gW1),
               (L1.b, L1.b-learning_rate*gb1),
               (L2.W, L2.W-learning_rate*gW2),
               (L2.b, L2.b-learning_rate*gb2)
               ] 
    )
#(L1.W, L1.W-learnging_rate*gW1)
#(原始的权重-学习的效率*下降的幅度)更新为L1.W

#4.预测结果
#训练时会给y求cost, 而预测输入只给出x用来做预测
predict = theano.function(inputs=[x],  outputs=L2.outputs)


#可视化分析

#plot the fake data
fig = plt.figure()
ax = fig.add_subplot(1,1,1)
ax.scatter(x_data,y_data) #散点图效果        

#输入红线并且连续不断更新
plt.ion()
plt.show() #参数block=True: 只显示这张图,关掉图后才运行后面程序

#每隔50步输出err并绘制拟合曲线
#cost值不断减小说明预测值与真实值误差在不断减小,拟合直线越来越接近
for i in range(1000):
    #误差: training 把x_data和y_data放到函数x、y中
    err = train(x_data, y_data)
    pre = predict(x_data)

    #to visualize the result and improvement
    #查看神经网络结果和拟合提升过程
    if i % 50 == 0: 

        #消除红线, 否则每次的红线绘制在一起
        #因为第一步没有红线, 故使用try忽略第一步
        try:
            ax.lines.remove(lines[0])
        except Exception:
            pass
        
        predicted_value = predict(x_data)
        #plot the prediction
        lines = ax.plot(x_data, predicted_value, 'r-', lw=5) #x和预测y lw线宽度
        plt.pause(1) #暂定1秒

        #输出误差
        print(err)
        #print(pre)
```

基础性文章，希望对您有所帮助，推荐大家阅读莫烦大神的学习视频，也建议大家一步步跟着学习，同时文章中存在错误或不足之处，还请海涵~
(By:Eastmount 2018-05-21 下午4点[http://blog.csdn.net/eastmount/](http://blog.csdn.net/eastmount/))









