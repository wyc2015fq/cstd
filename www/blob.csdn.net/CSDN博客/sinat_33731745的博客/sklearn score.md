
# sklearn score - sinat_33731745的博客 - CSDN博客

2017年10月13日 11:34:30[Tao-Tao-Tao](https://me.csdn.net/sinat_33731745)阅读数：616


from sklearn import metrics
y_true = [0, 1, 2, 0, 1, 2,1]
y_pred = [0, 2, 1, 0, 0, 1,1]
a = metrics.precision_score(y_true, y_pred, average='weighted')
b = metrics.recall_score(y_true, y_pred, average='weighted')
c = metrics.f1_score(y_true, y_pred, average='weighted')
print(a,b,c,2*a*b/(a+b))




micro 为不分类别进行计算，
macro 分母为类别总量，例子中为3
sample 分母为样本总量，例子中为7
weight 按照各类别的样本量为权重进行计算

http://scikit-learn.org/stable/modules/model_evaluation.html\#multiclass-and-multilabel-classification

