# 吴恩达 深度学习 1-3 课后作业 Planar data classification with one hidden layer - 别说话写代码的博客 - CSDN博客





2017年11月24日 16:53:49[别说话写代码](https://me.csdn.net/qq_21997625)阅读数：2387
所属专栏：[吴恩达深度学习DeepLearning.ai作业](https://blog.csdn.net/column/details/24629.html)












# Planar data classification with one hidden layer[](https://hub.coursera-notebooks.org/user/lzkxxedlrtubjabjzuijhp/nbconvert/html/Week%203/Planar%20data%20classification%20with%20one%20hidden%20layer/Planar%20data%20classification%20with%20one%20hidden%20layer%20v4.ipynb?download=false#Planar-data-classification-with-one-hidden-layer)

Welcome to your week 3 programming assignment. It's time to build your first neural network, which will have a hidden layer. You will see a big difference between this model and the one you implemented using logistic
 regression.

You will learn how to:
- Implement a 2-class classification neural network with a single hidden layer
- Use units with a non-linear activation function, such as tanh
- Compute the cross entropy loss
- Implement forward and backward propagation









## 1 - Packages[](https://hub.coursera-notebooks.org/user/lzkxxedlrtubjabjzuijhp/nbconvert/html/Week%203/Planar%20data%20classification%20with%20one%20hidden%20layer/Planar%20data%20classification%20with%20one%20hidden%20layer%20v4.ipynb?download=false#1---Packages)

Let's first import all the packages that you will need during this assignment.
- [numpy](https://hub.coursera-notebooks.org/user/lzkxxedlrtubjabjzuijhp/nbconvert/html/Week%203/Planar%20data%20classification%20with%20one%20hidden%20layer/www.numpy.org) is
 the fundamental package for scientific computing with Python.
- [sklearn](http://scikit-learn.org/stable/) provides simple and efficient tools for data mining and data analysis.
- [matplotlib](http://matplotlib.org/) is a library for plotting graphs in Python.
- testCases provides some test examples to assess the correctness of your functions
- planar_utils provide various useful functions used in this assignment






In [10]:



# Package importsimportnumpyasnpimportmatplotlib.pyplotaspltfromtestCases_v2import*importsklearnimportsklearn.datasetsimportsklearn.linear_modelfromplanar_utilsimportplot_decision_boundary,sigmoid,load_planar_dataset,load_extra_datasets%matplotlib inline  

np.random.seed()# set a seed so that the results are consistent










## 2 - Dataset[](https://hub.coursera-notebooks.org/user/lzkxxedlrtubjabjzuijhp/nbconvert/html/Week%203/Planar%20data%20classification%20with%20one%20hidden%20layer/Planar%20data%20classification%20with%20one%20hidden%20layer%20v4.ipynb?download=false#2---Dataset)

First, let's get the dataset you will work on. The following code will load a "flower" 2-class dataset into variables `X` and `Y`.






In [11]:



X,Y=load_planar_dataset()










Visualize the dataset using matplotlib. The data looks like a "flower" with some red (label y=0) and some blue (y=1) points. Your goal is to build a model to fit this data.






In [12]:



# Visualize the data:plt.scatter(X[,:],X[,:],c=Y,s=,cmap=plt.cm.Spectral);



























