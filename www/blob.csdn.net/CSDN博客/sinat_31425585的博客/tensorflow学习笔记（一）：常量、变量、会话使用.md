# tensorflow学习笔记（一）：常量、变量、会话使用 - sinat_31425585的博客 - CSDN博客
2018年07月02日 18:15:50[Mirror_Yu_Chen](https://me.csdn.net/sinat_31425585)阅读数：175
1、tensowflow常量
```python
import tensorflow as tf
​
# 创建一个常量op，产生一个1x2矩阵，这个op作为一个节点
# 加到默认图中
​
# 构造器的返回值代表该常量op的返回值
# 1x2矩阵
matrix1 = tf.constant([[3., 3.]])
# 创建另外一个2x1矩阵
matrix2 = tf.constant([[2.], [2.]])
# 创建一个乘法matmul op，将matrix1和matrix2作为输入
product = tf.matmul(matrix1, matrix2)
​
# 构造完成后，启动图
# 第一步创建一个Session对象
sess = tf.Session()
​
# 调用sess的 'run()'方法来执行矩阵乘法，传入'product'
result = sess.run(product)
print(result)
​
# 任务完成，关闭会话
sess.close()
```
输出：
```python
[[12.]]
```
2、Session每次调用之后，需要显式调用close()来关闭会话，还可以使用with代码块来自动完成关闭动作
```python
import tensorflow as tf
​
matrix1 = tf.constant([[2., 2.]])
matrix2 = tf.constant([[2.], [2.]])
​
product = tf.matmul(matrix1, matrix2)
​
with tf.Session() as sess:
    result = sess.run(product)
    print(result)
```
输出：
```python
[[8.]]
```
3、设定GPU
tf.device('/gpu:x')
例如：
tf.device('/cpu:0')：CPU模式
tf.device('/gpu:0')：第一块GPU
tf.device('/gpu:1'):第二块GPU
```python
import tensorflow as tf
​
matrix1 = tf.constant([[2., 3.]])
matrix2 = tf.constant([[2.], [2.]])
​
product = tf.matmul(matrix1, matrix2)
​
with tf.Session() as sess:
    with tf.device('/gpu:0'):
        result = sess.run(product)
        print(result)
```
```python
[[10.]]
```
4、对于IPython之类的交互环境，可以使用InteractiveSession代替Session，使用Tensor.eval()和Operation.run()代替Session.run()
```python
import tensorflow as tf
​
sess = tf.InteractiveSession()
​
x = tf.Variable([1.0, 2.0])
a = tf.constant([3.0, 3.0])
​
# 使用initializer op 的run来初始化'x'
x.initializer.run()
​
# 增加一个减法sub op，从x减去a，输出结果
sub = tf.subtract(x, a)
print(sub.eval())
```
输出：
```python
[-2. -1.]
```
5、tensor可以视为n维的数组或列表，一个tensor包含一个静态类型rank和一个shape
```python
'一个计数器'
# 创建一个变量，初始化为标量0
state = tf.Variable(0, name = 'counter')
​
# 创建一个op，其作用使state+1
one = tf.constant(1)
new_value = tf.add(state, one)
update = tf.assign(state, new_value) # 赋值操作
​
# 变量必须要初始化
# init_op = tf.initialize_all_variables()
# new initialize method
init_op = tf.global_variables_initializer()
​
# 启动图，执行op
with tf.Session() as sess:
    sess.run(init_op)
    
    # 对于变量初始化，必须要run才会执行
    print(sess.run(state))
    
    for _ in range(3):
        sess.run(update)
        print(sess.run(state))
```
输出：
​
```python
0
1
2
3
```
取回多个结果：
```python
import tensorflow as tf
input1 = tf.constant(3.0)
input2 = tf.constant(2.0)
input3 = tf.constant(5.0)
intermed = tf.add(input1, input3)
mul = tf.multiply(input1, intermed)
​
with tf.Session() as sess:
    result = sess.run([mul, intermed])
    print(result)
```
输出：​
```python
[24.0, 8.0]
```
6、tensor除了以常量或变量形式存储外，还可以使用feed机制，临时替代图中任意操作中的tensor
```python
import tensorflow as tf
input1 = tf.placeholder(tf.float32)
input2 = tf.placeholder(tf.float32)
output = tf.multiply(input1, input2)
​
with tf.Session() as sess:
    print(sess.run(output, feed_dict={input1: [7.], input2: [2.]}))
```
输出：
```python
[14.]
```
7、numpy数组与tensorflow张量之间转换
```python
import tensorflow as tf
import numpy as np
sess = tf.Session()
x_data = np.array([[1., 2., 3.], [2., 3., 6.]])
x = tf.convert_to_tensor(x_data, dtype = tf.float32)
```
参考文献：
TensorFlow 官方文档中文版 - v1.2
​
