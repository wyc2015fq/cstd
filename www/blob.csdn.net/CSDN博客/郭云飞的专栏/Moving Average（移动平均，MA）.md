
# Moving Average（移动平均，MA） - 郭云飞的专栏 - CSDN博客


2018年02月01日 13:03:52[guoyunfei20](https://me.csdn.net/guoyunfei20)阅读数：966


移动平均法是用一组最近的实际数据值来预测未来一期或几期内公司产品的需求量、公司产能等的一种常用方法。移动平均法适用于即期预测。当产品需求既不快速增长也不快速下降，且不存在季节性因素时，移动平均法能有效地消除预测中的随机波动，是非常有用的。移动平均法可以分为：
简单移动平均
加权移动平均
**简单移动平均：**
简单移动平均的各元素的权重都相等。简单的移动平均的计算公式如下：
![](https://img-blog.csdn.net/20180201125551163)

**加权移动平均法：**
加权移动平均给固定跨越期限内的每个变量值以不相等的权重。其原理是：历史各期产品需求的数据信息对预测未来期内的需求量的作用是不一样的。除了以n为周期的周期性变化外，远离目标期的变量值的影响力相对较低，故应给予较低的权重。加权移动平均法的计算公式如下：
![](https://img-blog.csdn.net/20180201125843647)

**tf.train.ExponentialMovingAverage**

```python
#coding:utf-8
import tensorflow as tf
"""
tf.train.ExponentialMovingAverage(decay, steps)
采用滑动平均的方法更新参数。这个函数初始化需要提供一个衰减速率（decay），
用于控制模型的更新速度。函数还会维护一个影子变量（也就是更新参数后的参数值），
这个影子变量的初始值就是这个变量的初始值，影子变量值的更新方式如下：
shadow_variable = decay * shadow_variable + (1-decay) * variable
decay为衰减速率。decay一般设为接近于1的数（0.99,0.999）。
tf.train.ExponentialMovingAverage还提供了自动更新decay的计算方式：
decay= min（decay，（1+steps）/（10+steps））
steps是迭代的次数，可以自己设定。
"""
v1 = tf.Variable(0, dtype=tf.float32)               # 定义一个变量，初始值为0
step = tf.Variable(0, trainable=False)              # step为迭代轮数变量，控制衰减率
ema = tf.train.ExponentialMovingAverage(0.99, step) # 初始设定衰减率为0.99
maintain_averages_op = ema.apply([v1])              # 更新列表中的变量
with tf.Session() as sess:
    init_op = tf.initialize_all_variables()
#   init_op = tf.global_variables_initializer()     # 初始化所有变量
    sess.run(init_op)
    print(sess.run([v1, ema.average(v1)]))          # decay=min(0.99, 1/10)=0.1, v1=0.1*0+0.9*0=0.0
    sess.run(tf.assign(v1, 5))                      # 更新v1的值
    sess.run(maintain_averages_op)                  # decay=min(0.99, 1/10)=0.1, v1=0.1*0+0.9*5=4.5
    print(sess.run([v1, ema.average(v1)]))
    sess.run(tf.assign(step, 10000))                # 更新迭代轮转数step
    sess.run(tf.assign(v1, 10))                     # 更新v1的值
    sess.run(maintain_averages_op)
    print(sess.run([v1, ema.average(v1)]))          # decay=min(0.99,(1+10000)/(10+10000))=0.99, v1=0.99*4.5+0.01*10=4.555
    
    sess.run(tf.assign(step, 1234))                 # 更新迭代轮转数step
    sess.run(tf.assign(v1, 15))                     # 更新v1的值
    sess.run(maintain_averages_op)
    print(sess.run([v1, ema.average(v1)]))          # decay=min(0.99,(1+1234)/(10+1234))=0.99, v1=0.99*4.555+0.01*15=4.65945
```
运行结果：
![](https://img-blog.csdn.net/20180201130303027)
可以看到运行结果和我们注释的计算是一致的！







