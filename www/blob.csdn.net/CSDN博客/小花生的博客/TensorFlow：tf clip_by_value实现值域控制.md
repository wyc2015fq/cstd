
# TensorFlow：tf.clip_by_value实现值域控制 - 小花生的博客 - CSDN博客


2018年07月20日 07:57:12[Peanut_范](https://me.csdn.net/u013841196)阅读数：319


**tf.clip_by_value(A, min, max)函数**：输入一个张量A，把A中的每一个元素的值都压缩在min和max之间。小于min的让它等于min，大于max的元素值等于max。
**代码实现：**
```python
import tensorflow
```
```python
as
```
```python
tf
A = tf.
```
```python
constant
```
```python
([
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
288
```
```python
,
```
```python
20
```
```python
,
```
```python
100
```
```python
])
```
```python
value
```
```python
= tf.clip_by_value(A,
```
```python
10
```
```python
,
```
```python
255
```
```python
)
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
    print(sess.run(
```
```python
value
```
```python
))
```
**result：**[ 10  10 255  20 100]

