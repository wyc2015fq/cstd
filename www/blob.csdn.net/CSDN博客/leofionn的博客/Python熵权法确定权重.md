
# Python熵权法确定权重 - leofionn的博客 - CSDN博客


2019年01月22日 10:30:10[leofionn](https://me.csdn.net/qq_36142114)阅读数：192


熵权法赋权是一种客观赋权方法， 在一些评价中， 通过对熵的计算确定权重， 就是根据各项评价指标值的差异程度， 确定各评价指标的权重。详细介绍及计算公式可参考文献[1]。
主要步骤包括（1）原始数据矩阵进行标准化（2）定义熵（3）定义熵权。具体步骤也可参考https://blog.csdn.net/wangh0802/article/details/53981356。这里不再赘述。这里通过Python实现计算步骤。
借用上面博客中的数据，下表是对各个科室指标考核后的评分结果。Xi为指标，ABCD..K为科室。
科室    X1    X2    X3    X4    X5    X6    X7    X8    X9
A    100    90    100    84    90    100    100    100    100
B    100    100    78.6    100    90    100    100    100    100
C    75    100    85.7    100    90    100    100    100    100
D    100    100    78.6    100    90    100    94.4    100    100
E    100    90    100    100    100    90    100    100    80
F    100    100    100    100    90    100    100    85.7    100
G    100    100    78.6    100    90    100    55.6    100    100
H    87.5    100    85.7    100    100    100    100    100    100
I    100    100    92.9    100    80    100    100    100    100
J    100    90    100    100    100    100    100    100    100
K    100    100    92.9    100    90    100    100    100    100
将上述数据保存到Excel表格中，并用xlrd读取。
Python程序如下
import numpy as np
import xlrd
\#读数据并求熵
path=u'K:\\选指标的.xlsx'
hn,nc=1,1
\#hn为表头行数,nc为表头列数
sheetname=u'Sheet3'
def readexcel(hn,nc):
data = xlrd.open_workbook(path)
table = data.sheet_by_name(sheetname)
nrows = table.nrows
data=[]
for i in range(hn,nrows):
data.append(table.row_values(i)[nc:])
return np.array(data)
def entropy(data0):
\#返回每个样本的指数
\#样本数，指标个数
n,m=np.shape(data0)
\#一行一个样本，一列一个指标
\#下面是归一化
maxium=np.max(data0,axis=0)
minium=np.min(data0,axis=0)
data= (data0-minium)*1.0/(maxium-minium)
\#\#计算第j项指标，第i个样本占该指标的比重
sumzb=np.sum(data,axis=0)
data=data/sumzb
\#对ln0处理
a=data*1.0
a[np.where(data==0)]=0.0001
\#    \#计算每个指标的熵
e=(-1.0/np.log(n))*np.sum(data*np.log(a),axis=0)
\#    \#计算权重
w=(1-e)/np.sum(1-e)
recodes=np.sum(data0*w,axis=1)
return recodes
data=readexcel(hn,nc)
grades=entropy(data)
计算的结果为：
In[32]:grades
Out[32]:
array([95.7069621 , 93.14062354, 93.17273781, 92.77037549, 95.84064938,
98.01005572, 90.20508545, 95.17203466, 95.96929203, 97.80841298,
97.021269  ])
上面的程序计算得分时用了标准化前的值×权重，这对于原始评分量纲相同时没有什么问题。
按照论文上的公式，计算得分时应该用标准化后的值×权重，这对于原始数据量纲不同时应该这样做，因此按照论文的公式计算的程序如下：
import numpy as np
import xlrd
\#读数据并求熵
path=u'K:\\选指标的.xlsx'
hn,nc=1,1
\#hn为表头行数,nc为表头列数
sheetname=u'Sheet3'
def readexcel(hn,nc):
data = xlrd.open_workbook(path)
table = data.sheet_by_name(sheetname)
nrows = table.nrows
data=[]
for i in range(hn,nrows):
data.append(table.row_values(i)[nc:])
return np.array(data)
def entropy(data0):
\#返回每个样本的指数
\#样本数，指标个数
n,m=np.shape(data0)
\#一行一个样本，一列一个指标
\#下面是归一化
maxium=np.max(data0,axis=0)
minium=np.min(data0,axis=0)
data= (data0-minium)*1.0/(maxium-minium)
\#\#计算第j项指标，第i个样本占该指标的比重
sumzb=np.sum(data,axis=0)
data=data/sumzb
\#对ln0处理
a=data*1.0
a[np.where(data==0)]=0.0001
\#    \#计算每个指标的熵
e=(-1.0/np.log(n))*np.sum(data*np.log(a),axis=0)
\#    \#计算权重
w=(1-e)/np.sum(1-e)
recodes=np.sum(data*w,axis=1)
return recodes
data=readexcel(hn,nc)
grades=entropy(data)
结果如下：
In[34]:grades
Out[34]:
array([0.08767219, 0.07639727, 0.08342572, 0.07555273, 0.08920511,
0.11506703, 0.06970125, 0.09550656, 0.09852824, 0.10232353,
0.10662037])
完。
参考文献：
[1] 倪九派, 李萍, 魏朝富,等. 基于AHP和熵权法赋权的区域土地开发整理潜力评价[J]. 农业工程学报, 2009, 25(5):202-209.
---------------------
作者：飞龟道人
原文：https://blog.csdn.net/weixin_40450867/article/details/81226705

