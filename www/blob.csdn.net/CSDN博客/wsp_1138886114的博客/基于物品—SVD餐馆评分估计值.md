# 基于物品—SVD餐馆评分估计值 - wsp_1138886114的博客 - CSDN博客





2018年09月21日 21:02:11[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：128











### 文章目录
- [1 导入模块 并 创建数据集](#1____1)
- [2 定义距离函数](#2__42)
- [3 基于物品相似度, 计算用户对物体的评分估计值](#3___56)
- [4 基于 SVD,  计算用户对物体的评分估计值](#4__SVD___75)




##### 1 导入模块 并 创建数据集

```python
```python
from numpy import *
from numpy import linalg as la

def loadExData():
    return[[0, 0, 0, 2, 2],
           [0, 0, 0, 3, 3],
           [0, 0, 0, 1, 1],
           [1, 1, 1, 0, 0],
           [2, 2, 2, 0, 0],
           [5, 5, 5, 0, 0],
           [1, 1, 1, 0, 0]]
    
def loadExData2():
    return[[0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 5],
           [0, 0, 0, 3, 0, 4, 0, 0, 0, 0, 3],
           [0, 0, 0, 0, 4, 0, 0, 1, 0, 4, 0],
           [3, 3, 4, 0, 0, 0, 0, 2, 2, 0, 0],
           [5, 4, 5, 0, 0, 0, 0, 5, 5, 0, 0],
           [0, 0, 0, 0, 5, 0, 1, 0, 0, 5, 0],
           [4, 3, 4, 0, 0, 0, 0, 5, 5, 0, 1],
           [0, 0, 0, 4, 0, 4, 0, 0, 0, 0, 4],
           [0, 0, 0, 2, 0, 2, 5, 0, 0, 1, 2],
           [0, 0, 0, 0, 5, 0, 0, 0, 0, 4, 0],
           [1, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0]]
    
def loadExData3():
    # 利用SVD提高推荐效果，菜肴矩阵
    return[[2, 0, 0, 4, 4, 0, 0, 0, 0, 0, 0],
           [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5],
           [0, 0, 0, 0, 0, 0, 0, 1, 0, 4, 0],
           [3, 3, 4, 0, 3, 0, 0, 2, 2, 0, 0],
           [5, 5, 5, 0, 0, 0, 0, 0, 0, 0, 0],
           [0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0],
           [4, 0, 4, 0, 0, 0, 0, 0, 0, 0, 5],
           [0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 4],
           [0, 0, 0, 0, 0, 0, 5, 0, 0, 5, 0],
           [0, 0, 0, 3, 0, 0, 0, 0, 4, 5, 0],
           [1, 1, 2, 1, 1, 2, 1, 0, 4, 5, 0]]
```
```

##### 2 定义距离函数

```python
```python
def ecludSim(inA,inB):	#计算欧几里得相似度（范数）
    return 1.0/(1.0 + la.norm(inA - inB))

def pearsSim(inA,inB): #皮尔森相似度
    if len(inA) < 3 : return 1.0
    return 0.5+0.5*corrcoef(inA, inB, rowvar = 0)[0][1]

def cosSim(inA,inB): #cos相似度
    num = float(inA.T*inB)
    denom = la.norm(inA)*la.norm(inB)
    return 0.5+0.5*(num/denom)
```
```

##### 3 基于物品相似度, 计算用户对物体的评分估计值

```python
```python
def standEst(dataMat, user, simMeas, item):	#estimate：评估器（数据矩阵，用户，相似度，菜种类）
    n = shape(dataMat)[1]		            #取列
    simTotal = 0.0; ratSimTotal = 0.0
    for j in range(n):
        userRating = dataMat[user,j]
        if userRating == 0: continue
        overLap = nonzero(logical_and(dataMat[:,item].A>0, \
                                      dataMat[:,j].A>0))[0]		#如果第j个菜和item同时被人吃（返回bool的Ture）
        if len(overLap) == 0: similarity = 0
        else: similarity = simMeas(dataMat[overLap,item], \
                                   dataMat[overLap,j])
        print('the %d and %d similarity is: %f' % (item, j, similarity))
        simTotal += similarity
        ratSimTotal += similarity * userRating
    if simTotal == 0: return 0
    else: return ratSimTotal/simTotal
```
```

##### 4 基于 SVD,  计算用户对物体的评分估计值

```python
```python
def svdEst(dataMat, user, simMeas, item):
    n = shape(dataMat)[1]
    simTotal = 0.0; ratSimTotal = 0.0
    U,Sigma,VT = la.svd(dataMat)
    # 在SVD分解之后，我们只利用包含了90%能量值的奇异值，
    Sig4 = mat(eye(4)*Sigma[:4]) # eye生成单位矩阵，整个语句将奇异值变为矩阵的形式
    # 利用U矩阵将物品转换到低维空间中，构建转换后的物品(物品+4个主要的特征)
    xformedItems = dataMat.T * U[:,:4] * Sig4.I  #变换坐标后的形式
    for j in range(n): # #变换坐标后的形式  # 列数 现在变成了 行数
        userRating = dataMat[user,j]
        if userRating == 0 or j==item: continue 
        similarity = simMeas(xformedItems[item,:].T,\
                             xformedItems[j,:].T)   # 因为矩阵已经转置了，所以每行代表商品
        print('the %d and %d similarity is: %f' % (item, j, similarity))
        simTotal += similarity
        ratSimTotal += similarity * userRating
    if simTotal == 0: return 0
    else: return ratSimTotal/simTotal

def recommend(dataMat, user, N=3, simMeas=cosSim, estMethod=standEst):
    unratedItems = nonzero(dataMat[user,:].A==0)[1]#find unrated items 
    if len(unratedItems) == 0: return 'you rated everything'
    itemScores = []
    for item in unratedItems:
        estimatedScore = estMethod(dataMat, user, simMeas, item)
        itemScores.append((item, estimatedScore))
    return sorted(itemScores, key=lambda jj: jj[1], reverse=True)[:N]

def printMat(inMat, thresh=0.8):
    for i in range(32):
        for k in range(32):
            if float(inMat[i,k]) > thresh:
                print(1, end='')
            else: print(0, end='')
        print('')

myMat = mat(loadExData3())
rec_res = recommend(myMat, 1, estMethod=svdEst)
print(rec_res)

rec_res = recommend(myMat, 1, estMethod=svdEst, simMeas=pearsSim)
print(rec_res)

rec_res = recommend(myMat, 2)
print(rec_res)
# 注释: loadExData3函数是新增的
```
```



