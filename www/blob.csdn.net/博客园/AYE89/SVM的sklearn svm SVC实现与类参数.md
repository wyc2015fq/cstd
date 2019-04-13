
# SVM的sklearn.svm.SVC实现与类参数 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[SVM的sklearn.svm.SVC实现与类参数](https://www.cnblogs.com/eniac1946/p/7373548.html)
|Posted on|2017-08-16 15:21|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7373548)|[收藏](#)
|SVC继承了父类BaseSVC
|SVC类主要方法：
|★__init__() 主要参数：
|C:|float参数 默认值为1.0
|错误项的惩罚系数。C越大，即对分错样本的惩罚程度越大，因此在训练样本中准确率越高，但是泛化能力降低，也就是对测试数据的分类准确率降低。相反，减小C的话，容许训练样本中有一些误分类错误样本，泛化能力强。对于训练样本带有噪声的情况，一般采用后者，把训练样本集中错误分类的样本作为噪声。
|kernel|: str参数 默认为‘rbf’
|算法中采用的核函数类型，可选参数有：
|‘linear’:线性核函数
|‘poly’：多项式核函数
|‘rbf’：径像核函数/高斯核
|‘sigmod’:sigmod核函数
|‘precomputed’:核矩阵
|具体这些核函数类型，请参考上一篇博客中的核函数。需要说明的是，precomputed表示自己提前计算好核函数矩阵，这时候算法内部就不再用核函数去计算核矩阵，而是直接用你给的核矩阵。核矩阵为如下形式：
![](https://images2015.cnblogs.com/blog/1035514/201704/1035514-20170412171732251-379421176.png)
|还有一点需要说明，除了上面限定的核函数外，还可以给出自己定义的核函数，其实内部就是用你自己定义的核函数来计算核矩阵。
|degree|:int型参数 默认为3
|这个参数只对多项式核函数有用，是指多项式核函数的阶数n
|如果给的核函数参数是其他核函数，则会自动忽略该参数。
|gamma|：float参数 默认为auto
|核函数系数，只对‘rbf’,‘poly’,‘sigmod’有效。
|如果gamma为auto，代表其值为样本特征数的倒数，即1/n_features.
|coef0|:float参数 默认为0.0
|核函数中的独立项，只有对‘poly’和‘sigmod’核函数有用，是指其中的参数c
|probability|：bool参数 默认为False
|是否启用概率估计。 这必须在调用fit()之前启用，并且会fit()方法速度变慢。
|shrinking|：bool参数 默认为True
|是否采用启发式收缩方式
|tol|: float参数  默认为1e^-3
|svm停止训练的误差精度
|cache_size|：float参数 默认为200
|指定训练所需要的内存，以MB为单位，默认为200MB。
|class_weight|：字典类型或者‘balance’字符串。默认为None
|给每个类别分别设置不同的惩罚参数C，如果没有给，则会给所有类别都给C=1，即前面参数指出的参数C.
|如果给定参数‘balance’，则使用y的值自动调整与输入数据中的类频率成反比的权重。
|verbose|：bool参数 默认为False
|是否启用详细输出。 此设置利用libsvm中的每个进程运行时设置，如果启用，可能无法在多线程上下文中正常工作。一般情况都设为False，不用管它。
|max_iter|：int参数 默认为-1
|最大迭代次数，如果为-1，表示不限制
|random_state|：int型参数 默认为None
|伪随机数发生器的种子,在混洗数据时用于概率估计。
|★fit()方法：用于训练SVM，具体参数已经在定义SVC对象的时候给出了，这时候只需要给出数据集X和X对应的标签y即可。
|★predict()方法：基于以上的训练，对预测样本T进行类别预测，因此只需要接收一个测试集T，该函数返回一个数组表示个测试样本的类别。
|★属性有哪些：
|svc.n_support_：各类各有多少个支持向量
|svc.support_：各类的支持向量在训练样本中的索引
|svc.support_vectors_：各类所有的支持向量
|\# -*- coding:utf-8 -*-
|from sklearn.svm import SVC
|import matplotlib.pyplot as plt
|import numpy as np
|X=np.array([[1,1],[1,2],[1,3],[1,4],[2,1],[2,2],[3,1],[4,1],[5,1],
|[5,2],[6,1],[6,2],[6,3],[6,4],[3,3],[3,4],[3,5],[4,3],[4,4],[4,5]])
|Y=np.array([1]*14+[-1]*6)
|T=np.array([[0.5,0.5],[1.5,1.5],[3.5,3.5],[4,5.5]])
|svc=SVC(kernel='poly',degree=2,gamma=1,coef0=0)
|svc.fit(X,Y)
|pre=svc.predict(T)
|print pre
|print svc.n_support_
|print svc.support_
|print svc.support_vectors_
|运行结果：
|[ 1  1 -1 -1]      \#预测结果
|[2 3]            \#-1类和+1类分别有2个和3个支持向量
|[14 17  3  5 13] \#-1类支持向量在元训练集中的索引为14,17，同理-1类支持向量在元训练集中的索引为3,5,13
|[[ 3.  3.]    \#给出各支持向量具体是哪些，前两个是-1类的
|[ 4.  3.]
|[ 1.  4.]   \#后3个是+1的支持向量
|[ 2.  2.]
|[ 6.  4.]]
![](https://images2015.cnblogs.com/blog/1035514/201704/1035514-20170412172021345-2078431821.png)
|结果如图所示。
![](https://images2015.cnblogs.com/blog/1035514/201704/1035514-20170411192029001-1818954008.png)
|\#参数的网格扫描
|\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#\#
\# Train a SVM classification model
print("Fitting the classifier to the training set")
t0 = time()
param_grid = {'C': [1e3, 5e3, 1e4, 5e4, 1e5],
              'gamma': [0.0001, 0.0005, 0.001, 0.005, 0.01, 0.1], }
clf = GridSearchCV(SVC(kernel='rbf', class_weight='balanced'), param_grid)
clf = clf.fit(X_train_pca, y_train)
print("done in %0.3fs" % (time() - t0))
print("Best estimator found by grid search:")
print(clf.best_estimator_)







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
