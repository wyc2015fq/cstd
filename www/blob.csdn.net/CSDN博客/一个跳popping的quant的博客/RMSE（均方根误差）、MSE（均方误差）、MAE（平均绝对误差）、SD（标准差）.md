# RMSE（均方根误差）、MSE（均方误差）、MAE（平均绝对误差）、SD（标准差） - 一个跳popping的quant的博客 - CSDN博客





2018年08月24日 20:20:10[敲代码的quant](https://me.csdn.net/FrankieHello)阅读数：7158










**RMSE（Root Mean Square Error）均方根误差**

衡量观测值与真实值之间的偏差。

常用来作为机器学习模型预测结果衡量的标准。

![RMSE(X, h)=\sqrt{\frac{1}{m}\sum_{i=1}^{m}(h(x_{i})-y_{i})^2}](https://private.codecogs.com/gif.latex?RMSE%28X%2C%20h%29%3D%5Csqrt%7B%5Cfrac%7B1%7D%7Bm%7D%5Csum_%7Bi%3D1%7D%5E%7Bm%7D%28h%28x_%7Bi%7D%29-y_%7Bi%7D%29%5E2%7D)



**MSE（Mean Square Error）均方误差**

MSE是真实值与预测值的差值的平方然后求和平均。

通过平方的形式便于求导，所以常被用作线性回归的**损失函数**。

![MSE=\frac{1}{m}\sum_{i=1}^{m}(y_{i}-\hat{y}_{i})^2](https://private.codecogs.com/gif.latex?MSE%3D%5Cfrac%7B1%7D%7Bm%7D%5Csum_%7Bi%3D1%7D%5E%7Bm%7D%28y_%7Bi%7D-%5Chat%7By%7D_%7Bi%7D%29%5E2)



**MAE（Mean Absolute Error）平方绝对误差**

是绝对误差的平均值。

可以更好地反映预测值误差的实际情况。

![MAE(X, h)=\frac{1}{m}\sum_{i=1}^{m}\left | h(x_{i})-y_{i} \right |](https://private.codecogs.com/gif.latex?MAE%28X%2C%20h%29%3D%5Cfrac%7B1%7D%7Bm%7D%5Csum_%7Bi%3D1%7D%5E%7Bm%7D%5Cleft%20%7C%20h%28x_%7Bi%7D%29-y_%7Bi%7D%20%5Cright%20%7C)



**SD（Standard Deviation）标准差**

方差的算术平均根。

用于衡量一组数值的离散程度。

![SD=\sqrt{\frac{1}{N}\sum_{i=1}^{N}(x_{i}-avg(x))^2}](https://private.codecogs.com/gif.latex?SD%3D%5Csqrt%7B%5Cfrac%7B1%7D%7BN%7D%5Csum_%7Bi%3D1%7D%5E%7BN%7D%28x_%7Bi%7D-avg%28x%29%29%5E2%7D)



ps : 我在csdn博客学latex~



