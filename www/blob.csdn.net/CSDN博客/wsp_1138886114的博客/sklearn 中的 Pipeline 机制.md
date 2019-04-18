# sklearn 中的 Pipeline 机制 - wsp_1138886114的博客 - CSDN博客





2018年07月25日 21:29:04[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：265








### 一 sklearn 中的 Pipeline 机制

管道机制在机器学习算法中得以应用：参数集在新数据集（比如测试集）上的重复使用。

管道机制实现了流式化封装和管理（streaming workflows with pipelines）。 
- 主要有两点好处：

直接调用fit和predict方法来**对pipeline中的所有算法模型进行训练和预测。**
可以结合grid search对参数进行选择

##### 1. 加载数据集并拆分

```python
from sklearn.pipeline import Pipeline
import pandas as pd
from sklearn.cross_validation import train_test_split
from sklearn.preprocessing import LabelEncoder

df = pd.read_csv('/dataset.csv', header=None)
X, y = df.values[:, 2:], df.values[:, 1]  # y为标签

encoder = LabelEncoder()
y = encoder.fit_transform(y)
>>> encoder.transform(['M', 'B'])
array([1, 0])
X_train, X_test, y_train, y_test = train_test_split(X, y,
                                                    test_size=.2,  
                                                    random_state=0)
```

##### 2. 构思算法的流程

```
Pipeline中的步骤可能有：
    1. 特征标准化是需要的，可作为第一个环节
    2. 既然是分类器，classifier也是少不了的，自然是最后一个环节
    3. 中间可加上比如数据降维（PCA）
```

```python
from sklearn.preprocessing import StandardScaler
from sklearn.decomposition import PCA
from sklearn.linear_model import LogisticRegression
from sklearn.pipeline import Pipeline

pipe_lr = Pipeline([('sc', StandardScaler()),       #标准定标器
                    ('pca', PCA(n_components=2)),   
                    ('clf', LogisticRegression(random_state=1))
                    ])
pipe_lr.fit(X_train, y_train)
print('Test accuracy: %.3f' % pipe_lr.score(X_test, y_test))
```

```
Pipeline对象接受二元tuple构成的list， 
我们用以获取（access）Pipeline object 中的 individual elements， 

第一个元素为： arbitrary identifier string， 
第二个元素是： scikit-learn与之相适配的transformer 或者 estimator。
```

```python
----------------PCA降维，SVC支持向量机---------------
from sklearn.pipeline import Pipeline
from sklearn.svm import SVC
from sklearn.decomposition import PCA

#评估器中定义了两个模型（PCA降维，SVC拟合训练）。
estimators = [('reduce_dim', PCA()), ('svm', SVC())]
clf = Pipeline(estimators)
clf 

Pipeline(steps=[('reduce_dim',  
                        PCA(copy=True, n_components=None,whiten=False)),  
                ('svm',  
                        SVC(C=1.0, cache_size=200, class_weight=None, 
                        coef0=0.0, decision_function_shape=None, degree=3,  
                        gamma='auto',kernel='rbf', max_iter=-1, 
                        probability=False, random_state=None,shrinking=True, 
                        tol=0.001, verbose=False)) 
                ])
```

##### 3. Pipeline执行流程的分析

Pipeline 的中间过程由scikit-learn相适配的转换器（transformer）构成， 最后一步是一个estimator。  

比如上述的代码，StandardScaler和PCA transformer 构成intermediate steps，LogisticRegression 作为最终的estimator。

当我们执行 pipe_lr.fit(X_train, y_train)时，  

首先由StandardScaler在训练集上执行 fit和transform方法，  

transformed后的数据又被传递给Pipeline对象的下一步，也即PCA()。  

和StandardScaler一样，PCA也是执行fit和transform方法，  

最终将转换后的数据传递给 LosigsticRegression。整个流程如下图所示：  
![这里写图片描述](https://img-blog.csdn.net/20180725205835637?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)



