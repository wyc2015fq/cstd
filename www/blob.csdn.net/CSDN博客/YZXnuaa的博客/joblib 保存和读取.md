# joblib 保存和读取 - YZXnuaa的博客 - CSDN博客
2018年06月14日 16:12:15[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：3758
个人分类：[机器学习																[Sklearn](https://blog.csdn.net/YZXnuaa/article/category/7736194)](https://blog.csdn.net/YZXnuaa/article/category/7432476)
在做模型训练的时候，尤其是在训练集上做交叉验证，通常想要将模型保存下来，然后放到独立的测试集上测试，下面介绍的是Python中训练模型的保存和再使用。
scikit-learn已经有了模型持久化的操作，导入joblib即可
```python
from sklearn.externals import joblib
```
- 1
### 模型保存
```python
>>> os.chdir("workspace/model_save")
>>> from sklearn import svm
>>> X = [[0, 0], [1, 1]]
>>> y = [0, 1]
>>> clf = svm.SVC()
>>> clf.fit(X, y)  
>>> clf.fit(train_X,train_y)
>>> joblib.dump(clf, "train_model.m")
```
- 1
- 2
- 3
- 4
- 5
- 6
- 7
- 8
通过joblib的dump可以将模型保存到本地，clf是训练的分类器
### 模型从本地调回
```python
>>> clf = joblib.load("train_model.m")
```
- 1
通过joblib的load方法，加载保存的模型。
然后就可以在测试集上测试了
`clf.predit(test_X) #此处test_X为特征集`
