# TensorFlow tf.clip_by_value tf.select 绝对值用法！ - YZXnuaa的博客 - CSDN博客
2018年03月29日 19:52:44[YZXnuaa](https://me.csdn.net/YZXnuaa)阅读数：308
## 1. 基本
- 
tf.clip_by_value() 截断，常和对数函数结合使用
```
# 计算交叉熵
crose_ent = -tf.reduce_mean(tf.log(y_*tf.clip_by_value(y, 1e-10, 1.)))
```
- 1
- 2
- 3
- 4
```
a = tf.reshape(tf.range(6, dtype=tf.float32), [2, 3])
tf.clip_by_value(a, 2.5, 4.5)           # 将值限定在 2.5 和 4.5 之间
array([[ 2.5,  2.5,  2.5],
       [ 3. ,  4. ,  4.5]], dtype=float32)
```
- 1
- 2
- 3
- 4
## 2. 条件分支：tf 下的三目运算符
f(x,y)={a(x−y),x>ya(y−x),x≤y
`tf.select(tf.greater(v1, v2), a*(v1-v2), a*(v2-v1))`- 1
当然上式也可化为：f(x,y)=a|x−y|；
