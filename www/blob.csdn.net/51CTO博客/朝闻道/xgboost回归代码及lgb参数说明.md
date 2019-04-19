# xgboost回归代码及lgb参数说明-朝闻道-51CTO博客
了解xgboost 找到网络一个图 侵删 感谢原作者提供图
[https://pic3.zhimg.com/v2-07783eb41e619927e8911b85442b9e38_r.jpg](https://pic3.zhimg.com/v2-07783eb41e619927e8911b85442b9e38_r.jpg)
![xgboost回归代码及lgb参数说明](https://s1.51cto.com/images/blog/201806/12/23072751b0ecf10fe55384857c7e3746.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
xgboost训练回归模型很简单，按照前面的博客安装了xgboost库之后：
xgboost的参数说明如下代码：
```
params={
'booster':'gbtree',
'objective': 'multi:softmax', #多分类的问题
'num_class':10, # 类别数，与 multisoftmax 并用
'gamma':0.1,  # 用于控制是否后剪枝的参数,越大越保守，一般0.1、0.2这样子。
'max_depth':12, # 构建树的深度，越大越容易过拟合
'lambda':2,  # 控制模型复杂度的权重值的L2正则化项参数，参数越大，模型越不容易过拟合。
'subsample':0.7, # 随机采样训练样本
'colsample_bytree':0.7, # 生成树时进行的列采样
'min_child_weight':3, 
# 这个参数默认是 1，是每个叶子里面 h 的和至少是多少，对正负样本不均衡时的 0-1 分类而言
#，假设 h 在 0.01 附近，min_child_weight 为 1 意味着叶子节点中最少需要包含 100 个样本。
#这个参数非常影响结果，控制叶子节点中二阶导的和的最小值，该参数值越小，越容易 overfitting。 
'silent':0 ,#设置成1则没有运行信息输出，最好是设置为0.
'eta': 0.007, # 如同学习率
'seed':1000,
'nthread':7,# cpu 线程数
#'eval_metric': 'auc'
}
```
使用代码如下：
```
import xgboost as xgb
data_train = xgb.DMatrix(train1[feature_use].fillna(-1), label=train1['target'])
data_test = xgb.DMatrix(test1[feature_use].fillna(-1), label=test1['target'])
watch_list = [(data_test, 'eval'), (data_train, 'train')]
param = {'max_depth': 5, 'eta': 0.01, 'silent': 1, 'objective': 'reg:linear','num_boost_round':950,'subsample':0.8,'colsample_bytree':0.2319,'min_child_weight':11}
bst = xgb.train(param, data_train, num_boost_round=950, evals=watch_list)
y_pred = bst.predict(data_test)
print("xgboost success")
```
其中的 train test都为 pandas中的dataframe类型 
lgb参数说明如下：
参考中文文档路径 [http://lightgbm.apachecn.org/cn/latest/Parameters.html#id2](http://lightgbm.apachecn.org/cn/latest/Parameters.html#id2)
通过上一篇博客中的粒子群算法可以确定一个参数的范围 
然后根据数据的情况对参数进行微调
其中有几个参数尤其要注意：
num_iterations, default=100, type=int, alias=num_iteration, num_tree, num_trees, num_round, num_rounds, num_boost_round
boosting 的迭代次数
Note: 对于 Python/R 包, 这个参数是被忽略的, 使用 train and cv 的输入参数 num_boost_round (Python) or nrounds (R) 来代替
Note: 在内部, LightGBM 对于 multiclass 问题设置 num_class * num_iterations 棵树
该参数调大可以更好拟合模型 也会导致过拟合，
抽样考虑的特征数量：
feature_fraction, default=1.0, type=double, 0.0 < feature_fraction < 1.0, alias=sub_feature, colsample_bytree
如果 feature_fraction 小于 1.0, LightGBM 将会在每次迭代中随机选择部分特征. 例如, 如果设置为 0.8, 将会在每棵树训练之前选择 80% 的特征
可以用来加速训练
可以用来处理过拟合
参数之间是互相配合的 
这些参数要根据你的数据集的分布情况和大小来确定
例如《集成学习》中有个说明基学习器个数和特征维数的关系：
![xgboost回归代码及lgb参数说明](https://s1.51cto.com/images/blog/201806/12/ba3a9378e41060bb0b7b13b0e8ddb630.jpg?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
d是特征维数 m为基学习器个数 也就是 基学习器个数应该随着特征维数增多而增加。
希望多多交流 
![xgboost回归代码及lgb参数说明](https://s1.51cto.com/images/blog/201806/12/7cc59839322e6a065dd9208b7c3f245e.png?x-oss-process=image/watermark,size_16,text_QDUxQ1RP5Y2a5a6i,color_FFFFFF,t_100,g_se,x_10,y_10,shadow_90,type_ZmFuZ3poZW5naGVpdGk=)
