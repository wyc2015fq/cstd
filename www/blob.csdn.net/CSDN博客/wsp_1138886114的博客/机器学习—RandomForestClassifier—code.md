# 机器学习—RandomForestClassifier—code - wsp_1138886114的博客 - CSDN博客





2018年08月08日 08:54:00[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：100















开发环境`jupyter notebook`


```python
from sklearn.datasets import load_iris 
from sklearn import model_selection
from sklearn.ensemble import RandomForestClassifier
from sklearn.grid_search import GridSearchCV
```

```
iris=load_iris()
x=iris.data
y=iris.target 

X_train,X_test,y_train,y_test = model_selection\
                                .train_test_split(x,y,test_size=0.3,
                                random_state=123456) 

rf=RandomForestClassifier()
rf.fit(X_train,y_train)
rf.score(X_test,y_test)            #测试集上的预测准确率 

iris.feature_names                 #查看特征
rf.feature_importances_            #特征重要性

#使用gridSearchCV 查找最优参数
parameters={
            'n_estimators':[5,10,15,20,30],  #子模型个数
            'max_features':[1,2,3,4],        #每一个决策树使用的最大特征数
            'criterion':['gini','entropy']   #决策树分类的评价标准
           }
rf=RandomForestClassifier()
grid_search=GridSearchCV(rf,parameters,scoring='accuracy',cv=5)
grid_search.fit(x,y) 

grid_search.best_estimator_          #查看最优参数 
grid_search.best_score_              #正确率 
grid_search.best_params_             #最佳组合  

rf2=RandomForestClassifier(criterion='gini',max_features=2,
                           n_estimators=10)
rf2.fit(X_train,y_train)
rf2.score(X_test,y_test)
```



