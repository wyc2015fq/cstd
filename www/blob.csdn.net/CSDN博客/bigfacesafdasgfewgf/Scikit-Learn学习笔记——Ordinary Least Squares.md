# Scikit-Learn学习笔记——Ordinary Least Squares - bigfacesafdasgfewgf - CSDN博客





2014年10月23日 16:18:02[bigface1234fdfg](https://me.csdn.net/puqutogether)阅读数：3858








# Scikit-Learn学习笔记——Ordinary Least Squares

    线性模型的核心思想：认为输出值可以由许多输入变量的线性组合来表示。在训练过程中，我们需要输入一组数据的特征以及对应的类别，学习出一组表示系数w，然后用这个学到的w来给测试样本分类。上面是大白话，是为了便于理解。这个过程也可以高大上一些，那就要请出我们伟大的数学表达式了~话说下面这个数学也不是很复杂哦。。

![](https://img-blog.csdn.net/20141023162746055?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


在分类问题中（sklearn把线性模型划分到了classification）x是样本的特征，y就是样本的标记，我们要求的w就是表示系数啦，这个还是很容易就可以看出来的。

在介绍一系列的线性模型之前，我们需要指明的是：这样子的线性模型如此简单，一旦数据有了噪声，那么线性模型基本上失效。（噗~~天底下哪有这么好的事情呢~一个样本的特征怎么可能就可以线性表示起来呢？怎么就一定满足上面的那种等价关系呢，too young, too naive~~）不过这种线性表示的思想是很有用的，在高级的机器学习算法中，例如sparse
 coding, dictionary learning中都借鉴了这种思想。所以我们还是有必要认真学习一下这里的线性模型的。




    官方公布的示例是使用的diabets数据库，只取样本的1维特征来做，前20个样本做训练，其余样本做测试。其中使用到了regr = linear_model.LinearRegression()  #调用线性模型中的线性回归。然后regr.fit()调用拟合模型；regr.coef_得到表示系数；regr.predict()预测测试样本的类别；regr.score()得到R方值。

关于上述提到的方法介绍如下图：

![](https://img-blog.csdn.net/20141023175714867?watermark/2/text/aHR0cDovL2Jsb2cuY3Nkbi5uZXQvcHVxdXRvZ2V0aGVy/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70/gravity/Center)


链接：

http://scikit-learn.org/stable/modules/generated/sklearn.linear_model.LinearRegression.html#sklearn.linear_model.LinearRegression




这里面对LinearRegression下的几个函数都做了详细的介绍。




程序如下：注意数据库的规模~





```python
import matplotlib.pyplot as plt
import numpy as np
from sklearn import datasets, linear_model

# Load the diabetes dataset
diabetes = datasets.load_diabetes()  #load sklearn中自带的数据库

print np.shape(diabetes) 

# Use only one feature
diabetes_X = diabetes.data[:, np.newaxis]
#print np.shape(diabetes_X) 
diabetes_X_temp = diabetes_X[:, :, 2]  
#print np.shape(diabetes_X_temp)   //大小是442*1

# Split the data into training/testing sets
diabetes_X_train = diabetes_X_temp[:-20]  #前20个样本用来训练
diabetes_X_test = diabetes_X_temp[-20:]  #后面的样本用来测试

# Split the targets into training/testing sets
diabetes_y_train = diabetes.target[:-20]  #标记也是和后面一样的
diabetes_y_test = diabetes.target[-20:]

# Create linear regression object
regr = linear_model.LinearRegression()  #调用线性模型中的线性回归

# Train the model using the training sets
regr.fit(diabetes_X_train, diabetes_y_train)  #fit函数需要输入训练样本的特征和标记，这里的特征只有1维

# The coefficients
print('Coefficients: \n', regr.coef_)  #得到表示系数
# The mean square error
print("Residual sum of squares: %.2f"
      % np.mean((regr.predict(diabetes_X_test) - diabetes_y_test) ** 2))  #残差平方和  可以直接使用predict对测试样本预测
# Explained variance score: 1 is perfect prediction
print('Variance score: %.2f' % regr.score(diabetes_X_test, diabetes_y_test))  #Returns the coefficient of determination R^2 of the prediction

# Plot outputs
plt.scatter(diabetes_X_test, diabetes_y_test,  color='black')
plt.plot(diabetes_X_test, regr.predict(diabetes_X_test), color='blue',
         linewidth=3)

plt.xticks(())
plt.yticks(())

plt.show()
```





















