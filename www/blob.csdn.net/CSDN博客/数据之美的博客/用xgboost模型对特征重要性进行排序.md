# 用xgboost模型对特征重要性进行排序 - 数据之美的博客 - CSDN博客
2018年09月28日 15:01:34[看穿数据之美](https://me.csdn.net/oppo62258801)阅读数：302
**用xgboost模型对特征重要性进行排序**
在这篇文章中，你将会学习到：
- xgboost对预测模型特征重要性排序的原理（即为什么xgboost可以对预测模型特征重要性进行排序）。
- 如何绘制xgboost模型得到的特征重要性条形图。
- 如何根据xgboost模型得到的特征重要性，在scikit-learn进行特征选择。
**梯度提升算法是如何计算特征重要性的？**
使用梯度提升算法的好处是在提升树被创建后，可以相对直接地得到每个属性的重要性得分。一般来说，重要性分数，衡量了特征在模型中的提升决策树构建中价值。一个属性越多的被用来在模型中构建决策树，它的重要性就相对越高。
属性重要性是通过对数据集中的每个属性进行计算，并进行排序得到。在单个决策书中通过每个属性分裂点改进性能度量的量来计算属性重要性，由节点负责加权和记录次数。也就说一个属性对分裂点改进性能度量越大（越靠近根节点），权值越大；被越多提升树所选择，属性越重要。性能度量可以是选择分裂节点的Gini纯度，也可以是其他度量函数。
最终将一个属性在所有提升树中的结果进行加权求和后然后平均，得到重要性得分。
**绘制特征重要性**
一个已训练的xgboost模型能够自动计算特征重要性，这些重要性得分可以通过成员变量**feature_importances_**得到。可以通过如下命令打印：
`print(model.feature_importances_)`
我们可以直接在条形图上绘制这些分数，以获得数据集中每个特征的相对重要性的直观显示。例如：
- 
`# plot`
- 
`pyplot.bar(range(len(model.feature_importances_)), model.feature_importances_)`
- 
`pyplot.show()`
我们可以通过在the Pima Indians onset of diabetes 数据集上训练XGBOOST模型来演示，并从计算的特征重要性中绘制条形图。
- 
`# plot feature importance manually`
- 
`from numpy import loadtxt`
- 
`from xgboost import XGBClassifier`
- 
`from matplotlib import pyplot`
- 
`# load data`
- 
`dataset = loadtxt('pima-indians-diabetes.csv', delimiter=",")`
- 
`# split data into X and y`
- 
`X = dataset[:,0:8]`
- 
`y = dataset[:,8]`
- 
`# fit model no training data`
- 
`model = XGBClassifier()`
- 
`model.fit(X, y)`
- 
`# feature importance`
- 
`print(model.feature_importances_)`
- 
`# plot`
- 
`pyplot.bar(range(len(model.feature_importances_)), model.feature_importances_)`
- 
`pyplot.show()`
运行这个示例，首先的输出特征重要性分数：
[0.089701, 0.17109634, 0.08139535, 0.04651163, 0.10465116, 0.2026578, 0.1627907, 0.14119601]
相对重要性条形图：
![](https://img-blog.csdn.net/20180812210138369?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dhaXRpbmd6Ynk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
这种绘制的缺点在于，只显示了特征重要性而没有排序，可以在绘制之前对特征重要性得分进行排序。
通过内建的绘制函数进行特征重要性得分排序后的绘制，这个函数就是**plot_importance()**，示例如下：
- 
`# plot feature importance using built-in function`
- 
`from numpy import loadtxt`
- 
`from xgboost import XGBClassifier`
- 
`from xgboost import plot_importance`
- 
`from matplotlib import pyplot`
- 
`# load data`
- 
`dataset = loadtxt('pima-indians-diabetes.csv', delimiter=",")`
- 
`# split data into X and y`
- 
`X = dataset[:,0:8]`
- 
`y = dataset[:,8]`
- 
`# fit model no training data`
- 
`model = XGBClassifier()`
- 
`model.fit(X, y)`
- 
`# plot feature importance`
- 
`plot_importance(model)`
- 
`pyplot.show()`
运行示例得到条形图：
![](https://img-blog.csdn.net/20180812210406958?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dhaXRpbmd6Ynk=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
根据其在输入数组中的索引，特征被自动命名为f0-f7。在问题描述中手动的将这些索引映射到名称，我们可以看到，F5（身体质量指数）具有最高的重要性，F3（皮肤折叠厚度）具有最低的重要性。
**根据xgboost特征重要性得分进行特征选择**
特征重要性得分，可以用于在scikit-learn中进行特征选择。通过SelectFromModel类实现，该类采用模型并将数据集转换为具有选定特征的子集。这个类可以采取预先训练的模型，例如在整个数据集上训练的模型。然后，它可以阈值来决定选择哪些特征。当在SelectFromModel实例上调用transform()方法时，该阈值被用于在训练集和测试集上一致性选择相同特征。
在下面的示例中，我们首先在训练集上训练xgboost模型，然后在测试上评估。使用从训练数据集计算的特征重要性，然后，将模型封装在一个SelectFromModel实例中。我们使用这个来选择训练集上的特征，用所选择的特征子集训练模型，然后在相同的特征方案下对测试集进行评估。
示例：
- 
`# select features using threshold`
- 
`selection = SelectFromModel(model, threshold=thresh, prefit=True)`
- 
`select_X_train = selection.transform(X_train)`
- 
`# train model`
- 
`selection_model = XGBClassifier()`
- 
`selection_model.fit(select_X_train, y_train)`
- 
`# eval model`
- 
`select_X_test = selection.transform(X_test)`
- 
`y_pred = selection_model.predict(select_X_test)`
我们可以通过测试多个阈值，来从特征重要性中选择特征。具体而言，每个输入变量的特征重要性，本质上允许我们通过重要性来测试每个特征子集。
完整示例如下：
- 
`# use feature importance for feature selection`
- 
`from numpy import loadtxt`
- 
`from numpy import sort`
- 
`from xgboost import XGBClassifier`
- 
`from sklearn.model_selection import train_test_split`
- 
`from sklearn.metrics import accuracy_score`
- 
`from sklearn.feature_selection import SelectFromModel`
- 
`# load data`
- 
`dataset = loadtxt('pima-indians-diabetes.csv', delimiter=",")`
- 
`# split data into X and y`
- 
`X = dataset[:,0:8]`
- 
`Y = dataset[:,8]`
- 
`# split data into train and test sets`
- 
`X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.33, random_state=7)`
- 
`# fit model on all training data`
- 
`model = XGBClassifier()`
- 
`model.fit(X_train, y_train)`
- 
`# make predictions for test data and evaluate`
- 
`y_pred = model.predict(X_test)`
- 
`predictions = [round(value) for value in y_pred]`
- 
`accuracy = accuracy_score(y_test, predictions)`
- 
`print("Accuracy: %.2f%%" % (accuracy * 100.0))`
- 
`# Fit model using each importance as a threshold`
- 
`thresholds = sort(model.feature_importances_)`
- 
`for thresh in thresholds:`
- 
`# select features using threshold`
- 
`selection = SelectFromModel(model, threshold=thresh, prefit=True)`
- 
`select_X_train = selection.transform(X_train)`
- 
`# train model`
- 
`selection_model = XGBClassifier()`
- 
`selection_model.fit(select_X_train, y_train)`
- 
`# eval model`
- 
`select_X_test = selection.transform(X_test)`
- 
`y_pred = selection_model.predict(select_X_test)`
- 
`predictions = [round(value) for value in y_pred]`
- 
`accuracy = accuracy_score(y_test, predictions)`
- 
`print("Thresh=%.3f, n=%d, Accuracy: %.2f%%" % (thresh, select_X_train.shape[1], accuracy*100.0))`
运行示例，得到输出：
- 
`Accuracy: 77.95%`
- 
`Thresh=0.071, n=8, Accuracy: 77.95%`
- 
`Thresh=0.073, n=7, Accuracy: 76.38%`
- 
`Thresh=0.084, n=6, Accuracy: 77.56%`
- 
`Thresh=0.090, n=5, Accuracy: 76.38%`
- 
`Thresh=0.128, n=4, Accuracy: 76.38%`
- 
`Thresh=0.160, n=3, Accuracy: 74.80%`
- 
`Thresh=0.186, n=2, Accuracy: 71.65%`
- 
`Thresh=0.208, n=1, Accuracy: 63.78%`
我们可以看到，模型的性能通常随着所选择的特征的数量而减少。在这一问题上，可以对测试集准确率和模型复杂度做一个权衡，例如选择4个特征，接受准确率从77.95%降到76.38%。这可能是对这样一个小数据集的清洗，但对于更大的数据集和使用交叉验证作为模型评估方案可能是更有用的策略。
--------------------- 本文来自 waitingzby 的CSDN 博客 ，全文地址请点击：https://blog.csdn.net/waitingzby/article/details/81610495?utm_source=copy
