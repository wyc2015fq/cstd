# 7. train的模型标签 - littlely_ll的博客 - CSDN博客





2017年03月27日 17:03:58[littlely_ll](https://me.csdn.net/littlely_ll)阅读数：812
所属专栏：[R语言caret包应用](https://blog.csdn.net/column/details/29369.html)









[1. 简介(The caret package )](http://blog.csdn.net/littlely_ll/article/details/58592298)
[2. 可视化(The caret package)](http://blog.csdn.net/littlely_ll/article/details/58596562)
[3. 预处理(The caret package)](http://blog.csdn.net/littlely_ll/article/details/59106802)
[4. 数据分割（The caret package）](http://blog.csdn.net/littlely_ll/article/details/60467087)
[5. 模型训练和调参（The caret package）](http://blog.csdn.net/littlely_ll/article/details/62221850)
[6. 可用模型(The caret package )](http://blog.csdn.net/littlely_ll/article/details/65442670)
# 7. train的模型标签

下面是基本的模型类型或相关特征的列表。整个列表的内容是有争议的。例如，随机森林理论上使用特征选择，支持支持向量机可能不能有效地使用L2正则化等。 
**内容：**
**Accepts Case Weights Bagging Bayesian Model Binary Predictors Only Boosting Categorical Predictors Only Cost Sensitive Learning Discriminant Analysis Distance Weighted Discrimination Ensemble Model Feature Extraction Feature Selection Wrapper Gaussian Process Generalized Additive Model Generalized Linear Model Handle Missing Predictor Data Implicit Feature Selection Kernel Method L1 Regularization L2 Regularization Linear Classifier Linear Regression Logic Regression Logistic Regression Mixture Model Model Tree Multivariate Adaptive Regression Splines Neural Network Oblique Tree Ordinal Outcomes Partial Least Squares Polynomial Model Prototype Models Quantile Regression Radial Basis Function Random Forest Regularization Relevance Vector Machines Ridge Regression Robust Methods Robust Model ROC Curves Rule-Based Model Self-Organising Maps String Kernel Support Vector Machines Text Mining Tree-Based Model Two Class Only ## 7.0.1 Accepts Case Weights**序列数据的相邻类别概率模型(Adjacent Categories Probability Model)**`method = "vglmAdjCat"`类型：分类 调参：  * `parallel`（并行曲线）  * `link`（连接函数） 加载包：VGAM **Bagged CART**`method = "treebag"`类型：回归，分类 该模型没有调参 加载包：ipred, plyr, e1071 **Bagged Flexible Discriminant Analysis**`method = "bagFDA"`类别：分类 调参：  * `degree` * `nprune`加载包：earth, mda 可以使用设定模型变量重要性度量。 **Bagged MARS**`method = "bagEarth"`类型：回归，分类 调参：  * `nprune` * `degree`加载包：earth 可以使用设定模型变量重要性度量。 **Bagged MARS using gCV Pruning**`method = "bagEarthGCV"`类型：回归，分类 调参：  * `degree`加载包：earth 可以使用设定模型变量重要性度量。 **Bayesian Generalized Linear Model**`method = "bayesglm"`类型：回归，分类 没有参数调整 加载包：arm **Boosted Generalized Additive Model**`method = "gamboost"`类型：回归，分类 调参：  * `mstop`(boost迭代）  * `prune`（AIC 修剪？） 加载包：mboost, plyr 注意：`prune`选项增强了迭代次数，这是通过全部迭代由最优AIC值所决定。`?mstop`查看例子。如果不用修剪，结果会使用`mstop`调参的确切值预测。 **Boosted Generalized Linear Model**`method = "glmboost`类型：回归，分类 调参：  * `mstop`（boost迭代）  * `prune`（AIC修剪？） 加载包：plyr, mboost 可以使用设定模型变量重要性度量。注意：`prune`选项增强了迭代次数，这是通过全部迭代由最优AIC值所决定。`?mstop`查看例子。如果不用修剪，结果会使用`mstop`调参的确切值预测。 **Boosted Tree**`method = "blackboost"`类型：回归，分类 调参：  * `mstop`（#树）  * `maxdepth`（最大树深） 加载包： party, mboost, plyr **C5.0**`method = "C5.0"`类型：分类 调参：  * `trials`（#boost迭代）  * `model`（模型类型）  * `winnow`（特征选择？） 加载包：C50，plyr 可以使用设定模型变量重要性度量。 **CART**`method = "rpart"`类型：回归，分类 调参：  * `cp`（复杂度参数） 加载包：rpart 可以使用设定模型变量重要性度量。 **CART**`method = "rpart1SE"`类型：回归，分类 没有参数调整 加载包： rpart 可以使用设定模型变量重要性度量。注意：CART模型应用`rpart`函数重复相同的过程，`rpart`函数的模型复杂度由单标准差方法决定。这个过程在`train`函数重抽样内重复进行，所以能获得外部重抽样估计值。 **CART**`method = "rpart2"`类型：回归，分类 调参：  * `maxdepth`（最大树深） 加载包： rpart 可以使用设定模型变量重要性度量。 **CART or Ordinal Responses**`method = "rpartScore`类型：分类 调参：  * `cp`（复杂度参数）  * `split`（分割函数）  * `prune`（修剪度量） 加载包：rpartScore, plyr 可以使用设定模型变量重要性度量。 **CHi-squared Automated Interaction Detection**`method = "chaid"`类型：分类 调参：  * `alpha2`（融合阈值）  * `alpha3`（分割前合并阈值）  * `alpha4`（分割前合并阈值） 加载包：CHAID **Conditional Inference Random Forest**`method = cforest`类型：回归，分类 调参：  * `mtry`（#随机选取特征） 加载包： party 可以使用设定模型变量重要性度量。 **Conditional Inference Tree**`method = "ctree"`类型：回归，分类 调参：  * `mincriterion`（1-P-value阈值） 加载包： party **Conditional Inference Tree**`method = "ctree2"`类型：回归，分类 调参：  * `maxdepth`（最大树深）  * `mincriterion`（1-P-value阈值） 加载包：party **Continuation Ratio Model for Ordinal Data**`method = "vglmContRatio"`类型：分类 调参：  * `parallel` （并行曲线）  * `link`（连接函数） 加载包：VGAM **Cost-Sensitive C5.0**`method = "C5.0Cost`类型：分类 调参：  * `trials`（boost迭代）  * `model`（模型类型）  * `winnow`（特征选择）  * `cost`（代价） 加载包：C50，plyr 可以使用设定模型变量重要性度量。 **Cost-Sensitive CART**`method = "rpartCost"`类型：分类 调参：  * `cp`（复杂度参数）  * `Cost`（代价） 加载包：rpart **Cumulative Probability Model for Ordinal Data**`method = "vglmCumlative"`类型：分类 调参：  * `parallel`（并行曲线）  * `link`（连接函数） 加载包：VGAM **DeepBoost**`method = "deepboost`类型：分类 调参：  * `num_iter` （#boost迭代）  * `tree_depth`（树深）  * `beta` （L1正则化）  * `lambda`（树深正则化）  * `loss_type`（损失） 加载包：deepboost **Flexible Discriminant Analysis**`method = "fda"`类型：分类 调参：  * `degree` * `nprune`加载包：earth, mda 可以使用设定模型变量重要性度量。 **Generalized Linear Model**`method = "glm"`类型：回归，分类 没有参数调整 可以使用设定模型变量重要性度量。 **Generalized Linear Model with Stepwise Feature Selection**`method = "glmStepAIC`类型：回归，分类 没有参数调整 加载包： MASS **Linear Regression**`method = "lm"`类型：回归 调参：  * `intercept`（截距） 可以使用设定模型变量重要性度量。 **Linear Regression with Stepwise Selection**`method = "lmStepAIC`类型：回归 没有参数调整 加载包：MASS **Model Averaged Neural Network**`method = "avNNet`类型：回归，分类 调参：  * `size`（隐层节点）  * `decay`（权重衰变）  * `bag`（bag） 加载包：nnet **Negative Binomial Generalized Linear Model**`method = "glm.nb`类型：回归 调参：  * `link`（连接函数） 可以使用设定模型变量重要性度量。 **Neural Network**`method = "nnet"`类型：回归，分类 调参：  * `size`（隐层节点）  * `decay`（权重衰变） 加载包：nnet 可以使用设定模型变量重要性度量。 **Neural Networks with Feature Extraction**`method = "pcaNNet`类型：回归，分类 调参：  * `size` （隐层节点）  * `decay`（权重衰变） 加载包：nnet **Ordered Logistic or Probit Regression**`method = "polr"`类型：分类 调参：  * `method`（参数） 加载包：MASS 可以使用设定模型变量重要性度量。 **Penalized Discriminant Analysis**`method = "pda"`类型：分类 调参：  * `lambda`（收缩惩罚系数） 加载包：mda **Penalized Multinomial Regression**`method = "multinom"`类型：分类 调参：  * `decay`（权重衰减） 加载包：nnet 可以使用设定模型变量重要性度量。**内容补充：**[1]. [train Models By Tag](https://topepo.github.io/caret/train-models-by-tag.html#Logistic_Regression)**




