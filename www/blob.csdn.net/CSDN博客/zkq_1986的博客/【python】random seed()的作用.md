# 【python】random.seed()的作用 - zkq_1986的博客 - CSDN博客





2018年12月27日 16:26:33[zkq_1986](https://me.csdn.net/zkq_1986)阅读数：486








import random

random.seed( 3 )
print("Random number with seed 3 : ", random.random())

# 生成同一个随机数
random.seed( 3 )
print("Random number with seed 3 : ", random.random())

# 生成同一个随机数
random.seed( 3 )
print("Random number with seed 3 : ", random.random())

# 生成同一个随机数
random.seed( 10000 )
print("Random number with seed 10000 : ", random.random())
输出结果：

Random number with seed 3 :  0.23796462709189137

Random number with seed 3 :  0.23796462709189137

Random number with seed 3 :  0.23796462709189137

Random number with seed 6 :  0.5779158167850091

只要random.seed( * ) seed里面的值一样，那随机出来的结果就一样。所以说，seed的作用是让随机结果可重现。



