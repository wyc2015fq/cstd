# 理解线性回归（二）——Logistic Regression 回归 - bigfacesafdasgfewgf - CSDN博客





2014年09月27日 17:32:10[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：2041








**理解线性回归（二）——Logistic Regression 回归**





# 1. LR的基本思想




假设现在有一些数据点，我们用一条直线对这些点进行拟合（该线称为最佳拟合直线），这个拟合过程就称作回归。利用LR回归进行分类的主要思想是：根据现有数据对分类边界线建立回归公式，以此进行分类。

因为LR中需要输出类别，那么有一个映射的需求，就是输入一个样本的特征，投影到一个便于分类的空间中。

LR回归采用的是Sigmoid函数：

![](https://img-blog.csdn.net/20140927173233131?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)






sigmoid函数的图像如下：




![](https://img-blog.csdn.net/20140927173307545?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







    可以看到：如果横坐标刻度足够大，Sigmoid函数看起来就像一个阶跃函数。

为了实现LR回归分类器，我们可以在每个特征上都乘以一个回归系数，然后把所有的结果只相加，如下：




![](https://img-blog.csdn.net/20140927173242406?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







将这个结果带入到Sigmoid函数中，作为很横坐标输入，进而输出了一个0~1之间的数。我们便可以简单地规定任何大于0.5的数据被分为1类，小于0.5的被归为0类。这样也就得到了分类结果。




显然，LR回归算法中最重要的就是上式中的回归系数（权重形式）w，我们需要找到最佳的系数，才能够使得分类精确。这里求解回归系数，需要用到最优化的方法。




# 2. 梯度上升法




基本思想：要找到某函数的最大值，最好的办法就是沿着该函数的梯度方向探寻。函数f(x,y)的梯度定义如下：

![](https://img-blog.csdn.net/20140927173347371?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




梯度意味着要沿着x的发现移动![](https://img-blog.csdn.net/20140927173453578?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)，沿着y的方向移动![](https://img-blog.csdn.net/20140927173514435?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)。




![](https://img-blog.csdn.net/20140927173548786?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)










    每次沿着梯度的方向移动，函数值都是增长，而且梯度算子总是指向函数值增长的最快的方向。这是移动方向。

而移动的大小称为步长step，一般用alpha表示。




    我们用梯度上升法求上面LR回归系数w时，就是：




![](https://img-blog.csdn.net/20140927173612062?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)







可以看到每次w更新的大小都是梯度算子和步长联合起来的。

上面的公式一一直迭代下去，直到符合了一定的终止条件。







注：梯度下降法类似，可以用来求函数的最小值。

《机器学习实战》书里的用Python实现的LR算法，代码如下：





```python
<span style="font-size:18px;">    xcord1=[]; ycord1=[]
    xcord2=[]; ycord2=[]
    for i in range(n): 
        if int(labelMat[i])==1: 
            xcord1.append(dataArr[i, 1]); ycord1.append(dataArr[i, 2]) 
        else: 
            xcord2.append(dataArr[i, 1]); ycord2.append(dataArr[i, 2]) 
    
    fig=plt.figure()
    ax=fig.add_subplot(111) 
    ax.scatter(xcord1, ycord1, s=30, c='red', marker='s') 
    ax.scatter(xcord2, ycord2, s=30, c='green')
    x=np.arange(-3.0, 3.0, 0.1) 
    y=(-weights[0][0]-weights[1][0]*x)/weights[2][0] 
    print weights[0][0], weights[1][0], weights[2][0]
    ax.plot(x, y) 
    plt.xlabel('X1'); plt.ylabel('X2'); 
    plt.show()
    
    
    

if __name__=="__main__": 
    dataArr, labelMat=loadDataSet() 
    weights=gradAscent(dataArr, labelMat)
    print weights
    plotBestFit(weights.getA())</span>
```


其中原代码中getA实在不知道是什么作用，我就改动了一下。其中np.ones((n,1))是生成长度为n的list，而且没有list的元素都是list，所有我用weights[0][0]类似的形式取出元素的。其中的一些代码注释对以后重新理解LR算法也很大的帮助。运行之后的结果如下图，结果还是不错的。




![](https://img-blog.csdn.net/20140928142617867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








    除了发现分类效果还不错的同时，运行程序的时候还发现运行时间不短，因为这里的数据集才100个样本。分析原因可知，是在迭代的时候，每次更新回归系数都需要遍历整个数据集。




# 3. 改进方法




    有一种改进的方法就是：每次仅用一个样本点来更新回归系数，该方法称为随机梯度上升算法，这个有点儿类似online learning的影子。

下面我们比较梯度上升算法和随机梯度上升算法：




![](https://img-blog.csdn.net/20140928144517328?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)![](https://img-blog.csdn.net/20140928144615859?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)








随机梯度上升算法的代码如下：





```python
<span style="font-size:18px;">def stoGradAscent0(dataMatrix, classLabels): 
    m,n=np.shape(dataMatrix) 
    alpha=0.01 
    weights=np.ones(n) 
    for i in range(m): 
        h=sigmoid(sum(dataMatrix[i]*weights)) 
        error =classLabels[i]-h 
        weights=weights+alpha*error*dataMatrix[i]
    return weights

    </span>
```


随机梯度上升算法也有两点需要改进：

- alpha要在每次迭代的时候进行调整，这会缓解数据波动；
- 随机选取样本来更新回归系数。每次随机从列表中选出一个值，然后从列表中删掉该值，进入下一次迭代；




其中第二点在更多算法中都有提到。





```python
<span style="font-size:18px;">def stoGradAscent1(dataMatrix, classLabels, numIter=150): 
    m, n=np.shape(dataMatrix) 
    weights=np.ones(n) 
    for j in range(numIter): 
        dataIndex=range(m) 
        for i in range(m): 
            alpha=4/(1.0+j+i)+0.01  #alpha每次迭代都更新，改进1
            randIndex=int(np.random.uniform(0, len(dataIndex))) 
            h=sigmoid(sum(dataMatrix[randIndex]*weights)) 
            error=classLabels[randIndex]-h 
            weights=weights+alpha*error*datamatrix[randIndex] 
            del(dataIndex[randIndex])  #删除随机选择的样本，改进2
    return weights 

    </span>
```





    最后，运用LR算法进行分类的时候，我们只需要把测试集上每个特征向量乘以最优化方法得到的回归系数，再将乘积结果求和，最后输入到Sigmoid函数中，如果Sigmoid值大于0.5，就是1类；否则，就是0类。




    需要注意的是其中随机梯度上升算法是一种Online learning的方法。

    随机梯度上升算法与梯度上升算法效果相当，但是需要更少的计算资源。此外，随机梯度上升算法是一个在线算法，可以在新数据到来时完成参数更新，而不需要重新读取真个数据集来进行批处理运算。










