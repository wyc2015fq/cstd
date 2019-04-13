
# sklearn 下的树模型 - Zhang's Wikipedia - CSDN博客


2018年04月16日 23:16:43[Inside_Zhang](https://me.csdn.net/lanchunhui)阅读数：943



> 树模型天然会对特征进行重要性排序，以分裂数据集，构建分支；
[http://www.cnblogs.com/pinard/p/6050306.html](http://www.cnblogs.com/pinard/p/6050306.html)
[https://www.cnblogs.com/pinard/p/6056319.html](https://www.cnblogs.com/pinard/p/6056319.html)
## 0. 决策树模型
模型参数：
criterion: ”gini” or “entropy”(default=”gini”)是计算属性的gini(基尼不纯度)还是entropy(信息增益)，来选择最合适的节点。
splitter: ”best” or “random”(default=”best”)随机选择属性还是选择不纯度最大的属性，建议用默认。
max_features: 选择最适属性时划分的特征不能超过此值。当为整数时，即最大特征数；当为小数时，训练集特征数*小数；
if “auto”, then max_features=sqrt(n_features).
If “sqrt”, thenmax_features=sqrt(n_features).
If “log2”, thenmax_features=log2(n_features).
If None, then max_features=n_features.

1. 使用 Random Forest
fromsklearn.datasets import load_bostonfromsklearn.ensemble import RandomForestRegressor

boston_data = load_boston()
X = boston_data['data']
y = boston_data['target']
    \# dir(boston_data) ⇒ 查看其支持的属性为 ['DESCR','data','feature_names','target']
rf = RandomForestRegressor()
rf.fit(X, y)
print(sorted(zip(boston_data['feature_names'], map(lambda x: round(x,4), 
                                                   rf.feature_importances_)),
             key=operator.itemgetter(1),reverse=True))

