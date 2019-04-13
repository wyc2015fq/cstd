
# tensorflow学习笔记——传入值之placeholder，feed_dict用法 - manduner的博客 - CSDN博客


2018年06月13日 14:51:03[Manduner_TJU](https://me.csdn.net/manduner)阅读数：2504


# 描述
tensorflow中需要传入值时，要同时用到placeholder、feed_dict。
- placeholder定义输入值的类型，以及数据结构
- feed_dict，表示python中的字典（python中dist是字典，元素是键值对的存储模式），用于接收真实的输入值。
# 相关公式
矩阵乘法中的两个矩阵，以及输出结果为：
×=
$$
\begin{bmatrix}
1 & 2 & 3 \\
0 & 1 & 1
\end{bmatrix} \times  
  \begin{bmatrix}
  3&2 \\
  1&2 \\
  0&1 
   \end{bmatrix} =
    \begin{bmatrix}
  5&9 \\
  1&3 \\
   \end{bmatrix}
$$

# 源代码
```python
#coding:UTF-8
```
```python
import
```
```python
tensorflow
```
```python
as
```
```python
tf
```
```python
#输入
```
```python
input1 = tf.placeholder(tf.float32,(
```
```python
2
```
```python
,
```
```python
3
```
```python
))
```
```python
#placeholder(dtype,shape),定义一个3行2列的矩阵
```
```python
input2 = tf.placeholder(tf.float32,(
```
```python
3
```
```python
,
```
```python
2
```
```python
))
```
```python
#定义一个2行3列的矩阵
```
```python
#输出
```
```python
output = tf.matmul(input1,input2)
```
```python
#matmul(),矩阵乘法
```
```python
#执行
```
```python
with
```
```python
tf.Session()
```
```python
as
```
```python
sess:
    result = sess.run(output,feed_dict = {input1:[[
```
```python
1
```
```python
,
```
```python
2
```
```python
,
```
```python
3
```
```python
],[
```
```python
0
```
```python
,
```
```python
1
```
```python
,
```
```python
1
```
```python
]],input2:[[
```
```python
3
```
```python
,
```
```python
2
```
```python
],[
```
```python
1
```
```python
,
```
```python
2
```
```python
],[
```
```python
0
```
```python
,
```
```python
1
```
```python
]]})
```
```python
print
```
```python
result
```

