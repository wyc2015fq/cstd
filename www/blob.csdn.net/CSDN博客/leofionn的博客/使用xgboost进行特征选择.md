
# 使用xgboost进行特征选择 - leofionn的博客 - CSDN博客


2018年12月25日 22:27:52[leofionn](https://me.csdn.net/qq_36142114)阅读数：330


使用基于决策树的梯度提升算法的一个好处是，可以自动地获取特征的重要性，从而有效地进行特征的筛选。本文基于xgboost进行特征选择的实践
使用gradient boosting计算特征重要性
通过梯度提升的方法，我们可以根据提升之后的树获取每个特征的重要性。
一般来说，特征的重要性表示这个特征在构建提升树的作用。如果一个特征在所有树中作为划分属性的次数越多，那么该特征就越重要。通过每个属性分割点改进性能度量的量来计算单个决策树的重要性，并由节点负责的观察数量加权。性能度量可以是用于选择分裂点的纯度（基尼指数）或另一个更具体的误差函数。最后，在模型中的所有决策树中对要素重要性进行平均。最终得到每个特征的重要性,之后可以特征排序或者进行相互比较。
基于xgboost的实践
xgboost是一个流行的机器学习第三方库，提供可python的借口，可以利用xgboost轻松的获取feature importance
可以利用scikit-learn提供的类 SelectFromModel来进行特征选择，关于SelectFromModel的具体用法可以参考https://scikit-learn.org/stable/modules/generated/sklearn.feature_selection.SelectFromModel.html
在下面这个例子中，我们首先将拆分出训练集和测试集，然后在训练集上训练XGBoost模型，用测试集来验证模型的准确率。此外，基于训练XGBoost得到的feature_impoerance，通过SelectFromModel进行特征选择，并比较不同特征重要性阈值下的准确率
为特征选择＃使用功能的重要性
，从numpy的进口loadtxt
从numpy的进口排序
从xgboost进口XGBClassifier
从sklearn.model_selection进口train_test_split
从sklearn.metrics导入accuracy_score
从sklearn.feature_selection进口SelectFromModel
＃负载数据
集= loadtxt（'皮马印第安人糖尿病.csv'，delimiter =“，”）
＃将数据拆分为X和y
X =数据集[：，0：8]
Y =数据集[：，8]
＃将数据拆分为训练集和测试集
X_train，X_test，y_train，y_test = train_test_split（X，Y，test_size = 0.33，random_state = 7）
所有训练数据
模型的\#fit模型= XGBClassifier（）
model.fit（X_train，y_train）
\#make预测测试数据并评估y_pred = model.predict（X_test）
预测= [圆（值）y_pred中的值]
准确度= accuracy_score（y_test，predictions）
print（“准确度：％。2f %%”％（精度） * 100.0））
＃使用每个重要性拟合模型作为阈
值阈值= sort（model.feature_importances_）
阈值阈值：\#select
features使用阈值
选择= SelectFromModel（model，threshold = thresh，prefit = True）
select_X_train = selection.transform （X_train）\#train
model
selection_model = XGBClassifier（）
selection_model.fit（select_X_train，y_train）\#eval
model
select_X_test = selection.transform（X_test）
y_pred = selection_model.predict（select_X_test）
predictions = [y_pred中值的round（值）] precision
= accuracy_score（y_test，predictions）
print（“Thresh =％。3f，n =％d，准确度：％。2f %%“％（thresh，select_X_train.shape [1]，精度* 100.0））
结果如下所示：
精度：77.95％
Thresh = 0.071，n = 8，精度：77.95％
Thresh = 0.073，n = 7，精度：76.38％
Thresh = 0.084，n = 6，精度：77.56％
Thresh = 0.090，n = 5，精度： 76.38％
Thresh = 0.128，n = 4，精度：76.38％
Thresh = 0.160，n = 3，精度：74.80％
Thresh = 0.186，n = 2，精度：71.65％
Thresh = 0.208，n = 1，精度：63.78％
从结果中可以看出随着特征重要性阈值的增加，选择特征数量的减少，模型的准确率也在下降
我们必须在模型复杂度（特征数量）和准确率做一个权衡，但是有些情况，特征数量的减少反而会是准确率升高，因为这些被剔除特征是噪声
参考
https://machinelearningmastery.com/feature-importance-and-feature-selection-with-xgboost- in-python /
---------------------
[https://blog.csdn.net/John_xyz/article/details/85165064](https://blog.csdn.net/John_xyz/article/details/85165064)

