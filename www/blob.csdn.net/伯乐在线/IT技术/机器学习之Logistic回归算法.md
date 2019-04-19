# 机器学习之Logistic回归算法 - 文章 - 伯乐在线
原文出处： [Holy Null](http://holynull.leanote.com/post/Logistic-2)
# Logistic回归算法
本文介绍机器学习中的Logistic回归算法，我们使用这个算法来给数据进行分类。Logistic回归算法同样是需要通过样本空间学习的监督学习算法，并且适用于数值型和标称型数据，例如，我们需要根据输入数据的特征值（数值型）的大小来判断数据是某种分类或者不是某种分类。
## 一、样本数据
在我们的例子中，我们有这样一些样本数据：
- 样本数据有3个特征值：，，
- 我们通过这3个特征值中的和来判断数据是否符合要求，即符合要求的为1，不符合要求的为0。
- 样本数据分类存放在一个数组中
我们在logRegres.py文件中编写如下函数来准备数据，并将数据打印观察一下：


```
#coding=utf-8
from numpy import *
def loadDataSet():
    dataMat = []; labelMat = []
    fr = open('testSet.txt')
    for line in fr.readlines():
        lineArr = line.strip().split()
        dataMat.append([1.0, float(lineArr[0]), float(lineArr[1])])
        labelMat.append(int(lineArr[2]))
    return dataMat,labelMat
if __name__=='__main__':
    dataMat,labelMat=loadDataSet()
    print 'dataMat:\n',dataMat
```
我们来观察一下这个数据样本：


```
dataMat:
[[1.0, -0.017612, 14.053064], [1.0, -1.395634, 4.662541], [1.0, -0.752157, 6.53862], [1.0, -1.322371, 7.152853], [1.0, 0.423363, 11.054677], [1.0, 0.406704, 7.067335], [1.0, 0.667394, 12.741452], [1.0, -2.46015, 6.866805], [1.0, 0.569411, 9.548755], [1.0, -0.026632, 10.427743], [1.0, 0.850433, 6.920334], [1.0, 1.347183, 13.1755], [1.0, 1.176813, 3.16702], [1.0, -1.781871, 9.097953], [1.0, -0.566606, 5.749003], [1.0, 0.931635, 1.589505], [1.0, -0.024205, 6.151823], [1.0, -0.036453, 2.690988], [1.0, -0.196949, 0.444165], [1.0, 1.014459, 5.754399], [1.0, 1.985298, 3.230619], [1.0, -1.693453, -0.55754], [1.0, -0.576525, 11.778922], [1.0, -0.346811, -1.67873], [1.0, -2.124484, 2.672471], [1.0, 1.217916, 9.597015], [1.0, -0.733928, 9.098687], [1.0, -3.642001, -1.618087], [1.0, 0.315985, 3.523953], [1.0, 1.416614, 9.619232], [1.0, -0.386323, 3.989286], [1.0, 0.556921, 8.294984], [1.0, 1.224863, 11.58736], [1.0, -1.347803, -2.406051], [1.0, 1.196604, 4.951851], [1.0, 0.275221, 9.543647], [1.0, 0.470575, 9.332488], [1.0, -1.889567, 9.542662], [1.0, -1.527893, 12.150579], [1.0, -1.185247, 11.309318], [1.0, -0.445678, 3.297303], [1.0, 1.042222, 6.105155], [1.0, -0.618787, 10.320986], [1.0, 1.152083, 0.548467], [1.0, 0.828534, 2.676045], [1.0, -1.237728, 10.549033], [1.0, -0.683565, -2.166125], [1.0, 0.229456, 5.921938], [1.0, -0.959885, 11.555336], [1.0, 0.492911, 10.993324], [1.0, 0.184992, 8.721488], [1.0, -0.355715, 10.325976], [1.0, -0.397822, 8.058397], [1.0, 0.824839, 13.730343], [1.0, 1.507278, 5.027866], [1.0, 0.099671, 6.835839], [1.0, -0.344008, 10.717485], [1.0, 1.785928, 7.718645], [1.0, -0.918801, 11.560217], [1.0, -0.364009, 4.7473], [1.0, -0.841722, 4.119083], [1.0, 0.490426, 1.960539], [1.0, -0.007194, 9.075792], [1.0, 0.356107, 12.447863], [1.0, 0.342578, 12.281162], [1.0, -0.810823, -1.466018], [1.0, 2.530777, 6.476801], [1.0, 1.296683, 11.607559], [1.0, 0.475487, 12.040035], [1.0, -0.783277, 11.009725], [1.0, 0.074798, 11.02365], [1.0, -1.337472, 0.468339], [1.0, -0.102781, 13.763651], [1.0, -0.147324, 2.874846], [1.0, 0.518389, 9.887035], [1.0, 1.015399, 7.571882], [1.0, -1.658086, -0.027255], [1.0, 1.319944, 2.171228], [1.0, 2.056216, 5.019981], [1.0, -0.851633, 4.375691], [1.0, -1.510047, 6.061992], [1.0, -1.076637, -3.181888], [1.0, 1.821096, 10.28399], [1.0, 3.01015, 8.401766], [1.0, -1.099458, 1.688274], [1.0, -0.834872, -1.733869], [1.0, -0.846637, 3.849075], [1.0, 1.400102, 12.628781], [1.0, 1.752842, 5.468166], [1.0, 0.078557, 0.059736], [1.0, 0.089392, -0.7153], [1.0, 1.825662, 12.693808], [1.0, 0.197445, 9.744638], [1.0, 0.126117, 0.922311], [1.0, -0.679797, 1.22053], [1.0, 0.677983, 2.556666], [1.0, 0.761349, 10.693862], [1.0, -2.168791, 0.143632], [1.0, 1.38861, 9.341997], [1.0, 0.317029, 14.739025]]
labelMat:
[0, 1, 0, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 1, 1, 0, 1, 1, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 1, 0, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 1, 1, 1, 1, 0, 1, 0, 1, 1, 1, 1, 0, 1, 1, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0]
```
样本数据`dataMat`的第一列，也就是我们的特征值全部为1，这个问题我们之后在计算回归参数时需要注意理解。所有的样本数据一共100条，对应的分类结果也是100个。
**那么，我们现在的问题是：**
**我们要找到样本空间中的特征值与分类结果的关系。设计一个函数或者功能，实现在输入一组特征值后，能够根据样本空间特征值与分类结果的关系，自动为输入的数据进行分类，即得到结果要么是1，要么是0。**
## 二、Sigmoid函数
为了解决上一节我们提到的问题，我们这里先介绍一下Sigmoid函数：
![](http://wx4.sinaimg.cn/mw690/63918611gy1fm60lvcgxjj208p03cdfn.jpg)
这个函数有如下几个特征：
- 当时，值为
- 当不断增大时，值将趋近于1
- 当不断减小时，值将趋近于0
我们来看一下函数的曲线图：
![](http://wx4.sinaimg.cn/mw690/63918611gy1fm60ltmj50j20fu0ag0sv.jpg)
我们如果将样本空间的3个特征值、和的值代入到函数中，计算出一个结果。那么这个结果将是接近与我们的分类结果的（0到1之间的一个数值）。如果这个结果接近0那么我们就认为分类为0，如果结果接近1我们就认为分类为1。
以什么方式代入到函数中呢？其实简单的相加就可以，因为不断增大或者减小时，函数的值就相应的趋近于1或者0。我们使
![](http://wx3.sinaimg.cn/mw690/63918611gy1fm60lvuesej2089027dfn.jpg)
但是实际的情况是我们的计算结果和实际的分类值，会有误差，甚至是完全不正确。为了矫正这个问题，我们为样本空间的3个特征值、和，一一定义一个回归系数、和，使这个误差减小。即使
![](http://wx1.sinaimg.cn/mw690/63918611gy1fm60lw5fe7j20a402ajr8.jpg)
其实不难想象，这组回归系数的值决定了我们计算结果的准确性，甚至是正确性。也就是说，这组的值反应了样本空间分类的规则。
那么，我们在输入一组样本之外的数据时，配合正确的回归系数，我们就能得到比较接近样本空间分类规则的分类结果。
问题又来了，我们怎么来得到这样一组回归系数呢？
## 三、梯度上升法
梯度上升法，是在函数的梯度方向上，不断的迭代计算参数值，以找到一个最大的参数值。迭代公式如下：
![](http://wx3.sinaimg.cn/mw690/63918611gy1fm60lwk7ihj208o01iglf.jpg)
其中，为步长，为函数梯度。关于梯度的推导请参考[这里](http://blog.csdn.net/taotaofu/article/details/52831965)。作者的数学能力有限，就不做说明了。
最后，我们可以得到梯度的计算公式：
![](http://wx2.sinaimg.cn/mw690/63918611gy1fm60lwxeqdj20ae02c746.jpg)
那么，迭代公式如下：
![](http://wx3.sinaimg.cn/mw690/63918611gy1fm60lx83osj20b502ca9y.jpg)
公式说明：
- 为本次迭代特征项的回归系数结果
- 为上一次迭代特征项的回归系数结果
- 为每次迭代向梯度方向移动的步长
- 为特征项中第i个元素
- 是样本中第i条记录的分类样本结果
- 是样本中第i条记录，使用sigmoid函数和作为回归系数计算的分类结果
- 是样本第i条记录对应的分类结果值,与sigmoid函数使用作为回归系数计算的分类结果值的误差值。
现在，我们有了计算回归系数的公式，下面我们在logRegres.py文件中来实现一个函数，实现计算样本空间的回归系数，并打印一下我们的结果：


```
def gradAscent(dataMatIn, classLabels):
    dataMatrix = mat(dataMatIn)             #100行3列
    #print dataMatrix
    labelMat = mat(classLabels).transpose() #100行1列
    #print 'labelMat:\n',labelMat
    print 'labelMat 的形状:rowNum=',shape(labelMat)[0],'colNum=',shape(labelMat)[1]
    rowNum,colNum = shape(dataMatrix)
    alpha = 0.001
    maxCycles = 500
    weights = ones((colNum,1))  #3行1列
    #print shape(dataMatrix)
    #print shape(weights)
    #print shape(labelMat)
    for k in range(maxCycles):              #heavy on matrix operations
        h = sigmoid(dataMatrix*weights)     #100行1列
        #print h
        error = (labelMat - h)              #vector subtraction
        weights = weights + alpha * dataMatrix.transpose()* error #3行1列
    return weights
if __name__=='__main__':
    dataMat,labelMat=loadDataSet()
    #weights=gradAscent(dataMat,labelMat)
    #print 'dataMat:\n',dataMat
    #print 'labelMat:\n',labelMat
    print weights
```
打印结果:


```
回归系数：
[[ 4.12414349]
 [ 0.48007329]
 [-0.6168482 ]]
```
为了验证我们计算的回顾系数的准确性，我们观察一下样本空间的散点图和回归系数的拟合曲线。我们以作为我们的拟合函数，在坐标系中画出它的拟合曲线。以样本空间中和的值作为横坐标和纵坐标，画出样本空间的散点。代码如下：


```
def plotBestFit(weights):
    import matplotlib.pyplot as plt
    dataMat,labelMat=loadDataSet()
    dataArr = array(dataMat)
    n = shape(dataArr)[0] 
    xcord1 = []; ycord1 = []
    xcord2 = []; ycord2 = []
    for i in range(n):
        if int(labelMat[i])== 1:
            xcord1.append(dataArr[i,1]); ycord1.append(dataArr[i,2])
        else:
            xcord2.append(dataArr[i,1]); ycord2.append(dataArr[i,2])
    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xcord1, ycord1, s=30, c='red', marker='s')
    ax.scatter(xcord2, ycord2, s=30, c='green')
    x = arange(-3.0, 3.0, 0.1)
    y = (-weights[0]-weights[1]*x)/weights[2]
    y = y.transpose()
    ax.plot(x, y)
    plt.xlabel('X1'); plt.ylabel('X2');
    plt.show()
if __name__=='__main__':
    dataMat,labelMat=loadDataSet()
    weights=gradAscent(dataMat,labelMat)
    print '回归系数：\n',weights
    plotBestFit(weights)
```
运行后，我们得到如下图片：
![](http://wx1.sinaimg.cn/mw690/63918611gy1fm60lu2rxoj20hs0dcdg0.jpg)
通过我们的观察，我们的这个回归系数的算法还是比较准确的，拟合曲线将样本数据分成两部分，并且符合样本的分类规则。
接下来，我们来实现一个分类器，并测试这个分类器：


```
def classify0(targetData,weights):
    v = sigmoid(targetData*weights)
    if v>0.5:
        return 1.0
    else :
        return 0
def testClassify0():
    dataMat,labelMat=loadDataSet()
    examPercent=0.7
    row,col=shape(dataMat)
    exam=[]
    exam_label=[]
    test=[]
    test_label=[]
    for i in range(row):
        if i < row*examPercent:
            exam.append(dataMat[i])
            exam_label.append(labelMat[i])
        else:
            test.append(dataMat[i])
            test_label.append(labelMat[i])
    weights=gradAscent(exam,exam_label)
    errCnt=0
    trow,tcol=shape(test)
    for i in range(trow):
        v=int(classify0(test[i],weights))
        if v != int(test_label[i]):
            errCnt += 1
            print '计算值：',v,' 原值',test_label[i]
    print '错误率：',errCnt/trow
if __name__=='__main__':
    #dataMat,labelMat=loadDataSet()
    #weights=gradAscent(dataMat,labelMat)
    ##print 'dataMat:\n',dataMat
    ##print 'labelMat:\n',labelMat
    #print '回归系数：\n',weights
    #plotBestFit(weights)
    testClassify0()
```
分类器的实现很简单。我们使用之前的样本数据中的70条数据作为我们测试的样本数据，计算出回归系数。然后用分类器对剩下的30条记录进行分类，然后将结果和样本数据进行对比。最后打印出错误率。我们可以看到，错误率是0，近乎完美！我们可以修改测试样本在原样本空间的比例多测试几遍。那么，结论是我们的算法的准确率还不错！
那么，到这里问题就解决了吗？好像还差一点什么。我们来仔细研究一下我们计算回归系数的方法，不难发现，这个过程中我们用样本数据组成的矩阵进行了矩阵乘法。也就是说，为了计算回归系数，我们遍历了整个样本数据。
我们的问题又来了，我们例子中的样本数据只有100条，如果处理成千上万的样本数据，我们的计算回归系数的函数的计算复杂度会直线上升。下面我们来看看如何优化这个算法。
## 四、优化梯度上升算法——随机梯度上升法
我们在理解了回归系数迭代计算的公式
![](http://wx1.sinaimg.cn/mw690/63918611gy1fm60lxkhvdj20ba02f746.jpg)
和我们实现的程序之后。我们将计算回归系数的方法进行如下改进：


```
def stocGradAscent0(dataMatrix, classLabels):
    m,n = shape(dataMatrix)
    alpha = 0.01
    weights = ones((n,1))   #initialize to all ones
    for i in range(m):
        h = sigmoid(sum(dataMatrix[i]*weights))
        error = classLabels[i] - h
        weights = weights + alpha * error * mat(dataMatrix[i]).transpose()
    return weights
```
每一次迭代计算回归系数时，只使用样本空间中的一个样本点来计算。我们通过程序生成一个样本散点和拟合曲线的图来看一下这个算法的准确程度：
![](http://wx2.sinaimg.cn/mw690/63918611gy1fm60luh3ndj20hs0dcdg5.jpg)
不难看出跟之前的算法相差还是比较大的。原因是之前的算法是通过500次迭代算出的结果，后者只经过了100次迭代。那么这里要说明的问题是，回归系数在随着迭代次数的增加是趋于收敛的，并且收敛的过程是存在波动的。说白了，就是迭代的次数越多，越接近我们想要的那个值，但是由于样本的数据是非线性的，这个过程也会有一定的误差。具体的回归系数和迭代次数的关系大家可以参考一些教材，例如《机器学习实战》中的描述，这里就不做详细介绍了。
我们这里只介绍一下如何改进我们的算法，使我们的算法能够快速的收敛并减小波动。方法如下：
- 每次迭代随机的抽取一个样本点来计算回归向量
- 迭代的步长随着迭代次数增大而不断减少，但是永远不等于0
改进代码，并打印出拟合曲线和样本散点图：


```
def stocGradAscent1(dataMatrix, classLabels, numIter=150):
    m,n = shape(dataMatrix)
    weights = ones((n,1))   #initialize to all ones
    for j in range(numIter):
        dataIndex = range(m)
        for i in range(m):
            alpha = 4/(1.0+j+i)+0.0001    #apha decreases with iteration, does not 
            randIndex = int(random.uniform(0,len(dataIndex)))#go to 0 because of the constant
            h = sigmoid(sum(dataMatrix[randIndex]*weights))
            error = classLabels[randIndex] - h
            weights = weights + alpha * error * mat(dataMatrix[randIndex]).transpose()
            del(dataIndex[randIndex])
    return weights
if __name__=='__main__':
    dataMat,labelMat=loadDataSet()
    #weights=stocGradAscent0(dataMat,labelMat)
    weights=stocGradAscent1(dataMat,labelMat)
    #weights=gradAscent(dataMat,labelMat)
    #print 'dataMat:\n',dataMat
    #print 'labelMat:\n',labelMat
    #print '回归系数：\n',weights
    plotBestFit(weights)
    #testClassify0()
```
默认是150迭代的样本散点图和拟合曲线图：
![](http://wx1.sinaimg.cn/mw690/63918611gy1fm60lutip0j20hs0dcjrk.jpg)
不难看出准确程度与第一个算法很接近了！
## 五、总结
Logistic回归算法主要是利用了Sgimoid函数来为数据分类，分类的准确的关键取决于从样本空间中计算出的回归系数。我们使用梯度上升法来计算回归系数，并采用随机梯度上升法来改进了算法的性能。
