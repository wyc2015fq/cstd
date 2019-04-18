# 泰坦尼克号数据挖掘项目实战——Task1 数据分析 - fighting！！！ - CSDN博客
2019年01月05日 11:05:37[dujiahei](https://me.csdn.net/dujiahei)阅读数：258
参考资料：[https://www.bilibili.com/video/av27536643](https://www.bilibili.com/video/av27536643)
[https://blog.csdn.net/aaronjny/article/details/79735998](https://blog.csdn.net/aaronjny/article/details/79735998)
[https://github.com/AaronJny/simple_titanic](https://github.com/AaronJny/simple_titanic)
[https://zhuanlan.zhihu.com/p/30538352](https://zhuanlan.zhihu.com/p/30538352)
[https://www.jianshu.com/p/9a5bce0de13f](https://www.jianshu.com/p/9a5bce0de13f)
## **1. 查看数据集**
```python
import pandas as pd
# 读取数据集
train_data = pd.read_csv('Data/train.csv')
test_data = pd.read_csv('Data/test.csv')
# 打印信息
train_data.info()
```
![](https://img-blog.csdnimg.cn/20190105100421591.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
   以上特征含义为：
- PassengerId 乘客编号
- Survived 是否幸存
- Pclass 船票等级
- Name 乘客姓名
- Sex 乘客性别
- SibSp、Parch 亲戚数量
- Ticket 船票号码
- Fare 船票价格
- Cabin 船舱
- Embarked 登录港口
## **2. 特征选择**
根据经验，认为乘客编号、乘客姓名、船票号码、船舱跟逃生几率无关，所以初步选择 ['Pclass', 'Sex', 'Age', 'SibSp', 'Parch', 'Fare', 'Embarked'] 作为训练特征。从打印出来的数据可以发现，序号是891个，但是并不是所有的特征都满足891，所以某些特征存在缺失值。
```python
# 选择用于训练的特征
features = ['Pclass', 'Sex', 'Age', 'SibSp', 'Parch', 'Fare', 'Embarked']
x_train = train_data[features]
x_test = test_data[features]
y_train=train_data['Survived']
# 检查缺失值
x_train.info()
print('-'*100)
x_test.info()
```
![](https://img-blog.csdnimg.cn/20190105101327233.png?x-oss-process=image/watermark,type_ZmFuZ3poZW5naGVpdGk,shadow_10,text_aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L2R1amlhaGVp,size_16,color_FFFFFF,t_70)
## **3. 特征缺失值处理**
所以我们发现，训练数据中的Age，Embarked，测试数据 Age，Fare,Embarked。送入模型中的标签和特征要可以一一对应，所以要么丢掉特征不完整的样本，要么填充。秉着尽可能使用样本的态度，我们选择填充特征。
如何补全数据集呢？最简单的方法：对于数值型数据，可以使用其平均值/中位数填补空值，尽量减小填补值对结果的影响。对于类别数据，可以使用众数来填补。稍微进阶一些的，可以借助其他辅助特征：例如测试数据中Fare缺失的数据，每个乘客的Ticket是唯一的，无法通过查找相同的票号加个来填补，但是，船票价格往往跟 `Pclass` （客舱等级）及 `Cabin`（客舱号）有关的，因此使用具有相同`Pclass`和`Cabin`的中位数或着众数来填补。 
这里先采用最简单的方法进行填补。
```python
# 使用平均年龄来填充年龄中的nan值
x_train['Age'].fillna(x_train['Age'].mean(), inplace=True)
x_test['Age'].fillna(x_test['Age'].mean(),inplace=True)
# 使用票价的均值填充票价中的nan值
x_test['Fare'].fillna(x_test['Fare'].mean(),inplace=True)
# 使用登录最多的港口来填充登录港口的nan值
print x_train['Embarked'].value_counts()
x_train['Embarked'].fillna('S', inplace=True)
x_test['Embarked'].fillna('S',inplace=True)
```
## **4. 特征值转换为特征向量**
机器学习中，送入模型的是矩阵，而上述特征中并不是所有的特征都是数值型的，需要把一些非数值型的特征转为数值。例如性别，我们可以简单的想到“0”，“1”来代表，那么“Embarked”的"C","Q","S" 要选择用“0”，“1”，“2”来代替吗？若两个特征之间的距离是，d(C, Q) = 1, d(Q, S) = 1, d(C, S) = 2。那么C和S工作之间就越不相似吗？显然这样的表示，计算出来的特征的距离是不合理。那如果使用one-hot编码，则得到C = (1, 0, 0), Q= (0, 1, 0), S = (0, 0, 1)，那么两个工作之间的距离就都是sqrt(2).即每两个特征之间的距离是一样的，显得更合理。因此，我们使用 one-hot编码。
```python
# 将特征值转换成特征向量
dvec=DictVectorizer(sparse=False)
x_train=dvec.fit_transform(x_train.to_dict(orient='record'))
x_test=dvec.transform(x_test.to_dict(orient='record'))
# 打印特征向量格式
print （dvec.feature_names_）
```
因此，将特征值转换为特征向量后的格式：
![](https://img-blog.csdnimg.cn/2019010510580574.png)
例如打印一条数据，具体内容：
```python
print （x_train[100]）
```
![](https://img-blog.csdnimg.cn/20190105105959147.png)
特征向量的值与上面的名称一一对应。
