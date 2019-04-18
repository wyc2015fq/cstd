# tree_DecisionTreeClassifier—决策树代码演示 - wsp_1138886114的博客 - CSDN博客





2018年08月07日 08:40:03[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1091








### 一、安装Graphviz:

##### 1.1 下载安装：[官网](http://www.graphviz.org/download/)

```
1.将graphviz安装目录（C:\Program Files (x86)\Graphviz2.38\）下的bin文件夹添加到Path环境变量中。
2.进入windows命令行界面，输入dot -version，然后按回车，如果显示graphviz的相关版本信息，则安装配置成功。
```


![这里写图片描述](https://img-blog.csdn.net/20180705141152194?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 1.2，安装Python插件

```
安装插件graphviz： pip install graphviz 
安装插件pydotplus：pip install pydotplus
```

### 二、 决策树代码演示

```python
from sklearn.datasets import load_iris # 导入类库
from sklearn import tree
import pydotplus
import sys
import os

os.environ["PATH"] += os.pathsep + 'C:/Program Files (x86)/Graphviz2.38/bin/'
                                       # 注意后面的路径是你自己的graphviz的bin目录。
iris = load_iris()                     # 载入sciki-learn的自带数据
clf = tree.DecisionTreeClassifier()    # 载入决策树分类模型
clf = clf.fit(iris.data, iris.target)  # 决策树拟合，得到模型

with open("iris.dot",'w') as f:        # 将模型存入dot文件iris.dot
    f = tree.export_graphviz(clf, out_file=f)
# 第一种方式，用graphviz的dot命令生成决策树的可视化文件
# 在命令行执行 dot -Tpdf iris.dot -o iris.pdf, 在当前目录生成决策树的可视化文件iris.pdf. 
# 打开可以看到决策树的模型图。

# 第二种方式，用pydotplus生成iris.pdf
dot_data = tree.export_graphviz(clf, out_file=None)
graph = pydotplus.graph_from_dot_data(dot_data)
graph.write_pdf("iris.pdf")
```


![这里写图片描述](https://img-blog.csdn.net/20180705135622757?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)


##### 1.2  参数优化

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



