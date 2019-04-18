# 【机器学习】SVM iris数据集实现2 - YZXnuaa的博客 - CSDN博客
2018年03月13日 17:36:55[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：917
**一、导入sklearn算法包**
　　Scikit-Learn库已经实现了所有基本机器学习的算法，具体使用详见官方文档说明：[http://scikit-learn.org/stable/auto_examples/index.html#support-vector-machines](http://scikit-learn.org/stable/auto_examples/index.html#support-vector-machines)。
　　skleran中集成了许多算法，其导入包的方式如下所示，
　　逻辑回归：from sklearn.linear_model import LogisticRegression
      朴素贝叶斯：from sklearn.naive_bayes import GaussianNB
 　　K-近邻：from sklearn.neighbors import KNeighborsClassifier
 　　决策树：from sklearn.tree import DecisionTreeClassifier
支持向量机：from sklearn import svm
**二、sklearn中svc的使用**
**（1）使用numpy中的loadtxt读入数据文件**
**loadtxt()的使用方法：**
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170426170343131-1625363622.png)
　　fname:文件路径。eg：C:/Dataset/iris.txt。
　　dtype：数据类型。eg：float、str等。
　　delimiter：分隔符。eg：‘，’。
　　converters：将数据列与转换函数进行映射的字典。eg：{1:fun}，含义是将第2列对应转换函数进行转换。
　　usecols：选取数据的列。
**以****Iris兰花数据****集为例子：**
　　由于从UCI数据库中下载的Iris原始数据集的样子是这样的，前四列为特征列，第五列为类别列，分别有三种类别Iris-setosa， Iris-versicolor， Iris-virginica。　　　
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170426181745615-1007053664.png)
　　当使用numpy中的loadtxt函数导入该数据集时，假设数据类型dtype为浮点型，但是很明显第五列的数据类型并不是浮点型。
　　因此我们要额外做一个工作，即通过loadtxt()函数中的converters参数将第五列通过转换函数映射成浮点类型的数据。
　　首先，我们要写出一个转换函数：
```
```python
def
```
```python
iris_type(s):
```
```python
```
```python
it
```
```python
=
```
```python
{
```
```python
'Iris-setosa'
```
```python
:
```
```python
0
```
```python
,
```
```python
'Iris-versicolor'
```
```python
:
```
```python
1
```
```python
,
```
```python
'Iris-virginica'
```
```python
:
```
```python
2
```
```python
}
```
```python
```
```python
return
```
```python
it[s]
```
```
　　接下来读入数据，converters={4: iris_type}中“4”指的是第5列：
```
```python
path
```
```python
=
```
```python
u
```
```python
'D:/f盘/python/学习/iris.data'
```
```python
# 数据文件路径
```
```python
data
```
```python
=
```
```python
np.loadtxt(path, dtype
```
```python
=
```
```python
float
```
```python
, delimiter
```
```python
=
```
```python
','
```
```python
, converters
```
```python
=
```
```python
{
```
```python
4
```
```python
: iris_type})
```
```
　　读入结果：
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170426184029053-362911109.png)
**（2）将Iris分为训练集与测试集**
```
```python
x, y
```
```python
=
```
```python
np.split(data, (
```
```python
4
```
```python
,), axis
```
```python
=
```
```python
1
```
```python
)
```
```python
x
```
```python
=
```
```python
x[:, :
```
```python
2
```
```python
]
```
```python
x_train, x_test, y_train, y_test
```
```python
=
```
```python
train_test_split(x, y, random_state
```
```python
=
```
```python
1
```
```python
, train_size
```
```python
=
```
```python
0.6
```
```python
)
```
```
　　1. split(数据，分割位置，轴=1（水平分割） or 0（垂直分割）)。
　　2. x = x[:, :2]是为方便后期画图更直观，故只取了前两列特征值向量训练。
　　3. sklearn.model_selection.train_test_split随机划分训练集与测试集。train_test_split(train_data,train_target,test_size=数字, random_state=0)
参数解释：
　　train_data：所要划分的样本特征集
　　train_target：所要划分的样本结果
　　test_size：样本占比，如果是整数的话就是样本的数量
　　random_state：是随机数的种子。
　　随机数种子：其实就是该组随机数的编号，在需要重复试验的时候，保证得到一组一样的随机数。比如你每次都填1，其他参数一样的情况下你得到的随机数组是一样的。但填0或不填，每次都会不一样。随机数的产生取决于种子，随机数和种子之间的关系遵从以下两个规则：种子不同，产生不同的随机数；种子相同，即使实例不同也产生相同的随机数。
**（3）训练svm分类器**
```
```python
# clf = svm.SVC(C=0.1, kernel='linear', decision_function_shape='ovr')
```
```python
```
```python
clf
```
```python
=
```
```python
svm.SVC(C
```
```python
=
```
```python
0.8
```
```python
, kernel
```
```python
=
```
```python
'rbf'
```
```python
, gamma
```
```python
=
```
```python
20
```
```python
, decision_function_shape
```
```python
=
```
```python
'ovr'
```
```python
)
```
```python
```
```python
clf.fit(x_train, y_train.ravel())
```
```
 　　kernel='linear'时，为线性核，C越大分类效果越好，但有可能会过拟合（defaul C=1）。
　　 kernel='rbf'时（default），为高斯核，gamma值越小，分类界面越连续；gamma值越大，分类界面越“散”，分类效果越好，但有可能会过拟合。
　　decision_function_shape='ovr'时，为one v rest，即一个类别与其他类别进行划分，
　　decision_function_shape='ovo'时，为one v one，即将类别两两之间进行划分，用二分类的方法模拟多分类的结果。
**（4）计算svc分类器的准确率**
```
```python
print
```
```python
clf.score(x_train, y_train)
```
```python
# 精度
```
```python
y_hat
```
```python
=
```
```python
clf.predict(x_train)
```
```python
show_accuracy(y_hat, y_train,
```
```python
'训练集'
```
```python
)
```
```python
print
```
```python
clf.score(x_test, y_test)
```
```python
y_hat
```
```python
=
```
```python
clf.predict(x_test)
```
```python
show_accuracy(y_hat, y_test,
```
```python
'测试集'
```
```python
)
```
```
 结果为：
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170427160949194-165722037.png)
　　如果想查看决策函数，可以通过decision_function()实现
```
```python
print
```
```python
'decision_function:\n'
```
```python
, clf.decision_function(x_train)
```
```python
print
```
```python
'\npredict:\n'
```
```python
, clf.predict(x_train)
```
```
 结果为：
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170427161310319-1879566644.png)![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170427161328490-1032156318.png)
　　decision_function中每一列的值代表距离各类别的距离。
**（5）绘制图像**
　　1.确定坐标轴范围，x，y轴分别表示两个特征
```
```python
x1_min, x1_max
```
```python
=
```
```python
x[:,
```
```python
0
```
```python
].
```
```python
min
```
```python
(), x[:,
```
```python
0
```
```python
].
```
```python
max
```
```python
()
```
```python
# 第0列的范围
```
```python
x2_min, x2_max
```
```python
=
```
```python
x[:,
```
```python
1
```
```python
].
```
```python
min
```
```python
(), x[:,
```
```python
1
```
```python
].
```
```python
max
```
```python
()
```
```python
# 第1列的范围
```
```python
x1, x2
```
```python
=
```
```python
np.mgrid[x1_min:x1_max:
```
```python
200j
```
```python
, x2_min:x2_max:
```
```python
200j
```
```python
]
```
```python
# 生成网格采样点
```
```python
grid_test
```
```python
=
```
```python
np.stack((x1.flat, x2.flat), axis
```
```python
=
```
```python
1
```
```python
)
```
```python
# 测试点
```
```python
# print 'grid_test = \n', grid_testgrid_hat = clf.predict(grid_test)       # 预测分类值grid_hat = grid_hat.reshape(x1.shape)  # 使之与输入的形状相同
```
```
 　　这里用到了**mgrid()函数**，该函数的作用这里简单介绍一下：
　　 假设假设目标函数F（x，y）=x+y。x轴范围1~3，y轴范围4~6，当绘制图像时主要分四步进行：
【step1：x扩展】(朝右扩展)：
       [1 1 1]
　　 [2 2 2] 
　　 [3 3 3]
　　【step2：y扩展】(朝下扩展)： 
　　 [4 5 6]
　　 [4 5 6]
　　 [4 5 6]
　　【step3：定位（xi，yi）】：
　　 [(1,4) (1,5) (1,6)]
　　 [(2,4) (2,5) (2,6)]
　　 [(3,4) (3,5) (3,6)]
　　【step4：将（xi，yi）代入F(x,y)=x+y】
　　因此这里x1, x2 = np.mgrid[x1_min:x1_max:200j, x2_min:x2_max:200j]后的结果为：
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170427165215147-1604916463.png)
　　再通过stack()函数，axis=1，生成测试点
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170427165432975-354852760.png)
　　2.指定默认字体
```
```python
mpl.rcParams[
```
```python
'font.sans-serif'
```
```python
]
```
```python
=
```
```python
[u
```
```python
'SimHei'
```
```python
]
```
```python
mpl.rcParams[
```
```python
'axes.unicode_minus'
```
```python
]
```
```python
=
```
```python
False
```
```
　　3.绘制
```
```python
cm_light
```
```python
=
```
```python
mpl.colors.ListedColormap([
```
```python
'#A0FFA0'
```
```python
,
```
```python
'#FFA0A0'
```
```python
,
```
```python
'#A0A0FF'
```
```python
])
```
```python
cm_dark
```
```python
=
```
```python
mpl.colors.ListedColormap([
```
```python
'g'
```
```python
,
```
```python
'r'
```
```python
,
```
```python
'b'
```
```python
])
```
```python
plt.pcolormesh(x1, x2, grid_hat, cmap
```
```python
=
```
```python
cm_light)
```
```python
plt.scatter(x[:,
```
```python
0
```
```python
], x[:,
```
```python
1
```
```python
], c
```
```python
=
```
```python
y, edgecolors
```
```python
=
```
```python
'k'
```
```python
, s
```
```python
=
```
```python
50
```
```python
, cmap
```
```python
=
```
```python
cm_dark)
```
```python
# 样本
```
```python
plt.scatter(x_test[:,
```
```python
0
```
```python
], x_test[:,
```
```python
1
```
```python
], s
```
```python
=
```
```python
120
```
```python
, facecolors
```
```python
=
```
```python
'none'
```
```python
, zorder
```
```python
=
```
```python
10
```
```python
)
```
```python
# 圈中测试集样本
```
```python
plt.xlabel(u
```
```python
'花萼长度'
```
```python
, fontsize
```
```python
=
```
```python
13
```
```python
)
```
```python
plt.ylabel(u
```
```python
'花萼宽度'
```
```python
, fontsize
```
```python
=
```
```python
13
```
```python
)
```
```python
plt.xlim(x1_min, x1_max)
```
```python
plt.ylim(x2_min, x2_max)
```
```python
plt.title(u
```
```python
'鸢尾花SVM二特征分类'
```
```python
, fontsize
```
```python
=
```
```python
15
```
```python
)
```
```python
# plt.grid()
```
```python
plt.show()
```
```
 　　pcolormesh(x,y,z,cmap)这里参数代入x1，x2，grid_hat，cmap=cm_light绘制的是背景。
　　 scatter中edgecolors是指描绘点的边缘色彩，s指描绘点的大小，cmap指点的颜色。
　　 xlim指图的边界。
最终结果为：
![](https://images2015.cnblogs.com/blog/541276/201704/541276-20170427173220537-1741632876.png)
