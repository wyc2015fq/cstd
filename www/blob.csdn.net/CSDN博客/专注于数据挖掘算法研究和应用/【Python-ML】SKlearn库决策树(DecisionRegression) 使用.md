# 【Python-ML】SKlearn库决策树(DecisionRegression) 使用 - 专注于数据挖掘算法研究和应用 - CSDN博客





2018年01月15日 16:47:51[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：610
所属专栏：[python专栏](https://blog.csdn.net/column/details/16333.html)









```python
# -*- coding: utf-8 -*-
'''
Created on 2018年1月15日
@author: Jason.F
@summary: Scikit-Learn库决策树算法
'''

from sklearn import datasets
import numpy as np
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import StandardScaler
from matplotlib.colors import ListedColormap
import matplotlib.pyplot as plt
from sklearn.svm import SVC
from sklearn.metrics import accuracy_score
from sklearn.tree import DecisionTreeClassifier
from sklearn.tree import export_graphviz 
from sklearn.ensemble import RandomForestClassifier
#决策边界函数
def plot_decision_regions(X,y,classifier,test_idx=None,resolution=0.02):
    # 设置标记点和颜色
    markers = ('s','x','o','^','v')
    colors = ('red', 'blue', 'lightgreen', 'gray', 'cyan')
    cmap = ListedColormap(colors[:len(np.unique(y))])

    # 绘制决策面
    x1_min, x1_max = X[:, 0].min() - 1, X[:, 0].max() + 1
    x2_min, x2_max = X[:, 1].min() - 1, X[:, 1].max() + 1
    xx1, xx2 = np.meshgrid(np.arange(x1_min, x1_max, resolution),np.arange(x2_min, x2_max, resolution))
    Z = classifier.predict(np.array([xx1.ravel(), xx2.ravel()]).T)
    Z = Z.reshape(xx1.shape)
    plt.contourf(xx1, xx2, Z, alpha=0.4, cmap=cmap)
    plt.xlim(xx1.min(), xx1.max())
    plt.ylim(xx2.min(), xx2.max())
    #绘制所有样本
    X_test,y_test=X[test_idx,:],y[test_idx]
    for idx,cl in enumerate(np.unique(y)):
        plt.scatter(x=X[y==cl,0],y=X[y==cl,1],alpha=0.8,c=cmap(idx),marker=markers[idx],label=cl)
    #高亮预测样本
    if test_idx:
        X_test,y_test =X[test_idx,:],y[test_idx]
        plt.scatter(X_test[:,0],X_test[:,1],c='',alpha=1.0,linewidths=1,marker='o',s=55,label='test set')
#数据导入
iris=datasets.load_iris()
X=iris.data[:,[2,3]]
y=iris.target
print (np.unique(y))
#训练集和测试集划分
X_train,X_test,y_train,y_test=train_test_split(X,y,test_size=0.3,random_state=0)
#标准化
sc=StandardScaler()
sc.fit(X_train)#计算样本的均值和标准差
X_train_std=sc.transform(X_train)
X_test_std=sc.transform(X_test)
#决策树模型，信息增益和纯度(熵、基尼系统、误分类率)
#深度越大，容易产生过拟合，通过剪枝来解决
tree=DecisionTreeClassifier(criterion='entropy',max_depth=3,random_state=0)
tree.fit(X_train_std,y_train)
#随机森林，集成多个弱学习器成鲁棒性强学习器
#参数：n_jobs处理器内核数量，n_estimators集成的单颗决策树数量
#forest=RandomForestClassifier(criterion='entropy',n_estimators=10,n_jobs=2,random_state=1)
#forest.fit(X_train_std,y_train)
#模型预测
y_pred=tree.predict(X_test_std)
print ('Accuracy:%.2f' %accuracy_score(y_test,y_pred))#准确率
#绘制决策边界
X_combined_std=np.vstack((X_train_std,X_test_std))
y_combined=np.hstack((y_train,y_test))
plot_decision_regions(X=X_combined_std, y=y_combined, classifier=tree, test_idx=range(105,150))
plt.xlabel('petal length[cm]')
plt.ylabel('petal width[cm]')
plt.legend(loc='upper left')
plt.show()
#导出决策树到dot格式
export_graphviz(tree,out_file='tree.dot',feature_names=['petal length','petal width'])
#下载http://www.graphviz.org/download/
#dot转换为png命令：dot -Tpng tree.dot -o tree.png
#windows下安装graphviz参考；http://blog.csdn.net/lanchunhui/article/details/49472949

#观察熵、基尼、误分类率对纯度的影响
def gini(p):
    return p*(1-p)+(1-p)*(1-(1-p))
def entropy(p):
    return -p*np.log2(p)-(1-p)*np.log2(1-p)
def error(p):
    return 1-np.max([p,1-p])
x=np.arange(0.0,1.0,0.01)
ent=[entropy(p) if p !=0 else None for p in x]#求熵
sc_ent=[e*0.5 if e else None for e in ent ]#熵缩放
err=[error(i) for i in x]
fig=plt.figure()
ax=plt.subplot(111)
for i,lab,ls,c in zip([ent,sc_ent,gini(x),err],\
                      ['Entropy','Entropy(scaled)','Gini Impurity','Misclassification Error'],\
                      ['-','-','--','-.'],\
                      ['black','lightgray','red','green']):
    line=ax.plot(x,i,label=lab,linestyle=ls,lw=2,color=c)
ax.legend(loc='upper center',bbox_to_anchor=(0.5,1.15),ncol=3,fancybox=True,shadow=False)
ax.axhline(y=0.5,linewidth=1,color='k',linestyle='--')
ax.axhline(y=1.0,linewidth=1,color='k',linestyle='--')
plt.ylim([0,1.1])
plt.xlabel('p(i=1)')
plt.ylabel('Impurity Index')
plt.show()
```

决策分类结果：

![](https://img-blog.csdn.net/20180115164617594?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

决策分类GraphViz图：

![](https://img-blog.csdn.net/20180115164641896?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

随机森林结果：

![](https://img-blog.csdn.net/20180115164706223?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)

信息增益：熵、基尼、误分类率对纯度的影响：

![](https://img-blog.csdn.net/20180115164740712?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvZmpzc2hhcnBzd29yZA==/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)




