# 机器学习实战系列（五）：SVM支持向量机 - Machine Learning with Peppa - CSDN博客





2018年07月10日 13:36:39[刺客五六柒](https://me.csdn.net/qq_39521554)阅读数：409








课程的所有数据和代码在我的Github：[Machine learning in Action](https://github.com/Y1ran/Machine-Learning-in-Action-Python3)，目前刚开始做，有不对的欢迎指正，也欢迎大家star。除了 版本差异，代码里的部分函数以及代码范式也和原书不一样（因为作者的代码实在让人看的别扭，我改过后看起来舒服多了）。在这个系列之后，我还会写一个scikit-learn机器学习系列，因为在实现了源码之后，带大家看看SKT框架如何使用也是非常重要的。    

## 1、支持向量机概述



### 1.1 原理简述

所谓支持向量机，顾名思义，分为两个部分了解，一什么是**支持向量**(简单来说，就是支持（或支撑）平面上把两类类别划分开来的超平面的向量点)，二这里的“**机（machine,机器）**”是一个算法。在机器学习领域，常把一些算法看做是一个机器，如分类机(当然，也叫做分类器)，而支持向量机本身便是一种监督式学习的方法，它广泛的应用于统计分类以及回归分析中。

用一个二维空间里仅有两类样本的分类问题来举个小例子。 如图所示

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161223104452964-1314182410.png)

正方形和圆圈是要区分的两个类别，在二维平面中它们的样本如上图所示。中间的直线就是一个分类函数，它可以将两类样本完全分开。在这种情况下边缘实心的几个数据点就叫做**支持向量**，这也是支持向量机这个分类算法名字的来源。在SVM中，**我们寻找一条最优的分界线使得它到两边的边界都最大（最大化支持向量到分隔线(面)的距离）**。

一般的，如果一个**线性函数**能够将样本完全正确的分开，就称这些数据是线性可分的，否则称为非线性可分的。什么叫线性函数呢？在一维空间里就是一个点，在二维空间里就是一条直线，三维空间里就是一个平面，可以如此想象下去，如果不关注空间的维数，这种线性函数还有一个统一的名称——**超平面**。

在实际中，我们经常会遇到线性不可分的样例，此时，我们的常用做法是把样例特征通过某种**核函数**映射到高维空间中去，使其线性可分。下图即是映射前后的结果，将坐标轴经过适当的旋转，就可以很明显地看出，数据是可以通过一个平面来分开的。

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161223102352307-1300242912.gif)

核函数的价值在于它虽然也是将特征进行从低维到高维的转换，但核函数厉害在它事先在低维上进行计算，而将实质上的分类效果表现在了高维上，避免了直接在高维空间中的复杂计算。





### 1.2 特点

将SVM算法和前面介绍的Logistic回归和决策树算法作对比，如下图所示，我们能直观看到SVM作为非线性分类器的优势。

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161223110813526-768038491.png)



**优点：**泛化错误率低，计算开销不大，结果易解释

**缺点：**对参数调节和核函数的选择敏感，原始分类器不加修改仅适用于处理二类问题

**适用数据类型：**数值型和标称型数据，类别标签为+1和-1



[回到顶部](http://www.cnblogs.com/hemiy/p/6214034.html#_labelTop)

## 2、寻找最大分类间隔



### 2.1 关于线性分类器

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227171327882-1207831080.png)

上图中红蓝两类数据点可以用线性函数 **g(x)=w*x+b** 区分开，关于这个线性函数要注意一下三点
- 式中的 x 不是二维坐标系中的横轴，而是样本的向量表示，例如一个样本点的坐标是(3,8)，则x=(3,8)，而不是x=3
- 这个形式并不局限于二维的情况，在 n 维空间中仍然可以使用这个表达式，只是式中的 w 成为了 n 维向量
- g(x) 不是中间那条直线的表达式，中间那条直线的表达式是g(x)=0，即  w*x+b = 0，也把这个函数叫做分类面

易知中间那条分界线并不是唯一的，把它稍微旋转或平移一下，仍然可以达到上面说的效果。那就牵涉到一个问题，对同一个问题，哪一个函数更好？通常的衡量指标叫做分类间隔。 



### 2.2 分类间隔

在监督学习中，每一个样本由一个特征向量和一个类别标签组成，如下：

Di=(xi,yi)

xi 就是特征向量，就是yi 分类标记。

在二元的线性分类中， 这个表示分类的标记只有两个值，+1和-1。有了这种表示法，我们就可以定义一个样本点到某个超平面的间隔（**函数间隔**）：

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227193645726-1935872859.png)

注意到如果某个样本属于该类别的话，那么wi*x+b > 0（这是因为我们所选的g(x)=wx+b就是通过大于0还是小于0来判断分类），而yi也大于0；若不属于该类别的话，那么wi*x+b < 0 ，而yi也小于0，这意味着yi(w*xi+b)总是大于0，而它的值就等于|wxi+b|, 也即|g(xi)|.

现在把w和b进行归一化，即用w/||w||和b/||w||分别代替原来的w和b，那么间隔就可以写成如下形式，叫做**几何间隔**，几何间隔所表示的正是点到超平面的欧式距离。

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227185429773-889635160.png)

其中，||w|| 叫做向量w的范数，范数是对向量长度的一种度量。我们常说的向量长度其实指的是它的2-范数，范数最一般的表示形式为p-范数，可以写成如下形式：

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227175521086-1182709825.png)

好，到这现在的目标就是找出分离器定义中的**w**和b。而前面我们知道SVM依据**最大化支持向量到分隔线(面)的距离，**为此我们必须找到具有最小间隔的数据点，而这些数据点也就是前面提到的支持向量。一旦找到具有最小间隔的数据点，我们就需要对该间隔最大化。这就可以写作： 

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227193050304-1846587242.png)

直接求解上述问题相当困难，可以将它转换成为另一种更容易求解的形式。考察上式中大括号内的部分，我们可以固定其中一个因子而最大化其他因子。如果令所有支持向量 ![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227194058023-395037592.png)的都为1，那么就可以通过求 ||w|| 的最小值来得到最终解。但是， 并非所有数据点的 ![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227194201914-92893103.png) 都等于1，只有那些离分隔超平面最近的点得到的值才为1。而离超平面越远的数据点，其值也就越大。 

上述问题是一个带约束条件的优化问题，这里的约束条件是![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227194555586-1191586036.png) 大于等于1，对于这类问题可以引入拉格朗日乘子法求解。由于这里的约束条件都是基于数据点的，因此我们就可以将超平面写成数据点的形式。于是，优化目标函数最后可以写成：

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227194819054-51524387.png)![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227194934257-1803940788.png)

其约束条件为：

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227195114086-1707591556.png)

这里我们有个假设即数据100%线性可分，但是实际上并不是所有数据都能达到该要求，因此我们可以通过引入所谓**松弛变量——C**，来允许有些数据点可以处于分隔面的错误一侧，这时约束条件变为：

![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227195534554-1345285697.png)![](https://images2015.cnblogs.com/blog/1064149/201612/1064149-20161227200007867-1110324906.png)

这里的常数C用于控制 “最大化间隔” 和 “保证大部分点的函数间隔小于1.0” 这两个目标的权重。在优化算法的实现代码中，常数C是一个参数，因此我们就可以通过调节该参数得到不同的结果。一旦求出了所有的alpha，那么分隔超平面就可以通过这些alpha来表达。  

[回到顶部](http://www.cnblogs.com/hemiy/p/6214034.html#_labelTop)

## 3、SMO算法

本节对前面的两个式子进行优化，一个是最小化的目标函数，一个是遵循的约束条件。优化的方法有很多种，但本章只关注其中最流行的一种实现，即序列最小优化(SMO)算法。SMO算法的目标是求出一系列alpha和b，一旦求出了这些alpha，就很容易计算出权重向量w并得到分隔超平面（2维平面中就是直线），再能够将之用于数据分类。

SMO算法的工作原理是：每次循环中选择两个alpha进行优化处理。一旦找到一对合适的alpha,那么就增大其中一个同时减小另一个。这里所谓的“合适” 就是指两个alpha必须要符合一定的条件，即这两个alpha必须要在间隔边界之外，且这两个alpha还没有进行过区间化处理或者不在边界上。



### 3.1 简化版SMO算法处理小规模数据集

SMO算法的完整实现需要大量代码。在接下来的第一个例子中，我们将会对算法进行简化处理，以便了解算法的基本工作思路，之后再基于简化版给出完整版。 

该算法伪代码大致如下：

```python
创建一个alpha向量并将其初始化为O向量

当迭代次数小于最大迭代次数时（外循环）

对数据集中的每个数据向量（内循环）： 

如果该数据向量可以被优化：

随机选择另外一个数据向量

同时优化这两个向量

如果两个向量都不能被优化，退出内循环

如果所有向量都没被优化，增加迭代数目，继续下一次循环
```

## 加载数据集

```python
import random

# SMO算法相关辅助中的辅助函数
# 解析文本数据函数，提取每个样本的特征组成向量，添加到数据矩阵
# 添加样本标签到标签向量
def loadDataSet(filename):
    dataMat=[];labelMat=[]
    fr=open(filename)
    
    for line in fr.readlines():
        lineArr=line.strip().split('\t')
        dataMat.append([float(lineArr[0]),float(lineArr[1])])
        #if int(lineArr[2]) == 0 :
        # labelMat.append((float(lineArr[2]) - 1))
        #else:
        labelMat.append((float(lineArr[2] )))
    return dataMat,labelMat

#2 在样本集中采取随机选择的方法选取第二个不等于第一个alphai的
#优化向量alphaj
def selectJrand(i,m):
    j=i
    while(j==i):
        j=int(random.uniform(0,m))
    return j

#3 约束范围L<=alphaj<=H内的更新后的alphaj值    
def clipAlpha(aj,H,L):
    if aj > H:
        aj = H
    if L > aj:
        aj = L
    return aj
```

## SMO算法（简单版）

```python
def smoSimple(dataMat,classLabels,C,toler,maxIter):
    '''
    @dataMat    ：数据列表
    @classLabels：标签列表
    @C          ：权衡因子（增加松弛因子而在目标优化函数中引入了惩罚项）
    @toler      ：容错率
    @maxIter    ：最大迭代次数
    '''
    #将列表形式转为矩阵或向量形式
    dataMatrix=mat(dataMat);labelMat=mat(classLabels).transpose()
    #初始化b=0，获取矩阵行列
    b=0;m,n=shape(dataMatrix)
    #新建一个m行1列的向量
    alphas=mat(zeros((m,1)))
    #迭代次数为0
    iters=0
    while(iters<maxIter):
        #改变的alpha对数
        alphaPairsChanged=0
        #遍历样本集中样本
        for i in range(m):
            #计算支持向量机算法的预测值
            fXi=float(multiply(alphas,labelMat).T*\
            (dataMatrix*dataMatrix[i,:].T))+b
            #计算预测值与实际值的误差
            Ei=fXi-float(labelMat[i])
            #如果不满足KKT条件，即labelMat[i]*fXi<1(labelMat[i]*fXi-1<-toler)
            #and alpha<C 或者labelMat[i]*fXi>1(labelMat[i]*fXi-1>toler)and alpha>0
            if(((labelMat[i]*Ei < -toler)and(alphas[i] < C)) or \
            ((labelMat[i]*Ei>toler) and (alphas[i]>0))):
                #随机选择第二个变量alphaj
                j = selectJrand(i,m)
                #计算第二个变量对应数据的预测值

                fXj = float(multiply(alphas,labelMat).T*(dataMatrix*\
                            dataMatrix[j,:].T)) + b
                #计算与测试与实际值的差值
                Ej = fXj - float(labelMat[j])
                #记录alphai和alphaj的原始值，便于后续的比较
                alphaIold=alphas[i].copy()
                alphaJold=alphas[j].copy()
                #如何两个alpha对应样本的标签不相同
                if(labelMat[i]!=labelMat[j]):
                    #求出相应的上下边界
                    L=max(0,alphas[j]-alphas[i])
                    H=min(C,C+alphas[j]-alphas[i])
                else:
                    L=max(0,alphas[j]+alphas[i]-C)
                    H=min(C,alphas[j]+alphas[i])
                if L==H: print("L==H");continue
                #根据公式计算未经剪辑的alphaj
                #------------------------------------------
                eta=2.0*dataMatrix[i,:]*dataMatrix[j,:].T-\
                    dataMatrix[i,:]*dataMatrix[i,:].T-\
                    dataMatrix[j,:]*dataMatrix[j,:].T
                #如果eta>=0,跳出本次循环
                if eta>=0:print("eta>=0"); continue
                alphas[j]-=labelMat[j]*(Ei-Ej)/eta
                alphas[j]=clipAlpha(alphas[j],H,L)
                #------------------------------------------    
                #如果改变后的alphaj值变化不大，跳出本次循环    
                if(abs(alphas[j]-alphaJold)<0.00001):print("j not moving\
                enough");continue
                #否则，计算相应的alphai值
                alphas[i]+=labelMat[j]*labelMat[i]*(alphaJold-alphas[j])
                #再分别计算两个alpha情况下对于的b值
                b1 = b - Ei- labelMat[i]*(alphas[i]-alphaIold)*dataMatrix[i,:]\
                 *dataMatrix[i,:].T - labelMat[j]*(alphas[j]-alphaJold)*\
                 dataMatrix[i,:]*dataMatrix[j,:].T
                b2=b-Ej-labelMat[i]*(alphas[i]-alphaIold)*\
                    dataMatrix[i,:]*dataMatrix[j,:].T-\
                    labelMat[j]*(alphas[j]-alphaJold)*\
                    dataMatrix[j,:]*dataMatrix[j,:].T
                #如果0<alphai<C,那么b=b1
                if(0<alphas[i]) and (C>alphas[i]):b=b1
                #否则如果0<alphai<C,那么b=b1
                elif (0<alphas[j]) and (C>alphas[j]):b=b2
                #否则，alphai，alphaj=0或C
                else:b=(b1+b2)/2.0
                #如果走到此步，表面改变了一对alpha值
                alphaPairsChanged+=1
                print("iters: %d i:%d,paird changed %d" %(iters,i,alphaPairsChanged))
        #最后判断是否有改变的alpha对，没有就进行下一次迭代
        if(alphaPairsChanged==0):iters+=1
        #否则，迭代次数置0，继续循环
        else:iters=0
        print("iteration number: %d" %iters)
    #返回最后的b值和alpha向量
    return b,alphas
```

## SMO算法（完全版）

```python
#内循环寻找alphaj
def innerL(i, oS):
    Ei = Opt_smo.calcEk(oS, i)
    if ((oS.labelMat[i]*Ei < -oS.tol) and (oS.alphas[i] < oS.C)) or ((oS.labelMat[i]*Ei > oS.tol) and (oS.alphas[i] > 0)):
        j,Ej = Opt_smo.selectJ(i, oS, Ei) #this has been changed from selectJrand
        alphaIold = oS.alphas[i].copy(); alphaJold = oS.alphas[j].copy();
        if (oS.labelMat[i] != oS.labelMat[j]):
            L = max(0, oS.alphas[j] - oS.alphas[i])
            H = min(oS.C, oS.C + oS.alphas[j] - oS.alphas[i])
        else:
            L = max(0, oS.alphas[j] + oS.alphas[i] - oS.C)
            H = min(oS.C, oS.alphas[j] + oS.alphas[i])
        if L==H: print("L==H"); return 0
        eta = 2.0 * oS.K[i,j] - oS.K[i,i] - oS.K[j,j] #changed for kernel
        if eta >= 0: print("eta>=0"); return 0
        oS.alphas[j] -= oS.labelMat[j]*(Ei - Ej)/eta
        oS.alphas[j] = clipAlpha(oS.alphas[j],H,L)
        Opt_smo.updateEk(oS, j) #added this for the Ecache
        if (abs(oS.alphas[j] - alphaJold) < 0.00001): print("j not moving enough"); return 0
        oS.alphas[i] += oS.labelMat[j]*oS.labelMat[i]*(alphaJold - oS.alphas[j])#update i by the same amount as j
        Opt_smo.updateEk(oS, i) #added this for the Ecache                    #the update is in the oppostie direction
        b1 = oS.b - Ei- oS.labelMat[i]*(oS.alphas[i]-alphaIold)*oS.K[i,i] - oS.labelMat[j]*(oS.alphas[j]-alphaJold)*oS.K[i,j]
        b2 = oS.b - Ej- oS.labelMat[i]*(oS.alphas[i]-alphaIold)*oS.K[i,j]- oS.labelMat[j]*(oS.alphas[j]-alphaJold)*oS.K[j,j]
        if (0 < oS.alphas[i]) and (oS.C > oS.alphas[i]): oS.b = b1
        elif (0 < oS.alphas[j]) and (oS.C > oS.alphas[j]): oS.b = b2
        else: oS.b = (b1 + b2)/2.0
        return 1
    else: return 0

def smoP(dataMatIn, classLabels, C, toler, maxIter,kTup=('lin', 0)):    #full Platt SMO
    oS = Opt_smo.optStruct(mat(dataMatIn),mat(classLabels).transpose(),C,toler, kTup)
    iter = 0
    entireSet = True; alphaPairsChanged = 0
    while (iter < maxIter) and ((alphaPairsChanged > 0) or (entireSet)):
        alphaPairsChanged = 0
        if entireSet:   #go over all
            for i in range(oS.m):        
                alphaPairsChanged += innerL(i,oS)
                print("fullSet, iter: %d i:%d, pairs changed %d" % (iter,i,alphaPairsChanged))
            iter += 1
        else:#go over non-bound (railed) alphas
            nonBoundIs = nonzero((oS.alphas.A > 0) * (oS.alphas.A < C))[0]
            for i in nonBoundIs:
                alphaPairsChanged += innerL(i,oS)
                print("non-bound, iter: %d i:%d, pairs changed %d" % (iter,i,alphaPairsChanged))
            iter += 1
        if entireSet: entireSet = False #toggle entire set loop
        elif (alphaPairsChanged == 0): entireSet = True  
        print("iteration number: %d" % iter)
    return oS.b,oS.alphas
```



## 核函数

```python
def kernelTrans( X, A, kTup):
    '''
    RBF kernel function
    '''
    m ,n = shape(X)
    K = mat(zeros((m,1)))
    if kTup[0] == 'lin': K = X * A.T
    elif kTup[0] == 'rbf':
        for j in range(m):
            deltaRow = X[j,:] - A
            K[j] = deltaRow * deltaRow.T
        K = numpy.exp(K / (-1 * kTup[1] ** 2))
    
    else:   raise NameError('huston ---')
    return K

def testRbf(k1=1.3):
    dataArr,labelArr = loadDataSet('testSetRBF.txt')
    b,alphas = SMO_platt.smoP(dataArr, labelArr, 200, 0.0001, 10000, ('rbf', k1)) #C=200 important
    datMat=mat(dataArr); labelMat = mat(labelArr).transpose()
    svInd=nonzero(alphas.A>0)[0]
    sVs=datMat[svInd] #get matrix of only support vectors
    labelSV = labelMat[svInd];
    print("there are %d Support Vectors" % shape(sVs)[0])
    m,n = shape(datMat)
    errorCount = 0
    for i in range(m):
        kernelEval = kernelTrans(sVs,datMat[i,:],('rbf', k1))
        predict=kernelEval.T * multiply(labelSV,alphas[svInd]) + b
        if sign(predict)!=sign(labelArr[i]): errorCount += 1
    print("the training error rate is: %f" % (float(errorCount)/m))
    dataArr,labelArr = loadDataSet('testSetRBF2.txt')
    errorCount = 0
    datMat=mat(dataArr); labelMat = mat(labelArr).transpose()
    m,n = shape(datMat)
    for i in range(m):
        kernelEval = kernelTrans(sVs,datMat[i,:],('rbf', k1))
        predict=kernelEval.T * multiply(labelSV,alphas[svInd]) + b
        if sign(predict)!=sign(labelArr[i]): errorCount += 1    
    print("the test error rate is: %f" % (float(errorCount)/m))
```







## 手写数字识别

```python
def img2vector(filename):
    returnVect = zeros((1,1024))
    fr = open(filename)
    for i in range(32):
        lineStr = fr.readline()
        for j in range(32):
            returnVect[0,32*i+j] = int(lineStr[j])
    return returnVect

def loadImages(dirName):
    from os import listdir
    hwLabels = []
    trainingFileList = listdir(dirName)           #load the training set
    m = len(trainingFileList)
    trainingMat = zeros((m,1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]     #take off .txt
        classNumStr = int(fileStr.split('_')[0])
        if classNumStr == 9: hwLabels.append(-1)
        else: hwLabels.append(1)
        trainingMat[i,:] = img2vector('%s/%s' % (dirName, fileNameStr))
    return trainingMat, hwLabels    

def testDigits(kTup=('rbf', 10)):
    dataArr,labelArr = loadImages('digits/trainingDigits')
    b,alphas = smoP(dataArr, labelArr, 200, 0.0001, 10000, kTup)
    datMat=mat(dataArr); labelMat = mat(labelArr).transpose()
    svInd=nonzero(alphas.A>0)[0]
    sVs=datMat[svInd] 
    labelSV = labelMat[svInd];
    print ("there are %d Support Vectors" % shape(sVs)[0])
    m,n = shape(datMat)
    errorCount = 0
    for i in range(m):
        kernelEval = kernelTrans(sVs,datMat[i,:],kTup)
        predict=kernelEval.T * multiply(labelSV,alphas[svInd]) + b
        if sign(predict)!=sign(labelArr[i]): errorCount += 1
    print ("the training error rate is: %f" % (float(errorCount)/m))
    dataArr,labelArr = loadImages('testDigits')
    errorCount = 0
    datMat=mat(dataArr); labelMat = mat(labelArr).transpose()
    m,n = shape(datMat)
    for i in range(m):
        kernelEval = kernelTrans(sVs,datMat[i,:],kTup)
        predict=kernelEval.T * multiply(labelSV,alphas[svInd]) + b
        if sign(predict)!=sign(labelArr[i]): errorCount += 1    
    print ("the test error rate is: %f" % (float(errorCount)/m))
```



