# 机器学习知识点(三十六)分类器性能度量指标f1-score - 专注于数据挖掘算法研究和应用 - CSDN博客





2017年06月15日 00:05:01[fjssharpsword](https://me.csdn.net/fjssharpsword)阅读数：4422
所属专栏：[机器学习专栏](https://blog.csdn.net/column/details/16315.html)









在用python机器学习库scikit-learn训练模型时，常用f1-score来度量模型性能，下面回顾和学习下这个指标。




## 内容概要[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#%E5%86%85%E5%AE%B9%E6%A6%82%E8%A6%81)
- 模型评估的目的及一般评估流程
- 分类准确率的用处及其限制
- 混淆矩阵（confusion matrix）是如何表示一个分类器的性能
- 混淆矩阵中的度量是如何计算的
- 通过改变分类阈值来调整分类器性能
- ROC曲线的用处
- 曲线下面积（Area Under the Curve, AUC）与分类准确率的不同






## 1. 回顾[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#1.-%E5%9B%9E%E9%A1%BE)


模型评估可以用于在不同的模型类型、调节参数、特征组合中选择适合的模型，所以我们需要一个模型评估的流程来估计训练得到的模型对于非样本数据的泛化能力，并且还需要恰当的模型评估度量手段来衡量模型的性能表现。


对于模型评估流程而言，之前介绍了K折交叉验证的方法，针对模型评估度量方法，回归问题可以采用平均绝对误差（Mean Absolute Error）、均方误差（Mean Squared Error）、均方根误差（Root Mean Squared Error），而分类问题可以采用分类准确率和这篇文章中介绍的度量方法。









## 2. 分类准确率（Classification accuracy）[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#2.-%E5%88%86%E7%B1%BB%E5%87%86%E7%A1%AE%E7%8E%87%EF%BC%88Classification-accuracy%EF%BC%89)


这里我们使用Pima Indians Diabetes dataset，其中包含健康数据和糖尿病状态数据，一共有768个病人的数据。






In [1]:



# read the data into a Pandas DataFrameimportpandasaspdurl='https://archive.ics.uci.edu/ml/machine-learning-databases/pima-indians-diabetes/pima-indians-diabetes.data'col_names=['pregnant','glucose','bp','skin','insulin','bmi','pedigree','age','label']pima=pd.read_csv(url,header=None,names=col_names)







In [2]:



# print the first 5 rows of datapima.head()







Out[2]:


||pregnant|glucose|bp|skin|insulin|bmi|pedigree|age|label|
|----|----|----|----|----|----|----|----|----|----|
|6|148|72|35|0|33.6|0.627|50|1| |
|1|85|66|29|0|26.6|0.351|31|0| |
|8|183|64|0|0|23.3|0.672|32|1| |
|1|89|66|23|94|28.1|0.167|21|0| |
|0|137|40|35|168|43.1|2.288|33|1| |













上面表格中的label一列，1表示该病人有糖尿病，0表示该病人没有糖尿病






In [3]:



# define X and yfeature_cols=['pregnant','insulin','bmi','age']X=pima[feature_cols]y=pima.label







In [4]:



# split X and y into training and testing setsfromsklearn.cross_validationimporttrain_test_splitX_train,X_test,y_train,y_test=train_test_split(X,y,random_state=)







In [5]:



# train a logistic regression model on the training setfromsklearn.linear_modelimportLogisticRegressionlogreg=LogisticRegression()logreg.fit(X_train,y_train)







Out[5]:

LogisticRegression(C=1.0, class_weight=None, dual=False, fit_intercept=True,
          intercept_scaling=1, max_iter=100, multi_class='ovr',
          penalty='l2', random_state=None, solver='liblinear', tol=0.0001,
          verbose=0)







In [6]:



# make class predictions for the testing sety_pred_class=logreg.predict(X_test)







In [7]:



# calculate accuracyfromsklearnimportmetricsprintmetrics.accuracy_score(y_test,y_pred_class)









0.692708333333











**分类准确率**分数是指所有分类正确的百分比。

**空准确率（null accuracy）**是指当模型总是预测比例较高的类别，那么其正确的比例是多少






In [8]:



# examine the class distribution of the testing set (using a Pandas Series method)y_test.value_counts()







Out[8]:

0    130
1     62
dtype: int64







In [9]:



# calculate the percentage of onesy_test.mean()







Out[9]:

0.32291666666666669







In [10]:



# calculate the percentage of zeros-y_test.mean()







Out[10]:

0.67708333333333326







In [11]:



# calculate null accuracy(for binary classification problems coded as 0/1)max(y_test.mean(),-y_test.mean())







Out[11]:

0.67708333333333326











我们看到空准确率是68%，而分类准确率是69%，这说明该分类准确率并不是很好的模型度量方法，**分类准确率的一个缺点是其不能表现任何有关[测试](http://lib.csdn.net/base/softwaretest)数据的潜在分布。**






In [12]:



# calculate null accuracy (for multi-class classification problems)y_test.value_counts().head()/len(y_test)







Out[12]:

0    0.677083
dtype: float64











比较真实和预测的类别响应值：






In [13]:



# print the first 25 true and predicted responsesprint"True:",y_test.values[:]print"Pred:",y_pred_class[:]









True: [1 0 0 1 0 0 1 1 0 0 1 1 0 0 0 0 1 0 0 0 1 1 0 0 0]
Pred: [0 0 0 0 0 0 0 1 0 1 0 1 0 0 0 0 0 0 0 0 0 0 0 0 0]












从上面真实值和预测值的比较中可以看出，当正确的类别是0时，预测的类别基本都是0；当正确的类别是1时，预测的类别大都不是1。换句话说，该训练的模型大都在比例较高的那项类别的预测中预测正确，而在另外一中类别的预测中预测失败，而我们没法从分类准确率这项指标中发现这个问题。


分类准确率这一衡量分类器的标准比较容易理解，但是**它不能告诉你响应值的潜在分布，并且它也不能告诉你分类器犯错的类型**。接下来介绍的混淆矩阵可以识别这个问题。









## 3. 混淆矩阵[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#3.-%E6%B7%B7%E6%B7%86%E7%9F%A9%E9%98%B5)






In [14]:



# IMPORTANT: first argument is true values, second argument is predicted valuesprintmetrics.confusion_matrix(y_test,y_pred_class)









[[118  12]
 [ 47  15]]











![](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/Image/confusion_matrix.png)








- 真阳性（True Positive，TP）：指被分类器正确分类的正例数据
- 真阴性（True Negative，TN）：指被分类器正确分类的负例数据
- 假阳性（False Positive，FP）：被错误地标记为正例数据的负例数据
- 假阴性（False Negative，FN）：被错误地标记为负例数据的正例数据






In [15]:



# save confusion matrix and slice into four piecesconfusion=metrics.confusion_matrix(y_test,y_pred_class)TP=confusion[,]TN=confusion[,]FP=confusion[,]FN=confusion[,]print"TP:",TPprint"TN:",TNprint"FP:",FPprint"FN:",FN









TP: 15
TN: 118
FP: 12
FN: 47











## 4. 基于混淆矩阵的评估度量[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#4.-%E5%9F%BA%E4%BA%8E%E6%B7%B7%E6%B7%86%E7%9F%A9%E9%98%B5%E7%9A%84%E8%AF%84%E4%BC%B0%E5%BA%A6%E9%87%8F)









**准确率、识别率（Classification Accuracy）**：分类器正确分类的比例






In [16]:



print(TP+TN)/float(TP+TN+FN+FP)printmetrics.accuracy_score(y_test,y_pred_class)









0.692708333333
0.692708333333











**错误率、误分类率（Classification Error）**：分类器误分类的比例






In [17]:



print(FP+FN)/float(TP+TN+FN+FP)print-metrics.accuracy_score(y_test,y_pred_class)









0.307291666667
0.307291666667












考虑**类不平衡问题**，其中感兴趣的主类是稀少的。即数据集的分布反映负类显著地占多数，而正类占少数。故面对这种问题，需要其他的度量，评估分类器正确地识别正例数据的情况和正确地识别负例数据的情况。

**灵敏性（Sensitivity），也称为真正例识别率、召回率（Recall）**：正确识别的正例数据在实际正例数据中的百分比






In [18]:



printTP/float(TP+FN)recall=metrics.recall_score(y_test,y_pred_class)printmetrics.recall_score(y_test,y_pred_class)









0.241935483871
0.241935483871











**特效性（Specificity），也称为真负例率**：正确识别的负例数据在实际负例数据中的百分比






In [19]:



printTN/float(TN+FP)









0.907692307692











**假阳率（False Positive Rate）**：实际值是负例数据，预测错误的百分比






In [20]:



printFP/float(TN+FP)specificity=TN/float(TN+FP)print-specificity









0.0923076923077
0.0923076923077











**精度（Precision）**：看做精确性的度量，即标记为正类的数据实际为正例的百分比






In [21]:



printTP/float(TP+FP)precision=metrics.precision_score(y_test,y_pred_class)printprecision









0.555555555556
0.555555555556











**F度量（又称为F1分数或F分数）**，是使用精度和召回率的方法组合到一个度量上









度量是精度和召回率的调和均值，它赋予精度和召回率相等的权重。

度量是精度和召回率的加权度量，它赋予召回率权重是赋予精度的倍。






In [22]:



print(*precision*recall)/(precision+recall)printmetrics.f1_score(y_test,y_pred_class)









0.337078651685
0.337078651685











**总结**


混淆矩阵赋予一个分类器性能表现更全面的认识，同时它通过计算各种分类度量，指导你进行模型选择。


使用什么度量取决于具体的业务要求：
- 垃圾邮件过滤器：优先优化**精度**或者**特效性**，因为该应用对假阳性（非垃圾邮件被放进垃圾邮件箱）的要求高于对假阴性（垃圾邮件被放进正常的收件箱）的要求
- 欺诈交易检测器：优先优化**灵敏度**，因为该应用对假阴性（欺诈行为未被检测）的要求高于假阳性（正常交易被认为是欺诈）的要求









## 5. 调整分类的阈值[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#5.-%E8%B0%83%E6%95%B4%E5%88%86%E7%B1%BB%E7%9A%84%E9%98%88%E5%80%BC)






In [23]:



# print the first 10 predicted responseslogreg.predict(X_test)[:]







Out[23]:

array([0, 0, 0, 0, 0, 0, 0, 1, 0, 1], dtype=int64)







In [24]:



y_test.values[:]







Out[24]:

array([1, 0, 0, 1, 0, 0, 1, 1, 0, 0], dtype=int64)







In [25]:



# print the first 10 predicted probabilities of class membershiplogreg.predict_proba(X_test)[:,:]







Out[25]:

array([[ 0.63247571,  0.36752429],
       [ 0.71643656,  0.28356344],
       [ 0.71104114,  0.28895886],
       [ 0.5858938 ,  0.4141062 ],
       [ 0.84103973,  0.15896027],
       [ 0.82934844,  0.17065156],
       [ 0.50110974,  0.49889026],
       [ 0.48658459,  0.51341541],
       [ 0.72321388,  0.27678612],
       [ 0.32810562,  0.67189438]])











上面的输出中，第一列显示的是预测值为0的百分比，第二列显示的是预测值为1的百分比。






In [26]:



# print the first 10 predicted probabilities for class 1logreg.predict_proba(X_test)[:,]







Out[26]:

array([ 0.36752429,  0.28356344,  0.28895886,  0.4141062 ,  0.15896027,
        0.17065156,  0.49889026,  0.51341541,  0.27678612,  0.67189438])











我们看到，预测为1的和实际的类别号差别很大，所以这里有50%作为分类的阈值显然不太合理。于是我们将所有预测类别为1的百分比数据用直方图的方式形象地表示出来，然后尝试重新设置阈值。






In [27]:



# store the predicted probabilities for class 1y_pred_prob=logreg.predict_proba(X_test)[:,]







In [28]:



# allow plots to appear in the notebook%matplotlib inline
importmatplotlib.pyplotasplt







In [29]:



# histogram of predicted probabilitiesplt.hist(y_pred_prob,bins=)plt.xlim(,)plt.title('Histogram of predicted probabilities')plt.xlabel('Predicted probability of diabetes')plt.ylabel('Frequency')







Out[29]:

<matplotlib.text.Text at 0x76853b0>

















我们发现在20%-30%之间的数高达45%，故以50%作为分类阈值时，只有很少的一部分数据会被认为是类别为1的情况。我们可以将阈值调小，以改变分类器的**灵敏度和特效性**。






In [30]:



# predict diabetes if the predicted probability is greater than 0.3fromsklearn.preprocessingimportbinarizey_pred_class=binarize(y_pred_prob,0.3)[]







In [31]:



# print the first 10 predicted probabilitiesy_pred_prob[:]







Out[31]:

array([ 0.36752429,  0.28356344,  0.28895886,  0.4141062 ,  0.15896027,
        0.17065156,  0.49889026,  0.51341541,  0.27678612,  0.67189438])







In [32]:



# print the first 10 predicted classes with the lower thresholdy_pred_class[:]







Out[32]:

array([ 1.,  0.,  0.,  1.,  0.,  0.,  1.,  1.,  0.,  1.])







In [33]:



y_test.values[:]







Out[33]:

array([1, 0, 0, 1, 0, 0, 1, 1, 0, 0], dtype=int64)











从上面两组数据对比来看，效果确实改善不少






In [34]:



# previous confusion matrix (default threshold of 0.5)printconfusion









[[118  12]
 [ 47  15]]








In [35]:



# new confusion matrix (threshold of 0.3)printmetrics.confusion_matrix(y_test,y_pred_class)









[[80 50]
 [16 46]]








In [36]:



# sensitivity has increased (used to be 0.24)print/float(+)printmetrics.recall_score(y_test,y_pred_class)









0.741935483871
0.741935483871








In [37]:



# specificity has decreased (used to be 0.91)print/float(+)









0.615384615385











**总结：**
- 0.5作为阈值时默认的情况
- 调节阈值可以改变灵敏性和特效性
- 灵敏性和特效性是一对相反作用的指标
- 该阈值的调节是作为改善分类性能的最后一步，应更多去关注分类器的选择或构建更好的分类器









## 6. ROC曲线和AUC[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#6.-ROC%E6%9B%B2%E7%BA%BF%E5%92%8CAUC)


ROC曲线指受试者工作特征曲线/接收器操作特性(receiver operating characteristic，ROC)曲线, 是反映灵敏性和特效性连续变量的综合指标,是用构图法揭示敏感性和特异性的相互关系，它通过将连续变量设定出多个不同的临界值，从而计算出一系列敏感性和特异性。


ROC曲线是根据一系列不同的二分类方式（分界值或决定阈），以真正例率（也就是灵敏度）（True Positive Rate,TPR）为纵坐标，假正例率（1-特效性）（False Positive Rate,FPR）为横坐标绘制的曲线。

**ROC观察模型正确地识别正例的比例与模型错误地把负例数据识别成正例的比例之间的权衡。TPR的增加以FPR的增加为代价。ROC曲线下的面积是模型准确率的度量。**






In [38]:



# IMPORTANT: first argument is true values, second argument is predicted probabilitiesfpr,tpr,thresholds=metrics.roc_curve(y_test,y_pred_prob)plt.plot(fpr,tpr)plt.xlim([0.0,1.0])plt.ylim([0.0,1.0])plt.title('ROC curve for diabetes classifier')plt.xlabel('False Positive Rate (1 - Specificity)')plt.ylabel('True Positive Rate (Sensitivity)')plt.grid(True)





















ROC曲线上的每一个点对应于一个threshold，对于一个分类器，每个threshold下会有一个TPR和FPR。 比如Threshold最大时，TP=FP=0，对应于原点；Threshold最小时，TN=FN=0，对应于右上角的点(1,1)


正如上面所述，TPR的增加以FPR的增加为代价，所以ROC曲线可以帮助我们选择一个可以平衡灵敏性和特效性的阈值。通过ROC曲线我们没法看到响应阈值的对应关系，所以我们用下面的函数来查看。






In [39]:



# define a function that accepts a threshold and prints sensitivity and specificitydefevaluate_threshold(threshold):print'Sensitivity:',tpr[thresholds>threshold][-]print'Specificity:',-fpr[thresholds>threshold][-]







In [40]:



evaluate_threshold(0.5)









Sensitivity: 0.241935483871
Specificity: 0.907692307692








In [41]:



evaluate_threshold(0.3)









Sensitivity: 0.741935483871
Specificity: 0.615384615385












AUC（Area Under Curve）被定义为ROC曲线下的面积，也可以认为是ROC曲线下面积占单位面积的比例，显然这个面积的数值不会大于1。又由于ROC曲线一般都处于y=x这条直线的上方，所以AUC的取值范围在0.5和1之间。

**对应AUC更大的分类器效果更好。**所以AUC是衡量分类器性能的一个很好的度量，并且它不像分类准确率那样，在类别比例差别很大的情况下，依然是很好的度量手段。在欺诈交易检测中，由于欺诈案例是很小的一部分，这时分类准确率就不再是一个良好的度量，而可以使用AUC来度量。






In [42]:



# IMPORTANT: first argument is true values, second argument is predicted probabilitiesprintmetrics.roc_auc_score(y_test,y_pred_prob)









0.724565756824








In [43]:



# calculate cross-validated AUCfromsklearn.cross_validationimportcross_val_scorecross_val_score(logreg,X,y,cv=,scoring='roc_auc').mean()







Out[43]:

0.73782336182336183










## 参考资料[¶](http://nbviewer.jupyter.org/github/jasonding1354/pyDataScienceToolkits_Base/blob/master/Scikit-learn/%286%29classification_metrics.ipynb#%E5%8F%82%E8%80%83%E8%B5%84%E6%96%99)
- scikit-learn documentation: [Model evaluation](http://scikit-learn.org/stable/modules/model_evaluation.html)
- [ROC曲线-阈值评价标准](http://blog.csdn.net/abcjennifer/article/details/7359370)





[](http://blog.csdn.net/jasonding1354/article/details/50562543#)[](http://blog.csdn.net/jasonding1354/article/details/50562543#)[](http://blog.csdn.net/jasonding1354/article/details/50562543#)[](http://blog.csdn.net/jasonding1354/article/details/50562543#)[](http://blog.csdn.net/jasonding1354/article/details/50562543#)[](http://blog.csdn.net/jasonding1354/article/details/50562543#)







