# 机器学习实战系列（四）：logistic回归 - Machine Learning with Peppa - CSDN博客





2018年07月08日 15:35:25[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：101








课程的所有数据和代码在我的Github：[Machine learning in Action](https://github.com/Y1ran/Machine-Learning-in-Action-Python3)，目前刚开始做，有不对的欢迎指正，也欢迎大家star。除了 版本差异，代码里的部分函数以及代码范式也和原书不一样（因为作者的代码实在让人看的别扭，我改过后看起来舒服多了）。在这个系列之后，我还会写一个scikit-learn机器学习系列，因为在实现了源码之后，带大家看看SKT框架如何使用也是非常重要的。    

## logistic回归

假设我们现有一些数据点，我们用一条直线对这些点进行拟合，这个拟合的过程就称作回归。利用logistic回归进行分类的主要思想是：根据现有数据对分类边界线建立回归公式，以此进行分类。

我们知道，logistic回归主要是进行二分类预测，也即是对于0~1之间的概率值，当概率大于0.5预测为1，小于0.5预测为0.显然，我们不能不提到一个函数，即sigmoid=1/(1+exp(-inX)),该函数的曲线类似于一个s型，在x=0处，函数值为0.5。

![](https://img-blog.csdn.net/20180708153739280?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

于是，为了实现logistic分类器，我们可以在每个特征上都乘以一个回归系数，然后所有的相乘结果进行累加，将这个总结作为输入，输入到sigmoid函数中，从而得到一个大小为0~1之间的值，当该值大于0.5归类为1，否则归类为0，这样就完成了二分类的任务。所以logistic回归可以看成是一种概率估计。

![](https://img-blog.csdn.net/20180708153743238?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





## 基于最优化方法

sigmoid函数的输入记为z，即z=w0x0+w1x1+w2x2+...+wnxn，如果用向量表示即为z=wTx，它表示将这两个数值向量对应元素相乘然后累加起来。其中，向量x是分类器的输入数据，w即为我们要拟合的最佳参数，从而使分类器预测更加准确。也就是说，logistic回归最重要的是要找到最佳的拟合参数。





### 梯度上升法

梯度上升法(等同于我们熟知的梯度下降法，前者是寻找最大值，后者寻找最小值)，它的基本思想是：要找到某函数的最大值，最好的方法就是沿着该函数的梯度方向搜寻。如果函数为f，梯度记为D，a为步长，那么梯度上升法的迭代公式为：w：w+a*Dwf(w)。该公式停止的条件是迭代次数达到某个指定值或者算法达到某个允许的误差范围。我们熟知的梯度下降法迭代公式为：w：w-a*Dwf(w)



### 使用梯度上升法寻找最佳参数

假设有100个样本点，每个样本有两个特征：x1和x2.此外为方便考虑，我们额外添加一个x0=1，将线性函数z=wTx+b转为z=wTx(此时向量w和x的维度均价1).那么梯度上升法的伪代码如下：

```python
初始化每个回归系数为1
　　重复R次：
　　　　计算整个数据集梯度
　　　　使用alpha*gradient更新回归系数的向量
　　返回回归系数
```




## 计算logistic，梯度上升法

```python
from numpy import *

def loadData(filename):
    '''
    '''
    datamat = []; labelmat = []
    with open(filename) as fr:
        for line in fr.readlines():
            line_arr = line.strip().split()
            datamat.append([1.0, float(line_arr[0]), float(line_arr[1])])
            # jisuan x0, x1,x2. x0wei 1
            labelmat.append(int(line_arr[2]))
    return datamat, labelmat

def sigmoid(inp):
    return 1.0 / (1 + exp(-inp))

def Grad_descent(datamat, labels):
    '''
    
    '''
    data = mat(datamat)
    label = mat(labels).transpose()
    
    m, n = shape(datamat)
    alpha = 0.001; max_iter = 500
    weights = ones((n, 1))   # 
    for k in range(max_iter):
        # 
        z = dot(datamat, weights)
        y_pred = sigmoid(z)
        error = (label - y_pred)
        # grad(x) = (y - f(x)) * x'
        weights = weights + alpha * data.transpose() * error
    return weights
```



## 画出决策边界

```python
import matplotlib.pyplot as plt
from numpy import *

def plot_fit(data, labelMat, weights):
    dataArr = array(data)
    n = shape(dataArr)[0]
    
    x_cord1 = []; y_cord1 = []
    x_cord2 = []; y_cord2 = []
    for i in range(n):  
        if int(labelMat[i]) == 1:  
            x_cord1.append(dataArr[i,1]); y_cord1.append(dataArr[i,2])  
        else: x_cord2.append(dataArr[i,1]); y_cord2.append(dataArr[i,2])  
    
    fig = plt.figure()
    ax = fig.add_subplot(111)  
    ax.scatter(x_cord1, y_cord1, s = 30, c = 'red', marker='s')  
    ax.scatter(x_cord2, y_cord2, s = 30, c = 'green')  
    
    x = arange(-3.0, 3.0, 0.1)  
    y = ((-weights[0]- weights[1] * x)/weights[2]).transpose()
    ax.plot(x, y)  
    plt.xlabel('X1');  
    plt.ylabel('X2');  
    plt.show()
```

## 随机梯度上升

```python
def Stoch_gdescent(datamat, labels, num_iter = 150):
    '''
    基于样本集中的每个样本（随机抽取）进行迭代
    求出优化的参数，并在此基础上对alpha进行衰减
    '''
    m, n = shape(datamat)
    alpha = 0.01
    weights = ones(n)   #
    
    for j in range(num_iter):
        for i in range(m):
            #j << x时衰减效果受到影响，0.01则为了保存一定的速率
            alpha = 4 / (1.0 + j + i) + 0.01
            randidx = int(random.uniform(0,len(range(m))))
            
            z = sum(datamat[randidx] * weights)
            y_pred = sigmoid(z)
            error = float(labels[randidx]) - y_pred
            # grad(x) = (y - f(x)) * x'为迭代公式（梯度）
            weights = weights + (alpha * error) * array(datamat[randidx])
    return weights
```

## 逻辑分类函数

```python
def classifyVec(inp, weights):
    prob = sigmoid(sum(list(array(inp) * array(weights))))
    if prob > 0.5: return 1.0
    else: return 0.0
    
#logistic回归预测算法
def colicTest():
    # 打开训练数据集
    frTrain=open('horseColicTraining.txt')
    # 打开测试数据集
    frTest=open('horseColicTest.txt')

    trainingSet=[];trainingLabels=[]
    # 读取训练集文档的每一行
    for line in frTrain.readlines():
        # 对当前行进行特征分割
        currLine=line.strip().split()
        # 新建列表存储每个样本的特征向量
        lineArr=[]
        # 遍历每个样本的特征
        for i in range(21):
            # 将该样本的特征存入lineArr列表
            lineArr.append(float(currLine[i]))
        #将该样本标签存入标签列表
        trainingLabels.append(currLine[21])
        #将该样本的特征向量添加到数据集列表
        trainingSet.append(lineArr)
    #调用随机梯度上升法更新logistic回归的权值参数
    trainWeights=Stoch_gdescent(trainingSet,trainingLabels,500)
    #统计测试数据集预测错误样本数量和样本总数
    errorCount=0; numTestVec=0.0
    #遍历测试数据集的每个样本
    for line in frTest.readlines():
        #样本总数加1
        numTestVec+=1.0
        #对当前行进行处理，分割出各个特征及样本标签
        currLine=line.strip().split()
        #新建特征向量
        lineArr=[]
        #将各个特征构成特征向量
        for i in range(21):
            lineArr.append(float(currLine[i]))
        #利用分类预测函数对该样本进行预测，并与样本标签进行比较
        if(classifyVec(lineArr,trainWeights)!=currLine[21]):
            #如果预测错误，错误数加1
            errorCount+=1
    #计算测试集总的预测错误率
    errorRate=(float(errorCount)/numTestVec)
    #打印错误率大小
    print('the error rate of this test is: %f' % (errorRate))    
    #返回错误率
    return errorRate

#多次测试算法求取预测误差平均值
def multTest():
    #设置测试次数为10次，并统计错误率总和
    numTests=10;errorRateSum=0.0
    #每一次测试算法并统计错误率
    for k in range(numTests):
        errorRateSum+=colicTest()
    #打印出测试10次预测错误率平均值
    print('after %d iterations the average error rate is: %f' \
     % (numTests,errorRateSum / float(numTests)))
```

## 测试输出

```python
if __name__ == '__main__':
    data, label = loadData('testSet.txt')
    print(Stoch_gdescent(data, label))
    
    weights = Stoch_gdescent(data, label)
    plot_fit(data, label, weights)
    
    multTest()
```

```python
[15.77415187  1.52139781 -2.35522914]
```

![](https://img-blog.csdn.net/20180708153418138?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3FxXzM5NTIxNTU0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



```python
the error rate of this test is : 0.328358
the error rate of this test is : 0.373134
the error rate of this test is : 0.328358
the error rate of this test is : 0.298507
the error rate of this test is : 0.388060
the error rate of this test is : 0.313433
the error rate of this test is : 0.268657
the error rate of this test is : 0.253731
the error rate of this test is : 0.313433
the error rate of this test is : 0.283582
after 10 iterations the average error rete is : 0.314925
```





