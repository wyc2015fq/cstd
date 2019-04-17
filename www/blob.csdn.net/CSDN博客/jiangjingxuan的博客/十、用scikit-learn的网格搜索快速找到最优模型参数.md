# 十、用scikit-learn的网格搜索快速找到最优模型参数 - jiangjingxuan的博客 - CSDN博客





2017年01月25日 10:22:32[jiangjingxuan](https://me.csdn.net/jiangjingxuan)阅读数：2598












![](http://www.shareditor.com/uploads/media/default/0001/01/thumb_136_default_big.jpeg)



任何一种机器学习模型都附带很多参数，不同场景对应不同的最佳参数，手工尝试各种参数无疑浪费很多时间，scikit-learn帮我们实现了自动化，那就是网格搜索

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

## 网格搜索

这里的网格指的是不同参数不同取值交叉后形成的一个多维网格空间。比如参数a可以取1、2，参数b可以取3、4，参数c可以取5、6，那么形成的多维网格空间就是：

```
1、3、5
1、3、6
1、4、5
1、4、6
2、3、5
2、3、6
2、4、5
2、4、6
```

一共2*2*2=8种情况

网格搜索就是遍历这8种情况进行模型训练和验证，最终选择出效果最优的参数组合



## 用法举例

```python
# coding:utf-8

import sys
reload(sys)
sys.setdefaultencoding( "utf-8" )

from sklearn.feature_extraction.text import TfidfVectorizer
from sklearn.linear_model.logistic import LogisticRegression
from sklearn.grid_search import GridSearchCV
from sklearn.pipeline import Pipeline

# 构造样本，这块得多构造点，不然会报class不足的错误，因为gridsearch会拆分成小组
X = []
X.append("fuck you")
X.append("fuck you all")
X.append("hello everyone")
X.append("fuck me")
X.append("hello boy")
X.append("fuck you")
X.append("fuck you all")
X.append("hello everyone")
X.append("fuck me")
X.append("hello boy")
X.append("fuck you")
X.append("fuck you all")
X.append("hello everyone")
X.append("fuck me")
X.append("hello boy")
X.append("fuck you")
X.append("fuck you all")
X.append("hello everyone")
X.append("fuck me")
X.append("hello boy")
X.append("fuck you")
X.append("fuck you all")
X.append("hello everyone")
X.append("fuck me")
X.append("hello boy")

y = [1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,0,1,1,0,1,0,1]

# 这是执行的序列，gridsearch是构造多进程顺序执行序列并比较结果
# 这里的vect和clf名字自己随便起，但是要和parameters中的前缀对应
pipeline = Pipeline([
    ('vect', TfidfVectorizer(stop_words='english')),
    ('clf', LogisticRegression())
    ])

# 这里面的max_features必须是TfidfVectorizer的参数, 里面的取值就是子进程分别执行所用
parameters = {
        'vect__max_features': (3, 5),
        }

# accuracy表示按精确度判断最优值
grid_search = GridSearchCV(pipeline, parameters, n_jobs = -1, verbose = 1, scoring = 'accuracy', cv = 3)
grid_search.fit(X, y)

print '最佳效果: %0.3f' % grid_search.best_score_
print '最优参数组合: '
best_parameters = grid_search.best_estimator_.get_params()
for param_name in sorted(parameters.keys()):
    print('\t%s: %r' % (param_name, best_parameters[param_name]))
```

请尊重原创，转载请注明来源网站[www.shareditor.com](http://www.shareditor.com)以及原始链接地址

执行结果如下：

```
Fitting 3 folds for each of 2 candidates, totalling 6 fits
[Parallel(n_jobs=-1)]: Done   7 out of   6 | elapsed:    0.0s remaining:   -0.0s
[Parallel(n_jobs=-1)]: Done   7 out of   6 | elapsed:    0.1s remaining:   -0.0s
[Parallel(n_jobs=-1)]: Done   7 out of   6 | elapsed:    0.1s remaining:   -0.0s
[Parallel(n_jobs=-1)]: Done   7 out of   6 | elapsed:    0.1s remaining:   -0.0s
[Parallel(n_jobs=-1)]: Done   7 out of   6 | elapsed:    0.1s remaining:   -0.0s
[Parallel(n_jobs=-1)]: Done   6 out of   6 | elapsed:    0.1s finished
最佳效果: 0.800
最优参数组合:
    vect__max_features: 3
```



这里面并行启动了6个任务，最终判断出max_features的最优解值是3




