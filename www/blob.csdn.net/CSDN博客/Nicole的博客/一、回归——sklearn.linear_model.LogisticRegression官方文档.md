# 一、回归——sklearn.linear_model.LogisticRegression官方文档 - Nicole的博客 - CSDN博客
2018年06月07日 19:47:14[Nicole_Liang](https://me.csdn.net/weixin_39541558)阅读数：472
# 1.logistic回归
logistic回归可以用于概率预测、分类等。
# 2.sklearn.linear_model.LogisticRegression函数参数
`LogisticRegression`(penalty=’l2’, dual=False, tol=0.0001, C=1.0, fit_intercept=True, intercept_scaling=1, class_weight=None, rando
m_state=None, solver=’liblinear’, max_iter=100, multi_class=’ovr’, verbose=0, warm_start=False, n_jobs=1)[[source]](https://github.com/scikit-learn/scikit-learn/blob/a24c8b46/sklearn/linear_model/logistic.py#L962)
## 注意：
（1）在多类别划分中，如果' multi_class '选项设置为' OvR '，训练算法将使用one-vs-rest (OvR)方案;如果' multi_class '选项设置为'多项式'，则使用交叉熵损失。
（2）这个类使用‘liblinear’ library, ‘newton-cg’, ‘sag’ 和‘lbfgs’求解器实现了规范化的logistic回归。它的输入矩阵可以是密集和稀疏的矩阵；使用C-ordered arrays or CSR matrices containing 64-bit floats可以获得最佳的性能；
（3）‘newton-cg’, ‘sag’, and ‘lbfgs求解器只支持原始公式下的L2正则化；liblinear求解器同时支持L1和L2的正则化，只对L2处罚采用对偶公式。
## 参数说明：
- **penalty**: str, ‘l1’ or ‘l2’, default: ‘l2’。用来指明惩罚的标准，The ‘newton-cg’, ‘sag’ and ‘lbfgs’ solvers仅支持l2 penalties.
- **dual**: bool, default: False。Dual or primal formulation.Dual formulation只适用于 l2 penalty with liblinear solver.Prefer dual=False when n_samples > n_features.
- **tol** : float, default: 1-4。Tolerance for stopping criteria.
- **C**: float, default: 1.0。逆正则化的强度，一定要是整数，就像支持向量机一样，较小的值有职责更好的正则化。
- **fit_intercept** : bool, default: True。是否存在截距，默认存在。
- **class_weight**: dict or ‘balanced’, default: None。用于标示分类模型中各种类型的权重，可以不输入，即不考虑权重。如果输入的话可以调用balanced库计算权重，或者是手动输入各类的权重。比如对于0,1的二元模型，我们可以定义class_weight={0:0.9, 1:0.1}，这样类型0的权重为90%，而类型1的权重为10%。
- **random_state**：随机数种子，默认为无，仅在正则化优化算法为sag,liblinear时有用。
- max_iter: int, default: 100。Useful only for the newton-cg, sag and lbfgs solvers，求解的最大迭代次数
- multi_class: str, {‘ovr’, ‘multinomial’}, default: ‘ovr’。多类别问题的处理方式。'ovo'：一对一
- **verbose**：日志冗长度int：冗长度；0：不输出训练过程；1：偶尔输出； >1：对每个子模型都输出
- warm_start: bool, default: False。是否热启动，如果是，则下一次训练是以追加树的形式进行（重新使用上一次的调用作为初始化），bool：热启动，False：默认值
- n_jobs：并行数，int：个数；-1：跟CPU核数一致；1:默认值
- **solver**: {‘newton-cg’, ‘lbfgs’, ‘liblinear’, ‘sag’, ‘saga’},default: ‘liblinear’ Algorithm to use in the optimization problem.
- 对于小的数据集，, 选择 ‘liblinear’较好，‘sag’ 和‘saga’ 对于大数据集更好；
- 对于多级分类的问题，只有‘newton-cg’, ‘sag’, ‘saga’ and ‘lbfgs’，libniear只支持多元逻辑回归的OvR，不支持MvM，但MVM相对精确。
- newton-cg’, ‘lbfgs’ and ‘sag’ only handle L2 penalty, 相反地‘liblinear’ and ‘saga’ handle L1 penalty.
# 3.Attibutes
- **coef_**: 变量中的系数。shape (1, n_features) or (n_classes, n_features)
- **intercept_**：截距。shape (1,) or (n_classes,)
- **n_iter_**** ：**所有类的实际迭代次数。shape (n_classes,) or (1, )
# 4.**Methods**
- （1）[`decision_function`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.decision_function)(X)：预测样本的 confidence scores
- （2）[`densify`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.densify)()：将系数矩阵转化成密集矩阵的格式
- （3）[`fit`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.fit)(X, y[, sample_weight])：根据给出的训练数据来训练模型。用来训练LR分类器，其中X是训练样本，y是对应的标记样本。
- （4）[`get_params`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.get_params)([deep])：Get parameters for this estimator.
- （5）[`predict`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.predict)(X)：用来预测测试样本的标记，也就是分类。预测x的标签
- （6）[`predict_log_proba`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.predict_log_proba)(X)：对数概率估计
- （7）[`predict_proba`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.predict_log_proba)(X)：概率估计
- （8）[`score`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.score)(X, y[, sample_weight])：返回给定的测试数据和标签的平均精度
- （9）[`set_params`](http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LogisticRegression.html#sklearn.linear_model.LogisticRegression.set_params)(**params)：设置estimate的参数
- （10）sparsify（）：将系数矩阵转换成稀疏矩阵格式。
# 5.Methods的参数
# （1）`fit`(X, y, sample_weight=None)
Fit the model according to the given training data.
Returns:
**self** : object
> 
Returns self. 拟合模型，用来训练LR分类器，其中X是训练样本，y是对应的标记向量
`（2）get_params`(*deep=True*)[[source]](https://github.com/scikit-learn/scikit-learn/blob/a24c8b46/sklearn/base.py#L213)
Get parameters for this estimator.
Returns:
**params** : mapping of string to any
> 
Parameter names mapped to their values.
（3）`predict`(*X*)     Predict class labels for samples in X.
Returns:
**C** : array, shape = [n_samples]
> 
Predicted class label per sample.
（4）`predict_log_proba`(X)  Log of probability estimates.
The returned estimates for all classes are ordered by the label of classes.
Returns:
**T** : array-like, shape = [n_samples, n_classes]
> 
Returns the log-probability of the sample for each class in the model, where classes are ordered as they are in `self.classes_`.
（5）`predict_proba`(X)  Probability estimates.
The returned estimates for all classes are ordered by the label of classes.
For a multi_class problem, if multi_class is set to be “multinomial” the softmax function is used to find the predicted probability of each class. Else use a one-vs-rest approach, i.e calculate the probability of each class assuming it to be positive using the logistic function. and normalize these values across all the classes.
Returns:
**T** : array-like, shape = [n_samples, n_classes]
> 
Returns the probability of the sample for each class in the model, where classes are ordered as they are in `self.classes_`.
（6）`score`(X, y, sample_weight=None)：[[source]](https://github.com/scikit-learn/scikit-learn/blob/a24c8b46/sklearn/base.py#L324)
In multi-label classification, this is the subset accuracy which is a harsh metric since you require for each sample that each label set be correctly predicted.
Returns:
**score** : float
> 
Mean accuracy of self.predict(X) wrt. y.
