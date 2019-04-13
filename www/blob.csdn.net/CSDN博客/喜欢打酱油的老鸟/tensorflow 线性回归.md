
# tensorflow 线性回归 - 喜欢打酱油的老鸟 - CSDN博客


2018年08月04日 07:50:48[喜欢打酱油的老鸟](https://me.csdn.net/weixin_42137700)阅读数：74标签：[线性回归 																](https://so.csdn.net/so/search/s.do?q=线性回归 &t=blog)[Tensor																](https://so.csdn.net/so/search/s.do?q=Tensor&t=blog)[
							](https://so.csdn.net/so/search/s.do?q=线性回归 &t=blog)个人分类：[人工智能																](https://blog.csdn.net/weixin_42137700/article/category/7820233)



## [https://blog.csdn.net/zhangpengzp/article/details/81384154](https://blog.csdn.net/zhangpengzp/article/details/81384154)
学习tensorflow，希望与大家共同进步，下面讲述的是如何利用tensorflow实现一个线性回归的模型的训练。
## 随机数据
目前的任务是随机100个点，（x, y）在x和y上存在着高斯误差，看下面代码：
`import tensorflow as tf
import numpy as np
import matplotlib.pyplot as plt
num_point = 100
points = []
for i in range(num_point):
    # 符合高斯分布的x的值，均值为0，标准差为0.6
    x = np.random.normal(0.0, 0.6)
    # 随机y的坐标值，线性波动也是符合高斯分布 均值0， 标准差0.01
    y = x*0.1 + 0.3 + np.random.normal(0.0, 0.01)
    points.append([x, y])

x_data = [point[0] for point in points]
y_data = [point[1] for point in points]
plt.scatter(x_data, y_data, c='r')
plt.show()`随机后点如下：
![这里写图片描述](https://img-blog.csdn.net/201808031058492?watermark/2/text/aHR0cHM6Ly9ibG9nLmNzZG4ubmV0L3poYW5ncGVuZ3pw/font/5a6L5L2T/fontsize/400/fill/I0JBQkFCMA==/dissolve/70)
## tensorflow训练
利用上面的随机方法知道，上面随机的点符合一个基本的线性关系：
`y = wx+b`因此这里让tensorflow 利用上面随机倒的样本点来，来训练出我们的w和b，根据上面随机方法：
`y = x*0.1 + 0.3 + np.random.normal(0.0, 0.01)`得知我们训练的结果w=0.1 和 b= 0.3
下面为训练的代码，其中一些关于numpy的方法不解的可以参考我的这个博客
`# 随机w 的值，shape为[1] , 随机区间为[-1,1)和 b 的初始值为0
w = tf.Variable(tf.random_uniform([1], -1.0, 1.0), name="w")
b = tf.Variable(tf.zeros([1]), name="b")
y = w*x_data + b
# 这里利用均方差作为损失值
loss = tf.reduce_mean(tf.square((y - y_data)), name="loss")
# 训练方式是梯度下降，学习率为0.5
optimizal = tf.train.GradientDescentOptimizer(0.5)
# 训练时，最小化loss
train = optimizal.minimize(loss, name="train")
# 上面定义好了操作和变量，下面则开始训练
with tf.Session() as sess:
    init = tf.initialize_all_variables()
    sess.run(init)
    for step in range(20):
        sess.run(train)
        print("w = ", sess.run(w), "b = ", sess.run(b), "loss=", sess.run(loss))`最终打印的结果如下，说明达到了预期的效果，w=0.1，b=0.3
`w =  [0.3092508] b =  [0.30994922] loss= 0.014750148
w =  [0.23917985] b =  [0.30663484] loss= 0.0065604914
w =  [0.19262347] b =  [0.30467868] loss= 0.002946823
w =  [0.16169746] b =  [0.30337894] loss= 0.0013522736
w =  [0.14115424] b =  [0.30251557] loss= 0.0006486713
w =  [0.12750798] b =  [0.30194208] loss= 0.000338203
w =  [0.11844319] b =  [0.30156112] loss= 0.00020120744
w =  [0.11242171] b =  [0.30130804] loss= 0.00014075743
w =  [0.10842182] b =  [0.30113995] loss= 0.00011408362
w =  [0.10576482] b =  [0.30102828] loss= 0.000102313614
w =  [0.10399985] b =  [0.3009541] loss= 9.712006e-05
w =  [0.10282744] b =  [0.3009048] loss= 9.482836e-05
w =  [0.10204864] b =  [0.3008721] loss= 9.381716e-05
w =  [0.1015313] b =  [0.30085036] loss= 9.337097e-05
w =  [0.10118765] b =  [0.3008359] loss= 9.3174036e-05
w =  [0.10095937] b =  [0.3008263] loss= 9.308716e-05
w =  [0.10080773] b =  [0.30081993] loss= 9.304883e-05
w =  [0.100707] b =  [0.3008157] loss= 9.30319e-05
w =  [0.10064009] b =  [0.3008129] loss= 9.302446e-05
w =  [0.10059565] b =  [0.30081102] loss= 9.302115e-05`为了方便大家学习，这里给出我的源码，欢迎大家下载：tensorflow线性回归demo

