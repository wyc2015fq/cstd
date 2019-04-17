# 机器学习方法总结（三）：K-近邻 - qq_38593211的博客 - CSDN博客





2018年07月20日 12:41:55[小屁猴多看书](https://me.csdn.net/qq_38593211)阅读数：74标签：[KNN																[MachineLearning																[k-mean																[dbscan](https://so.csdn.net/so/search/s.do?q=dbscan&t=blog)
个人分类：[机器学习](https://blog.csdn.net/qq_38593211/article/category/7811819)





# K-近邻算法

## 1.流程

   三要素：距离度量、K值、分类决策规则。

   输入：![T={(x_{1},y_{1}),(x_{2},y_{2})......(x_{N},y_{N}))))}](https://private.codecogs.com/gif.latex?T%3D%7B%28x_%7B1%7D%2Cy_%7B1%7D%29%2C%28x_%7B2%7D%2Cy_%7B2%7D%29......%28x_%7BN%7D%2Cy_%7BN%7D%29%29%29%29%7D)，xi是实例的特征向量，yi是实例的类别。

   输出：实例x所属的类y。
-    根据给定的距离度量，在训练集T中找到与x最临近的k个点，涵盖这k个点的邻域记为Nk(x)。
-    在Nk(x)中根据分类决策规则（类别的众数）找出类别：

![y=argmax_{cj}\sum I(yi=cj) xi\in Nk(x)](https://private.codecogs.com/gif.latex?y%3Dargmax_%7Bcj%7D%5Csum%20I%28yi%3Dcj%29%20xi%5Cin%20Nk%28x%29)

## 2.优缺点

   优点：精度高、对异常值不敏感，无数据输入假定。

   缺点：计算复杂度高，空间复杂度高，K值不好确定。

## 3.实现方法

   kd树是对k维空间中的实例点进行存储以便对其进行快速检索的树形结构，kd树是二叉树，是对k维空间的一个划分，kd树的每一个节点对应于一个k维超矩形区域（即划分出来的区域）。

   1.构造方法如下：
- 构造根节点，使根节点对应于k维空间中包含的所有实例点的超矩形区域。
- 在超矩形区域（结点）上选择一个坐标轴和在此坐标轴上的一个切分点（实例点的中位数），确定一个超平面，这个超平面通过选定的切分点并垂直于选定的坐标轴，将当前超矩形区域切分为左右两个子区域（子结点）。
- 使用上述方法进行递归。

    2.搜索方法如下：
- 给定一个目标点x，搜索其最近邻，首先找到包含目标点的叶节点。
- 从根结点出发，递归向下访问kd树，若x小于切分点，移到左子结点，否则反之，记录其路径。
- 然后从该叶节点出发，以路径的最后一个点为最近邻点，一直退回溯到父节点，在这中间可能会出现路径点的兄弟结点对应的区域有更近的点（自己画图想想），如果有就更新最近邻点。

## 4.延伸

> 
除了KNN这种基于欧氏距离的有监督分类模型还有类似的像K-means和DESCAN这样基于距离或密度的无监督聚类模型，所谓无监督就是没有给样本划分标签，我们只需要根据样本间存在的联系和不同的聚类规则划分不同的簇，定好簇头即可。

- 相似度计算（这里对常用的相似性度量算法的原理、优缺点和适用场景进行介绍）
- 曼哈顿距离:就是两个样本之差的绝对值，优点就是计算速度快，对于用户数据特别大的相似度时就非常有利。
- 欧氏距离：最易于理解的一种距离计算方法，其实就是空间中两点间的距离公式；欧几里得距离在*数据完整* 或者数据密集的情况下效果最好，当数据有缺失、异常值或者度量尺度不统一的时候表现不好。
- 闵可夫斯基距离：是一类距离的定义，![d_{12}=\sqrt[p]{\sum_{k=1}^{n}|x_{1}-x_{2}|^{p} }](https://private.codecogs.com/gif.latex?d_%7B12%7D%3D%5Csqrt%5Bp%5D%7B%5Csum_%7Bk%3D1%7D%5E%7Bn%7D%7Cx_%7B1%7D-x_%7B2%7D%7C%5E%7Bp%7D%20%7D),当p=1时，就是曼哈顿距离,当p=2时，就是欧氏距离,当p→∞时，就是切比雪夫距离 。
- 余弦相似度：就是余弦的公式，只能分辨个体在特征之间的差异，没法衡量每个特征数值的差异，但在数据数据分布稀疏的情况下很好用，但存在缺点（比如用户对内容评分，5分制，X和Y两个用户对两个内容的评分分别为（1，2）和（4，5），使用余弦相似度得出的结果是0.98，两者极为相似，但从评分上看X似乎不喜欢这2个内容，而Y比较喜欢，余弦相似度对数值的不敏感导致了结果的误差，需要修正这种不合理性）。
- 皮尔逊相关系数：又称相关相似性，是余弦相似度在维度值缺失情况下的一种改进，通过Peason相关系数来度量两个用户的相似性，Peason相关系数是用协方差除以两个变量的标准差得到的，协方差能反映两个随机变量的相关程度（协方差大于0的时候表示两者正相关，小于0的时候表示两者负相关），当相关系数为1时，成为完全正相关；当相关系数为-1时，成为完全负相关；相关系数的绝对值越大，相关性越强；相关系数越接近于0，相关度越弱，它修正了上述提到的余弦相似度的“偏好”缺点。
- Jaccard相似系数：![Jaccard(X,Y)=\frac{X\bigcap Y}{X\bigcup Y}](https://private.codecogs.com/gif.latex?Jaccard%28X%2CY%29%3D%5Cfrac%7BX%5Cbigcap%20Y%7D%7BX%5Cbigcup%20Y%7D)，Jaccard系数主要用于计算符号度量或布尔值度量的个体间的相似度，无法衡量差异具体值的大小，只能获得“是否相同”这个结果，所以Jaccard系数只关心个体间共同具有的特征是否一致这个问题。

    2.  K-means：是一种给广泛使用的最基础的聚类算法，给定样本集，按照样本之间的欧氏距离将样本集划分为K个簇。

      流程：
- 随机初始化K个样本为每个簇的中心（也可以人工初始化，效果更好）。
- 计算每个样本距离各个簇心的欧式距离，选取距离最近的簇，将这个样本放到簇内，以簇心为簇样本的类别。
- 当所有样本都分好之后就更新簇心（以簇内样本的均值为新的簇心，也可以用中位数）。
- 重复二三步骤直到达到停止条件。

      优点：理解容易，如果簇近似高斯分布聚类效果不错。

      缺点：K值的数量和每个簇心的初始化不好确定，离群点对模型的影响较大。

    3.  基于密度的聚类方法(DBSCAN)

     首先确定半径r和minPoints. 从一个没有被访问过的任意数据点开始，以这个点为中心，r为半径的圆内包含的点的数量是否大于或等于minPoints，如果大于或等于minPoints则改点被标记为核心对象。重复此步骤，如果某个点不属于任何簇则会被标记为噪声点。某些样本可能到两个核心对象的距离都小于r，此时DBSCAN采用先来后到，先进行聚类的类别簇会标记这个样本为它的类别，也就是说BDSCAN的算法不是完全稳定的算法。

     优点：可以对任意形状的稠密数据集进行聚类，相对的，K-Means之类的聚类算法一般只适用于凸数据集；可以在聚类的同时发现异常点，对数据集中的异常点不敏感；初始值不需要人工确定。   

   缺点：需要事先确定距离r；如果样本集的密度不均匀、聚类间距差相差很大时，聚类质量较差，这时用DBSCAN聚类一般不适合；如果样本集较大时，聚类收敛时间较长，此时可以对搜索最近邻时建立的KD树进行规模限制来改进。

    4.  均值漂移:在未被分类的数据点中随机选择一个点作为中心点；找出离中心点距离在r之内的所有点，记做集合M；计算从中心点开始到集合M中每个元素的向量，将这些向量相加，得到偏移向量；中心点沿着偏移向量的方向移动，移动距离是偏移向量的模；重复上述步骤知道满足条件。此算法还可以应用在图像分割（将图像映射到特征空间，对采样点进行均值漂移聚类）和目标跟踪上。

## 5.实现 

```python
#KNN分类算法
#此代码也是基于手写数字的数据集进行测试的，数据集可以去网上下载，如果没有总结完了我会统一上传云盘。
#1.算距离：给定测试对象，计算它与训练集中的每个对象的距离
#2.找邻居：圈定距离最近的K(一般不大于20)个训练对象，作为测试对象的近邻
#3.做分类：根据这K个近邻归属的主要类别，来对测试对象分类
#优点：精度高、对异常值不敏感、无数据输入假定
#缺点：计算复杂度高、空间复杂度高
#核心分类算法
def classify0(inX,dataSet,labels,k):  
#inX是用于分类的输入向量；dataSet是输入的训练样本集合；labels是标签向量；k是选择的邻居数目
    dataSetSize=dataSet.shape[0]
    #训练样本的个数
    diffMat=tile(inX,(dataSetSize,1))-dataSet
    #将inX在列方向上重复dataSetSize次，算坐标差
    sqDiffMat=diffMat**2
    sqDistances=sqDiffMat.sum(axis=1)
    distance=sqDistances**0.5
    sortedDistIndicies=distance.argsort()
    #将与距离差进行排序，并将对应序号存入矩阵
    classCount={}
    for i in range(k):
        voteIlabe=labels[sortedDistIndicies[i]]
        classCount[voteIlabe]=classCount.get(voteIlabe,0)+1
    sortedClassCount=sorted(classCount.iteritems(),key=operator.itemgetter(1),reverse=True)
    return sortedClassCount[0][0]
#将图像矩阵（32*32）转换为int型的1024维的向量
def img2vector(filename):
    returnVect = zeros((1,1024))
    fr = open(filename)
    for i in range(32):
        lineStr = fr.readline()
        for j in range(32):
            returnVect[0,32*i+j] = int(lineStr[j])
    return returnVect
def handwritingClassTest():
    hwlabels = []
    #获取这个目录下的所有的文件名字
    trainingFileList = listdir('trainingDigits')
    m = len(trainingFileList)
    trainingMat = zeros((m,1024))
    for i in range(m):
        fileNameStr = trainingFileList[i]
        #将文件名与后缀分开
        fileStr = fileNameStr.split('.')[0]
        #分离出图片序号
        classNumStr = int(fileStr.split('_')[0])
        hwlabels.append(classNumStr)
        trainingMat[i,:] = img2vector(fileNameStr)
    testFileList = listdir('testDigits')
    errorCount = 0.0
    mTest - len(testFileList)
    for i in range(mTest):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classNumStr = int(fileStr.split('_')[0])
        vectorUnderTest = img2vector(fileNameStr)
        classifierResult = classify0(vectorUnderTest,trainingMat,hwlabels,3)
        print("the classifier came back with: %d the real answer is : %d"\
             %(classifierResult,classNumStr))
        if(classifierResult != classNumStr):errorCount += 1.0
```](https://so.csdn.net/so/search/s.do?q=k-mean&t=blog)](https://so.csdn.net/so/search/s.do?q=MachineLearning&t=blog)](https://so.csdn.net/so/search/s.do?q=KNN&t=blog)




