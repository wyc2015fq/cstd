# 岭回归（ridge regression） - wydbyxr的博客 - CSDN博客
2018年10月22日 13:40:16[whitenightwu](https://me.csdn.net/wydbyxr)阅读数：80
个人分类：[机器学习具体算法](https://blog.csdn.net/wydbyxr/article/category/7945743)
所属专栏：[经典机器学习算法](https://blog.csdn.net/column/details/28812.html)
# 岭回归（ridge regression）
## 在处理回归问题时，常遇到的问题
  在处理复杂的数据的回归问题时，普通的线性回归会遇到一些问题，主要表现在：
  预测精度：这里要处理好这样一对为题，即样本的数量和特征的数量
  模型的解释能力：如果模型中的特征之间有相互关系，这样会增加模型的复杂程度，并且对整个模型的解释能力并没有提高，这时，我们就要进行特征选择。
## 岭回归的概念
  岭回归(Ridge Regression)是在平方误差的基础上增加正则项,
  通过确定的值可以使得在方差和偏差之间达到平衡：随着的增大，模型方差减小而偏差增大。可以认为是在回归分析中，用一种方法改进回归系数的最小二乘估计后所得的回归（方差指的是模型之间的差异，而偏差指的是模型预测值和数据之间的差异。我们需要找到方差和偏差的折中）。
![在这里插入图片描述](https://img-blog.csdn.net/2018102213404832?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3d5ZGJ5eHI=/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## 在进行特征选择时，一般有三种方式
  1）子集选择
  2）收缩方式(Shrinkage method)，又称为正则化(Regularization)。主要包括岭回归个lasso回归。
  3）维数缩减
## 请问岭回归问题是有全局最优解对吗？该方法为什么在人脸subject增加的时候保持了较好的准确率？
  回答：是的，岭回归问题是凸的，有全局最优解。该方法在人脸subject增加是能够保持较高的准确率，我们认为原因来自于在对二维数据学习时保存了数据的固有二维空间结构信息，使得该算法有优于对比算法的表现。
