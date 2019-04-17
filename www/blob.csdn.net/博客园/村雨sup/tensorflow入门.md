# tensorflow入门 - 村雨sup - 博客园








[博客园](https://www.cnblogs.com/)[首页](https://www.cnblogs.com/cunyusup/)[新随笔](https://i.cnblogs.com/EditPosts.aspx?opt=1)[联系](https://msg.cnblogs.com/send/%E6%9D%91%E9%9B%A8sup)[管理](https://i.cnblogs.com/)[订阅](https://www.cnblogs.com/cunyusup/rss)![订阅](https://www.cnblogs.com/images/xml.gif)





```
import tensorflow as tf
#创建常量op
m1 = tf.constant([[3,3]])
m2 = tf.constant([[2],[3]])
#创建一个矩阵乘法op,m1,m2传入
product = tf.matmul(m1,m2)

#定义一个会话，启动默认图
sess = tf.Session()
#调用sess的run方法来执行乘法op
#run(product)触发了图中3个op
result = sess.run(product)

print(result)

sess.close()
```

——

```
import tensorflow as tf

x = tf.Variable([1,2])
a = tf.constant([3,3])

#增加一个减法op
sub = tf.subtract(x,a)
#增加一个加法op
add = tf.add(x,sub)
#定义全局变量
init = tf.global_variables_initializer()

with tf.Session() as sess:#用with打开无需关掉
    sess.run(init)
    print(sess.run(sub))
    print(sess.run(add))
```

——

```
import tensorflow as tf

#创建一个变量初始化为0
state = tf.Variable(0,name='counter')
#创建一个op，作用是state加1
new_val = tf.add(state,1)
#赋值op,把new_val赋值给state
update = tf.assign(state,new_val)
#变量初始化
init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)
    print(sess.run(state))

    for _ in range(5):
        sess.run(update)#每次调用更新变量的操作
        print(sess.run(state))
```

输出结果：0,1,2,3,4,5

——

```
import tensorflow as tf

input1 = tf.constant(3.0)
input2 = tf.constant(2.0)
input3 = tf.constant(5.0)

add = tf.add(input2,input3)
mul = tf.multiply(input1,add)

with tf.Session() as sess:
    result = sess.run([mul,add])   #fetch可以同时运行多个op
    print(result)
```

输出结果：[21.0, 7.0]

——

```
import tensorflow as tf

input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)
output = tf.multiply(input1,input2)

with tf.Session() as sess:  #feed 占位符，以字典形式传入
    print(sess.run(output,feed_dict={input1:[8.],input2:[2.]}))
```

——

```
import tensorflow as tf
import numpy as np

#使用numpy生成100个随机点
x_data = np.random.rand(100)
y_data = x_data*0.1 + 0.2

#构造一个线性模型
b = tf.Variable(0.)
k = tf.Variable(0.)
y = k*x_data + b


#二次代价函数      平均值          误差
loss = tf.reduce_mean(tf.square(y_data - y))
#定义一个梯度下降法来进行训练的优化器
optimizer = tf.train.GradientDescentOptimizer(0.2)
#最小化代价函数
train = optimizer.minimize(loss)

#初始化变量
init = tf.global_variables_initializer()

with tf.Session() as sess:
    sess.run(init)
    for step in range(201): #经过200次训练
        sess.run(train)
        if step%20 == 0:
            print(step,sess.run([k,b]))
```

利用tensorflow多次训练接近初始值

输出结果：

0 [0.051719286, 0.099555954]
20 [0.10192502, 0.19899791]
40 [0.10117736, 0.19938719]
60 [0.10072004, 0.19962522]
80 [0.10044037, 0.19977079]
100 [0.10026933, 0.19985981]
120 [0.10016473, 0.19991426]
140 [0.10010075, 0.19994757]
160 [0.10006161, 0.19996794]
180 [0.10003767, 0.19998039]
200 [0.10002304, 0.19998801]

可以看到最后结果无限接近于0.1 ， 0.2

——













