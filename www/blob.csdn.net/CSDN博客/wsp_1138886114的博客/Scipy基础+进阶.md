# Scipy基础+进阶 - wsp_1138886114的博客 - CSDN博客





2018年05月25日 14:31:49[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1838
所属专栏：[科学计算库与可视化](https://blog.csdn.net/column/details/23452.html)









# Python科学计算库—Scipy

### 一、Scipy 入门

> 
#### 1.1、Scipy 简介及安装

```
官网：http://www.scipy.org/SciPy
安装：在C:\Python27\Scripts下打开cmd执行:
执行：pip install scipy
```

#### 1.2、安装Anaconda及环境搭建(举例演示)

```
创建环境：conda create -n env_name python=3.6
示例：   conda create -n Py_36 python=3.6  #创建名为Py_367的环境
列出所有环境：conda info -e
进入环境：   source activate Py_36  (OSX/LINUX系统)
            activate Py_36          (windows系统)
```

#### 1.3、jupyter 安装

```
jupyter简介：jupyter（Jupyter Notebook）是一个交互式笔记本
            支持运行40多种编程语言
            数据清理和转换，数值模拟，统计建模，机器学习等 
jupyter 安装：conda install jupyter notebook
启动 jupyter：激活相应环境
在控制台执行 ：jupyter notebook
notebook服务器运行地址：http://localhost:8888   
                新建（notebook，文本文件，文件夹）
关闭notebook：ctrl+c执行两次
jupyter notebook 使用：
    快捷键：shift+Enter     运行本单元，选中下个单元
           Ctrl+Enter      运行本单元，在其下方插入新单元
            Y              单元进入代码状态
            M              单元进入markdown状态
            A              在上方插入新单元
            B              在下方插入新单元 
            X              剪切选中单元 
            Shift+V        在上方粘贴单元
```

#### 1.4、scipy的’hello word’

```
需求：将一个多维数组保存a.mat文件，后加载该mat文件，获取内容并打印
步骤1：导入scipy需要的模块
    from scipy import io    #(需要使用的模块)
步骤2：利用savemat保存数据
    io.savemat(file_name,mdict)
    io.savemat('a.mat',{''array:a})
步骤3：利用 loadmat载入数据
    io.loadmat(file_name)
    data = io.loadmat('a.mat')
```


```python
举例1：
from scipy import io              #导入io
import numpy as np                #导入numpy并命名为np    
arr = np.array([1,2,3,4,5,6])
io.savemat('test.mat',{'arr1':arr})
loadArr=io.loadmat('test.mat')

举例2
from matplotlib import pyplot as plt
from scipy import io
import numpy as np
matrix1 = np.arange(1,10).reshape(3,3)          #创建矩阵
io.savemat("matrix1.mat", {"array": matrix1})   #保存矩阵文件
data=io.loadmat('matrix1.mat')                  #读取矩阵文件
print (data["array"])                           #输出矩阵

p1 = np.random.normal(size = 10000)  #新建随机数
plt.hist(p1)                         #绘制柱形图
plt.show()                           #显示
```

### 二、利用Scipy实现统计功能

> 
#### 需求：用Scipy的`scipy.stats`中的统计函数分析随机数

stats提供了产生连续性分布的函数 
**均匀分布（uniform）**
$~~~~~~~~$`x=stats.uniform.rvs(size = 20)`    生成20个[0,1]均匀分布随机数 
**-正态分布（norm）**
$~~~~~~~~$`x=stats.norm.rvs(size = 20)`   生成20个正态分布随机数 
**-贝塔分布（beta）**
$~~~~~~~~$`x=stats.beta.rvs(size=20，a=3,b=4)`生成20个服从参数a=3,b=4贝塔分布随机数 
**-离散分布**
**-伯努利分布（Bernoulli）**
**-几何分布（geom）**
**-泊松分布（poisson）**
`x=stats.poisson.rvs(0.6,loc=0,size = 20)`生成20个服从泊松分布随机数    
### 三、计算随机数均值和标准差

> 
`stats.norm.fit` :利用正态分布去拟合生成的数据，得到其均值和标准差


### 四、计算随机数的偏度

> 
1.概念： 
$~~~~~~~~$偏度（skewness）描述的是概率分布的偏度（非对称）程度。 
$~~~~~~~~$有两个返回值，第二个为p-value，即数据集服从正态分布的概率（0~1）
2 利用 `stats.skewtest()`计算偏度

### 五、计算随机数的峰度

> 
1 概念：峰度（`kurtosis`）-描述的是概率分布曲线陡峭程度 

    2 利用 `stats.kurtosis()` 计算峰度 

    3 正态分布峰度值为`3`，`excess_k`为`0`
$~~~~~~~$低阔峰(platykurtic) 相对于正态分布来说更扁平 `excess_k<0`
$~~~~~~~$高狭峰(leptokurtic) 相对于正态分布来说更陡峭 `excess_k>0`
```python
示例：(../Scipy/Test01/test1)
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt

arr = stats.norm.rvs(size=900)
(mean,std) = stats.norm.fit(arr)
print('平均值',mean)       #mean平均值
print('std标准差',std)     #std标准差
(skewness,pvalue1) = stats.skewtest(arr)
print('偏度值')    
print(skewness)
print('符合正态分布数据的概率为')
print(pvalue1)
(Kurtosistest,pvalue2) = stats.kurtosistest(arr)
print('Kurtosistest',Kurtosistest)    #峰度
print('pvalue2',pvalue2)
(Normltest,pvalue3) = stats.normaltest(arr)
print('Normltest',Normltest)          #服从正太分布度
print('pvalue3',pvalue3)
num = stats.scoreatpercentile(arr,95) #某一百分比处的数值
print('在95%处的数值：')                #某一百分比处的数值
print num
indexPercent = stats.percentileofscore(arr,1) #某一数值处的百分比
print ('在数值1处的百分比：')                   #某一数值处的百分比
print indexPercent
plt.hist(arr)   #设置直方图
plt.show()      #显示图
```

### 六、正态分布程度检验

> 
1 正态性检验(normality test),同样返回两个值，第二个返回p-values 

    2 利用 检验$~~~$`stats.normaltest()`
$~~~~$一般情况$~~$`pvalue>0.05`$~~~~~~~~$表示服从正态分布
### 七、计算数据所在区域中某一百分比处的数值

> 
1 利用scoreatpercentile 计算在某一百分比位置的数值 
$~~~~~~~~$格式：`scoreatpercentile` (数据集、百分比) 
$~~~~~~~~~~~~~~~~~$`stats.scoreatpercentile(name_arr,percent)`

   2 示例：求出95%所在位置的数值 
$~~~~~~~~$`num = stats.scoreatpercentile(arr,95)`
$~~~~~~~~$`print num`
### 八、从某数值出发找到对应的百分比

> 
利用`percentileofscore`计算在某数值对应的百分比 
$~~~~~~~~$格式：`percentileofscore(数据集,数值)`
$~~~~~~~~$示例：`indexPercent = stats.percentileofscore(arr,1)`
### 九、直方图显示

> 
`import matplotlib.pyplot as plt`

  在Anaconda环境下(py36)C:\Users\lenovo>导入：`conda install matplotlib`
$~~~~~~~~$`plt.hist(arr)`  #设置直方图 
$~~~~~~~~$`plt.show()`    #显示图
### 九、综合练习

###### 1 求出考试分数的以下值：

```
均值          中位数       众数        极差          方差  
标准差        变异系数(均值/方差)       偏度          峰度
```

###### 2 步骤1：    创建两个二维数组:[分数，出现次数]

```
arrEasy=np.array([[0,2],[2.5,4],[5,6],[7.5,9],[10,13],[12.5,16],[15,19],[17.5,23],
                [20,27],[22.5,31],[25,35],[27.5,40],[30,53],[32.5,68],[35,90],
                [37.5,110],[40,130],[42.5,148],[45,165],[47.5,182],[50,195],
                [52.5,208],[55,217],[57.5,226],[60,334],[62.5,342],[65,349],
                [67.5,500],[70,511],[72.5,300],[75,200],[77.5,80],[80,20]])
arrDiff=np.array([[0,20],[2.5,30],[5,45],[7.5,70],[10,100],[12.5,135],[15,170],
                [17.5,205],[20,226],[22.5,241],[25,251],[27.5,255],[30,256],
                [32.5,253],[35,249],[37.5,242],[40,234],[42.5,226],[45,217],
                [47.5,208],[50,195],[52.5,182],[55,165],[57.5,148],[60,130],
                [62.5,110],[65,40],[67.5,30],[70,20],[72.5,5],[75,5],[77.5,0],[80,0]])
```

###### 步骤2：创建函数，将传入的多维数组扁平化->变成一维数组

```python
方法1：
def createScore(arr):
    score = []          #所有学员分数
    row = arr.shape[0]
    for i in np.arange(0,row):
        for j in np.arange(0,int(arr[i][1])):
        score.append(arr[i][1]))
    score = np.array(score)
    return score
```

```php
方法2
score_Easy, num_Easy = arrEasy[:,0],arrEasy[:,1]  #所有行第一列；所有行第二列
score_Diff, num_Diff = arrDiff[:,0],arrDiff[:,1]  #同上
print (score_Easy,num_Easy )
print (score_Diff,num_Diff )
```

###### 步骤3：创建函数，根据传入数组，对其进行统计

```python
def calStatValue(score):
    #集中趋势度量
    print('均值')
    print(np.mean(score))
    print('中位数')
    print(np.median(score))
    print('众数')
    print(stats.mode(score))
    #离散趋势度量
    print('极差')
    print(np.ptp(score))
    print('方差')
    print(np.var(score))
    print('标准差')
    print(np.std(score))
    print('变异系数')
    print(np.mean(score)/np.std(score))
    #偏度与峰度的度量
    print('偏度')
    print(stats.skewness(score))
    print('峰度')
    print(stats.Kurtosis(score))
```

###### 步骤4：创建函数，做一个简单的箱线图/柱形图

```php
def drawGraghic(score)
    plt.boxplot([score],labels['score'])    #箱线图
    plt.title('箱线图')
    plt.show()
    plt.hist(score,100)
    plt.show()

步骤5：
步骤6：

案例完整代码：
import numpy as np
from scipy import stats
import matplotlib.pyplot as plt
def createScore(arr):
    score = []                  #所有学员分数
    row = arr.shape[0]          #获取多少组元素
    for i in np.arange(0,row):  #遍历所有元素组
        for j in np.arange(0,int(arr[i][1])):#从0开始填充次数，第i行第1列
            score.append(arr[i][0]))
    score = np.array(score)
    return score
_________________________________
使用切片获取分数
score_Easy, num_Easy = arrEasy[:,0],arrEasy[:,1]  #所有行第一列；所有行第二列
score_Diff, num_Diff = arrDiff[:,0],arrDiff[:,1]  #同上
print (score_Easy,num_Easy)          #查看分数，人数
print (score_Diff,num_Diff)          #同上

All_score_Easy = np.repeat(list(score_Easy),list(num_Easy)) #所有分数
All_score_Diff = np.repeat(list(score_Diff),list(num_Diff)) #所有分数
________________________________
def createOneScore():
    arrEasy=np.array([[0,2],[2.5,4],[5,6],[7.5,9],[10,13],[12.5,16],[15,19],[17.5,23],
                [20,27],[22.5,31],[25,35],[27.5,40],[30,53],[32.5,68],[35,90],
                [37.5,110],[40,130],[42.5,148],[45,165],[47.5,182],[50,195],
                [52.5,208],[55,217],[57.5,226],[60,334],[62.5,342],[65,349],
                [67.5,500],[70,511],[72.5,300],[75,200],[77.5,80],[80,20]])
    return createScore(arrOne)
def createTwoScore():       
    arrDiff=np.array([[0,20],[2.5,30],[5,45],[7.5,70],[10,100],[12.5,135],[15,170],
                [17.5,205],[20,226],[22.5,241],[25,251],[27.5,255],[30,256],
                [32.5,253],[35,249],[37.5,242],[40,234],[42.5,226],[45,217],
                [47.5,208],[50,195],[52.5,182],[55,165],[57.5,148],[60,130],
                [62.5,110],[65,40],[67.5,30],[70,20],[72.5,5],[75,5],[77.5,0],[80,0]])
    return createScore(arrTwo)
def calStatValue(score):
    #集中趋势度量
    print('均值')
    print(np.mean(score))
    print('中位数')
    print(np.median(score))
    print('众数')
    print(stats.mode(score))
    #离散趋势度量
    print('极差')
    print(np.ptp(score))
    print('方差')
    print(np.var(score))
    print('标准差')
    print(np.std(score))
    print('变异系数')
    print(np.mean(score)/np.std(score))

    #偏度与峰度的度量
    (skewness,pvalue1) = stats.skewtest(score)  
    print('偏度')
    print(stats.skewness(score))

    (Kurtosistest,pvalue2) = stats.kurtosistest(arr)
    print('峰度')
    print(stats.Kurtosis(score))    
    return

#画图
def drawGraghic(score)
    plt.boxplot([score],labels['score'])    #箱线图
    plt.title('箱线图')
    plt.show()
    plt.hist(score,100)
    plt.show()
    return
```

















