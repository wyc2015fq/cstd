# 火爆GitHub：100天搞定机器学习编程，超赞信息图+代码 - 知乎
# 



> 问耕 栗子 发自 麦蒿寺
量子位 出品 | 公众号 QbitAI



![](https://pic2.zhimg.com/v2-84e895f2f5114982ada7161b8c479745_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='608'></svg>)



你是想喝一辈子糖水，还是想用AI改变世界？

但怎么想是一回事，怎么做往往是另一回事。学习和健身一样，不少人都停留在口头上，有各种借口不曾付诸实施。

为此，YouTube网红Siraj Raval发起了一个挑战赛：#100DaysOfMLCode。

这个挑战赛意在号召大家行动起来，从参与活动的那天起，每天至少花费1小时的时间来学习提升或者应用编程，连续坚持100天，从而更好的理解和掌握机器学习这个强大的工具。积跬步、至千里。



![](https://pic2.zhimg.com/v2-fb593d3de46f8d5180d4588b3dd39f95_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)



而且参加这个活动，还要使用#100DaysOfMLCode这个标签，在社交平台每日“打卡”，公开记录自己的工作。


你想在这100天里学到什么，或者完成一个项目全凭自主决定。大家的选择五花八门，其中有一个小哥的学习计划，很快引发大家的关注。

这个小哥名叫Avik Jain（重名的不少），他的百天计划，是从机器学习的基础概念起步，逐层递进，内容比较适合初学者。



![](https://pic1.zhimg.com/v2-578403643e3421e102e2bb220ef1b92c_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1080' height='537'></svg>)



这个100天搞定机器学习编程的项目，现在已经是爆红GitHub，很快累积了3000多标星，在Twitter上，也有一大票人热捧这个项目。

同是百天计划，这个有什么特别之处？三点：

1、超赞的学习图片

2、配套的代码

3、相应的数据集

对这个项目的评价，多以awesome、great、fantastic、outstanding、perfect等评价，大家纷纷表示感谢（并祝楼主好人一生平安）。

项目地址在此：

[https://github.com/Avik-Jain/100-Days-Of-ML-Code](https://link.zhihu.com/?target=https%3A//github.com/Avik-Jain/100-Days-Of-ML-Code)

请你一定收藏好。

目前作者的100天计划，已经进行到第25天，已有的内容包括：

数据预处理、线性回归、逻辑回归、K最邻近算法、支持向量机、深度学习专项课程等，最新的内容已经讲到决策树及实现等。

量子位摘录其中的部分内容，做个示例。

## **第一天**



![](https://pic4.zhimg.com/v2-b63274c92a052b6908fb51a8a68e6033_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='2000'></svg>)



看上去，真是美好的一天。

## **第一步：导入一些库**
![](https://pic3.zhimg.com/v2-053b2424afc9b310da5ba3a4152b5bfe_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='506' height='462'></svg>)
Numpy和Pandas这两个，是非常重要的库。

Numpy里面有各种数学函数，Pandas是用来导入数据集、管理数据集的。


```python
import numpy as np
  import pandas as pd
```


## **第二步：导入数据集**
![](https://pic2.zhimg.com/v2-6d0e814562fcdcb0b90a810799d6e3c1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='516' height='458'></svg>)
数据集通常是.csv格式。CSV文件，是把表格数据，存储成纯文本。每一行是一个数据记录 (Data Record) 。

在Pandas库里面，用**read_csv**的方法，来读取本地的CSV文件，每个文件是一个数据帧 (Data Frame) 。

给每个数据帧里的自变量和因变量，分别做矩阵和向量。


```python
dataset = pd.read_csv('Data.csv')
  X = dataset.iloc[ : , :-].values
  Y = dataset.iloc[ : , ].values第三步：处理缺失数据
```

![](https://pic4.zhimg.com/v2-3862eef1a9405d49d4d46e12d6e5d39b_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='458'></svg>)
我们获取的数据，通常是异构数据。导致缺失数据 (Missing Data) 的原因有很多，需要处理一下，模型的表现才不会减损。

可以把缺失的数据，用整列数据的**平均值**或者**中位数**代替。拿**sklearn.preprocessing**里的**Imputer**来处理。


```
1  from sklearn.preprocessing import Imputer
2  imputer = Imputer(missing_values = "NaN", strategy = "mean", axis = 0)
3  imputer = imputer.fit(X[ : , 1:3])
4  X[ : , 1:3] = imputer.transform(X[ : , 1:3])
```


**第四步：编码分类数据**

![](https://pic2.zhimg.com/v2-4525ee99458b392570e58b2f4458318d_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='510' height='442'></svg>)
分类数据 (Categorical Data) 里的变量，不包含**数值**，只包含分类**标签**。

比如，是/否，性别，婚姻状态，这样的变量，是没办法当成数值直接运算的。

所以，才需要把它们编码成能够运算的数值。用**sklearn.preprocessing**里面的**LabelEncoder**就可以了。


```
1  from sklearn.preprocessing import LabelEncoder, OneHotEncoder
2  labelencoder_X = LabelEncoder()
3  X[ : , 0] = labelencoder_X.fit_transform(X[ : , 0])
```


创建个虚拟变量：



```
1  onehotencoder = OneHotEncoder(categorical_features = [0])
2  X = onehotencoder.fit_transform(X).toarray()
3  labelencoder_Y = LabelEncoder()
4  Y =  labelencoder_Y.fit_transform(Y)
```


**第五步：分开训练集和测试集**

![](https://pic1.zhimg.com/v2-4acdf3a99c981875848fb26821628218_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='506' height='424'></svg>)
要把数据集分成两半，一个当训练集，另一个当测试集。

一般来说，可以按80/20这样分，训练集大一些。用**sklearn.crossvalidation**里面的**train_test_split()** 来分，就行了。


```
1  from sklearn.cross_validation import train_test_split
2  X_train, X_test, Y_train, Y_test = train_test_split( X , Y , test_size = 0.2, random_state = 0)
```


**第六步：特征缩放**

![](https://pic4.zhimg.com/v2-1d21fcd893c1d59a8466a9d7a91376b7_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='512' height='468'></svg>)
大部分机器学习算法，都会拿两个数据点之间的欧几里得距离 (Euclidean Distance) 做计算。

这样一来，如果一个特征比其他特征的范围值更大，这个特征值就会成为主导。

而我们希望其他特征，也得到同等的重视，所以用**特征标准化** (Feature Standardization) 来解决这个问题。

**sklearn.preprocessing**里面的**StandardScalar**，就派上用场了。


```
1  from sklearn.preprocessing import StandardScaler
2  sc_X = StandardScaler()
3  X_train = sc_X.fit_transform(X_train)
4  X_test = sc_X.fit_transform(X_test)
```


## **第二天**



![](https://pic2.zhimg.com/v2-01be5efcb0e8ae6dc4ef1a2bb97faf45_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='2000'></svg>)



今天，要做的是**简单线性回归** (Simple Linear Regression) 。
![](https://pic4.zhimg.com/v2-1710a16b91b4c959be83fcb8674ea11f_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='830' height='776'></svg>)
就是拿已知的那些 (x,y) 数据点，做出一条线性的趋势，来预测其他x值，对应的y。

## **第一步：数据预处理**
![](https://pic4.zhimg.com/v2-06cbec96db851337111e58fbfdf9b363_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='404' height='386'></svg>)
其实，就是把**第一天**做过的事情，复习一下：

· 导入一些库
· 导入数据集
· 处理缺失数据
· 把数据集分成训练集和测试集
· 特征缩放的话，交给库了


```
1  import pandas as pd
 2  import numpy as np
 3  import matplotlib.pyplot as plt
 4
 5  dataset = pd.read_csv('studentscores.csv')
 6  X = dataset.iloc[ : ,   : 1 ].values
 7  Y = dataset.iloc[ : , 1 ].values
 8
 9  from sklearn.cross_validation import train_test_split
10  X_train, X_test, Y_train, Y_test = train_test_split( X, Y, test_size = 1/4, random_state = 0)
```


**第二步：拟合训练集**

![](https://pic4.zhimg.com/v2-c878e259c78d3eeec942437090777e93_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='398' height='390'></svg>)
要把数据集，拟合到简单线性回归模型里面去，可以用**sklearn.linear_model**里面的**LinearRegression**来解决。

在**LinearRegression**里面，建一个叫做**regressor**的对象。

然后，把**regressor**拟合到数据集里去，用**fit()** 就行了。


```
1  from sklearn.linear_model import LinearRegression
2  regressor = LinearRegression()
3  regressor = regressor.fit(X_train, Y_train)
```


**第三步：预测结果**

![](https://pic2.zhimg.com/v2-72ccac4a944fc4fb8dcf43c9da92fc91_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='412' height='388'></svg>)
现在，用测试集来预测一下。

把输出存到一个向量**Y_pred**里面。然后，用**LinearRegression**里面的预测方法，来支配上一步训练过的**regressor**。

`1  Y_pred Y_pred == regressor.predict(X_test) regressor.predict(X_test)`

**第四步：可视化**

![](https://pic2.zhimg.com/v2-eb6073a3d2e19c3ce554bbc3defbc775_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='420' height='362'></svg>)
最后一步，就是给预测结果做个可视化。

用**matplotlib.pyplot**做散点图，给训练集和测试集都做一下，看看预测结果是不是接近真实。

训练集可视化：


```
1  plt.scatter(X_train , Y_train, plt.scatter(X_train , Y_train, colorcolor  ==  ''redred'')
2  plt.plot(X_train , regressor.predict(X_train), ) plt.plot(X_train , regressor.p color ='blue')
```


测试集可视化：


```
1  plt.scatter(X_test , Y_test, color = 'red')
2  plt.plot(X_test , regressor.predict(X_test), color ='blue')
```


## **两件需要提醒的事**

总之，大概就是上面这个样子。部分已经放出的信息图集合如下，大家可以预览一下。



![](https://pic2.zhimg.com/v2-95a6554558cd7af816e6308dd91498e1_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='1000' height='918'></svg>)



如果你也想入门或者提升自己的机器学习能力，不妨试试跟着这个小哥一起学习进步。虽然这个项目全是英文，但并不晦涩高深，说不定还能顺便提高一下英文水平，以后读paper也用得到~


还有两件事，需要提醒一下。

1、这个项目已经开始汉化了

@zhyongquan 在GitHub上已经开始尝试中文版，效果如下图所示。



![](https://pic4.zhimg.com/v2-d2ef4fe4b27d6384867d33d5aa2227cf_b.jpg)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='800' height='2000'></svg>)



不过目前只进行了第一天内容的汉化。如果你感兴趣，可以持续关注，或者加入汉化的行动中。地址在此：

[https://github.com/Avik-Jain/100-Days-Of-ML-Code/issues/8](https://link.zhihu.com/?target=https%3A//github.com/Avik-Jain/100-Days-Of-ML-Code/issues/8)

2、项目内容不能盲信

尽信书不如无书。这个学习项目，也不是百分百完全正确，比如第四天讲逻辑回归时的右下角配图，就被指出存在错误。

作者也承诺将更新改正这个问题。

就酱。

最后，再发一次这个项目的地址：

[https://github.com/Avik-Jain/100-Days-Of-ML-Code](https://link.zhihu.com/?target=https%3A//github.com/Avik-Jain/100-Days-Of-ML-Code)

请你一定收藏好，如果能开始学习那就更好啦。

如果这种偏入门的内容不适合你，还有更多进阶或者具体项目实现的100天计划可供参考，希望能帮助你精进。

比方#100DaysOfMLCode挑战赛发起人Siraj Raval，就选择了在100天里，使用机器学习技术，根据气温、降水、植被变化等因素，预测每周的登革热疫情，改善防疫的研究计划和资源分配。

你也可以选择其他项目，或者在Twitter、GitHub等看看别人的100天挑战，有没有你合适跟随的，或者全新启动你的挑战。

对了Siraj Raval还在GitHub和YouTube上发布过很多其他机器学习课程，例如三个月学习机器学习，六周入门深度学习等。

相关地址在此：

[https://github.com/llSourcell](https://link.zhihu.com/?target=https%3A//github.com/llSourcell)

总之，原则就是每天进步一点点。

加油。



![](https://pic3.zhimg.com/v2-0a5f662accd8e8a09b6b89c6fc72ea56_b.gif)![](data:image/svg+xml;utf8,<svg xmlns='http://www.w3.org/2000/svg' width='600' height='338'></svg>)



— **完** —

欢迎大家关注我们的专栏：[量子位 - 知乎专栏](https://zhuanlan.zhihu.com/qbitai)

诚挚招聘

量子位正在招募编辑/记者，工作地点在北京中关村。期待有才气、有热情的同学加入我们！相关细节，请在量子位公众号(QbitAI)对话界面，回复“招聘”两个字。

[量子位 QbitAI](https://zhuanlan.zhihu.com/qbitai)· 头条号签约作者

վ'ᴗ' ի 追踪AI技术和产品新动态


