
# sklearn.svm.SVC 参数说明 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[sklearn.svm.SVC 参数说明](https://www.cnblogs.com/eniac1946/p/7359204.html)
|Posted on|2017-08-14 17:52|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7359204)|[收藏](#)
|原文地址：|[sklearn.svm.SVC 参数说明](http://blog.csdn.net/szlcw1/article/details/52336824)
|==============================
|资源：
|[sklearn官网+DOC](http://scikit-learn.org/stable/)
|[库下载GitHub](https://github.com/scikit-learn/scikit-learn)
|==============================
|经常用到sklearn中的SVC函数，这里把文档中的参数翻译了一些，以备不时之需。
|svm分为SVC和SVR，前者用来做分类Classification后者用来做回归Regression
|本身这个函数也是基于libsvm实现的，所以在参数设置上有很多相似的地方。（PS: libsvm中的二次规划问题的解决|[算法](http://lib.csdn.net/base/datastructure)|是SMO）。
|sklearn.svm.SVC(|C=1.0|,|kernel='rbf'|,|degree=3|,|gamma='auto'|,|coef0=0.0|,|shrinking=True|,|probability=False|,
|tol=0.001|,|cache_size=200|,|class_weight=None|,|verbose=False|,|max_iter=-1|,|decision_function_shape=None|,|random_state=None|)
|参数：
|l  C：C-SVC的惩罚参数C?默认值是1.0
|（理论取值范围0~无穷大，0对应于忽视离群点，无穷大对应于“硬间隔”，C依靠经验和试验选取）
|C越大，相当于惩罚松弛变量，希望松弛变量接近0，即对误分类的惩罚增大，趋向于对训练集全分对的情况，这样对训练集|[测试](http://lib.csdn.net/base/softwaretest)|时准确率很高，但泛化能力弱。C值小，对误分类的惩罚减小，允许容错，将他们当成噪声点，泛化能力较强。
|l|kernel|：核函数，默认是rbf，可以是‘linear’, ‘poly’, ‘rbf’, ‘sigmoid’, ‘precomputed’
|0 – 线性：u'v
|1 – 多项式：(gamma*u'*v + coef0)^degree
|2 – RBF函数：exp(-gamma|u-v|^2)
|3 –sigmoid：tanh(gamma*u'*v + coef0)
|l|degree|：多项式poly函数的维度，默认是3，选择其他核函数时会被忽略。
|l|gamma|： ‘rbf’,‘poly’ 和‘sigmoid’的核函数参数。默认是’auto’，则会选择1/n_features
|l|coef0|：核函数的常数项。对于‘poly’和 ‘sigmoid’有用。
|l|probability ：|是否采用概率估计？.默认为False
|l|shrinking|：是否采用shrinking heuristic方法，默认为true
|l|tol ：|停止训练的误差值大小，默认为1e-3
|l|cache_size|：核函数cache缓存大小，默认为200
|l|class_weight|：类别的权重，字典形式传递。设置第几类的参数C为weight*C(C-SVC中的C)
|l|verbose|：允许冗余输出？
|l|max_iter|：最大迭代次数。-1为无限制。
|l|decision_function_shape|：‘ovo’, ‘ovr’ or None, default=None3
|l|random_state|：数据洗牌时的种子值，int值
|主要调节的参数有：C、kernel、degree、gamma、coef0。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
