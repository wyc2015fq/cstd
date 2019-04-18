# 机器学习——逻辑回归(logistic regression) - wsp_1138886114的博客 - CSDN博客





2018年05月27日 15:56:26[SongpingWang](https://me.csdn.net/wsp_1138886114)阅读数：1536
所属专栏：[机器学习算法](https://blog.csdn.net/column/details/24447.html)









大家好！ 此开篇第一个机器学习方法。 



- - - - - [一、逻辑回归原理](#一逻辑回归原理)
- [二、最大似然估计 正则化](#二最大似然估计-正则化)
- [三、逻辑回归代码（结果解释）](#三逻辑回归代码结果解释)
- [四、模型自动化调参（多分类问题/特征变换）](#四模型自动化调参多分类问题特征变换)
- [五、准确率 和 召回率 （收益曲线 -ROC曲线和AUC ）](#五准确率-和-召回率-收益曲线-roc曲线和auc)
- [六、K折交叉验证](#六k折交叉验证)








##### 一、逻辑回归原理
- 逻辑回归：
面对一个回归或者分类问题，建立代价函数，然后通过优化方法迭代求解出最优的模型参数，然后测试验证我们这个求解的模型的好坏。Logistic回归虽然名字里带“回归”，但是它实际上是一种分类方法，主要用于两分类问题（即输出只有两种，分别代表两个类别）回归模型中，y是一个定性变量，比如y=0或1，logistic方法主要应用于研究某些事件发生的概率 
![这里写图片描述](https://img-blog.csdn.net/20180728102546269?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

逻辑回归的原理和方法![这里写图片描述](https://img-blog.csdn.net/20180620112234142?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

用Sigmoid函数来拟合离散的目标值 


$P(x)=\frac{1}{1+e^{-(\beta _0+\beta_1x )}}~~~~~~~其中 \beta _0：截距 。\beta_1：系数$
![这里写图片描述](https://img-blog.csdn.net/2018072810371057?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

##### 二、最大似然估计 正则化
- 最大似然估计：
找到 $\beta _0$，$\beta_1$ 值让似然概率最大化- 目标函数 损失函数（代价函数 cost functon）：
参数估计优化的目标->损失最小化 

对数似然损失函数 (log-likehood loss function) 


$Cos(y,\beta)=\left\{\begin{matrix}-ln(P) & y=1~~~~~~~~~~~~~当 P=1 ⇨  -ln(P) = 0\\ -ln(1-P)& y=0~~~~~~~~~~~~~当 P=0 ⇨  -ln(P) = ∞\end{matrix}\right. $


$J(\beta )=\frac{1}{m}\sum _{i=1}^mCos(y,\beta )$- 对β 优化求解的算法：梯度下降
寻找让损失函数 $J(\beta )$ 取得最小值时的 β- 正则化：
损失函数中增加惩罚项：参数值越大惩罚越大–>让算法去尽量减少参数值 

损失函数 $J(\beta )$: 


$J(\beta )=\frac{1}{m}\sum _{i=1}^mCos(y,\beta ) + \frac{\lambda }{2m}\sum_{j=1}^n\beta_j ^2$

● 当模型参数 β 过多时，损失函数会很大，算法要努力减少 β 参数值来让损失函数最小。 

● λ 正则项重要参数，λ  越大惩罚越厉害，模型越欠拟合，反之则倾向过拟合。 
![这里写图片描述](https://img-blog.csdn.net/20180527154149153?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)

● L1 正则化得到稀疏的权重值（可理解为去掉特征项权值） 

● L2 正则化得到平滑的权值（可理解为减少特征项权重的比重）
##### 三、逻辑回归代码（结果解释）
- Scikit-Learn(SkLearn)
[http://scikit-learn.org/stable/index.html](http://scikit-learn.org/stable/index.html)
```php
代码：
from sklearn.linear_model import LogisticRegression  #逻辑回归
LogisticRegression(
                    penalty = '12',
                    dual = False,
                    tol = 0.0001,
                    C = 1.0,
                    fit_intercept = True,
                    intercept_scaling = 1,
                    class_weight = None,
                    random_state = None,
                    solver = 'liblinear',
                    max_iter = 100,
                    multi_class = 'ovr',
                    verbose = 0,
                    warm_state = False,
                    n_jobs = 1
                    )
一些重要的参数：
penalty：（惩罚项） str,'l1'or'l2', default:'l2'-L1和L2正则化
C： float,default:  1.0--正则化强度的倒数，越小-->越强的正则化（惩罚越大）
fit_intercept:      bool,default:True--是否拟合截距
class_weight:       dict or 'balanced',default:None--是否对样本加权
solver:             {'newton-cg','lbfgs','liblinear','sag'},\
                    default:'liblinear' ---参数优化求解算法
multi_class:        str,{'ovr','multinomial'},default:'ovr' --多分类问题处理方法
```

逻辑回归模型的属性（结果） 

```
Coeficient-系数
coef_:array,shape(n_classes,n_features)
Coeficient of the feature in the decision function
intercept_: array,shape(n_classes,)
```

逻辑回归结果解释 

```
Odd Ratio (比值 比)
In(Odds1/Odds0) = beta1
```

##### 四、模型自动化调参（多分类问题/特征变换）

```python
在 Sklearn中用 GridSearchCV 自动化调参
from sklearn.grid_search import GridSearchCV
GridSearchCV(
            esmtimator,
            param_grid,
            scoring = None,
            fit_params = None,
            n_jobs = 1,
            iid = True,
            refit = True,
            cv = None,
            verbose = 0
            pre_dispatch = '2*n_jobs',
            error_score = 'raise',
            return_train_score = True
            )
        一些重要的参数：
        esmtimator: esmtimator object --选择一个分类模型
        param_grid: dict or list of dictionaries --待搜索的所有参数组合
        scoring：--模型评估指标：accuracy，precision recall
        cv： ---K折 cross validtion的K值
```
- 4.1 多分类问题处理
将多分类问题转换成多个二分类问题 

    对每一个类训练一个逻辑回归模型，有多少个类就有多少个模型 

    用每一个模型对新数据分别进行预测，取概率最大的模型决定新数据的预测类别- 4.2 类别型特征变换
onehot 独热编码- 4.3  连续型特征变换
把连续性变量 离散化 好处： 

        非线性–> 线性 

        提升稳定性，避免异常数值影响 

        模型更易理解- 4.4 特征组合
特征组合前–对前特征离散化 

    特征组合后–维度会急剧膨胀 

    特征组合是用  高维特征 + 简单模型 来提升效果- 4.5 预测概率转换为分数
例如：芝麻信用
##### 五、[准确率 和 召回率 （收益曲线 -ROC曲线和AUC ）]()

![这里写图片描述](https://img-blog.csdn.net/20180728110756306?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
![这里写图片描述](https://img-blog.csdn.net/20180728163200353?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
##### 六、K折交叉验证

![这里写图片描述](https://img-blog.csdn.net/20180728111142379?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3dzcF8xMTM4ODg2MTE0/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)




