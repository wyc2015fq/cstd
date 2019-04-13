
# sklearn安装 - AYE89 - 博客园|[AYE89](https://www.cnblogs.com/eniac1946/)
|coding & learning|
|posts - 153, comments - 3, trackbacks - 0, articles - 2

|
|导航
|[博客园](https://www.cnblogs.com/)|[首页](https://www.cnblogs.com/eniac1946/)|[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)|[联系](https://msg.cnblogs.com/send/AYE89)![订阅](//www.cnblogs.com/images/xml.gif)|[订阅](https://www.cnblogs.com/eniac1946/rss)|[管理](https://i.cnblogs.com/)
|公告


|[sklearn安装](https://www.cnblogs.com/eniac1946/p/7364643.html)
|Posted on|2017-08-15 13:36|[AYE89](https://www.cnblogs.com/eniac1946/)|阅读(|...|) 评论(|...|)|[编辑](https://i.cnblogs.com/EditPosts.aspx?postid=7364643)|[收藏](#)
|1、准备工作
|安装|sklearn|之前，我们需要先安装|numpy|，|scipy|函数库。
|由于用pip install numpy, scipy大概率安装失败（Win下），一个简单的解决方法是安装UCI第三方库——
|教程参见|[Python安装SciPy](http://blog.csdn.net/riverflowrand/article/details/52768489)
|用到，「非官方的Windows二进制文件|[Python](http://lib.csdn.net/base/python)|扩展包（Unofficial Windows Binaries for|[python](http://lib.csdn.net/base/python)|Extension Packages）」
|http://www.lfd.uci.edu/~gohlke/pythonlibs/\#numpy
|[http://www.lfd.uci.edu/~gohlke/pythonlibs/\#scipy ](http://www.lfd.uci.edu/~gohlke/pythonlibs/#scipy)
|\#安装完成后，
|然后可以在python下执行相应的测试程序：
|python
|>>>|import numpy
|>>>|import scipy
|>>>|numpy.test('full')
|等待。。。。
|>>>|scipy.test('full')
|2、安装sklearn机器学习库
|下载地址：|[https://github.com/scikit-learn/scikit-learn](https://github.com/scikit-learn/scikit-learn)
|下载压缩包之后，解该压缩包压缩。使用CMD进入sklearn文件夹，执行
|pip install -U scikit-learn
|\#参数-U：|pip install -h|就有说明了，就是|--upgrade|，意思是如果已安装就升级到最新版。|3. scikit-learn库简介
|[http://scikit-learn.org/stable/](http://scikit-learn.org/stable/)
|建立在NumPy，SciPy和matplotlib上，Python中的机器学习库，简单高效的数据挖掘和数据分析工具。
|功能有分类（SVM,nearest neighbors,random forest等），回归（SVR, ridge regression, Lasso等），聚类（k-Means, spectral clustering, mean-shift等），降维（PCA, feature selection, non-negative matrix factorization等），选择模型（grid search, cross validation, metrics等），预处理（preprocessing, feature extraction等）。







|[刷新评论](javascript:void(0);)|[刷新页面](#)|[返回顶部](#top)






|
|Powered by:
|博客园
|Copyright © AYE89
|
