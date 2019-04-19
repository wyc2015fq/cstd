# XGBoost：在Python中使用XGBoost - 数据之美的博客 - CSDN博客
2017年07月25日 10:03:48[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：748
# 在Python中使用XGBoost
下面将介绍XGBoost的[Python](http://lib.csdn.net/base/python)模块，内容如下：
* [编译及导入Python模块](http://blog.csdn.net/zc02051126/article/details/46771793#%E7%BC%96%E8%AF%91%E5%8F%8A%E5%AF%BC%E5%85%A5Python%E6%A8%A1%E5%9D%97)
* [数据接口](http://blog.csdn.net/zc02051126/article/details/46771793#%E6%95%B0%E6%8D%AE%E6%8E%A5%E5%8F%A3)
* [参数设置](http://blog.csdn.net/zc02051126/article/details/46771793#setting-parameters)
* [训练模型l](http://blog.csdn.net/zc02051126/article/details/46771793#training-model)
* [提前终止程序](http://blog.csdn.net/zc02051126/article/details/46771793#early-stopping)
* [预测](http://blog.csdn.net/zc02051126/article/details/46771793#prediction)
A [walk through python example](https://github.com/tqchen/xgboost/blob/master/demo/guide-python) for UCI Mushroom dataset is provided.
### 安装
首先安装XGBoost的C++版本，然后进入源文件的根目录下的 `wrappers`文件夹执行如下脚本安装[python](http://lib.csdn.net/base/python)模块
`python setup.py install`- 1
- 1
安装完成后按照如下方式导入XGBoost的Python模块
```python
import xgboost as xgb
```
- 1
- 1
=
### 数据接口
XGBoost可以加载libsvm格式的文本数据，加载的数据格式可以为Numpy的二维数组和XGBoost的二进制的缓存文件。加载的数据存储在对象`DMatrix`中。
- 加载libsvm格式的数据和二进制的缓存文件时可以使用如下方式
```python
dtrain = xgb.DMatrix('train.svm.txt')
dtest = xgb.DMatrix('test.svm.buffer')
```
- 1
- 2
- 1
- 2
- 加载numpy的数组到`DMatrix`对象时，可以用如下方式
```python
data = np.random.rand(5,10) # 5 entities, each contains 10 features
label = np.random.randint(2, size=5) # binary target
dtrain = xgb.DMatrix( data, label=label)
```
- 1
- 2
- 3
- 1
- 2
- 3
- 将`scipy.sparse`格式的数据转化为 `DMatrix`格式时，可以使用如下方式
```python
csr = scipy.sparse.csr_matrix( (dat, (row,col)) )
dtrain = xgb.DMatrix( csr )
```
- 1
- 2
- 1
- 2
- 将 `DMatrix` 格式的数据保存成XGBoost的二进制格式，在下次加载时可以提高加载速度，使用方式如下
```python
dtrain = xgb.DMatrix('train.svm.txt')
dtrain.save_binary("train.buffer")
```
- 1
- 2
- 1
- 2
- 可以用如下方式处理 `DMatrix`中的缺失值：
```python
dtrain = xgb.DMatrix( data, label=label, missing = -999.0)
```
- 1
- 1
- 当需要给样本设置权重时，可以用如下方式
```python
w = np.random.rand(5,1)
dtrain = xgb.DMatrix( data, label=label, missing = -999.0, weight=w)
```
- 1
- 2
- 1
- 2
### 参数设置
XGBoost使用key-value格式保存[参数](http://blog.csdn.net/zc02051126/article/details/parameter.md). Eg
* Booster（基本学习器）参数
```python
param = {'bst:max_depth':2, 'bst:eta':1, 'silent':1, 'objective':'binary:logistic' }
param['nthread'] = 4
plst = param.items()
plst += [('eval_metric', 'auc')] # Multiple evals can be handled in this way
plst += [('eval_metric', 'ams@0')]
```
- 1
- 2
- 3
- 4
- 5
- 1
- 2
- 3
- 4
- 5
- 还可以定义验证数据集，验证算法的性能
```python
evallist  = [(dtest,'eval'), (dtrain,'train')]
```
- 1
- 1
=
### 训练模型
有了参数列表和数据就可以训练模型了 
* 训练
```python
num_round = 10
bst = xgb.train( plst, dtrain, num_round, evallist )
```
- 1
- 2
- 1
- 2
- 保存模型 
在训练完成之后可以将模型保存下来，也可以查看模型内部的结构
```python
bst.save_model('0001.model')
```
- 1
- 1
- Dump Model and Feature Map 
You can dump the model to txt and review the meaning of model
```python
# dump model
bst.dump_model('dump.raw.txt')
# dump model with feature map
bst.dump_model('dump.raw.txt','featmap.txt')
```
- 1
- 2
- 3
- 4
- 1
- 2
- 3
- 4
- 加载模型 
通过如下方式可以加载模型
```python
bst = xgb.Booster({'nthread':4}) #init model
bst.load_model("model.bin") # load data
```
- 1
- 2
- 1
- 2
=
### 提前终止程序
如果有评价数据，可以提前终止程序，这样可以找到最优的迭代次数。如果要提前终止程序必须至少有一个评价数据在参数`evals`中。 If there’s more than one, it will use the last.
`train(..., evals=evals, early_stopping_rounds=10)`
The model will train until the validation score stops improving. Validation error needs to decrease at least every
`early_stopping_rounds` to continue training.
If early stopping occurs, the model will have two additional fields: `bst.best_score` and
`bst.best_iteration`. Note that `train()` will return a model from the last iteration, not the best one.
This works with both metrics to minimize (RMSE, log loss, etc.) and to maximize (MAP, NDCG, AUC).
=
#### Prediction
After you training/loading a model and preparing the data, you can start to do prediction.
```python
data = np.random.rand(7,10) # 7 entities, each contains 10 features
dtest = xgb.DMatrix( data, missing = -999.0 )
ypred = bst.predict( xgmat )
```
- 1
- 2
- 3
- 1
- 2
- 3
If early stopping is enabled during training, you can predict with the best iteration.
```python
ypred = bst.predict(xgmat,ntree_limit=bst.best_iteration)
```
