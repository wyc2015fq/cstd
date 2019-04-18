# tf.train.ExponentialMovingAverage的用法 - YZXnuaa的博客 - CSDN博客
2018年03月30日 19:00:55[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：276

tf.train.ExponentialMovingAverage(decay, steps)
tf.train.ExponentialMovingAverage这个函数用于更新参数，就是采用滑动平均的方法更新参数。这个函数初始化需要提供一个衰减速率（decay），用于控制模型的更新速度。这个函数还会维护一个影子变量（也就是更新参数后的参数值），这个影子变量的初始值就是这个变量的初始值，影子变量值的更新方式如下：
shadow_variable = decay * shadow_variable + (1-decay) * variable
shadow_variable是影子变量，variable表示待更新的变量，也就是变量被赋予的值，decay为衰减速率。decay一般设为接近于1的数（0.99,0.999）。decay越大模型越稳定，因为decay越大，参数更新的速度就越慢，趋于稳定。
tf.train.ExponentialMovingAverage这个函数还提供了自己动更新decay的计算方式：
decay= min（decay，（1+steps）/（10+steps））
steps是迭代的次数，可以自己设定。
比如：
**[python]**[view plain](https://blog.csdn.net/uestc_c2_403/article/details/72235334#)[copy](https://blog.csdn.net/uestc_c2_403/article/details/72235334#)
- import tensorflow as tf;  
- import numpy as np;  
- import matplotlib.pyplot as plt;  
- 
- v1 = tf.Variable(0, dtype=tf.float32)  
- step = tf.Variable(tf.constant(0))  
- 
- ema = tf.train.ExponentialMovingAverage(0.99, step)  
- maintain_average = ema.apply([v1])  
- 
- with tf.Session() as sess:  
-     init = tf.initialize_all_variables()  
-     sess.run(init)  
- 
- print sess.run([v1, ema.average(v1)]) #初始的值都为0
- 
-     sess.run(tf.assign(v1, 5)) #把v1变为5
-     sess.run(maintain_average)  
- print sess.run([v1, ema.average(v1)]) # decay=min(0.99, 1/10)=0.1, v1=0.1*0+0.9*5=4.5
- 
-     sess.run(tf.assign(step, 10000)) # steps=10000
-     sess.run(tf.assign(v1, 10)) # v1=10
-     sess.run(maintain_average)  
- print sess.run([v1, ema.average(v1)]) # decay=min(0.99,(1+10000)/(10+10000))=0.99, v1=0.99*4.5+0.01*10=4.555
- 
-     sess.run(maintain_average)  
- print sess.run([v1, ema.average(v1)]) #decay=min(0.99,<span style="font-family:Arial, Helvetica, sans-serif;">(1+10000)/(10+10000)</span><span style="font-family:Arial, Helvetica, sans-serif;">)=0.99, v1=0.99*4.555+0.01*10=4.6</span>
输出：
[0.0, 0.0]
[5.0, 4.5]
[10.0, 4.5549998]
[10.0, 4.6094499]
解释：每次更新完以后，影子变量的值更新，varible的值就是你设定的值。如果在下一次运行这个函数的时候你不在指定新的值，那就不变，影子变量更新。如果指定，那就variable改变，影子变量也改变
