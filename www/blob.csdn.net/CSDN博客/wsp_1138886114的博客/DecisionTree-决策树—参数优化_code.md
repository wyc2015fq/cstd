# DecisionTree-决策树—参数优化_code - wsp_1138886114的博客 - CSDN博客





2018年08月06日 21:39:45[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：365














```python
from sklearn import tree
from sklearn import model_selection
from sklearn.datasets import load_iris
from sklearn.grid_search import GridSearchCV

from sklearn.metrics import confusion_matrix
from sklearn.metrics import precision_score
from sklearn.metrics import recall_score
from sklearn.metrics import f1_score
from sklearn.metrics import accuracy_score
from sklearn.metrics import classification_report
```

```
iris=load_iris()
x=iris.data
y=iris.target 

X_train,X_test,y_train,y_test = model_selection\
                                .train_test_split(x,y,test_size=0.3,
                                                 random_state=123456) 

parameters={
            'criterion':['gini','entropy'],
            'max_depth':[1,2,3,4,5,6,7,8]
            }
dtree=tree.DecisionTreeClassifier()
grid_search=GridSearchCV(dtree,parameters,scoring='accuracy',cv=5)
grid_search.fit(x,y) 

grid_search.best_estimator_   #查看grid_search方法 
grid_search.best_score_       #正确率 
grid_search.best_params_      #最佳 参数组合  

dtree=tree.DecisionTreeClassifier(criterion='gini',max_depth=5)
dtree.fit(X_train,y_train)   
pred=dtree.predict(X_test)

print(classification_report(y_test,pred))
```



