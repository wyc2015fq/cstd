# 机器学习十大算法总览（含Python3.X和R语言代码） - BlueSky - CSDN博客
2017年11月17日 14:46:00[ASIA_kobe](https://me.csdn.net/ASIA_kobe)阅读数：613
- - [引言](http://blog.csdn.net/kicilove/article/details/78423555#引言)
- [一监督学习](http://blog.csdn.net/kicilove/article/details/78423555#一监督学习)
- [二无监督学习](http://blog.csdn.net/kicilove/article/details/78423555#二无监督学习)
- [三强化学习](http://blog.csdn.net/kicilove/article/details/78423555#三强化学习)
- [四通用机器学习算法列表](http://blog.csdn.net/kicilove/article/details/78423555#四通用机器学习算法列表)- [线性回归Linear Regression](http://blog.csdn.net/kicilove/article/details/78423555#1线性回归linear-regression)
- [逻辑回归Logistic Regression](http://blog.csdn.net/kicilove/article/details/78423555#2-逻辑回归logistic-regression)
- [决策树Decision Tree](http://blog.csdn.net/kicilove/article/details/78423555#3决策树decision-tree)
- [支持向量机SVM Support Vector Machine](http://blog.csdn.net/kicilove/article/details/78423555#4-支持向量机svm-support-vector-machine)
- [朴素贝叶斯Naive Bayes](http://blog.csdn.net/kicilove/article/details/78423555#5-朴素贝叶斯naive-bayes)
- [K近邻KNN K- Nearest Neighbors](http://blog.csdn.net/kicilove/article/details/78423555#6-k近邻knn-k-nearest-neighbors)
- [K均值K-Means](http://blog.csdn.net/kicilove/article/details/78423555#7-k均值k-means)- [K-means如何形成群类](http://blog.csdn.net/kicilove/article/details/78423555#k-means如何形成群类)
- [随机森林Random Forest](http://blog.csdn.net/kicilove/article/details/78423555#8-随机森林random-forest)
- [降维算法Dimensionality Reduction Algorithms](http://blog.csdn.net/kicilove/article/details/78423555#9-降维算法dimensionality-reduction-algorithms)
- [梯度提升算法Gradient Boosting Algorithms](http://blog.csdn.net/kicilove/article/details/78423555#10-梯度提升算法gradient-boosting-algorithms)- [1 GBM](http://blog.csdn.net/kicilove/article/details/78423555#101-gbm)
- [2 XGBoost](http://blog.csdn.net/kicilove/article/details/78423555#102-xgboost)
- [3 LightGBM](http://blog.csdn.net/kicilove/article/details/78423555#103-lightgbm)
- [4 Catboost](http://blog.csdn.net/kicilove/article/details/78423555#104-catboost)
- [总结](http://blog.csdn.net/kicilove/article/details/78423555#总结)
## 引言
![这里写图片描述](https://img-blog.csdn.net/20171102121855255?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
整体来说，机器学习算法可以分为3大类
## 一、监督学习：
工作原理：该算法由自变量（协变量、预测变量）和因变量（结果变量）组成，由一组自变量对因变量进行预测。通过这些变量集合，我们生成一个将输入映射到输出的函数。 训练过程达到我们设定的损失阈值停止训练，也就是使模型达到我们需要的准确度等水平。
监督学习的例子：回归，决策树，随机森林，KNN，逻辑回归等
## 二、无监督学习：
工作原理：在无监督学习算法中，我们没有目标或结果变量来预测。 通常用于不同群体的群体聚类。
无监督学习的例子：Apriori算法，K-means。
## 三、强化学习：
工作原理： 强化学习(reinforcement learning)，又称再励学习、评价学习，学习不是单一方法，而是一种机器学习方式，在智能控制机器人及分析预测等领域有许多应用。
强化学习例子：马尔可夫决策过程
## 四、通用机器学习算法列表
此处列举常用的机器学习算法， 这些算法几乎可以应用于所有的数据问题：
- 线性回归
- Logistic回归
- 决策树
- SVM
- 朴素贝叶斯
- KNN
- K均值
- 随机森林
- 降维算法
- 梯度提升算法 
1.GBM 
2.XGBoost 
3.LightGBM 
4.CatBoost
### 1.线性回归（Linear Regression）
它用于连续变量的估计（比如说房屋成本，总销售额等）。 此处，我们通过拟合自变量和因变量之间的最佳拟合直线来建立它们之间的关系。 这条线就被叫做回归线，由线性方程表示为：。
在上述等式中，各自代表意义如下：
- Y - 因变量
- a - 斜率，坡度
- X - 自变量
- b - 截距项
系数a和b是基于数据点与回归线之间的距离的差的平方之和最小化而得出的。
看下面的例子。 这里我们已经确定了具有线性方程y = 0.2811x + 13.9的最佳拟合线。 现在使用这个等式，可以由一个人的重量，知道一个人的大体身高：
![这里写图片描述](https://img-blog.csdn.net/20171106194452535?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
线性回归主要有两种类型：简单线性回归和多元线性回归：
- 简单线性回归的特征是一个自变量。
- 多元线性回归（顾名思义）是以多个（多于一个）自变量为特征的。
Python 代码
```
#Import Library
#Import other necessary libraries like pandas, numpy...
from sklearn import linear_model
#Load Train and Test datasets
#Identify feature and response variable(s) and values must be numeric and numpy arrays
x_train=input_variables_values_training_datasets
y_train=target_variables_values_training_datasets
x_test=input_variables_values_test_datasets
# Create linear regression object
linear = linear_model.LinearRegression()
# Train the model using the training sets and check score
linear.fit(x_train, y_train)
linear.score(x_train, y_train)
#Equation coefficient and Intercept
print('Coefficient: \n', linear.coef_)
print('Intercept: \n', linear.intercept_)
#Predict Output
predicted= linear.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
R 语言代码
```
#Load Train and Test datasets
#Identify feature and response variable(s) and values must be numeric and numpy arrays
x_train <- input_variables_values_training_datasets
y_train <- target_variables_values_training_datasets
x_test <- input_variables_values_test_datasets
x <- cbind(x_train,y_train)
# Train the model using the training sets and check score
linear <- lm(y_train ~ ., data = x)
summary(linear)
#Predict Output
predicted= predict(linear,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
### 2. 逻辑回归（Logistic Regression）
这个算法的名字具有迷惑性，从建模任务的角度来看，这是一种分类而不是回归算法。 它用于基于给定的一组自变量估计因变量（离散值（二进制值，如0/1，是/否，真/假）），当然也用于多分类的任务。 简而言之，它通过将数据拟合到logit函数来预测事件发生的可能性。 因此，它也被称为logit回归。 因为它预测了概率，其输出值在0和1之间：
从统计学的角度来看就是自变量的线性组合建模得出因变量y的结果，然后，这个线性组合被表示成分类事件发生概率与不发生概率的对数函数，具体公式如下：
以上，是存在的我们感兴趣的特征预测发生的概率。
Python代码
```
#Import Library
from sklearn.linear_model import LogisticRegression
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create logistic regression object
model = LogisticRegression()
# Train the model using the training sets and check score
model.fit(X, y)
model.score(X, y)
#Equation coefficient and Intercept
print('Coefficient: \n', model.coef_)
print('Intercept: \n', model.intercept_)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
R 语言代码
```
x <- cbind(x_train,y_train)
# Train the model using the training sets and check score
logistic <- glm(y_train ~ ., data = x,family='binomial')
summary(logistic)
#Predict Output
predicted= predict(logistic,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
### 3.决策树（Decision Tree）
决策树算法 是一种主要用于分类问题的监督学习算法。 但是它是个两面身吆，既适用于分类也适用于连续因变量的预测也就是回归，所以通常会说回归树或者分类树。 下面是一棵树的简化版：
![这里写图片描述](https://img-blog.csdn.net/20171106201144146?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在上面的图片中，可以看到根据多个属性最终将人口分为四个不同的组，以识别“他们是否会玩”。 这其中根据不同的决策树算法又会使用基尼指数，信息增益，卡方，熵等多种不同的技术。
Python代码
```
#Import Library
#Import other necessary libraries like pandas, numpy...
from sklearn import tree
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create tree object 
model = tree.DecisionTreeClassifier(criterion='gini') # for classification, here you can change the algorithm as gini or entropy (information gain) by default it is gini  
# model = tree.DecisionTreeRegressor() for regression
# Train the model using the training sets and check score
model.fit(X, y)
model.score(X, y)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
R 语言代码
```
library(rpart)
x <- cbind(x_train,y_train)
# grow tree 
fit <- rpart(y_train ~ ., data = x,method="class")
summary(fit)
#Predict Output 
predicted= predict(fit,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 4. 支持向量机（SVM –Support Vector Machine）
支持向量机是一种分类和回归算法。 在这个算法中，我们将每个数据项绘制为n维空间中的一个点（其中n是您拥有的特征的数量），每个特征的值是特定坐标的值。也就是将数据映射到高维空间。
例如，如果我们只有一个人的身高和头发长度两个特征，我们首先将这两个变量绘制在二维空间中，每个点有两个坐标（这些坐标被称为支持向量）
![这里写图片描述](https://img-blog.csdn.net/20171106201725296?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
现在，我们将找到一些将两个不同分类的数据分割的线。 我们真正要找的是这样一条线，两个组中离这条线各自最近的的点，整体上相对于所有线来说这两个点到此线的距离最大。
![这里写图片描述](https://img-blog.csdn.net/20171106202742573?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
在上面所示的例子中，将数据分成两个不同分类组的线是黑线，因为两个最近的点离线距离和最远。 这条线就是我们的分类器。 然后，根据测试数据在线两侧的位置，可以将新数据分类到相应的类别。
Python代码
```
#Import Library
from sklearn import svm
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create SVM classification object 
model = svm.svc() # there is various option associated with it, this is simple for classification. You can refer link, for mo# re detail.
# Train the model using the training sets and check score
model.fit(X, y)
model.score(X, y)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
R语言代码
```
library(e1071)
x <- cbind(x_train,y_train)
# Fitting model
fit <-svm(y_train ~ ., data = x)
summary(fit)
#Predict Output 
predicted= predict(fit,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 5. 朴素贝叶斯（Naive Bayes）
朴素贝叶斯是一种基于贝叶斯定理的分类算法，假设条件时预测变量之间是相互独立的。 简而言之，朴素贝叶斯分类器假设类中特定特征的存在与任何其他特征的存在无关。 例如，如果水果是红色的，圆形的，并且直径约3英寸，则水果可以被认为是苹果。 即使这些特征彼此依赖或者依赖于其他特征的存在，朴素贝叶斯分类器也会考虑所有这些特性来独立地贡献这个果实是苹果的可能性。
朴素贝叶斯模型易于构建，特别适用于非常大的数据集。
贝叶斯定理提供了一种从，和计算后验概率的方法。
 看下面的公式：
![这里写图片描述](https://img-blog.csdn.net/20171106203250570?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
这里：
- 是给定预测器（属性）的类（目标）的后验概率。
- 是判断为此类的先验概率。
- 是预测器给定类的概率的可能性。
- 是自变量发生的先验概率。
例子：让我们用一个例子来理解它。 下面我有一个天气为自变量及相应的目标变量“Play”组成的训练数据集。 现在，我们需要根据天气状况来分类玩家是否玩游戏。 让我们按照下面的步骤来执行它。
步骤1：将数据集转换为频率表
步骤2：从表中可以创建右侧似然表格，可以计算出Overcast的比例=0.29，Play的频率 = 0.64。
![这里写图片描述](https://img-blog.csdn.net/20171106203817307?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
步骤3：现在，使用朴素贝叶斯公式来计算每个类别的后验概率。 后验概率最高的是预测相应类的结果。
问题：如果天气晴朗，玩家会出去玩，这个说法是否正确？
我们可以用上面讨论的方法来解决这个问题， 
= 
这里我们有=
 3/9 = 0.33，=
 5/14 = 0.36，=
 9/14 = 0.64
现在，=
 0.33 * 0.64 / 0.36 = 0.60，其概率较高，因此我们就认为晴天出去玩的可能性大。
Python代码
```
#Import Library
from sklearn.naive_bayes import GaussianNB
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create SVM classification object model = GaussianNB() # there is other distribution for multinomial classes like Bernoulli Naive Bayes, Refer link
# Train the model using the training sets and check score
model.fit(X, y)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
R语言代码
```
library(e1071)
x <- cbind(x_train,y_train)
# Fitting model
fit <-naiveBayes(y_train ~ ., data = x)
summary(fit)
#Predict Output 
predicted= predict(fit,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 6. K近邻（KNN –K- Nearest Neighbors）
K近邻算法可以用于分类和回归问题。 但是，它在业内在分类问题上用的比较广泛。 K个最近邻居是对这个算法的简单的描述，通过其k个邻居的多数投票来分类。 某数据被分配到某一类是由距离函数测量的K个最近的邻居中是最常出现的类别为它相应的类。
这些距离函数可以是欧几里得，曼哈顿，闵可夫斯基和海明距离等。 前三个函数用于连续函数，第四个函数（Hamming）用于分类变量。 有时候，在执行KNN建模时选择K是一个挑战。
![这里写图片描述](https://img-blog.csdn.net/20171106204810668?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQva2ljaWxvdmU=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/SouthEast)
KNN可以很容易地映射到我们的真实生活中。 如果你想了解一个你没有任何信息的人，你可能想知道他的密友和他的圈子，并因此获得他/她的信息，类似六度空间理论！
Python代码
```
#Import Library
from sklearn.neighbors import KNeighborsClassifier
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create KNeighbors classifier object model 
KNeighborsClassifier(n_neighbors=6) # default value for n_neighbors is 5
# Train the model using the training sets and check score
model.fit(X, y)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
R语言代码
```
library(knn)
x <- cbind(x_train,y_train)
# Fitting model
fit <-knn(y_train ~ ., data = x,k=5)
summary(fit)
#Predict Output 
predicted= predict(fit,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 7. K均值（K-Means）
K均值是一种解决聚类问题的无监督算法。通过一定数量的聚类（假设k个聚类）对给定的数据集进行分类。 集群内的数据点对同组来说是同质且异构的，就是类里尽量距离最近类间尽量最大距离。
#### K-means如何形成群类：
1.K-means为每个簇选取k个点，称为质心。 
2.每个数据点形成具有最接近的质心（即k个聚类）的聚类。 
3.根据现有集群成员查找每个集群的质心。 这里我们有新的质心。 
4.由于我们有新的质心，请重复步骤2和3.从新质心找到每个数据点的最近距离，并与新的k-簇进行关联。 重复这个过程直到收敛发生，即质心不变。
Python代码
```
#Import Library
from sklearn.cluster import KMeans
#Assumed you have, X (attributes) for training data set and x_test(attributes) of test_dataset
# Create KNeighbors classifier object model 
k_means = KMeans(n_clusters=3, random_state=0)
# Train the model using the training sets and check score
model.fit(X)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
R语言代码
```
library(cluster)
fit <- kmeans(X, 3) # 5 cluster solution
```
- 1
- 2
### 8. 随机森林（Random Forest）
随机森林是决策树的融合。 在随机森林中，我们收集了决策树（所谓的“森林”）。 为了根据属性对一个新进来的数据进行分类，每棵树都给出了该数据的一个分类，这些棵树对类别进行“投票”。 森林选择票数最多的分类为此新数据的类。
每棵树按如下方式生长：
1.如果训练集中的样本数为N，从这N个样本中有放回随机抽取样本，这些个样本用来训练生成树。 
2.如果有M个输入变量，则指定一个数m << M，使得在每个节点处，从M中随机选择m个变量，并且使用这些m上的最佳分割来分割节点。 在森林生长期间，m的值保持不变。 
3.每棵树都尽可能地长大。 不给修剪。
Python代码
```
#Import Library
from sklearn.ensemble import RandomForestClassifier
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create Random Forest object
model= RandomForestClassifier()
# Train the model using the training sets and check score
model.fit(X, y)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
R语言代码
```
library(randomForest)
x <- cbind(x_train,y_train)
# Fitting model
fit <- randomForest(Species ~ ., x,ntree=500)
summary(fit)
#Predict Output 
predicted= predict(fit,x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
### 9. 降维算法（Dimensionality Reduction Algorithms）
在过去的4-5年里，采集的数据呈指数级增长。
例如：电子商务公司正在抓住更多关于客户的细节，例如他们的上网历史，他们喜欢什么或不喜欢什么，购买历史等，使他们比杂货店店主更注重客户个性化推荐。
降维算法可以帮助我们连同决策树，随机森林，主成分分析，因子分析等进行数据分析。
Python代码
```
#Import Library
from sklearn import decomposition
#Assumed you have training and test data set as train and test
# Create PCA obeject pca= decomposition.PCA(n_components=k) #default value of k =min(n_sample, n_features)
# For Factor analysis
#fa= decomposition.FactorAnalysis()
# Reduced the dimension of training dataset using PCA
train_reduced = pca.fit_transform(train)
#Reduced the dimension of test dataset
test_reduced = pca.transform(test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
R语言代码
```
library(stats)
pca <- princomp(train, cor = TRUE)
train_reduced  <- predict(pca,train)
test_reduced  <- predict(pca,test)
```
- 1
- 2
- 3
- 4
### 10. 梯度提升算法（Gradient Boosting Algorithms）
#### 10.1 GBM
GBM是一种提升算法，处理大量的数据提高预测性能。 Boosting实际上是一些弱学习算法的集合，它结合了几个弱算法估计量的预测，以提高预测的鲁棒性。 它将多个弱预测指标或平均预测指标组合成一个强预测指标。 这些提升算法在Kaggle，AV Hackathon，CrowdAnalytix等数据科学竞赛中表现突出。
Python代码
```
#Import Library
from sklearn.ensemble import GradientBoostingClassifier
#Assumed you have, X (predictor) and Y (target) for training data set and x_test(predictor) of test_dataset
# Create Gradient Boosting Classifier object
model= GradientBoostingClassifier(n_estimators=100, learning_rate=1.0, max_depth=1, random_state=0)
# Train the model using the training sets and check score
model.fit(X, y)
#Predict Output
predicted= model.predict(x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
R语言代码
```
library(caret)
x <- cbind(x_train,y_train)
# Fitting model
fitControl <- trainControl( method = "repeatedcv", number = 4, repeats = 4)
fit <- train(y ~ ., data = x, method = "gbm", trControl = fitControl,verbose = FALSE)
predicted= predict(fit,x_test,type= "prob")[,2]
```
- 1
- 2
- 3
- 4
- 5
- 6
GradientBoostingClassifier和Random Forest是两个不同的提升树分类器。
#### 10.2 XGBoost
另一个经典的增强算法，被称为Kaggle神器。因为使用这个算法得当，往往会取得较好的名次。
XGBoost具有非常好的预测能力，它有线性模型和树模型算法，该算法比现有GBM快近10倍。
支持包括各种目标函数，包括回归，分类和排序。
XGBoost也被叫做正则化的提升算法，因为它可以使用正则化减少模型的过拟合。Scala，Java，R，Python，Julia和C ++等一系列语言支持XGBoost。
关于XGBoost的参数和调参指南，请参考[这篇文章](http://blog.csdn.net/kicilove/article/details/78413112)，给出了参数详解以及调参步骤以及实例分析。
Python代码
```python
from xgboost import XGBClassifier
from sklearn.model_selection import train_test_split
from sklearn.metrics import accuracy_score
X = dataset[:,0:10]
Y = dataset[:,10:]
seed = 1
X_train, X_test, y_train, y_test = train_test_split(X, Y, test_size=0.33, random_state=seed)
model = XGBClassifier()
model.fit(X_train, y_train)
#Make predictions for test data
y_pred = model.predict(X_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
R语言代码
```
require(caret)
x <- cbind(x_train,y_train)
# Fitting model
TrainControl <- trainControl( method = "repeatedcv", number = 10, repeats = 4)
model<- train(y ~ ., data = x, method = "xgbLinear", trControl = TrainControl,verbose = FALSE)
#OR 
model<- train(y ~ ., data = x, method = "xgbTree", trControl = TrainControl,verbose = FALSE)
predicted <- predict(model, x_test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
#### 10.3 LightGBM
LightGBM是一个梯度提升框架，使用基于树的学习算法。 它被设计成分布式，具有以下优点：
- 更快的训练速度和更高的效率
- 降低内存使用量
- 更好的准确性
- 支持并行和GPU学习
- 能够处理大规模的数据
该框架是一种基于决策树算法的快速高性能梯度提升算法，用于排序，分类等多种机器学习任务。 它是在Microsoft的分布式机器学习工具包项目下开发的。
Python代码
```
data = np.random.rand(500, 10) # 500 entities, each contains 10 features
label = np.random.randint(2, size=500) # binary target
train_data = lgb.Dataset(data, label=label)
test_data = train_data.create_valid('test.svm')
param = {'num_leaves':31, 'num_trees':100, 'objective':'binary'}
param['metric'] = 'auc'
num_round = 10
bst = lgb.train(param, train_data, num_round, valid_sets=[test_data])
bst.save_model('model.txt')
# 7 entities, each contains 10 features
data = np.random.rand(7, 10)
ypred = bst.predict(data)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
R语言代码1
```
library(RLightGBM)
data(example.binary)
#Parameters
num_iterations <- 100
config <- list(objective = "binary",  metric="binary_logloss,auc", learning_rate = 0.1, num_leaves = 63, tree_learner = "serial", feature_fraction = 0.8, bagging_freq = 5, bagging_fraction = 0.8, min_data_in_leaf = 50, min_sum_hessian_in_leaf = 5.0)
#Create data handle and booster
handle.data <- lgbm.data.create(x)
lgbm.data.setField(handle.data, "label", y)
handle.booster <- lgbm.booster.create(handle.data, lapply(config, as.character))
#Train for num_iterations iterations and eval every 5 steps
lgbm.booster.train(handle.booster, num_iterations, 5)
#Predict
pred <- lgbm.booster.predict(handle.booster, x.test)
#Test accuracy
sum(y.test == (y.pred > 0.5)) / length(y.test)
#Save model (can be loaded again via lgbm.booster.load(filename))
lgbm.booster.save(handle.booster, filename = "/tmp/model.txt")
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
R语言代码2
```
require(caret)
require(RLightGBM)
data(iris)
model <-caretModel.LGBM()
fit <- train(Species ~ ., data = iris, method=model, verbosity = 0)
print(fit)
y.pred <- predict(fit, iris[,1:4])
library(Matrix)
model.sparse <- caretModel.LGBM.sparse()
#Generate a sparse matrix
mat <- Matrix(as.matrix(iris[,1:4]), sparse = T)
fit <- train(data.frame(idx = 1:nrow(iris)), iris$Species, method = model.sparse, matrix = mat, verbosity = 0)
print(fit)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
#### 10.4. Catboost
CatBoost是Yandex最近开源的机器学习算法。 它可以很容易地与谷歌的TensorFlow和苹果公司的核心ML等深度学习框架相结合。
CatBoost最棒的地方在于它不需要像其他ML模型那样要大量数据训练，对于不同的数据格式它也可以应付自如。
Python代码
```python
import pandas as pd
import numpy as np
from catboost import CatBoostRegressor
#Read training and testing files
train = pd.read_csv("train.csv")
test = pd.read_csv("test.csv")
#Imputing missing values for both train and test
train.fillna(-999, inplace=True)
test.fillna(-999,inplace=True)
#Creating a training set for modeling and validation set to check model performance
X = train.drop(['Item_Outlet_Sales'], axis=1)
y = train.Item_Outlet_Sales
from sklearn.model_selection import train_test_split
X_train, X_validation, y_train, y_validation = train_test_split(X, y, train_size=0.7, random_state=1234)
categorical_features_indices = np.where(X.dtypes != np.float)[0]
#importing library and building model
from catboost import CatBoostRegressormodel=CatBoostRegressor(iterations=50, depth=3, learning_rate=0.1, loss_function='RMSE')
model.fit(X_train, y_train,cat_features=categorical_features_indices,eval_set=(X_validation, y_validation),plot=True)
submission = pd.DataFrame()
submission['Item_Identifier'] = test['Item_Identifier']
submission['Outlet_Identifier'] = test['Outlet_Identifier']
submission['Item_Outlet_Sales'] = model.predict(test)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
- 28
- 29
- 30
- 31
- 32
R语言代码
```
set.seed(1)
require(titanic)
require(caret)
require(catboost)
tt <- titanic::titanic_train[complete.cases(titanic::titanic_train),]
data <- as.data.frame(as.matrix(tt), stringsAsFactors = TRUE)
drop_columns = c("PassengerId", "Survived", "Name", "Ticket", "Cabin")
x <- data[,!(names(data) %in% drop_columns)]y <- data[,c("Survived")]
fit_control <- trainControl(method = "cv", number = 4,classProbs = TRUE)
grid <- expand.grid(depth = c(4, 6, 8),learning_rate = 0.1,iterations = 100, l2_leaf_reg = 1e-3,            rsm = 0.95, border_count = 64)
report <- train(x, as.factor(make.names(y)),method = catboost.caret,verbose = TRUE, preProc = NULL,tuneGrid = grid, trControl = fit_control)
print(report)
importance <- varImp(report, scale = FALSE)
print(importance)
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
- 9
- 10
- 11
- 12
- 13
- 14
- 15
- 16
- 17
- 18
- 19
- 20
- 21
- 22
- 23
- 24
- 25
- 26
- 27
## 总结：
到目前为止，我相信，你会对常用的机器学习算法在脑袋中形成一个大体的框架。 那就马上开始，如果你热衷于掌握机器学习，拿起问题，应用这些代码，享受建模的乐趣！
