# 基于聚类（Kmeans）算法实现客户价值分析系统(电信运营商) - wsp_1138886114的博客 - CSDN博客





2018年09月22日 18:12:28[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：926











### 文章目录
- [一、电信运营商--客户价值分析](#_4)
- [二、使用聚类模型—分析项目需求](#_14)
- [三、聚类模型的原理和方法](#_36)
- [四、代码：](#_52)
- [4.1 数据感知](#41__53)
- [4.2 数据预处理](#42__72)
- [4.3 模型建立](#43__103)
- [4.4 概率密度图](#44__154)



 开发环境 jupyter notebook

数据集下载地址：[https://download.csdn.net/download/wsp_1138886114/10616250](https://download.csdn.net/download/wsp_1138886114/10616250)

##### 一、电信运营商–客户价值分析

从客户需求出发，了解客户需要什么，他们有怎么样的特征，

电信运营商为客户设置不同的优惠套餐

争取更多的用户：推出不同的优惠套餐

降低客户流失率

提高收入

增加 ARPU 值（average revenue per user 每个用户平均收益）

精准的市场营销策略定制
##### 二、使用聚类模型—分析项目需求

```
由于客户多，消费行为复杂，很难人工对客户打标签，这种情况下：
采用无监督学习的聚类算法更恰当

通过对客户的特征，日常消费行为进行分析，了解其偏好， 
为降低客户流失率和争取新用户提供个性化营销依据

目标客户：
	公众客户
	商业客户
	大客户

初步目标
	中高端用户
	中端用户
	离网趋势用户
	其它需求用户
	
通过聚类，将公众客户分为多个类别
聚类完成后，对分组数据的各方面做一个观察，年龄、性别、消费情况
```

##### 三、聚类模型的原理和方法
- 3.1 聚类（物以类聚，人以群分）

聚类（无监督）

分类（有监督，已经知道事务类别）
- 3.2 聚类效果评价标准（聚成几个类比较合适）

层次聚类（`hierarchical clustering`）

是一种很直观的算法，一层一层地进行，把小的cluster 逐步聚拢（agglomerative clustering））,也可以将大的cluster逐步分割（divisive cluster）。逐步聚拢用的多

`层次聚类的 dendrogram 树`（亲缘关系树状图解）
`scipy.cluster.hierarchy.linkage`进行层次聚类的时候，可以使用
`scipy.cluster.hierarchy.dendrogram`画图，画出一棵二叉树，高度表示两个后代相互之间的距离
如何切割 dendrogram 树

##### 四、代码：

###### 4.1 数据感知

```python
```python
import pandas as pd
import matplotlib.pyplot as plt
from sklearn.cluster import KMeans
from scipy.cluster.hierarchy import linkage,dendrogram

custinfo = pd.read_csv(r'.\\data\\custinfo.csv')
custcall = pd.read_csv(r'.\\data\\custcall.csv')
custcall.head()
```
```
|$~$|Customer_ID|Peak_calls|Peak_mins|OffPeak_calls|OffPeak_mins|Weekend_calls|Weekend_mins|International_mins|Nat_call_costmonth0K1001301210.58746554.47931200.0000004.381410041K1001301411.53007674.87810913.0457564.771490062K100130109.10947043.85400700.0000003.769771013K1001301210.53095654.45540400.0000004.358024034K100130119.50731944.02232700.0000003.93441302###### 4.2 数据预处理```python```python# 数据聚合:--对整个DataFrame数值求平均值,删除最后一列【month】custcall2 = custcall.groupby(custcall['Customer_ID']).mean()custcall3 = custcall2.drop('month', 1)# 数据合并data = pd.merge(custinfo,custcall3,left_on='Customer_ID',right_index=True)data.index = data['Customer_ID']data = data.drop('Customer_ID',1) # 数据探索：(mean,std,min,max,25%,50%,75%)desc = data.describe()print(desc)   gender_cnt = pd.value_counts(data['Gender'])print(gender_cnt)tariff_cnt = pd.value_counts(data['Tariff'])print(tariff_cnt)handset_cnt = pd.value_counts(data['Handset'])print(handset_cnt)for col in data.columns:    if not col in [u'Gender',u'Tariff',u'Handset']:        fig = plt.figure()        ax=fig.add_subplot(1,1,1)        data[col].hist(bins=20)        ax.set_title(col)        fig.show()``````![这里写图片描述](https://img-blog.csdn.net/20180820204420788?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)###### 4.3 模型建立```python```pythondata_feature = data.drop('Age',1)data_feature = data_feature.drop('Gender',1)data_feature = data_feature.drop('Tariff',1)data_feature = data_feature.drop('Handset',1)data_zs = 1.0*(data_feature - data_feature.mean())/data_feature.std()       #数据标准化  Z = linkage(data_zs, method = 'ward',             metric = 'euclidean')                #谱系聚类图（欧式距离）P = dendrogram(Z, 0)                             #画谱系聚类图plt.show()   k = 4                                            #聚类的类别iteration = 500                                  #聚类最大循环次数model = KMeans(n_clusters = k,                n_jobs = 1,                max_iter = iteration)             #分为k类，并发数1，数值大系统卡死model.fit(data_zs)                               #开始聚类r1 = pd.Series(model.labels_).value_counts()     #统计各个类别的数目r2 = pd.DataFrame(model.cluster_centers_)        #找出聚类中心r = pd.concat([r2, r1], axis = 1)                #横向连接（0是纵向），得到聚类中心对应的类别下的数目r.columns = list(data_zs.columns) + [u'class']   #重命名表头print(r)#类中心比较# r[cols].plot(figsize=(10,10))r2.columns = list(data_feature.columns)r2.plot(figsize=(10,10))plt.show()#详细输出原始数据及其类别res = pd.concat([data,                  pd.Series(model.labels_, index = data.index)],                 axis = 1)                              #详细输出每个样本对应的类别res.columns = list(data.columns) + [u'class']          #重命名表头res.to_excel('.\\data\\result.xls')                    #保存结果pd.crosstab(res['Tariff'],res['class'])pd.crosstab(res['Handset'],res['class'])pd.crosstab(res['Gender'],res['class'])res[[u'Age',u'class']].hist(by='class')res[u'Age'].groupby(res['class']).mean()``````![这里写图片描述](https://img-blog.csdn.net/20180820205200900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)###### 4.4 概率密度图```python```pythondef density_plot(data):                        #自定义作图函数    plt.rcParams['axes.unicode_minus'] = False #用来正常显示负号    p = data.plot(kind='kde',                   linewidth = 2,                   subplots = True,                   sharex = False,                  figsize=(10,15) )    [p[i].set_ylabel(u'密度',fontproperties='SimHei') for i in range(k)]    plt.legend()    return plt"""    看密度图的话可以看到更多的细节，但是对比效果不明显。    pd_: 概率密度图文件名前缀"""pic_output = '.\\data\\pd_'                   for i in range(k):    density_plot(data[res[u'class']==i]).savefig(u'%s%s.png' %(pic_output, i))``````![这里写图片描述](https://img-blog.csdn.net/2018082021001693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)|month|
|----|----|----|----|----|----|----|----|----|----|----|
|0|K100130|12|10.587465|5|4.479312|0|0.000000|4.381410|0|4|
|1|K100130|14|11.530076|7|4.878109|1|3.045756|4.771490|0|6|
|2|K100130|10|9.109470|4|3.854007|0|0.000000|3.769771|0|1|
|3|K100130|12|10.530956|5|4.455404|0|0.000000|4.358024|0|3|
|4|K100130|11|9.507319|4|4.022327|0|0.000000|3.934413|0|2|

###### 4.2 数据预处理

```python
```python
# 数据聚合:--对整个DataFrame数值求平均值,删除最后一列【month】
custcall2 = custcall.groupby(custcall['Customer_ID']).mean()
custcall3 = custcall2.drop('month', 1)

# 数据合并
data = pd.merge(custinfo,custcall3,left_on='Customer_ID',right_index=True)
data.index = data['Customer_ID']
data = data.drop('Customer_ID',1) 

# 数据探索：(mean,std,min,max,25%,50%,75%)
desc = data.describe()
print(desc)   

gender_cnt = pd.value_counts(data['Gender'])
print(gender_cnt)
tariff_cnt = pd.value_counts(data['Tariff'])
print(tariff_cnt)
handset_cnt = pd.value_counts(data['Handset'])
print(handset_cnt)

for col in data.columns:
    if not col in [u'Gender',u'Tariff',u'Handset']:
        fig = plt.figure()
        ax=fig.add_subplot(1,1,1)
        data[col].hist(bins=20)
        ax.set_title(col)
        fig.show()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180820204420788?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 4.3 模型建立

```python
```python
data_feature = data.drop('Age',1)
data_feature = data_feature.drop('Gender',1)
data_feature = data_feature.drop('Tariff',1)
data_feature = data_feature.drop('Handset',1)
data_zs = 1.0*(data_feature - data_feature.mean())/data_feature.std()       #数据标准化  

Z = linkage(data_zs, method = 'ward', 
            metric = 'euclidean')                #谱系聚类图（欧式距离）
P = dendrogram(Z, 0)                             #画谱系聚类图
plt.show()
   
k = 4                                            #聚类的类别
iteration = 500                                  #聚类最大循环次数

model = KMeans(n_clusters = k, 
               n_jobs = 1, 
               max_iter = iteration)             #分为k类，并发数1，数值大系统卡死
model.fit(data_zs)                               #开始聚类


r1 = pd.Series(model.labels_).value_counts()     #统计各个类别的数目
r2 = pd.DataFrame(model.cluster_centers_)        #找出聚类中心

r = pd.concat([r2, r1], axis = 1)                #横向连接（0是纵向），得到聚类中心对应的类别下的数目
r.columns = list(data_zs.columns) + [u'class']   #重命名表头
print(r)

#类中心比较
# r[cols].plot(figsize=(10,10))
r2.columns = list(data_feature.columns)
r2.plot(figsize=(10,10))
plt.show()


#详细输出原始数据及其类别
res = pd.concat([data, 
                 pd.Series(model.labels_, index = data.index)], 
                axis = 1)                              #详细输出每个样本对应的类别
res.columns = list(data.columns) + [u'class']          #重命名表头
res.to_excel('.\\data\\result.xls')                    #保存结果

pd.crosstab(res['Tariff'],res['class'])
pd.crosstab(res['Handset'],res['class'])
pd.crosstab(res['Gender'],res['class'])

res[[u'Age',u'class']].hist(by='class')
res[u'Age'].groupby(res['class']).mean()
```
```

![这里写图片描述](https://img-blog.csdn.net/20180820205200900?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

###### 4.4 概率密度图

```python
```python
def density_plot(data):                        #自定义作图函数
    plt.rcParams['axes.unicode_minus'] = False #用来正常显示负号
    p = data.plot(kind='kde', 
                  linewidth = 2, 
                  subplots = True, 
                  sharex = False,
                  figsize=(10,15) )
    [p[i].set_ylabel(u'密度',fontproperties='SimHei') for i in range(k)]
    plt.legend()
    return plt
"""
    看密度图的话可以看到更多的细节，但是对比效果不明显。
    pd_: 概率密度图文件名前缀
"""
pic_output = '.\\data\\pd_'                   
for i in range(k):
    density_plot(data[res[u'class']==i]).savefig(u'%s%s.png' %(pic_output, i))
```
```

![这里写图片描述](https://img-blog.csdn.net/2018082021001693?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)





